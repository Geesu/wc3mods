
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

	WAR3_Show_Spectator_Info(id, a)
	
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

public on_PlayerAction(){ 

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new sArg[MAX_VAR_LENGTH], sAction[MAX_VAR_LENGTH] 
	new sName[MAX_NAME_LENGTH]  
	new iUserId, id

	read_logargv(0,sArg,MAX_VAR_LENGTH) 
	read_logargv(2,sAction,MAX_VAR_LENGTH) 
	parse_loguser(sArg,sName,MAX_NAME_LENGTH,iUserId) 
	id = find_player("k",iUserId)

	if(id==0)						// Prevents the server from gaining XP
		return PLUGIN_CONTINUE

	// Bomb planted 
	if (equal(sAction,"Planted_The_Bomb")) {  

		g_fBombTime = get_gametime() + get_cvar_float("mp_c4timer")
		
		set_task((get_cvar_float("mp_c4timer") - 0.7), "on_B4TargetBombed",TASK_BOMBTIMER)

		new origin[3]
		new teamname[32]
		new player[32]
		new numplayers
		new targetorigin[3]
		new targetid
		
		get_user_origin(id, origin)
		get_user_team(id, teamname, 31)
		get_players(player, numplayers, "ae", teamname)
		g_bombCarrier = 0	

		new iXP = 3 * xpgiven[p_data[id][P_LEVEL]]												
		
		iXP = XP_give(id, iXP)
					
		if (get_pcvar_num( CVAR_wc3_show_objectives )){				
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_PLANTING_BOMB",sName,iXP)
		}

		for (new i=0; i<numplayers; ++i){		// Includes self
			targetid=player[i]
			get_user_origin(targetid, targetorigin)
			if (get_distance(origin, targetorigin)<=get_pcvar_num(CVAR_wc3_xp_radius)){
				iXP = get_pcvar_num( CVAR_wc3_kill_bomb_planter ) + xpgiven[p_data[targetid][P_LEVEL]]	

				iXP = XP_give(targetid, iXP)

				if (get_pcvar_num( CVAR_wc3_show_objectives )){
					get_user_name(targetid,sName,31)					
					client_print(targetid,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SUPPORTING_BOMB",sName,iXP)
				}				
			}
		}
	}
   // Bomb defused 
	else if (equal(sAction,"Defused_The_Bomb")) {
		new origin[3]
		new teamname[32]
		new player[32]
		new numplayers
		new targetorigin[3]
		new targetid
		
		get_user_origin(id, origin)
		get_user_team(id, teamname, 31)
		get_players(player, numplayers, "ae", teamname)
		
				
		new iXP = 3 *  xpgiven[p_data[id][P_LEVEL]]											
		
		iXP = XP_give(id, iXP)
				
		if (get_pcvar_num( CVAR_wc3_show_objectives )){
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_DEFUSING_BOMB",sName,iXP)
		}

		for (new i=0; i<numplayers; ++i){		// Includes self
			targetid=player[i]
			get_user_origin(targetid, targetorigin)
			if (get_distance(origin, targetorigin) <= get_pcvar_num(CVAR_wc3_xp_radius)){
				
				iXP = get_pcvar_num( CVAR_wc3_defuse_bomb ) +  xpgiven[p_data[targetid][P_LEVEL]]										
				iXP = XP_give(targetid, iXP)
							
				if (get_pcvar_num( CVAR_wc3_show_objectives )){
					get_user_name(targetid,sName,31)						
					client_print(targetid,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SUPPORT_BOMB_D",sName,iXP)
				}					
			}
		}		
	} 
	// Bomb defusing with a kit 
	else if (equal(sAction,"Begin_Bomb_Defuse_With_Kit")) { 

		if((++p_data[id][P_DEFUSERINDEX] == 1) && (get_user_team(id) == 2) ){  // Team 1 = Terror, Team 2 = CT
			new iXP = get_pcvar_num( CVAR_wc3_defuse_bomb ) + xpgiven[p_data[id][P_LEVEL]]

			iXP = XP_give(id, iXP)
				
			if (get_pcvar_num( CVAR_wc3_show_objectives )){	
				client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_ATTEMPT_BOMB_D",sName,iXP)
			}
		}               
   } 
   // Bomb defusing without a kit 
	else if (equal(sAction,"Begin_Bomb_Defuse_Without_Kit")) { 
     	
		if( (++p_data[id][P_DEFUSERINDEX] == 1) && (get_user_team(id) == 2) ){  // Team 1 = Terror, Team 2 = CT
			new iXP = xpgiven[p_data[id][P_LEVEL]]

			iXP = XP_give(id, iXP)

			if (get_pcvar_num( CVAR_wc3_show_objectives )){
				client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_ATTEMPT_BOMB_D_BARE",id,iXP)
			}
		}
   } 
	// Spawned with the bomb 
	else if (equal(sAction,"Spawned_With_The_Bomb")) { 
		g_bombCarrier = id
	  
		new iXP = xpgiven[p_data[id][P_LEVEL]]											

		iXP = XP_give(id, iXP)

		if (get_pcvar_num( CVAR_wc3_show_objectives )){
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SPAWNING_WITH_BOMB",sName,iXP)
		}
	} 

	// Dropped bomb 
	else if (equal(sAction,"Dropped_The_Bomb")) { 
		g_bombCarrier = 0
	  
		new iXP = -1 * xpgiven[p_data[id][P_LEVEL]]											

		iXP = XP_give(id, iXP)

		if (get_pcvar_num( CVAR_wc3_show_objectives ))
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_LOST_BOMB",sName,(-1*iXP))

   } 
	// Picked up bomb 
	else if (equal(sAction,"Got_The_Bomb")) { 
		g_bombCarrier = id
	  
		new iXP = xpgiven[p_data[id][P_LEVEL]]	

		iXP = XP_give(id, iXP)

		if (get_pcvar_num( CVAR_wc3_show_objectives ))
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_PICKING_UP_BOMB",sName,iXP)

	} 
	// Hostage touched 
	else if (equal(sAction,"Touched_A_Hostage")) { 
		new iXP = xpgiven[p_data[id][P_LEVEL]]	

		iXP = XP_give(id, iXP)
				
		if (get_pcvar_num( CVAR_wc3_show_objectives ))
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_BEGINNING_HOSTAGE_RESCUE",sName,iXP)

		g_hostageSaver = id
	} 
	// Hostage rescued 
	else if (equal(sAction,"Rescued_A_Hostage")) { 
		new origin[3]
		new teamname[32]
		new player[32]
		new numplayers
		new targetorigin[3]
		new targetid		
		
		get_user_origin(id, origin)
		get_user_team(id, teamname, 31)
		get_players(player, numplayers, "ae", teamname)
		
		new iXP = xpgiven[p_data[id][P_LEVEL]]												
			
		iXP = XP_give(id, iXP)
				
		if (get_pcvar_num( CVAR_wc3_show_objectives ))
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_RESCUING_HOSTAGE",sName,iXP)

		// Supporters and self	
		for (new i=0; i<numplayers; ++i){
			targetid=player[i]
			get_user_origin(targetid, targetorigin)
			if (get_distance(origin, targetorigin)<=get_pcvar_num(CVAR_wc3_xp_radius)){
				iXP = get_pcvar_num( CVAR_wc3_rescue_hostage ) +  xpgiven[p_data[targetid][P_LEVEL]]	

				iXP = XP_give(targetid, iXP)

				if (get_pcvar_num( CVAR_wc3_show_objectives )){
					get_user_name(targetid,sName,31)
					client_print(targetid,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SUPPORTING_HOSTAGE_RUN",sName,iXP)
				}					
			}				
		}				
   } 

	// Hostage killed 
	else if (equal(sAction,"Killed_A_Hostage")) { 
		new iXP = -1 * xpgiven[p_data[id][P_LEVEL]]

		iXP = XP_give(id, iXP)

		if (get_pcvar_num( CVAR_wc3_show_objectives ))
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_KILLING_HOSTAGE",sName,(-1*iXP))		

	} 
	// VIP spawn 
	else if (equal(sAction,"Became_VIP")) { 
		g_vipID = id
	  
		new iXP = xpgiven[p_data[id][P_LEVEL]]										
		iXP = XP_give(id, iXP)

		if (get_pcvar_num( CVAR_wc3_show_objectives )){
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SPAWN_AS_VIP",sName,iXP)
		}
		
	}

	// VIP assassinated 
	else if (equal(sAction,"Assassinated_The_VIP")) { 
		new sNameVIP[MAX_NAME_LENGTH] 
		get_user_name( g_vipID,sNameVIP,MAX_NAME_LENGTH) 		

		new iXP = get_pcvar_num( CVAR_wc3_kill_vip ) + xpgiven[p_data[id][P_LEVEL]]	

		iXP = XP_give(id, iXP)

		if (get_pcvar_num( CVAR_wc3_show_objectives ))
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_KILLING_VIP",sName,iXP,sNameVIP) 	
	} 
	// VIP escaped 
	else if (equal(sAction,"Escaped_As_VIP")) { 							
		new origin[3]
		new teamname[32]
		new player[32]
		new numplayers
		new targetorigin[3]
		new targetid

		get_user_origin(id, origin)
		get_user_team(id, teamname, 31)
		get_players(player, numplayers, "ae", teamname)
		
		new iXP = xpgiven[p_data[id][P_LEVEL]]

		iXP = XP_give(id, iXP)

		new nName[31]			
		if (get_pcvar_num( CVAR_wc3_show_objectives )){	
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_ESCAPED_VIP",sName,iXP)
		}

		for (new i=0; i<numplayers; ++i){		// Includes self
			targetid=player[i]
			get_user_origin(targetid, targetorigin)
			if (get_distance(origin, targetorigin)<=get_pcvar_num(CVAR_wc3_xp_radius) && targetid!=id){
				iXP = get_pcvar_num( CVAR_wc3_vip_escape ) + xpgiven[p_data[targetid][P_LEVEL]]
					
				iXP = XP_give(targetid, iXP)

				if (get_pcvar_num( CVAR_wc3_show_objectives )){
					get_user_name(targetid,nName,31)					
					client_print(targetid,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SUPPORTING_VIP",nName,iXP,sName)
				}				
			}
		}
	} 

	return PLUGIN_HANDLED 
}

public on_TargetBombed() {

	if (!warcraft3)
		return PLUGIN_CONTINUE

	set_task(0.1,"_on_TargetBombed",TASK_TARGETBOMBED)

	return PLUGIN_CONTINUE
}

public _on_TargetBombed(){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if (get_gametime() - g_fBombTime < 1.0){
		new players[32], numberofplayers, id, i
		get_players(players, numberofplayers)
		for(i=0;i<numberofplayers;i++){
			id = players[i]
			
			if( !p_data_b[id][PB_JUSTJOINED] && !is_user_alive(id) ){
				p_data_b[id][PB_DIEDLASTROUND] = true
			}
		}
	}

	return PLUGIN_CONTINUE
}

public on_B4TargetBombed()
{
	return;
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

public on_TerroristWin(){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new players[32], numberofplayers,a, bool:give
	get_players(players,numberofplayers,"e","TERRORIST") 
	for (a = 0; a < numberofplayers; ++a){
		give = true

		if(p_data_b[players[a]][PB_JUSTJOINED] || p_data_b[players[a]][PB_CHANGINGTEAM])
			give = false

		if(give){
			new iXP = (get_pcvar_num(CVAR_wc3_win_round) + xpgiven[p_data[players[a]][P_LEVEL]])
			iXP = XP_give(players[a], iXP)
			if (get_pcvar_num( CVAR_wc3_show_objectives )){				
				client_print(players[a],print_chat, "%s %L", g_MODclient, players[a],"AWARD_FOR_WINNING_ROUND",iXP)
			}				
		}
	}
	return PLUGIN_CONTINUE
}

public on_CTWin(){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new players[32], numberofplayers,a, bool:give
	get_players(players,numberofplayers,"e","CT") 
	for (a = 0; a < numberofplayers; ++a){
		give = true

		if(p_data_b[players[a]][PB_JUSTJOINED] || p_data_b[players[a]][PB_CHANGINGTEAM])
			give = false

		if(give){
			new iXP = (get_pcvar_num(CVAR_wc3_win_round) + xpgiven[p_data[players[a]][P_LEVEL]])
			iXP = XP_give(players[a], iXP)

			if (get_pcvar_num( CVAR_wc3_show_objectives )){			
				client_print(players[a],print_chat, "%s %L", g_MODclient, players[a],"AWARD_FOR_WINNING_ROUND",iXP)
			}				
		}
	}
	return PLUGIN_CONTINUE
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
		client_cmd(id,"speak %s", SOUND_ANTEND)
	}

	// Used to show player icons
	if (g_spritesEnabled){
		if (warcraft3 && (get_user_team(id) == get_user_team(pid))){
			if (get_pcvar_num( CVAR_wc3_race_icon ) && p_data[id][P_SHOWICONS]){
				Create_TE_PLAYERATTACHMENT(id, pid, 55, g_sRace[p_data[pid][P_RACE]], 15)
			}

			if (get_pcvar_num( CVAR_wc3_level_icon ) && p_data[id][P_SHOWICONS])
				Create_TE_PLAYERATTACHMENT(id, pid, 35, g_sLevel[p_data[pid][P_LEVEL]], 16)
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
