public Ultimate_Warden(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("Ultimate_Warden",id)
	#endif

	new spawnID, playersInVicinity, entList[1], origin[3], i
	new ent = -1
	new Float:spawnOrigin[3]
	new Float:vicinity = 96.0		//(32x32x96)
	new bool:found = false
	new bool:immunityNear = false
	if(get_user_team(id) == CTS)
		spawnID = 0
	else
		spawnID = 1

	do {
		ent = find_ent_by_class(ent,spawnEntString[spawnID])
		if (ent != 0) {
			entity_get_vector(ent,EV_VEC_origin,spawnOrigin)
			for(i=0;i<3;i++)
				origin[i] = floatround(spawnOrigin[i])

			playersInVicinity = find_sphere_class(0, "player", vicinity, entList, 1, spawnOrigin)
			if(!WAR3_Immunity_Found_Near(id, origin)){
				if (playersInVicinity == 0)
					found = true
				immunityNear = false
			}
			else
				immunityNear = true
		}
	}
	while (ent && !found)

	if (!found){
		set_hudmessage(255, 255, 10, -1.0, -0.4, 1, 0.5, 1.5, 0.2, 0.2,5)

		if(immunityNear)
			show_hudmessage(id,"%L",id,"TELEPORT_FAILED_ENEMY_IMMUNITY")
		else
			show_hudmessage(id,"%L",id,"NO_FREE_SPAWN_FOUND")
	}
	else{
		get_user_origin(id,origin)

		Create_TE_TELEPORT(origin)

		for(i=0;i<3;i++)
			origin[i] = floatround(spawnOrigin[i])

		set_user_origin(id, origin)

		if(get_user_health(id)<50)
			set_user_health(id,50)
	
		emit_sound(id,CHAN_STATIC, SOUND_VENGEANCE, 1.0, ATTN_NORM, 0, PITCH_NORM)

		p_data_b[id][PB_ULTIMATEUSED]=true

		icon_controller(id,ICON_HIDE)

		new parm[2]
		parm[0]=id
		
		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_WAR3_Ultimate_Delay(parm)
	}
}

