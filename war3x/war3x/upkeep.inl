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


/* - Upkeep Functions ------------------------------------------- */


public Upkeep_Remove() {

    remove_task( TASK_UPKEEP, 0 );
    return PLUGIN_HANDLED;
}


public Upkeep_Dead() {

    new Players[32], iTotalPlayers;

    // Check both teams

    get_players( Players, iTotalPlayers, "b" );

    for ( new iPlayerNum; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new player = Players[iPlayerNum];
        new iTeamNum = get_user_team( player );

        new iMoney = cs_get_user_money( player );
        iMoney += iTotalPlayers * UPKEEP_MONEY_DEAD[g_TeamUpkeep[iTeamNum - 1]];

        if ( iMoney > CS_MAX_MONEY )
            iMoney = CS_MAX_MONEY;

        cs_set_user_money( player, iMoney, 0 );

        // Display message
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

        new iMoney = cs_get_user_money( player );
        iMoney += iTotalPlayers * UPKEEP_MONEY_NEWROUND[g_TeamUpkeep[iTeamNum - 1]];

        if ( iMoney > CS_MAX_MONEY )
            iMoney = CS_MAX_MONEY;

        cs_set_user_money( player, iMoney, 0 );

        // Display message
    }

    return PLUGIN_HANDLED;
}


// Set new level of upkeep on this event

public Upkeep_RoundStart() {

    // Check Kill Streaks and build points

    // Check Average Level and build points

    // Check Total Players and build points

    // Check Kill/Death Ratio and build points

    // Find difference of points between teams

    // Set new upkeep level (if applicable)

    // Set new Upkeep timer

    set_task( UPKEEP_TIMER, "Upkeep_Dead", TASK_UPKEEP );

    return PLUGIN_HANDLED;
}