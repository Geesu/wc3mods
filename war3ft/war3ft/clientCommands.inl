public change_race(id,saychat){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	WAR3_chooserace(id)

	return PLUGIN_CONTINUE
}

public cmd_Teamselect(id,key) {

	// key+1 is the team they choose
	p_data_b[id][PB_CHANGINGTEAM] = true

	if(!p_data_b[id][PB_JUSTJOINED] && !is_user_alive(id))
		p_data_b[id][PB_DIEDLASTROUND] = true

}

public cmd_Jointeam(id){

/*	new szTeam[4]
	read_argv(1,szTeam,3)

	if ( str_to_num(szTeam) == 1 || str_to_num(szTeam) == 2 || str_to_num(szTeam) == 5 )
		p_data_b[id][PB_CHANGINGTEAM] = true*/

	if(!p_data_b[id][PB_JUSTJOINED] && !is_user_alive(id))
		p_data_b[id][PB_DIEDLASTROUND] = true

	return PLUGIN_CONTINUE
}

public cmd_Level(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	WAR3_Display_Level(id, DISPLAYLEVEL_SHOWRACECHAT)

	return PLUGIN_HANDLED
}

public cmd_Say(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

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
		menu_ResetXP(id);
	else if (equali(said,"^"/itemsinfo^"") || equali(said,"^"itemsinfo^""))
		MOTD_Itemsinfo(id)
	else if (equali(said,"^"/war3menu^"") || equali(said,"^"war3menu^""))
		menu_War3menu(id)
	else if (equali(said,"^"/savexp^"") || equali(said,"^"savexp^""))
       client_print( id, print_chat, "%s XP is saved automatically, you do not need to type this command", g_MODclient );
	else if (equali(said,"^"/resetskills^"") || equali(said,"^"resetskills^""))
		cmd_ResetSkill(id, 1);
	else if (equali(said,"^"/geesu^"") || equali(said,"^"/pimpdaddy^"") || equali(said,"^"/ootoaoo^""))
		WAR3_Check_Dev(id)

	if(get_pcvar_num( CVAR_wc3_races ) > 4){
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

	if (!warcraft3)
		return PLUGIN_CONTINUE

	set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 1)

	if (!g_spritesEnabled){
		show_hudmessage(id,"%L",id,"ICONS_ARE_DISABLED")
		client_print(id,print_chat,"%s %L",g_MODclient,id,"ICONS_ARE_DISABLED_DO")
		return PLUGIN_CONTINUE
	}

	if( get_pcvar_num( CVAR_wc3_race_icon ) || get_pcvar_num( CVAR_wc3_level_icon ) ){
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

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(!get_pcvar_num( CVAR_wc3_buy_dead ) && !is_user_alive(id)){
		client_print(id,print_center,"%L",id,"NOT_BUY_ITEMS_WHEN_DEAD")
		return PLUGIN_HANDLED
	}
	#if MOD == 0
		else if(get_pcvar_num( CVAR_wc3_buy_time ) && !g_buyTime){
			new Float:thetime = get_cvar_float("mp_buytime")*60.0
			client_print(id,print_center,"%L",id,"SECONDS_HAVE_PASSED_CANT_BUY",thetime)
			return PLUGIN_HANDLED
		}
		else if(get_pcvar_num( CVAR_wc3_buy_zone ) && !cs_get_user_buyzone(id) && is_user_alive(id)){
			client_print(id,print_center,"%L",id,"MUST_BE_IN_BUYZONE")
			return PLUGIN_HANDLED
		}
	#endif

	if(get_pcvar_num( CVAR_wc3_races )<5)
		return PLUGIN_CONTINUE

	new usermoney
	new parm[2]
	parm[0]=id

	if (p_data[id][P_ITEM2]==ITEM_AMULET)
		p_data_b[id][PB_SILENT] = false
	if (p_data[id][P_ITEM2]==ITEM_HELM)
		p_data_b[id][PB_IMMUNE_HEADSHOTS] = false;
	if (p_data[id][P_ITEM2]==ITEM_CHAMELEON)
		changeskin(id,SKIN_RESET)
	
	while(p_data[id][P_RINGS]<5){
		usermoney = SHARED_GetUserMoney(id)
		if(usermoney<itemcost2[ITEM_RING-1])
			break
		SHARED_SetUserMoney(id,usermoney-itemcost2[ITEM_RING-1],1)
		++p_data[id][P_RINGS]
		p_data[id][P_ITEM2]=ITEM_RING
		if(!task_exists(TASK_ITEM_RINGERATE+id))
			_Item_Ring(parm)
	}

	WAR3_Display_Level(id,DISPLAYLEVEL_NONE)

	return PLUGIN_HANDLED
}

public cmd_fullupdate(){

	return PLUGIN_HANDLED
}

public cmd_ability(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(get_pcvar_num( CVAR_wc3_races ) < 5)
		return PLUGIN_CONTINUE
	
	if ( p_data_b[id][PB_HEXED] )
	{
		WC3_Status_Text( id, 4.0, HUDMESSAGE_POS_INFO, "%L", id, "HEX_NO_ABILITY" );
		return PLUGIN_HANDLED;
	}

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

			WC3_Status_Text( id, 3.5, HUDMESSAGE_POS_INFO, "%L", id, "SERPENT_WARD", p_data[id][P_SERPENTCOUNT] );
		}
	}
	return PLUGIN_HANDLED
}

