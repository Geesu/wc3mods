
/* - Events	----------------------------------------------------- */


public OR_skills_offensive(	attacker, victim, weapon, iDamage, headshot, Float:fDamageOrigin[3]	) {

	if ( g_PlayerInfo[attacker][CURRENT_RACE] == RACE_ORC && get_user_team(	attacker ) != get_user_team( victim	) && get_user_health( victim ) > 0 )
	{
		// Berserk

		if ( g_PlayerInfo[attacker][CURRENT_SKILL1]	&& weapon != CSW_HEGRENADE )
		{
			OR_S_BERSERK_damage( attacker, victim, weapon, iDamage,	headshot );
		}

		// Pulverize

		if ( g_PlayerInfo[attacker][CURRENT_SKILL2]	&& weapon == CSW_HEGRENADE )
		{
			OR_S_PULVERIZE(	attacker, victim, fDamageOrigin, iDamage );
		}

		// Pillage

		if ( g_PlayerInfo[attacker][CURRENT_SKILL3]	&& weapon != CSW_HEGRENADE )
		{
			OR_S_PILLAGE( attacker,	victim,	iDamage, weapon	);
		}
	}

	return PLUGIN_HANDLED;
}


public OR_skills_defensive(	victim,	iDamage	) {

	if ( g_PlayerInfo[victim][CURRENT_RACE]	== RACE_ORC	)
	{
		OR_S_BERSERK_effect( victim, iDamage );

		// Regeneration	( taken	care of	in on_Health() )
	}

	return PLUGIN_HANDLED;
}


public OR_ultimates( caster, target	) {

	// Chain Lightning

	if ( g_PlayerInfo[caster][CURRENT_ULTIMATE]	== ULTIMATE_LIGHTNING && get_user_team(	target ) !=	get_user_team( caster )	)
	{
		OR_U_CHAINLIGHTNING( caster, target	);
		Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT	);

		SHARED_INVIS_cooldown( caster );
	}

	// Healing Wave

	else if	( g_PlayerInfo[caster][CURRENT_ULTIMATE] ==	ULTIMATE_HEALINGWAVE &&	get_user_team( target )	== get_user_team( caster ) )
	{
		if ( get_user_health( target ) == WAR3_get_maxhealth( target ) )
		{
			new	szMessage[64];

			if ( caster	== target )
			{
				formatex( szMessage, 63, "%L", caster, "FULLHEALTH_SELF" );
			}

			else
			{
				formatex( szMessage, 63, "%L", caster, "FULLHEALTH_TARGET" );
			}

			WAR3_status_text( caster, szMessage, 0.5 );
			Ultimate_Beep( caster );
		}

		else
		{
			OR_U_HEALINGWAVE( caster, target );
			Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT	);

			SHARED_INVIS_cooldown( caster );
		}
	}

	// Wind	Walk

	else if	( g_PlayerInfo[caster][CURRENT_ULTIMATE] ==	ULTIMATE_WINDWALK )
	{
		OR_U_WINDWALK( caster );
		Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT	);
	}

	else
	{
		Ultimate_Beep( caster );
		return PLUGIN_HANDLED;
	}

	return PLUGIN_HANDLED;
}


/* - Racial	Ability	--------------------------------------------- */


// Regeneration

public Float:OR_S_REGENERATION_get(	iLevel ) {

	new	Float:fLevel = float( iLevel );

	if ( fLevel	> LEVEL_RACIALCAP )
		fLevel = LEVEL_RACIALCAP;

	new	Float:fRegenRate = OR_fRegeneration[0] - ( ( ( OR_fRegeneration[0] - OR_fRegeneration[1] ) / LEVEL_RACIALCAP ) * fLevel	);

	if ( fRegenRate	< OR_fRegeneration[1] )
		fRegenRate = OR_fRegeneration[1];

	return ( fRegenRate	);
}


public OR_S_REGENERATION_set( id ) {

	if ( g_bPlayerRegen[id]	|| ( WAR3_get_maxhealth( id	) == get_user_health( id ) && (	!get_user_armor( id	) || get_user_armor( id	) == WAR3_get_maxarmor(	id ) ) ) )
		return PLUGIN_HANDLED;

	g_bPlayerRegen[id] = true;

	new	iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
	new	Float:fRegenRate = OR_S_REGENERATION_get( iLevel );

	new	parm_Regen[1];
	parm_Regen[0] =	id;

	new	task = TASK_REGEN +	id;
	set_task( fRegenRate, "OR_S_REGENERATION_heal",	task, parm_Regen, 1	);

	// Regeneration	Icon

	return PLUGIN_HANDLED;
}


