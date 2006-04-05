// Begin NIGHTELF.INL


/* - Events	----------------------------------------------------- */


public NE_skills_offensive(	attacker, victim, weapon, iDamage, headshot	) {

	if ( g_PlayerInfo[attacker][CURRENT_RACE] == RACE_NIGHTELF && get_user_team( attacker )	!= get_user_team( victim ) )
	{
		// Trueshot	Aura

		if ( g_PlayerInfo[attacker][CURRENT_SKILL3]	&& weapon != CSW_KNIFE && weapon !=	CSW_HEGRENADE && cs_get_weapon_type_( weapon ) != CS_WEAPON_TYPE_SNIPER	&& get_user_health(	victim ) > 0 )
			NE_S_TRUESHOT( attacker, victim, weapon, iDamage, headshot );
	}

	return PLUGIN_HANDLED;
}


public NE_skills_defensive(	attacker, victim, weapon, iDamage, headshot	) {

	if ( g_PlayerInfo[victim][CURRENT_RACE]	== RACE_NIGHTELF )
	{
		// Elune's Grace ( called from WAR3_damage() as	well to	catch all bonus	damage )

		if ( iDamage &&	weapon == CSW_KNIFE	&& get_user_team( attacker ) !=	get_user_team( victim )	)
		{
			iDamage	-= NE_S_ELUNES_get_physical( victim, weapon, iDamage );
		}

		// Evasion

		if ( g_PlayerInfo[victim][CURRENT_SKILL1] && get_user_health( victim ) > 0 )
		{
			if ( g_bEvadeNextShot[victim] )
				NE_S_EVASION( attacker,	victim,	weapon,	iDamage, headshot );

			NE_S_EVASION_prep( victim );
			NE_S_EVASION_health( victim	);
		}
	}

	return PLUGIN_HANDLED;
}


public NE_ultimates( caster, target	) {

	// Rejuvenation

	if ( g_PlayerInfo[caster][CURRENT_ULTIMATE]	== ULTIMATE_REJUVENATION &&	get_user_team( target )	== get_user_team( caster ) )
	{
		if ( g_PlayerRejuv[target] )
		{
			new	szMessage[128];
			formatex( szMessage, 127, "%L",	caster,	"REJUVINATE_ALREADY" );

			WAR3_status_text( caster, szMessage, 0.5 );

			Ultimate_Beep( caster );
		}

		else
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
				NE_U_REJUVENATION( caster, target );
				Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT	);

				SHARED_INVIS_cooldown( caster );
			}
		}
	}

	// Entangling Roots

	else if	( g_PlayerInfo[caster][CURRENT_ULTIMATE] ==	ULTIMATE_ROOTS && get_user_team( target	) != get_user_team(	caster ) &&	!g_PlayerRooted[target]	)
	{
		NE_U_ROOT( caster, target );
		Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT	);

		SHARED_INVIS_cooldown( caster );
	}

	// Shadowstrike

	else if	( g_PlayerInfo[caster][CURRENT_ULTIMATE] ==	ULTIMATE_SHADOWSTRIKE && get_user_team(	target ) !=	get_user_team( caster )	)
	{
		if ( g_PlayerStruck[target]	)
			NE_U_SHADOWSTRIKE_remove( target );

		NE_U_SHADOWSTRIKE( caster, target );
		Ultimate_Cooldown( caster, ULTIMATE_COOLDOWNDEFAULT	);

		SHARED_INVIS_cooldown( caster );
	}

	else
	{
		Ultimate_Beep( caster );
		return PLUGIN_HANDLED;
	}

	return PLUGIN_HANDLED;
}


/* - Racial	Ability	--------------------------------------------- */


// Elune's Grace

public NE_S_ELUNES_get_physical( id, weapon, iDamage ) {

	if ( get_user_health( id ) > WAR3_get_minhealth( id	) )
	{
		new	iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

		// Set new damage amount

		new	Float:fAbsorb =	float( iDamage ) * NE_S_ELUNES_get_knife( iLevel );
		new	iAbsorb	= floatround( fAbsorb );

		iDamage	-= iAbsorb;
		NE_S_ELUNES_absorb(	id,	iAbsorb	);

		// Give	health back	( physical damage has already been dealt )

		set_user_health( id, get_user_health( id ) + iAbsorb );
		WAR3_check_health( id );

		return ( iAbsorb );
	}

	return ( 0 );
}


