// Begin MENUS.INL

/*
    Warcarft 3: Expansion
    Main Menu

    1. Select Race
    2. Select Skills
    3. Buy Items
    4. Show Level
    5. Player Skills
    6. Player Options
    7. Race Options
    8. Admin Options        (coming soon)
    9. Help Topics

    0. Exit
*/

/* - Warcraft 3 Menu ( /war3menu ) ------------------------------ */


// Display

public menu_War3menu( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new szMenu[512], iLen;
    new iKeys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<8)|(1<<9);

    // Draw Menu

    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_TITLE );

    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_1 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_2 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_3 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_4 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_5 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_6 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_7 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_8 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_WAR3MENU_9 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_CANCEL );

    show_menu( id, iKeys, szMenu );

    return PLUGIN_HANDLED;
}


// Commands

public _menu_War3menu( id, iKey ) {

    if ( iKey == KEY_CANCEL )
        return PLUGIN_HANDLED;

    switch ( iKey + 1 )
    {
        case 1:     menu_SelectRace( id );
        case 2:     menu_SelectSkills( id );
        case 3:     menu_ItemShop( id );

        case 4:     WAR3_hud_level( id );
        case 5:     HELP_motd_players( id );

        case 6:     menu_PlayerOptions( id );
        case 7:     menu_RaceOptions( id );
//        case 8:     admin menu;

        case 9:     menu_HelpTopics( id );
    }

    return PLUGIN_HANDLED;
}


/* - 1. Select Race Menu ( /changerace ) ------------------------ */


// Display

public menu_SelectRace( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    if ( !g_bPlayerConnected[id] )
    {
        client_print( id, print_center, "%L", id, "INFO_NOTCONNECTED" );
        return PLUGIN_HANDLED;
    }

    new iKeys;
    new szMenu[512], iLen;

    iKeys = (1<<9);
    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_SELECTRACE_TITLE );

    // Build list of races ( and levels if saving xp )

    if ( g_bSaveXp )
    {
        if ( !g_SaveIds[id][0] )
            Get_SaveId( id );

        if ( !g_bXPfetched[id] )
        {
            // Check if Player in Save Queue ( quit/rejoin same round )

            new iArrayIndex = Queue_Check( g_SaveIds[id] );

            if ( iArrayIndex >= 0 )
            {
                // Fetch from Queue

                Queue_Retrieve( id, iArrayIndex );
            }

            else
            {
                // Fetch from Database

                Retrieve_Xp( id );
                Retrieve_Options( id );
            }

            g_bXPfetched[id] = true;
        }

        // Build Menu

        for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
        {
			new szRaceName[32];
			LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31 );

			if ( get_pcvar_bitsum( CVAR_restrict_races ) & WAR3_get_race_flag( iRaceNum ) )
            {
				LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31 )
                iLen += formatex( szMenu[iLen], 512 - iLen, "\d%d. %s\R\rD isab led          ^n", iRaceNum + 1, szRaceName );
            }

            else
            {
                new iCurrentLevel = WAR3_get_level( g_iXPtotal[id][iRaceNum] );
                new szLineColor[3];

                // Dont highlight / key current race

                if ( !g_PlayerInfo[id][CURRENT_RACE] || g_PlayerInfo[id][CURRENT_RACE] - 1 != iRaceNum )
                {
                    copy( szLineColor, 2, "\w" );
                    iKeys |= ( 1<<iRaceNum );
                }

                else
                {
                    copy( szLineColor, 2, "\d" );
                }

                // Draw Text

                iLen += formatex( szMenu[iLen], 512 - iLen, "%s%d. %s\R\dLeve l %d          ^n", szLineColor, iRaceNum + 1, szRaceName, iCurrentLevel );
            }
        }
    }

    else
    {
        // Retrieve Default Options

        if ( !g_PlayerInfo[id][CURRENT_RACE] )
            Retrieve_Defaults( id );

        for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
        {
			new szRaceName[32];
			LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31 );

            if ( get_pcvar_bitsum( CVAR_restrict_races ) & WAR3_get_race_flag( iRaceNum ) )
            {
                iLen += formatex( szMenu[iLen], 512 - iLen, "\d%d. %s\R\rD isab led          ^n", iRaceNum + 1, szRaceName );
            }

            else
            {
                new szLineColor[3];

                // Dont highlight / key current race

                if ( !g_PlayerInfo[id][CURRENT_RACE] || g_PlayerInfo[id][CURRENT_RACE] - 1 != iRaceNum )
                {
                    copy( szLineColor, 2, "\w" );
                    iKeys |= ( 1<<iRaceNum );
                }

                else
                {
                    copy( szLineColor, 2, "\d" );
                }

                iLen += formatex( szMenu[iLen], 512 - iLen, "%s%d. %s^n", szLineColor, iRaceNum + 1, szRaceName );
            }
        }
    }

    if ( g_PlayerInfo[id][CURRENT_RACE] )
        iLen += formatex( szMenu[iLen], 512 - iLen, "^n\d5. Random^n^n^n\w" );

    else
    {
        iLen += formatex( szMenu[iLen], 512 - iLen, "^n\w5. Random^n^n^n" );
        iKeys |= (1<<4);
    }

    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_CANCEL );

    show_menu( id, iKeys, szMenu );

    return PLUGIN_HANDLED;
}


