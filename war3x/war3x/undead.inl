// Begin UNDEAD.INL

/* - Events ----------------------------------------------------- */


public UD_skills_offensive( attacker, victim, weapon, iDamage, headshot ) {

    if ( g_PlayerInfo[attacker][CURRENT_RACE] == RACE_UNDEAD && get_user_team( attacker ) != get_user_team( victim ) )
    {
        // Vampiric Aura

        if ( g_PlayerInfo[attacker][CURRENT_SKILL1] && cs_get_weapon_type_( weapon ) != CS_WEAPON_TYPE_GRENADE )
            UD_S_VAMPIRIC( attacker, victim, weapon, iDamage );
    }

    return PLUGIN_HANDLED;
}


public UD_skills_defensive( attacker, victim, weapon, iDamage, headshot ) {

    if ( g_PlayerInfo[victim][CURRENT_RACE] == RACE_UNDEAD && get_user_team( attacker ) != get_user_team( victim ) && attacker )
    {
        // Frost Nova ( called from on_Death )

        // Frost Armor

        if ( g_PlayerInfo[victim][CURRENT_SKILL3] && get_user_armor( victim ) )
            UD_S_FROSTARMOR( victim, attacker );
    }

    return PLUGIN_HANDLED;
}


public UD_ultimates( caster, target ) {

    // Death Coil

    if ( g_PlayerInfo[caster][CURRENT_ULTIMATE] == ULTIMATE_DEATHCOIL )
    {
        // Heal

        if ( get_user_team( target ) == get_user_team( caster ) && g_PlayerInfo[target][CURRENT_RACE] == RACE_UNDEAD )
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
                UD_U_DEATHCOIL( caster, target );
                Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT );

                SHARED_INVIS_cooldown( caster );
            }
        }

        // Damage

        else if ( get_user_team( target ) != get_user_team( caster ) && g_PlayerInfo[target][CURRENT_RACE] != RACE_UNDEAD )
        {
            UD_U_DEATHCOIL( caster, target );
            Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT );

            SHARED_INVIS_cooldown( caster );
        }

        else
        {
            new szMessage[128];
            formatex( szMessage, 127, "%L", caster, "CANT_TARGET_DEATHCOIL" );

            WAR3_status_text( caster, szMessage, 0.5 );

            Ultimate_Beep( caster );
        }
    }

    // Impale

    else if ( g_PlayerInfo[caster][CURRENT_ULTIMATE] == ULTIMATE_IMPALE && get_user_team( target ) != get_user_team( caster ) )
    {
        if ( !( get_entity_flags( target ) & FL_ONGROUND ) )
        {
            new szMessage[128];
            formatex( szMessage, 127, "%L", caster, "CANT_TARGET_AIR" );

            WAR3_status_text( caster, szMessage, 0.5 );

            Ultimate_Beep( caster );
        }

        else
        {
            UD_U_IMPALE( caster, target );
            Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT );

            SHARED_INVIS_cooldown( caster );
        }
    }

    // Sleep

    else if ( g_PlayerInfo[caster][CURRENT_ULTIMATE] == ULTIMATE_SLEEP && get_user_team( target ) != get_user_team( caster ) )
    {
        UD_U_SLEEP( caster, target );
        Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT );

        SHARED_INVIS_cooldown( caster );
    }

    else
    {
        Ultimate_Beep( caster );
        return PLUGIN_HANDLED;
    }

    return PLUGIN_HANDLED;
}


/* - Racial Ability --------------------------------------------- */


// Unholy Aura ( get speed )

public Float:UD_S_UNHOLY_get_speed( iLevel ) {

    new Float:fLevel = float( iLevel );

    if ( fLevel > LEVEL_RACIALCAP )
        fLevel = LEVEL_RACIALCAP;

    new Float:fUnholySpeed = UD_fUnholyAura_speed[0] + ( ( ( UD_fUnholyAura_speed[1] - UD_fUnholyAura_speed[0] ) / LEVEL_RACIALCAP ) * fLevel );

    // Final checks

    if ( fUnholySpeed > UD_fUnholyAura_speed[1] )
        fUnholySpeed = UD_fUnholyAura_speed[1];

    else if ( fUnholySpeed < UD_fUnholyAura_speed[0] )
        fUnholySpeed = UD_fUnholyAura_speed[0];

    return ( fUnholySpeed );
}


