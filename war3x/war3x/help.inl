
// Begin MOTD.INL

/* - MOTD Windows ----------------------------------------------- */


public HELP_generate( id ) {

    // Generate files

    HELP_create_files();

    // Echo to id

    new szMessage[256];
    formatex( szMessage, 255, "%L", id, "INFO_GENERATEHELP_CHAT" );

    status_print( id, szMessage );

    return PLUGIN_HANDLED;
}



// Warcraft 3 Help

public HELP_motd_war3x( id ) {

    new szFilename[64];
    formatex( szFilename, 63, "%s/help/war3help.html", WAR3X_DIR );

    show_motd( id, szFilename, WAR3X_PLUGINNAME );

    return PLUGIN_HANDLED;
}


// Items Info

public HELP_motd_items( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new szFilename[64];
    formatex( szFilename, 63, "%s/help/items.html", WAR3X_DIR );

    new szHeader[64];
    formatex( szHeader, 63, "%L", id, "WAR3X_MOTD_ITEMS" );

    show_motd( id, szFilename, szHeader );

    return PLUGIN_HANDLED;
}


public HELP_motd_race( id, iRaceNum ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new szFilename[64];
    formatex( szFilename, 63, "%s/help/%s.html", WAR3X_DIR, RACEKEYNAME[iRaceNum] );

    new szMOTDHeader[32], szRaceName[32];
	LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31, true );

    formatex( szMOTDHeader, 31, "%s %L", szRaceName, id, "LANG_SKILLS" );

    show_motd( id, szFilename, szMOTDHeader );

    return PLUGIN_HANDLED;
}


// Player Skills

public HELP_motd_players( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new szMotd[2048], iLen;

    new Players[32];
    new iTotalPlayers;

    // HTML Header

    iLen += formatex( szMotd[iLen], 2047 - iLen, HTML_HEADER );

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
                iLen += formatex( szMotd[iLen], 2047 - iLen, "^n^n^n" );

			new szRaceName[32];
			LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31 );

            iLen += formatex( szMotd[iLen], 2047 - iLen, "<h3>%s</h3>(%i players)^n", szRaceName, iRaceTotal );
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

                        iLen += formatex( szMotd[iLen], 2047 - iLen, "^n<b class=%s>%s</b>", szTeamName, szPlayerName );

                        // Add player level

                        new iLevel = WAR3_get_level( g_PlayerInfo[pIndex][CURRENT_XP] );

                        new szClassName[64];
                        WAR3_race_info( pIndex, iRaceNum, RACEINFO_CLASSNAME, szClassName );

                        iLen += formatex( szMotd[iLen], 2047 - iLen, " (%i) %s", iLevel, szClassName );
                    }
                }
            }
        }
    }

    iLen += formatex( szMotd[iLen], 2047 - iLen, "^n^n " );

    new szHeader[64];
    formatex( szHeader, 63, "%L", id, "WAR3X_MOTD_PLAYERSKILLS" );

    show_motd( id, szMotd, szHeader );

    return PLUGIN_HANDLED;
}


// Skills

