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
			case ITEM_BOOTS:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_2", (100.0 * (1.0-(260.0/get_pcvar_float( CVAR_wc3_boots )))))
		#endif
		#if MOD == 1
			case ITEM_ANKH:			client_print(id, print_chat,"%s %L", g_MODclient, id, "DOD_INFO_SHOPMENU_1")
			case ITEM_BOOTS:		client_print(id, print_chat,"%s %L", g_MODclient, id, "DOD_INFO_SHOPMENU_2")
		#endif
			case ITEM_CLAWS:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_3", get_pcvar_num( CVAR_wc3_claw ))
			case ITEM_CLOAK:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_4", (100.0 * (float(get_pcvar_num( CVAR_wc3_cloak ))/255.0)))
			case ITEM_MASK:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_5", (100.0 * get_pcvar_num( CVAR_wc3_mask )))
			case ITEM_NECKLACE:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_6")
			case ITEM_FROST:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_7", (100.0 * (get_pcvar_float( CVAR_wc3_frost )/260.0)))
			case ITEM_HEALTH:		client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_8", get_pcvar_num( CVAR_wc3_health ))
		#if MOD == 0
			case ITEM_TOME:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_9", (get_pcvar_num( CVAR_wc3_tome ) + xpgiven[p_data[id][P_LEVEL]]))
		#endif
		#if MOD == 1
			case ITEM_TOME:			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_9", (2 * (get_pcvar_num( CVAR_wc3_tome ) + xpgiven[p_data[id][P_LEVEL]])))
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
			case ITEM_SOCK:				client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU2_5")
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

