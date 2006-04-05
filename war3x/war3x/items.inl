
/* - AMX Mod Script --------------------------------------------- *

 *  Warcraft 3: Expansion
 *  (c) Copyright 2003
 *
 *  Shop Item Constants/Functions

 * -------------------------------------------------------------- */



/* - Buy/Drop Item Functions ------------------------------------ */


// Buy Normal Item

public Item_Buy( id, iNewItem ) {

	new iOldItem = g_PlayerInfo[id][CURRENT_ITEM];

	// Insufficient Funds

	if ( cs_get_user_money( id ) + ( ITEM_COST[iOldItem] / ITEM_BUYBACK ) < ITEM_COST[iNewItem] )
	{
		client_print( id, print_center, "%L", id, "ITEM_INSUFFICIENT_FUNDS" );
		return PLUGIN_HANDLED;
	}

	// Already Equipped

	if ( g_PlayerInfo[id][CURRENT_ITEM] == iNewItem )
	{
		client_print( id, print_center, "%L", id, "ITEM_EQUIPPED" );
		return PLUGIN_HANDLED;
	}

	// Equip the Item

	g_PlayerInfo[id][CURRENT_ITEM] = iNewItem;

	if ( !Item_Equip( id, iNewItem ) )
	{
		g_PlayerInfo[id][CURRENT_ITEM] = iOldItem;
		return PLUGIN_HANDLED;
	}

	// Check for item changes

	Item_Change( id, iOldItem, iNewItem );

	if ( g_bFreezeTime )
	{
		new Float:fFreezetime = get_pcvar_float( CVAR_mp_freezetime );
		WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.0, 0.0, fFreezetime );
	}

	else
	{
		WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );
	}

	// Play Sound

	emit_sound( id, CHAN_STATIC, SOUND_PICKUPITEM, 1.0, ATTN_NORM, 0, PITCH_NORM );

	if ( iOldItem )
	{
		// Give back some cash

		new iOldCost = ITEM_COST[iOldItem] / ITEM_BUYBACK;
		new iNewCost = ITEM_COST[iNewItem];

		new iDifference = iNewCost - iOldCost;

		cs_update_money( id, -iDifference, 1 );

		// Inform player

		new szMessage[128], szItemName[32];

		LANG_GetItemName ( iOldItem, SHOP_COMMON, id, szItemName, 31 )
		formatex( szMessage, 127, "%s %L", WAR3X_PREFIX, id, "ITEM_BUYBACK_MESSAGE", szItemName, iOldCost );

		client_print( id, print_chat, "%s", szMessage );
	}

	else
	{
		// Update money

		cs_update_money( id, -ITEM_COST[iNewItem], 1 );
	}

	return PLUGIN_HANDLED;
}


// Buy Tome function

public Item_BuyTome( id ) {

	// Level 10 ( no bonus )

	if ( WAR3_get_level(g_PlayerInfo[id][CURRENT_XP]) == TOTAL_LEVELS )
	{
		client_print( id, print_center, "%L", id, "ITEM_NOBONUS" );
		return PLUGIN_HANDLED;
	}

	// Insufficient Funds

	if ( cs_get_user_money( id ) < ITEM_COST[ITEM_TOME] )
	{
		client_print( id, print_center, "%L", id, "ITEM_INSUFFICIENT_FUNDS" );
		return PLUGIN_HANDLED;
	}

	new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );

	new iTomeValue = ITEM_TOME_VALUE;
	new Float:fTomeValue = float( iTomeValue );

	client_print( id, print_center, "%L", id, "ITEM_TOME_MESSAGE", floatround( fTomeValue * fMultiplier ) );

	new iOldXp = g_PlayerInfo[id][CURRENT_XP];
	new iNewXp = g_PlayerInfo[id][CURRENT_XP] + floatround( fTomeValue * fMultiplier );

	XP_Give( id, iOldXp, iNewXp );

	// Play Sound

	emit_sound( id, CHAN_STATIC, SOUND_TOME, 1.0, ATTN_NORM, 0, PITCH_NORM );

	cs_update_money( id, -ITEM_COST[ITEM_TOME], 1 );


	return PLUGIN_HANDLED;
}