public HELP_motd_target( id, targetId ) {

    new szMotd[2048], szData[64], szHeader[64], iLen;
    new szSkillDesc[256], szSkillName[32], szSkillLevel[16];

    new szPlayerName[32];
    get_user_name( targetId, szPlayerName, 31 );

    formatex( szHeader, 63, "%L ( %s )", id, "WAR3X_MOTD_PLAYERSKILLS", szPlayerName );

    // HTML Header

    iLen += formatex( szMotd[iLen], 2047 - iLen, HTML_HEADER );

    // Make sure race is selected

    if ( !g_PlayerInfo[targetId][CURRENT_RACE] )
        iLen += formatex( szMotd[iLen], 2047 - iLen, "%L", id, "INFO_FOLLOW_NORACE" );

    else
    {
        // Race

        new raceId = g_PlayerInfo[targetId][CURRENT_RACE] - 1;
        new iLevel  = WAR3_get_level( g_PlayerInfo[targetId][CURRENT_XP] );

		new szRaceName[32];
		LANG_GetRaceName( raceId + 1, id, szRaceName, 31 );
        iLen += formatex( szMotd[iLen], 2047 - iLen, "<h3>%s</h3>", szRaceName );

        // Level

        iLen += formatex( szMotd[iLen], 2047 - iLen, "%L %i^n^n", id, "LANG_LEVEL", iLevel );

        // Classname

        WAR3_race_info( targetId, raceId, RACEINFO_CLASSNAME, szData );
        iLen += formatex( szMotd[iLen], 2047 - iLen, "<b>%L</b>: %s", id, "LANG_CLASS", szData );

        if ( !g_PlayerInfo[targetId][CURRENT_ULTIMATE] )
        {
            new szRankLevels[8];

            for ( new i = 2; i >= 0; i-- )
            {
                if ( iLevel >= GETCLASSCHECK[i] && !szRankLevels[0] )
                {
                    switch ( i )
                    {
                        case 0:     formatex( szRankLevels, 7, "0-%d", GETCLASSCHECK[i + 1] - 1 );
                        case 1:     formatex( szRankLevels, 7, "%d-%d", GETCLASSCHECK[i], GETCLASSCHECK[i + 1] - 1 );
                        case 2:     formatex( szRankLevels, 7, "%d%s", GETCLASSCHECK[i], "+" );
                    }
                }
            }

            iLen += formatex( szMotd[iLen], 2047 - iLen, " (%Ls %s)", id, "LANG_LEVEL", szRankLevels );
        }

        iLen += formatex( szMotd[iLen], 2047 - iLen, "^n^n^n<i>%L</i>^n^n", id, "MOTD_PS_CURRENTSKILLS" );

        // Skills

        for ( new iSkillNum = 0; iSkillNum < TOTAL_SKILLS; iSkillNum++ )
        {
            // Racial Ability

            if ( iSkillNum == SKILL_RACIAL )
            {
                HELP_description_racial( raceId, id, szSkillDesc, iLevel );

                new Float:fRacialPercentage = ( ( float( iLevel ) + 1.0 ) / ( LEVEL_RACIALCAP + 1.0 ) ) * 100.0;

                if ( fRacialPercentage > 100.0 )
                    fRacialPercentage = 100.0;

                format( szSkillLevel, 15, "(%L %0.0f%%)", id, "LANG_RACIAL", fRacialPercentage );
            }

            // Trainable Skills

            else if ( g_PlayerInfo[targetId][iSkillNum + 1] )
            {
                new iSkillLvl = g_PlayerInfo[targetId][iSkillNum + 1];

                HELP_description_trained( raceId, iSkillNum, id, szSkillDesc, iSkillLvl );
                formatex( szSkillLevel, 15, "(%L %i)", id, "LANG_LEVEL", iSkillLvl );
            }

            else
            {
                formatex( szSkillDesc, 255, "%L", id, "MOTD_PS_NOSKILL" );
                formatex( szSkillLevel, 15, "" );
            }

            Get_SkillName( raceId, iSkillNum, id, szSkillName );
            iLen += formatex( szMotd[iLen], 2047 - iLen, "<b>%s</b> %s^n%s^n^n", szSkillName, szSkillLevel, szSkillDesc );
        }

        // Ultimate

        if ( g_PlayerInfo[targetId][CURRENT_ULTIMATE] )
        {
            iLen += formatex( szMotd[iLen], 2047 - iLen, "^n<i>%L</i>^n^n", id, "MOTD_PS_CURRENTULTIMATE" );

            new iUltimateNum = g_PlayerInfo[targetId][CURRENT_ULTIMATE] + TOTAL_SKILLS - 1;

            Get_SkillName( raceId, iUltimateNum, id, szSkillName );
            HELP_description_ultimate( raceId, iUltimateNum, id, szSkillDesc );

            iLen += formatex( szMotd[iLen], 2047 - iLen, "<b>%s</b>^n%s", szSkillName, szSkillDesc );
        }
    }

    // display a list of all the player's current levels, etc.

    if ( g_bXPfetched[targetId] )
    {
        iLen += formatex( szMotd[iLen], 2047 - iLen, "<i>%L</i>^n", id, "MOTD_PS_ALLLEVELS" );

        for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
        {
            if ( iRaceNum > 0 )
                iLen += formatex( szMotd[iLen], 2047 - iLen, ", " );

			new szRaceName[32];
			LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31 );
            iLen += formatex( szMotd[iLen], 2047 - iLen, "<b>%s</b> (%d)", szRaceName, WAR3_get_level( g_iXPtotal[targetId][iRaceNum] ) );
        }
    }

    iLen += formatex( szMotd[iLen], 2047 - iLen, "^n^n " );

    show_motd( id, szMotd, szHeader );

    return PLUGIN_HANDLED;
}


