
// Advanced Swear Filter and Punishment plugin uses this function
public Admin_TakeXP()
{
	if ( !warcraft3 )
		return PLUGIN_HANDLED;

	new szIndex[3], szXp[32];
	read_argv( 1, szIndex, 2 );
	read_argv( 2, szXp, 31 );

	new id = str_to_num( szIndex );

	p_data[id][P_XP] -= str_to_num( szXp );

	XP_Check( id );

	return PLUGIN_HANDLED;
}

public changeXP()
{
	if ( !warcraft3 )
		return PLUGIN_HANDLED;

	new szId[4], szXp[8];
	
	read_argv( 1, szId, 3 );
	read_argv( 2, szXp, 7 );
		
	new id = str_to_num( szId );
	new iXp = str_to_num( szXp );

	if(  (p_data[id][P_XP] + iXp ) < 0 )
	{
		p_data[id][P_XP] = 0;
	}
	else
	{
		p_data[id][P_XP] += iXp;
	}

	XP_Check( id );
	
	return PLUGIN_HANDLED;
}

public Admin_GiveXP( id )
{
	if (!warcraft3 )
		return PLUGIN_HANDLED;

    if ( !( get_user_flags( id ) & XP_get_admin_flag() ) )
	{
			client_print( id, print_console, "%L", id, "YOU_HAVE_NO_ACCESS", g_MODclient );
			return PLUGIN_HANDLED;
	}

	new szTarget[32], szXp[10];

	read_argv( 1, szTarget, 31 );
	read_argv( 2, szXp, 9 );

	_Admin_GiveXP( id, szTarget, str_to_num( szXp ) );

	return PLUGIN_HANDLED;
}

public _Admin_GiveXP( id, szTarget[], iXp )
{
	if( equali( szTarget, "@ALL" ) )
	{
		new iPlayers[32], iTotalPlayers, i;
		get_players( iPlayers, iTotalPlayers );

		for( i = 1; i <= iTotalPlayers; i++ )
		{
			client_print( iPlayers[i], print_chat, "%s %L", g_MODclient, id, "THE_ADMIN_GAVE_YOU_EXPERIENCE", iXp );

			p_data[iPlayers[i]][P_XP] += iXp;

			XP_Check( iPlayers[i] );

			set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + iPlayers[i] );
		}
	}
	else if ( szTarget[0] == '@' )
	{ 
		new iPlayers[32], iTotalPlayers, i;
		get_players( iPlayers, iTotalPlayers, "e", szTarget[1] );

		if ( iTotalPlayers < 1 )
		{ 
			console_print( id, "%L", id, "NO_CLIENTS_IN_TEAM" ); 
			return PLUGIN_HANDLED;
		} 

		for( i = 1; i <= iTotalPlayers; i++ )
		{
			client_print( iPlayers[i], print_chat, "%s %L", g_MODclient, id, "THE_ADMIN_GAVE_YOU_EXPERIENCE", iXp );

			p_data[iPlayers[i]][P_XP] += iXp;

			XP_Check( iPlayers[i] );

			set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + iPlayers[i] );
		}
	}
	else
	{ 
		new iPlayer = find_target( id, szTarget );

		if ( !iPlayer )
			return PLUGIN_HANDLED;

		client_print( iPlayer, print_chat, "%s %L", g_MODclient, id, "THE_ADMIN_GAVE_YOU_EXPERIENCE", iXp );

		p_data[iPlayer][P_XP] += iXp;

		XP_Check( iPlayer );

		set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + iPlayer );
	}

	return PLUGIN_HANDLED;
}