public NE_S_ELUNES_get_bonus( id, weapon, iDamage )	{

	new	iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

	// Set new damage amount

	if ( weapon	== CSW_KNIFE )
	{
		new	Float:fAbsorb =	float( iDamage ) * NE_S_ELUNES_get_knife( iLevel );
		new	iAbsorb	= floatround( fAbsorb );

		NE_S_ELUNES_absorb(	id,	iAbsorb	);

		return ( iAbsorb );
	}

	else if	( WAR3_is_ultimate(	weapon ) )
	{
		new	Float:fAbsorb =	float( iDamage ) * NE_S_ELUNES_get_magic( iLevel );
		new	iAbsorb	= floatround( fAbsorb );

		NE_S_ELUNES_absorb(	id,	iAbsorb	);

		return ( iAbsorb );
	}

	return ( 0 );
}


public Float:NE_S_ELUNES_get_knife(	iLevel ) {

	new	Float:fLevel = float( iLevel );

	if ( fLevel	> LEVEL_RACIALCAP )
		fLevel = LEVEL_RACIALCAP;

	new	Float:fAbsorb =	NE_fElunes_knife[0]	+ (	( (	NE_fElunes_knife[1]	- NE_fElunes_knife[0] )	/ LEVEL_RACIALCAP )	* fLevel );

	if ( fAbsorb < NE_fElunes_knife[1] )
		fAbsorb	= NE_fElunes_knife[1];

	// Check if	Night bonus	applies	( coming soon )

	//	if ( g_bIsNighttime	)
	//		fAbsorb	*= ELUNES_NIGHTBONUS;

	return ( fAbsorb );
}


public Float:NE_S_ELUNES_get_magic(	iLevel ) {

	new	Float:fLevel = float( iLevel );

	if ( fLevel	> LEVEL_RACIALCAP )
		fLevel = LEVEL_RACIALCAP;

	new	Float:fAbsorb =	NE_fElunes_magic[0]	+ (	( (	NE_fElunes_magic[1]	- NE_fElunes_magic[0] )	/ LEVEL_RACIALCAP )	* fLevel );

	if ( fAbsorb < NE_fElunes_magic[1] )
		fAbsorb	= NE_fElunes_magic[1];

	// Check if	Night bonus	applies	( coming soon )

	//	if ( g_bIsNighttime	)
	//		fAbsorb	*= ELUNES_NIGHTBONUS;

	return ( fAbsorb );
}


static NE_S_ELUNES_absorb( id, iAbsorbed ) {

	if ( iAbsorbed )
	{
		new	iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

		// Hud Message

		new	szMessage[64];
		formatex( szMessage, 63, "%L", id, "DAMAGE_ELUNE", iAbsorbed );

		WAR3_status_text( id, szMessage, 3.0 );


		new	iFadeAlpha = iAbsorbed * 3;

		if ( iFadeAlpha	> GLOW_MAX )
			iFadeAlpha = GLOW_MAX;

		new	Shell[3];
		for	( new i	= 0; i < 3;	i++	)
		{
			new	Float:fColor = float( ELUNES_SHELL_RGB[i] )	/ LEVEL_RACIALCAP;
			fColor += fColor * float( iLevel );
			new	iColor = floatround( fColor	);

			Shell[i] = iColor;
		}

		// Glow	shell

		SHARED_GLOW_set( id, 0.1, Shell, 48	);

		// Screen Fade

		Create_ScreenFade( id, (1<<10),	(1<<10), FADE_OUT, ELUNES_SHELL_RGB[GLOW_R]	* 2, ELUNES_SHELL_RGB[GLOW_G] *	2, ELUNES_SHELL_RGB[GLOW_B]	* 2, iFadeAlpha	);
	}

	return PLUGIN_HANDLED;
}


/* - Skills	----------------------------------------------------- */


// Nature's	Blessing ( speed )

public Float:NE_S_BLESSING_get_speed( id ) {

	new	weapon,	iClip, iAmmo;
	weapon = get_user_weapon( id, iClip, iAmmo );

	new	Float:fSpeed = CS_WEAPON_SPEED[weapon];

	if ( g_bPlayerZoomed[id] )
		fSpeed = CS_WEAPON_SPEED_ZOOM[weapon];

	new	Float:fBlessingSpeed = fSpeed +	( fSpeed * NE_fBlessing_speed[g_PlayerInfo[id][CURRENT_SKILL2] - 1]	);

	// Do not exceed cap

	if ( fBlessingSpeed	> BLESSING_MAXSPEED	)
		fBlessingSpeed = BLESSING_MAXSPEED;

	return ( fBlessingSpeed	);
}


