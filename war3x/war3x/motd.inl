// Begin MOTD.INL

/* - MOTD Windows ----------------------------------------------- */


// Warcraft 3 Help

public motd_War3xHelp( id ) {
#if ADVANCED_DEBUG
	log_function("public motd_War3xHelp( id ) {");
#endif

    new szFilename[64];
    format( szFilename, 63, "%s/help/war3help.html", WAR3X_DIR );

    show_motd( id, szFilename, WAR3X_MOTD_HELP );

    return PLUGIN_HANDLED;
}


// Items Info

public motd_ItemsHelp( id )
{
#if ADVANCED_DEBUG
	log_function("public motd_ItemsHelp( id )");
#endif
    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new szFilename[64];
    format( szFilename, 63, "%s/help/items.html", WAR3X_DIR );

    show_motd( id, szFilename, WAR3X_MOTD_ITEMS );

    return PLUGIN_HANDLED;
}


public motd_SkillsHelp( id, iRaceNum ) {
#if ADVANCED_DEBUG
	log_function("public motd_SkillsHelp( id, iRaceNum ) {");
#endif

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new szFilename[64];
    format( szFilename, 63, "%s/help/%s.html", WAR3X_DIR, RACEKEYNAME[iRaceNum] );

    new szMOTDHeader[32];
    format( szMOTDHeader, 31, "%s %s", RACENAME_SHORT[iRaceNum], LANGUAGE_COMMON_SKILLS );

    show_motd( id, szFilename, szMOTDHeader );

    return PLUGIN_HANDLED;
}


// Player Skills

public motd_PlayerSkills( id ) {
#if ADVANCED_DEBUG
	log_function("public motd_PlayerSkills( id ) {");
#endif

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new szMotd[2048], iLen;

    new Players[32];
    new iTotalPlayers;

    // HTML Header

    iLen += format( szMotd[iLen], 2047 - iLen, HTML_HEADER );

    // Sort by race

    get_players( Players, iTotalPlayers );
    new bool:bRaceFound;

    for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
    {
        new iRaceTotal = 0;

        // First count number of players w/ race

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new pIndex = Players[iPlayerNum];

            if ( g_PlayerInfo[pIndex][CURRENT_RACE] - 1 == iRaceNum )
                iRaceTotal++;
        }

        // Run loop again to sort if players with race

        if ( iRaceTotal > 0 )
        {
            // Create header

            if ( bRaceFound )
                iLen += format( szMotd[iLen], 2047 - iLen, "^n^n^n" );

            iLen += format( szMotd[iLen], 2047 - iLen, "<h3>%s</h3>(%i players)^n", RACENAME[iRaceNum], iRaceTotal );
            bRaceFound = true;

            // Sort by Team

            for ( new iTeamNum = CS_TEAM_TERRORIST; iTeamNum <= CS_TEAM_CT; iTeamNum++ )
            {
                for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
                {
                    new pIndex = Players[iPlayerNum];

                    if ( get_user_team( pIndex ) == iTeamNum && g_PlayerInfo[pIndex][CURRENT_RACE] - 1 == iRaceNum )
                    {
                        // Add player

                        new szPlayerName[128], szTeamName[2];
                        get_user_name( pIndex, szPlayerName, 127 );

                        // Replace <>'s

                        replace( szPlayerName, 127, "<", "&lt;" );
                        replace( szPlayerName, 127, ">", "&gt;" );

                        copy( szTeamName, 1, CS_TEAM_NAME[iTeamNum] );

                        iLen += format( szMotd[iLen], 2047 - iLen, "^n<b class=%s>%s</b>", szTeamName, szPlayerName );

                        // Add player level

                        new iLevel = WAR3_get_level( g_PlayerInfo[pIndex][CURRENT_XP] );

                        new szClassName[64];
                        WAR3_race_info( pIndex, iRaceNum, RACEINFO_CLASSNAME, szClassName );

                        iLen += format( szMotd[iLen], 2047 - iLen, " (%i) %s", iLevel, szClassName );
                    }
                }
            }
        }
    }

    iLen += format( szMotd[iLen], 2047 - iLen, "^n^n " );

    show_motd( id, szMotd, WAR3X_MOTD_PLAYERSKILLS );

    return PLUGIN_HANDLED;
}


// Skills

