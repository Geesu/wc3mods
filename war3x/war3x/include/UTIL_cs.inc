
/* - AMX Mod Include -------------------------------------------- *

 *  UTIL_cs
 *  (c) Copyright 2003-2004, Created by Ryan.
 *
 *  A collection of useful functions that can be used in all cs
 *  script files. These functions are 'stock', so they will not
 *  be compiled unless used.
 *
 *
 *  Revision v1.0:
 *  --------------
 *      Initial Release:
 *
 *      * commands here...
 *
 *  Enjoy!

 * -------------------------------------------------------------- */


#if defined _util_cs_inc
    #endinput
#endif
#define _util_cs_inc


/* -------------------------------------------------------------- */


// CS Damage Types (icons)

#define CS_DMG_POISON              (1<<17)     // Damage types
#define CS_DMG_COLD                (1<<14)
#define CS_DMG_DROWN               (1<<20)
#define CS_DMG_BURN                 (1<<3)
#define CS_DMG_NERVE                (1<<4)
#define CS_DMG_RAD                 (1<<16)
#define CS_DMG_SHOCK               (1<<18)

// CS Weapon Types

#define CS_WEAPON_TYPE_C4              (1<<0)
#define CS_WEAPON_TYPE_GRENADE         (1<<1)
#define CS_WEAPON_TYPE_PISTOL          (1<<2)
#define CS_WEAPON_TYPE_SHOTGUN         (1<<3)
#define CS_WEAPON_TYPE_SMG             (1<<4)
#define CS_WEAPON_TYPE_RIFLE           (1<<5)
#define CS_WEAPON_TYPE_MG              (1<<6)
#define CS_WEAPON_TYPE_SNIPER          (1<<7)
#define CS_WEAPON_TYPE_AUTOSNIPER      (1<<8)
#define CS_WEAPON_TYPE_KNIFE           (1<<9)

// CS Weapon Groups

#define CS_WEAPON_GROUP_OTHER          (1<<0)  // Other (C4, etc)
#define CS_WEAPON_GROUP_PRIMARY        (1<<1)  // Primary weapons
#define CS_WEAPON_GROUP_SECONDARY      (1<<2)  // Sidearms
#define CS_WEAPON_GROUP_KNIFE          (1<<3)  // Knife
#define CS_WEAPON_GROUP_GRENADE        (1<<4)  // Grenades

// CS AmmoX Types

#define CS_AMMOX_338MAGNUM               1     // AWP
#define CS_AMMOX_762NATO                 2     // AK47, SCOUT, G3SG1
#define CS_AMMOX_556NATOBOX              3     // MG
#define CS_AMMOX_556NATO                 4     // SG552, GALI, FAMAS, M4A1, AUG, SG550
#define CS_AMMOX_BUCKSHOT                5     // M3, XM1014
#define CS_AMMOX_45ACP                   6     // MAC10, UMP45, USP
#define CS_AMMOX_57MM                    7     // P90, FIVESEVEN
#define CS_AMMOX_50AE                    8     // DEAGLE
#define CS_AMMOX_357SIG                  9     // P228
#define CS_AMMOX_9MM                    10     // MP5NAVY, TMP, GLOCK18, ELITE
#define CS_AMMOX_FLASHBANG              11     // Flashbang
#define CS_AMMOX_HEGRENADE              12     // HE Grenade
#define CS_AMMOX_SMOKEGRENADE           13     // Smoke Grenade

#define CS_HEADSHOT_NO                   0
#define CS_HEADSHOT_YES                  1

#define CS_TEAM_SPECTATOR                0
#define CS_TEAM_TERRORIST                1
#define CS_TEAM_CT                       2

#define CS_TEAMID_TEAMMATE               1
#define CS_TEAMID_ENEMY                  2
#define CS_TEAMID_NEUTRAL                3

#define CS_MP_PLAYERID_SHOWALL           0
#define CS_MP_PLAYERID_SHOWTEAM          1
#define CS_MP_PLAYERID_SHOWNONE          2

#define CS_MAX_MONEY                   16000

stock CS_TEAM_NAME[3][] =
{
    "SPECTATOR",
    "TERRORIST",
    "CT"
};

