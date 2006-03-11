// Begin ORC.INL

/* - Skill Configuration ---------------------------------------- */


new Float:s_Regeneration[2]     = {2.8,0.8};                    // (racial) Regeneration (regeneration rate)
new Float:s_BerserkDmg[3]		= {0.3,0.6,1.0};				// (skill1) Berserk (bonus % damage based on health)
new Float:s_BerserkSpeed[3]		= {280.0,300.0,320.0};			// (skill1) Max possible speed with Berserk at health 0
new Float:s_PulverizeRange[3]	= {4.0,5.0,6.0};				// (skill2) Pulverize (distance from detonation)
new Float:s_PulverizeBonus[3]	= {1.0,2.0,3.0};				// (skill2) Pulverize (bonus damage)
new Float:s_Reincarnate[3]      = {0.3,0.6,0.9};                // (skill3) Reincarnation (percent chance)
new Float:s_Pillage[3]          = {0.10,0.20,1.00};             // (skill3) Pillage (percent chance)
new s_PillageNades[3]           = {4,9,25};                     // (skill3) Pillage (nade model index)

/* - Skill Constants Configuration ------------------------------ */


#define BERSERK_HEALTH				 75		// (integer) lowest health to start giving bonus
#define BERSERK_MAXSPEED		  320.0		// (integer) max speed for berserk

#define BLOODLUST_MODIFIER          1.0     // (  float) % of max damage that will calculate min damage
#define BLOODLUST_KNIFEBONUS          3     // (integer) blood lust knife bonus multiplier

#define PULVERIZE_ARMOR            0.25     // (  float) % armor removed on pulverize based on damage

#define PILLAGE_ARMOR              0.30
#define PILLAGE_AMMO               0.30
#define PILLAGE_MONEY                 4

#define REGENERATION_AMMOUNT          1     // (integer) health gained each cycle


/* - Ultimate Configuration ------------------------------------- */


#define HEALINGWAVE_HEAL             50     // (integer) initial heal
#define HEALINGWAVE_RANGE            20     // (integer) max range of jump targets
#define HEALINGWAVE_MULTIPLIER        3     // (integer) jump health multiplier (1/X)
#define HEALINGWAVE_JUMPS             6     // (integer) maximum healing wave jumps

#define CHAINLIGHTNING_DAMAGE        65     // (integer) initial damage
#define CHAINLIGHTNING_RANGE         15     // (integer) max range of jump targets
#define CHAINLIGHTNING_MULTIPLIER     3     // (integer) jump damage multiplier (1/X)
#define CHAINLIGHTNING_JUMPS          6     // (integer) maximum chain lightning jumps

#define WINDWALK_INVISIBILITY        28     // (integer) visibility at peak of windwalk
#define WINDWALK_SPEED            350.0     // (  float) max speed when windwalking
#define WINDWALK_DURATION            10     // (integer) seconds windwalk is active
#define WINDWALK_PREWALKTIME          1     // (integer) seconds it takes to become fully invisible
#define WINDWALK_DAMAGE              75     // (integer) damage on successful windwalk knife attack
#define WINDWALK_FOV                 50     // (integer) ammount to pull back FOV


/* - Events ----------------------------------------------------- */


public Skills_Offensive_OR( attackerId, victimId, weaponId, iDamage, headshot, Float:fDamageOrigin[3] ) {
#if ADVANCED_DEBUG
	log_function("public Skills_Offensive_OR( attackerId, victimId, weaponId, iDamage, headshot ) {");
#endif

    if ( g_PlayerInfo[attackerId][CURRENT_RACE] == RACE_ORC && get_user_team( attackerId ) != get_user_team( victimId ) && get_user_health( victimId ) > 0 )
    {
       // Bloodlust

        //if ( g_PlayerInfo[attackerId][CURRENT_SKILL1] )
        //    SBloodlust( attackerId, victimId, weaponId, headshot );

		// Berserk

        if ( g_PlayerInfo[attackerId][CURRENT_SKILL1] && weaponId != CSW_HEGRENADE )
		{
            SBerserkDmg( attackerId, victimId, weaponId, iDamage, headshot );
		}

        // Pulverize
		
        if ( g_PlayerInfo[attackerId][CURRENT_SKILL2] && weaponId == CSW_HEGRENADE )
		{
            SPulverize( attackerId, victimId, fDamageOrigin, iDamage );
		}

		// Pillage

		if ( g_PlayerInfo[attackerId][CURRENT_SKILL3] )
		{
			SPillage( attackerId, victimId, iDamage );
		}
    }

    return PLUGIN_HANDLED;
}


public Skills_Defensive_OR( victimId ) {
#if ADVANCED_DEBUG
	log_function("public Skills_Defensive_OR( victimId ) {");
#endif

    if ( g_PlayerInfo[victimId][CURRENT_RACE] == RACE_ORC )
    {
		SBerserkSpeed( victimId );

        // Regeneration ( taken care of in on_Health() )
    }

    return PLUGIN_HANDLED;
}


