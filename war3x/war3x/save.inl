// Begin SAVE.INL

/* - General Functions ------------------------------------------ */


// Get Save ID

public Get_SaveId( id ) {

    if ( get_cvar_num( "war3x_save_by_ip" ) )
        get_user_ip( id, g_SaveIds[id], 31 );

    else
    {
        get_user_authid( id, g_SaveIds[id], 31 );
    }

    return PLUGIN_HANDLED;
}


/* - Retrieve Stored Data --------------------------------------- */


// Retrieve Player XP from Database

public Retrieve_Xp( id ) {
	

	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        get_mysql_xp( id );
	}
	else
	{
        new szKeyName[64], szKeyData[64];

        format( szKeyName, 63, "war3x_%s_XP", g_SaveIds[id] );
        get_vaultdata( szKeyName, szKeyData, 63 );

        // Copy to Globals

        new iLen = copy( szKeyData, 63, szKeyData );

        if ( iLen > 0 )
        {
            for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
            {
                g_iXPtotal[id][iRaceNum] = iGettok( szKeyData, iRaceNum + 1, ',' );
            }
        }

        else
        {
            for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
            {
                g_iXPtotal[id][iRaceNum] = 0;
            }
        }
    }

    return PLUGIN_HANDLED;
}


// Retrieve Player Skills from Database

public Retrieve_Skills( id, iRaceId ) {

    new szData[64];

	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        get_mysql_skills( id, iRaceId + 1, szData );
	}
	else
	{
        new szKeyName[64];

        format( szKeyName, 63, "war3x_%s_%s", g_SaveIds[id], RACEKEYNAME[iRaceId] );
        get_vaultdata( szKeyName, szData, 63 );
	}

    // Copy to Globals

    g_PlayerInfo[id][CURRENT_SKILL1]    = iGettok( szData, 1, ',' );
    g_PlayerInfo[id][CURRENT_SKILL2]    = iGettok( szData, 2, ',' );
    g_PlayerInfo[id][CURRENT_SKILL3]    = iGettok( szData, 3, ',' );
    g_PlayerInfo[id][CURRENT_ULTIMATE]  = iGettok( szData, 4, ',' );

    return PLUGIN_HANDLED;
}


// Retrieve Player Options from Database

public Retrieve_Options( id ) {

    new szData[64];

	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        get_mysql_skills( id, 0, szData );
	}
	else
	{
        new szKeyName[64];

        format( szKeyName, 63, "war3x_%s_OPTIONS", g_SaveIds[id] );
        get_vaultdata( szKeyName, szData, 63 );
	}

    // Copy to Globals

    new iLen = copy( szData, 63, szData );

    // Check for older version(s)

    new bool:bNewVersion;

    if ( iLen == 3 )
        bNewVersion = true;

    // Store current player options

    if ( iLen > 0 )
    {
        // Retrieve Current Options/Default New Options

        if ( bNewVersion )
            Retrieve_Defaults( id );

        else
        {
            for ( new iOptionNum = 0; iOptionNum < TOTAL_OPTIONS; iOptionNum++ )
            {
                g_PlayerOptions[id][iOptionNum] = iGettok( szData, iOptionNum + 1, ',' );
            }
        }
    }

    // Set default options for new player

    else
    {
        for ( new iOptionNum = 0; iOptionNum < TOTAL_OPTIONS; iOptionNum++ )
        {
            // Show all icons

            if ( iOptionNum == OPTION_RACEICONS || iOptionNum == OPTION_HEALICONS || iOptionNum == OPTION_DISPELLICONS )
                g_PlayerOptions[id][iOptionNum] = 1;

            // Default screen resolution (1024x768)

            else if ( iOptionNum == OPTION_RESOLUTION )
                g_PlayerOptions[id][iOptionNum] = 4;

            else
            {
                g_PlayerOptions[id][iOptionNum] = 0;
            }
        }
    }

    return PLUGIN_HANDLED;
}


public Retrieve_Defaults( id ) {

    for ( new iOptionNum = 0; iOptionNum < TOTAL_OPTIONS; iOptionNum++ )
    {
        // Race Icons

        if ( iOptionNum == OPTION_RACEICONS )
            g_PlayerOptions[id][iOptionNum] = PLAYEROPTION_RACEICONS;

        // Heal Icons

        else if ( iOptionNum == OPTION_HEALICONS )
            g_PlayerOptions[id][iOptionNum] = PLAYEROPTION_HEALICONS;

        // Dispell Icons

        else if ( iOptionNum == OPTION_DISPELLICONS )
            g_PlayerOptions[id][iOptionNum] = PLAYEROPTION_DISPELLICONS;

        // Default screen resolution

        else if ( iOptionNum == OPTION_RESOLUTION )
            g_PlayerOptions[id][iOptionNum] = PLAYEROPTION_RESOLUTION;

        // Auto-Cast support ultimates

        else
        {
            g_PlayerOptions[id][iOptionNum] = PLAYEROPTION_AUTOCAST;
        }
    }

    return PLUGIN_HANDLED;
}


