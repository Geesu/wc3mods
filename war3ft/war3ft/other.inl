public showRank(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("showRank",id)
	#endif

	if(!p_data[id][P_SPECMODE] || !warcraft3 || !iCvar[FT_SPEC_INFO])
		return PLUGIN_CONTINUE

	new a = read_data(2)

	if (!p_data_b[a][PB_ISCONNECTED])
		return PLUGIN_CONTINUE		// not valid target 

	WAR3_Show_Spectator_Info(id, a)
	
	return PLUGIN_CONTINUE
}

#if MOD == 0
	public setSpecMode(id) {
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("setSpecMode",0)
		#endif

		new arg[12] 
		read_data( 2 , arg , 11 ) 
		p_data[id][P_SPECMODE] = ( arg[10] == '2' ) ? 1 : 0 
	}
#endif

fire_flamethrower(id){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("fire_flamethrower",id)
	#endif

   emit_sound(id, CHAN_STATIC, SOUND_FLAMESTRIKE, 1.0, ATTN_NORM, 0, PITCH_NORM) 

   new vec[3] 
   new aimvec[3] 
   new velocityvec[3] 
   new length     
   new speed = 10 
   get_user_origin(id,vec) 
   get_user_origin(id,aimvec,2) 
   new dist = get_distance(vec,aimvec) 

   if (p_data[id][P_FLAMECOUNT]==1){
      new parm[2]
      parm[0]=id

      p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
      _WAR3_Ultimate_Delay(parm)
   }
   new speed1 = 160 
   new speed2 = 350 
   new radius = 105 

   if(dist < 50){ 
      radius = 0 
      speed = 5 
   } 
   else if(dist < 150){ 
      speed1 = speed2 = 1 
      speed = 5 
      radius = 50 
   } 
   else if(dist < 200){ 
      speed1 = speed2 = 1 
      speed = 5 
      radius = 90 
   } 
   else if(dist < 250){ 
      speed1 = speed2 = 90 
      speed = 6 
      radius = 90 
   } 
   else if(dist < 300){ 
      speed1 = speed2 = 140 
      speed = 7 
   } 
   else if(dist < 350){ 
      speed1 = speed2 = 190 
      speed = 7 
   } 
   else if(dist < 400){ 
      speed1 = 150 
      speed2 = 240 
      speed = 8 
   } 
   else if(dist < 450){ 
      speed1 = 150 
      speed2 = 290 
      speed = 8 
   } 
   else if(dist < 500){ 
      speed1 = 180 
      speed2 = 340 
      speed = 9 
   } 

   velocityvec[0]=aimvec[0]-vec[0] 
   velocityvec[1]=aimvec[1]-vec[1] 
   velocityvec[2]=aimvec[2]-vec[2] 
   length=sqroot(velocityvec[0]*velocityvec[0]+velocityvec[1]*velocityvec[1]+velocityvec[2]*velocityvec[2]) 
   velocityvec[0]=velocityvec[0]*speed/length 
   velocityvec[1]=velocityvec[1]*speed/length 
   velocityvec[2]=velocityvec[2]*speed/length 

   new args[8] 
   args[0] = vec[0] 
   args[1] = vec[1] 
   args[2] = vec[2] 
   args[3] = velocityvec[0] 
   args[4] = velocityvec[1] 
   args[5] = velocityvec[2] 
	
   set_task(0.1,"te_spray",TASK_TESPRAY+id,args,8,"a",2) 
   check_burnzone(id,vec,aimvec,speed1,speed2,radius) 

} 

public te_spray(args[]){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("te_spray",0)
	#endif

	new position[3], direction[3]
	position[0] = args[0]
	position[1] = args[1]
	position[2] = args[2]
	direction[0] = args[3]
	direction[1] = args[4]
	direction[2] = args[5]

	if(!g_mapDisabled)
		Create_TE_SPRAY(position, direction, g_sFire, 8, 70, 100, 5)

	return PLUGIN_CONTINUE 
} 

check_burnzone(id,vec[],aimvec[],speed1,speed2,radius){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("check_burnzone",id)
	#endif

   new maxplayers = get_maxplayers()+1 
   new tbody,tid 
   get_user_aiming(id,tid,tbody,550) 
   if((tid > 0) && (tid < maxplayers) && get_user_team(tid)!=get_user_team(id)){ 
       burn_victim(tid,id,0) 
   } 
    
   new burnvec1[3],burnvec2[3],length1 

   burnvec1[0]=aimvec[0]-vec[0] 
   burnvec1[1]=aimvec[1]-vec[1] 
   burnvec1[2]=aimvec[2]-vec[2] 

   length1=sqroot(burnvec1[0]*burnvec1[0]+burnvec1[1]*burnvec1[1]+burnvec1[2]*burnvec1[2]) 
   burnvec2[0]=burnvec1[0]*speed2/length1 
   burnvec2[1]=burnvec1[1]*speed2/length1 
   burnvec2[2]=burnvec1[2]*speed2/length1 
   burnvec1[0]=burnvec1[0]*speed1/length1 
   burnvec1[1]=burnvec1[1]*speed1/length1 
   burnvec1[2]=burnvec1[2]*speed1/length1 
   burnvec1[0] += vec[0] 
   burnvec1[1] += vec[1] 
   burnvec1[2] += vec[2] 
   burnvec2[0] += vec[0] 
   burnvec2[1] += vec[1] 
   burnvec2[2] += vec[2] 

   new origin[3] 
   for (new i=1; i<=maxplayers; i++) { 
	 if(is_user_alive(i) && (i != id) && get_user_team(i)!=get_user_team(id)){ 
		get_user_origin(i,origin) 
		if(get_distance(origin,burnvec1) < radius) 
		   burn_victim(i,id,0) 
		else if(get_distance(origin,burnvec2) < radius) 
		   burn_victim(i,id,0) 
      } 
   } 
   return PLUGIN_CONTINUE       
} 

burn_victim(id,killer,tk){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("burn_victim",id)
	#endif

   if(p_data_b[id][PB_ISBURNING]) 
      return PLUGIN_CONTINUE 
   if (p_data[id][P_ITEM]==ITEM_NECKLACE || p_data_b[id][PB_WARDENBLINK])
      return PLUGIN_CONTINUE
   p_data_b[id][PB_ISBURNING] = true

   new hp,args[4] 
   hp = get_user_health(id) 

   args[0] = id 
   args[1] = killer 
   args[2] = tk    
   set_task(0.3,"on_fire",TASK_ONFIRE,args,4,"a",hp / 10) 

   return PLUGIN_CONTINUE 
} 