public OR_S_REGENERATION_heal( parm_Regen[1] ) {

	new	id = parm_Regen[0];

	if ( get_user_health( id ) >= WAR3_get_maxhealth( id ) && get_user_armor( id ) >= WAR3_get_maxarmor( id	) )
		OR_S_REGENERATION_remove( id );

	else
	{
		// Health

		if ( get_user_health( id ) && get_user_health( id )	< WAR3_get_maxhealth( id ) )
		{
			new	iHealth	= get_user_health( id );
			new	iNewHealth = iHealth + REGENERATION_AMOUNT;

			set_user_health( id, iNewHealth	);

			// Increase	total health of	victim for kill	assist

			g_KillAssist_iTotalHealth[id] += REGENERATION_AMOUNT;
		}

		// Armor

		if ( get_user_armor( id	) && get_user_armor( id	) <	WAR3_get_maxarmor( id )	)
		{
			new	iArmor = get_user_armor( id	);
			new	iNewArmor =	iArmor + REGENERATION_AMOUNT;

			set_user_armor(	id,	iNewArmor );
		}

		new	iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );
		new	Float:fRegenRate = OR_S_REGENERATION_get( iLevel );

		new	task = TASK_REGEN +	id;
		set_task( fRegenRate, "OR_S_REGENERATION_heal",	task, parm_Regen, 1	);
	}

	return PLUGIN_HANDLED;
}


public OR_S_REGENERATION_remove( id	) {

	g_bPlayerRegen[id] = false;

	new	task = TASK_REGEN +	id;
	remove_task( task, 0 );

	// Regeneration	Icon

	return PLUGIN_HANDLED;
}


/* - Skills	----------------------------------------------------- */


// Berserk

public Float:OR_S_BERSERK_get_speed( id	) {

	new	iLevel = g_PlayerInfo[id][CURRENT_SKILL1] -	1;

	new	iMaxHealth = WAR3_get_maxhealth( id	);
	new	iHealth	= get_user_health( id );

	new	Float:fBerserkSpeed	= SPEED_KNIFE +	( (	OR_fBerserk_speed[iLevel] -	SPEED_KNIFE	) *	( (	float( iMaxHealth )	- float( iHealth ) ) / float( iMaxHealth ) ) );

	// Don't exceed	max	berserk	speed

	if ( fBerserkSpeed > OR_fBerserk_speed[2] )
		fBerserkSpeed =	OR_fBerserk_speed[2];

	return ( fBerserkSpeed );
}


public OR_S_BERSERK_set_speed( id )	{

	if ( !WAR3_skill_enabled( id, RACE_ORC,	SKILL_1	) || !is_user_alive( id	) )
		return PLUGIN_HANDLED;

	new	Float:fBerserkSpeed	= OR_S_BERSERK_get_speed( id );
	set_user_maxspeed( id, fBerserkSpeed );

	return PLUGIN_HANDLED;
}


static OR_S_BERSERK_damage(	attacker, victim, weapon, iDamage, headshot	) {

	// Check if	the	attacker already had berserk enabled

	if ( WAR3_skill_enabled( attacker, RACE_ORC, SKILL_1 ) )
	{
		new	iHealth	= get_user_health( attacker	);

		if ( iHealth <=	BERSERK_HEALTH )
		{
			// (100	- Current Health) *	( Damage * Multiplier )

			new	Float:fHealthMultiplier	= float( ( 100 - iHealth ) ) / 100.0;

			new	iBonusDamage = floatround( fHealthMultiplier * (OR_fBerserk_damage[g_PlayerInfo[attacker][CURRENT_SKILL1] -	1] * iDamage) );

			// Apply Damage

			WAR3_damage( attacker, victim, weapon, iBonusDamage, headshot, DAMAGE_CHECKARMOR );

			// Add to player stats array

			if ( get_cvar_num( "mp_war3stats" )	)
			{
				playerSkill1Info[attacker][0] += iBonusDamage;
			}

			// Screen Fade

			new	iFadeAlpha = iBonusDamage *	3;
			Create_ScreenFade( victim, (1<<10),	(1<<10), FADE_OUT, 255,	0, 0, iFadeAlpha );
		}
	}

	return PLUGIN_HANDLED;
}


static OR_S_BERSERK_effect(	id,	iDamage	) {

	if ( ( get_user_health(	id ) + iDamage ) > BERSERK_HEALTH )
	{
		// Screen Fade

		// Create_ScreenFade( id, (1<<10), (1<<10),	FADE_OUT, 255, 0, 0, 100 );

		// Play	Status Text
	}

	return PLUGIN_HANDLED;
}


// Pulverize

