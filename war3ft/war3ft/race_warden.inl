/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Warden Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define VENGEANCE_HEALTH		50			// Health the user should have after using his ult

WA_ULT_Vengeance( id )
{
	
	// If the user's ultimate isnot available, then why are we here?
	if ( !ULT_Available( id ) )
	{
		return;
	}

	// We need to respawn the user!!
	if ( !is_user_alive( id ) )
	{

		// Make sure the user isn't changing teams and we're not at the end of the round
		if ( !p_data_b[id][PB_CHANGINGTEAM] && !endround && !p_data[id][P_RESPAWNBY] )
		{

			// Remove the ultimate delay if it existed (in theory it shouldn't)
			if ( task_exists( TASK_UDELAY + id ) )
			{
				remove_task( TASK_UDELAY + id );
			}

			// Set up a new ultimate delay
			ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) );

			// Set up the respawn task
			set_task( 1.2, "_SHARED_Spawn", TASK_SPAWN + id );

			// We need to set that we're about to respawn
			p_data[id][P_RESPAWNBY] = RESPAWN_VENGEANCE;
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
						
			// Hide the user's ultimate icon b/c we just used it!
			ULT_Icon( id, ICON_HIDE );

			// Set up a new ultimate delay
			ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) );
		}
	}

	return;
}

// User's Blink Ability
WA_Blink( id )
{
	
	// User has the ability, lets initiate a "check"
	if ( Verify_Skill(id, RACE_WARDEN, SKILL2) )
	{

		if ( random_float( 0.0, 1.0 ) <= p_blink[p_data[id][P_SKILL2]-1] )
		{
			p_data_b[id][PB_WARDENBLINK] = true;
			client_print( id, print_chat, "%s You will now be immune to all ultimates.", g_MODclient );
		}
		else
		{
			p_data_b[id][PB_WARDENBLINK] = false;
		}
	}

	// User doesn't have the ability so why give them blink?  duh!
	else
	{
		p_data_b[id][PB_WARDENBLINK] = false;
	}
}