public Ultimates_OR( Caster, Target ) {
#if ADVANCED_DEBUG
	log_function("public Ultimates_OR( Caster, Target ) {");
#endif

    // Chain Lightning

    if ( g_PlayerInfo[Caster][CURRENT_ULTIMATE] == ULTIMATE_LIGHTNING && get_user_team( Target ) != get_user_team( Caster ) )
    {
        UClightning_Cast( Caster, Target );
        Ultimate_Cooldown( Caster, ULTIMATE_COOLDOWNDEFAULT );

        Invis_Cooldown( Caster );
    }

    // Healing Wave

    else if ( g_PlayerInfo[Caster][CURRENT_ULTIMATE] == ULTIMATE_HEALINGWAVE && get_user_team( Target ) == get_user_team( Caster ) )
    {
        if ( get_user_health( Target ) == WAR3_get_maxhealth( Target ) )
        {
            new szMessage[64];

            if ( Caster == Target )
            {
                format( szMessage, 63, FULLHEALTH_SELF );
            }

            else
            {
                format( szMessage, 63, FULLHEALTH_TARGET );
            }

            WAR3_status_text( Caster, szMessage, 0.5 );
            Ultimate_Beep( Caster );
        }

        else
        {
            UHealWave_Cast( Caster, Target );
            Ultimate_Cooldown( Caster, ULTIMATE_COOLDOWNDEFAULT );

            Invis_Cooldown( Caster );
        }
    }

    // Wind Walk

    else if ( g_PlayerInfo[Caster][CURRENT_ULTIMATE] == ULTIMATE_WINDWALK )
    {
        UWindwalk_Cast( Caster );
        Ultimate_Cooldown( Caster, ULTIMATE_COOLDOWNDEFAULT );
    }

    else
    {
        Ultimate_Beep( Caster );
        return PLUGIN_HANDLED;
    }

    return PLUGIN_HANDLED;
}


/* - Racial Ability --------------------------------------------- */


// Regeneration

public Float:SRegen_Get( iLevel ) {
#if ADVANCED_DEBUG
	log_function("public Float:SRegen_Get( iLevel ) {");
#endif

    new Float:fLevel = float( iLevel );

    if ( fLevel > LEVEL_RACIALCAP )
        fLevel = LEVEL_RACIALCAP;

    new Float:fRegenRate = s_Regeneration[0] - ( ( ( s_Regeneration[0] - s_Regeneration[1] ) / LEVEL_RACIALCAP ) * fLevel );

    if ( fRegenRate < s_Regeneration[1] )
        fRegenRate = s_Regeneration[1];

    return ( fRegenRate );
}


public SRegen_Set( id ) {
#if ADVANCED_DEBUG
	log_function("public SRegen_Set( id ) {");
#endif

    if ( g_bPlayerRegen[id] || ( WAR3_get_maxhealth( id ) == get_user_health( id ) && ( !get_user_armor( id ) || get_user_armor( id ) == WAR3_get_maxarmor( id ) ) ) )
        return PLUGIN_HANDLED;

    g_bPlayerRegen[id] = true;

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
    new Float:fRegenRate = SRegen_Get( iLevel );

    new parm_Regen[1];
    parm_Regen[0] = id;

    new iTaskId = TASK_REGEN + id;
    set_task( fRegenRate, "SRegen_Heal", iTaskId, parm_Regen, 1 );

    // Regeneration Icon

    return PLUGIN_HANDLED;
}


public SRegen_Heal( parm_Regen[1] ) {
#if ADVANCED_DEBUG
	log_function("public SRegen_Heal( parm_Regen[1] ) {");
#endif

    new id = parm_Regen[0];

    if ( get_user_health( id ) >= WAR3_get_maxhealth( id ) && get_user_armor( id ) >= WAR3_get_maxarmor( id ) )
        SRegen_Remove( id );

    else
    {
        // Health

        if ( get_user_health( id ) && get_user_health( id ) < WAR3_get_maxhealth( id ) )
        {
            new iHealth = get_user_health( id );
            new iNewHealth = iHealth + REGENERATION_AMMOUNT;

            set_user_health( id, iNewHealth );
        }

        // Armor

        if ( get_user_armor( id ) && get_user_armor( id ) < WAR3_get_maxarmor( id ) )
        {
            new iArmor = get_user_armor( id );
            new iNewArmor = iArmor + REGENERATION_AMMOUNT;

            set_user_armor( id, iNewArmor );
        }

        new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
        new Float:fRegenRate = SRegen_Get( iLevel );

        new iTaskId = TASK_REGEN + id;
        set_task( fRegenRate, "SRegen_Heal", iTaskId, parm_Regen, 1 );
    }

    return PLUGIN_HANDLED;
}

public SRegen_Remove( id ) {
#if ADVANCED_DEBUG
	log_function("public SRegen_Remove( id ) {");
#endif

    g_bPlayerRegen[id] = false;

    new iTaskId = TASK_REGEN + id;
    remove_task( iTaskId, 0 );

    // Regeneration Icon

    return PLUGIN_HANDLED;
}


/* - Skills ----------------------------------------------------- */

// Berserk

public SBerserkSpeed( id ){
	#if ADVANCED_DEBUG
		log_function("SBerserkSpeed( id ){");
	#endif

    if ( WAR3_skill_enabled( id, RACE_ORC, SKILL_1 ) && is_user_alive( id ) )
	{
		new iClip, iAmmo, iWeapon;
		iWeapon =  get_user_weapon( id, iClip, iAmmo );

		if ( iWeapon == CSW_KNIFE )
		{
			new iHealth = get_user_health( id );
			new Float:fNewSpeed = 0.0;

			if( g_PlayerInfo[id][CURRENT_ITEM] == ITEM_BOOTS )
				fNewSpeed = VALUE_BOOTS + ( float(100 - iHealth)/100.0 * ( s_BerserkSpeed[g_PlayerInfo[id][CURRENT_SKILL1] - 1] - VALUE_BOOTS ) );
			else
				fNewSpeed = SPEED_KNIFE + ( float(100 - iHealth)/100.0 * ( s_BerserkSpeed[g_PlayerInfo[id][CURRENT_SKILL1] - 1] - SPEED_KNIFE ) );

			set_user_maxspeed( id, fNewSpeed );
		}
	}

    return PLUGIN_HANDLED;
}

