public change_race(id,saychat){
	 #if ADVANCED_DEBUG == 1
		writeDebugInfo("change_race",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_CONTINUE
		}
	}

	XP_Client_Save(id,0)

	WAR3_chooserace(id)

	return PLUGIN_CONTINUE
}

public cmd_Teamselect(id,key) {
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("cmd_Teamselect",id)
	#endif

	// key+1 is the team they choose
	p_data_b[id][PB_CHANGINGTEAM] = true
	if(!p_data_b[id][PB_JUSTJOINED])
		p_data_b[id][PB_DIEDLASTROUND] = true

}

public cmd_Jointeam(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("cmd_Jointeam",id)
	#endif

	//new arg[2]
	//read_argv(1,arg,1)	//This is the command they pressed
	p_data_b[id][PB_CHANGINGTEAM] = true

	if(!p_data_b[id][PB_JUSTJOINED])
		p_data_b[id][PB_DIEDLASTROUND] = true
}

public cmd_Level(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("cmd_Level",id)
	#endif

	WAR3_Display_Level(id, DISPLAYLEVEL_SHOWRACECHAT)
}

public cmd_Say(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("cmd_Say",id)
	#endif

	new said[32]
	read_args(said,31) 

	if (equali(said,"^"/changerace^"") || equali(said,"^"changerace^""))
		change_race(id,1)
	else if (equali(said,"^"/selectskill^"") || equali(said,"^"selectskill^""))
		menu_Select_Skill(id,1)
	else if (equali(said,"^"/playerskills^"") || equali(said,"^"playerskills^""))
		MOTD_Playerskills(id, 1)
	else if (equali(said,"^"/skillsinfo^"") || equali(said,"^"skillsinfo^""))
		MOTD_Skillsinfo(id)
	else if (equali(said,"^"/war3help^"") || equali(said,"^"war3help^""))
		MOTD_War3help(id)
	else if (equali(said,"^"/icons^"") || equali(said,"^"icons^""))
		say_Icons(id)
	else if (equali(said,"^"/level^"") || equali(said,"^"level^""))
		WAR3_Display_Level(id,DISPLAYLEVEL_SHOWRACECHAT)
	else if (equali(said,"^"/shopmenu^"") || equali(said,"^"shopmenu^""))
		menu_Shopmenu_One(id)
	else if (equali(said,"^"/resetxp^"") || equali(said,"^"resetxp^""))
		amx_resetxp(id,1)
	else if (equali(said,"^"/itemsinfo^"") || equali(said,"^"itemsinfo^""))
		MOTD_Itemsinfo(id)
	else if (equali(said,"^"/war3menu^"") || equali(said,"^"war3menu^""))
		menu_War3menu(id)
	else if (equali(said,"^"/savexp^"") || equali(said,"^"savexp^""))
       	XP_Client_Save(id,1)  
	else if (equali(said,"^"/resetskills^"") || equali(said,"^"resetskills^""))
		cmd_ResetSkill(id,1)

	if(iCvar[FT_RACES] > 4){
		if (equali(said,"^"/itemsinfo2^"") || equali(said,"^"itemsinfo2^""))
			MOTD_Itemsinfo2(id)
		else if (equali(said,"^"/rings^"") || equali(said,"^"rings^""))
			cmd_Rings(id)
		else if (equali(said,"^"/ability^"") || equali(said,"^"ability^""))
			cmd_ability(id)
		else if (equali(said,"^"/shopmenu2^"") || equali(said,"^"shopmenu2^""))
			menu_Shopmenu_Two(id)
	}

	return PLUGIN_CONTINUE
}

public say_Icons(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("say_Icons",id)
	#endif

	set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 1)

	if (!g_spritesEnabled){
		show_hudmessage(id,"%L",id,"ICONS_ARE_DISABLED")
		client_print(id,print_chat,"%s %L",g_MODclient,id,"ICONS_ARE_DISABLED_DO")
		return PLUGIN_CONTINUE
	}

	if(iCvar[FT_RACE_ICONS] || iCvar[FT_LEVEL_ICONS]){
		if(p_data[id][P_SHOWICONS]){
			p_data[id][P_SHOWICONS]=false

			show_hudmessage(id,"%L",id,"NO_LONGER_SEE_ICONS")
		}
		else{
			p_data[id][P_SHOWICONS]=true

			show_hudmessage(id,"%L",id,"NOW_SEE_ICONS")
		}
	}
	else
		show_hudmessage(id,"%L",id,"ICONS_DISABLED_THIS_SERVER")

	return PLUGIN_HANDLED
}