stock CS_WEAPON_NAME[31][] =
{
    "",
    "weapon_p228",                  // CSW_P228
    "",
    "weapon_scout",                 // CSW_SCOUT
    "weapon_hegrenade",             // CSW_HEGRENADE
    "weapon_xm1014",                // CSW_XM1014
    "weapon_c4",                    // CSW_C4
    "weapon_mac10",                 // CSW_MAC10
    "weapon_aug",                   // CSW_AUG
    "weapon_smokegrenade",          // CSW_SMOKEGRENADE
    "weapon_elite",                 // CSW_ELITE
    "weapon_fiveseven",             // CSW_FIVESEVEN
    "weapon_ump45",                 // CSW_UMP45
    "weapon_sg550",                 // CSW_SG550
    "weapon_galil",                 // CSW_GALI
    "weapon_famas",                 // CSW_FAMAS
    "weapon_usp",                   // CSW_USP
    "weapon_glock18",               // CSW_GLOCK18
    "weapon_awp",                   // CSW_AWP
    "weapon_mp5navy",               // CSW_MP5NAVY
    "weapon_m249",                  // CSW_M249
    "weapon_m3",                    // CSW_M3
    "weapon_m4a1",                  // CSW_M4A1
    "weapon_tmp",                   // CSW_TMP
    "weapon_g3sg1",                 // CSW_G3SG1
    "weapon_flashbang",             // CSW_FLASHBANG
    "weapon_deagle",                // CSW_DEAGLE
    "weapon_sg552",                 // CSW_SG552
    "weapon_ak47",                  // CSW_AK47
    "weapon_knife",                 // CSW_KNIFE
    "weapon_p90"                    // CSW_P90

};

stock CS_AMMO_NAME[31][] =
{
    "",
    "ammo_357sig",                  // CSW_P228
    "",
    "ammo_762nato",                 // CSW_SCOUT
    "",                             // CSW_HEGRENADE
    "ammo_buckshot",                // CSW_XM1014
    "",                             // CSW_C4
    "ammo_45acp",                   // CSW_MAC10
    "ammo_556nato",                 // CSW_AUG
    "",                             // CSW_SMOKEGRENADE
    "ammo_9mm",                     // CSW_ELITE
    "ammo_57mm",                    // CSW_FIVESEVEN
    "ammo_45acp",                   // CSW_UMP45
    "ammo_556nato",                 // CSW_SG550
    "ammo_556nato",                 // CSW_GALI
    "ammo_556nato",                 // CSW_FAMAS
    "ammo_45acp",                   // CSW_USP
    "ammo_9mm",                     // CSW_GLOCK18
    "ammo_338magnum",               // CSW_AWP
    "ammo_9mm",                     // CSW_MP5NAVY
    "ammo_556natobox",              // CSW_M249
    "ammo_buckshot",                // CSW_M3
    "ammo_556nato",                 // CSW_M4A1
    "ammo_9mm",                     // CSW_TMP
    "ammo_762nato",                 // CSW_G3SG1
    "",                             // CSW_FLASHBANG
    "ammo_50ae",                    // CSW_DEAGLE
    "ammo_556nato",                 // CSW_SG552
    "ammo_762nato",                 // CSW_AK47
    "",                             // CSW_KNIFE
    "ammo_57mm"                     // CSW_P90
};

// Model locations. Use with CS_MODEL_NAME.

stock CS_P_MODEL[] = "models/p_%s";
stock CS_V_MODEL[] = "models/v_%s";
stock CS_W_MODEL[] = "models/w_%s";

stock CS_MODEL_NAME[31][] =
{
    "",
    "p228.mdl",                     // CSW_P228
    "",
    "scout.mdl",                    // CSW_SCOUT
    "hegrenade.mdl",                // CSW_HEGRENADE
    "xm1014.mdl",                   // CSW_XM1014
    "c4.mdl",                       // CSW_C4
    "mac10.mdl",                    // CSW_MAC10
    "aug.mdl",                      // CSW_AUG
    "smokegrenade.mdl",             // CSW_SMOKEGRENADE
    "elite.mdl",                    // CSW_ELITE
    "fiveseven.mdl",                // CSW_FIVESEVEN
    "ump45.mdl",                    // CSW_UMP45
    "sg550.mdl",                    // CSW_SG550
    "gali.mdl",                     // CSW_GALI
    "famas.mdl",                    // CSW_FAMAS
    "usp.mdl",                      // CSW_USP
    "glock18.mdl",                  // CSW_GLOCK18
    "awp.mdl",                      // CSW_AWP
    "mp5.mdl",                      // CSW_MP5NAVY
    "m249.mdl",                     // CSW_M249
    "m3.mdl",                       // CSW_M3
    "m4a1.mdl",                     // CSW_M4A1
    "tmp.mdl",                      // CSW_TMP
    "g3sg1.mdl",                    // CSW_G3SG1
    "flashbang.mdl",                // CSW_FLASHBANG
    "deagle.mdl",                   // CSW_DEAGLE
    "sg552.mdl",                    // CSW_SG552
    "ak47.mdl",                     // CSW_AK47
    "knife.mdl",                    // CSW_KNIFE
    "p90.mdl"                       // CSW_P90
};

