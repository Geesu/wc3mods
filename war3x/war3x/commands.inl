/* - Client Commands -------------------------------------------- */


// Ultimate Command

public cmd_Ultimate( iCasterId ) {
#if ADVANCED_DEBUG
	log_function("public cmd_Ultimate( iCasterId ) {");
#endif

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new iTargetId = g_PlayerTarget[iCasterId];

    // Press when dead ( target skills ) *REMOVED TEMPORARILY*

    if ( !is_user_alive( iCasterId ) )
    {
        //motd_Skills( iCasterId, iTargetId );
        return PLUGIN_HANDLED;
    }

    // Press when restricted

    else if ( get_cvar_bitsum( "war3x_restrict_ultimates" ) & WAR3_get_ult_flag( g_PlayerInfo[iCasterId][CURRENT_RACE], g_PlayerInfo[iCasterId][CURRENT_ULTIMATE] ) )
    {
        new szMessage[128];
        copy( szMessage, 127, ULTIMATE_RESTRICTED );

        WAR3_status_text( iCasterId, szMessage, 0.5 );

        return PLUGIN_HANDLED;
    }

    // Press when sleeping

    else if ( g_bPlayerSleeping[iCasterId] )
    {
        new szMessage[128];
        copy( szMessage, 127, ULTIMATE_NOCAST_SLEEP );

        WAR3_status_text( iCasterId, szMessage, 1.0 );
    }

    // Pressing Key During Cooldown

    else if ( g_fUltimateCooldown[iCasterId] && is_user_alive( iCasterId ) )
    {
        if ( iCasterId == g_Vip )
        {
            new szMessage[128];
            copy( szMessage, 127, ULTIMATE_NOCAST_VIP );

            WAR3_status_text( iCasterId, szMessage, 1.0 );
        }

        else if ( g_PlayerInfo[iCasterId][CURRENT_ULTIMATE] && g_iCurrentRound <= 1 )
        {
            new szMessage[128];
            copy( szMessage, 127, ULTIMATE_NOCAST_PISTOL );

            WAR3_status_text( iCasterId, szMessage, 1.0 );
        }

        else if ( g_PlayerInfo[iCasterId][CURRENT_ULTIMATE] )
        {
            if ( g_bFreezeTime )
            {
                new szMessage[128];
                copy( szMessage, 127, ULTIMATE_NOTREADY );

                WAR3_status_text( iCasterId, szMessage, 0.5 );
            }

            else
            {
                new iRemainingTime;
                new Float:fCooldownTime;

                if ( g_bUltimateWarmup )
                {
                    fCooldownTime = get_cvar_float( "war3x_ultimatewarmup" );
                }

                else
                {
                    fCooldownTime = ULTIMATE_COOLDOWNDEFAULT;
                }

                iRemainingTime = floatround( fCooldownTime - ( get_gametime() - g_fUltimateCooldown[iCasterId] ) );

                new szMessage[64];
                format( szMessage, 63, ULTIMATE_NOTREADY_COOLDOWN, iRemainingTime );

                WAR3_status_text( iCasterId, szMessage, 0.5 );
            }
        }

        else
        {
            new szMessage[128];
            copy( szMessage, 127, ULTIMATE_NOTFOUND );

            WAR3_status_text( iCasterId, szMessage, 0.5 );
        }

        // play client sound

        client_cmd( iCasterId, "speak warcraft3/bonus/Error.wav" );
    }

    // Cast on Self ( some ultimates )

    else if ( ( g_iChargeUltimate[iCasterId] && Ultimate_Target( iCasterId ) & ULTIMATE_TARGET_SELFONLY ) || ( g_iChargeUltimate[iCasterId] && Ultimate_Target( iCasterId ) & ULTIMATE_TARGET_SELF ) )
        Ultimate_Cast( iCasterId, iCasterId );

    // Cast on Target

    else if ( iTargetId && !( Ultimate_Target( iCasterId ) & ULTIMATE_TARGET_SELFONLY ) )
        Ultimate_Cast( iCasterId, iTargetId );

    else
    {
        if ( g_iChargeUltimate[iCasterId] && !( Ultimate_Target( iCasterId ) & ULTIMATE_TARGET_SELF ) )
        {
            new szMessage[128];
            copy( szMessage, 127, CANT_TARGET_SELF );

            WAR3_status_text( iCasterId, szMessage, 0.5 );
        }

        Ultimate_Beep( iCasterId );
    }

    return PLUGIN_HANDLED;
}