// Unholy Aura ( get gravity )

public Float:UD_S_UNHOLY_get_gravity( iLevel ) {

    new Float:fLevel = float( iLevel );

    if ( fLevel > LEVEL_RACIALCAP )
        fLevel = LEVEL_RACIALCAP;

    new Float:fUnholyGravity = UD_fUnholyAura_gravity[0] - ( ( ( UD_fUnholyAura_gravity[0] - UD_fUnholyAura_gravity[1] ) / LEVEL_RACIALCAP ) * fLevel );

    if ( fUnholyGravity < UD_fUnholyAura_gravity[1] )
        fUnholyGravity = UD_fUnholyAura_gravity[1];

    if ( fUnholyGravity > UD_fUnholyAura_gravity[0] )
        fUnholyGravity = UD_fUnholyAura_gravity[0];

    return ( fUnholyGravity );
}


// Unholy Aura ( set speed/gravity )

public UD_S_UNHOLY_set_speed( id ) {

    // Check if restricted

    if ( !WAR3_skill_enabled( id, RACE_UNDEAD, SKILL_RACIAL ) )
        return PLUGIN_HANDLED;

    // Get speed amount(s)

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new Float:fUnholySpeed = UD_S_UNHOLY_get_speed( iLevel );

    // Set speed

    set_user_maxspeed( id, fUnholySpeed );

    // Set gravity

    new Float:fUnholyGravity = UD_S_UNHOLY_get_gravity( iLevel );
    set_user_gravity( id, fUnholyGravity );

    return PLUGIN_HANDLED;
}


/* - Trainable Skills ------------------------------------------- */


// Vampiric Aura

static UD_S_VAMPIRIC( attacker, victim, weapon, iDamage ) {

    // Check if restricted

    if ( !WAR3_skill_enabled( attacker, RACE_UNDEAD, SKILL_1 ) )
        return PLUGIN_HANDLED;

    // Calculate health gained

    new iVictimHealth = get_user_health( victim ) + iDamage;

    if ( iDamage > iVictimHealth )
        iDamage = iVictimHealth;

    new Float:fBonusHealth = float( iDamage ) * UD_fVampiricAura[g_PlayerInfo[attacker][CURRENT_SKILL1] - 1];

    if ( weapon == CSW_KNIFE )
        fBonusHealth += fBonusHealth * VAMPIRIC_KNIFEBONUS;

    // Make sure health is gained

    if ( fBonusHealth < 1.0 )
        return PLUGIN_HANDLED;

    // Make sure health does not exceed max

    new iNewHealth = get_user_health( attacker ) + floatround( fBonusHealth );

    if ( iNewHealth > 100 )
    {
        fBonusHealth -= ( float( iNewHealth ) - 100.0 );
        iNewHealth = 100;
    }

    // Add to player stats array

    if ( get_cvar_num( "mp_war3stats" ) )
    {
        playerSkill1Info[attacker][0] += floatround( fBonusHealth );
    }

    // Set health

    if ( fBonusHealth >= 1.0 )
    {
        set_user_health( attacker, iNewHealth );

        new iFadeAlpha = floatround( fBonusHealth * 2.5 );

        if ( iFadeAlpha > GLOW_MAX )
            iFadeAlpha = GLOW_MAX;

        // Glow

        new iRGB[3];
        iRGB[GLOW_G] = iFadeAlpha / 4;

        SHARED_GLOW_set( attacker, glow_duration( iFadeAlpha ), iRGB, 2 );

        // Screen fade

        if ( !g_bPlayerSleeping[attacker] )
        {
            Create_ScreenFade( attacker, (1<<12), (1<<10), FADE_OUT, 0, 255, 0, iFadeAlpha );
        }
    }

    return PLUGIN_HANDLED;
}


// Frost Nova