public Item_Equip( id, iNewItem ) {

	// Ankh of Reincarnation

	if ( iNewItem == ITEM_ANKH )
	{
		// Don't give to VIP

		if ( id == g_Vip )
		{
			client_print( id, print_center, "%L", id, "ITEM_UNAVAILABLE" );
			return ( 0 );
		}

		new szMessage[128], iLen;
		iLen += formatex( szMessage[iLen], 127 - iLen, "%L", id, "LANG_PRIMARY_WEAPON" );

		if ( get_pcvar_num( CVAR_ankhpistols ) )
			iLen += formatex( szMessage[iLen], 127 - iLen, ", %L", id, "LANG_SECONDARY_WEAPON" );

		if ( get_pcvar_num( CVAR_ankhgrenades ) )
			iLen += formatex( szMessage[iLen], 127 - iLen, ", %L", id, "LANG_GRENADES" );

		new Float:fAnkhValue = ITEM_ANKH_VALUE;
		client_print( id, print_chat, "%s %L (%s)", WAR3X_PREFIX, id, "ITEM_ANKH_MESSAGE", floatround( fAnkhValue * 100.0 ), szMessage );
	}

	// Boots of Speed

	else if ( iNewItem == ITEM_BOOTS )
	{
		client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "ITEM_BOOTS_MESSAGE", ( ( ( ITEM_BOOTS_get_speed( id ) / SPEED_KNIFE ) - 1.0 ) * 100 ) );
		WAR3_set_speed( id );
	}

	// Claws of Attack +6

	else if ( iNewItem == ITEM_CLAWS )
	{
		client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "ITEM_CLAWS_MESSAGE", ITEM_CLAWS_VALUE );
	}

	// Cloak of Shadows

	else if ( iNewItem == ITEM_CLOAK )
	{
		client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "ITEM_CLOAK_MESSAGE", ( ITEM_CLOAK_VALUE * 100.0 ) );
		SHARED_INVIS_set( id );
	}

	// Sobi Mask

	else if ( iNewItem == ITEM_MASK )
	{
		// Don't give mask if no ultimate

		if ( !g_PlayerInfo[id][CURRENT_ULTIMATE] )
		{
			client_print( id, print_center, "%L", id, "ITEM_NOMASK" );
			return ( 0 );
		}

		if ( g_fUltimateCooldown[id] && ( g_fBuyTime + get_pcvar_float( CVAR_ultimatewarmup ) ) < get_gametime() )
		{
			new task = TASK_ULTIMATECOOLDOWN + id;
			remove_task( task, 0 );

			new Float:fCurrentCooldown = get_gametime() - g_fUltimateCooldown[id];
			new Float:fRemainingCooldown = ULTIMATE_COOLDOWNDEFAULT - fCurrentCooldown;

			new Float:fTimeRemoved = fRemainingCooldown * ( ITEM_MASK_VALUE / ULTIMATE_COOLDOWNDEFAULT );
			new Float:fNewCooldown = fRemainingCooldown - fTimeRemoved;

			g_fUltimateCooldown[id] -= fTimeRemoved;

			new parm_Cooldown[1];
			parm_Cooldown[0] = id;

			set_task( fNewCooldown, "Ultimate_Restore", task, parm_Cooldown, 1 );
		}

		client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "ITEM_MASK_MESSAGE", ULTIMATE_COOLDOWNDEFAULT, ULTIMATE_COOLDOWNDEFAULT - ITEM_MASK_VALUE );
	}

	// Amulet of Shielding

	else if ( iNewItem == ITEM_AMULET )
	{
		g_iAmuletCharges[id] = ITEM_AMULET_VALUE;
		client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "ITEM_AMULET_MESSAGE", ITEM_AMULET_VALUE );
	}

	// Ring of Protection +5

	else if ( iNewItem == ITEM_RING )
	{
		if ( !get_user_armor( id ) )
		{
			client_print( id, print_center, "%L", id, "ITEM_NOARMOR" );
			return ( 0 );
		}

		set_user_armor( id, get_user_armor( id ) + ITEM_RING_VALUE );
		client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "ITEM_RING_MESSAGE", WAR3_get_maxarmor( id ) );
	}

	return ( 1 );
}