public Ultimate_Teleport(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("Ultimate_Teleport",id)
	#endif

	if(iCvar[FT_BLINKENABLED]){

		if(get_user_maxspeed(id)<=10)
			return PLUGIN_HANDLED
		
		new oldLocation[3], newLocation[3]
		new parm[3], teleparm[6]
		parm[0] = id	

		// Get the player's current location
		get_user_origin(id, oldLocation)

		// Get where the player is looking (where the player will teleport)
		get_user_origin(id, newLocation, 3)	

		// Make sure it doesn't teleport you back into the ground
		oldLocation[ZPOS] += 30 

		// Change coordinates to make sure player won't get stuck in the ground/wall
		if((newLocation[XPOS] - oldLocation[XPOS]) > 0)
			newLocation[XPOS] -= 50
		else
			newLocation[XPOS] += 50
		
		if((newLocation[YPOS] - oldLocation[YPOS]) > 0)
			newLocation[YPOS] -= 50
		else
			newLocation[YPOS] += 50
		
		newLocation[ZPOS] += 40				

		// Makes sure nearby enemy doesn't have immunity
		
		if(WAR3_Immunity_Found_Near(id, oldLocation) || WAR3_Immunity_Found_Near(id, newLocation)){
			set_hudmessage(255, 255, 10, -1.0, -0.4, 1, 0.5, BLINK_COOLDOWN, 0.2, 0.2,5)
			show_hudmessage(id,"%L",id,"TELEPORT_FAILED_ENEMY_IMMUNITY")
			
			new coolparm[2]
			coolparm[0] = id

			p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]

			_WAR3_Ultimate_Delay(coolparm)

			emit_sound(id,CHAN_STATIC, SOUND_BLINK, 1.0, ATTN_NORM, 0, PITCH_NORM)

			return PLUGIN_HANDLED
		}
	
		emit_sound(id,CHAN_STATIC, SOUND_BLINK, 1.0, ATTN_NORM, 0, PITCH_NORM)
		
		new origin[3], origin2[3]
		origin[0] = oldLocation[0]
		origin[1] = oldLocation[1]
		origin[2] = oldLocation[2] + 10
		origin2[0] = oldLocation[0]
		origin2[1] = oldLocation[1]
		origin2[2] = oldLocation[2] + 10 + TELEPORT_RADIUS

		// blast circles
		Create_TE_BEAMCYLINDER(oldLocation, origin, origin2, g_sSpriteTexture, 0, 0, 3, 60, 0, 255, 255, 255, 255, 0)
		
		// Stop bomb planting...
		client_cmd(id,"-use")

		if (iCvar[FT_BLINK_PROTECTION]){
			new mapname[32]
			get_mapname(mapname,32) 
			if (equali(mapname,"as_oilrig")){
				if (newLocation[0]>1530 && get_user_team(id)==CTS){
					set_msg_block(gmsgDeathMsg,BLOCK_ONCE)
					user_kill(id)
					client_print(id,print_chat,"%s %L",g_MODclient, id,"SLAIN_FOR_TELEPORTING")
				
					return PLUGIN_HANDLED
				}
			}

			set_task(1.5, "ceiling_check", TASK_CEILING+id, parm, 2)	
		}

		// Test sending player, should work most of the time.
		set_user_origin(id, newLocation)

		// Check if Blink landed you in a wall, if so, abort
		teleparm[0] = id
		teleparm[1] = 1
		teleparm[2] = oldLocation[0]
		teleparm[3] = oldLocation[1]
		teleparm[4] = oldLocation[2]
		teleparm[5] = newLocation[2]
		
		set_task(0.1, "_Ultimate_Blink_Controller", TASK_BLINKCONT+id, teleparm, 6)
						
		p_data_b[id][PB_ULTIMATEUSED]=true
		icon_controller(id,ICON_HIDE)

		emit_sound(id, CHAN_STATIC, SOUND_BLINK, 1.0, ATTN_NORM, 0, PITCH_NORM)

		origin[0] = oldLocation[0]
		origin[1] = oldLocation[1]
		origin[2] = oldLocation[2] + 90
		origin2[0] = oldLocation[0]
		origin2[1] = oldLocation[1]
		origin2[2] = oldLocation[2] + 90 + TELEPORT_RADIUS

		Create_TE_BEAMCYLINDER(oldLocation, origin, origin2, g_sSpriteTexture, 0, 0, 3, 60, 0, 255, 255, 255, 255, 0)		
	}
	else{
		new i
		new j
		new tmp
		new numberofplayers
		new targetid
		new targetid2
		new teamname[32]
		new distancebetween
		new distancebetween2
		new origin[3]
		new targetorigin[3]
		new targetorigin2[3]
		get_user_team(id, teamname, 31)
		get_players(teleportid[id],numberofplayers,"ae",teamname)
		get_user_origin(id,origin)
		for (i=0; i<numberofplayers; i++) {	// Sort by closest to furthest
			for (j=i+1; j<numberofplayers; j++){
				targetid=teleportid[id][i]
				get_user_origin(targetid,targetorigin)
				distancebetween = get_distance(origin,targetorigin)
				targetid2=teleportid[id][j]
				get_user_origin(targetid2,targetorigin2)
				distancebetween2 = get_distance(origin,targetorigin2)

				if (distancebetween2 < distancebetween && targetid2!=id) {	// Don't put self first
					tmp = teleportid[id][i]
					teleportid[id][i] = teleportid[id][j]
					teleportid[id][j] = tmp
				}
				else if (targetid==id) {	// Put self last
					tmp = teleportid[id][i]
					teleportid[id][i] = teleportid[id][j]
					teleportid[id][j] = tmp
				}
			}
		}

		if (numberofplayers > 9)
			numberofplayers=9
		else
			--numberofplayers	// Remove self from list

		if (numberofplayers){
			remove_task(666+id)
			p_data[id][P_TELEMENU]=true
			new menuparm[2]
			menuparm[0]=id
			menuparm[1]=numberofplayers
			_Ultimate_Telemenu(menuparm)
		}
		else{
			set_hudmessage(178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 5)
			show_hudmessage(id,"%L",id,"NO_VALID_TARGETS_FOUND")
		}
	}
	return PLUGIN_CONTINUE
}