public SBerserkDmg( attackerId, victimId, weaponId, damage, headshot ) {
	#if ADVANCED_DEBUG
		log_function("SBerserkDmg( attackerId, victimId, weaponId, headshot, damage )");
	#endif

	// Check if the attacker already had berserk enabled

    if ( WAR3_skill_enabled( attackerId, RACE_ORC, SKILL_1 ) )
	{
		new iHealth = get_user_health( attackerId );

		if ( iHealth <= BERSERK_HEALTH )
		{
			// (100 - Current Health) * ( Damage * Multiplier )

			new Float:fHealthMultiplier = float( ( 100 - iHealth ) ) / 100.0;

			new iBonusDamage = floatround( fHealthMultiplier * (s_BerserkDmg[g_PlayerInfo[attackerId][CURRENT_SKILL1] - 1] * damage) );

		    // Apply Damage

		    WAR3_damage( attackerId, victimId, weaponId, iBonusDamage, headshot, DAMAGE_CHECKARMOR );

		    // Add to player stats array

			if ( get_cvar_num( "mp_war3stats" ) )
			{
				playerSkill1Info[attackerId][0] += iBonusDamage;
			}

			// Screen Fade

			new iFadeAlpha = iBonusDamage * 3;
			Create_ScreenFade( victimId, (1<<10), (1<<10), FADE_OUT, 255, 0, 0, iFadeAlpha );
		}
	}

    return PLUGIN_HANDLED;
}

// Pulverize

public SPulverize( attackerId, victimId, Float:grenadeOrigin[3], damage ) {
#if ADVANCED_DEBUG
	log_function("SPulverize");
#endif

    // Check if restricted
	
    if ( !WAR3_skill_enabled( attackerId, RACE_ORC, SKILL_2 ) )
        return PLUGIN_HANDLED;

    new Teammates[32], szTeamName[16];
    new iTotalPlayers;
	new bool:bHitPlayers = false;

    get_user_team( victimId, szTeamName, 15 );
    get_players( Teammates, iTotalPlayers, "ae", szTeamName );

	// Convert origin to int
	
	new iGrenadeOrigin[3];
	for ( new i = 0; i < 3; i++ )
		iGrenadeOrigin[i] = floatround( grenadeOrigin[i] );

	for ( new iPlayerNum = 0; iPlayerNum < iTotalPlayers; iPlayerNum++ )
	{
		new teamId = Teammates[iPlayerNum];

		if ( teamId != victimId && !g_bPlayerSleeping[teamId] )
		{
			new teamOrigin[3];

			get_user_origin( teamId, teamOrigin );

			new Float:fMetricDistance = distance( get_distance( iGrenadeOrigin, teamOrigin ) );

			// Determine if a nearby teammate is close enough to damage

			if ( fMetricDistance <= s_PulverizeRange[g_PlayerInfo[attackerId][CURRENT_SKILL2] - 1] )
			{
				bHitPlayers = true;

				// Damage Calculation

				new iPulverizeDamage = floatround( s_PulverizeBonus[g_PlayerInfo[attackerId][CURRENT_SKILL2] - 1] * float( damage ) );

                // Armor Calculation

                if ( get_user_armor( teamId ) )
                {
                    new Float:fArmorDamage = float( iPulverizeDamage ) * PULVERIZE_ARMOR;
                    new iNewArmor = get_user_armor( teamId ) - floatround( fArmorDamage );

                    if ( iNewArmor < 0 )
                        iNewArmor = 0;

                    set_user_armor( teamId, iNewArmor );
                }

                new szMessage[128];
                format( szMessage, 127, DAMAGE_PULVERIZE, iPulverizeDamage );

                WAR3_status_text( teamId, szMessage, 3.0 );

                // Add to player stats array

                if ( get_cvar_num( "mp_war3stats" ) )
                {
                    playerSkill2Info[attackerId][0] += iPulverizeDamage;
                }

                // Apply Damage

                WAR3_damage( attackerId, teamId, CSW_PULVERIZE, iPulverizeDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

                // Screen Fade

                if ( is_user_alive( teamId ) )
                {
                    new iFadeAlpha = iPulverizeDamage * 2;

                    if ( iFadeAlpha > GLOW_MAX )
                        iFadeAlpha = GLOW_MAX;

                    Create_ScreenFade( teamId, (1<<10), (1<<10), FADE_OUT, 147, 115, 78, iFadeAlpha );
                }

                // Blood Sprites

                new Origin[3];
                get_user_origin( teamId, Origin );

                for ( new i = 0; i < 5; i++ )
                {
                    Origin[0] += random_num( -100,100 );
                    Origin[1] += random_num( -100,100 );
                    Origin[2] += random_num( 0,100 );

                    Create_TE_BLOODSPRITE( SHOWTO_ALL_BROADCAST, Origin, SPR_BLOODSPRAY, SPR_BLOODDROP, 248, 15 );
                }

                // Blood decals

                for ( new i = 0; i < 5; i++ )
                {
                    static const blood_small[7] = {190,191,192,193,194,195,197};

                    get_user_origin( teamId, Origin );

                    Origin[0] += random_num( -100,100 );
                    Origin[1] += random_num( -100,100 );
                    Origin[2] -= 36;

                    Create_TE_WORLDDECAL( SHOWTO_ALL_BROADCAST, Origin, blood_small[random_num( 0,6 )] );
                }
			}
		}
	}

	// Show Pulverize Rings if we hit a player

	if ( bHitPlayers )
	{
		new iRadius = floatround( s_PulverizeRange[g_PlayerInfo[attackerId][CURRENT_SKILL2] - 1] );

		new iRingRed    = ( 85 * g_PlayerInfo[attackerId][CURRENT_SKILL2] );
		new iRingGreen  = ( 85 * g_PlayerInfo[attackerId][CURRENT_SKILL2] );
		new iRingBlue   = ( 64 * g_PlayerInfo[attackerId][CURRENT_SKILL2] );

		new iBlueMod;

		switch ( iRingBlue )
		{
			case 64:    iBlueMod = 64;
			case 128:   iBlueMod = 48;
			case 192:   iBlueMod = 32;
		}

		// Play Sound

		//emit_sound( victimId, CHAN_STATIC, SOUND_PULVERIZE, 1.0, ATTN_NORM, 0, PITCH_NORM );

		new iRingSize = iRadius * 2 * 40;
		new OuterRadius[3], InnerRadius[3];

		OuterRadius[2] = iRingSize + 40;
		InnerRadius[2] = iRingSize / 2;

		// Outer Ring

		Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, iGrenadeOrigin, OuterRadius, SPR_SHOCKWAVE, 0, 0, 3, 6, 0, iRingRed, iRingGreen, iRingBlue - iBlueMod, 255, 0 );

		// Inner Ring

		Create_TE_BEAMCYLINDER( SHOWTO_ALL_BROADCAST, iGrenadeOrigin, InnerRadius, SPR_SHOCKWAVE, 0, 0, 3, 3, 0, iRingRed, iRingGreen, iRingBlue, 255, 0 );
	}

	return PLUGIN_HANDLED;
}

