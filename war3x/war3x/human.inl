
/* - Events ----------------------------------------------------- */


public HU_skills_offensive( attacker, victim, weapon, iDamage, headshot ) {

    if ( g_PlayerInfo[attacker][CURRENT_RACE] == RACE_HUMAN && get_user_team( attacker ) != get_user_team( victim ) && get_user_health( victim ) > 0 )
    {
        // Bash

        if ( g_PlayerInfo[attacker][CURRENT_SKILL2] )
            HU_S_BASH( attacker, victim, weapon, headshot );

        // Inner Fire

        if ( g_PlayerInfo[attacker][CURRENT_SKILL3] && get_user_armor( attacker ) )
            HU_S_INNERFIRE( attacker, victim, weapon, iDamage, headshot );
    }

    return PLUGIN_HANDLED;
}


public HU_skills_defensive( victim ) {

    if ( g_PlayerInfo[victim][CURRENT_RACE] == RACE_HUMAN )
    {
        // Invisibility cooldown ( taken care of in on_Damage() )
    }

    return PLUGIN_HANDLED;
}


public HU_ultimates( caster, target ) {

    // Holy Light

    if ( g_PlayerInfo[caster][CURRENT_ULTIMATE] == ULTIMATE_HOLYLIGHT )
    {
        // Heal

        if ( get_user_team( target ) == get_user_team( caster ) && g_PlayerInfo[target][CURRENT_RACE] != RACE_UNDEAD )
        {
            if ( get_user_health( target ) == WAR3_get_maxhealth( target ) && caster != target )
            {
                new szMessage[128];
                formatex( szMessage, 127, "%L", caster, "FULLHEALTH_TARGET" );

                WAR3_status_text( caster, szMessage, 0.5 );

                Ultimate_Beep( caster );
            }

            else
            {
                HU_U_HOLYLIGHT_heal( caster, target );
                Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT );

                SHARED_INVIS_cooldown( caster );
            }
        }

        // Damage

        else if ( get_user_team( target ) != get_user_team( caster ) && g_PlayerInfo[target][CURRENT_RACE] == RACE_UNDEAD )
        {
            HU_U_HOLYLIGHT_damage( caster, target );
            Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT );

            SHARED_INVIS_cooldown( caster );
        }

        else
        {
            new szMessage[128];
            formatex( szMessage, 127, "%L", caster, "CANT_TARGET_HOLYLIGHT" );

            WAR3_status_text( caster, szMessage, 0.5 );

            Ultimate_Beep( caster );
        }
    }

    // Flame Strike

    else if ( g_PlayerInfo[caster][CURRENT_ULTIMATE] == ULTIMATE_FLAMESTRIKE && get_user_team( target ) != get_user_team( caster ) )
    {
        HU_U_FLAMESTRIKE_init( caster, target );
        Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT );

        SHARED_INVIS_cooldown( caster );
    }

    // Avatar

    else if ( g_PlayerInfo[caster][CURRENT_ULTIMATE] == ULTIMATE_AVATAR )
    {
        HU_U_AVATAR( caster );
        Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT );
    }

    else
    {
        Ultimate_Beep( caster );
        return PLUGIN_HANDLED;
    }

    return PLUGIN_HANDLED;
}


/* - Racial Ability --------------------------------------------- */


// Fortitude

public HU_S_FORTITUDE_get( iLevel ) {

    new Float:fLevel = float( iLevel );

    if ( fLevel > LEVEL_RACIALCAP )
        fLevel = LEVEL_RACIALCAP;

    new Float:fNewHealth = HU_fFortitude[0] + ( ( ( HU_fFortitude[1] - HU_fFortitude[0] ) / LEVEL_RACIALCAP ) * fLevel );
    new iNewHealth = floatround( fNewHealth );

    return ( iNewHealth );
}