// Item Descriptions ( returns number of parameters )

public HELP_get_values_items( iItemNum, szValue[32] ) {

    switch ( iItemNum )
    {
        // Items

        case ITEM_ANKH:
        {
            formatex( szValue, 31, "%0.0f%%", ( ITEM_ANKH_VALUE * 100.0 ) );
            return ( 1 );
        }

        case ITEM_BOOTS:
        {
            formatex( szValue, 31, "%0.0f%%", ( ( ( ITEM_BOOTS_VALUE / SPEED_KNIFE ) - 1.0 ) * 100.0 ) );
            return ( 1 );
        }

        case ITEM_CLAWS:
        {
            formatex( szValue, 31, "%i", ITEM_CLAWS_VALUE );
            return ( 1 );
        }

        case ITEM_CLOAK:
        {
            formatex( szValue, 31, "%0.0f%%", ( ITEM_CLOAK_VALUE * 100.0 ) );
            return ( 1 );
        }

        case ITEM_MASK:
        {
            formatex( szValue, 31, "%0.0f", ITEM_MASK_VALUE );
            return ( 1 );
        }

        case ITEM_AMULET:
        {
            formatex( szValue, 31, "%d", ITEM_AMULET_VALUE );
            return ( 1 );
        }

        case ITEM_RING:
        {
            formatex( szValue, 31, "%i", ITEM_RING_VALUE );
            return ( 1 );
        }

        case ITEM_TOME:
        {
            formatex( szValue, 31, "%i", ITEM_TOME_VALUE );
            return ( 1 );
        }
    }

    return PLUGIN_HANDLED;
}


// Skill Descriptions ( returns number of parameters )