#if MOD == 0
	public cmd_hegren(id){ 

		if (!warcraft3)
			return PLUGIN_CONTINUE

		if(get_pcvar_num( CVAR_wc3_grenade_protection )==0)
			return PLUGIN_CONTINUE

		if(!cs_get_user_buyzone(id))
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

		if (!warcraft3)
			return PLUGIN_CONTINUE

		if(!cs_get_user_buyzone(id))
			return PLUGIN_HANDLED

		return PLUGIN_CONTINUE
	} 
#endif

public cmd_ResetSkill(id,saychat){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(saychat==1){
		client_print(id,print_center,"%s %L",g_MODclient, id,"SKILLS_RESET_NEXT_ROUND")
	}
	else{
		console_print(id,"%L",id,"SKILLS_RESET_NEXT_ROUND")
	}
	p_data_b[id][PB_RESETSKILLS]=true


	return PLUGIN_HANDLED
}

public cmd_Ultimate(id)
{

	if ( !warcraft3 )
		return PLUGIN_CONTINUE

	if (!is_user_alive(id))
		return PLUGIN_HANDLED

	if ( p_data_b[id][PB_HEXED] )
	{
		WC3_Status_Text( id, 3.0, HUDMESSAGE_POS_STATUS, "%L", id, "HEX_NO_ABILITY" );

		client_cmd(id, "speak warcraft3/bonus/Error.wav")

		return PLUGIN_HANDLED
	}

	if(!p_data[id][P_ULTIMATE])
	{
		WC3_Status_Text( id, 0.5, HUDMESSAGE_POS_STATUS, "%L", id, "ULTIMATE_NOT_FOUND" );

		client_cmd(id, "speak warcraft3/bonus/Error.wav")

		return PLUGIN_HANDLED
	}

	if(p_data_b[id][PB_ULTIMATEUSED])
	{
		WC3_Status_Text( id, 0.5, HUDMESSAGE_POS_STATUS, "%L", id, "ULTIMATE_NOT_READY", iUltimateDelay );

		client_cmd(id, "speak warcraft3/bonus/Error.wav")

		return PLUGIN_HANDLED
	}

	if ( iUltimateDelay > 0 )
	{
		WC3_Status_Text( id, 0.5, HUDMESSAGE_POS_STATUS, "%L", id, "ULTIMATE_NOT_READY", iUltimateDelay );

		client_cmd(id, "speak warcraft3/bonus/Error.wav")

		return PLUGIN_HANDLED
	}

	// Suicide Bomber
	if ( Verify_Skill(id, RACE_UNDEAD, SKILL4) ){
		if( p_data_b[id][PB_SUICIDEATTEMPT] ){

			WAR3_Kill(id, 0)
		
			p_data_b[id][PB_SUICIDEATTEMPT] = false
		}
		else{
			new parm[1]
			parm[0]=id
			Ultimate_Icon(id,ICON_FLASH)
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

	// Blink
	else if ( Verify_Skill(id, RACE_HUMAN, SKILL4) ){
		Ultimate_Blink(id)
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
		#if ADVANCED_STATS
			new WEAPON = CSW_FLAME - CSW_WAR3_MIN
			iStatsShots[id][WEAPON]++
		#endif
		Ultimate_FlameStrike(id) 
		p_data[id][P_FLAMECOUNT]++
		if(p_data[id][P_FLAMECOUNT]>5){
			p_data_b[id][PB_ULTIMATEUSED]=true
			Ultimate_Icon(id,ICON_HIDE)
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
		#if ADVANCED_STATS
			new WEAPON = CSW_LOCUSTS - CSW_WAR3_MIN
			iStatsShots[id][WEAPON]++
		#endif
		Ultimate_LocustSwarm(id)
	}

	return PLUGIN_HANDLED
}
