// Begin UNDEAD.INL

/* - Skill Configuration ---------------------------------------- */


new Float:s_UnholyAura[2]       = {SPEED_KNIFE,300.0};  // (racial) Unholy Aura (260.0 = knife speed)
new Float:s_UnholyGravity[2]    = {1.0,0.75};           // (racial) Unholy Aura (gravity percentage)
new Float:s_VampiricAura[3]     = {0.15,0.30,0.45};     // (skill1) Vampiric Aura (health percent)
new s_fnDamage[3]               = {5,10,15};            // (skill2) Frost Nova (damage dealt)
new s_fnRange[3]                = {4,5,6};              // (skill2) Frost Nova (range of blast)
new s_faArmor[3]                = {110,120,130};        // (skill3) Frost Armor (armor ammount)
new Float:s_faSlow[3]           = {0.1,0.2,0.3};        // (skill3) Frost Armor (chance to slow)


/* - Skill Constants Configuration ------------------------------ */


#define VAMPIRIC_KNIFEBONUS                 0.50        // (  float) bonus to apply to vampiric aura when using knife.

#define FROSTNOVA_SLOWSPEED                 75.0        // (  float) max speed when slowed by frost nova
#define FROSTNOVA_SLOWDURATION_MIN           1.0        // (  float) min duration in seconds player is slowed by frost nova ( minimum = 1 second )
#define FROSTNOVA_SLOWDURATION_MAX           5.0        // (  float) max duration in seconds player is slowed by frost nova ( minimum = 1 second )

#define FROSTARMOR_ARMOR                      15        // (integer) armor removed when player slowed with frost armor
#define FROSTARMOR_SLOWSPEED               150.0        // (  float) max speed when slowed by frost armor
#define FROSTARMOR_SLOWDURATION              3.0        // (  float) seconds player is slowed by frost armor

new FROST_RGB[3] =                    {23,42,62};       // (integer) RGB of frost slow shell ( when slowed )


/* - Ultimate Configuration ------------------------------------- */


#define DEATHCOIL_HEAL                        75        // (integer) ammount of health to give friendly undead target
#define DEATHCOIL_DAMAGE                      60        // (integer) ammount of damage to deal living enemy target
#define DEATHCOIL_VELOCITY                 800.0        // (  float) velocity of death coil towards target
#define DEATHCOIL_DURATION                   3.0        // (  float) life of deathcoil before removal

#define IMPALE_DAMAGE                         15        // (integer) initial impale damage
#define IMPALE_ARMORDAMAGE                    20        // (integer) initial impale damage
#define IMPALE_VELOCITY                      630        // (integer) upward velocity (620 ~ 30 damage)
#define IMPALE_MINDISTANCE                   200        // (integer) any impale that doesnt span this distance will assume ceiling collision
#define IMPALE_COLLISIONDAMAGE                20        // (integer) damage dealt for collision with ceiling

#define SLEEP_DURATION_MIN                   8.0        // (  float) min duration player is disabled
#define SLEEP_DURATION_MAX                  12.0        // (  float) max duration player is disabled
#define SLEEP_MOVETIME                       0.8        // (  float) duration after sleep is done that player is unattackable

new DEATHCOIL_SHELL_RGB[3] =          {160,255,0};       // (integer) RGB of glow shell over skull
new DEATHCOIL_TRAIL_RGB[3] =           {96,224,0};       // (integer) RGB of beamfollow trail from skull


/* - Events ----------------------------------------------------- */


public Skills_Offensive_UD( attackerId, victimId, weaponId, iDamage, headshot ) {
#if ADVANCED_DEBUG
  log_function("public Skills_Offensive_UD( attackerId, victimId, weaponId, iDamage, headshot ) {");
#endif

    if ( g_PlayerInfo[attackerId][CURRENT_RACE] == RACE_UNDEAD && get_user_team( attackerId ) != get_user_team( victimId ) )
    {
        // Vampiric Aura

        if ( g_PlayerInfo[attackerId][CURRENT_SKILL1] && cs_get_weapon_type_( weaponId ) != CS_WEAPON_TYPE_GRENADE )
            SVampiricAura( attackerId, victimId, weaponId, iDamage );
    }

    return PLUGIN_HANDLED;
}


public Skills_Defensive_UD( attackerId, victimId, weaponId, iDamage, headshot ) {
#if ADVANCED_DEBUG
  log_function("public Skills_Defensive_UD( attackerId, victimId, weaponId, iDamage, headshot ) {");
#endif

    if ( g_PlayerInfo[victimId][CURRENT_RACE] == RACE_UNDEAD && get_user_team( attackerId ) != get_user_team( victimId ) && attackerId )
    {
        // Frost Armor

        if ( g_PlayerInfo[victimId][CURRENT_SKILL3] && get_user_armor( victimId ) )
            SFrostArmor( victimId, attackerId );
    }

    return PLUGIN_HANDLED;
}