public HELP_get_values_skills( iRaceId, iSkillNum, iSkillLevel, szValue[32] ) {

    switch ( iRaceId + 1 )
    {
        case RACE_UNDEAD:
        {
            switch ( iSkillNum )
            {
                // Skills

                case SKILL_RACIAL:
                {
                    formatex( szValue, 31, "%0.0f%% %0.0f%%", ( ( ( UD_S_UNHOLY_get_speed( iSkillLevel ) - SPEED_KNIFE ) / SPEED_KNIFE ) * 100.0 ), ( 1.0 - UD_S_UNHOLY_get_gravity( iSkillLevel ) ) * 100.0 );
                    return ( 2 );
                }

                case SKILL_1:
                {
                    formatex( szValue, 31, "%0.0f%%", ( UD_fVampiricAura[iSkillLevel] * 100.0 ) );
                    return ( 1 );
                }

                case SKILL_2:
                {
                    formatex( szValue, 31, "%d %d %0.0f", UD_iFrostNova_range[iSkillLevel], UD_iFrostNova_damage[iSkillLevel], FROSTNOVA_SLOWDURATION_MAX );
                    return ( 3 );
                }

                case SKILL_3:
                {
                    formatex( szValue, 31, "%d %0.0f%% %0.1f", ( UD_iFrostArmor_armor[iSkillLevel] - 100 ), ( UD_fFrostArmor_slow[iSkillLevel] * 100.0 ), FROSTARMOR_SLOWDURATION );
                    return ( 3 );
                }

                // Ultimates

                case ULTIMATE_1:
                {
                    formatex( szValue, 31, "%d %d", DEATHCOIL_DAMAGE, DEATHCOIL_HEAL );
                    return ( 2 );
                }

                case ULTIMATE_2:
                {
                    formatex( szValue, 31, "%d", IMPALE_DAMAGE );
                    return ( 1 );
                }

                case ULTIMATE_3:
                {
                    formatex( szValue, 31, "%0.1f", SLEEP_DURATION_MAX );
                    return ( 1 );
                }
            }
        }

        case RACE_HUMAN:
        {
            switch ( iSkillNum )
            {
                // Skills

                case SKILL_RACIAL:
                {
                    formatex( szValue, 31, "%d", HU_S_FORTITUDE_get( iSkillLevel ) );
                    return ( 1 );
                }

                case SKILL_1:
                {
                    formatex( szValue, 31, "%0.0f%%", ( HU_fInvisibility[iSkillLevel] * 100.0 ) );
                    return ( 2 );
                }

                case SKILL_2:
                {
                    formatex( szValue, 31, "%0.0f%% %d %0.1f", ( HU_fBash[iSkillLevel] * 100.0 ), HU_iBashDamage[iSkillLevel], BASH_DURATION_MAX );
                    return ( 3 );
                }

                case SKILL_3:
                {
                    formatex( szValue, 31, "%d %0.0f%%", ( HU_iInnerFire_armor[iSkillLevel] - 100 ), ( HU_fInnerFire_damage[iSkillLevel] * 100.0 ) );
                    return ( 2 );
                }

                // Ultimates

                case ULTIMATE_1:
                {
                    formatex( szValue, 31, "%d %d", HOLYLIGHT_HEAL, HOLYLIGHT_DAMAGE );
                    return ( 2 );
                }

                case ULTIMATE_2:
                {
                    formatex( szValue, 31, "%d %d %d %d", FLAMESTRIKE_DAMAGEIN, FLAMESTRIKE_DAMAGEOUT, FLAMESTRIKE_SINGEDURATION, FLAMESTRIKE_DURATION );
                    return ( 4 );
                }

                case ULTIMATE_3:
                {
                    formatex( szValue, 31, "%i %d %0.0f", AVATAR_HEALTH, AVATAR_DAMAGE, AVATAR_DURATION );
                    return ( 3 );
                }
            }
        }

        case RACE_ORC:
        {
            switch ( iSkillNum )
            {
                // Skills

                case SKILL_RACIAL:
                {
                    formatex( szValue, 31, "%d %0.1f", REGENERATION_AMOUNT, OR_S_REGENERATION_get( iSkillLevel ) );
                    return ( 2 );
                }

                case SKILL_1:
                {
                    formatex( szValue, 31, "%0.0f%% %d %0.0f%%", ( ( ( OR_fBerserk_speed[iSkillLevel] - SPEED_KNIFE ) / SPEED_KNIFE ) * 100.0 ), BERSERK_HEALTH, ( OR_fBerserk_damage[iSkillLevel] * 100.0 ) );
                    return ( 3 );
                }

                case SKILL_2:
                {
                    formatex( szValue, 31, "%0.0f%% %0.0f", ( OR_fPulverize_bonus[iSkillLevel] * 100.0 ), OR_fPulverize_range[iSkillLevel] );
                    return ( 2 );
                }

                case SKILL_3:
                {
                    formatex( szValue, 31, "%0.0f%%", ( OR_fPillage[iSkillLevel] * 100.0 ) );
                    return ( 1 );
                }

                // Ultimates

                case ULTIMATE_1:
                {
                    formatex( szValue, 31, "%d %d %0.0f%%", HEALINGWAVE_JUMPS, HEALINGWAVE_HEAL, ( ( 1.0 / float( HEALINGWAVE_MULTIPLIER ) ) * 100.0 ) );
                    return ( 3 );
                }

                case ULTIMATE_2:
                {
                    formatex( szValue, 31, "%d %d %0.0f%%", CHAINLIGHTNING_DAMAGE, CHAINLIGHTNING_JUMPS, ( ( 1.0 / float( CHAINLIGHTNING_MULTIPLIER ) ) * 100.0 ) );
                    return ( 3 );
                }

                case ULTIMATE_3:
                {
                    formatex( szValue, 31, "%0.0f%% %0.0f%% %0.1f %d", ( ( ( 255.0 - WINDWALK_INVISIBILITY ) / 255.0 ) * 100.0 ), ( ( ( WINDWALK_SPEED - SPEED_KNIFE ) / 255.0 ) * 100.0 ), float( WINDWALK_DURATION ), WINDWALK_DAMAGE );
                    return ( 4 );
                }
            }
        }

        case RACE_NIGHTELF:
        {
            switch ( iSkillNum )
            {
                // Skills

                case SKILL_RACIAL:
                {
                    formatex( szValue, 31, "%0.0f%% %0.0f%%", ( NE_S_ELUNES_get_knife( iSkillLevel ) * 100.0 ), ( NE_S_ELUNES_get_magic( iSkillLevel ) * 100.0 ) );
                    return ( 2 );
                }

                case SKILL_1:
                {
                    formatex( szValue, 31, "%0.0f%%", ( NE_fEvasion[iSkillLevel] * 100.0 ) );
                    return ( 1 );
                }

                case SKILL_2:
                {
                    formatex( szValue, 31, "%d %0.0f%%", ( NE_iBlessing_armor[iSkillLevel] - 100 ), ( ( NE_fBlessing_speed[iSkillLevel] ) * 100.0 ) );
                    return ( 2 );
                }

                case SKILL_3:
                {
                    formatex( szValue, 31, "%0.0f%%", ( NE_fTrueshotAura[iSkillLevel] * 100.0 ) );
                    return ( 1 );
                }

                // Ultimates

                case ULTIMATE_1:
                {
                    formatex( szValue, 31, "%d %0.1f", REJUVENATION_MAXHEALTH, REJUVENATION_DURATION );
                    return ( 2 );
                }

                case ULTIMATE_2:
                {
                    formatex( szValue, 31, "%0.1f %d", ROOT_DURATION, ROOT_MAXDAMAGE );
                    return ( 2 );
                }

                case ULTIMATE_3:
                {
                    formatex( szValue, 31, "%d %d %0.1f %0.0f%%", SHADOWSTRIKE_DAMAGE, SHADOWSTRIKE_DOT, float( SHADOWSTRIKE_DURATION ), ( ( ( SPEED_KNIFE - SHADOWSTRIKE_SPEED ) / 255.0 ) * 100.0 ) );
                    return ( 4 );
                }
            }
        }
    }

    return PLUGIN_HANDLED;
}