public NE_S_BLESSING_set_speed(	id ) {

	// Check if	restricted

	if ( !WAR3_skill_enabled( id, RACE_NIGHTELF, SKILL_2 ) )
		return PLUGIN_HANDLED;

	new	Float:fBlessingSpeed = NE_S_BLESSING_get_speed(	id );
	set_user_maxspeed( id, fBlessingSpeed );

	return PLUGIN_HANDLED;
}


public NE_S_BLESSING_set_armor(	id ) {

	// coming soon..

	return PLUGIN_HANDLED;
}


// Trueshot	Aura

static NE_S_TRUESHOT( attacker,	victim,	weapon,	iDamage, headshot )	{

	// Check if	restricted

	if ( !WAR3_skill_enabled( attacker,	RACE_NIGHTELF, SKILL_3 ) )
		return PLUGIN_HANDLED;

	new	Float:fBonusDamage = float(	iDamage	) *	NE_fTrueshotAura[g_PlayerInfo[attacker][CURRENT_SKILL3]	- 1];
	new	iBonusDamage = floatround( fBonusDamage	);

	// Add to player stats array

	if ( get_cvar_num( "mp_war3stats" )	)
	{
		playerSkill3Info[attacker][0] += iBonusDamage;
	}

	// Apply Damage

	if ( iBonusDamage >	0 )
		WAR3_damage( attacker, victim, weapon, iBonusDamage, headshot, DAMAGE_CHECKARMOR );

	return PLUGIN_HANDLED;
}


// Evasion

static NE_S_EVASION( attacker, victim, weapon, iDamage,	headshot ) {

	// Player dies to team attack /	fall with evade

	if ( get_user_health( victim ) <= 1024 && get_user_team( attacker )	== get_user_team( victim ) )
		WAR3_death(	attacker, victim, weapon, headshot );

	// Attacked	by Wind	Walker

	else if	( get_user_health( victim )	<= 1024	&& g_bPlayerWalk[attacker] )
		WAR3_death(	attacker, victim, weapon, headshot );

	// Player takes	grenade	/ knife	damage ( not evadeable )

	else if	( weapon ==	CSW_KNIFE || weapon	== CSW_HEGRENADE )
	{
		if ( get_user_health( victim ) <= 1024 )
			WAR3_death(	attacker, victim, weapon, headshot );
	}

	// Evadeable damage

	else if	( get_user_health( victim )	> 0	&& get_user_team( victim ) != get_user_team( attacker )	)
	{
		g_bEvadeNextShot[victim] = false;

		new	iHealth	= get_user_health( victim )	+ iDamage;
		new	iNewHealth = iHealth - 1024;

		set_user_health( victim, iNewHealth	);

		g_fEvasionTime[victim] = get_gametime();
		g_iEvasionDamage[victim] +=	iDamage;

		// Decrease	damage from	attacker for kill assist

		g_KillAssist_iTotalDamage[attacker][victim]	-= iDamage;

		// Inform player

		new	szMessage[128];
		formatex( szMessage, 127, "%L",	victim,	"DAMAGE_EVASION", iDamage );

		WAR3_status_text( victim, szMessage, 3.0 );

		// Set task	to re-check	for	evasion

		new	parm_Evasion[1];
		parm_Evasion[0]	= victim;

		new	task = TASK_EVASION	+ victim;
		set_task( EVASION_SHOTGAP, "NE_S_EVASION_cooldown",	task, parm_Evasion,	1 );

		// Add to player stats array

		if ( get_cvar_num( "mp_war3stats" )	)
		{
			playerSkill1Info[victim][0]	+= iDamage;
		}

		new	iFadeAlpha = iDamage;

		if ( iFadeAlpha	> GLOW_MAX )
			iFadeAlpha = GLOW_MAX;

		// Glow

		new	iRGB[3];
		iRGB[GLOW_B] = iFadeAlpha;

		SHARED_GLOW_set( victim, glow_duration(	iFadeAlpha ), iRGB,	36 );

		// Screen Fade

		Create_ScreenFade( victim, (1<<10),	(1<<10), FADE_OUT, 0, 0, 255, iFadeAlpha );
	}

	return PLUGIN_HANDLED;
}


