
// Advanced Swear Filter and Punishment plugin uses this function
public Admin_TakeXP(){
	#if ADVANCED_DEBUG
		writeDebugInfo("Admin_TakeXP",0)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new argID[3], argXP[32]
	read_argv(1,argID,2)
	read_argv(2,argXP,31)

	new id = str_to_num(argID)

	p_data[id][P_XP] -= str_to_num(argXP)

	WAR3_Display_Level(id,DISPLAYLEVEL_NONE)

	return PLUGIN_HANDLED
}

public Admin_GiveMole(id, level, cid){
	#if ADVANCED_DEBUG
		writeDebugInfo("Admin_GiveMole",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

    if ( id && !( get_user_flags( id ) & XP_get_admin_flag() ) )
	{
		client_print(id,print_console,"%L",id,"YOU_HAVE_NO_ACCESS", g_MODclient)
		return PLUGIN_HANDLED
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
		new player = find_target(id,arg) 
		if (!player) return PLUGIN_HANDLED 
		p_data[player][P_ITEM2]=ITEM_MOLE
		WAR3_Display_Level(player,DISPLAYLEVEL_NONE)
	} 

	return PLUGIN_HANDLED 
}

public changeXP(){ 
	#if ADVANCED_DEBUG
		writeDebugInfo("changeXP",0)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

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

	return PLUGIN_CONTINUE
}

public Admin_SaveXP(id, level, cid){
	#if ADVANCED_DEBUG
		writeDebugInfo("Admin_SaveXP",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

    if ( id && !( get_user_flags( id ) & XP_get_admin_flag() ) )
	{
		client_print(id,print_console,"%L",id,"YOU_HAVE_NO_ACCESS", g_MODclient)
		return PLUGIN_HANDLED
	}

	new target[32]
	read_argv(1,target,31) 

	_Admin_SaveXP(id, target)

	return PLUGIN_HANDLED
}

_Admin_SaveXP(id, target[]){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Admin_SaveXP",id)
	#endif


	if(equali(target,"@ALL")){
		new players[32], inum
		get_players(players,inum,"c") 

		for(new a=0;a<inum;++a){
			XP_Save(players[a])
		} 

		return PLUGIN_CONTINUE
	}
	else if (target[0]=='@'){ 
		new players[32], inum
		get_players(players,inum,"ce",target) 

		if (inum==0){ 
			console_print(id,"%L",id,"NO_CLIENTS_IN_TEAM") 
			return PLUGIN_CONTINUE 
		} 

		for(new a=0;a<inum;++a){
			XP_Save(players[a])
		} 
	}
	else { 
		new player = find_target(id,target)

		if (!player)
			return PLUGIN_CONTINUE 

		XP_Save(player)
	} 
	return PLUGIN_CONTINUE 
} 

public Admin_GiveXP(id, level, cid){
	#if ADVANCED_DEBUG
		writeDebugInfo("Admin_GiveXP",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

    if ( id && !( get_user_flags( id ) & XP_get_admin_flag() ) )
	{
			client_print(id,print_console,"%L",id,"YOU_HAVE_NO_ACCESS", g_MODclient)
			return PLUGIN_HANDLED
	}

	new target[32], xp[10] 
	read_argv(1,target,31) 
	read_argv(2,xp,9) 

	_Admin_GiveXP(id, target, str_to_num(xp))

	return PLUGIN_HANDLED
}

public _Admin_GiveXP(id, target[], iXP){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Admin_GiveXP",id)
	#endif


	if(equali(target,"@ALL")){
		new players[32], inum
		get_players(players,inum)

		for(new a=0;a<inum;++a){
			client_print(players[a],print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_GAVE_YOU_EXPERIENCE", iXP)

			p_data[players[a]][P_XP] += iXP

			WAR3_Display_Level(players[a],DISPLAYLEVEL_NONE)
		}

		return PLUGIN_CONTINUE
	}
	else if (target[0]=='@'){ 
		new players[32], inum
		get_players(players,inum,"e",target[1]) 

		if (inum==0){ 
			console_print(id,"%L",id,"NO_CLIENTS_IN_TEAM") 

			return PLUGIN_CONTINUE 
		} 

		for(new a=0;a<inum;++a){
			client_print(players[a],print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_GAVE_YOU_EXPERIENCE", iXP)

			p_data[players[a]][P_XP] += iXP

			WAR3_Display_Level(players[a],DISPLAYLEVEL_NONE)
		}
	}
	else { 
		new player = find_target(id,target) 

		if (!player)
			return PLUGIN_CONTINUE 

		client_print(player,print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_GAVE_YOU_EXPERIENCE", iXP)

		p_data[player][P_XP] += iXP

		WAR3_Display_Level(player,DISPLAYLEVEL_NONE)
	}
	return PLUGIN_CONTINUE 
} 
