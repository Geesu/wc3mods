public MOTD_Itemsinfo(id,saychat){
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

	new temp[1024], pos = 0
	new theforward[8],backward[8]
	if(iCvar[FT_STEAM]){
		format(theforward,7,"<li>")
		format(backward,7,"</li>")
		pos += format(temp[pos], 1024-pos, "<body bgcolor=#000000><font color=#FFB000>")
	}
	else{
		format(theforward,7,"")
		format(backward,7,"^n")
	}

	new item_name[9][ITEM_NAME_LENGTH]
	for(new i=0;i<9;i++){
		itemname(i+1,id,item_name[i],ITEM_NAME_LENGTH_F)
	}

	if(saychat==1){
		#if MOD == 0
			pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_1",item_name[0],backward)
			pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_2",item_name[1],backward)
		#endif
		#if MOD == 1
			pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"DOD_SHOPMENU_1",item_name[0],backward)
			pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"DOD_SHOPMENU_2",item_name[1],backward)
		#endif
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_3",item_name[2],iCvar[FT_CLAW],iCvar[FT_CLAW],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_4",item_name[3],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_5",item_name[4],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_6",item_name[5],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_7",item_name[6],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_8",item_name[7],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU_9",item_name[8],backward)

		new motdmessage[128]
		format(motdmessage, 127, "%L",id,"SHOPMENU_TITLE")
		show_motd(id,temp,motdmessage)

	}
	else{
		console_print(id,"---- %L ----",id,"SHOPMENU_TITLE")
		console_print(id,"%L",id,"SHOPMENU_1",item_name[0])
		console_print(id,"%L",id,"SHOPMENU_2",item_name[1])
		console_print(id,"%L",id,"SHOPMENU_3",item_name[2],iCvar[FT_CLAW],iCvar[FT_CLAW])
		console_print(id,"%L",id,"SHOPMENU_4",item_name[3])
		console_print(id,"%L",id,"SHOPMENU_5",item_name[4])
		console_print(id,"%L",id,"SHOPMENU_6",item_name[5])
		console_print(id,"%L",id,"SHOPMENU_7",item_name[6])
		console_print(id,"%L",id,"SHOPMENU_8",item_name[7])
		console_print(id,"%L",id,"SHOPMENU_9",item_name[8])
		console_print(id,"---- %L ----",id,"SHOPMENU_TITLE")
	}

	return PLUGIN_CONTINUE
}

public MOTD_Itemsinfo2(id,saychat){
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

	new temp[1024], pos = 0
	new theforward[8],backward[8]
	if(iCvar[FT_STEAM]){
		format(theforward,7,"<li>")
		format(backward,7,"</li>")
		pos += format(temp[pos], 1024-pos, "<body bgcolor=#000000><font color=#FFB000>")
	}
	else{
		format(theforward,7,"")
		format(backward,7,"^n")
	}

	new item_name2[9][ITEM_NAME_LENGTH_F]
	for(new i=0;i<9;i++){
		itemname2(i+1,id,item_name2[i],ITEM_NAME_LENGTH_F)
	}

	if(saychat==1){
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_1",item_name2[0],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_2",item_name2[1],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_3",item_name2[2],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_4",item_name2[3],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_5",item_name2[4],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_6",item_name2[5],iCvar[FT_GLOVE_TIMER],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_7",item_name2[6],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_8",item_name2[7],backward)
		pos += format(temp[pos], 1024-pos, "%s %L %s",theforward,id,"SHOPMENU2_9",item_name2[8],backward)

		new motdmessage[128]
		format(motdmessage, 127, "%L",id,"SHOPMENU2_TITLE")
		show_motd(id,temp,motdmessage)
	}
	else{
		console_print(id,"%L",id,"---- SHOPMENU2_TITLE ----")
		console_print(id,"%L",id,"SHOPMENU2_1",item_name2[0])
		console_print(id,"%L",id,"SHOPMENU2_2",item_name2[1])
		console_print(id,"%L",id,"SHOPMENU2_3",item_name2[2])
		console_print(id,"%L",id,"SHOPMENU2_4",item_name2[3])
		console_print(id,"%L",id,"SHOPMENU2_5",item_name2[4])
		console_print(id,"%L",id,"SHOPMENU2_6",item_name2[5],iCvar[FT_GLOVE_TIMER])
		console_print(id,"%L",id,"SHOPMENU2_7",item_name2[6])
		console_print(id,"%L",id,"SHOPMENU2_8",item_name2[7])
		console_print(id,"%L",id,"SHOPMENU2_9",item_name2[8])
		console_print(id,"%L",id,"---- SHOPMENU2_TITLE ----")
	}

	return PLUGIN_CONTINUE
}