public Item_Check( id )
{

	if (!warcraft3)
		return PLUGIN_CONTINUE
	
	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(p_data_b[id][PB_DIEDLASTROUND])
	{
		WC3_ShowBar( id );
	}

	
	SHARED_SetGravity( id );

	if(p_data[id][P_ITEM2]!=ITEM_CHAMELEON && p_data[id][P_SKINCHANGED]==SKIN_SWITCH)
		SHARED_ChangeSkin(id,SKIN_RESET)

	if (p_data[id][P_ITEM]==ITEM_HEALTH)
		set_user_health(id,get_user_health(id)+get_pcvar_num( CVAR_wc3_health ))
	
	if (p_data[id][P_ITEM2]==ITEM_CHAMELEON && is_user_alive(id))
		SHARED_ChangeSkin(id,SKIN_SWITCH)


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
		ITEM_Gloves(id)
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

public ITEM_Gloves(id) 
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

public ITEM_BuyRings( id )
{
	new iMoney;
	
	// Lets buy as many rings as we can!
	while ( p_data[id][P_RINGS] < 5 )
	{
		iMoney = SHARED_GetUserMoney( id );

		// Check to see if the user can buy another ring
		if ( iMoney < itemcost2[ITEM_RING-1] )
		{
			break;
		}
		
		// Take their money
		SHARED_SetUserMoney( id, iMoney - itemcost2[ITEM_RING-1], 1 );

		// Give them the rings
		ITEM_Set( id, ITEM_RING, SHOPMENU_TWO );
	}
}

public ITEM_Ring( id )
{
	if ( !warcraft3 )
	{
		return;
	}

	if ( id > TASK_ITEM_RING )
	{
		id -= TASK_ITEM_RING;
	}


	// User no longer has rings or isn't connected
	if( !p_data_b[id][PB_ISCONNECTED] || p_data[id][P_ITEM2] != ITEM_RING )
	{
		return;
	}

	new iBonusHealth = p_data[id][P_RINGS];

	while( iBonusHealth > 0 )
	{
		if ( get_user_actualhealth( id ) + 1 <= get_user_maxhealth( id ) )
		{
			set_user_health( id, get_user_health( id ) + 1 );
		}

		iBonusHealth--;
	}

	set_task( 2.0, "ITEM_Ring", TASK_ITEM_RING + id );

	return;
}

public ITEM_Set( id, iItemID, iShopmenuID )
{

	new iLastItemID = p_data[id][iShopmenuID];

	// Lets "officially" give them the new item
	if ( iItemID != -1 )
	{
		p_data[id][iShopmenuID] = iItemID;
	}

	// Give an item bonus for shopmenu 1
	if ( iShopmenuID == SHOPMENU_ONE )
	{

		// Remove health bonus after buying new item
		if ( iLastItemID == ITEM_HEALTH )
		{
			set_user_health( id, get_user_health( id ) - get_pcvar_num( CVAR_wc3_health ) );
		}

		// Give health bonus for buying periapt of health
		if ( iItemID == ITEM_HEALTH )
		{
			set_user_health( id, get_user_health( id ) + get_pcvar_num( CVAR_wc3_health ) );
		}
		
		// If the current or last item was boots, we need to change their speed
		if ( iLastItemID == ITEM_BOOTS || iItemID == ITEM_BOOTS )
		{
			SHARED_SetSpeed( id );
		}

		// If the current or last item was cloak, we need to change their invisibility
		else if ( iLastItemID == ITEM_CLOAK || iItemID == ITEM_CLOAK )
		{
			SHARED_INVIS_Set( id );
		}
	}

	// Give an item bonus for shopmenu 2
	else if ( iShopmenuID == SHOPMENU_TWO )
	{

		// Remove all ring of regeneration functions
		if ( iLastItemID == ITEM_RING && iItemID != ITEM_RING )
		{
			if ( task_exists( TASK_ITEM_RING + id ) )
			{
				remove_task( TASK_ITEM_RING + id );
			}

			p_data[id][P_RINGS] = 0;
		}
	
		// The user no longer has gloves, lets not give him grenades
		else if ( iLastItemID == ITEM_GLOVES )
		{
			if ( task_exists( TASK_ITEM_GLOVES + id ) )
			{
				remove_task( TASK_ITEM_GLOVES + id );
			}
		}

		// Reset the user's skin if they don't have chameleon anymore
		else if ( iLastItemID == ITEM_CHAMELEON )
		{
			SHARED_ChangeSkin( id, SKIN_RESET );
		}
		
		//******* START GIVING
		
		// User just bought chameleon
		if ( iItemID == ITEM_CHAMELEON )
		{
			SHARED_ChangeSkin( id, SKIN_SWITCH );
		}

		// User has bought gloves...
		else if ( iItemID == ITEM_GLOVES )
		{
			ITEM_Gloves( id );
		}

		// User just bought a ring
		else if ( iItemID == ITEM_RING )
		{
			p_data[id][P_RINGS]++;

			if( !task_exists( TASK_ITEM_RING + id ) )
			{
				ITEM_Ring( id )
			}
		}

		// Change the user's gravity if they bought/lost sock of the feather
		if ( iLastItemID == ITEM_SOCK || iItemID == ITEM_SOCK )
		{
			SHARED_SetGravity( id );
		}
	}
	
	// Display the new item on the user's HUD
	WC3_ShowBar( id );
}

public ITEM_CanBuy( id )
{
	// Can we buy items when dead?
	if( !get_pcvar_num( CVAR_wc3_buy_dead ) && !is_user_alive( id ) )
	{
		client_print( id, print_center, "%L", id, "NOT_BUY_ITEMS_WHEN_DEAD" );
		
		return false;
	}
	
	// Counter-strike and Condition Zero ONLY check
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		
		// Are we restricting to only buying during the buytime?
		if( get_pcvar_num( CVAR_wc3_buy_time ) && !g_buyTime )
		{
			client_print( id, print_center, "%L", id, "SECONDS_HAVE_PASSED_CANT_BUY", ( get_cvar_float( "mp_buytime" ) * 60.0) );

			return false;
		}
		
		// Can we only purchase an item if we're in the buyzone?
		else if( get_pcvar_num( CVAR_wc3_buy_zone ) && !cs_get_user_buyzone( id ) && is_user_alive( id ) )
		{
			client_print( id, print_center, "%L", id, "MUST_BE_IN_BUYZONE" );
			
			return false;
		}
	}

	return true;
}

