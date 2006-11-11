/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Item Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

// Item Setup Functions
ITEM_Init()
{
	ITEM_COST[ITEM_ANKH]	    = 1500;			// Ankh of Reincarnation
	ITEM_COST[ITEM_BOOTS]       = 2500;			// Boots of Speed
	ITEM_COST[ITEM_CLAWS]	    = 1000;			// Claws of Attack
	ITEM_COST[ITEM_CLOAK]	    = 800;			// Cloak of Shadows
	ITEM_COST[ITEM_MASK]	    = 2000;			// Mask of Death
	ITEM_COST[ITEM_NECKLACE]	= 800;			// Necklace of Immunity
	ITEM_COST[ITEM_FROST]	    = 2000;			// Orb of Frost
	ITEM_COST[ITEM_HEALTH]	    = 1000;			// Periapt of Health
	ITEM_COST[ITEM_TOME]	    = 4000;			// Tome of Experience
	ITEM_COST[ITEM_SCROLL]	    = 6000;			// Scroll of Respawning
	ITEM_COST[ITEM_PROTECTANT]	= 1500;			// Mole Protectant
	ITEM_COST[ITEM_HELM]	    = 3000;			// Helm of Excellence
	ITEM_COST[ITEM_AMULET]	    = 1500;			// Amulet of the Cat
	ITEM_COST[ITEM_SOCK]	    = 1500;			// Sock of the Feather
	ITEM_COST[ITEM_GLOVES]	    = 1750;			// Flaming Gloves of Warmth
	ITEM_COST[ITEM_RING]	    = 1000;			// Ring of Regeneration + 1
	ITEM_COST[ITEM_CHAMELEON]	= 4500;			// Chameleon
	ITEM_COST[ITEM_MOLE]	    = 16000;		// Mole

	// Item costs are a little different for DOD
	if ( g_MOD == GAME_DOD )
	{
		ITEM_COST[ITEM_SCROLL]	= 800;
	}

	// Items are chargeable
	g_iFlag[ITEM_NECKLACE]	|= ITEM_CHARGEABLE;
	g_iFlag[ITEM_HELM]		|= ITEM_CHARGEABLE;
	g_iFlag[ITEM_RING]		|= ITEM_CHARGEABLE;

	// Items should be used when bought
	g_iFlag[ITEM_TOME]		|= ITEM_USEONBUY;

	// Items CAN be bought when dead
	g_iFlag[ITEM_ANKH]		|= ITEM_BUYWHENDEAD;
	g_iFlag[ITEM_SCROLL]	|= ITEM_BUYWHENDEAD;
	g_iFlag[ITEM_MOLE]		|= ITEM_BUYWHENDEAD;
}

