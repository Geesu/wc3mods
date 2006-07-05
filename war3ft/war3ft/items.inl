/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Item Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/


#define DOD_BOOT_SPEED 45.0

public ITEM_Buy( id, iItem )
{

	// User doesn't have the money
	if ( SHARED_GetUserMoney( id ) < ITEM_COST[iItem] )
	{
		client_print( id, print_center, "%L", id, "INSUFFICIENT_FUNDS" );

		return;
	}
	
	// User already owns the item!
	else if ( ITEM_Has( id, iItem ) && iItem != ITEM_RING && iItem != ITEM_NECKLACE && iItem != ITEM_HELM )
	{
		client_print( id, print_center, "%L", id, "ALREADY_OWN_THAT_ITEM" );

		return;
	}
	
	// Only MOLE + ANKH + SCROLL can be bought when dead
	else if ( !is_user_alive( id ) && ( iItem != ITEM_ANKH && iItem != ITEM_MOLE && iItem != ITEM_SCROLL ) ) 
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
	else if ( iItem == ITEM_ANKH && SM_VerifySkill( id, SKILL_REINCARNATION ) == 3 )
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

	if ( iItem == ITEM_TOME )
	{
		ITEM_Tome( id );
		new iNewMoney = SHARED_GetUserMoney( id ) - ITEM_COST[iItem];
		SHARED_SetUserMoney( id, iNewMoney );
	}

	else if ( ITEM_Set( id, iItem ) )
	{
		new iNewMoney = SHARED_GetUserMoney( id ) - ITEM_COST[iItem];
		SHARED_SetUserMoney( id, iNewMoney );
	}

	return;
}

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

public ITEM_Set( id, iItem )
{
	
	// Check for the items we can buy more than one of!
	if ( iItem == ITEM_RING && ITEM_Has( id, ITEM_RING ) )
	{}

	else if ( iItem == ITEM_NECKLACE && ITEM_Has( id, ITEM_NECKLACE ) )
	{}

	else if ( iItem == ITEM_HELM && ITEM_Has( id, ITEM_HELM ) )
	{}

	else
	{
		new iItemSlot = ITEM_GetSlot( id );
		
		server_print( "%d:%d", id, iItemSlot );

		new iOldItem = g_iShopMenuItems[id][iItemSlot];

		// Remove the user's old item if necessary
		if ( iOldItem > ITEM_NONE )
		{
			ITEM_Remove( id, iOldItem, iItemSlot );
		}
		
		// Set their new item
		g_iShopMenuItems[id][iItemSlot] = iItem;
	}

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
				new Float:fSpeed =  100.0 * ( 1.0-( 260.0 / get_pcvar_float( CVAR_wc3_boots ) ) );
				client_print( id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_2", fSpeed );
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
			new Float:fMask = ( 100.0 * get_pcvar_num( CVAR_wc3_mask ) );
			client_print(id, print_chat,"%s %L", g_MODclient, id, "INFO_SHOPMENU_5", fMask );
		}

		case ITEM_NECKLACE:
		{
			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU_6", NECKLACE_CHARGES );

			g_iNecklaceCharges[id] += NECKLACE_CHARGES;
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

		case ITEM_HELM:
		{
			new iRandomCharges = random_num( 3, 5 );

			g_iHelmCharges[id] += iRandomCharges;

			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_3", iRandomCharges );
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

		case ITEM_RING:
		{
			client_print( id, print_chat, "%s %L", g_MODclient, id, "INFO_SHOPMENU2_7" );

			g_iTotalRings[id]++;

			if ( !task_exists( TASK_ITEM_RING + id ) )
			{
				_ITEM_Ring( id );
			}
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

	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_PICKUPITEM], 1.0, ATTN_NORM, 0, PITCH_NORM );
	WC3_ShowBar( id );

	return 1;
}

ITEM_RemoveSlot( id, iItemSlot )
{
	new iOldItem = g_iShopMenuItems[id][iItemSlot];

	// Remove the user's old item if necessary
	if ( iOldItem > ITEM_NONE )
	{
		ITEM_Remove( id, iOldItem, iItemSlot );
	}
}

// Remove an item based on it's ID
ITEM_RemoveID( id, iItem )
{
	
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] == iItem )
	{
		ITEM_Remove( id, iItem, ITEM_SLOT_ONE );
	}

	else if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] == iItem )
	{
		ITEM_Remove( id, iItem, ITEM_SLOT_TWO );
	}
}

public ITEM_Remove( id, iItem, iItemSlot )
{
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
				g_iTotalRings[id] = 0;

				remove_task( TASK_ITEM_RING + id );
			}
		}

		case ITEM_CHAMELEON:
		{
			SHARED_ChangeSkin( id, SKIN_RESET );
		}
	}

	WC3_ShowBar( id );

	return PLUGIN_HANDLED;
}

ITEM_GetSlot( id )
{
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE )
		return ITEM_SLOT_FULL;

	else if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE )
		return ITEM_SLOT_TWO;

	return ITEM_SLOT_ONE;
}

bool:ITEM_Has( id, iItem )
{
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] == iItem || g_iShopMenuItems[id][ITEM_SLOT_TWO] == iItem )
		return true;

	return false;
}

