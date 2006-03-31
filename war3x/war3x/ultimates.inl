

public Ultimate_Target( id ) {

    new Ultimate = g_PlayerInfo[id][CURRENT_ULTIMATE];

    switch ( g_PlayerInfo[id][CURRENT_RACE] )
    {
        case RACE_UNDEAD:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_DEATHCOIL:    return ULTIMATE_TARGET_ENEMY|ULTIMATE_TARGET_TEAM;
                case ULTIMATE_IMPALE:       return ULTIMATE_TARGET_ENEMY;
                case ULTIMATE_SLEEP:        return ULTIMATE_TARGET_ENEMY;
            }
        }

        case RACE_HUMAN:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_HOLYLIGHT:    return ULTIMATE_TARGET_ENEMY|ULTIMATE_TARGET_TEAM;
                case ULTIMATE_FLAMESTRIKE:  return ULTIMATE_TARGET_ENEMY;
                case ULTIMATE_AVATAR:       return ULTIMATE_TARGET_SELFONLY;
            }
        }

        case RACE_ORC:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_HEALINGWAVE:  return ULTIMATE_TARGET_SELF|ULTIMATE_TARGET_TEAM;
                case ULTIMATE_LIGHTNING:    return ULTIMATE_TARGET_ENEMY;
                case ULTIMATE_WINDWALK:     return ULTIMATE_TARGET_SELFONLY;
            }
        }

        case RACE_NIGHTELF:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_REJUVENATION: return ULTIMATE_TARGET_SELF|ULTIMATE_TARGET_TEAM;
                case ULTIMATE_ROOTS:        return ULTIMATE_TARGET_ENEMY;
                case ULTIMATE_SHADOWSTRIKE: return ULTIMATE_TARGET_ENEMY;
            }
        }

    }

    return ( 0 );
}


public Ultimate_Type( id ) {

    new Ultimate = g_PlayerInfo[id][CURRENT_ULTIMATE];

    switch ( g_PlayerInfo[id][CURRENT_RACE] )
    {
        case RACE_UNDEAD:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_DEATHCOIL:    return ULTIMATE_TYPE_HEAL|ULTIMATE_TYPE_OFFENSIVE;
                case ULTIMATE_IMPALE:       return ULTIMATE_TYPE_OFFENSIVE;
                case ULTIMATE_SLEEP:        return ULTIMATE_TYPE_OFFENSIVE;
            }
        }

        case RACE_HUMAN:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_HOLYLIGHT:    return ULTIMATE_TYPE_OFFENSIVE|ULTIMATE_TYPE_HEAL;
                case ULTIMATE_FLAMESTRIKE:  return ULTIMATE_TYPE_OFFENSIVE;
                case ULTIMATE_AVATAR:       return ULTIMATE_TYPE_DEFENSIVE;
            }
        }

        case RACE_ORC:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_HEALINGWAVE:  return ULTIMATE_TYPE_HEAL;
                case ULTIMATE_LIGHTNING:    return ULTIMATE_TYPE_OFFENSIVE;
                case ULTIMATE_WINDWALK:     return ULTIMATE_TYPE_OFFENSIVE;
            }
        }

        case RACE_NIGHTELF:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_REJUVENATION: return ULTIMATE_TYPE_HEAL;
                case ULTIMATE_ROOTS:        return ULTIMATE_TYPE_OFFENSIVE;
                case ULTIMATE_SHADOWSTRIKE: return ULTIMATE_TYPE_OFFENSIVE;
            }
        }

    }

    return ( 0 );
}