public SPulverize_Trail( id, gIndex ) {
#if ADVANCED_DEBUG
	log_function("public SPulverize_Trail( id, gIndex ) {");
#endif

    // Check if restricted

    if ( !WAR3_skill_enabled( id, RACE_ORC, SKILL_2 ) )
        return PLUGIN_HANDLED;

    new iWidth = g_PlayerInfo[id][CURRENT_SKILL2];
    new iBrightness = ( 120 / 3 ) * g_PlayerInfo[id][CURRENT_SKILL2];

    Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST, gIndex, SPR_ROOT, 10, iWidth, 255, 0, 0, iBrightness );

    return PLUGIN_HANDLED;
}

public SPillage( attackerId, victimId, iDamage ) {
#if ADVANCED_DEBUG
	log_function( "public SPillage( attackerId, victimId, iDamage ) {");
#endif

	if ( !WAR3_skill_enabled( attackerId, RACE_ORC, SKILL_3 ) )
		return PLUGIN_HANDLED;

	new Float:fPillageChance = s_Pillage[g_PlayerInfo[attackerId][CURRENT_SKILL3] - 1];
    new Float:fRandomNum = random_float( 0.0, 1.0 );

	if ( fRandomNum > fPillageChance )
        return PLUGIN_HANDLED;
	
	new iType = 1//random_num( 1, 4 );
	switch( iType )
	{
		case 1:
		{
			new iVictimArmor = get_user_armor( victimId );
			new iAttackerArmor = get_user_armor( attackerId );

			if ( iVictimArmor <= 0 )
				return PLUGIN_HANDLED;
			
			new iStolenArmor = floatround( PILLAGE_ARMOR * iDamage );
			
			if ( iVictimArmor - iStolenArmor < 0 )
				iStolenArmor = iVictimArmor;

			iVictimArmor -= iStolenArmor;
			iAttackerArmor += iStolenArmor;

			if ( iAttackerArmor > WAR3_get_maxarmor( attackerId ) )
				iAttackerArmor = WAR3_get_maxarmor( attackerId );

			set_user_armor( victimId, iVictimArmor );
			set_user_armor( attackerId, iAttackerArmor );
		}
				
		case 2:
		{
			new iVictimClip, iVictimAmmo;
			new iVictimWeapon = get_user_weapon( victimId, iVictimClip, iVictimAmmo );
			
			new iAttackerClip, iAttackerAmmo;
			new iAttackerWeapon = get_user_weapon( attackerId, iAttackerClip, iAttackerAmmo );
			
			if ( iVictimAmmo <= 0 )
				return PLUGIN_HANDLED;
			
			new iStolenAmmo = floatround( PILLAGE_AMMO * iDamage );

			if ( iVictimAmmo - iStolenAmmo < 0 )
				iStolenAmmo = iVictimAmmo;

			cs_update_ammo( victimId, -iStolenAmmo, iVictimWeapon );
			cs_update_ammo( attackerId, iStolenAmmo, iAttackerWeapon );
		}

		case 3:
		{
			new i;
			new bool:bHasNade;
	
			for ( i = 0; i < CS_MAX_NADES; i++ )
			{
				if ( cs_find_grenade( victimId, CS_MODEL_NAME[s_PillageNades[i]] ) )
				{
					client_print( attackerId, print_chat, "nade found" );
					bHasNade = true;
					break;
				}
			}	
		
			if ( bHasNade )
			{
				new iVictimClip, iVictimAmmo;
				new iVictimWeapon = get_user_weapon( victimId, iVictimClip, iVictimAmmo );

				new iAttackerClip, iAttackerAmmo;
				get_user_weapon( attackerId, iAttackerClip, iAttackerAmmo );

				if ( iVictimWeapon == s_PillageNades[i] )
					cs_switchweapon( victimId, CS_WEAPON_GROUP_KNIFE );
				
				cs_update_ammo( victimId, s_PillageNades[i], -1 );
    			cs_update_ammo( attackerId, s_PillageNades[i], 1 );
    			
    			bHasNade = false;
			}
		}	

		case 4:
		{
			new iVictimMoney = cs_get_user_money( victimId );

			if ( iVictimMoney <= 0 )
				return PLUGIN_HANDLED;
			
			new iStolenMoney = ( PILLAGE_MONEY * iDamage );
			
			if ( iVictimMoney - iStolenMoney < 0 )
				iStolenMoney = iVictimMoney;

			cs_update_money( victimId, iStolenMoney, 1 );
			cs_update_money( attackerId, iStolenMoney, 1 );
		}
	}

    return PLUGIN_HANDLED;
}