public Item_Change( id, iOldItem, iNewItem ) {

	// Remove Speed Bonus if New Item is not Boots

	if ( iOldItem == ITEM_BOOTS && iNewItem != ITEM_BOOTS )
	{
		WAR3_set_speed( id );
	}

	// Remove Invisibility if New Item is not Cloak

	if ( iOldItem == ITEM_CLOAK && iNewItem != ITEM_CLOAK )
	{
		new weapon, iClip, iAmmo;
		weapon = get_user_weapon( id, iClip, iAmmo );

		if ( g_PlayerInfo[id][CURRENT_RACE] != RACE_HUMAN || ( g_PlayerInfo[id][CURRENT_RACE] == RACE_HUMAN && !g_PlayerInfo[id][CURRENT_SKILL1] ) || weapon != CSW_KNIFE )
			SHARED_INVIS_remove( id );
	}

	// Remove Cooldown Bonus if New Item is not Sobi Mask

	if ( iOldItem == ITEM_MASK && iNewItem != ITEM_MASK )
	{
		if ( g_fUltimateCooldown[id] )
		{
			new task = TASK_ULTIMATECOOLDOWN + id;
			remove_task( task, 0 );

			new Float:fCurrentCooldown = get_gametime() - ( g_fUltimateCooldown[id] + ITEM_MASK_VALUE );
			new Float:fCooldownUsed = fCurrentCooldown / ( ULTIMATE_COOLDOWNDEFAULT - ITEM_MASK_VALUE );
			new Float:fTimeRemoved = fCooldownUsed * ULTIMATE_COOLDOWNDEFAULT;
			new Float:fNewCooldown = ULTIMATE_COOLDOWNDEFAULT - fTimeRemoved;

			g_fUltimateCooldown[id] = get_gametime() - fTimeRemoved;

			new parm_Cooldown[1];
			parm_Cooldown[0] = id;

			set_task( fNewCooldown, "Ultimate_Restore", task, parm_Cooldown, 1 );
		}
	}

	// Remove Bonus Armor if New Item is not Ring of Protection

	if ( iOldItem == ITEM_RING && iNewItem != ITEM_RING )
	{
		new iNewArmor = get_user_armor( id ) - ITEM_RING_VALUE;

		if ( iNewArmor < 0 )
			iNewArmor = 0;

		set_user_armor( id, iNewArmor );
	}

	return PLUGIN_HANDLED;
}


public Item_Create( id, iItemNum ) {

	new Float:fOrigin[3];
	entity_get_vector( id, EV_VEC_origin, fOrigin );

	new iEnt = WAR3_ENTITY_create( "WAR3X_ITEM", "models/sphere.mdl", fOrigin, MOVETYPE_TOSS, SOLID_TRIGGER, ITEM_DROPLIFE );

	entity_set_int( iEnt, EV_INT_rendermode, kRenderTransColor );
	entity_set_int( iEnt, EV_INT_renderfx, kRenderFxGlowShell );

	new Float:fColors[3];
	fColors[0] = float( ITEM_COLOR[iItemNum][GLOW_R] );
	fColors[1] = float( ITEM_COLOR[iItemNum][GLOW_G] );
	fColors[2] = float( ITEM_COLOR[iItemNum][GLOW_B] );

	entity_set_vector( iEnt, EV_VEC_rendercolor, fColors );
	entity_set_float( iEnt, EV_FL_renderamt, 150.0 );

	entity_set_int( iEnt, EV_ENT_owner, iItemNum );

	return ( iEnt );
}