public Ultimates_UD( casterId, targetId ) {
#if ADVANCED_DEBUG
  log_function("public Ultimates_UD( casterId, targetId ) {");
#endif

    // Death Coil

    if ( g_PlayerInfo[casterId][CURRENT_ULTIMATE] == ULTIMATE_DEATHCOIL )
    {
        // Heal

        if ( get_user_team( targetId ) == get_user_team( casterId ) && g_PlayerInfo[targetId][CURRENT_RACE] == RACE_UNDEAD )
        {
            if ( get_user_health( targetId ) == WAR3_get_maxhealth( targetId ) && casterId != targetId )
            {
                new szMessage[128];
                copy( szMessage, 127, FULLHEALTH_TARGET );

                WAR3_status_text( casterId, szMessage, 0.5 );

                Ultimate_Beep( casterId );
            }

            else
            {
                UCoil_Cast( casterId, targetId );
                Ultimate_Cooldown( casterId, ULTIMATE_COOLDOWNDEFAULT );

                Invis_Cooldown( casterId );
            }
        }

        // Damage

        else if ( get_user_team( targetId ) != get_user_team( casterId ) && g_PlayerInfo[targetId][CURRENT_RACE] != RACE_UNDEAD )
        {
            UCoil_Cast( casterId, targetId );
            Ultimate_Cooldown( casterId, ULTIMATE_COOLDOWNDEFAULT );

            Invis_Cooldown( casterId );
        }

        else
        {
            new szMessage[128];
            copy( szMessage, 127, CANT_TARGET_DEATHCOIL );

            WAR3_status_text( casterId, szMessage, 0.5 );

            Ultimate_Beep( casterId );
        }
    }

    // Impale

    else if ( g_PlayerInfo[casterId][CURRENT_ULTIMATE] == ULTIMATE_IMPALE && get_user_team( targetId ) != get_user_team( casterId ) )
    {
        if ( !( get_entity_flags( targetId ) & FL_ONGROUND ) )
        {
            new szMessage[128];
            copy( szMessage, 127, CANT_TARGET_AIR );

            WAR3_status_text( casterId, szMessage, 0.5 );

            Ultimate_Beep( casterId );
        }

        else
        {
            UImpale_Cast( casterId, targetId );
            Ultimate_Cooldown( casterId, ULTIMATE_COOLDOWNDEFAULT );

            Invis_Cooldown( casterId );
        }
    }

    // Sleep

    else if ( g_PlayerInfo[casterId][CURRENT_ULTIMATE] == ULTIMATE_SLEEP && get_user_team( targetId ) != get_user_team( casterId ) )
    {
        USleep_Cast( casterId, targetId );
        Ultimate_Cooldown( casterId, ULTIMATE_COOLDOWNDEFAULT );

        Invis_Cooldown( casterId );
    }

    else
    {
        Ultimate_Beep( casterId );
        return PLUGIN_HANDLED;
    }

    return PLUGIN_HANDLED;
}


/* - Racial Ability --------------------------------------------- */


// Unholy Aura (get speed)

public Float:SUnholyAura_Get( iLevel ) {
#if ADVANCED_DEBUG
  log_function("public Float:SUnholyAura_Get( iLevel ) {");
#endif

    new Float:fLevel = float( iLevel );

    if ( fLevel > LEVEL_RACIALCAP )
        fLevel = LEVEL_RACIALCAP;

    new Float:fUnholySpeed = s_UnholyAura[0] + ( ( ( s_UnholyAura[1] - s_UnholyAura[0] ) / LEVEL_RACIALCAP ) * fLevel );

    if ( fUnholySpeed > s_UnholyAura[1] )
        fUnholySpeed = s_UnholyAura[1];

    return ( fUnholySpeed );
}


// Unholy Aura (get gravity)

public Float:SUnholyGravity_Get( iLevel ) {
#if ADVANCED_DEBUG
  log_function("public Float:SUnholyGravity_Get( iLevel ) {");
#endif

    new Float:fLevel = float( iLevel );

    if ( fLevel > LEVEL_RACIALCAP )
        fLevel = LEVEL_RACIALCAP;

    new Float:fUnholyGravity = s_UnholyGravity[1] + ( ( ( s_UnholyGravity[0] - s_UnholyGravity[1] ) / LEVEL_RACIALCAP ) * fLevel );

    if ( fUnholyGravity < s_UnholyGravity[1] )
        fUnholyGravity = s_UnholyGravity[1];

    return ( fUnholyGravity );
}


// Unholy Aura (set speed/gravity)

public SUnholyAura_Set( id, weaponId ) {
#if ADVANCED_DEBUG
  log_function("public SUnholyAura_Set( id, weaponId ) {");
#endif

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new Float:fUnholySpeed = SUnholyAura_Get( iLevel );

    // Check if restricted

    if ( !WAR3_skill_enabled( id, RACE_UNDEAD, SKILL_RACIAL ) )
        return PLUGIN_HANDLED;

    if ( fUnholySpeed < SPEED_KNIFE && weaponId == CSW_KNIFE )
        set_user_maxspeed( id, SPEED_KNIFE );

    else
    {
        set_user_maxspeed( id, fUnholySpeed );
    }

    // Set gravity

    new Float:fUnholyGravity = SUnholyGravity_Get( iLevel );
    set_user_gravity( id, fUnholyGravity );

    return PLUGIN_HANDLED;
}


