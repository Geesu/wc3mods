// Begin XP.INL

/* - Experience Configuration ----------------------------------- *

 *  Kill Experience         : ( XP_KILL * _bonus_xp_modifiers_ * war3x_xp_normal )
 *  Objective XP (team)     : ( XP_OBJECTIVE * war3x_xp_normal * war3x_xp_objective )
 *  Objective XP (personal) : ( XP_OBJECTIVE * war3x_xp_normal * war3x_xp_objective + ( level * XP_OBJECTIVE_LVL_BONUS ) )

 * -------------------------------------------------------------- */


#define XP_KILL                    18.0     // (  float) base XP for each kill (before multipliers)
#define XP_TEAMKILL                25.0     // (  float) penalty for teamkill
#define XP_HEADSHOT_MULTIPLIER     1.35     // (  float) headshot multiplier (1.0 disables headshot multiplier)
#define XP_OBJECTIVE_LVL_BONUS      2.0     // (  float) personal objective bonus ( called on personal objective xp events )

#define XP_HOSTAGE_TOUCH            3.0     // (  float) bonus XP for touching hostages
#define XP_HOSTAGE_KILLRESCUER     10.0     // (  float) bonus XP for killing hostage rescuer
#define XP_BOMBPLANT               50.0     // (  float) bonus XP for planting bomb
#define XP_DEFUSE                  50.0     // (  float) bonus XP for defusing bomb
#define XP_KILLPLANTER             10.0     // (  float) bonus XP for killing bomb planter
#define XP_KILLDEFUSER             10.0     // (  float) bonus XP for killing bomb defuser
#define XP_VIPESCAPE               75.0     // (  float) bonus XP for escaping as VIP
#define XP_KILLVIP                 20.0     // (  float) bonus XP for killing VIP

#define XP_HOSTAGE_SHARE           10.0     // (  float) bonus XP for rescuing hostages
#define XP_BOMBPLANT_SHARE         30.0     // (  float) bonus XP for everyone in range of bomb when planted (even planter)
#define XP_DEFUSE_SHARE            30.0     // (  float) bonus XP for protecting defuser
#define XP_VIPESCAPE_SHARE         30.0     // (  float) bonus XP for protecting VIP

#define XP_TARGETBOMBED            10.0     // (  float) bonus XP for bombing target (all t's get)
#define XP_TARGETSAVED_CT          15.0     // (  float) bonus XP for saving target (all ct's get)
#define XP_VIPNOTESCAPE_T          15.0     // (  float) bonus XP for preventing VIP escape (all t's get)
#define XP_ALLHOSTAGESRESCUED      15.0     // (  float) bonus XP for rescuing all hostages (all cts get)
#define XP_HOSTAGENOTRESCUED_T     15.0     // (  float) bonus XP for preventing hostage rescue (all t's get)
#define XP_TARGETSAVED_T           20.0     // (  float) XP penalty for not attempting to bomb target (all alive t's get)
#define XP_VIPNOTESCAPE_VIP        20.0     // (  float) XP penalty for not attempting to escape (VIP gets)
#define XP_HOSTAGENOTRESCUED_CT    20.0     // (  float) XP penalty for not attempting to rescue hostages (all alive ct's get)

#define XP_KILLHOSTAGE             15.0     // (  float) XP penalty for killing hostages
#define XP_KILLHOSTAGE_NUM            2     // (integer) Number of hostages allowed killed before penalty.
#define XP_KILLHOSTAGE_SUBNUM         1     // (integer) Number of hostage kills to subtract on new round.

#define XP_LEVELBONUS               4.0     // (  float) bonus XP awarded to killer for each level victim is above him

#define XP_HOSTAGE_SHARE_RADIUS      15     // (integer) meters around hostage rescuers to grant _SHARE XP
#define XP_BOMBPLANT_SHARE_RADIUS    15     // (integer) meters around bomb planter to grant _SHARE XP
#define XP_DEFUSE_SHARE_RADIUS       15     // (integer) meters around bomb defuser to grant _SHARE XP
#define XP_VIPESCAPE_SHARE_RADIUS    15     // (integer) meters around VIP to grant _SHARE XP

#define XP_KILLSTREAK              25.0     // (  float) bonus XP for killing _NUM players in a row
#define XP_KILLSTREAK_NUM             5     // (integer) number of players to kill in a row to receive bonus XP

#define XP_SUPPORT_HEAL            0.75     // (  float) ratio of total ammount of health healed on teammate to give as xp
#define XP_SUPPORT_DISPELL           20     // (integer) xp given for each effect dispelled on a teammate

#define WAR3X_SETXP                   0     // (integer) set xp arg switch
#define WAR3X_SETLEVEL                1     // (integer) set xp arg switch

new const LONGTERM_XP[11]   = {             // (integer) XP required to gain each level ( longterm )

//  0,  1,  2,  3,  4,   5,   6,   7,    8,    9,   10
    0,100,200,400,800,1600,3200,6400,12800,25600,50000

};

new const SHORTTERM_XP[11]  = {             // (integer) XP required to gain each level ( shortterm )

//  0, 1, 2, 3,  4,  5,   6,  7,  8,  9,  10
    0,25,75,125,175,250,350,475,625,800,1000

};


/* -------------------------------------------------------------- */


// Set Level Xp Values