public ITEM_Buy( id, iItem )
{

	// User doesn't have the money
	if ( SHARED_GetUserMoney( id ) < ITEM_COST[iItem] )
	{
		client_print( id, print_center, "%L", id, "INSUFFICIENT_FUNDS" );

		return;
	}
	
	// User already owns the item and it's not a chargeable item!
	else if ( ITEM_Has( id, iItem ) > ITEM_NONE && !ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
	{
		client_print( id, print_center, "%L", id, "ALREADY_OWN_THAT_ITEM" );

		return;
	}
	
	// Make sure these items can be bought if the user is dead
	else if ( !is_user_alive( id ) && !ITEM_CheckFlag( iItem, ITEM_BUYWHENDEAD ) ) 
	{
		client_print( id, print_center, "%L", id, "NOT_PURCHASE_WHEN_DEAD" );

		return;
	}
	
	// User has necklace + blink, they don't need a necklace
	else if ( iItem == ITEM_NECKLACE && p_data_b[id][PB_WARDENBLINK] )
	{
		client_print( id, print_center, "You are already immune to ultimates through one of your skills!" );

		return;
	}

	// User doesn't need an ankh if they're going to reincarnate
	else if ( iItem == ITEM_ANKH && SM_GetSkillLevel( id, SKILL_REINCARNATION ) == 3 )
	{
		client_print( id, print_center, "You will already reincarnate your weapons through one of your skills!" );

		return;
	}
	
	// User has purchased the maximum allowed rings
	else if ( g_iTotalRings[id] >= 5 && iItem == ITEM_RING )
	{
		client_print( id, print_center, "%L", id, "NOT_PURCHASE_MORE_THAN_FIVE_RINGS" );

		return;
	}

	// User has purchased gloves when they're disabled on this map
	else if ( iItem == ITEM_GLOVES && g_bGlovesDisabled )
	{
		client_print( id, print_chat, "Gloves are disabled on this map!" );

		return;
	}

	// Check to see if the user can equip the item (Has < 2 items and item isn't a buy on use)
	else if ( ITEM_GetSlot( id ) == ITEM_SLOT_FULL && !ITEM_CheckFlag( iItem, ITEM_USEONBUY ) )
	{
		new bool:bShowReplaceMenu = false;

		// If the item isn't chargeable we need to replace it
		if ( !ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
		{
			client_print( id, print_chat, "[DEBUG] Item is not chargeable" );

			bShowReplaceMenu = true;
		}

		// We also need to replace it if the item is chargeable but they don't own that item
		if ( ITEM_Has( id, iItem ) == ITEM_NONE && ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
		{
			client_print( id, print_chat, "[DEBUG] Doesn't have item and new item is chargeable" );

			bShowReplaceMenu = true;
		}

		if ( bShowReplaceMenu )
		{
			g_iFutureItem[id] = iItem;

			MENU_ReplaceItem( id )

			return;
		}
	}

	if ( ITEM_PreSet( id, iItem ) )
	{
		new iNewMoney = SHARED_GetUserMoney( id ) - ITEM_COST[iItem];
		SHARED_SetUserMoney( id, iNewMoney );
	}

	return;
}

// Format the item for WC3_ShowBar
ITEM_Format( id, iItem, szItemString[], iLen )
{
	new szItemName[32];
	LANG_GetItemName( iItem, id, szItemName, 31, true );

	// Special options
	if ( iItem == ITEM_NECKLACE )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, g_iNecklaceCharges[id] );
	}

	else if ( iItem == ITEM_HELM )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, g_iHelmCharges[id] );
	}

	else if ( iItem == ITEM_RING )
	{
		formatex( szItemString, iLen, "%s[%d]", szItemName, g_iTotalRings[id] );
	}
	
	// All other cases
	else
	{
		copy( szItemString, iLen, szItemName );
	}
}

// Item Buy Functions
bool:ITEM_CanBuy( id )
{
	if ( !get_pcvar_num( CVAR_wc3_buy_dead ) && !is_user_alive( id ) )
	{
		client_print( id, print_center, "%L", id, "NOT_BUY_ITEMS_WHEN_DEAD" );
		
		return false;
	}
	
	else if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		
		if ( get_pcvar_num( CVAR_wc3_buy_time ) && !g_buyTime )
		{
			client_print( id, print_center, "%L", id, "SECONDS_HAVE_PASSED_CANT_BUY", ( get_cvar_float( "mp_buytime" ) * 60.0 ) );

			return false;
		}
		
		else if ( get_pcvar_num( CVAR_wc3_buy_zone ) && !cs_get_user_buyzone( id ) && is_user_alive( id ) )
		{
			client_print( id, print_center, "%L", id, "MUST_BE_IN_BUYZONE" );
			
			return false;
		}
	}
	
	return true;
}

// Item Preset Function
public ITEM_PreSet( id, iItem )
{

	// This item we should use instantly
	if ( ITEM_CheckFlag( iItem, ITEM_USEONBUY ) )
	{
		if ( iItem == ITEM_TOME )
		{
			ITEM_Tome( id );

			return 1;
		}
	}

	// They are just adding some charges
	else if ( ITEM_CheckFlag( iItem, ITEM_CHARGEABLE ) )
	{
		ITEM_SetMultipleItems( id, iItem );
	}

	// This is a replaceable item, lets do it!
	else
	{
		// Equip the item
		ITEM_Equip( id, iItem );

		// Give bonuses
		ITEM_Set( id, iItem )

		return 1;
	}

	ITEM_Equip( id, iItem );

	return 1;
}

