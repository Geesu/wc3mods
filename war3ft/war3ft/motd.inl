

public MOTD_ItemsInfo( id, iStart )
{

	static szTmp[256], szTmp2[256], pos, i;
	pos = 0;

	// Add header
	pos += formatex( szTmpMsg[pos], 2047-pos, "%s", MOTD_header );

	// Add the item information
	for ( i = iStart; i < MAX_ITEMS_PAGE + iStart; i++ )
	{
		LANG_GetItemInfo( i, id, szTmp, 127 );
		LANG_GetItemName( i, id, szTmp2, 127 );

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%s</div><br>", szTmp, szTmp2 );
	}

	if ( iStart == 0 )
	{
		formatex( szTmp, 127, "%L", id, "MOTD_TITLE_SHOPMENU" );
	}
	else
	{
		formatex( szTmp, 127, "%L", id, "MOTD_TITLE_SHOPMENU2" );
	}

	show_motd( id, szTmpMsg, szTmp );
}

public MOTD_PlayerSkills( id )
{

	new iPlayers[32], iNumPlayers, i;
	get_players( iPlayers, iNumPlayers );

	// Lets find out what races are in use
	new bool:bRaceUsed[MAX_RACES+1] = {false}, iRaceID, iTargetID;
	for ( i = 0; i < iNumPlayers; i++ )
	{
		iTargetID	= iPlayers[i];
		iRaceID		= p_data[iTargetID][P_RACE];
		
		bRaceUsed[iRaceID] = true;
	}
	
	new pos = 0, iTeam, k, szTeamColor[2];

	// Then we have a player, and we're not on the server console
	if ( id != 0 )
	{
		pos += formatex( szTmpMsg[pos], 2047 - pos, "%s", MOTD_header );
	}

	new szTmpName[64];
	// Loop through each race
	for ( iRaceID = 1; iRaceID < get_pcvar_num( CVAR_wc3_races ) + 1; iRaceID++ )
	{

		// Make sure this race was used somewhere!
		if ( bRaceUsed[iRaceID] )
		{

			// Get the race's name
			lang_GetRaceName ( iRaceID, id, szTmpName, 63 );

			// Player
			if ( id != 0 )
			{
				pos += formatex( szTmpMsg[pos], 2047 - pos, "<b>%s</b><ul>", szTmpName );
			}

			// Server Console
			else
			{
				server_print( "**** %s ****", szTmpName );
			}


			// Lets show them per team, T, then CT, then Spectators
			for ( iTeam = TEAM_T; iTeam <= TEAM_CT + 1; iTeam++ )
			{
				
				// Default color
				formatex( szTeamColor, 1, "p" );

				if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
				{
					if ( iTeam == TEAM_T )
					{
						formatex( szTeamColor, 1, "t" );
					}
					else if ( iTeam == TEAM_CT )
					{
						formatex( szTeamColor, 1, "c" );
					}
				}

				else if ( g_MOD == GAME_DOD )
				{
					if ( iTeam == ALLIES )
					{
						formatex( szTeamColor, 1, "a" );
					}
					else if ( iTeam == AXIS )
					{
						formatex( szTeamColor, 1, "x" );
					}
				}

				// Loop through all players
				for ( k = 0; k < iNumPlayers; k++ )
				{
					iTargetID = iPlayers[k];

					
					// Make sure the user has this race and is on the proper team
					if ( p_data[iTargetID][P_RACE] == iRaceID && get_user_team( iTargetID ) == iTeam )
					{

						get_user_name( iTargetID, szTmpName, 63 );
						
						// Player
						if ( id != 0 )
						{
							pos += formatex( szTmpMsg[pos], 2047 - pos, "<li id='%s'>(%d) %s</li>", szTeamColor, p_data[iTargetID][P_LEVEL], szTmpName );
						}

						// Server Console
						else
						{
							console_print( id, "   (%d) %s", p_data[iTargetID][P_LEVEL], szTmpName );
						}
					}
				}//end player loop
			}//end team loop

			// Player
			if ( id != 0 )
			{
				pos += formatex( szTmpMsg[pos], 2047 - pos, "</ul>" );
			}
		}
	}//end race loop

	// Player
	if ( id != 0 )
	{
		new szTitle[128];
		formatex( szTitle, 127, "%L", id, "PLAYER_SKILLS" );

		console_print( id, "%s", szTmpMsg );
		log_amx( "%s", szTmpMsg );

		show_motd( id, szTmpMsg, szTitle );
	}

	
	return;
}

