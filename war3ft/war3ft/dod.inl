
public client_score(index,score,total){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("client_score",index)
	#endif
	set_user_money(index,get_user_money(index)+(score*1000),1)
}

public _DOD_showMoney(parm[3]){
/*	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_DOD_showMoney",parm[0])
	#endif*/

	new id = parm[0]
	new Float:timer = 0.7

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(get_user_team(id) == ALLIES || get_user_team(id) == AXIS){
		if(parm[1]==1){		// Should we show the change in money?
			new message[128]
			if(parm[2]<0)
				format(message,127,"$%d^n%d", p_data[id][P_MONEY], parm[2])	// Lost money
			else
				format(message,127,"$%d^n+%d", p_data[id][P_MONEY], parm[2])	// Gained money

			set_hudmessage(255,255,255,0.5,0.0,2, 1.5, 40.0, 0.1, 0.5, HUDMESSAGE_CHAN_MONEY)
			show_hudmessage(id,message)
			if(task_exists(TASK_MONEYLOOP+id))
				remove_task(TASK_MONEYLOOP+id)
			
			timer = 2.0
		}
		else{
			new message[128]
			format(message,127,"$%d", p_data[id][P_MONEY])
			set_hudmessage(255,255,255,0.5,0.0,2, 1.5, 40.0, 0.1, 0.5, HUDMESSAGE_CHAN_MONEY)
			show_hudmessage(id,message)
		}
	}

	parm[1] = 0
	parm[2] = 0
	set_task(timer, "_DOD_showMoney", TASK_MONEYLOOP+id, parm, 3)

	return PLUGIN_CONTINUE
}

public on_EndRound(){
	 #if ADVANCED_DEBUG == 1
		writeDebugInfo("on_EndRound",0)
	#endif

	new winner = read_data(1)		// Allies = 3, Axis = 4

	endround = true

	new players[32], numberofplayers, a, id
	new bool:give

	get_players(players,numberofplayers)
	
	for (a = 0; a < numberofplayers; ++a){
		id = players[a]
		give = false
	
		// Skip reincarnation
		if( Verify_Skill(id, RACE_ORC, SKILL3) || p_data[id][P_ITEM] == ITEM_ANKH )
			p_data_b[id][PB_REINCARNATION_SKIP] = true


		// Run a check to award experience

		if(get_user_team(id) == ALLIES && winner == 3)
			give = true
		else if(get_user_team(id) == AXIS && winner == 4)
			give = true

		if(give){
			new iXP = (iCvar[FT_ROUND_WIN_XP] + xpgiven[p_data[id][P_LEVEL]])
			iXP = XP_give(players[a], iXP)
			if (iCvar[FT_OBJECTIVES])
				client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_WINNING_ROUND",iXP)
		}
	}
}
// DOD Specific stocks
stock Create_HudText(id,message[], flag){
#if ADVANCED_DEBUG == 1
	writeDebugInfo("Create_HudText",id)
#endif

	message_begin( MSG_ONE, gmsgHudText, {0,0,0}, id)
	write_string(message)
	write_byte(flag)					// 1 = Show
	message_end()
}

stock Create_DeathMsg_DOD(killer_id,victim_id,weapon){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("Create_DeathMsg_DOD",weapon)
	#endif

	message_begin(MSG_ALL,gmsgDeathMsg)
	write_byte(killer_id)
	write_byte(victim_id)
	write_byte(weapon)
	message_end()
}

public on_Spectate(){
	new name[32], team[32]
	read_data(4,team,31)
	read_data(3,name,31)

	new id = get_user_index(name)

	if(equali(team,"Spectators")){
		p_data[id][P_SPECMODE] = 1
	}
	else{
		p_data[id][P_SPECMODE] = 0
	}
}

public on_SetSpecMode(id){

	// #Spec_Mode2 = 3rd person
	// #Spec_Mode4 = 1st person
	// #Spec_Mode3 = free look

	new specMode[32]
	read_data(2, specMode, 31)

	// Third Person Spectating
	if( equal(specMode, "#Spec_Mode2") ){
		p_data[id][P_SPECMODE] = 2
	}

	// First Person Spectating
	else if( equal(specMode, "#Spec_Mode4") ){
		p_data[id][P_SPECMODE] = 4
	}
}

public on_StatusValue(id){	

	if((p_data[id][P_SPECMODE] != 4 && p_data[id][P_SPECMODE] != 2) || !warcraft3 || !iCvar[FT_SPEC_INFO])
		return PLUGIN_CONTINUE

	new origin[3], targetorigin[3]
	new players[32], numberofplayers, i
	new targetid = 0
	new name[32]

	get_user_origin(id, origin)

	get_players(players, numberofplayers, "a")

	for (i=0;i<numberofplayers;i++){
		if(players[i] != id ){
			get_user_origin(players[i], targetorigin)
			get_user_name(players[i], name, 31)
			console_print(id, "Player: %s, Distance:%d", name, get_distance(origin, targetorigin))

			if( get_distance(origin, targetorigin) < 50 ){
				targetid = players[i]
				break
			}
		}
	}

	if( targetid == 0 )
		return PLUGIN_CONTINUE

	WAR3_Show_Spectator_Info(id, targetid)

	return PLUGIN_CONTINUE
}