// omg this crap is so sloppy right now. oh well.

public HELP_create_files() {

    new szFileName[64], szText[256];

    // Skill/Ultimate Help

    for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
    {
        new szSkillDesc[256], szSkillName[32], szSkillHeader[192];

        // Set Filename

        formatex( szFileName, 63, "%s/help/%s.html", WAR3X_DIR, RACEKEYNAME[iRaceNum] );

        // Delete old file(s)

        delete_file( szFileName );

        // Html Header

        write_file( szFileName, HTML_HEADER, -1 );

        // Skills

        for ( new iSkillNum = 0; iSkillNum < TOTAL_SKILLS; iSkillNum++ )
        {
            // Racial Ability

            if ( iSkillNum == SKILL_RACIAL )
            {
                formatex( szSkillHeader, 191, "%L", LANG_SERVER, "HELP_HEADER_RACIAL" );
                write_file( szFileName, szSkillHeader, -1 );

                HELP_description_racial( iRaceNum, LANG_SERVER, szSkillDesc, LEVEL_ALL );
            }

            // Trainable Skills

            else
            {
                if ( iSkillNum == SKILL_1 )
                {
                    formatex( szSkillHeader, 191, "^n%L", LANG_SERVER, "HELP_HEADER_SKILLS" );
                    write_file( szFileName, szSkillHeader, -1 );
                }

                HELP_description_trained( iRaceNum, iSkillNum, LANG_SERVER, szSkillDesc, LEVEL_ALL );
            }

            Get_SkillName( iRaceNum, iSkillNum, LANG_SERVER, szSkillName );

            formatex( szText, 255, "<li><b>%s</b>^n%s", szSkillName, szSkillDesc );

            write_file( szFileName, szText, -1 );
        }

        // Ultimates

        for ( new iUltimateNum = TOTAL_SKILLS; iUltimateNum < TOTAL_SKILLS + TOTAL_ULTIMATES; iUltimateNum++ )
        {
            if ( iUltimateNum == ULTIMATE_1 )
            {
                new szUltimate[192];
                formatex( szUltimate, 191, "%L", LANG_SERVER, "HELP_HEADER_ULTIMATES", LEVEL_ULTIMATE );

                formatex( szSkillHeader, 191, "^n%s", szUltimate );
                write_file( szFileName, szSkillHeader, -1 );
            }

            Get_SkillName( iRaceNum, iUltimateNum, LANG_SERVER, szSkillName );
            HELP_description_ultimate( iRaceNum, iUltimateNum, LANG_SERVER, szSkillDesc );

            formatex( szText, 255, "<li><b>%s</b>^n%s", szSkillName, szSkillDesc );
            write_file( szFileName, szText, -1 );
        }
    }

    // Item Help

    new szItemDesc[256], szItemName[32], szItemHeader[256];

    // Set Filename

    formatex( szFileName, 63, "%s/help/items.html", WAR3X_DIR );

    // Delete old file(s)

    delete_file( szFileName );

    // Html Header

    write_file( szFileName, HTML_HEADER, -1 );

    formatex( szItemHeader, 255, "%L", LANG_SERVER, "HELP_HEADER_ITEMS" );
    write_file( szFileName, szItemHeader, -1 );

    // Grab Values and Fill HTML

    for ( new iItemNum = 1; iItemNum <= TOTAL_SHOPITEMS; iItemNum++ )
    {
		LANG_GetItemName ( iItemNum, SHOP_COMMON, LANG_SERVER, szItemName, 31 )
		LANG_GetItemDesc ( iItemNum, SHOP_COMMON, LANG_SERVER, szItemDesc, 255 )

        // Grab values

        new szValues[32], szValueArgs[5][8];
        HELP_get_values_items( iItemNum, szValues );

        // Currently support for only 5 args

        parse( szValues, szValueArgs[0], 7, szValueArgs[1], 7, szValueArgs[2], 7, szValueArgs[3], 7, szValueArgs[4], 7 );

        // Create String ( Max 5 args )

        formatex( szItemDesc, 255, szItemDesc, szValueArgs[0], szValueArgs[1], szValueArgs[2], szValueArgs[3], szValueArgs[4] );

        formatex( szText, 255, "<li><b><font color=rgb(%i,%i,%i)>%s</font></b>^n%s", ITEM_COLOR[iItemNum][GLOW_R], ITEM_COLOR[iItemNum][GLOW_G], ITEM_COLOR[iItemNum][GLOW_B], szItemName, szItemDesc );
        write_file( szFileName, szText, -1 );
    }

    return PLUGIN_HANDLED;
}


