

HA_ULT_Blink( id )
{
	
	// User can't Blink when he/she's stunned
	if ( p_data_b[id][PB_STUNNED] )
	{
		client_print( id, print_center, "You can't blink when you're stunned!" );
		return PLUGIN_HANDLED;
	}
	
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
	client_cmd( id, "-use" );

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
	ULT_Icon(id,ICON_HIDE)

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

	}
	else{
		
		p_data[id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )

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