// Commands

public _menu_SelectRace( id, iKey ) {

    if ( iKey == KEY_CANCEL )
        return PLUGIN_HANDLED;

    // Random ( make sure restricted race not selected )

    while ( iKey == KEY_RANDOM )
    {
        new iRaceNum = random_num( 1, TOTAL_RACES ) - 1;

        if ( !( get_pcvar_bitsum( CVAR_restrict_races ) & WAR3_get_race_flag( iRaceNum ) ) )
            iKey = iRaceNum;
    }

    // Make sure change can be made

    if ( g_PlayerInfo[id][CURRENT_RACE] && !g_bFreezeTime )
    {
        // Wait till next round to switch

        set_hudmessage( 255, 160, 0, HUDMESSAGE_POS_CENTER, HUDMESSAGE_POS_INFO, 0, 6.0, 5.0, 0.5, 1.0, HUDMESSAGE_CHAN_INFO );

		new szRaceName[32];
		LANG_GetRaceName( iKey + 1, id, szRaceName, 31, true );

        new szMessage[256];
        formatex( szMessage, 255, "%L", id, "INFO_NEWRACENEXTROUND", szRaceName );

        show_hudmessage( id, "%s", szMessage );
        g_PlayerInfo[id][CURRENT_NEXTRACE] = iKey + 1;
    }

    else
    {
        // Check if first race chosen on server

        if ( WAR3_is_first_race( id ) )
        {
            new iLevel = get_pcvar_num( CVAR_startlevel_first );

            if ( iLevel )
            {
                g_iXPtotal[id][iKey] = g_iLevelXp[iLevel];

                new szMessage[128];
                formatex( szMessage, 127, "^n%s %L^n", WAR3X_PREFIX, id, "INFO_LEVEL_FIRST", iLevel );

                client_print( id, print_chat, "%s", szMessage );
            }
        }

        // Check if 0 xp

        else if ( !g_iXPtotal[id][iKey] )
        {
            new iLevel = get_pcvar_num( CVAR_startlevel_other );

            if ( iLevel )
            {
                g_iXPtotal[id][iKey] = g_iLevelXp[iLevel];

                new szMessage[128];
                formatex( szMessage, 127, "^n%s %L^n", WAR3X_PREFIX, id, "INFO_LEVEL_OTHER", iLevel );

                client_print( id, print_chat, "%s", szMessage );
            }
        }

        // Update Race Info

        War3x_StoreSession( id, iKey );

        // Check if number of skills > allowed

        if ( WAR3_skills_left( id ) < 0 )
        {
            Reset_Skills( id );
            Reset_Ultimate( id );

            if ( WAR3_get_level( g_iXPtotal[id][iKey] ) )
                menu_SelectSkills( id );
        }

        WAR3_hud_level( id );
        war3_chatskills( id, iKey, 1 );
        WAR3_check_skills( id );

        // Hudmessage

        WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );

        // Activate Abilities ( if not freezetime )

        if ( !g_bFreezeTime && is_user_alive( id ) )
            WAR3_enable_skills( id );

        // Make sure health/armor is not greater than new maximum

        WAR3_check_health( id );
        WAR3_check_armor( id );

        // Apply Bonus Health

        WAR3_set_health( id );


        // Restricted Map Notification ( short term only )

        if ( g_bMapRestricted )
        {
            client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "INFO_RESTRICTEDMAP_CHAT" );

            set_hudmessage( 255, 160, 0, HUDMESSAGE_POS_CENTER, HUDMESSAGE_POS_SERVER, 0, 0.1, 6.0, 1.0, 2.0, HUDMESSAGE_CHAN_SERVER );
            show_hudmessage( id, "%L", id, "INFO_RESTRICTEDMAP_HUD" );
        }
    }

    return PLUGIN_HANDLED;
}