public Item_Drop( id ) {

	if ( !g_PlayerInfo[id][CURRENT_ITEM] )
	{
		client_print( id, print_center, "%L", id, "ITEM_NODROP" );
		return PLUGIN_CONTINUE;
	}

	// Remove Item

	new iOldItem = g_PlayerInfo[id][CURRENT_ITEM];
	g_PlayerInfo[id][CURRENT_ITEM] = ITEM_NONE;

	Item_Change( id, iOldItem, ITEM_NONE );

	WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );
	client_print( id, print_center, "%L", id, "ITEM_DROP" );

	// Create Item

	new iEnt = Item_Create( id, iOldItem );

	// Toss Item

	new Float:fOrigin[3];
	entity_get_vector( id, EV_VEC_origin, fOrigin );

	new iaimOrigin[3];
	get_user_origin( id, iaimOrigin, 3 );

	new Float:faimOrigin[3];
	faimOrigin[0] = float( iaimOrigin[0] );
	faimOrigin[1] = float( iaimOrigin[1] );
	faimOrigin[2] = fOrigin[2];

	set_velocity_to_origin( iEnt, faimOrigin, 500.0 );

	return PLUGIN_HANDLED;
}


public Item_Pickup( id, iItemEnt, iNewItem ) {

	// Check if restricted

	if ( get_pcvar_bitsum( CVAR_restrict_items ) & WAR3_get_item_flag( iNewItem ) )
		return PLUGIN_HANDLED;

	g_PlayerInfo[id][CURRENT_ITEM] = iNewItem;

	if ( !Item_Equip( id, iNewItem ) )
	{
		g_PlayerInfo[id][CURRENT_ITEM] = ITEM_NONE;
		return PLUGIN_HANDLED;
	}

	Item_Change( id, ITEM_NONE, iNewItem );

	WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );
	client_print( id, print_center, "%L", id, "ITEM_PICKUP" );

	// Play Sound

	emit_sound( id, CHAN_STATIC, SOUND_PICKUPITEM, 1.0, ATTN_NORM, 0, PITCH_NORM );

	remove_entity( iItemEnt );

	return PLUGIN_HANDLED;
}


public Item_DropDead( id, iItemNum ) {

	// Create Item

	new iEnt = Item_Create( id, iItemNum );

	// Toss Item ( give player's velocity and then some )

	new Float:fVelocity[3];
	entity_get_vector( id, EV_VEC_velocity, fVelocity );

	fVelocity[0] *= 1.2;
	fVelocity[1] *= 1.2;
	fVelocity[2] *= 1.2;

	entity_set_vector( iEnt, EV_VEC_velocity, fVelocity );

	return PLUGIN_HANDLED;
}


public Item_Touch( iToucherId, iPlayerId ) {

	// Sanity checks

	if ( iPlayerId < 1 || iPlayerId > 32 || iToucherId < 1 || !is_user_alive( iPlayerId ) )
		return PLUGIN_CONTINUE;

	if ( !g_PlayerInfo[iPlayerId][CURRENT_ITEM] )
	{
		// Make sure on ground

		if ( get_entity_flags( iToucherId ) & FL_ONGROUND )
		{
			new iItemNum = entity_get_int( iToucherId, EV_ENT_owner );
			Item_Pickup( iPlayerId, iToucherId, iItemNum );
		}
	}

	return PLUGIN_CONTINUE;
}


/* - Item Functions --------------------------------------------- */


// Ankh of Reincarnation