/* - Store Current Player --------------------------------------- */


// Stores all Player XP to Database

public Store_Xp( id ) {

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;


	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        for ( new iRaceId = 0; iRaceId < TOTAL_RACES; iRaceId++ )
        {
            if ( g_iXPtotal[id][iRaceId] != g_iXPfetched[id][iRaceId] && g_iXPtotal[id][iRaceId] )
            {
                if ( iRaceId == g_PlayerInfo[id][CURRENT_RACE] - 1 )
                {
                    new szData[32], iLen;

                    // Data

                    iLen += format( szData[iLen], 31 - iLen,  "%d", g_PlayerInfo[id][CURRENT_SKILL1] );
                    iLen += format( szData[iLen], 31 - iLen, ",%d", g_PlayerInfo[id][CURRENT_SKILL2] );
                    iLen += format( szData[iLen], 31 - iLen, ",%d", g_PlayerInfo[id][CURRENT_SKILL3] );
                    iLen += format( szData[iLen], 31 - iLen, ",%d", g_PlayerInfo[id][CURRENT_ULTIMATE] );

                    write_mysql_all( g_SaveIds[id], szData, iRaceId + 1, g_iXPtotal[id][iRaceId], g_iXPfetched[id][iRaceId] );
                }
                else
                {
                    write_mysql_xp( g_SaveIds[id], iRaceId + 1, g_iXPtotal[id][iRaceId], g_iXPfetched[id][iRaceId] );
                }

                g_iXPfetched[id][iRaceId] = g_iXPtotal[id][iRaceId];
            }
        }
	}
	else
	{
		new szKeyName[64], szKeyData[64], iLen;

        // Key Name

        format( szKeyName, 63, "war3x_%s_XP", g_SaveIds[id] );

        // Key Data

        for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
        {
            if ( iRaceNum > 0 )
                iLen += format( szKeyData[iLen], 63 - iLen, "," );

            iLen += format( szKeyData[iLen], 63 - iLen, "%d", g_iXPtotal[id][iRaceNum] );
        }

        // Store Data

        set_vaultdata( szKeyName, szKeyData );
	}

    return PLUGIN_HANDLED;
}


// Store Player Options to Database

public Store_Options( id ) {

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;


    new szData[32], iLen;

	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        new totalOptionNum = 0;             // no query is necessary if no options have been changed from default

        // Data

        for ( new iOptionNum = 0; iOptionNum < TOTAL_OPTIONS; iOptionNum++ )
        {
            if ( iOptionNum > 0 )
                iLen += format( szData[iLen], 31 - iLen, "," );

            iLen += format( szData[iLen], 31 - iLen, "%d", g_PlayerOptions[id][iOptionNum] );

            totalOptionNum += g_PlayerOptions[id][iOptionNum];
        }

        if ( totalOptionNum )
        {
            write_mysql_options( g_SaveIds[id], szData );
        }
	}
	else
	{
        new szKeyName[64];

        // Key Name

        format( szKeyName, 63, "war3x_%s_OPTIONS", g_SaveIds[id] );

        // Key Data

        for ( new iOptionNum = 0; iOptionNum < TOTAL_OPTIONS; iOptionNum++ )
        {
            if ( iOptionNum > 0 )
                iLen += format( szData[iLen], 31 - iLen, "," );

            iLen += format( szData[iLen], 31 - iLen, "%d", g_PlayerOptions[id][iOptionNum] );
        }

        // Store Data

        set_vaultdata( szKeyName, szData );
	}

    return PLUGIN_HANDLED;
}


// Store Player Skills to Database ( vault only )

public Store_Skills( id ) {

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;


    new szKeyName[64], szKeyData[64], iLen;
    new iRaceId = g_PlayerInfo[id][CURRENT_RACE] - 1;

    // Key Name

    format( szKeyName, 63, "war3x_%s_%s", g_SaveIds[id], RACEKEYNAME[iRaceId] );

    // Key Data

    iLen += format( szKeyData[iLen], 63 - iLen,  "%d", g_PlayerInfo[id][CURRENT_SKILL1] );
    iLen += format( szKeyData[iLen], 63 - iLen, ",%d", g_PlayerInfo[id][CURRENT_SKILL2] );
    iLen += format( szKeyData[iLen], 63 - iLen, ",%d", g_PlayerInfo[id][CURRENT_SKILL3] );
    iLen += format( szKeyData[iLen], 63 - iLen, ",%d", g_PlayerInfo[id][CURRENT_ULTIMATE] );

    // Store Data

    set_vaultdata( szKeyName, szKeyData );

    return PLUGIN_HANDLED;
}