/* - Chain Lightning -------------------------------------------- */


public UClightning_Cast( Caster, Target ) {
#if ADVANCED_DEBUG
	log_function("public UClightning_Cast( Caster, Target ) {");
#endif

    // Play Sound

    emit_sound( Caster, CHAN_STATIC, SOUND_CHAINLIGHTNING, 1.0, ATTN_NORM, 0, PITCH_NORM );
    emit_sound( Target, CHAN_STATIC, SOUND_CHAINLIGHTNING, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Lightning Bolt

    Create_TE_BEAMENTS( SHOWTO_ALL_BROADCAST, Caster, Target, SPR_CHAINLIGHTNING, 0, 15, 10, 80, 10, 255, 255, 255, 255, 0 );

    // Light Effects

    Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, Caster, 100, 255, 255, 255, 10, 0 );
    Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, Target, 100, 255, 255, 255, 10, 0 );

    // Check for Amulet

    if ( !IAmulet_Ready( Caster, Target ) )
    {
        // Hud Message

        new szMessage[128], szPlayerName[32];
        get_user_name( Caster, szPlayerName, 31 );

        format( szMessage, 127, CAST_CHAINLIGHTNING, szPlayerName, CHAINLIGHTNING_DAMAGE );

        WAR3_status_text( Target, szMessage, 3.0 );

        // Shock Damage

        Create_Damage( Target, 0, 0, CS_DMG_SHOCK );

        // Apply Damage

        WAR3_damage( Caster, Target, CSW_LIGHTNING, CHAINLIGHTNING_DAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );
    }

    g_ChainJumps[Caster][0] = Target;

    new parmJump[2];
    parmJump[0] = Caster;
    parmJump[1] = 1;

    new TaskId = TASK_ULTIMATE + Caster;
    set_task( 0.2, "UClightning_Jump", TaskId, parmJump, 2 );

    return PLUGIN_HANDLED;
}


public UClightning_Jump( parmJump[2] ) {
#if ADVANCED_DEBUG
	log_function("public UClightning_Jump( parmJump[2] ) {");
#endif

    new Caster       = parmJump[0];
    new iCurrentJump = parmJump[1];
    new LastTarget   = g_ChainJumps[Caster][iCurrentJump - 1];

    new LastOrigin[3], NextOrigin[3];
    get_user_origin( LastTarget, LastOrigin );

    // Get all alive teammates

    new szTeamName[16];
    get_user_team( LastTarget, szTeamName, 15 );

    new Teammates[32], iTotalPlayers;
    get_players( Teammates, iTotalPlayers, "ae", szTeamName );

    // Cast on next teammate in range

    new iPlayerNum, bool:bTargetFound;

    while ( iPlayerNum < iTotalPlayers && !bTargetFound )
    {
        new NextTarget = Teammates[iPlayerNum];

        get_user_origin( NextTarget, NextOrigin );

        if ( get_distance( LastOrigin, NextOrigin ) / 40 <= CHAINLIGHTNING_RANGE && ent_in_view( LastTarget, NextTarget ) )
        {
            // Check if target already hit

            new bool:bTargetHit;

            for ( new i = 0; i <= CHAINLIGHTNING_JUMPS; i++ )
            {
                if ( g_ChainJumps[Caster][i] == NextTarget )
                    bTargetHit = true;
            }

            // Don't Jump to Players Already Hit / Sleeping / Invis / Avatar'd

            new bool:bPlayerImmune;

            if ( g_bPlayerSleeping[NextTarget] || g_bPlayerInvis[NextTarget] || g_iPlayerAvatar[NextTarget] )
                bPlayerImmune = true;

            if ( !bTargetHit && !bPlayerImmune )
            {
                bTargetFound = true;

                // Set Damage of Lightning
                // we don't count final jump because if this is final jump, damage has not been set yet

                new iBoltDamage = CHAINLIGHTNING_DAMAGE;

                for ( new iDamageCheck = 0; iDamageCheck <= CHAINLIGHTNING_JUMPS; iDamageCheck++ )
                {
                    if ( g_ChainJumps[Caster][iDamageCheck] )
                        iBoltDamage -= ( iBoltDamage / CHAINLIGHTNING_MULTIPLIER );
                }

                g_ChainJumps[Caster][iCurrentJump] = NextTarget;


                // Play Sound

                emit_sound( LastTarget, CHAN_STATIC, SOUND_CHAINLIGHTNING, 1.0, ATTN_NORM, 0, PITCH_NORM );
                emit_sound( NextTarget, CHAN_STATIC, SOUND_CHAINLIGHTNING, 1.0, ATTN_NORM, 0, PITCH_NORM );

                // Lightning Bolt

                Create_TE_BEAMENTS( SHOWTO_ALL_BROADCAST, LastTarget, NextTarget, SPR_CHAINLIGHTNING, 0, 15, 10, 80, 10, 255, 255, 255, 255, 0 );

                // Light Effect (target)

                Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, LastTarget, 100, 255, 255, 255, 10, 0 );
                Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, NextTarget, 100, 255, 255, 255, 10, 0 );

                // Check for Amulet

                if ( !IAmulet_Ready( Caster, NextTarget ) )
                {
                    // Hud Message

                    new szMessage[128], szPlayerName[32];
                    get_user_name( Caster, szPlayerName, 31 );

                    format( szMessage, 127, CAST_CHAINLIGHTNING_JUMP, szPlayerName, iBoltDamage );

                    WAR3_status_text( NextTarget, szMessage, 3.0 );

                    // Shock Damage

                    Create_Damage( NextTarget, 0, 0, CS_DMG_SHOCK );

                    // Apply Damage

                    WAR3_damage( Caster, NextTarget, CSW_LIGHTNING, iBoltDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );
                }
            }
        }

        iPlayerNum++;
    }

    if ( iCurrentJump != CHAINLIGHTNING_JUMPS && bTargetFound )
    {
        parmJump[1] = iCurrentJump + 1;

        new TaskId = TASK_ULTIMATE + Caster;
        set_task( 0.2, "UClightning_Jump", TaskId, parmJump, 2 );
    }

    else
    {
        for ( new i = 0; i <= CHAINLIGHTNING_JUMPS; i++ )
        {
            g_ChainJumps[Caster][i] = 0;
        }
    }

    return PLUGIN_HANDLED;
}


