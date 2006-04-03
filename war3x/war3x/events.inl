
//////////////////////////////////// Event Configuration /////


// Status icons

public on_StatusIcon( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new szStatusItem[8];
    read_data( 2, szStatusItem, 7 );

    if ( equali( szStatusItem, "defuser" ) )
    {
        if ( read_data( 1 ) )
            g_bGotDefuser[id] = true;

        else
        {
            g_bGotDefuser[id] = false;
        }
    }

    else if ( equali( szStatusItem, "buyzone" ) )
    {
        if ( read_data( 1 ) )
            g_bInBuyZone[id] = true;

        else
        {
            g_bInBuyZone[id] = false;
        }
    }

    return PLUGIN_CONTINUE;
}


// Armor Bought

public on_BuyArmor( id, iKey ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new iArmorCost;
    new bool:bArmorGiven;

    new iCurrentArmor = get_user_armor( id );

    new bool:bHasHelmet;

    if ( g_iPlayerArmor[id][ARMOR_TYPE] == ARMORTYPE_HELMET )
        bHasHelmet = true;

    new const KEY_KEVLAR        = 0;
    new const KEY_ASSAULTSUIT   = 1;


    if ( iCurrentArmor == WAR3_get_maxarmor( id ) && iKey == KEY_KEVLAR )
    {
        Create_TextMsg( id, print_center, "#Already_Have_Kevlar" );
        return PLUGIN_HANDLED;
    }

    else if ( iCurrentArmor == WAR3_get_maxarmor( id ) && iKey == KEY_ASSAULTSUIT && bHasHelmet )
    {
        Create_TextMsg( id, print_center, "#Already_Have_Kevlar_Helmet" );
        return PLUGIN_HANDLED;
    }

    else if ( iKey == KEY_KEVLAR )
    {
        iArmorCost = 650;

        if ( cs_get_user_money( id ) >= iArmorCost )
        {
            if ( bHasHelmet )
            {
                Create_TextMsg( id, print_center, "#Already_Have_Helmet_Bought_Kevlar" );
                give_item( id, "item_assaultsuit" );
            }

            else
            {
                give_item( id, "item_kevlar" );
            }

            set_user_armor( id, WAR3_get_maxarmor( id ) );

            cs_update_money( id, -iArmorCost, 1 );

            if ( iCurrentArmor == 0 )
                WAR3_armorskill_on( id );

            g_bArmorDepleted[id] = false;
            bArmorGiven = true;
        }
    }

    else if ( iKey == KEY_ASSAULTSUIT )
    {
        if ( bHasHelmet )
            iArmorCost = 650;

        else
        {
            if ( iCurrentArmor == WAR3_get_maxarmor( id ) )
                iArmorCost = 350;

            else
            {
                iArmorCost = 1000;
            }
        }

        if ( cs_get_user_money( id ) >= iArmorCost )
        {
            if ( !bHasHelmet && iCurrentArmor )
                Create_TextMsg( id, print_center, "#Already_Have_Kevlar_Bought_Helmet" );

            else if ( bHasHelmet )
                Create_TextMsg( id, print_center, "#Already_Have_Helmet_Bought_Kevlar" );

            give_item( id, "item_assaultsuit" );
            set_user_armor( id, WAR3_get_maxarmor( id ) );

            cs_update_money( id, -iArmorCost, 1 );
            g_iPlayerArmor[id][ARMOR_TYPE] = ARMORTYPE_HELMET;

            if ( iCurrentArmor == 0 )
                WAR3_armorskill_on( id );

            g_bArmorDepleted[id] = false;
            bArmorGiven = true;
        }
    }

    // Check for speed boost ( nature's blessing )

    if ( bArmorGiven && g_PlayerInfo[id][CURRENT_RACE] == RACE_NIGHTELF && g_PlayerInfo[id][CURRENT_SKILL2] )
        WAR3_set_speed( id );

    return PLUGIN_CONTINUE;
}


// Armor picked up

public on_ArmorPickup( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    if ( g_bArmorDepleted[id] )
        WAR3_armorskill_on( id );

    set_user_armor( id, WAR3_get_maxarmor( id ) );
    g_bArmorDepleted[id] = false;

    return PLUGIN_CONTINUE;
}


// Armor type

public on_ArmorType( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    if ( read_data( 1 ) )
        g_iPlayerArmor[id][ARMOR_TYPE] = ARMORTYPE_HELMET;

    else
    {
        g_iPlayerArmor[id][ARMOR_TYPE] = ARMORTYPE_KEVLAR;
    }

    return PLUGIN_CONTINUE;
}


// Target ( team )

public on_Target_Team( id ) {

	if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    g_PlayerTarget[id] = 0;
    g_PlayerTargetTeam[id] = read_data( 2 );

    return PLUGIN_CONTINUE;
}


// Target ( id )