/* - Store Queued Player Data ----------------------------------- */


// Stores XP of Queued Player to Database

public Store_Queue_Xp( iArrayIndex, szSaveId[32] ) {

	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        for ( new iRaceId = 0; iRaceId < TOTAL_RACES; iRaceId++ )
        {
            if ( g_SaveQueue_iXP[iArrayIndex][iRaceId] != g_SaveQueue_iXPfetched[iArrayIndex][iRaceId] && g_SaveQueue_iXP[iArrayIndex][iRaceId] )
            {
                if ( iRaceId == g_SaveQueue_iInfo[iArrayIndex][CURRENT_RACE] - 1 )
                {
                    new szData[32], iLen;

                    // Data
                    iLen += format( szData[iLen], 31 - iLen,  "%d", g_SaveQueue_iInfo[iArrayIndex][CURRENT_SKILL1] );
                    iLen += format( szData[iLen], 31 - iLen, ",%d", g_SaveQueue_iInfo[iArrayIndex][CURRENT_SKILL2] );
                    iLen += format( szData[iLen], 31 - iLen, ",%d", g_SaveQueue_iInfo[iArrayIndex][CURRENT_SKILL3] );
                    iLen += format( szData[iLen], 31 - iLen, ",%d", g_SaveQueue_iInfo[iArrayIndex][CURRENT_ULTIMATE] );

                    write_mysql_all( szSaveId, szData, iRaceId + 1, g_SaveQueue_iXP[iArrayIndex][iRaceId], g_SaveQueue_iXPfetched[iArrayIndex][iRaceId] );
                }
                else
                {
                    write_mysql_xp( szSaveId, iRaceId + 1, g_SaveQueue_iXP[iArrayIndex][iRaceId], g_SaveQueue_iXPfetched[iArrayIndex][iRaceId] );
                }

                g_SaveQueue_iXPfetched[iArrayIndex][iRaceId] = g_SaveQueue_iXP[iArrayIndex][iRaceId];
            }
        }
	}
	else
	{
        new szKeyName[64], szKeyData[64], iLen;

        // Key Name

        format( szKeyName, 63, "war3x_%s_XP", szSaveId );

        // Key Data

        for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
        {
            if ( iRaceNum > 0 )
                iLen += format( szKeyData[iLen], 63 - iLen, "," );

            iLen += format( szKeyData[iLen], 63 - iLen, "%d", g_SaveQueue_iXP[iArrayIndex][iRaceNum] );
        }

        // Store Data

        set_vaultdata( szKeyName, szKeyData );
	}

    return PLUGIN_HANDLED;
}


// Stores Queued Player Skills to Database ( vault only )

public Store_Queue_Skills( iArrayIndex, szSaveId[32] ) {

    new szKeyName[64], szKeyData[64], iLen;
    new iLastRace = g_SaveQueue_iInfo[iArrayIndex][CURRENT_RACE] - 1;

    // Key Name

    format( szKeyName, 63, "war3x_%s_%s", szSaveId, RACEKEYNAME[iLastRace] );

    // Key Data

    iLen += format( szKeyData[iLen], 63 - iLen,  "%d", g_SaveQueue_iInfo[iArrayIndex][CURRENT_SKILL1] );
    iLen += format( szKeyData[iLen], 63 - iLen, ",%d", g_SaveQueue_iInfo[iArrayIndex][CURRENT_SKILL2] );
    iLen += format( szKeyData[iLen], 63 - iLen, ",%d", g_SaveQueue_iInfo[iArrayIndex][CURRENT_SKILL3] );
    iLen += format( szKeyData[iLen], 63 - iLen, ",%d", g_SaveQueue_iInfo[iArrayIndex][CURRENT_ULTIMATE] );

    // Store Data

    set_vaultdata( szKeyName, szKeyData );

    return PLUGIN_HANDLED;
}


// Stores Queued Player Options to Database

