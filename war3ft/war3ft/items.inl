public Item_ShowHUD(parm2[2]){					// Displays the player's items in the bottom center of the screen
	#if ADVANCED_DEBUG
		writeDebugInfo("items",parm2[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

#if MOD == 0
	if(!iCvar[FT_ITEMS_IN_HUD])
		return PLUGIN_CONTINUE
#endif

	new temp[128]
	new message[256]
	new id = parm2[0]

	new item_name[ITEM_NAME_LENGTH], item_name2[ITEM_NAME_LENGTH]
	itemname(p_data[id][P_ITEM],id,item_name,ITEM_NAME_LENGTH_F)
	itemname2(p_data[id][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F)

	if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]!=0){			// User has 2 items
		if (p_data[id][P_ITEM2]==ITEM_RING)
			format(temp,127,"%L",id,"CURRENT_ITEMS_RING",item_name,item_name2,p_data[id][P_RINGS])
		else
			format(temp,127,"%L",id,"CURRENT_ITEMS",item_name,item_name2)
	}
	else if (p_data[id][P_ITEM]==0 && p_data[id][P_ITEM2]!=0){		// User only has an item from shopmenu2
		if (p_data[id][P_ITEM2]==ITEM_RING)
			format(temp,127,"%L",id,"CURRENT_ITEM_RING",item_name2,p_data[id][P_RINGS])
		else
			format(temp,127,"%L",id,"CURRENT_ITEM",item_name2)
	}
	else if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]==0)		// User only has an item from shopmenu2
		format(temp,127,"%L",id,"CURRENT_ITEM",item_name)

	add(message,255,temp)

	set_hudmessage(255,255,255,-1.0,0.945,2, 1.5, 40.0, 0.02, 5.0, 1) 
	if (is_user_alive(id))
		show_hudmessage(id,message)

// 640x480:		.017 .850
// 800x600:		.013 .881
// 1024x768:	.010 .908
// 1280x1024:	.008 .935
//set_hudmessage(224, 160, 0, 0.010, 0.908, HUDMESSAGE_FX_FADEIN, 10.0, 1.0, 2.0, 3.0, HUDMESSAGE_CHAN_ITEM)
//show_hudmessage(id, "Undead Level: 10")

// 640x480:		.017 .880
// 800x600:		.013 .908
// 1024x768:	.010 .928
// 1280x1024:	.008 .948
//set_hudmessage(224, 160, 0, 0.010, 0.928, HUDMESSAGE_FX_FADEIN, 10.0, 1.0, 2.0, 3.0, 1)
//show_hudmessage(id, "Current Item: Blah Item here")

	//set_hudmessage(224, 160, 0, 0.011, 0.91, HUDMESSAGE_FX_FADEIN, 10.0, 1.0, 2.0, 3.0, HUDMESSAGE_CHAN_ITEM)
	//show_hudmessage(id, message)

	set_task(1.0,"_WAR3_showHUDItems",TASK_ITEMS+id,parm2,2)
	return PLUGIN_CONTINUE
}

