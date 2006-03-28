/* - Client Commands -------------------------------------------- */


// Ultimate Command

public cmd_Ultimate( caster ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_HANDLED;

    new target = g_PlayerTarget[caster];

    // Press when dead ( target skills ) *REMOVED TEMPORARILY*

    if ( !is_user_alive( caster ) )
    {
        //HELP_motd_target( caster, target );
        return PLUGIN_HANDLED;
    }

    // Press when restricted

    else if ( get_pcvar_bitsum( CVAR_restrict_ultimates ) & WAR3_get_ult_flag( g_PlayerInfo[caster][CURRENT_RACE], g_PlayerInfo[caster][CURRENT_ULTIMATE] ) )
    {
        new szMessage[128];
        formatex( szMessage, 127, "%s", ULTIMATE_RESTRICTED );

        WAR3_status_text( caster, szMessage, 0.5 );

        return PLUGIN_HANDLED;
    }

    // Press when sleeping

    else if ( g_bPlayerSleeping[caster] )
    {
        new szMessage[128];
        formatex( szMessage, 127, "%s", ULTIMATE_NOCAST_SLEEP );

        WAR3_status_text( caster, szMessage, 1.0 );
    }

    // Pressing Key During Cooldown

    else if ( g_fUltimateCooldown[caster] && is_user_alive( caster ) )
    {
        if ( caster == g_Vip )
        {
            new szMessage[128];
            formatex( szMessage, 127, "%s", ULTIMATE_NOCAST_VIP );

            WAR3_status_text( caster, szMessage, 1.0 );
        }

        else if ( g_PlayerInfo[caster][CURRENT_ULTIMATE] && g_iCurrentRound <= 1 )
        {
            new szMessage[128];
            formatex( szMessage, 127, "%s", ULTIMATE_NOCAST_PISTOL );

            WAR3_status_text( caster, szMessage, 1.0 );
        }

        else if ( g_PlayerInfo[caster][CURRENT_ULTIMATE] )
        {
            if ( g_bFreezeTime )
            {
                new szMessage[128];
                formatex( szMessage, 127, "%s", ULTIMATE_NOTREADY );

                WAR3_status_text( caster, szMessage, 0.5 );
            }

            else
            {
                new iRemainingTime;
                new Float:fCooldownTime;

                if ( g_bUltimateWarmup )
                {
                    fCooldownTime = get_pcvar_float( CVAR_ultimatewarmup );
                }

                else
                {
                    fCooldownTime = ULTIMATE_COOLDOWNDEFAULT;
                }

                iRemainingTime = floatround( fCooldownTime - ( get_gametime() - g_fUltimateCooldown[caster] ) );

                new szMessage[64];
                formatex( szMessage, 63, ULTIMATE_NOTREADY_COOLDOWN, iRemainingTime );

                WAR3_status_text( caster, szMessage, 0.5 );
            }
        }

        else
        {
            new szMessage[128];
            formatex( szMessage, 127, "%s", ULTIMATE_NOTFOUND );

            WAR3_status_text( caster, szMessage, 0.5 );
        }

        // play client sound

        client_cmd( caster, "speak warcraft3/bonus/Error.wav" );
    }

    // Cast on Self ( some ultimates )

    else if ( ( g_iChargeUltimate[caster] && Ultimate_Target( caster ) & ULTIMATE_TARGET_SELFONLY ) || ( g_iChargeUltimate[caster] && Ultimate_Target( caster ) & ULTIMATE_TARGET_SELF ) )
        Ultimate_Cast( caster, caster );

    // Cast on Target

    else if ( target && !( Ultimate_Target( caster ) & ULTIMATE_TARGET_SELFONLY ) )
        Ultimate_Cast( caster, target );

    else
    {
        if ( g_iChargeUltimate[caster] && !( Ultimate_Target( caster ) & ULTIMATE_TARGET_SELF ) )
        {
            new szMessage[128];
            formatex( szMessage, 127, "%s", CANT_TARGET_SELF );

            WAR3_status_text( caster, szMessage, 0.5 );
        }

        Ultimate_Beep( caster );
    }

    return PLUGIN_HANDLED;
}


public Cmd_Say( id ) {

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

    else if ( equali( szText, "shopmenu" ) || equali( szText, "itemshop" ) || equali( szText, "buyitems" ) )
        menu_ItemShop( id );

    else if ( equali( szText, "selectskill" ) || equali( szText, "selectskills" ) )
        menu_SelectSkills( id );

    else if ( equali( szText, "playerskills" ) )
        HELP_motd_players( id );

    else if ( equali( szText, "skillinfo" ) || equali( szText, "skillsinfo" ) || equali( szText, "skillhelp" ) || equali( szText, "skillshelp" ) )
        menu_SkillsHelp( id );

    else if ( equali( szText, "war3help" ) || equali( szText, "wc3help" ) || equali( szText, "war3xhelp" ) || equali( szText, "wc3xhelp" ) )
        menu_HelpTopics( id );

    else if ( equali( szText, "level" ) || equali( szText, "showlevel" ) )
        WAR3_hud_level( id );

    else if ( equali( szText, "skills" ) || equali( szText, "myskills" ) )
        HELP_motd_target( id, id );

    else if ( equali( szText, "iteminfo" ) || equali( szText, "itemsinfo" ) || equali( szText, "itemhelp" ) || equali( szText, "itemshelp" ) )
        HELP_motd_items( id );

    else if ( equali( szText, "war3menu" ) || equali( szText, "wc3menu" ) || equali( szText, "war3xmenu" ) || equali( szText, "wc3xmenu" ) )
        menu_War3menu( id );

    else if ( equali( szText, "options" ) || equali( szText, "playeroptions" ) || equali( szText, "war3options" ) || equali( szText, "wc3options" ) || equali( szText, "war3xoptions" ) || equali( szText, "wc3xoptions" ) )
        menu_PlayerOptions( id );

    return PLUGIN_CONTINUE;
}


public Cmd_Drop( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    if ( g_bAnkhDropWeapons[id] )
    {
        new parm_Ankh[1];
        parm_Ankh[0] = id;

        remove_task( TASK_ANKH + id, 0 );
        set_task( 0.1, "ITEM_ANKH_give", TASK_ANKH + id, parm_Ankh, 1 );
    }

    return PLUGIN_CONTINUE;
}


public cmd_DropItem( id ) {

    Item_Drop( id );
    return PLUGIN_HANDLED;
}


public cmd_War3menu( id ) {

    menu_War3menu( id );
    return PLUGIN_HANDLED;
}


public cmd_Shopmenu( id ) {

    menu_ItemShop( id );
    return PLUGIN_HANDLED;
}