/* - Healing Wave ----------------------------------------------- */


public UHealWave_Cast( Caster, Target ) {
#if ADVANCED_DEBUG
	log_function("public UHealWave_Cast( Caster, Target ) {");
#endif

    // Play Sound

    emit_sound( Caster, CHAN_STATIC, SOUND_HEALINGWAVE, 1.0, ATTN_NORM, 0, PITCH_NORM );
    emit_sound( Target, CHAN_STATIC, SOUND_HEALINGWAVE, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Status Text

    WAR3_status_text( Caster, HEAL_CAST, 1.0 );

    // Healing Wave

    if ( Caster != Target )
    {
        Create_TE_BEAMENTS( SHOWTO_ALL_BROADCAST, Caster, Target, SPR_SMOOTHBEAM, 0, 10, 15, 100, 4, 255, 255, 128, 255, 0 );
        Create_TE_BEAMENTS( SHOWTO_ALL_BROADCAST, Caster, Target, SPR_CHAINLIGHTNING, 0, 15, 15, 80, 4, 255, 255, 255, 255, 0 );
    }

    // Heal Effect

    Shared_Heal_Effect( Target );

    // Light Effects

    Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, Caster, 100, 255, 255, 128, 10, 0 );
    Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, Target, 100, 255, 255, 128, 10, 0 );


    new iNewHealth = get_user_health( Target ) + HEALINGWAVE_HEAL;
    new iMaxHealth = WAR3_get_maxhealth( Target );

    new iHealthGiven = HEALINGWAVE_HEAL;

    if ( iNewHealth > iMaxHealth )
    {
        iHealthGiven -= ( iNewHealth - iMaxHealth );
        iNewHealth = iMaxHealth;
    }

    if ( Target != Caster )
        g_iHealingWaveHealth[Caster] += iHealthGiven;

    new szMessage[128], szPlayerName[32];
    get_user_name( Caster, szPlayerName, 31 );

    if ( Caster == Target )
        format( szMessage, 127, HEAL_SELF, iHealthGiven );

    else
    {
        format( szMessage, 127, HEAL_TARGET, szPlayerName, iHealthGiven );
    }

    WAR3_status_text( Target, szMessage, 3.0 );

    set_user_health( Target, iNewHealth );

    // Invisibility Cooldown

    Invis_Cooldown( Target );

    g_ChainJumps[Caster][0] = Target;

    new parmJump[2];
    parmJump[0] = Caster;
    parmJump[1] = 1;

    new TaskId = TASK_ULTIMATE + Caster;
    set_task( 0.2, "UHealWave_Jump", TaskId, parmJump, 2 );

    return PLUGIN_HANDLED;
}