public UD_S_FROSTNOVA( id ) {

    // Check if restricted

    if ( !WAR3_skill_enabled( id, RACE_UNDEAD, SKILL_2 ) )
        return PLUGIN_HANDLED;

    // Play sound

    emit_sound( id, CHAN_STATIC, SOUND_FROSTNOVA, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Blast effect

    new Origin[3];
    get_user_origin( id, Origin );

    Origin[2] -= 35;

    if ( get_user_button( id ) & IN_DUCK )
        Origin[2] += 20;

    // Create rings

    new iRingSize = UD_iFrostNova_range[g_PlayerInfo[id][CURRENT_SKILL2] - 1] * 40;

    new Radius[3][3];
    Radius[0][2] = iRingSize;

    // Outer ring

    Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, Origin, Radius[0], SPR_SMOOTHWAVE, 0, 0, 5, 12, 5, 255, 255, 255, 255, 0 );

    // Entity light

    Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, id, random_num( 60,100 ), FROSTARMOR_RGB[GLOW_R], FROSTARMOR_RGB[GLOW_G], FROSTARMOR_RGB[GLOW_B], 10, 5 );

    // Inner ring(s)

    switch ( g_PlayerInfo[id][CURRENT_SKILL2] )
    {
        case 2:
        {
            Radius[1][2] = iRingSize / 2;
            Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, Origin, Radius[1], SPR_SMOOTHWAVE, 0, 0, 5,  6, 5, FROSTARMOR_RGB[GLOW_R], FROSTARMOR_RGB[GLOW_G], FROSTARMOR_RGB[GLOW_B], 255, 0 );
        }

        case 3:
        {
            Radius[1][2] = iRingSize / 3 * 2;
            Radius[2][2] = iRingSize / 3;

            Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, Origin, Radius[1], SPR_SMOOTHWAVE, 0, 0, 5,  6, 5, FROSTARMOR_RGB[GLOW_R], FROSTARMOR_RGB[GLOW_G], FROSTARMOR_RGB[GLOW_B], 255, 0 );
            Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, Origin, Radius[2], SPR_SMOOTHWAVE, 0, 0, 5,  4, 5, FROSTARMOR_RGB[GLOW_R], FROSTARMOR_RGB[GLOW_G], FROSTARMOR_RGB[GLOW_B], 220, 0 );
        }
    }

    // Find enemies in range

    new Enemies[32], szTeamName[16];
    new iTotalEnemies;

    if ( get_user_team( id ) == CS_TEAM_TERRORIST )
        copy( szTeamName, 15, "CT" );               // Enemy team name

    else
    {
        copy( szTeamName, 15, "TERRORIST" );
    }

    get_players( Enemies, iTotalEnemies, "ae", szTeamName );

    if ( iTotalEnemies > 0 )
    {
        new iRadius = UD_iFrostNova_range[g_PlayerInfo[id][CURRENT_SKILL2] - 1];

        for ( new iPlayerNum = 0; iPlayerNum < iTotalEnemies; iPlayerNum++ )
        {
            new enemy = Enemies[iPlayerNum];

            new EnemyOrigin[3];
            get_user_origin( enemy, EnemyOrigin );

            if ( get_distance( Origin, EnemyOrigin ) / 40 <= iRadius && !g_bPlayerSleeping[enemy] )
            {
                // Damage

                new iDamage = UD_iFrostNova_damage[g_PlayerInfo[id][CURRENT_SKILL2] - 1];
                WAR3_damage( id, enemy, CSW_FROSTNOVA, iDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

                // Cold damage

                Create_Damage( enemy, 0, 0, CS_DMG_COLD );

                // Glow

                new Float:fDuration = random_float( FROSTNOVA_SLOWDURATION_MIN, FROSTNOVA_SLOWDURATION_MAX );
                SHARED_GLOW_set( enemy, fDuration - 0.5, FROSTNOVA_RGB, 36 );

                // Screen fade

                if ( !g_bPlayerSleeping[enemy] )
                {
                    Create_ScreenFade( enemy, (1<<10), (1<<10), FADE_OUT, FROSTNOVA_RGB[GLOW_R], FROSTNOVA_RGB[GLOW_G], FROSTNOVA_RGB[GLOW_B], 100 );
                }

                // Slow movement

                if ( get_user_health( enemy ) - iDamage > 0 )
                {
                    UD_S_FROSTNOVA_slow( enemy, fDuration );
                }

                // Message

                new szMessage[128], szPlayer[32];

                get_user_name( id, szPlayer, 31 );
                formatex( szMessage, 127, "%L", enemy, "DAMAGE_FROSTNOVA", szPlayer, iDamage );

                WAR3_status_text2( enemy, szMessage, 3.0 );

                // Play client sound

                client_cmd( enemy, "speak warcraft3/bonus/FrostNovaStateEnd.wav" );
            }
        }
    }

    return PLUGIN_HANDLED;
}