public _Ultimate_Blink_Controller(parm[]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Ultimate_Blink_Controller",parm[0])
	#endif


	new id = parm[0]	 

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new oldLocation[3]
	new origin[3]

	oldLocation[0] = parm[2]
	oldLocation[1] = parm[3]
	oldLocation[2] = parm[4]

	// Teleport failure check and unsticker
	new coolparm[2]
	coolparm[0] = id

	get_user_origin(id, origin)

	if(parm[5] == origin[2]){ // Teleportation Failure

		set_hudmessage(255, 255, 10, -1.0, -0.4, 1, 0.5, BLINK_COOLDOWN, 0.2, 0.2,5)
		show_hudmessage(id,"%L",id,"TELEPORT_FAILED_BAD_DESTINATION")
		
		set_user_origin(id, oldLocation)	

		p_data[id][P_ULTIMATEDELAY] = floatround(BLINK_COOLDOWN + 1.0)

		_WAR3_Ultimate_Delay(coolparm)

	}
	else{
		
		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_WAR3_Ultimate_Delay(coolparm)

		// Sprays white bubbles everywhere
		new origin2[3]
		origin2[0] = origin[0]
		origin2[1] = origin[1]
		origin2[2] = origin[2] + 40
		
		if(!g_mapDisabled)
			Create_TE_SPRITETRAIL(origin2, origin, g_sFlare, 30, 10, 1, 50, 10)

		// Flash the player
		if(iCvar[FT_BLINK_DIZZINESS] == 2)
			Create_ScreenFade(id, (1<<15), (1<<10), (1<<12), 0, 0, 255, 180)			
		else
			Create_ScreenFade(id, (1<<15), (1<<10), (1<<12), 255, 255, 255, 255)			
	}	
	
	return PLUGIN_CONTINUE
}

public _Ultimate_Telemenu(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Ultimate_Telemenu",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new numberofplayers = parm[1]
	new targetid
	new name[32]
	new origin[3]
	new targetorigin[3]
	new distancebetween
	new temp[64]
	new i
	new keys = (1<<9)

	new menu_body[512]
	format(menu_body,511,"%L",id,"MENU_TELEPORT_TO")

	get_user_origin(id,origin)
	for (i = 0; i < numberofplayers; ++i){
		targetid=teleportid[id][i]
		get_user_name(targetid,name,31)
		get_user_origin(targetid,targetorigin)
		distancebetween = get_distance(origin,targetorigin)
		if (is_user_alive(targetid)){
			format(temp,63,"^n\w%d. %s (\y%dm\w)",i+1,name,distancebetween/40)
			keys |= (1<<i)
		}
		else
			format(temp,63,"^n\d%d. %s",i+1,name)
		add(menu_body,255,temp)
	}
	format(temp,63,"%L",id,"CANCEL_TELEMENU")
	add(menu_body,255,temp)
	show_menu(id,keys,menu_body,-1)

	new menuparm[2]
	menuparm[0]=id
	menuparm[1]=numberofplayers
	if (p_data[id][P_TELEMENU])
		set_task(1.0,"_Ultimate_Telemenu",TASK_TELEMENU+id,parm,2)
	return PLUGIN_HANDLED
}

public _menu_Teleport(id,key){		// Teleport
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_menu_Teleport",id)
	#endif

	new targetid = teleportid[id][key]
	if(task_exists(TASK_TELEMENU+id))
		remove_task(TASK_TELEMENU+id)

	p_data[id][P_TELEMENU]=false

	client_cmd(id,"slot10")
		
	if (is_user_alive(id) && is_user_alive(targetid) && get_user_maxspeed(id)>10 && get_user_team(id)==get_user_team(targetid) && key!=9 && !p_data_b[id][PB_ULTIMATEUSED]){
		
		icon_controller(id,ICON_HIDE)

		p_data_b[id][PB_ULTIMATEUSED]=true

		new waitparm[6]
		waitparm[0] = id
		waitparm[1] = targetid
		waitparm[5] = floatround(get_user_maxspeed(id))

		set_user_maxspeed(id,1.0)

		p_data_b[id][PB_STUNNED]=true

		telewaitstop(waitparm)

		new parm[1]
		parm[0]=id

		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_WAR3_Ultimate_Delay(parm)
	}

	return PLUGIN_HANDLED
}

public telewaitstop(parm[6]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("telewaitstop",parm[0])
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
		set_task(0.6,"reset_maxspeed",TASK_RESETSPEED+id,resetparm,2)
		new teleportparm[6]
		teleportparm[0]=parm[0]
		teleportparm[1]=parm[1]
		teleport(teleportparm)
	}
	else{
		parm[2]=origin[0]
		parm[3]=origin[1]
		parm[4]=origin[2]
		set_task(0.1,"telewaitstop",TASK_TELEWAITSTOP+id,parm,6)
	}
	return PLUGIN_CONTINUE
}