public NE_S_EVASION_prep( id ) {

	if ( !WAR3_skill_enabled( id, RACE_NIGHTELF, SKILL_1 ) || g_iEvasionDamage[id] >= EVASION_MAXDAMAGE	)
	{
		g_fEvasionTime[id] = get_gametime()	+ 1000.0;
		return PLUGIN_HANDLED;
	}

	// Don't prep for evasion if EVASION_SHOTGAP not met

	if ( get_gametime()	- g_fEvasionTime[id] < EVASION_SHOTGAP )
		return PLUGIN_HANDLED;

	new	Float:fEvadeChance = NE_fEvasion[g_PlayerInfo[id][CURRENT_SKILL1] -	1];
	new	Float:fRandomNum = random_float( 0.0, 1.0 );

	if ( fEvadeChance >= fRandomNum	)
		g_bEvadeNextShot[id] = true;

	return PLUGIN_HANDLED;
}


public NE_S_EVASION_cooldown( parm_Evasion[1] )	{

	new	id = parm_Evasion[0];

	g_fEvasionTime[id] = 0.0;

	NE_S_EVASION_prep( id );
	NE_S_EVASION_health( id	);

	return PLUGIN_HANDLED;
}


public NE_S_EVASION_health(	id ) {

	new	iMaxHealth = 100;

	// Any health bonus	checks go here

	if ( g_bEvadeNextShot[id] && get_user_health( id ) <= iMaxHealth )
	{
		new	iNewHealth = get_user_health( id ) + 1024;
		set_user_health( id, iNewHealth	);
	}

	else if	( !g_bEvadeNextShot[id]	&& get_user_health(	id ) > 1024	)
	{
		new	iNewHealth = get_user_health( id ) - 1024;
		set_user_health( id, iNewHealth	);
	}

	return PLUGIN_HANDLED;
}



/* - Entangling	Roots ------------------------------------------- */


static NE_U_ROOT( caster, target ) {

	g_PlayerRooted[target] = caster;

	// Display message ( to	target )

	new	szMessage[128],	szPlayerName[32];
	get_user_name( caster, szPlayerName, 31	);

	formatex( szMessage, 127, "%L",	target,	"CAST_ROOTS", szPlayerName );

	WAR3_status_text( target, szMessage, 3.0 );

	// Trail effect

	Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST,	target,	SPR_BEAMFOLLOW,	10,	3, 10, 108,	23,	255	);

	new	parm[1];
	parm[0]	= target;

	NE_U_ROOT_immobilize( parm );

	return PLUGIN_HANDLED;
}


public NE_U_ROOT_immobilize( parmRoot[1] ) {

	new	target = parmRoot[0];

	if ( get_entity_flags( target )	& FL_ONGROUND )
	{
		// Effect(s)

		NE_U_ROOT_effects( target );

		// Remove Trail

		Remove_TE_BEAMFOLLOW( SEENBY_ALL, target );

		// Check for Amulet

		if ( g_PlayerInfo[target][CURRENT_ITEM]	== ITEM_AMULET )
		{
			ITEM_AMULET_block( target, g_PlayerRooted[target] );
			g_PlayerRooted[target] = 0;

			return PLUGIN_HANDLED;
		}

		set_entity_velocity( target, {0,0,0} );

		g_bPlayerCantMove[target] =	true;

		WAR3_set_speed(	target );

		Icon_DispellMe(	target );

		new	Float:fDuration	= ROOT_DURATION;
		new	iCounter = floatround( fDuration );

		new	parm_Dot[2];
		parm_Dot[0]	= target;
		parm_Dot[1]	= iCounter;

		new	task = TASK_ROOT + target;
		set_task( 1.0, "NE_U_ROOT_dot",	task, parm_Dot,	2 );
	}

	else
	{
		new	task = TASK_ROOT + target;
		set_task( 0.1, "NE_U_ROOT_immobilize", task, parmRoot, 1 );
	}

	return PLUGIN_HANDLED;
}