public Store_Queue_Options( iArrayIndex, szSaveId[32] ) {

	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        new szData[32], iLen;
        new totalOptionNum = 0;             // no query is necessary if no options have been changed from default

        for ( new iOptionNum = 0; iOptionNum < TOTAL_OPTIONS; iOptionNum++ )
        {
            if ( iOptionNum > 0 )
                iLen += format( szData[iLen], 31 - iLen, "," );

            iLen += format( szData[iLen], 31 - iLen, "%d", g_SaveQueue_iOptions[iArrayIndex][iOptionNum] );
            totalOptionNum += g_SaveQueue_iOptions[iArrayIndex][iOptionNum];
        }

        if (totalOptionNum)
        {
            write_mysql_options( szSaveId, szData );
        }
    }
	else
	{
        new szKeyName[64], szKeyData[64], iLen;

        // Key Name

        format( szKeyName, 63, "war3x_%s_OPTIONS", szSaveId );

        // Key Data

        for ( new iOptionNum = 0; iOptionNum < TOTAL_OPTIONS; iOptionNum++ )
        {
            if ( iOptionNum > 0 )
                iLen += format( szKeyData[iLen], 63 - iLen, "," );

            iLen += format( szKeyData[iLen], 63 - iLen, "%d", g_SaveQueue_iOptions[iArrayIndex][iOptionNum] );
        }

        // Store Data

        set_vaultdata( szKeyName, szKeyData );
	}

    return PLUGIN_HANDLED;
}


// Stores Queued Player Options to Database ( vault only )

public Store_Queue_CurDate( szSaveId[32] ) {

    new szKeyName[64], szKeyData[64], szLastPlayed[16];

    // Key Name

    format( szKeyName, 63, "war3x_%s_LASTPLAYED", szSaveId );

    // Key Data

    get_time( "%m,%d,%Y", szLastPlayed, 15 );
    format( szKeyData, 63, "%s", szLastPlayed );

    // Store Data

    set_vaultdata( szKeyName, szKeyData );

    return PLUGIN_HANDLED;
}


/* - Player Queue Functions ------------------------------------- */


// Add Player to Save Queue

public Queue_Add( id, iArrayIndex, szSaveId[32] ) {
#if ADVANCED_DEBUG
	log_function("public Queue_Add( id, iArrayIndex, szSaveId[32] ) {");
#endif

    // Copy Authid to the SaveQueue Authid Array

    copy( g_SaveQueue_ids[iArrayIndex], 31, szSaveId );

    // Copy All Player XP to the SaveQueue XP Array

    for ( new iRaceId = 0; iRaceId < TOTAL_RACES; iRaceId++ )
    {
        g_SaveQueue_iXP[iArrayIndex][iRaceId] = g_iXPtotal[id][iRaceId];

		if ( get_cvar_num("war3x_save_xp_sql") == 1 )
		{
            g_SaveQueue_iXPfetched[iArrayIndex][iRaceId] = g_iXPfetched[id][iRaceId];
		}
    }

    // Copy PlayerInfo Array to SaveQueue Info Array

    for ( new i = 0; i < ( TOTAL_SKILLSTRAINED + 3 ); i++ )
    {
        g_SaveQueue_iInfo[iArrayIndex][i] = g_PlayerInfo[id][i];
    }

    // Copy PlayerOptions Array to SaveQueue Options Array

    for ( new i = 0; i < TOTAL_OPTIONS; i++ )
    {
        g_SaveQueue_iOptions[iArrayIndex][i] = g_PlayerOptions[id][i];
    }

    g_SaveQueue_iTotal++;

    return PLUGIN_HANDLED;
}


// Returns Array Index if szSaveId found in Queue Array

public Queue_Check( szSaveId[32] ) {
#if ADVANCED_DEBUG
	log_function("public Queue_Check( szSaveId[32] ) {");
#endif

    for ( new iArrayIndex = 0; iArrayIndex < TOTAL_QUEUES; iArrayIndex++ )
    {
        if ( g_SaveQueue_ids[iArrayIndex][0] && equal( g_SaveQueue_ids[iArrayIndex], szSaveId ) )
        {
            return ( iArrayIndex );
        }
    }

    return ( -1 );
}


// Copies Information from Queue to Globals

public Queue_Retrieve( id, iArrayIndex ) {

    // Retrieve XP from Queue

    for ( new iRaceId = 0; iRaceId < TOTAL_RACES; iRaceId++ )
    {
        g_iXPtotal[id][iRaceId] = g_SaveQueue_iXP[iArrayIndex][iRaceId];

		if ( get_cvar_num("war3x_save_xp_sql") == 1 )
		{
            g_iXPfetched[id][iRaceId] = g_SaveQueue_iXPfetched[iArrayIndex][iRaceId];
		}
    }

    // Retrieve Options from Queue

    for ( new i = 0; i < TOTAL_OPTIONS; i++ )
    {
        g_PlayerOptions[id][i] = g_SaveQueue_iOptions[iArrayIndex][i];
    }

    // Remove from Queue

    Queue_Remove( iArrayIndex );

    return PLUGIN_HANDLED;
}


