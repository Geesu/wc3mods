/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Shadow Hunter Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define SH_HEX_LENGTH			5.0			// Length that hex lasts in seconds
#define SH_HEX_SPEED			175.0		// Speed hexed player will move at
#define SH_HEALING_WAVE_RANGE	750			// Range to heal teammates

#define VENGEANCE_HEALTH		50			// Health the user should have after using his ult

// ****************************************
// Shadow Hunter's Healing Wave
// ****************************************

public _SH_HealingWave( id )
{

	id -= TASK_WAVE;

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	// Continue healing...
	if ( Verify_Skill( id, RACE_SHADOW, SKILL1 ) && is_user_alive( id ) )
	{
		set_task( p_heal[p_data[id][P_SKILL1]-1], "_SH_HealingWave", TASK_WAVE + id );
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
				if ( get_user_actualhealth( targetid ) + 1 <= get_user_maxhealth( targetid ) )
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
		return;
	}
	
	p_data_b[id][PB_CAN_RENDER] = true;
	p_data_b[id][PB_HEXED]		= false;
	
	// Reset the user's speed
	SHARED_ResetMaxSpeed( id );

	// Reset the user's rendering
	set_user_rendering( id );

	emit_sound( id, CHAN_STATIC, SOUND_HEX, 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}


// ****************************************
// Shadow Hunter's Serpent Ward
// ****************************************

public SH_PlaceSerpentWard( id )
{

	// User is hexed, can't use any skills
	if ( p_data_b[id][PB_HEXED] )
	{
		WC3_Status_Text( id, 4.0, HUDMESSAGE_POS_INFO, "%L", id, "HEX_NO_ABILITY" );
	}

	// User is alive we can place a ward!
	else if ( is_user_alive( id ) )
	{

		// Serpent Ward
		if ( Verify_Skill(id, RACE_SHADOW, SKILL3) && p_data[id][P_SERPENTCOUNT] > 0 && !endround )
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

			WC3_Status_Text( id, 3.5, HUDMESSAGE_POS_INFO, "%L", id, "SERPENT_WARD", p_data[id][P_SERPENTCOUNT] );
		}
	}
}

public _SH_DrawSerpentWard( parm[5] )
{

	if ( !WAR3_Check() || endround )
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
	Create_TE_BEAMPOINTS( start, end, g_sLightning, 1, 5, 2, 500, 20, red, green, blue, 100, 100 );

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
				WAR3_damage( targetid, id, 10, CSW_SERPENTWARD, -1 );

				// Make the sound when they're attacked
				client_cmd( targetid, "speak ambience/thunder_clap.wav" );

				// Create screen fade on the user's screen
				Create_ScreenFade( targetid, (1<<10), (1<<10), (1<<12), red, green, blue, 255 );
			}
		}
	}

	set_task( 0.5, "_SH_DrawSerpentWard", TASK_LIGHT + id, parm, 5 );

	return;
}

public SH_ULT_Vengeance( id )
{
	
	// If the user's ultimate is used or we're still in the ultimate delay, then why are we here?
	if ( p_data_b[id][PB_ULTIMATEUSED] || g_ultimateDelay )
	{
		return;
	}

	// We need to respawn the user!!
	if ( !is_user_alive( id ) )
	{

		// Make sure the user isn't changing teams and we're not at the end of the round
		if ( !p_data_b[id][PB_CHANGINGTEAM] && !endround )
		{
			
			// Ultimate is now used
			p_data_b[id][PB_ULTIMATEUSED]	= true;

			// Remove the ultimate delay if it existed (in theory it shouldn't)
			if ( task_exists( TASK_UDELAY + id ) )
			{
				remove_task( TASK_UDELAY + id );
			}

			// Set up a new ultimate delay
			p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown );
			_ULT_Delay( id );

			// We need to make sure spawn knows that it's b/c of Vengeance
			p_data_b[id][PB_VENGEANCE_SPAWN] = true;

			// Set up the respawn task
			set_task( 1.2, "_SHARED_Spawn", TASK_SPAWN + id );
		}
	}
	
	// Otherwise teleport back to spawn
	else
	{		
		// Lets find a free spawn and obey immunity rules
		new ent = SHARED_FindFreeSpawn( id, true );
		
		// No spawn found
		if ( ent < 0 )
		{
			set_hudmessage( 255, 255, 10, -1.0, -0.4, 1, 0.5, 1.5, 0.2, 0.2 ,5 );
			
			// Immune person nearby
			if ( ent == -2 )
			{
				show_hudmessage( id, "%L", id, "VENGEANCE_FAILED_ENEMY_IMMUNITY" );
			}

			// No spawn found
			else
			{
				show_hudmessage( id, "%L", id, "NO_FREE_SPAWN_FOUND" );
			}
		}

		// Spawn found!
		else
		{
			new vOrigin[3], vNewOrigin[3], Float:fSpawnOrigin[3];
			
			// Get the user's origin
			get_user_origin( id, vOrigin );

			// Get the origin of the spawn
			entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );
			
			// Convert float vector to int vector
			FVecIVec( fSpawnOrigin, vNewOrigin );
			
			// Move the user
			set_user_origin( id, vNewOrigin );

			// Play teleport animation at old location
			Create_TE_TELEPORT( vOrigin );

			// Change user's health
			set_user_health( id, VENGEANCE_HEALTH );

			// Play the Vengeance sound		
			emit_sound( id, CHAN_STATIC, SOUND_VENGEANCE, 1.0, ATTN_NORM, 0, PITCH_NORM );
			
			// User's ultimate is used, so set up delay + hide icon
			p_data_b[id][PB_ULTIMATEUSED]	= true;
			p_data[id][P_ULTIMATEDELAY]		= get_pcvar_num( CVAR_wc3_ult_cooldown );

			Ultimate_Icon( id, ICON_HIDE );
			_ULT_Delay( id );
		}
	}
}