public MOTD_SkillsInfo( id )
{

	static iSkillID, bool:bHeaderShown, pos;
	new szTitle[128], szTmpDesc[256], szSkillName[128], szRaceName[64];

	pos = 0;
	pos += formatex( szTmpMsg[pos], 2047-pos, "%s", MOTD_header );

	// Valid race found!
	if ( SM_IsValidRace( p_data[id][P_RACE] ) )
	{
		// Get the Race Name
		lang_GetRaceName( p_data[id][P_RACE], id, szRaceName, 63 );

		// format the title
		pos += formatex( szTmpMsg[pos], 2047-pos, "<div id=^"title^">%s</div><br><br>", szRaceName );
		
		// Format the MOTD title
		formatex( szTitle, 127, "%s %L", szRaceName, id, "WORD_INFORMATION" );
	}

	// No race selected
	else
	{
		pos += formatex( szTmpMsg[pos], 2047-pos, "<div id=^"title^">No Race Selected</div><br><br>" );

		// Format the MOTD title
		formatex( szTitle, 127, "Skills Information" );
	}


	// Lets get the trainable skills first!
	bHeaderShown = false;
	iSkillID = SM_GetSkillOfType( id, SKILL_TYPE_TRAINABLE );

	while ( iSkillID != -1 )
	{
		if ( !bHeaderShown )
		{
			pos += formatex( szTmpMsg[pos], 2047-pos, "<h1>%L</h1>", id, "WORD_TRAINABLE_SKILLS" );

			bHeaderShown = true;
		}

		LANG_GetSkillName( iSkillID, id, szSkillName, 127 )
		LANG_GetSkillInfo( iSkillID, id, szTmpDesc, 255 );

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul><br>", szSkillName, szTmpDesc );

		iSkillID = SM_GetSkillOfType( id, SKILL_TYPE_TRAINABLE, iSkillID + 1 );
	}

	// Now add the user's ultimate(s)
	bHeaderShown = false;
	iSkillID = SM_GetSkillOfType( id, SKILL_TYPE_ULTIMATE );
	while ( iSkillID != -1 )
	{
		if ( !bHeaderShown )
		{
			pos += formatex( szTmpMsg[pos], 2047-pos, "<h1>%L</h1>", id, "WORD_ULTIMATE" );

			bHeaderShown = true;
		}

		LANG_GetSkillName( iSkillID, id, szSkillName, 127 )
		LANG_GetSkillInfo( iSkillID, id, szTmpDesc, 255 );

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul>", szSkillName, szTmpDesc );

		iSkillID = SM_GetSkillOfType( id, SKILL_TYPE_ULTIMATE, iSkillID + 1 );
	}

	// Now add the user's passive ability(s)
	bHeaderShown = false;
	iSkillID = SM_GetSkillOfType( id, SKILL_TYPE_PASSIVE );
	while ( iSkillID != -1 )
	{
		if ( !bHeaderShown )
		{
			pos += formatex( szTmpMsg[pos], 2047-pos, "<h1>%L</h1>", id, "WORD_HERO_ABILITY" );

			bHeaderShown = true;
		}

		LANG_GetSkillName( iSkillID, id, szSkillName, 127 )
		LANG_GetSkillInfo( iSkillID, id, szTmpDesc, 255 );

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul><br>", szSkillName, szTmpDesc );

		iSkillID = SM_GetSkillOfType( id, SKILL_TYPE_PASSIVE, iSkillID + 1 );
	}

	// Add the footer
	pos += formatex( szTmpMsg[pos], 2047-pos, "<br><div>%L</div>", id, "MOTD_MORE_INFO", g_MOD );

	show_motd( id, szTmpMsg, szTitle );

	return;
}

public MOTD_War3help(id)
{

	if ( !WC3_Check( id ) )
	{
		return;
	}


	// Get the game title
	new szGame[32];
	get_modname( szGame, 31 );

	new pos = 0;

	// Add header
	pos += formatex( szTmpMsg[pos], 2047-pos, "%s", MOTD_header );

	// Add the content
	pos += formatex( szTmpMsg[pos], 2047-pos, "%L<br>"	, id, "MOTD_DEVELOPED_BY" );
	pos += formatex( szTmpMsg[pos], 2047-pos, "%L<br>"	, id, "MOTD_CLICK_FOR_LATEST" );
	pos += formatex( szTmpMsg[pos], 2047-pos, "%L<p>"	, id, "MOTD_MORE_INFO", szGame );
	pos += formatex( szTmpMsg[pos], 2047-pos, "%L"		, id, "WAR3HELP" );
	
	// Set up the title
	new szTitle[128];
	formatex( szTitle, 127, "%L", id, "WAR3HELP_TITLE" );

	show_motd( id, szTmpMsg, szTitle );

	return;
}
