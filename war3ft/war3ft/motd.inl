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

public MOTD_Playerskills(id){
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
	new race_skill[4][RACE_SKILL_LENGTH]

	get_players(players, numberofplayers)

	if(iCvar[FT_STEAM])
		pos += format(message[pos],2048-pos, "<body bgcolor=#000000><font color=#FFB000>")

	for(j=1;j<(iCvar[FT_RACES]+1);j++){
		iTotalRace = 0
	
		for(k=0;k<numberofplayers;k++){
			if(p_data[players[k]][P_RACE] == j)
				iTotalRace++
		}

		if(iTotalRace > 0){
			race_name=""
			racename(j,id,race_name,RACE_NAME_LENGTH_F)
			
			if(iCvar[FT_STEAM])
				pos += format(message[pos],2048-pos, "<font color='#00FF00'><b>%s</b></font><ul>",race_name)
			else
				pos += format(message[pos],2048-pos, "^n%s",race_name)

			for (i = 0; i < numberofplayers; ++i){
				playerid=players[i]

				if(p_data[playerid][P_RACE] == j){
					get_user_name(playerid,name,31)

					replace(name, 127, "<", "&lt;")
					replace(name, 127, ">", "&gt;")

					if (iCvar[FT_STEAM])
						pos += format(message[pos],2048-pos,"<li>%s - %d",name,p_data[playerid][P_LEVEL])
					else
						pos += format(message[pos],2048-pos,"/t%s - %d",name,p_data[playerid][P_LEVEL])
	
					for(k=0;k<4;k++){
						raceskill(p_data[playerid][0],k+1,id,race_skill[k],RACE_SKILL_LENGTH_F)
					}

					if(p_data[playerid][P_SKILL1])
						pos += format(message[pos],2048-pos,", %s %d",race_skill[0],p_data[playerid][P_SKILL1])
					if(p_data[playerid][P_SKILL2])
						pos += format(message[pos],2048-pos,", %s %d",race_skill[1],p_data[playerid][P_SKILL2])
					if(p_data[playerid][P_SKILL3])
						pos += format(message[pos],2048-pos,", %s %d",race_skill[2],p_data[playerid][P_SKILL3])
					if(p_data[playerid][P_ULTIMATE])
						pos += format(message[pos],2048-pos,", %s",race_skill[3])

					if (iCvar[FT_STEAM])
						pos += format(message[pos],2048-pos,"</li>")
					else
						pos += format(message[pos],2048-pos,"^n")
				}
			}
			if (iCvar[FT_STEAM])
				pos += format(message[pos],2048-pos,"</ul>")
		}
	}

	new motdmessage[128]
	format(motdmessage, 127, "%L",id,"PLAYER_SKILLS","","")
	show_motd(id,message,motdmessage)

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

	new message[2048]
	new title[64], temp[1024]
	new pos = 0, len = 0, p[8]
	if(iCvar[FT_STEAM]){
		new szGame[8]

		if (is_running("cstrike"))
			szGame = "cstrike"
		else if (is_running("czero"))
			szGame = "czero"
		else if (is_running("dod"))
			szGame = "dod"

		format(p,7,"<p>")
		len += format(message[len],1024-len,"<pre><body bgcolor=#000000><font color=#FFB000>")
		len += format(message[len],1024-len,"%L %s",id,"CLICK_HERE", szGame, p)
	}
	else{
		format(p,7,"^n^n")
	}
	new race_name[RACE_NAME_LENGTH]
	racename(p_data[id][P_RACE],id,race_name,RACE_NAME_LENGTH_F)

	format(title,63,"%L",id,"SKILLS_STRING",race_name)
	new race_skill[4][RACE_SKILL_LENGTH]
	for(new i=0;i<4;i++){
		raceskill(p_data[id][P_RACE],i+1,id,race_skill[i],RACE_SKILL_LENGTH_F)
	}

	if (p_data[id][P_RACE]==1){
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE1_SKILL1_INFO",race_skill[0],floatround(p_vampiric[0]*100),floatround(p_vampiric[1]*100),floatround(p_vampiric[2]*100),p)
	#if MOD == 0
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE1_SKILL2_INFO",race_skill[1],p)
	#endif
	#if MOD == 1
		pos+= format(temp[pos],1024-pos,"%L%s",id,"DOD_RACE1_SKILL2_INFO",race_skill[1],p)
	#endif
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE1_SKILL3_INFO",race_skill[2],p)
		pos+= format(temp[pos],1024-pos,"%L",id,"RACE1_SKILL4_INFO",race_skill[3])
	}
	else if (p_data[id][P_RACE]==2){
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE2_SKILL1_INFO",race_skill[0],p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE2_SKILL2_INFO",race_skill[1],p_devotion[0],p_devotion[1],p_devotion[2],p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE2_SKILL3_INFO",race_skill[2],floatround(p_bash[0]*100),floatround(p_bash[1]*100),floatround(p_bash[2]*100),p)
		if (iCvar[FT_BLINKENABLED])
			pos+= format(temp[pos],1024-pos,"%L",id,"RACE2_SKILL4_INFO_B",race_skill[3])
		else
			pos+= format(temp[pos],1024-pos,"%L",id,"RACE2_SKILL4_INFO_T",race_skill[3])
	}
	else if (p_data[id][P_RACE]==3){
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE3_SKILL1_INFO",race_skill[0],floatround(p_critical[0]*100),p)
		#if MOD == 1
			pos+= format(temp[pos],1024-pos,"%L%s",id,"DOD_RACE3_SKILL2_INFO",race_skill[1],floatround(p_grenade[0]),floatround(p_grenade[1]),floatround(p_grenade[2]),p)
		#else
			pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE3_SKILL2_INFO",race_skill[1],floatround(p_grenade[0]),floatround(p_grenade[1]),floatround(p_grenade[2]),p)
		#endif
		#if MOD == 1
			pos+= format(temp[pos],1024-pos,"%L%s",id,"DOD_RACE3_SKILL3_INFO",race_skill[2],floatround(p_ankh[0]*100),floatround(p_ankh[1]*100),floatround(p_ankh[2]*100),p)
		#else
			pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE3_SKILL3_INFO",race_skill[2],floatround(p_ankh[0]*100),floatround(p_ankh[1]*100),floatround(p_ankh[2]*100),p)
		#endif
		pos+= format(temp[pos],1024-pos,"%L",id,"RACE3_SKILL4_INFO",race_skill[3])
	}
	else if (p_data[id][P_RACE]==4){
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE4_SKILL1_INFO",race_skill[0],floatround(p_evasion[0]*100),floatround(p_evasion[1]*100),floatround(p_evasion[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE4_SKILL2_INFO",race_skill[1],floatround(p_thorns[0]*100),floatround(p_thorns[1]*100),floatround(p_thorns[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE4_SKILL3_INFO",race_skill[2],floatround(p_trueshot[0]*100),floatround(p_trueshot[1]*100),floatround(p_trueshot[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L",id,"RACE4_SKILL4_INFO",race_skill[3])
	}
	else if (p_data[id][P_RACE]==5){
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE5_SKILL1_INFO",race_skill[0],floatround(p_pheonix[0]*100),floatround(p_pheonix[1]*100),floatround(p_pheonix[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE5_SKILL2_INFO",race_skill[1],floatround(p_banish[0]*100),floatround(p_banish[1]*100),floatround(p_banish[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE5_SKILL3_INFO",race_skill[2],floatround(p_mana[0]*100),floatround(p_mana[1]*100),floatround(p_mana[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L",id,"RACE5_SKILL4_INFO",race_skill[3])
	}
	else if (p_data[id][P_RACE]==6){
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE6_SKILL1_INFO",race_skill[0],floatround(p_heal[0]),floatround(p_heal[1]),floatround(p_heal[2]),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE6_SKILL2_INFO",race_skill[1],floatround(p_hex[0]*100),floatround(p_hex[1]*100),floatround(p_hex[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE6_SKILL3_INFO",race_skill[2],p_serpent[0],p_serpent[1],p_serpent[2],p)
		pos+= format(temp[pos],1024-pos,"%L",id,"RACE6_SKILL4_INFO",race_skill[3])
	}
	else if (p_data[id][P_RACE]==7){
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE7_SKILL1_INFO",race_skill[0],floatround(p_fan[0]*100),floatround(p_fan[1]*100),floatround(p_fan[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE7_SKILL2_INFO",race_skill[1],floatround(p_blink[0]*100),floatround(p_blink[1]*100),floatround(p_blink[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE7_SKILL3_INFO",race_skill[2],floatround(p_shadow[0]*100),floatround(p_shadow[1]*100),floatround(p_shadow[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L",id,"RACE7_SKILL4_INFO",race_skill[3])
	}
	else if (p_data[id][P_RACE]==8){
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE8_SKILL1_INFO",race_skill[0],p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE8_SKILL2_INFO",race_skill[1],floatround(p_spiked[0]*100),floatround(p_spiked[1]*100),floatround(p_spiked[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE8_SKILL3_INFO",race_skill[2],floatround(p_carrion[0]*100),floatround(p_carrion[1]*100),floatround(p_carrion[2]*100),p)
		pos+= format(temp[pos],1024-pos,"%L",id,"RACE8_SKILL4_INFO",race_skill[3])
	}
	else if (p_data[id][P_RACE]==9){
		switch(race9Options[1]){
			case 1: pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE1_SKILL1_INFO",race_skill[0],floatround(p_vampiric[0]*100),floatround(p_vampiric[1]*100),floatround(p_vampiric[2]*100),p)
			case 2: pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE2_SKILL1_INFO",race_skill[0],p)
			case 3: pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE3_SKILL1_INFO",race_skill[0],floatround(p_critical[0]*100),p)
			case 4:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE4_SKILL1_INFO",race_skill[0],floatround(p_evasion[0]*100),floatround(p_evasion[1]*100),floatround(p_evasion[2]*100),p)
			case 5: pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE5_SKILL1_INFO",race_skill[0],floatround(p_pheonix[0]*100),floatround(p_pheonix[1]*100),floatround(p_pheonix[2]*100),p)
			case 6: pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE6_SKILL1_INFO",race_skill[0],floatround(p_heal[0]),floatround(p_heal[1]),floatround(p_heal[2]),p)
			case 7: pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE7_SKILL1_INFO",race_skill[0],floatround(p_fan[0]*100),floatround(p_fan[1]*100),floatround(p_fan[2]*100),p)
			case 8: pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE8_SKILL1_INFO",race_skill[0],p)
		}
		switch(race9Options[2]){
			case 1:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE1_SKILL2_INFO",race_skill[1],p)
			case 2:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE2_SKILL2_INFO",race_skill[1],p_devotion[0],p_devotion[1],p_devotion[2],p)
			case 3:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE3_SKILL2_INFO",race_skill[1],floatround(p_grenade[0]),floatround(p_grenade[1]),floatround(p_grenade[2]),p)
			case 4:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE4_SKILL2_INFO",race_skill[1],floatround(p_thorns[0]*100),floatround(p_thorns[1]*100),floatround(p_thorns[2]*100),p)
			case 5:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE5_SKILL2_INFO",race_skill[1],floatround(p_banish[0]*100),floatround(p_banish[1]*100),floatround(p_banish[2]*100),p)
			case 6:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE6_SKILL2_INFO",race_skill[1],floatround(p_hex[0]*100),floatround(p_hex[1]*100),floatround(p_hex[2]*100),p)
			case 7:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE7_SKILL2_INFO",race_skill[1],floatround(p_blink[0]*100),floatround(p_blink[1]*100),floatround(p_blink[2]*100),p)
			case 8:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE8_SKILL2_INFO",race_skill[1],floatround(p_spiked[0]*100),floatround(p_spiked[1]*100),floatround(p_spiked[2]*100),p)
		}
		switch(race9Options[3]){
			case 1:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE1_SKILL3_INFO",race_skill[2],p)
			case 2:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE2_SKILL3_INFO",race_skill[2],floatround(p_bash[0]*100),floatround(p_bash[1]*100),floatround(p_bash[2]*100),p)
			case 3:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE3_SKILL3_INFO",race_skill[2],floatround(p_ankh[0]*100),floatround(p_ankh[1]*100),floatround(p_ankh[2]*100),p)
			case 4:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE4_SKILL3_INFO",race_skill[2],floatround(p_trueshot[0]*100),floatround(p_trueshot[1]*100),floatround(p_trueshot[2]*100),p)
			case 5:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE5_SKILL3_INFO",race_skill[2],floatround(p_mana[0]*100),floatround(p_mana[1]*100),floatround(p_mana[2]*100),p)
			case 6:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE6_SKILL3_INFO",race_skill[2],p_serpent[0],p_serpent[1],p_serpent[2],p)
			case 7:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE7_SKILL3_INFO",race_skill[2],floatround(p_shadow[0]*100),floatround(p_shadow[1]*100),floatround(p_shadow[2]*100),p)
			case 8:	pos+= format(temp[pos],1024-pos,"%L%s",id,"RACE8_SKILL3_INFO",race_skill[2],floatround(p_carrion[0]*100),floatround(p_carrion[1]*100),floatround(p_carrion[2]*100),p)
		}
		switch(race9Options[4]){
			case 1:	pos+= format(temp[pos],1024-pos,"%L",id,"RACE1_SKILL4_INFO",race_skill[3])
			case 2:{
				if (iCvar[FT_BLINKENABLED])
					pos+= format(temp[pos],1024-pos,"%L",id,"RACE2_SKILL4_INFO_B",race_skill[3])
				else
					pos+= format(temp[pos],1024-pos,"%L",id,"RACE2_SKILL4_INFO_T",race_skill[3])
			}
			case 3:		pos+= format(temp[pos],1024-pos,"%L",id,"RACE3_SKILL4_INFO",race_skill[3])
			case 4:		pos+= format(temp[pos],1024-pos,"%L",id,"RACE4_SKILL4_INFO",race_skill[3])
			case 5:		pos+= format(temp[pos],1024-pos,"%L",id,"RACE5_SKILL4_INFO",race_skill[3])
			case 6:		pos+= format(temp[pos],1024-pos,"%L",id,"RACE6_SKILL4_INFO",race_skill[3])
			case 7:		pos+= format(temp[pos],1024-pos,"%L",id,"RACE7_SKILL4_INFO",race_skill[3])
			case 8:		pos+= format(temp[pos],1024-pos,"%L",id,"RACE8_SKILL4_INFO",race_skill[3])
		}


	}
	else{
		console_print(id,"%L",id,"SELECT_RACE_BEFORE_SKILLS")
		return PLUGIN_HANDLED
	}

	if(p_data[id][P_RACE]!=0){
		len += format(message[len],1024-len,temp)
	}

	new race_info[128]
	format(race_info,127,"%L",id,"RACE_INFORMATION")
	show_motd(id,message,race_info)
	if (saychat==1)
		return PLUGIN_CONTINUE
	return PLUGIN_HANDLED
}