public UHealWave_Jump( parmJump[2] ) {
#if ADVANCED_DEBUG
	log_function("public UHealWave_Jump( parmJump[2] ) {");
#endif

    new Caster       = parmJump[0];
    new iCurrentJump = parmJump[1];
    new LastTarget   = g_ChainJumps[Caster][iCurrentJump - 1];

    new LastOrigin[3], NextOrigin[3];
    get_user_origin( LastTarget, LastOrigin );

    // Get all alive teammates

    new szTeamName[16];
    get_user_team( LastTarget, szTeamName, 15 );

    new Teammates[32], iTotalPlayers
    get_players( Teammates, iTotalPlayers, "ae", szTeamName );

    new iPlayerNum, bool:bTargetFound;

    while ( iPlayerNum < iTotalPlayers && !bTargetFound )
    {
        new NextTarget = Teammates[iPlayerNum];

        get_user_origin( NextTarget, NextOrigin );

        // Don't attempt to hit players out of range / not in view

        if ( get_distance( LastOrigin, NextOrigin ) / 40 <= HEALINGWAVE_RANGE && ent_in_view( LastTarget, NextTarget ) )
        {
            // Check if target already hit

            new bool:bTargetHit;

            for ( new i = 0; i <= HEALINGWAVE_JUMPS; i++ )
            {
                if ( g_ChainJumps[Caster][i] == NextTarget )
                    bTargetHit = true;
            }

            // Don't heal players already hit / at full health / Avatar'd

            if ( !bTargetHit && get_user_health( NextTarget ) != WAR3_get_maxhealth( NextTarget ) && !g_iPlayerAvatar[NextTarget] )
            {
                bTargetFound = true;

                // Set Heal Ammount
                // we don't count final jump because if this is final jump, heal ammount has not been set yet

                new iWaveHealth = HEALINGWAVE_HEAL;

                for ( new iHealthCheck = 0; iHealthCheck <= HEALINGWAVE_JUMPS; iHealthCheck++ )
                {
                    if ( g_ChainJumps[Caster][iHealthCheck] )
                        iWaveHealth -= ( iWaveHealth / HEALINGWAVE_MULTIPLIER );
                }

                g_ChainJumps[Caster][iCurrentJump] = NextTarget;


                // Play Sound

                emit_sound( LastTarget, CHAN_STATIC, SOUND_HEALINGWAVE, 1.0, ATTN_NORM, 0, PITCH_NORM );
                emit_sound( NextTarget, CHAN_STATIC, SOUND_HEALINGWAVE, 1.0, ATTN_NORM, 0, PITCH_NORM );

                // Healing Wave

                Create_TE_BEAMENTS( SHOWTO_ALL_BROADCAST, LastTarget, NextTarget, SPR_SMOOTHBEAM, 0, 10, 15, 100, 4, 255, 255, 128, 255, 0 );
                Create_TE_BEAMENTS( SHOWTO_ALL_BROADCAST, LastTarget, NextTarget, SPR_CHAINLIGHTNING, 0, 15, 15, 80, 4, 255, 255, 255, 255, 0 );

                // Heal Effects

                Shared_Heal_Effect( NextTarget );

                // Light Effects

                Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, LastTarget, 100, 255, 255, 128, 10, 0 );
                Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST, NextTarget, 100, 255, 255, 128, 10, 0 );

                // Calculate Health

                new iNewHealth = get_user_health( NextTarget ) + iWaveHealth;
                new iMaxHealth = WAR3_get_maxhealth( NextTarget );

                new iHealthGiven = iWaveHealth;

                if ( iNewHealth > iMaxHealth )
                {
                    iHealthGiven -= ( iNewHealth - iMaxHealth );
                    iNewHealth = iMaxHealth;
                }

                if ( NextTarget != Caster )
                    g_iHealingWaveHealth[Caster] += iHealthGiven;

                // Hud Message

                new szMessage[128], szPlayerName[32];
                get_user_name( Caster, szPlayerName, 31 );

                if ( Caster == NextTarget )
                    format( szMessage, 127, HEAL_SELF, iHealthGiven );

                else
                {
                    format( szMessage, 127, HEAL_TARGET, szPlayerName, iHealthGiven );
                }

                WAR3_status_text( NextTarget, szMessage, 3.0 );

                set_user_health( NextTarget, iNewHealth );

                // Invisibility Cooldown

                Invis_Cooldown( NextTarget );
            }
        }

        iPlayerNum++;
    }

    if ( iCurrentJump != HEALINGWAVE_JUMPS && bTargetFound )
    {
        parmJump[1] = iCurrentJump + 1;

        new TaskId = TASK_ULTIMATE + Caster;
        set_task( 0.2, "UHealWave_Jump", TaskId, parmJump, 2 );
    }

    else
    {
        // Give Support Xp

        XP_Support_Heal( Caster, g_iHealingWaveHealth[Caster] );

        g_iHealingWaveHealth[Caster] = 0;

        for ( new i = 0; i <= HEALINGWAVE_JUMPS; i++ )
        {
            g_ChainJumps[Caster][i] = 0;
        }
    }

    return PLUGIN_HANDLED;
}


/* - Windwalk --------------------------------------------------- */


public UWindwalk_Cast( id ) {
#if ADVANCED_DEBUG
	log_function("public UWindwalk_Cast( id ) {");
#endif

    UWindwalk_Remove( id );

    // Play Sound

    emit_sound( id, CHAN_STATIC, SOUND_WINDWALK, 1.0, ATTN_NORM, 0, PITCH_NORM );

    // Set Speed

    g_bPlayerWalk[id]  = true;
    g_bPlayerInvis[id] = true;

    set_user_maxspeed( id, WINDWALK_SPEED );

    // Switch to Knife

    engclient_cmd( id, "weapon_knife" );

    // Fade into Invisibility

    new parmCast[3];
    parmCast[0] = id;
    parmCast[1] = WINDWALK_PREWALKTIME * 10;

    if ( g_PlayerInfo[id][CURRENT_ITEM] == ITEM_CLOAK )
        parmCast[2] = floatround( entity_get_float( id, EV_FL_renderamt ) );

    else
    {
        parmCast[2] = 255;
    }

    UWindwalk_Prewalk( parmCast );

    return PLUGIN_HANDLED;
}


public UWindwalk_Prewalk( parmCast[3] ) {
#if ADVANCED_DEBUG
	log_function("public UWindwalk_Prewalk( parmCast[3] ) {");
#endif

    new id         = parmCast[0];
    new iCounter   = parmCast[1];
    new iBaseTrans = parmCast[2];

    UWindwalk_Effects( id, iBaseTrans, iCounter );

    // Check if Player Visible

    iCounter -= 1;

    if ( iCounter )
    {
        parmCast[1] = iCounter;

        new TaskId = TASK_WINDWALK + id;
        set_task( 0.1, "UWindwalk_Prewalk", TaskId, parmCast, 3 );
    }

    else
    {
        new parmWalk[1];
        parmWalk[0] = id;

        UWindwalk_Walk( parmWalk );
    }

    return PLUGIN_HANDLED;
}


