public menu_Shopmenu_One(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_Shopmenu_One",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	if(!iCvar[FT_BUYDEAD] && !is_user_alive(id)){
		client_print(id,print_center,"%L",id,"NOT_BUY_ITEMS_WHEN_DEAD")
		return PLUGIN_CONTINUE
	}
	#if MOD == 0
		else if(iCvar[FT_BUYTIME] && !g_buyTime){
			new Float:thetime = get_cvar_float("mp_buytime")*60.0
			client_print(id,print_center,"%L",id,"SECONDS_HAVE_PASSED_CANT_BUY",thetime)
			return PLUGIN_CONTINUE
		}
		else if(iCvar[FT_BUYZONE] && !p_data_b[id][PB_BUYZONE]){
			client_print(id,print_center,"%L",id,"MUST_BE_IN_BUYZONE")
			return PLUGIN_CONTINUE
		}
	#endif
	
	new pos = 0
	new keys = (1<<9)
	new menu_body[512]

	pos += format(menu_body[pos], 511-pos, "%L",id,"MENU_BUY_ITEM")
	
	new item_name[9][ITEM_NAME_LENGTH]
	for(new i=0;i<9;i++){
		itemname(i+1,id,item_name[i],ITEM_NAME_LENGTH_F)

		pos += format(menu_body[pos], 511-pos, "\w%d. %s\y\R%d^n",i+1,item_name[i],itemcost[i])
		keys |= (1<<i)
	}

	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")


	show_menu(id,keys,menu_body,-1)
	return PLUGIN_HANDLED
}

public menu_Shopmenu_Two(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_Shopmenu_Two",id)
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

	if(!iCvar[FT_BUYDEAD] && !is_user_alive(id)){
		client_print(id,print_center,"%L",id,"NOT_BUY_ITEMS_WHEN_DEAD")
		return PLUGIN_CONTINUE
	}
	#if MOD == 0
		else if(iCvar[FT_BUYTIME] && !g_buyTime){
			new Float:thetime = get_cvar_float("mp_buytime")*60.0
			client_print(id,print_center,"%L",id,"SECONDS_HAVE_PASSED_CANT_BUY",thetime)
			return PLUGIN_CONTINUE
		}
		else if(iCvar[FT_BUYZONE] && !p_data_b[id][PB_BUYZONE]){
			client_print(id,print_center,"%L",id,"MUST_BE_IN_BUYZONE")
			return PLUGIN_CONTINUE
		}
	#endif
	new pos = 0
	new keys = (1<<9)
	new menu_body[512]

	pos += format(menu_body[pos], 511-pos, "%L",id,"MENU_BUY_ITEM2")

	new item_name2[9][ITEM_NAME_LENGTH]
	for(new i=0;i<9;i++){
		itemname2(i+1,id,item_name2[i],ITEM_NAME_LENGTH_F)

	#if MOD == 1
		if(i==ITEM_CHAMELEON-1 || i==ITEM_SCROLL-1)
			pos += format(menu_body[pos], 511-pos, "\d%d. %s\y\R%d^n",i+1,item_name2[i],itemcost2[i])
		else{
	#endif
		pos += format(menu_body[pos], 511-pos, "\w%d. %s\y\R%d^n",i+1,item_name2[i],itemcost2[i])
		keys |= (1<<i)
	#if MOD == 1
		}
	#endif
	}

	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")

	show_menu(id,keys,menu_body,-1)
	return PLUGIN_HANDLED
}

