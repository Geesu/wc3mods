
// From the HL SDK
#define MAX_WEAPONS		32
// From CSX Module
#define MAX_CWEAPONS	6

public on_SetSpecMode(id) {

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new arg[12] 
	read_data( 2 , arg , 11 ) 
	p_data[id][P_SPECMODE] = ( arg[10] == '2' ) ? 1 : 0 

	return PLUGIN_CONTINUE
}

public on_Spectate(id){

	if(!p_data[id][P_SPECMODE] || !warcraft3 || !SHOW_SPECTATE_INFO)
		return PLUGIN_CONTINUE

	new a = read_data(2)

	if ( is_user_bot(id) )
		return PLUGIN_CONTINUE

	// Not a valid target
	if (!p_data_b[a][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	WC3_ShowSpecInfo(id, a)
	
	return PLUGIN_CONTINUE
}

public on_EndRound(){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	g_EndRound = true;

	//set_task( 4.9, "_EVENT_Before_ResetHUD", TASK_BEFORE_ROUND_START );

	new players[32], numberofplayers
	new y, id
	get_players(players, numberofplayers)

	for (y = 0; y < numberofplayers; ++y){
		id = players[y]

		p_data_b[id][PB_MOLE] = false
		p_data[id][P_DEFUSERINDEX] = 0

	}

	for(y=0;y<33;++y)
		spawnPointsused[y]=false

	g_freezeCalled = false

	if( task_exists(TASK_BUYTIME) )
		remove_task(TASK_BUYTIME)

	if( task_exists(TASK_BOMBTIMER) )
		remove_task(TASK_BOMBTIMER)
	
	// Save XP at the end of the round?
	if ( get_pcvar_num( CVAR_wc3_save_xp ) && get_pcvar_num( CVAR_wc3_save_end_round ) )
	{
		XP_SaveAll();
	}
	
	return PLUGIN_CONTINUE
}

public on_FreezeTimeComplete() {

	if (!warcraft3)
		return PLUGIN_CONTINUE

	g_freezeTime = false
	
	new players[32],numberofplayers, id, i
	get_players(players, numberofplayers)
	
	for (i = 0; i < numberofplayers; ++i){
		id=players[i]
		
		WC3_ShowBar( id );

		set_task( 0.1, "SHARED_SetSpeed", TASK_UNHOLYSPEED + id );
	}

	return PLUGIN_CONTINUE
}

public on_TerroristWin()
{

	if ( !WAR3_Check() )
	{
		return;
	}

	XP_WinRound( TEAM_T );

	return;
}

public on_CTWin()
{
	if ( !WAR3_Check() )
	{
		return;
	}

	XP_WinRound( TEAM_CT );

	return;
}


// Function called when a user's armor changes
public on_Battery( id )
{	
	if ( !WAR3_Check() )
	{
		return;
	}
	
	// Store the previous value (we do this b/c the Battery event is triggered (and set to 0) before weapon reincarnation)
	p_data[id][P_LASTARMOR] = p_data[id][P_ARMORONDEATH];

	p_data[id][P_ARMORONDEATH] = read_data( 1 );

	return;
}

public on_ArmorType(id)
{

	if ( !WAR3_Check() )
	{
		return;
	}
	
	// Save user's armor
	p_data[id][P_ARMORONDEATH] = cs_get_user_armor( id, g_ArmorType[id] );

	return;
}

public on_ShowStatus(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if (g_freezeTime){
		Create_StatusText(id, 0, "")
		return PLUGIN_HANDLED
	}

	new pid = read_data(2)

	if(get_user_team(id)==get_user_team(pid) && (ITEM_Has( id, ITEM_CHAMELEON ) || p_data[pid][P_SKINCHANGED]==SKIN_SWITCH) && is_user_alive(pid)){
		client_print(id,print_center,"%L",id,"HES_ON_YOUR_TEAM_DONT_SHOOT")
		client_cmd(id,"speak %s", g_szSounds[SOUND_ANTEND])
	}

	// Used to show player icons
	if (g_bExtraSpritesEnabled){
		if (warcraft3 && (get_user_team(id) == get_user_team(pid))){
			if (get_pcvar_num( CVAR_wc3_race_icon ) && p_data[id][P_SHOWICONS]){
				Create_TE_PLAYERATTACHMENT(id, pid, 55, g_iRaceSprites[p_data[pid][P_RACE]], 15)
			}

			if (get_pcvar_num( CVAR_wc3_level_icon ) && p_data[id][P_SHOWICONS])
				Create_TE_PLAYERATTACHMENT(id, pid, 35, g_iLevelSprites[p_data[pid][P_LEVEL]], 16)
		}
	}

	if ( get_pcvar_num( CVAR_wc3_show_player ) && !g_freezeTime ){
		// From miscstats.sma
		new name[32], red = 0, blue = 0
		new team = get_user_team(pid)
		new idteam = get_user_team(id)

		get_user_name(pid,name,31)
		
		if ( team == TS ){
			if ( (p_data_b[pid][PB_MOLE] && team != idteam) && !ITEM_Has( id, ITEM_PROTECTANT ) )
				blue = 255
			else
				red = 255
		}
		else if ( team == CTS ){
			if ( (p_data_b[pid][PB_MOLE] && team != idteam) && !ITEM_Has( id, ITEM_PROTECTANT ) )
				red = 255
			else
				blue = 255
		}
		
		// Teammate
		if ( idteam == team || p_data_b[pid][PB_MOLE] ) {
			new wpnname[32] , clip, ammo, wpnid = get_user_weapon(pid,clip,ammo) 
			
			if ( wpnid > 0 && wpnid < MAX_WEAPONS + MAX_CWEAPONS ){
				get_weaponname(wpnid,wpnname,31) 
			}
			set_hudmessage(red,50,blue,-1.0,0.60,1, 0.01, 3.0, 0.01, 0.01, 4)
			show_hudmessage(id,"%s -- %d HP / %d AP / %s", name, get_user_health(pid), get_user_armor(pid), wpnname)
		} 
		else { 
			set_hudmessage(red,50,blue,-1.0,0.60,1, 0.01, 3.0, 0.01, 0.01, 4) 
			show_hudmessage(id, name) 
		} 
	}

	return PLUGIN_CONTINUE
}

public on_HideStatus(id){

	if ( get_pcvar_num( CVAR_wc3_show_player ) && !g_freezeTime ){
		set_hudmessage(0,0,0,0.0,0.0,0, 0.0, 0.01, 0.0, 0.0, 4) 
		show_hudmessage(id,"")
	}
}

public WAR3_Mole_Fix(){

	if ( get_pcvar_num( CVAR_wc3_query_client ) )
	{
		new players[32], num
		get_players(players, num, "c")
		
		for(new i = 0; i < num; i++)
		{
			if ( !is_user_bot( players[i] ) )
			{
				query_client_cvar(players[i], "cl_minmodels", "check_cvars");
			}
		}
	}
}

public check_cvars(id, const cvar[], const value[])
{
	if( equali(cvar,"cl_minmodels") && str_to_num(value) > 0 )
	{
		//client_cmd(id, "cl_minmodels 0");
		client_cmd(id, "echo ^"======== Warcraft 3 Frozen Throne ========^"");
		client_cmd(id, "echo ^"You were kicked because cl_minmodels is set to 1 on your client, please change this to 0.^"");
		client_cmd(id, "echo ^"Type ^"cl_minmodels 0^" in your console and press enter to do this.^"");
		client_cmd(id, "echo ^"=========================================^"");
		server_cmd("kick #%d ^"cl_minmodels 1 is not allowed on this server^"", get_user_userid(id));
	} 
} 

public on_Damage( iVictim )
{

	if ( !WAR3_Check() )
	{
		return;
	}
	
	if ( !SHARED_ValidPlayer( iVictim ) )
	{
		return;
	}
	
	new iWeapon, iHitPlace, iAttacker;
	new iDamage = read_data( 2 );
	iAttacker = get_user_attacker( iVictim, iWeapon, iHitPlace );

	EVENT_Damage( iVictim, iAttacker, iDamage, iWeapon, iHitPlace );

	return;
}