// Clears Data from Given Queue Array Index

public Queue_Remove( iArrayIndex ) {

    // Clar Authid

    g_SaveQueue_ids[iArrayIndex][0] = 0;

    // Clear Player XP

    for ( new iRaceId = 0; iRaceId < TOTAL_RACES; iRaceId++ )
    {
        g_SaveQueue_iXP[iArrayIndex][iRaceId] = 0;

		if ( get_cvar_num("war3x_save_xp_sql") == 1 )
		{
            g_SaveQueue_iXPfetched[iArrayIndex][iRaceId] = 0;
		}
    }

    // Clear PlayerInfo Array

    for ( new i = 0; i < ( TOTAL_SKILLSTRAINED + 3 ); i++ )
    {
        g_SaveQueue_iInfo[iArrayIndex][i] = 0;
    }

    // Clear PlayerOptions Array

    for ( new i = 0; i < TOTAL_OPTIONS; i++ )
    {
        g_SaveQueue_iOptions[iArrayIndex][i] = 0;
    }

    g_SaveQueue_iTotal--;

    return PLUGIN_HANDLED;
}


/* - Reset Database Information --------------------------------- */


// Reset Skills for Current Race

public Reset_Skills( id ) {

    // Reset Skills

    g_PlayerInfo[id][CURRENT_SKILL1] = 0;
    g_PlayerInfo[id][CURRENT_SKILL2] = 0;
    g_PlayerInfo[id][CURRENT_SKILL3] = 0;

    // Prep for write

    g_bStoreSkills[id] = true;

    return PLUGIN_HANDLED;
}


// Reset Ultimate for Current Race

public Reset_Ultimate( id ) {

    // Reset Skills

    g_PlayerInfo[id][CURRENT_ULTIMATE] = 0;

    // Prep for write

    g_bStoreSkills[id] = true;

    return PLUGIN_HANDLED;
}


// Reset XP for Current Race

public Reset_XP( id ) {

    new iRaceNum = g_PlayerInfo[id][CURRENT_RACE] - 1;

    // Reset XP

    g_iXPtotal[id][iRaceNum] = 0;

    // Reset Skills

    Reset_Skills( id );
    Reset_Ultimate( id );

    // Prep for write

    g_bStoreXp[id] = true;
    g_bStoreSkills[id] = true;

	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        g_iXPfetched[id][iRaceNum] = 1;      // entry still exists, even if its 0
	}

    return PLUGIN_HANDLED;
}


/* - Store All Player Data -------------------------------------- */


// Stores all Current Player data to Database

public Store_ActivePlayer( id ) {

    if ( is_user_bot( id ) || !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_HANDLED;

    // .. add a || g_UnsavedRounds[id] < ROUNDS_UNSAVED

    // Store Only Needed Values

	if ( get_cvar_num("war3x_save_xp_sql") == 1 )
	{
        if ( g_bStoreXp[id] || g_bStoreSkills[id])
            Store_Xp( id );

        if ( g_bStoreOptions[id] )
            Store_Options( id );
	}
	else
	{
        if ( g_bStoreXp[id] )
            Store_Xp( id );

        if ( g_bStoreSkills[id] )
            Store_Skills( id );

        if ( g_bStoreOptions[id] )
            Store_Options( id );
	}

    g_bStoreXp[id]      = false;
    g_bStoreSkills[id]  = false;
    g_bStoreOptions[id] = false;

    return PLUGIN_HANDLED;
}


// Stores all Queued Player data to Database

public Store_Queue_All() {

    if ( !g_SaveQueue_iTotal )
        return PLUGIN_HANDLED;

    new iArrayIndex;

    while ( g_SaveQueue_iTotal > 0 && iArrayIndex < TOTAL_QUEUES )
    {
        if ( g_SaveQueue_ids[iArrayIndex][0] )
        {
            new szSaveId[32];
            copy( szSaveId, 31, g_SaveQueue_ids[iArrayIndex] );

            Store_Queue_Xp( iArrayIndex, szSaveId );
            Store_Queue_Options( iArrayIndex, szSaveId );

			if ( !get_cvar_num("war3x_save_xp_sql") )
			{
                Store_Queue_Skills( iArrayIndex, szSaveId );
                Store_Queue_CurDate( szSaveId );
			}

            // Remove from Queue

            Queue_Remove( iArrayIndex );
        }

        iArrayIndex++;
    }

    return PLUGIN_HANDLED;
}


// End of SAVE.INL