public ITEM_ANKH_set( id ) {

	if ( id == g_Vip || g_iCurrentRound < 1 )
		return PLUGIN_HANDLED;


	new Float:fAnkhChance;
	new Float:fRandomNum = random_float( 0.0,1.0 );

	if ( g_PlayerInfo[id][CURRENT_ITEM] == ITEM_ANKH )
		fAnkhChance += ITEM_ANKH_VALUE;

	// Give Weapons

	if ( fAnkhChance >= fRandomNum )
	{
		new Weapons[32], iTotalWeapons;
		get_user_weapons( id, Weapons, iTotalWeapons );

		for ( new iWeaponNum = 0; iWeaponNum < iTotalWeapons; iWeaponNum++ )
		{
			new weapon = Weapons[iWeaponNum];

			switch ( cs_get_weapon_group( weapon ) )
			{
				case CS_WEAPON_GROUP_PRIMARY:
				{
					if ( get_pcvar_num( CVAR_ankhautosnipers ) || ( !get_pcvar_num( CVAR_ankhautosnipers ) && cs_get_weapon_type_( weapon ) != CS_WEAPON_TYPE_AUTOSNIPER ) )
					{
						g_PlayerBackpack[id][CURRENT_PRIMARY] = weapon;
					}
				}

				case CS_WEAPON_GROUP_SECONDARY:
				{
					if ( get_pcvar_num( CVAR_ankhpistols ) )
					{
						g_PlayerBackpack[id][CURRENT_SECONDARY] = weapon;
					}
				}

				case CS_WEAPON_GROUP_GRENADE:
				{
					if ( get_pcvar_num( CVAR_ankhgrenades ) )
					{
						new iClip, iAmmo;

						switch ( weapon )
						{
							case CSW_HEGRENADE:
							{
								get_user_ammo( id, CSW_HEGRENADE, iClip, iAmmo );
								g_PlayerBackpack[id][CURRENT_HEGRENADE] = iAmmo;
							}

							case CSW_SMOKEGRENADE:
							{
								get_user_ammo( id, CSW_SMOKEGRENADE, iClip, iAmmo );
								g_PlayerBackpack[id][CURRENT_SMOKEGRENADE] = iAmmo;
							}

							case CSW_FLASHBANG:
							{
								get_user_ammo( id, CSW_FLASHBANG, iClip, iAmmo );
								g_PlayerBackpack[id][CURRENT_FLASHBANG] = iAmmo;
							}
						}
					}
				}
			}
		}

		g_PlayerBackpack[id][CURRENT_ARMOR] = get_user_armor( id );
		g_PlayerBackpack[id][CURRENT_ARMORTYPE] = g_iPlayerArmor[id][ARMOR_TYPE];

		if ( g_bGotDefuser[id] )
			g_PlayerBackpack[id][CURRENT_DEFUSE] = 1;
	}

	return PLUGIN_HANDLED;
}


public ITEM_ANKH_drop( id ) {

	if ( id == g_Vip )
	{
		client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "VIP_ANKH_MESSAGE" );
		return PLUGIN_HANDLED;
	}

	g_bAnkhDropWeapons[id] = false;

	new CurrentWeapons[2];

	new Weapons[32], iTotalWeapons;
	get_user_weapons( id, Weapons, iTotalWeapons );

	// Drop Dupilcate/Extra Weapon(s)

	for ( new iWeaponNum = 0; iWeaponNum < iTotalWeapons; iWeaponNum++ )
	{
		new weapon = Weapons[iWeaponNum];

		// If Weapon(s) are saved in Ankh array, Drop all of the same
		// type to make room. If Weapon(s) stored already exist, don't
		// drop (or give) the item. If no Weapon is set to reincarnate,
		// but one exists, drop all others of the same type.

		switch ( cs_get_weapon_group( weapon ) )
		{
			// Primary Weapon(s)

			case CS_WEAPON_GROUP_PRIMARY:
			{
				if ( g_PlayerBackpack[id][CURRENT_PRIMARY] )
				{
					if ( weapon == g_PlayerBackpack[id][CURRENT_PRIMARY] )
						g_PlayerBackpack[id][CURRENT_PRIMARY] = 0;

					else
					{
						g_bAnkhDropWeapons[id] = true;
						client_cmd( id, "drop %s", CS_WEAPON_NAME[weapon] );
					}
				}

				else
				{
					if ( CurrentWeapons[CURRENT_PRIMARY] )
						client_cmd( id, "drop %s", CS_WEAPON_NAME[weapon] );

					else
					{
						CurrentWeapons[CURRENT_PRIMARY] = weapon;
					}
				}
			}

			// Sidearm(s)

			case CS_WEAPON_GROUP_SECONDARY:
			{
				if ( g_PlayerBackpack[id][CURRENT_SECONDARY] )
				{
					if ( weapon == g_PlayerBackpack[id][CURRENT_SECONDARY] )
						g_PlayerBackpack[id][CURRENT_SECONDARY] = 0;

					else
					{
						g_bAnkhDropWeapons[id] = true;
						client_cmd( id, "drop %s", CS_WEAPON_NAME[weapon] );
					}
				}

				else
				{
					if ( CurrentWeapons[CURRENT_SECONDARY] )
						client_cmd( id, "drop %s", CS_WEAPON_NAME[weapon] );

					else
					{

						CurrentWeapons[CURRENT_SECONDARY] = weapon;
					}
				}
			}
		}
	}

	if ( !g_bAnkhDropWeapons[id] )
	{
		new parm_Ankh[1];
		parm_Ankh[0] = id;

		ITEM_ANKH_give( parm_Ankh );
	}

	return PLUGIN_HANDLED;
}