/* - 2. Select Skills Menu ( /selectskils ) --------------------- */


// Display

public menu_SelectSkills( id ) {

    if ( !g_bWar3xEnabled )	return PLUGIN_HANDLED;

    // Make sure race is selected

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
    {
        menu_SelectRace( id );
        return PLUGIN_HANDLED;
    }

    // Make sure skills can be selected

    if ( WAR3_skills_left( id ) <= 0 )
    {
        set_hudmessage( 255, 160, 0, HUDMESSAGE_POS_CENTER, HUDMESSAGE_POS_INFO, 0, 6.0, 5.0, 0.5, 1.0, HUDMESSAGE_CHAN_INFO );
        show_hudmessage( id, "%L", id, "INFO_NOSKILLPOINTS" );

        return PLUGIN_HANDLED;
    }

    // Before displaying, auto-select skill(s) if possible

    if ( WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] ) > ( TOTAL_SKILLLEVELS * ( TOTAL_SKILLSTRAINED - 1 ) + 1 ) )
    {

				new iSkillNum = 0;
        // Auto-select all skills except ultimate if level 10

				if ( WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] ) == TOTAL_LEVELS )
        {
            for ( iSkillNum = 0 + OFFSET_CURRENT_SKILLS; iSkillNum < TOTAL_SKILLSTRAINED + OFFSET_CURRENT_SKILLS; iSkillNum++ )
            {
                g_PlayerInfo[id][iSkillNum] = TOTAL_SKILLLEVELS;
            }

            // Don't show menu if ultimate already selected

            if ( g_PlayerInfo[id][CURRENT_ULTIMATE] )
                return PLUGIN_HANDLED;
        }

        // Find total trainable skills and see if all but one are trained fully

				else if ( g_PlayerInfo[id][CURRENT_ULTIMATE] )
				{
						new iSkillsFull = 0;

						for ( iSkillNum = 0 + OFFSET_CURRENT_SKILLS; iSkillNum < TOTAL_SKILLSTRAINED + OFFSET_CURRENT_SKILLS; iSkillNum++ )
						{
								if ( g_PlayerInfo[id][iSkillNum] == TOTAL_SKILLLEVELS )
								iSkillsFull++;
						}

						if ( iSkillsFull == TOTAL_SKILLSTRAINED - 1 )
						{
								for ( iSkillNum = 0 + OFFSET_CURRENT_SKILLS; iSkillNum < TOTAL_SKILLSTRAINED + OFFSET_CURRENT_SKILLS; iSkillNum++ )
								{
										if ( g_PlayerInfo[id][iSkillNum] != TOTAL_SKILLLEVELS )
												g_PlayerInfo[id][iSkillNum] = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] ) - ( TOTAL_SKILLLEVELS * ( TOTAL_SKILLSTRAINED - 1 ) + 1 );
								}
						}
				}
		}

    // Display Menu

    new iKeys;
    new szMenu[512], iLen;

    new szSkillName[64], szMenuSkill[64], szDisabled[20];
    new iCurrentSkill, iNextSkillLvl;

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new iRaceNum = g_PlayerInfo[id][CURRENT_RACE];

    iKeys = (1<<9)|(1<<8);

	new szRaceName[32];
	LANG_GetRaceName( iRaceNum, id, szRaceName, 31 );

    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_SELECTSKILLS_TITLE );
    iLen += formatex( szMenu[iLen], 512 - iLen, "\d%s^n", szRaceName );

    // Primary Skills

    for ( new iSkillNum = 1; iSkillNum <= TOTAL_SKILLSTRAINED; iSkillNum++ )
    {
        WAR3_race_info( id, iRaceNum - 1, iSkillNum, szSkillName );

        iCurrentSkill = iSkillNum + 1;
        iNextSkillLvl = g_PlayerInfo[id][iCurrentSkill] + 1;

        // Disabled

        if ( get_pcvar_bitsum( CVAR_restrict_skills ) & WAR3_get_skill_flag( iRaceNum, iSkillNum ) )
            copy( szDisabled, 19, "\r(R estr icted )\d" );

        else if ( get_pcvar_bitsum( CVAR_restrict_skills ) == 0 && get_pcvar_bitsum( CVAR_restrict_ultimates ) == 0 )
            copy( szDisabled, 19, "\d          \d" );

        else
        {
            copy( szDisabled, 19, "\y   (A llow ed )\d" );
        }

        // Allowed to Select Skill

        if ( War3x_GetSkillCheck( iLevel, iNextSkillLvl ) )
        {
            formatex( szMenuSkill, 63, "^n\w%d. %s \d\R%s Leve l %d ", iSkillNum, szSkillName, szDisabled, iNextSkillLvl );
            iKeys |= ( 1<<iSkillNum - 1 );
        }

        // Not Allowed

        else
        {
            // Skill Already at MAX

            if ( iNextSkillLvl > TOTAL_SKILLLEVELS )
            {
                formatex( szMenuSkill, 63, "^n\d%d. %s \d\R%s Leve l MAX ", iSkillNum, szSkillName, szDisabled );
            }

            // Skill Not Available

            else
            {
                formatex( szMenuSkill, 63, "^n\d%d. %s \d\R%s Leve l %d ", iSkillNum, szSkillName, szDisabled, iNextSkillLvl );
            }
        }

        iLen += formatex( szMenu[iLen], 512 - iLen, "%s", szMenuSkill );
    }

    iLen += formatex( szMenu[iLen], 512 - iLen, "^n" );

    // Ultimates

    for ( new iSkillNum = TOTAL_SKILLSTRAINED + 1; iSkillNum < TOTAL_ULTIMATES + ( TOTAL_SKILLSTRAINED + 1 ); iSkillNum++ )
    {
        WAR3_race_info( id, iRaceNum - 1, iSkillNum, szSkillName );
        iCurrentSkill = iSkillNum + 1;

        new iUltimateNum = iSkillNum - TOTAL_SKILLSTRAINED;

        // Disabled

        if ( get_pcvar_bitsum( CVAR_restrict_ultimates ) & WAR3_get_ult_flag( iRaceNum, iUltimateNum ) )
            copy( szDisabled, 19, "\r(R estr icted )\d" );

        else if ( get_pcvar_bitsum( CVAR_restrict_ultimates ) == 0 && get_pcvar_bitsum( CVAR_restrict_skills ) == 0 )
            copy( szDisabled, 19, "\d          \d" );

        else
        {
            copy( szDisabled, 19, "\y   (A llow ed )\d" );
        }

        // Can't Yet Obtain an Ultimate

        if ( iLevel < LEVEL_ULTIMATE )
            formatex( szMenuSkill, 63, "^n\d%d. Ultimate: %s \R%s ", iSkillNum, szSkillName, szDisabled );

        // Already Have Ultimate ( show only selected )

        else if ( g_PlayerInfo[id][CURRENT_ULTIMATE] )
        {
            if ( iSkillNum - TOTAL_SKILLSTRAINED == g_PlayerInfo[id][CURRENT_ULTIMATE] )
                formatex( szMenuSkill, 63, "^n\d%d. Ultimate: %s \R%s ", iSkillNum, szSkillName, szDisabled );

            else
            {
                formatex( szMenuSkill, 63, "^n" );
            }
        }

        // Can Select an Ultimate

        else
        {
            formatex( szMenuSkill, 63, "^n\w%d. Ultimate: %s \R%s ", iSkillNum, szSkillName, szDisabled );
            iKeys |= ( 1<<iSkillNum - 1 );
        }

        iLen += formatex( szMenu[iLen], 512 - iLen, "%s", szMenuSkill );
    }

    iLen += formatex( szMenu[iLen], 512 - iLen, "^n^n\w%s", MENU_SELECTSKILLS_9 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "^n^n\w%s", MENU_CANCEL );

    // Show Skills Menu

    if ( !is_user_bot( id ) )
        show_menu( id, iKeys, szMenu );

    // Pick Random Skill(s) for Bots

    else
    {
        new iUltimate;

        if ( g_PlayerInfo[id][CURRENT_ULTIMATE] )
            iUltimate = 1;

        new iPoints = g_PlayerInfo[id][CURRENT_SKILL1] + g_PlayerInfo[id][CURRENT_SKILL2] + g_PlayerInfo[id][CURRENT_SKILL3] + iUltimate;

        // Ultimate

        if ( iLevel >= LEVEL_ULTIMATE && !iUltimate )
        {
            new iRandomNum = random_num( 1,TOTAL_ULTIMATES );
            g_PlayerInfo[id][CURRENT_ULTIMATE] = iRandomNum;
            iPoints++;
        }

        // Skills

        while ( iLevel != iPoints )
        {
            new iRandomNum = random_num( 1, TOTAL_SKILLSTRAINED );
            new iSkillNum = iRandomNum + 1;

            if ( g_PlayerInfo[id][iSkillNum] + 1 <= TOTAL_SKILLLEVELS && GETSKILLCHECK[g_PlayerInfo[id][iSkillNum]] <= iLevel )
            {
                g_PlayerInfo[id][iSkillNum]++;
                iPoints++;
            }
        }
    }

    if ( g_bSaveXp )
        g_bStoreSkills[id] = true;

    return PLUGIN_HANDLED;
}


