public MOTD_Itemsinfo(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("MOTD_Itemsinfo",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	new message[2048], title[128], pos = 0
	new item_name[9][ITEM_NAME_LENGTH]

	for(new i=0;i<9;i++){
		itemname(i+1,id,item_name[i],ITEM_NAME_LENGTH_F)
	}

	// Add header

	pos += format(message[pos], 2048-pos, "%s", MOTD_header)

	// Add Item Information

	#if MOD == 0
		pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[0],id,"SHOPMENU_1")
		pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[1],id,"SHOPMENU_2")
	#endif
	#if MOD == 1
		pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[0],id,"DOD_SHOPMENU_1")
		pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[1],id,"DOD_SHOPMENU_2")
	#endif
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[2],id,"SHOPMENU_3",iCvar[FT_CLAW],iCvar[FT_CLAW])
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[3],id,"SHOPMENU_4")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[4],id,"SHOPMENU_5")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[5],id,"SHOPMENU_6")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[6],id,"SHOPMENU_7")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[7],id,"SHOPMENU_8")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name[8],id,"SHOPMENU_9")

	format(title, 127, "%L",id,"SHOPMENU_TITLE")
	show_motd(id,message,title)

	return PLUGIN_HANDLED
}

public MOTD_Itemsinfo2(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("MOTD_Itemsinfo2",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE
	if(iCvar[FT_RACES] < 5)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	new message[2048], title[128], pos = 0
	new item_name2[9][64]

	for(new i=0;i<9;i++){
		itemname2(i+1,id,item_name2[i],64)
	}

	// Add header

	pos += format(message[pos], 2048-pos, "%s", MOTD_header)

	// Add Item Information

	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[0],id,"SHOPMENU2_1")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[1],id,"SHOPMENU2_2")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[2],id,"SHOPMENU2_3")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[3],id,"SHOPMENU2_4")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[4],id,"SHOPMENU2_5")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[5],id,"SHOPMENU2_6",iCvar[FT_GLOVE_TIMER])
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[6],id,"SHOPMENU2_7")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[7],id,"SHOPMENU2_8")
	pos += format(message[pos], 2048-pos, "<font color='#F4D285'><li>%s</li></font><div id='s'>%L</div><br>",item_name2[8],id,"SHOPMENU2_9")

	format(title, 127, "%L",id,"SHOPMENU2_TITLE")
	show_motd(id,message,title)

	return PLUGIN_HANDLED
}