// Item Set Functions
public ITEM_Set( id, iItem )
{	
	// Display a message regarding the item they just purchased
	switch ( iItem )
	{
		case ITEM_ANKH:
		{
			(g_MOD == GAME_DOD)								? client_print( id, print_chat,"%s %L", g_MODclient, id, "DOD_INFO_SHOPMENU_1" ) : 0;
			(g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO)	? client_print( id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_1" ) : 0;

			g_bPlayerBoughtAnkh[id] = true;
		}

		case ITEM_BOOTS:
		{
			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{
				client_print( id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_2", ( ( ITEM_BOOT_INCREASE - 1.00 ) * 100.00 ) );
			}

			else if ( g_MOD == GAME_DOD )
			{
				client_print( id, print_chat,"%s %L", g_MODclient, id, "DOD_INFO_SHOPMENU_2" );
			}

			SHARED_SetSpeed( id );
		}

		case ITEM_CLAWS:
		{
			client_print( id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_3", get_pcvar_num( CVAR_wc3_claw ) );
		}

		case ITEM_CLOAK:
		{
			new Float:fInvis = 100.0 * ( float( get_pcvar_num( CVAR_wc3_cloak ) ) / 255.0 );
			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_4", fInvis );
			SHARED_INVIS_Set( id );
		}

		case ITEM_MASK:
		{
			new Float:fMask = ( 100.0 * get_pcvar_float( CVAR_wc3_mask ) );
			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_5", fMask );
		}

		case ITEM_FROST:
		{
			new Float:fFrost = 100.0 * ( get_pcvar_float( CVAR_wc3_frost ) / 260.0 );
			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_7", fFrost );
		}

		case ITEM_HEALTH:
		{
			new iHealth = get_pcvar_num( CVAR_wc3_health );
			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_8", iHealth );

			iHealth += get_user_health( id );
			set_user_health( id, iHealth );
		}

		case ITEM_SCROLL:
		{
			if ( is_user_alive( id ) )
			{
				client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_1" );
			}

			else
			{
				client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_1_DEAD" );

				ITEM_Scroll( id );
			}
		}

		case ITEM_PROTECTANT:
		{
			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_2" );
		}

		case ITEM_AMULET:
		{
			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_4" );
		}

		case ITEM_SOCK:
		{
			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_5" );
			SHARED_SetGravity( id );
		}

		case ITEM_GLOVES:
		{
			new iGlovesTimer = get_pcvar_num( CVAR_wc3_glove_timer );
			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_6", iGlovesTimer );
			ITEM_Gloves( id );
		}

		case ITEM_CHAMELEON:
		{
			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{
				client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_8" );

				SHARED_ChangeSkin( id, SKIN_SWITCH );
			}
		}

		case ITEM_MOLE:
		{
			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_9" );
			
			g_bPlayerBoughtMole[id] = true;
		}

	}

	return 1;
}

// Item Equip Function
public ITEM_Equip( id, iItem )
{
	new iItemSlot = ITEM_GetSlot( id );

	// Items are full
	if ( iItemSlot != ITEM_SLOT_FULL )
	{

		new iOldItem = g_iShopMenuItems[id][iItemSlot];

		if ( iItem == iOldItem || ITEM_Has( id, iItem ) > ITEM_NONE )
		{
			return;
		}

		// Remove the user's old item if necessary
		else if ( g_iShopMenuItems[id][iItemSlot] > ITEM_NONE )
		{
			ITEM_Remove( id, iItemSlot );
		}
			
		// Set their new item
		g_iShopMenuItems[id][iItemSlot] = iItem;
	}

	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_PICKUPITEM], 1.0, ATTN_NORM, 0, PITCH_NORM );
	WC3_ShowBar( id );

	return;
}

ITEM_SetMultipleItems( id, iItem )
{
	switch( iItem )
	{
		case ITEM_NECKLACE:
		{
			g_iNecklaceCharges[id] += NECKLACE_CHARGES;

			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU_6", NECKLACE_CHARGES );
		}

		case ITEM_HELM:
		{
			new iCharges = HELM_CHARGES;

			g_iHelmCharges[id] += iCharges;
	
			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_3", iCharges );
		}

		case ITEM_RING:
		{
			g_iTotalRings[id] += RING_INCREMENT;

			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_7" );

			if ( !task_exists( TASK_ITEM_RING + id ) )
			{
				client_print( id, print_chat, "[DEBUG] Calling _ITEM_Ring" );
				_ITEM_Ring( id );
			}
		}
	}

	WC3_ShowBar( id );

	return 1;
}

// Item Remove Functions
ITEM_RemoveID( id, iItem )
{
	new iItemSlot = ITEM_Has( id, iItem );

	if ( iItemSlot > ITEM_NONE )
	{
		ITEM_Remove( id, iItemSlot );
	}

	return;
}

public ITEM_Remove( id, iItemSlot )
{
	new iItem = g_iShopMenuItems[id][iItemSlot];

	g_iShopMenuItems[id][iItemSlot] = ITEM_NONE;

	switch( iItem )
	{
		case ITEM_BOOTS:
		{
			SHARED_SetSpeed( id );
		}

		case ITEM_CLOAK:
		{
			SHARED_INVIS_Set( id );
		}

		case ITEM_NECKLACE:
		{
			g_iNecklaceCharges[id] = 0;
		}

		case ITEM_HEALTH:
		{
			new iNewHealth = get_user_health( id ) - get_pcvar_num( CVAR_wc3_health );
			
			// Lets not kill the user, give them 1 health
			iNewHealth = ( ( iNewHealth <= 0 ) ? 1 : iNewHealth );

			set_user_health( id, iNewHealth );
		}

		case ITEM_HELM:
		{
			g_iHelmCharges[id] = 0;
		}
		
		case ITEM_SOCK:
		{
			SHARED_SetGravity( id );
		}

		case ITEM_GLOVES:
		{
			if ( task_exists( TASK_ITEM_GLOVES + id ) )
			{
				remove_task( TASK_ITEM_GLOVES + id );
			}
		}

		case ITEM_RING:
		{
			if ( task_exists( TASK_ITEM_RING + id ) )
			{
				remove_task( TASK_ITEM_RING + id );
			}
			
			// Set the number of rings to 0
			g_iTotalRings[id] = 0;
		}

		case ITEM_CHAMELEON:
		{
			SHARED_ChangeSkin( id, SKIN_RESET );
		}
	}

	WC3_ShowBar( id );

	return;
}

ITEM_RemoveMultipleItems( id, iItem )
{
	if ( ITEM_Has( id, iItem ) > ITEM_NONE )
	{
		switch ( iItem )
		{
			case ITEM_NECKLACE:
			{
				g_iNecklaceCharges[id] -= CHARGE_DISPOSE;
				
				if ( g_iNecklaceCharges[id] <= 0 )
				{
					ITEM_RemoveID( id, iItem );
				}
			}

			case ITEM_HELM:
			{
				g_iHelmCharges[id] -= CHARGE_DISPOSE;
				
				if ( g_iHelmCharges[id] <= 0 )
				{
					ITEM_RemoveID( id, iItem );
				}
			}

			case ITEM_RING:
			{
				g_iTotalRings[id] -= CHARGE_DISPOSE;
				
				if ( g_iTotalRings[id] <= 0 )
				{
					ITEM_RemoveID( id, iItem );
				}
			}
		}
	}

	WC3_ShowBar( id );
	return;
}


// Item Get Functions
ITEM_GetSlot( id )
{
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE )
		return ITEM_SLOT_FULL;

	else if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE )
		return ITEM_SLOT_TWO;

	return ITEM_SLOT_ONE;
}