public ITEM_ANKH_give( parm_Ankh[1] ) {

	new id = parm_Ankh[0];

	g_bAnkhDropWeapons[id] = false;
	new bool:bWeaponsGiven, bool:bArmorSkill;

	// Give Primary Weapon

	if ( g_PlayerBackpack[id][CURRENT_PRIMARY] )
	{
		bWeaponsGiven = true;
		give_item( id, CS_WEAPON_NAME[g_PlayerBackpack[id][CURRENT_PRIMARY]] );

		for ( new i = 0; i < ITEM_ANKH_AMMO; i++ )
		{
			give_item( id, CS_AMMO_NAME[g_PlayerBackpack[id][CURRENT_PRIMARY]] );

			/*
			switch ( g_PlayerBackpack[id][CURRENT_PRIMARY] )
			{
				case CSW_M3:        give_item( id, "ammo_buckshot" );
				case CSW_XM1014:    give_item( id, "ammo_buckshot" );
				case CSW_MP5NAVY:   give_item( id, "ammo_9mm" );
				case CSW_TMP:       give_item( id, "ammo_9mm" );
				case CSW_P90:       give_item( id, "ammo_57mm" );
				case CSW_MAC10:     give_item( id, "ammo_45acp" );
				case CSW_UMP45:     give_item( id, "ammo_45acp" );
				case CSW_AK47:      give_item( id, "ammo_762nato" );
				case CSW_SG552:     give_item( id, "ammo_556nato" );
				case CSW_GALI:      give_item( id, "ammo_556nato" );
				case CSW_FAMAS:     give_item( id, "ammo_556nato" );
				case CSW_M4A1:      give_item( id, "ammo_556nato" );
				case CSW_AUG:       give_item( id, "ammo_556nato" );
				case CSW_SCOUT:     give_item( id, "ammo_762nato" );
				case CSW_AWP:       give_item( id, "ammo_338magnum" );
				case CSW_G3SG1:     give_item( id, "ammo_762nato" );
				case CSW_SG550:     give_item( id, "ammo_556nato" );
				case CSW_M249:      give_item( id, "ammo_556natobox" );
			}
			*/
		}
	}

	// Give Sidearm

	if ( g_PlayerBackpack[id][CURRENT_SECONDARY] )
	{
		bWeaponsGiven = true;
		give_item( id, CS_WEAPON_NAME[g_PlayerBackpack[id][CURRENT_SECONDARY]] );

		for ( new i = 0; i < ITEM_ANKH_AMMO; i++ )
		{
			give_item( id, CS_AMMO_NAME[g_PlayerBackpack[id][CURRENT_SECONDARY]] );

			/*
			switch ( g_PlayerBackpack[id][CURRENT_SECONDARY] )
			{
				case CSW_USP:       give_item( id, "ammo_45acp" );
				case CSW_DEAGLE:    give_item( id, "ammo_50ae" );
				case CSW_GLOCK18:   give_item( id, "ammo_9mm" );
				case CSW_ELITE:     give_item( id, "ammo_9mm" );
				case CSW_P228:      give_item( id, "ammo_357sig" );
				case CSW_FIVESEVEN: give_item( id, "ammo_57mm" );
			}
			*/
		}
	}

	// Give Grenade(s)

	if ( g_PlayerBackpack[id][CURRENT_HEGRENADE] )
	{
		give_item( id, CS_WEAPON_NAME[CSW_HEGRENADE] );
		bWeaponsGiven = true;
	}

	if ( g_PlayerBackpack[id][CURRENT_SMOKEGRENADE] )
	{
		give_item( id, CS_WEAPON_NAME[CSW_SMOKEGRENADE] );
		bWeaponsGiven = true;
	}

	if ( g_PlayerBackpack[id][CURRENT_FLASHBANG] )
	{
		give_item( id, CS_WEAPON_NAME[CSW_FLASHBANG] );
		bWeaponsGiven = true;

//        if ( g_PlayerBackpack[id][CURRENT_FLASHBANG] == 2)
//            give_item( id, "ammo_flashbang" );
	}

	// Give Armor

	if ( g_PlayerBackpack[id][CURRENT_ARMOR] )
	{
		if ( g_PlayerBackpack[id][CURRENT_ARMORTYPE] )
			give_item( id, "item_assaultsuit" );

		else
		{
			give_item( id, "item_kevlar" );
		}

		set_user_armor( id, g_PlayerBackpack[id][CURRENT_ARMOR] );
		WAR3_check_armor( id );

		g_bArmorDepleted[id] = false;
		bWeaponsGiven = true;
	}

	// Give Defuser

	if ( g_PlayerBackpack[id][CURRENT_DEFUSE] )
	{
		give_item( id, "item_thighpack" );
		bWeaponsGiven = true;
	}


	if ( bWeaponsGiven )
	{
		// Play armor skill effect if armor given

		if ( g_PlayerBackpack[id][CURRENT_ARMOR] && get_user_armor(id) == g_PlayerBackpack[id][CURRENT_ARMOR] )
		{
			if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_UNDEAD && g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				bArmorSkill = true;
				WAR3_armorskill_on( id );
			}

			else if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_HUMAN && g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				bArmorSkill = true;
				WAR3_armorskill_on( id );
			}

			else if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC && g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				bArmorSkill = true;
				WAR3_armorskill_on( id );
			}

			else if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_NIGHTELF )
			{
				bArmorSkill = true;
				WAR3_armorskill_on( id );
			}
		}

		if ( !bArmorSkill )
		{
			// Play Client Sound

			client_cmd( id, "speak warcraft3/bonus/Reincarnation.wav" );

			// Screen Fade

			Create_ScreenFade( id, (1<<10), (1<<10), FADE_OUT, 0, 255, 0, 96 );
		}
	}

	else
	{
		client_print( id, print_center, "%L", id, "ITEM_ANKH_NOITEMS" );
	}


	g_PlayerBackpack[id] = {0,0,0,0,0,0,0,0};

	return PLUGIN_HANDLED;
}