public HU_S_FORTITUDE_set( id ) {

    // Check if restricted

    if ( !WAR3_skill_enabled( id, RACE_HUMAN, SKILL_RACIAL ) )
        return PLUGIN_HANDLED;

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new iNewHealth = HU_S_FORTITUDE_get( iLevel );

    set_user_health( id, iNewHealth );

    return PLUGIN_HANDLED;
}


/* - Skills ----------------------------------------------------- */


// Invisibility

public HU_S_INVISIBILITY_set( id ) {

    g_bPlayerInvis[id] = true;

    new Float:fInvis = HU_fInvisibility[g_PlayerInfo[id][CURRENT_SKILL1] - 1];
    fInvis = ( 1.0 - fInvis ) * 255.0;

    // Add 30% of Cloak value to invisibility level

    if ( g_PlayerInfo[id][CURRENT_ITEM] == ITEM_CLOAK )
        fInvis -= ( ( ( 1.0 - ITEM_CLOAK_VALUE ) * 255 ) * 0.3 );

    if ( fInvis < ( ( 1.0 - CAP_INVISIBILITY ) * 255.0 ) )
        fInvis = ( ( 1.0 - CAP_INVISIBILITY ) * 255.0 );

    new iInvis = floatround( fInvis );
    set_user_rendering( id, kRenderFxNone, 0, 0, 0, kRenderTransTexture, iInvis );

    SHARED_INVIS_icon( id, ICON_SHOW );

    // Set shift task

    new arg_write[1];
    arg_write[0] = id;

    new task = TASK_INVIS + id;
    set_task( INVISIBILITY_PRESHIFT, "HU_S_INVISIBILITY_shift", task, arg_write, 1 );

    return PLUGIN_HANDLED;
}


public HU_S_INVISIBILITY_shift( arg_read[1] ) {

    new id = arg_read[0];

    if ( !is_user_connected( id ) )
        return PLUGIN_HANDLED;

    // Find max shift value(s)

    new Float:fCurrentInvis = entity_get_float( id, EV_FL_renderamt );
    new Float:fInvisPercent = 1.0 - ( fCurrentInvis / 255.0 );

    if ( WAR3_is_invisible( id ) && fInvisPercent > HU_fInvisibility[LEVEL_1] )
    {
        new Float:fMaxLevel = ( 1.0 - HU_fInvisibility[g_PlayerInfo[id][CURRENT_SKILL1] - 1] ) * 255.0;
        new Float:fMinLevel = ( 1.0 - HU_fInvisibility[LEVEL_1] ) * 255.0;

        new Float:fInvisShift = ( fMinLevel - fMaxLevel ) / float( INVISIBILITY_SHIFTS );
        entity_set_float( id, EV_FL_renderamt, fCurrentInvis + fInvisShift );

        SHARED_INVIS_icon( id, ICON_SHOW );

        // Repeat

        new Float:fNextCheck = INVISIBILITY_DURATION / float( INVISIBILITY_SHIFTS );

        new task = TASK_INVIS + id;
        set_task( fNextCheck, "HU_S_INVISIBILITY_shift", task, arg_read, 1 );
    }

    else
    {
        new task = TASK_INVIS + id;
        remove_task( task, 0 );
    }

    return PLUGIN_HANDLED;
}


// Bash