public Admin_SetLevel( id )
{
	if ( !( get_user_flags( id ) & XP_get_admin_flag() ) )
	{
			client_print( id, print_console, "%L", id, "YOU_HAVE_NO_ACCESS", g_MODclient );
			return PLUGIN_HANDLED;
	}

	new szTarget[32], szLevel[3];

	read_argv( 1, szTarget, 31 );
	read_argv( 2, szLevel, 2 );

	new iLevel = str_to_num( szLevel );
	new iXp = xplevel[iLevel];

	if( equali( szTarget, "@ALL" ) )
	{
		new iPlayers[32], iTotalPlayers, i;
		get_players( iPlayers, iTotalPlayers );

		for( i = 1; i <= iTotalPlayers; i++ )
		{
			//client_print( iPlayers[i], print_chat, "%s %L", g_MODclient, id, "THE_ADMIN_GAVE_YOU_EXPERIENCE", iXp );

			p_data[iPlayers[i]][P_XP] = iXp;

			XP_Check( iPlayers[i] );

			set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + iPlayers[i] );
		}
	}
	else if ( szTarget[0] == '@' )
	{ 
		new iPlayers[32], iTotalPlayers, i;
		get_players( iPlayers, iTotalPlayers, "e", szTarget[1] );

		if ( iTotalPlayers < 1 )
		{ 
			console_print( id, "%L", id, "NO_CLIENTS_IN_TEAM" ); 
			return PLUGIN_HANDLED;
		} 

		for( i = 1; i <= iTotalPlayers; i++ )
		{
			//client_print( iPlayers[i], print_chat, "%s %L", g_MODclient, id, "THE_ADMIN_GAVE_YOU_EXPERIENCE", iXp );

			p_data[iPlayers[i]][P_XP] = iXp;

			XP_Check( iPlayers[i] );

			set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + iPlayers[i] );
		}
	}
	else
	{ 
		new iPlayer = find_target( id, szTarget );

		if ( !iPlayer )
			return PLUGIN_HANDLED;

		//client_print( iPlayer, print_chat, "%s %L", g_MODclient, id, "THE_ADMIN_GAVE_YOU_EXPERIENCE", iXp );

		p_data[iPlayer][P_XP] = iXp;

		XP_Check( iPlayer );

		set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + iPlayer );
	}

	return PLUGIN_HANDLED;
}

public Admin_wc3( id )
{

    if ( !( get_user_flags( id ) & XP_get_admin_flag() ) )
	{
			client_print( id, print_console, "%L", id, "YOU_HAVE_NO_ACCESS", g_MODclient );
			return PLUGIN_HANDLED;
	}
	
	new szArgCmd[16];
	read_argv( 0, szArgCmd, 15 );

	if ( read_argc() < 2 )
	{
		Admin_Print( id, "Format: %s <on|off>", szArgCmd );
		return PLUGIN_HANDLED;
	}

	new szArg[32];
	read_argv( 1, szArg, 31 );

	if ( equal( szArg, "on" ) || equal( szArg, "1" ) )
	{
		warcraft3 = true;
		Admin_Print( id, "War3ft plugin enabled" );
	}
	else if ( equal( szArg, "off" ) || equal( szArg, "0" ) )
	{
		warcraft3 = false;
		Admin_Print( id, "War3ft plugin disabled" );

		// Save everyone's XP since we're not going to be saving it later
		if ( get_pcvar_num( CVAR_wc3_save_xp ) )
		{
			XP_SaveAll();
		}
	}
	else
	{
		Admin_Print( id, "Format: %s <on|off>", szArgCmd );
	}

	return PLUGIN_HANDLED;
}

// Function will print to server console or client console based on the ID number
Admin_Print( id, text[], {Float,_}:...)
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

public ADMIN_GiveItem( id )
{

    if ( !( get_user_flags( id ) & XP_get_admin_flag() ) )
	{
			client_print( id, print_console, "%L", id, "YOU_HAVE_NO_ACCESS", g_MODclient );
			return PLUGIN_HANDLED;
	}
	
	if ( read_argc() < 3 )
	{
		new szArgCmd[16];
		read_argv( 0, szArgCmd, 15 );

		Admin_Print( id, "Format: %s <player|team|@ALL> <item id>", szArgCmd );
		return PLUGIN_HANDLED;
	}

	new szTarget[32], szItemID[6];
	read_argv( 1, szTarget, 31 );
	read_argv( 2, szItemID, 5 );

	
	new iTarget = 0, bool:bTargetFound = false;

	// Do this while we continue having a target!
	while ( ( iTarget = FindTarget( iTarget, szTarget ) ) > 0 )
	{
		server_print( "Target Found: %d team: %d", iTarget, get_user_team( iTarget ) );

		bTargetFound = true;
	}

	if ( !bTargetFound )
	{
		Admin_Print( id, "%s Unable to find target(s) '%s'", g_MODclient, szTarget );

		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			Admin_Print( id, "%s Available targets are: @ALL, @CT, @T or the player's name", g_MODclient );
		}

		else if ( g_MOD == GAME_DOD )
		{
			Admin_Print( id, "%s Available targets are: @ALL, @ALLIES, @AXIS or the player's name", g_MODclient );
		}
	}

	
	return PLUGIN_HANDLED;
}

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

		if ( iTeam == -1 )
		{
			return -1;
		}


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

	// Otherwise search for a player
	/*
	else
	{ 
		new iPlayer = find_target( id, szTarget );

		if ( !iPlayer )
			return PLUGIN_HANDLED;

		//client_print( iPlayer, print_chat, "%s %L", g_MODclient, id, "THE_ADMIN_GAVE_YOU_EXPERIENCE", iXp );

		p_data[iPlayer][P_XP] = iXp;

		XP_Check( iPlayer );

		set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + iPlayer );
	}
	*/


	return iTarget;
}