/* - Skills ----------------------------------------------------- */


// Vampiric Aura

public SVampiricAura( attacker, victim, weapon, iDamage ) {
#if ADVANCED_DEBUG
  log_function("public SVampiricAura( attackerId, victimId, iDamage ) {");
#endif

    // Check if restricted

    if ( !WAR3_skill_enabled( attacker, RACE_UNDEAD, SKILL_1 ) )
        return PLUGIN_HANDLED;

    new iVictimHealth = get_user_health( victim ) + iDamage;

    if ( iDamage > iVictimHealth )
        iDamage = iVictimHealth;

    new Float:fBonusHealth = float( iDamage ) * s_VampiricAura[g_PlayerInfo[attacker][CURRENT_SKILL1] - 1];

    if ( weapon == CSW_KNIFE )
        fBonusHealth += fBonusHealth * VAMPIRIC_KNIFEBONUS;

    // Make sure health is gained

    if ( fBonusHealth < 1.0 )
        return PLUGIN_HANDLED;


    // Calculate health

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

        Glow_Set( attacker, glow_duration( iFadeAlpha ), iRGB, 2 );

        // Screen Fade

        if ( !g_bPlayerSleeping[attacker] )
        {
            Create_ScreenFade( attacker, (1<<12), (1<<10), FADE_OUT, 0, 255, 0, iFadeAlpha );
        }
    }

    return PLUGIN_HANDLED;
}


// Frost Nova

public SFrostNova( id ) {

    // Check if restricted

    if ( !WAR3_skill_enabled( id, RACE_UNDEAD, SKILL_2 ) )
        return PLUGIN_HANDLED;

    // Play sound


    // Blast effect

    new Origin[3];
    get_user_origin( id, Origin );

    Origin[2] -= 20;

    new iRingSize = s_fnRange[g_PlayerInfo[id][CURRENT_SKILL2] - 1] * 40;

    new Radius[3][3];
    Radius[0][2] = iRingSize;

    // Outer Ring

    Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, Origin, Radius[0], SPR_SMOOTHWAVE, 0, 0, 5, 12, 5, 255, 255, 255, 255, 0 );

    // Entity Light

    Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, id, random_num( 60,100 ), FROST_RGB[GLOW_R], FROST_RGB[GLOW_G], FROST_RGB[GLOW_B], 10, 5 );

    // Inner Ring(s)

    switch ( g_PlayerInfo[id][CURRENT_SKILL2] )
    {
        case 2:
        {
            Radius[1][2] = iRingSize / 2;
            Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, Origin, Radius[1], SPR_SMOOTHWAVE, 0, 0, 5,  6, 5, FROST_RGB[GLOW_R], FROST_RGB[GLOW_G], FROST_RGB[GLOW_B], 255, 0 );
        }

        case 3:
        {
            Radius[1][2] = iRingSize / 3 * 2;
            Radius[2][2] = iRingSize / 3;

            Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, Origin, Radius[1], SPR_SMOOTHWAVE, 0, 0, 5,  6, 5, FROST_RGB[GLOW_R], FROST_RGB[GLOW_G], FROST_RGB[GLOW_B], 255, 0 );
            Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, Origin, Radius[2], SPR_SMOOTHWAVE, 0, 0, 5,  4, 5, FROST_RGB[GLOW_R], FROST_RGB[GLOW_G], FROST_RGB[GLOW_B], 220, 0 );
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
        new iRadius = s_fnRange[g_PlayerInfo[id][CURRENT_SKILL2] - 1];

        for ( new iPlayerNum = 0; iPlayerNum < iTotalEnemies; iPlayerNum++ )
        {
            new enemy = Enemies[iPlayerNum];

            new EnemyOrigin[3];
            get_user_origin( enemy, EnemyOrigin );

            if ( get_distance( Origin, EnemyOrigin ) / 40 <= iRadius && !g_bPlayerSleeping[enemy] )
            {
                // Damage

                new iDamage = s_fnDamage[g_PlayerInfo[id][CURRENT_SKILL2] - 1];
                WAR3_damage( id, enemy, CSW_FROSTNOVA, iDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

                // Cold Damage

                Create_Damage( enemy, 0, 0, CS_DMG_COLD );

                // Glow

                new Float:fDuration = random_float( FROSTNOVA_SLOWDURATION_MIN, FROSTNOVA_SLOWDURATION_MAX );
                Glow_Set( enemy, fDuration - 0.5, FROST_RGB, 36 );

                // Screen Fade

                if ( !g_bPlayerSleeping[enemy] )
                {
                    Create_ScreenFade( enemy, (1<<10), (1<<10), FADE_OUT, 91, 168, 248, 100 );
                }

                // Slow movement

                if ( get_user_health( enemy ) - iDamage > 0 )
                {
                    SFrostNova_Slow( enemy, fDuration );
                }

                // Message

                new szMessage[128];
                format( szMessage, 127, DAMAGE_FROSTNOVA, id, iDamage );

                WAR3_status_text2( enemy, szMessage, 3.0 );

            }
        }
    }

    return PLUGIN_HANDLED;
}