public Item_Message(id, item, shopmenu){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(shopmenu==SHOPMENU_ONE){
		new item_name[32]
		itemname(item,id,item_name,31)
		client_print(id, print_chat,"%s %s %L", g_MODclient, item_name, id, "WORD_PURCHASED")
		switch(item){
		#if MOD == 0
			case ITEM_ANKH:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_1")
			case ITEM_BOOTS:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_2", (100.0 * (1.0-(260.0/fCvar[FT_BOOTSPEED]))))
		#endif
		#if MOD == 1
			case ITEM_ANKH:			client_print(id, print_chat,"%s %L", g_MODclient, id, "DOD_INFO_SHOPMENU_1")
			case ITEM_BOOTS:		client_print(id, print_chat,"%s %L", g_MODclient, id, "DOD_INFO_SHOPMENU_2")
		#endif
			case ITEM_CLAWS:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_3", iCvar[FT_CLAW])
			case ITEM_CLOAK:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_4", (100.0 * (float(iCvar[FT_CLOAK])/255.0)))
			case ITEM_MASK:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_5", (100.0 * fCvar[FT_MASK_OF_DEATH]))
			case ITEM_NECKLACE:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_6")
			case ITEM_FROST:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_7", (100.0 * (fCvar[FT_FROST_SPEED]/260.0)))
			case ITEM_HEALTH:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_8", iCvar[FT_HEALTH_BONUS])
			case ITEM_TOME:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_9", (iCvar[FT_XPBONUS] + xpgiven[p_data[id][P_LEVEL]]))
		}
	}
	else if(shopmenu==SHOPMENU_TWO){
		new item_name[32]
		itemname2(item,id,item_name,31)
		client_print(id, print_chat,"%s %s %L", g_MODclient, item_name, id, "WORD_PURCHASED")
		switch(item){
		#if MOD == 0
			case ITEM_SCROLL:{
				if(is_user_alive(id))
					client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_1")
				else
					client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_1_DEAD")
			}
		#endif
			case ITEM_PROTECTANT:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_2")
			case ITEM_HELM:				client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_3")
			case ITEM_AMULET:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_4")
			case ITEM_SOCK:				client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_5")
			case ITEM_GLOVES:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_6", iCvar[FT_GLOVE_TIMER])
			case ITEM_RING:				client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_7")
		#if MOD == 0
			case ITEM_CHAMELEON:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_8")
		#endif
			case ITEM_MOLE:				client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_9")
		}
	}
	return PLUGIN_CONTINUE
}