// Commands

public _menu_SelectSkills( id, iKey ) {

    if ( iKey == KEY_CANCEL )
        return PLUGIN_HANDLED;

    switch ( iKey + 1 )
    {
        case 1:         g_PlayerInfo[id][CURRENT_SKILL1] += 1;
        case 2:         g_PlayerInfo[id][CURRENT_SKILL2] += 1;
        case 3:         g_PlayerInfo[id][CURRENT_SKILL3] += 1;
        case 4,5,6:     g_PlayerInfo[id][CURRENT_ULTIMATE] = iKey - ( TOTAL_SKILLSTRAINED - 1 );

        case 9:         HELP_motd_race( id, g_PlayerInfo[id][CURRENT_RACE] - 1 );
    }

    // Check to See if More Skills can be Selected

    if ( WAR3_skills_left( id ) > 0 && iKey != KEY_CANCEL )
        menu_SelectSkills( id );

    else
    {
        WAR3_hud_level( id );
    }

    // Activate New Skills

    if ( !g_bFreezeTime && is_user_alive( id ) )
        WAR3_enable_skills( id );

    g_bStoreSkills[id] = true;

    return PLUGIN_HANDLED;
}


/* - 3. Shop Menu ( /shopmenu ) --------------------------------- */


// Display

public menu_ItemShop( id ) {

    if ( !g_bWar3xEnabled || !is_user_alive( id ) )
        return PLUGIN_HANDLED;

    // Shopmenus Disabled

    if ( !get_pcvar_num( CVAR_shopmenus ) )
    {
        client_print( id, print_center, "%L", id, "INFO_SHOP_DISABLED" );
        return PLUGIN_HANDLED;
    }

    // Make sure race is selected

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
    {
        menu_SelectRace( id );
        return PLUGIN_HANDLED;
    }

    // VIP Restriction

    if ( id == g_Vip )
    {
        client_print( id, print_center, "%L", id, "INFO_SHOP_VIP" );
        return PLUGIN_HANDLED;
    }

    // Buy Zone Restriction

    if ( get_pcvar_num( CVAR_shopzone ) && !g_bInBuyZone[id] )
    {
        client_print( id, print_center, "%L", id, "INFO_SHOP_BUYZONE" );
        return PLUGIN_HANDLED;
    }

    // Buy Time Expired

    if ( get_pcvar_num( CVAR_shoptime ) && get_pcvar_float( CVAR_mp_buytime ) < ( get_gametime() - g_fBuyTime ) / 60.0  )
    {
        Create_TextMsg( id, print_center, "#Cant_buy" );
        return PLUGIN_HANDLED;
    }

    new iKeys;
    new szMenu[512], iLen;

    iKeys = (1<<8)|(1<<9);

    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_BUYITEM_TITLE );

    for ( new iItemNum = 1; iItemNum <= TOTAL_SHOPITEMS; iItemNum++ )
    {
		new szItemName[32];
		LANG_GetItemName ( iItemNum, SHOP_COMMON, id, szItemName, 31 )

        // Item Restricted

        if ( get_pcvar_bitsum( CVAR_restrict_items ) & WAR3_get_item_flag( iItemNum ) )
            iLen += formatex( szMenu[iLen], 512 - iLen, "\d%d. %s\R\rD isab led ^n", iItemNum, szItemName );

        // Not enough players for Tome

        else if ( iItemNum == ITEM_TOME && get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        {
            iLen += formatex( szMenu[iLen], 512 - iLen, "\d%d. %s\R\y%d ^n", iItemNum, szItemName, ITEM_COST[iItemNum] );
        }

        else
        {
            iKeys |= ( 1<<iItemNum - 1 );
            iLen += formatex( szMenu[iLen], 512 - iLen, "\w%d. %s\R\y%d ^n", iItemNum, szItemName, ITEM_COST[iItemNum] );
        }
    }

    iLen += formatex( szMenu[iLen], 512 - iLen, "^n\w%s", MENU_BUYITEM_9 );
    iLen += formatex( szMenu[iLen], 512 - iLen, "^n^n\w%s", MENU_CANCEL );

    show_menu( id, iKeys, szMenu );

    return PLUGIN_HANDLED;
}