public on_Target_Id( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new target = read_data( 2 );
    new bool:bNewTarget;

    if ( g_PlayerTarget[id] != target )
        bNewTarget = true;

    g_PlayerTarget[id] = target;

    // Check if ultimate is charging

    if ( is_user_alive( id ) && g_iChargeUltimate[id] && target && !( Ultimate_Target( id ) & ULTIMATE_TARGET_SELFONLY ) )
    {
        Ultimate_Cast( id, target );
    }

    // Auto-cast on teammate(s)

    else if ( target && g_PlayerInfo[id][CURRENT_ULTIMATE] && !g_fUltimateCooldown[id] && id != g_Vip )
    {
        // Prime/cast ultimate ( if bot )

        if ( is_user_bot( id ) )
        {
            // Heal teammate

            if ( get_user_team( id ) == get_user_team( target ) )
            {
                if ( Ultimate_Type( id ) & ULTIMATE_TYPE_HEAL && WAR3_is_lowhealth( target ) && WAR3_can_heal( id, target ) )
                    cmd_Ultimate( id );
            }

            // Damage enemy

            else if ( Ultimate_Target( id ) & ULTIMATE_TARGET_ENEMY && Ultimate_Type( id ) & ULTIMATE_TYPE_OFFENSIVE )
            {
                new Float:fRandomNum = random_float( 0.0,1.0 );

                if ( fRandomNum <= BOT_ULTIMATECHANCE )
                    cmd_Ultimate( id );
            }
        }

        // Auto-cast heal on teammate if player option allows

        else if ( get_user_team( id ) == get_user_team( target ) && g_PlayerOptions[id][OPTION_AUTOCAST] )
        {
            if ( Ultimate_Type( id ) & ULTIMATE_TYPE_HEAL && WAR3_is_lowhealth( target ) && WAR3_can_heal( id, target ) )
                cmd_Ultimate( id );

            /* DISPELLS NOT IMPLEMENTED
            else if ( Ultimate_Type( id ) & ULTIMATE_TYPE_DISPELL && Ultimate_Target( id ) & ULTIMATE_TARGET_TEAMMATE )
            */
        }
    }

    // Team race icons

    if ( is_user_alive( id ) && g_PlayerOptions[id][OPTION_RACEICONS] && !g_bPlayerSaveMe[target] && !g_bPlayerDispellMe[target] )
    {
        if ( target && g_PlayerTargetTeam[id] == CS_TEAMID_TEAMMATE )
        {
            new IconIndex;

            switch ( g_PlayerInfo[target][CURRENT_RACE] )
            {
                case RACE_UNDEAD:       IconIndex = ICON_UNDEAD;
                case RACE_HUMAN:        IconIndex = ICON_HUMAN;
                case RACE_ORC:          IconIndex = ICON_ORC;
                case RACE_NIGHTELF:     IconIndex = ICON_NIGHTELF;
            }

            if ( IconIndex )
                Create_TE_PLAYERATTACHMENT( id, target, 44, IconIndex, 5 );
        }
    }

    // Show spectator info

    else if ( !is_user_alive( id ) && bNewTarget && target && target != id && g_bXPfetched[id] )
    {
        new iResNum = g_PlayerOptions[id][OPTION_RESOLUTION];
        new Float:fXpos = 13.0 / STEAM_RESOLUTION[iResNum][_X];
        new Float:fYpos = float( STEAM_RESOLUTION[iResNum][_Y] - 78 + 5 ) / float( STEAM_RESOLUTION[iResNum][_Y] );

        new iHealth = get_user_health( target );

        if ( g_bEvadeNextShot[target] )
            iHealth -= 1024;

        new szMessage[256], szM1[256], szM2[256], szM3[256];
        formatex( szM3, 255, "%L", id, "INFO_FOLLOW_MESSAGE_3", iHealth, get_user_armor( target ), cs_get_user_money( target ) );

        if ( g_PlayerInfo[target][CURRENT_RACE] )
        {
            new iRaceNum = g_PlayerInfo[target][CURRENT_RACE] - 1;
            new iItemNum = g_PlayerInfo[target][CURRENT_ITEM];
            new iXp      = g_PlayerInfo[target][CURRENT_XP];
            new iLevel   = WAR3_get_level( iXp );
            new iNextXp  = WAR3_get_next_xp( iXp );

            new iLastlvlXp = WAR3_get_last_xp( iXp );
            new iNextlvlXp = WAR3_get_next_xp( iXp );

            new Float:fPercentage = ( float( iXp ) - float( iLastlvlXp ) ) / ( float( iNextlvlXp ) - float( iLastlvlXp ) ) * 100.0;
            new iPercentage = floatround( fPercentage );

			// Get item and race name

			new szItemName[32], szRaceName[32];

			LANG_GetItemName( iItemNum, SHOP_COMMON, id, szItemName, 31 );
			LANG_GetRaceName ( iRaceNum + 1, id, szRaceName, 31, true )

            // Create message

            formatex( szM1, 255, "%L", id, "INFO_FOLLOW_MESSAGE_1", szRaceName, iLevel, iXp, iNextXp, iPercentage, "%%" );
            formatex( szM2, 255, "%L", id, "INFO_FOLLOW_MESSAGE_2", szItemName, "None" );

            formatex( szMessage, 255, "%s^n%s^n%s", szM1, szM2, szM3 );

            // Show message

            set_hudmessage( 255, 255, 255, fXpos, fYpos, 0, 6.0, 500.0, 0.1, 0.5, HUDMESSAGE_CHAN_DEAD );
            show_hudmessage( id, "%s", szMessage );
        }

        else
        {
            formatex( szMessage, 255, "%L^n%s", id, "INFO_FOLLOW_NORACE", szM3 );

            set_hudmessage( 255, 255, 255, fXpos, fYpos, 0, 6.0, 500.0, 0.1, 0.5, HUDMESSAGE_CHAN_DEAD );
            show_hudmessage( id, "%s", szMessage );
        }
    }

    // Enemy

    if ( g_PlayerTargetTeam[id] == CS_TEAMID_ENEMY )
    {
        if ( get_pcvar_num( CVAR_mp_playerid ) == CS_MP_PLAYERID_SHOWALL && entity_get_int( target, EV_INT_rendermode ) != kRenderTransTexture )
            cs_status_target( id );

        else
        {
            cs_status_hide( id );
        }
    }

    // No target

    else if ( g_PlayerTargetTeam[id] != CS_TEAMID_NEUTRAL && g_PlayerTargetTeam[id] != CS_TEAMID_TEAMMATE )
    {
        cs_status_hide( id );

        //cs_status_info( id );

        g_PlayerTarget[id] = 0;
        g_PlayerTargetTeam[id] = 0;
    }

    return PLUGIN_CONTINUE;
}