stock Float:CS_SPEED_VIP = 227.0;

stock Float:CS_WEAPON_SPEED[31] =
{
    0.0,
    250.0,      // CSW_P228
    0.0,
    260.0,      // CSW_SCOUT
    250.0,      // CSW_HEGRENADE
    240.0,      // CSW_XM1014
    250.0,      // CSW_C4
    250.0,      // CSW_MAC10
    240.0,      // CSW_AUG
    250.0,      // CSW_SMOKEGRENADE
    250.0,      // CSW_ELITE
    250.0,      // CSW_FIVESEVEN
    250.0,      // CSW_UMP45
    210.0,      // CSW_SG550
    240.0,      // CSW_GALI
    240.0,      // CSW_FAMAS
    250.0,      // CSW_USP
    250.0,      // CSW_GLOCK18
    210.0,      // CSW_AWP
    250.0,      // CSW_MP5NAVY
    220.0,      // CSW_M249
    230.0,      // CSW_M3
    230.0,      // CSW_M4A1
    250.0,      // CSW_TMP
    210.0,      // CSW_G3SG1
    250.0,      // CSW_FLASHBANG
    250.0,      // CSW_DEAGLE
    235.0,      // CSW_SG552
    221.0,      // CSW_AK47
    250.0,      // CSW_KNIFE
    245.0       // CSW_P90
};

stock Float:CS_WEAPON_SPEED_ZOOM[31] =
{
    0.0,
    0.0,
    0.0,
    220.0,      // CSW_SCOUT
    0.0,
    0.0,
    0.0,
    0.0,
    240.0,      // CSW_AUG
    0.0,
    0.0,
    0.0,
    0.0,
    150.0,      // CSW_SG550
    0.0,
    0.0,
    0.0,
    0.0,
    150.0,      // CSW_AWP
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    150.0,      // CSW_G3SG1
    0.0,
    0.0,
    235.0,      // CSW_SG552
    0.0,
    0.0,
    0.0
};


/* -------------------------------------------------------------- */


// Forces a player to switch weapon to WeaponGroup.

public cs_switchweapon( id, WeaponGroup ) {
#if ADVANCED_DEBUG
	log_function("public cs_switchweapon( id, WeaponGroup ) {");
#endif

    new Weapons[32], iTotalWeapons = 0;
    get_user_weapons( id, Weapons, iTotalWeapons );

    for ( new i = 0; i < iTotalWeapons; i++ )
    {
        new Weapon = Weapons[i];

        if ( cs_get_weapon_group( Weapon ) == WeaponGroup )
            engclient_cmd( id, CS_WEAPON_NAME[Weapon] );
    }

    return PLUGIN_HANDLED;
}


// Returns weapon type of Weapon

public cs_get_weapon_type_( Weapon ) {
#if ADVANCED_DEBUG
	log_function("public cs_get_weapon_type_( Weapon ) {");
#endif

    switch ( Weapon )
    {
        case CSW_KNIFE:
            return CS_WEAPON_TYPE_KNIFE;

        case CSW_C4:
            return CS_WEAPON_TYPE_C4;

        case CSW_FLASHBANG, CSW_HEGRENADE, CSW_SMOKEGRENADE:
            return CS_WEAPON_TYPE_GRENADE;

        case CSW_USP, CSW_DEAGLE, CSW_GLOCK18, CSW_ELITE, CSW_P228, CSW_FIVESEVEN:
            return CS_WEAPON_TYPE_PISTOL;

        case CSW_XM1014, CSW_M3:
            return CS_WEAPON_TYPE_SHOTGUN;

        case CSW_MP5NAVY, CSW_UMP45, CSW_P90, CSW_TMP, CSW_MAC10:
            return CS_WEAPON_TYPE_SMG;

        case CSW_M4A1, CSW_AK47, CSW_AUG, CSW_SG552, CSW_GALI, CSW_FAMAS:
            return CS_WEAPON_TYPE_RIFLE;

        case CSW_M249:
            return CS_WEAPON_TYPE_MG;

        case CSW_AWP, CSW_SCOUT:
            return CS_WEAPON_TYPE_SNIPER;

        case CSW_G3SG1, CSW_SG550:
            return CS_WEAPON_TYPE_AUTOSNIPER;
    }

    return PLUGIN_HANDLED;
}