// Commands

public _menu_ItemShop( id, iKey ) {

    if ( iKey == KEY_CANCEL )
        return PLUGIN_HANDLED;

    if ( iKey == KEY_HELP )
    {
        HELP_motd_items( id );
        return PLUGIN_HANDLED;
    }

    // Buy item

    new iItemNum = iKey + 1;

    if ( iItemNum == ITEM_TOME )
        Item_BuyTome( id );

    else
    {
        Item_Buy( id, iItemNum );
    }

    return PLUGIN_HANDLED;
}


/* - 6. Skills Info Menu (/skillsinfo) -------------------------- */


// Display

public menu_SkillsHelp( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new iKeys = (1<<9);
    new szMenu[512], iLen;

    iLen += formatex( szMenu[iLen], 512 - iLen, "%L", id, "MENU_SKILLSINFO_TITLE" );

    for ( new iRaceNum = 0; iRaceNum < TOTAL_RACES; iRaceNum++ )
    {
		new szRaceName[32];
		LANG_GetRaceName( iRaceNum + 1, id, szRaceName, 31 );

        iLen += formatex( szMenu[iLen], 512 - iLen, "%d. %s^n", iRaceNum + 1, szRaceName );
        iKeys |= ( 1<<iRaceNum );
    }

    iLen += formatex( szMenu[iLen], 512 - iLen, "^n^n%s", MENU_CANCEL );

    show_menu( id, iKeys, szMenu );

    return PLUGIN_HANDLED;
}


