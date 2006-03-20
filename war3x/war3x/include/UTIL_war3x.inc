
/* - AMX Mod Include -------------------------------------------- *

 *  UTIL_war3x
 *  (c) Copyright 2003-2004, Created by Ryan.
 *
 *  A collection of useful functions that are used in the war3x
 *  plugin source.
 *
 *
 *  Enjoy!

 * -------------------------------------------------------------- */


#define LEVEL_ALL   -1          // Skills help file creation level


/* -------------------------------------------------------------- */


// Weapon XP Multiplier

public WAR3_set_xp() {
#if ADVANCED_DEBUG
	log_function("public WAR3_set_xp() {");
#endif

    g_fWeaponXp[CSW_WORLDSPAWN]      = 0.0;

    g_fWeaponXp[CSW_USP]             = 1.25;
    g_fWeaponXp[CSW_DEAGLE]          = 1.25;
    g_fWeaponXp[CSW_GLOCK18]         = 1.25;
    g_fWeaponXp[CSW_ELITE]           = 1.75;
    g_fWeaponXp[CSW_P228]            = 1.5;
    g_fWeaponXp[CSW_FIVESEVEN]       = 1.75;

    g_fWeaponXp[CSW_XM1014]          = 1.25;
    g_fWeaponXp[CSW_M3]              = 1.5;

    g_fWeaponXp[CSW_MP5NAVY]         = 1.0;
    g_fWeaponXp[CSW_UMP45]           = 1.25;
    g_fWeaponXp[CSW_P90]             = 1.25;
    g_fWeaponXp[CSW_TMP]             = 1.5;
    g_fWeaponXp[CSW_MAC10]           = 1.5;

    g_fWeaponXp[CSW_AWP]             = 1.0;
    g_fWeaponXp[CSW_M4A1]            = 1.0;
    g_fWeaponXp[CSW_AK47]            = 1.0;
    g_fWeaponXp[CSW_AUG]             = 1.0;
    g_fWeaponXp[CSW_SG552]           = 1.0;
    g_fWeaponXp[CSW_GALI]            = 1.0;
    g_fWeaponXp[CSW_FAMAS]           = 1.0;
    g_fWeaponXp[CSW_G3SG1]           = 1.25;
    g_fWeaponXp[CSW_SG550]           = 1.25;
    g_fWeaponXp[CSW_M249]            = 1.25;
    g_fWeaponXp[CSW_SCOUT]           = 1.25;

    g_fWeaponXp[CSW_HEGRENADE]       = 2.0;
    g_fWeaponXp[CSW_KNIFE]           = 2.0;

    g_fWeaponXp[CSW_C4]              = 1.0;
    g_fWeaponXp[CSW_SMOKEGRENADE]    = 1.0;
    g_fWeaponXp[CSW_FLASHBANG]       = 1.0;

    g_fWeaponXp[CSW_SHADOWSTRIKE]    = 1.0;      // Expansion weapons
    g_fWeaponXp[CSW_PULVERIZE]       = 1.0;
    g_fWeaponXp[CSW_IMPALE]          = 1.0;
    g_fWeaponXp[CSW_HOLYLIGHT]       = 1.0;
    g_fWeaponXp[CSW_LIGHTNING]       = 1.0;
    g_fWeaponXp[CSW_ROOT]            = 1.0;
    g_fWeaponXp[CSW_FLAMESTRIKE]     = 1.0;
    g_fWeaponXp[CSW_DEATHCOIL]       = 1.0;
    g_fWeaponXp[CSW_WINDWALK]        = 1.0;
    g_fWeaponXp[CSW_FROSTNOVA]       = 1.0;

    return PLUGIN_HANDLED;
}


public WAR3_get_damage( weaponId, iBonusDamage )
{
#if ADVANCED_DEBUG
	log_function("public WAR3_get_damage( weaponId, iBonusDamage )");
#endif
    new Float:fBonusDamage = float( iBonusDamage );

    switch ( cs_get_weapon_type_( weaponId ) )
    {
        case CS_WEAPON_TYPE_KNIFE:         return floatround( fBonusDamage * SKILLDAMAGE_KNIFE );
        case CS_WEAPON_TYPE_PISTOL:        return floatround( fBonusDamage * SKILLDAMAGE_PISTOL );
        case CS_WEAPON_TYPE_GRENADE:       return floatround( fBonusDamage * SKILLDAMAGE_GRENADE );
        case CS_WEAPON_TYPE_SHOTGUN:       return floatround( fBonusDamage * SKILLDAMAGE_SHOTGUN );
        case CS_WEAPON_TYPE_SMG:           return floatround( fBonusDamage * SKILLDAMAGE_SMG );
        case CS_WEAPON_TYPE_RIFLE:         return floatround( fBonusDamage * SKILLDAMAGE_RIFLE );
        case CS_WEAPON_TYPE_SNIPER:        return floatround( fBonusDamage * SKILLDAMAGE_SNIPER );
        case CS_WEAPON_TYPE_AUTOSNIPER:    return floatround( fBonusDamage * SKILLDAMAGE_SNIPER );
        case CS_WEAPON_TYPE_MG:            return floatround( fBonusDamage * SKILLDAMAGE_MG );
    }

    return PLUGIN_CONTINUE;
}


