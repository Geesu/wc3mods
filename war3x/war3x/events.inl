/* - Events --------------------------------------------------- */


public on_StatusIcon( id ) {

    if ( !get_cvar_num( "war3x_enabled" ) )
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

    if ( equali( szStatusItem, "buyzone" ) )
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

    // Check for Speed Boost ( nature's blessing )

    if ( bArmorGiven )
    {
        WAR3_set_speed( id );
        return PLUGIN_HANDLED;
    }

    return PLUGIN_CONTINUE;
}


// Armor Picked Up

public on_ArmorPickup( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    if ( g_bArmorDepleted[id] )
        WAR3_armorskill_on( id );

    set_user_armor( id, WAR3_get_maxarmor( id ) );
    g_bArmorDepleted[id] = false;

    return PLUGIN_CONTINUE;
}


// Armor Type

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


// Target (team)

public on_Target_Team( id ) {

	if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    g_PlayerTarget[id] = 0;
    g_PlayerTargetTeam[id] = read_data( 2 );

    return PLUGIN_CONTINUE;
}


// Target (id)

public on_Target_Id( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    new iTargetId = read_data( 2 );
    new bool:bNewTarget;

    if ( g_PlayerTarget[id] != iTargetId )
        bNewTarget = true;

    g_PlayerTarget[id] = iTargetId;

    // Check if Ultimate is Charging

    if ( is_user_alive( id ) && g_iChargeUltimate[id] && iTargetId && !( Ultimate_Target( id ) & ULTIMATE_TARGET_SELFONLY ) )
    {
        Ultimate_Cast( id, iTargetId );
    }

    // Auto-Cast on teammate(s)

    else if ( iTargetId && g_PlayerInfo[id][CURRENT_ULTIMATE] && !g_fUltimateCooldown[id] && id != g_Vip )
    {
        // Prime/Cast Ultimate ( if bot )

        if ( is_user_bot( id ) )
        {
            // Heal Teammate

            if ( get_user_team( id ) == get_user_team( iTargetId ) && Ultimate_Type( id ) & ULTIMATE_TYPE_HEAL )
            {
                new iMaxHealth = WAR3_get_maxhealth( iTargetId );
                new iHealth = get_user_health( iTargetId );

                if ( float( iHealth ) / float( iMaxHealth ) <= HEAL_NOTIFYHEALTH )
                    cmd_Ultimate( id );
            }

            else
            {
                new Float:fRandomNum = random_float( 0.0,1.0 );

                if ( fRandomNum <= BOT_ULTIMATECHANCE )
                {
                    // Damage Enemy

                    if ( get_user_team( id ) != get_user_team( iTargetId ) && Ultimate_Target( id ) & ULTIMATE_TARGET_ENEMY && Ultimate_Type( id ) & ULTIMATE_TYPE_OFFENSIVE )
                        cmd_Ultimate( id );
                }
            }
        }

        else if ( get_user_team( id ) == get_user_team( iTargetId ) && g_PlayerOptions[id][OPTION_AUTOCAST] )
        {
            if ( Ultimate_Type( id ) & ULTIMATE_TYPE_HEAL && WAR3_is_lowhealth( iTargetId ) && WAR3_can_heal( id, iTargetId ) )
                cmd_Ultimate( id );

            /* DISPELLS NOT IMPLEMENTED
            else if ( Ultimate_Type( id ) & ULTIMATE_TYPE_DISPELL && Ultimate_Target( id ) & ULTIMATE_TARGET_TEAMMATE )
            */
        }
    }

    // Team race icons

    if ( is_user_alive( id ) && g_PlayerOptions[id][OPTION_RACEICONS] && !g_bPlayerSaveMe[iTargetId] && !g_bPlayerDispellMe[iTargetId] )
    {
        if ( iTargetId && g_PlayerTargetTeam[id] == CS_TEAMID_TEAMMATE )
        {
            new IconIndex;

            switch ( g_PlayerInfo[iTargetId][CURRENT_RACE] )
            {
                case RACE_UNDEAD:       IconIndex = ICON_UNDEAD;
                case RACE_HUMAN:        IconIndex = ICON_HUMAN;
                case RACE_ORC:          IconIndex = ICON_ORC;
                case RACE_NIGHTELF:     IconIndex = ICON_NIGHTELF;
            }

            if ( IconIndex )
                Create_TE_PLAYERATTACHMENT( id, iTargetId, 44, IconIndex, 5 );
        }
    }

    // Show Spectator Info

    else if ( !is_user_alive( id ) && bNewTarget && iTargetId && iTargetId != id && g_bXPfetched[id] )
    {
        new iResNum = g_PlayerOptions[id][OPTION_RESOLUTION];
        new Float:fXpos = 13.0 / STEAM_RESOLUTION[iResNum][_X];
        new Float:fYpos = float( STEAM_RESOLUTION[iResNum][_Y] - 78 + 5 ) / float( STEAM_RESOLUTION[iResNum][_Y] );

        new iHealth = get_user_health( iTargetId );

        if ( g_bEvadeNextShot[iTargetId] )
            iHealth -= 1024;

        new szMessage[256], szM1[256], szM2[256], szM3[256];
        format( szM3, 255, INFO_FOLLOW_MESSAGE_3, iHealth, get_user_armor( iTargetId ), cs_get_user_money( iTargetId ) );

        if ( g_PlayerInfo[iTargetId][CURRENT_RACE] )
        {
            new iRaceNum = g_PlayerInfo[iTargetId][CURRENT_RACE] - 1;
            new iItemNum = g_PlayerInfo[iTargetId][CURRENT_ITEM];
            new iXp      = g_PlayerInfo[iTargetId][CURRENT_XP];
            new iLevel   = WAR3_get_level( iXp );
            new iNextXp  = WAR3_get_next_xp( iXp );

            new iLastlvlXp = WAR3_get_last_xp( iXp );
            new iNextlvlXp = WAR3_get_next_xp( iXp );

            new Float:fPercentage = ( float( iXp ) - float( iLastlvlXp ) ) / ( float( iNextlvlXp ) - float( iLastlvlXp ) ) * 100.0;
            new iPercentage = floatround( fPercentage );

			// Get Item and Race Name

			new szItemName[32], szRaceName[32];
			
			LANG_GetItemName( iItemNum, SHOP_COMMON, id, szItemName, 31 );
			LANG_GetRaceName ( iRaceNum + 1, id, szRaceName, 31, true )

            // Create Message

            format( szM1, 255, INFO_FOLLOW_MESSAGE_1, szRaceName, iLevel, iXp, iNextXp, iPercentage, "%%" );
            format( szM2, 255, INFO_FOLLOW_MESSAGE_2, szItemName, "None" );

            format( szMessage, 255, "%s^n%s^n%s", szM1, szM2, szM3 );

            // Show Message

            set_hudmessage( 255, 255, 255, fXpos, fYpos, 0, 6.0, 500.0, 0.1, 0.5, HUDMESSAGE_CHAN_DEAD );
            show_hudmessage( id, szMessage );
        }

        else
        {
            format( szMessage, 255, "%s^n%s", INFO_FOLLOW_NORACE, szM3 );

            set_hudmessage( 255, 255, 255, fXpos, fYpos, 0, 6.0, 500.0, 0.1, 0.5, HUDMESSAGE_CHAN_DEAD );
            show_hudmessage( id, szMessage );
        }
    }

    // Enemy

    if ( g_PlayerTargetTeam[id] == CS_TEAMID_ENEMY )
    {
        if ( get_cvar_num( "mp_playerid" ) == CS_MP_PLAYERID_SHOWALL && entity_get_int( iTargetId, EV_INT_rendermode ) != kRenderTransTexture )
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
        if ( get_cvar_num( "mp_playerid" ) != CS_MP_PLAYERID_SHOWNONE )
            cs_status_target( id );

        else
        {
            cs_status_hide( id );
        }
    }

    // Hostage

    else if ( g_PlayerTargetTeam[id] == CS_TEAMID_NEUTRAL )
    {
        if ( get_cvar_num( "mp_playerid" ) != CS_MP_PLAYERID_SHOWNONE )
            cs_status_target( id );

        else
        {
            cs_status_hide( id );
        }
    }

    // No Target

    else
    {
        g_PlayerTarget[id] = 0;
        g_PlayerTargetTeam[id] = 0;

        cs_status_hide( id );
    }

    return PLUGIN_CONTINUE;
}