// Commands

public _menu_SkillsHelp( id, iKey ) {

    if ( iKey == KEY_CANCEL )
        return PLUGIN_HANDLED;

    HELP_motd_race( id, iKey );

    return PLUGIN_HANDLED;
}


/* - 6. Race Options -------------------------------------------- */

/*
    Player Options

    1. Show Race Icons              ON/OFF
    2. Show Health Icons            ON/OFF
    3. Show Dispell Icons           ON/OFF
    4. Auto-Cast Heals/Dispells     ON/OFF

    5. Resolution                  XXXxXXX


    0. Exit
*/

public menu_PlayerOptions( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    // Make sure race is selected

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
    {
        menu_SelectRace( id );
        return PLUGIN_HANDLED;
    }

    new szMenu[512], szOnOff[8], iLen;
    new iKeys = (1<<9)|(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4);

    // Draw Menu

    iLen += formatex( szMenu[iLen], 512 - iLen, "%s", MENU_PLAYEROPTIONS_TITLE );

    WAR3_get_onoff( id, g_PlayerOptions[id][OPTION_RACEICONS], szOnOff );
    iLen += formatex( szMenu[iLen], 512 - iLen, "\w%s \y\R%s            ^n", MENU_PLAYEROPTIONS_1, szOnOff );

    WAR3_get_onoff( id, g_PlayerOptions[id][OPTION_HEALICONS], szOnOff );
    iLen += formatex( szMenu[iLen], 512 - iLen, "\w%s \y\R%s            ^n", MENU_PLAYEROPTIONS_2, szOnOff );

    WAR3_get_onoff( id, g_PlayerOptions[id][OPTION_DISPELLICONS], szOnOff );
    iLen += formatex( szMenu[iLen], 512 - iLen, "\w%s \y\R%s            ^n", MENU_PLAYEROPTIONS_3, szOnOff );

    WAR3_get_onoff( id, g_PlayerOptions[id][OPTION_AUTOCAST], szOnOff );
    iLen += formatex( szMenu[iLen], 512 - iLen, "\w%s \y\R%s            ^n^n", MENU_PLAYEROPTIONS_4, szOnOff );

    new iRes = g_PlayerOptions[id][OPTION_RESOLUTION];
    iLen += formatex( szMenu[iLen], 512 - iLen, "\w%s \y\R%ix%i            ^n^n", MENU_PLAYEROPTIONS_5, STEAM_RESOLUTION[iRes][_X], STEAM_RESOLUTION[iRes][_Y] );
    iLen += formatex( szMenu[iLen], 512 - iLen, "\w%s", MENU_CANCEL );

    show_menu( id, iKeys, szMenu );

    return PLUGIN_HANDLED;
}