// Get race skill/ultimate string

public WAR3_race_info( id, iRaceId, iSkillNum, szData[64] ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_race_info( id, iRaceId, iSkillNum, szData[64] ) {");
#endif

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new iUltimateNum = g_PlayerInfo[id][CURRENT_ULTIMATE];

    // Create 'Class' ID Num

    if ( iSkillNum == RACEINFO_CLASSNAME )
    {
        if ( iUltimateNum )
            iUltimateNum += 2;

        else
        {
            for ( new i = 2; i >= 0; i-- )
            {
                if ( iLevel >= GETCLASSCHECK[i] && !iUltimateNum )
                    iUltimateNum = i;
            }
        }
    }

    // Pass Data Back to Function Call

    switch ( iRaceId + 1 )
    {
        case RACE_UNDEAD:
        {
            switch ( iSkillNum )
            {
                case RACEINFO_CLASSNAME:    copy( szData, 63, RACE1CLASS[iUltimateNum] );
                default:                    copy( szData, 63, RACE1SKILL[iSkillNum] );
            }
        }

        case RACE_HUMAN:
        {
            switch ( iSkillNum )
            {
                case RACEINFO_CLASSNAME:    copy( szData, 63, RACE2CLASS[iUltimateNum] );
                default:                    copy( szData, 63, RACE2SKILL[iSkillNum] );
            }
        }

        case RACE_ORC:
        {
            switch ( iSkillNum )
            {
                case RACEINFO_CLASSNAME:    copy( szData, 63, RACE3CLASS[iUltimateNum] );
                default:                    copy( szData, 63, RACE3SKILL[iSkillNum] );
            }
        }

        case RACE_NIGHTELF:
        {
            switch ( iSkillNum )
            {
                case RACEINFO_CLASSNAME:    copy( szData, 63, RACE4CLASS[iUltimateNum] );
                default:                    copy( szData, 63, RACE4SKILL[iSkillNum] );
            }
        }
    }

    return PLUGIN_HANDLED;
}


// Warcraft 3: Expansion Precaching

public WAR3_precache_sound( szWar3Sound[], szDefaultSound[] ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_precache_sound( szWar3Sound[], szDefaultSound[] ) {");
#endif

    new szWar3SoundPath[64];
    format( szWar3SoundPath, 63, "sound/%s", szWar3Sound );

    if ( file_exists( szWar3SoundPath ) )
        precache_sound( szWar3Sound );

    else if ( szDefaultSound[0] )
    {
        precache_sound( szDefaultSound );
    }

    return PLUGIN_HANDLED;
}

public WAR3_precache_model( szWar3Model[], szDefaultModel[] ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_precache_model( szWar3Model[], szDefaultModel[] ) {");
#endif

    if ( file_exists( szWar3Model ) )
        precache_model( szWar3Model );

    else if ( szDefaultModel[0] )
    {
        precache_model( szDefaultModel );
    }

    return PLUGIN_HANDLED;
}

public WAR3_precache() {
#if ADVANCED_DEBUG
	log_function("public WAR3_precache() {");
#endif

    // Race Icons

    #if PRECACHE_RACEICONS

        if ( file_exists( "sprites/warcraft3/icon_ud.spr" ) && file_exists( "sprites/warcraft3/icon_human.spr" ) && file_exists( "sprites/warcraft3/icon_orc.spr" ) && file_exists( "sprites/warcraft3/icon_ne.spr" ) )
        {
            ICON_UNDEAD     = precache_model( "sprites/warcraft3/icon_ud.spr" );
            ICON_HUMAN      = precache_model( "sprites/warcraft3/icon_human.spr" );
            ICON_ORC        = precache_model( "sprites/warcraft3/icon_orc.spr" );
            ICON_NIGHTELF   = precache_model( "sprites/warcraft3/icon_ne.spr" );
        }

    #endif

    // Blinking Icons

    #if PRECACHE_BLINKICONS

        if ( file_exists( "sprites/warcraft3/saveme.spr" ) && file_exists( "sprites/warcraft3/dispellme.spr" ) )
        {
            ICON_SAVEME     = precache_model( "sprites/warcraft3/saveme.spr" );
            ICON_DISPELLME  = precache_model( "sprites/warcraft3/dispellme.spr" );
        }

    #endif

    // Sounds

    #if PRECACHE_WAR3SOUNDS

        // Skills / Ultimates

        copy( SOUND_IMPALE, 63,           "warcraft3/ImpaleHit.wav"                     );  // Impale
        copy( SOUND_DEATHCOIL, 63,        "warcraft3/DeathCoilSpecialArt1.wav"          );  // Death Coil
        copy( SOUND_SLEEP, 63,            "warcraft3/SleepBirth1.wav"                   );  // Sleep
        copy( SOUND_HOLYLIGHT, 63,        "warcraft3/HolyBolt.wav"                      );  // Holy Light
        copy( SOUND_FLAMESTRIKE, 63,      "warcraft3/FlameStrikeTargetWaveNonLoop1.wav" );  // Flame Strike
        copy( SOUND_FLAMESTRIKE_BURN, 63, "ambience/flameburst1.wav"                    );  // Flame Strike ( burned )
        copy( SOUND_AVATAR, 63,           "warcraft3/Avatar.wav"                        );  // Avatar
        copy( SOUND_PULVERIZE, 63,        "warcraft3/Shockwave.wav"                     );  // Pulverize
        copy( SOUND_HEALINGWAVE, 63,      "warcraft3/HealingWave.wav"                   );  // Healing Wave
        copy( SOUND_CHAINLIGHTNING, 63,   "warcraft3/LightningBolt.wav"                 );  // Chain Lightning
        copy( SOUND_WINDWALK, 63,         "warcraft3/WindWalk.wav"                      );  // Wind Walk
        copy( SOUND_REJUVENATION, 63,     "warcraft3/RejuvenationTarget1.wav"           );  // Rejuvenation
        copy( SOUND_ROOTS, 63,            "warcraft3/EntanglingRootsTarget1.wav"        );  // Entangling Roots
        copy( SOUND_SHADOWSTRIKE, 63,     "warcraft3/ShadowStrikeBirth1.wav"            );  // Shadow Strike

        // Miscellaneous

        copy( SOUND_LEVELUP, 63,          "warcraft3/Levelupcaster.wav"                 );  // Level up
        copy( SOUND_PICKUPITEM, 63,       "warcraft3/PickUpItem.wav"                    );  // Buy / Pick up item
        copy( SOUND_TOME, 63,             "warcraft3/Tomes.wav"                         );  // Tome of Experience
        copy( SOUND_ULTIMATESCAN, 63,     "turret/tu_ping.wav"                          );  // Ultimate Beep
        copy( SOUND_ULTIMATEREADY, 63,    "warcraft3/ResurrectTarget.wav"               );  // Ultimate Beep

    #else

        // Skills / Ultimates

        copy( SOUND_IMPALE, 63,           "doors/aliendoor3.wav"       );   // Impale
        copy( SOUND_DEATHCOIL, 63,        "doors/aliendoor1.wav"       );   // Death Coil
        copy( SOUND_SLEEP, 63,            "x/x_laugh1.wav"             );   // Sleep
        copy( SOUND_HOLYLIGHT, 63,        "headcrab/hc_pain1.wav"      );   // Holy Light
        copy( SOUND_FLAMESTRIKE, 63,      "garg/gar_flameoff1.wav"     );   // Flame Strike
        copy( SOUND_FLAMESTRIKE_BURN, 63, "ambience/flameburst1.wav"   );   // Flame Strike ( burned )
        copy( SOUND_AVATAR, 63,           "garg/gar_alert2.wav"        );   // Avatar
        copy( SOUND_PULVERIZE, 63,        "debris/bustconcrete2.wav"   );   // Pulverize
        copy( SOUND_HEALINGWAVE, 63,      "boid/boid_idle1.wav"        );   // Healing Wave
        copy( SOUND_CHAINLIGHTNING, 63,   "weapons/gauss2.wav"         );   // Chain Lightning
        copy( SOUND_WINDWALK, 63,         "houndeye/he_attack1.wav"    );   // Wind Walk
        copy( SOUND_REJUVENATION, 63,     "bullchicken/bc_acid1.wav"   );   // Rejuvenation
        copy( SOUND_ROOTS, 63,            "weapons/electro5.wav"       );   // Entangling Roots
        copy( SOUND_SHADOWSTRIKE, 63,     "controller/con_attack3.wav" );   // Shadow Strike

        // Miscellaneous

        copy( SOUND_LEVELUP, 63,          "plats/elevbell1.wav"        );   // Level up
        copy( SOUND_PICKUPITEM, 63,       "items/ammopickup1.wav"      );   // Buy / Pick up item
        copy( SOUND_TOME, 63,             "items/suitchargeok1.wav"    );   // Tome of Experience
        copy( SOUND_ULTIMATESCAN, 63,     "turret/tu_ping.wav"         );   // Ultimate Beep
        copy( SOUND_ULTIMATEREADY, 63,    "buttons/bell1.wav"          );   // Ultimate Beep

    #endif

    // Skills / Ultimates

    precache_sound( SOUND_IMPALE );
    precache_sound( SOUND_DEATHCOIL );
    precache_sound( SOUND_SLEEP );
    precache_sound( SOUND_HOLYLIGHT );
    precache_sound( SOUND_FLAMESTRIKE );
    precache_sound( SOUND_FLAMESTRIKE_BURN );
    precache_sound( SOUND_AVATAR );
    precache_sound( SOUND_PULVERIZE );
    precache_sound( SOUND_HEALINGWAVE );
    precache_sound( SOUND_CHAINLIGHTNING );
    precache_sound( SOUND_WINDWALK );
    precache_sound( SOUND_REJUVENATION );
    precache_sound( SOUND_ROOTS );
    precache_sound( SOUND_SHADOWSTRIKE );

    // Miscellaneous

    precache_sound( SOUND_LEVELUP );
    precache_sound( SOUND_PICKUPITEM );
    precache_sound( SOUND_TOME );
    precache_sound( SOUND_ULTIMATESCAN );
    precache_sound( SOUND_ULTIMATEREADY );


    // Bonus Sounds

    #if PRECACHE_WAR3BONUSSOUNDS

        precache_sound( "warcraft3/bonus/BarkSkinTarget1.wav"         );
        precache_sound( "warcraft3/bonus/BerzerkerCaster.wav"         );
        precache_sound( "warcraft3/bonus/DeathCoilMissileLaunch1.wav" );
        precache_sound( "warcraft3/bonus/EntanglingRootsDecay1.wav"   );
        precache_sound( "warcraft3/bonus/Error.wav"                   );
        precache_sound( "warcraft3/bonus/FrostArmorTarget1.wav"       );
        precache_sound( "warcraft3/bonus/InnerFireBirth.wav"          );
        precache_sound( "warcraft3/bonus/Reincarnation.wav"           );
        precache_sound( "warcraft3/bonus/ShadowMeld1.wav"             );

    #endif


    // Models / Sprites

    SPR_BEAMFOLLOW      = precache_model( "sprites/smoke.spr" );
    SPR_SMOOTHWAVE      = precache_model( "sprites/laserbeam.spr" );
    SPR_SHOCKWAVE       = precache_model( "sprites/shockwave.spr" );
    SPR_BLOODDROP       = precache_model( "sprites/blood.spr" );
    SPR_BLOODSPRAY      = precache_model( "sprites/bloodspray.spr" );

    MDL_HORNET          = precache_model( "models/hornet.mdl" );

    SPR_CHAINLIGHTNING  = precache_model( "sprites/lgtning.spr" );

    SPR_SMOOTHBEAM      = precache_model( "sprites/plasma.spr" );

    SPR_DEATHCOIL       = precache_model( "sprites/flare6.spr" );

    SPR_ROOT            = precache_model( "sprites/zbeam4.spr" );

    SPR_FLAMEBURST      = precache_model( "sprites/explode1.spr" );

    // Flame Strike

    precache_model( "sprites/fire.spr" );

    // Sleep Z

    precache_model( "sprites/warcraft3/sleep.spr" );

    // Generic Heal Effect

    precache_model( "sprites/muz6.spr" );

    // Rejuvination

    precache_model( "sprites/muz7.spr" );
    precache_model( "sprites/blueflare2.spr" );

    // Impale

    precache_model( "models/tentacle2.mdl" );

    // Death Coil

    precache_model( "models/bskull_template1.mdl" );

    // Entangling Roots

    precache_model( "models/uplant1.mdl" );
    precache_model( "models/uplant2.mdl" );
    precache_model( "models/uplant3.mdl" );

    // Flame Strike

    precache_model( "sprites/cexplo.spr" );
    precache_model( "sprites/fexplo.spr" );

    // Item

    precache_model( "models/sphere.mdl" );


    // WAR3X Style Sheet (for motds)

    precache_generic( "war3x.css" );

    return PLUGIN_HANDLED;
}

// Status Text ( beneficial )

public WAR3_status_text( id, szMessage[], Float:fDuration ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_status_text( id, szMessage[], Float:fDuration ) {");
#endif

    // Print to status area

    set_hudmessage( 255, 208, 0, -1.0, 0.82, 0, 6.0, fDuration, 0.1, 0.5, 4 );
    show_hudmessage( id, szMessage );

    // Print important messages to console

    if ( fDuration >= 3.0 )
    {
        client_print( id, print_console, "%s%s", STATUS_PREFIX, szMessage );
    }

    return PLUGIN_HANDLED;
}


// Status Text ( harmful )

public WAR3_status_text2( id, szMessage[], Float:fDuration ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_status_text2( id, szMessage[], Float:fDuration ) {");
#endif

    // Print to status area

    set_hudmessage( 224, 180, 0, -1.0, 0.88, 0, 6.0, fDuration, 0.1, 0.5, 4 );
    show_hudmessage( id, szMessage );

    // Print important messages to console

    if ( fDuration >= 3.0 )
    {
        client_print( id, print_console, "%s%s", STATUS_PREFIX, szMessage );
    }

    return PLUGIN_HANDLED;
}


// Gets flag of an item

public WAR3_get_item_flag( iItemNum ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_get_item_flag( iItemNum ) {");
#endif

    new szFlag[2];
    get_letter( iItemNum , szFlag );

    return ( read_flags( szFlag ) );
}


// Gets flag of a skill

public WAR3_get_skill_flag( iRaceNum, iSkillNum ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_get_skill_flag( iRaceNum, iSkillNum ) {");
#endif

    new iLetterNum = ( iRaceNum - 1 ) * TOTAL_SKILLS;
    iLetterNum += ( iSkillNum + 1 );

    new szFlag[2];
    get_letter( iLetterNum, szFlag );

    return ( read_flags( szFlag ) );
}


// Gets flag of an ultimate

public WAR3_get_ult_flag( iRaceNum, iUltimateNum ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_get_ult_flag( iRaceNum, iUltimateNum ) {");
#endif

    new iLetterNum = ( iRaceNum - 1 ) * TOTAL_ULTIMATES;
    iLetterNum += iUltimateNum;

    new szFlag[2];
    get_letter( iLetterNum, szFlag );

    return ( read_flags( szFlag ) );
}


// Gets flag of an ultimate

public WAR3_get_race_flag( iRaceNum ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_get_race_flag( iRaceNum ) {");
#endif

    new szFlag[2];
    get_letter( iRaceNum + 1, szFlag );

    return ( read_flags( szFlag ) );
}


// Restriction check(s) **INCOMPLETE**

public WAR3_restrict_check( id ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_restrict_check( id ) {");
#endif

    // Check current item

    if ( get_cvar_bitsum( "war3x_restrict_items" ) & WAR3_get_item_flag( g_PlayerInfo[id][CURRENT_ITEM] ) )
    {
        // Give back full cash ammount

        // Inform Player
    }

    // Check current race

    if ( get_cvar_bitsum( "war3x_restrict_races" ) & WAR3_get_race_flag( g_PlayerInfo[id][CURRENT_RACE] - 1 ) )
    {
        // Reset player race

        // Inform Player
    }

    /*
        // check current skill(s). if restricted, inform player. (set bool)

        if ( read_flags( g_RestrictedSkills ) & WAR3_get_skill_flag( g_PlayerInfo[id][CURRENT_RACE] - 1 ) )

        // check current ultimate(s). if restricted, inform player. (set bool)

        if ( get_cvar_bitsum( "war3x_restrict_races" ) & WAR3_get_race_flag( g_PlayerInfo[id][CURRENT_RACE] - 1 ) )
    */

    return PLUGIN_HANDLED;
}


public WAR3_hud_xp( id, iEffect, Float:fFxTime, Float:fFadeIn, Float:fFadeOut, Float:fHoldTime ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_hud_xp( id, iEffect, Float:fFxTime, Float:fFadeIn, Float:fFadeOut, Float:fHoldTime ) {");
#endif

    if ( !is_user_alive( id ) || !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;

    new iCurrentXp = g_PlayerInfo[id][CURRENT_XP];

    new iLastlvlXp = WAR3_get_last_xp( iCurrentXp );
    new iNextlvlXp = WAR3_get_next_xp( iCurrentXp );

    new Float:fPercentage = ( float( iCurrentXp ) - float( iLastlvlXp ) ) / ( float( iNextlvlXp ) - float( iLastlvlXp ) ) * 100.0;

    new szMessage[64];
    format( szMessage, 63, "XP: %i/%i (%0.0f%s)", iCurrentXp, iNextlvlXp, fPercentage, "%%" );

    // Get Coordinates

    new iResNum = g_PlayerOptions[id][OPTION_RESOLUTION];
    new Float:fXpos = 13.0 / STEAM_RESOLUTION[iResNum][_X];
    new Float:fYpos = float( STEAM_RESOLUTION[iResNum][_Y] - 78 + 5 ) / float( STEAM_RESOLUTION[iResNum][_Y] );

    set_hudmessage( 160, 160, 160, fXpos, fYpos, iEffect, fFxTime, fHoldTime, fFadeIn, fFadeOut, HUDMESSAGE_CHAN_XP );
    show_hudmessage( id, szMessage );

    return PLUGIN_HANDLED;
}

public WAR3_hud_item( id, iEffect, Float:fFxTime, Float:fFadeIn, Float:fFadeOut, Float:fHoldTime ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_hud_item( id, iEffect, Float:fFxTime, Float:fFadeIn, Float:fFadeOut, Float:fHoldTime ) {");
#endif

    if ( !is_user_alive( id ) || !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;

    new iRaceNum = g_PlayerInfo[id][CURRENT_RACE] - 1;
    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    new szMessage[64];
    format( szMessage, 63, "%s Level %i   Item: %s", RACENAME_SHORT[iRaceNum], iLevel, ITEMNAME[g_PlayerInfo[id][CURRENT_ITEM]] );

    // Get Coordinates

    new iResNum = g_PlayerOptions[id][OPTION_RESOLUTION];
    new Float:fXpos = 13.0 / STEAM_RESOLUTION[iResNum][_X];
    new Float:fYpos = float( STEAM_RESOLUTION[iResNum][_Y] - 78 + 20 ) / float( STEAM_RESOLUTION[iResNum][_Y] );

    set_hudmessage( 224, 160, 0, fXpos, fYpos, iEffect, fFxTime, fHoldTime, fFadeIn, fFadeOut, HUDMESSAGE_CHAN_ITEM );
    show_hudmessage( id, szMessage );

    return PLUGIN_HANDLED;
}

public _WAR3_hud_item() {
#if ADVANCED_DEBUG
	log_function("public _WAR3_hud_item() {");
#endif

    new Players[32], iTotalPlayers;
    get_players( Players, iTotalPlayers, "a" );

    for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new id = Players[iPlayerNum];
        WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 1.0, 2.0, 3.0 );
    }

    set_task( HUDMESSAGE_FREQ_ITEM, "_WAR3_hud_item", TASK_SHOWITEM );

    return PLUGIN_HANDLED;
}


public WAR3_skill_enabled( id, iRaceNum, iSkillNum ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_skill_enabled( id, iRaceNum, iSkillNum ) {");
#endif

    if ( id == g_Vip || read_flags( g_RestrictedSkills ) & WAR3_get_skill_flag( iRaceNum, iSkillNum ) )
        return ( 0 );

    return ( 1 );
}

public WAR3_is_ultimate( wIndex ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_is_ultimate( wIndex ) {");
#endif

    if ( wIndex == CSW_DEATHCOIL || wIndex == CSW_IMPALE )
        return ( 1 );

    if ( wIndex == CSW_HOLYLIGHT || wIndex == CSW_FLAMESTRIKE )
        return ( 1 );

    if ( wIndex == CSW_LIGHTNING || wIndex == CSW_WINDWALK )
        return ( 1 );

    if ( wIndex == CSW_ROOT || wIndex == CSW_SHADOWSTRIKE )
        return ( 1 );

    return ( 0 );
}


public WAR3_skills_left( id ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_skills_left( id ) {");
#endif

    new iUsedPoints = 0;
    new iTotalPoints = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    if ( g_PlayerInfo[id][CURRENT_ULTIMATE] )
        iUsedPoints += 1;

    iUsedPoints += g_PlayerInfo[id][CURRENT_SKILL1] + g_PlayerInfo[id][CURRENT_SKILL2] + g_PlayerInfo[id][CURRENT_SKILL3];

    return ( iTotalPoints - iUsedPoints );
}


public WAR3_get_onoff( iStatus, szOnOff[8] ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_get_onoff( iStatus, szOnOff[8] ) {");
#endif

    if ( iStatus )
        copy( szOnOff, 7, LANGUAGE_COMMON_ON );

    else
    {
        copy( szOnOff, 7, LANGUAGE_COMMON_OFF );
    }

    return PLUGIN_HANDLED;
}


public WAR3_is_lowhealth( id ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_is_lowhealth( id ) {");
#endif

    new iMaxHealth = WAR3_get_maxhealth( id );
    new iHealth = get_user_health( id );

    if ( g_bEvadeNextShot[id] )
    {
        iHealth -= 1024;
        iMaxHealth -= 1024;
    }

    if ( float( iHealth ) / float( iMaxHealth ) <= HEAL_NOTIFYHEALTH )
        return ( 1 );

    return ( 0 );
}


public WAR3_is_dispellable( id ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_is_dispellable( id ) {");
#endif

    new bool:bDispellMe;

    if ( g_bPlayerSleeping[id] || g_PlayerOnFire[id] || g_PlayerSingeing[id] || g_PlayerStruck[id] || g_PlayerRooted[id] )
        bDispellMe = true;

    if ( bDispellMe )
        return ( 1 );

    return ( 0 );
}


// Checks that cIndex's ultimate can heal tIndex

public WAR3_can_heal( cIndex, tIndex ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_can_heal( cIndex, tIndex ) {");
#endif

    new cRaceId = g_PlayerInfo[cIndex][CURRENT_RACE] - 1;
    new tRaceId = g_PlayerInfo[tIndex][CURRENT_RACE] - 1;

    if ( get_user_team( cIndex ) != get_user_team( tIndex ) )
        return ( 0 );

    if ( read_flags( HEAL_TARGETS[cRaceId] ) & WAR3_get_race_flag( tRaceId ) )
        return ( 1 );

    return ( 0 );
}


// Get Level from current XP

public WAR3_get_level( iXp ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_get_level( iXp ) {");
#endif

    if ( iXp < 0 )
        return ( 0 );

    for ( new iLevel = 10; iLevel >= 0; iLevel-- )
    {
        if ( iXp >= g_iLevelXp[iLevel] )
             return iLevel;
    }

    return PLUGIN_HANDLED;
}


// Get XP needed for next Level

public WAR3_get_next_xp( iXp ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_get_next_xp( iXp ) {");
#endif

    for ( new iLevel = TOTAL_LEVELS; iLevel >= 0; iLevel-- )
    {
        if ( iXp >= g_iLevelXp[iLevel] )
        {
            if ( iLevel == TOTAL_LEVELS )
                return g_iLevelXp[iLevel];

            else
            {
                return g_iLevelXp[iLevel + 1];
            }
        }
    }

    return PLUGIN_HANDLED;
}


public WAR3_get_last_xp( iXp ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_get_last_xp( iXp ) {");
#endif

    for ( new iLevel = TOTAL_LEVELS; iLevel >= 0; iLevel-- )
    {
        if ( iXp >= g_iLevelXp[iLevel] )
        {
            if ( iLevel == TOTAL_LEVELS )
                return g_iLevelXp[iLevel - 1];

            else
            {
                return g_iLevelXp[iLevel];
            }
        }
    }

    return PLUGIN_HANDLED;
}


// Damage by Warcraft 3 Skill

public WAR3_damage( iAttackerId, iVictimId, iWeaponId, iDamage, iHeadshot, iCheckArmor ) {

    #if ADVANCED_DEBUG

    	log_function( "public WAR3_damage( iAttackerId, iVictimId, iWeaponId, iDamage, iHeadshot, iCheckArmor ) {" );

    #endif

    // Elune's Grace ( absorb bonus knife/ultimate damage )

    if ( g_PlayerInfo[iVictimId][CURRENT_RACE] == RACE_NIGHTELF && iDamage )
    {
        new iLevel = WAR3_get_level( g_PlayerInfo[iVictimId][CURRENT_XP] );

        // Set new damage amount

        if ( iWeaponId == CSW_KNIFE )
        {
            new Float:fAbsorb = float( iDamage ) * SElunes_Knife_Get( iLevel );
            iDamage -= floatround( fAbsorb );

            new iAbsorb = floatround( fAbsorb );
            SElunes_Absorb( iVictimId, iAbsorb );
        }

        else if ( WAR3_is_ultimate( iWeaponId ) )
        {
            new Float:fAbsorb = float( iDamage ) * SElunes_Magic_Get( iLevel );
            iDamage -= floatround( fAbsorb );

            new iAbsorb = floatround( fAbsorb );
            SElunes_Absorb( iVictimId, iAbsorb );
        }
    }

    // Notify damage to wind walker (if applicable)

    if ( iWeaponId == CSW_WINDWALK )
    {
        UWindwalk_Notify( iAttackerId, g_iWindwalkDamage[iAttackerId] + iDamage );
    }

    // Armor calculation

    if ( iCheckArmor && get_user_armor( iVictimId ) )
    {
        new iArmorDamage = iDamage;
        iDamage = WAR3_get_damage( iWeaponId, iDamage );

        new iNewArmor = get_user_armor( iVictimId ) - iArmorDamage;

        if ( iNewArmor < 0 )
        {
            iDamage += ( iNewArmor * -1 );
            iNewArmor = 0;
        }

        set_user_armor( iVictimId, iNewArmor );
    }

    new iNewHealth = get_user_health( iVictimId ) - iDamage;
    new iMinHealth = WAR3_get_minhealth( iVictimId );

    // Apply Damage

    if ( iNewHealth <= iMinHealth )
        WAR3_death( iAttackerId, iVictimId, iWeaponId, iHeadshot );

    else
    {
        set_user_health( iVictimId, iNewHealth );

        // Invisibility Cooldown

        Invis_Cooldown( iVictimId );
    }

    return PLUGIN_HANDLED;
}


// Death by Warcraft 3 Skill

public WAR3_death( killerId, victimId, weaponId, Headshot ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_death( killerId, victimId, weaponId, Headshot ) {");
#endif

    if ( !is_user_alive( victimId ) && !g_PlayerImpaled[victimId] )
        return PLUGIN_HANDLED;


    new szWeaponName[32];

    // Perform Victim Actions

    WAR3_death_victim( victimId );

    // Make sure death was not to DoT from disconnected killer

    if ( is_user_connected( killerId ) )
    {
        // Check if Teamkill

        if ( get_user_team( killerId ) == get_user_team( victimId ) && killerId != victimId )
        {
            XP_Kill_Teammate( killerId );

            cs_update_money( killerId, -3300, 1 );

            new iKillerFrags = get_user_frags( killerId ) - 1;
            new iKillerDeaths = get_user_deaths( killerId );

            set_user_frags( killerId, iKillerFrags );

            // Update frags ( realtime )

            Create_ScoreInfo( SHOWTO_ALL, killerId, iKillerFrags, iKillerDeaths, 0, get_user_team( killerId ) );
        }

        // Normal Kill

        else
        {
            XP_Kill( killerId, victimId, weaponId, Headshot );

            cs_update_money( killerId, 300, 1 );

            new iKillerFrags = get_user_frags( killerId ) + 1;
            new iKillerDeaths = get_user_deaths( killerId );

            set_user_frags( killerId, iKillerFrags );

            // Update frags ( realtime )

            Create_ScoreInfo( SHOWTO_ALL, killerId, iKillerFrags, iKillerDeaths, 0, get_user_team( killerId ) );
        }
    }

    // Create Death Message

    if ( is_user_alive( victimId ) )
    {
        new iVictimFrags = get_user_frags( victimId );
        new iVictimDeaths = get_user_deaths( victimId );

        // Kill Victim

        new gmsgDeathMsg = get_user_msgid( "DeathMsg" );

        set_msg_block( gmsgDeathMsg, BLOCK_SET );
        user_kill( victimId, 1 );
        set_msg_block( gmsgDeathMsg, BLOCK_NOT );

        // Update frags ( realtime )

        Create_ScoreInfo( SHOWTO_ALL, victimId, iVictimFrags, iVictimDeaths, 0, get_user_team( victimId ) );

        switch ( weaponId )
        {
            case CSW_KNIFE:         copy( szWeaponName, 31, "knife" );

            case CSW_USP:           copy( szWeaponName, 31, "usp" );
            case CSW_GLOCK18:       copy( szWeaponName, 31, "glock18" );
            case CSW_DEAGLE:        copy( szWeaponName, 31, "deagle" );
            case CSW_P228:          copy( szWeaponName, 31, "p228" );
            case CSW_ELITE:         copy( szWeaponName, 31, "elite" );
            case CSW_FIVESEVEN:     copy( szWeaponName, 31, "fiveseven" );

            case CSW_M3:            copy( szWeaponName, 31, "m3" );
            case CSW_XM1014:        copy( szWeaponName, 31, "xm1014" );

            case CSW_MP5NAVY:       copy( szWeaponName, 31, "mp5navy" );
            case CSW_TMP:           copy( szWeaponName, 31, "tmp" );
            case CSW_P90:           copy( szWeaponName, 31, "p90" );
            case CSW_MAC10:         copy( szWeaponName, 31, "mac10" );
            case CSW_UMP45:         copy( szWeaponName, 31, "ump45" );

            case CSW_AK47:          copy( szWeaponName, 31, "ak47" );
            case CSW_SG552:         copy( szWeaponName, 31, "sg552" );
            case CSW_M4A1:          copy( szWeaponName, 31, "m4a1" );
            case CSW_AUG:           copy( szWeaponName, 31, "aug" );
            case CSW_SCOUT:         copy( szWeaponName, 31, "scout" );
            case CSW_AWP:           copy( szWeaponName, 31, "awp" );
            case CSW_SG550:         copy( szWeaponName, 31, "sg550" );
            case CSW_G3SG1:         copy( szWeaponName, 31, "g3sg1" );

            case CSW_GALI:          copy( szWeaponName, 31, "galil" );
            case CSW_FAMAS:         copy( szWeaponName, 31, "famas" );

            case CSW_M249:          copy( szWeaponName, 31, "m249" );

            case CSW_HEGRENADE:     copy( szWeaponName, 31, "grenade" );

            case CSW_WORLDSPAWN:    copy( szWeaponName, 31, "worldspawn" );

            case CSW_IMPALE:        copy( szWeaponName, 31, "impale" );
            case CSW_DEATHCOIL:     copy( szWeaponName, 31, "death coil" );
            case CSW_HOLYLIGHT:     copy( szWeaponName, 31, "holy light" );
            case CSW_FLAMESTRIKE:   copy( szWeaponName, 31, "flame strike" );
            case CSW_PULVERIZE:     copy( szWeaponName, 31, "pulverize" );
            case CSW_LIGHTNING:     copy( szWeaponName, 31, "chain lightning" );
            case CSW_WINDWALK:      copy( szWeaponName, 31, "wind walk" );
            case CSW_ROOT:          copy( szWeaponName, 31, "entangling roots" );
            case CSW_SHADOWSTRIKE:  copy( szWeaponName, 31, "shadow strike" );
            case CSW_FROSTNOVA:     copy( szWeaponName, 31, "frost nova" );
        }

        Create_DeathMsg( SHOWTO_ALL, killerId, victimId, Headshot, szWeaponName );
    }

    // Log Death

    new szKillerName[32], szVictimName[32];
    new szKillerTeam[32], szVictimTeam[32];

    get_user_name( killerId, szKillerName, 31 );
    get_user_name( victimId, szVictimName, 31 );

    get_user_team( killerId, szKillerTeam, 31 );
    get_user_team( victimId, szVictimTeam, 31 );

    log_message( "^"%s<%d><%s><%s>^" killed ^"%s<%d><%s><%s>^" with ^"%s^"", szKillerName, get_user_userid( killerId ), g_SaveIds[killerId], szKillerTeam, szVictimName, get_user_userid( victimId ), g_SaveIds[victimId], szVictimTeam, szWeaponName );

    return PLUGIN_HANDLED;
}


public WAR3_is_invisible( id ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_is_invisible( id ) {");
#endif

    new iRenderMode = entity_get_int( id, EV_INT_rendermode );
    new iRenderFx = entity_get_int( id, EV_INT_renderfx );

    if ( iRenderMode == kRenderTransTexture && iRenderFx == kRenderFxNone )
        return ( 1 );

    return ( 0 );
}


public WAR3_player_init( id ) {
#if ADVANCED_DEBUG
	log_function("public WAR3_player_init( id ) {");
#endif

    g_iPlayerRounds[id] = 0;

    // Initialize all Saving Globals

    for ( new iRaceId; iRaceId < TOTAL_RACES; iRaceId++ )
    {
        g_iXPtotal[id][iRaceId] = 0;

		if ( get_cvar_num("war3x_save_xp_sql") == 1 )
		{
           	g_iXPfetched[id][iRaceId] = 0;
        }
    }

    g_iXPkill[id]       = {0,0};
    g_iXPobjective[id]  = {0,0};
    g_iXPsupport[id]    = {0,0};

    g_bXPfetched[id]    = false;

    g_bStoreXp[id]      = false;
    g_bStoreSkills[id]  = false;

    g_SaveIds[id][0]    = 0;

    g_PlayerInfo[id] = {0,0,0,0,0,0,0,0,0,0,0,0};

    return PLUGIN_HANDLED;
}


public WAR3_players_init() {
#if ADVANCED_DEBUG
	log_function("public WAR3_players_init() {");
#endif

    new Players[32], iTotalPlayers;
    get_players( Players, iTotalPlayers );

    for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new id = Players[iPlayerNum];
        WAR3_player_init( id );
    }

    return PLUGIN_HANDLED;
}

// Added by Geesu
public WAR3_player_valid( id )
{
#if ADVANCED_DEBUG
	log_function("public WAR3_player_valid( id )");
#endif
	if ( id > 0 && id <= get_maxplayers() )
	{
		return true;
	}

	return false;
}


public WAR3_is_first_race( id ) {

    for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
    {
        if ( g_iXPtotal[id][iRaceNum] )
            return ( 0 );
    }

    return ( 1 );
}

// End of UTIL_WAR3X.INC