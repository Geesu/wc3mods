// Begin NIGHTELF.INL

/* - Skill Configuration ---------------------------------------- */


new Float:s_BlessingArmor[2]    = {110.0,150.0};                // (racial) Nature's Blessing (armor) {level 0,level LEVEL_RACIALCAP}
new Float:s_BlessingSpeed[2]    = {210.0,250.0};                // (racial) Nature's Blessing (min/max speed attainable) (used to calculate % speed bonus)
new Float:s_Evasion[3]          = {0.10,0.20,0.30};             // (skill1) Evasion (chance to evade)
new Float:s_MoonGlaive[3]       = {0.15,0.30,0.45};             // (skill2) Moon Glaive (chance to occur)
new Float:s_TrueshotAura[3]     = {0.15,0.30,0.45};             // (skill3) Trueshot Aura (bonus damage)


/* - Skill Constants Configuration ------------------------------ */


#define EVASION_MAXDAMAGE           250     // (integer) maximum damage (ish) avoidable with evasion
#define EVASION_SHOTGAP             0.2     // (  float) seconds between evades before a consecutive shot may be evaded

#define MOONGLAIVE_DAMAGE          0.75     // (  float) percent of base damage to apply to glaive recipient
#define MOONGLAIVE_RANGE             20     // (integer) maximum distance to check for glaive
#define MOONGLAIVE_SPEED         1800.0     // (  float) speed of glaive. slower = more chance to avoid
#define MOONGLAIVE_DURATION         1.0     // (  float) life of moonglaive before destoyed


/* - Ultimate Configuration ------------------------------------- */


#define ROOT_DURATION               5.0     // (  float) seconds player will be rooted
#define ROOT_MAXDAMAGE               25     // (integer) total damage taken over time for duration of root

#define REJUVENATION_MAXHEALTH       60     // (integer) health gained per wave
#define REJUVENATION_WAVES            5     // (integer) number of waves
#define REJUVENATION_DURATION       8.0     // (  float) duration of effect

#define SHADOWSTRIKE_DURATION        10
#define SHADOWSTRIKE_DAMAGE          25
#define SHADOWSTRIKE_ARMOR           15
#define SHADOWSTRIKE_DOT             20
#define SHADOWSTRIKE_SPEED        150.0


/* - Events ----------------------------------------------------- */


public Skills_Offensive_NE( attackerId, victimId, weaponId, iDamage, headshot ) {
#if ADVANCED_DEBUG
	log_function("public Skills_Offensive_NE( attackerId, victimId, weaponId, iDamage, headshot ) {");
#endif

    if ( g_PlayerInfo[attackerId][CURRENT_RACE] == RACE_NIGHTELF && get_user_team( attackerId ) != get_user_team( victimId ) )
    {
        // Moon Glaive

        if ( g_PlayerInfo[attackerId][CURRENT_SKILL2] && weaponId != CSW_KNIFE && weaponId != CSW_HEGRENADE )
            SMoonGlaive( attackerId, victimId, iDamage );

        // Trueshot Aura

        if ( g_PlayerInfo[attackerId][CURRENT_SKILL3] && weaponId != CSW_KNIFE && weaponId != CSW_HEGRENADE && get_user_health( victimId ) > 0 )
            STrueshotAura( attackerId, victimId, weaponId, iDamage, headshot );
    }

    return PLUGIN_HANDLED;
}


public Skills_Defensive_NE( attackerId, victimId, weaponId, iDamage, headshot ) {
#if ADVANCED_DEBUG
	log_function("public Skills_Defensive_NE( attackerId, victimId, weaponId, iDamage, headshot ) {");
#endif

    if ( g_PlayerInfo[victimId][CURRENT_RACE] == RACE_NIGHTELF )
    {
        // Evasion

        if ( g_PlayerInfo[victimId][CURRENT_SKILL1] && get_user_health( victimId ) > 0 )
        {
            if ( g_bEvadeNextShot[victimId] )
                SEvasion( attackerId, victimId, weaponId, iDamage, headshot );

            SEvasion_Check( victimId );
            SEvasion_Health( victimId );
        }
    }

    return PLUGIN_HANDLED;
}