public _menu_PlayerOptions( id, iKey ) {

    if ( iKey == KEY_CANCEL )
        return PLUGIN_HANDLED;

    if ( iKey == OPTION_RESOLUTION )
    {
        g_PlayerOptions[id][iKey] += 1;

        if ( g_PlayerOptions[id][iKey] >= TOTAL_RESOLUTIONS )
            g_PlayerOptions[id][iKey] = 0;

        // Draw Hud messages for player

        WAR3_hud_xp( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 0.2, 3.0 );
        WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 0.2, 3.0 );
    }

    else
    {
        g_PlayerOptions[id][iKey] = AMX_get_opposite( g_PlayerOptions[id][iKey] );
    }

    // Re-Draw menu

    menu_PlayerOptions( id );

    g_bStoreOptions[id] = true;

    return PLUGIN_HANDLED;
}

/* - 7. Race Options -------------------------------------------- */


// Display

public menu_RaceOptions( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    // Make sure race is selected

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
    {
        menu_SelectRace( id );
        return PLUGIN_HANDLED;
    }

    new iKeys = (1<<9);
    new szMenu[512], iLen;

    new raceId = g_PlayerInfo[id][CURRENT_RACE] - 1;

	new szRaceName[32];
	LANG_GetRaceName( raceId + 1, id, szRaceName, 31 );

    iLen += formatex( szMenu[iLen], 511 - iLen, MENU_RACEOPTIONS_TITLE );
    iLen += formatex( szMenu[iLen], 511 - iLen, MENU_RACEOPTIONS_TITLE2, szRaceName );

    // Reset Skills

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    if ( iLevel >= RACEOPTIONS_SKILLS )
    {
        iLen += formatex( szMenu[iLen], 511 - iLen, "\w" );
        iKeys |= (1<<0);
    }

    else
    {
        iLen += formatex( szMenu[iLen], 511 - iLen, "\d" );
    }

    iLen += formatex( szMenu[iLen], 511 - iLen, MENU_RACEOPTIONS_1, RACEOPTIONS_SKILLS );

    // Reset Ultimate

    if ( iLevel >= RACEOPTIONS_ULTIMATE && g_PlayerInfo[id][CURRENT_ULTIMATE] && !g_bChangeUltimate[id] )
    {
        iLen += formatex( szMenu[iLen], 511 - iLen, "\w" );
        iKeys |= (1<<1);
    }

    else
    {
        iLen += formatex( szMenu[iLen], 511 - iLen, "\d" );
    }

    iLen += formatex( szMenu[iLen], 511 - iLen, MENU_RACEOPTIONS_2, RACEOPTIONS_ULTIMATE );

    // Reset Experience

    iLen += formatex( szMenu[iLen], 511 - iLen, "\w^n%s", MENU_RACEOPTIONS_5 );
    iKeys |= (1<<4);


    iLen += formatex( szMenu[iLen], 511 - iLen, "\w^n^n%s", MENU_CANCEL );
    show_menu( id, iKeys, szMenu );

    return PLUGIN_HANDLED;
}


// Race Options