// Boots of Speed

public Float:ITEM_BOOTS_get_speed( id ) {

	new Float:fBootSpeed = ITEM_BOOTS_VALUE;

	if ( ITEM_BOOTS_VALUE > SPEED_KNIFE || ( g_PlayerInfo[id][CURRENT_RACE] == RACE_NIGHTELF && ITEM_BOOTS_VALUE > BLESSING_MAXSPEED ) )
	{
		if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_UNDEAD && WAR3_skill_enabled( id, RACE_UNDEAD, SKILL_RACIAL ) )
		{
			new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

			fBootSpeed = UD_S_UNHOLY_get_speed( iLevel );

			// Make sure we're at least boot speed

			if ( fBootSpeed < ITEM_BOOTS_VALUE )
				fBootSpeed = ITEM_BOOTS_VALUE * ( fBootSpeed / SPEED_KNIFE );

			// increase by same percentage as boots are to knife speed

			else
			{
				fBootSpeed *= ( ITEM_BOOTS_VALUE / SPEED_KNIFE );
			}

			// Final checks

			if ( fBootSpeed > UD_fUnholyAura_speed[1] )
				fBootSpeed = UD_fUnholyAura_speed[1];

			else if ( fBootSpeed < ITEM_BOOTS_VALUE )
				fBootSpeed = ITEM_BOOTS_VALUE;
		}

		else if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC && g_PlayerInfo[id][CURRENT_SKILL1] && WAR3_skill_enabled( id, RACE_ORC, SKILL_1 ) )
		{
			new weapon, iClip, iAmmo;
			weapon = get_user_weapon( id, iClip, iAmmo );

			if ( weapon == CSW_KNIFE || weapon == CSW_C4 || cs_get_weapon_type_( weapon ) == CS_WEAPON_TYPE_GRENADE )
			{
				fBootSpeed = OR_S_BERSERK_get_speed( id );

				// Make sure we're at least boot speed

				if ( fBootSpeed < ITEM_BOOTS_VALUE )
					fBootSpeed = ITEM_BOOTS_VALUE * ( fBootSpeed / SPEED_KNIFE );

				// increase by same percentage as boots are to knife speed

				else
				{
					fBootSpeed *= ( ITEM_BOOTS_VALUE / SPEED_KNIFE );
				}

				// Final checks

				if ( fBootSpeed > OR_fBerserk_speed[2] )
					fBootSpeed = OR_fBerserk_speed[2];

				if ( fBootSpeed < ITEM_BOOTS_VALUE )
					fBootSpeed = ITEM_BOOTS_VALUE;

				client_print( id, print_chat, "DEBUG: Speed: %0.3f", fBootSpeed );
			}
		}

		else if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_NIGHTELF && g_PlayerInfo[id][CURRENT_SKILL2] && WAR3_skill_enabled( id, RACE_NIGHTELF, SKILL_RACIAL ) )
		{
			fBootSpeed = ITEM_BOOTS_VALUE * ( ITEM_BOOTS_VALUE / BLESSING_MAXSPEED );
		}
	}

	return ( fBootSpeed );
}


