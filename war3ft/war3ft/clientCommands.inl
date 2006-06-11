#define ULT_SEARCHTIME		50			// Translates to 5 seconds

// This isn't actually called when they join spectator :/
public cmd_Teamselect( id, key )
{
	p_data_b[id][PB_CHANGINGTEAM] = true;
}

public cmd_Jointeam(id)
{
	p_data_b[id][PB_CHANGINGTEAM] = true;
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
	if ( p_data_b[id][PB_ULTIMATEUSED] || p_data[id][P_ULTIMATEDELAY] > 0 )
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

			// Flash the user's ultimate icon
			ULT_Icon( id, ICON_FLASH );

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
		HU_ULT_Blink( id );
	}

	// Chain Lightning
	else if ( Verify_Skill(id, RACE_ORC, SKILL4) && !p_data_b[id][PB_ISSEARCHING] ){
		p_data_b[id][PB_ISSEARCHING] = true;

		// Don't continue if task already exists...
		if ( !task_exists( TASK_ULTPING + id ) )
		{

			new parm[2]
			parm[0] = id
			parm[1] = 5
			_ULT_Ping( parm );
		}
	}

	// Entangling Roots
	else if ( Verify_Skill(id, RACE_ELF, SKILL4) && !p_data_b[id][PB_ISSEARCHING] ){
		p_data_b[id][PB_ISSEARCHING] = true;

		// Don't continue if task already exists...
		if ( !task_exists( TASK_ULTPING + id ) )
		{
			new parm[2]
			parm[0] = id
			parm[1] = 5
			_ULT_Ping( parm );
		}
	}

	// Immolate
	else if ( Verify_Skill(id, RACE_BLOOD, SKILL4) ){
		p_data_b[id][PB_ISSEARCHING] = true;

		// Don't continue if task already exists...
		if ( !task_exists( TASK_ULTPING + id ) )
		{
			new parm[2]
			parm[0] = id
			parm[1] = 5
			_ULT_Ping( parm );
		}
	}

	// Big Bad Voodoo
	else if ( Verify_Skill(id, RACE_SHADOW, SKILL4) ){
		SH_Ult_BigBadVoodoo( id );
	}

	// Vengeance
	else if ( Verify_Skill(id, RACE_WARDEN, SKILL4) )
	{
		WA_ULT_Vengeance( id );
	}

	// Locust Swarm
	else if ( Verify_Skill(id, RACE_CRYPT, SKILL4) ){
		#if ADVANCED_STATS
			new WEAPON = CSW_LOCUSTS - CSW_WAR3_MIN
			iStatsShots[id][WEAPON]++
		#endif
		CL_ULT_LocustSwarm( id );
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