public MOTD_War3help(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("war3_info",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	new message[2048]

	new title[128], szGame[8]
	new pos = 0

	if (is_running("cstrike"))
		szGame = "cstrike"
	else if (is_running("czero"))
		szGame = "czero"
	else if (is_running("dod"))
		szGame = "dod"

	format(title,127,"%L",id,"WAR3HELP_TITLE")

	pos += format(message[pos], 2048-pos, "<body bgcolor=#000000><font color=#FFB000>")
	pos += format(message[pos], 2048-pos, "%L<br>",id,"THIS_MOD_WAS_DEVELOPED")
	pos += format(message[pos], 2048-pos, "%L<br>",id,"CLICK_HERE_FOR_LATEST")	
	pos += format(message[pos], 2048-pos, "%L<p>",id,"CLICK_HERE", szGame)
	pos += format(message[pos], 2048-pos, "%L",id,"WAR3HELP")

	show_motd(id,message,title)

	return PLUGIN_HANDLED
}

public MOTD_Playerskills(id, saychat){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("player_skills",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE
		
	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	new name[32], message[2048]
	
	new race_name[64]
	new players[32], numberofplayers, i, j, k, playerid, pos, iTotalRace

	get_players(players, numberofplayers)

	if(saychat)
		pos += format(message[pos],2048-pos, "<body bgcolor=#000000><font color=#FFB000>")

	for(j=1;j<(iCvar[FT_RACES]+1);j++){
		iTotalRace = 0
	
		for(k=0;k<numberofplayers;k++){
			if(p_data[players[k]][P_RACE] == j)
				iTotalRace++
		}

		if(iTotalRace > 0){
			race_name=""
			racename(j,id,race_name,64)
			
			if ( saychat ) {
				pos += format(message[pos],2048-pos, "<font color=#00FF00><b>%s</b></font><ul>",race_name)
			}
			else{
				console_print(id, "**** %s ****", race_name)
			}

			for (i = 0; i < numberofplayers; ++i){
				playerid=players[i]

				if(p_data[playerid][P_RACE] == j){
					get_user_name(playerid,name,31)
					
					if ( saychat ) {
						replace(name, 127, "<", "&lt;")
						replace(name, 127, ">", "&gt;")

						pos += format(message[pos],2048-pos,"<li>(%d) %s",p_data[playerid][P_LEVEL],name)
					}
					else{
						console_print(id, "%-2s(%d) %s","",p_data[playerid][P_LEVEL],name)
					}
					
					if( saychat ) {
						pos += format(message[pos],2048-pos,"</li>")
					}
				}
			}
			if (saychat)
				pos += format(message[pos],2048-pos,"</ul>")
		}
	}

	if( saychat ) {
		new motdmessage[128]
		format(motdmessage, 127, "%L",id,"PLAYER_SKILLS","","")
		show_motd(id,message,motdmessage)
	}

	return PLUGIN_CONTINUE
}

public MOTD_Skillsinfo(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("skills_info",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	if (0 < p_data[id][P_RACE] <= iCvar[FT_RACES]){
		new message[2048], szGame[8]
		new race_skill[4][64], skill_description[4][256], race_name[64]
		new pos = 0, i

		if (is_running("cstrike"))
			szGame = "cstrike"
		else if (is_running("czero"))
			szGame = "czero"
		else if (is_running("dod"))
			szGame = "dod"

		// Get the Race Name

		racename(p_data[id][P_RACE],id,race_name,RACE_NAME_LENGTH_F)

		// Format the Title

		pos += format(message[pos],2048-pos, "%s", MOTD_header)
		pos += format(message[pos],1024-pos,"%L <br>",id,"CLICK_HERE", szGame)
		pos += format(message[pos],2048-pos,"<br><br><font face=^"Verdana, Arial, Helvetica, sans-serif^" color=^"#00FF00^" size=+1><b>%s</b></font><br><br>",race_name)

		for(i=0;i<4;i++){

			// Get Skill Name
			raceskill(p_data[id][P_RACE],i+1,id,race_skill[i],64)

			// Get Skill Description
			if(p_data[id][P_RACE] == RACE_CHAMELEON)
				Lang_Skill_Info(race9Options[i+1], i+1, id, skill_description[i], 256)
			else
				Lang_Skill_Info(p_data[id][P_RACE], i+1, id, skill_description[i], 256)

		}

		for(i=0;i<4;i++){

			// Add each skill
			if(0 <= i < 3){
				pos += format(message[pos],2048-pos,"<font color='#F4D285'><li>%s</li></font><div id='s'>%s</div><br>", race_skill[i], skill_description[i])

			}

			// Add the ultimate
			else{
				pos += format(message[pos],2048-pos,"<font color='#F4D285'><li>%L, %s</li></font><div id='s'>%s</div><br>", id, "WORD_ULTIMATE", race_skill[i], skill_description[i])

			}
		}

		new race_info[128]
		format(race_info,127,"%L",id,"RACE_INFORMATION")
		show_motd(id,message,race_info)

	}
	else{
		client_print(id, print_chat,"%L",id,"SELECT_RACE_BEFORE_SKILLS")
		console_print(id,"%L",id,"SELECT_RACE_BEFORE_SKILLS")
		return PLUGIN_HANDLED
	}

	return PLUGIN_HANDLED
}
