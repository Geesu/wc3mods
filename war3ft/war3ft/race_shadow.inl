/*
*	Race: Shadow Hunter Functions
*/

#define SH_HEX_LENGTH			  5.0		// Length that hex lasts in seconds
#define SH_HEX_SPEED			  175.0		// Speed hexed player will move at
#define SH_HEALING_WAVE_RANGE	  750		// Range to heal teammates

#define SH_BIGBADVOODOO_DURATION  2

#define SH_CONCOCTION_DAMAGE	  15		// Damage done by Unstable Concoction
#define SH_CONCOCTION_RADIUS	  300

new g_SH_SerpentGiven[33];
new bool:bWardBombCheck = false;

// ****************************************
// Shadow Hunter's Healing Wave
// ****************************************


// Helper function to start the healing process
public SH_HealingWave( id )
{
	if ( !task_exists( id + TASK_WAVE ) )
	{
		_SH_HealingWave( id );
	}
}

// We don't need to ever actually "turn this off" since this task will take care of itself
public _SH_HealingWave( id )
{
	
	if ( id >= TASK_WAVE )
	{
		id -= TASK_WAVE;
	}

	// User is no longer connected :/
	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}
	
	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( id, SKILL_HEALINGWAVE );

	// User doesn't have this skill, so lets return
	//  - Also return if this was called twice
	if ( iSkillLevel <= 0 )
	{
		return;
	}

	// Continue healing...
	if ( is_user_alive( id ) )
	{
		set_task( p_heal[iSkillLevel-1], "_SH_HealingWave", TASK_WAVE + id );
	}
	
	// Prevent healing if this player is hexed
	if ( p_data_b[id][PB_HEXED] )
	{
		return;
	}


	new players[32], numberofplayers, targetid, targetorigin[3], origin[3];
	new iTeam = get_user_team( id );

	get_user_origin( id, origin );
	get_players( players, numberofplayers, "a" );
	
	// Loop through each player and check to see if one is close enough to be healed
	for ( new i = 0; i < numberofplayers; i++ )
	{
		targetid = players[i];
		
		// User is on the caster's team
		if ( p_data_b[targetid][PB_ISCONNECTED] && get_user_team( targetid ) == iTeam )
		{
			get_user_origin( targetid, targetorigin );
			
			// User is close enough
			if ( get_distance(origin, targetorigin) < SH_HEALING_WAVE_RANGE )
			{
				get_user_origin( targetid, origin )
				
				// User needs health
				if ( get_user_health( targetid ) + 1 <= get_user_maxhealth( targetid ) )
				{
					set_user_health( targetid, get_user_health( targetid ) + 1 );

					Create_TE_IMPLOSION( origin, 100, 8, 1 );
				}
			}
		}
	}

	return;
}


// ****************************************
// Shadow Hunter's Hex
// ****************************************

public _SH_RemoveHex( id )
{
	id -= TASK_HEX;

	if( !p_data_b[id][PB_ISCONNECTED] )
	{
		return PLUGIN_HANDLED;
	}
	
	p_data_b[id][PB_CAN_RENDER] = true;
	p_data_b[id][PB_HEXED]		= false;
	
	if ( is_user_alive( id ) )
	{
		// Reset the user's speed
		SHARED_ResetMaxSpeed( id );

		// Reset the user's gravity
		SHARED_SetGravity( id );

		// Reset the user's rendering
		SHARED_INVIS_Set( id );

		// Reset the user's devotion aura
		HU_DevotionAura( id );

		emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_HEX], 1.0, ATTN_NORM, 0, PITCH_NORM );
	}

	return PLUGIN_HANDLED;
}


// ****************************************
// Shadow Hunter's Serpent Ward
// ****************************************

