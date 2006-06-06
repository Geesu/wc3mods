/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Shadow Hunter Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define SH_HEX_LENGTH			5.0			// Length that hex lasts in seconds
#define SH_HEX_SPEED			175.0		// Speed hexed player will move at
#define SH_HEALING_WAVE_RANGE	750			// Range to heal teammates

#define VENGEANCE_HEALTH		50			// Health the user should have after using his ult

#define CONCOCTION_DAMAGE		15			// Damage done by Unstable Concoction
#define CONCOCTION_RADIUS		300

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
	else if ( is_user_alive( id ) && Verify_Skill( id, RACE_SHADOW, SKILL3 ) && p_data[id][P_SERPENTCOUNT] > 0 && !endround )
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

			WC3_Status_Text( id, 3.5, HUDMESSAGE_POS_INFO, "%L", id, "SERPENT_WARD", p_data[id][P_SERPENTCOUNT] );
		}
		else
		{
			// Geesu needs to add language support for this message
			WC3_Status_Text( id, 3.5, HUDMESSAGE_POS_INFO, "You cannot place a serpent ward here." );
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
    		return false;
	}
	
	iEnt = -1;
	
	while( ( iEnt = find_ent_by_class( iEnt, "hostage_entity" ) ) > 0 )
	{
    	entity_get_vector( iEnt, EV_VEC_origin, vEntOrigin )
    
    	vEntityOrigin[0] = floatround( vEntOrigin[0] );
    	vEntityOrigin[1] = floatround( vEntOrigin[1] );
    	vEntityOrigin[2] = floatround( vEntOrigin[2] );
    	
    	if ( get_distance( vPlayerOrigin, vEntityOrigin ) < 200 )
    		return false;
	}	 	
	return true;
}

// Unstable Concoction
public SH_Concoction( iVictim, iAttacker )
{
	
	// We can't do anything if the user is hexed :/
	if ( p_data_b[iVictim][PB_HEXED] )
	{
		return;
	}

	// Check to see if we should "concoction"
	if ( random_float( 0.0, 1.0 ) <= p_concoction[p_data[iVictim][P_LEVEL]] )
	{
		new vOrigin[3], vInitOrigin[3], vAxisOrigin[3], i;
		
		// Get origin of attacker
		get_user_origin( iAttacker, vOrigin );
		
		// Play sound on attacker
		emit_sound( iAttacker, CHAN_STATIC, SOUND_CONCOCTION_CAST, 1.0, ATTN_NORM, 0, PITCH_NORM );
		
		// Set up the origins for the effect
		vInitOrigin[0] = vOrigin[0];
		vInitOrigin[1] = vOrigin[1];
		vInitOrigin[2] = vOrigin[2] - 16;

		vAxisOrigin[0] = vOrigin[0];
		vAxisOrigin[1] = vOrigin[1];
		vAxisOrigin[2] = vOrigin[2] + CONCOCTION_RADIUS;
		
		// Display the effect on the attacker
		for ( i = 0; i < 200; i += 25 )
		{
			Create_TE_BEAMCYLINDER( vOrigin, vInitOrigin, vAxisOrigin, g_sSpriteTexture, 0, 0, 9, 20, 0, 188, 220, 255, 255, 0 );

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
				if ( get_distance( vOrigin, vTargetOrigin ) <= CONCOCTION_RADIUS )
				{
					// Damage
					WAR3_damage( players[i], iVictim, CONCOCTION_DAMAGE, CSW_CONCOCTION, 0 );
				
					// Let the victim know he hit someone
					emit_sound( iVictim, CHAN_STATIC, SOUND_CONCOCTION_HIT, 1.0, ATTN_NORM, 0, PITCH_NORM );
				}
			}
		}
	}
	#if ADVANCED_STATS
	else
	{
		new WEAPON = CSW_CONCOCTION - CSW_WAR3_MIN;
		iStatsShots[iVictim][WEAPON]++;
	}
	#endif

	return;
}