static OR_S_PULVERIZE( attacker, victim, Float:fGrenadeOrigin[3], damage ) {

	// Check if	restricted

	if ( !WAR3_skill_enabled( attacker,	RACE_ORC, SKILL_2 )	)
		return PLUGIN_HANDLED;

	new	Enemies[32], szTeamName[16];
	new	iTotalEnemies;

	new	bool:bHitPlayers = false;

	get_user_team( victim, szTeamName, 15 );
	get_players( Enemies, iTotalEnemies, "ae", szTeamName );

	// Convert origin to int

	new	iGrenadeOrigin[3];

	for	( new i	= 0; i < 3;	i++	)
		iGrenadeOrigin[i] =	floatround(	fGrenadeOrigin[i] );

	// Check for enemies in	range of victim(s)

	for	( new iPlayerNum = 0; iPlayerNum < iTotalEnemies; iPlayerNum++ )
	{
		new	enemy =	Enemies[iPlayerNum];

		if ( enemy != victim &&	!g_bPlayerSleeping[enemy] )
		{
			new	teamOrigin[3];
			get_user_origin( enemy,	teamOrigin );

			new	Float:fMetricDistance =	distance( get_distance(	iGrenadeOrigin,	teamOrigin ) );

			// Determine if	a nearby teammate is close enough to damage

			if ( fMetricDistance <=	OR_fPulverize_range[g_PlayerInfo[attacker][CURRENT_SKILL2] - 1]	)
			{
				bHitPlayers	= true;

				// Damage calculation

				new	iPulverizeDamage = floatround( OR_fPulverize_bonus[g_PlayerInfo[attacker][CURRENT_SKILL2] -	1] * float(	damage ) );

				// Armor calculation

				if ( get_user_armor( enemy ) )
				{
					new	Float:fArmorDamage = float(	iPulverizeDamage ) * PULVERIZE_ARMOR;
					new	iNewArmor =	get_user_armor(	enemy )	- floatround( fArmorDamage );

					if ( iNewArmor < 0 )
						iNewArmor =	0;

					set_user_armor(	enemy, iNewArmor );
				}

				new	szMessage[128];
				formatex( szMessage, 127, "%L",	enemy, "DAMAGE_PULVERIZE", iPulverizeDamage	);

				WAR3_status_text2( enemy, szMessage, 3.0 );

				// Add to player stats array

				if ( get_cvar_num( "mp_war3stats" )	)
				{
					playerSkill2Info[attacker][0] += iPulverizeDamage;
				}

				// Apply Damage

				WAR3_damage( attacker, enemy, CSW_PULVERIZE, iPulverizeDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );

				// Screen Fade

				if ( is_user_alive(	enemy )	)
				{
					new	iFadeAlpha = iPulverizeDamage *	2;

					if ( iFadeAlpha	> GLOW_MAX )
						iFadeAlpha = GLOW_MAX;

					Create_ScreenFade( enemy, (1<<10), (1<<10),	FADE_OUT, 160, 0, 0, iFadeAlpha	);
				}
			}
		}
	}

	// Show	Pulverize Rings	if we hit a	player

	if ( bHitPlayers )
	{
		// Raise grenade origin	to waist level

		iGrenadeOrigin[2] += 40;

		new	iRGB[3];

		for	( new i	= 0; i < 3;	i++	)
			iRGB[i]	= PULVERIZE_RING_RGB[i]	- (	8 *	( 3	- g_PlayerInfo[attacker][CURRENT_SKILL2] ) );

		new	iRingSize =	floatround(	OR_fPulverize_range[g_PlayerInfo[attacker][CURRENT_SKILL2] - 1]	* 2.0 *	40.0 );
		new	OuterRadius[3],	InnerRadius[3];

		OuterRadius[2] = iRingSize;
		InnerRadius[2] = iRingSize / 2;

		// Inner Ring

		Create_TE_BEAMCYLINDER(	SHOWTO_ALL_BROADCAST, iGrenadeOrigin, InnerRadius, SPR_SHOCKWAVE, 0, 0,	3, 3, 0, iRGB[GLOW_R], iRGB[GLOW_G], iRGB[GLOW_B], 255,	0 );

		// Outer Ring

		iRGB[GLOW_B] -=	32;
		Create_TE_BEAMCYLINDER(	SHOWTO_ALL_BROADCAST, iGrenadeOrigin, OuterRadius, SPR_SHOCKWAVE, 0, 0,	3, 6, 0, iRGB[GLOW_R], iRGB[GLOW_G], iRGB[GLOW_B], 255,	0 );
	}

	return PLUGIN_HANDLED;
}


public OR_S_PULVERIZE_trail( id, grenade ) {

	// Check if	restricted

	if ( !WAR3_skill_enabled( id, RACE_ORC,	SKILL_2	) )
		return PLUGIN_HANDLED;

	new	iWidth = g_PlayerInfo[id][CURRENT_SKILL2];
	new	iBrightness	= (	75 / 3 ) * g_PlayerInfo[id][CURRENT_SKILL2];

	Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST,	grenade, SPR_ROOT, 10, iWidth, 255,	0, 0, iBrightness );

	return PLUGIN_HANDLED;
}


// Pillage

static OR_S_PILLAGE( attacker, victim, iDamage,	weapon ) {

	if ( !WAR3_skill_enabled( attacker,	RACE_ORC, SKILL_3 )	)
		return PLUGIN_HANDLED;

	new	Float:fPillageChance = OR_fPillage[g_PlayerInfo[attacker][CURRENT_SKILL3] -	1];
	new	Float:fRandomNum = random_float( 0.0, 1.0 );

	if ( weapon	== CSW_KNIFE )
	{
		fPillageChance += fPillageChance * PILLAGE_KNIFEBONUS;

		if ( fRandomNum	> fPillageChance )
			return PLUGIN_HANDLED;

		// Steal money and grenades	( if applicable	)

		OR_S_PILLAGE_money(	attacker, victim, iDamage, weapon );
		OR_S_PILLAGE_grenade( attacker,	victim );
	}

	else
	{
		if ( fRandomNum	> fPillageChance )
			return PLUGIN_HANDLED;

		// Steal random	type ( if applicable )

		new	iType =	random_num(	1, 3 );

		switch(	iType )
		{
			case INDEX_MONEY:	OR_S_PILLAGE_money(	attacker, victim, iDamage, weapon );
			case INDEX_AMMO:	OR_S_PILLAGE_ammo( attacker/*, victim, iDamage*/ );
			case INDEX_GRENADE:	OR_S_PILLAGE_grenade( attacker,	victim );
		}
	}

	return PLUGIN_HANDLED;
}


