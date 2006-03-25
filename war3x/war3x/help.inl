
// Begin MOTD.INL

/* - MOTD Windows ----------------------------------------------- */


public HELP_generate( id ) {

    // Generate files

    HELP_create_files();

    // Echo to id

    new szMessage[256];
    copy( szMessage, 255, INFO_GENERATEHELP_CHAT );

    status_print( id, szMessage );

    return PLUGIN_HANDLED;
}



// Warcraft 3 Help

public HELP_motd_war3x( id ) {

    new szFilename[64];
    format( szFilename, 63, "%s/help/war3help.html", WAR3X_DIR );

    show_motd( id, szFilename, WAR3X_MOTD_HELP );

    return PLUGIN_HANDLED;
}


// Items Info

public HELP_motd_items( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new szFilename[64];
    format( szFilename, 63, "%s/help/items.html", WAR3X_DIR );

    show_motd( id, szFilename, WAR3X_MOTD_ITEMS );

    return PLUGIN_HANDLED;
}


public HELP_motd_race( id, iRaceNum ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new szFilename[64];
    format( szFilename, 63, "%s/help/%s.html", WAR3X_DIR, RACEKEYNAME[iRaceNum] );

    new szMOTDHeader[32], szRaceName[32];
	LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31, true );

    format( szMOTDHeader, 31, "%s %s", szRaceName, LANGUAGE_COMMON_SKILLS );

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
			
			new szRaceName[32];
			LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31 );

            iLen += format( szMotd[iLen], 2047 - iLen, "<h3>%s</h3>(%i players)^n", szRaceName, iRaceTotal );
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

public HELP_motd_target( id, targetId ) {

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

        new raceId = g_PlayerInfo[targetId][CURRENT_RACE];
        new iLevel  = WAR3_get_level( g_PlayerInfo[targetId][CURRENT_XP] );

		new szRaceName[32];
		LANG_GetRaceName( raceId, id, szRaceName, 31 );
        iLen += format( szMotd[iLen], 2047 - iLen, "<h3>%s</h3>", szRaceName );

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
                HELP_description_racial( raceId, id, szSkillDesc, iLevel );

                new Float:fRacialPercentage = ( ( float( iLevel ) + 1.0 ) / ( LEVEL_RACIALCAP + 1.0 ) ) * 100.0;

                if ( fRacialPercentage > 100.0 )
                    fRacialPercentage = 100.0;

                format( szSkillLevel, 15, "(%s %0.0f%s)", LANGUAGE_COMMON_RACIAL, fRacialPercentage, "%" );
            }

            // Trainable Skills

            else if ( g_PlayerInfo[targetId][iSkillNum + 1] )
            {
                new iSkillLvl = g_PlayerInfo[targetId][iSkillNum + 1];

                HELP_description_trained( raceId, iSkillNum, id, szSkillDesc, iSkillLvl );
                format( szSkillLevel, 15, "(%s %i)", LANGUAGE_COMMON_LEVEL, iSkillLvl );
            }

            else
            {
                format( szSkillDesc, 255, MOTD_TS_NOSKILL );
                format( szSkillLevel, 15, "" );
            }

            Get_SkillName( raceId, iSkillNum, id, szSkillName );
            iLen += format( szMotd[iLen], 2047 - iLen, "<b>%s</b> %s^n%s^n^n", szSkillName, szSkillLevel, szSkillDesc );
        }

        // Ultimate

        if ( g_PlayerInfo[targetId][CURRENT_ULTIMATE] )
        {
            iLen += format( szMotd[iLen], 2047 - iLen, "^n<i>%s</i>^n^n", MOTD_TS_CURRENTULTIMATE );

            new iUltimateNum = g_PlayerInfo[targetId][CURRENT_ULTIMATE] + TOTAL_SKILLS - 1;

            Get_SkillName( raceId, iUltimateNum, id, szSkillName );
            HELP_description_ultimate( raceId, iUltimateNum, id, szSkillDesc );

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

			new szRaceName[32];
			LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31 );
            iLen += format( szMotd[iLen], 2047 - iLen, "<b>%s</b> (%d)", szRaceName, WAR3_get_level( g_iXPtotal[targetId][iRaceNum] ) );
        }
    }

    iLen += format( szMotd[iLen], 2047 - iLen, "^n^n " );

    show_motd( id, szMotd, szTitle );

    return PLUGIN_HANDLED;
}


