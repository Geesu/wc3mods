// Begin UPKEEP.INL

/* - Constants Configuration ------------------------------------ */


#define UPKEEP_NO                0      // (integer) No Upkeep ID
#define UPKEEP_LOW               1      // (integer) Low Upkeep ID
#define UPKEEP_HIGH              2      // (integer) High Upkeep ID

#define UPKEEP_POINTS_LOW        3      // (integer) Points needed to reach low upkeep
#define UPKEEP_POINTS_HIGH       8      // (integer) Points needed to reach high upkeep

#define UPKEEP_STREAK          1.5      // (  float) Win Streak modifier
#define UPKEEP_KILLRATIO       0.5      // (  float) Kill Ratio modifier
#define UPKEEP_AVGLEVEL        1.0      // (  float) Average Level modifier
#define UPKEEP_PLAYERS         1.5      // (  float) Total Players modifier

#define UPKEEP_TIMER          10.0      // (  float) Interval to award dead players


new UPKEEP_MONEY_DEAD[3] =              // Money awarded to dead players
{
    50,                                 // No Upkeep
    25,                                 // Low Upkeep
    10                                  // High Upkeep
};

new UPKEEP_MONEY_NEWROUND[3] =          // Money to award players on new round
{
    1000,                               // No Upkeep
    500,                                // Low Upkeep
    150                                 // High Upkeep
};

new UPKEEP_NAME[3][] =
{
    "No Upkeep",
    "Low Upkeep",
    "High Upkeep"
};

new UPKEEP_RGB[3][3] =
{
    {0,255,0},
    {224,160,0},
    {255,0,0}
};

/* - Upkeep Functions ------------------------------------------- */


public Upkeep_Remove() {

    remove_task( TASK_UPKEEP, 0 );
    return PLUGIN_HANDLED;
}


public Upkeep_Dead() {

    new Players[32], iTotalPlayers;

    for ( new iTeamNum = CS_TEAM_TERRORIST; iTeamNum <= CS_TEAM_CT; iTeamNum++ )
    {
        // Check both teams

        if ( iTeamNum == CS_TEAM_TERRORIST )
            get_players( Players, iTotalPlayers, "be", "TERRORIST" );

        else
        {
            get_players( Players, iTotalPlayers, "be", "CT" );
        }

        for ( new iPlayerNum; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new player = Players[iPlayerNum];
            new iUpkeep = g_TeamUpkeep[iTeamNum - 1];

            new iDifference = iTotalPlayers * UPKEEP_MONEY_DEAD[iUpkeep];
            new iMoney = cs_get_user_money( player ) + iDifference;

            if ( iMoney > CS_MAX_MONEY )
                iMoney = CS_MAX_MONEY;

            cs_set_user_money( player, iMoney, 1 );

            // Display message

            new szMessage[32];
            format( szMessage, 31, "%s^n+$%d", UPKEEP_NAME[iUpkeep], iDifference );

            set_hudmessage( UPKEEP_RGB[iUpkeep][GLOW_R], UPKEEP_RGB[iUpkeep][GLOW_G], UPKEEP_RGB[iUpkeep][GLOW_B], HUDMESSAGE_POS_CENTER, 0.12, HUDMESSAGE_FX_FADEIN, 3.0, 2.0, 0.5, 1.0, HUDMESSAGE_CHAN_UPKEEP );
            show_hudmessage( player, szMessage );
        }
    }

    return PLUGIN_HANDLED;
}


public Upkeep_Newround() {

    new Players[32], iTotalPlayers;

    // Check both teams

    get_players( Players, iTotalPlayers, "a" );

    for ( new iPlayerNum; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new player = Players[iPlayerNum];
        new iTeamNum = get_user_team( player );

        new iUpkeep = g_TeamUpkeep[iTeamNum - 1];
        new iMoney = cs_get_user_money( player ) + ( UPKEEP_MONEY_NEWROUND[iUpkeep] );

        if ( iMoney > CS_MAX_MONEY )
            iMoney = CS_MAX_MONEY;

        cs_set_user_money( player, iMoney, 1 );

        // Display message

        new szMessage[32];
        format( szMessage, 31, "%s^n+$%d", UPKEEP_NAME[iUpkeep], UPKEEP_MONEY_NEWROUND[iUpkeep] );

        set_hudmessage( UPKEEP_RGB[iUpkeep][GLOW_R], UPKEEP_RGB[iUpkeep][GLOW_G], UPKEEP_RGB[iUpkeep][GLOW_B], HUDMESSAGE_POS_CENTER, 0.04, HUDMESSAGE_FX_FADEIN, 1.0, 2.0, 0.5, 1.0, HUDMESSAGE_CHAN_UPKEEP2 );
        show_hudmessage( player, szMessage );

    }

    return PLUGIN_HANDLED;
}