// Target ( heath ) ( teammates/hostages only )

public on_Target_Health( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // Teammate

    if ( g_PlayerTargetTeam[id] == CS_TEAMID_TEAMMATE )
    {
        if ( get_pcvar_num( CVAR_mp_playerid ) != CS_MP_PLAYERID_SHOWNONE )
            cs_status_target( id );

        else
        {
            cs_status_hide( id );
        }
    }

    // Hostage

    else if ( g_PlayerTargetTeam[id] == CS_TEAMID_NEUTRAL )
    {
        if ( get_pcvar_num( CVAR_mp_playerid ) != CS_MP_PLAYERID_SHOWNONE )
            cs_status_target( id );

        else
        {
            cs_status_hide( id );
        }
    }

    // No target

    else
    {
        g_PlayerTarget[id] = 0;
        g_PlayerTargetTeam[id] = 0;

        cs_status_hide( id );
    }

    return PLUGIN_CONTINUE;
}


// Normal damage

public on_Damage( victim ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new iDamage = read_data( 2 );

    new Float:fDamageOrigin[3];
    read_data( 4, fDamageOrigin[0] );   // coord (x)
    read_data( 5, fDamageOrigin[1] );   // coord (y)
    read_data( 6, fDamageOrigin[2] );   // coord (z)

    new weapon, bodypart, headshot;
    new attacker = get_user_attacker( victim, weapon, bodypart );

    if ( bodypart == HIT_HEAD )
        headshot = 1;

	// Update kill assist information

	//WAR3_damage_assist( attacker, victim, iDamage );

    // Check for bomb explosion

    if ( get_gametime() - g_fBombTime < 1.0 && !attacker )
    {
        ITEM_ANKH_set( victim );

        new arg_write[1];
        arg_write[0] = victim;

        set_task( 0.1, "on_BombDeath", 0, arg_write, 1 );
    }

    // Check if impaled

    if ( g_PlayerImpaled[victim] && victim == attacker )
        UD_U_IMPALE_remove( victim );

    // Check if offensive skills should be ignored

    new bool:bAllowOffensive = true;

    if ( WAR3_player_valid( attacker ) && g_bPlayerDispelled[attacker] || attacker == g_Vip )
        bAllowOffensive = false;

    if ( g_bPlayerWalk[victim] || ( WAR3_player_valid( attacker ) && g_bPlayerWalk[attacker] ) || g_bPlayerInvis[victim] || g_iPlayerAvatar[victim] )
        bAllowOffensive = false;

    if ( g_bEvadeNextShot[victim] && ( get_gametime() - g_fEvasionTime[victim] ) >= EVASION_SHOTGAP )
        bAllowOffensive = false;

    if ( cs_get_weapon_type_( weapon ) & CS_WEAPON_TYPE_AUTOSNIPER )
        bAllowOffensive = false;

    // We need to make sure that we have a valid attacker

    if ( WAR3_player_valid( attacker ) && bAllowOffensive )
    {
        switch ( g_PlayerInfo[attacker][CURRENT_RACE] )
        {
            case RACE_UNDEAD:       UD_skills_offensive( attacker, victim, weapon, iDamage, headshot );
            case RACE_HUMAN:        HU_skills_offensive( attacker, victim, weapon, iDamage, headshot );
            case RACE_ORC:          OR_skills_offensive( attacker, victim, weapon, iDamage, headshot, fDamageOrigin );
            case RACE_NIGHTELF:     NE_skills_offensive( attacker, victim, weapon, iDamage, headshot );
        }
    }

    // Check if defensive skills should be ignored

    new bool:bAllowDefensive = true;

    if ( g_bPlayerDispelled[victim] || victim == g_Vip )
        bAllowDefensive = false;

    if ( WAR3_player_valid( attacker ) && bAllowDefensive )
    {
        switch ( g_PlayerInfo[victim][CURRENT_RACE] )
        {
            case RACE_UNDEAD:       UD_skills_defensive( attacker, victim, weapon, iDamage, headshot );
            case RACE_HUMAN:        HU_skills_defensive( victim );
            case RACE_ORC:          OR_skills_defensive( victim, iDamage );
            case RACE_NIGHTELF:     NE_skills_defensive( attacker, victim, weapon, iDamage, headshot );
        }
    }

    // Claws of Attack +6

    if ( WAR3_player_valid( attacker ) && g_PlayerInfo[attacker][CURRENT_ITEM] == ITEM_CLAWS && get_user_health( victim ) > 0 && get_user_team( attacker ) != get_user_team( victim ) && bAllowOffensive )
        ITEM_CLAWS_damage( attacker, victim, weapon, headshot );

    // Check if attacked by Avatar

    if ( WAR3_player_valid(attacker) && g_iPlayerAvatar[attacker] && get_user_team( attacker ) != get_user_team( victim ) )
        HU_U_AVATAR_damage( attacker, victim, weapon, headshot );

    // Check if attacked by Wind Walker

    if ( WAR3_player_valid( attacker ) && g_bPlayerWalk[attacker] && get_user_team( attacker ) != get_user_team( victim ) )
    {
        g_iWindwalkDamage[attacker] = iDamage;
        OR_U_WINDWALK_strike( attacker, victim, weapon, headshot );
    }

    // Invisibility cooldown

    SHARED_INVIS_cooldown( victim );

    // Bot ultimates ( defensive )

    if ( is_user_bot( victim ) && g_PlayerInfo[victim][CURRENT_ULTIMATE] && !g_fUltimateCooldown[victim] && victim != g_Vip )
    {
        new Float:fRandomNum = random_float( 0.0,1.0 );

        if ( fRandomNum <= BOT_ULTIMATECHANCE )
        {
            // Cast defensive ultimate

            if (  Ultimate_Target( victim ) & ULTIMATE_TARGET_SELF && Ultimate_Type( victim ) & ULTIMATE_TYPE_DEFENSIVE )
                cmd_Ultimate( victim );

            // Self-heal

            else if ( fRandomNum <= BOT_ULTIMATECHANCE && Ultimate_Target( victim ) & ULTIMATE_TARGET_SELF && Ultimate_Type( victim ) & ULTIMATE_TYPE_HEAL )
            {
                cmd_Ultimate( victim );
                cmd_Ultimate( victim );   // second time to actually activate.
            }
        }
    }

    return PLUGIN_CONTINUE;
}