public NE_U_ROOT_dot( parm[2] )	{

	new	target = parm[0];
	new	caster = g_PlayerRooted[target];
	new	iCounter = parm[1];

	new	Float:fDuration	= ROOT_DURATION;
	new	iDuration =	floatround(	fDuration );

	new	iDamage	= (	ROOT_MAXDAMAGE / iDuration );

	// Blood decals

	static const BLOOD_SMALL[7]	= {190,191,192,193,194,195,197};

	new	Origin[3];
	get_user_origin( target, Origin	);

	Origin[0] += random_num( -100,100 );
	Origin[1] += random_num( -100,100 );
	Origin[2] -= 36;

	Create_TE_WORLDDECAL( SHOWTO_ALL_BROADCAST,	Origin,	BLOOD_SMALL[random_num(0,6)] );

	// Apply Damage

	WAR3_damage( caster, target, CSW_ROOT, iDamage,	CS_HEADSHOT_NO,	DAMAGE_NOCHECKARMOR	);

	if ( is_user_alive(	target ) )
	{
		// Poison Damage

		Create_Damage( target, 0, 0, CS_DMG_POISON );

		// Screen Fade

		if ( !g_bPlayerSleeping[target]	)
		{
			Create_ScreenFade( target, (1<<10),	(1<<10), FADE_OUT, 10, 108,	23,	20 );
		}

		iCounter--;

		if ( iCounter )
		{
			parm[1]	= iCounter;

			new	task = TASK_ROOT + target;
			set_task( 1.0, "NE_U_ROOT_dot",	task, parm,	2 );
		}

		else
		{
			// Play	Client-Only	Soundfile

			client_cmd(	target,	"speak warcraft3/bonus/EntanglingRootsDecay1.wav" );

			NE_U_ROOT_remove( target );
		}
	}

	return PLUGIN_HANDLED;
}


static NE_U_ROOT_effects( target ) {

	new	x1,	y1,	x2,	y2;
	new	iRadius	= 20, iCounter = 0;

	new	Float:fDuration	= ROOT_DURATION;
	new	iDuration =	floatround(	fDuration );

	new	TargetOrigin[3];
	get_user_origin( target, TargetOrigin );

	// Play	Sound

	emit_sound(	target,	CHAN_STATIC, SOUND_ROOTS, 1.0, ATTN_NORM, 0, PITCH_NORM	);

	// Draw	Coil

	while (	iCounter <=	7 )
	{
		switch ( iCounter )
		{
			case 0,8:	x1 = -iRadius;
			case 1,7:	x1 = -iRadius *	100	/ 141;
			case 2,6:	x1 = 0;
			case 3,5:	x1 = iRadius * 100 / 141;
			case 4:		x1 = iRadius;
		}

		if ( iCounter <= 4 )
			y1 = sqrt( iRadius * iRadius - x1 *	x1 );

		else
			y1 = sqrt( iRadius * iRadius - x1 *	x1 ) * -1;


		iCounter++;

		switch ( iCounter )
		{
			case 0,8:	x2 = -iRadius;
			case 1,7:	x2 = -iRadius *	100	/ 141;
			case 2,6:	x2 = 0;
			case 3,5:	x2 = iRadius * 100 / 141;
			case 4:		x2 = iRadius;
		}

		if ( iCounter <= 4 )
			y2 = sqrt( iRadius * iRadius - x2 *	x2 );

		else
			y2 = sqrt( iRadius * iRadius - x2 *	x2 ) * -1;


		new	iHeight	= 16 + ( 2 * iCounter );

		while (	iHeight	> -40 )
		{
			new	Origin1[3];
			Origin1[0] = TargetOrigin[0] + x1;
			Origin1[1] = TargetOrigin[1] + y1;
			Origin1[2] = TargetOrigin[2] + iHeight;

			new	Origin2[3];
			Origin2[0] = TargetOrigin[0] + x2;
			Origin2[1] = TargetOrigin[1] + y2;
			Origin2[2] = TargetOrigin[2] + iHeight + 2;

			Create_TE_BEAMPOINTS( SHOWTO_ALL_BROADCAST,	Origin1, Origin2, SPR_ROOT,	0, 0, iDuration	* 10, 10, 5, 10, 108, 23, 255, 0 );

			iHeight	-= 16;
		}
	}

	// Plants

	for	( new iPlantNum	= 1; iPlantNum <= 8; iPlantNum++ )
	{
		new	xOffset, yOffset;

		switch ( iPlantNum )
		{
			case 1:		xOffset	= random_num( 10,25	);
			case 2:		xOffset	= random_num( -25,-10 );
			case 3:		yOffset	= random_num( 10,25	);
			case 4:		yOffset	= random_num( -25,-10 );

			case 5:
			{
				xOffset	= random_num( 10,25	);
				yOffset	= random_num( 10,25	);
			}

			case 6:
			{
				xOffset	= random_num( -25,-10 );
				yOffset	= random_num( -25,-10 );
			}

			case 7:
			{
				xOffset	= random_num( 10,25	);
				yOffset	= random_num( -25,-10 );
			}

			case 8:
			{
				xOffset	= random_num( -25,-10 );
				yOffset	= random_num( 10,25	);
			}
		}

		new	plantOrigin[3];
		plantOrigin[0] = TargetOrigin[0] + xOffset;
		plantOrigin[1] = TargetOrigin[1] + yOffset;
		plantOrigin[2] = TargetOrigin[2] - 34;

		if ( get_user_button( target ) & IN_DUCK )
			plantOrigin[2] += 18;

		NE_U_ROOT_effect_plant(	target,	plantOrigin	);
	}

	return PLUGIN_HANDLED;
}