public on_fire(args[]){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("on_fire",args[0])
	#endif

	new rx,ry,rz,forigin[3], position[3]
	new id = args[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new killer = args[1] 
	if(!p_data_b[id][PB_ISBURNING]) 
		return PLUGIN_CONTINUE 
	rx = random_num(-30,30) 
	ry = random_num(-30,30) 
	rz = random_num(-30,30) 
	get_user_origin(id,forigin) 

	position[0] = forigin[0]+rx
	position[1] = forigin[1]+ry
	position[2] = forigin[2]+10+rz

	if(!g_mapDisabled)
		Create_TE_SPRITE(position, g_sBurning, 30, 200)

	position[0] = forigin[0]+(rx*2)
	position[1] = forigin[1]+(ry*2)
	position[2] = forigin[2]+100+(rz*2)

#if MOD == 0
	if(!g_mapDisabled)
		Create_TE_Smoke(position, position, g_sSmoke, 60, 15)
#endif

	if(!is_user_alive(id)) 
		return PLUGIN_CONTINUE 

	WAR3_damage(id, killer, 10, CSW_FLAME, 0)

	return PLUGIN_CONTINUE 
}

public healitnow(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("healitnow",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (Verify_Skill(id, RACE_SHADOW, SKILL1) && !endround && is_user_alive(id)){
		set_task(p_heal[p_data[id][P_SKILL1]-1],"healitnow",TASK_HEALNOW+id,parm,2)
	}
	healingnext(parm)
	return PLUGIN_CONTINUE
}

public healingnext(parm[2]){		// Healing Wave
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("healingnext",parm[0])
	#endif

	new id=parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new origin[3]
	get_user_origin(id, origin)
	new players[32]
	new numberofplayers
	new teamname[32]
	get_user_team(id, teamname, 31)
	get_players(players, numberofplayers,"ae",teamname)
	new i
	new targetid = 0
	new distancebetween = 0
	new targetorigin[3]
	new heal = 1
	new closestid = 0
	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]
		if (get_user_team(id)==get_user_team(targetid) && is_user_alive(targetid)){
			get_user_origin(targetid,targetorigin)
			distancebetween=get_distance(origin,targetorigin)
			if (distancebetween < iCvar[FT_HEALING_RANGE] && !p_data_b[targetid][PB_HEALINGHIT])
				closestid = targetid
		}
	}

	if (closestid){
		healingeffect(id,closestid,heal)
		parm[0]=targetid
		parm[1]=heal
		set_task(0.2,"healingnext",TASK_HEALNEXT+id,parm,2)
	}
	else{
		for (i = 0; i < numberofplayers; ++i){
			targetid=players[i]
			p_data_b[targetid][PB_HEALINGHIT]=false
		}
	}
	return PLUGIN_CONTINUE
}

public healingeffect(id,targetid,heal){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("healingeffect",id)
	#endif

	p_data_b[targetid][PB_HEALINGHIT]=true

	if (!is_user_alive(targetid))
		return PLUGIN_HANDLED

	while(heal!=0){
		if( get_user_actualhealth(targetid) + heal <= get_user_maxhealth(targetid) ){
			set_user_health(targetid, get_user_health(targetid) + heal)
		}
		heal--
	}

	new origin[3]
	get_user_origin(targetid,origin)

	Create_TE_IMPLOSION(origin, 100, 8, 1)

	return PLUGIN_CONTINUE
}

public lightpls(parm[6]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("lightpls",parm[5])
	#endif

	new id = parm[5]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (!is_user_alive(id))
		return PLUGIN_CONTINUE

	new origin[3], damage, start[3], end[3], red, blue, green
	origin[0]=parm[0]
	origin[1]=parm[1]
	origin[2]=parm[2]

	start[0] = origin[0]
	start[1] = origin[1]
	start[2] = origin[2] + 600
	end[0] = origin[0]
	end[1] = origin[1]
	end[2] = origin[2] - 600

#if MOD == 0
	if(p_data[id][P_SERPENTTEAM]==TS){
		red = 255
		blue = 0
		green = 0
	}
	else{
		red = 0
		blue = 255
		green = 0
	}
#endif
#if MOD == 1
	if(p_data[id][P_SERPENTTEAM]==AXIS){
		red = 255
		blue = 63
		green = 63
	}
	else{
		red = 76
		blue = 102
		green = 76
	}
#endif
	if(!g_mapDisabled)
		Create_TE_BEAMPOINTS(start, end, g_sLightning, 1, 5, 2, 500, 20, red, green, blue, 100, 100)


	new players[32]
	new numberofplayers
	get_players(players, numberofplayers)
	new i
	new targetid = 0
	new distancebetween = 0
	new targetorigin[3]
	
	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]
		if (p_data[id][P_SERPENTTEAM]!=get_user_team(targetid) &&is_user_alive(targetid)){
			get_user_origin(targetid,targetorigin)
			distancebetween=sqroot((origin[1]-targetorigin[1])*(origin[1]-targetorigin[1])+(origin[0]-targetorigin[0])*(origin[0]-targetorigin[0]))
			if ((distancebetween < 85)){
				damage = 10
				WAR3_damage(targetid, id, damage, CSW_SERPENTWARD, 0)

				client_cmd(targetid, "speak ambience/thunder_clap.wav")

				Create_ScreenFade(targetid, (1<<10), (1<<10), (1<<12), red, green, blue, 255)
			}
		}
	}

	if (!endround)
		set_task(0.5,"lightpls",TASK_LIGHT+id,parm,6)

	return PLUGIN_CONTINUE
}

public checkmap(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("checkmap",0)
	#endif

	#if MOD ==0
		new mapname[32]
		get_mapname(mapname,31) 
		if (containi(mapname,"ka_")!=-1 || containi(mapname,"scoutzknivez")!=-1){
			g_givePistol=false
			if (iCvar[FT_NO_GLOVES_ON_KA])
				g_giveHE=false
			else
				g_giveHE=true
		}
		else{
			g_giveHE=true
			g_givePistol=true
		}
		if(iCvar[FT_NO_ORCNADES]){
			if (containi(mapname,"ka_")!=-1 || containi(mapname,"jail_riot")!=-1 || containi(mapname,"fy_")!=-1) 
				g_notAllowHE=true
			else
				g_notAllowHE=false
		}
	#endif
	
	return PLUGIN_CONTINUE
}

public changeskin(id,reset){							// Function changes your skin for ITEM_MOLE and Chameleon
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("changeskin",id)
	#endif
#if MOD == 0
	new num = 0, newSkin[32]
	if (is_running("cstrike"))
		num = random_num(0,3)
	else if (is_running("czero"))
		num = random_num(0,4)

	
	if (reset==SKIN_RESET && (p_data[id][P_SKINCHANGED] == SKIN_HEX || p_data[id][P_SKINCHANGED]==SKIN_SWITCH)){
		cs_reset_user_model(id)
		p_data[id][P_SKINCHANGED]=SKIN_RESET
		//console_print(id,"### Skin reset", newSkin)
	}
	else if (reset==SKIN_HEX){ 
		if(!g_mapDisabled)
			cs_set_user_model(id,"alien4")
		p_data[id][P_SKINCHANGED]=SKIN_HEX
		//console_print(id,"### You should look like an alien", newSkin)
	}
	else if (reset==SKIN_SWITCH){
		if (get_user_team(id)==TS)
			add(newSkin,31,CTSkins[num])
		else
			add(newSkin,31,TSkins[num])

		cs_set_user_model(id,newSkin)
		//console_print(id,"### You should look like: %s", newSkin)
		p_data[id][P_SKINCHANGED]=SKIN_SWITCH
	}
#endif
	return PLUGIN_CONTINUE
}