ITEM_Tome( id )
{
	if ( !WAR3_Check() || !p_data_b[id][PB_ISCONNECTED] )
		return;

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
	if ( !WAR3_Check() )
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
	if ( !WAR3_Check() )
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
	new iMoney, bool:bSet = false;
	
	while ( g_iTotalRings[id] < 5 )
	{
		iMoney = SHARED_GetUserMoney( id );

		if ( iMoney < ITEM_COST[ITEM_RING] )
		{
			break;
		}
		
		new iNewMoney = iMoney - ITEM_COST[ITEM_RING];
		SHARED_SetUserMoney( id, iNewMoney, 1 );
		
		if ( !bSet )
		{
			ITEM_Set( id, ITEM_RING );

			bSet = true;
		}
	}

	return;
}

public _ITEM_Ring( id )
{
	if ( !WAR3_Check() )
	{
		return;
	}

	if ( id >= TASK_ITEM_RING )
	{
		id -= TASK_ITEM_RING;
	}

	if ( !p_data_b[id][PB_ISCONNECTED] || !ITEM_Has( id, ITEM_RING ) )
	{
		return;
	}

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

ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace )
{
	if ( !SHARED_ValidPlayer( iAttacker ) )
	{
		return;
	}

	// Claws of Attack
	if ( ITEM_Has( iAttacker, ITEM_CLAWS ) )
	{	
		WC3_Damage( iVictim, iAttacker, get_pcvar_num( CVAR_wc3_claw ), iWeapon, iHitPlace );
		
		SHARED_Glow( iAttacker, (2 * get_pcvar_num( CVAR_wc3_claw ) ), 0, 0, 0 );

		Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 0, 0, g_GlowLevel[iVictim][0] );
	}

	// Mask of Death
	else if ( ITEM_Has( iAttacker, ITEM_MASK ) )
	{
		new iHealth = get_user_health( iAttacker );
		new iBonusHealth = floatround( float( iDamage ) * get_pcvar_num( CVAR_wc3_mask ) );
		
		new iVampiricBonus = SM_VerifySkill( iAttacker, SKILL_VAMPIRICAURA );
		
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
	else if ( ITEM_Has( iAttacker, ITEM_FROST ) )
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

// Called last after a user dies
ITEM_UserDied( id )
{

	// The user just died, remove all items
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE )
	{
		ITEM_Remove( id, g_iShopMenuItems[id][ITEM_SLOT_ONE], ITEM_SLOT_ONE );
	}

	if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		ITEM_Remove( id, g_iShopMenuItems[id][ITEM_SLOT_TWO], ITEM_SLOT_TWO );
	}
}

// Initialize the ITEM costs
ITEM_Init()
{
	
	ITEM_COST[0]	= 1500;			// Ankh of Reincarnation
	ITEM_COST[1]	= 2500;			// Boots of Speed
	ITEM_COST[2]	= 1000;			// Claws of Attack
	ITEM_COST[3]	= 800;			// Cloak of Shadows
	ITEM_COST[4]	= 2000;			// Mask of Death
	ITEM_COST[5]	= 800;			// Necklace of Immunity
	ITEM_COST[6]	= 2000;			// Orb of Frost
	ITEM_COST[7]	= 1000;			// Periapt of Health
	ITEM_COST[8]	= 4000;			// Tome of Experience
	ITEM_COST[9]	= 6000;			// Scroll of Respawning
	ITEM_COST[10]	= 1500;			// Mole Protectant
	ITEM_COST[11]	= 3000;			// Helm of Excellence
	ITEM_COST[12]	= 1500;			// Amulet of the Cat
	ITEM_COST[13]	= 1500;			// Sock of the Feather
	ITEM_COST[14]	= 1750;			// Flaming Gloves of Warmth
	ITEM_COST[15]	= 1000;			// Ring of Regeneration + 1
	ITEM_COST[16]	= 4500;			// Chameleon
	ITEM_COST[17]	= 16000;		// Mole

	// Item costs are a little different for DOD
	if ( g_MOD == GAME_DOD )
	{
		ITEM_COST[9]	= 800;
	}
}

ITEM_NeckRemoveCharge( id )
{
	g_iNecklaceCharges[id]--;
	
	if ( g_iNecklaceCharges[id] <= 0 )
	{
		ITEM_RemoveID( id, ITEM_NECKLACE );
	}
}

ITEM_HelmRemoveCharge( id )
{
	g_iHelmCharges[id]--;
	
	client_print( id, print_chat, "[DEBUG] You lost a helm charge!  %d left", g_iHelmCharges[id] );

	if ( g_iHelmCharges[id] <= 0 )
	{
		ITEM_RemoveID( id, ITEM_HELM );
	}
}

// Format the item for WC3_ShowBar
ITEM_Format( id, iItem, szItemString[], len )
{
	new szItemName[32];
	LANG_GetItemName( iItem, id, szItemName, 31, true );

	// Special options
	if ( iItem == ITEM_NECKLACE )
	{
		formatex( szItemString, len, "%s x%d", szItemName, g_iNecklaceCharges[id] );
	}

	else if ( iItem == ITEM_HELM )
	{
		formatex( szItemString, len, "%s x%d", szItemName, g_iHelmCharges[id] );
	}

	else if ( iItem == ITEM_RING )
	{
		formatex( szItemString, len, "%s x%d", szItemName, g_iTotalRings[id] );
	}
	
	// All other cases
	else
	{
		copy( szItemString, len, szItemName );
	}
}