public menu_Select_Skill(id,saychat){
	 #if ADVANCED_DEBUG == 1
		writeDebugInfo("select_skill",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	if (p_data[id][P_RACE] == 0){
		if (saychat==1){
			set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
			show_hudmessage(id,"%L",id,"SELECT_RACE_BEFORE_SKILLS")
		}
		else{
			console_print(id,"%L",id,"SELECT_RACE_BEFORE_SKILLS")
		}
		return PLUGIN_HANDLED
	}

	new message[256]
	new temp[128]

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]

	if (skillsused>=p_data[id][P_LEVEL]){
		if (saychat==1){
			set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
			show_hudmessage(id,"%L",id,"ALREADY_SELECTED_SKILL_POINTS")
		}
		else{
			console_print(id,"%L",id,"ALREADY_SELECTED_SKILL_POINTS")
		}
		return PLUGIN_HANDLED
	}

	if (is_user_bot(id)){
		new randomskill
		while (skillsused < p_data[id][P_LEVEL]){
			randomskill = random_num(1,3)
			if (p_data[id][P_ULTIMATE]==0 && p_data[id][P_LEVEL]>=6)
				p_data[id][P_ULTIMATE]=1
			else if (p_data[id][randomskill]!=3 && p_data[id][P_LEVEL]>2*p_data[id][randomskill]){
				++p_data[id][randomskill]
			}
			skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
		}
		return PLUGIN_HANDLED
	}

	format(message,255,"%L",id,"MENU_SELECT_SKILL")


	new skillcounter = 0
	new skillcurrentrace[4][64]

	while (skillcounter < 4){
		new race_skill[RACE_SKILL_LENGTH]
		raceskill(p_data[id][P_RACE],skillcounter,id,race_skill,RACE_SKILL_LENGTH_F)
		copy(skillcurrentrace[skillcounter],63,race_skill)

		++skillcounter
	}

	skillcounter = 1
	while (skillcounter< 4){
		if (p_data[id][skillcounter]!=3){
			if (p_data[id][P_LEVEL]<=2*p_data[id][skillcounter]){
				format(temp,127,"\d")
				add(message,255,temp)
			}
			new race_skill[RACE_SKILL_LENGTH]
			raceskill(p_data[id][P_RACE],skillcounter,id,race_skill,RACE_SKILL_LENGTH_F)

			format(temp,127,"%L",id,"LEVEL_SELECT_SKILL_FUNC",skillcounter,race_skill,p_data[id][skillcounter]+1)
			add(message,255,temp)
		}
		++skillcounter
	}
	if (p_data[id][P_ULTIMATE]==0){
		if (p_data[id][P_LEVEL]<=5){
			format(temp,127,"\d")
			add(message,255,temp)
		}
		new race_skill[RACE_SKILL_LENGTH]
		raceskill(p_data[id][P_RACE],4,id,race_skill,RACE_SKILL_LENGTH_F)

		format(temp,127,"%L",id,"ULTIMATE_SELECT_SKILL_FUNC",race_skill)
		add(message,255,temp)
	}

	new keys = (1<<9)

	if (p_data[id][P_SKILL1]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL1] && skillsused<p_data[id][P_LEVEL])
		keys |= (1<<0)
	if (p_data[id][P_SKILL2]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL2] && skillsused<p_data[id][P_LEVEL])
		keys |= (1<<1)
	if (p_data[id][P_SKILL3]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL3] && skillsused<p_data[id][P_LEVEL])
		keys |= (1<<2)
	if (p_data[id][P_ULTIMATE]==0 && p_data[id][P_LEVEL]>=6 && skillsused<p_data[id][P_LEVEL])
		keys |= (1<<3)

	format(temp,127,"%L",id,"CANCEL_SELECT_SKILL_FUNC")
	add(message,255,temp)
	show_menu(id,keys,message,-1)
	if (saychat==1)
		return PLUGIN_CONTINUE
	return PLUGIN_HANDLED
}

