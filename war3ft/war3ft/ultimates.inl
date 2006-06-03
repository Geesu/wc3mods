
#define	ENTANGLE_TIME		10.0


// Function will stop all ultimates from functioning
public ULT_Reset()
{
	new players[32], num, id, i;
	get_players( players, num );
	
	for( i = 0; i < num; i++ )
	{
		id = players[i];
		
		// Set this to true so that ultimates cannot be used
		p_data_b[id][PB_ULTIMATEUSED] = true

		// Remove any explosions that are occuring, we don't want them carried over to the next round
		task_exists( TASK_EXPLOSION + id )		? remove_task( TASK_EXPLOSION + id ) : 0;
		task_exists( TASK_BEAMCYLINDER + id)	? remove_task( TASK_BEAMCYLINDER + id ) : 0;

		// Stop the user from searching (chain lightning)
		task_exists( TASK_LIGHTSEARCH + id )	? remove_task( TASK_LIGHTSEARCH + id ) : 0;

		// Stop the user from searching (entangling roots)
		task_exists( TASK_SEARCHTARGET + id )	? remove_task( TASK_SEARCHTARGET + id ) : 0;

		// Set this to false just to be safe (used by NE and ORC ultimates)
		p_data_b[id][PB_ISSEARCHING] = false;

		// Need to add big bad voodoo check here...
	}
}

// ****************************************
// Undead's Suicide Bomber
// ****************************************

