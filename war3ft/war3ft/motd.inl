

public MOTD_ItemsInfo( id, iStart )
{

	if ( !WAR3_Check( id ) )
	{
		return;
	}

	new szTitle[128], szMsg[2048], pos = 0, i;
	new szItemName[9][64], iItemID;

	// Build the list of items
	for ( i = 0; i < 9; i++ )
	{
		iItemID = iStart + i;

		LANG_GetItemName ( iItemID, id, szItemName[i], 63 );
	}

	// Add header
	pos += formatex( szMsg[pos], 2048-pos, "%s", MOTD_header );

	// Add the item information
	
	// "Shopmenu 1"
	if ( iStart == 0 )
	{
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[0], id, "SHOPMENU_1" );
			pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[1], id, "SHOPMENU_2" );
		}
		else if ( g_MOD == GAME_DOD )
		{
			pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[0], id, "DOD_SHOPMENU_1" );
			pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[1], id, "DOD_SHOPMENU_2" );
		}

		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[2], id, "SHOPMENU_3", get_pcvar_num( CVAR_wc3_claw ), get_pcvar_num( CVAR_wc3_claw ) );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[3], id, "SHOPMENU_4" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[4], id, "SHOPMENU_5" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[5], id, "SHOPMENU_6" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[6], id, "SHOPMENU_7" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[7], id, "SHOPMENU_8" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[8], id, "SHOPMENU_9" );
	
		formatex( szTitle, 127, "%L", id, "SHOPMENU_TITLE" );
	}

	// "Shopmenu 2"
	else if ( iStart == 9 )
	{

		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[0], id, "SHOPMENU2_1" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[1], id, "SHOPMENU2_2" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[2], id, "SHOPMENU2_3" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[3], id, "SHOPMENU2_4" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[4], id, "SHOPMENU2_5" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[5], id, "SHOPMENU2_6", get_pcvar_num( CVAR_wc3_glove_timer ) );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[6], id, "SHOPMENU2_7" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[7], id, "SHOPMENU2_8" );
		pos += formatex( szMsg[pos], 2047-pos, "<li>%s</li><div id='s'>%L</div><br>", szItemName[8], id, "SHOPMENU2_9" );

		formatex( szTitle, 127, "%L", id, "SHOPMENU2_TITLE" );

	}


	show_motd( id, szMsg, szTitle );

}

public MOTD_War3help(id){

	if ( !WAR3_Check( id ) )
		return PLUGIN_CONTINUE

	new title[128], szGame[8]
	new pos = 0

	if (is_running("cstrike"))
		szGame = "cstrike"
	else if (is_running("czero"))
		szGame = "czero"
	else if (is_running("dod"))
		szGame = "dod"

	formatex(title,127,"%L",id,"WAR3HELP_TITLE")

	// Add header

	pos += formatex(szTmpMsg[pos], 2048-pos, "%s", MOTD_header)

	pos += formatex(szTmpMsg[pos], 2048-pos, "%L<br>",id,"THIS_MOD_WAS_DEVELOPED")
	pos += formatex(szTmpMsg[pos], 2048-pos, "%L<br>",id,"CLICK_HERE_FOR_LATEST")	
	pos += formatex(szTmpMsg[pos], 2048-pos, "%L<p>",id,"CLICK_HERE", szGame)
	pos += formatex(szTmpMsg[pos], 2048-pos, "%L",id,"WAR3HELP")

	show_motd(id,szTmpMsg,title)

	return PLUGIN_HANDLED
}