public _menu_Select_Skill(id,key){
	 #if ADVANCED_DEBUG == 1
		writeDebugInfo("set_skill",id)
	#endif

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]

	if (key == KEY_1 && p_data[id][P_SKILL1]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL1] && skillsused<p_data[id][P_LEVEL])
		++p_data[id][P_SKILL1]
	else if (key == KEY_2 && p_data[id][P_SKILL2]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL2] && skillsused<p_data[id][P_LEVEL])
		++p_data[id][P_SKILL2]
	else if (key == KEY_3 && p_data[id][P_SKILL3]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL3] && skillsused<p_data[id][P_LEVEL])
		++p_data[id][P_SKILL3]
	else if (key == KEY_4 && p_data[id][P_ULTIMATE]==0 && p_data[id][P_LEVEL]>=6 && skillsused<p_data[id][P_LEVEL]){
		p_data[id][P_ULTIMATE]=1
		p_data_b[id][PB_ULTIMATEUSED]=false
	}
	else if (key == KEY_0)
		return PLUGIN_HANDLED

	skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
	if (skillsused < p_data[id][P_LEVEL])
		menu_Select_Skill(id,0)
	else
		WAR3_Display_Level(id, DISPLAYLEVEL_NONE)


	// Initiate cooldown for player's ultimate, or give them they're ultimate

	if( !task_exists(TASK_UDELAY+id) && key == KEY_4 ){
		new parm[1]
		parm[0] = id

		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_WAR3_Ultimate_Delay(parm)
	}
	else if ( key == KEY_4 && !p_data[id][P_ULTIMATEDELAY] && !p_data_b[id][PB_ULTIMATEUSED]){
		WAR3_Ultimate_Ready(id)
	}

	// Serpent Ward Chosen
	if ( Verify_Skill(id, RACE_SHADOW, SKILL3) && key == KEY_3 ){
		p_data[id][P_SERPENTCOUNT]++
	}
	// Carrion Beetle Chosen
	if ( Verify_Skill(id, RACE_CRYPT, SKILL3) &&  key == KEY_3 ){
		if( p_data[id][P_CARRIONCOUNT] < 3 ){
			p_data[id][P_CARRIONCOUNT]++
		}
	}
	// Shadow Strike Chosen
	if ( Verify_Skill(id, RACE_WARDEN, SKILL3) && key == KEY_3 ){
		if (p_data[id][P_SHADOWCOUNT] < 3 ){
			p_data[id][P_SHADOWCOUNT] = 2
		}
	}
	// Devotion Aura Chosen
	if ( Verify_Skill(id, RACE_HUMAN, SKILL2) && key == KEY_2 && is_user_alive(id)){
		if(p_data[id][P_SKILL2]==1)
			set_user_health(id,get_user_health(id) + (p_devotion[0] - 100))
		else if(p_data[id][P_SKILL2]==2)
			set_user_health(id,get_user_health(id) + (p_devotion[1] - p_devotion[0]))
		else if(p_data[id][P_SKILL2]==3)
			set_user_health(id,get_user_health(id) + (p_devotion[2] - p_devotion[1]))
	}

	return PLUGIN_HANDLED
}