public motd_Skills( id, targetId ) {
#if ADVANCED_DEBUG
	log_function("public motd_Skills( id, targetId ) {");
#endif

    new szMotd[2048], szData[64], szTitle[64], iLen;
    new szSkillDesc[256], szSkillName[32], szSkillLevel[16];

    new szPlayerName[32];
    get_user_name( targetId, szPlayerName, 31 );

    format( szTitle, 63, WAR3X_MOTD_TARGETSKILLS, szPlayerName );

    // HTML Header

    iLen += format( szMotd[iLen], 2047 - iLen, HTML_HEADER );

    // Make sure race is selected

    if ( !g_PlayerInfo[targetId][CURRENT_RACE] )
        iLen += format( szMotd[iLen], 2047 - iLen, INFO_FOLLOW_NORACE );

    else
    {
        // Race

        new raceId = g_PlayerInfo[targetId][CURRENT_RACE] - 1;
        new iLevel  = WAR3_get_level( g_PlayerInfo[targetId][CURRENT_XP] );

        iLen += format( szMotd[iLen], 2047 - iLen, "<h3>%s</h3>", RACENAME[raceId] );

        // Level

        iLen += format( szMotd[iLen], 2047 - iLen, "%s %i^n^n", LANGUAGE_COMMON_LEVEL, iLevel );

        // Classname

        WAR3_race_info( targetId, raceId, RACEINFO_CLASSNAME, szData );
        iLen += format( szMotd[iLen], 2047 - iLen, "<b>%s</b>: %s", LANGUAGE_COMMON_CLASS, szData );

        if ( !g_PlayerInfo[targetId][CURRENT_ULTIMATE] )
        {
            new szRankLevels[8];

            for ( new i = 2; i >= 0; i-- )
            {
                if ( iLevel >= GETCLASSCHECK[i] && !szRankLevels[0] )
                {
                    switch ( i )
                    {
                        case 0:     format( szRankLevels, 7, "0-%d", GETCLASSCHECK[i + 1] - 1 );
                        case 1:     format( szRankLevels, 7, "%d-%d", GETCLASSCHECK[i], GETCLASSCHECK[i + 1] - 1 );
                        case 2:     format( szRankLevels, 7, "%d%s", GETCLASSCHECK[i], "+" );
                    }
                }
            }

            iLen += format( szMotd[iLen], 2047 - iLen, " (%ss %s)", LANGUAGE_COMMON_LEVEL, szRankLevels );
        }

        iLen += format( szMotd[iLen], 2047 - iLen, "^n^n^n<i>%s</i>^n^n", MOTD_TS_CURRENTSKILLS );

        // Skills

        for ( new iSkillNum = 0; iSkillNum < TOTAL_SKILLS; iSkillNum++ )
        {
            // Racial Ability

            if ( iSkillNum == SKILL_RACIAL )
            {
                SkillHelp_Racial( raceId, szSkillDesc, iLevel );

                new Float:fRacialPercentage = ( ( float( iLevel ) + 1.0 ) / ( LEVEL_RACIALCAP + 1.0 ) ) * 100.0;

                if ( fRacialPercentage > 100.0 )
                    fRacialPercentage = 100.0;

                format( szSkillLevel, 15, "(%s %0.0f%s)", LANGUAGE_COMMON_RACIAL, fRacialPercentage, "%" );
            }

            // Trainable Skills

            else if ( g_PlayerInfo[targetId][iSkillNum + 1] )
            {
                new iSkillLvl = g_PlayerInfo[targetId][iSkillNum + 1];

                SkillHelp_Skills( raceId, iSkillNum, szSkillDesc, iSkillLvl );
                format( szSkillLevel, 15, "(%s %i)", LANGUAGE_COMMON_LEVEL, iSkillLvl );
            }

            else
            {
                format( szSkillDesc, 255, MOTD_TS_NOSKILL );
                format( szSkillLevel, 15, "" );
            }

            Get_SkillName( raceId, iSkillNum, szSkillName );
            iLen += format( szMotd[iLen], 2047 - iLen, "<b>%s</b> %s^n%s^n^n", szSkillName, szSkillLevel, szSkillDesc );
        }

        // Ultimate

        if ( g_PlayerInfo[targetId][CURRENT_ULTIMATE] )
        {
            iLen += format( szMotd[iLen], 2047 - iLen, "^n<i>%s</i>^n^n", MOTD_TS_CURRENTULTIMATE );

            new iUltimateNum = g_PlayerInfo[targetId][CURRENT_ULTIMATE] + TOTAL_SKILLS - 1;

            Get_SkillName( raceId, iUltimateNum, szSkillName );
            SkillHelp_Ultimate( raceId, iUltimateNum, szSkillDesc );

            iLen += format( szMotd[iLen], 2047 - iLen, "<b>%s</b>^n%s", szSkillName, szSkillDesc );
        }
    }

    // display a list of all the player's current levels, etc.

    if ( g_bXPfetched[targetId] )
    {
        iLen += format( szMotd[iLen], 2047 - iLen, "<i>%s</i>^n", MOTD_TS_ALLLEVELS );

        for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
        {
            if ( iRaceNum > 0 )
                iLen += format( szMotd[iLen], 2047 - iLen, ", " );

            iLen += format( szMotd[iLen], 2047 - iLen, "<b>%s</b> (%d)", RACENAME[iRaceNum], WAR3_get_level( g_iXPtotal[targetId][iRaceNum] ) );
        }
    }

    iLen += format( szMotd[iLen], 2047 - iLen, "^n^n " );

    show_motd( id, szMotd, szTitle );

    return PLUGIN_HANDLED;
}

// End of MOTD.INL