public UWindwalk_Effects( id, iBaseTrans, iCounter ) {
#if ADVANCED_DEBUG
	log_function("public UWindwalk_Effects( id, iBaseTrans, iCounter ) {");
#endif

    // Fade Into Invisibility From Current Invis Level

    new iTransLevel = WINDWALK_INVISIBILITY + ( iCounter * ( ( iBaseTrans - WINDWALK_INVISIBILITY ) / ( WINDWALK_PREWALKTIME * 10 ) ) );
    set_user_rendering( id, kRenderNormal, 0, 0, 0, kRenderTransTexture, iTransLevel );

    // Stretch View

    new iStretchAmmount = 90 + ( ( WINDWALK_FOV / ( WINDWALK_PREWALKTIME * 10 ) ) * ( ( WINDWALK_PREWALKTIME * 10 ) - ( iCounter ) ) );
    Create_SetFOV( id, iStretchAmmount );

    return PLUGIN_HANDLED;
}


public UWindwalk_Walk( parmWalk[1] ) {
#if ADVANCED_DEBUG
	log_function("public UWindwalk_Walk( parmWalk[1] ) {");
#endif

    new id = parmWalk[0];

    // Make Invisible

    set_user_rendering( id, kRenderNormal, 0, 0, 0, kRenderTransTexture, WINDWALK_INVISIBILITY );

	// Bartime

    Create_BarTime( id, WINDWALK_DURATION );

    // Set Task to Make Visible

    new parmPost[2];
    parmPost[0] = id;

    new TaskId = TASK_WINDWALK + id;

    new iDuration = WINDWALK_DURATION;
    new Float:fDuration = float( iDuration );

    set_task( fDuration, "UWindwalk_Postwalk", TaskId, parmPost, 2 );

    return PLUGIN_HANDLED;
}


public UWindwalk_Strike( Attacker, Victim, Weapon, Headshot ) {
#if ADVANCED_DEBUG
	log_function("public UWindwalk_Strike( Attacker, Victim, Weapon, Headshot ) {");
#endif

    // Remove Windwalk

    UWindwalk_Remove( Attacker );

    if ( Weapon != CSW_KNIFE )
        return PLUGIN_HANDLED;

    // Check for Amulet

    if ( !IAmulet_Ready( Attacker, Victim ) && !g_iPlayerAvatar[Victim] && Victim != g_Vip )
    {
        // Deal Damage

        WAR3_damage( Attacker, Victim, CSW_WINDWALK, WINDWALK_DAMAGE, Headshot, DAMAGE_NOCHECKARMOR );

        // Glow Player

        new iRGB[3];
        iRGB[GLOW_R] = 255;

        Glow_Set( Victim, 2.0, iRGB, 36 );

        // Screen Fade

        Create_ScreenFade( Victim, (1<<10), (1<<10), FADE_OUT, 255, 0, 0, 255 );

        // Bloodstream

        new Origin[3];
        get_user_origin( Victim, Origin );

        Origin[2] += 30;

        for ( new i = 0; i < 5; i++ )
        {
            new Vector[3];
            Vector[0] = random_num( -20, 20 );
            Vector[1] = random_num( -20, 20 );
            Vector[2] = random_num( 50, 300 );

            Create_TE_BLOODSTREAM( SHOWTO_ALL_BROADCAST, Origin, Vector, 70, random_num( 100, 200 ) );
        }
    }

    return PLUGIN_HANDLED;
}


public UWindwalk_Notify( id, iTotalDamage ) {
#if ADVANCED_DEBUG
	log_function("public UWindwalk_Notify( id, iTotalDamage ) {");
#endif

    new szMessage[128];
    format( szMessage, 127, CAST_WINDWALK, iTotalDamage );

    WAR3_status_text( id, szMessage, 3.0 );

    g_iWindwalkDamage[id] = 0;

    return PLUGIN_HANDLED;
}


public UWindwalk_Postwalk( parmPost[2] ) {
#if ADVANCED_DEBUG
	log_function("public UWindwalk_Postwalk( parmPost[2] ) {");
#endif

    new id       = parmPost[0];
    new iCounter = parmPost[1];

    if ( !iCounter )
    {
        g_bPlayerWalk[id] = false;

        // Make sure Task is Unset

        new TaskId = TASK_WINDWALK + id;
        remove_task( TaskId, 0 );

        // Remove Bartime

        Remove_BarTime( id );

        // Hud Message

        new szMessage[128];
        copy( szMessage, 127, FINISH_WINDWALK );

        WAR3_status_text( id, szMessage, 0.5 );
    }

    iCounter += 1;

    UWindwalk_Effects( id, 255, iCounter );

    // Check if Player Visible

    if ( iCounter == WINDWALK_PREWALKTIME * 10 )
    {
        UWindwalk_Remove( id );
    }

    else
    {
        parmPost[1] = iCounter;

        new TaskId = TASK_WINDWALK + id;
        set_task( 0.1, "UWindwalk_Postwalk", TaskId, parmPost, 2 );
    }

    return PLUGIN_HANDLED;
}


public UWindwalk_Remove( id ) {
#if ADVANCED_DEBUG
	log_function("public UWindwalk_Remove( id ) {");
#endif

    g_bPlayerWalk[id]  = false;
    g_bPlayerInvis[id] = false;

    new TaskId = TASK_WINDWALK + id;
    remove_task( TaskId, 0 );

    // Remove Invisiblity

    set_user_rendering( id );

    // Remove View Stretch

    Remove_SetFOV( id );

    // Cooldown

    Ultimate_Cooldown( id, ULTIMATE_COOLDOWNDEFAULT );

    // Remove Bartime

    Remove_BarTime( id );

    // Set Player Speed

    WAR3_set_speed( id );

    // Invisibility Cooldown

    Invis_Cooldown( id );

    return PLUGIN_HANDLED;
}

// End of ORC.INL