static OR_S_PILLAGE_money( attacker, victim, iDamage, weapon ) {

	new	Float:fDamage =	float( iDamage );

	new	iVictimMoney = cs_get_user_money( victim );

	if ( iVictimMoney <= 0 )
		return PLUGIN_HANDLED;

	new	Float:fStolenMoney = PILLAGE_MONEY * fDamage;

	if ( weapon	== CSW_KNIFE )
		fStolenMoney +=	fStolenMoney * PILLAGE_KNIFEBONUS;

	new	iStolenMoney = floatround( fStolenMoney	);

	if ( iVictimMoney -	iStolenMoney < 0 )
		iStolenMoney = iVictimMoney;

	cs_update_money( victim, -iStolenMoney,	1 );
	cs_update_money( attacker, iStolenMoney, 0 );

	return PLUGIN_HANDLED;
}


static OR_S_PILLAGE_ammo( attacker/*, victim, iDamage*/	) {

	//new Float:fDamage	= float( iDamage );

	//new iVictimClip, iVictimAmmo;
	//new iVictimWeapon	= get_user_weapon( victim, iVictimClip,	iVictimAmmo	);

	new	iAttackerClip, iAttackerAmmo;
	new	iAttackerWeapon	= get_user_weapon( attacker, iAttackerClip,	iAttackerAmmo );

	if ( iAttackerWeapon ==	CSW_KNIFE )
		return PLUGIN_HANDLED;

	//if ( iVictimAmmo <= 0	)
	//	return PLUGIN_HANDLED;

	//new iStolenAmmo =	floatround(	PILLAGE_AMMO * fDamage );

	//if ( iVictimAmmo - iStolenAmmo < 0 )
	//	iStolenAmmo	= iVictimAmmo;


	// Temporary Solution

	for	( new i	= 1; i <= PILLAGE_AMMO;	i++	)
		give_item( attacker, CS_AMMO_NAME[iAttackerWeapon] );


	//cs_update_ammo( victim, -iStolenAmmo,	iVictimWeapon );
	//cs_update_ammo( attacker,	iStolenAmmo, iAttackerWeapon );

	return PLUGIN_HANDLED;
}


static OR_S_PILLAGE_grenade( attacker, victim )	{

	new	Grenades[3], iTotalGrenades;

	new	Weapons[32], iTotalWeapons;
	get_user_weapons( victim, Weapons, iTotalWeapons );

	for	( new weaponNum	= 0; weaponNum < iTotalWeapons;	weaponNum++	)
	{
		new	weapon = Weapons[weaponNum];

		if ( cs_get_weapon_type_( weapon ) == CS_WEAPON_TYPE_GRENADE )
		{
			new	iClip, iAmmo;
			get_user_ammo( attacker, weapon, iClip,	iAmmo );

			if ( !iAmmo	|| ( weapon	== CSW_FLASHBANG &&	iAmmo <	2 )	)
			{
				Grenades[iTotalGrenades] = weapon;
				iTotalGrenades++;
			}
		}
	}

	if ( iTotalGrenades	> 0	)
	{
		new	j =	random_num(	0, iTotalGrenades -	1 );
		new	grenade	= Grenades[j];

		//new iVictimClip, iVictimAmmo;
		//new iVictimWeapon	= get_user_weapon( victim, iVictimClip,	iVictimAmmo	);

		//if ( iVictimWeapon ==	grenade	)
		//	engclient_cmd( victim, "lastinv" );


		// Temporary Solution

		give_item( attacker, CS_WEAPON_NAME[grenade] );


		//cs_update_ammo( victim, -1, OR_fPillageNades[i] );
		//cs_update_ammo( attacker,	1, OR_fPillageNades[i] );
	}

	return PLUGIN_HANDLED;
}


/* - Chain Lightning -------------------------------------------- */