public cmd_Rings(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("cmd_Rings",id)
	#endif

	if (!is_user_alive(id))
		return PLUGIN_HANDLED

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			return PLUGIN_CONTINUE
		}
	}

	if(iCvar[FT_RACES]<5)
		return PLUGIN_CONTINUE

	new usermoney
	new parm[2]
	parm[0]=id

	if (p_data[id][P_ITEM2]==ITEM_AMULET)
		p_data_b[id][PB_SILENT] = false
	if (p_data[id][P_ITEM2]==ITEM_HELM)
		Item_Set_Helm(id,1)
	if (p_data[id][P_ITEM2]==ITEM_CHAMELEON)
		changeskin(id,SKIN_RESET)
	
	while(p_data[id][P_RINGS]<5){
		usermoney = get_user_money(id)
		if(usermoney<itemcost2[ITEM_RING-1])
			break
		set_user_money(id,usermoney-itemcost2[ITEM_RING-1],1)
		++p_data[id][P_RINGS]
		p_data[id][P_ITEM2]=ITEM_RING
		if(!task_exists(TASK_ITEM_RINGERATE+id))
			_Item_Ring(parm)
	}

	WAR3_Display_Level(id,DISPLAYLEVEL_NONE)

	return PLUGIN_HANDLED
}

public cmd_fullupdate(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("cmd_fullupdate",0)
	#endif

	return PLUGIN_HANDLED
}

public cmd_ability(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("ability",id)
	#endif

	if(iCvar[FT_RACES] < 5)
		return PLUGIN_CONTINUE

	if(is_user_alive(id)){
		if ( Verify_Skill(id, RACE_SHADOW, SKILL3) && p_data[id][P_SERPENTCOUNT]>0 && !endround){	 //Serpent Ward

			new parm[5], origin[3]

			get_user_origin(id,origin)
			parm[0]=origin[0]
			parm[1]=origin[1]
			parm[2]=origin[2]
			parm[3]=id
			parm[4]=get_user_team(id)

			_Skill_SerpentWard(parm)
			p_data[id][P_SERPENTCOUNT]--

			new message[128]
			format(message, 127,"%L",id,"SERPENT_WARD", p_data[id][P_SERPENTCOUNT])

			Status_Text(id, message, 3.5, HUDMESSAGE_POS_INFO)

			//set_hudmessage(200, 100, 0, 0.2, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
			//show_hudmessage(id,"%L",id,"SERPENT_WARD", p_data[id][P_SERPENTCOUNT])
		}
	}
	return PLUGIN_HANDLED
}

#if MOD == 0
	public cmd_Shield(id){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("cmd_Shield",id)
		#endif

		if (!warcraft3)
			return PLUGIN_HANDLED

		if(!p_data_b[id][PB_ISCONNECTED])
			return PLUGIN_HANDLED

		if(!p_data_b[id][PB_BUYZONE])
			return PLUGIN_HANDLED

		new parm[1]
		parm[0] = id
		set_task(0.1,"_cmd_Shield",TASK_SHIELD+id,parm,1)
		
		return PLUGIN_CONTINUE
	}

	public _cmd_Shield(parm[1]){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("_cmd_Shield",parm[0])
		#endif

		new id = parm[0]

		if(!p_data_b[id][PB_ISCONNECTED])
			return PLUGIN_CONTINUE

		new seq = entity_get_int(id,EV_INT_sequence)
		
		if(seq == 89){
			p_data_b[id][PB_SHIELD] = true
		}
		else if(seq == 94){
			p_data_b[id][PB_SHIELD] = true
		}
		else if(seq == 98){
			p_data_b[id][PB_SHIELD] = true
		}
		else{
			p_data_b[id][PB_SHIELD] = false
		}

		return PLUGIN_CONTINUE
	}

	public cmd_Drop(id) { 
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("cmd_Drop",id)
		#endif

		if (!warcraft3)
			return PLUGIN_CONTINUE

		cmd_Shield(id)
		
		return PLUGIN_CONTINUE 
	}

	public cmd_hegren(id){ 
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("cmd_hegren",id)
		#endif

		if(iCvar[MP_GRENADEPROTECTION]==0)
			return PLUGIN_CONTINUE

		if(!p_data_b[id][PB_BUYZONE])
			return PLUGIN_HANDLED

		if (p_data[id][P_HECOUNT]>0){ 
			client_print(id,print_center,"%L",id,"ONLY_ONE_GRENADE_PER_ROUND") 
			return PLUGIN_HANDLED
		} 
		else{
			++p_data[id][P_HECOUNT]
			return PLUGIN_CONTINUE

		}

		return PLUGIN_HANDLED 
	} 

	public cmd_flash(id){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("cmd_flash",id)
		#endif
		if(!p_data_b[id][PB_BUYZONE])
			return PLUGIN_HANDLED

		return PLUGIN_CONTINUE
	} 
#endif