// Normal death

public on_Death() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    new killer = read_data( 1 );
    new victim = read_data( 2 );
    new headshot = read_data( 3 );

    // Perform victim actions

    WAR3_death_victim( victim );

    // Check if suicide ( worldspawn/triggers )

    if ( !killer )
    {
        // Check if player was Impaled

        if ( g_PlayerImpaled[victim] )
        {
            WAR3_death( g_PlayerImpaled[victim], victim, CSW_IMPALE, 0 );
            g_PlayerImpaled[victim] = 0;
        }

        return PLUGIN_CONTINUE;
    }

    new weapon, iClip, iAmmo;
    weapon = get_user_weapon( killer, iClip, iAmmo );

    // Check if suicide ( slap/slay/grenade/etc )

    if ( killer == victim )
        return PLUGIN_CONTINUE;

    // Check if teamkill

    if ( get_user_team( killer ) == get_user_team( victim ) )
        XP_Kill_Teammate( killer );

    // Normal kill

    else
    {
        XP_Kill( killer, victim, weapon, headshot );
    }

    return PLUGIN_CONTINUE;
}


// Weapon fired / changed / etc

public on_CurWeapon( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // Player has now officially spawned at least once!

    if ( !g_bPlayerConnected[id] )
    {
        g_bPlayerConnected[id] = true;

        // Welcome message

        new szName[32];
        get_user_name( id, szName, 31 );

        client_print( id, print_chat, "^n %s %L", WAR3X_PREFIX, id, "CONSOLE_WELCOME_VERSION", szName, WAR3X_PLUGINNAME, WAR3X_VERSION, WAR3X_DATE );
        client_print( id, print_chat, "^n %s %L^n^n", WAR3X_PREFIX, id, "CONSOLE_WELCOME_COMMANDS" );
    }

    // Display race select menu

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
        menu_SelectRace( id );

    new weapon = read_data( 2 );
    new iAmmo = read_data( 3 );

    // Invisibility cooldown

    if ( is_weapon_fired( iAmmo, g_iPlayerAmmo[id][weapon] ) && weapon != CSW_KNIFE && weapon != CSW_C4 && cs_get_weapon_type_( weapon ) != CS_WEAPON_TYPE_GRENADE )
    {
        SHARED_INVIS_cooldown( id );
    }

    // Update run speed

    if ( !is_weapon_fired( iAmmo, g_iPlayerAmmo[id][weapon] ) || weapon == CSW_KNIFE || weapon == CSW_C4 || cs_get_weapon_type_( weapon ) == CS_WEAPON_TYPE_GRENADE )
    {
        WAR3_set_speed( id );
    }

    // Prevent moving when slowed/immobilized ( EXTRA VERIFICATION )

    if ( g_bPlayerCantMove[id] || g_bPlayerSlowed[id] )
    {
        WAR3_set_speed( id );
    }

    // Update ammo for current weapon;

    g_iPlayerAmmo[id][weapon] = iAmmo;

    // End Wind Walk

    if ( g_bPlayerWalk[id] && weapon != CSW_KNIFE )
    {
        new arg_write[2];
        arg_write[0] = id;

        OR_U_WINDWALK_out( arg_write );
    }

    // Prevent weapon switching while sleeping

    if ( g_bPlayerSleeping[id] && weapon != CSW_KNIFE )
    {
        engclient_cmd( id, "weapon_knife" );
        WAR3_set_speed( id );
    }

    // Set invisibility

    if ( g_bPlayerInvis[id] )
    {
        new task = TASK_INVIS + id;
        remove_task( task, 0 );
    }

    if ( !g_bInvisCooldown[id] )
        SHARED_INVIS_set( id );

    return PLUGIN_CONTINUE;
}