public teleport(parm[6]){		// Teleport
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("teleport",parm[0])
	#endif

	new id=parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new thetarget=parm[1]
	new origin[3]
	get_user_origin(id,origin)
	new targetorigin[3]
	targetorigin[0]=parm[3]
	targetorigin[1]=parm[4]
	targetorigin[2]=parm[5]

	if (parm[2]==0){

		emit_sound(id,CHAN_STATIC, SOUND_TELEPORT, 1.0, ATTN_NORM, 0, PITCH_NORM)

		set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransAdd,255)

			// blast circles

		new origin2[3], origin3[3]
		origin2[0] = origin[0]
		origin2[1] = origin[1]
		origin2[2] = origin[2] + 10
		origin3[0] = origin[0]
		origin3[1] = origin[1]
		origin3[2] = origin[2] + 10 + TELEPORT_RADIUS

		if(!g_mapDisabled)
			Create_TE_BEAMCYLINDER(origin, origin2, origin3, g_sSpriteTexture, 0, 0, 3, 60, 0, 255, 255, 255, 255, 0)

		get_user_origin(thetarget,targetorigin)

		emit_sound(thetarget,CHAN_STATIC, SOUND_TELEPORT, 1.0, ATTN_NORM, 0, PITCH_NORM)

		origin2[0] = targetorigin[0]
		origin2[1] = targetorigin[1]
		origin2[2] = targetorigin[2] + 90
		origin3[0] = targetorigin[0]
		origin3[1] = targetorigin[1]
		origin3[2] = targetorigin[2] + 90 + TELEPORT_RADIUS

		if(!g_mapDisabled)
			Create_TE_BEAMCYLINDER(origin, origin2, origin3, g_sSpriteTexture, 0, 0, 3, 60, 0, 255, 255, 255, 255, 0)

		parm[3]=targetorigin[0]
		parm[4]=targetorigin[1]
		parm[5]=targetorigin[2]
	}

	if (parm[2]==1){
		targetorigin[2]+=80

		set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,128)

		set_user_origin(id, targetorigin)
		new origin2[3]
		origin2[0] = targetorigin[0]
		origin2[1] = targetorigin[1]
		origin2[2] = targetorigin[2] + 40

		if(!g_mapDisabled)
			Create_TE_SPRITETRAIL(origin2, targetorigin, g_sFlare, 30, 10, 1, 50, 10)
		
		new fadeinparm[3]
		fadeinparm[0]=id
		fadeinparm[1]=3
		fadeinparm[2]=targetorigin[2]
		teleportfadein(fadeinparm)
	}
	++parm[2]
	if (parm[2]<2)
		set_task(0.3,"teleport",TASK_TELEPORT+id,parm,6)
	return PLUGIN_CONTINUE
}

public teleportfadein(parm[3]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("teleportfadein",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (parm[1]==3)
		set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,170)
	if (parm[1]==2)
		set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,212)
	if (parm[1]==1){

		set_user_rendering(id)

		new origin[3]
		get_user_origin(id,origin)
		if (origin[2]==parm[2]){
			origin[2]-=80
			new unstickparm[4]
			unstickparm[0]=id
			unstickparm[1]=origin[0]
			unstickparm[2]=origin[1]
			unstickparm[3]=origin[2]
			unstickit(unstickparm)
		}
	}
	--parm[1]
	if (parm[1]>0)
		set_task(0.1,"teleportfadein",TASK_TELEFADEIN+id,parm,3)
	return PLUGIN_CONTINUE
}

public unstickit(unstickparm[]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("unstickit",unstickparm[0])
	#endif

	new id=unstickparm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new origin[3]
	new targetorigin[3]
	origin[0]=unstickparm[1]
	origin[1]=unstickparm[2]
	origin[2]=unstickparm[3]
	new players[32]
	new numberofplayers
	get_players(players, numberofplayers)
	new i
	new playerid
	new bool:clear
	clear = true
	new distancebetween
	new tolerance = 100
	for (i = 0; i < numberofplayers; ++i){
		playerid=players[i]
		get_user_origin(playerid,targetorigin)
		distancebetween = get_distance(origin,targetorigin)
		if (distancebetween<tolerance && is_user_alive(playerid) && playerid!=id){
			clear=false
		}
	}
	if (clear){
		set_user_origin(id,origin)
	}
	else{
		set_task(0.1,"unstickit",TASK_UNSTICK+id,unstickparm,4)
	}
	return PLUGIN_CONTINUE
}