public menu_Select_Race(id, racexp[9]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_Select_Race",0)
	#endif

	if(g_mapDisabled){
		client_print(id,print_chat,"%s %L", g_MODclient, id, "MAP_DISABLED")
		client_print(id,print_chat,"%s %L", g_MODclient, id, "MAP_DISABLED_DUE")
	}

	new race_name[10][RACE_NAME_LENGTH], i, pos, menu_msg[512], selectrace[128]
	new keys
	format(selectrace, 127, "%L",id ,"MENU_SELECT_RACE")

	for(i=1;i<(iCvar[FT_RACES]+1);i++){
		racename(i,id,race_name[i],RACE_NAME_LENGTH_F)
	}

	if(iCvar[MP_SAVEXP]){
		pos += format(menu_msg[pos], 512-pos, "%L",id,"SELECT_RACE_TITLE", selectrace)

		for(i=1; i<(iCvar[FT_RACES]+1);i++){
			if(i==5){
				new selecthero[128]
				format(selecthero, 127, "%L",id ,"SELECT_HERO")
				pos += format(menu_msg[pos], 512-pos, "%s", selecthero)
			}

			if(i==p_data[id][P_RACE])
				pos += format(menu_msg[pos], 512-pos, "\d%d. %s\d\R%d^n", i, race_name[i], racexp[i-1])
			else if(i==p_data[id][P_CHANGERACE])
				pos += format(menu_msg[pos], 512-pos, "\r%d. %s\r\R%d^n", i, race_name[i], racexp[i-1])
			else
				pos += format(menu_msg[pos], 512-pos, "\w%d. %s\y\R%d^n", i, race_name[i], racexp[i-1])

			keys |= (1<<(i-1))
		}
	}
	else{
		pos += format(menu_msg[pos], 512-pos, "%s^n^n", selectrace)

		for(i=1; i<(iCvar[FT_RACES]+1);i++){
			if(i==5){
				new selecthero[128]
				format(selecthero, 127, "%L",id ,"SELECT_HERO")
				pos += format(menu_msg[pos], 512-pos, "%s", selecthero)
			}

			if(i==p_data[id][P_RACE])
				pos += format(menu_msg[pos], 512-pos, "\d%d. %s^n", i, race_name[i])
			else if(i==p_data[id][P_CHANGERACE])
				pos += format(menu_msg[pos], 512-pos, "\r%d. %s^n", i, race_name[i])
			else
				pos += format(menu_msg[pos], 512-pos, "\w%d. %s^n", i, race_name[i])

			keys |= (1<<(i-1))
		}
	}


	keys |= (1<<(i-1))

	if(iCvar[FT_RACES] == 9)
		i = 0

	pos += format(menu_msg[pos], 512-pos, "%L",id,"SELECT_RACE_FOOTER", i)

	if(iCvar[FT_RACES] != 9){	// Add a cancel button
		keys |= (1<<9)
		pos += format(menu_msg[pos], 512-pos, "^n\w0. %L", id, "WORD_CANCEL")
	}

	show_menu(id, keys, menu_msg, -1)

	return PLUGIN_HANDLED
}

public _menu_Select_Race(id,key){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_Select_Race",id)
	#endif

	if(iCvar[FT_RACES]<9 && key-1 == iCvar[FT_RACES])
		return PLUGIN_HANDLED

	new race, autoselectkey

	if(iCvar[FT_RACES] == 9)
		autoselectkey = KEY_0
	else
		autoselectkey = iCvar[FT_RACES]

	if (key == autoselectkey)
		race = random_num(1,iCvar[FT_RACES])
	else
		race = (key+1)

	if(p_data[id][P_RACE]!=0){
		if(race != p_data[id][P_RACE])
			p_data[id][P_CHANGERACE] = race
	}
	else
		WAR3_set_race(id, race)

	return PLUGIN_HANDLED
}

