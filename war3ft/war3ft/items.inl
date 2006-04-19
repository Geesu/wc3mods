#define DOD_BOOT_SPEED 45.0			// Stamina for users with boots of speed

public Item_Message(id, item, shopmenu){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(shopmenu==SHOPMENU_ONE){
		new item_name[32]
		lang_GetItemName ( item, id, item_name, 31, 1 );
		client_print(id, print_chat,"%s %s %L", g_MODclient, item_name, id, "WORD_PURCHASED")
		switch(item){
		#if MOD == 0
			case ITEM_ANKH:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_1")
			case wc3_boots:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_2", (100.0 * (1.0-(260.0/get_pcvar_float( CVAR_wc3_boots )))))
		#endif
		#if MOD == 1
			case ITEM_ANKH:			client_print(id, print_chat,"%s %L", g_MODclient, id, "DOD_INFO_SHOPMENU_1")
			case wc3_boots:		client_print(id, print_chat,"%s %L", g_MODclient, id, "DOD_INFO_SHOPMENU_2")
		#endif
			case wc3_clawS:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_3", get_pcvar_num( CVAR_wc3_claw ))
			case wc3_cloak:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_4", (100.0 * (float(get_pcvar_num( CVAR_wc3_cloak ))/255.0)))
			case ITEM_MASK:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_5", (100.0 * get_pcvar_num( CVAR_wc3_mask )))
			case ITEM_NECKLACE:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_6")
			case wc3_frost:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_7", (100.0 * (get_pcvar_float( CVAR_wc3_frost )/260.0)))
			case wc3_health:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_8", get_pcvar_num( CVAR_wc3_health ))
		#if MOD == 0
			case wc3_tome:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_9", (get_pcvar_num( CVAR_wc3_tome ) + xpgiven[p_data[id][P_LEVEL]]))
		#endif
		#if MOD == 1
			case wc3_tome:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_9", (2 * (get_pcvar_num( CVAR_wc3_tome ) + xpgiven[p_data[id][P_LEVEL]])))
		#endif
		}
	}
	else if(shopmenu==SHOPMENU_TWO){
		new item_name[32]
		lang_GetItemName ( item, id, item_name, 31, 2)
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
			case wc3_sock:				client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_5")
			case ITEM_GLOVES:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_6", get_pcvar_num( CVAR_wc3_glove_timer ))
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

	if (!warcraft3)
		return PLUGIN_CONTINUE

	// Remove Helm
	if(p_data[id][P_ITEM2]==ITEM_HELM)
		p_data_b[id][PB_IMMUNE_HEADSHOTS] = false;

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

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]
	
	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(p_data[id][P_ITEM2]==ITEM_MOLE)
		set_task(0.1,"_Item_Mole",TASK_FAN+id,parm,1)
		
	if(p_data_b[id][PB_DIEDLASTROUND])
	{
		p_data[id][P_ITEM]=0
		p_data[id][P_ITEM2]=0
		WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
	}

	p_data_b[id][PB_DIEDLASTROUND] = false
	
	Skill_UnholyAura(id);

	if(p_data[id][P_ITEM2]!=ITEM_CHAMELEON && p_data[id][P_SKINCHANGED]==SKIN_SWITCH)
		changeskin(id,SKIN_RESET)

	if (task_exists(TASK_ITEM_RINGERATE+id) && p_data[id][P_ITEM2]!=ITEM_RING)
		remove_task(TASK_ITEM_RINGERATE+id)

	if (p_data[id][P_ITEM]==wc3_health)
		set_user_health(id,get_user_health(id)+get_pcvar_num( CVAR_wc3_health ))
	
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
// Gloves of Warmth
// ****************************************

public _Item_Glove(parm[2]){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	// Stop calling this if the user is no longer connected

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	// If the user buys another item, we should not be continuing this
	if( p_data[id][P_ITEM2] != ITEM_GLOVES )
	{
		#if MOD == 0
			set_hudmessage(0, 100, 0, 0.05, 0.65, 2, 0.02, 10.0, 0.01, 0.1, 2)	
			show_hudmessage(id,"")
		#endif
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

public Item_Glove_Give(id) 
{ 
	if ( !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}

	// User needs to be connected and alive
	if( !p_data_b[id][PB_ISCONNECTED] || !is_user_alive(id) )
	{
		return PLUGIN_CONTINUE;
	}

	// If the user doesn't have a grenade then we can give him one
	if ( !SHARED_HasGrenade( id ) )
	{
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			set_hudmessage( 0, 100, 0, 0.05, 0.65, 2, 0.02, 10.0, 0.01, 0.1, 2 );
			show_hudmessage( id, "%L", id, "ENJOY_A_GRENADE" );

			give_item( id, "weapon_hegrenade" );
		}
		else if ( g_MOD == GAME_DOD )
		{
			new szMessage[128];
			format( szMessage, 127, "%L", id, "ENJOY_A_GRENADE" );
			Create_HudText( id, szMessage, 1 );
		
			// Give the user a grenade based on their team
			if( get_user_team( id ) == ALLIES )
			{
				give_item( id, "weapon_handgrenade" );
			}
			else
			{
				give_item( id, "weapon_stickgrenade" );
			}
		}
	} 

	p_data_b[id][PB_NADEJUSTRECEIVED] = false;

	return PLUGIN_CONTINUE;
} 


public ITEM_Glove_Check( id )
{
	if ( p_data[id][P_ITEM2] == ITEM_GLOVES )
	{
		
		if( !p_data_b[id][PB_NADEJUSTRECEIVED] && !SHARED_HasGrenade( id ) )
		{
			new parm[2];
			parm[0] = id;
			parm[1] = get_pcvar_num( CVAR_wc3_glove_timer );

			p_data_b[id][PB_NADEJUSTRECEIVED] = true;
			_Item_Glove( parm );
		}
	}
}

// ****************************************
// Mole
// ****************************************

public _Item_Mole(parm[]){ // For ITEM_MOLE, checks to see if there is an open spot on the other team's spawn 

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE
	
	new entSpawn
	new Float:spawnAngle[3] 
	new Float:spawnOrigin[3]

	if(get_user_team(id)==CTS)
		 entSpawn = find_free_spawn(id, TS, spawnOrigin, spawnAngle)
	else
		 entSpawn = find_free_spawn(id, CTS, spawnOrigin, spawnAngle)

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
			set_user_health(id, get_user_health(id) + 1)
		}

		awardHealth--
	}

	set_task(2.0,"_Item_Ring",TASK_ITEM_RINGERATE+id,parm,2)

	return PLUGIN_CONTINUE
}

// Called when a user looks/shoots somewhere
public traceline(Float:v1[3], Float:v2[3], noMonsters, pentToSkip)
{
	new iAttacker = pentToSkip;
	new iVictim = get_tr(TR_pHit);
	new iHitZone = (1 << get_tr(TR_iHitgroup));

	if ( iVictim >= 1 && iVictim <= MAXPLAYERS && p_data_b[iVictim][PB_IMMUNE_HEADSHOTS] )
	{
		// If its a headshot then we want to block it
		if ( iHitZone & (1 << 1) )
		{
			set_tr(TR_flFraction, 1.0);
			
			// Do the check to see if we should flash the screen orange
			new Float:time = halflife_time();
			if ( (iAttacker >= 1) && (iAttacker <= MAXPLAYERS) && time - fLastShotFired[iAttacker] < 0.1 )
			{
				Create_ScreenFade(iVictim, (1<<10), (1<<10), (1<<12), 250, 164, 20, 150);
			}
			
			return FMRES_SUPERCEDE;
		}
	}
	
	return FMRES_IGNORED;
}