// Global new round

public on_FreezeStart() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    // Freeze start purges

    Purge_FreezeStart();

    // Upkeep

    Upkeep_Newround();

    // Retrieve skill restrictions

    get_pcvar_string( CVAR_restrict_skills, g_RestrictedSkills, 63 );

    new Players[32], iTotalPlayers;
    get_players( Players, iTotalPlayers );

    for ( new iPlayerNum; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new id = Players[iPlayerNum];

        // Display race select menu

        if ( !g_PlayerInfo[id][CURRENT_RACE] )
            menu_SelectRace( id );

        else if ( get_user_team( id ) && g_PlayerInfo[id][CURRENT_RACE] )
        {
            g_iPlayerRounds[id] += 1;

            // Check if race change attempt was made

            if ( g_PlayerInfo[id][CURRENT_NEXTRACE] )
            {
                new next_race = g_PlayerInfo[id][CURRENT_NEXTRACE] - 1;

                // Check if 0 xp

                if ( !g_iXPtotal[id][next_race] )
                {
                    new iLevel = get_pcvar_num( CVAR_startlevel_other );

                    if ( iLevel )
                    {
                        g_iXPtotal[id][next_race] = g_iLevelXp[iLevel];

                        new szMessage[128];
                        formatex( szMessage, 127, "^n%s %L^n", WAR3X_PREFIX, id, "INFO_LEVEL_OTHER", iLevel );

                        client_print( id, print_chat, "%s", szMessage );
                    }
                }

                // Update race info

                War3x_StoreSession( id, next_race );
                WAR3_hud_level( id );
                war3_chatskills( id, next_race, 1 );
                WAR3_check_skills( id );

                g_PlayerInfo[id][CURRENT_NEXTRACE] = 0;

                g_bEvadeNextShot[id] = false;
            }

            // Check for ankh

            if ( g_PlayerBackpack[id][CURRENT_PRIMARY] || g_PlayerBackpack[id][CURRENT_SECONDARY] || g_PlayerBackpack[id][CURRENT_HEGRENADE] || g_PlayerBackpack[id][CURRENT_FLASHBANG] || g_PlayerBackpack[id][CURRENT_SMOKEGRENADE] || g_PlayerBackpack[id][CURRENT_ARMOR] || g_PlayerBackpack[id][CURRENT_DEFUSE] )
            {
                if ( id == g_OldVip || g_PlayerInfo[id][CURRENT_ITEM] == ITEM_ANKH || g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC )
                    ITEM_ANKH_drop( id );

                else
                {
                    g_PlayerBackpack[id] = {0,0,0,0,0,0,0,0};
                }

                g_PlayerInfo[id][CURRENT_ITEM] = 0;
            }

            // Set invisibility

            if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_HUMAN || g_PlayerInfo[id][CURRENT_ITEM] == ITEM_CLOAK )
                SHARED_INVIS_set( id );

            // Set bonus health

            WAR3_set_health( id );

            // Make max health/armor adjustments

            WAR3_check_health( id );
            WAR3_check_armor( id );

            // Freeze player

            set_user_maxspeed( id, SPEED_IMMOBILIZE );

            // Subtract hostage kill(s)

            if ( g_iHostageKills[id] )
            {
                g_iHostageKills[id] -= XP_KILLHOSTAGE_SUBNUM;

                if ( g_iHostageKills[id] < 0 )
                   g_iHostageKills[id] = 0;
            }

            new iCurrentRace = g_PlayerInfo[id][CURRENT_RACE] - 1;

            WAR3_hud_level( id );
            war3_chatskills( id, iCurrentRace, 0 );
            WAR3_check_skills( id );

            // Hud messages

            new Float:fHoldTime = get_pcvar_float( CVAR_mp_freezetime );
            WAR3_hud_xp( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 0.0, fHoldTime );
            WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 0.0, fHoldTime );

            // Save XP

            if ( g_bSaveXp )
                Store_ActivePlayer( id );
        }

        // Reset round XP globals

        g_iXPkill[id][XP_TOTAL] += g_iXPkill[id][XP_ROUND];
        g_iXPkill[id][XP_ROUND] = 0;

        g_iXPobjective[id][XP_TOTAL] += g_iXPobjective[id][XP_ROUND];
        g_iXPobjective[id][XP_ROUND] =0;

        g_iXPsupport[id][XP_TOTAL] += g_iXPsupport[id][XP_ROUND];
        g_iXPsupport[id][XP_ROUND] = 0;
    }

    // Save player queue

    if ( g_bSaveXp && g_SaveQueue_iTotal >= QUEUES_BEFORE_SAVE )
        Store_Queue_All();

    return PLUGIN_HANDLED;
}


// Player spawn