// Normal Damage

public on_Damage( victim ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new iDamage = read_data( 2 );

    new Float:fDamageOrigin[3];
    read_data( 4, fDamageOrigin[0] );   // coord (x)
    read_data( 5, fDamageOrigin[1] );   // coord (y)
    read_data( 6, fDamageOrigin[2] );   // coord (z)

    new weapon, Bodypart, Headshot;
    new attacker = get_user_attacker( victim, weapon, Bodypart );

    if ( Bodypart == HIT_HEAD )
        Headshot = 1;

    // Check for Bomb Explosion

    if ( get_gametime() - g_fBombTime < 1.0 && !attacker )
    {
        ITEM_ANKH_set( victim );

        new parm_Bomb[1];
        parm_Bomb[0] = victim;

        set_task( 0.1, "on_BombDeath", 0, parm_Bomb, 1 );
    }

    // Check if Impaled

    if ( g_PlayerImpaled[victim] && victim == attacker )
        UD_U_IMPALE_remove( victim );

    // Check if Offensive Skills Should Be Ignored

    new bool:bAllowOffensive = true;

    if ( WAR3_player_valid(attacker) && g_bPlayerDispelled[attacker] || attacker == g_Vip )
        bAllowOffensive = false;

    if ( g_bPlayerWalk[victim] || (WAR3_player_valid(attacker) && g_bPlayerWalk[attacker]) || g_bPlayerInvis[victim] || g_iPlayerAvatar[victim] )
        bAllowOffensive = false;

    if ( g_bEvadeNextShot[victim] && ( get_gametime() - g_fEvasionTime[victim] ) >= EVASION_SHOTGAP )
        bAllowOffensive = false;

    if ( cs_get_weapon_type_( weapon ) & CS_WEAPON_TYPE_AUTOSNIPER )
        bAllowOffensive = false;

    // We need to make sure that we have a valid attacker ID
    if ( WAR3_player_valid(attacker) && bAllowOffensive )
    {
        switch ( g_PlayerInfo[attacker][CURRENT_RACE] )
        {
            case RACE_UNDEAD:       UD_skills_offensive( attacker, victim, weapon, iDamage, Headshot );
            case RACE_HUMAN:        HU_skills_offensive( attacker, victim, weapon, iDamage, Headshot );
            case RACE_ORC:          OR_skills_offensive( attacker, victim, weapon, iDamage, Headshot, fDamageOrigin );
            case RACE_NIGHTELF:     NE_skills_offensive( attacker, victim, weapon, iDamage, Headshot );
        }
    }

    // Check if Defensive Skills Should Be Ignored

    new bool:bAllowDefensive = true;

    if ( g_bPlayerDispelled[victim] || victim == g_Vip )
        bAllowDefensive = false;

    if ( WAR3_player_valid(attacker) && bAllowDefensive )
    {
        switch ( g_PlayerInfo[victim][CURRENT_RACE] )
        {
            case RACE_UNDEAD:       UD_skills_defensive( attacker, victim, weapon, iDamage, Headshot );
            case RACE_HUMAN:        HU_skills_defensive( victim );
            case RACE_ORC:          OR_skills_defensive( victim, iDamage );
            case RACE_NIGHTELF:     NE_skills_defensive( attacker, victim, weapon, iDamage, Headshot );
        }
    }

    // Claws of Attack +6

    if ( WAR3_player_valid(attacker) && g_PlayerInfo[attacker][CURRENT_ITEM] == ITEM_CLAWS && get_user_health( victim ) > 0 && get_user_team( attacker ) != get_user_team( victim ) && bAllowOffensive )
        ITEM_CLAWS_damage( attacker, victim, weapon, Headshot );

    // Check if Attacked by Avatar

    if ( WAR3_player_valid(attacker) && g_iPlayerAvatar[attacker] && get_user_team( attacker ) != get_user_team( victim ) )
        HU_U_AVATAR_damage( attacker, victim, weapon, Headshot );

    // Check if Attacked by Wind Walker

    if ( WAR3_player_valid(attacker) && g_bPlayerWalk[attacker] && get_user_team( attacker ) != get_user_team( victim ) )
    {
        g_iWindwalkDamage[attacker] = iDamage;
        OR_U_WINDWALK_strike( attacker, victim, weapon, Headshot );
    }

    // Invisibility Cooldown

    SHARED_INVIS_cooldown( victim );

    // Bot Ultimates ( Defensive )

    if ( is_user_bot( victim ) && g_PlayerInfo[victim][CURRENT_ULTIMATE] && !g_fUltimateCooldown[victim] && victim != g_Vip )
    {
        new Float:fRandomNum = random_float( 0.0,1.0 );

        if ( fRandomNum <= BOT_ULTIMATECHANCE )
        {
            // Cast Defensive Ultimate

            if (  Ultimate_Target( victim ) & ULTIMATE_TARGET_SELF && Ultimate_Type( victim ) & ULTIMATE_TYPE_DEFENSIVE )
                cmd_Ultimate( victim );

            // Self-Heal

            else if ( fRandomNum <= BOT_ULTIMATECHANCE && Ultimate_Target( victim ) & ULTIMATE_TARGET_SELF && Ultimate_Type( victim ) & ULTIMATE_TYPE_HEAL )
            {
                cmd_Ultimate( victim );
                cmd_Ultimate( victim );   // second time to actually activate.
            }
        }
    }

    return PLUGIN_CONTINUE;
}