static NE_U_ROOT_effect_plant( target, Origin[3] ) {

	// Select plant	model

	new	szModelName[64];

	switch ( random_num( 1,3 ) )
	{
		case 1:		copy( szModelName, 63, "models/uplant1.mdl"	);
		case 2:		copy( szModelName, 63, "models/uplant2.mdl"	);
		case 3:		copy( szModelName, 63, "models/uplant3.mdl"	);
	}

	new	Float:fOrigin[3];
	fOrigin[0] = float(	Origin[0] );
	fOrigin[1] = float(	Origin[1] );
	fOrigin[2] = float(	Origin[2] );

	new	plant =	WAR3_ENTITY_create(	"ROOT_PLANT", szModelName, fOrigin,	MOVETYPE_TOSS, SOLID_NOT, 0.0 );

	entity_set_edict( plant, EV_ENT_owner, target );

	// Animate

	entity_set_int(	plant, EV_INT_sequence,	0 );
	entity_set_float( plant, EV_FL_frame, random_float(	1.0, 50.0 )	);
	entity_set_float( plant, EV_FL_framerate, random_float(	1.0, 2.0 ) );

	return PLUGIN_HANDLED;
}


public NE_U_ROOT_remove( id	) {

	new	task = TASK_ROOT + id;
	remove_task( task, 0 );

	g_PlayerRooted[id] = 0;
	Remove_TE_BEAMFOLLOW( SEENBY_ALL, id );

	SHARED_IMMOBILIZE_remove( id );
	WAR3_set_speed(	id );

	Icon_DispellMe(	id );

	return PLUGIN_HANDLED;
}


/* - Shadow	Strike ---------------------------------------------- */


static NE_U_SHADOWSTRIKE( caster, target ) {

	// Play	sound

	emit_sound(	target,	CHAN_STATIC, SOUND_SHADOWSTRIKE, 1.0, ATTN_NORM, 0,	PITCH_NORM );

	// Implosion

	new	TargetOrigin[3];
	get_user_origin( target, TargetOrigin );

	Create_TE_IMPLOSION( SHOWTO_ALL_BROADCAST, TargetOrigin, 100, 120, 4 );

	// "Fizzy" explosion

	Create_TE_EXPLODEMODEL(	SHOWTO_ALL_BROADCAST, TargetOrigin,	250, MDL_HORNET, 40, 50	);

	// Check for Amulet

	if ( g_PlayerInfo[target][CURRENT_ITEM]	== ITEM_AMULET )
	{
		ITEM_AMULET_block( target, caster );
		return PLUGIN_HANDLED;
	}

	// Hud message

	new	szPlayerName[32];
	get_user_name( caster, szPlayerName, 31	);

	new	szMessage[128];
	formatex( szMessage, 127, "%L",	target,	"CAST_SHADOWSTRIKE", szPlayerName );

	WAR3_status_text2( target, szMessage, 3.0 );

	// Adjust armor

	if ( get_user_armor( target	) )
	{
		new	iNewArmor =	get_user_armor(	target ) - SHADOWSTRIKE_ARMOR;

		if ( iNewArmor < 0 )
			iNewArmor =	0;

		set_user_armor(	target,	iNewArmor );
	}

	// Apply damage

	WAR3_damage( caster, target, CSW_SHADOWSTRIKE, SHADOWSTRIKE_DAMAGE,	CS_HEADSHOT_NO,	DAMAGE_NOCHECKARMOR	);

	if ( is_user_alive(	target ) )
	{
		// Set globals

		g_PlayerStruck[target] = caster;
		g_bPlayerSlowed[target]	= true;

		// Set player speed

		new	Velocity[3];
		get_entity_velocity( target, Velocity );

		Velocity[0]	= 0;
		Velocity[1]	= 0;

		set_entity_velocity( target, Velocity );
		WAR3_set_speed(	target );

		// Set damage over time

		new	parm_Dot[3];
		parm_Dot[0]	= caster;
		parm_Dot[1]	= target;
		parm_Dot[2]	= SHADOWSTRIKE_DURATION;

		new	task = TASK_SHADOWSTRIKE + caster;
		set_task( 1.0, "NE_U_SHADOWSTRIKE_dot",	task, parm_Dot,	3 );

		Icon_DispellMe(	target );
	}

	return PLUGIN_HANDLED;
}


