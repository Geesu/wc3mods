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
	// If the function is called from a task, modify the id
	if ( id >= TASK_UDELAY )
	{
		id -= TASK_UDELAY;
	}
	
	// Player call
	if ( id != 0 )
	{
		if ( !p_data_b[id][PB_ISCONNECTED] )
		{
			return;
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

	return;
}

public Ultimate_Ready(id)
{
	if ( !WAR3_Check() )
	{
		return;
	}
	

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
	return ( p_data[id][P_ITEM] == ITEM_NECKLACE || p_data_b[id][PB_WARDENBLINK] )
}

public _ULT_Ping( parm[] )
{
	new id = parm[0];
	new iTimeLeft = parm[1];
	
	// Decrement our timer
	parm[1]--;
	
	// User died or diconnected
	if ( !p_data_b[id][PB_ISCONNECTED] || !is_user_alive( id ) )
	{
		p_data_b[id][PB_ISSEARCHING] = false;
	}
	
	// This is the last "playing" of the sound, no target was found :/
	if ( iTimeLeft == 0 )
	{
		p_data_b[id][PB_ISSEARCHING] = false;

		Ultimate_Icon( id, ICON_SHOW );
	}
	
	// Then we need to play the sound + flash their icon!
	if ( p_data_b[id][PB_ISSEARCHING] )
	{
		// Flash their ultimate icon
		Ultimate_Icon( id, ICON_FLASH );

		// Play the ping sound
		emit_sound( id, CHAN_STATIC, SOUND_ULTIMATESCAN, 1.0, ATTN_NORM, 0, PITCH_NORM );

		set_task( 1.0, "_ULT_Ping", TASK_ULTPING + id, parm, 2 );
	}

	return;
}

// Function called before user's spawn
ULT_Reset( id )
{
	// Remove any explosions that are occuring, we don't want them carried over to the next round
	task_exists( TASK_EXPLOSION + id )		? remove_task( TASK_EXPLOSION + id ) : 0;
	task_exists( TASK_BEAMCYLINDER + id )	? remove_task( TASK_BEAMCYLINDER + id ) : 0;

	// Stop Crypt Lord's Ultimate
	task_exists( TASK_FUNNELS + id )		? remove_task( TASK_FUNNELS + id ) : 0;

	// Stop the ultimate cooldowns since they will reset at round start
	task_exists( TASK_UDELAY + id )			? remove_task( TASK_UDELAY + id ) : 0;
	
	// Reset Big Bad Voodoo
	if ( task_exists( TASK_RESETGOD + id ) )
	{
		remove_task( TASK_RESETGOD + id );

		new parm[2];
		parm[0] = id;
		parm[1] = 0;

		_Ultimate_BigBadVoodoo( parm );
	}

	// Set this to false to stop searching ultimates (used by NE + ORC + BM ultimates)
	p_data_b[id][PB_ISSEARCHING] = false;

	// Hide their ultimate icon
	Ultimate_Icon( id, ICON_HIDE );
}