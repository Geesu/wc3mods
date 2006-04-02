
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


public WAR3_get_damage( weapon, iBonusDamage )
{
    new Float:fBonusDamage = float( iBonusDamage );

    switch ( cs_get_weapon_type_( weapon ) )
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


// Warcraft 3: Expansion Precaching

public WAR3_precache_sound( szWar3Sound[], szDefaultSound[] ) {

    new szWar3SoundPath[64];
    formatex( szWar3SoundPath, 63, "sound/%s", szWar3Sound );

    if ( file_exists( szWar3SoundPath ) )
        precache_sound( szWar3Sound );

    else if ( szDefaultSound[0] )
    {
        precache_sound( szDefaultSound );
    }

    return PLUGIN_HANDLED;
}

public WAR3_precache_model( szWar3Model[], szDefaultModel[] ) {

    if ( file_exists( szWar3Model ) )
        precache_model( szWar3Model );

    else if ( szDefaultModel[0] )
    {
        precache_model( szDefaultModel );
    }

    return PLUGIN_HANDLED;
}

public WAR3_precache() {

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
        copy( SOUND_HEALINGWAVE, 63,      "warcraft3/HealingWave.wav"                   );  // Healing Wave
        copy( SOUND_CHAINLIGHTNING, 63,   "warcraft3/LightningBolt.wav"                 );  // Chain Lightning
        copy( SOUND_WINDWALK, 63,         "warcraft3/WindWalk.wav"                      );  // Wind Walk
        copy( SOUND_REJUVENATION, 63,     "warcraft3/RejuvenationTarget1.wav"           );  // Rejuvenation
        copy( SOUND_ROOTS, 63,            "warcraft3/EntanglingRootsTarget1.wav"        );  // Entangling Roots
        copy( SOUND_SHADOWSTRIKE, 63,     "warcraft3/ShadowStrikeBirth1.wav"            );  // Shadow Strike
        copy( SOUND_FROSTNOVA, 63,        "warcraft3/FrostNova.wav"                     );

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
        copy( SOUND_HEALINGWAVE, 63,      "boid/boid_idle1.wav"        );   // Healing Wave
        copy( SOUND_CHAINLIGHTNING, 63,   "weapons/gauss2.wav"         );   // Chain Lightning
        copy( SOUND_WINDWALK, 63,         "houndeye/he_attack1.wav"    );   // Wind Walk
        copy( SOUND_REJUVENATION, 63,     "bullchicken/bc_acid1.wav"   );   // Rejuvenation
        copy( SOUND_ROOTS, 63,            "weapons/electro5.wav"       );   // Entangling Roots
        copy( SOUND_SHADOWSTRIKE, 63,     "controller/con_attack3.wav" );   // Shadow Strike
        copy( SOUND_FROSTNOVA, 63,        "houndeye/he_blast1.wav"     );   // Frost Nova

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
    precache_sound( SOUND_HEALINGWAVE );
    precache_sound( SOUND_CHAINLIGHTNING );
    precache_sound( SOUND_WINDWALK );
    precache_sound( SOUND_REJUVENATION );
    precache_sound( SOUND_ROOTS );
    precache_sound( SOUND_SHADOWSTRIKE );
    precache_sound( SOUND_FROSTNOVA );

    // Miscellaneous

    precache_sound( SOUND_LEVELUP );
    precache_sound( SOUND_PICKUPITEM );
    precache_sound( SOUND_TOME );
    precache_sound( SOUND_ULTIMATESCAN );
    precache_sound( SOUND_ULTIMATEREADY );


    // Bonus Sounds

    #if PRECACHE_WAR3BONUSSOUNDS

        precache_sound( "warcraft3/bonus/BarkSkinTarget1.wav"         );    // Nature's Blessing
        precache_sound( "warcraft3/bonus/BerzerkerCaster.wav"         );    // Berserk
        precache_sound( "warcraft3/bonus/DeathCoilMissileLaunch1.wav" );    // Death Coil (cast)
        precache_sound( "warcraft3/bonus/EntanglingRootsDecay1.wav"   );    // Entangling Roots (end)
        precache_sound( "warcraft3/bonus/Error.wav"                   );    // Error
        precache_sound( "warcraft3/bonus/FrostArmorTarget1.wav"       );    // Frost Armor
        precache_sound( "warcraft3/bonus/InnerFireBirth.wav"          );    // Inner Fire
        precache_sound( "warcraft3/bonus/Reincarnation.wav"           );    // Reincarnation
        precache_sound( "warcraft3/bonus/FrostNovaStateEnd.wav"       );    // Frost Nova (frozen)
        precache_sound( "warcraft3/bonus/OrcBurrowBattleStations.wav" );    // Regeneration
        precache_sound( "warcraft3/bonus/UpkeepRing.wav"              );    // Upkeep
        precache_sound( "warcraft3/bonus/ShadowMeld1.wav"             );    // Armor Gone

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

    // Print to status area

    set_hudmessage( 255, 208, 0, -1.0, 0.82, 0, 6.0, fDuration, 0.1, 0.5, 4 );
    show_hudmessage( id, "%s", szMessage );

    // Print important messages to console

    if ( fDuration >= 3.0 )
    {
        client_print( id, print_console, "%s%s", STATUS_PREFIX, szMessage );
    }

    return PLUGIN_HANDLED;
}


// Status Text ( harmful )

public WAR3_status_text2( id, szMessage[], Float:fDuration ) {

    // Print to status area

    set_hudmessage( 224, 180, 0, -1.0, 0.88, 0, 6.0, fDuration, 0.1, 0.5, 4 );
    show_hudmessage( id, "%s", szMessage );

    // Print important messages to console

    if ( fDuration >= 3.0 )
    {
        client_print( id, print_console, "%s%s", STATUS_PREFIX, szMessage );
    }

    return PLUGIN_HANDLED;
}


// Gets flag of an item

public WAR3_get_item_flag( iItemNum ) {

    new szFlag[2];
    get_letter( iItemNum , szFlag );

    return ( read_flags( szFlag ) );
}


// Gets flag of a skill

public WAR3_get_skill_flag( iRaceNum, iSkillNum ) {

    new iLetterNum = ( iRaceNum - 1 ) * TOTAL_SKILLS;
    iLetterNum += ( iSkillNum + 1 );

    new szFlag[2];
    get_letter( iLetterNum, szFlag );

    return ( read_flags( szFlag ) );
}


// Gets flag of an ultimate

public WAR3_get_ult_flag( iRaceNum, iUltimateNum ) {

    new iLetterNum = ( iRaceNum - 1 ) * TOTAL_ULTIMATES;
    iLetterNum += iUltimateNum;

    new szFlag[2];
    get_letter( iLetterNum, szFlag );

    return ( read_flags( szFlag ) );
}


// Gets flag of an ultimate

public WAR3_get_race_flag( iRaceNum ) {

    new szFlag[2];
    get_letter( iRaceNum + 1, szFlag );

    return ( read_flags( szFlag ) );
}


// Restriction check(s) **INCOMPLETE**

public WAR3_restrict_check( id ) {

    // Check current item

    if ( get_pcvar_bitsum( CVAR_restrict_items ) & WAR3_get_item_flag( g_PlayerInfo[id][CURRENT_ITEM] ) )
    {
        // Give back full cash ammount

        // Inform Player
    }

    // Check current race

    if ( get_pcvar_bitsum( CVAR_restrict_races ) & WAR3_get_race_flag( g_PlayerInfo[id][CURRENT_RACE] - 1 ) )
    {
        // Reset player race

        // Inform Player
    }

    /*
        // check current skill(s). if restricted, inform player. (set bool)

        if ( read_flags( g_RestrictedSkills ) & WAR3_get_skill_flag( g_PlayerInfo[id][CURRENT_RACE] - 1 ) )

        // check current ultimate(s). if restricted, inform player. (set bool)

        if ( get_pcvar_bitsum( CVAR_restrict_races ) & WAR3_get_race_flag( g_PlayerInfo[id][CURRENT_RACE] - 1 ) )
    */

    return PLUGIN_HANDLED;
}


public WAR3_hud_xp( id, iEffect, Float:fFxTime, Float:fFadeIn, Float:fFadeOut, Float:fHoldTime ) {

    if ( !is_user_alive( id ) || !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;

    new iCurrentXp = g_PlayerInfo[id][CURRENT_XP];

    new iLastlvlXp = WAR3_get_last_xp( iCurrentXp );
    new iNextlvlXp = WAR3_get_next_xp( iCurrentXp );

    new Float:fPercentage = ( float( iCurrentXp ) - float( iLastlvlXp ) ) / ( float( iNextlvlXp ) - float( iLastlvlXp ) ) * 100.0;

    new szMessage[64];
    formatex( szMessage, 63, "XP: %i/%i (%0.0f%%)", iCurrentXp, iNextlvlXp, fPercentage );

    // Get Coordinates

    new iResNum = g_PlayerOptions[id][OPTION_RESOLUTION];
    new Float:fXpos = 13.0 / STEAM_RESOLUTION[iResNum][_X];
    new Float:fYpos = float( STEAM_RESOLUTION[iResNum][_Y] - 78 + 5 ) / float( STEAM_RESOLUTION[iResNum][_Y] );

    set_hudmessage( 160, 160, 160, fXpos, fYpos, iEffect, fFxTime, fHoldTime, fFadeIn, fFadeOut, HUDMESSAGE_CHAN_XP );
    show_hudmessage( id, "%s", szMessage );

    return PLUGIN_HANDLED;
}

public WAR3_hud_item( id, iEffect, Float:fFxTime, Float:fFadeIn, Float:fFadeOut, Float:fHoldTime ) {

    if ( !is_user_alive( id ) || !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;

    new iRaceNum = g_PlayerInfo[id][CURRENT_RACE] - 1;
    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    new szMessage[64], szItemName[32];
	LANG_GetItemName ( g_PlayerInfo[id][CURRENT_ITEM], SHOP_COMMON, id, szItemName, 31 )


	new szShortRaceName[16];
	LANG_GetRaceName( iRaceNum + 1, id, szShortRaceName, 15, true );

    formatex( szMessage, 63, "%s Level %i   Item: %s", szShortRaceName, iLevel, szItemName );

    // Get Coordinates

    new iResNum = g_PlayerOptions[id][OPTION_RESOLUTION];
    new Float:fXpos = 13.0 / STEAM_RESOLUTION[iResNum][_X];
    new Float:fYpos = float( STEAM_RESOLUTION[iResNum][_Y] - 78 + 20 ) / float( STEAM_RESOLUTION[iResNum][_Y] );

    set_hudmessage( 224, 160, 0, fXpos, fYpos, iEffect, fFxTime, fHoldTime, fFadeIn, fFadeOut, HUDMESSAGE_CHAN_ITEM );
    show_hudmessage( id, "%s", szMessage );

    return PLUGIN_HANDLED;
}

public _WAR3_hud_item() {

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

    if ( id == g_Vip || read_flags( g_RestrictedSkills ) & WAR3_get_skill_flag( iRaceNum, iSkillNum ) )
        return ( 0 );

    return ( 1 );
}

public WAR3_is_ultimate( wIndex ) {

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

    new iUsedPoints = 0;
    new iTotalPoints = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    if ( g_PlayerInfo[id][CURRENT_ULTIMATE] )
        iUsedPoints += 1;

    iUsedPoints += g_PlayerInfo[id][CURRENT_SKILL1] + g_PlayerInfo[id][CURRENT_SKILL2] + g_PlayerInfo[id][CURRENT_SKILL3];

    return ( iTotalPoints - iUsedPoints );
}


public WAR3_get_onoff( id, iStatus, szOnOff[8] ) {

    if ( iStatus )
        formatex( szOnOff, 7, "%L", id, "LANG_ON" );

    else
    {
        formatex( szOnOff, 7, "%L", id, "LANG_OFF" );
    }

    return PLUGIN_HANDLED;
}


public WAR3_is_lowhealth( id ) {

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

    new bool:bDispellMe;

    if ( g_bPlayerSleeping[id] || g_PlayerOnFire[id] || g_PlayerSingeing[id] || g_PlayerStruck[id] || g_PlayerRooted[id] )
        bDispellMe = true;

    if ( bDispellMe )
        return ( 1 );

    return ( 0 );
}


// Checks that cIndex's ultimate can heal tIndex

public WAR3_can_heal( cIndex, tIndex ) {

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


// Healed by WAR3 skill

public WAR3_heal( caster, target, iAmount ) {

    // Calculate new health

    new iNewHealth = get_user_health( target ) + iAmount;
    new iMaxHealth = WAR3_get_maxhealth( target );

    new iHealthGiven = iAmount;

    if ( iNewHealth > iMaxHealth )
    {
        iHealthGiven -= ( iNewHealth - iMaxHealth );
        iNewHealth = iMaxHealth;
    }

    // Apply health

    set_user_health( target, iNewHealth );

    // Give support XP

    XP_Support_Heal( caster, iHealthGiven );

    // Invis cooldown on target ( if invisible )

    if ( g_bPlayerInvis[target] )
        SHARED_INVIS_cooldown_set( target, COOLDOWN_INVIS_HEALED );

    return ( iHealthGiven );
}


// Damage by Warcraft 3 Skill

public WAR3_damage( attacker, victim, weapon, iDamage, headshot, iCheckArmor ) {

    // Elune's Grace ( absorb bonus knife/ultimate damage )

    if ( g_PlayerInfo[victim][CURRENT_RACE] == RACE_NIGHTELF && iDamage )
        iDamage -= NE_S_ELUNES_get_bonus( victim, weapon, iDamage );

    // Notify damage to wind walker ( if applicable )

    if ( weapon == CSW_WINDWALK )
        OR_U_WINDWALK_notify( attacker, g_iWindwalkDamage[attacker] + iDamage );

    // Armor calculation

    if ( iCheckArmor && get_user_armor( victim ) )
    {
        new iArmorDamage = iDamage;
        iDamage = WAR3_get_damage( weapon, iDamage );

        new iNewArmor = get_user_armor( victim ) - iArmorDamage;

        if ( iNewArmor < 0 )
        {
            iDamage += ( iNewArmor * -1 );
            iNewArmor = 0;
        }

        set_user_armor( victim, iNewArmor );
    }

    new iNewHealth = get_user_health( victim ) - iDamage;
    new iMinHealth = WAR3_get_minhealth( victim );

    // Apply Damage

    if ( iNewHealth <= iMinHealth )
        WAR3_death( attacker, victim, weapon, headshot );

    else
    {
        set_user_health( victim, iNewHealth );

        // Invisibility Cooldown

        SHARED_INVIS_cooldown( victim );
    }

    // Update kill assist information

	WAR3_damage_assist( attacker, victim, iDamage );

    return PLUGIN_HANDLED;
}


// Death by Warcraft 3 Skill

public WAR3_death( killer, victim, weapon, headshot ) {

    if ( !is_user_alive( victim ) && !g_PlayerImpaled[victim] )
        return PLUGIN_HANDLED;


    new szWeaponName[32];

    // Perform Victim Actions

    WAR3_death_victim( victim );

    // Check for kill assist

    XP_Support_Kill( killer, victim );

    // Make sure death was not to DoT from disconnected killer

    if ( is_user_connected( killer ) )
    {
        // Check if Teamkill

        if ( get_user_team( killer ) == get_user_team( victim ) && killer != victim )
        {
            XP_Kill_Teammate( killer );

            cs_update_money( killer, -3300, 1 );

            new killerFrags = get_user_frags( killer ) - 1;
            new killerDeaths = get_user_deaths( killer );

            set_user_frags( killer, killerFrags );

            // Update frags ( realtime )

            Create_ScoreInfo( SHOWTO_ALL, killer, killerFrags, killerDeaths, 0, get_user_team( killer ) );
        }

        // Normal Kill

        else
        {
            XP_Kill( killer, victim, weapon, headshot );

            cs_update_money( killer, 300, 1 );

            new killerFrags = get_user_frags( killer ) + 1;
            new killerDeaths = get_user_deaths( killer );

            set_user_frags( killer, killerFrags );

            // Update frags ( realtime )

            Create_ScoreInfo( SHOWTO_ALL, killer, killerFrags, killerDeaths, 0, get_user_team( killer ) );
        }
    }

    // Create Death Message

    if ( is_user_alive( victim ) )
    {
        new iVictimFrags = get_user_frags( victim );
        new iVictimDeaths = get_user_deaths( victim );

        // Kill Victim

        new gmsgDeathMsg = get_user_msgid( "DeathMsg" );

        set_msg_block( gmsgDeathMsg, BLOCK_SET );
        user_kill( victim, 1 );
        set_msg_block( gmsgDeathMsg, BLOCK_NOT );

        // Update frags ( realtime )

        Create_ScoreInfo( SHOWTO_ALL, victim, iVictimFrags, iVictimDeaths, 0, get_user_team( victim ) );

        switch ( weapon )
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

        Create_DeathMsg( SHOWTO_ALL, killer, victim, headshot, szWeaponName );
    }

    // Log Death

    new szKillerName[32], szVictimName[32];
    new szKillerTeam[32], szVictimTeam[32];

    get_user_name( killer, szKillerName, 31 );
    get_user_name( victim, szVictimName, 31 );

    get_user_team( killer, szKillerTeam, 31 );
    get_user_team( victim, szVictimTeam, 31 );

    log_message( "^"%s<%d><%s><%s>^" killed ^"%s<%d><%s><%s>^" with ^"%s^"", szKillerName, get_user_userid( killer ), g_SaveIds[killer], szKillerTeam, szVictimName, get_user_userid( victim ), g_SaveIds[victim], szVictimTeam, szWeaponName );

    return PLUGIN_HANDLED;
}

public WAR3_damage_assist( attacker, victim, iDamage ) {

	if ( !( WAR3_player_valid( attacker ) || WAR3_player_valid( victim ) ) )
		return PLUGIN_HANDLED;

	g_iPlayerDamage[victim][attacker] += iDamage;
	g_iPlayerDamageTaken[victim] += iDamage;
	g_iPlayersThatDamage[victim][g_iTotalPlayersThatDamage[victim]] = attacker;
	g_iTotalPlayersThatDamage[victim]++;

	return PLUGIN_HANDLED;
}

public WAR3_is_invisible( id ) {

    new iRenderMode = entity_get_int( id, EV_INT_rendermode );
    new iRenderFx = entity_get_int( id, EV_INT_renderfx );

    if ( iRenderMode == kRenderTransTexture && iRenderFx == kRenderFxNone )
        return ( 1 );

    return ( 0 );
}


public WAR3_player_init( id ) {

    g_iPlayerRounds[id] = 0;

    // Initialize all Saving Globals

    for ( new iRaceId; iRaceId < TOTAL_RACES; iRaceId++ )
    {
        g_iXPtotal[id][iRaceId] = 0;

		if ( get_pcvar_num( CVAR_save_xp_sql ) == 1 )
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


public WAR3_player_valid( id )
{
	if ( id > 0 && id <= get_global_int( GL_maxClients ) )
	{
        if ( g_bPlayerConnected[id] )
    		return ( 1 );
	}

	return ( 0 );
}


public WAR3_is_first_race( id ) {

    for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
    {
        if ( g_iXPtotal[id][iRaceNum] )
            return ( 0 );
    }

    return ( 1 );
}


public WAR3_is_missing_files() {

    new bool:bMissingFiles = false;

    #if PRECACHE_RACEICONS

        new iMissingRaceIcons, szMissingRaceIcons[5][64];
        new bool:bMissingRaceIcons = false;

        if ( !file_exists( "sprites/warcraft3/icon_ud.spr" ) )
        {
            formatex( szMissingRaceIcons[iMissingRaceIcons++], 63, "sprites/warcraft3/icon_ud.spr" );
            bMissingRaceIcons = true;
        }

        if ( !file_exists( "sprites/warcraft3/icon_human.spr" ) )
        {
            formatex( szMissingRaceIcons[iMissingRaceIcons++], 63, "sprites/warcraft3/icon_human.spr" );
            bMissingRaceIcons = true;
        }

        if ( !file_exists( "sprites/warcraft3/icon_orc.spr" ) )
        {
            formatex( szMissingRaceIcons[iMissingRaceIcons++], 63, "sprites/warcraft3/icon_orc.spr" );
            bMissingRaceIcons = true;
        }

        if ( !file_exists( "sprites/warcraft3/icon_ne.spr" ) )
        {
            formatex( szMissingRaceIcons[iMissingRaceIcons++], 63, "sprites/warcraft3/icon_ne.spr" );
            bMissingRaceIcons = true;
        }

        if ( bMissingRaceIcons )
        {
            WAR3_missing_files();

            for ( new i = 0; i < iMissingRaceIcons; i++ )
                WAR3_missing_filename( "SPRITE", szMissingRaceIcons[i] );

            WAR3_missing_optional( "PRECACHE_RACEICONS" );

            bMissingFiles = true;
        }

    #endif

    #if PRECACHE_BLINKICONS

        if ( !file_exists( "sprites/warcraft3/saveme.spr" ) )
        {
            WAR3_missing_files();
            WAR3_missing_filename( "SPRITE", "sprites/warcraft3/saveme.spr" );
            WAR3_missing_optional( "PRECACHE_BLINKICONS" );

            bMissingFiles = true;
        }

    #endif

    #if PRECACHE_WAR3SOUNDS

        new iMissingWar3Sounds, szMissingWar3Sounds[17][64];
        new bool:bMissingWar3Sounds = false;

        if ( !file_exists( "sound/warcraft3/ImpaleHit.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/ImpaleHit.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/DeathCoilSpecialArt1.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/DeathCoilSpecialArt1.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/SleepBirth1.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/SleepBirth1.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/HolyBolt.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/HolyBolt.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/FlameStrikeTargetWaveNonLoop1.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/FlameStrikeTargetWaveNonLoop1.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/Avatar.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/Avatar.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/HealingWave.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/HealingWave.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/LightningBolt.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/LightningBolt.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/WindWalk.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/WindWalk.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/RejuvenationTarget1.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/RejuvenationTarget1.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/EntanglingRootsTarget1.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/EntanglingRootsTarget1.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/ShadowStrikeBirth1.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/ShadowStrikeBirth1.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/FrostNova.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/FrostNova.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/Levelupcaster.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/Levelupcaster.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/PickUpItem.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/PickUpItem.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/Tomes.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/Tomes.wav" );
            bMissingWar3Sounds = true;
        }

        if ( !file_exists( "sound/warcraft3/ResurrectTarget.wav" ) )
        {
            formatex( szMissingWar3Sounds[iMissingWar3Sounds++], 63, "sound/warcraft3/ResurrectTarget.wav" );
            bMissingWar3Sounds = true;
        }

        if ( bMissingWar3Sounds )
        {
            WAR3_missing_files();

            for ( new i = 0; i < iMissingWar3Sounds; i++ )
                WAR3_missing_filename( "SOUND", szMissingWar3Sounds[i] );

            WAR3_missing_optional( "PRECACHE_WAR3SOUNDS" );

            bMissingFiles = true;
        }

    #endif

    #if PRECACHE_WAR3BONUSSOUNDS

        new iMissingWar3BonusSounds, szMissingWar3BonusSounds[12][64];
        new bool:bMissingWar3BonusSounds = false;

        if ( !file_exists( "sound/warcraft3/bonus/BarkSkinTarget1.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/BarkSkinTarget1.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/BerzerkerCaster.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/BerzerkerCaster.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/DeathCoilMissileLaunch1.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/DeathCoilMissileLaunch1.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/EntanglingRootsDecay1.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/EntanglingRootsDecay1.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/Error.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/Error.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/FrostArmorTarget1.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/FrostArmorTarget1.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/InnerFireBirth.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/InnerFireBirth.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/Reincarnation.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/Reincarnation.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/FrostNovaStateEnd.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/FrostNovaStateEnd.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/OrcBurrowBattleStations.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/OrcBurrowBattleStations.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/UpkeepRing.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/UpkeepRing.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( !file_exists( "sound/warcraft3/bonus/ShadowMeld1.wav" ) )
        {
            formatex( szMissingWar3BonusSounds[iMissingWar3BonusSounds++], 63, "sound/warcraft3/bonus/ShadowMeld1.wav" );
            bMissingWar3BonusSounds = true;
        }

        if ( bMissingWar3BonusSounds )
        {
            WAR3_missing_files();

            for ( new i = 0; i < iMissingWar3BonusSounds; i++ )
                WAR3_missing_filename( "SOUND", szMissingWar3BonusSounds[i] );

            WAR3_missing_optional( "PRECACHE_WAR3BONUSSOUNDS" );

            bMissingFiles = true;
        }

    #endif

    // Required warcraft sprites

    if ( !file_exists( "sprites/warcraft3/sleep.spr" ) )
    {
        WAR3_missing_files();
        WAR3_missing_filename( "SPRITE", "sprites/warcraft3/sleep.spr" );
        WAR3_missing_required();

        bMissingFiles = true;
    }

    // Cascading style sheet

    if ( !file_exists( "war3x.css" ) )
    {
        WAR3_missing_files();
        WAR3_missing_filename( "GENERIC", "war3x.css" );
        WAR3_missing_required();

        bMissingFiles = true;
    }

    static AMXX_DIR_DATA[64];
	get_datadir( AMXX_DIR_DATA, 63 );

    new szLangFile[64];
    formatex( szLangFile, 63, "%s/lang/war3x.txt", AMXX_DIR_DATA );

    // Language file

    if ( !file_exists( szLangFile ) )
    {
        WAR3_missing_files();
        WAR3_missing_filename( "LANGUAGE", szLangFile );
        WAR3_missing_required();

        bMissingFiles = true;
    }

    // War3x Configuration

    static AMXX_DIR_CONFIGS[64];
	get_configsdir( AMXX_DIR_CONFIGS, 63 );

    new szWar3xCfgFile[64];
    formatex( szWar3xCfgFile, 63, "%s/war3x/war3x.cfg", AMXX_DIR_CONFIGS );

    if ( !file_exists( szWar3xCfgFile ) )
    {
        WAR3_missing_files();
        WAR3_missing_filename( "CONFIG", szWar3xCfgFile );
        WAR3_missing_required();

        bMissingFiles = true;
    }

    // Short term XP

    new szShortTermFile[64];
    formatex( szShortTermFile, 63, "%s/war3x/short_term_maps.cfg", AMXX_DIR_CONFIGS );

    if ( !file_exists( szShortTermFile ) )
    {
        WAR3_missing_warning();
        WAR3_missing_filename( "CONFIG", szShortTermFile );
        WAR3_missing_warning_note();
    }

    // Disabled maps

    new szDisabledFile[64];
    formatex( szDisabledFile, 63, "%s/war3x/disabled_maps.cfg", AMXX_DIR_CONFIGS );

    if ( !file_exists( szDisabledFile ) )
    {
        WAR3_missing_warning();
        WAR3_missing_filename( "CONFIG", szDisabledFile );
        WAR3_missing_warning_note();
    }

    if ( bMissingFiles )
        return ( true );

    return ( false );
}


public WAR3_missing_files() {

    log_amx( " " );
    log_amx( "* %L", LANG_SERVER, "ERROR_MISSING_FILES", WAR3X_PLUGINNAME );
    log_amx( "========================" );

    return PLUGIN_HANDLED;
}


public WAR3_missing_filename( szCategory[16], szFilename[64] ) {

    log_amx( "  -- ( %L: %s ) %s", LANG_SERVER, "LANG_CATEGORY", szCategory, szFilename );
    return PLUGIN_HANDLED;
}


public WAR3_missing_optional( szPrecacheName[32] ) {

    log_amx( "========================" );
    log_amx( "* %L", LANG_SERVER, "ERROR_MISSING_OPTIONAL", WAR3X_PLUGINNAME_SHORT, szPrecacheName );
    log_amx( " " );

    return PLUGIN_HANDLED;
}


public WAR3_missing_required() {

    log_amx( "========================" );
    log_amx( "* %L", LANG_SERVER, "ERROR_MISSING_REQUIRED" );
    log_amx( " " );

    return PLUGIN_HANDLED;
}


public WAR3_missing_warning() {

    log_amx( " " );
    log_amx( "* %L", LANG_SERVER, "WARNING_MISSING_FILES", WAR3X_PLUGINNAME );
    log_amx( "========================" );

    return PLUGIN_HANDLED;
}


public WAR3_missing_warning_note() {

    log_amx( "========================" );
    log_amx( "* %L", LANG_SERVER, "WARNING_MISSING_LISTED", WAR3X_PLUGINNAME_SHORT );
    log_amx( " " );

    return PLUGIN_HANDLED;
}


// Creates a WAR3 entity, which can be modified with entvars

public WAR3_ENTITY_create( szClassName[32], szModel[64], Float:fOrigin[3], iMoveType, iSolidType, Float:fLife ) {

    new ent = create_entity( "info_target" );
    entity_set_string( ent, EV_SZ_classname, szClassName );

    entity_set_model( ent, szModel );
    entity_set_origin( ent, fOrigin );

    entity_set_int( ent, EV_INT_movetype, iMoveType );
    entity_set_int( ent, EV_INT_solid, iSolidType );

    if ( fLife )
    {
        new arg_write[1];
        arg_write[0] = ent;

        new task = TASK_TEMPENTITY + ent;

        set_task( fLife, "WAR3_ENTITY_remove", task, arg_write, 1 );
    }

    return ( ent );
}


public WAR3_ENTITY_sprite( szClassName[32], szModel[64], Float:fOrigin[3], iMoveType, iSolidType, Float:fFramerate, Float:fScale, Float:fLife ) {

    new sprite = create_entity( "env_sprite" );

    entity_set_model( sprite, szModel );
    entity_set_float( sprite, EV_FL_framerate, fFramerate );

    DispatchSpawn( sprite );

    entity_set_origin( sprite, fOrigin );
    entity_set_string( sprite, EV_SZ_classname, szClassName );

    entity_set_int( sprite, EV_INT_movetype, iMoveType );
    entity_set_int( sprite, EV_INT_solid, iSolidType );
    entity_set_float( sprite, EV_FL_scale, fScale );

    set_entity_rendering( sprite, kRenderFxNone, 0, 0, 0, kRenderTransAdd, 255 );

    if ( fLife )
    {
        new arg_write[1];
        arg_write[0] = sprite;

        new task = TASK_TEMPENTITY + sprite;

        set_task( fLife, "WAR3_ENTITY_remove", task, arg_write, 1 );
    }

    return ( sprite );
}


public WAR3_ENTITY_remove( arg_read[1] ) {

	new ent = arg_read[0];

	if ( WAR3_is_valid_ent( ent ) )
		remove_entity( ent );

    return PLUGIN_HANDLED;
}


public WAR3_ENTITY_remove_class( szClassname[32], owner ) {

    new ent = find_ent_by_class( -1, szClassname );

    while ( ent > 0 )
    {
        if ( ( !owner || entity_get_edict( ent, EV_ENT_owner ) == owner ) && WAR3_is_valid_ent( ent ) )
        {
            new task = TASK_TEMPENTITY + ent;
            remove_task( task, 0 );

            remove_entity( ent );

            ent = find_ent_by_class( -1, szClassname );
        }

        else
        {
            ent = find_ent_by_class( ent, szClassname );
        }
    }

    return PLUGIN_HANDLED;
}


public WAR3_is_valid_ent( ent ) {

	// Don't want to look up info for an invalid ent

	if ( !is_valid_ent( ent ) )
		return ( false );

	// Entities that war3x creates

	new szWar3Entities[][64] = { "WAR3X_ITEM", "DEATH_COIL", "ROOT_PLANT", "IMPALE_CLAW", "SLEEP_Z", "REJUV_FLARE", "FLAME_STRIKE", "HEAL_EFFECT" };

    new szClassName[64];
	entity_get_string( ent, EV_SZ_classname, szClassName, 63 );

	// See if this class is in our list

	for ( new i = 0; i < sizeof( szWar3Entities ); i++ )
	{
		if ( equal( szClassName, szWar3Entities[i] ) )
			return ( true );
	}

	return ( false );
}


public WAR3_get_setxp_flag() {

    new szFlags[24];
    get_pcvar_string( CVAR_setxp_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}


public WAR3_log_setxp( id, szCommand[16], szPlayerName[32], szRaceName[16], szNewValue[16] ) {

    new szLogFile[64];
    formatex( szLogFile, 63, "%s/setxp_log.txt", WAR3X_DIR );

    if ( !file_exists( szLogFile ) )
    {
        write_file( szLogFile, "W A R C R A F T 3 : Expansion", -1 );
        write_file( szLogFile, "Logging of war3x_setxp and war3x_setlevel.", -1 );
        write_file( szLogFile, " ", -1 );
    }

    new szAdminName[32];
    get_user_name( id, szAdminName, 31 );

    new szCurrentTime[32];
    get_time( "%m.%d.%Y %H:%M:%S", szCurrentTime, 31 );

    new szLogEntry[256];
    formatex( szLogEntry, 255, "[%s] %s (%s) used command: '%s %s %s %s'", szCurrentTime, szAdminName, g_SaveIds[id], szCommand, szPlayerName, szRaceName, szNewValue );

    write_file( szLogFile, szLogEntry, -1 );

    return PLUGIN_HANDLED;
}

// ------------------------------------------------- End. - //