public cmd_ResetSkill(id,saychat){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("cmd_ResetSkill",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%s %L",g_MODclient, id,"CHEATING_DEATH_NOT_INSTALLED")
			return PLUGIN_CONTINUE
		}
	}

	if(saychat==1){
		client_print(id,print_center,"%L",id,"SKILLS_RESET_NEXT_ROUND")
	}
	else{
		console_print(id,"%L",id,"SKILLS_RESET_NEXT_ROUND")
	}
	p_data_b[id][PB_RESETSKILLS]=true


	return PLUGIN_HANDLED
}

public cmd_Ultimate(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("ultimate",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_HANDLED
		}
	}

	if (!is_user_alive(id))
		return PLUGIN_HANDLED

	if(!p_data[id][P_ULTIMATE]){
		new message[128]
		format(message, 127, "%L",id,"ULTIMATE_NOT_FOUND")
		Status_Text(id, message, 0.5, HUDMESSAGE_POS_STATUS)
		client_cmd(id, "speak warcraft3/bonus/Error.wav")
		return PLUGIN_HANDLED
	}

	if(p_data_b[id][PB_ULTIMATEUSED]){
		new message[128]
		format(message, 127, "%L",id,"ULTIMATE_NOT_READY",p_data[id][P_ULTIMATEDELAY])
		Status_Text(id, message, 0.5, HUDMESSAGE_POS_STATUS)
		client_cmd(id, "speak warcraft3/bonus/Error.wav")
		return PLUGIN_HANDLED
	}

	// Suicide Bomber
	if ( Verify_Skill(id, RACE_UNDEAD, SKILL4) ){
		if (iCvar[FT_WARN_SUICIDE]){
			if( p_data_b[id][PB_SUICIDEATTEMPT] ){
				set_msg_block(gmsgDeathMsg,BLOCK_ONCE)
			#if MOD == 0
				user_kill(id)
			#endif
			#if MOD == 1
				dod_user_kill(id)
				on_Death(id, 0, 0, 0)
				//client_cmd(id,"kill")
			#endif
			
				p_data_b[id][PB_SUICIDEATTEMPT] = false
			}
			else{
				new parm[1]
				parm[0]=id
				icon_controller(id,ICON_FLASH)
				p_data_b[id][PB_SUICIDEATTEMPT] = true
			#if MOD == 0
				set_hudmessage(178, 14, 41, -1.0, -0.4, 1, 0.5, 1.7, 0.2, 0.2,5)
				show_hudmessage(id,"%L",id,"SUICIDE_BOMB_ARMED")
			#endif
			#if MOD == 1
				new szMessage[128]
				format(szMessage, 127, "%L", id, "SUICIDE_BOMB_ARMED")
				Create_HudText(id, szMessage, 1)
			#endif
			}
		}
		else{
			set_msg_block(gmsgDeathMsg,BLOCK_ONCE)
		#if MOD == 0
			user_kill(id)
		#endif
		#if MOD == 1
			//client_cmd(id,"kill")
			dod_user_kill(id)
			on_Death(id, 0, 0, 0)
		#endif

		}
	}

	// Teleport
	else if ( Verify_Skill(id, RACE_HUMAN, SKILL4) ){
		Ultimate_Teleport(id)
	}

	// Chain Lightning
	else if ( Verify_Skill(id, RACE_ORC, SKILL4) && !p_data_b[id][PB_ISSEARCHING] ){
		new parm[2]
		parm[0]=id
		parm[1]=ULTIMATESEARCHTIME
		lightsearchtarget(parm)
	}

	// Entangling Roots
	else if ( Verify_Skill(id, RACE_ELF, SKILL4) && !p_data_b[id][PB_ISSEARCHING] ){
		new parm[2]
		parm[0]=id
		parm[1]=ULTIMATESEARCHTIME
		searchtarget(parm)
	}

	// Flame Strike
	else if ( Verify_Skill(id, RACE_BLOOD, SKILL4) ){
		Ultimate_FlameStrike(id) 
		p_data[id][P_FLAMECOUNT]++
		if(p_data[id][P_FLAMECOUNT]>5){
			p_data_b[id][PB_ULTIMATEUSED]=true
			icon_controller(id,ICON_HIDE)
			p_data[id][P_FLAMECOUNT]=0
		}
	}

	// Big Bad Voodoo
	else if ( Verify_Skill(id, RACE_SHADOW, SKILL4) ){
		new parm[2]
		parm[0] = id
		parm[1] = 1
		_Ultimate_BigBadVoodoo(parm)
	}

	// Vengeance
	else if ( Verify_Skill(id, RACE_WARDEN, SKILL4) ){
		Ultimate_Vengeance(id)
	}

	// Locust Swarm
	else if ( Verify_Skill(id, RACE_CRYPT, SKILL4) ){
		Ultimate_LocustSwarm(id)
	}

	return PLUGIN_HANDLED
}