public MOTD_Playerskills(id, saychat){

	if ( !WAR3_Check( id ) )
		return PLUGIN_CONTINUE
		
	new name[32]
	
	new race_name[MAX_RACES+1][64]
	new players[32], numberofplayers, i, k, playerid, pos
	new iTotalRace[MAX_RACES+1]
	get_players(players, numberofplayers)

	for(k=0;k<numberofplayers;k++){
		iTotalRace[p_data[players[k]][P_RACE]]++
	}

	for(k=1;k<get_pcvar_num( CVAR_wc3_races )+1;k++){
		if( iTotalRace[k] > 0 )
			lang_GetRaceName(k,id,race_name[k],64)
	}

	if(saychat)
		pos += formatex(szTmpMsg[pos], 2048-pos, "%s", MOTD_header)

	for(k=1;k<(get_pcvar_num( CVAR_wc3_races )+1);k++){

		if(iTotalRace[k] > 0){
			
			if ( saychat ) {
				pos += formatex(szTmpMsg[pos],2047-pos, "<b>%s</b><ul>",race_name[k])
			}
			else{
				console_print(id, "**** %s ****", race_name[k])
			}
			
			for (new iTeam = TS; iTeam <= CTS+1; iTeam++){
				for (i = 0; i < numberofplayers; ++i){
					playerid=players[i]
					new pTeam = get_user_team(playerid)

					if(p_data[playerid][P_RACE] == k && (pTeam == iTeam || (iTeam == CTS+1 && pTeam != CTS && pTeam != TS))){
						get_user_name(playerid,name,31)
						
						if ( saychat ) {
							//replace(name, 127, "<", "|")
							//replace(name, 127, ">", "|")
						#if MOD == 0
							if(get_user_team(playerid) == CTS)
								pos += formatex(szTmpMsg[pos],2047-pos,"<li id='c'>(%d) %s</li>",p_data[playerid][P_LEVEL],name)
							else if (get_user_team(playerid) == TS)
								pos += formatex(szTmpMsg[pos],2047-pos,"<li id='t'>(%d) %s</li>",p_data[playerid][P_LEVEL],name)
						#endif
						#if MOD == 1
							if(get_user_team(playerid) == ALLIES)
								pos += formatex(szTmpMsg[pos],2047-pos,"<li id='a'>(%d) %s</li>",p_data[playerid][P_LEVEL],name)
							else if(get_user_team(playerid) == AXIS)
								pos += formatex(szTmpMsg[pos],2047-pos,"<li id='x'>(%d) %s</li>",p_data[playerid][P_LEVEL],name)
						#endif
							else
								pos += formatex(szTmpMsg[pos],2047-pos,"<li id='p'>(%d) %s</li>",p_data[playerid][P_LEVEL],name)
						}
						else{
							console_print(id, "%-2s(%d) %s","",p_data[playerid][P_LEVEL],name)
						}
					}
				}
			}
			if (saychat)
				pos += formatex(szTmpMsg[pos],2047-pos,"</ul>")
		}
	}

	if( saychat ) {
		new motdmessage[128]
		formatex(motdmessage, 127, "%L",id,"PLAYER_SKILLS")
		show_motd(id,szTmpMsg,motdmessage)
	}
	
	return PLUGIN_HANDLED
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
		pos += formatex( szTmpMsg[pos], 2048-pos, "%s", MOTD_header );
		pos += formatex( szTmpMsg[pos], 2048-pos, "<div id=^"title^">%s</div><br><br>", szRaceName );

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
					pos += formatex( szTmpMsg[pos], 2048-pos, "<h1>%L</h1>", id, "WORD_TRAINABLE_SKILLS" );
				}

				pos += formatex( szTmpMsg[pos], 2048-pos, "<li>%s</li><ul>%s</ul><br>", szTmp, szTmpDesc );
			}

			// Add the ultimate
			else if ( i == 3 )
			{
				pos += formatex( szTmpMsg[pos], 2048-pos, "<h1>%L</h1>", id, "WORD_ULTIMATE" );
				pos += formatex( szTmpMsg[pos], 2048-pos, "<li>%s</li><ul>%s</ul>", szTmp, szTmpDesc );

			}

			// Add the hero's passive ability
			else if ( i == 4 && p_data[id][P_RACE] > 4 )
			{
				pos += formatex( szTmpMsg[pos], 2048-pos, "<br><h1>%L</h1>", id, "WORD_HERO_ABILITY" );
				pos += formatex( szTmpMsg[pos], 2048-pos, "<li>%s</li><ul>%s</ul><br>", szTmp, szTmpDesc );
			}
		}

		pos += formatex( szTmpMsg[pos], 2048-pos, "<br><div>%L</div>", id, "CLICK_HERE", g_MOD );

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