static HELP_description_racial( iRaceNum, id, szDescription[256], iLevel ) {

    // Grab skill description template

	LANG_GetSkillDesc( iRaceNum + 1, 1, SKILL_T_RACIAL, id, szDescription, 255 )

    // Grab values

    new szMinValues[32], szMaxValues[32];
    new szMinValueArgs[5][8], szMaxValueArgs[5][8], szNewValues[5][16];

    new iTotalArgs;

    if ( iLevel == LEVEL_ALL )
    {
        iTotalArgs = HELP_get_values_skills( iRaceNum, SKILL_RACIAL, 0, szMinValues );
        iTotalArgs = HELP_get_values_skills( iRaceNum, SKILL_RACIAL, 9, szMaxValues );
    }

    else
    {
        iTotalArgs = HELP_get_values_skills( iRaceNum, SKILL_RACIAL, iLevel, szMinValues );
        iTotalArgs = HELP_get_values_skills( iRaceNum, SKILL_RACIAL, iLevel, szMaxValues );
    }

    // Currently support for only 5 args

    parse( szMinValues, szMinValueArgs[0], 7, szMinValueArgs[1], 7, szMinValueArgs[2], 7, szMinValueArgs[3], 7, szMinValueArgs[4], 7 );
    parse( szMaxValues, szMaxValueArgs[0], 7, szMaxValueArgs[1], 7, szMaxValueArgs[2], 7, szMaxValueArgs[3], 7, szMaxValueArgs[4], 7 );

    for ( new i = 0; i < iTotalArgs; i++ )
    {
        new szTempMin[8], szTempMax[8];
        copy( szTempMin, 7, szMinValueArgs[i] );
        copy( szTempMax, 7, szMaxValueArgs[i] );

        if ( equal( szTempMin, szTempMax ) )
            copy( szNewValues[i], 15, szTempMin );

        else
        {
            // Combine args into one

            formatex( szNewValues[i], 15, "%s-%s", szTempMin, szTempMax );
        }
    }

    // Create String ( Max 5 args )

    format( szDescription, 255, szDescription, szNewValues[0], szNewValues[1], szNewValues[2], szNewValues[3], szNewValues[4] );

    return PLUGIN_HANDLED;

}