public menu_War3menu(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_War3menu",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	new pos = 0, i, menu_body[512], menu_items[5][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<9)

	format(menu_items[0],31,"%L",id,"SKILLS_MENU")
	format(menu_items[1],31,"%L",id,"RACE_MENU")
	format(menu_items[2],31,"%L",id,"ITEM_MENU")
	format(menu_items[3],31,"%L",id,"HELP")
	format(menu_items[4],31,"%L",id,"ADMIN_MENU_TITLE")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_WAR3_FT_MENU")
	for (i = 0; i<5; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_HANDLED
}

public _menu_War3menu(id,key){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_War3menu",id)
	#endif

	switch (key){
		case 0:	menu_Skill_Options(id)
		case 1:	menu_Race_Options(id)
		case 2:	menu_Item_Options(id)
		case 3:	MOTD_War3help(id)
		case 4:	menu_Admin_Options(id)
		default:	return PLUGIN_HANDLED
	}
	
	return PLUGIN_HANDLED
}

public menu_Skill_Options(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_Skill_Options",id)
	#endif

	new pos = 0, i, menu_body[512], menu_items[3][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)


	format(menu_items[0],31,"%L",id,"SELECT_SKILLS")
	format(menu_items[1],31,"%L",id,"SKILLS_INFORMATION")
	format(menu_items[2],31,"%L",id,"RESELECT_SKILLS")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_SKILLS_OPTIONS")
	for (i = 0; i<3; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Skill_Options(id,key){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_Skill_Options",id)
	#endif

	switch (key){
		case 0:	menu_Select_Skill(id,1)
		case 1:	MOTD_Skillsinfo(id)
		case 2:	resetSkills(id,1)
		case 8: menu_War3menu(id)
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_Race_Options(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_Race_Options",id)
	#endif

	new pos = 0, i, menu_body[512], menu_items[4][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

	format(menu_items[0],31,"%L",id,"CHANGE_RACE")
	format(menu_items[1],31,"%L",id,"SHOW_LEVEL")
	format(menu_items[2],31,"%L",id,"RESET_XP")
	format(menu_items[3],31,"%L",id,"SHOW_PLAYER_SKILLS")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_RACE_OPTIONS")
	for (i = 0; i<4; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Race_Options(id,key){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_Race_Options",id)
	#endif

	switch (key){
		case 0:	change_race(id,1)
		case 1:	WAR3_Display_Level(id,DISPLAYLEVEL_SHOWRACE)
		case 2:	amx_resetxp(id,1)
		case 3:	MOTD_Playerskills(id, 1)
		case 8: menu_War3menu(id)
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_Item_Options(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_Item_Options",id)
	#endif

	new pos = 0, i, menu_body[512], menu_items[4][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

	format(menu_items[0],31,"%L",id,"SHOPMENU1_OPTION")
	format(menu_items[1],31,"%L",id,"SHOPMENU2_OPTION")
	format(menu_items[2],31,"%L",id,"SHOW_SHOPMENU1_INFO")
	format(menu_items[3],31,"%L",id,"SHOW_SHOPMENU2_INFO")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_ITEM_OPTIONS")
	for (i = 0; i<4; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Item_Options(id,key){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_Item_Options",id)
	#endif

	switch (key){
		case 0:	menu_Shopmenu_One(id)
		case 1:	menu_Shopmenu_Two(id)
		case 2:	MOTD_Itemsinfo(id)
		case 3:	MOTD_Itemsinfo2(id)
		case 8: menu_War3menu(id)
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_Admin_Options(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_Admin_Options",id)
	#endif

	if (!(get_user_flags(id)&ADMIN_LEVEL_WC3)) { 
		if(id != 0){
			client_print(id,print_center,"%s %L",g_MODclient, id,"YOU_HAVE_NO_ACCESS")
			return PLUGIN_HANDLED
		}
	}

	new pos = 0, i, menu_body[512], menu_items[3][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)

	format(menu_items[0],31,"%L",id,"GIVE_IND_XP")
	format(menu_items[1],31,"%L",id,"GIVE_MULT_XP")
	format(menu_items[2],31,"%L",id,"SAVE_ALL_XP")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_ADMIN_MENU")
	for (i = 0; i<3; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Admin_Options(id,key){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_Admin_Options",id)
	#endif

	switch (key){
		case 0:{
			g_menuOption[id] = 1
			g_menuSettings[id] = 50
			menu_PlayerXP_Options(id,g_menuPosition[id] = 0)
		}
		case 1:{
			g_menuOption[id] = 1
			g_menuSettings[id] = 50
			menu_TeamXP_Options(id)
		}
		case 2: XP_Save_All()
		case 8: menu_War3menu(id)
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_PlayerXP_Options(id,pos){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_PlayerXP_Options",id)
	#endif

	if (pos < 0){
		menu_Admin_Options(id)
		return PLUGIN_CONTINUE
	}

	get_players(g_menuPlayers[id],g_menuPlayersNum[id])
	new menuBody[512]
	new b = 0
	new i
	new name[32], team[4], title[128], back[16], exitstring[16]
	new start = pos * 7
	if (start >= g_menuPlayersNum[id])
		start = pos = g_menuPosition[id] = 0
	format(title,127,"%L",id,"MENU_GIVE_PLAYERS_XP")
	new len = format(menuBody,511, "%s\R%d/%d^n\w^n",title,pos+1,(g_menuPlayersNum[id] / 7 + ((g_menuPlayersNum[id] % 7) ? 1 : 0 )))
	new end = start + 7
	new keys = (1<<9)|(1<<7)

	if (end > g_menuPlayersNum[id])
		end = g_menuPlayersNum[id]

	for(new a = start; a < end; ++a){
		i = g_menuPlayers[id][a]
		get_user_name(i,name,31)
		get_user_team(i,team,3)
		keys |= (1<<b)
		len += format(menuBody[len],511-len,"\w%d. %s^n\w",++b,name)
	}

	format(title,127,"%L",id,"GIVE")
	len += format(menuBody[len],511-len,"^n8. %s  %d XP^n",title,g_menuSettings[id])

	format(back,15,"%L",id,"BACK_STRING")

	if (end != g_menuPlayersNum[id]){
		format(menuBody[len],511-len,"^n9. %L...^n0. %s", id,"MORE_STRING", pos ? back : back)
		keys |= (1<<8)
	}
	else{
		format(exitstring,15,"%L",id,"EXIT_STRING")
		format(menuBody[len],511-len,"^n0. %s", pos ? back : exitstring)
	}


	show_menu(id,keys,menuBody,-1)
	return PLUGIN_CONTINUE

}

public _menu_PlayerXP_Options(id,key){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_PlayerXP_Options",id)
	#endif

	switch(key){
		case 7:{
			++g_menuOption[id]
			if (g_menuOption[id]>6){
				g_menuOption[id]=1
			}
			switch(g_menuOption[id]){
				case 1: g_menuSettings[id] = 50
				case 2: g_menuSettings[id] = 100
				case 3: g_menuSettings[id] = 500
				case 4: g_menuSettings[id] = 1000
				case 5: g_menuSettings[id] = 5000
				case 6: g_menuSettings[id] = 10000
			}
			menu_PlayerXP_Options(id,g_menuPosition[id])
		}
		case 8: menu_PlayerXP_Options(id,++g_menuPosition[id])
		case 9: return PLUGIN_HANDLED
		default:{
			new player = g_menuPlayers[id][g_menuPosition[id] * 7 + key]
			client_print(player,print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_JUST_GAVE_YOU_XP",g_menuSettings[id])
			p_data[player][P_XP] += g_menuSettings[id]

			WAR3_Display_Level(player,DISPLAYLEVEL_SHOWGAINED) 

			menu_PlayerXP_Options(id,g_menuPosition[id])
		}
	}
	return PLUGIN_HANDLED
}

public menu_TeamXP_Options(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("menu_TeamXP_Options",id)
	#endif

	new pos = 0, i, menu_body[512], menu_items[3][32], give[16]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<7)|(1<<8)|(1<<9)

	format(menu_items[0],31,"%L",id,"TERRORISTS")
	format(menu_items[1],31,"%L",id,"CT")
	format(menu_items[2],31,"%L",id,"EVERYONE")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_TEAM_XP")
	for (i = 0; i<3; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menu_items[i])
	}
	format(give,15,"%L",id,"GIVE")
	pos += format(menu_body[pos], 511-pos,"^n8. %s  %d XP^n",give,g_menuSettings[id])
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_TeamXP_Options(id,key){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_TeamXP_Options",id)
	#endif

	switch(key){
		case 0:{
			server_cmd("amx_givexp @TERRORIST %d",g_menuSettings[id])
			menu_TeamXP_Options(id)
		}
		case 1:{
			server_cmd("amx_givexp @CT %d",g_menuSettings[id])
			menu_TeamXP_Options(id)
		}
		case 2:{
			server_cmd("amx_givexp @ALL %d",g_menuSettings[id])
			menu_TeamXP_Options(id)
		}
		case 7:{
			++g_menuOption[id]
			if (g_menuOption[id]>6){
				g_menuOption[id]=1
			}
			switch(g_menuOption[id]){
				case 1: g_menuSettings[id] = 50
				case 2: g_menuSettings[id] = 100
				case 3: g_menuSettings[id] = 500
				case 4: g_menuSettings[id] = 1000
				case 5: g_menuSettings[id] = 5000
				case 6: g_menuSettings[id] = 10000
			}
			menu_TeamXP_Options(id)
		}
		case 8: menu_Admin_Options(id)
		case 9: return PLUGIN_HANDLED
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}