public apacheexplode(parm[5]){		// Suicide Bomber
	new id = parm[0]
	// random explosions

#if ADVANCED_STATS
	new WEAPON = CSW_SUICIDE - CSW_WAR3_MIN
	iStatsShots[id][WEAPON]++
#endif

	new origin[3], origin2[3]
	origin[0] = parm[2]
	origin[1] = parm[3]
	origin[2] = parm[4]

	origin2[0] = origin[0] + random_num( -100, 100 )
	origin2[1] = origin[1] + random_num( -100, 100 )
	origin2[2] = origin[2] + random_num( -50, 50 )
	
	Create_TE_EXPLOSION(origin, origin2, g_sFireball, (random_num(0,20) + 20), 12, TE_EXPLFLAG_NONE)

#if MOD == 0
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
				WAR3_damage(targetid, id, damage, CSW_SUICIDE, -1)

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

	if ( endround )
		return 0

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
// Human's Blink
// ****************************************

Ultimate_Blink(id){

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
	
	if(WC3_IsImmunePlayerNear(id, oldLocation) || WC3_IsImmunePlayerNear(id, newLocation)){
		set_hudmessage(255, 255, 10, -1.0, -0.4, 1, 0.5, BLINK_COOLDOWN, 0.2, 0.2,5)
		show_hudmessage(id,"%L",id,"TELEPORT_FAILED_ENEMY_IMMUNITY")

		p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )

		_ULT_Delay( id )

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

	if ( get_pcvar_num( CVAR_wc3_blink_protection ) ){
		new mapname[32]
		get_mapname(mapname,32) 
		if (equali(mapname,"as_oilrig")){
			if (newLocation[0]>1530 && get_user_team(id)==CTS){
				WAR3_Kill(id, 0)
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
	Ultimate_Icon(id,ICON_HIDE)

	emit_sound(id, CHAN_STATIC, SOUND_BLINK, 1.0, ATTN_NORM, 0, PITCH_NORM)

	origin[0] = oldLocation[0]
	origin[1] = oldLocation[1]
	origin[2] = oldLocation[2] + 90
	origin2[0] = oldLocation[0]
	origin2[1] = oldLocation[1]
	origin2[2] = oldLocation[2] + 90 + TELEPORT_RADIUS

	Create_TE_BEAMCYLINDER(oldLocation, origin, origin2, g_sSpriteTexture, 0, 0, 3, 60, 0, 255, 255, 255, 255, 0)	
			
	return PLUGIN_CONTINUE
}

public _Ultimate_Blink_Controller(parm[]){

	new id = parm[0]	 

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new oldLocation[3]
	new origin[3]

	oldLocation[0] = parm[2]
	oldLocation[1] = parm[3]
	oldLocation[2] = parm[4]

	// Teleport failure check and unsticker

	get_user_origin(id, origin)

	if(parm[5] == origin[2]){ // Teleportation Failure

		set_hudmessage(255, 255, 10, -1.0, -0.4, 1, 0.5, BLINK_COOLDOWN, 0.2, 0.2,5)
		show_hudmessage(id,"%L",id,"TELEPORT_FAILED_BAD_DESTINATION")
		
		set_user_origin(id, oldLocation)	

		p_data[id][P_ULTIMATEDELAY] = floatround(BLINK_COOLDOWN + 1.0)

		_ULT_Delay( id )

	}
	else{
		
		p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
		_ULT_Delay( id )

		// Sprays white bubbles everywhere
		new origin2[3]
		origin2[0] = origin[0]
		origin2[1] = origin[1]
		origin2[2] = origin[2] + 40
		
		Create_TE_SPRITETRAIL(origin2, origin, g_sFlare, 30, 10, 1, 50, 10)

		// Flash the player
		if( get_pcvar_num( CVAR_wc3_blink_dizziness ) == 1 )
			Create_ScreenFade(id, (1<<15), (1<<10), (1<<12), 0, 0, 255, 180)
		else
			Create_ScreenFade(id, (1<<15), (1<<10), (1<<12), 255, 255, 255, 255)			
	}	
	
	return PLUGIN_CONTINUE
}




public ceiling_check(parm[2]){

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
		client_print(id,print_chat,"%L",id,"SLAIN_FOR_TELEPORTING",g_MODclient)
		user_kill(id)
	}

	return PLUGIN_CONTINUE
}

// ****************************************
// Orc's Change Lightning
// ****************************************

public lightsearchtarget(parm[2]){
	
	// This is the caster
	new id = parm[0]
	
	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(!is_user_alive(id)){
		p_data_b[id][PB_ISSEARCHING] = false
		return PLUGIN_CONTINUE
	}

	new enemy, bodypart
	get_user_aiming(id,enemy,bodypart)

	new CasterTeam = get_user_team(id)

	if ( 0<enemy<=MAXPLAYERS && CasterTeam!=get_user_team(enemy) && p_data[enemy][P_ITEM]!=ITEM_NECKLACE && !p_data_b[enemy][PB_WARDENBLINK] && is_user_alive(enemy)){
		p_data_b[id][PB_ULTIMATEUSED]=true
		Ultimate_Icon(id,ICON_HIDE)
		new linewidth = 80
		new damage = 50

		p_data_b[id][PB_ISSEARCHING]=false
		
		lightningeffect(id,enemy,linewidth,damage,bodypart)
		new lightparm[5]
		lightparm[0]=enemy
		lightparm[1]=damage
		lightparm[2]=linewidth
		lightparm[3]=id
		lightparm[4]=bodypart
		set_task(0.2,"lightningnext",TASK_LIGHTNING+enemy,lightparm,5)

		p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
		_ULT_Delay( id )
	}
	else{
		p_data_b[id][PB_ISSEARCHING]=true
		Ultimate_Icon(id,ICON_FLASH)
		new counter = parm[1]
		while (counter >= 0){
			counter -= 10
			if (counter==0){
				#if ADVANCED_STATS
					new WEAPON = CSW_LIGHTNING - CSW_WAR3_MIN
					iStatsShots[id][WEAPON]++
				#endif

				emit_sound(id,CHAN_STATIC, "turret/tu_ping.wav", 1.0, ATTN_NORM, 0, PITCH_NORM)
			}
		}
		--parm[1]	
		if(!p_data_b[id][PB_ULTIMATEUSED]){
			if (parm[1]>0 && get_user_health(id)>=0){
				set_task(0.1,"lightsearchtarget",TASK_LIGHTSEARCH+id,parm,2)
			}else{
				p_data_b[id][PB_ISSEARCHING]=false
				Ultimate_Icon(id,ICON_SHOW)
			}
		}
		else{
			Ultimate_Icon(id,ICON_HIDE)
		}
	}

	return PLUGIN_CONTINUE
}


public lightningnext(parm[5]){		// Chain Lightning

	// parm[0] = enemy
	// parm[1] = damage
	// parm[2] = linewidth
	// parm[3] = caster
	// parm[4] = bodypart

	new enemy = parm[0]

	if(!p_data_b[enemy][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new caster=parm[3]
	new bodypart=parm[4]
	new CasterTeam = get_user_team(caster)

	new origin[3]
	get_user_origin(enemy, origin)

	new players[32], numberofplayers
	get_players(players, numberofplayers,"a")


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
		if ( get_user_team(targetid) != CasterTeam && is_user_alive(targetid) ){
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
		lightningeffect(caster,closestid,linewidth,damage,bodypart)
		parm[0]=targetid
		parm[1]=damage
		parm[2]=linewidth
		parm[3]=caster
		parm[4]=0
		set_task(0.2,"lightningnext",TASK_LIGHTNINGNEXT+caster,parm,4)
	}
	else{
		for (i = 0; i < numberofplayers; ++i){
			targetid=players[i]
			p_data_b[targetid][PB_LIGHTNINGHIT]=false
		}
	}
	return PLUGIN_CONTINUE
}

public lightningeffect(caster,targetid,linewidth,damage,bodypart){

	p_data_b[targetid][PB_LIGHTNINGHIT]=true

	new parm[1]
	parm[0]=caster
	Ultimate_Icon(caster,ICON_HIDE)

	WAR3_damage(targetid, caster, damage, CSW_LIGHTNING, bodypart)

	Create_TE_BEAMENTS(caster, targetid, g_sLightning, 0, 15, 10, linewidth, 10, 255, 255, 255, 255, 0)

	new origin[3]
	get_user_origin(targetid,origin)

	Create_TE_ELIGHT(targetid, origin, 100, 255, 255, 255, 10, 0)

	emit_sound(caster,CHAN_STATIC, SOUND_LIGHTNING, 1.0, ATTN_NORM, 0, PITCH_NORM)

	return PLUGIN_CONTINUE
}


// ****************************************
// Night Elf's Entangle
// ****************************************

public searchtarget(parm[2]){

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(!is_user_alive(id)){
		p_data_b[id][PB_ISSEARCHING] = false
		return PLUGIN_CONTINUE
	}

	new enemy, body
	get_user_aiming(id,enemy,body)

	if ( 0<enemy<=MAXPLAYERS &&!p_data_b[enemy][PB_STUNNED] && get_user_team(id)!=get_user_team(enemy) && p_data[enemy][P_ITEM]!=ITEM_NECKLACE && !p_data_b[enemy][PB_WARDENBLINK] && is_user_alive(id) && is_user_alive(enemy)){
		p_data_b[id][PB_ISSEARCHING]=false
		p_data_b[id][PB_ULTIMATEUSED]=true
		Ultimate_Icon(id,ICON_HIDE)

		Create_TE_BEAMFOLLOW(enemy, g_siTrail, 10, 5, 10, 108, 23, 255)

//		emit_sound(id,CHAN_STATIC, SOUND_ENTANGLING, 1.0, ATTN_NORM, 0, PITCH_NORM)


		p_data[enemy][PB_STUNNED] = true;

		SHARED_SetSpeed( enemy );
		
		// Start waiting for the user to stop...
		new waitparm[4];
		waitparm[0] = enemy;
		waitparm[1] = 0;
		waitparm[2] = 0;
		waitparm[3] = 0;
		_ULT_Entangle_Wait(waitparm)
		
		if( get_pcvar_num( CVAR_wc3_entangle_drop ) )
		{
			new ammo, clip
			new weapon = get_user_weapon(enemy, ammo, clip)

			if(SHARED_IsPrimaryWeapon(weapon))
				client_cmd(enemy, "drop")
		}

		p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
		_ULT_Delay( id )	
	}
	else{
		p_data_b[id][PB_ISSEARCHING]=true
		Ultimate_Icon(id,ICON_FLASH)
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
				Ultimate_Icon(id,ICON_SHOW)
			}
		}
		else{
			Ultimate_Icon(id,ICON_HIDE)
		}
	}
	return PLUGIN_CONTINUE
}

public _ULT_Entangle_Wait( parm[4] )
{

	new id = parm[0];

	if( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	new origin[3];
	get_user_origin( id, origin );
	
	// Checking to see if the user has actually stopped yet?
	if ( origin[0] == parm[1] && origin[1] == parm[2] && origin[2] == parm[3] )
	{
		// Reset the user's speed in ENTANGLE_TIME amount of time
		set_task( ENTANGLE_TIME, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + id );
		
		// Entangle the user
		ULT_NE_Entangle( id )
	}

	// If not lets run another check in 0.1 seconds
	else
	{
		parm[1] = origin[0];
		parm[2] = origin[1];
		parm[3] = origin[2];

		set_task(0.1,"_ULT_Entangle_Wait",TASK_ENTANGLEWAIT+id,parm,4)
	}
	return;
}

public ULT_NE_Entangle( id )
{
	new life	= floatround( ENTANGLE_TIME );
	new radius	= 20;
	new counter = 0;
	new origin[3];
	new x1, y1, x2, y2;

	get_user_origin( id, origin );

	emit_sound( id, CHAN_STATIC, SOUND_ENTANGLING, 1.0, ATTN_NORM, 0, PITCH_NORM );

	while (counter<=7)
	{
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

		while (height > -40)
		{
			
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

	return;
}


// ****************************************
// Blood Mage's Flame Strike
// ****************************************

Ultimate_FlameStrike(id){ 

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

      p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
      _ULT_Delay( id )
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

	new position[3], direction[3]
	position[0] = args[0]
	position[1] = args[1]
	position[2] = args[2]
	direction[0] = args[3]
	direction[1] = args[4]
	direction[2] = args[5]

	Create_TE_SPRAY(position, direction, g_sFire, 8, 70, 100, 5)

	return PLUGIN_CONTINUE 
} 

check_burnzone(id,vec[],aimvec[],speed1,speed2,radius){ 

	new maxplayers = get_maxplayers()
	new bodypart, victim 
	
	get_user_aiming(id,victim,bodypart,550) 

	if( victim > 0 && victim <= maxplayers ){
		if( get_user_team(victim)!=get_user_team(id) )
			burn_victim(victim,id,bodypart)
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
				burn_victim(victim,id,bodypart) 
			else if(get_distance(origin, burnvec2) < radius) 
				burn_victim(victim,id,bodypart) 
		} 
	} 
	return PLUGIN_CONTINUE       
} 

burn_victim(victim,attacker,bodypart){ 

	if(p_data_b[victim][PB_ISBURNING]) 
		return PLUGIN_CONTINUE 

	if (p_data[victim][P_ITEM]==ITEM_NECKLACE || p_data_b[victim][PB_WARDENBLINK])
		return PLUGIN_CONTINUE

	p_data_b[victim][PB_ISBURNING] = true

	new hp,args[3] 
	hp = get_user_actualhealth(victim) 

	args[0] = victim 
	args[1] = attacker  
	args[2] = bodypart
	set_task(0.3,"on_fire",TASK_ONFIRE,args,3,"a",hp / 10) 

	return PLUGIN_CONTINUE 
} 

public on_fire(args[]){ 

	new victim = args[0]
	new attacker = args[1] 
	new bodypart = args[2]

	if (!bodypart)
		bodypart = random_num(1,8)

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

	Create_TE_SPRITE(position, g_sBurning, 30, 200)

	position[0] = origin[0]+(rx*2)
	position[1] = origin[1]+(ry*2)
	position[2] = origin[2]+100+(rz*2)

#if MOD == 0
	Create_TE_Smoke(position, position, g_sSmoke, 60, 15)
#endif

	if( !is_user_alive(victim) && p_data[victim][P_ITEM] == ITEM_NECKLACE ) 
		return PLUGIN_CONTINUE 

	WAR3_damage(victim, attacker, 10, CSW_FLAME, bodypart)

	return PLUGIN_CONTINUE 
}


// ****************************************
// Shadow Hunter's Big Bad Voodoo
// ****************************************

public _Ultimate_BigBadVoodoo(parm[2]){

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(parm[1] == 1){
	#if MOD == 0
		if ( get_user_team(id) == TS )
			set_user_rendering( id, kRenderFxGlowShell, 255, 0, 0,  kRenderNormal, 255 )
		else
			set_user_rendering( id, kRenderFxGlowShell, 0, 0, 255,  kRenderNormal, 255 )
	#endif
	#if MOD == 1
		if ( get_user_team(id) == ALLIES )
			set_user_rendering( id, kRenderFxGlowShell, 255, 63, 63,  kRenderNormal, 255 )
		else
			set_user_rendering( id, kRenderFxGlowShell, 76, 102, 76,  kRenderNormal, 255 )
	#endif
		p_data_b[id][PB_GODMODE] = true

		/* Do not allow other renderings to take place, like switching to invis, etc... */
		p_data_b[id][PB_CAN_RENDER] = false

		set_user_health(id, get_user_health(id) + 2048)

		#if MOD == 0
			Create_BarTime(id, 2, 0)
		#endif
		
		emit_sound(id,CHAN_STATIC, SOUND_VOODOO, 1.0, ATTN_NORM, 0, PITCH_NORM)

		p_data_b[id][PB_ULTIMATEUSED]=true
		
		Ultimate_Icon(id,ICON_FLASH)

		parm[1] = 0
		set_task(2.0,"_Ultimate_BigBadVoodoo",TASK_RESETGOD+id,parm,2)
	}
	else{
		/* Allow other renderings to take place now */
		p_data_b[id][PB_CAN_RENDER] = true

		set_user_rendering(id)

		p_data_b[id][PB_GODMODE] = false
		
		if( is_user_alive(id) )
			set_user_health(id, get_user_health(id) - 2048)
	
		Ultimate_Icon(id,ICON_HIDE)

		p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
		_ULT_Delay( id )
	}

	return PLUGIN_CONTINUE
}

// ****************************************
// Crypt Lord's Locust Swarm
// ****************************************

Ultimate_LocustSwarm(id){

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

	Ultimate_Icon(id,ICON_FLASH)

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
		Ultimate_Icon(caster,ICON_SHOW)	
		return PLUGIN_HANDLED
	}

	new origin[3], funnel[3], name[32], name2[32]
	get_user_name(id,name,31)
	get_user_name(caster,name2,31)
	get_user_origin(id,origin)

	funnel[0]=parm[0]			// Origin of the funnel
	funnel[1]=parm[1]
	funnel[2]=parm[2]
	
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
			#if MOD == 1
				set_task(0.5,"drawfunnels",caster+TASK_FUNNELS,parm,11)
			#else
				set_task(0.1,"drawfunnels",caster+TASK_FUNNELS,parm,11)
			#endif
		}
		else{
			WAR3_damage(id, caster, 45, CSW_LOCUSTS, -1)

			emit_sound(id,CHAN_STATIC, SOUND_LOCUSTSWARM, 1.0, ATTN_NORM, 0, PITCH_NORM)

			Ultimate_Icon(caster,ICON_HIDE)
			
			p_data[caster][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
			_ULT_Delay( caster )
		}
	}
	return PLUGIN_HANDLED
}

public _ULT_Delay( id )
{
	if ( !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}
	
	// If the function is called from a task, modify the id
	if ( id >= TASK_UDELAY )
	{
		id -= TASK_UDELAY;
	}
	
	// Player call
	if ( id != 0 )
	{
		if( !p_data_b[id][PB_ISCONNECTED] )
		{
			return PLUGIN_CONTINUE;
		}

		p_data[id][P_ULTIMATEDELAY]--;

		if ( p_data[id][P_ULTIMATEDELAY] > 0 )
		{
			// If this exists then the user is getting his ult back quicker than he should
			if ( task_exists( TASK_UDELAY + id ) )
			{
				remove_task( TASK_UDELAY + id );
			}

			set_task( 1.0, "_ULT_Delay", TASK_UDELAY + id );
		}
		else
		{
			Ultimate_Ready( id );
		}
	}

	// Server check
	else
	{
		g_iUltimateDelay--;

		if ( g_iUltimateDelay > 0 )
		{
			set_task( 1.0, "_ULT_Delay", TASK_UDELAY + id );
		}
		else
		{
			g_iUltimateDelay = 0;
		}
	}

	return PLUGIN_CONTINUE;
}

public Ultimate_Ready(id)
{
	p_data_b[id][PB_ULTIMATEUSED] = false;

	if ( is_user_alive( id ) && p_data_b[id][PB_ISCONNECTED] && p_data[id][P_ULTIMATE] )
	{
		// Play the ultimate ready sound
		client_cmd( id, "speak %s", SOUND_ULTIMATEREADY )
		
		// Give the user a graphical message that their ultimate is ready
		WC3_Status_Text( id, 2.0, HUDMESSAGE_POS_STATUS, "%L", id, "ULTIMATE_READY" );
		
		// Show their ultimate icon
		Ultimate_Icon( id, ICON_SHOW );
	}
	
	return;
}

public Ultimate_Icon(id, value){

	if (!warcraft3)
		return PLUGIN_CONTINUE
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
		switchValue = g_ChamSkills[4]
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

public Ultimate_Clear_Icons(id){
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

public ULT_IsImmune( id )
{
	return ( p_data[enemy][P_ITEM] == ITEM_NECKLACE || p_data_b[enemy][PB_WARDENBLINK] )
}

// Function will search for a target (where player is looking)
public _ULT_FindTarget( parm[2] )
{
	
	// This is the caster
	new id = parm[0]
	
	// Stop searching if user died or isn't connected anymore
	if ( !p_data_b[id][PB_ISCONNECTED] || !is_user_alive( id ) )
	{
		p_data_b[id][PB_ISSEARCHING] = false;

		return;
	}

	// Find out where the user is aiming at...

	new iEnemy, iBodyPart
	get_user_aiming( id, iEnemy, iBodyPart );

	new iCasterTeam = get_user_team( id )
	
	// Make sure we have a valid enemy and the enemy is not immune
	if ( 0 < iEnemy < MAXPLAYERS && iCasterTeam != get_user_team( iEnemy ) && !ULT_IsImmune( iEnemy ) )
	{
		
		//p_data_b[id][PB_ULTIMATEUSED]=true
		//Ultimate_Icon(id,ICON_HIDE)

		switch ( p_data[id][P_RACE] )
		{




		}


		//p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
		//_ULT_Delay( id )

	}
	else
	{
		// User is searching for a target
		p_data_b[id][PB_ISSEARCHING] = true;

		// Flash their ultimate icon
		Ultimate_Icon( id, ICON_FLASH );
		
		// This is the number of seconds we should search
		new iCounter = parm[1];
		
		// Every second we need to execute the "ping" sound
	
		if ( ( iCounter -= 10 ) % 10 == 0 )
		while ( iCounter >= 0 )
		{
			iCounter -= 10;

			if ( counter == 0 )
			{
				//#if ADVANCED_STATS
				//	new WEAPON = CSW_LIGHTNING - CSW_WAR3_MIN;
				//	iStatsShots[id][WEAPON]++;
				//#endif

				emit_sound( id, CHAN_STATIC, "turret/tu_ping.wav", 1.0, ATTN_NORM, 0, PITCH_NORM );
			}
		}

		// Decrement the overall counter
		--parm[1]	
		if(!p_data_b[id][PB_ULTIMATEUSED]){
			if (parm[1]>0 && get_user_health(id)>=0){
				set_task(0.1,"_ULT_FindTarget",TASK_LIGHTSEARCH+id,parm,2)
			}else{
				p_data_b[id][PB_ISSEARCHING]=false
				Ultimate_Icon(id,ICON_SHOW)
			}
		}
		else{
			Ultimate_Icon(id,ICON_HIDE)
		}
	}

	return PLUGIN_CONTINUE
}