// Normal Death

public on_Death() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    new killerId = read_data( 1 );
    new victim = read_data( 2 );
    new Headshot = read_data( 3 );

    // Perform Victim Actions

    WAR3_death_victim( victim );

    // Check if Suicide ( worldspawn/triggers )

    if ( !killerId )
    {
        // Check if Player was Impaled

        if ( g_PlayerImpaled[victim] )
        {
            WAR3_death( g_PlayerImpaled[victim], victim, CSW_IMPALE, 0 );
            g_PlayerImpaled[victim] = 0;
        }

        return PLUGIN_CONTINUE;
    }

    new weapon, iClip, iAmmo;
    weapon = get_user_weapon( killerId, iClip, iAmmo );

    // Check if Suicide ( slap/slay/grenade/etc )

    if ( killerId == victim )
        return PLUGIN_CONTINUE;

    // Check if Teamkill

    if ( get_user_team( killerId ) == get_user_team( victim ) )
        XP_Kill_Teammate( killerId );

    // Normal Kill

    else
    {
        XP_Kill( killerId, victim, weapon, Headshot );
    }

    return PLUGIN_CONTINUE;
}


// Weapon fired / changed / etc

public on_CurWeapon( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // Player has now officially spawned at least once!

    if ( !g_bPlayerConnected[id] )
        g_bPlayerConnected[id] = true;

    // Display Race Select Menu

    if ( !g_PlayerInfo[id][CURRENT_RACE] )
        menu_SelectRace( id );

    new weapon = read_data( 2 );
    new iAmmo = read_data( 3 );

    // Invisibility Cooldown

    if ( is_weapon_fired( iAmmo, g_iPlayerAmmo[id][weapon] ) && weapon != CSW_KNIFE && weapon != CSW_C4 && cs_get_weapon_type_( weapon ) != CS_WEAPON_TYPE_GRENADE )
    {
        SHARED_INVIS_cooldown( id );
    }

    // Update run speed

    if ( !is_weapon_fired( iAmmo, g_iPlayerAmmo[id][weapon] ) || weapon == CSW_KNIFE || weapon == CSW_C4 || cs_get_weapon_type_( weapon ) == CS_WEAPON_TYPE_GRENADE )
    {
        WAR3_set_speed( id );
    }

    // Update Ammo for Current weapon;

    g_iPlayerAmmo[id][weapon] = iAmmo;

    // End Windwalk

    if ( g_bPlayerWalk[id] && weapon != CSW_KNIFE )
    {
        new parm_Walk[2];
        parm_Walk[0] = id;

        OR_U_WINDWALK_out( parm_Walk );
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


// Global New Round

public on_FreezeStart() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    // Freeze Start Purges

    Purge_FreezeStart();

    // Upkeep

    Upkeep_Newround();

    // Retrieve skill restrictions

    get_cvar_string( "war3x_restrict_skills", g_RestrictedSkills, 63 );

    new Players[32], iTotalPlayers;
    get_players( Players, iTotalPlayers );

    for ( new iPlayerNum; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new id = Players[iPlayerNum];

        // Display Race Select Menu

        if ( !g_PlayerInfo[id][CURRENT_RACE] )
            menu_SelectRace( id );

        else if ( get_user_team( id ) && g_PlayerInfo[id][CURRENT_RACE] )
        {
            g_iPlayerRounds[id] += 1;

            // Check if Race Change Attempt was Made

            if ( g_PlayerInfo[id][CURRENT_NEXTRACE] )
            {
                new iNextRaceId = g_PlayerInfo[id][CURRENT_NEXTRACE] - 1;

                // Check if 0 xp

                if ( !g_iXPtotal[id][iNextRaceId] )
                {
                    new iLevel = get_pcvar_num( CVAR_startlevel_other );

                    if ( iLevel )
                    {
                        g_iXPtotal[id][iNextRaceId] = g_iLevelXp[iLevel];

                        new szMessage[128];
                        format( szMessage, 127, INFO_LEVEL_OTHER, iLevel );

                        client_print( id, print_chat, szMessage );
                    }
                }

                // Update Race Info

                War3x_StoreSession( id, iNextRaceId );
                WAR3_hud_level( id );
                war3_chatskills( id, iNextRaceId, 1 );
                WAR3_check_skills( id );

                g_PlayerInfo[id][CURRENT_NEXTRACE] = 0;

                g_bEvadeNextShot[id] = false;
            }

            // Check for Ankh

            if ( g_PlayerBackpack[id][CURRENT_PRIMARY] || g_PlayerBackpack[id][CURRENT_SECONDARY] || g_PlayerBackpack[id][CURRENT_HEGRENADE] || g_PlayerBackpack[id][CURRENT_FLASHBANG] || g_PlayerBackpack[id][CURRENT_SMOKEGRENADE] || g_PlayerBackpack[id][CURRENT_ARMOR] || g_PlayerBackpack[id][CURRENT_DEFUSE] )
            {
                if ( id == g_OldVip || g_PlayerInfo[id][CURRENT_ITEM] == ITEM_ANKH || g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC )
                    ITEM_ANKH_drop( id );

                // Race Change from Orc

                else
                {
                    g_PlayerBackpack[id] = {0,0,0,0,0,0,0,0};
                }

                g_PlayerInfo[id][CURRENT_ITEM] = 0;
            }

            // Set Invisibility

            if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_HUMAN || g_PlayerInfo[id][CURRENT_ITEM] == ITEM_CLOAK )
                SHARED_INVIS_set( id );

            // Set Bonus Health

            WAR3_set_health( id );

            // Make Max Health/Armor Adjustments

            WAR3_check_health( id );
            WAR3_check_armor( id );

            // Freeze Player

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

            // Hudmessages

            new Float:fHoldTime = get_cvar_float( "mp_freezetime" );
            WAR3_hud_xp( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 0.0, fHoldTime );
            WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 0.0, fHoldTime );

            // Save XP

            if ( g_bSaveXp )
                Store_ActivePlayer( id );
        }

        // Reset Round XP Globals

        g_iXPkill[id][XP_TOTAL] += g_iXPkill[id][XP_ROUND];
        g_iXPkill[id][XP_ROUND] = 0;

        g_iXPobjective[id][XP_TOTAL] += g_iXPobjective[id][XP_ROUND];
        g_iXPobjective[id][XP_ROUND] =0;

        g_iXPsupport[id][XP_TOTAL] += g_iXPsupport[id][XP_ROUND];
        g_iXPsupport[id][XP_ROUND] = 0;
    }

    // Save Player Queue

    if ( g_bSaveXp && g_SaveQueue_iTotal >= QUEUES_BEFORE_SAVE )
        Store_Queue_All();

    return PLUGIN_HANDLED;
}


