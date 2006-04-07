
public client_score(index,score,total){
	#if ADVANCED_DEBUG
		writeDebugInfo("client_score", index)
	#endif
	if (!warcraft3)
		return PLUGIN_CONTINUE

	// Award the user money
	set_user_money( index, get_user_money(index)+(score*1000), 1 );

	// Award the user XP
	new iXP, iXPAwarded;
	iXP = score * xpgiven[p_data[index][P_LEVEL]];
	iXPAwarded = XP_give( index, iXP );
	if (get_pcvar_num( XP_Show_Objectives ))
	{
		client_print(index, print_chat, "%s %L", g_MODclient, index, "DOD_AWARDED_XP_OBJECTIVE", iXPAwarded);
	}

	return PLUGIN_CONTINUE
}

public _DOD_showMoney(parm[3]){
/*	#if ADVANCED_DEBUG
		writeDebugInfo("_DOD_showMoney",parm[0])
	#endif*/

	if (!warcraft3)
		return PLUGIN_CONTINUE
	
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
	 #if ADVANCED_DEBUG
		writeDebugInfo("on_EndRound",0)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE;
	
	// The round has ended
	endround = true;

	// Allies = 3, Axis = 4
	new winner = read_data(1);
	new players[32], numberofplayers, a, id, iXP, iXPAwarded;
	get_players( players, numberofplayers );
	
	for ( a = 0; a < numberofplayers; ++a )
	{
		id = players[a];
		p_data_b[id][PB_REINCARNATION_DELAY] = false;

		// Skip reincarnation for the start of the next round
		if( Verify_Skill(id, RACE_ORC, SKILL3) || p_data[id][P_ITEM] == ITEM_ANKH )
		{
			p_data_b[id][PB_REINCARNATION_SKIP] = true;
		}

		// Should we award XP ?
		new iTeam = get_user_team( id );
		if ( (iTeam == ALLIES && winner == 3) || (iTeam == AXIS && winner == 4) )
		{
			iXP = get_pcvar_num(XP_Win_Round) + xpgiven[p_data[id][P_LEVEL]];
			iXPAwarded = XP_give(id, iXP);
			if (get_pcvar_num( XP_Show_Objectives ))
			{
				client_print(id,print_chat, "%s %L", g_MODclient, id, "AWARD_FOR_WINNING_ROUND", iXPAwarded);
			}
		}
	}

	return PLUGIN_CONTINUE
}
// DOD Specific stocks
stock Create_HudText(id,message[], flag)
{
#if ADVANCED_DEBUG
	writeDebugInfo("Create_HudText",id)
#endif

	message_begin( MSG_ONE, gmsgHudText, {0,0,0}, id)
	write_string(message)
	write_byte(flag)					// 1 = Show
	message_end()
}

stock Create_DeathMsg_DOD(killer_id,victim_id,weapon)
{
	#if ADVANCED_DEBUG
		writeDebugInfo("Create_DeathMsg_DOD",weapon)
	#endif

	message_begin(MSG_ALL,gmsgDeathMsg)
	write_byte(killer_id)
	write_byte(victim_id)
	write_byte(weapon)
	message_end()
}

public on_StatusValue(id)
{	
	if(!warcraft3 || !iCvar[FT_SPEC_INFO] || !p_data_b[id][PB_ISCONNECTED])
	{
		return PLUGIN_CONTINUE;
	}

	new targetid = entity_get_int(id, EV_INT_iuser2);

	if( targetid == 0 )
	{
		return PLUGIN_CONTINUE;
	}

	WAR3_Show_Spectator_Info(id, targetid);

	return PLUGIN_CONTINUE;
}