static UD_S_FROSTNOVA_slow( id, Float:fDuration ) {

    g_bPlayerNova[id] = true;
    g_bPlayerSlowed[id] = true;

    new arg_write[1];
    arg_write[0] = id;

    new task = TASK_NOVASLOW + id;

    remove_task( task, 0 );
    set_task( fDuration, "UD_S_FROSTNOVA_remove", task, arg_write, 1 );

    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


public UD_S_FROSTNOVA_remove( arg_read[1] ) {

    new id = arg_read[0];

    new task = TASK_NOVASLOW + id;
    remove_task( task, 0 );

    g_bPlayerNova[id] = false;
    SHARED_SLOW_remove( id );

    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


// Frost Armor

public UD_S_FROSTARMOR( victim, attacker ) {

    // Check if restricted

    if ( !WAR3_skill_enabled( victim, RACE_UNDEAD, SKILL_3 ) )
        return PLUGIN_HANDLED;

    new Float:fSlowChance = UD_fFrostArmor_slow[g_PlayerInfo[victim][CURRENT_SKILL3] - 1];
    new Float:fRandomNum = random_float( 0.0,1.0 );

    if ( fSlowChance >= fRandomNum )
    {
        if ( get_user_armor( attacker ) )
        {
            new iNewArmor = get_user_armor( attacker ) - FROSTARMOR_ARMOR;

            if ( iNewArmor < 0 )
                iNewArmor = 0;

            set_user_armor( attacker, iNewArmor );

            // Regenerate Armor (if orc)

            if ( g_PlayerInfo[attacker][CURRENT_RACE] == RACE_ORC && !g_bPlayerRegen[attacker] && get_user_armor( attacker ) )
                OR_S_REGENERATION_set( attacker );
        }

        // Add to player stats array

        if ( get_cvar_num( "mp_war3stats" ) )
        {
            playerSkill3Info[victim][0] += 1;
        }

        // Slow movement

        UD_S_FROSTARMOR_slow( attacker );

        // Glow

        SHARED_GLOW_set( attacker, FROSTARMOR_SLOWDURATION - 0.5, FROSTARMOR_RGB, 36 );

        // Screen fade

        if ( !g_bPlayerSleeping[attacker] )
        {
            Create_ScreenFade( attacker, (1<<10), (1<<10), FADE_OUT, 91, 168, 248, 100 );
        }

        // Cold damage

        Create_Damage( attacker, 0, 0, CS_DMG_COLD );

        // Slow icon ( client pack )

        Create_StatusIcon( victim, ICON_SHOW, "slow", 255, 170, 0 );
    }

    return PLUGIN_HANDLED;
}


static UD_S_FROSTARMOR_slow( id ) {

    g_bPlayerFrosted[id] = true;
    g_bPlayerSlowed[id] = true;

    new arg_write[1];
    arg_write[0] = id;

    new task = TASK_FROSTSLOW + id;

    remove_task( task, 0 );
    set_task( FROSTARMOR_SLOWDURATION, "UD_S_FROSTARMOR_remove", task, arg_write, 1 );

    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


public UD_S_FROSTARMOR_remove( arg_read[1] ) {

    new id = arg_read[0];

    new task = TASK_FROSTSLOW + id;
    remove_task( task, 0 );

    g_bPlayerFrosted[id] = false;
    SHARED_SLOW_remove( id );

    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


/* - Death Coil ------------------------------------------------- */


static UD_U_DEATHCOIL( caster, target ) {

    // Play client sound

    client_cmd( caster, "speak warcraft3/bonus/DeathCoilMissileLaunch1.wav" );

    // Create skull model

    new Float:fOrigin[3];
    entity_get_vector( caster, EV_VEC_origin, fOrigin );

    new skull = WAR3_ENTITY_create( "DEATH_COIL", "models/bskull_template1.mdl", fOrigin, MOVETYPE_NOCLIP, SOLID_TRIGGER, DEATHCOIL_DURATION );

    entity_set_edict( skull, EV_ENT_owner, caster );
    entity_set_edict( skull, EV_ENT_enemy, target );

    // Skull effects

    set_entity_rendering( skull, kRenderFxGlowShell, DEATHCOIL_SHELL_RGB[GLOW_R], DEATHCOIL_SHELL_RGB[GLOW_G], DEATHCOIL_SHELL_RGB[GLOW_B], kRenderNormal, 48 );
    Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST, skull, SPR_BEAMFOLLOW, 5, 8, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G], DEATHCOIL_TRAIL_RGB[GLOW_B], 160 );

    // Seek target

    new arg_write[1];
    arg_write[0] = skull;

    UD_U_DEATHCOIL_seek( arg_write );

    return PLUGIN_HANDLED;
}


public UD_U_DEATHCOIL_seek( arg_read[1] ) {

    new skull = arg_read[0];

    if ( !WAR3_is_valid_ent( skull ) )
        return PLUGIN_HANDLED;

    new target = entity_get_edict( skull, EV_ENT_enemy );

    if ( !is_user_alive( target ) )
        return PLUGIN_HANDLED;

    // Project skull towards target

    set_velocity_to_ent( skull, target, DEATHCOIL_VELOCITY );

    // Repeat until target is hit

    new task = TASK_TEMPENTITY + skull;
    set_task( 0.1, "UD_U_DEATHCOIL_seek", task, arg_read, 1 );

    return PLUGIN_HANDLED;
}


static UD_U_DEATHCOIL_damage( caster, target ) {

    // Play sound

    emit_sound( target, CHAN_STATIC, SOUND_DEATHCOIL, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Generate effects

    UD_U_DEATHCOIL_effects( target );

    // Check for Amulet

    if ( g_PlayerInfo[target][CURRENT_ITEM] == ITEM_AMULET )
    {
        ITEM_AMULET_block( target, caster );
        return PLUGIN_HANDLED;
    }

    // Hud Message

    new szMessage[128], szPlayerName[32];
    get_user_name( caster, szPlayerName, 31 );

    formatex( szMessage, 127, "%L", target, "CAST_DEATHCOIL", szPlayerName );

    WAR3_status_text2( target, szMessage, 3.0 );

    // Unset velocity

    new Float:fVelocity[3];
    entity_get_vector( target, EV_VEC_velocity, fVelocity );

    fVelocity[0] = 0.0;
    fVelocity[1] = 0.0;

    // Apply damage

    WAR3_damage( caster, target, CSW_DEATHCOIL, DEATHCOIL_DAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    return PLUGIN_HANDLED;
}


static UD_U_DEATHCOIL_heal( caster, target ) {

    // Play sound

    emit_sound( target, CHAN_STATIC, SOUND_DEATHCOIL, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Status text

    new szCastMessage[64];
    formatex( szCastMessage, 63, "%L", caster, "HEAL_CAST" );

    WAR3_status_text( caster, szCastMessage, 1.0 );

    // Heal player

    new iHealthGiven = WAR3_heal( caster, target, DEATHCOIL_HEAL );

    // Hud message

    new szMessage[128], szPlayerName[32];
    get_user_name( caster, szPlayerName, 31 );

    formatex( szMessage, 127, "%L", target, "HEAL_TARGET", szPlayerName, iHealthGiven );

    WAR3_status_text( target, szMessage, 3.0 );

    // Display effects

    UD_U_DEATHCOIL_effects( target );

    return PLUGIN_HANDLED;
}


static UD_U_DEATHCOIL_effects( target ) {

    // Green screefade

    Create_ScreenFade( target, (1<<10), (1<<10), FADE_OUT, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G], DEATHCOIL_TRAIL_RGB[GLOW_B], 160 );

    // Play explosion

    new Origin[3];
    get_user_origin( target, Origin );

    Create_TE_EXPLOSION( SHOWTO_ALL_BROADCAST, Origin, SPR_DEATHCOIL, 20, 24, 4 );

    // Glow

    Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, target, 100, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G], DEATHCOIL_TRAIL_RGB[GLOW_B], 10, 0 );

    return PLUGIN_HANDLED;
}


static UD_U_DEATHCOIL_remove( skull ) {

    new task = TASK_TEMPENTITY + skull;
    remove_task( task, 0 );

    remove_entity( skull );

    return PLUGIN_HANDLED;
}


public UD_U_DEATHCOIL_touch( skull, player ) {

    // Sanity checks

    if ( player < 1 || player > 32 || skull < 1 || !is_user_alive( player ) )
        return PLUGIN_CONTINUE;

    new caster = entity_get_edict( skull, EV_ENT_owner );
    new target = entity_get_edict( skull, EV_ENT_enemy );

    if ( player == target )
    {
        if ( get_user_team( caster ) == get_user_team( target ) )
            UD_U_DEATHCOIL_heal( caster, target );

        else
        {
            UD_U_DEATHCOIL_damage( caster, target );
        }

        UD_U_DEATHCOIL_remove( skull );
    }

    return PLUGIN_CONTINUE;
}


/* - Impale ----------------------------------------------------- */


static UD_U_IMPALE( caster, target ) {

    // Play sound

    emit_sound( target, CHAN_STATIC, SOUND_IMPALE, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Raise claw

    new Float:fOrigin[3], Origin[3];
    entity_get_vector( target, EV_VEC_origin, fOrigin );

    fOrigin[2] += 180.0;
    new claw = WAR3_ENTITY_create( "IMPALE_CLAW", "models/tentacle2.mdl", fOrigin, MOVETYPE_TOSS, SOLID_NOT, 1.5 );

    new Float:fVelocity[3] = { 0.0, 0.0, 500.0 };
    entity_set_vector( claw, EV_VEC_velocity, fVelocity );
    entity_set_float( claw, EV_FL_gravity, 1.5 );

    set_entity_rendering( claw, kRenderFxGlowShell, 128, 96, 160, kRenderTransTexture, 32 );

    // Check for Amulet

    if ( g_PlayerInfo[target][CURRENT_ITEM] == ITEM_AMULET )
    {
        ITEM_AMULET_block( target, caster );
        return PLUGIN_HANDLED;
    }

    // Launch into air

    new Velocity[3];
    get_entity_velocity( target, Velocity );

    Velocity[2] = IMPALE_VELOCITY;
    set_entity_velocity( target, Velocity );

    // Beam trail

    Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST, target, SPR_BEAMFOLLOW, 5, 3, 160, 0, 48, 128 );

    // Blood sprites

    for ( new i = 0; i < 2; i++ )
    {
        get_user_origin( target, Origin );

        for ( new j = 0; j < 5; j++ )
        {
            Origin[0] += random_num( -100,100 ) * j;
            Origin[1] += random_num( -100,100 ) * j;
            Origin[2] += random_num( 0,100 ) * j;

            Create_TE_BLOODSPRITE( SHOWTO_ALL_BROADCAST, Origin, SPR_BLOODSPRAY, SPR_BLOODDROP, 248, 15 );
        }
    }

    // Blood decals

    for ( new i = 0; i < 5; i++ )
    {
        static const BLOOD_SMALL[7] = {190,191,192,193,194,195,197};

        get_user_origin( target, Origin );

        Origin[0] += random_num( -100,100 );
        Origin[1] += random_num( -100,100 );
        Origin[2] -= 36;

        Create_TE_WORLDDECAL( SHOWTO_ALL, Origin, BLOOD_SMALL[random_num( 0,6 )] );
    }

    // Remove armor

    if ( get_user_armor( target ) )
    {
        new iNewArmor = get_user_armor( target ) - IMPALE_ARMORDAMAGE;

        if ( iNewArmor < 0 )
            iNewArmor = 0;

        set_user_armor( target, iNewArmor );
    }

    // Remove health

    WAR3_damage( caster, target, CSW_IMPALE, IMPALE_DAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( is_user_alive( target ) )
    {
        // Screen Fade

        Create_ScreenFade( target, (1<<10), (1<<10), FADE_OUT, 160, 0, 48, 160 );

        new arg_write[3];
        arg_write[0] = target;
        arg_write[1] = Origin[2];

        g_PlayerImpaled[target] = caster;

        new task = TASK_IMPALE + target;
        set_task( 0.1, "UD_U_IMPALE_think", task, arg_write, 3 );
    }

    return PLUGIN_HANDLED;
}


public UD_U_IMPALE_think( arg_read[3] ) {

    new target = arg_read[0];
    new caster = g_PlayerImpaled[target];

    new OldOrigin = arg_read[1];
    new OldVelocity = arg_read[2];

    new Velocity[3];
    get_entity_velocity( target, Velocity );

    // Checks player height on descent, assumes collision if < IMPALE_MINDISTANCE

    if ( OldVelocity >= 0 && Velocity[2] <= 0 )
    {
        new Origin[3];
        get_user_origin( target, Origin );

        new iDistance = Origin[2] - OldOrigin;

        if ( iDistance < IMPALE_MINDISTANCE )
        {
            // Collision sound

            new szSoundfile[32];

            switch ( random_num( 1, 3 ) )
            {
                case 1: copy( szSoundfile, 31, "player/headshot1.wav" );
                case 2: copy( szSoundfile, 31, "player/headshot2.wav" );
                case 3: copy( szSoundfile, 31, "player/headshot3.wav" );
            }

            emit_sound( target, CHAN_STATIC, szSoundfile, 1.0, ATTN_NORM, 0, PITCH_NORM );

            // Blood streams

            Origin[2] += 10;

            for ( new i = 0; i < 2; i++ )
            {
                new Vector[3];
                Vector[0] = random_num( 20,300 );
                Vector[1] = random_num( 20,300 );
                Vector[2] = 0;

                Create_TE_BLOODSTREAM( SHOWTO_ALL_BROADCAST, Origin, Vector, 70, random_num( 100,200 ) );
            }

            // Apply damage

            WAR3_damage( caster, target, CSW_IMPALE, IMPALE_COLLISIONDAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

            // Screen fade

            if ( is_user_alive( target ) )
                Create_ScreenFade( target, (1<<10), (1<<10), FADE_MODULATE_OUT, 0, 0, 0, 160 );
        }
    }

    if ( !( get_entity_flags( target ) & FL_ONGROUND ) && g_PlayerImpaled[target] )
    {
        arg_read[2] = Velocity[2];

        new task = TASK_IMPALE + target;
        set_task( 0.1, "UD_U_IMPALE_think", task, arg_read, 3 );
    }

    else
    {
        UD_U_IMPALE_remove( target );
    }

    return PLUGIN_HANDLED;
}


public UD_U_IMPALE_remove( id ) {

    new task = TASK_IMPALE + id;
    remove_task( task, 0 );

    g_PlayerImpaled[id] = 0;
    Remove_TE_BEAMFOLLOW( SEENBY_ALL, id );
}


/* - Sleep ------------------------------------------------------ */


static UD_U_SLEEP( caster, target ) {

    UD_U_SLEEP_remove( target );

    // Play sound

    emit_sound( target, CHAN_STATIC, SOUND_SLEEP, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Check for Amulet

    if ( g_PlayerInfo[target][CURRENT_ITEM] == ITEM_AMULET )
    {
        ITEM_AMULET_block( target, caster );
        return PLUGIN_HANDLED;
    }

    // Switch to knife

    new weapon, iClip, iAmmo;
    weapon = get_user_weapon( target, iClip, iAmmo );

    if ( weapon != CSW_KNIFE )
        engclient_cmd( target, "weapon_knife" );

    // Immobilize

    g_bPlayerSleeping[target] = true;
    g_bPlayerCantMove[target] = true;

    WAR3_set_speed( target );

    // Hud Message

    new szMessage[128], szPlayerName[32];
    get_user_name( caster, szPlayerName, 31 );

    formatex( szMessage, 127, "%L", target, "CAST_SLEEP", szPlayerName );

    WAR3_status_text2( target, szMessage, 3.0 );

    // Hit zones ( remove )

    set_user_hitzones( target, 0, 0 );
    entity_set_float( target, EV_FL_takedamage, 0.0 );

    // Render

    set_user_rendering( target, kRenderFxNone, 0, 0, 0, kRenderTransAdd, 128 );

    // Screen fade

    new Float:fDuration = random_float( SLEEP_DURATION_MIN, SLEEP_DURATION_MAX );
    new iHoldTime = floatround( fDuration );

    fDuration = float( iHoldTime );

    Create_ScreenFade( target, (1<<12), (iHoldTime<<12), FADE_MODULATE_IN, 0, 0, 0, 160 );

    // Progress bar

    Create_BarTime( target, iHoldTime );

    // Effects

    new arg_Z1[3], arg_Z2[3];
    arg_Z1[0] = target;
    arg_Z2[0] = target;

    UD_U_SLEEP_effect( arg_Z1 );

    new task = TASK_SLEEP + target;
    set_task( 0.4, "UD_U_SLEEP_effect", task, arg_Z2, 3 );

    // Wake task

    new arg_write[1];
    arg_write[0] = target;

    set_task( fDuration, "UD_U_SLEEP_wake", task, arg_write, 1 );

    return PLUGIN_HANDLED;
}


public UD_U_SLEEP_effect( arg_read[3] ) {

    new target = arg_read[0];
    new sleep_z = arg_read[1];
    new iCounter  = arg_read[2];

    if ( !iCounter )
    {
        // Create Z's

        new Float:fTargetOrigin[3];
        entity_get_vector( target, EV_VEC_origin, fTargetOrigin );

        fTargetOrigin[2] += 36.0;

        sleep_z = WAR3_ENTITY_create( "SLEEP_Z", "sprites/warcraft3/sleep.spr", fTargetOrigin, MOVETYPE_NOCLIP, SOLID_NOT, 1.5 );

        entity_set_float( sleep_z, EV_FL_renderamt, 128.0 );
        entity_set_int( sleep_z, EV_INT_rendermode, kRenderTransAdd );
        entity_set_int( sleep_z, EV_INT_renderfx, kRenderFxClampMinScale );

        entity_set_edict( sleep_z, EV_ENT_owner, target );

        // Project Upwards

        new Float:fVelocity[3];
        fVelocity[0] = random_float( -10.0, 10.0 );
        fVelocity[1] = random_float( -10.0, 10.0 );
        fVelocity[2] = 25.0;

        entity_set_vector( sleep_z, EV_VEC_velocity, fVelocity );
        entity_set_float( sleep_z, EV_FL_scale, random_float( 0.3, 0.4 ) );
    }

    if ( iCounter <= 10 )
    {
        if ( iCounter > 5 && is_valid_ent( sleep_z ) )
        {
            new Float:fScale = entity_get_float( sleep_z, EV_FL_scale );

            fScale -= 0.02;
            entity_set_float( sleep_z, EV_FL_scale, fScale );
        }

        // Re-render player ( HACKHACK )

        set_user_rendering( target, kRenderFxNone, 0, 0, 0, kRenderTransAdd, 128 );

        // Do it all again!

        iCounter++;

        arg_read[1] = sleep_z;
        arg_read[2] = iCounter;

        new task = TASK_TEMPENTITY + sleep_z;
        set_task( 0.1, "UD_U_SLEEP_effect", task, arg_read, 3 );
    }

    else if ( g_bPlayerSleeping[target] )
    {
        arg_read[1] = 0;
        arg_read[2] = 0;

        new task = TASK_TEMPENTITY + sleep_z;
        set_task( 0.5, "UD_U_SLEEP_effect", task, arg_read, 3 );
    }

    return PLUGIN_HANDLED;
}


public UD_U_SLEEP_wake( arg_read[1] ) {

    new target = arg_read[0];

    // Screen fade

    Create_ScreenFade( target, (1<<12), 0, FADE_MODULATE_OUT, 0, 0, 0, 160 );

    // Hud message

    new szMessage[128];
    formatex( szMessage, 127, "%L", target, "FINISH_SLEEP" );

    WAR3_status_text( target, szMessage, 3.0 );

    // Enable attack for an additional second ( to prevent sniper exploiting ).

    set_user_hitzones( target, 0, 255 );

    new task = TASK_SLEEP + target;
    set_task( SLEEP_MOVETIME, "UD_U_SLEEP_finish", task, arg_read, 1 );

    g_bPlayerSleeping[target] = false;

    // Set player speed

    SHARED_IMMOBILIZE_remove( target );
    WAR3_set_speed( target );

    return PLUGIN_HANDLED;
}


public UD_U_SLEEP_finish( arg_read[1] ) {

    new target = arg_read[0];
    UD_U_SLEEP_remove( target );

    return PLUGIN_HANDLED;
}


public UD_U_SLEEP_remove( id ) {

    new task = TASK_SLEEP + id;
    remove_task( task, 0 );

    g_bPlayerSleeping[id] = false;

    // Hit zones ( restore )

    set_user_hitzones( id, 0, 255 );
    entity_set_float( id, EV_FL_takedamage, 2.0 );

    // Rendering

    set_user_rendering( id );

    // Invis cooldown

    SHARED_INVIS_cooldown( id );

    // Set player speed

    SHARED_IMMOBILIZE_remove( id );
    WAR3_set_speed( id );

    // Remove z's

    WAR3_ENTITY_remove_class( "SLEEP_Z", id );

    Icon_DispellMe( id );

    return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. - //