// Returns weapon group of Weapon

public cs_get_weapon_group( Weapon ) {
#if ADVANCED_DEBUG
	log_function("public cs_get_weapon_group( Weapon ) {");
#endif

    switch ( cs_get_weapon_type_( Weapon ) )
    {
        case CS_WEAPON_TYPE_SHOTGUN, CS_WEAPON_TYPE_SMG, CS_WEAPON_TYPE_RIFLE, CS_WEAPON_TYPE_MG, CS_WEAPON_TYPE_SNIPER, CS_WEAPON_TYPE_AUTOSNIPER:
            return CS_WEAPON_GROUP_PRIMARY;

        case CS_WEAPON_TYPE_PISTOL:
            return CS_WEAPON_GROUP_SECONDARY;

        case CS_WEAPON_TYPE_KNIFE:
            return CS_WEAPON_GROUP_KNIFE;

        case CS_WEAPON_TYPE_GRENADE:
            return CS_WEAPON_GROUP_GRENADE;

        default:
            return CS_WEAPON_GROUP_OTHER;
    }

    return PLUGIN_HANDLED;
}


public Create_Money( id, iAmmount, iFlash ) {
#if ADVANCED_DEBUG
	log_function("public Create_Money( id, iAmmount, iFlash ) {");
#endif

    new iMsgMoney = get_user_msgid( "Money" );

    if ( id > MAX_PLAYERS )         // Force Reliable
        message_begin( MSG_ALL, iMsgMoney );

    else
    {
        if ( is_user_bot( id ) )
            return PLUGIN_HANDLED;

        else
        {
            message_begin( MSG_ONE, iMsgMoney, {0,0,0}, id );
        }
    }

    write_long( iAmmount );         // new ammount
    write_byte( iFlash );           // flash difference
    message_end();

    return PLUGIN_HANDLED;
}


// Update HUD $ (linux fix)

public cs_update_money( id, iAmmount, iFlash ) {
#if ADVANCED_DEBUG
	log_function("public cs_update_money( id, iAmmount, iFlash ) {");
#endif

    if ( !is_user_connected( id ) )
        return PLUGIN_HANDLED;

    new iOldMoney = cs_get_user_money( id );
    new iNewMoney = cs_get_user_money( id ) + iAmmount;

    if ( iNewMoney > CS_MAX_MONEY )
        iNewMoney = CS_MAX_MONEY;

    if ( iNewMoney < 0 )
        iNewMoney = 0;


    cs_set_user_money( id, iNewMoney, 0 );

    Create_Money( id, iOldMoney, 0 );
    Create_Money( id, iNewMoney, iFlash );

    return PLUGIN_HANDLED;
}


// Hide status text

public cs_status_hide( id ) {
#if ADVANCED_DEBUG
	log_function("public cs_status_hide( id ) {");
#endif

    Create_StatusText( id, 0, "0" );

    return PLUGIN_HANDLED;
}

// Display current target ( center )

public cs_status_target( id ) {
#if ADVANCED_DEBUG
	log_function("public cs_status_target( id ) {");
#endif

    if ( g_bNewTargetType[id] )
    {
        g_bNewTargetType[id] = false;
        return PLUGIN_HANDLED;
    }

    Create_StatusText( id, 0, g_TargetFormat[id] );

    return PLUGIN_HANDLED;
}

// Find grenade

public cs_find_grenade( id, szGrenade[] ) {
#if ADVANCED_DEBUG
	log_function("public cs_find_grenade( id, szGrenade[] ) {");
#endif

    new eIndex = find_ent_by_class( -1, "grenade" );

    while ( eIndex > 0 )
    {
        if ( entity_get_edict( eIndex, EV_ENT_owner ) == id )
        {
            new szModel[64];
            entity_get_string( eIndex, EV_SZ_model, szModel, 63 );

            if ( equali( szGrenade, szModel ) )
                return ( eIndex );
        }

        eIndex = find_ent_by_class( eIndex, "grenade" );
    }

    return ( 0 );
}