static HU_S_BASH( attacker, victim, weapon, headshot ) {

    // Check if restricted

    if ( !WAR3_skill_enabled( attacker, RACE_HUMAN, SKILL_2 ) )
        return PLUGIN_HANDLED;

    new Float:fBashChance = HU_fBash[g_PlayerInfo[attacker][CURRENT_SKILL2] - 1];
    new Float:fRandomNum  = random_float( 0.0,1.0 );

    new iBonusDamage = HU_iBashDamage[g_PlayerInfo[attacker][CURRENT_SKILL2] - 1];

    // Apply bonus modifier

    if ( weapon == CSW_KNIFE )
    {
        fBashChance += fBashChance * BASH_KNIFEBONUS;

        new Float:fDamageMod = float( iBonusDamage ) * BASH_KNIFEBONUS;
        iBonusDamage += floatround( fDamageMod );
    }

    // Check if bash was successful

    if ( fBashChance < fRandomNum )
        return PLUGIN_HANDLED;

    // Add to player stats array

    if ( get_cvar_num( "mp_war3stats" ) )
    {
        playerSkill2Info[attacker][0] += iBonusDamage;
        playerSkill2Info[attacker][1] += 1;
    }

    new Float:fDuration = random_float( BASH_DURATION_MIN, BASH_DURATION_MAX );

    // Glow

    SHARED_GLOW_set( victim, fDuration, BASH_RGB, 24 );

    // Apply damage

    if ( iBonusDamage > 0 )
        WAR3_damage( attacker, victim, weapon, iBonusDamage, headshot, DAMAGE_CHECKARMOR );

    if ( is_user_alive( victim ) )
    {
        // Immobilize

        HU_S_BASH_stun( victim, fDuration );

        // Screen fade

        Create_ScreenFade( victim, (1<<10), (1<<10), FADE_OUT, 255, 255, 255, 100 );
    }

    return PLUGIN_HANDLED;
}


static HU_S_BASH_stun( id, Float:fDuration ) {

    g_bPlayerBashed[id] = true;
    g_bPlayerCantMove[id] = true;

    WAR3_set_speed( id );

    new arg_write[1];
    arg_write[0] = id;

    new task = TASK_BASH + id;

    remove_task( task, 0 );
    set_task( fDuration, "SBash_Remove", task, arg_write, 1 );

    return PLUGIN_HANDLED;
}


