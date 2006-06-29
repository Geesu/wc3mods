

public MOTD_ItemsInfo( id, iStart )
{

	if ( !WAR3_Check( id ) )
	{
		return;
	}

	new szTitle[128], pos = 0, i;
	new szItemName[9][64], iItemID;

	// Build the list of items
	for ( i = 0; i < 9; i++ )
	{
		iItemID = iStart + i;

		LANG_GetItemName ( iItemID, id, szItemName[i], 63 );
	}

	// Add header
	pos += formatex( szTmpMsg[pos], 2047-pos, "%s", MOTD_header );

	// Add the item information
	
	// "Shopmenu 1"
	if ( iStart == 0 )
	{
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[0], id, "SHOPMENU_1" );
			pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[1], id, "SHOPMENU_2" );
		}
		else if ( g_MOD == GAME_DOD )
		{
			pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[0], id, "DOD_SHOPMENU_1" );
			pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[1], id, "DOD_SHOPMENU_2" );
		}

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[2], id, "SHOPMENU_3", get_pcvar_num( CVAR_wc3_claw ), get_pcvar_num( CVAR_wc3_claw ) );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[3], id, "SHOPMENU_4" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[4], id, "SHOPMENU_5" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[5], id, "SHOPMENU_6" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[6], id, "SHOPMENU_7" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[7], id, "SHOPMENU_8" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[8], id, "SHOPMENU_9" );
	
		formatex( szTitle, 127, "%L", id, "SHOPMENU_TITLE" );
	}

	// "Shopmenu 2"
	else if ( iStart == 9 )
	{

		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[0], id, "SHOPMENU2_1" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[1], id, "SHOPMENU2_2" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[2], id, "SHOPMENU2_3" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[3], id, "SHOPMENU2_4" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[4], id, "SHOPMENU2_5" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[5], id, "SHOPMENU2_6", get_pcvar_num( CVAR_wc3_glove_timer ) );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[6], id, "SHOPMENU2_7" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[7], id, "SHOPMENU2_8" );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[8], id, "SHOPMENU2_9" );

		formatex( szTitle, 127, "%L", id, "SHOPMENU2_TITLE" );

	}


	show_motd( id, szTmpMsg, szTitle );

}

public MOTD_PlayerSkills( id )
{

	if ( !WAR3_Check( id ) )
	{
		return;
	}


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
	
	
	new pos = 0, iTeam, k, iTeamColor;

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
				iTeamColor = 'p';

				if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
				{
					if ( iTeam == TEAM_T )
					{
						iTeamColor = 't';
					}
					else if ( iTeam == TEAM_CT )
					{
						iTeamColor = 'c';
					}
				}

				else if ( g_MOD == GAME_DOD )
				{
					if ( iTeam == ALLIES )
					{
						iTeamColor = 'a';
					}
					else if ( iTeam == AXIS )
					{
						iTeamColor = 'x';
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
							pos += formatex( szTmpMsg[pos], 2047 - pos, "<li id='%c'>(%d) %s</li>", iTeamColor, p_data[iTargetID][P_LEVEL], szTmpName );
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

		show_motd( id, szTmpMsg, szTitle );
	}

	
	return;
}

public MOTD_SkillsInfo( id )
{

	if ( !WAR3_Check( id ) )
	{
		return;
	}

	if ( 0 < p_data[id][P_RACE] <= get_pcvar_num( CVAR_wc3_races ) )
	{

		new szTmp[128], szTmpDesc[256], szRaceName[64];
		new pos = 0, i, iSkillID;

		// Get the Race Name
		lang_GetRaceName( p_data[id][P_RACE], id, szRaceName, 63 );

		// format the title
		pos += formatex( szTmpMsg[pos], 2047-pos, "%s", MOTD_header );
		pos += formatex( szTmpMsg[pos], 2047-pos, "<div id=^"title^">%s</div><br><br>", szRaceName );

		// Lets add each skill to the msg!
		for ( i = 0; i < 5; i++ )
		{
			if ( p_data[id][P_RACE] == RACE_CHAMELEON )
			{
				iSkillID = g_ChamSkills[i];
			}
			else
			{
				iSkillID = SM_GetSkill( p_data[id][P_RACE], i );
			}
			
			LANG_GetSkillName( iSkillID, id, szTmp, 127 )
			LANG_GetSkillInfo( iSkillID, id, szTmpDesc, 255 );

			// Add each skill
			if ( i < 3)
			{
				// Add the trainable skills header
				if ( i == 0 )
				{
					pos += formatex( szTmpMsg[pos], 2047-pos, "<h1>%L</h1>", id, "WORD_TRAINABLE_SKILLS" );
				}

				pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul><br>", szTmp, szTmpDesc );
			}

			// Add the ultimate
			else if ( i == 3 )
			{
				pos += formatex( szTmpMsg[pos], 2047-pos, "<h1>%L</h1>", id, "WORD_ULTIMATE" );
				pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul>", szTmp, szTmpDesc );

			}

			// Add the hero's passive ability
			else if ( i == 4 && p_data[id][P_RACE] > 4 )
			{
				pos += formatex( szTmpMsg[pos], 2047-pos, "<br><h1>%L</h1>", id, "WORD_HERO_ABILITY" );
				pos += formatex( szTmpMsg[pos], 2047-pos, "<li>%s</li><ul>%s</ul><br>", szTmp, szTmpDesc );
			}
		}

		pos += formatex( szTmpMsg[pos], 2047-pos, "<br><div>%L</div>", id, "MOTD_MORE_INFO", g_MOD );

		formatex( szTmp, 127, "%s %L", szRaceName, id, "WORD_INFORMATION" );
		show_motd( id, szTmpMsg, szTmp );
	}
	else
	{
		show_motd( id, "Select a race before trying to view skillsinfo", "No race selected" );

		return;
	}

	return;
}

public MOTD_War3help(id)
{

	if ( !WAR3_Check( id ) )
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