public on_ResetHud( id ) {

    if ( !g_bWar3xEnabled || !is_user_connected( id ) || !g_bPlayerConnected[id] )
        return PLUGIN_CONTINUE;

    if ( g_bRoundEnd )
    {
        g_bRoundEnd = false;
        g_bFreezeTime = true;
    }

    // Clear target from follow mode

    g_PlayerTarget[id] = 0;
    g_PlayerTargetTeam[id] = 0;

    // Purge all tasks from previous round

    Purge_NewRound( id );

    // Remove item from inventory on round 1

    if ( g_iCurrentRound == 1 )
        g_PlayerInfo[id][CURRENT_ITEM] = ITEM_NONE;


    // Select race ( if bot )

    if ( is_user_bot( id ) && !g_PlayerInfo[id][CURRENT_RACE] )
    {
        new iRaceId = random_num( 0, TOTAL_RACES - 1 );
        War3x_StoreSession( id, iRaceId );

        // Give random level if longTerm

        if ( g_bSaveXp )
        {
            g_PlayerInfo[id][CURRENT_XP] = random_num( 0, LONGTERM_XP[TOTAL_LEVELS] );
            WAR3_check_level( id, 0, g_PlayerInfo[id][CURRENT_XP] );
        }
    }

    if ( g_PlayerInfo[id][CURRENT_RACE] )
    {
        // Activate skills ( pre-commencing )

        if ( g_iCurrentRound == 0 )
            WAR3_enable_skills( id );
    }

    return PLUGIN_CONTINUE;
}


// Global events

public on_World_Action() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new szLogAction[32];
    read_logargv( 1, szLogAction, 64 );

    // Round start ( after freeze time )

    if ( equal( szLogAction, "Round_Start" ) )
    {
        g_bFreezeTime = false;
        g_fBuyTime = get_gametime();

        // Ultimate warmup

        if ( get_pcvar_num( CVAR_ultimatewarmup ) )
        {
            new Float:fWarmup = get_pcvar_float( CVAR_ultimatewarmup );
            set_task( fWarmup, "Ultimate_Warmup", TASK_ULTIMATEWARMUP );

            g_bUltimateWarmup = true;
        }

        else
        {
            g_bUltimateWarmup = false;
        }

        // Item hud message

        set_task( HUDMESSAGE_FREQ_ITEM, "_WAR3_hud_item", TASK_SHOWITEM );

        new Players[32], iTotalPlayers;
        get_players( Players, iTotalPlayers, "a" );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new id = Players[iPlayerNum];

            // Activate player skills / ultimate

            WAR3_enable_skills( id );

            // Attempt to buy item if bot

            if ( is_user_bot( id ) && !g_PlayerInfo[id][CURRENT_ITEM] && get_pcvar_num( CVAR_shopmenus ) )
            {
                new Float:fRandomNum = random_float( 0.0,1.0 );

                if ( fRandomNum <= BOT_BUYCHANCE )
                {
                    new itemId = random_num( 1, TOTAL_SHOPITEMS );
                    new iITEM_COST = ITEM_COST[itemId];

                    if ( cs_get_user_money( id ) > iITEM_COST && !g_PlayerInfo[id][CURRENT_ITEM] )
                    {
                        cs_set_user_money( id, cs_get_user_money( id ) - iITEM_COST, 0 );

                        if ( itemId != ITEM_TOME )
                            g_PlayerInfo[id][CURRENT_ITEM] = itemId;
                    }
                }
            }
        }

        // Upkeep

        Upkeep_RoundStart();
    }


    // Round end

    else if ( equal( szLogAction, "Round_End" ) )
    {
        g_bRoundEnd = true;

        // Make sure freezetime cvar set > 0

        if ( CVAR_mp_freezetime && !get_pcvar_float( CVAR_mp_freezetime ) )
            set_pcvar_float( CVAR_mp_freezetime, 1.0 );

        if ( g_iCurrentRound == 0 )
            g_fBuyTime = get_gametime() + 4.0 + get_pcvar_float( CVAR_mp_freezetime );

        else
        {
            g_fBuyTime = get_gametime() + 6.0 + get_pcvar_float( CVAR_mp_freezetime );
        }

        new Players[32], iTotalPlayers;
        get_players( Players, iTotalPlayers );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new id = Players[iPlayerNum];

            // Make sure race selected

            if ( g_PlayerInfo[id][CURRENT_RACE] )
            {
                // Check if race change attempt was made

                if ( g_PlayerInfo[id][CURRENT_NEXTRACE] )
                {
                    new next_race = g_PlayerInfo[id][CURRENT_NEXTRACE] - 1;

                    new szMessage[64], szRaceName[32];
					LANG_GetRaceName( next_race + 1, id, szRaceName, 31, true )

                    formatex( szMessage, 63, "%L", id, "INFO_NEWRACENOW", szRaceName );

                    set_hudmessage( 255, 208, 0, HUDMESSAGE_POS_CENTER, HUDMESSAGE_POS_INFO, 0, 6.0, 1.0, 0.1, 0.5, HUDMESSAGE_CHAN_INFO );
                    show_hudmessage( id, "%s", szMessage );
                }

                else
                {
                    WAR3_hud_level( id );
                    WAR3_check_skills( id );
                }
            }

            else
            {
                menu_SelectRace( id );
            }
        }

        g_iCurrentRound ++;

        // Cancel ultimate warmup

        remove_task( TASK_ULTIMATEWARMUP, 0 );

        // Cancel upkeep timer

        remove_task( TASK_UPKEEP, 0 );

        // Set 5.0 second task for freeze start

        set_task( 5.0, "on_FreezeStart", TASK_NEWROUND );
    }

    // Game commencing

    else if ( equal( szLogAction, "Game_Commencing" ) )
    {
        g_bRoundEnd = true;
        g_iCurrentRound = 0;

        // Set 3.0 second task for freeze start

        set_task( 3.0, "on_FreezeStart", TASK_NEWROUND );

        // Cancel upkeep timer

        remove_task( TASK_UPKEEP, 0 );

        // Reset team wins for upkeep

        g_TeamWins[0] = 0;
        g_TeamWins[1] = 0;
    }

    // Round restart

    else if ( containi( szLogAction, "Restart_Round_" ) != -1 )
    {
        g_bRoundEnd = true;

        new Float:fRestartTimer = get_pcvar_float( CVAR_sv_restart );
        set_task( fRestartTimer, "on_FreezeStart", TASK_NEWROUND );

        if ( !g_bSaveXp )
        {
            new Players[32], iTotalPlayers;
            get_players( Players, iTotalPlayers );

            // Reset playerInfo array(s)

            for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
            {
                new id = Players[iPlayerNum];
                g_PlayerInfo[id] = {0,0,0,0,0,0,0,0,0,0,0,0};
            }
        }

        // Cancel upkeep timer

        remove_task( TASK_UPKEEP, 0 );
    }

    return PLUGIN_CONTINUE;
}