ITEM_Has( id, iItem )
{
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] == iItem )
		return ITEM_SLOT_ONE;

	else if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] == iItem )
		return ITEM_SLOT_TWO;

	return ITEM_NONE;
}

// Item Death Function
ITEM_UserDied( id )
{
	// The user just died, remove all items
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE )
	{
		ITEM_Remove( id, ITEM_SLOT_ONE );
	}

	if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		ITEM_Remove( id, ITEM_SLOT_TWO );
	}
}

// Item Specific Functions

ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )
{

	// Claws of Attack
	if ( ITEM_Has( iAttacker, ITEM_CLAWS ) > ITEM_NONE )
	{	
		WC3_Damage( iVictim, iAttacker, get_pcvar_num( CVAR_wc3_claw ), iWeapon, iHitPlace );
		
		SHARED_Glow( iAttacker, (2 * get_pcvar_num( CVAR_wc3_claw ) ), 0, 0, 0 );

		Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
	}

	// Mask of Death
	else if ( ITEM_Has( iAttacker, ITEM_MASK ) > ITEM_NONE )
	{
		new iHealth = get_user_health( iAttacker );
		new iBonusHealth = floatround( float( iDamage ) * get_pcvar_num( CVAR_wc3_mask ) );
		
		new iVampiricBonus = SM_GetSkillLevel( iAttacker, SKILL_VAMPIRICAURA );
		
		// Then the user already gets a bonus, lets lower the total amount the user is going to get
		if ( iVampiricBonus )
		{
			iBonusHealth /= iVampiricBonus;
		}
		
		// User needs to be set to max health
		if ( iHealth + iBonusHealth > get_user_maxhealth( iAttacker ) )
		{
			set_user_health( iAttacker, get_user_maxhealth( iAttacker ) );
		}
		
		// Give them bonus
		else
		{
			set_user_health( iAttacker, iHealth + iBonusHealth );
		}

		SHARED_Glow( iAttacker, 0, iBonusHealth, 0, 0 );

		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 0, 255, 0, g_GlowLevel[iAttacker][1] );
	}

	// Orb of Frost
	else if ( ITEM_Has( iAttacker, ITEM_FROST ) > ITEM_NONE )
	{
		// Only slow them if they aren't slowed/stunned already
		if ( !SHARED_IsPlayerSlowed( iVictim ) )
		{

			p_data_b[iVictim][PB_SLOWED]	= true;

			SHARED_SetSpeed( iVictim );

			set_task( 1.0, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + iVictim );

			SHARED_Glow( iAttacker, 0, 0, 0, 100 );

			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 255, 255, g_GlowLevel[iVictim][3] );
		}
	}
}