// Item Descriptions ( returns number of parameters )

public HELP_get_values_items( iItemNum, szValue[32] ) {

    switch ( iItemNum )
    {
        // Items

        case ITEM_ANKH:
        {
            format( szValue, 31, "%0.0f%s", ( VALUE_ANKH * 100.0 ), "%" );
            return ( 1 );
        }

        case ITEM_BOOTS:
        {
            format( szValue, 31, "%0.0f%s", ( ( ( VALUE_BOOTS / SPEED_KNIFE ) - 1.0 ) * 100.0 ), "%" );
            return ( 1 );
        }

        case ITEM_CLAWS:
        {
            format( szValue, 31, "%i", VALUE_CLAWS );
            return ( 1 );
        }

        case ITEM_CLOAK:
        {
            format( szValue, 31, "%0.0f%s", ( VALUE_CLOAK * 100.0 ), "%" );
            return ( 1 );
        }

        case ITEM_MASK:
        {
            format( szValue, 31, "%0.0f", VALUE_MASK );
            return ( 1 );
        }

        case ITEM_AMULET:
        {
            format( szValue, 31, "%d", VALUE_AMULET );
            return ( 1 );
        }

        case ITEM_RING:
        {
            format( szValue, 31, "%i", VALUE_RING );
            return ( 1 );
        }

        case ITEM_TOME:
        {
            format( szValue, 31, "%i", VALUE_TOME );
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
                    format( szValue, 31, "%0.0f%s %0.0f%s", ( ( ( UD_S_UNHOLY_get_speed( iSkillLevel ) - SPEED_KNIFE ) / SPEED_KNIFE ) * 100.0 ), "%", ( 1.0 - UD_S_UNHOLY_get_gravity( iSkillLevel ) ) * 100.0, "%" );
                    return ( 2 );
                }

                case SKILL_1:
                {
                    format( szValue, 31, "%0.0f%s", ( UD_fVampiricAura[iSkillLevel] * 100.0 ), "%" );
                    return ( 1 );
                }

                case SKILL_2:
                {
                    format( szValue, 31, "%d %d %0.0f", UD_iFrostNova_range[iSkillLevel], UD_iFrostNova_damage[iSkillLevel], FROSTNOVA_SLOWDURATION_MAX );
                    return ( 3 );
                }

                case SKILL_3:
                {
                    format( szValue, 31, "%d %0.0f%s %0.1f", ( UD_iFrostArmor_armor[iSkillLevel] - 100 ), ( UD_fFrostArmor_slow[iSkillLevel] * 100.0 ), "%", FROSTARMOR_SLOWDURATION );
                    return ( 3 );
                }

                // Ultimates

                case ULTIMATE_1:
                {
                    format( szValue, 31, "%d %d", DEATHCOIL_DAMAGE, DEATHCOIL_HEAL );
                    return ( 2 );
                }

                case ULTIMATE_2:
                {
                    format( szValue, 31, "%d", IMPALE_DAMAGE );
                    return ( 1 );
                }

                case ULTIMATE_3:
                {
                    format( szValue, 31, "%0.1f", SLEEP_DURATION_MAX );
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
                    format( szValue, 31, "%d", HU_S_FORTITUDE_get( iSkillLevel ) );
                    return ( 1 );
                }

                case SKILL_1:
                {
                    format( szValue, 31, "%0.0f%s", ( s_Invisibility[iSkillLevel] * 100.0 ), "%" );
                    return ( 2 );
                }

                case SKILL_2:
                {
                    format( szValue, 31, "%0.0f%s %d %0.1f", ( s_Bash[iSkillLevel] * 100.0 ), "%", s_BashDamage[iSkillLevel], BASH_DURATION_MAX );
                    return ( 3 );
                }

                case SKILL_3:
                {
                    format( szValue, 31, "%d %0.0f%s", ( s_ifArmor[iSkillLevel] - 100 ), ( s_ifDamage[iSkillLevel] * 100.0 ), "%" );
                    return ( 2 );
                }

                // Ultimates

                case ULTIMATE_1:
                {
                    format( szValue, 31, "%d %d", HOLYLIGHT_HEAL, HOLYLIGHT_DAMAGE );
                    return ( 2 );
                }

                case ULTIMATE_2:
                {
                    format( szValue, 31, "%d %d %d %d", FLAMESTRIKE_DAMAGEIN, FLAMESTRIKE_DAMAGEOUT, FLAMESTRIKE_SINGEDURATION, FLAMESTRIKE_DURATION );
                    return ( 4 );
                }

                case ULTIMATE_3:
                {
                    format( szValue, 31, "%i %d %0.0f", AVATAR_HEALTH, AVATAR_DAMAGE, AVATAR_DURATION );
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
                    format( szValue, 31, "%d %0.1f", REGENERATION_AMMOUNT, OR_S_REGENERATION_get( iSkillLevel ) );
                    return ( 2 );
                }

                case SKILL_1:
                {
                    format( szValue, 31, "%0.0f%s %d %0.0f%s", ( ( ( s_BerserkSpeed[iSkillLevel] - SPEED_KNIFE ) / SPEED_KNIFE ) * 100.0 ), "%", BERSERK_HEALTH, ( s_BerserkDmg[iSkillLevel] * 100.0 ), "%" );
                    return ( 3 );
                }

                case SKILL_2:
                {
                    format( szValue, 31, "%0.0f%s %0.0f", ( s_PulverizeBonus[iSkillLevel] * 100.0 ), "%", s_PulverizeRange[iSkillLevel] );
                    return ( 2 );
                }

                case SKILL_3:
                {
                    format( szValue, 31, "%0.0f%s", ( s_Pillage[iSkillLevel] * 100.0 ), "%" );
                    return ( 1 );
                }

                // Ultimates

                case ULTIMATE_1:
                {
                    format( szValue, 31, "%d %d %0.0f%s", HEALINGWAVE_JUMPS, HEALINGWAVE_HEAL, ( ( 1.0 / float( HEALINGWAVE_MULTIPLIER ) ) * 100.0 ), "%" );
                    return ( 3 );
                }

                case ULTIMATE_2:
                {
                    format( szValue, 31, "%d %d %0.0f%s", CHAINLIGHTNING_DAMAGE, CHAINLIGHTNING_JUMPS, ( ( 1.0 / float( CHAINLIGHTNING_MULTIPLIER ) ) * 100.0 ), "%" );
                    return ( 3 );
                }

                case ULTIMATE_3:
                {
                    format( szValue, 31, "%0.0f%s %0.0f%s %0.1f %d", ( ( ( 255.0 - WINDWALK_INVISIBILITY ) / 255.0 ) * 100.0 ), "%", ( ( ( WINDWALK_SPEED - SPEED_KNIFE ) / 255.0 ) * 100.0 ), "%", float( WINDWALK_DURATION ), WINDWALK_DAMAGE );
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
                    format( szValue, 31, "%0.0f%s %0.0f%s", ( NE_S_ELUNES_get_knife( iSkillLevel ) * 100.0 ), "%", ( NE_S_ELUNES_get_magic( iSkillLevel ) * 100.0 ), "%" );
                    return ( 2 );
                }

                case SKILL_1:
                {
                    format( szValue, 31, "%0.0f%s", ( s_Evasion[iSkillLevel] * 100.0 ), "%" );
                    return ( 1 );
                }

                case SKILL_2:
                {
                    format( szValue, 31, "%d %0.0f%s", ( s_BlessingArmor[iSkillLevel] - 100 ), ( ( s_BlessingSpeed[iSkillLevel] ) * 100.0 ) , "%" );
                    return ( 2 );
                }

                case SKILL_3:
                {
                    format( szValue, 31, "%0.0f%s", ( s_TrueshotAura[iSkillLevel] * 100.0 ), "%" );
                    return ( 1 );
                }

                // Ultimates

                case ULTIMATE_1:
                {
                    format( szValue, 31, "%d %0.1f", REJUVENATION_MAXHEALTH, REJUVENATION_DURATION );
                    return ( 2 );
                }

                case ULTIMATE_2:
                {
                    format( szValue, 31, "%0.1f %d", ROOT_DURATION, ROOT_MAXDAMAGE );
                    return ( 2 );
                }

                case ULTIMATE_3:
                {
                    format( szValue, 31, "%d %d %0.1f %0.0f%s", SHADOWSTRIKE_DAMAGE, SHADOWSTRIKE_DOT, float( SHADOWSTRIKE_DURATION ), ( ( ( SPEED_KNIFE - SHADOWSTRIKE_SPEED ) / 255.0 ) * 100.0 ), "%" );
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

        format( szFileName, 63, "%s/help/%s.html", WAR3X_DIR, RACEKEYNAME[iRaceNum] );

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
                format( szSkillHeader, 191, "%s", HELP_HEADER_RACIAL );
                write_file( szFileName, szSkillHeader, -1 );

                HELP_description_racial( iRaceNum, LANG_SERVER, szSkillDesc, LEVEL_ALL );
            }

            // Trainable Skills

            else
            {
                if ( iSkillNum == SKILL_1 )
                {
                    format( szSkillHeader, 191, "^n%s", HELP_HEADER_SKILLS );
                    write_file( szFileName, szSkillHeader, -1 );
                }

                HELP_description_trained( iRaceNum, iSkillNum, LANG_SERVER, szSkillDesc, LEVEL_ALL );
            }

            Get_SkillName( iRaceNum, iSkillNum, LANG_SERVER, szSkillName );

            format( szText, 255, "<li><b>%s</b>^n%s", szSkillName, szSkillDesc );

            write_file( szFileName, szText, -1 );
        }

        // Ultimates

        for ( new iUltimateNum = TOTAL_SKILLS; iUltimateNum < TOTAL_SKILLS + TOTAL_ULTIMATES; iUltimateNum++ )
        {
            if ( iUltimateNum == ULTIMATE_1 )
            {
                new szUltimate[192];
                format( szUltimate, 191, HELP_HEADER_ULTIMATES, LEVEL_ULTIMATE );

                format( szSkillHeader, 191, "^n%s", szUltimate );
                write_file( szFileName, szSkillHeader, -1 );
            }

            Get_SkillName( iRaceNum, iUltimateNum, LANG_SERVER, szSkillName );
            HELP_description_ultimate( iRaceNum, iUltimateNum, LANG_SERVER, szSkillDesc );

            format( szText, 255, "<li><b>%s</b>^n%s", szSkillName, szSkillDesc );
            write_file( szFileName, szText, -1 );
        }
    }

    // Item Help

    new szItemDesc[256], szItemName[32], szItemHeader[256];

    // Set Filename

    format( szFileName, 63, "%s/help/items.html", WAR3X_DIR );

    // Delete old file(s)

    delete_file( szFileName );

    // Html Header

    write_file( szFileName, HTML_HEADER, -1 );

    format( szItemHeader, 255, "%s", HELP_HEADER_ITEMS );
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

        format( szItemDesc, 255, szItemDesc, szValueArgs[0], szValueArgs[1], szValueArgs[2], szValueArgs[3], szValueArgs[4] );

        format( szText, 255, "<li><b><font color=rgb(%i,%i,%i)>%s</font></b>^n%s", ITEMCOLOR[iItemNum][GLOW_R], ITEMCOLOR[iItemNum][GLOW_G], ITEMCOLOR[iItemNum][GLOW_B], szItemName, szItemDesc );
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

            format( szNewValues[i], 15, "%s-%s", szTempMin, szTempMax );
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

            format( szNewValues[i], 15, "%s/%s/%s", szTempArgs[0], szTempArgs[1], szTempArgs[2] );
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