SH_SerpentWard( id )
{
	static iSkillLevel;

	iSkillLevel = SM_GetSkillLevel( id, SKILL_SERPENTWARD );

	// User should have some!
	if ( iSkillLevel > 0 )
	{

		// Then we can give the user a serpent ward!
		while ( g_SH_SerpentGiven[id] < p_serpent[iSkillLevel-1] )
		{
			// Increase the available serpent wards
			p_data[id][P_SERPENTCOUNT]++;

			// Increase the total we have given the user!
			g_SH_SerpentGiven[id]++;

			//client_print( id, print_chat, "[DEBUG] Total wards increased to %d (total given so far: %d)", p_data[id][P_SERPENTCOUNT], g_SH_SerpentGiven[id] );
		}
	}
	
	// User should not have any!
	else
	{
		// Remove all available serpents!
		p_data[id][P_SERPENTCOUNT] = 0;

		// We don't need to remove any of the wards b/c the task knows to check the skill level
	}
}

public SH_PlaceSerpentWard( id )
{

	// User is hexed, can't use any skills
	if ( p_data_b[id][PB_HEXED] )
	{
		WC3_StatusText( id, 0, "%L", id, "HEX_NO_ABILITY" );
	}

	// User is alive we can place a ward!
	else if ( is_user_alive( id ) && SM_GetSkillLevel( id, SKILL_SERPENTWARD ) > 0 && p_data[id][P_SERPENTCOUNT] > 0 )
	{

		// Serpent Ward
		if ( SH_CanPlaceWard( id ) )
		{
			new parm[5], origin[3]

			get_user_origin( id, origin );
			parm[0] = origin[0];
			parm[1] = origin[1];
			parm[2] = origin[2];
			parm[3] = id;
			parm[4] = get_user_team( id );

			_SH_DrawSerpentWard( parm );
			p_data[id][P_SERPENTCOUNT]--;

			WC3_StatusText( id, 0, "%L", id, "SERPENT_WARD", p_data[id][P_SERPENTCOUNT] );
		}
		else
		{
			WC3_StatusText( id, 0, "You cannot place a serpent ward here" );
		}
	}
}

// Called when the bomb is planted
SH_BombPlanted()
{
	bWardBombCheck = true;

	set_task( 1.0, "_SH_ResetBombCheck", 0 );
}


public _SH_ResetBombCheck()
{
	bWardBombCheck = false;
}

public _SH_DrawSerpentWard( parm[5] )
{

	if ( !WC3_Check() )
	{
		return;
	}

	new id = parm[3];

	// User is no longer connected, stop drawing wards
	if( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	// User is no longer alive, don't draw wards
	if ( !is_user_alive( id ) )
	{
		return;
	}

	// User doesn't have this skill anymore! Remove the ward!
	if ( SM_GetSkillLevel( id, SKILL_SERPENTWARD ) <= 0 )
	{
		return;
	}

	// Don't allow the ward to be by a hostage or the bom
	if ( bWardBombCheck )
	{
		if ( !SH_CanPlaceWard( id ) )
		{
			client_print( id, print_chat, "%s Serpent ward removed, you can't have them near bombs or hostages!", g_MODclient );

			return;
		}
	}

	new origin[3], start[3], end[3], red, blue, green
	origin[0]=parm[0]
	origin[1]=parm[1]
	origin[2]=parm[2]
	
	// Set up the top of the ward
	start[0] = origin[0]
	start[1] = origin[1]
	start[2] = origin[2] + 600

	// Set up the bottom of the ward
	end[0] = origin[0]
	end[1] = origin[1]
	end[2] = origin[2] - 600

	// Set up colors based on what game/team
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( parm[4] == TS )
		{
			red		= 255;
			blue	= 0;
			green	= 0;
		}
		else
		{
			red		= 0;
			blue	= 255;
			green	= 0;
		}
	}
	
	else if ( g_MOD == GAME_DOD )
	{
		if ( parm[4] == AXIS )
		{
			red		= 255;
			blue	= 63;
			green	= 63;
		}
		else
		{
			red		= 76;
			blue	= 102;
			green	= 76;
		}
	}

	// Draw the serpent ward
	Create_TE_BEAMPOINTS( start, end, g_iSprites[SPR_LIGHTNING], 1, 5, 2, 500, 20, red, green, blue, 100, 100 );

	// Now lets check and see if any players are near that need to be damaged
	new players[32], numberofplayers;
	new targetid, targetorigin[3];

	get_players( players, numberofplayers, "a" );
	
	for ( new i = 0; i < numberofplayers; i++ )
	{
		targetid = players[i];

		if ( parm[4] != get_user_team( targetid ) )
		{
			get_user_origin( targetid, targetorigin );

			if ( get_distance( origin, targetorigin ) < 85 )
			{

				// Damage the user
				WC3_Damage( targetid, id, 10, CSW_SERPENTWARD, -1 );

				// Make the sound when they're attacked
				client_cmd( targetid, "speak ambience/thunder_clap.wav" );

				// Create screen fade on the owners screen
				Create_ScreenFade( id, (1<<10), (1<<10), (1<<12), red, green, blue, 55 );
			}
		}
	}

	set_task( 0.5, "_SH_DrawSerpentWard", TASK_LIGHT + id, parm, 5 );

	return;
}