public Ultimate_Icon( id, iStatus ) {

    // Make sure server enabled

    if ( !get_pcvar_num( CVAR_statusicons ) && iStatus == ICON_SHOW )
        return PLUGIN_HANDLED;

    // Make sure ultimate was cast

    if ( iStatus == ICON_HIDE && !g_fUltimateCooldown[id] )
        return PLUGIN_HANDLED;

    // Make sure player is alive before showing

    if ( !is_user_alive( id ) && iStatus == ICON_SHOW )
        return PLUGIN_HANDLED;

    new szIcon[16], iRGB[3];
    new Ultimate = g_PlayerInfo[id][CURRENT_ULTIMATE];

    switch ( g_PlayerInfo[id][CURRENT_RACE] )
    {
        case RACE_UNDEAD:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_DEATHCOIL:
                {
                    copy( szIcon, 15, "cross" );
                    iRGB = {96,224,0};
                }
                case ULTIMATE_IMPALE:
                {
                    copy( szIcon, 15, "d_skull" );
                    iRGB = {128,96,160};
                }
                case ULTIMATE_SLEEP:
                {
                    copy( szIcon, 15, "d_skull" );
                    iRGB = {255,201,169};
                }
            }
        }

        case RACE_HUMAN:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_HOLYLIGHT:
                {
                    copy( szIcon, 15, "cross" );
                    iRGB = {255,228,148};
                }
                case ULTIMATE_FLAMESTRIKE:
                {
                    copy( szIcon, 15, "d_skull" );
                    iRGB = {224,128,0};
                }
                case ULTIMATE_AVATAR:
                {
                    copy( szIcon, 15, "suit_full" );
                    iRGB = {103,230,222};
                }
            }
        }

        case RACE_ORC:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_HEALINGWAVE:
                {
                    copy( szIcon, 15, "cross" );
                    iRGB = {255,228,151};
                }
                case ULTIMATE_LIGHTNING:
                {
                    copy( szIcon, 15, "d_skull" );
                    iRGB = {22,166,253};
                }
                case ULTIMATE_WINDWALK:
                {
                    copy( szIcon, 15, "d_skull" );
                    iRGB = {161,227,243};
               }
            }
        }

        case RACE_NIGHTELF:
        {
            switch ( Ultimate )
            {
                case ULTIMATE_REJUVENATION:
                {
                    copy( szIcon, 15, "cross" );
                    iRGB = {253,86,253};
                }
                case ULTIMATE_ROOTS:
                {
                    copy( szIcon, 15, "d_skull" );
                    iRGB = {0,228,32};
                }
                case ULTIMATE_SHADOWSTRIKE:
                {
                    copy( szIcon, 15, "d_skull" );
                    iRGB = {206,249,75};
                }
            }
        }
    }

    Create_StatusIcon( id, iStatus, szIcon, iRGB[GLOW_R], iRGB[GLOW_G], iRGB[GLOW_B] );

    return ( 0 );
}


// Ping

public Ultimate_Scan( parm_Scan[1] ) {

    new id = parm_Scan[0];

    if ( g_iChargeUltimate[id] && is_user_alive( id ) )
    {
        g_iChargeUltimate[id] -= 1;

        if ( g_iChargeUltimate[id] )
        {
            emit_sound( id, CHAN_STATIC, SOUND_ULTIMATESCAN, 1.0, ATTN_NORM, 0, PITCH_NORM );

            new task = TASK_ULTIMATE + id;
            set_task( 1.0, "Ultimate_Scan", task, parm_Scan, 1 );
        }
    }

    return PLUGIN_HANDLED;
}


// Cast Ultimate

public Ultimate_Cast( Caster, Target ) {

    if ( g_fUltimateCooldown[Caster] || !is_user_alive( Caster ) || !is_user_alive( Target ) )
        return PLUGIN_HANDLED;

    // Make sure target is not immune to ultimates

    if ( g_iPlayerAvatar[Target] || Target == g_Vip )
    {
        new szMessage[128];
        formatex( szMessage, 127, "%L", Caster, "CANT_TARGET" );

        WAR3_status_text( Caster, szMessage, 0.5 );

        // play client sound

        client_cmd( Caster, "speak warcraft3/bonus/Error.wav" );

        Ultimate_Beep( Caster );

        return PLUGIN_HANDLED;
    }

    // Make sure target is not invisible

    if ( entity_get_int( Target, EV_INT_rendermode ) == kRenderTransTexture && get_user_team( Caster ) != get_user_team( Target ) )
    {
        Ultimate_Beep( Caster );
        return PLUGIN_HANDLED;
    }

    // Make sure caster is on ground

    if ( !( get_entity_flags( Caster ) & FL_ONGROUND ) )
    {
        new szMessage[128];
        formatex( szMessage, 127, "%L", Caster, "ULTIMATE_NOCAST_AIR" );

        WAR3_status_text( Caster, szMessage, 0.5 );

        // play client sound

        client_cmd( Caster, "speak warcraft3/bonus/Error.wav" );

        Ultimate_Beep( Caster );

        return PLUGIN_HANDLED;
    }

    // Check for Sleeping Player

    else if ( g_bPlayerSleeping[Target] && get_user_team( Target ) != get_user_team( Caster )  )
    {
        if ( g_PlayerInfo[Caster][CURRENT_RACE] != RACE_UNDEAD || g_PlayerInfo[Caster][CURRENT_RACE] == RACE_UNDEAD && g_PlayerInfo[Caster][CURRENT_ULTIMATE] != ULTIMATE_SLEEP )
        {
            new szMessage[128];
            formatex( szMessage, 127, "%L", Caster, "CANT_TARGET_SLEEP" );

            WAR3_status_text( Caster, szMessage, 0.5 );

            // play client sound

            client_cmd( Caster, "speak warcraft3/bonus/Error.wav" );

            Ultimate_Beep( Caster );

            return PLUGIN_HANDLED;
        }
    }

    // Check for Wind Walk

    else if ( g_bPlayerWalk[Target] && get_user_team( Caster ) != get_user_team( Target ) )
    {
        new szMessage[128];
        formatex( szMessage, 127, "%L", Caster, "CANT_TARGET_INVIS" );

        WAR3_status_text( Caster, szMessage, 0.5 );

        // play client sound

        client_cmd( Caster, "speak warcraft3/bonus/Error.wav" );

        Ultimate_Beep( Caster );

        return PLUGIN_HANDLED;
    }

    // Cast Ultimate

    switch ( g_PlayerInfo[Caster][CURRENT_RACE] )
    {
        case RACE_UNDEAD:   UD_ultimates( Caster, Target );
        case RACE_HUMAN:    HU_ultimates( Caster, Target );
        case RACE_ORC:      OR_ultimates( Caster, Target );
        case RACE_NIGHTELF: NE_ultimates( Caster, Target );
    }

    return PLUGIN_HANDLED;
}