// Player Spawn

public on_ResetHud( id ) {

    if ( !g_bWar3xEnabled || !is_user_connected( id ) )
        return PLUGIN_CONTINUE;

    if ( g_bRoundEnd )
    {
        g_bRoundEnd = false;
        g_bFreezeTime = true;
    }

    new parm_Spawn[1];
    parm_Spawn[0] = id;

    // Clear Target from Follow Mode

    g_PlayerTarget[id] = 0;
    g_PlayerTargetTeam[id] = 0;

    // Purge all tasks from previous round

    Purge_NewRound( id );

    // Remove Item from Inventory on Round 1

    if ( g_iCurrentRound == 1 )
        g_PlayerInfo[id][CURRENT_ITEM] = ITEM_NONE;


    // Select Race ( if bot )

    if ( is_user_bot( id ) && !g_PlayerInfo[id][CURRENT_RACE] )
    {
        new iRaceId = random_num( 0, TOTAL_RACES - 1 );
        War3x_StoreSession( id, iRaceId );

        // Give Random Level if LongTerm

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


// Global Events

public on_World_Action() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    new szLogAction[32];
    read_logargv( 1, szLogAction, 64 );

    // Round Start ( after freeze time )

    if ( equal( szLogAction, "Round_Start" ) )
    {
        g_bFreezeTime = false;
        g_fBuyTime = get_gametime();

        // Ultimate Warmup

        if ( get_cvar_num( "war3x_ultimatewarmup" ) )
        {
            new Float:fWarmup = get_cvar_float( "war3x_ultimatewarmup" );
            set_task( fWarmup, "Ultimate_Warmup", TASK_ULTIMATEWARMUP );

            g_bUltimateWarmup = true;
        }

        else
        {
            g_bUltimateWarmup = false;
        }

        // Item Hudmessage

        set_task( HUDMESSAGE_FREQ_ITEM, "_WAR3_hud_item", TASK_SHOWITEM );

        new Players[32], iTotalPlayers;
        get_players( Players, iTotalPlayers, "a" );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new id = Players[iPlayerNum];

            // Activate Player skills / Ultimate

            WAR3_enable_skills( id );

            // Attempt to Buy Item if Bot

            if ( is_user_bot( id ) && !g_PlayerInfo[id][CURRENT_ITEM] && get_cvar_num( "war3x_shopmenus" ) )
            {
                new Float:fRandomNum = random_float( 0.0,1.0 );

                if ( fRandomNum <= BOT_BUYCHANCE )
                {
                    new itemId = random_num( 1, TOTAL_SHOPITEMS );
                    new iItemCost = ITEMCOST[itemId];

                    if ( cs_get_user_money( id ) > iItemCost && !g_PlayerInfo[id][CURRENT_ITEM] )
                    {
                        cs_set_user_money( id, cs_get_user_money( id ) - iItemCost, 0 );

                        if ( itemId != ITEM_TOME )
                            g_PlayerInfo[id][CURRENT_ITEM] = itemId;
                    }
                }
            }
        }

        // Upkeep

        Upkeep_RoundStart();
    }


    // Round End

    else if ( equal( szLogAction, "Round_End" ) )
    {
        g_bRoundEnd = true;

        // Make sure freezetime cvar set > 0

        if ( !get_cvar_float( "mp_freezetime" ) )
            set_cvar_float( "mp_freezetime", 1.0 );

        if ( g_iCurrentRound == 0 )
            g_fBuyTime = get_gametime() + 4.0 + get_cvar_float( "mp_freezetime" );

        else
        {
            g_fBuyTime = get_gametime() + 6.0 + get_cvar_float( "mp_freezetime" );
        }

        new Players[32], iTotalPlayers;
        get_players( Players, iTotalPlayers );

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new id = Players[iPlayerNum];

            // Make Sure Race Selected

            if ( g_PlayerInfo[id][CURRENT_RACE] )
            {
                // Check if Race Change Attempt was Made

                if ( g_PlayerInfo[id][CURRENT_NEXTRACE] )
                {
                    new iNextRaceId = g_PlayerInfo[id][CURRENT_NEXTRACE] - 1;

                    new szMessage[64], szRaceName[32];
					LANG_GetRaceName( iNextRaceId + 1, id, szRaceName, 31, true )

                    format( szMessage, 63, INFO_NEWRACENOW, szRaceName );

                    set_hudmessage( 255, 208, 0, HUDMESSAGE_POS_CENTER, HUDMESSAGE_POS_INFO, 0, 6.0, 1.0, 0.1, 0.5, HUDMESSAGE_CHAN_INFO );
                    show_hudmessage( id, szMessage );
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

        // Cancel Ultimate Warmup

        remove_task( TASK_ULTIMATEWARMUP, 0 );

        // Cancel Upkeep Timer

        remove_task( TASK_UPKEEP, 0 );

        // Set 5.0 Second task for Freeze Start

        set_task( 5.0, "on_FreezeStart", TASK_NEWROUND );
    }

    // Game Commencing

    else if ( equal( szLogAction, "Game_Commencing" ) )
    {
        g_bRoundEnd = true;
        g_iCurrentRound = 0;

        // Set 3.0 Second task for Freeze Start

        set_task( 3.0, "on_FreezeStart", TASK_NEWROUND );

        // Cancel Upkeep Timer

        remove_task( TASK_UPKEEP, 0 );

        // Reset Team Wins for Upkeep

        g_TeamWins[0] = 0;
        g_TeamWins[1] = 0;
    }

    // Round Restart

    else if ( containi( szLogAction, "Restart_Round_" ) != -1 )
    {
        g_bRoundEnd = true;

        new Float:fRestartTimer = get_cvar_float( "sv_restart" );
        set_task( fRestartTimer, "on_FreezeStart", TASK_NEWROUND );

        if ( !g_bSaveXp )
        {
            new Players[32], iTotalPlayers;
            get_players( Players, iTotalPlayers );

            // Reset PlayerInfo Array(s)

            for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
            {
                new id = Players[iPlayerNum];
                g_PlayerInfo[id] = {0,0,0,0,0,0,0,0,0,0,0,0};
            }
        }

        // Cancel Upkeep Timer

        remove_task( TASK_UPKEEP, 0 );
    }

    return PLUGIN_CONTINUE;
}


// Map Change

public on_Intermission() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // maybe throw something in here (stats?)

    // Cancel Upkeep Timer

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


    // Hostage Touched

    if ( equal( szLogAction, "Touched_A_Hostage" ) )
    {
        g_bTouchHostage[id] = true;
        XP_Hostage_Touch( id );
    }

    // Hostage Rescued

    else if ( equal( szLogAction, "Rescued_A_Hostage" ) )
    {
        g_bHostageRescued = true;
        XP_Hostage_Rescued( id );
    }

    // Hostage Killed

    else if ( equal( szLogAction, "Killed_A_Hostage" ) )
    {
        g_iHostageKills[id] += 1;
        XP_Hostage_Killed( id );
    }

    // Spawn with Bomb / Pickup Bomb

    else if ( equal( szLogAction, "Spawned_With_The_Bomb" ) || equal( szLogAction, "Got_The_Bomb" ) )
    {
        g_BombCarrier = id;
        XP_Bomb_Pickup( id );
    }

    // Dropped Bomb

    else if ( equal( szLogAction, "Dropped_The_Bomb" ) )
    {
        g_BombCarrier = 0;
        XP_Bomb_Drop( id );
    }

    // Bomb Planted

    else if ( equal( szLogAction, "Planted_The_Bomb" ) )
    {
        g_bBombPlanted = true;
        g_fBombTime = get_gametime() + get_cvar_float( "mp_c4timer" );

        XP_Bomb_Plant( id );
        XP_Bomb_Share( id );

        g_BombPlanter = 0;
    }

    // Bomb Defused

    else if ( equal( szLogAction, "Defused_The_Bomb" ) )
    {
        g_BombDefuser = 0;

        XP_Bomb_Defuse( id );
        XP_Defuse_Share( id );

        g_BombDefuser = 0;
    }

    // Bomb Defusing

    else if ( equal( szLogAction, "Begin_Bomb_Defuse_With_Kit" ) || equal( szLogAction, "Begin_Bomb_Defuse_Without_Kit" ) )
    {
        // Moved to on_BarTime()
    }

    // VIP Spawn

    else if ( equal( szLogAction, "Became_VIP" ) )
    {
        if ( g_Vip != id )
            g_OldVip = g_Vip;

        if ( is_user_alive( id ) && id != g_OldVip )
            ITEM_ANKH_set( id );

        g_Vip = id;
        XP_Vip_Spawn( id );
    }

    // VIP Escaped

    else if ( equal( szLogAction, "Escaped_As_VIP" ) )
    {
        XP_Vip_Escape( id );
        XP_Vip_Share( id );
    }

    return PLUGIN_CONTINUE;
}



// Team based

public on_Objective_Team() {

    if ( !g_bWar3xEnabled || get_playersnum() < get_cvar_num( "war3x_xp_minplayers" ) )
        return PLUGIN_CONTINUE;


    new szLogTeam[32], szLogAction[64];

    read_logargv( 1, szLogTeam, 31 );
    read_logargv( 3, szLogAction,63 );

    // Team Wins

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

    // Target Bombed

    else if ( equal( szLogAction, "Target_Bombed" ) )
    {
        // Moved to on_TargetBombed() ( more reliable )
    }

    // Target Saved

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

    // All hostages Rescued

    else if ( equal( szLogAction, "All_Hostages_Rescued" ) )
    {
        XP_All_Hostages_CT();
    }

    return PLUGIN_CONTINUE;
}


// Bomb Plant / Defuse

public on_BarTime( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    new BarTime = read_data( 1 );

    // Bomb Plant

    if ( BarTime == 3 )
        g_BombPlanter = id;

    // Bomb Defuse

    else if ( BarTime == 5 || BarTime == 10 )
        g_BombDefuser = id;

    // Finished Planting/Defusing

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
            format( szGrenade, 63, CS_W_MODEL, CS_MODEL_NAME[CSW_HEGRENADE] );

            new gIndex = cs_find_grenade( id, szGrenade );

            OR_S_PULVERIZE_trail( id, gIndex );
        }
    }

    return PLUGIN_CONTINUE;
}