ITEM_Tome( id )
{
	new iXp = get_pcvar_num( CVAR_wc3_tome ) + xpgiven[p_data[id][P_LEVEL]];
		
	if ( g_MOD == GAME_DOD )
	{
		iXp *= 2;
	}

	client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU_9", iXp );

	XP_Give( id, iXp );

	emit_sound( id, CHAN_STATIC, "warcraft3/Tomes.wav", 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}

ITEM_Gloves( id )
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( !SHARED_HasGrenade( id ) )
	{
		_ITEM_Glove_Give( id );
	}

	return;
}

public _ITEM_Glove_Give( id )
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( id >= TASK_ITEM_GLOVES )
	{
		id -= TASK_ITEM_GLOVES;
	}

	if ( !p_data_b[id][PB_ISCONNECTED] || !is_user_alive( id ) )
	{
		return;
	}

	// Counter-Strike or Condition Zero grenade
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		give_item( id, "weapon_hegrenade" );
	}

	// Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		if ( get_user_team( id ) == ALLIES )
		{
			give_item( id, "weapon_handgrenade" );
		}
		else
		{
			give_item( id, "weapon_stickgrenade" );
		}
	}
	
	// Display a message to the user
	WC3_StatusText( id, TXT_TOP_CENTER, "%L", id, "ENJOY_A_GRENADE" )

	return;
}		

ITEM_BuyRings( id )
{
	new iMoney;
	new iAdditionalRings = 0;
	
	while ( g_iTotalRings[id] + iAdditionalRings < 5 )
	{
		iMoney = SHARED_GetUserMoney( id );

		if ( iMoney < ITEM_COST[ITEM_RING] )
		{
			break;
		}

		iAdditionalRings++;
		
		new iNewMoney = iMoney - ITEM_COST[ITEM_RING];
		SHARED_SetUserMoney( id, iNewMoney, 1 );
	}

	// Then we need to give them some rings!
	if ( iAdditionalRings > 0 )
	{

		// Subtract 1 b/c ITEM_PreSet will add one
		g_iTotalRings[id] += ( ( iAdditionalRings * RING_INCREMENT ) - ( RING_INCREMENT ) );

		ITEM_PreSet( id, ITEM_RING );
	}

	return;
}

public _ITEM_Ring( id )
{
	if ( !WC3_Check() )
	{
		return;
	}

	if ( id >= TASK_ITEM_RING )
	{
		id -= TASK_ITEM_RING;
	}

	client_print( id, print_chat, "[DEBUG] _ITEM_Ring %d || %d", (!p_data_b[id][PB_ISCONNECTED]), (ITEM_Has( id, ITEM_RING ) == ITEM_NONE) );

	if ( !p_data_b[id][PB_ISCONNECTED] || ( ITEM_Has( id, ITEM_RING ) == ITEM_NONE && g_iTotalRings[id] == 0 ) )
	{
		return;
	}

	client_print( id, print_chat, "[DEBUG] 1" );

	new iBonusHealth = g_iTotalRings[id];

	while ( iBonusHealth > 0 )
	{
		new iHealth =  get_user_health( id ) + 1;

		if ( iHealth <= get_user_maxhealth( id ) )
		{
			set_user_health( id, iHealth );
		}

		iBonusHealth--;
	}

	client_print( id, print_chat, "[DEBUG] 2" );

	set_task( 2.0, "_ITEM_Ring", TASK_ITEM_RING + id );

	return;
}

ITEM_Scroll( id )
{
	// Make sure the user isn't about to respawn when we do these checks
	if ( !p_data[id][P_RESPAWNBY] )
	{
		p_data[id][P_RESPAWNBY] = RESPAWN_ITEM;

		set_task( SPAWN_DELAY, "_SHARED_Spawn", TASK_SPAWN + id );
	}
}

ITEM_CheckFlag( iItemID, iFlag )
{
	if ( g_iFlag[iItemID] & iFlag )
	{
		return true;
	}

	return false;	
}

ITEM_RoundStart()
{



}