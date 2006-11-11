/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Admin Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

// Advanced Swear Filter and Punishment plugin uses this function
public ADMIN_ServerHandler()
{
	// Make sure WC3 is loaded
	if ( !WC3_Check() )
	{
		return PLUGIN_HANDLED;
	}

	new szCmd[32], szArg1[32], szArg2[32];
	read_argv( 0, szCmd, 31 );
	read_argv( 1, szArg1, 31 );
	read_argv( 2, szArg2, 31 );
	
	new id	= str_to_num( szArg1 );
	new iXP = str_to_num( szArg2 );

	if ( equal( szCmd, "amx_takexp" ) )
	{
		iXP *= -1;

		iXP += p_data[id][P_XP];
	}

	else if ( equal( szCmd, "changexp" ) )
	{
		iXP += p_data[id][P_XP];
	}

	ADMIN_SetXP( id, iXP );

	return PLUGIN_HANDLED;
}

// This will handle every admin client command
public ADMIN_Handler( id )
{
	new szCmd[32];
	read_argv( 0, szCmd, 31 );

	// Make sure WC3 is loaded
	if ( !WC3_Check() && !equal( szCmd, "wc3_disable" ) && !equal( szCmd, "wc3_enable" ) )
	{
		ADMIN_Print( id, "%s WC3 has been disabled by an admin of this server", g_MODclient );

		return PLUGIN_HANDLED;
	}
	
	// Make sure the user is an admin
    if ( !( get_user_flags( id ) & XP_GetAdminFlag() ) )
	{
		client_print( id, print_console, "%L", id, "YOU_HAVE_NO_ACCESS", g_MODclient );

		return PLUGIN_HANDLED;
	}

	// OK we're free to go!!!


	new szArg1[32], szArg2[32];
	read_argv( 1, szArg1, 31 );
	read_argv( 2, szArg2, 31 );

	// Give the user XP
	if ( equal( szCmd, "wc3_givexp" ) || equal( szCmd, "amx_givexp" ) )
	{
		if ( read_argc() < 3 )
		{
			ADMIN_Print( id, "Format: %s <name|#id|auth|@TEAM|@ALL> <xp>", szCmd );

			return PLUGIN_HANDLED;
		}

		ADMIN_GiveXP( id, szArg1, str_to_num( szArg2 ) );
	}

	// Set the level of a user's race
	else if ( equal( szCmd, "wc3_setlevel" ) )
	{
		if ( read_argc() < 3 )
		{
			ADMIN_Print( id, "Format: %s <name|#id|auth|@TEAM|@ALL> <level>", szCmd );

			return PLUGIN_HANDLED;
		}
		
		new iLevel = str_to_num( szArg2 );

		if ( iLevel < 0 || iLevel >= 11 )
		{
			ADMIN_Print( id, "%s Error, level must be in between (or equal to) 0 and 10", g_MODclient );

			return PLUGIN_HANDLED;
		}

		ADMIN_SetLevel( id, szArg1, iLevel );
	}

	// Give the user an item
	else if ( equal( szCmd, "wc3_giveitem" ) )
	{
		if ( read_argc() < 3 )
		{
			ADMIN_Print( id, "Format: %s <name|#id|auth|@TEAM|@ALL> <item id or name>", szCmd );

			return PLUGIN_HANDLED;
		}

		new iItemID = FindItem( szArg2 );

		ADMIN_GiveItem( id, szArg1, iItemID );
	}

	// Enable the plugin
	else if ( equal( szCmd, "wc3_enable" ) )
	{

		// Plugin was already enabled, why re-enable?
		if ( warcraft3 )
		{
			ADMIN_Print( id, "%s The plugin was already enabled!", g_MODclient );
		}

		// Enable the plugin!
		else
		{
			warcraft3 = true;
			ADMIN_Print( id, "%s Plugin enabled!", g_MODclient );
		}

	}

	// Disable the plugin
	else if ( equal( szCmd, "wc3_disable" ) )
	{
		// Plugin was already enabled, why re-enable?
		if ( !warcraft3 )
		{
			ADMIN_Print( id, "%s The plugin was already disabled!", g_MODclient );
		}

		// Disable the plugin!
		else
		{
			// What needs to happen here?
			//    - all skills set to 0 for each player
			//    - call SHARED_SetGravity
			//    - call SHARED_SetInvis
			//    - call SHARED_SetSpeed
			//    - stop tasks - like money task for dod
			//	  - ULT_ClearIcons( id )


			warcraft3 = false;
			ADMIN_Print( id, "%s Plugin disabled!", g_MODclient );
		}
	}

	return PLUGIN_HANDLED;
}

// Actually set the user's XP
ADMIN_SetXP( id, iXP )
{
	if ( iXP < 0 )
	{
		iXP = 0;
	}

	p_data[id][P_XP] = iXP;

	XP_Check( id );

	set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + id );
}

// Set the user's level
ADMIN_SetLevel( id, szTarget[], iLevel )
{

	new iXP = xplevel[iLevel];

	new iTarget = 0, bool:bTargetFound = false;

	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		ADMIN_SetXP( iTarget, iXP );

		client_print( iTarget, print_chat, "%s The admin has set your race's level to %d", g_MODclient, iLevel );

		bTargetFound = true;
	}
	
	// No target found :/
	if ( !bTargetFound )
	{
		ADMIN_NoTargetFound( id, szTarget, ( iTarget == -2 ) );
	}
}