public Set_LevelXp() {

    // Long Term XP

    if ( g_bSaveXp )
    {
        for ( new iLevel = 0; iLevel <= TOTAL_LEVELS; iLevel++ )
        {
            g_iLevelXp[iLevel] = LONGTERM_XP[iLevel];
        }
    }

    // Short Term XP

    else
    {
        for ( new iLevel = 0; iLevel <= TOTAL_LEVELS; iLevel++ )
        {
            g_iLevelXp[iLevel] = SHORTTERM_XP[iLevel];
        }
    }

    return PLUGIN_HANDLED;
}


public get_setxp_flag() {

    new szFlags[24];
    get_pcvar_string( CVAR_setxp_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}


public Cmd_SetXp( id ) {

    if ( id && !( get_user_flags( id ) & get_setxp_flag() ) )
    {
        client_print( id, print_console, "* [WC3x] You have no access to that command." );
        return PLUGIN_HANDLED;
    }

    if ( read_argc() < 4 )
    {
        new szErrorMsg[256];
        formatex( szErrorMsg, 255, "* [WC3x] Invalid number of Arguments.^n* [WC3x] Format is: war3x_setxp < name/part of name > < racename/part of racename > < [+/-]new value >." );

        status_print( id, szErrorMsg );
    }

    else
    {
        new szArgs[128];
        read_args( szArgs, 127 );

        Cmd_SetXpLevel( id, "war3x_setxp", szArgs );
    }

    return PLUGIN_HANDLED;
}


public Cmd_SetLevel( id ) {

    if ( id && !( get_user_flags( id ) & get_setxp_flag() ) )
    {
        client_print( id, print_console, "* [WC3x] You have no access to that command." );
        return PLUGIN_HANDLED;
    }

    if ( read_argc() < 4 )
    {
        new szErrorMsg[256];
        formatex( szErrorMsg, 255, "* [WC3x] Invalid number of Arguments.^n* [WC3x] Format is: war3x_setxp < name/part of name > < racename/part of racename > < [+/-]new value >." );

        status_print( id, szErrorMsg );
    }

    else
    {
        new szArgs[128];
        read_args( szArgs, 127 );

        Cmd_SetXpLevel( id, "war3x_setlevel", szArgs );
    }

    return PLUGIN_HANDLED;
}


public Cmd_SetXpLevel( id, szCommand[16], szArgs[128] ) {

    new iSetType;

    if ( equal( szCommand, "war3x_setlevel" ) )
        iSetType = WAR3X_SETLEVEL;

    // Grab Args

    new szPlayerName[32], szRaceName[16], szNewValue[16];
    parse( szArgs, szPlayerName, 31, szRaceName, 15, szNewValue, 15 );

    // Get Player ID

    new playerId = find_player( "b", szPlayerName );

    if ( !playerId )
    {
        new szErrorMsg[256];
        formatex( szErrorMsg, 255, "* [WC3x] WARNING: No players found matching '%s'.", szPlayerName );
        status_print( id, szErrorMsg );

        return PLUGIN_HANDLED;
    }

    else if ( !g_PlayerInfo[playerId][CURRENT_RACE] )
    {
        new szErrorMsg[256];
        formatex( szErrorMsg, 255, "* [WC3x] WARNING: XP Info for %s has not been retrieved yet.", szPlayerName );
        status_print( id, szErrorMsg );

        return PLUGIN_HANDLED;
    }

    get_user_name( playerId, szPlayerName, 31 );

    // Get Race ID

    new szAbRaceName[2], iRaceId;
    copy( szAbRaceName, 1, szRaceName );

    strtolower( szAbRaceName );

    if ( equali( szAbRaceName, "u" ) )
        iRaceId = RACE_UNDEAD - 1;

    else if ( equali( szAbRaceName, "h" ) )
        iRaceId = RACE_HUMAN - 1;

    else if ( equali( szAbRaceName, "o" ) )
        iRaceId = RACE_ORC - 1;

    else if ( equali( szAbRaceName, "n" ) )
        iRaceId = RACE_NIGHTELF - 1;

    else
    {
        status_print( id, "* [WC3x] ERROR: Invalid Racename." );
        return PLUGIN_HANDLED;
    }

    // Get New Value

    new iNewValue, iOldValue, bool:bAddXp;

    new szMessage[128], szAdminName[32];
    get_user_name( id, szAdminName, 31 );

    switch ( iSetType )
    {
        case WAR3X_SETXP:
        {
            // Get New Ammount

            new szAddXp[2];
            copy( szAddXp, 1, szNewValue );

            if ( equali( szAddXp, "+" ) )
                bAddXp = true;

            iNewValue = str_to_num( szNewValue );
            iOldValue = g_iXPtotal[playerId][iRaceId];

            if ( iNewValue == iOldValue && !bAddXp )
            {
                status_print( id, "* [WC3x] WARNING: Old and new xp values are the same." );
                return PLUGIN_HANDLED;
            }

			new szShortRaceName[32];
			LANG_GetRaceName( iRaceId + 1, id, szShortRaceName, 31, true );

            // Set Message(s)

            if ( bAddXp )
            {
                formatex( szMessage, 127, "%s has rewarded %d experience to your^n%s character", szAdminName, iNewValue, szShortRaceName );
                client_print( playerId, print_chat, "* [WC3x] %s has rewarded %d experience to your %s character", szAdminName, iNewValue, szShortRaceName );

                iNewValue = iOldValue + iNewValue;
            }

            else if ( iNewValue < 0 )
            {
                formatex( szMessage, 127, "%s has removed %d experience from your^n%s character", szAdminName, iNewValue * -1, szShortRaceName );
                client_print( playerId, print_chat, "* [WC3x] %s has removed %d experience from your %s character", szAdminName, iNewValue * -1, szShortRaceName );

                iNewValue = iOldValue + iNewValue;
            }

            else
            {
                formatex( szMessage, 127, "%s has set your %s experience to %d", szAdminName, szShortRaceName, iNewValue );
                client_print( playerId, print_chat, "* [WC3x] %s has set your %s experience to %d", szAdminName, szShortRaceName, iNewValue );
            }
        }

        case WAR3X_SETLEVEL:
        {
            new szAddLevel[2];
            copy( szAddLevel, 1, szNewValue );

            if ( equali( szAddLevel, "+" ) )
                bAddXp = true;

            iNewValue = str_to_num( szNewValue );
            iOldValue = WAR3_get_level( g_iXPtotal[playerId][iRaceId] );

            if ( iNewValue == iOldValue && !bAddXp )
            {
                status_print( id, "* [WC3x] WARNING: Old and new values are the same." );
                return PLUGIN_HANDLED;
            }

            // Set Message(s)

			new szShortRaceName[32];
			LANG_GetRaceName( iRaceId + 1, id, szShortRaceName, 31, true );

            if ( bAddXp )
            {
                formatex( szMessage, 127, "%s has rewarded %d level(s) to your^n%s character", szAdminName, iNewValue, szShortRaceName );
                client_print( playerId, print_chat, "* [WC3x] %s has rewarded %d level(s) to your %s character", szAdminName, iNewValue, szShortRaceName );

                iNewValue = iOldValue + iNewValue;
            }

            else if ( iNewValue < 0 )
            {
                formatex( szMessage, 127, "%s has removed %d level(s) from your^n%s character", szAdminName, iNewValue * -1, szShortRaceName );
                client_print( playerId, print_chat, "* [WC3x] %s has removed %d level(s) from your %s character", szAdminName, iNewValue * -1, szShortRaceName );

                iNewValue = iOldValue + iNewValue;
            }

            else
            {
                formatex( szMessage, 127, "%s has set your %s character to level %d", szAdminName, szShortRaceName, iNewValue );
                client_print( playerId, print_chat, "* [WC3x] %s has set your %s character to level %d", szAdminName, szShortRaceName, iNewValue );
           }

            if ( iNewValue < 0 )
                iNewValue = 0;

            if ( iNewValue > TOTAL_LEVELS )
                iNewValue = TOTAL_LEVELS;

            // Set Proper XP Values

            iOldValue = g_iLevelXp[iOldValue];
            iNewValue = g_iLevelXp[iNewValue];
        }
    }

    // Hudmessage Color

    if ( bAddXp || iNewValue > iOldValue )
        set_hudmessage( 0, 196, 0, HUDMESSAGE_POS_CENTER, HUDMESSAGE_POS_SERVER - 0.02, 0, 0.1, 6.0, 0.5, 2.0, HUDMESSAGE_CHAN_SERVER );

    else if ( iNewValue < 0 )
        set_hudmessage( 196, 0, 0, HUDMESSAGE_POS_CENTER, HUDMESSAGE_POS_SERVER - 0.02, 0, 0.1, 6.0, 0.5, 2.0, HUDMESSAGE_CHAN_SERVER );

    else
    {
        set_hudmessage( 196, 0, 0, HUDMESSAGE_POS_CENTER, HUDMESSAGE_POS_SERVER, 0, 0.1, 6.0, 0.5, 2.0, HUDMESSAGE_CHAN_SERVER );
    }

    show_hudmessage( playerId, "%s", szMessage );

    // Change XP

    g_iXPtotal[playerId][iRaceId] = iNewValue;

    // Reset Skills/Ultimates if current race

    if ( g_PlayerInfo[playerId][CURRENT_RACE] == iRaceId + 1 )
    {
        g_PlayerInfo[playerId][CURRENT_XP] = iNewValue;
        WAR3_hud_item( playerId, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );

        // Check if current level less than old level

        if ( WAR3_get_level( iNewValue ) < WAR3_get_level( iOldValue ) )
        {
            // Reset all skills and ultimate

            Reset_Skills( playerId );
            Reset_Ultimate( playerId );

            // Show skill selection menu

            if ( WAR3_get_level( iNewValue ) )
                menu_SelectSkills( playerId );
        }

        else if ( WAR3_get_level( iNewValue ) )
        {
            WAR3_check_level( playerId, iOldValue, iNewValue );
            WAR3_check_skills( playerId );
        }
    }

    if ( g_bSaveXp )
        g_bStoreXp[playerId] = true;


    // Log Usage

	new szShortRaceName[16];
	LANG_GetRaceName( iRaceId + 1, id, szShortRaceName, 15, true );

    if ( iSetType == WAR3X_SETLEVEL )
        XP_Log_to_file( id, "war3x_setlevel", szPlayerName, szShortRaceName, szNewValue );

    else
    {
        XP_Log_to_file( id, "war3x_setxp", szPlayerName, szShortRaceName, szNewValue );
    }

    // Echo to Player that Issued Command

    new szConfirm[256];
    formatex( szConfirm, 255, "* [WC3x] New value(s) set for %s's %s character. Old XP: %d, New XP: %d", szPlayerName, szShortRaceName, iOldValue, iNewValue );

    status_print( id, szConfirm );

    return PLUGIN_HANDLED;
}


public XP_Log_to_file( id, szCommand[16], szPlayerName[32], szRaceName[16], szNewValue[16] ) {

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


// Calculate / Give XP

public XP_Give( id, iOldXp, iNewXp ) {

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new iRaceNum = g_PlayerInfo[id][CURRENT_RACE] - 1;

    if ( g_PlayerInfo[id][CURRENT_RACE] && iLevel != TOTAL_LEVELS )
    {
        if ( iNewXp > g_iLevelXp[TOTAL_LEVELS] )
            iNewXp = g_iLevelXp[TOTAL_LEVELS];

        WAR3_check_level( id, iOldXp, iNewXp );

        g_PlayerInfo[id][CURRENT_XP] = iNewXp;
        g_iXPtotal[id][iRaceNum] = iNewXp;

        // Hudmessage

        WAR3_hud_xp( id, HUDMESSAGE_FX_WRITEOUT, 0.05, 0.02, 3.0, 0.1 );

        if ( g_bSaveXp )
            g_bStoreXp[id] = true;
    }

    return PLUGIN_HANDLED;
}


// Calculate / Give XP Loss

public XP_Remove( id, iOldXp, iNewXp ) {

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    if ( !g_PlayerInfo[id][CURRENT_RACE] || iLevel == 10 )
        return ( 0 );


    if ( iNewXp < 0 )
        iNewXp = 0;

    new iXpLoss;

    if ( WAR3_get_level( iOldXp ) > WAR3_get_level( iNewXp ) )
    {
        iXpLoss = iOldXp - g_iLevelXp[WAR3_get_level( iOldXp )];
        g_PlayerInfo[id][CURRENT_XP] = g_iLevelXp[WAR3_get_level( iOldXp )];
    }

    else
    {
        iXpLoss = iOldXp - iNewXp;
        g_PlayerInfo[id][CURRENT_XP] = iNewXp;
    }

    // Hudmessage

    WAR3_hud_xp( id, HUDMESSAGE_FX_WRITEOUT, 0.05, 0.02, 3.0, 0.1 );

    if ( g_bSaveXp )
        g_bStoreXp[id] = true;

    return ( iXpLoss );
}


// Calculate / Give Objective Xp

public XP_Objective_Player( id, Float:fObjectiveXp ) {

    new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
    new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new Float:fLevel = float( iLevel );

    new Float:fObjLevelBonus = XP_OBJECTIVE_LVL_BONUS;

    new iBonusXp = floatround( fObjectiveXp * fMultiplier * fObjMultiplier + ( fLevel * fObjLevelBonus ) );

    new iOldXp = g_PlayerInfo[id][CURRENT_XP];
    new iNewXp = g_PlayerInfo[id][CURRENT_XP] + iBonusXp;

    XP_Give( id, iOldXp, iNewXp );

    return ( iBonusXp );
}


// Calculate / Give Shared Objective Xp

public XP_Objective_Share( id, teamId, iRadius, Float:fShareXp ) {

    new Origin[3], teamOrigin[3];

    get_user_origin( id, Origin );
    get_user_origin( teamId, teamOrigin );

    if ( get_distance( Origin, teamOrigin ) / 40 <= iRadius )
    {
        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new iLevel = WAR3_get_level( g_PlayerInfo[teamId][CURRENT_XP] );
        new Float:fLevel = float( iLevel );

        new Float:fObjLevelBonus = XP_OBJECTIVE_LVL_BONUS;

        new iBonusShareXp = floatround( fShareXp * fMultiplier * fObjMultiplier + ( fLevel * fObjLevelBonus ) );

        new iOldXp = g_PlayerInfo[teamId][CURRENT_XP];
        new iNewXp = g_PlayerInfo[teamId][CURRENT_XP] + iBonusShareXp;

        XP_Give( teamId, iOldXp, iNewXp );

        return ( iBonusShareXp );
    }

    return ( 0 );
}


// Calculate / Give Team Objective XP

public XP_Objective_Team( szTeamName[], iTeamXp ) {

    new TeamPlayers[32], iTotalPlayers;
    get_players( TeamPlayers, iTotalPlayers, "e", szTeamName );

    for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new teamId = TeamPlayers[iPlayerNum];
        new iLevel = WAR3_get_level( g_PlayerInfo[teamId][CURRENT_RACE] );

        if ( g_PlayerInfo[teamId][CURRENT_RACE] && iLevel < 10 )
        {
            new iOldXp = g_PlayerInfo[teamId][CURRENT_XP];
            new iNewXp = g_PlayerInfo[teamId][CURRENT_XP] + iTeamXp;

            XP_Give( teamId, iOldXp, iNewXp );
        }
    }

    return PLUGIN_HANDLED;
}


/* -------------------------------------------------------------- */


// XP Awarded on Kill

public XP_Kill( killer, victim, weapon, headshot ) {

    // Update streaks

    g_PlayerInfo[killer][CURRENT_KILLSTREAK] += 1;
    g_PlayerInfo[killer][CURRENT_DEATHSTREAK] = 0;

    new killerLevel = WAR3_get_level( g_PlayerInfo[killer][CURRENT_XP] );

    if ( killerLevel == 10 )
        return PLUGIN_HANDLED;


    new killerNewXp, killerOldXp;
    new Float:fKillXp, iLevelBonusXp, iKillStreakXp, iObjectiveXp;

    new iVictimLevel = WAR3_get_level( g_PlayerInfo[victim][CURRENT_XP] );

    new Float:fKillerLevel = float( killerLevel );
    new Float:fVictimLevel = float( iVictimLevel );

    new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
    new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

    // Kill XP

    new Float:fKill = XP_KILL;
    fKillXp = fKill * fMultiplier * g_fWeaponXp[weapon];

    // Check for headshot

    if ( headshot )
        fKillXp *= XP_HEADSHOT_MULTIPLIER;


    // Award Level Bonus

    if ( iVictimLevel > killerLevel )
    {
        new Float:fLevelBonus = XP_LEVELBONUS * ( fVictimLevel - fKillerLevel );
        iLevelBonusXp = floatround( fLevelBonus * fMultiplier );
    }

    // Check for Killstreak XP Bonus

    if ( g_PlayerInfo[killer][CURRENT_KILLSTREAK] > 0 && g_PlayerInfo[killer][CURRENT_KILLSTREAK] % XP_KILLSTREAK_NUM == 0 )
    {
        new Float:fKillStreak = XP_KILLSTREAK;

        iKillStreakXp = ( g_PlayerInfo[killer][CURRENT_KILLSTREAK] / XP_KILLSTREAK_NUM ) * floatround( fKillStreak * fMultiplier );
        client_print( killer, print_chat, XP_KILLSTREAK_MESSAGE, iKillStreakXp, g_PlayerInfo[killer][CURRENT_KILLSTREAK] );
    }

    // Objective XP

    new Float:fObjLevelBonus = XP_OBJECTIVE_LVL_BONUS;

    // Check if VIP Killed

    if ( victim == g_Vip )
    {
        new Float:fKillVip = XP_KILLVIP;

        iObjectiveXp = floatround( fKillVip * fMultiplier * fObjMultiplier + ( fKillerLevel * fObjLevelBonus ) );
        client_print( killer, print_chat, XP_KILLVIP_MESSAGE, iObjectiveXp );
    }

    // Check if Bomb Planter Killed

    else if ( victim == g_BombPlanter )
    {
        new Float:fKillPlanter = XP_KILLPLANTER;

        iObjectiveXp = floatround( fKillPlanter * fMultiplier * fObjMultiplier + ( fKillerLevel * fObjLevelBonus ) );
        client_print( killer, print_chat, XP_KILLPLANTER_MESSAGE, iObjectiveXp );
    }

    // Check if Bomb Defuser Killed

    else if ( victim == g_BombDefuser )
    {
        new Float:fKillDefuser = XP_KILLDEFUSER;

        iObjectiveXp = floatround( fKillDefuser * fMultiplier * fObjMultiplier + ( fKillerLevel * fObjLevelBonus ) );
        client_print( killer, print_chat, XP_KILLDEFUSER_MESSAGE, iObjectiveXp );
    }

    // Check if Hostage Rescuer Killed

    else if ( g_bTouchHostage[victim] )
    {
        new Float:fKillRescuer = XP_HOSTAGE_KILLRESCUER;

        iObjectiveXp = floatround( fKillRescuer * fMultiplier * fObjMultiplier + ( fKillerLevel * fObjLevelBonus ) );
        client_print( killer, print_chat, XP_HOSTAGE_KILLRESCUER_MESSAGE, iObjectiveXp );

        g_bTouchHostage[victim] = false;
    }


    // Give XP if race is selected

    killerOldXp = g_PlayerInfo[killer][CURRENT_XP];
    killerNewXp = killerOldXp + floatround( fKillXp ) + iLevelBonusXp + iKillStreakXp + iObjectiveXp;

    XP_Give( killer, killerOldXp, killerNewXp );

    return PLUGIN_HANDLED;
}


// XP Loss on Teamkill

public XP_Kill_Teammate( id ) {

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    if ( iLevel == 10 )
        return PLUGIN_HANDLED;


    new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
    new Float:fTeamKill = XP_TEAMKILL;

    new killerOldXp = g_PlayerInfo[id][CURRENT_XP];
    new killerNewXp = g_PlayerInfo[id][CURRENT_XP] - floatround( fTeamKill * fMultiplier );


    new iXpLoss = XP_Remove( id, killerOldXp, killerNewXp );

    if ( iXpLoss )
        client_print( id, print_chat, XP_TEAMKILL_MESSAGE, iXpLoss );

    // Reset Kill Streak(s)

    g_PlayerInfo[id][CURRENT_KILLSTREAK] = 0;

    return PLUGIN_HANDLED;
}


// XP Loss on Death

public XP_Death( id ) {

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    if ( iLevel == 10 )
        return PLUGIN_HANDLED;

    // Update Streaks

    g_PlayerInfo[id][CURRENT_KILLSTREAK] = 0;

    return PLUGIN_HANDLED;
}


/* -------------------------------------------------------------- */


// Hostage Touched

public XP_Hostage_Touch( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_HOSTAGE_TOUCH

        if ( g_PlayerInfo[id][CURRENT_RACE] )
        {
            new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

            if ( iLevel == 10 )
                client_print( id, print_chat, "%s", XP_MAX_MESSAGE );

            else
            {
                new iBonusXp = XP_Objective_Player( id, XP_HOSTAGE_TOUCH );
                client_print( id, print_chat, XP_HOSTAGE_TOUCH_MESSAGE, iBonusXp );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// Hostage Rescued

public XP_Hostage_Rescued( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_HOSTAGE_SHARE

        // Give XP to all alive CTs within share radius

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new szTeamName[16];
        new vTeamPlayers[32], iTotalPlayers;

        get_user_team( id, szTeamName, 15 );
        get_players( vTeamPlayers, iTotalPlayers, "ae", szTeamName );

        new rescueOrigin[3], teamOrigin[3];
        get_user_origin( id, rescueOrigin );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new teamId = vTeamPlayers[iPlayerNum];
            get_user_origin( teamId, teamOrigin );

            new iLevel = WAR3_get_level( g_PlayerInfo[teamId][CURRENT_XP] );

            if ( get_distance( rescueOrigin, teamOrigin ) / 40 <= XP_HOSTAGE_SHARE_RADIUS && g_PlayerInfo[teamId][CURRENT_RACE] && iLevel < 10 )
            {
                new Float:fShare = XP_HOSTAGE_SHARE;
                new Float:fObjLevelBonus = XP_OBJECTIVE_LVL_BONUS;

                new Float:fLevel = float( iLevel );

                new iShareXp = floatround( fShare * fMultiplier * fObjMultiplier + ( fLevel * fObjLevelBonus ) );

                client_print( teamId, print_chat, XP_HOSTAGE_SHARE_MESSAGE, iShareXp );

                new iOldXp = g_PlayerInfo[teamId][CURRENT_XP];
                new iNewXp = g_PlayerInfo[teamId][CURRENT_XP] + iShareXp;

                XP_Give( teamId, iOldXp, iNewXp );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// Hostage Killed

public XP_Hostage_Killed( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) || !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;


    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    if ( g_iHostageKills[id] > XP_KILLHOSTAGE_NUM && iLevel < 10 )
    {
        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fKillHostage = XP_KILLHOSTAGE;

        new iKillHostageXp = floatround( fKillHostage * fMultiplier * fObjMultiplier );

        new iOldXp = g_PlayerInfo[id][CURRENT_XP];
        new iNewXp = g_PlayerInfo[id][CURRENT_XP] - iKillHostageXp;

        new iXpLoss = XP_Remove( id, iOldXp, iNewXp );

        if ( iXpLoss )
            client_print( id, print_chat, XP_KILLHOSTAGE_MESSAGE, iXpLoss );
    }

    // Reset Kill Streak(s)

    g_PlayerInfo[id][CURRENT_KILLSTREAK] = 0;

    return PLUGIN_HANDLED;
}


// Spawn with Bomb / Pickup Bomb

public XP_Bomb_Pickup( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) || get_user_team( id ) != CS_TEAM_TERRORIST )
        return PLUGIN_HANDLED;


    // Give XP Notification to Bomb Carrier

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    #if XP_BOMBPLANT

        if ( iLevel == 10 )
            client_print( id, print_chat, "%s", XP_MAX_BOMBPICKUP_MESSAGE );

        else
        {
            new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
            new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

            new Float:fLevel = float( iLevel );

            new Float:fBombPlant = XP_BOMBPLANT;
            new Float:fObjLevelBonus = XP_OBJECTIVE_LVL_BONUS;

            new iBombPlantXp = floatround( fBombPlant * fMultiplier * fObjMultiplier + ( fLevel * fObjLevelBonus ) );
            client_print( id, print_chat, XP_BOMBPICKUP_MESSAGE, iBombPlantXp );
        }

    #endif

    return PLUGIN_HANDLED;
}


// Bomb Dropped

public XP_Bomb_Drop( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    // Currently no XP for Dropping Bomb
    // This Function is here for Expansion Purposes

    return PLUGIN_HANDLED;
}


// Bomb Planted ( planter )

public XP_Bomb_Plant( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_BOMBPLANT

        if ( g_PlayerInfo[id][CURRENT_RACE] )
        {
            new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

            if ( iLevel == 10 )
                client_print( id, print_chat, "%s", XP_MAX_MESSAGE );

            else
            {
                new iBonusXp = XP_Objective_Player( id, XP_BOMBPLANT );
                client_print( id, print_chat, XP_BOMBPLANT_MESSAGE, iBonusXp );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// Bomb Planted ( supporters )

public XP_Bomb_Share( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_BOMBPLANT_SHARE

        new szTeamName[16];
        new TeamPlayers[32], iTotalPlayers;

        get_user_team( id, szTeamName, 15 );
        get_players( TeamPlayers, iTotalPlayers, "ae", szTeamName );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new teamId = TeamPlayers[iPlayerNum];
            new iLevel = WAR3_get_level( g_PlayerInfo[teamId][CURRENT_XP] );

            if ( g_PlayerInfo[teamId][CURRENT_RACE] && iLevel < 10 && teamId != id )
            {
                new iBonusXp = XP_Objective_Share( id, teamId, XP_BOMBPLANT_SHARE_RADIUS, XP_BOMBPLANT_SHARE );

                if ( iBonusXp )
                    client_print( teamId, print_chat, XP_BOMBPLANT_SHARE_MESSAGE, iBonusXp );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// Bomb Defused ( defuser )

public XP_Bomb_Defuse( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_DEFUSE

        if ( g_PlayerInfo[id][CURRENT_RACE] )
        {
            new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

            if ( iLevel == 10 )
                client_print( id, print_chat, "%s", XP_MAX_MESSAGE );

            else
            {
                new iBonusXp = XP_Objective_Player( id, XP_DEFUSE );
                client_print( id, print_chat, XP_DEFUSE_MESSAGE, iBonusXp );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// Bomb Defused ( supporters )

public XP_Defuse_Share( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_DEFUSE_SHARE

        new szTeamName[16];
        new TeamPlayers[32], iTotalPlayers;

        get_user_team( id, szTeamName, 15 );
        get_players( TeamPlayers, iTotalPlayers, "ae", szTeamName );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new teamId = TeamPlayers[iPlayerNum];
            new iLevel = WAR3_get_level( g_PlayerInfo[teamId][CURRENT_XP] );

            if ( g_PlayerInfo[teamId][CURRENT_RACE] && iLevel < 10 && teamId != id )
            {
                new iBonusXp = XP_Objective_Share( id, teamId, XP_DEFUSE_SHARE_RADIUS, XP_DEFUSE_SHARE );

                if ( iBonusXp )
                    client_print( teamId, print_chat, XP_DEFUSE_SHARE_MESSAGE, iBonusXp );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// Vip Spawn

public XP_Vip_Spawn( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_VIPESCAPE

        new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

        if ( iLevel == 10 )
            client_print( id, print_chat, "%s", XP_MAX_VIPSPAWN_MESSAGE );

        else
        {
            new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
            new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

            new Float:fLevel = float( iLevel );

            new Float:fVipEscape = XP_VIPESCAPE;
            new Float:fObjLevelBonus = XP_OBJECTIVE_LVL_BONUS;

            new iVipEscapeXp = floatround( fVipEscape * fMultiplier * fObjMultiplier + ( fLevel * fObjLevelBonus ) );
            client_print( id, print_chat, XP_VIPSPAWN_MESSAGE, iVipEscapeXp );
        }

    #endif

    return PLUGIN_HANDLED;
}


// Vip Escape ( vip )

public XP_Vip_Escape( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_VIPESCAPE

        if ( g_PlayerInfo[id][CURRENT_RACE] )
        {
            new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

            if ( iLevel == 10 )
                client_print( id, print_chat, "%s", XP_MAX_MESSAGE );

            else
            {
                new iBonusXp = XP_Objective_Player( id, XP_VIPESCAPE );
                client_print( id, print_chat, XP_VIPESCAPE_MESSAGE, iBonusXp );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// Vip Escape ( protectors )

public XP_Vip_Share( id ) {

    if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_HANDLED;


    #if XP_VIPESCAPE_SHARE

        new szTeamName[16];
        new TeamPlayers[32], iTotalPlayers;

        get_user_team( id, szTeamName, 15 );
        get_players( TeamPlayers, iTotalPlayers, "ae", szTeamName );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new teamId = TeamPlayers[iPlayerNum];
            new iLevel = WAR3_get_level( g_PlayerInfo[teamId][CURRENT_XP] );

            if ( g_PlayerInfo[teamId][CURRENT_RACE] && iLevel < 10 && teamId != id )
            {
                new iBonusXp = XP_Objective_Share( id, teamId, XP_VIPESCAPE_SHARE_RADIUS, XP_VIPESCAPE_SHARE );

                if ( iBonusXp )
                    client_print( teamId, print_chat, XP_VIPESCAPE_SHARE_MESSAGE, iBonusXp );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


/* -------------------------------------------------------------- */


// Target Bombed ( terrorist )
// Occurs when target is successfully bombed

public XP_Target_Bombed_T() {

    #if XP_TARGETBOMBED

		if ( get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
			return PLUGIN_HANDLED;

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fTargetBombed = XP_TARGETBOMBED;

        new iTargetBombedXp = floatround( fTargetBombed * fMultiplier * fObjMultiplier );
        client_print( 0, print_chat, XP_TARGETBOMBED_MESSAGE, iTargetBombedXp );

        new szTeamName[] = "TERRORIST";

        XP_Objective_Team( szTeamName, iTargetBombedXp );

    #endif

    return PLUGIN_HANDLED;
}


// Target Saved ( terrorist )
// Occurs when round time elapses and no bomb plants attempted

public XP_Target_Saved_T() {

    if ( g_bBombPlanted )
        return PLUGIN_HANDLED;


    #if XP_TARGETSAVED_T

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fTargetSaved = XP_TARGETSAVED_T;

        new iTargetSavedXp = floatround( fTargetSaved * fMultiplier * fObjMultiplier );

        new TeamPlayers[32], iTotalPlayers;
        get_players( TeamPlayers, iTotalPlayers, "ae", "TERRORIST" );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new teamId = TeamPlayers[iPlayerNum];
            new iLevel = WAR3_get_level( g_PlayerInfo[teamId][CURRENT_XP] );

            if ( g_PlayerInfo[teamId][CURRENT_RACE] && iLevel < 10 )
            {
                new iOldXp = g_PlayerInfo[teamId][CURRENT_XP];
                new iNewXp = g_PlayerInfo[teamId][CURRENT_XP] - iTargetSavedXp;

                new iXpLoss = XP_Remove( teamId, iOldXp, iNewXp );

                if ( iXpLoss )
                    client_print( teamId, print_chat, XP_TARGETSAVED_T_MESSAGE, iXpLoss );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// Target Saved ( counter-terrorist )
// Occurs when round time elapses and no bomb plants attempted -and- when bomb defused

public XP_Target_Saved_CT() {

    #if XP_TARGETSAVED_CT

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fTargetSaved = XP_TARGETSAVED_CT;

        new iTargetSavedXp = floatround( fTargetSaved * fMultiplier * fObjMultiplier );
        client_print( 0, print_chat, XP_TARGETSAVED_CT_MESSAGE, iTargetSavedXp );

        new szTeamName[] = "CT";

        XP_Objective_Team( szTeamName, iTargetSavedXp );

    #endif

    return PLUGIN_HANDLED;
}


// Vip Not Escaped ( terrorist )
// Occurs when round time elapses and vip has not escaped -and- when VIP assassinated

public XP_Not_Escaped_T() {

    #if XP_VIPNOTESCAPE_T

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fVipNotEscape = XP_VIPNOTESCAPE_T;

        new iVipNotEscapeXp = floatround( fVipNotEscape * fMultiplier * fObjMultiplier );
        client_print( 0, print_chat, XP_VIPNOTESCAPE_T_MESSAGE, iVipNotEscapeXp );

        new szTeamName[] = "TERRORIST";

        XP_Objective_Team( szTeamName, iVipNotEscapeXp );

    #endif

    return PLUGIN_HANDLED;
}


// Vip Not Escaped ( vip )
// Occurs when round time elapses and vip has not escaped

public XP_Not_Escaped_VIP() {

    if ( !g_Vip )
        return PLUGIN_HANDLED;


    #if XP_VIPNOTESCAPE_VIP

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fVipNotEscape = XP_VIPNOTESCAPE_VIP;

        new iVipNotEscapeXp = floatround( fVipNotEscape * fMultiplier * fObjMultiplier );

        new iOldXp = g_PlayerInfo[g_Vip][CURRENT_XP];
        new iNewXp = g_PlayerInfo[g_Vip][CURRENT_XP] - iVipNotEscapeXp;

        new iXpLoss = XP_Remove( g_Vip, iOldXp, iNewXp );

        if ( iXpLoss )
            client_print( g_Vip, print_chat, XP_VIPNOTESCAPE_VIP_MESSAGE );

    #endif

    return PLUGIN_HANDLED;
}


// Hostages Not Rescued ( terrorist )
// Occurs when round time elapses and no hostages have been rescued

public XP_Not_Rescued_T() {

    #if XP_HOSTAGENOTRESCUED_T

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fNotRescued = XP_HOSTAGENOTRESCUED_T;

        new iNotRescuedXp = floatround( fNotRescued * fMultiplier * fObjMultiplier );
        client_print( 0, print_chat, XP_HOSTAGENOTRESCUED_T_MESSAGE, iNotRescuedXp );

        new szTeamName[] = "TERRORIST";

        XP_Objective_Team( szTeamName, iNotRescuedXp );

    #endif

    return PLUGIN_HANDLED;
}


// Hostages Not Rescued ( counter-terrorist )
// Occurs when round time elapses and no hostages have been rescued

public XP_Not_Rescued_CT() {

    if ( g_bHostageRescued )
        return PLUGIN_HANDLED;


    #if XP_HOSTAGENOTRESCUED_CT

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fNotRescued = XP_HOSTAGENOTRESCUED_CT;

        new iNotRescuedXp = floatround( fNotRescued * fMultiplier * fObjMultiplier );

        new TeamPlayers[32], iTotalPlayers;
        get_players( TeamPlayers, iTotalPlayers, "ae", "CT" );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new teamId = TeamPlayers[iPlayerNum];
            new iLevel = WAR3_get_level( g_PlayerInfo[teamId][CURRENT_XP] );

            if ( g_PlayerInfo[teamId][CURRENT_RACE] && iLevel < 10 )
            {
                new iOldXp = g_PlayerInfo[teamId][CURRENT_XP];
                new iNewXp = g_PlayerInfo[teamId][CURRENT_XP] - iNotRescuedXp;

                new iXpLoss = XP_Remove( teamId, iOldXp, iNewXp );

                if ( iXpLoss )
                    client_print( teamId, print_chat, XP_HOSTAGENOTRESCUED_CT_MESSAGE, iXpLoss );
            }
        }

    #endif

    return PLUGIN_HANDLED;
}


// All Hostages Rescued ( counter-terrorist )
// Occurs when all hostages have been rescued ( duh! )

public XP_All_Hostages_CT() {

    #if XP_ALLHOSTAGESRESCUED

        new Float:fMultiplier = get_pcvar_float( CVAR_xp_normal );
        new Float:fObjMultiplier = get_pcvar_float( CVAR_xp_objective );

        new Float:fAllHostages = XP_ALLHOSTAGESRESCUED;

        new iAllHostagesXp = floatround( fAllHostages * fMultiplier * fObjMultiplier );
        client_print( 0, print_chat, XP_ALLHOSTAGESRESCUED_MESSAGE, iAllHostagesXp );

        new szTeamName[] = "CT";

        XP_Objective_Team( szTeamName, iAllHostagesXp );

    #endif

    return PLUGIN_HANDLED;
}


/* -------------------------------------------------------------- */


// Give Support XP

public XP_Support_Heal( caster, iHealthGiven ) {

    if ( !iHealthGiven )
        return PLUGIN_HANDLED;

    if ( WAR3_get_level( g_PlayerInfo[caster][CURRENT_XP] ) == TOTAL_LEVELS )
    {
        client_print( caster, print_chat, XP_SUPPORT_HEALMSG_MAX, iHealthGiven );
        return PLUGIN_HANDLED;
    }


    new Float:fSupportXp = float( iHealthGiven ) * XP_SUPPORT_HEAL * get_pcvar_float( CVAR_xp_normal );
    new iSupportXp = floatround( fSupportXp );

    g_iXPsupport[caster][XP_ROUND] += iSupportXp;

    new iOldXp = g_PlayerInfo[caster][CURRENT_XP];
    new iNewXp = iOldXp + iSupportXp;

    XP_Give( caster, iOldXp, iNewXp );

    client_print( caster, print_chat, XP_SUPPORT_HEALMSG, iSupportXp, iHealthGiven );

    return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. - //