// Beep (if no/invalid target)

public Ultimate_Beep( id ) {

    if ( !g_iChargeUltimate[id] )
    {
        new parm_Beep[1];
        parm_Beep[0] = id;

        new task = TASK_ULTIMATE + id;
        set_task( 1.0, "Ultimate_Scan", task, parm_Beep, 1 );

        g_iChargeUltimate[id] = DURATION_ULTIMATEPING;

        emit_sound( id, CHAN_STATIC, SOUND_ULTIMATESCAN, 1.0, ATTN_NORM, 0, PITCH_NORM );
    }

    return PLUGIN_HANDLED;
}


// Cooldown

public Ultimate_Cooldown( id, Float:fCooldown ) {

    // Stop Scanning

    g_iChargeUltimate[id] = 0;

    // Set cast time

    g_fUltimateCooldown[id] = get_gametime();

    if ( g_PlayerInfo[id][CURRENT_ITEM] == ITEM_MASK )
    {
        g_fUltimateCooldown[id] -= ITEM_MASK_VALUE;
        fCooldown -= ITEM_MASK_VALUE;
    }

    // Hide icon

    Ultimate_Icon( id, ICON_HIDE );

    new parm_Cooldown[1];
    parm_Cooldown[0] = id;

    new task = TASK_ULTIMATECOOLDOWN + id;
    remove_task( task, 0 );

    set_task( fCooldown, "Ultimate_Restore", task, parm_Cooldown, 1 );

    return PLUGIN_HANDLED;
}


// Restore

public Ultimate_Restore( parm_Restore[1] ) {

    new id = parm_Restore[0];
    Ultimate_Ready( id );

    return PLUGIN_HANDLED;
}


public Ultimate_Enable( id ) {

    if ( g_bChangeUltimate[id] )
        return PLUGIN_HANDLED;


    // Enable Ultimates

    if ( g_bUltimateWarmup )
        g_fUltimateCooldown[id] = get_gametime();

    else
    {
        Ultimate_Ready( id );
    }


    return PLUGIN_HANDLED;
}


public Ultimate_Disable( id ) {

    new task = TASK_ULTIMATECOOLDOWN + id;
    remove_task( task, 0 );

    return PLUGIN_HANDLED;
}


// Sounds / Message

public Ultimate_Ready( id ) {

    if ( g_bUltimateWarmup || id == g_Vip )
        return PLUGIN_HANDLED;


    if ( g_iCurrentRound > 1 )
    {
        if ( g_PlayerInfo[id][CURRENT_ULTIMATE] )
        {
            g_fUltimateCooldown[id] = 0.0;

            new szMessage[128];
            formatex( szMessage, 127, "%L", id, "ULTIMATE_READY" );

            WAR3_status_text( id, szMessage, 2.0 );

            // Play client sound

            new szCommand[32];
            formatex( szCommand, 31, "speak %s", SOUND_ULTIMATEREADY );

            client_cmd( id, szCommand );

            // Show Icon

            Ultimate_Icon( id, ICON_SHOW );
        }
    }

    else
    {
        g_fUltimateCooldown[id] = get_gametime();
    }

    return PLUGIN_HANDLED;
}


// Warmup done

public Ultimate_Warmup() {

    g_bUltimateWarmup = false;

    new Players[32], iTotalPlayers;
    get_players( Players, iTotalPlayers, "a" );

    for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
    {
        new id = Players[iPlayerNum];

        if ( g_PlayerInfo[id][CURRENT_ULTIMATE] )
            Ultimate_Ready( id );
    }

    return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. - //