public NE_U_SHADOWSTRIKE_dot( parm_Dot[3] )	{

	new	caster = parm_Dot[0];
	new	target = parm_Dot[1];
	new	iCounter = parm_Dot[2];

	new	iDamage	= SHADOWSTRIKE_DOT / SHADOWSTRIKE_DURATION;

	// Apply Damage

	WAR3_damage( caster, target, CSW_SHADOWSTRIKE, iDamage,	CS_HEADSHOT_NO,	DAMAGE_NOCHECKARMOR	);

	if ( is_user_alive(	target ) )
	{
		// Poison Damage

		Create_Damage( target, 0, 0, CS_DMG_POISON );

		// Screen Fade

		if ( !g_bPlayerSleeping[target]	)
		{
			Create_ScreenFade( target, (1<<10),	(1<<10), FADE_OUT, 255,	255, 0,	20 );
		}

		// Check DOT Counter

		iCounter--;

		if ( iCounter )
		{
			parm_Dot[2]	= iCounter;

			new	task = TASK_SHADOWSTRIKE + caster;
			set_task( 1.0, "NE_U_SHADOWSTRIKE_dot",	task, parm_Dot,	3 );
		}

		else
		{
			NE_U_SHADOWSTRIKE_remove( target );
		}
	}

	return PLUGIN_HANDLED;
}


public NE_U_SHADOWSTRIKE_remove( target	) {

	new	caster = g_PlayerStruck[target];

	new	task = TASK_SHADOWSTRIKE + caster;
	remove_task( task, 0 );

	g_PlayerStruck[target] = 0;

	SHARED_SLOW_remove(	target );
	WAR3_set_speed(	target );

	Icon_DispellMe(	target );

	return PLUGIN_HANDLED;
}


/* - Rejuvenation ----------------------------------------------- */


static NE_U_REJUVENATION( caster, target ) {

	g_PlayerRejuv[target] =	caster;

	new	szMessage[64];

	// Status text

	if ( caster	== target )
	{
		formatex( szMessage, 127, "%L",	caster,	"REJUVINATE_SELF" );
		WAR3_status_text( caster, szMessage, 3.0 );
	}

	else
	{
		new	szCastMessage[64];
		formatex( szCastMessage, 63, "%L", caster, "HEAL_CAST" );

		WAR3_status_text( caster, szCastMessage, 1.0 );

		new	szPlayerName[32];
		get_user_name( caster, szPlayerName, 31	);

		formatex( szMessage, 127, "%L",	target,	"REJUVINATE_TARGET", szPlayerName );
		WAR3_status_text( target, szMessage, 3.0 );
	}

	// Play	sound

	emit_sound(	target,	CHAN_STATIC, SOUND_REJUVENATION, 1.0, ATTN_NORM, 0,	PITCH_NORM );

	new	parmHot[3];
	parmHot[0] = caster;
	parmHot[1] = target;
	parmHot[2] = REJUVENATION_WAVES;

	NE_U_REJUVENATION_hot( parmHot );

	return PLUGIN_HANDLED;
}