static OR_U_CHAINLIGHTNING(	caster,	target ) {

	// Play	Sound

	emit_sound(	caster,	CHAN_STATIC, SOUND_CHAINLIGHTNING, 1.0,	ATTN_NORM, 0, PITCH_NORM );
	emit_sound(	target,	CHAN_STATIC, SOUND_CHAINLIGHTNING, 1.0,	ATTN_NORM, 0, PITCH_NORM );

	// Lightning bolt

	Create_TE_BEAMENTS(	SHOWTO_ALL_BROADCAST, caster, target, SPR_CHAINLIGHTNING, 0, 15, 10, 80, 10, 255, 255, 255,	255, 0 );

	// Light effects

	Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	caster,	100, 255, 255, 255,	10,	0 );
	Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	target,	100, 255, 255, 255,	10,	0 );

	// Check for Amulet

	if ( g_PlayerInfo[target][CURRENT_ITEM]	== ITEM_AMULET )
		ITEM_AMULET_block( target, caster );

	else
	{
		// Hud message

		new	szMessage[128],	szPlayerName[32];
		get_user_name( caster, szPlayerName, 31	);

		formatex( szMessage, 127, "%L",	target,	"CAST_CHAINLIGHTNING", szPlayerName, CHAINLIGHTNING_DAMAGE );

		WAR3_status_text2( target, szMessage, 3.0 );

		// Shock damage

		Create_Damage( target, 0, 0, CS_DMG_SHOCK );

		// Apply damage

		WAR3_damage( caster, target, CSW_LIGHTNING,	CHAINLIGHTNING_DAMAGE, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );
	}

	g_ChainJumps[caster][0]	= target;

	new	parmJump[2];
	parmJump[0]	= caster;
	parmJump[1]	= 1;

	new	task = TASK_ULTIMATE + caster;
	set_task( 0.2, "OR_U_CHAINLIGHTNING_jump", task, parmJump, 2 );

	return PLUGIN_HANDLED;
}


public OR_U_CHAINLIGHTNING_jump( parmJump[2] ) {

	new	caster		 = parmJump[0];
	new	iCurrentJump = parmJump[1];
	new	LastTarget	 = g_ChainJumps[caster][iCurrentJump - 1];

	new	LastOrigin[3], NextOrigin[3];
	get_user_origin( LastTarget, LastOrigin	);

	// Get all alive teammates

	new	szTeamName[16];
	get_user_team( LastTarget, szTeamName, 15 );

	new	Teammates[32], iTotalPlayers;
	get_players( Teammates,	iTotalPlayers, "ae", szTeamName	);

	// Cast	on next	teammate in	range

	new	iPlayerNum,	bool:bTargetFound;

	while (	iPlayerNum < iTotalPlayers && !bTargetFound	)
	{
		new	NextTarget = Teammates[iPlayerNum];

		get_user_origin( NextTarget, NextOrigin	);

		if ( get_distance( LastOrigin, NextOrigin )	/ 40 <=	CHAINLIGHTNING_RANGE &&	ent_in_view( LastTarget, NextTarget	) )
		{
			// Check if	target already hit

			new	bool:bTargetHit;

			for	( new i	= 0; i <= CHAINLIGHTNING_JUMPS;	i++	)
			{
				if ( g_ChainJumps[caster][i] ==	NextTarget )
					bTargetHit = true;
			}

			// Don't Jump to Players Already Hit / Sleeping	/ Invis	/ Avatar'd

			new	bool:bPlayerImmune;

			if ( g_bPlayerSleeping[NextTarget] || g_bPlayerInvis[NextTarget] ||	g_iPlayerAvatar[NextTarget]	)
				bPlayerImmune =	true;

			if ( !bTargetHit &&	!bPlayerImmune )
			{
				bTargetFound = true;

				// Set Damage of Lightning
				// we don't	count final	jump because if	this is	final jump,	damage has not been	set	yet

				new	iBoltDamage	= CHAINLIGHTNING_DAMAGE;

				for	( new iDamageCheck = 0;	iDamageCheck <=	CHAINLIGHTNING_JUMPS; iDamageCheck++ )
				{
					if ( g_ChainJumps[caster][iDamageCheck]	)
						iBoltDamage	-= ( iBoltDamage / CHAINLIGHTNING_MULTIPLIER );
				}

				g_ChainJumps[caster][iCurrentJump] = NextTarget;


				// Play	Sound

				emit_sound(	LastTarget,	CHAN_STATIC, SOUND_CHAINLIGHTNING, 1.0,	ATTN_NORM, 0, PITCH_NORM );
				emit_sound(	NextTarget,	CHAN_STATIC, SOUND_CHAINLIGHTNING, 1.0,	ATTN_NORM, 0, PITCH_NORM );

				// Lightning Bolt

				Create_TE_BEAMENTS(	SHOWTO_ALL_BROADCAST, LastTarget, NextTarget, SPR_CHAINLIGHTNING, 0, 15, 10, 80, 10, 255, 255, 255,	255, 0 );

				// Light Effect	(target)

				Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	LastTarget,	100, 255, 255, 255,	10,	0 );
				Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	NextTarget,	100, 255, 255, 255,	10,	0 );

				// Check for Amulet

				if ( g_PlayerInfo[NextTarget][CURRENT_ITEM]	== ITEM_AMULET )
					ITEM_AMULET_block( NextTarget, caster );

				else
				{
					// Hud Message

					new	szMessage[128],	szPlayerName[32];
					get_user_name( caster, szPlayerName, 31	);

					formatex( szMessage, 127, "%L",	NextTarget,	"CAST_CHAINLIGHTNING_JUMP",	szPlayerName, iBoltDamage );

					WAR3_status_text2( NextTarget, szMessage, 3.0 );

					// Shock Damage

					Create_Damage( NextTarget, 0, 0, CS_DMG_SHOCK );

					// Apply Damage

					WAR3_damage( caster, NextTarget, CSW_LIGHTNING,	iBoltDamage, CS_HEADSHOT_NO, DAMAGE_NOCHECKARMOR );
				}
			}
		}

		iPlayerNum++;
	}

	if ( iCurrentJump != CHAINLIGHTNING_JUMPS && bTargetFound )
	{
		parmJump[1]	= iCurrentJump + 1;

		new	task = TASK_ULTIMATE + caster;
		set_task( 0.2, "OR_U_CHAINLIGHTNING_jump", task, parmJump, 2 );
	}

	else
	{
		for	( new i	= 0; i <= CHAINLIGHTNING_JUMPS;	i++	)
		{
			g_ChainJumps[caster][i]	= 0;
		}
	}

	return PLUGIN_HANDLED;
}