public MOTD_War3help(id, saychat){
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

	new temp[2048], message[1024], title[128]
	new pos = 0
	new thereturn[8], thep[8]

	if(iCvar[FT_STEAM]){
		new szGame[8]

		if (is_running("cstrike"))
			szGame = "cstrike"
		else if (is_running("czero"))
			szGame = "czero"
		else if (is_running("dod"))
			szGame = "dod"

		format(thereturn,7,"<br>")
		format(thep,7,"<p>")
		pos += format(message[pos], 1024-pos, "<body bgcolor=#000000><font color=#FFB000>")
		pos += format(message[pos], 1024-pos, "%L",id,"THIS_MOD_WAS_DEVELOPED",thereturn)
		pos += format(message[pos], 1024-pos, "%L %s",id,"CLICK_HERE_FOR_LATEST",thereturn)	
		pos += format(message[pos], 1024-pos, "%L %s",id,"CLICK_HERE", szGame, thereturn)
	}
	else{
		format(thereturn,7,"^n")
		format(thep,7,"^n^n")
		pos += format(message[pos], 1024-pos, "%L",id,"THIS_g_MOD_WAS_DEVELOPED",thereturn)
	}

	if(saychat==1){
		format(title,127,"%L",id,"WAR3HELP_TITLE")
		format(temp,2047,"%L",id,"WAR3HELP",message,thep,thereturn,thereturn,thereturn,thereturn,thep,thep,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn)
		show_motd(id,temp,title)
	}
	else{		// Currently doesn't work correctly
		thereturn = "^n"
		thep = "^n"
		format(title,127,"---- %L ----^n^n",id,"WAR3HELP_TITLE")
		format(temp,2047,"%L",id,"WAR3HELP",title,thep,thereturn,thereturn,thereturn,thereturn,thep,thep,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn,thereturn)
		console_print(id,temp)
	}

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
	
	new race_name[RACE_NAME_LENGTH]
	new players[32], numberofplayers, i, j, k, playerid, pos, iTotalRace

	get_players(players, numberofplayers)

	if(iCvar[FT_STEAM] && saychat)
		pos += format(message[pos],2047-pos, "<body bgcolor=#000000><font color=#FFB000>")

	for(j=1;j<(iCvar[FT_RACES]+1);j++){
		iTotalRace = 0
	
		for(k=0;k<numberofplayers;k++){
			if(p_data[players[k]][P_RACE] == j)
				iTotalRace++
		}

		if(iTotalRace > 0){
			race_name=""
			racename(j,id,race_name,RACE_NAME_LENGTH_F)
			
			if ( saychat ) {
				if(iCvar[FT_STEAM])
					pos += format(message[pos],2047-pos, "<font color=#00FF00><b>%s</b></font><ul>",race_name)
				else
					pos += format(message[pos],2047-pos, "^n%s",race_name)
			}
			else{
				console_print(id, "**** %s ****", race_name)
			}

			for (i = 0; i < numberofplayers; ++i){
				playerid=players[i]

				if(p_data[playerid][P_RACE] == j){
					get_user_name(playerid,name,31)
					
					if ( saychat ) {
						if ( iCvar[FT_STEAM] ){
							replace(name, 127, "<", "&lt;")
							replace(name, 127, ">", "&gt;")

							pos += format(message[pos],2047-pos,"<li>(%d) %s",p_data[playerid][P_LEVEL],name)
						}
						else
							pos += format(message[pos],2047-pos,"^t(%d) %s",p_data[playerid][P_LEVEL],name)
					}
					else{
						console_print(id, "%-2s(%d) %s","",p_data[playerid][P_LEVEL],name)
					}
					
					if( saychat ) {
						if (iCvar[FT_STEAM])
							pos += format(message[pos],2047-pos,"</li>")
						else
							pos += format(message[pos],2047-pos,"^n")
					}
				}
			}
			if (iCvar[FT_STEAM] && saychat)
				pos += format(message[pos],2047-pos,"</ul>")
		}
	}

	if( saychat ) {
		new motdmessage[128]
		format(motdmessage, 127, "%L",id,"PLAYER_SKILLS","","")
		show_motd(id,message,motdmessage)
	}

	return PLUGIN_CONTINUE
}

public MOTD_Skillsinfo(id,saychat){
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

		pos += format(message[pos],1024-pos,"<body bgcolor=#000000><font color=#FFB000>")
		pos += format(message[pos],1024-pos,"%L <br>",id,"CLICK_HERE", szGame)


		// Get the Race Name

		racename(p_data[id][P_RACE],id,race_name,RACE_NAME_LENGTH_F)

		// Format the Title

		pos += format(message[pos],2048-pos,"<html><head><style type=^"text/css^">#s{text-indent:35px;width:500px;}</style></head><body>")
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