// Map change

public on_Intermission() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // maybe throw something in here (stats?)

    // Cancel upkeep timer

    remove_task( TASK_UPKEEP, 0 );

    return PLUGIN_CONTINUE;
}


// Player based

public on_Objective_Player() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    new szLogArg[64], szLogAction[64];
    new szPlayerName[32], id;

    read_logargv( 0, szLogArg, 63 );
    read_logargv( 2, szLogAction, 63 );
    parse_loguser( szLogArg, szPlayerName, 32, id );

    id = get_user_index( szPlayerName );


    // Hostage touched

    if ( equal( szLogAction, "Touched_A_Hostage" ) )
    {
        g_bTouchHostage[id] = true;
        XP_Hostage_Touch( id );
    }

    // Hostage rescued

    else if ( equal( szLogAction, "Rescued_A_Hostage" ) )
    {
        g_bHostageRescued = true;
        XP_Hostage_Rescued( id );
    }

    // Hostage killed

    else if ( equal( szLogAction, "Killed_A_Hostage" ) )
    {
        g_iHostageKills[id] += 1;
        XP_Hostage_Killed( id );
    }

    // Spawn with bomb / pickup bomb

    else if ( equal( szLogAction, "Spawned_With_The_Bomb" ) || equal( szLogAction, "Got_The_Bomb" ) )
    {
        g_BombCarrier = id;
        XP_Bomb_Pickup( id );
    }

    // Dropped bomb

    else if ( equal( szLogAction, "Dropped_The_Bomb" ) )
    {
        g_BombCarrier = 0;
        XP_Bomb_Drop( id );
    }

    // Bomb planted

    else if ( equal( szLogAction, "Planted_The_Bomb" ) )
    {
        g_bBombPlanted = true;
        g_fBombTime = get_gametime() + get_pcvar_float( CVAR_mp_c4timer );

        XP_Bomb_Plant( id );
        XP_Bomb_Share( id );

        g_BombPlanter = 0;
    }

    // Bomb defused

    else if ( equal( szLogAction, "Defused_The_Bomb" ) )
    {
        g_BombDefuser = 0;

        XP_Bomb_Defuse( id );
        XP_Defuse_Share( id );

        g_BombDefuser = 0;
    }

    // Bomb defusing

    else if ( equal( szLogAction, "Begin_Bomb_Defuse_With_Kit" ) || equal( szLogAction, "Begin_Bomb_Defuse_Without_Kit" ) )
    {
        // Moved to on_BarTime()
    }

    // VIP spawn

    else if ( equal( szLogAction, "Became_VIP" ) )
    {
        if ( g_Vip != id )
            g_OldVip = g_Vip;

        if ( is_user_alive( id ) && id != g_OldVip )
            ITEM_ANKH_set( id );

        g_Vip = id;
        XP_Vip_Spawn( id );
    }

    // VIP escaped

    else if ( equal( szLogAction, "Escaped_As_VIP" ) )
    {
        XP_Vip_Escape( id );
        XP_Vip_Share( id );
    }

    return PLUGIN_CONTINUE;
}


// Team based

public on_Objective_Team() {

    if ( !g_bWar3xEnabled || get_playersnum() < get_pcvar_num( CVAR_xp_minplayers ) )
        return PLUGIN_CONTINUE;

    new szLogTeam[32], szLogAction[64];

    read_logargv( 1, szLogTeam, 31 );
    read_logargv( 3, szLogAction,63 );

    // Team wins

    if ( equal( szLogAction, "CTs_Win" ) || equal( szLogAction, "Terrorists_Win" ) )
    {
        new szLogTwins[8], szTwins[8];
        new szLogCTwins[8], szCTwins[8];
        new szNull[8];

        read_logargv( 4, szLogCTwins, 7 );
        read_logargv( 5, szLogTwins, 7 );

        parse( szLogCTwins, szNull, 7, szCTwins, 7 );
        parse( szLogTwins, szNull, 7, szTwins, 7 );

        g_TeamWins[0] = str_to_num( szTwins );
        g_TeamWins[1] = str_to_num( szCTwins );
    }

    // Target bombed

    else if ( equal( szLogAction, "Target_Bombed" ) )
    {
        // Moved to on_TargetBombed() ( more reliable )
    }

    // Target saved

    else if ( equal( szLogAction, "Target_Saved" ) )
    {
        XP_Target_Saved_T();
        XP_Target_Saved_CT();
    }

    // VIP not escaped

    else if ( equal( szLogAction, "VIP_Not_Escaped" ) )
    {
        XP_Not_Escaped_T();
        XP_Not_Escaped_VIP();
    }

    // Hostages not rescued

    else if ( equal( szLogAction, "Hostages_Not_Rescued" ) )
    {
        XP_Not_Rescued_T();
        XP_Not_Rescued_CT();
    }

    // All hostages rescued

    else if ( equal( szLogAction, "All_Hostages_Rescued" ) )
    {
        XP_All_Hostages_CT();
    }

    return PLUGIN_CONTINUE;
}