/* - Healing Wave ----------------------------------------------- */


static OR_U_HEALINGWAVE( caster, target	) {

	// Status Text

	new	szCastMessage[64];
	formatex( szCastMessage, 63, "%L", caster, "HEAL_CAST" );

	WAR3_status_text( caster, szCastMessage, 1.0 );

	// Play	Sound

	emit_sound(	caster,	CHAN_STATIC, SOUND_HEALINGWAVE,	1.0, ATTN_NORM,	0, PITCH_NORM );

	// Healing Wave

	if ( caster	!= target )
	{
		emit_sound(	target,	CHAN_STATIC, SOUND_HEALINGWAVE,	1.0, ATTN_NORM,	0, PITCH_NORM );

		Create_TE_BEAMENTS(	SHOWTO_ALL_BROADCAST, caster, target, SPR_SMOOTHBEAM, 0, 10, 15, 100, 4, 255, 255, 128,	255, 0 );
		Create_TE_BEAMENTS(	SHOWTO_ALL_BROADCAST, caster, target, SPR_CHAINLIGHTNING, 0, 15, 15, 80, 4,	255, 255, 255, 255,	0 );

		Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	target,	100, 255, 255, 128,	10,	0 );
	}

	// Heal	Effect

	Shared_Heal_Effect(	target );

	// Light Effects

	Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	caster,	100, 255, 255, 128,	10,	0 );

	// Calculate actual	heal amount	( self healing ultimate	)

	new	iHealAmount	= HEALINGWAVE_HEAL;

	if ( target	== caster )
	{
		new	Float:fHealAmount =	float( iHealAmount ) * SELFHEAL_MODIFIER;
		iHealAmount	= floatround( fHealAmount );
	}

	// Heal	player

	new	iHealthGiven = WAR3_heal( caster, target, iHealAmount );

	if ( target	!= caster )
		g_iHealingWaveHealth[caster] +=	iHealthGiven;

	// Hud message

	new	szMessage[128];

	if ( caster	== target )
		formatex( szMessage, 127, "%L",	caster,	"HEAL_SELF", iHealthGiven );

	else
	{
		new	szPlayerName[32];
		get_user_name( caster, szPlayerName, 31	);

		formatex( szMessage, 127, "%L",	target,	"HEAL_TARGET", szPlayerName, iHealthGiven );
	}

	WAR3_status_text( target, szMessage, 3.0 );

	// Jump	to next	target

	g_ChainJumps[caster][0]	= target;

	new	parmJump[2];
	parmJump[0]	= caster;
	parmJump[1]	= 1;

	new	task = TASK_ULTIMATE + caster;
	set_task( 0.2, "OR_U_HEALINGWAVE_jump",	task, parmJump,	2 );

	return PLUGIN_HANDLED;
}