public ITEM_BOOTS_set_speed( id ) {

	new Float:fSpeed = ITEM_BOOTS_get_speed( id );
	set_user_maxspeed( id, fSpeed );

	return PLUGIN_HANDLED;
}


// Claws of Attack +6

public ITEM_CLAWS_damage( attacker, victim, weapon, headshot ) {

	// Add to bonus damage array

	WAR3_damage( attacker, victim, weapon, ITEM_CLAWS_VALUE, headshot, DAMAGE_CHECKARMOR );

	if ( is_user_alive( victim ) )
	{
		// Screen Fade

		new iFadeAlpha = ITEM_CLAWS_VALUE * 3;
		Create_ScreenFade( victim, (1<<10), (1<<10), FADE_OUT, 255, 0, 0, iFadeAlpha );
	}

	return PLUGIN_HANDLED;
}


// Cloak of Invisibility

public ITEM_CLOAK_set( id ) {

	g_bPlayerInvis[id] = true;

	new Float:fInvis = ( 1.0 - ITEM_CLOAK_VALUE ) * 255.0;
	new iInvis = floatround( fInvis );

	set_user_rendering( id, kRenderFxNone, 0, 0, 0, kRenderTransTexture, iInvis );

	SHARED_INVIS_icon( id, ICON_SHOW );

	return PLUGIN_HANDLED;
}


// Amulet of Shielding

public ITEM_AMULET_block( target, caster ) {

	// Message

	new szMessage[128];

	formatex( szMessage, 127, "%L", target, "AMULET_BLOCK_TARGET" );
	WAR3_status_text( target, szMessage, 3.0 );

	formatex( szMessage, 127, "%L", caster, "AMULET_BLOCK_CASTER" );
	WAR3_status_text( caster, szMessage, 3.0 );

	// Screen Fade

	if ( !g_bPlayerSleeping[target] )
	{
		Create_ScreenFade( target, (1<<10), (1<<10), FADE_OUT, ITEM_AMULET_RGB[GLOW_R], ITEM_AMULET_RGB[GLOW_G], ITEM_AMULET_RGB[GLOW_B], 160 );
	}

	// Glow

	SHARED_GLOW_set( target, 1.0, ITEM_AMULET_RGB, 48 );

	// Sparks

	for ( new i = 0; i < 4; i++ )
	{
		new Origin[3];
		get_user_origin( target, Origin );

		Origin[0] += random_num( -20, 20 );
		Origin[1] += random_num( -20, 20 );
		Origin[2] += random_num( -20, 20 );

		Create_TE_SPARKS( SHOWTO_ALL_BROADCAST, Origin );
	}

	// Remove charge, destroy if 0 left

	g_iAmuletCharges[target]--;

	if ( g_iAmuletCharges[target] == 0 )
	{
		g_PlayerInfo[target][CURRENT_ITEM] = ITEM_NONE;

		WAR3_hud_item( target, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );
		client_print( target, print_chat, "%s %L", WAR3X_PREFIX, target, "ITEM_AMULET_DESTROYED" );
	}

	return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. - //