bool:SH_CanPlaceWard( id )
{
	new vPlayerOrigin[3];
 	get_user_origin( id, vPlayerOrigin );

	// Make sure the user isn't trying to place a ward near a hostage
	if ( SHARED_NearObjective( vPlayerOrigin ) == OBJENT_HOSTAGE )
	{
		return false;
	}

	// Make sure the user isn't trying to place a ward near the bomb
 	new Float:vEntOrigin[3];
 	new vEntityOrigin[3];
 	
 	new iEnt = find_ent_by_model( -1, "grenade", "models/w_c4.mdl" )
 	
	if ( iEnt && is_valid_ent( iEnt ) ) 
	{
		
    	entity_get_vector( iEnt, EV_VEC_origin, vEntOrigin )
    
    	vEntityOrigin[0] = floatround( vEntOrigin[0] );
    	vEntityOrigin[1] = floatround( vEntOrigin[1] );
    	vEntityOrigin[2] = floatround( vEntOrigin[2] );
    	
    	if ( get_distance( vPlayerOrigin, vEntityOrigin ) < 250 )
		{
    		return false;
		}
	}
	
	return true;
}

// ****************************************
// Shadow Hunter's Big Bad Voodoo
// ****************************************

public SH_Ult_BigBadVoodoo( id )
{
	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	p_data_b[id][PB_GODMODE] = true;

	p_data_b[id][PB_CAN_RENDER] = false;

	ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) + SH_BIGBADVOODOO_DURATION, false );

	ULT_Icon( id, ICON_FLASH );
	
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		Create_BarTime( id, SH_BIGBADVOODOO_DURATION, 0 );
	}
		
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_VOODOO], 1.0, ATTN_NORM, 0, PITCH_NORM );

	set_user_rendering( id, kRenderFxGlowShell, 255, 245, 50, kRenderNormal, 16 );

	new vOrigin[3];
	get_user_origin( id, vOrigin );
	vOrigin[2] += 75;

	Create_TE_ELIGHT( id, vOrigin, 100, 255, 245, 200, SH_BIGBADVOODOO_DURATION, 0 );

	set_task( float( SH_BIGBADVOODOO_DURATION ), "SH_Ult_Remove", TASK_RESETGOD + id );

	return;
}

public SH_Ult_Remove( id )
{
	if ( id >= TASK_RESETGOD )
	{
		id -= TASK_RESETGOD;
	}

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	p_data_b[id][PB_CAN_RENDER] = true;

	p_data_b[id][PB_GODMODE] = false;

	ULT_Icon( id, ICON_HIDE );

	SHARED_INVIS_Set( id );

	return;
}