public Item_Clear(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("Item_Clear",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	// Remove Helm
	if(p_data[id][P_ITEM2]==ITEM_HELM)
		Item_Set_Helm(id,0)

	// Reset Skin
	if (p_data[id][P_ITEM2]==ITEM_CHAMELEON)
		changeskin(id,1)	

	// Amulet of the Cat
	if (p_data[id][P_ITEM2] == ITEM_AMULET)
		p_data_b[id][PB_SILENT] = false

	// Reset rings and footsteps
	p_data[id][P_RINGS]=0
	p_data_b[id][PB_SILENT] = false

	return PLUGIN_CONTINUE
}


public Item_Check(parm[]){
	#if ADVANCED_DEBUG
		writeDebugInfo("Item_Check",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]
	
	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(p_data[id][P_ITEM2]==ITEM_MOLE)
		set_task(0.1,"_Item_Mole",TASK_FAN+id,parm,1)
		
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
		set_user_actualhealth(id,get_user_health(id)+iCvar[FT_HEALTH_BONUS], "Item_Buy, add health")
	
	if(p_data[id][P_ITEM2]==ITEM_RING && !task_exists(TASK_ITEM_RINGERATE+id))
		_Item_Ring(parm)

	if (p_data[id][P_ITEM2]==ITEM_CHAMELEON && is_user_alive(id))
		changeskin(id,SKIN_SWITCH)

	if (p_data[id][P_ITEM2] == ITEM_AMULET)
		p_data_b[id][PB_SILENT] = true
	else
		p_data_b[id][PB_SILENT] = false

	return PLUGIN_CONTINUE
}


// ****************************************
// Helm of Excellence
// ****************************************

public Item_Set_Helm(id, status){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(id==0)
		return PLUGIN_CONTINUE

	// set_user_hitzones(shooter, the one getting hit, zone)
	new zone

	if(status==1){		// Give helm
		zone = 253
		#if DEBUG
			client_print(id,print_chat, "### Helm enabled")
		#endif
	}
	else{				// Reset zones to normal
		zone = 255
		#if DEBUG
			client_print(id,print_chat, "### Helm disabled")
		#endif
	}

	set_user_hitzones(0, id, zone)

	//for (new j = 1; j <= MAXPLAYERS; j++){
	//	set_user_hitzones(j, id, zone)
	//}

	return PLUGIN_CONTINUE
}


// ****************************************
// Gloves of Warmth
// ****************************************

public _Item_Glove(parm[2]){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Item_Glove",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	// Stop calling this if the user is no longer connected

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	// If the user buys another item, we should not be continuing this

	if(p_data[id][P_ITEM2]!=ITEM_GLOVES){
		set_hudmessage(0, 100, 0, 0.05, 0.65, 2, 0.02, 10.0, 0.01, 0.1, 2)	
		show_hudmessage(id,"")
		#if MOD == 1
			Create_HudText(id, "", 1)
		#endif
		return PLUGIN_CONTINUE
	}

	// If the user dies, they won't get a nade, display a message

	if (!is_user_alive(id)){
		#if MOD == 0
			set_hudmessage(0, 100, 0, 0.05, 0.65, 2, 0.02, 10.0, 0.01, 0.1, 2)	
			show_hudmessage(id,"%L",id,"DONT_DIE_GOOSE")
		#endif
		#if MOD == 1
			new message[128]
			format(message, 127,"%L",id,"DONT_DIE_GOOSE")
			Create_HudText(id, message, 1)
		#endif

		return PLUGIN_CONTINUE
	}

	// Give the nade or subtract 1 from the timer and call this again a second later
	if(parm[1] < 1){
		Item_Glove_Give(id)
	}else{
		#if MOD == 0
			set_hudmessage(0, 100, 0, 0.05, 0.65, 2, 0.02, 10.0, 0.01, 0.1, 2)	
			show_hudmessage(id,"%L", id, "UNTIL_YOUR_NEXT_GRENADE", parm[1])
		#endif
		#if MOD == 1
			new message[128]
			format(message, 127,"%L", id, "UNTIL_YOUR_NEXT_GRENADE", parm[1])
			Create_HudText(id, message, 1)
		#endif
		parm[1]--
		set_task(1.0,"_Item_Glove",TASK_ITEM_GLOVES+id,parm,2)
	}
	
	return PLUGIN_CONTINUE
}

public Item_Glove_Give(id) { 
	#if ADVANCED_DEBUG
		writeDebugInfo("Item_Glove_Give",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

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


// ****************************************
// Mole
// ****************************************

public _Item_Mole(parm[]){ // For ITEM_MOLE, checks to see if there is an open spot on the other team's spawn 
	#if ADVANCED_DEBUG
		writeDebugInfo("Item_Mole",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE
	
	new entSpawn
	new Float:spawnAngle[3] 
	new Float:spawnOrigin[3]

	if(get_user_team(id)==CTS)
		 entSpawn = find_free_spawn(TS, spawnOrigin, spawnAngle)
	else
		 entSpawn = find_free_spawn(CTS, spawnOrigin, spawnAngle)

	if (!entSpawn) { 
		// Didn't find a free spawn spot. Quit... 
		if(parm[1]!=7){
			set_user_money(id,get_user_money(id)+itemcost2[8],1)
			client_print(id,print_chat,"%s %L",g_MODclient,id,"NO_SPOT_MOLE_MONEY")
		}
		else{
			client_print(id,print_chat,"%s %L",g_MODclient,id,"NO_SPOT_TO_MOLE")
		}
		return PLUGIN_CONTINUE 
	}

	new origin[3]

	if(p_data[id][P_ITEM2] == ITEM_MOLE)
		p_data[id][P_ITEM2] = 0

	p_data_b[id][PB_MOLE] = true

	FVecIVec(spawnOrigin, origin)

	changeskin(id,SKIN_SWITCH)

	entity_set_vector(id, EV_VEC_angles, spawnAngle)
	set_user_origin(id,origin) 

	Create_ScreenShake(id, (255<< 14), (10 << 14), (255<< 14))

	return PLUGIN_HANDLED 
} 

public _Item_Ring(parm[]){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Item_Ring",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(p_data[id][P_ITEM2]!=ITEM_RING)
		return PLUGIN_HANDLED

	new awardHealth = p_data[id][P_RINGS]

	while(awardHealth!=0){
		if( get_user_actualhealth(id) + 1 <= get_user_maxhealth(id) ){
			set_user_actualhealth(id, get_user_health(id) + 1, "_Item_Ring")
		}

		awardHealth--
	}

	set_task(2.0,"_Item_Ring",TASK_ITEM_RINGERATE+id,parm,2)

	return PLUGIN_CONTINUE
}