public NE_U_REJUVENATION_effects( parm_Effects[2] )	{

	new	target = parm_Effects[0];
	new	iCounter = parm_Effects[1];

	// Flares!

	for	( new i	= 0; i < 4;	i++	)
	{
		new	Float:fTargetOrigin[3];
		entity_get_vector( target, EV_VEC_origin, fTargetOrigin	);

		// Randomize

		fTargetOrigin[0] = random_float( fTargetOrigin[0] -	20.0, fTargetOrigin[0] + 20.0 );
		fTargetOrigin[1] = random_float( fTargetOrigin[1] -	20.0, fTargetOrigin[1] + 20.0 );
		fTargetOrigin[2] = random_float( fTargetOrigin[2] -	20.0, fTargetOrigin[2] + 20.0 );

		// Sprite

		new	szSpriteName[64];

		switch ( random_num( 1,2 ) )
		{
			case 1:		copy( szSpriteName,	63,	"sprites/blueflare2.spr" );
			case 2:		copy( szSpriteName,	63,	"sprites/muz7.spr" );
		}

		new	flare =	WAR3_ENTITY_create(	"REJUV_FLARE", szSpriteName, fTargetOrigin,	MOVETYPE_NOCLIP, SOLID_NOT,	0.4	);

		// Project Upwards

		new	Float:fVelocity[3];
		fVelocity[2] = random_float( 125.0,	175.0 );

		entity_set_vector( flare, EV_VEC_velocity, fVelocity );

		// Create Trail

		Create_TE_BEAMFOLLOW( SHOWTO_ALL_BROADCAST,	flare, SPR_BEAMFOLLOW, 5, random_num( 1,3 ), random_num( 127,255 ),	0, random_num( 127,255 ), 128 );

		// Create Lighting ( on	player )

		Create_TE_ELIGHT( SHOWTO_ALL_BROADCAST,	target,	random_num(	25,100 ), random_num( 127,255 ), 0,	random_num(	127,255	), 5, 5	);

		// Render

		entity_set_float( flare, EV_FL_renderamt, 128.0	);
		entity_set_int(	flare, EV_INT_rendermode, kRenderTransAdd );
		entity_set_int(	flare, EV_INT_renderfx,	kRenderFxNone );

		entity_set_float( flare, EV_FL_scale, random_float(	0.2, 0.6 ) );
	}

	iCounter++;

	if ( iCounter <= 6 )
	{
		parm_Effects[1]	= iCounter;

		new	task = 2000;
		set_task( 0.1, "NE_U_REJUVENATION_effects",	task, parm_Effects,	2 );
	}

	return PLUGIN_HANDLED;
}


public NE_U_REJUVENATION_hot( parmHot[3] ) {

	new	caster = parmHot[0];
	new	target = parmHot[1];
	new	iCounter = parmHot[2];

	// Adjust Health

	new	iWaves = REJUVENATION_WAVES;
	new	iHealAmount	= REJUVENATION_MAXHEALTH;

	new	Float:fBonusHealth = float(	iHealAmount	) /	float( iWaves );

	if ( caster	== target )
		fBonusHealth *=	SELFHEAL_MODIFIER;

	new	iBonusHealth = floatround( fBonusHealth	);

	// Heal	player

	WAR3_heal( caster, target, iBonusHealth	);

	// Add to total	health array ( for hud message )

	if ( caster	!= target )
		g_iRejuvHealth[caster] += iBonusHealth;

	// Add to player stats array

	if ( get_cvar_num( "mp_war3stats" )	)
	{
		playerSkill2Info[caster][1]	+= iBonusHealth;
	}

	// Generate	Effect(s)

	new	parm_Effects[2];
	parm_Effects[0]	= target;

	NE_U_REJUVENATION_effects( parm_Effects	);


	iCounter--;

	if ( iCounter )
	{
		parmHot[2] = iCounter;

		new	iWaves = REJUVENATION_WAVES;
		new	Float:fRate	= REJUVENATION_DURATION	/ (	float( iWaves )	 - 1.0 );

		new	task = TASK_REJUV +	target;
		set_task( fRate, "NE_U_REJUVENATION_hot", task,	parmHot, 3 );
	}

	else
	{
		NE_U_REJUVENATION_remove( target );
	}

	return PLUGIN_HANDLED;
}


public NE_U_REJUVENATION_remove( target	) {

	new	caster = g_PlayerRejuv[target];

	// Give	support	XP

	XP_Support_Heal( caster, g_iRejuvHealth[caster]	);

	g_iRejuvHealth[caster] = 0;

	// Unset

	g_PlayerRejuv[target] =	0;

	new	task = TASK_REJUV +	target;
	remove_task( task, 0 );

	return PLUGIN_HANDLED;
}


// ------------------------------------------------- End. -	//