public Ultimates_NE( casterId, targetId ) {
#if ADVANCED_DEBUG
	log_function("public Ultimates_NE( casterId, targetId ) {");
#endif

    // Rejuvenation

    if ( g_PlayerInfo[casterId][CURRENT_ULTIMATE] == ULTIMATE_REJUVENATION && get_user_team( targetId ) == get_user_team( casterId ) )
    {
        if ( g_PlayerRejuv[targetId] )
        {
            new szMessage[128];
            copy( szMessage, 127, REJUVINATE_ALREADY );

            WAR3_status_text( casterId, szMessage, 0.5 );

            Ultimate_Beep( casterId );
        }

        else
        {
            if ( get_user_health( targetId ) == WAR3_get_maxhealth( targetId ) )
            {
                new szMessage[64];

                if ( casterId == targetId )
                {
                    format( szMessage, 63, FULLHEALTH_SELF );
                }

                else
                {
                    format( szMessage, 63, FULLHEALTH_TARGET );
                }

                WAR3_status_text( casterId, szMessage, 0.5 );

                Ultimate_Beep( casterId );
            }

            else
            {
                URejuv_Cast( casterId, targetId );
                Ultimate_Cooldown( casterId, ULTIMATE_COOLDOWNDEFAULT );

                Invis_Cooldown( casterId );
            }
        }
    }

    // Entangling Roots

    else if ( g_PlayerInfo[casterId][CURRENT_ULTIMATE] == ULTIMATE_ROOTS && get_user_team( targetId ) != get_user_team( casterId ) && !g_PlayerRooted[targetId] )
    {
        URoot_Cast( casterId, targetId );
        Ultimate_Cooldown( casterId, ULTIMATE_COOLDOWNDEFAULT );

        Invis_Cooldown( casterId );
    }

    // Shadowstrike

    else if ( g_PlayerInfo[casterId][CURRENT_ULTIMATE] == ULTIMATE_SHADOWSTRIKE && get_user_team( targetId ) != get_user_team( casterId ) )
    {
        if ( g_PlayerStruck[targetId] )
            USstrike_Remove( targetId );

        USstrike_Cast( casterId, targetId );
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


// Nature's Blessing ( armor )

public SBlessing_Armor( iLevel ) {
#if ADVANCED_DEBUG
	log_function("public SBlessing_Armor( iLevel ) {");
#endif

    new Float:fLevel = float( iLevel );

    if ( fLevel > LEVEL_RACIALCAP )
        fLevel = LEVEL_RACIALCAP;

    new Float:fBonusArmor = s_BlessingArmor[0] + ( ( ( s_BlessingArmor[1] - s_BlessingArmor[0] ) / LEVEL_RACIALCAP ) * fLevel );

    if ( fBonusArmor > s_BlessingArmor[1] )
        fBonusArmor = s_BlessingArmor[1];

    return ( floatround( fBonusArmor ) );
}


// Nature's Blessing ( speed )

public Float:SBlessing_Speed_Get( iLevel ) {
#if ADVANCED_DEBUG
	log_function("public Float:SBlessing_Speed_Get( iLevel ) {");
#endif

    new Float:fLevel = float( iLevel );

    if ( fLevel > LEVEL_RACIALCAP )
        fLevel = LEVEL_RACIALCAP;

    new Float:fBlessingSpeed = ( ( ( s_BlessingSpeed[1] - s_BlessingSpeed[0] ) / LEVEL_RACIALCAP ) * fLevel );

    return ( fBlessingSpeed );
}


public SBlessing_Speed_Set( id, iWeaponId ) {
#if ADVANCED_DEBUG
	log_function("public SBlessing_Speed_Set( id, iWeaponId ) {");
#endif

    // Check if restricted

    if ( !WAR3_skill_enabled( id, RACE_NIGHTELF, SKILL_RACIAL ) )
        return PLUGIN_HANDLED;

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new Float:fBlessingSpeed;

    if ( g_bPlayerZoomed[id] )
        fBlessingSpeed = CS_WEAPON_SPEED_ZOOM[iWeaponId];

    else
    {
        fBlessingSpeed = CS_WEAPON_SPEED[iWeaponId];
    }

    // Do not slow down weapons currently faster than max speed

    if ( fBlessingSpeed > s_BlessingSpeed[1] )
    {
        set_user_maxspeed( id, fBlessingSpeed );
        return PLUGIN_HANDLED;
    }

    fBlessingSpeed += SBlessing_Speed_Get( iLevel );

    if ( fBlessingSpeed > s_BlessingSpeed[1] )
        fBlessingSpeed = s_BlessingSpeed[1];

    set_user_maxspeed( id, fBlessingSpeed );

    return PLUGIN_HANDLED;
}


/* - Skills ----------------------------------------------------- */


// Moon Glaive

public SMoonGlaive( attackerId, victimId, iDamage ) {
#if ADVANCED_DEBUG
	log_function("public SMoonGlaive( attackerId, victimId, iDamage ) {");
#endif

    new Float:fGlaiveChance = s_MoonGlaive[g_PlayerInfo[attackerId][CURRENT_SKILL2] - 1];
    new Float:fRandomNum = random_float( 0.0,1.0 );

    if ( fGlaiveChance < fRandomNum )
        return PLUGIN_HANDLED;


    new iTeamPlayers[32], szTeamName[16];
    new iTotalPlayers;

    get_user_team( victimId, szTeamName, 15 );
    get_players( iTeamPlayers, iTotalPlayers, "ae", szTeamName );

    // Make sure there are teammates to hit with glaive

    if ( iTotalPlayers > 1 || ( iTotalPlayers == 1 && !is_user_alive( victimId ) ) )
    {
        new victimOrigin[3], teamOrigin[3];
        get_user_origin( victimId, victimOrigin );

        new iPlayerNum;
        new iGlaiveTargets[33], iTotalTargets;

        while ( iPlayerNum < iTotalPlayers )
        {
            new teamId = iTeamPlayers[iPlayerNum];
            get_user_origin( teamId, teamOrigin );

            if ( victimId != teamId && get_distance( victimOrigin, teamOrigin ) / 40 <= MOONGLAIVE_RANGE && ent_in_view( victimId, teamId ) && !g_bPlayerSleeping[teamId] && !g_bPlayerInvis[teamId] && !g_bPlayerWalk[teamId] )
            {
                // Build potential target array ( so we're not hitting the same person each time )

                iGlaiveTargets[iTotalTargets] = teamId;
                iTotalTargets++;
            }

            iPlayerNum++;
        }

        // Hit random target in-range ( if any )

        if ( iTotalTargets > 0 )
        {
            new targetId = iGlaiveTargets[random_num( 0,iTotalTargets - 1 )];

            // Create Glaive

            new Float:fVictimOrigin[3];
            entity_get_vector( victimId, EV_VEC_origin, fVictimOrigin );

            new iGlaiveEnt = Create_TempEnt( "MOON_GLAIVE", "models/hornet.mdl", fVictimOrigin, MOVETYPE_FLY, SOLID_TRIGGER, MOONGLAIVE_DURATION );

            // Set Glaive Properties

            entity_set_edict( iGlaiveEnt, EV_ENT_owner, attackerId );
            entity_set_edict( iGlaiveEnt, EV_ENT_enemy, victimId );

            // Project Glaive towards target ( handled in vexd_pfntouch )

            new Float:fTargetOrigin[3];
            entity_get_vector( targetId, EV_VEC_origin, fTargetOrigin );

            new Float:fTime = get_ent_traveltime( fVictimOrigin, fTargetOrigin, MOONGLAIVE_SPEED );

            new Float:fNewTargetOrigin[3];
            predict_ent_origin( targetId, fTime, fNewTargetOrigin );

            set_velocity_to_origin( iGlaiveEnt, fNewTargetOrigin, MOONGLAIVE_SPEED );

            // Create Glaive Trail

            Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST, iGlaiveEnt, SPR_BEAMFOLLOW, 15, 3, random_num( 127,255 ), 0, random_num( 127,255 ), 255 );

            // Set Damage Ammount

            new Float:fGlaiveDamage = float( iDamage ) * MOONGLAIVE_DAMAGE;
            entity_set_float( iGlaiveEnt, EV_FL_dmg, fGlaiveDamage );
        }
    }

    return PLUGIN_HANDLED;
}

// Moon Glaive ( damages ) ( called from vexd_pfntouch )

public SMoonGlaive_Damage( attackerId, targetId, iDamage ) {
#if ADVANCED_DEBUG
	log_function("public SMoonGlaive_Damage( attackerId, targetId, iDamage ) {");
#endif

    // Play Sound

    // .. we'll add one sometime...

    // Hud Message

    new szMessage[128];
    format( szMessage, 127, DAMAGE_MOONGLAIVE, iDamage );

    WAR3_status_text( targetId, szMessage, 3.0 );

    // Add to player stats array

    if ( get_cvar_num( "mp_war3stats" ) )
    {
        playerSkill2Info[attackerId][0] += iDamage;
    }

    // Blood Sprites

    new Origin[3];
    get_user_origin( targetId, Origin );

    for ( new i = 0; i < 3; i++ )
    {
        Origin[0] += random_num( -50,50 );
        Origin[1] += random_num( -50,50 );
        Origin[2] += random_num( -10,10 );

        Create_TE_BLOODSPRITE( SHOWTO_ALL_BROADCAST, Origin, SPR_BLOODSPRAY, SPR_BLOODDROP, 248, 15 );
    }

    // Blood decals

    for ( new i = 0; i < 3; i++ )
    {
        static const blood_small[7] = {190,191,192,193,194,195,197};

        get_user_origin( targetId, Origin );

        Origin[0] += random_num( -100,100 );
        Origin[1] += random_num( -100,100 );
        Origin[2] -= 36;

        Create_TE_WORLDDECAL( SHOWTO_ALL_BROADCAST, Origin, blood_small[random_num(0,6)] );
    }

    // Apply Damage

    WAR3_damage( attackerId, targetId, CSW_MOONGLAIVE, iDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( is_user_alive( targetId ) )
    {
        // Screen Fade

        new iFadeAlpha = iDamage * 2;

        if ( iFadeAlpha > GLOW_MAX )
            iFadeAlpha = GLOW_MAX;

        Create_ScreenFade( targetId, (1<<10), (1<<10), FADE_OUT, 255, 0, 255, iFadeAlpha );

        // Set Velocity

        new Velocity[3];
        get_entity_velocity( targetId, Velocity );

        Velocity[0] = 0;
        Velocity[1] = 0;

        set_entity_velocity( targetId, Velocity );
    }

    return PLUGIN_HANDLED;
}


public SMoonGlaive_Destroy( iGlaiveId ) {
#if ADVANCED_DEBUG
	log_function("public SMoonGlaive_Destroy( iGlaiveId ) {");
#endif

    new TaskId = TASK_TEMPENTITY + iGlaiveId;
    remove_task( TaskId, 0 );

    remove_entity( iGlaiveId );

    return PLUGIN_HANDLED;
}

public SMoonGlaive_Touch( iToucherId, iPlayerId )
{
#if ADVANCED_DEBUG
	log_function("public SMoonGlaive_Touch( iToucherId, iPlayerId )");
#endif

	// Sanity checks
    if ( iPlayerId < 1 || iPlayerId > 32 || iToucherId < 1 || !is_user_alive( iPlayerId ) )
        return PLUGIN_CONTINUE;

	new iAttackerId    = entity_get_edict( iToucherId, EV_ENT_owner );
	new iFirstVictimId = entity_get_edict( iToucherId, EV_ENT_enemy );

	// Don't Hit First Victim / Teammates

	if ( iFirstVictimId != iPlayerId && get_user_team( iAttackerId ) != get_user_team( iToucherId ) )
	{
		new Float:fDamage = entity_get_float( iToucherId, EV_FL_dmg );
		new iDamage = floatround( fDamage );

		SMoonGlaive_Damage( iAttackerId, iPlayerId, iDamage );

		SMoonGlaive_Destroy( iToucherId );
	}

	return PLUGIN_CONTINUE;
}

// Trueshot Aura

public STrueshotAura( iAttackerId, iVictimId, iWeaponId, iDamage, iHeadshot ) {
#if ADVANCED_DEBUG
	log_function("public STrueshotAura( iAttackerId, iVictimId, iWeaponId, iDamage, iHeadshot ) {");
#endif

    // Check if restricted

    if ( !WAR3_skill_enabled( iAttackerId, RACE_NIGHTELF, SKILL_3 ) )
        return PLUGIN_HANDLED;

    new Float:fBonusDamage = float( iDamage ) * s_TrueshotAura[g_PlayerInfo[iAttackerId][CURRENT_SKILL3] - 1];
    new iBonusDamage = floatround( fBonusDamage );

    // Add to player stats array

    if ( get_cvar_num( "mp_war3stats" ) )
    {
        playerSkill3Info[iAttackerId][0] += iBonusDamage;
    }

    // Apply Damage

    if ( iBonusDamage > 0 )
        WAR3_damage( iAttackerId, iVictimId, iWeaponId, iBonusDamage, iHeadshot, DAMAGE_CHECKARMOR );

    return PLUGIN_HANDLED;
}


// Evasion

public SEvasion( attackerId, victimId, weaponId, iDamage, headshot ) {
#if ADVANCED_DEBUG
	log_function("public SEvasion( attackerId, victimId, weaponId, iDamage, headshot ) {");
#endif

    // Player dies to grenade / fall with evade

    if ( get_user_health( victimId ) <= 1024 && get_user_team( attackerId ) == get_user_team( victimId ) )
        WAR3_death( attackerId, victimId, weaponId, headshot );

    // Attacked by Wind Walker

    else if ( get_user_health( victimId ) <= 1024 && g_bPlayerWalk[attackerId] )
        WAR3_death( attackerId, victimId, weaponId, headshot );

    else if ( get_user_health( victimId ) > 0 && get_user_team( victimId ) != get_user_team( attackerId ) )
    {
        g_bEvadeNextShot[victimId] = false;

        new iHealth = get_user_health( victimId ) + iDamage;
        new iNewHealth = iHealth - 1024;

        set_user_health( victimId, iNewHealth );

        g_fEvasionTime[victimId] = get_gametime();
        g_iEvasionDamage[victimId] += iDamage;

        // Inform Player

        new szMessage[128];
        format( szMessage, 127, DAMAGE_EVASION, iDamage );

        WAR3_status_text( victimId, szMessage, 3.0 );

        // Set task to re-check for evasion

        new parm_Evasion[1];
        parm_Evasion[0] = victimId;

        new TaskId = TASK_EVASION + victimId;
        set_task( EVASION_SHOTGAP, "SEvasion_ReCheck", TaskId, parm_Evasion, 1 );

        // Add to player stats array

        if ( get_cvar_num( "mp_war3stats" ) )
        {
            playerSkill1Info[victimId][0] += iDamage;
        }

        new iFadeAlpha = iDamage;

        if ( iFadeAlpha > GLOW_MAX )
            iFadeAlpha = GLOW_MAX;

        // Glow

        new iRGB[3];
        iRGB[GLOW_B] = iFadeAlpha;

        Glow_Set( victimId, glow_duration( iFadeAlpha ), iRGB, 36 );

        // Screen Fade

        Create_ScreenFade( victimId, (1<<10), (1<<10), FADE_OUT, 0, 0, 255, iFadeAlpha );
    }

    return PLUGIN_HANDLED;
}


public SEvasion_Check( id ) {
#if ADVANCED_DEBUG
	log_function("public SEvasion_Check( id ) {");
#endif

    if ( !WAR3_skill_enabled( id, RACE_NIGHTELF, SKILL_1 ) || g_iEvasionDamage[id] >= EVASION_MAXDAMAGE )
    {
        g_fEvasionTime[id] = get_gametime() + 1000.0;
        return PLUGIN_HANDLED;
    }

    // Don't prep for evasion if EVASION_SHOTGAP not met

    if ( get_gametime() - g_fEvasionTime[id] < EVASION_SHOTGAP )
        return PLUGIN_HANDLED;

    new Float:fEvadeChance = s_Evasion[g_PlayerInfo[id][CURRENT_SKILL1] - 1];
    new Float:fRandomNum = random_float( 0.0, 1.0 );

    if ( fEvadeChance >= fRandomNum )
        g_bEvadeNextShot[id] = true;

    return PLUGIN_HANDLED;
}


public SEvasion_ReCheck( parm_Evasion[1] ) {
#if ADVANCED_DEBUG
	log_function("public SEvasion_ReCheck( parm_Evasion[1] ) {");
#endif

    new id = parm_Evasion[0];

    g_fEvasionTime[id] = 0.0;

    SEvasion_Check( id );
    SEvasion_Health( id );

    return PLUGIN_HANDLED;
}


public SEvasion_Health( id ) {
#if ADVANCED_DEBUG
	log_function("public SEvasion_Health( id ) {");
#endif

    new iMaxHealth = 100;

    // Any health bonus checks go here

    if ( g_bEvadeNextShot[id] && get_user_health( id ) <= iMaxHealth )
    {
        new iNewHealth = get_user_health( id ) + 1024;
        set_user_health( id, iNewHealth );
    }

    else if ( !g_bEvadeNextShot[id] && get_user_health( id ) > 1024 )
    {
        new iNewHealth = get_user_health( id ) - 1024;
        set_user_health( id, iNewHealth );
    }

    return PLUGIN_HANDLED;
}



/* - Entangling Roots ------------------------------------------- */


public URoot_Cast( casterId, targetId ) {
#if ADVANCED_DEBUG
	log_function("public URoot_Cast( casterId, targetId ) {");
#endif

    g_PlayerRooted[targetId] = casterId;

    // Display Message ( to target )

    new szMessage[128], szPlayerName[32];
    get_user_name( casterId, szPlayerName, 31 );

    format( szMessage, 127, CAST_ROOTS, szPlayerName );

    WAR3_status_text( targetId, szMessage, 3.0 );

    // Trail Effect

    Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST, targetId, SPR_BEAMFOLLOW, 10, 3, 10, 108, 23, 255 );

    new parm[1];
    parm[0] = targetId;

    URoot_Immobilize( parm );

    return PLUGIN_HANDLED;
}

public URoot_Immobilize( parmRoot[1] ) {
#if ADVANCED_DEBUG
	log_function("public URoot_Immobilize( parmRoot[1] ) {");
#endif

    new targetId = parmRoot[0];

    if ( get_entity_flags( targetId ) & FL_ONGROUND )
    {
        // Effect(s)

        URoot_Effects( targetId );

        // Remove Trail

        Remove_TE_BEAMFOLLOW( SEENBY_ALL, targetId );

        // Check for Amulet

        if ( IAmulet_Ready( g_PlayerRooted[targetId], targetId ) )
        {
            g_PlayerRooted[targetId] = 0;
            return PLUGIN_HANDLED;
        }

        set_entity_velocity( targetId, {0,0,0} );

        g_bPlayerCantMove[targetId] = true;

        WAR3_set_speed( targetId );

        Icon_DispellMe( targetId );

        new Float:fDuration = ROOT_DURATION;
        new iCounter = floatround( fDuration );

        new parm_Dot[2];
        parm_Dot[0] = targetId;
        parm_Dot[1] = iCounter;

        new TaskId = TASK_ROOT + targetId;
        set_task( 1.0, "URoot_Dot", TaskId, parm_Dot, 2 );
    }

    else
    {
        new TaskId = TASK_ROOT + targetId;
        set_task( 0.1, "URoot_Immobilize", TaskId, parmRoot, 1 );
    }

    return PLUGIN_HANDLED;
}


public URoot_Dot( parm[2] ) {
#if ADVANCED_DEBUG
	log_function("public URoot_Dot( parm[2] ) {");
#endif

    new targetId = parm[0];
    new casterId = g_PlayerRooted[targetId];
    new iCounter = parm[1];

    new Float:fDuration = ROOT_DURATION;
    new iDuration = floatround( fDuration );

    new iDamage = ( ROOT_MAXDAMAGE / iDuration );

    // Blood Sprites

    new Origin[3];
    get_user_origin( targetId, Origin );

    Origin[0] += random_num( -50,50 );
    Origin[1] += random_num( -50,50 );
    Origin[2] += random_num( -10,10 );

    Create_TE_BLOODSPRITE( SHOWTO_ALL_BROADCAST, Origin, SPR_BLOODSPRAY, SPR_BLOODDROP, 248, 15 );

    // Blood decals

    for ( new i = 0; i < 3; i++ )
    {
        static const BLOOD_SMALL[7] = {190,191,192,193,194,195,197};

        get_user_origin( targetId, Origin );

        Origin[0] += random_num( -100,100 );
        Origin[1] += random_num( -100,100 );
        Origin[2] -= 36;

        Create_TE_WORLDDECAL( SHOWTO_ALL_BROADCAST, Origin, BLOOD_SMALL[random_num(0,6)] );
    }

    // Apply Damage

    WAR3_damage( casterId, targetId, CSW_ROOT, iDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( is_user_alive( targetId ) )
    {
        // Poison Damage

        Create_Damage( targetId, 0, 0, CS_DMG_POISON );

        // Screen Fade

        if ( !g_bPlayerSleeping[targetId] )
        {
            Create_ScreenFade( targetId, (1<<10), (1<<10), FADE_OUT, 10, 108, 23, 20 );
        }

        iCounter--;

        if ( iCounter )
        {
            parm[1] = iCounter;

            new TaskId = TASK_ROOT + targetId;
            set_task( 1.0, "URoot_Dot", TaskId, parm, 2 );
        }

        else
        {
            // Play Client-Only Soundfile

            client_cmd( targetId, "speak warcraft3/bonus/EntanglingRootsDecay1.wav" );

            URoot_Remove( targetId );
        }
    }

    return PLUGIN_HANDLED;
}


public URoot_CreatePlant( iTargetId, Origin[3] ) {
#if ADVANCED_DEBUG
	log_function("public URoot_CreatePlant( iTargetId, Origin[3] ) {");
#endif

    // Select Plant Model

    new szModelName[64];

    switch ( random_num( 1,3 ) )
    {
        case 1:     copy( szModelName, 63, "models/uplant1.mdl" );
        case 2:     copy( szModelName, 63, "models/uplant2.mdl" );
        case 3:     copy( szModelName, 63, "models/uplant3.mdl" );
    }

    new Float:fOrigin[3];
    fOrigin[0] = float( Origin[0] );
    fOrigin[1] = float( Origin[1] );
    fOrigin[2] = float( Origin[2] );

    new iPlantEnt = Create_TempEnt( "ROOT_PLANT", szModelName, fOrigin, MOVETYPE_TOSS, SOLID_NOT, 0.0 );

    entity_set_edict( iPlantEnt, EV_ENT_owner, iTargetId );

    // Animate

    entity_set_int( iPlantEnt, EV_INT_sequence, 0 );
    entity_set_float( iPlantEnt, EV_FL_frame, random_float( 1.0, 50.0 ) );
    entity_set_float( iPlantEnt, EV_FL_framerate, random_float( 1.0, 2.0 ) );

    return PLUGIN_HANDLED;
}


public URoot_Effects( targetId ) {
#if ADVANCED_DEBUG
	log_function("public URoot_Effects( targetId ) {");
#endif

    new x1, y1, x2, y2;
    new iRadius = 20, iCounter = 0;

    new Float:fDuration = ROOT_DURATION;
    new iDuration = floatround( fDuration );

    new targetOrigin[3];
    get_user_origin( targetId, targetOrigin );

    // Play Sound

    emit_sound( targetId, CHAN_STATIC, SOUND_ROOTS, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Draw Coil

    while ( iCounter <= 7 )
    {
        switch ( iCounter )
        {
            case 0,8:   x1 = -iRadius;
            case 1,7:   x1 = -iRadius * 100 / 141;
            case 2,6:   x1 = 0;
            case 3,5:   x1 = iRadius * 100 / 141;
            case 4:     x1 = iRadius;
        }

        if ( iCounter <= 4 )
            y1 = sqrt( iRadius * iRadius - x1 * x1 );

        else
            y1 = sqrt( iRadius * iRadius - x1 * x1 ) * -1;


        iCounter++;

        switch ( iCounter )
        {
            case 0,8:   x2 = -iRadius;
            case 1,7:   x2 = -iRadius * 100 / 141;
            case 2,6:   x2 = 0;
            case 3,5:   x2 = iRadius * 100 / 141;
            case 4:     x2 = iRadius;
        }

        if ( iCounter <= 4 )
            y2 = sqrt( iRadius * iRadius - x2 * x2 );

        else
            y2 = sqrt( iRadius * iRadius - x2 * x2 ) * -1;


        new iHeight = 16 + ( 2 * iCounter );

        while ( iHeight > -40 )
        {
            new Origin1[3];
            Origin1[0] = targetOrigin[0] + x1;
            Origin1[1] = targetOrigin[1] + y1;
            Origin1[2] = targetOrigin[2] + iHeight;

            new Origin2[3];
            Origin2[0] = targetOrigin[0] + x2;
            Origin2[1] = targetOrigin[1] + y2;
            Origin2[2] = targetOrigin[2] + iHeight + 2;

            Create_TE_BEAMPOINTS( SHOWTO_ALL_BROADCAST, Origin1, Origin2, SPR_ROOT, 0, 0, iDuration * 10, 10, 5, 10, 108, 23, 255, 0 );

            iHeight -= 16;
        }
    }

    // Plants

    for ( new iPlantNum = 1; iPlantNum <= 8; iPlantNum++ )
    {
        new xOffset, yOffset;

        switch ( iPlantNum )
        {
            case 1:     xOffset = random_num( 10,25 );
            case 2:     xOffset = random_num( -25,-10 );
            case 3:     yOffset = random_num( 10,25 );
            case 4:     yOffset = random_num( -25,-10 );

            case 5:
            {
                xOffset = random_num( 10,25 );
                yOffset = random_num( 10,25 );
            }

            case 6:
            {
                xOffset = random_num( -25,-10 );
                yOffset = random_num( -25,-10 );
            }

            case 7:
            {
                xOffset = random_num( 10,25 );
                yOffset = random_num( -25,-10 );
            }

            case 8:
            {
                xOffset = random_num( -25,-10 );
                yOffset = random_num( 10,25 );
            }
        }

        new plantOrigin[3];
        plantOrigin[0] = targetOrigin[0] + xOffset;
        plantOrigin[1] = targetOrigin[1] + yOffset;
        plantOrigin[2] = targetOrigin[2] - 34;

        if ( get_user_button( targetId ) & IN_DUCK )
            plantOrigin[2] += 18;

        URoot_CreatePlant( targetId, plantOrigin );
    }

    return PLUGIN_HANDLED;
}


public URoot_Remove( id ) {
#if ADVANCED_DEBUG
	log_function("public URoot_Remove( id ) {");
#endif

    new TaskId = TASK_ROOT + id;
    remove_task( TaskId, 0 );

    g_PlayerRooted[id] = 0;
    Remove_TE_BEAMFOLLOW( SEENBY_ALL,id );

    Immobilize_Remove( id );
    WAR3_set_speed( id );

    Icon_DispellMe( id );

    return PLUGIN_HANDLED;
}


/* - Shadow Strike ---------------------------------------------- */


public USstrike_Cast( casterId, targetId ) {
#if ADVANCED_DEBUG
	log_function("public USstrike_Cast( casterId, targetId ) {");
#endif

    // Play Sound

    emit_sound( targetId, CHAN_STATIC, SOUND_SHADOWSTRIKE, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Implosion

    new targetOrigin[3];
    get_user_origin( targetId, targetOrigin );

    Create_TE_IMPLOSION( SHOWTO_ALL_BROADCAST, targetOrigin, 100, 120, 4 );

    // "Fizzy" Explosion

    Create_TE_EXPLODEMODEL( SHOWTO_ALL_BROADCAST, targetOrigin, 250, MDL_HORNET, 40, 50 );

    // Check for Amulet

    if ( IAmulet_Ready( casterId, targetId ) )
        return PLUGIN_HANDLED;

    // Hud Message

    new szPlayerName[32];
    get_user_name( casterId, szPlayerName, 31 );

    new szMessage[128];
    format( szMessage, 127, CAST_SHADOWSTRIKE, szPlayerName );

    WAR3_status_text( targetId, szMessage, 3.0 );

    // Adjust Armor

    if ( get_user_armor( targetId ) )
    {
        new iNewArmor = get_user_armor( targetId ) - SHADOWSTRIKE_ARMOR;

        if ( iNewArmor < 0 )
            iNewArmor = 0;

        set_user_armor( targetId, iNewArmor );
    }

    // Apply Damage

    WAR3_damage( casterId, targetId, CSW_SHADOWSTRIKE, SHADOWSTRIKE_DAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( is_user_alive( targetId ) )
    {
        // Set Globals

        g_PlayerStruck[targetId] = casterId;
        g_bPlayerSlowed[targetId] = true;

        // Set Player Speed

        new Velocity[3];
        get_entity_velocity( targetId, Velocity );

        Velocity[0] = 0;
        Velocity[1] = 0;

        set_entity_velocity( targetId, Velocity );
        WAR3_set_speed( targetId );

        // Set Damage Over Time

        new parm_Dot[3];
        parm_Dot[0] = casterId;
        parm_Dot[1] = targetId;
        parm_Dot[2] = SHADOWSTRIKE_DURATION;

        new TaskId = TASK_SHADOWSTRIKE + casterId;
        set_task( 1.0, "USstrike_Dot", TaskId, parm_Dot, 3 );

        Icon_DispellMe( targetId );
    }

    return PLUGIN_HANDLED;
}


public USstrike_Dot( parm_Dot[3] ) {
#if ADVANCED_DEBUG
	log_function("public USstrike_Dot( parm_Dot[3] ) {");
#endif

    new casterId = parm_Dot[0];
    new targetId = parm_Dot[1];
    new iCounter = parm_Dot[2];

    new iDamage = SHADOWSTRIKE_DOT / SHADOWSTRIKE_DURATION;

    // Apply Damage

    WAR3_damage( casterId, targetId, CSW_SHADOWSTRIKE, iDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

    if ( is_user_alive( targetId ) )
    {
        // Poison Damage

        Create_Damage( targetId, 0, 0, CS_DMG_POISON );

        // Screen Fade

        if ( !g_bPlayerSleeping[targetId] )
        {
            Create_ScreenFade( targetId, (1<<10), (1<<10), FADE_OUT, 255, 255, 0, 20 );
        }

        // Check DOT Counter

        iCounter--;

        if ( iCounter )
        {
            parm_Dot[2] = iCounter;

            new TaskId = TASK_SHADOWSTRIKE + casterId;
            set_task( 1.0, "USstrike_Dot", TaskId, parm_Dot, 3 );
        }

        else
        {
            USstrike_Remove( targetId );
        }
    }

    return PLUGIN_HANDLED;
}


public USstrike_Remove( targetId ) {
#if ADVANCED_DEBUG
	log_function("public USstrike_Remove( targetId ) {");
#endif

    new casterId = g_PlayerStruck[targetId];

    new TaskId = TASK_SHADOWSTRIKE + casterId;
    remove_task( TaskId, 0 );

    g_PlayerStruck[targetId] = 0;

    Slow_Remove( targetId );
    WAR3_set_speed( targetId );

    Icon_DispellMe( targetId );

    return PLUGIN_HANDLED;
}


/* - Rejuvenation ----------------------------------------------- */


public URejuv_Cast( casterId, targetId ) {
#if ADVANCED_DEBUG
	log_function("public URejuv_Cast( casterId, targetId ) {");
#endif

    g_PlayerRejuv[targetId] = casterId;

    new szMessage[64], szPlayerName[32];
    get_user_name( casterId, szPlayerName, 31 );

    // Status Text

    if ( casterId == targetId )
    {
        format( szMessage, 127, REJUVINATE_SELF );
        WAR3_status_text( casterId, szMessage, 3.0 );
    }

    else
    {
        WAR3_status_text( casterId, HEAL_CAST, 1.0 );

        format( szMessage, 127, REJUVINATE_TARGET, szPlayerName );
        WAR3_status_text( targetId, szMessage, 3.0 );
    }

    // Play Sound

    emit_sound( targetId, CHAN_STATIC, SOUND_REJUVENATION, 1.0, ATTN_NORM, 0, PITCH_NORM );

    new parmHot[3];
    parmHot[0] = casterId;
    parmHot[1] = targetId;
    parmHot[2] = REJUVENATION_WAVES;

    URejuv_Hot( parmHot );

    return PLUGIN_HANDLED;
}


public URejuv_Effects( parm_Effects[2] ) {             // Shared Effect(s)
#if ADVANCED_DEBUG
	log_function("public URejuv_Effects( parm_Effects[2] ) {             // Shared Effect(s)");
#endif

    new targetId = parm_Effects[0];
    new iCounter = parm_Effects[1];

    // Flares!

    for ( new i = 0; i < 4; i++ )
    {
        new Float:fTargetOrigin[3];
        entity_get_vector( targetId, EV_VEC_origin, fTargetOrigin );

        // Randomize

        fTargetOrigin[0] = random_float( fTargetOrigin[0] - 20.0, fTargetOrigin[0] + 20.0 );
        fTargetOrigin[1] = random_float( fTargetOrigin[1] - 20.0, fTargetOrigin[1] + 20.0 );
        fTargetOrigin[2] = random_float( fTargetOrigin[2] - 20.0, fTargetOrigin[2] + 20.0 );

        // Sprite

        new szSpriteName[64];

        switch ( random_num( 1,2 ) )
        {
            case 1:     copy( szSpriteName, 63, "sprites/blueflare2.spr" );
            case 2:     copy( szSpriteName, 63, "sprites/muz7.spr" );
        }

        new iFlareEnt = Create_TempEnt( "REJUV_FLARE", szSpriteName, fTargetOrigin, MOVETYPE_NOCLIP, SOLID_NOT, 0.4 );

        // Project Upwards

        new Float:fVelocity[3];
        fVelocity[2] = random_float( 125.0, 175.0 );

        entity_set_vector( iFlareEnt, EV_VEC_velocity, fVelocity );

        // Create Trail

        Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST, iFlareEnt, SPR_BEAMFOLLOW, 5, random_num( 1,3 ), random_num( 127,255 ), 0, random_num( 127,255 ), 128 );

        // Create Lighting ( on player )

        Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, targetId, random_num( 25,100 ), random_num( 127,255 ), 0, random_num( 127,255 ), 5, 5 );

        // Render

        entity_set_float( iFlareEnt, EV_FL_renderamt, 128.0 );
        entity_set_int( iFlareEnt, EV_INT_rendermode, kRenderTransAdd );
        entity_set_int( iFlareEnt, EV_INT_renderfx, kRenderFxNone );

        entity_set_float( iFlareEnt, EV_FL_scale, random_float( 0.2, 0.6 ) );
    }

    iCounter++;

    if ( iCounter <= 6 )
    {
        parm_Effects[1] = iCounter;

        new TaskId = 2000;
        set_task( 0.1, "URejuv_Effects", TaskId, parm_Effects, 2 );
    }

    return PLUGIN_HANDLED;
}


public URejuv_Hot( parmHot[3] ) {              // Heal-Over Time
#if ADVANCED_DEBUG
	log_function("public URejuv_Hot( parmHot[3] ) {              // Heal-Over Time");
#endif

    new casterId = parmHot[0];
    new targetId = parmHot[1];
    new iCounter = parmHot[2];

    // Adjust Health

    new iBonusHealth = REJUVENATION_MAXHEALTH / REJUVENATION_WAVES;
    new iNewHealth = get_user_health( targetId ) + iBonusHealth;
    new iMaxHealth = WAR3_get_maxhealth( targetId );

    if ( iNewHealth > iMaxHealth )
    {
        iBonusHealth -= ( iNewHealth - iMaxHealth );
        iNewHealth = iMaxHealth;
    }

    if ( casterId != targetId )
        g_iRejuvHealth[casterId] += iBonusHealth;

    set_user_health( targetId, iNewHealth );

    // Add to player stats array

    if ( get_cvar_num( "mp_war3stats" ) )
    {
        playerSkill2Info[casterId][1] += iBonusHealth;
    }

    // Generate Effect(s)

    new parm_Effects[2];
    parm_Effects[0] = targetId;

    URejuv_Effects( parm_Effects );


    iCounter--;

    if ( iCounter )
    {
        parmHot[2] = iCounter;

        new iWaves = REJUVENATION_WAVES;
        new Float:fRate = REJUVENATION_DURATION / ( float( iWaves )  - 1.0 );

        new TaskId = TASK_REJUV + targetId;
        set_task( fRate, "URejuv_Hot", TaskId, parmHot, 3 );
    }

    else
    {
        URejuv_Remove( targetId );
    }

    return PLUGIN_HANDLED;
}


public URejuv_Remove( iTargetId ) {                    // Remove
#if ADVANCED_DEBUG
	log_function("public URejuv_Remove( iTargetId ) {                    // Remove");
#endif

    new iCasterId = g_PlayerRejuv[iTargetId];

    // Give Support Xp

    XP_Support_Heal( iCasterId, g_iRejuvHealth[iCasterId] );

    g_iRejuvHealth[iCasterId] = 0;

    // Unset

    g_PlayerRejuv[iTargetId] = 0;

    new TaskId = TASK_REJUV + iTargetId;
    remove_task( TaskId, 0 );

    return PLUGIN_HANDLED;
}

// End of NIGHTELF.INL