public SFrostNova_Slow( id, Float:fDuration ) {

    g_bPlayerNova[id] = true;
    g_bPlayerSlowed[id] = true;

    new parm_Slow[1];
    parm_Slow[0] = id;

    new TaskId = TASK_NOVASLOW + id;

    remove_task( TaskId, 0 );
    set_task( fDuration, "SFrostNova_Remove", TaskId, parm_Slow, 1 );

    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


public SFrostNova_Remove( parm_Slow[1] ) {

    new id = parm_Slow[0];

    new TaskId = TASK_NOVASLOW + id;
    remove_task( TaskId, 0 );

    g_bPlayerNova[id] = false;
    Slow_Remove( id );

    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


// Frost Armor

public SFrostArmor( victimId, attackerId ) {
#if ADVANCED_DEBUG
  log_function("public SFrostArmor( victimId, attackerId ) {");
#endif

    // Check if restricted

    if ( !WAR3_skill_enabled( victimId, RACE_UNDEAD, SKILL_3 ) )
        return PLUGIN_HANDLED;

    new Float:fSlowChance = s_faSlow[g_PlayerInfo[victimId][CURRENT_SKILL3] - 1];
    new Float:fRandomNum = random_float( 0.0,1.0 );

    if ( fSlowChance >= fRandomNum )
    {
        if ( get_user_armor( attackerId ) )
        {
            new iNewArmor = get_user_armor( attackerId ) - FROSTARMOR_ARMOR;

            if ( iNewArmor < 0 )
                iNewArmor = 0;

            set_user_armor( attackerId, iNewArmor );

            // Regenerate Armor (if orc)

            if ( g_PlayerInfo[attackerId][CURRENT_RACE] == RACE_ORC && !g_bPlayerRegen[attackerId] )
                SRegen_Set( attackerId );
        }

        // Add to player stats array

        if ( get_cvar_num( "mp_war3stats" ) )
        {
            playerSkill3Info[victimId][0] += 1;
        }

        // Slow movement

        SFrostArmor_Slow( attackerId );

        // Glow

        Glow_Set( attackerId, FROSTARMOR_SLOWDURATION - 0.5, FROST_RGB, 36 );

        // Screen Fade

        if ( !g_bPlayerSleeping[attackerId] )
        {
            Create_ScreenFade( attackerId, (1<<10), (1<<10), FADE_OUT, 91, 168, 248, 100 );
        }

        // Cold Damage

        Create_Damage( attackerId, 0, 0, CS_DMG_COLD );

        // Slow Icon (client pack)

        Create_StatusIcon( victimId, ICON_SHOW, "slow", 255, 170, 0 );
    }

    return PLUGIN_HANDLED;
}


public SFrostArmor_Slow( id ) {
#if ADVANCED_DEBUG
  log_function("public SFrostArmor_Slow( id ) {");
#endif

    g_bPlayerFrosted[id] = true;
    g_bPlayerSlowed[id] = true;

    new parm_Slow[1];
    parm_Slow[0] = id;

    new TaskId = TASK_FROSTSLOW + id;

    remove_task( TaskId, 0 );
    set_task( FROSTARMOR_SLOWDURATION, "SFrostArmor_Remove", TaskId, parm_Slow, 1 );

    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


public SFrostArmor_Remove( parm_Slow[1] ) {
#if ADVANCED_DEBUG
  log_function("public SFrostArmor_Remove( parm_Slow[1] ) {");
#endif

    new id = parm_Slow[0];

    new TaskId = TASK_FROSTSLOW + id;
    remove_task( TaskId, 0 );

    g_bPlayerFrosted[id] = false;
    Slow_Remove( id );

    WAR3_set_speed( id );

    return PLUGIN_HANDLED;
}


/* - Death Coil ------------------------------------------------- */


public UCoil_Cast( iCasterId, iTargetId ) {
#if ADVANCED_DEBUG
  log_function("public UCoil_Cast( iCasterId, iTargetId ) {");
#endif

    // Play client sound

    client_cmd( iCasterId, "speak warcraft3/bonus/DeathCoilMissileLaunch1.wav" );

    // Create Model

    new Float:fOrigin[3];
    entity_get_vector( iCasterId, EV_VEC_origin, fOrigin );

    new iSkullId = Create_TempEnt( "DEATH_COIL", "models/bskull_template1.mdl", fOrigin, MOVETYPE_NOCLIP, SOLID_TRIGGER, DEATHCOIL_DURATION );

    entity_set_edict( iSkullId, EV_ENT_owner, iCasterId );
    entity_set_edict( iSkullId, EV_ENT_enemy, iTargetId );

    // Skull Effects

    set_entity_rendering( iSkullId, kRenderFxGlowShell, DEATHCOIL_SHELL_RGB[GLOW_R], DEATHCOIL_SHELL_RGB[GLOW_G], DEATHCOIL_SHELL_RGB[GLOW_B], kRenderNormal, 48 );
    Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST, iSkullId, SPR_BEAMFOLLOW, 5, 8, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G], DEATHCOIL_TRAIL_RGB[GLOW_B], 160 );

    // Seek Target

    new parm_Seek[1];
    parm_Seek[0] = iSkullId;

    UCoil_Seek( parm_Seek );

    return PLUGIN_HANDLED;
}


public UCoil_Seek( parm_Seek[1] ) {
#if ADVANCED_DEBUG
  log_function("public UCoil_Seek( parm_Seek[1] ) {");
#endif

    new iSkullId = parm_Seek[0];
    new iTargetId = entity_get_edict( iSkullId, EV_ENT_enemy );

    if ( !is_user_alive( iTargetId ) )
        return PLUGIN_HANDLED;


    // Project Skull Towards Target

    set_velocity_to_ent( iSkullId, iTargetId, DEATHCOIL_VELOCITY );

    // Repeat Until Target is Hit

    new TaskId = TASK_TEMPENTITY + iSkullId;
    set_task( 0.1, "UCoil_Seek", TaskId, parm_Seek, 1 );

    return PLUGIN_HANDLED;
}


public UCoil_Damage( iCasterId, iTargetId ) {
#if ADVANCED_DEBUG
  log_function("public UCoil_Damage( iCasterId, iTargetId ) {");
#endif

    // Play Sound

    emit_sound( iTargetId, CHAN_STATIC, SOUND_DEATHCOIL, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Generate Effects

    UCoil_Effects( iTargetId );

    // Check for Amulet

    if ( IAmulet_Ready( iCasterId, iTargetId ) )
        return PLUGIN_HANDLED;

    // Unset Velocity

    new Float:fVelocity[3];
    entity_get_vector( iTargetId, EV_VEC_velocity, fVelocity );

    fVelocity[0] = 0.0;
    fVelocity[1] = 0.0;

    // Apply Damage

    WAR3_damage( iCasterId, iTargetId, CSW_DEATHCOIL, DEATHCOIL_DAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    return PLUGIN_HANDLED;
}


public UCoil_Heal( iCasterId, iTargetId ) {
#if ADVANCED_DEBUG
  log_function("public UCoil_Heal( iCasterId, iTargetId ) {");
#endif

    // Play Sound

    emit_sound( iTargetId, CHAN_STATIC, SOUND_DEATHCOIL, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Status Text

    WAR3_status_text( iCasterId, HEAL_CAST, 1.0);

    // Calculate New Health

    new iNewHealth = get_user_health( iTargetId ) + DEATHCOIL_HEAL;
    new iMaxHealth = WAR3_get_maxhealth( iTargetId );

    new iHealthGiven = DEATHCOIL_HEAL;

    if ( iNewHealth > iMaxHealth )
    {
        iHealthGiven -= ( iNewHealth - iMaxHealth );
        iNewHealth = iMaxHealth;
    }

    // Hud Message

    new szMessage[128], szPlayerName[32];
    get_user_name( iCasterId, szPlayerName, 31 );

    format( szMessage, 127, HEAL_TARGET, szPlayerName, iHealthGiven );

    WAR3_status_text( iTargetId, szMessage, 3.0 );

    // Apply Health

    set_user_health( iTargetId, iNewHealth );

    // Display Effects

    UCoil_Effects( iTargetId );

    // Give Support XP

    XP_Support_Heal( iCasterId, iHealthGiven );

    // Invisibility Cooldown

    Invis_Cooldown( iTargetId );

    return PLUGIN_HANDLED;
}


public UCoil_Effects( iTargetId ) {
#if ADVANCED_DEBUG
  log_function("public UCoil_Effects( iTargetId ) {");
#endif

    // Green Screefade

    Create_ScreenFade( iTargetId, (1<<10), (1<<10), FADE_OUT, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G], DEATHCOIL_TRAIL_RGB[GLOW_B], 160 );

    // Play Explosion

    new Origin[3];
    get_user_origin( iTargetId, Origin );

    Create_TE_EXPLOSION( SHOWTO_ALL_BROADCAST, Origin, SPR_DEATHCOIL, 20, 24, 4 );

/*
    // Sparks

    new StartOrigin[3];
    StartOrigin[0] = Origin[0];
    StartOrigin[1] = Origin[1];
    StartOrigin[2] = Origin[2] + 40;

    Create_TE_SPRITETRAIL( SHOWTO_ALL_BROADCAST, StartOrigin, Origin, SPR_DEATHCOIL, 24, 10, 1, 36, 10 );
*/
    // Glow

    Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, iTargetId, 100, DEATHCOIL_TRAIL_RGB[GLOW_R], DEATHCOIL_TRAIL_RGB[GLOW_G], DEATHCOIL_TRAIL_RGB[GLOW_B], 10, 0 );

    return PLUGIN_HANDLED;
}


public UCoil_Remove( iSkullId ) {
#if ADVANCED_DEBUG
  log_function("public UCoil_Remove( iSkullId ) {");
#endif

    new TaskId = TASK_TEMPENTITY + iSkullId;
    remove_task( TaskId, 0 );

    remove_entity( iSkullId );

    return PLUGIN_HANDLED;
}

public UCoil_Touch( iToucherId, iPlayerId )
{
#if ADVANCED_DEBUG
  log_function("public UCoil_Touch( iToucherId, iPlayerId )");
#endif

  // Sanity checks
    if ( iPlayerId < 1 || iPlayerId > 32 || iToucherId < 1 || !is_user_alive( iPlayerId ) )
        return PLUGIN_CONTINUE;

  new iCasterId = entity_get_edict( iToucherId, EV_ENT_owner );
  new iTargetId = entity_get_edict( iToucherId, EV_ENT_enemy );

  if ( iPlayerId == iTargetId )
  {
    if ( get_user_team( iCasterId ) == get_user_team( iTargetId ) )
      UCoil_Heal( iCasterId, iTargetId );

    else
    {
      UCoil_Damage( iCasterId, iTargetId );
    }

    UCoil_Remove( iToucherId );
  }

  return PLUGIN_CONTINUE;
}

/* - Impale ----------------------------------------------------- */


public UImpale_Cast( casterId, targetId ) {
#if ADVANCED_DEBUG
  log_function("public UImpale_Cast( casterId, targetId ) {");
#endif

    // Play Sound

    emit_sound( targetId, CHAN_STATIC, SOUND_IMPALE, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Raise Claw

    new Float:fOrigin[3], Origin[3];
    entity_get_vector( targetId, EV_VEC_origin, fOrigin );

    fOrigin[2] += 180.0;
    new iClawEnt = Create_TempEnt( "IMPALE_CLAW", "models/tentacle2.mdl", fOrigin, MOVETYPE_TOSS, SOLID_NOT, 1.5 );

    new Float:fVelocity[3] = {0.0,0.0,500.0};
    entity_set_vector( iClawEnt, EV_VEC_velocity, fVelocity );
    entity_set_float( iClawEnt, EV_FL_gravity, 1.5 );

    set_entity_rendering( iClawEnt, kRenderFxGlowShell, 128, 96, 160, kRenderTransTexture, 32 );

    // Check for Amulet

    if ( IAmulet_Ready( casterId, targetId ) )
        return PLUGIN_HANDLED;

    // Launch into Air

    new Velocity[3];
    get_entity_velocity( targetId, Velocity );

    Velocity[2] = IMPALE_VELOCITY;
    set_entity_velocity( targetId, Velocity );

    // Beam Trail

    Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST, targetId, SPR_BEAMFOLLOW, 5, 3, 160, 0, 48, 128 );

    // Blood Sprites

    for ( new i = 0; i < 5; i++ )
    {
        get_user_origin( targetId, Origin );

        for ( new j = 0; j < 5; j++ )
        {
            Origin[0] += random_num( -100,100 ) * j;
            Origin[1] += random_num( -100,100 ) * j;
            Origin[2] += random_num( 0,100 ) * j;

            Create_TE_BLOODSPRITE( SHOWTO_ALL_BROADCAST, Origin, SPR_BLOODSPRAY, SPR_BLOODDROP, 248, 15 );
        }
    }

    // Blood decals

    for ( new i = 0; i < 20; i++ )
    {
        static const BLOOD_SMALL[7] = {190,191,192,193,194,195,197};

        get_user_origin( targetId, Origin );

        Origin[0] += random_num( -100,100 );
        Origin[1] += random_num( -100,100 );
        Origin[2] -= 36;

        Create_TE_WORLDDECAL( SHOWTO_ALL, Origin, BLOOD_SMALL[random_num( 0,6 )] );
    }

    // Remove Armor

    if ( get_user_armor( targetId ) )
    {
        new iNewArmor = get_user_armor( targetId ) - IMPALE_ARMORDAMAGE;

        if ( iNewArmor < 0 )
            iNewArmor = 0;

        set_user_armor( targetId, iNewArmor );
    }

    // Remove Health

    WAR3_damage( casterId, targetId, CSW_IMPALE, IMPALE_DAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( is_user_alive( targetId ) )
    {
        // Screen Fade

        Create_ScreenFade( targetId, (1<<10), (1<<10), FADE_OUT, 160, 0, 48, 160 );

        new parm[3];
        parm[0] = targetId;
        parm[1] = Origin[2];

        g_PlayerImpaled[targetId] = casterId;

        new TaskId = TASK_IMPALE + targetId;
        set_task( 0.1, "UImpale_CheckHeight", TaskId, parm, 3 );
    }

    return PLUGIN_HANDLED;
}


public UImpale_CheckHeight( parm[3] ) {
#if ADVANCED_DEBUG
  log_function("public UImpale_CheckHeight( parm[3] ) {");
#endif

    new targetId = parm[0];
    new casterId = g_PlayerImpaled[targetId];

    new oldOrigin = parm[1];
    new oldVelocity = parm[2];

    new Velocity[3];
    get_entity_velocity( targetId, Velocity );

    // Check for Collision
    // ( Checks player height on descent, assumes collision if < IMPALE_MINDISTANCE )

    if ( oldVelocity >= 0 && Velocity[2] <= 0 )
    {
        new Origin[3];
        get_user_origin( targetId, Origin );

        new iDistance = Origin[2] - oldOrigin;

        if ( iDistance < IMPALE_MINDISTANCE )
        {
            // Collision Sound

            new szSoundfile[32];

            switch ( random_num( 1, 3 ) )
            {
                case 1: copy( szSoundfile, 31, "player/headshot1.wav" );
                case 2: copy( szSoundfile, 31, "player/headshot2.wav" );
                case 3: copy( szSoundfile, 31, "player/headshot3.wav" );
            }

            emit_sound( targetId, CHAN_STATIC, szSoundfile, 1.0, ATTN_NORM, 0, PITCH_NORM );

            // Blood Streams

            Origin[2] += 10;

            for ( new i = 0; i < 5; i++ )
            {
                new Vector[3];
                Vector[0] = random_num( 20,300 );
                Vector[1] = random_num( 20,300 );
                Vector[2] = 0;

                Create_TE_BLOODSTREAM( SHOWTO_ALL_BROADCAST, Origin, Vector, 70, random_num( 100,200 ) );
            }

            // Apply Damage

            WAR3_damage( casterId, targetId, CSW_IMPALE, IMPALE_COLLISIONDAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

            if ( is_user_alive( targetId ) )
            {
                // Screenfade (for victim)

                Create_ScreenFade( targetId, (1<<10), (1<<10), FADE_MODULATE_OUT, 0, 0, 0, 160 );
            }
        }
    }

    if ( !( get_entity_flags( targetId ) & FL_ONGROUND ) && g_PlayerImpaled[targetId] )
    {
        parm[2] = Velocity[2];

        new TaskId = TASK_IMPALE + targetId;
        set_task( 0.1, "UImpale_CheckHeight", TaskId, parm, 3 );
    }

    else
    {
        UImpale_Remove( targetId );
    }

    return PLUGIN_HANDLED;
}


public UImpale_Remove( id ) {
#if ADVANCED_DEBUG
  log_function("public UImpale_Remove( id ) {");
#endif

    new TaskId = TASK_IMPALE + id;
    remove_task( TaskId, 0 );

    g_PlayerImpaled[id] = 0;
    Remove_TE_BEAMFOLLOW( SEENBY_ALL, id );
}


/* - Sleep ------------------------------------------------------ */


public USleep_Cast( casterId, targetId ) {
#if ADVANCED_DEBUG
  log_function("public USleep_Cast( casterId, targetId ) {");
#endif

    USleep_Remove( targetId );

    // Play Sound

    emit_sound( targetId, CHAN_STATIC, SOUND_SLEEP, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Check for Amulet

    if ( IAmulet_Ready( casterId, targetId ) )
        return PLUGIN_HANDLED;

    // Switch to knife

    new iWeaponId, iClip, iAmmo;
    iWeaponId = get_user_weapon( targetId, iClip, iAmmo );

    if ( iWeaponId != CSW_KNIFE )
        engclient_cmd( targetId, "weapon_knife" );

    // Immobilize

    g_bPlayerSleeping[targetId] = true;
    g_bPlayerCantMove[targetId] = true;

    WAR3_set_speed( targetId );

    // Hud Message

    new szMessage[128], szPlayerName[32];
    get_user_name( casterId, szPlayerName, 31 );

    format( szMessage, 127, CAST_SLEEP, szPlayerName );

    WAR3_status_text2( targetId, szMessage, 3.0 );

    // Hit Zones ( remove )

    set_user_hitzones( targetId, 0, 0 );
    entity_set_float( targetId, EV_FL_takedamage, 0.0 );

    // Render

    set_user_rendering( targetId, kRenderFxNone, 0, 0, 0, kRenderTransAdd, 128 );

    // Screen Fade

    new Float:fDuration = random_float( SLEEP_DURATION_MIN, SLEEP_DURATION_MAX );
    new iHoldTime = floatround( fDuration );

    fDuration = float( iHoldTime );

    Create_ScreenFade( targetId, (1<<12), (iHoldTime<<12), FADE_MODULATE_IN, 0, 0, 0, 160 );

    // Progress Bar

    Create_BarTime( targetId, iHoldTime );

    // Effects

    new parm_Z1[3], parm_Z2[3];
    parm_Z1[0] = targetId;
    parm_Z2[0] = targetId;

    USleep_Effect( parm_Z1 );

    new TaskId = TASK_SLEEP + targetId;
    set_task( 0.4, "USleep_Effect", TaskId, parm_Z2, 3 );

    // Wake Task

    new parm_Remove[1];
    parm_Remove[0] = targetId;

    set_task( fDuration, "USleep_Wake", TaskId, parm_Remove, 1 );

    return PLUGIN_HANDLED;
}


public USleep_Effect( parm_Effects[3] ) {
#if ADVANCED_DEBUG
  log_function("public USleep_Effect( parm_Effects[3] ) {");
#endif

    new iTargetId = parm_Effects[0];
    new iSleepEnt = parm_Effects[1];
    new iCounter  = parm_Effects[2];

    if ( !iCounter )
    {
        // Create Z's

        new Float:fTargetOrigin[3];
        entity_get_vector( iTargetId, EV_VEC_origin, fTargetOrigin );

        fTargetOrigin[2] += 36.0;

        iSleepEnt = Create_TempEnt( "SLEEP_Z", "sprites/warcraft3/sleep.spr", fTargetOrigin, MOVETYPE_NOCLIP, SOLID_NOT, 1.5 );

        entity_set_float( iSleepEnt, EV_FL_renderamt, 128.0 );
        entity_set_int( iSleepEnt, EV_INT_rendermode, kRenderTransAdd );
        entity_set_int( iSleepEnt, EV_INT_renderfx, kRenderFxClampMinScale );

        entity_set_edict( iSleepEnt, EV_ENT_owner, iTargetId );

        // Project Upwards

        new Float:fVelocity[3];
        fVelocity[0] = random_float( -10.0, 10.0 );
        fVelocity[1] = random_float( -10.0, 10.0 );
        fVelocity[2] = 25.0;

        entity_set_vector( iSleepEnt, EV_VEC_velocity, fVelocity );
        entity_set_float( iSleepEnt, EV_FL_scale, random_float( 0.3, 0.4 ) );
    }

    if ( iCounter <= 10 )
    {
        if ( iCounter > 5 && is_valid_ent( iSleepEnt ) )
        {
            new Float:fScale = entity_get_float( iSleepEnt, EV_FL_scale );

            fScale -= 0.02;
            entity_set_float( iSleepEnt, EV_FL_scale, fScale );
        }

        // Re-Render Player ( HACKHACK )

        set_user_rendering( iTargetId, kRenderFxNone, 0, 0, 0, kRenderTransAdd, 128 );

        // Do it all again!

        iCounter++;

        parm_Effects[1] = iSleepEnt;
        parm_Effects[2] = iCounter;

        new TaskId = TASK_TEMPENTITY + iSleepEnt;
        set_task( 0.1, "USleep_Effect", TaskId, parm_Effects, 3 );
    }

    else if ( g_bPlayerSleeping[iTargetId] )
    {
        parm_Effects[1] = 0;
        parm_Effects[2] = 0;

        new TaskId = TASK_TEMPENTITY + iSleepEnt;
        set_task( 0.5, "USleep_Effect", TaskId, parm_Effects, 3 );
    }

    return PLUGIN_HANDLED;
}


public USleep_Wake( parm_Remove[1] ) {
#if ADVANCED_DEBUG
  log_function("public USleep_Wake( parm_Remove[1] ) {");
#endif

    new targetId = parm_Remove[0];

    // Screen Fade

    Create_ScreenFade( targetId, (1<<12), 0, FADE_MODULATE_OUT, 0, 0, 0, 160 );

    // Hudmessage

    new szMessage[128];
    format( szMessage, 127, FINISH_SLEEP );

    WAR3_status_text2( targetId, szMessage, 3.0 );

    // Enable Attack for an additional second ( to prevent sniper exploiting ).

    set_user_hitzones( targetId, 0, 255 );

    new TaskId = TASK_SLEEP + targetId;
    set_task( SLEEP_MOVETIME, "USleep_Finished", TaskId, parm_Remove, 1 );

    g_bPlayerSleeping[targetId] = false;

    // Set Player Speed

    Immobilize_Remove( targetId );
    WAR3_set_speed( targetId );

    return PLUGIN_HANDLED;
}


public USleep_Finished( parm_Remove[1] ) {
#if ADVANCED_DEBUG
  log_function("public USleep_Finished( parm_Remove[1] ) {");
#endif

    new targetId = parm_Remove[0];
    USleep_Remove( targetId );

    return PLUGIN_HANDLED;
}


public USleep_Remove( id ) {
#if ADVANCED_DEBUG
  log_function("public USleep_Remove( id ) {");
#endif

    new TaskId = TASK_SLEEP + id;
    remove_task( TaskId, 0 );

    g_bPlayerSleeping[id] = false;

    // Hit Zones ( restore )

    set_user_hitzones( id, 0, 255 );
    entity_set_float( id, EV_FL_takedamage, 2.0 );

    // Rendering

    set_user_rendering( id );

    // Invis Cooldown

    Invis_Cooldown( id );

    // Set Player Speed

    Immobilize_Remove( id );
    WAR3_set_speed( id );

    // Remove Z's

    Remove_TempEnts( "SLEEP_Z", id );

    Icon_DispellMe( id );

    return PLUGIN_HANDLED;
}

// End of UNDEAD.INL