public OR_U_HEALINGWAVE_jump( parmJump[2] )	{

	new	caster		 = parmJump[0];
	new	iCurrentJump = parmJump[1];
	new	LastTarget	 = g_ChainJumps[caster][iCurrentJump - 1];

	new	LastOrigin[3], NextOrigin[3];
	get_user_origin( LastTarget, LastOrigin	);

	// Get all alive teammates

	new	szTeamName[16];
	get_user_team( LastTarget, szTeamName, 15 );

	new	Teammates[32], iTotalPlayers
	get_players( Teammates,	iTotalPlayers, "ae", szTeamName	);

	new	iPlayerNum,	bool:bTargetFound;

	while (	iPlayerNum < iTotalPlayers && !bTargetFound	)
	{
		new	NextTarget = Teammates[iPlayerNum];

		get_user_origin( NextTarget, NextOrigin	);

		// Don't attempt to	hit	players	out	of range / not in view

		if ( get_distance( LastOrigin, NextOrigin )	/ 40 <=	HEALINGWAVE_RANGE && ent_in_view( LastTarget, NextTarget ) )
		{
			// Check if	target already hit

			new	bool:bTargetHit;

			for	( new i	= 0; i <= HEALINGWAVE_JUMPS; i++ )
			{
				if ( g_ChainJumps[caster][i] ==	NextTarget )
					bTargetHit = true;
			}

			// Don't heal players already hit /	at full	health / Avatar'd

			if ( !bTargetHit &&	get_user_health( NextTarget	) != WAR3_get_maxhealth( NextTarget	) && !g_iPlayerAvatar[NextTarget] )
			{
				bTargetFound = true;

				// Set Heal	Ammount
				// we don't	count final	jump because if	this is	final jump,	heal ammount has not been set yet

				new	iWaveHealth	= HEALINGWAVE_HEAL;

				for	( new iHealthCheck = 0;	iHealthCheck <=	HEALINGWAVE_JUMPS; iHealthCheck++ )
				{
					if ( g_ChainJumps[caster][iHealthCheck]	)
						iWaveHealth	-= ( iWaveHealth / HEALINGWAVE_MULTIPLIER );
				}

				g_ChainJumps[caster][iCurrentJump] = NextTarget;

				// Play	Sound

				emit_sound(	LastTarget,	CHAN_STATIC, SOUND_HEALINGWAVE,	1.0, ATTN_NORM,	0, PITCH_NORM );
				emit_sound(	NextTarget,	CHAN_STATIC, SOUND_HEALINGWAVE,	1.0, ATTN_NORM,	0, PITCH_NORM );

				// Healing Wave

				Create_TE_BEAMENTS(	SHOWTO_ALL_BROADCAST, LastTarget, NextTarget, SPR_SMOOTHBEAM, 0, 10, 15, 100, 4, 255, 255, 128,	255, 0 );
				Create_TE_BEAMENTS(	SHOWTO_ALL_BROADCAST, LastTarget, NextTarget, SPR_CHAINLIGHTNING, 0, 15, 15, 80, 4,	255, 255, 255, 255,	0 );

				// Light Effects

				Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	LastTarget,	100, 255, 255, 128,	10,	0 );
				Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	NextTarget,	100, 255, 255, 128,	10,	0 );

				// Heal	Effects

				Shared_Heal_Effect(	NextTarget );

				// Heal	player

				new	iHealthGiven = WAR3_heal( caster, NextTarget, iWaveHealth );

				if ( NextTarget	!= caster )
					g_iHealingWaveHealth[caster] +=	iHealthGiven;

				// Hud Message

				new	szMessage[128];

				if ( caster	== NextTarget )
					formatex( szMessage, 127, "%L",	caster,	"HEAL_SELF", iHealthGiven );

				else
				{
					new	szPlayerName[32];
					get_user_name( caster, szPlayerName, 31	);

					formatex( szMessage, 127, "%L",	NextTarget,	"HEAL_TARGET", szPlayerName, iHealthGiven );
				}

				WAR3_status_text( NextTarget, szMessage, 3.0 );
			}
		}

		iPlayerNum++;
	}

	if ( iCurrentJump != HEALINGWAVE_JUMPS && bTargetFound )
	{
		parmJump[1]	= iCurrentJump + 1;

		new	task = TASK_ULTIMATE + caster;
		set_task( 0.2, "OR_U_HEALINGWAVE_jump",	task, parmJump,	2 );
	}

	else
	{
		// Give	Support	Xp

		XP_Support_Heal( caster, g_iHealingWaveHealth[caster] );

		g_iHealingWaveHealth[caster] = 0;

		for	( new i	= 0; i <= HEALINGWAVE_JUMPS; i++ )
		{
			g_ChainJumps[caster][i]	= 0;
		}
	}

	return PLUGIN_HANDLED;
}


/* - Windwalk --------------------------------------------------- */


static OR_U_WINDWALK( id ) {

	OR_U_WINDWALK_remove( id );

	// Play	Sound

	emit_sound(	id,	CHAN_STATIC, SOUND_WINDWALK, 1.0, ATTN_NORM, 0,	PITCH_NORM );

	// Set Speed

	g_bPlayerWalk[id]  = true;
	g_bPlayerInvis[id] = true;

	set_user_maxspeed( id, WINDWALK_SPEED );

	// Switch to Knife

	engclient_cmd( id, "weapon_knife" );

	// Fade	into Invisibility

	new	parmCast[3];
	parmCast[0]	= id;
	parmCast[1]	= WINDWALK_PREWALKTIME * 10;

	if ( g_PlayerInfo[id][CURRENT_ITEM]	== ITEM_CLOAK )
		parmCast[2]	= floatround( entity_get_float(	id,	EV_FL_renderamt	) );

	else
	{
		parmCast[2]	= 255;
	}

	OR_U_WINDWALK_in( parmCast );

	return PLUGIN_HANDLED;
}


public OR_U_WINDWALK_in( parmCast[3] ) {

	new	id		   = parmCast[0];
	new	iCounter   = parmCast[1];
	new	iBaseTrans = parmCast[2];

	OR_U_WINDWALK_effects( id, iBaseTrans, iCounter	);

	// Check if	Player Visible

	iCounter -=	1;

	if ( iCounter )
	{
		parmCast[1]	= iCounter;

		new	task = TASK_WINDWALK + id;
		set_task( 0.1, "OR_U_WINDWALK_in", task, parmCast, 3 );
	}

	else
	{
		new	parmWalk[1];
		parmWalk[0]	= id;

		OR_U_WINDWALK_invis( parmWalk );
	}

	return PLUGIN_HANDLED;
}