public clear_all_icons(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("clear_all_icons",id)
	#endif
	#if MOD == 0
		if(id==0)
			return PLUGIN_CONTINUE

		new string[8][32] = {"dmg_rad","item_longjump","dmg_shock","item_healthkit","dmg_heat","suit_full","cross","dmg_gas"}
		for(new i=0;i<8;i++){
			Create_StatusIcon(id, ICON_HIDE, string[i], 0, 0, 0)
		}
	#endif
	return PLUGIN_CONTINUE
}

public icon_controller(id, value){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("icon_controller",id)
	#endif

	if (!iCvar[FT_SHOW_ICONS])
		return PLUGIN_HANDLED
	if(!p_data[id][P_ULTIMATE])
		return PLUGIN_HANDLED
	if (g_ultimateDelay > 0.0)
		return PLUGIN_HANDLED
	if(p_data_b[id][PB_ULTIMATEUSED] && value!=ICON_HIDE)
		return PLUGIN_HANDLED
	if(id==0)
		return PLUGIN_HANDLED
#if MOD == 0
	new string[32], r, g, b, switchValue
	if(p_data[id][P_RACE] == 9)
		switchValue = race9Options[4]
	else
		switchValue = p_data[id][P_RACE]

	switch(switchValue){
		case 1:format(string,31,"dmg_rad"),			r=255,	g=0,	b=0			// Undead
		case 2:format(string,31,"item_longjump"),	r=0,	g=120,	b=120		// Human
		case 3:format(string,31,"dmg_shock"),		r=255,	g=255,	b=255		// Orc
		case 4:format(string,31,"item_healthkit"),	r=0,	g=0,	b=255		// Night Elf
		case 5:format(string,31,"dmg_heat"),		r=255,	g=0,	b=0			// Blood Mage
		case 6:format(string,31,"suit_full"),		r=0,	g=200,	b=200		// Shadow Hunter
		case 7:format(string,31,"cross"),			r=255,	g=0,	b=0			// Warden
		case 8:format(string,31,"dmg_gas"),			r=0,	g=255,	b=0			// Crypt Lord
	}

	if(!is_user_alive(id))			// If the user is dead then hide the icon
		value = ICON_HIDE

	if(value==ICON_FLASH){
		if( Verify_Skill(id, RACE_UNDEAD, SKILL4) )
			r=255,g=255,b=255
	}

	Create_StatusIcon(id, value, string, r, g, b)
#endif
	return PLUGIN_CONTINUE
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

public calllocustfunction(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("calllocustfunction",id)
	#endif


	new parm[11]
	parm[7]=id
	new players[32], numberofplayers, i, player, possibility[33], count = 0
	get_players(players, numberofplayers)
	for (i = 0; i < numberofplayers; ++i){
		player=players[i]
		if(get_user_team(player)!=get_user_team(id) && is_user_alive(player) && !p_data_b[player][PB_WARDENBLINK] && p_data[player][P_ITEM]!=ITEM_NECKLACE){
			possibility[count]=player
			count++
		}
	}
	if(count==0){
		set_hudmessage(178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 5)
		show_hudmessage(id,"%L",id,"NO_VALID_TARGETS_FOUND")
		return PLUGIN_CONTINUE
	}

	while(parm[6]==0)								// Prevents target from being the server
		parm[6]=possibility[random_num(0,count)]

	new origin[3], origin2[3]
	get_user_origin(id,origin)
	get_user_origin(parm[6],origin2)
	parm[0]=origin[0]
	parm[1]=origin[1]
	parm[2]=origin[2]

	icon_controller(id,ICON_FLASH)

	p_data_b[id][PB_ULTIMATEUSED]=true

	drawfunnels(parm)
	return PLUGIN_CONTINUE
}

public drawfunnels(parm[]){
	new MULTIPLIER = 150 // the lower the number the faster it reaches the target
	new id = parm[6]
	new caster = parm[7]

	if(p_data[id][P_ITEM]==ITEM_NECKLACE || p_data_b[id][PB_WARDENBLINK] || !is_user_alive(id) || !p_data_b[id][PB_ISCONNECTED]){
		p_data_b[caster][PB_ULTIMATEUSED]=false
		icon_controller(caster,ICON_SHOW)	
		return PLUGIN_HANDLED
	}

	new origin[3], funnel[3], name[32], name2[32]
	get_user_name(id,name,31)
	get_user_name(caster,name2,31)
	get_user_origin(id,origin)

	funnel[0]=parm[0]			// Origin of the funnel
	funnel[1]=parm[1]
	funnel[2]=parm[2]
	
	if(!g_mapDisabled)
		Create_TE_LARGEFUNNEL(funnel, g_sSnow, 0)
	
	new xdist = diff(origin[0],funnel[0])
	new ydist = diff(origin[1],funnel[1])
	new zdist = diff(origin[2],funnel[2])	

	if(diff(origin[0],(funnel[0]-MULTIPLIER))<xdist)
		parm[0]=funnel[0]-MULTIPLIER
	else if(diff(origin[0],(funnel[0]+MULTIPLIER))<xdist)
		parm[0]=funnel[0]+MULTIPLIER
	else
		parm[0]=origin[0]

	if(diff(origin[1],(funnel[1]-MULTIPLIER))<ydist)
		parm[1]=funnel[1]-MULTIPLIER
	else if(diff(origin[1],(funnel[1]+MULTIPLIER))<ydist)
		parm[1]=funnel[1]+MULTIPLIER
	else
		parm[1]=origin[1]

	if(diff(origin[2],(funnel[2]-MULTIPLIER))<zdist)
		parm[2]=funnel[2]-MULTIPLIER
	else if(diff(origin[2],(funnel[2]+MULTIPLIER))<zdist)
		parm[2]=funnel[2]+MULTIPLIER
	else
		parm[2]=origin[2]

	if(!endround){
		if(!(xdist<50 && ydist<50 && zdist<50)){
			set_task(0.1,"drawfunnels",caster+TASK_FUNNELS,parm,11)
		}
		else{
			WAR3_damage(id, caster, 45, CSW_LOCUSTS, 0)

			emit_sound(id,CHAN_STATIC, SOUND_LOCUSTSWARM, 1.0, ATTN_NORM, 0, PITCH_NORM)

			icon_controller(caster,ICON_HIDE)

			new cooldownparm[2]
			cooldownparm[0]=caster
			
			p_data[caster][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
			_WAR3_Ultimate_Delay(cooldownparm)
		}
	}
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

public FT_controller(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("FT_controller",0)
	#endif

	FT_control = get_cvar_num("FT_control")
	FT_start = get_cvar_num("FT_start")
	FT_stop = get_cvar_num("FT_stop")
	FT_message = get_cvar_num("FT_message")

	new stime[4],sminute[4]
	get_time("%M",sminute,3)
	get_time("%H",stime,3)
	new minutes = str_to_num(sminute)
	new hours  = str_to_num(stime)
	new nextmap[32]
	get_cvar_string("amx_nextmap",nextmap,31)
	new timeleft = get_timeleft()
	set_hudmessage(50, 50, 200, 0.13, 0.01, 2, 1.5, 12.0, 0.02, 5.0,15)
	new bool:okidoki = false
	new message[128]
	if (FT_control){
		if(FT_start > FT_stop){
			if (((hours >= FT_start) && (hours > FT_stop)) || ((hours < FT_start) && (hours < FT_stop)))
				okidoki = true
		}
		if (FT_stop > FT_start){
			if ((hours >= FT_start) && (hours < FT_stop))
				okidoki = true
		}	
		if (FT_start == FT_stop)
			okidoki = true
		if(okidoki){
			set_cvar_num("sv_warcraft3",1)
			if (FT_start == FT_stop)
				format(message,127,"")
			else{			
				format(message,127,"%L",LANG_PLAYER,"BETWEEN",FT_start,FT_stop)
			}
		}		
		else{
			set_cvar_num("sv_warcraft3",0)
			format(message,127,"de %dh a %dh",FT_stop,FT_start)
		}
		if (!get_cvar_num("sv_warcraft3") && FT_message){
			show_hudmessage(0,"%L",LANG_PLAYER,"INACTIVE",WC3NAME,WC3VERSION,message,hours,minutes,timeleft / 60, timeleft % 60,nextmap)
		}
		if (get_cvar_num("sv_warcraft3") && FT_message){
			show_hudmessage(0,"%L",LANG_PLAYER,"ACTIVE",WC3NAME,WC3VERSION,message,hours,minutes,timeleft / 60, timeleft % 60,nextmap)
		}
		set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 10.0, 0.1, 0.2, 17)
		if ((g_lastAnnounce != minutes) && (hours == (FT_start - 1)) && ((minutes == 30) || (minutes == 45)|| (minutes == 55))){
			g_lastAnnounce = minutes
			show_hudmessage(0,"%L",LANG_PLAYER,"IS_ACTIVATED",WC3NAME,60 - minutes)			
		}
		if ((g_lastAnnounce != minutes) && (hours == (FT_stop - 1)) && ((minutes == 30) || (minutes == 45) || (minutes == 55))){
			g_lastAnnounce = minutes
			show_hudmessage(0,"%L",LANG_PLAYER,"WILL_BE_DEACTIVATED_IN",WC3NAME,60 - minutes)
		}
	}
	else
	{
		if (warcraft3==false && FT_message)	{	
			show_hudmessage(0,"%L",LANG_PLAYER,"INACTIVE_ELSE",WC3NAME,WC3VERSION,hours,minutes,timeleft / 60, timeleft % 60,nextmap)	
		}
		if (warcraft3==true && FT_message){
			show_hudmessage(0,"%L",LANG_PLAYER,"ACTIVE_ELSE",WC3NAME,WC3VERSION,hours,minutes,timeleft / 60, timeleft % 60,nextmap)
		}
	}		
	return PLUGIN_CONTINUE
}

public giveheifnothas(parm[2]) { 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("giveheifnothas",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new wpnList[32] = 0 
	new number = 0
	new foundNade = false 
	get_user_weapons(id,wpnList,number) 
	for (new i = 0;i < number && !foundNade;i++) { 
		#if MOD == 0
			if (wpnList[i] == CSW_HEGRENADE) 
				foundNade = true 
		#endif
		#if MOD == 1
			if (wpnList[i] == DODW_HANDGRENADE || wpnList[i] == DODW_STICKGRENADE) 
				foundNade = true 
		#endif
	}

	if (!foundNade && is_user_alive(id)){
	#if MOD == 0
		set_hudmessage(0, 100, 0, 0.05, 0.65, 2, 0.02, 10.0, 0.01, 0.1, 2)
		show_hudmessage(id,"%L",id,"ENJOY_A_GRENADE")

		give_item(id,"weapon_hegrenade")
	#endif
	#if MOD == 1
		new message[128]
		format(message,127,"%L",id,"ENJOY_A_GRENADE")
		Create_HudText(id, message, 1)
	
		if(get_user_team(id)==ALLIES)
			give_item(id,"weapon_handgrenade")
		else
			give_item(id,"weapon_stickgrenade")
	#endif
	} 

	p_data_b[id][PB_NADEJUSTRECEIVED]=false
	return PLUGIN_CONTINUE 
} 

public getuserinput(parm[1]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("getuserinput",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
	if (p_data[id][P_RACE] == 0){
		WAR3_chooserace(id)
	}
	else if (skillsused < p_data[id][P_LEVEL])
		menu_Select_Skill(id,0)
	else
		WAR3_Display_Level(id, DISPLAYLEVEL_NONE)

	return PLUGIN_HANDLED
}

public check_blink(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("check_blink",id)
	#endif

	if ( Verify_Skill(id, RACE_WARDEN, SKILL2) ){			// Blink
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= p_blink[p_data[id][P_SKILL2]-1])
			p_data_b[id][PB_WARDENBLINK]=true	
		else
			p_data_b[id][PB_WARDENBLINK]=false
	}
	else
		p_data_b[id][PB_WARDENBLINK]=false
}

public check_fan(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("check_fan",id)
	#endif

	// Fan of Knives

	if ( Verify_Skill(id, RACE_WARDEN, SKILL1) && is_user_alive(id) ){
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= p_fan[p_data[id][P_SKILL1]-1]){
			new parm[2]
			parm[0]=id
			parm[1]=7
			set_task(0.1,"_Item_Mole",TASK_FAN+id,parm,2)
		}
	}
}

public check_skills(id){							// Gives skill abilities at beginning of round and when skills are selected
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("check_skills",id)
	#endif

	new parm[2]
	parm[0]=id

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if ( Verify_Skill(id, RACE_ELF, SKILL1) && p_data_b[id][PB_EVADENEXTSHOT] ){		// Evasion
		set_user_health(id,1124)
	}
	else if ( Verify_Skill(id, RACE_HUMAN, SKILL2) ){		// Devotion Aura
		set_user_health(id,p_devotion[p_data[id][P_SKILL2]-1])
	}


	if ( Verify_Skill(id, RACE_SHADOW, SKILL1) ){			// Healing Wave
		if (task_exists(id+TASK_WAVE))
			remove_task(id+TASK_WAVE)
		set_task(1.0,"healitnow",TASK_WAVE+id,parm,2)
	}
	else{
		if (task_exists(id+TASK_WAVE))
			remove_task(id+TASK_WAVE)
	}

	if ( Verify_Skill(id, RACE_SHADOW, SKILL3) )			// Serpent Ward
		p_data[id][P_SERPENTCOUNT]=p_serpent[p_data[id][P_SKILL3]-1]
	else
		p_data[id][P_SERPENTCOUNT]=0

	check_blink(id)
		
	if ( Verify_Skill(id, RACE_BLOOD, SKILL1) ){			// Pheonix
		new Float:randomnumber = random_float(0.0,1.0)
		new teamnumber = get_user_team(id)
		if (randomnumber <= p_pheonix[p_data[id][P_SKILL1]-1]){
			p_data_b[id][PB_PHEONIXCASTER] = true
			if (teamnumber==1)
				g_pheonixExistsT++
			else
				g_pheonixExistsCT++
		}
	}
	icon_controller(id,ICON_SHOW)

	return PLUGIN_CONTINUE
}

public item_functions(parm[]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("item_functions",parm[0])
	#endif

	new id = parm[0]
	
	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(p_data[id][P_ITEM2]==ITEM_MOLE)
		set_task(0.1,"_Item_Mole",TASK_FAN+12,parm,1)
		
	if(p_data_b[id][PB_DIEDLASTROUND]){
		p_data[id][P_ITEM]=0
		p_data[id][P_ITEM2]=0
		WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
	}

	p_data_b[id][PB_DIEDLASTROUND]=false
	
	if(get_cvar_num("sv_gravity")>650){
		if ( Verify_Skill(id, RACE_UNDEAD, SKILL3) ){		// Levitation
			if (get_user_gravity(id)!=p_levitation[p_data[id][P_SKILL3]-1])
				set_user_gravity(id,p_levitation[p_data[id][P_SKILL3]-1])
		}
		else if (p_data[id][P_ITEM2] == ITEM_SOCK)
			set_user_gravity(id, fCvar[FT_SOCK])
		else
			set_user_gravity(id,1.0)
	}
	else
		set_user_gravity(id,1.0)

	if(p_data[id][P_ITEM2]!=ITEM_CHAMELEON && (p_data[id][P_SKINCHANGED] == SKIN_HEX || p_data[id][P_SKINCHANGED]==SKIN_SWITCH))
		changeskin(id,SKIN_RESET)

	if (task_exists(TASK_ITEM_RINGERATE+id) && p_data[id][P_ITEM2]!=ITEM_RING)
		remove_task(TASK_ITEM_RINGERATE+id)

	if (p_data[id][P_ITEM]==ITEM_HEALTH)
		set_user_health(id,get_user_health(id)+iCvar[FT_HEALTH_BONUS])
	
	if(p_data[id][P_ITEM2]==ITEM_RING && !task_exists(TASK_ITEM_RINGERATE+id))
		_Item_Ring(parm)

	if (p_data[id][P_ITEM2]==ITEM_CHAMELEON && is_user_alive(id))
		changeskin(id,SKIN_SWITCH)

	return PLUGIN_CONTINUE
}


public resetSkills(id,saychat){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("resetSkills",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%s %L",g_MODclient, id,"CHEATING_DEATH_NOT_INSTALLED")
			return PLUGIN_CONTINUE
		}
	}

	if(saychat==1){
		client_print(id,print_center,"%L",id,"SKILLS_RESET_NEXT_ROUND")
	}
	else{
		console_print(id,"%L",id,"SKILLS_RESET_NEXT_ROUND")
	}
	p_data_b[id][PB_RESETSKILLS]=true


	return PLUGIN_HANDLED
}

public reset_position(parm[4]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("reset_position",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new origin[3]
	origin[0]=parm[1]
	origin[1]=parm[2]
	origin[2]=parm[3]
	set_user_origin(id,origin)

	return PLUGIN_CONTINUE
}

public weapon_controller( parm[2]  ){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("weapon_controller",parm[0])
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	#if MOD == 0
		new id = parm[0]
		
		if(!p_data_b[id][PB_ISCONNECTED])
			return PLUGIN_CONTINUE

		new bool:reincarnate = false
		
		// Give items because of respawning...
		if(p_data_b[id][PB_GIVEITEMS]){
			reincarnate = true
			p_data_b[id][PB_GIVEITEMS]=false
		}
									
		// Equipement & Eligibility Check for Re-Incarnation
		if (p_data_b[id][PB_DIEDLASTROUND]){	// DIED LAST ROUND		
			new Float:randomnumber = random_float(0.0,1.0)   
			if ( Verify_Skill(id, RACE_ORC, SKILL3) ){
				if( randomnumber <= p_ankh[p_data[id][P_SKILL3]-1] ){
					reincarnate = true				
				}
			}
			if (p_data[id][P_ITEM]==ITEM_ANKH){
				reincarnate=true
			}	
		}

		if (reincarnate){
				client_cmd(id, "speak warcraft3/soundpack/reincarnation.wav")

				if (iglow[id][1] < 1){
					parm[0] = id
					set_task(0.1,"glow_change",TASK_GLOW+id,parm,2)
				} 
				iglow[id][1] += 100
				iglow[id][0] = 0
				iglow[id][2] = 0
				iglow[id][3] = 0
				if (iglow[id][1]>MAXGLOW)
					iglow[id][1]=MAXGLOW

				// Screen fade green
				Create_ScreenFade(id, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[id][1])

				_WAR3_Drop_Items(id)
				_WAR3_Give_Items(id)
		}else{
			if(!cs_get_user_nvg(id))
				p_data_b[id][PB_NIGHTVISION]=false
			if(!cs_get_user_defuse(id))
				p_data_b[id][PB_DEFUSE]=false
		}
	#endif

	item_functions(parm)

	return PLUGIN_HANDLED	
}	

public saveweapons(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("saveweapons",id)
	#endif

	#if MOD == 0
		p_data[id][P_ARMORONDEATH]=get_user_armor(id)
	#endif
	for (new i=0; i<32; ++i){		// Equipment Reincarnation
		savedweapons[id][i]=0
	}
	p_data[id][P_SAVEDNUMBER]=0
	get_user_weapons(id,savedweapons[id],p_data[id][P_SAVEDNUMBER])
	return PLUGIN_CONTINUE
}

public apacheexplode(parm[5]){		// Suicide Bomber
	new id = parm[0]
	// random explosions

	new origin[3], origin2[3]
	origin[0] = parm[2]
	origin[1] = parm[3]
	origin[2] = parm[4]

	origin2[0] = origin[0] + random_num( -100, 100 )
	origin2[1] = origin[1] + random_num( -100, 100 )
	origin2[2] = origin[2] + random_num( -50, 50 )
	
	if(!g_mapDisabled)
		Create_TE_EXPLOSION(origin, origin2, g_sFireball, (random_num(0,20) + 20), 12, TE_EXPLFLAG_NONE)

#if MOD == 0
	if(!g_mapDisabled)
		Create_TE_Smoke(origin, origin2, g_sSmoke, 60, 10)
#endif

	new players[32]
	new numberofplayers
	get_players(players, numberofplayers)
	new i
	new targetid
	new distancebetween
	new targetorigin[3]
	new damage
	new multiplier

	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]
		get_user_origin(targetid,targetorigin)
		distancebetween=get_distance(origin,targetorigin)
		if (distancebetween < EXPLOSION_RANGE && get_user_team(id)!=get_user_team(targetid) && p_data[targetid][P_ITEM]!=ITEM_NECKLACE && !p_data_b[targetid][PB_WARDENBLINK]){
			multiplier=(EXPLOSION_MAX_DAMAGE*EXPLOSION_MAX_DAMAGE)/EXPLOSION_RANGE
			damage=(EXPLOSION_RANGE-distancebetween)*multiplier
			damage=sqroot(damage)
			if(is_user_alive(targetid))
				WAR3_damage(targetid, id, damage, CSW_SUICIDE, 0)

		}
		if (distancebetween < EXPLOSION_RANGE){
			Create_ScreenShake(targetid, (1<<14), (1<<13), (1<<14))
		}
	}

	--parm[1]
	if (parm[1]>0)
		set_task(0.1,"apacheexplode",TASK_EXPLOSION+id,parm,5)
	return PLUGIN_CONTINUE
}