public on_Health( id ) {

    // Regeneration

    if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC && !g_bPlayerRegen[id] && ( get_user_health( id ) < WAR3_get_maxhealth( id ) || get_user_armor( id ) < WAR3_get_maxarmor( id ) ) )
    {
        OR_S_REGENERATION_set( id );
    }

    // Berserk Check

    if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC )
    {
        OR_S_BERSERK_speed( id );
    }

    // Dispell Harmful Ultimate(s) ( bot )

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

    // Save Me!

    WAR3_icon_lowhealth( id );

    return PLUGIN_CONTINUE;
}


public on_ArmorZero( id ) {

    if ( !g_bWar3xEnabled || !get_user_team( id ) || !g_PlayerInfo[id][CURRENT_RACE] )
        return PLUGIN_CONTINUE;

    if ( !g_bArmorDepleted[id] && !g_bFreezeTime )
        WAR3_armorskill_off( id );

    g_bArmorDepleted[id] = true;
    g_iPlayerArmor[id][ARMOR_AMMOUNT] = 0;
    g_iPlayerArmor[id][ARMOR_TYPE] = 0;

    return PLUGIN_CONTINUE;
}


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


public on_Zoom( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    if ( read_data(1) < 90 )
    {
        g_bPlayerZoomed[id] = true;
    }

    else
    {
        g_bPlayerZoomed[id] = false;
    }

    return PLUGIN_CONTINUE;
}


public on_WeapPickup( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    return PLUGIN_CONTINUE;
}



public on_UseShield( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // Add some kind of invis nerf thing in here...

    return PLUGIN_CONTINUE;
}


public on_Target_Type( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;


    g_bNewTargetType[id] = true;
    read_data( 2, g_TargetFormat[id], 255 );

    return PLUGIN_CONTINUE;
}


public on_ResetHud_NOWAR3( id ) {

    if ( !g_bDisabledNotify[id] && is_user_alive( id ) )
    {
        client_print( id, print_chat, INFO_DISABLEDMAP_CHAT );
        g_bDisabledNotify[id] = true;
    }

    return PLUGIN_CONTINUE;
}


public on_BombDeath( parm_Bomb[1] ) {

    new id = parm_Bomb[0];

    if ( !is_user_alive( id ) )
        WAR3_death_victim( id );

    else
    {
        g_PlayerBackpack[id] = {0,0,0,0,0,0,0,0};
    }

    return PLUGIN_HANDLED;
}