// Give the user some XP
ADMIN_GiveXP( id, szTarget[], iXP )
{
	new iTarget = 0, bool:bTargetFound = false;
	
	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		ADMIN_SetXP( iTarget, p_data[id][P_XP] + iXP );

		client_print( iTarget, print_chat, "%s %L", g_MODclient, iTarget, "THE_ADMIN_GAVE_YOU_EXPERIENCE", iXP );

		bTargetFound = true;
	}

	// No target found :/
	if ( !bTargetFound )
	{
		ADMIN_NoTargetFound( id, szTarget, ( iTarget == -2 ) );
	}
}

// Give the user an item
ADMIN_GiveItem( id, szTarget[], iItemID )
{
	new szItemName[32];
	LANG_GetItemName ( iItemID, id, szItemName, 31 );

	new iTarget = 0, bool:bTargetFound = false;

	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		client_print( iTarget, print_chat, "%s The admin has given you the item '%s'", g_MODclient, szItemName );
		
		ITEM_GiveItem( iTarget, iItemID );

		bTargetFound = true;
	}

	// No target found :/
	if ( !bTargetFound )
	{
		ADMIN_NoTargetFound( id, szTarget, ( iTarget == -2 ) );
	}
}

// Attempt to find out what item this is
FindItem( szItem[] )
{
	return str_to_num( szItem );
}

// Find a user based on szTarget
FindTarget( iLastID, szTarget[] )
{
	
	new iTarget = -1;

	// Then we want to basically return everyone!
	if ( equali( szTarget, "@ALL" ) )
	{
		new players[32], iTotalPlayers, i;
		get_players( players, iTotalPlayers );
		
		// Loop through and search for the next target
		for ( i = 0; i < iTotalPlayers; i++ )
		{
			// Target found, so lets return the next one (if possible)!!
			if ( players[i] == iLastID && i + 1 != iTotalPlayers )
			{
				iTarget = players[i+1];
			}
		}

		// No target was found so return the first one
		if ( iTotalPlayers > 0 && iLastID == 0 )
		{
			iTarget = players[0];
		}
	}

	// Find a target based on the team
	else if ( szTarget[0] == '@' )
	{
		new iTeam = -1;
		
		// Counter-Strike and Condition Zero Checks
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			if ( equali( szTarget, "@T" ) )
			{
				iTeam = TEAM_T;
			}
			else if ( equali( szTarget, "@CT" ) )
			{
				iTeam = TEAM_CT;
			}
		}
		
		// Day of Defeat check
		else if ( g_MOD == GAME_DOD )
		{
			if ( equali( szTarget, "@ALLIES" ) )
			{
				iTeam = ALLIES;
			}
			else if ( equali( szTarget, "@AXIS" ) )
			{
				iTeam = AXIS;
			}
		}

		// Make sure a team was found
		if ( iTeam != -1 )
		{
			new players[32], iTotalPlayers, i, iFirstPlayer = -1, bool:bSaveNext = false;
			get_players( players, iTotalPlayers );

			// Loop through and search for the next target
			for ( i = 0; i < iTotalPlayers; i++ )
			{			
				// Make sure they're on the same team
				if ( iTeam == get_user_team( players[i] ) )
				{
					
					// This is the next available player
					if ( bSaveNext )
					{
						iTarget = players[i];
						break;
					}

					// If this is the previous target, we need to get the next one!
					if ( players[i] == iLastID )
					{
						bSaveNext = true;
					}
					
					// Save the FIRST player on this team
					if ( iFirstPlayer == -1 )
					{
						iFirstPlayer = players[i];
					}
				}
			}

			// No target was found so return the first one that matches the team (the target could still be -1 if iFirstPlayer wasn't found)
			if ( iLastID == 0 )
			{
				iTarget = iFirstPlayer;
			}
		}
	}

	// Otherwise search for a player
	else
	{
		// Initial search is by player name
		new iPlayer = find_player( "a", szTarget );
		
		// If not found, search by partial match
		if ( !iPlayer )
		{
			iPlayer = find_player( "bl", szTarget );

			if ( iPlayer )
			{

				// Then Multiple clients found
				if ( iPlayer != find_player( "blj", szTarget ) )
				{
					return -2;
				}
			}
		}

		// If not found, search by auth id
		if ( !iPlayer )
		{
			iPlayer = find_player( "c" , szTarget );
		}

		// If not found, search by user id
		if ( !iPlayer )
		{
			// Make sure we have a user id
			if ( szTarget[0] == '#' && szTarget[1] )
			{
				iPlayer = find_player( "k", str_to_num( szTarget[1] ) )
			}
		}
		
		// Yay we have a match!!!
		if ( iPlayer && iLastID != iPlayer )
		{
			iTarget = iPlayer;
		}
	}

	return iTarget;
}

ADMIN_NoTargetFound( id, szTarget[], bool:bMulti )
{
	// Multiple clients found
	if ( bMulti )
	{
		ADMIN_Print( id, "%s There is more than one client matching '%s'", g_MODclient, szTarget );
	}

	else
	{
		ADMIN_Print( id, "%s Unable to find target(s) '%s'", g_MODclient, szTarget );
	}

	// List what the available targets are
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		ADMIN_Print( id, "%s Available targets are: @ALL, @CT, @T or the player's name/auth/#id", g_MODclient );
	}

	else if ( g_MOD == GAME_DOD )
	{
		ADMIN_Print( id, "%s Available targets are: @ALL, @ALLIES, @AXIS or the player's name/auth/#id", g_MODclient );
	}
}

// Function will print to server console or client console based on the ID number
ADMIN_Print( id, text[], {Float,_}:...)
{    
	// format the text as needed

	new szFormattedText[128];
	format_args( szFormattedText, 127, 1 );

	if ( id == 0 )
	{
		server_print( szFormattedText );
	}
	else
	{
		client_print( id, print_console, szFormattedText );
	}

	// Gets rid of compiler warning
	if ( text[0] == 0 )
	{
		return;
	}
}  