public blastcircles(parm[5]){
	// Create Blast Circles
	if(g_mapDisabled)
		return PLUGIN_CONTINUE

	new origin[3], origin2[3]

	origin[0] = parm[2]
	origin[1] = parm[3]
	origin[2] = parm[4] - 16

	origin2[0] = origin[0]
	origin2[1] = origin[1]
	origin2[2] = origin[2] + EXPLOSION_BLAST_RADIUS

	Create_TE_BEAMCYLINDER(origin, origin, origin2, g_sSpriteTexture, 0, 0, 6, 16, 0, 188, 220, 255, 255, 0)

	origin2[2] = (origin[2] - EXPLOSION_BLAST_RADIUS) + (EXPLOSION_BLAST_RADIUS / 2 )

	Create_TE_BEAMCYLINDER(origin, origin, origin2, g_sSpriteTexture, 0, 0, 6, 16, 0, 188, 220, 255, 255, 0)

	return PLUGIN_CONTINUE
}


public glow_change(parm[]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("glow_change",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if ( Verify_Skill(id, RACE_HUMAN, SKILL1) ){	// Don't glow if player is invisible
		iglow[id][0] = 0
		iglow[id][1] = 0
		iglow[id][2] = 0
		iglow[id][3] = 0
	}
	else if ( p_data[id][P_ITEM]==ITEM_CLOAK ){		// Don't glow if player is invisible
		iglow[id][0] = 0
		iglow[id][1] = 0
		iglow[id][2] = 0
		iglow[id][3] = 0
	}
	else if (iglow[id][0] > 5){
		set_user_rendering(id,kRenderFxGlowShell,iglow[id][0],0,0, kRenderNormal, 16)
		iglow[id][0] -= 5
		set_task(0.2,"glow_change",TASK_GLOW+id,parm,2)
	}
	else if (iglow[id][1] > 5){
		set_user_rendering(id,kRenderFxGlowShell,0,iglow[id][1],0, kRenderNormal, 16)
		iglow[id][1] -= 5
		set_task(0.2,"glow_change",TASK_GLOW+id,parm,2)
	}
	else if (iglow[id][2] > 5){
		set_user_rendering(id,kRenderFxGlowShell,0,0,iglow[id][2], kRenderNormal, 16)
		iglow[id][2] -= 5
		set_task(0.2,"glow_change",TASK_GLOW+id,parm,2)
	}
	else if (iglow[id][3] > 5){
		set_user_rendering(id,kRenderFxGlowShell,iglow[id][3],iglow[id][3],iglow[id][3], kRenderNormal, 16)
		iglow[id][3] -= 5
		set_task(0.2,"glow_change",TASK_GLOW+id,parm,2)
	}
	else{
		iglow[id][0] = 0
		iglow[id][1] = 0
		iglow[id][2] = 0
		iglow[id][3] = 0
		set_user_rendering(id)
	}
	return PLUGIN_CONTINUE
}

public reset_maxspeed(parm[]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("reset_maxspeed",parm[0])
	#endif

	new enemy = parm[0]

	if(!p_data_b[enemy][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new normalspeed = parm[1]
	p_data_b[enemy][PB_STUNNED]=false
	p_data_b[enemy][PB_SLOWED]=false
	set_user_maxspeed(enemy, float(normalspeed))
	return PLUGIN_CONTINUE
}

public unholyspeed(parm[1]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("unholyspeed",parm[0])
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (p_data_b[id][PB_STUNNED]){
		set_user_maxspeed(id,1.0)
		return PLUGIN_HANDLED
	}
	else if (p_data_b[id][PB_SLOWED]){
		set_user_maxspeed(id,fCvar[FT_FROST_SPEED])
		return PLUGIN_HANDLED
	}
#if MOD == 1
	else if(entity_get_int(id,EV_INT_iuser3)){	// User is in prone position, don't change speed
		if(get_user_maxspeed(id) > 500.0)
			set_user_maxspeed(id, 50.0)

		return PLUGIN_HANDLED
	}
	else if(get_user_maxspeed(id) == 50.0){		// User has a rocket launcher "mounted"
		set_user_maxspeed(id, 600.0)
	}
#endif
#if MOD == 0
	else if (get_user_maxspeed(id)>5 && Verify_Skill(id, RACE_UNDEAD, SKILL2) && !g_freezetime){              // Unholy Aura
		if (get_user_maxspeed(id)>5 && get_user_maxspeed(id)!=p_unholy[p_data[id][P_SKILL2]-1])
			set_user_maxspeed(id,(p_unholy[p_data[id][P_SKILL2]-1]))
	}
	else if (get_user_maxspeed(id)>5 && p_data[id][P_ITEM]==ITEM_BOOTS && !g_freezetime){			// Boots of Speed
		if (get_user_maxspeed(id)!=fCvar[FT_BOOTSPEED])
			set_user_maxspeed(id,fCvar[FT_BOOTSPEED])
	}
#endif

	return PLUGIN_CONTINUE
}

public ultimate(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("ultimate",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			client_print(id,print_chat,"%L",id,"CHEATING_DEATH_NOT_INSTALLED",g_MOD)
			return PLUGIN_HANDLED
		}
	}

	if (!is_user_alive(id))
		return PLUGIN_HANDLED

	if(!p_data[id][P_ULTIMATE]){
		new message[128]
		format(message, 127, "%L",id,"ULTIMATE_NOT_FOUND")
		Status_Text(id, message, 0.5, HUDMESSAGE_POS_STATUS)
		client_cmd(id, "speak warcraft3/bonus/Error.wav")
		return PLUGIN_HANDLED
	}

	if(p_data_b[id][PB_ULTIMATEUSED]){
		new message[128]
		format(message, 127, "%L",id,"ULTIMATE_NOT_READY",p_data[id][P_ULTIMATEDELAY])
		Status_Text(id, message, 0.5, HUDMESSAGE_POS_STATUS)
		client_cmd(id, "speak warcraft3/bonus/Error.wav")
		return PLUGIN_HANDLED
	}

	// Locust Swarm
	if ( Verify_Skill(id, RACE_CRYPT, SKILL4) ){
		calllocustfunction(id)
	}

	// Suicide Bomber
	else if ( Verify_Skill(id, RACE_UNDEAD, SKILL4) ){
		if (iCvar[FT_WARN_SUICIDE]){
			if( p_data_b[id][PB_SUICIDEATTEMPT] ){
				set_msg_block(gmsgDeathMsg,BLOCK_ONCE)
			#if MOD == 0
				user_kill(id)
			#endif
			#if MOD == 1
				dod_user_kill(id)
				on_Death(id, 0, 0, 0)
				//client_cmd(id,"kill")
			#endif
			
				p_data_b[id][PB_SUICIDEATTEMPT] = false
			}
			else{
				new parm[1]
				parm[0]=id
				icon_controller(id,ICON_FLASH)
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
		else{
			set_msg_block(gmsgDeathMsg,BLOCK_ONCE)
		#if MOD == 0
			user_kill(id)
		#endif
		#if MOD == 1
			//client_cmd(id,"kill")
			dod_user_kill(id)
			on_Death(id, 0, 0, 0)
		#endif

		}
	}

	// Big Bad Voodoo
	else if ( Verify_Skill(id, RACE_SHADOW, SKILL4) ){
		new parm[2]
		parm[0] = id
		parm[1] = 1
		_Ultimate_BigBadVoodoo(parm)
	}

	// Flame Strike
	else if ( Verify_Skill(id, RACE_BLOOD, SKILL4) ){
		fire_flamethrower(id) 
		p_data[id][P_FLAMECOUNT]++
		if(p_data[id][P_FLAMECOUNT]>5){
			p_data_b[id][PB_ULTIMATEUSED]=true
			icon_controller(id,ICON_HIDE)
			p_data[id][P_FLAMECOUNT]=0
		}
	}

	// Teleport
	else if ( Verify_Skill(id, RACE_HUMAN, SKILL4) ){
		Ultimate_Teleport(id)
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

	// Vengeance
	else if ( Verify_Skill(id, RACE_WARDEN, SKILL4) ){
		Ultimate_Warden(id)
	}

	return PLUGIN_HANDLED
}

public searchtarget(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("searchtarget",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new enemy, body
	get_user_aiming(id,enemy,body)

	if ( 0<enemy<=MAXPLAYERS &&!p_data_b[enemy][PB_STUNNED] && get_user_team(id)!=get_user_team(enemy) && p_data[enemy][P_ITEM]!=ITEM_NECKLACE && !p_data_b[enemy][PB_WARDENBLINK] && is_user_alive(id) && is_user_alive(enemy)){
		p_data_b[id][PB_ISSEARCHING]=false
		p_data_b[id][PB_ULTIMATEUSED]=true
		icon_controller(id,ICON_HIDE)

		if(!g_mapDisabled)
			Create_TE_BEAMFOLLOW(enemy, g_siTrail, 10, 5, 10, 108, 23, 255)

		emit_sound(id,CHAN_STATIC, SOUND_ENTANGLING, 1.0, ATTN_NORM, 0, PITCH_NORM)

		new waitparm[6]
		waitparm[0]=enemy
		waitparm[1]=100
		waitparm[5]=floatround(get_user_maxspeed(enemy))
		set_user_maxspeed(enemy,1.0)			
		waitstop(waitparm)
		
		p_data_b[enemy][PB_STUNNED]=true
		new cooldownparm[2]
		cooldownparm[0]=id

		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_WAR3_Ultimate_Delay(cooldownparm)	
	}
	else{
		p_data_b[id][PB_ISSEARCHING]=true
		icon_controller(id,ICON_FLASH)
		new counter = parm[1]
		while (counter >= 0){
			counter -= 10
			if (counter==0){
				emit_sound(id,CHAN_STATIC, SOUND_ULTIMATESCAN, 1.0, ATTN_NORM, 0, PITCH_NORM)
			}
		}			
		--parm[1]
		if (parm[1]>0 && get_user_health(id)>0 && !endround){
			set_task(0.1,"searchtarget",TASK_SEARCHTARGET+id,parm,2)
		}else{
			p_data_b[id][PB_ISSEARCHING]=false
			icon_controller(id,ICON_SHOW)
		}
	}
	return PLUGIN_CONTINUE
}

public waitstop(parm[6]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("waitstop",parm[0])
	#endif

	new id=parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new origin[3]
	get_user_origin(id, origin)
	if (origin[0]==parm[2] && origin[1]==parm[3] && origin[2]==parm[4]){
		new normalspeed = parm[5]
		new resetparm[2]
		resetparm[0]=id
		resetparm[1]=normalspeed
		set_task(float(parm[1]/10),"reset_maxspeed",TASK_RESETSPEED+id,resetparm,2)
		new entangleparm[2]
		entangleparm[0]=parm[0]
		entangleparm[1]=parm[1]
		entangle(entangleparm)
	}
	else{
		parm[2]=origin[0]
		parm[3]=origin[1]
		parm[4]=origin[2]
		set_task(0.1,"waitstop",TASK_WAITSTOP+id,parm,6)
	}
	return PLUGIN_CONTINUE
}

public entangle(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("entangle",parm[0])
	#endif

	new id=parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new life=parm[1]
	new radius = 20
	new counter = 0
	new origin[3]
	new x1
	new y1
	new x2
	new y2
	get_user_origin(id,origin)

	emit_sound(id,CHAN_STATIC, SOUND_ENTANGLING, 1.0, ATTN_NORM, 0, PITCH_NORM)

	new parm2[2]
	parm2[0]=id

	p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
	_WAR3_Ultimate_Delay(parm2)


	while (counter<=7){
		if (counter==0 || counter==8)
			x1= -radius
		else if (counter==1 || counter==7)
			x1= -radius*100/141
		else if (counter==2 || counter==6)
			x1= 0
		else if (counter==3 || counter==5)
			x1= radius*100/141
		else if (counter==4)
			x1= radius
		if (counter<=4)
			y1 = sqroot(radius*radius-x1*x1)
		else
			y1 = -sqroot(radius*radius-x1*x1)
		++counter
		if (counter==0 || counter==8)
			x2= -radius
		else if (counter==1 || counter==7)
			x2= -radius*100/141
		else if (counter==2 || counter==6)
			x2= 0
		else if (counter==3 || counter==5)
			x2= radius*100/141
		else if (counter==4)
			x2= radius
		if (counter<=4)
			y2 = sqroot(radius*radius-x2*x2)
		else
			y2 = -sqroot(radius*radius-x2*x2)
		new height=16+2*counter
		new start[3], end[3]
		if(!g_mapDisabled){
			while (height > -40){
				
				start[0] = origin[0] + x1
				start[1] = origin[1]+y1
				start[2] = origin[2]+height
				end[0] = origin[0]+x2
				end[1] = origin[1]+y2
				end[2] = origin[2]+height+2
				
				Create_TE_BEAMPOINTS(start, end, g_siBeam4, 0, 0, life, 10, 5, 10, 108, 23, 255, 0)

				height -= 16
			}
		}
	}
	return PLUGIN_CONTINUE
}

public lightsearchtarget(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("lightsearchtarget",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new enemy, body
	get_user_aiming(id,enemy,body)

	if ( 0<enemy<=MAXPLAYERS && get_user_team(id)!=get_user_team(enemy) && p_data[enemy][P_ITEM]!=ITEM_NECKLACE && !p_data_b[enemy][PB_WARDENBLINK] && is_user_alive(id) && is_user_alive(enemy)){
		p_data_b[id][PB_ULTIMATEUSED]=true
		icon_controller(id,ICON_HIDE)
		new linewidth = 80
		new damage = 50

		p_data_b[id][PB_ISSEARCHING]=false
		lightningeffect(id,enemy,linewidth,damage,id)
		new lightparm[4]
		lightparm[0]=enemy
		lightparm[1]=damage
		lightparm[2]=linewidth
		lightparm[3]=id
		set_task(0.2,"lightningnext",TASK_LIGHTNING+enemy,lightparm,4)
		new cooldownparm[2]
		cooldownparm[0]=id

		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_WAR3_Ultimate_Delay(cooldownparm)
	}
	else{
		p_data_b[id][PB_ISSEARCHING]=true
		icon_controller(id,ICON_FLASH)
		new counter = parm[1]
		while (counter >= 0){
			counter -= 10
			if (counter==0)
				emit_sound(id,CHAN_STATIC, "turret/tu_ping.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
		}
		--parm[1]	
		if (parm[1]>0 && get_user_health(id)>=0 && !endround){
			set_task(0.1,"lightsearchtarget",TASK_LIGHTSEARCH+id,parm,2)
		}else{
			p_data_b[id][PB_ISSEARCHING]=false
			icon_controller(id,ICON_SHOW)
		}	
	}
	return PLUGIN_CONTINUE
}


public lightningnext(parm[4]){		// Chain Lightning
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("lightningnext",parm[0])
	#endif

	new id=parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new caster=parm[3]
	new origin[3]
	get_user_origin(id, origin)
	new players[32]
	new teamname[32]
	get_user_team(id, teamname, 31)
	new numberofplayers
	get_players(players, numberofplayers,"ae",teamname)
	new i
	new targetid = 0
	new distancebetween = 0
	new targetorigin[3]
	new damage = parm[1]*2/3
	new linewidth = parm[2]*2/3
	new closestdistance = 0
	new closestid = 0
	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]
		if (get_user_team(id)==get_user_team(targetid) && is_user_alive(targetid) && is_user_alive(id)){
			get_user_origin(targetid,targetorigin)
			distancebetween=get_distance(origin,targetorigin)
			if (distancebetween < LIGHTNING_RANGE && !p_data_b[targetid][PB_LIGHTNINGHIT] && p_data[targetid][P_ITEM]!=ITEM_NECKLACE && !p_data_b[targetid][PB_WARDENBLINK]){
				if (distancebetween < closestdistance || closestid==0){
					closestdistance = distancebetween
					closestid = targetid
				}
			}
		}
	}

	if (closestid){
		lightningeffect(id,closestid,linewidth,damage,caster)
		parm[0]=targetid
		parm[1]=damage
		parm[2]=linewidth
		parm[3]=caster
		set_task(0.2,"lightningnext",TASK_LIGHTNINGNEXT+id,parm,4)
	}
	else{
		for (i = 0; i < numberofplayers; ++i){
			targetid=players[i]
			p_data_b[targetid][PB_LIGHTNINGHIT]=false
		}
	}
	return PLUGIN_CONTINUE
}

public lightningeffect(id,targetid,linewidth,damage,caster){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("lightningeffect",id)
	#endif

	p_data_b[targetid][PB_LIGHTNINGHIT]=true

	new parm[1]
	parm[0]=id
	icon_controller(id,ICON_HIDE)

	WAR3_damage(targetid, caster, damage, CSW_LIGHTNING, 0)

#if MOD == 0
	if (get_user_armor(targetid)-damage<=0)
		set_user_armor(targetid,0)
	else
		set_user_armor(targetid,get_user_armor(targetid)-damage)
#endif

	if(!g_mapDisabled)
		Create_TE_BEAMENTS(id, targetid, g_sLightning, 0, 15, 10, linewidth, 10, 255, 255, 255, 255, 0)

	new origin[3]
	get_user_origin(targetid,origin)

	Create_TE_ELIGHT(targetid, origin, 100, 255, 255, 255, 10, 0)

	emit_sound(id,CHAN_STATIC, SOUND_LIGHTNING, 1.0, ATTN_NORM, 0, PITCH_NORM)

	return PLUGIN_CONTINUE
}

public func_spawn(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("func_spawn",parm[0])
	#endif

	if (endround){
		set_user_money(parm[0],get_user_money(parm[0])+itemcost2[0],1)
		return PLUGIN_HANDLED
	}

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(get_user_team(id)==UNASSIGNED)
		return PLUGIN_CONTINUE

	p_data[id][P_ITEM]=0
	p_data[id][P_ITEM2]=0
	
	if (is_user_alive(id))
		return PLUGIN_CONTINUE

	new team[32]
	get_user_team(id,team,31)
	p_data_b[id][PB_PLAYERSPAWNED]=true
	icon_controller(id,ICON_SHOW)
	spawn(id)
	p_data_b[id][PB_SLOWED]=false
	p_data_b[id][PB_STUNNED]=false
	spawn(id)
	set_task(0.3,"spawn_player",TASK_SPAWNPLAYER+id,parm,2)
	set_task(0.5,"player_giveitems",TASK_GIVEITEMS+id,parm,2)

#if MOD == 0
	give_item(id, "weapon_knife")
#endif
	
	new money = get_user_money(id)

	#if MOD == 0
		if (money < get_cvar_num("mp_startmoney"))
			set_user_money(id,get_cvar_num("mp_startmoney"),0)
	#endif
	#if MOD == 1
		if (money < iCvar[DOD_STARTMONEY])
			set_user_money(id,iCvar[DOD_STARTMONEY],0)

	#endif
	return PLUGIN_CONTINUE
}


public player_giveitems(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("player_giveitems",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	// Vengeance
	if( Verify_Skill(id, RACE_WARDEN, SKILL4) && !p_data_b[id][PB_SPAWNEDFROMITEM])
		set_user_health(id,50)

	// Evasion
	else if ( Verify_Skill(id, RACE_ELF, SKILL1) && p_data_b[id][PB_EVADENEXTSHOT])
		set_user_health(id,1124)

	// Devotion Aura
	else if ( Verify_Skill(id, RACE_HUMAN, SKILL2))							
		set_user_health(id,p_devotion[p_data[id][P_SKILL2]-1])

	check_blink(id)
	p_data_b[id][PB_GIVEITEMS]=true
	set_task(0.1, "weapon_controller", TASK_REINCARNATION+id, parm, 2)							// Give weapons back

	give_item(id, "item_suit")

	#if MOD == 0
		give_item(id, "weapon_knife")
	#endif

	// The following code decides if pistols should be given
	#if MOD == 0
		if (!g_givePistol)
			return PLUGIN_CONTINUE

		new wpnList[32] = 0, number = 0, foundGlock = false, foundUSP = false 
		get_user_weapons(id,wpnList,number)
		for (new i = 0;i < number;i++) { 
			if (wpnList[i] == CSW_GLOCK18) 
				foundGlock = true 
			if (wpnList[i] == CSW_USP) 
				foundUSP = true 
		} 
		if (get_user_team(id)==TS){
			if(foundGlock)
				return PLUGIN_CONTINUE
			else{
				give_item(id,"weapon_glock18")
				give_item(id,"ammo_9mm")
				give_item(id,"ammo_9mm")
				give_item(id,"ammo_9mm")
			}

		}
		if (get_user_team(id)==CTS){
			if(foundUSP)
				return PLUGIN_CONTINUE
			else{
				give_item(id,"weapon_usp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
			}
		}
	#endif
	return PLUGIN_CONTINUE

}

public spawn_player(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("spawn_player",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	p_data_b[id][PB_PLAYERSPAWNED]=true
	spawn(id)
	give_item(id, "item_suit")
#if MOD == 0
	give_item(id, "weapon_knife")
#endif
	return PLUGIN_CONTINUE

}