// Bomb plant / befuse

public on_BarTime( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new BarTime = read_data( 1 );

    // Bomb plant

    if ( BarTime == 3 )
        g_BombPlanter = id;

    // Bomb defuse

    else if ( BarTime == 5 || BarTime == 10 )
        g_BombDefuser = id;

    // Finished planting/defusing

    else if ( BarTime == 0 )
    {
        if ( id == g_BombPlanter )
            g_BombPlanter = 0;

        if ( id == g_BombDefuser )
            g_BombDefuser = 0;
    }

    return PLUGIN_CONTINUE;
}


public on_ThrowGren( id ) {

    new iAmmoType = read_data( 1 );
    new iAmmoCount = read_data( 2 );

    // Pulverize

    if ( iAmmoCount == 0 && iAmmoType == CS_AMMOX_HEGRENADE )
    {
        if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC && g_PlayerInfo[id][CURRENT_SKILL2] )
        {
            // Get grenade id

            new szGrenade[64];
            formatex( szGrenade, 63, CS_W_MODEL, CS_MODEL_NAME[CSW_HEGRENADE] );

            new gIndex = cs_find_grenade( id, szGrenade );

            OR_S_PULVERIZE_trail( id, gIndex );
        }
    }

    return PLUGIN_CONTINUE;
}


public on_Health( id ) {

    // Regeneration

    if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC && !g_bPlayerRegen[id] && get_user_armor( id ) && ( get_user_health( id ) < WAR3_get_maxhealth( id ) || get_user_armor( id ) < WAR3_get_maxarmor( id ) ) )
    {
        OR_S_REGENERATION_set( id );
    }

    // Berserk speed ( make sure to avoid breaking effects / VIP restriction )

    if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC && g_PlayerInfo[id][CURRENT_SKILL1] )
    {
        WAR3_set_speed( id );
    }

    // Dispell harmful ultimate(s) ( bot )

    if ( is_user_bot( id ) )
    {
        if ( g_PlayerRooted[id] || g_PlayerStruck[id] || g_PlayerOnFire[id] )
        {
            if ( Ultimate_Type( id ) & ULTIMATE_TYPE_DISPELL && Ultimate_Target( id ) & ULTIMATE_TARGET_SELF )
            {
                cmd_Ultimate( id );
                cmd_Ultimate( id );     // second time to actually activate.
            }
        }
    }

    // Low health icon

    WAR3_icon_lowhealth( id );

    return PLUGIN_CONTINUE;
}


// Armor depletion

public on_ArmorZero( id ) {

    if ( !g_bWar3xEnabled || !get_user_team( id ) || !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_CONTINUE;

    if ( !g_bArmorDepleted[id] && !g_bFreezeTime )
        WAR3_armorskill_off( id );

    g_bArmorDepleted[id] = true;
    g_iPlayerArmor[id][ARMOR_AMOUNT] = 0;
    g_iPlayerArmor[id][ARMOR_TYPE] = 0;

    return PLUGIN_CONTINUE;
}


// Target bombed ( triggers even after round end )

public on_TargetBombed() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // BOMB EXPLOSION
    // read_data(1) = 17    (TE_SPRITE)
    // read_data(6) = -105  (scale in 0.1's)
    // read_data(7) = 17    (brightness)

    if ( get_gametime() - g_fBombTime < 1.0 )
        XP_Target_Bombed_T();

    return PLUGIN_CONTINUE;
}


// Weapon zoom

public on_Zoom( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    if ( read_data( 1 ) < 90 )
    {
        g_bPlayerZoomed[id] = true;
    }

    else
    {
        g_bPlayerZoomed[id] = false;
    }

    return PLUGIN_CONTINUE;
}


// Weapon pickup

public on_WeapPickup( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    return PLUGIN_CONTINUE;
}


// Use shield

public on_UseShield( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // Add some kind of invis nerf thing in here...

    return PLUGIN_CONTINUE;
}


// Target type ( friend / enemy / hostage )

public on_Target_Type( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    g_bNewTargetType[id] = true;
    read_data( 2, g_TargetFormat[id], 255 );

    return PLUGIN_CONTINUE;
}


// Player spawn ( war3x disabled )

public on_ResetHud_NOWAR3( id ) {

    if ( !g_bDisabledNotify[id] && is_user_alive( id ) )
    {
        client_print( id, print_chat, "%s %L", WAR3X_PREFIX, id, "INFO_DISABLEDMAP_CHAT", WAR3X_PLUGINNAME );
        g_bDisabledNotify[id] = true;
    }

    return PLUGIN_CONTINUE;
}


// Death to bomb explosion

public on_BombDeath( arg_read[1] ) {

    new id = arg_read[0];

    if ( !is_user_alive( id ) )
        WAR3_death_victim( id );

    else
    {
        g_PlayerBackpack[id] = {0,0,0,0,0,0,0,0};
    }

    return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. - //
