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

public cmd_fullupdate()
{
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

public cmd_Ultimate(id)
{

	if ( !warcraft3 )
		return PLUGIN_CONTINUE

	if (!is_user_alive(id))
		return PLUGIN_HANDLED

	if ( p_data_b[id][PB_HEXED] )
	{
		WC3_Status_Text( id, 3.0, HUDMESSAGE_POS_STATUS, "%L", id, "HEX_NO_ABILITY" );

		client_cmd(id, "speak %s", SOUND_ERROR)

		return PLUGIN_HANDLED
	}

	if(!p_data[id][P_ULTIMATE])
	{
		WC3_Status_Text( id, 0.5, HUDMESSAGE_POS_STATUS, "%L", id, "ULTIMATE_NOT_FOUND" );

		client_cmd(id, "speak %s", SOUND_ERROR)

		return PLUGIN_HANDLED
	}
	
	// Global ultimate delay > 0
	if ( g_iUltimateDelay > 0 )
	{
		WC3_Status_Text( id, 0.5, HUDMESSAGE_POS_STATUS, "%L", id, "ULTIMATE_NOT_READY", g_iUltimateDelay );

		client_cmd(id, "speak %s", SOUND_ERROR)

		return PLUGIN_HANDLED
	}

	// Ultimate is used
	if ( p_data_b[id][PB_ULTIMATEUSED] )
	{
		WC3_Status_Text( id, 0.5, HUDMESSAGE_POS_STATUS, "%L", id, "ULTIMATE_NOT_READY", p_data[id][P_ULTIMATEDELAY] );

		client_cmd(id, "speak %s", SOUND_ERROR)

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
	else if ( Verify_Skill(id, RACE_WARDEN, SKILL4) )
	{
		SH_ULT_Vengeance( id );
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


public CMD_Handler( id )
{

	new szCmd[32];

	read_argv( 0, szCmd, 31 );

	WC3_HandleCommand( id, szCmd );

	return PLUGIN_HANDLED;
}

public cmd_Say( id )
{
	
	if ( !WAR3_Check( id ) )
	{
		return;
	}

	new szSaid[32];
	read_args( szSaid, 31 );

	remove_quotes( szSaid );

	WC3_HandleCommand( id, szSaid );

	return;
}