// Set new level of upkeep on this event

public Upkeep_RoundStart() {

    new Float:fTeamPoints[2];
    new iTeamLevels[2], iTeamKills[2], iTeamDeaths[2];

    new Players[32], iTotalPlayers;

    for ( new iTeamNum = CS_TEAM_TERRORIST; iTeamNum <= CS_TEAM_CT; iTeamNum++ )
    {
        // Check both teams

        if ( iTeamNum == CS_TEAM_TERRORIST )
            get_players( Players, iTotalPlayers, "be", "TERRORIST" );

        else
        {
            get_players( Players, iTotalPlayers, "be", "CT" );
        }

        for ( new iPlayerNum; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new player = Players[iPlayerNum];

            iTeamLevels[iTeamNum - 1] += WAR3_get_level( g_PlayerInfo[player][CURRENT_XP] );

            if ( get_user_frags( player ) >= 0 )
                iTeamKills[iTeamNum - 1] += get_user_frags( player );

            iTeamDeaths[iTeamNum - 1] += cs_get_user_deaths( player );
        }

        // Check Win Streaks and build points

        fTeamPoints[iTeamNum - 1] += float( g_TeamWinStreaks[iTeamNum - 1] ) * UPKEEP_STREAK;

        // Check Average Level and build points

        fTeamPoints[iTeamNum - 1] += ( float( iTeamLevels[iTeamNum - 1] ) / float( iTotalPlayers ) ) * UPKEEP_AVGLEVEL;

        // Check Total Players and build points

        fTeamPoints[iTeamNum - 1] += float( iTotalPlayers ) * UPKEEP_PLAYERS;

        // Check Kill/Death Ratio and build points

        fTeamPoints[iTeamNum - 1] += ( float( iTeamKills[iTeamNum - 1] ) / float( iTeamDeaths[iTeamNum - 1] ) ) * UPKEEP_KILLRATIO;
    }

    new iTeamPoints[2];
    iTeamPoints[0] = floatround( fTeamPoints[0] );
    iTeamPoints[1] = floatround( fTeamPoints[1] );

    // Find difference of points between teams

    new iMorePointsTeam;

    if ( iTeamPoints[0] > iTeamPoints[1] )
    {
        iMorePointsTeam = 0;
        iTeamPoints[0] = iTeamPoints[0] - iTeamPoints[1];
        iTeamPoints[1] = 0;

        Upkeep_Update( 1, g_TeamUpkeep[1], UPKEEP_NO );
    }

    else
    {
        iMorePointsTeam = 1;
        iTeamPoints[1] = iTeamPoints[1] - iTeamPoints[0];
        iTeamPoints[0] = 0;

        Upkeep_Update( 0, g_TeamUpkeep[0], UPKEEP_NO );
    }

    // Set new upkeep level (if applicable)

    new iCurrentUpkeep;

    if ( iTeamPoints[iMorePointsTeam] < UPKEEP_LOW )
        iCurrentUpkeep = UPKEEP_NO

    else if ( iTeamPoints[iMorePointsTeam] < UPKEEP_HIGH )
    {
        iCurrentUpkeep = UPKEEP_LOW
    }

    else
    {
        iCurrentUpkeep = UPKEEP_HIGH
    }

    Upkeep_Update( iMorePointsTeam, g_TeamUpkeep[iMorePointsTeam], iCurrentUpkeep );


    // Display Message to all teammates

    get_players( Players, iTotalPlayers, "a" );

    for ( new iPlayerNum; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new player = Players[iPlayerNum];
        new iTeamNum = get_user_team( player );

        new iUpkeep = g_TeamUpkeep[iTeamNum - 1];

        new szMessage[32];
        format( szMessage, 31, "%s", UPKEEP_NAME[iUpkeep] );

        set_hudmessage( UPKEEP_RGB[iUpkeep][GLOW_R], UPKEEP_RGB[iUpkeep][GLOW_G], UPKEEP_RGB[iUpkeep][GLOW_B], HUDMESSAGE_POS_CENTER, 0.04, HUDMESSAGE_FX_FADEIN, 1.0, 5.0, 0.5, 1.0, HUDMESSAGE_CHAN_UPKEEP );
        show_hudmessage( player, szMessage );
    }

    // Set new Upkeep timer

    new parm_Null[1];
    set_task( UPKEEP_TIMER, "Upkeep_Dead", TASK_UPKEEP, parm_Null, 0, "b" );

    return PLUGIN_HANDLED;
}


public Upkeep_Update( iTeamNum, iCurrentUpkeep, iNewUpkeep ) {

    if ( iCurrentUpkeep != iNewUpkeep )
    {
        // Update global

        g_TeamUpkeep[iTeamNum] = iNewUpkeep;

        // Play Sound ( client side )
    }

    return PLUGIN_HANDLED;
}