static HELP_description_trained( iRaceNum, iSkillNum, id, szDescription[256], iLevel ) {

    // Grab skill description template

	LANG_GetSkillDesc( iRaceNum + 1, iSkillNum + 1, SKILL_T_TRAINED, id, szDescription, 255 )

    // Grab values

    new szValues[_TST][32];
    new szLvl1Args[5][8], szLvl2Args[5][8], szLvl3Args[5][8];
    new szNewValues[5][16];

    new iTotalArgs;

    for ( new i = 0; i < TOTAL_SKILLSTRAINED; i++ )
    {
        if ( iLevel == LEVEL_ALL )
        {
            iTotalArgs = HELP_get_values_skills( iRaceNum, iSkillNum, i, szValues[i] );
        }

        else
        {
            iTotalArgs = HELP_get_values_skills( iRaceNum, iSkillNum, iLevel - 1, szValues[i] );
        }
    }

    // Currently support for only 5 args

    parse( szValues[0], szLvl1Args[0], 7, szLvl1Args[1], 7, szLvl1Args[2], 7, szLvl1Args[3], 7, szLvl1Args[4], 7 );
    parse( szValues[1], szLvl2Args[0], 7, szLvl2Args[1], 7, szLvl2Args[2], 7, szLvl2Args[3], 7, szLvl2Args[4], 7 );
    parse( szValues[2], szLvl3Args[0], 7, szLvl3Args[1], 7, szLvl3Args[2], 7, szLvl3Args[3], 7, szLvl3Args[4], 7 );

    for ( new i = 0; i < iTotalArgs; i++ )
    {
        new szTempArgs[_TST][8];

        copy( szTempArgs[0], 7, szLvl1Args[i] );
        copy( szTempArgs[1], 7, szLvl2Args[i] );
        copy( szTempArgs[2], 7, szLvl3Args[i] );

        if ( equal( szTempArgs[0], szTempArgs[1] ) && equal( szTempArgs[1], szTempArgs[2] ) )
            copy( szNewValues[i], 15, szTempArgs[0] );

        else
        {
            // Combine args into one

            formatex( szNewValues[i], 15, "%s/%s/%s", szTempArgs[0], szTempArgs[1], szTempArgs[2] );
        }
    }

    // Create String ( Max 5 args )

    format( szDescription, 255, szDescription, szNewValues[0], szNewValues[1], szNewValues[2], szNewValues[3], szNewValues[4] );

    return PLUGIN_HANDLED;

}


static HELP_description_ultimate( iRaceNum, iUltimateNum, id, szDescription[256] ) {

    // Grab skill description template

	LANG_GetSkillDesc( iRaceNum + 1, iUltimateNum + 1, SKILL_T_ULTIMATE, id, szDescription, 255 )

    // Grab values

    new szValues[32];
    new szValueArgs[5][8], szNewValues[5][16];

    new iTotalArgs = HELP_get_values_skills( iRaceNum, iUltimateNum, 0, szValues );

    // Currently support for only 5 args

    parse( szValues, szValueArgs[0], 7, szValueArgs[1], 7, szValueArgs[2], 7, szValueArgs[3], 7, szValueArgs[4], 7 );

    for ( new i = 0; i < iTotalArgs; i++ )
    {
        copy( szNewValues[i], 15, szValueArgs[i] );
    }

    // Create String ( Max 5 args )

    format( szDescription, 255, szDescription, szNewValues[0], szNewValues[1], szNewValues[2], szNewValues[3], szNewValues[4] );

    return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. - //
