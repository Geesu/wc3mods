/*
	This file is a part of Warcraft 3 Frozen Throne

	It contains functions that are used by admins
*/


public amx_takexp(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("amx_takexp",0)
	#endif

	new arg[32], arg2[32]
	read_argv(1,arg,31)
	read_argv(2,arg2,31)
	new id = str_to_num(arg)
	p_data[id][P_XP] -= str_to_num(arg2)
	WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
	return PLUGIN_HANDLED
}

public amx_givemole(id, level, cid){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("amx_givemole",id)
	#endif

	if (!(get_user_flags(id)&ADMIN_LEVEL_A)) { 
		if(id != 0){
			client_print(id,print_console,"%L",id,"YOU_HAVE_NO_ACCESS", g_MOD)
			return PLUGIN_HANDLED
		}
	}
	new arg[32] 
	read_argv(1,arg,31)  
	if(equali(arg,"@ALL")){
		new players[32], inum
		get_players(players,inum) 
		for(new a=0;a<inum;++a){
			p_data[players[a]][P_ITEM2]=ITEM_MOLE
			WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
		} 
		return PLUGIN_HANDLED
	}
	else if (arg[0]=='@'){ 
		new players[32], inum
		get_players(players,inum,"e",arg[1]) 
		if (inum==0){ 
			console_print(id,"%L",id,"NO_CLIENTS_IN_TEAM") 
			return PLUGIN_HANDLED 
		} 
		for(new a=0;a<inum;++a){
			p_data[players[a]][P_ITEM2]=ITEM_MOLE
			WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
		} 
	}
	else { 
		new player = cmd_target2(id,arg) 
		if (!player) return PLUGIN_HANDLED 
		p_data[player][P_ITEM2]=ITEM_MOLE
		WAR3_Display_Level(player,DISPLAYLEVEL_NONE)
	} 

	return PLUGIN_HANDLED 
}

public changeXP(){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("changeXP",0)
	#endif

	new arg1[4]
	new arg2[8]
	read_argv(1,arg1,3)
	read_argv(2,arg2,7)
		
	new id=str_to_num(arg1)
	new xp=str_to_num(arg2)

	if((p_data[id][P_XP] + xp)< 0)
		p_data[id][P_XP] = 0
	else
		p_data[id][P_XP] += xp
	
	WAR3_Display_Level(id, DISPLAYLEVEL_NONE)
}