public ceiling_check(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("ceiling_check",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new mapname[32]
	new slay=0
	get_mapname(mapname,32) 

	new Float:porigin[3]
	entity_get_vector(id,EV_VEC_origin,porigin)
//#define CONTENTS_SKY -6 

	if(PointContents(porigin)==-6)
		slay=1

	#if MOD == 0
		new origin[3]
		get_user_origin(id,origin)

		new x = origin[0]
		new y = origin[1]
		new z = origin[2]

		if (equali(mapname,"de_dust")){
			if(z>220)
				slay=1
		}
		else if (equali(mapname,"awp_assault")){
			if(z>520){
				if((y>2400 && y<2600))
					slay=1
			}
		}
		else if (equali(mapname,"de_dust_cz")){
			if(z>220)
			slay=1
		}
		else if (equali(mapname,"de_aztec_cz")){
			if(z>300)
				slay=1
		}
		else if (equali(mapname,"cs_assault_upc")){
			if(z>650)
				slay=1
		}
		else if (equali(mapname,"de_aztec")){
			if(z>300)
			slay=1
		}
		else if (equali(mapname,"de_cbble")){
			if (z>315){
				if((x>-1320 && x<-1150) && (y>2600 && y<2900))
					return PLUGIN_CONTINUE
				else
					slay=1
			}           
		}
		else if (equali(mapname,"de_cbble_cz")){
			if (z>315){
				if((x>-1320 && x<-1150) && (y>2600 && y<2900))
					return PLUGIN_CONTINUE
				else
					slay=1
			}           
		}
		else if (equali(mapname,"cs_assault")){
			if(z>700)
				slay=1
		}
		else if (equali(mapname,"cs_militia")){
			if(z>500)
				slay=1
		}
		else if (equali(mapname,"cs_militia_cz")){
			if(z>500)
				slay=1
		}
		else if (equali(mapname,"cs_italy")){
			if(z>-220){
				if(y<-2128)
					slay=1
			}
			if(z>250){
				if ((x<-1000 && x>-1648) && (y>1900 && y<2050))
					slay=1
				if ((x<-1552 && x>-1648) && (y>1520 && y<2050))
					slay=1
			}
		}
		else if (equali(mapname,"cs_italy_cz")){
			if(y>2608)
				slay=1
		}
		else if (equali(mapname,"de_dust2")){
			if(z>270)
				slay=1
		}
		else if (equali(mapname,"de_dust2_cz")){
			if(z>270)
				slay=1
		}
		else if (equali(mapname,"fy_dustworld")){
			if(z>82)
				slay=1
		}
		else if (equali(mapname,"fy_pool_day")){
			if(z>190)
				slay=1
		}
		else if (equali(mapname,"as_oilrig") ){
			if (x>1530 && get_user_team(id)==CTS)
				slay=1
			else if(x>1700)
				slay=1
		}
		else
			slay=0
		
		if(equali(mapname,"cs_mice_final"))
			slay=0
	#endif
	
	if(slay){
		client_print(id,print_chat,"%L",id,"SLAIN_FOR_TELEPORTING",g_MOD)
		user_kill(id)
	}

	return PLUGIN_CONTINUE
}

public _Ultimate_BigBadVoodoo(parm[2]){

	#if ADVANCED_DEBUG == 1
		writeDebugInfo("reset_godmode",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(parm[1] == 1){
		entity_set_int(id,EV_INT_effects,EF_BRIGHTFIELD)

		p_data_b[id][PB_GODMODE] = true

		set_user_health(id, get_user_health(id) + 2048)

		#if MOD == 0
			Create_BarTime(id, 2, 0)
		#endif
		
		emit_sound(id,CHAN_STATIC, SOUND_VOODOO, 1.0, ATTN_NORM, 0, PITCH_NORM)

		p_data_b[id][PB_ULTIMATEUSED]=true
		
		icon_controller(id,ICON_FLASH)

		parm[1] = 0
		set_task(2.0,"_Ultimate_BigBadVoodoo",TASK_RESETGOD+id,parm,2)
	}
	else{
		entity_set_int(id,EV_INT_effects,0)
	
		p_data_b[id][PB_GODMODE] = false
		
		set_user_health(id, get_user_health(id) - 2048)
	
		icon_controller(id,ICON_HIDE)

		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_WAR3_Ultimate_Delay(parm)
	}

	return PLUGIN_CONTINUE
}