static OR_U_WINDWALK_effects( id, iBaseTrans, iCounter ) {

	// Fade	Into Invisibility From Current Invis Level

	new	iTransLevel	= WINDWALK_INVISIBILITY	+ (	iCounter * ( ( iBaseTrans -	WINDWALK_INVISIBILITY )	/ (	WINDWALK_PREWALKTIME * 10 )	) );
	set_user_rendering(	id,	kRenderNormal, 0, 0, 0,	kRenderTransTexture, iTransLevel );

	// Stretch View

	new	iStretchAmmount	= 90 + ( ( WINDWALK_FOV	/ (	WINDWALK_PREWALKTIME * 10 )	) *	( (	WINDWALK_PREWALKTIME * 10 )	- (	iCounter ) ) );
	Create_SetFOV( id, iStretchAmmount );

	return PLUGIN_HANDLED;
}


public OR_U_WINDWALK_invis(	parmWalk[1]	) {

	new	id = parmWalk[0];

	// Make	Invisible

	set_user_rendering(	id,	kRenderNormal, 0, 0, 0,	kRenderTransTexture, WINDWALK_INVISIBILITY );

	// Bartime

	Create_BarTime(	id,	WINDWALK_DURATION );

	// Set task	to Make	Visible

	new	parmPost[2];
	parmPost[0]	= id;

	new	task = TASK_WINDWALK + id;

	new	iDuration =	WINDWALK_DURATION;
	new	Float:fDuration	= float( iDuration );

	set_task( fDuration, "OR_U_WINDWALK_out", task,	parmPost, 2	);

	return PLUGIN_HANDLED;
}


public OR_U_WINDWALK_strike( attacker, victim, weapon, headshot	) {

	// Remove Windwalk

	OR_U_WINDWALK_remove( attacker );

	if ( weapon	!= CSW_KNIFE )
		return PLUGIN_HANDLED;

	// Check for Amulet

	if ( g_PlayerInfo[victim][CURRENT_ITEM]	== ITEM_AMULET )
	{
		ITEM_AMULET_block( victim, attacker	);
		return PLUGIN_HANDLED;
	}

	if ( !g_iPlayerAvatar[victim] && victim	!= g_Vip )
	{
		// Deal	Damage

		WAR3_damage( attacker, victim, CSW_WINDWALK, WINDWALK_DAMAGE, headshot,	DAMAGE_NOCHECKARMOR	);

		// Glow	Player

		new	iRGB[3];
		iRGB[GLOW_R] = 255;

		SHARED_GLOW_set( victim, 2.0, iRGB,	36 );

		// Screen Fade

		Create_ScreenFade( victim, (1<<10),	(1<<10), FADE_OUT, 255,	0, 0, 255 );

		// Bloodstream

		new	Origin[3];
		get_user_origin( victim, Origin	);

		Origin[2] += 30;

		for	( new i	= 0; i < 5;	i++	)
		{
			new	Vector[3];
			Vector[0] =	random_num(	-20, 20	);
			Vector[1] =	random_num(	-20, 20	);
			Vector[2] =	random_num(	50,	300	);

			Create_TE_BLOODSTREAM( SHOWTO_ALL_BROADCAST, Origin, Vector, 70, random_num( 100, 200 )	);
		}
	}

	return PLUGIN_HANDLED;
}


public OR_U_WINDWALK_notify( id, iTotalDamage )	{

	new	szMessage[128];
	formatex( szMessage, 127, "%L",	id,	"CAST_WINDWALK", iTotalDamage );

	WAR3_status_text( id, szMessage, 3.0 );

	g_iWindwalkDamage[id] =	0;

	return PLUGIN_HANDLED;
}


public OR_U_WINDWALK_out( parmPost[2] )	{

	new	id = parmPost[0];
	new	iCounter = parmPost[1];

	if ( !iCounter )
	{
		g_bPlayerWalk[id] =	false;

		// Make	sure task is Unset

		new	task = TASK_WINDWALK + id;
		remove_task( task, 0 );

		// Remove Bartime

		Remove_BarTime(	id );

		// Hud Message

		new	szMessage[128];
		formatex( szMessage, 127, "%L",	id,	"FINISH_WINDWALK" );

		WAR3_status_text( id, szMessage, 0.5 );
	}

	iCounter +=	1;

	OR_U_WINDWALK_effects( id, 255,	iCounter );

	// Check if	Player Visible

	if ( iCounter == WINDWALK_PREWALKTIME *	10 )
	{
		OR_U_WINDWALK_remove( id );
	}

	else
	{
		parmPost[1]	= iCounter;

		new	task = TASK_WINDWALK + id;
		set_task( 0.1, "OR_U_WINDWALK_out",	task, parmPost,	2 );
	}

	return PLUGIN_HANDLED;
}


public OR_U_WINDWALK_remove( id	) {

	g_bPlayerWalk[id]  = false;
	g_bPlayerInvis[id] = false;

	new	task = TASK_WINDWALK + id;
	remove_task( task, 0 );

	// Remove Invisiblity

	set_user_rendering(	id );

	// Remove View Stretch

	Remove_SetFOV( id );

	// Cooldown

	Ultimate_Cooldown( id, ULTIMATE_COOLDOWNDEFAULT	);

	// Remove Bartime

	Remove_BarTime(	id );

	// Set Player Speed

	WAR3_set_speed(	id );

	// Invisibility	Cooldown

	SHARED_INVIS_cooldown( id );

	return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. -	//