public _menu_RaceOptions( id, iKey ) {

    if ( iKey == KEY_CANCEL )
        return PLUGIN_HANDLED;

    new iRaceId = g_PlayerInfo[id][CURRENT_RACE] - 1;

    switch ( iKey + 1 )
    {
        case 1:     // Reset Skills
        {
            Reset_Skills( id );
            menu_SelectSkills( id );

            client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "INFO_RESETSKILLS" );
        }

        case 2:     // Reset Ultimate
        {
            Reset_Ultimate( id );
            menu_SelectSkills( id );

            g_bChangeUltimate[id] = true;

            client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "INFO_RESETULTIMATE" );
        }

        case 5:     // Reset Experience
        {
            new iCurrentItem = g_PlayerInfo[id][CURRENT_ITEM];
            new iCurrentRace = g_PlayerInfo[id][CURRENT_RACE];

            g_PlayerInfo[id] = {0,0,0,0,0,0,0,0,0,0,0,0};

            g_PlayerInfo[id][CURRENT_RACE] = iCurrentRace;
            g_PlayerInfo[id][CURRENT_ITEM] = iCurrentItem;

            // Check if first race chosen on server

            if ( WAR3_is_first_race( id ) )
            {
                new iLevel = get_pcvar_num( CVAR_startlevel_first );

                if ( iLevel )
                {
                    g_iXPtotal[id][g_PlayerInfo[id][CURRENT_RACE] - 1] = g_iLevelXp[iLevel];
                    g_PlayerInfo[id][CURRENT_XP] = g_iLevelXp[iLevel];

                    new szMessage[128];
                    formatex( szMessage, 127, "^n%s %L^n", WAR3X_PREFIX, id, "INFO_LEVEL_FIRST", iLevel );

                    client_print( id, print_chat, "%s", szMessage );
                }
            }

            else
            {
                new iLevel = get_pcvar_num( CVAR_startlevel_other );

                if ( iLevel )
                {
                    g_iXPtotal[id][g_PlayerInfo[id][CURRENT_RACE] - 1] = g_iLevelXp[iLevel];
                    g_PlayerInfo[id][CURRENT_XP] = g_iLevelXp[iLevel];

                    new szMessage[128];
                    formatex( szMessage, 127, "^n%s %L^n", WAR3X_PREFIX, id, "INFO_LEVEL_OTHER", iLevel );

                    client_print( id, print_chat, "%s", szMessage );
                }
            }

            WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );
            WAR3_hud_level( id );

            if ( get_user_health( id ) > 100 )
                set_user_health( id, 100 );

            if ( get_user_armor( id ) > 100 )
                set_user_armor( id, 100 );

            if ( g_PlayerInfo[id][CURRENT_ITEM] == ITEM_RING )
                set_user_armor( id, get_user_armor( id ) + ITEM_RING_VALUE );

            // Reset XP

            Reset_XP( id );

            new szMessage[128], szRaceName[32];

			LANG_GetRaceName( iRaceId + 1, id, szRaceName, 31 );
            formatex( szMessage, 127, "%s %L", WAR3X_PREFIX, id, "INFO_RESETXP", szRaceName );

            client_print( id, print_chat, "%s", szMessage );
        }
    }

    return PLUGIN_HANDLED;
}


/* - 9. Help Topics --------------------------------------------- */


public menu_HelpTopics( id ) {

    new iKeys = (1<<0)|(1<<1)|(1<<8)|(1<<9);
    new szMenu[512], iLen;

    iLen += formatex( szMenu[iLen], 511 - iLen, "%s", MENU_HELPTOPICS_TITLE );

    iLen += formatex( szMenu[iLen], 511 - iLen, "%s", MENU_HELPTOPICS_1 );
    iLen += formatex( szMenu[iLen], 511 - iLen, "%s", MENU_HELPTOPICS_2 );

    iLen += formatex( szMenu[iLen], 511 - iLen, "%s", MENU_HELPTOPICS_9 );

    iLen += formatex( szMenu[iLen], 511 - iLen, "%s", MENU_CANCEL );

    show_menu( id, iKeys, szMenu );

    return PLUGIN_HANDLED;
}


public _menu_HelpTopics( id, iKey ) {

    if ( iKey == KEY_CANCEL )
        return PLUGIN_HANDLED;

    switch ( iKey + 1 )
    {
        case 1:     menu_SkillsHelp( id );
        case 2:     HELP_motd_items( id );

        case 9:     HELP_motd_war3x( id );
    }

    return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. - //