//amx_wc3 command
public amx_wc3_launch(id, level, cid){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("amx_wc3_launch",id)
	#endif

	if (!(get_user_flags(id)&ADMIN_LEVEL_WC3)) { 
		if(id != 0){
			client_print(id,print_console,"%L",id,"YOU_HAVE_NO_ACCESS", g_MOD)
			return PLUGIN_HANDLED
		}
	}
	new cmds[32]
	read_argv(1,cmds,31)
	set_hudmessage(200, 100, 0, -1.0, 0.3, 2, 1.5, 10.0, 0.02, 5.0,15)
	if (equal(cmds, "on" ))
	{
		set_cvar_num("FT_control",0)
		set_cvar_num("sv_warcraft3",1)
		show_hudmessage(0,"%L",id,"IS_ACTIVATED_NOW",WC3NAME,WC3VERSION)
		return PLUGIN_HANDLED
	}
	else if (equal(cmds, "off" ))
	{
		set_cvar_num("FT_control",0)
		set_cvar_num("sv_warcraft3",0)
		show_hudmessage(0,"%L",id,"IS_NO_LONGER_ACTIVE",WC3NAME,WC3VERSION)
		return PLUGIN_HANDLED
	}
	else if (equal(cmds, "auto" ))
	{
		set_cvar_num("FT_control",1)
		show_hudmessage(0,"%L",id,"IS_IN_AUTOMATIC_g_MODE",WC3NAME,WC3VERSION)
		return PLUGIN_HANDLED
	}
	else
	{
		client_print(id,print_console,"%s %L",g_MODclient, id,"USAGE_STRING")
		return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public amx_savexp(id, level, cid){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("amx_savexp",id)
	#endif

	if (!(get_user_flags(id)&ADMIN_LEVEL_WC3)) { 
		if(id != 0){
			client_print(id,print_console,"%L",id,"YOU_HAVE_NO_ACCESS", g_MOD)
			return PLUGIN_HANDLED
		}
	}
	new arg[32], xp[10] 
	read_argv(1,arg,31) 
	read_argv(2,xp,9) 
	if(equali(arg,"@ALL")){
		new players[32], inum
		get_players(players,inum) 
		for(new a=0;a<inum;++a){
			XP_Save(players[a])
		} 
		return PLUGIN_HANDLED
	}
	if (arg[0]=='@'){ 
		new players[32], inum
		get_players(players,inum,"e",arg[1]) 
		if (inum==0){ 
			console_print(id,"%L",id,"NO_CLIENTS_IN_TEAM") 
			return PLUGIN_HANDLED 
		} 
		for(new a=0;a<inum;++a){
			XP_Save(players[a])
		} 
	}
	else { 
		new player = cmd_target2(id,arg) 
		if (!player) return PLUGIN_HANDLED 
		XP_Save(player)
	} 
	return PLUGIN_HANDLED 
} 

public amx_resetxp(id,saychat){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("amx_resetxp",id)
	#endif

	p_data[id][P_LEVEL]=0
	p_data[id][P_XP]=0
	p_data[id][P_SKILL1]=0
	p_data[id][P_SKILL2]=0
	p_data[id][P_SKILL3]=0
	p_data[id][P_ULTIMATE]=0
	WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
	XP_Save(id)
	if(saychat==1)
		client_print(id,print_chat,"%s %L",g_MODclient, id,"YOUR_XP_HAS_BEEN_RESET")
	else
		console_print(id,"%L",id,"YOUR_XP_HAS_BEEN_RESET",g_MOD)
}

public amx_givexp(id, level, cid){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("amx_givexp",id)
	#endif

	if (!(get_user_flags(id)&ADMIN_LEVEL_WC3)) {
		if(id != 0){
			client_print(id,print_console,"%L",id,"YOU_HAVE_NO_ACCESS", g_MOD)
			return PLUGIN_HANDLED
		}
	}
	new arg[32], xp[10]
	read_argv(1,arg,31) 
	read_argv(2,xp,9)
	if(equali(arg,"@ALL")){
		new players[32], inum
		get_players(players,inum)
		for(new a=0;a<inum;++a){
			client_print(players[a],print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_GAVE_YOU_EXPERIENCE", str_to_num(xp))
			p_data[players[a]][P_XP] += str_to_num(xp)
			WAR3_Display_Level(players[a],DISPLAYLEVEL_NONE)
		}
		return PLUGIN_HANDLED
	}
	if (arg[0]=='@'){ 
		new players[32], inum
		get_players(players,inum,"e",arg[1]) 
		if (inum==0){ 
			console_print(id,"%L",id,"NO_CLIENTS_IN_TEAM") 
			return PLUGIN_HANDLED 
			
		} 
		for(new a=0;a<inum;++a){
			client_print(players[a],print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_GAVE_YOU_EXPERIENCE", str_to_num(xp))
			p_data[players[a]][P_XP] += str_to_num(xp)
			WAR3_Display_Level(players[a],DISPLAYLEVEL_NONE)
		}
	}
	else { 
		new player = cmd_target2(id,arg) 
		if (!player) return PLUGIN_HANDLED 
		client_print(player,print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_GAVE_YOU_EXPERIENCE", str_to_num(xp))
		p_data[player][P_XP] += str_to_num(xp)
		WAR3_Display_Level(player,DISPLAYLEVEL_NONE)
	}
	return PLUGIN_HANDLED 
} 