public ITEM_CanBuyItem( id, iItemID, iShopmenuID )
{
	if ( !ITEM_CanBuy( id ) )
	{
		return false;
	}

	// Shopmenu 1 Checks
	if ( iShopmenuID == SHOPMENU_ONE )
	{
		// Cannot purchase some items when dead
		if ( !is_user_alive(id) && ( iItemID == ITEM_BOOTS || iItemID == ITEM_CLAWS || iItemID == ITEM_CLOAK || iItemID == ITEM_MASK || iItemID == ITEM_NECKLACE || iItemID == ITEM_FROST || iItemID == ITEM_HEALTH) ) 
		{
			client_print( id, print_center, "%L", id, "NOT_PURCHASE_WHEN_DEAD" );

			return false;
		}

		// Cannot purchase an item they already own
		else if ( iItemID == p_data[id][iShopmenuID] && iItemID!=ITEM_TOME )
		{
			client_print( id, print_center, "%L", id, "ALREADY_OWN_THAT_ITEM" );

			return false;
		}
		
		// They don't have enough money
		else if ( SHARED_GetUserMoney( id ) < itemcost[iItemID - 1] )
		{
			client_print( id, print_center, "%L", id, "INSUFFICIENT_FUNDS" );

			return false;
		}

		// They don't need a necklace when they have warden's blink!!
		else if ( iItemID == ITEM_NECKLACE && p_data_b[id][PB_WARDENBLINK] )
		{
			client_print( id, print_center, "You are already immune to ultimates through one of your skills!" );

			return false;
		}
	}

	// Shopmenu 2 Checks
	else if ( iShopmenuID == SHOPMENU_TWO )
	{
		// Cannot only purchase mole when dead
		if ( !is_user_alive(id) && iItemID != ITEM_MOLE )
		{
			client_print( id, print_center, "%L", id, "NOT_PURCHASE_WHEN_DEAD" );

			return false;
		}

		// Cannot purchase an item they already own
		else if( iItemID == p_data[id][iShopmenuID] && iItemID != ITEM_RING )
		{
			client_print( id, print_center, "%L", id, "ALREADY_OWN_THAT_ITEM" );

			return false;
		}

		// They don't have enough money
		else if ( SHARED_GetUserMoney( id ) < itemcost2[iItemID - 1] )
		{
			client_print( id, print_center, "%L", id, "INSUFFICIENT_FUNDS" );

			return false;
		}

		// Cannot buy more than 5 rings
		else if( p_data[id][P_RINGS] > 4 && iItemID == ITEM_RING )
		{
			client_print( id, print_center, "%L", id, "NOT_PURCHASE_MORE_THAN_FIVE_RINGS" );

			return false;
		}
		
		// Counter-Strike and Condition Zero only check
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			if ( iItemID == ITEM_SCROLL && endround )
			{
				client_print( id, print_center, "%L", id, "NOT_PURCHASE_AFTER_ENDROUND" );

				return false;
			}
		}
	}

	return true;
}

// Called when the user dies so we can remove the abilities
public ITEM_Reset( id )
{
	// Lets save the user's items from when they died
	p_data[id][P_LASTITEM]			= p_data[id][P_ITEM];
	p_data[id][P_LASTITEM2]			= p_data[id][P_ITEM2];

	// Reset shopmenu 1 items
	ITEM_Set( id, -1, SHOPMENU_ONE );

	// Reset shopmenu 2 items
	ITEM_Set( id, -1, SHOPMENU_TWO );
}

// Save the user's items!!
public ITEM_Save( id )
{
	// Lets save the user's items from when they died
	p_data[id][P_LASTITEM]			= p_data[id][P_ITEM];
	p_data[id][P_LASTITEM2]			= p_data[id][P_ITEM2];
}

public ITEM_DeleteAll( id )
{
	// Reset the user's items
	p_data[id][P_ITEM]		= 0;
	p_data[id][P_ITEM2]		= 0;

}