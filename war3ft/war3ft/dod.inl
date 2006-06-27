
public EVENT_DOD_EndRound()
{
	
	if ( !WAR3_Check() )
	{
		return;
	}
	
	// The round has ended
	g_EndRound = true;

	// Allies = 3, Axis = 4
	new iWinner = read_data( 1 );
	new iTeam = -1;

	// Allies won
	if ( iWinner == 3 )
	{
		iTeam = ALLIES;
	}

	// Axis won!
	else if ( iWinner == 4 )
	{
		iTeam = AXIS;
	}

	XP_WinRound( iTeam );

	return;
}


public client_score(index,score,total){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	// Award the user money
	SHARED_SetUserMoney( index, SHARED_GetUserMoney(index)+(score*1000), 1 );

	// Award the user XP
	new iXP, iXPAwarded;
	iXP = score * xpgiven[p_data[index][P_LEVEL]];
	iXPAwarded = XP_Give( index, iXP );
	if (get_pcvar_num( CVAR_wc3_show_objectives ))
	{
		client_print(index, print_chat, "%s %L", g_MODclient, index, "DOD_AWARDED_XP_OBJECTIVE", iXPAwarded);
	}

	return PLUGIN_CONTINUE
}

public _DOD_showMoney(parm[3]){

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

public on_StatusValue(id)
{	
	if(!warcraft3 || !SHOW_SPECTATE_INFO || !p_data_b[id][PB_ISCONNECTED])
	{
		return PLUGIN_CONTINUE;
	}

	new targetid = entity_get_int(id, EV_INT_iuser2);

	if( targetid == 0 )
	{
		return PLUGIN_CONTINUE;
	}

	WC3_ShowSpecInfo(id, targetid);

	return PLUGIN_CONTINUE;
}

public client_damage( attacker, victim, damage, wpnindex, hitplace, TA )
{

	if ( !WAR3_Check() )
	{
		return;
	}

	if ( g_MOD != GAME_DOD )
	{
		return;
	}


	EVENT_Damage( victim, attacker, damage, wpnindex, hitplace );

	return;
}
