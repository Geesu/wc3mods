

// ****************************************
// Undead's Suicide Bomber
// ****************************************

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


// ****************************************
// Human's Teleport
// ****************************************

Ultimate_Teleport(id){
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

// ****************************************
// Orc's Change Lightning
// ****************************************

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
		if(!p_data_b[id][PB_ULTIMATEUSED]){
			if (parm[1]>0 && get_user_health(id)>=0){
				set_task(0.1,"lightsearchtarget",TASK_LIGHTSEARCH+id,parm,2)
			}else{
				p_data_b[id][PB_ISSEARCHING]=false
				icon_controller(id,ICON_SHOW)
			}
		}
		else{
			icon_controller(id,ICON_HIDE)
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

	if(!g_mapDisabled)
		Create_TE_BEAMENTS(id, targetid, g_sLightning, 0, 15, 10, linewidth, 10, 255, 255, 255, 255, 0)

	new origin[3]
	get_user_origin(targetid,origin)

	Create_TE_ELIGHT(targetid, origin, 100, 255, 255, 255, 10, 0)

	emit_sound(id,CHAN_STATIC, SOUND_LIGHTNING, 1.0, ATTN_NORM, 0, PITCH_NORM)

	return PLUGIN_CONTINUE
}


// ****************************************
// Night Elf's Entangle
// ****************************************

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
		
		if( iCvar[FT_ENTANGLE_DROP] ){
			new ammo, clip
			new weapon = get_user_weapon(enemy, ammo, clip)

			if(isPrimary(weapon))
				client_cmd(enemy, "drop")
		}

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
		if(!p_data_b[id][PB_ULTIMATEUSED]){
			if (parm[1]>0 && get_user_health(id)>0){
				set_task(0.1,"searchtarget",TASK_SEARCHTARGET+id,parm,2)
			}else{
				p_data_b[id][PB_ISSEARCHING]=false
				icon_controller(id,ICON_SHOW)
			}
		}
		else{
			icon_controller(id,ICON_HIDE)
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


// ****************************************
// Blood Mage's Flame Strike
// ****************************************

Ultimate_FlameStrike(id){ 
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

	new maxplayers = get_maxplayers()
	new bodypart, victim 
	
	get_user_aiming(id,victim,bodypart,550) 

	if( victim > 0 && victim <= maxplayers ){
		if( get_user_team(victim)!=get_user_team(id) )
			burn_victim(victim,id)
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

	new origin[3], players[32], numberofplayers
	get_players(players, numberofplayers)

	for (new i=0; i < numberofplayers; i++){
		victim = players[i]
		if( is_user_alive(victim) && get_user_team(victim)!=get_user_team(id) ){ 
			get_user_origin(victim,origin) 
			if(get_distance(origin, burnvec1) < radius) 
				burn_victim(victim,id) 
			else if(get_distance(origin, burnvec2) < radius) 
				burn_victim(victim,id) 
		} 
	} 
	return PLUGIN_CONTINUE       
} 

burn_victim(victim,attacker){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("burn_victim",victim)
	#endif

	if(p_data_b[victim][PB_ISBURNING]) 
		return PLUGIN_CONTINUE 

	if (p_data[victim][P_ITEM]==ITEM_NECKLACE || p_data_b[victim][PB_WARDENBLINK])
		return PLUGIN_CONTINUE

	p_data_b[victim][PB_ISBURNING] = true

	new hp,args[2] 
	hp = get_user_health(victim) 

	args[0] = victim 
	args[1] = attacker  
	set_task(0.3,"on_fire",TASK_ONFIRE,args,2,"a",hp / 10) 

	return PLUGIN_CONTINUE 
} 

public on_fire(args[]){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("on_fire",args[0])
	#endif

	new victim = args[0]
	new attacker = args[1] 

	if(!p_data_b[victim][PB_ISCONNECTED])
		return PLUGIN_CONTINUE


	if(!p_data_b[victim][PB_ISBURNING]) 
		return PLUGIN_CONTINUE 

	new rx,ry,rz, origin[3], position[3]

	rx = random_num(-30,30) 
	ry = random_num(-30,30) 
	rz = random_num(-30,30) 

	get_user_origin(victim,origin) 

	position[0] = origin[0] + rx
	position[1] = origin[1] + ry
	position[2] = origin[2] + 10 + rz

	if(!g_mapDisabled)
		Create_TE_SPRITE(position, g_sBurning, 30, 200)

	position[0] = origin[0]+(rx*2)
	position[1] = origin[1]+(ry*2)
	position[2] = origin[2]+100+(rz*2)

#if MOD == 0
	if(!g_mapDisabled)
		Create_TE_Smoke(position, position, g_sSmoke, 60, 15)
#endif

	if(!is_user_alive(victim)) 
		return PLUGIN_CONTINUE 

	WAR3_damage(victim, attacker, 10, CSW_FLAME, 0)

	return PLUGIN_CONTINUE 
}


// ****************************************
// Shadow Hunter's Big Bad Voodoo
// ****************************************

public _Ultimate_BigBadVoodoo(parm[2]){

	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Ultimate_BigBadVoodoo",parm[0])
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
		
		if( is_user_alive(id) )
			set_user_health(id, get_user_health(id) - 2048)
	
		icon_controller(id,ICON_HIDE)

		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_WAR3_Ultimate_Delay(parm)
	}

	return PLUGIN_CONTINUE
}


// ****************************************
// Warden's Vengeance
// ****************************************

Ultimate_Vengeance(id){
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


// ****************************************
// Crypt Lord's Locust Swarm
// ****************************************

Ultimate_LocustSwarm(id){
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