public Cmd_Say( id ) {
#if ADVANCED_DEBUG
	log_function("public Cmd_Say( id ) {");
#endif

    new szText[32];
    read_args( szText, 31 );

    remove_quotes( szText );

    // Remove "/" from command(s)

    if ( equal( szText, "/", 1 ) )
        copy( szText, 31, szText[1] );

    // War3 Vote

    if ( equali( szText, "war3vote" ) || equali( szText, "wc3vote" ) || equali( szText, "war3xvote" ) || equali( szText, "wc3xvote" ) )
        WAR3_vote_start( id );

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // Check for keywords

    if ( equali( szText, "changerace" ) || equali( szText, "selectrace" ) || equali( szText, "chooserace" ) )
        menu_SelectRace( id );

    else if ( equali( szText, "selectskill" ) || equali( szText, "selectskills" ) )
        menu_SelectSkills( id );

    else if ( equali( szText, "playerskills" ) )
        motd_PlayerSkills( id );

    else if ( equali( szText, "skillinfo" ) || equali( szText, "skillsinfo" ) || equali( szText, "skillhelp" ) || equali( szText, "skillshelp" ) )
        menu_SkillsHelp( id );

    else if ( equali( szText, "war3help" ) || equali( szText, "wc3help" ) || equali( szText, "war3xhelp" ) || equali( szText, "wc3xhelp" ) )
        menu_HelpTopics( id );

    else if ( equali( szText, "level" ) || equali( szText, "showlevel" ) )
        WAR3_hud_level( id );

    else if ( equali( szText, "skills" ) || equali( szText, "myskills" ) )
        motd_Skills( id, id );

    else if ( equali( szText, "iteminfo" ) || equali( szText, "itemsinfo" ) || equali( szText, "itemhelp" ) || equali( szText, "itemshelp" ) )
        motd_ItemsHelp( id );

    else if ( equali( szText, "war3menu" ) || equali( szText, "wc3menu" ) || equali( szText, "war3xmenu" ) || equali( szText, "wc3xmenu" ) )
        menu_War3menu( id );

    else if ( equali( szText, "options" ) || equali( szText, "playeroptions" ) || equali( szText, "war3options" ) || equali( szText, "wc3options" ) || equali( szText, "war3xoptions" ) || equali( szText, "wc3xoptions" ) )
        menu_PlayerOptions( id );

    return PLUGIN_CONTINUE;
}


public Cmd_Drop( id ) {
#if ADVANCED_DEBUG
	log_function("public Cmd_Drop( id ) {");
#endif

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    if ( g_bAnkhDropWeapons[id] )
    {
        new parm_Ankh[1];
        parm_Ankh[0] = id;

        remove_task( TASK_ANKH + id, 0 );
        set_task( 0.1, "Ankh_GiveItems", TASK_ANKH + id, parm_Ankh, 1 );
    }

    return PLUGIN_CONTINUE;
}


public cmd_DropItem( id ) {
#if ADVANCED_DEBUG
	log_function("public cmd_DropItem( id ) {");
#endif

    Item_Drop( id );
    return PLUGIN_HANDLED;
}


public cmd_War3menu( id ) {
#if ADVANCED_DEBUG
	log_function("public cmd_War3menu( id ) {");
#endif

    menu_War3menu( id );
    return PLUGIN_HANDLED;
}


public cmd_Shopmenu( id ) {
#if ADVANCED_DEBUG
	log_function("public cmd_Shopmenu( id ) {");
#endif

    menu_ItemShop( id );
    return PLUGIN_HANDLED;
}