SH_SkillsOffensive( iAttacker, iVictim )
{

	static iSkillLevel;

	// Hex
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_HEX );

	if ( iSkillLevel > 0 )
	{
		if ( random_float( 0.0, 1.0 ) <= p_hex[iSkillLevel-1] )
		{
						
			// We need to identify the victim as slowed + hexed			
			p_data_b[iVictim][PB_HEXED] = true
			p_data_b[iVictim][PB_SLOWED] = true;
				
			// Slow the user's speed
			SHARED_SetSpeed( iVictim );

			// Set the user's gravity
			SHARED_SetGravity( iVictim );

			// Set the user's rendering
			SHARED_INVIS_Set( iVictim );

			// Remove user's devotion aura
			new iHealth = get_user_health( iVictim );
			if ( iHealth - g_HU_DevotionAuraGiven[iVictim] < 0 )
			{
				WC3_Kill( iVictim, iAttacker, CSW_WORLDSPAWN, 1 );
			}
			else
			{
				set_user_health( iVictim, iHealth - g_HU_DevotionAuraGiven[iVictim] );
			}
			g_HU_DevotionAuraGiven[iVictim] = 0;

			// Create the "remove hex" task
			set_task( SH_HEX_LENGTH ,"_SH_RemoveHex", TASK_HEX + iVictim );

			// Play the hex sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_HEX], 1.0, ATTN_NORM, 0, PITCH_NORM );

			// Make the user glow!
			SHARED_Glow( iVictim, 150, 150, 150, 0 );

			// Lets create a screen fade
			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 255, 255, 75 )
		}
	}
}

SH_SkillsDefensive( iAttacker, iVictim )
{
	static iSkillLevel;

	// Unstable Concoction
	iSkillLevel = SM_GetSkillLevel( iVictim, PASS_UNSTABLECONCOCTION );
	if ( iSkillLevel > 0 )
	{
		// Check to see if we should "concoction"
		if ( random_float( 0.0, 1.0 ) <= p_concoction[iSkillLevel] )
		{
			new vOrigin[3], vInitOrigin[3], vAxisOrigin[3], i;
			
			// Get origin of attacker
			get_user_origin( iAttacker, vOrigin );
			
			// Play sound on attacker
			emit_sound( iAttacker, CHAN_STATIC, g_szSounds[SOUND_CONCOCTION_CAST], 1.0, ATTN_NORM, 0, PITCH_NORM );
			
			// Set up the origins for the effect
			vInitOrigin[0] = vOrigin[0];
			vInitOrigin[1] = vOrigin[1];
			vInitOrigin[2] = vOrigin[2] - 16;

			vAxisOrigin[0] = vOrigin[0];
			vAxisOrigin[1] = vOrigin[1];
			vAxisOrigin[2] = vOrigin[2] + SH_CONCOCTION_RADIUS;
			
			// Display the effect on the attacker
			for ( i = 0; i < 200; i += 25 )
			{
				Create_TE_BEAMCYLINDER( vOrigin, vInitOrigin, vAxisOrigin, g_iSprites[SPR_SHOCKWAVE], 0, 0, 9, 20, 0, 188, 220, 255, 255, 0 );

				vInitOrigin[2] += 25;
			}

			new team = get_user_team( iVictim );
			new players[32], numberofplayers, vTargetOrigin[3];
			get_players(players, numberofplayers, "a");

			
			// Loop through all players and see if anyone nearby needs to be damaged
			for( i = 0; i < numberofplayers; i++ )
			{
				
				// Then we have a target on the other team!!
				if ( get_user_team( players[i] ) != team )
				{
					get_user_origin( players[i], vTargetOrigin );

					// Make sure they are close enough
					if ( get_distance( vOrigin, vTargetOrigin ) <= SH_CONCOCTION_RADIUS )
					{
						// Damage
						WC3_Damage( players[i], iVictim, SH_CONCOCTION_DAMAGE, CSW_CONCOCTION, 0 );
					
						// Let the victim know he hit someone
						emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_CONCOCTION_HIT], 1.0, ATTN_NORM, 0, PITCH_NORM );
					}
				}
			}
		}

		else if ( get_pcvar_num( CVAR_wc3_psychostats ) )
		{
			new WEAPON = CSW_CONCOCTION - CSW_WAR3_MIN;

			iStatsShots[iVictim][WEAPON]++;
		}
	}
}

// This function is called to reset the ability of an attacker to attack a user running big bad voodoo
public _SH_ResetBigBadAttacker( id )
{

	if ( !WC3_Check() )
	{
		return;
	}

	if ( id >= TASK_BIGBADATTACKER )
	{
		id -= TASK_BIGBADATTACKER;
	}

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	p_data_b[id][PB_BIGBAD_ATTACKER] = false;

	client_print( id, print_chat, "%s You can no longer damage players with Big Bad Voodoo activated", g_MODclient );
}