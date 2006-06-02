/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Warden Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define VENGEANCE_HEALTH		50			// Health the user should have after using his ult

public WA_ULT_Vengeance( id )
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
	return PLUGIN_HANDLED;
}