public SBash_Remove( arg_read[1] ) {

    new id = arg_read[0];

    new task = TASK_BASH + id;
    remove_task( task, 0 );

    g_bPlayerBashed[id] = false;

    SHARED_IMMOBILIZE_remove( id );
    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


// Inner Fire

static HU_S_INNERFIRE( attacker, victim, weapon, iDamage, headshot ) {

    // Check if restricted

    if ( !WAR3_skill_enabled( attacker, RACE_HUMAN, SKILL_3 ) )
        return PLUGIN_HANDLED;

    new Float:fBonusDamage = float( iDamage ) * HU_fInnerFire_damage[g_PlayerInfo[attacker][CURRENT_SKILL3] - 1];
    new iBonusDamage = floatround( fBonusDamage );

    // Add to player stats array

    if ( get_cvar_num( "mp_war3stats" ) )
    {
        playerSkill3Info[attacker][0] += iBonusDamage;
    }

    // Apply damage

    if ( iBonusDamage > 0 )
        WAR3_damage( attacker, victim, weapon, iBonusDamage, headshot, DAMAGE_CHECKARMOR );

    return PLUGIN_HANDLED;
}


/* - Holy Light ------------------------------------------------- */


// Healing Version

static HU_U_HOLYLIGHT_heal( caster, target ) {

    // Play sound

    emit_sound( target, CHAN_STATIC, SOUND_HOLYLIGHT, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Status text

    new szCastMessage[64];
    formatex( szCastMessage, 63, "%L", caster, "HEAL_CAST" );

    WAR3_status_text( caster, szCastMessage, 1.0 );

    // Heal player

    new iHealthGiven = WAR3_heal( caster, target, HOLYLIGHT_HEAL );

    // Hud message

    new szMessage[128], szPlayerName[32];
    get_user_name( caster, szPlayerName, 31 );

    formatex( szMessage, 127, "%L", target, "HEAL_TARGET", szPlayerName, iHealthGiven );

    WAR3_status_text( target, szMessage, 3.0 );

    // Display effects

    HU_U_HOLYLIGHT_effects( target );

    return PLUGIN_HANDLED;
}


// Damage Version

static HU_U_HOLYLIGHT_damage( caster, target ) {

    // Play sound

    emit_sound( target, CHAN_STATIC, SOUND_HOLYLIGHT, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Display effects

    HU_U_HOLYLIGHT_effects( target );

    // Check for Amulet

    if ( g_PlayerInfo[target][CURRENT_ITEM] == ITEM_AMULET )
    {
        ITEM_AMULET_block( target, caster );
        return PLUGIN_HANDLED;
    }

    // Hud message

    new szMessage[128], szPlayerName[32];
    get_user_name( caster, szPlayerName, 31 );

    formatex( szMessage, 127, "%L", target, "CAST_HOLYLIGHT", szPlayerName, HOLYLIGHT_DAMAGE );

    new iBlindTime = HOLYLIGHT_BLINDTIME;
    new Float:fDuration = float( iBlindTime ) + 3.0;

    WAR3_status_text2( target, szMessage, fDuration );

    // Apply Damage

    WAR3_damage( caster, target, CSW_HOLYLIGHT, HOLYLIGHT_DAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( is_user_alive( target ) )
    {
        // Set Velocity

        new Velocity[3];
        get_entity_velocity( target, Velocity );

        Velocity[0] = 0;
        Velocity[1] = 0;

        set_entity_velocity( target, Velocity );

        // Screen Fade

        Create_ScreenFade( target, (1<<10), (HOLYLIGHT_BLINDTIME<<12), FADE_OUT, 255, 255, 255, 255 );
    }

    return PLUGIN_HANDLED;
}


// Effect

static HU_U_HOLYLIGHT_effects( target ) {

    new Origin[3], Radius[3];

    // Main cone

    get_user_origin( target, Origin );
    Origin[2] += 16;
    Radius[2] = 50;

    Create_TE_BEAMCYLINDER( SHOWTO_ALL, Origin, Radius, SPR_SHOCKWAVE, 0, 0, 3, 50, 0, 255, 245, 220, 128, 0 );

    // Dark ring

    get_user_origin( target, Origin );
    Origin[2] += 4;
    Radius[2] = 160;

    Create_TE_BEAMCYLINDER( SHOWTO_ALL, Origin, Radius, SPR_SMOOTHWAVE, 0, 0, 3, 6, 0, 255, 180, 36, 128, 0 );

   // Light ring

    get_user_origin( target, Origin );
    Origin[2] += 16;
    Radius[2] = 200;

    Create_TE_BEAMCYLINDER( SHOWTO_ALL, Origin, Radius, SPR_SMOOTHWAVE, 0, 0, 3, 4, 0, 255, 220, 144, 128, 0 );

    // Light

    Create_TE_ELIGHT( SHOWTO_ALL, target, 100, 255, 245, 220, 3, 0 );

    return PLUGIN_HANDLED;
}


/* - Flame Strike ----------------------------------------------- */


static HU_U_FLAMESTRIKE_init( caster, target ) {

    // Play Sound

    emit_sound( target, CHAN_STATIC, SOUND_FLAMESTRIKE, 1.0, ATTN_NORM, 0, PITCH_NORM );

    new arg_write[2];
    arg_write[0] = caster;
    arg_write[1] = target;

    HU_U_FLAMESTRIKE( arg_write );

    return PLUGIN_HANDLED;
}


// Cast Flame Strike

public HU_U_FLAMESTRIKE( arg_read[2] ) {

    new caster = arg_read[0];
    new target = arg_read[1];

    // Make sure player is on ground

    if ( get_entity_flags( target ) & FL_ONGROUND )
    {
        new Origin[3];
        get_user_origin( target, Origin );

        // Check for Amulet

        if ( g_PlayerInfo[target][CURRENT_ITEM] == ITEM_AMULET )
        {
            ITEM_AMULET_block( target, caster );

            // Play effects for a second

            HU_U_FLAMESTRIKE_effects( Origin, 1.5 );

            return PLUGIN_HANDLED;
        }

        // Hud message

        new szMessage[128], szPlayerName[32];
        get_user_name( caster, szPlayerName, 31 );

        formatex( szMessage, 127, "%L", target, "CAST_FLAMESTRIKE", szPlayerName );

        WAR3_status_text2( target, szMessage, 3.0 );

        // Cast

        HU_U_FLAMESTRIKE_effects( Origin, FLAMESTRIKE_DURATION );
        g_bFlameStrikeCast[caster] = true;

        new Float:fDuration = FLAMESTRIKE_DURATION;
        new iDuration = floatround( fDuration );

        new iCounter = ( iDuration * 5 );

        new arg_write[5];
        arg_write[0] = caster;
        arg_write[1] = iCounter;
        arg_write[2] = Origin[0];
        arg_write[3] = Origin[1];
        arg_write[4] = Origin[2];

        HU_U_FLAMESTRIKE_think( arg_write );
    }

    else
    {
        new task = TASK_FLAMESTRIKE_CASTER + caster;
        set_task( 0.1, "HU_U_FLAMESTRIKE", task, arg_read, 2 );
    }

    return PLUGIN_HANDLED;
}


// Fire Check

public HU_U_FLAMESTRIKE_think( arg_read[5] ) {

    new caster = arg_read[0];
    new iCounter = arg_read[1];

    if ( iCounter && g_bFlameStrikeCast[caster] )
    {
        new Origin[3];
        Origin[0] = arg_read[2];
        Origin[1] = arg_read[3];
        Origin[2] = arg_read[4];

        iCounter--;

        // Check for players in range

        new Players[32], iTotalPlayers, PlayerOrigin[3];
        get_players( Players, iTotalPlayers, "a" );

        // Find player distance(s)

        for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
        {
            new player = Players[iPlayerNum];
            get_user_origin( player, PlayerOrigin );

            new bool:bPlayerImmune;

            if ( g_PlayerOnFire[player] || g_bPlayerSleeping[player] || g_iPlayerAvatar[player] || player == g_Vip )
                bPlayerImmune = true;

            if ( !bPlayerImmune && get_distance( Origin, PlayerOrigin ) / 40 <= FLAMESTRIKE_RADIUS )
            {
                new arg_write[5];
                arg_write[0] = caster;
                arg_write[1] = player;
                arg_write[2] = PlayerOrigin[0];
                arg_write[3] = PlayerOrigin[1];
                arg_write[4] = PlayerOrigin[2];

                HU_U_FLAMESTRIKE_damage( arg_write );
            }
        }

        arg_read[1] = iCounter;

        new task = TASK_FLAMESTRIKE_CASTER + caster;
        set_task( 0.2, "HU_U_FLAMESTRIKE_think", task, arg_read, 5 );
    }

    else
    {
        g_bFlameStrikeCast[caster] = false;
    }

    return PLUGIN_HANDLED;
}


// Fire Damage

public HU_U_FLAMESTRIKE_damage( arg_read[5] ) {

    new caster = arg_read[0];
    new victim = arg_read[1];

    // Play sound!

    emit_sound( victim, CHAN_STATIC, SOUND_FLAMESTRIKE_BURN, 0.5, ATTN_NORM, 0, PITCH_NORM );

    // Burn Effect(s)

    new Origin[3];
    Origin[0] = arg_read[2];
    Origin[1] = arg_read[3];
    Origin[2] = arg_read[4];

    Create_TE_SPRITE( SHOWTO_ALL_BROADCAST, Origin, SPR_FLAMEBURST, 5, 200 );

    // Apply Damage

    WAR3_damage( caster, victim, CSW_FLAMESTRIKE, FLAMESTRIKE_DAMAGEIN, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( is_user_alive( victim ) )
    {
        // Fire Damage

        Create_Damage( victim, 0, 0, CS_DMG_BURN );

        // Screen Fade

        Create_ScreenFade( victim, (1<<10), (1<<10), FADE_OUT, 255, 108, 0, 160 );

        // Set Globals

        g_PlayerSingeing[victim] = 0;
        g_PlayerOnFire[victim]   = caster;

        new task = TASK_FLAMESTRIKE_VICTIM + victim;
        set_task( 1.0, "HU_U_FLAMESTRIKE_onfire", task, arg_read, 5 );

        Icon_DispellMe( victim );
    }

    return PLUGIN_HANDLED;
}


// Fire task

public HU_U_FLAMESTRIKE_onfire( arg_read[5] ) {

    new caster = arg_read[0];
    new victim = arg_read[1];

    new Origin[3];
    Origin[0] = arg_read[2];
    Origin[1] = arg_read[3];
    Origin[2] = arg_read[4];

    new VictimOrigin[3];
    get_user_origin( victim, VictimOrigin );

    new bool:bPlayerImmune;

    if ( g_bPlayerSleeping[victim] || g_iPlayerAvatar[victim] )
        bPlayerImmune = true;

    if ( !bPlayerImmune && get_distance( Origin, VictimOrigin ) / 40 <= FLAMESTRIKE_RADIUS && g_bFlameStrikeCast[caster] )
        HU_U_FLAMESTRIKE_damage( arg_read );

    else
    {
        g_PlayerOnFire[victim] = 0;
        g_PlayerSingeing[victim] = caster;

        new arg_write[3];
        arg_write[0] = caster;
        arg_write[1] = victim;
        arg_write[2] = FLAMESTRIKE_SINGEDURATION;

        HU_U_FLAMESTRIKE_singe( arg_write );
    }

    return PLUGIN_HANDLED;
}


// Singe Damage

public HU_U_FLAMESTRIKE_singe( arg_read[3] ) {

    new caster = arg_read[0];
    new victim = arg_read[1];

    new iCounter = arg_read[2];

    // Apply damage

    WAR3_damage( caster, victim, CSW_FLAMESTRIKE, FLAMESTRIKE_DAMAGEOUT, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( !g_PlayerOnFire[victim] && is_user_alive( victim ) )
    {
        // Screen fade

        if ( !g_bPlayerSleeping[victim] )
        {
            Create_ScreenFade( victim, (1<<10), (1<<10), FADE_OUT, 255, 108, 0, 20 );
        }

        // Check DOT counter

        iCounter--;

        if ( iCounter )
        {
            arg_read[2] = iCounter;

            new task = TASK_FLAMESTRIKE_VICTIM + victim;
            set_task( 1.0, "HU_U_FLAMESTRIKE_singe", task, arg_read, 3 );
        }
    }

    else
    {
        HU_U_FLAMESTRIKE_remove( victim );
    }

    return PLUGIN_HANDLED;
}


// Remove

public HU_U_FLAMESTRIKE_remove( victim ) {

    g_PlayerOnFire[victim] = 0;
    g_PlayerSingeing[victim] = 0;

    new task = TASK_FLAMESTRIKE_VICTIM + victim;
    remove_task( task, 0 );

    Icon_DispellMe( victim );

    return PLUGIN_HANDLED;
}


// Flame Pillar Effects

public HU_U_FLAMESTRIKE_effects( Origin[3], Float:fDuration ) {

    Origin[2] -= 5;

    new Float:fFlameBorder = 75.0;
    new Float:fTempOrigin[3];

    // Border

    for ( new i = 1; i < 5; i++ )
    {
        fTempOrigin[0] = float( Origin[0] );
        fTempOrigin[1] = float( Origin[1] );
        fTempOrigin[2] = float( Origin[2] );

        switch ( i )
        {
            case 1:         fTempOrigin[0] += ( fFlameBorder );
            case 2:         fTempOrigin[0] += ( -1.0 * fFlameBorder );
            case 3:         fTempOrigin[1] += ( fFlameBorder );
            case 4:         fTempOrigin[1] += ( -1.0 * fFlameBorder );
        }

        WAR3_ENTITY_sprite( "FLAME_STRIKE", "sprites/cexplo.spr", fTempOrigin, MOVETYPE_NONE, SOLID_TRIGGER, random_float( 10.0, 20.0 ), random_float( 0.4, 0.8 ), fDuration );
    }

    // Inner diagonal

    fFlameBorder = 2.0 * ( fFlameBorder / 3.0 );

    for ( new i = 1; i < 5; i++ )
    {
        fTempOrigin[0] = float( Origin[0] );
        fTempOrigin[1] = float( Origin[1] );
        fTempOrigin[2] = float( Origin[2] );

        switch ( i )
        {
            case 1:
            {
                fTempOrigin[0] += ( fFlameBorder );
                fTempOrigin[1] += ( -1.0 * fFlameBorder );
            }
            case 2:
            {
                fTempOrigin[0] += ( fFlameBorder );
                fTempOrigin[1] += ( fFlameBorder );
            }
            case 3:
            {
                fTempOrigin[0] += ( -1.0 * fFlameBorder );
                fTempOrigin[1] += ( -1.0 * fFlameBorder );
            }
            case 4:
            {
                fTempOrigin[0] += ( -1.0 * fFlameBorder );
                fTempOrigin[1] += ( fFlameBorder );
            }
        }

        WAR3_ENTITY_sprite( "FLAME_STRIKE", "sprites/fexplo.spr", fTempOrigin, MOVETYPE_NONE, SOLID_TRIGGER, random_float( 10.0, 20.0 ), random_float( 0.4, 0.8 ), fDuration );
    }


    // Inner square

    fFlameBorder = 2.0 * ( fFlameBorder / 3.0 );

    for ( new i = 1; i < 5; i++ )
    {
        fTempOrigin[0] = float( Origin[0] );
        fTempOrigin[1] = float( Origin[1] );
        fTempOrigin[2] = float( Origin[2] );

        switch ( i )
        {
            case 1:         fTempOrigin[0] += ( fFlameBorder );
            case 2:         fTempOrigin[0] += ( -1.0 * fFlameBorder );
            case 3:         fTempOrigin[1] += ( fFlameBorder );
            case 4:         fTempOrigin[1] += ( -1.0 * fFlameBorder );
        }

        WAR3_ENTITY_sprite( "FLAME_STRIKE", "sprites/cexplo.spr", fTempOrigin, MOVETYPE_NONE, SOLID_TRIGGER, random_float( 10.0, 20.0 ), random_float( 0.4, 0.8 ), fDuration );
    }

    return PLUGIN_HANDLED;
}


/* - Avatar ----------------------------------------------------- */


static HU_U_AVATAR( id ) {

    SHARED_INVIS_remove( id );

    g_bPlayerCantMove[id] = true;
    g_bPlayerAvatarGrow[id] = true;

    g_iPlayerAvatar[id] = 0;

    // Play sound

    emit_sound( id, CHAN_STATIC, SOUND_AVATAR, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Immobilize

    set_user_maxspeed( id, SPEED_IMMOBILIZE );

    // Status text

    new szMessage[128];
    formatex( szMessage, 127, "%L", id, "CAST_AVATAR" );

    WAR3_status_text( id, szMessage, 3.0 );

    // Screenshake

    Create_ScreenShake( id, (10<<12), (2<<12), (5<<12) );

    // Add health/effects

    new arg_write[2];
    arg_write[0] = id;

    HU_U_AVATAR_grow( arg_write );

    return PLUGIN_HANDLED;
}


public HU_U_AVATAR_grow( arg_read[2] ) {

    new id = arg_read[0];
    new iCounter = arg_read[1];

    new iMaxCounts = AVATAR_HEALTH / AVATAR_STEPSIZE;

    iCounter += 1;

    // Calculate new health

    new iNewHealth = get_user_health( id ) + AVATAR_STEPSIZE;

    if ( iNewHealth > WAR3_get_maxhealth( id ) )
        iNewHealth = WAR3_get_maxhealth( id );

    // Increase health

    set_user_health( id, iNewHealth );

    // Increase shell size

    new iShellSize = ( AVATAR_SIZE / iMaxCounts ) * iCounter;
    set_entity_rendering( id, kRenderFxGlowShell, AVATAR_RGB[GLOW_R], AVATAR_RGB[GLOW_G], AVATAR_RGB[GLOW_B], kRenderNormal, iShellSize );

    // Loop again

    if ( iCounter < iMaxCounts )
    {
        arg_read[1] = iCounter;

        new task = TASK_AVATAR + id;
        set_task( 0.1, "HU_U_AVATAR_grow", task, arg_read, 2 );
    }

    else
    {
        HU_U_AVATAR_immune( id );
    }

    return PLUGIN_HANDLED;
}


static HU_U_AVATAR_immune( id ) {

    g_bPlayerAvatarGrow[id] = false;
    g_iPlayerAvatar[id] = get_user_health( id );

    SHARED_IMMOBILIZE_remove( id );
    WAR3_set_speed( id );

	// Bartime

    new iDuration = floatround( AVATAR_DURATION );
    Create_BarTime( id, iDuration );

    // Set duration timer

    new arg_write[3];
    arg_write[0] = id;

    new task = TASK_AVATAR + id;
    set_task( AVATAR_DURATION, "HU_U_AVATAR_shrink", task, arg_write, 2 );

    return PLUGIN_HANDLED;
}


public HU_U_AVATAR_shrink( arg_read[2] ) {

    new id = arg_read[0];
    new iCounter = arg_read[1];


    new iMaxCounts = AVATAR_HEALTH / AVATAR_STEPSIZE;

    iCounter += 1;

    // Decrease health

    new iNewHealth = get_user_health( id ) - AVATAR_STEPSIZE;

     // Don't kill player

    if ( iNewHealth < 1 )
        iNewHealth = 1;

    set_user_health( id, iNewHealth );

    // Decrease shell size

    new iShellSize = AVATAR_SIZE - ( ( AVATAR_SIZE / iMaxCounts ) * iCounter );
    set_entity_rendering( id, kRenderFxGlowShell, AVATAR_RGB[GLOW_R], AVATAR_RGB[GLOW_G], AVATAR_RGB[GLOW_B], kRenderNormal, iShellSize );

    // Loop again

    if ( iCounter < iMaxCounts )
    {
        arg_read[1] = iCounter;

        new task = TASK_AVATAR + id;
        set_task( 0.1, "HU_U_AVATAR_shrink", task, arg_read, 2 );
    }

    else
    {
        HU_U_AVATAR_finish( id );
    }

    return PLUGIN_HANDLED;
}


static HU_U_AVATAR_finish( id ) {

    // Status text

    new szMessage[128];
    formatex( szMessage, 127, "%L", id, "FINISH_AVATAR" );

    WAR3_status_text( id, szMessage, 3.0 );

    HU_U_AVATAR_remove( id );

    // Remove shell

    set_user_rendering( id );

    // Invisibility cooldown

    SHARED_INVIS_cooldown( id );

    // Reset speed

    WAR3_set_speed( id );

    // Cooldown

    Ultimate_Cooldown( id, ULTIMATE_COOLDOWNDEFAULT );

    return PLUGIN_HANDLED;
}


public HU_U_AVATAR_damage( attacker, victim, weapon, headshot ) {

    // Add to bonus damage array

    WAR3_damage( attacker, victim, weapon, AVATAR_DAMAGE, headshot, DAMAGE_CHECKARMOR );

    // Screen fade

    if ( is_user_alive( victim ) && !g_bPlayerSleeping[victim] )
    {
        new iFadeAlpha = AVATAR_DAMAGE * 3;
        Create_ScreenFade( victim, (1<<10), (1<<10), FADE_OUT, AVATAR_RGB[GLOW_R], AVATAR_RGB[GLOW_G], AVATAR_RGB[GLOW_B], iFadeAlpha );
    }

    return PLUGIN_HANDLED;
}


public HU_U_AVATAR_remove( id ) {

    g_bPlayerAvatarGrow[id] = false;
    g_iPlayerAvatar[id] = 0;

    // Remove bartime

    Remove_BarTime( id );

    new task = TASK_AVATAR + id;
    remove_task( task, 0 );

    return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. - //
