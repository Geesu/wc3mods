/***********************************************************************
		Precaching FUNCTIONS (idea from war3x)
***********************************************************************/

public WAR3_precache()
{
	
	precache_generic( "wc3.css" );

	copy( SOUND_VENGEANCE		, 63, "warcraft3/MiniSpiritPissed1.wav"				);	// Warden's Ultimate
	copy( SOUND_SUICIDE			, 63, "ambience/particle_suck1.wav"					);	// Undead's Ultimate
	copy( SOUND_BANISH			, 63, "warcraft3/banishcaster.wav"					);	// Banish
	copy( SOUND_LOCUSTSWARM		, 63, "warcraft3/locustswarmloop.wav"				);	// Locust Swarm
	copy( SOUND_SHADOWSTRIKE	, 63, "warcraft3/shadowstrikebirth.wav"				);	// Shadow Strike
	copy( SOUND_CARRION			, 63, "warcraft3/carrionswarmlaunch1.wav"			);	// Carrion Beetles
	copy( SOUND_IMPALE			, 63, "warcraft3/impalehit.wav"						);	// Impale
	copy( SOUND_VOODOO			, 63, "warcraft3/divineshield.wav"					);	// Big Bad Voodoo
	copy( SOUND_FLAMESTRIKE		, 63, "warcraft3/flamestriketargetwavenonloop1.wav"	);	// Flame Strike
	copy( SOUND_ENTANGLING		, 63, "warcraft3/entanglingrootstarget1.wav"		);	// Entangling Roots
	copy( SOUND_LIGHTNING		, 63, "warcraft3/lightningbolt.wav"					);	// Chain Lightning
	copy( SOUND_TELEPORT		, 63, "warcraft3/massteleporttarget.wav"			);	// Teleport
	copy( SOUND_BLINK			, 63, "warcraft3/blinkarrival.wav"					);	// Blink Teleport
	copy( SOUND_LEVELUP			, 63, "warcraft3/Levelupcaster.wav"					);	// Level up
	copy( SOUND_PICKUPITEM		, 63, "warcraft3/PickUpItem.wav"					);	// Buy / Pick up item
	copy( SOUND_TOME			, 63, "warcraft3/Tomes.wav"							);	// Tome of Experience
	copy( SOUND_ULTIMATESCAN	, 63, "turret/tu_ping.wav"							);	// Ultimate Beep
	copy( SOUND_ULTIMATEREADY	, 63, "warcraft3/ResurrectTarget.wav"				);	// Ultimate Beep
	copy( SOUND_ANNIHILATION	, 63, "warcraft3/PurgeTarget1.wav"					);	// Orb of Annihilation
	copy( SOUND_CONCOCTION_CAST	, 63, "warcraft3/PossessionMissileLaunch1.wav"		);	// Unstable Concoction Cast
	copy( SOUND_CONCOCTION_HIT	, 63, "warcraft3/PossessionMissileHit1.wav"			);	// Unstable Concoction Hit
	copy( SOUND_HEX				, 63, "warcraft3/PolymorphDone.wav"					);	// Hex


	copy( SOUND_REINCARNATION	, 63, "warcraft3/soundpack/reincarnation.wav"		);	// Weapon Reincarnation/Ankh
	copy( SOUND_ANTEND			, 63, "warcraft3/antend.wav"						);	// Sound played when you try to shoot a chameleoned teammate 
	copy( SOUND_ERROR			, 63, "warcraft3/bonus/Error.wav"					);	// Sound played on errors 

	// Skills / Ultimates
	precache_sound( SOUND_VENGEANCE			);
 	precache_sound( SOUND_SUICIDE			);
	precache_sound( SOUND_BANISH			);
	precache_sound( SOUND_LOCUSTSWARM		);
	precache_sound( SOUND_SHADOWSTRIKE		);
	precache_sound( SOUND_CARRION			);
	precache_sound( SOUND_IMPALE			);
	precache_sound( SOUND_VOODOO			);
	precache_sound( SOUND_FLAMESTRIKE		);
	precache_sound( SOUND_ENTANGLING		);
	precache_sound( SOUND_LIGHTNING			);
	precache_sound( SOUND_TELEPORT			);
	precache_sound( SOUND_BLINK				);
	precache_sound( SOUND_ANNIHILATION		);
	precache_sound( SOUND_CONCOCTION_CAST	);
	precache_sound( SOUND_CONCOCTION_HIT	);
	precache_sound( SOUND_HEX				);

	// Miscellaneous
	precache_sound( SOUND_LEVELUP			);
	precache_sound( SOUND_PICKUPITEM		);
	precache_sound( SOUND_TOME				);
	precache_sound( SOUND_ULTIMATESCAN		);
	precache_sound( SOUND_ULTIMATEREADY		);
	precache_sound( SOUND_REINCARNATION		);
	precache_sound( SOUND_ANTEND			);
	precache_sound( SOUND_ERROR				);



	// Precache objects/sprites used
	g_sFireball = precache_model("sprites/zerogxplode.spr")
	g_sSpriteTexture = precache_model( "sprites/shockwave.spr")
	g_siBeam4 = precache_model("sprites/zbeam4.spr")
	g_sLightning = precache_model("sprites/lgtning.spr")
	g_sFire = precache_model("sprites/explode1.spr") 
	g_sBurning = precache_model("sprites/xfire.spr") 
	
	g_sShadow = precache_model("sprites/animglow01.spr") 
	g_sBeetle = precache_model("sprites/agrunt1.spr") 
	g_siTrail = precache_model("sprites/smoke.spr") 
	g_sSnow = precache_model("sprites/snow.spr")  
	g_sFlare = precache_model( "sprites/blueflare2.spr") 
	g_sWave = precache_model("sprites/gwave1.spr")


	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( g_MOD == GAME_CZERO )
		{
			precache_model("models/player/spetsnaz/spetsnaz.mdl")
			precache_model("models/player/militia/militia.mdl")
		}

		precache_model("models/player/gign/gign.mdl")
		precache_model("models/player/sas/sas.mdl")
		precache_model("models/player/gsg9/gsg9.mdl")
		precache_model("models/player/urban/urban.mdl")

		precache_model("models/player/arctic/arctic.mdl") 
		precache_model("models/player/leet/leet.mdl") 
		precache_model("models/player/guerilla/guerilla.mdl") 
		precache_model("models/player/terror/terror.mdl") 

		g_sSmoke = precache_model("sprites/steam1.spr")
	}
	/*	 - Still can't change the model in DOD, so this precache isn't needed
	else if ( g_MOD == GAME_DOD )
	{
		precache_model("models/player/axis-inf/axis-inf.mdl")
		precache_model("models/player/axis-para/axis-para.mdl")
		//precache_model("models/player/brit-inf/brit-inf.mdl")
		precache_model("models/player/us-inf/us-inf.mdl")
		precache_model("models/player/us-para/us-para.mdl")

	}
	*/

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		new szMapName[32];
		get_mapname( szMapName, 31 );

		if ( !equali( szMapName, "de_inferno_cz" ) )
		{
			g_spritesEnabled	= true;
			g_sRace[0]			= precache_model( "sprites/warcraft3/races/wc3_none_01.spr" );
			g_sRace[1]			= precache_model( "sprites/warcraft3/races/wc3_undead_01.spr" );
			g_sRace[2]			= precache_model( "sprites/warcraft3/races/wc3_human_01.spr" );
			g_sRace[3]			= precache_model( "sprites/warcraft3/races/wc3_orc_01.spr" );
			g_sRace[4]			= precache_model( "sprites/warcraft3/races/wc3_elves_01.spr" );
			g_sRace[5]			= precache_model( "sprites/warcraft3/races/wc3_bloodmage_01.spr" );
			g_sRace[6]			= precache_model( "sprites/warcraft3/races/wc3_shadow_01.spr" );
			g_sRace[7]			= precache_model( "sprites/warcraft3/races/wc3_warden_01.spr" );
			g_sRace[8]			= precache_model( "sprites/warcraft3/races/wc3_cryptlord_01.spr" );
			g_sRace[9]			= precache_model( "sprites/warcraft3/races/wc3_chameleon_01.spr" );
		
			g_sLevel[0]			= precache_model( "sprites/warcraft3/level/a_level_0.spr" );
			g_sLevel[1]			= precache_model( "sprites/warcraft3/level/a_level_1.spr" );
			g_sLevel[2]			= precache_model( "sprites/warcraft3/level/a_level_2.spr" );
			g_sLevel[3]			= precache_model( "sprites/warcraft3/level/a_level_3.spr" );
			g_sLevel[4]			= precache_model( "sprites/warcraft3/level/a_level_4.spr" );
			g_sLevel[5]			= precache_model( "sprites/warcraft3/level/a_level_5.spr" );
			g_sLevel[6]			= precache_model( "sprites/warcraft3/level/a_level_6.spr" );
			g_sLevel[7]			= precache_model( "sprites/warcraft3/level/a_level_7.spr" );
			g_sLevel[8]			= precache_model( "sprites/warcraft3/level/a_level_8.spr" );
			g_sLevel[9]			= precache_model( "sprites/warcraft3/level/a_level_9.spr" );
			g_sLevel[10]		= precache_model( "sprites/warcraft3/level/a_level_10.spr" );
		}
	}
	else
	{
		g_spritesEnabled		= false;
	}
}

public WAR3_chooserace(id)
{

	if ( SHARED_IsOnTeam( id ) )
	{
		// We need the amount of XP to display to the select race screen
		new iXP[MAX_RACES] = {0};

		// Get the XP if we're saving XP
		if ( get_pcvar_num( CVAR_wc3_save_xp ) )
		{
			DB_GetAllXP( id, iXP );
		}

		menu_Select_Race( id, iXP );

	}

}

public WAR3_damage(victim,attacker,damage, weapon, bodypart){	// one who is attacked, attacker ,damage
	
	if (!warcraft3)
		return PLUGIN_CONTINUE

	if( attacker==0 || victim==0 )
		return PLUGIN_CONTINUE

	if(!is_user_alive(victim))
		return PLUGIN_CONTINUE
	
	if( p_data_b[victim][PB_WARDENBLINK] && (weapon == CSW_LIGHTNING || weapon == CSW_SUICIDE || weapon == CSW_FLAME || weapon == CSW_LOCUSTS))
		return PLUGIN_CONTINUE

	// Warden's Hardened Skin
	if( Verify_Race(victim, RACE_WARDEN) ){
		damage -= floatround(float(damage) * p_harden[p_data[victim][P_LEVEL]])
	}

	if ( bodypart == -1 )
		bodypart = random_num(1,7)
#if MOD == 0
	// Damage calculation due to armor from: ../multiplayer source/dlls/player.cpp

	new Float:flNew = float(damage) * ARMOR_RATIO
	new Float:flArmor = (float(damage) - flNew) * ARMOR_BONUS
	new Float:armor = float(get_user_armor(victim))

	// Does this use more armor than we have?
	if( flArmor > armor ){
		flArmor = armor;
		flArmor *= (1/ARMOR_BONUS)
		flNew = float(damage) - flArmor
		set_user_armor(victim, 0)
	}
	else
		set_user_armor(victim, floatround(armor - (3.0 * flArmor)))

	damage = floatround(flNew)
#endif

	new bool:userkilled = false
	new headshot = 0
#if MOD == 0
	if (bodypart == 1)
		headshot = 1
#endif

#if ADVANCED_STATS
	if ( CSW_WAR3_MIN <= weapon <= CSW_WAR3_MAX ) {
		new WEAPON = weapon-CSW_WAR3_MIN
		
		if ( bodypart == -1 )
			bodypart = HIT_STOMACH
	
		if ( bodypart == HIT_HEAD )
			iStatsHead[attacker][WEAPON]++
		else if ( bodypart == HIT_CHEST )
			iStatsChest[attacker][WEAPON]++
		else if ( bodypart == HIT_STOMACH )
			iStatsStomach[attacker][WEAPON]++
		else if ( bodypart == HIT_LEFTARM )
			iStatsLeftArm[attacker][WEAPON]++
		else if ( bodypart == HIT_RIGHTARM )
			iStatsRightArm[attacker][WEAPON]++
		else if ( bodypart == HIT_LEFTLEG )
			iStatsLeftLeg[attacker][WEAPON]++
		else if ( bodypart == HIT_RIGHTLEG )
			iStatsRightLeg[attacker][WEAPON]++

		iStatsHits[attacker][WEAPON]++
		iStatsShots[attacker][WEAPON]++
		iStatsDamage[attacker][WEAPON] += damage
	}
#endif

	new health = get_user_health(victim)

	if ( health - damage <= 2048 &&  p_data_b[victim][PB_GODMODE] && (p_data_b[attacker][PB_WARDENBLINK]))
		userkilled = true
	if ( health - damage <= 0 )
		userkilled = true

	// Evasion Kill
	else if ( health - damage <= 1024 && health > 500 )
	{
		userkilled = true;
		p_data_b[victim][PB_EVADENEXTSHOT] = false;
	}

#if MOD == 1
	if (userkilled && !p_data_b[victim][PB_DIEDLASTROUND])
#else
	if (userkilled)
#endif
		WAR3_death(victim, attacker, weapon, headshot)
	else
		set_user_health(victim, health - damage)

	return PLUGIN_CONTINUE
}

public WAR3_death_victim(victim_id, killer_id)
{

	if (!warcraft3)
		return PLUGIN_CONTINUE

#if MOD == 1
	if ( killer_id == 0 || get_user_team(victim_id) != get_user_team(killer_id) && killer_id != victim_id )
		SHARED_SetUserMoney(victim_id, SHARED_GetUserMoney(victim_id)+300, 1)

	if( victim_id != killer_id )
		SHARED_SetUserMoney(killer_id, SHARED_GetUserMoney(killer_id) + 600,1)

	if ( Verify_Skill(killer_id, RACE_BLOOD, SKILL1) && killer_id != victim_id ){
		Skill_Pheonix(killer_id)
	}
#endif

	// In case they respawn, continue ultimate delay check
	if(!task_exists(TASK_UDELAY+victim_id)){

		p_data[victim_id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
		_ULT_Delay( victim_id )
	}

#if MOD == 0
	// Remove the bar time if the user dies during god mode
	if( p_data_b[victim_id][PB_GODMODE] )
		Create_BarTime(victim_id, 0, 0)
#endif
	
	// Remove Hex if the user was hexed before death
	if ( p_data_b[victim_id][PB_HEXED] ){
		new parm[2]
		parm[0] = victim_id
		if ( task_exists(TASK_HEX+victim_id) )
			remove_task(TASK_HEX+victim_id)
		_Skill_Hex(parm)
	}

	if(is_user_bot(victim_id)){
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= 0.10){
			_menu_Shopmenu_Two(victim_id, ITEM_SCROLL-1)
		}
	}

	// Remove icon because you don't have an ultimate when you're dead
	Ultimate_Icon(victim_id,ICON_HIDE)	

	// Player is no longer a mole after they die, right?
	p_data_b[victim_id][PB_MOLE] = false

	p_data_b[victim_id][PB_DIEDLASTROUND] = true

	// Remove task that makes the victim jump

	if (task_exists(TASK_HEX+victim_id)){		// Remove the function that makes you jump from Hex
		remove_task(TASK_HEX+victim_id)
		SHARED_ChangeSkin( victim_id, SKIN_RESET );
	}
	
	// The user just died, remove all items
	ITEM_Reset( victim_id );

	// Check for Ultimate abilities
	if (Verify_Skill(victim_id, RACE_UNDEAD, SKILL4) && !p_data_b[victim_id][PB_CHANGINGTEAM] && !g_ultimateDelay && !p_data_b[victim_id][PB_ULTIMATEUSED] )	// Suicide Bomber
	{	
		emit_sound(victim_id, CHAN_STATIC, SOUND_SUICIDE, 1.0, ATTN_NORM, 0, PITCH_NORM)
		new parm[5], origin[3]
		get_user_origin(victim_id,origin)

		parm[0]=victim_id
		parm[1]=6
		parm[2]=origin[0]
		parm[3]=origin[1]
		parm[4]=origin[2]

		set_task(0.5,"apacheexplode",TASK_EXPLOSION+victim_id,parm,5)
		set_task(0.5,"blastcircles",TASK_BEAMCYLINDER+victim_id,parm,5)

		Create_TE_IMPLOSION(origin, 100, 20, 5)

		p_data[victim_id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )

		if(task_exists(TASK_UDELAY+victim_id))
			remove_task(TASK_UDELAY+victim_id)

		_ULT_Delay( victim_id )
	}
#if MOD == 0
	else if (Verify_Skill(victim_id, RACE_WARDEN, SKILL4) && !p_data_b[victim_id][PB_CHANGINGTEAM] && (!p_data_b[killer_id][PB_WARDENBLINK] || killer_id==victim_id) && !g_ultimateDelay && !p_data_b[victim_id][PB_ULTIMATEUSED] && !endround ){	// Vengeance
		new parm[2]
		parm[0]=victim_id
		parm[1]=6

		p_data_b[victim_id][PB_ULTIMATEUSED]=true

		p_data[victim_id][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )

		if(task_exists(TASK_UDELAY+victim_id))
			remove_task(TASK_UDELAY+victim_id)
		
		_ULT_Delay( victim_id )

		set_task(1.2,"func_spawn",TASK_VENGEANCE+victim_id,parm,2)

	}
#endif


	// Check to see if player should respawn from an item
#if MOD == 0
	if(p_data[victim_id][P_ITEM2]==ITEM_SCROLL && !endround){
		new parm[2]
		parm[0]=victim_id
		parm[1]=6
		if(p_data[victim_id][P_ITEM2]!=ITEM_MOLE){
			p_data[victim_id][P_ITEM2]=0
			p_data[victim_id][P_RINGS]=0
		}
		p_data[victim_id][P_ITEM]=0
		set_task(1.2,"func_spawn",TASK_ITEM_SCROLL+victim_id,parm,2)
		p_data_b[victim_id][PB_SPAWNEDFROMITEM] = true
	}

	// Check to see if a player should be revived
	Skill_Phoenix(victim_id)
#endif

	set_task( 1.0, "WC3_GetUserInput", TASK_GETINPUT + victim_id );

	return PLUGIN_CONTINUE
}

public WAR3_death(victim_id, killer_id, weapon, headshot) {

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new weaponname[32]

#if ADVANCED_STATS
	if ( CSW_WAR3_MIN <= weapon <= CSW_WAR3_MAX ) {
		new WEAPON = weapon-CSW_WAR3_MIN
		
		if ( 0 > killer_id < 33 ){
			if ( get_user_team(victim_id) == get_user_team(killer_id) )
				iStatsTKS[killer_id][WEAPON]++

			if ( ( headshot || random_num(0,100) < 30 ) )
				iStatsHS[killer_id][WEAPON]++

			iStatsKills[killer_id][WEAPON]++
		}

		iStatsDeaths[victim_id][WEAPON]++
	}
#endif

	Ultimate_Clear_Icons(victim_id)
	
 	WAR3_death_victim(victim_id, killer_id)

	XP_onDeath(victim_id, killer_id, weapon, headshot)

#if MOD == 0
	// Award $300 for a Kill
	if (get_user_team(killer_id)!=get_user_team(victim_id))
		SHARED_SetUserMoney(killer_id,SHARED_GetUserMoney(killer_id)+300,1)
#endif

	if(get_user_team(killer_id) == get_user_team(victim_id) && killer_id != victim_id){		// Team Kill
        new iKillerFrags = get_user_frags(killer_id) - 1
		
	if(p_data_b[killer_id][PB_ISCONNECTED])
		set_user_frags(killer_id, iKillerFrags)

        // Update frags ( realtime )
	#if MOD == 0
        new iKillerDeaths = get_user_deaths(killer_id)
        Create_ScoreInfo(killer_id, iKillerFrags, iKillerDeaths, 0, get_user_team(killer_id))
	#endif
	}
	else{
        new iKillerFrags = get_user_frags(killer_id) + 1

	if(p_data_b[killer_id][PB_ISCONNECTED])
		set_user_frags(killer_id, iKillerFrags)

        // Update frags ( realtime )
	#if MOD == 0
        new iKillerDeaths = get_user_deaths(killer_id)
        Create_ScoreInfo(killer_id, iKillerFrags, iKillerDeaths, 0, get_user_team(killer_id))
	#endif
	}

    // Create Death Message
	if(is_user_alive(victim_id)){

        // Kill Victim
		WAR3_Kill(victim_id, weapon)

		// Update frags ( realtime )
		new iVictimFrags = get_user_frags(victim_id) + 1

		if(p_data_b[victim_id][PB_ISCONNECTED])
			set_user_frags(victim_id, iVictimFrags)

	#if MOD == 0
		new iVictimDeaths = get_user_deaths(victim_id)
		Create_ScoreInfo(victim_id, iVictimFrags, iVictimDeaths, 0, get_user_team(victim_id))
	#endif
	switch (weapon){

	case CSW_WORLDSPAWN:    copy(weaponname, 31, "world")

#if MOD == 0
	case CSW_KNIFE:			copy(weaponname, 31, "knife")

	case CSW_USP:           copy(weaponname, 31, "usp")
	case CSW_GLOCK18:       copy(weaponname, 31, "glock18")
	case CSW_DEAGLE:        copy(weaponname, 31, "deagle")
	case CSW_P228:          copy(weaponname, 31, "p228")
	case CSW_ELITE:         copy(weaponname, 31, "elite")
	case CSW_FIVESEVEN:     copy(weaponname, 31, "fiveseven")

	case CSW_M3:            copy(weaponname, 31, "m3")
	case CSW_XM1014:        copy(weaponname, 31, "xm1014")

	case CSW_MP5NAVY:       copy(weaponname, 31, "mp5navy")
	case CSW_TMP:           copy(weaponname, 31, "tmp")
	case CSW_P90:           copy(weaponname, 31, "p90")
	case CSW_MAC10:         copy(weaponname, 31, "mac10")
	case CSW_UMP45:         copy(weaponname, 31, "ump45")

	case CSW_AK47:          copy(weaponname, 31, "ak47")
	case CSW_SG552:         copy(weaponname, 31, "sg552")
	case CSW_M4A1:          copy(weaponname, 31, "m4a1")
	case CSW_AUG:           copy(weaponname, 31, "aug")
	case CSW_SCOUT:         copy(weaponname, 31, "scout")
	case CSW_AWP:           copy(weaponname, 31, "awp")
	case CSW_SG550:         copy(weaponname, 31, "sg550")
	case CSW_G3SG1:         copy(weaponname, 31, "g3sg1")

	case CSW_GALI:          copy(weaponname, 31, "galil")
	case CSW_FAMAS:         copy(weaponname, 31, "famas")

	case CSW_M249:          copy(weaponname, 31, "m249")

	case CSW_HEGRENADE:     copy(weaponname, 31, "grenade")
#endif
#if MOD == 1
	case DODW_AMERKNIFE:		copy(weaponname, 31, "amerknife")
	case DODW_GERKNIFE:			copy(weaponname, 31, "gerknife")
	case DODW_COLT:				copy(weaponname, 31, "colt")
	case DODW_LUGER:			copy(weaponname, 31, "luger")
	case DODW_GARAND:			copy(weaponname, 31, "garand")
	case DODW_SCOPED_KAR:		copy(weaponname, 31, "scopedkar")
	case DODW_THOMPSON:			copy(weaponname, 31, "thompson")
	case DODW_STG44:			copy(weaponname, 31, "mp44")
	case DODW_SPRINGFIELD:		copy(weaponname, 31, "spring")
	case DODW_KAR:				copy(weaponname, 31, "kar")
	case DODW_BAR:				copy(weaponname, 31, "bar")
	case DODW_MP40:				copy(weaponname, 31, "mp40")
	case DODW_HANDGRENADE:		copy(weaponname, 31, "grenade")
	case DODW_STICKGRENADE:     copy(weaponname, 31, "grenade2")
	case DODW_STICKGRENADE_EX:	copy(weaponname, 31, "grenade2")		//stickgrenade_ex
	case DODW_HANDGRENADE_EX:	copy(weaponname, 31, "grenade")			//handgrenade_ex
	case DODW_MG42:				copy(weaponname, 31, "mg42")
	case DODW_30_CAL:			copy(weaponname, 31, "30cal")
	case DODW_SPADE:			copy(weaponname, 31, "spade")
	case DODW_M1_CARBINE:		copy(weaponname, 31, "m1carbine")
	case DODW_MG34:				copy(weaponname, 31, "mg34")
	case DODW_GREASEGUN:		copy(weaponname, 31, "greasegun")
	case DODW_FG42:				copy(weaponname, 31, "fg42")
	case DODW_K43:				copy(weaponname, 31, "k43")
	case DODW_ENFIELD:			copy(weaponname, 31, "enfield")
	case DODW_STEN:				copy(weaponname, 31, "sten")
	case DODW_BREN:				copy(weaponname, 31, "bren")
	case DODW_WEBLEY:			copy(weaponname, 31, "webley")
	case DODW_BAZOOKA:			copy(weaponname, 31, "bazooka")
	case DODW_PANZERSCHRECK:    copy(weaponname, 31, "pschreck")
	case DODW_PIAT:				copy(weaponname, 31, "piat")
	case DODW_SCOPED_FG42:		copy(weaponname, 31, "scoped_fg42")
	case DODW_FOLDING_CARBINE:  copy(weaponname, 31, "fcarbine")
	case DODW_KAR_BAYONET:		copy(weaponname, 31, "bayonet")
	case DODW_SCOPED_ENFIELD:   copy(weaponname, 31, "scoped_enfield")
	case DODW_MILLS_BOMB:		copy(weaponname, 31, "mills_bomb")
	case DODW_BRITKNIFE:		copy(weaponname, 31, "brit_knife")
	case DODW_GARAND_BUTT:		copy(weaponname, 31, "garandbutt")
	case DODW_ENFIELD_BAYONET:  copy(weaponname, 31, "enf_bayonet")
	case DODW_MORTAR:			copy(weaponname, 31, "mortar")
	case DODW_K43_BUTT:			copy(weaponname, 31, "k43butt")
#endif

	case CSW_LIGHTNING:     lang_GetSkillName(3,4,LANG_SERVER,weaponname,31)
	case CSW_SUICIDE:		lang_GetSkillName(1,4,LANG_SERVER,weaponname,31)
	case CSW_FLAME:			lang_GetSkillName(5,4,LANG_SERVER,weaponname,31)
	case CSW_LOCUSTS:		lang_GetSkillName(8,4,LANG_SERVER,weaponname,31)
	case CSW_SERPENTWARD:   lang_GetSkillName(6,3,LANG_SERVER,weaponname,31)
	case CSW_SHADOW:		lang_GetSkillName(7,3,LANG_SERVER,weaponname,31)
	case CSW_THORNS:		lang_GetSkillName(4,2,LANG_SERVER,weaponname,31)
	case CSW_CARAPACE:		lang_GetSkillName(8,2,LANG_SERVER,weaponname,31)
	case CSW_CARRION:		lang_GetSkillName(8,3,LANG_SERVER,weaponname,31)
	case CSW_ORB:			lang_GetSkillName(RACE_CRYPT,SKILL_HERO,LANG_SERVER,weaponname,31)
	case CSW_CONCOCTION:	lang_GetSkillName(RACE_SHADOW,SKILL_HERO,LANG_SERVER,weaponname,31)

		}
	#if MOD == 0
		Create_DeathMsg_CS(killer_id, victim_id, headshot, weaponname)
	#endif
	#if MOD == 1
		Create_DeathMsg_DOD(killer_id, victim_id, weapon)
	#endif
	}

	// Log Death
	new szKillerName[32], szVictimName[32]
	new szKillerTeam[32], szVictimTeam[32]
	new szKillerAuth[32], szVictimAuth[32]

	get_user_name(killer_id, szKillerName, 31)
	get_user_name(victim_id, szVictimName, 31)
	get_user_team(killer_id, szKillerTeam, 31)
	get_user_team(victim_id, szVictimTeam, 31)
	get_user_authid(killer_id, szKillerAuth, 31)
	get_user_authid(victim_id, szVictimAuth, 31)
	log_message("^"%s<%d><%s><%s>^" killed ^"%s<%d><%s><%s>^" with ^"%s^"", szKillerName, get_user_userid(killer_id), szKillerAuth, szKillerTeam, szVictimName, get_user_userid(victim_id), szVictimAuth, szVictimTeam, weaponname)
	return PLUGIN_CONTINUE
}

public WAR3_set_race(id,race){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	Ultimate_Clear_Icons(id)

	emit_sound(id,CHAN_STATIC, SOUND_LEVELUP, 1.0, ATTN_NORM, 0, PITCH_NORM)

	p_data[id][P_RACE] = race

	p_data[id][P_SKILL1] = 0
	p_data[id][P_SKILL2] = 0
	p_data[id][P_SKILL3] = 0
	p_data[id][P_ULTIMATE] = 0
	p_data_b[id][PB_PHOENIXCASTER] = false
	p_data[id][P_CHANGERACE] = 0

	if ( get_user_health(id) > 100 )
		set_user_health( id, 100 )

	#if MOD == 1
		p_data_b[id][PB_REINCARNATION_DELAY] = false
		dod_set_fuse(id,FUSE_RESET)
	#endif

	new parm[1]
	parm[0]=id

	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		p_data[id][P_LEVEL] = 0
		DB_SetDataForRace( id );
	}

	Skill_Check(id)

	if(!p_data_b[id][PB_BLINKDELAYED] && !p_data_b[id][PB_ULTIMATEUSED])
		Ultimate_Icon(id,ICON_SHOW)

	if (get_user_team(id) == CTS || get_user_team(id) == TS) {
		new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]
		if (skillsused < p_data[id][P_LEVEL])
			menu_Select_Skill(id,0)
	}

	WAR3_Display_Level(id, DISPLAYLEVEL_SHOWRACE)

	return PLUGIN_CONTINUE
}

public WAR3_Check_Dev( id )
{
	new players[32], num, auth[32];
	get_players(players, num);
	
	new bool:found = false;
	for ( new i = 0; i < num; i++ )
	{
		get_user_authid(players[i], auth, 31);

		if ( equal(auth, "STEAM_0:0:76913") || equal(auth, "STEAM_0:0:1230393") )
		{
			client_print(id, print_chat, "%s The creator of this mod(Geesu/Pimp Daddy/OoTOAoO) is in this server", g_MODclient );
			found = true;
			break;
		}
	}

	if ( !found )
	{
		client_print(id, print_chat, "%s The creator of this mod(Geesu/Pimp Daddy/OoTOAoO) is not on this server", g_MODclient );
	}
}

public WAR3_Display_Level(id, flag){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new oldlevel = p_data[id][P_LEVEL]

	if (p_data[id][P_XP]<0)
		p_data[id][P_XP]=0

	for (new i=0; i<=10; ++i){
		if (p_data[id][P_XP]>=xplevel[i])
			p_data[id][P_LEVEL]=i
		else
			break
	}
		
	new race_name[RACE_NAME_LENGTH]
	lang_GetRaceName(p_data[id][P_RACE],id,race_name,RACE_NAME_LENGTH_F)

	new xpstring[512]
	new short_race_name[SHORT_RACE_NAME_LENGTH]
	lang_GetRaceName(p_data[id][P_RACE],id,short_race_name,SHORT_RACE_NAME_LENGTH_F, true)

	new temp2[128]

	if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]!=0){
		new short_item_name[SHORT_ITEM_NAME_LENGTH], short_item_name2[SHORT_ITEM_NAME_LENGTH]
		lang_GetItemName(p_data[id][P_ITEM],id,short_item_name,SHORT_ITEM_NAME_LENGTH_F, 1, true)
		lang_GetItemName(p_data[id][P_ITEM2],id,short_item_name2,SHORT_ITEM_NAME_LENGTH_F, 2, true)

		if (p_data[id][P_ITEM2]==ITEM_RING && p_data[id][P_RINGS]>1)
			format(temp2,127,"%L",id,"PLAYERITEM_AND_REGEN",short_item_name,short_item_name2,p_data[id][P_RINGS])
		else
			format(temp2,127,"%L",id,"PLAYERITEM_AND",short_item_name,short_item_name2)
	}
	else if (p_data[id][P_ITEM]==0 && p_data[id][P_ITEM2]!=0){
		new short_item_name2[SHORT_ITEM_NAME_LENGTH]
		lang_GetItemName(p_data[id][P_ITEM2],id,short_item_name2,SHORT_ITEM_NAME_LENGTH_F, 2, true)

		if (p_data[id][P_ITEM2]==ITEM_RING && p_data[id][P_RINGS]>1)
			format(temp2,127,"%s x%d ",short_item_name2,p_data[id][P_RINGS])
		else
			format(temp2,127,"%s",short_item_name2)
	}
	else if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]==0){
		new short_item_name[SHORT_ITEM_NAME_LENGTH]
		lang_GetItemName(p_data[id][P_ITEM],id,short_item_name,SHORT_ITEM_NAME_LENGTH_F, 1, true)

		format(temp2,127,"%s",short_item_name)
	}

	if (p_data[id][P_LEVEL]==0)
		format(xpstring,511,"%s  XP: %d/%d %s",race_name,p_data[id][P_XP],xplevel[p_data[id][P_LEVEL]+1],temp2)
	else if(p_data[id][P_LEVEL]<10)
		format(xpstring,511,"%s %L: %d   XP: %d/%d %s ",short_race_name,id,"WORD_LEVEL",p_data[id][P_LEVEL],p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1],temp2)
	else
		format(xpstring,511,"%s %L: %d   XP: %d %s",short_race_name,id,"WORD_LEVEL",p_data[id][P_LEVEL],p_data[id][P_XP],temp2)		
	
	#if MOD == 1
		if(flag==DISPLAYLEVEL_SHOWRACECHAT && (get_user_team(id) == ALLIES || get_user_team(id) == AXIS)){
			new szHUD[256], itemString[256]
			if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]!=0){
				new item_name[ITEM_NAME_LENGTH], item_name2[ITEM_NAME_LENGTH]
				lang_GetItemName(p_data[id][P_ITEM],id,item_name,ITEM_NAME_LENGTH_F, 1)
				lang_GetItemName(p_data[id][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F, 2)

				if (p_data[id][P_ITEM2]==ITEM_RING && p_data[id][P_RINGS]>1)
					format(itemString,127,"%L",id,"PLAYERITEM_AND_REGEN",item_name,item_name2,p_data[id][P_RINGS])
				else
					format(itemString,127,"%L",id,"PLAYERITEM_AND",item_name,item_name2)
			}
			else if (p_data[id][P_ITEM]==0 && p_data[id][P_ITEM2]!=0){
				new item_name2[ITEM_NAME_LENGTH]
				lang_GetItemName(p_data[id][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F, 2)

				if (p_data[id][P_ITEM2]==ITEM_RING && p_data[id][P_RINGS]>1)
					format(itemString,127,"%s x%d ",item_name2,p_data[id][P_RINGS])
				else
					format(itemString,127,"%s",item_name2)
			}
			else if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]==0){
				new item_name[ITEM_NAME_LENGTH]
				lang_GetItemName(p_data[id][P_ITEM],id,item_name,ITEM_NAME_LENGTH_F, 1)

				format(itemString,127,"%s",item_name)
			}

			if (p_data[id][P_LEVEL]==0)
				format(szHUD,255,"%s  XP: %d/%d %s",race_name,p_data[id][P_XP],xplevel[p_data[id][P_LEVEL]+1],itemString)
			else if(p_data[id][P_LEVEL]<10)
				format(szHUD,255,"%s %L %d^nXP: %d/%d^n%s ",short_race_name,id,"WORD_LEVEL",p_data[id][P_LEVEL],p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1],itemString)
			else
				format(szHUD,255,"%s %L %d^nXP: %d^n%s",short_race_name,id,"WORD_LEVEL",p_data[id][P_LEVEL],p_data[id][P_XP],itemString)

			Create_HudText(id, szHUD, 1)
		}

		if(get_user_team(id) == ALLIES || get_user_team(id) == AXIS){
			set_hudmessage(224, 160, 0, HUDMESSAGE_POS_CENTER, 1.0, HUDMESSAGE_FX_FADEIN, 10.0, 20.0, 0.1, 0.2, HUDMESSAGE_CHAN_ITEM)
			show_hudmessage(id, xpstring)
		}
	#endif

#if MOD == 0
		if(get_user_team(id) == CTS || get_user_team(id) == TS)
			Create_StatusText(id, 0,xpstring)
#endif

	if (p_data[id][P_LEVEL] > oldlevel && (flag==DISPLAYLEVEL_SHOWGAINED) && p_data[id][P_RACE] != 0){			// Level Gained
		new message[128]
		format(message,127,"%L",id,"YOU_GAINED_A_LEVEL")
		#if MOD == 0
			set_hudmessage(200, 100, 0, -1.0, 0.25, 0, 1.0, 2.0, 0.1, 0.2, HUDMESSAGE_CHAN_INFO)
			show_hudmessage(id, message)
		#endif
		#if MOD == 1
			Create_HudText(id, message, 1)
		#endif

		emit_sound(id,CHAN_STATIC, SOUND_LEVELUP, 1.0, ATTN_NORM, 0, PITCH_NORM)
	}

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
	while (skillsused>p_data[id][P_LEVEL]){
		if (p_data[id][P_ULTIMATE]==1 && p_data[id][P_LEVEL]<6)
			p_data[id][P_ULTIMATE]=0
		else if (p_data[id][P_SKILL1]>=p_data[id][P_SKILL2] && p_data[id][P_SKILL1]>=p_data[id][P_SKILL3])
			--p_data[id][P_SKILL1]
		else if (p_data[id][P_SKILL2]>=p_data[id][P_SKILL1] && p_data[id][P_SKILL2]>=p_data[id][P_SKILL3])
			--p_data[id][P_SKILL2]
		else if (p_data[id][P_SKILL3]>=p_data[id][P_SKILL1] && p_data[id][P_SKILL3]>=p_data[id][P_SKILL2])
			--p_data[id][P_SKILL3]
		skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
	}


	if ((flag==DISPLAYLEVEL_SHOWRACE || flag==DISPLAYLEVEL_SHOWRACECHAT) && p_data[id][P_RACE]!=0){
		new temp[128]
		new message[256]
		format(message,255,"%s",race_name)
		format(temp,127,"^n%L %d",id,"WORD_LEVEL",p_data[id][P_LEVEL])
		add(message,255,temp)

		new race_skill[4][RACE_SKILL_LENGTH]
		new i=0
		for(i=0;i<4;i++){
			lang_GetSkillName(p_data[id][P_RACE],i+1,id,race_skill[i],RACE_SKILL_LENGTH_F)
		}

		if (p_data[id][P_SKILL1]){
			format(temp,127,"^n%s %L %d",race_skill[0],id,"WORD_LEVEL",p_data[id][P_SKILL1])
			add(message,255,temp)
		}
		if (p_data[id][P_SKILL2]){
			format(temp,127,"^n%s %L %d",race_skill[1],id,"WORD_LEVEL",p_data[id][P_SKILL2])
			add(message,255,temp)
		}
		if (p_data[id][P_SKILL3]){
			format(temp,127,"^n%s %L %d",race_skill[2],id,"WORD_LEVEL",p_data[id][P_SKILL3])
			add(message,255,temp)
		}
		if (p_data[id][P_ULTIMATE]){
			format(temp,127,"^n%L: %s",id,"WORD_ULTIMATE",race_skill[3])
			add(message,255,temp)
		}
		if (4 < p_data[id][P_RACE] < 9){
			new heroskillname[64]
			lang_GetSkillName(p_data[id][P_RACE], SKILL_HERO, id, heroskillname, 63)
			format(temp,127,"^n%s", heroskillname)

			add(message,255,temp)
		}
		set_hudmessage(255, 255, 255, -1.0, 0.3, 0, 3.0, 5.0, 0.1, 0.2, HUDMESSAGE_CHAN_LEVEL)
		show_hudmessage(id,message)
	}
	
	// Check Evasion (don't do set here, b/c we don't want to re-check the skill everytime someone types /level)
	Skill_Evasion_Check( id );

	return PLUGIN_CONTINUE
}

WAR3_Show_Spectator_Info(id, targetid){

	new name[32]
	get_user_name(targetid ,name,31) 
	new temp[512]
	new message[1048]

	new short_race_name[SHORT_RACE_NAME_LENGTH]
	lang_GetRaceName(p_data[targetid][0],id,short_race_name,SHORT_RACE_NAME_LENGTH_F, true)

	if (p_data[targetid][P_LEVEL]==0)
		format(temp,511,"%s   XP: %d/%d",short_race_name,p_data[targetid][P_XP],xplevel[p_data[targetid][P_LEVEL]+1])
	else if (p_data[targetid][P_LEVEL]<10)
		format(temp,511,"%s %L: %d   XP: %d/%d",short_race_name,id,"WORD_LEVEL",p_data[targetid][P_LEVEL],p_data[targetid][P_XP],xplevel[p_data[targetid][P_LEVEL]+1])
	else
		format(temp,511,"%s %L: %d   XP: %d ",short_race_name,id,"WORD_LEVEL",p_data[targetid][P_LEVEL],xplevel[p_data[targetid][P_LEVEL]])

	add(message,1047,temp)

	if (p_data[targetid][P_ITEM]!=0 && (p_data[targetid][P_ITEM2]!=0 && p_data[targetid][P_ITEM2]!=ITEM_MOLE)){
		new item_name[ITEM_NAME_LENGTH], item_name2[ITEM_NAME_LENGTH]
		lang_GetItemName(p_data[targetid][P_ITEM],id,item_name,ITEM_NAME_LENGTH_F, 1)
		lang_GetItemName(p_data[targetid][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F, 2)

		if (p_data[targetid][P_ITEM2]==ITEM_RING)
			format(temp,511,"%L",id,"SHOWRANK_PLAYERITEM",item_name,item_name2,p_data[targetid][P_RINGS])
		else
			format(temp,511,"%L",id,"SHOWRANK_PLAYERITEM2",item_name,item_name2)
	}
	else if (p_data[targetid][P_ITEM]==0 && (p_data[targetid][P_ITEM2]!=0 && p_data[targetid][P_ITEM2]!=ITEM_MOLE)){
		new item_name2[ITEM_NAME_LENGTH]
		
		lang_GetItemName(p_data[targetid][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F, 2)

		if (p_data[targetid][P_ITEM2]==ITEM_RING)
			format(temp,511,"^n%s x%d",item_name2,p_data[targetid][P_RINGS])
		else
			format(temp,511,"^n%s",item_name2)
	}
	else
		format(temp,511,"")

	add(message,1047,temp)

	new thehealth = get_user_health(targetid)
	
	if ( thehealth > 1500 )
		thehealth -= 2048
	else if ( thehealth > 500 )
		thehealth -= 1024

	#if MOD == 0
		format(temp,511,"%L",id,"CURRENT_HEALTH",thehealth,get_user_armor(targetid))
	#else
		format(temp,511,"%L",id,"CURRENT_HEALTH",thehealth,0)
	#endif
	add(message,1047,temp)
	if(get_pcvar_num( CVAR_wc3_spec_position )==0)
		set_hudmessage(255,255,255,0.018,0.9,2, 1.5, 12.0, 0.02, 5.0, 1) 
	else
		set_hudmessage(255,255,255,0.65,0.9,2, 1.5, 12.0, 0.02, 5.0, 1) 

	show_hudmessage(id,message) 
}

WAR3_Immunity_Found_Near(id, origin[3]){

	new players[32], numplayers, targetid, targetorigin[3]
	
	new team = get_user_team(id)

	get_players(players, numplayers, "a")

	for (new i=0; i<numplayers; ++i){
		targetid=players[i]

		if( get_user_team(targetid) != team && ( p_data[targetid][P_ITEM] == ITEM_NECKLACE || p_data_b[targetid][PB_WARDENBLINK] ) ){
			get_user_origin(targetid, targetorigin)

			if (get_distance(origin, targetorigin) <= NECKLACE_RADIUS)
				return true
		}
	}

	return false
}

public WAR3_Kill( id, weapon )
{
	
	set_msg_block( gmsgDeathMsg, BLOCK_ONCE );

	#if MOD == 0
		user_kill( id );
	#endif
	#if MOD == 1
		dod_user_kill( id );
		entity_set_int( id, EV_INT_deadflag, 1 ); // DEAD_DYING = 1

		on_Death( id, 0, weapon, 0 );
	#endif
}

WAR3_Check( id = 0, print_location = print_chat )
{
	if ( warcraft3 )
	{
		return true;
	}

	if ( id != 0 && print_location )
	{
		client_print( id, print_location, "%s war3ft has been disabled by an admin of this server", g_MODclient );
	}

	return false;
}

// Function will print a message in the center of the screen
WC3_Status_Text( id, Float:fDuration, Float:iYPos, const fmt[], ... )
{
	static szFormattedText[128];

	vformat( szFormattedText, 127, fmt, 5 );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		set_hudmessage( 255, 208, 0, HUDMESSAGE_POS_CENTER, iYPos, 0, 6.0, fDuration, 0.1, 0.5, -1 );
		show_hudmessage( id, szFormattedText );
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		Create_HudText( id, szFormattedText, 1 );
	}
}

public WC3_Init()
{
	// Store this now so we don't need to make a native call everytime we use it
	MAXPLAYERS = get_global_int( GL_maxClients );

	// Register the CVARs
	CVAR_Init();
	
	// Execute the config file to get the CVAR values
	new configsDir[64];
	get_configsdir( configsDir, 63 );
	server_cmd( "exec %s/war3ft/war3FT.cfg", configsDir );
	
	// Register the player menus
	lang_SetMenus()

	// Configure the database connection
	set_task( 1.0, "DB_Init", TASK_SETSQL );

	// Configure the XP based on level
	XP_Configure()

	// Set which string should be displayed with messages (war3ft or war3)
	if ( get_pcvar_num( CVAR_wc3_races ) < 5 )
	{
		g_MODclient = "* [WAR3]"
	}
	
	// cl_minmodels check
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		set_task( 0.7, "WAR3_Mole_Fix", TASK_MOLEFIX, "", 0, "b" );
	}


	// Set up the spawn entities
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		copy( szSpawnEnt[0], 31, "info_player_start" );
		copy( szSpawnEnt[1], 31, "info_player_deathmatch" );
	}
	else if ( g_MOD == GAME_DOD )
	{
		copy( szSpawnEnt[0], 31, "info_player_axis" );
		copy( szSpawnEnt[1], 31, "info_player_allies" );
	}
}

public WC3_DetermineGame()
{

	// This may be used in the future to remove the #defines based on which mod is running
	if ( is_running("cstrike") )
	{
		g_MOD = GAME_CSTRIKE;
	}
	else if ( is_running("czero") )
	{
		g_MOD = GAME_CZERO;
	}
	else if ( is_running("dod") )
	{
		g_MOD = GAME_DOD;
	}
}

// Function will prompt a user for a race or skill selection
public WC3_GetUserInput( id )
{
	
	if ( id > TASK_GETINPUT )
	{
		id -= TASK_GETINPUT;
	}

	if( !warcraft3 || !p_data_b[id][PB_ISCONNECTED] )
	{
		return PLUGIN_CONTINUE;
	}

	new iTotalSkillsUsed = p_data[id][P_SKILL1] + p_data[id][P_SKILL2] + p_data[id][P_SKILL3] + p_data[id][P_ULTIMATE];

	if ( p_data[id][P_RACE] == 0 )
	{
		WAR3_chooserace( id );
	}
	else if ( iTotalSkillsUsed < p_data[id][P_LEVEL] )
	{
		menu_Select_Skill( id, 0 );
	}
	else
	{
		WAR3_Display_Level( id, DISPLAYLEVEL_NONE );
	}

	return PLUGIN_CONTINUE;
}

// Function is called when the buytime is over
public _WC3_BuyTimeDone()
{
	g_buyTime = false;
}

// Function will "reset" the game - i.e. "sv_restart 1"
public WC3_ResetGame()
{
	new players[32], numplayers, id, i
	get_players(players, numplayers);

	for ( i=0; i < numplayers; i++ )
	{
		id = players[i];
		
		// Remove Ultimate Delay
		task_exists( TASK_UDELAY + id ) ? remove_task( TASK_UDELAY + id ) : 0;
		
		// User didn't die last round... 
		p_data_b[id][PB_DIEDLASTROUND]		= false;
		
		// Remove player's items
		p_data[id][P_ITEM]					= 0;
		p_data[id][P_ITEM2]					= 0;
		p_data[id][P_RINGS]					= 0;

		// Reset user's XP
		p_data[id][P_LEVEL]					= 0;
		p_data[id][P_RACE]					= 0;
		p_data[id][P_SKILL1]				= 0;
		p_data[id][P_SKILL2]				= 0;
		p_data[id][P_SKILL3]				= 0;
		p_data[id][P_ULTIMATE]				= 0;
		p_data[id][P_XP]					= 0;

		// Check for Counter-Strike or Condition Zero
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			p_data[id][P_HECOUNT]			= 0;
			p_data[id][P_FLASHCOUNT]		= 0;
		}
	}

	g_GameRestarting = false;
}

// Called .3 seconds after the modules have been "loaded"
public _WC3_CheckModules()
{
	new bool:bReloadMap = false;	
	
	// Loop through all the modules and determine what we need to do
	for ( new i = 0; i < iTotalNotLoadedModules; i++ )
	{

		// Enable cstrike module
		if ( equal( szNotLoadedModules[i], "cstrike" ) )
		{
			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{
				WC3_EnableModule( "cstrike_amxx" );
				bReloadMap = true;
			}
		}

		// Enable cstrike module
		else if ( equal( szNotLoadedModules[i], "csx" ) )
		{
			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{
				WC3_EnableModule( "csx_amxx" );
				bReloadMap = true;
			}
		}

		// Enable DOD Fun module
		else if ( equal( szNotLoadedModules[i], "dod" ) )
		{
			if ( g_MOD == GAME_DOD )
			{
				WC3_EnableModule( "dodfun_amxx" );
				bReloadMap = true;
			}
		}

		// Enable DOD X module
		else if ( equal( szNotLoadedModules[i], "dodx" ) )
		{
			if ( g_MOD == GAME_DOD )
			{
				WC3_EnableModule( "dodx_amxx" );
				bReloadMap = true;
			}
		}

		// Enable SQL Module - I'm pretty sure we can safely enable this, since in theory no other plugins need mysql (DBI isn't running after all)
		else if ( equal( szNotLoadedModules[i], "dbi" ) )
		{
			// Make sure the module exists - only need to check this since we didn't in WC3_MissingModules
			if ( WC3_ModuleExists( "sqlite_amxx" ) )
			{
				WC3_EnableModule( "sqlite_amxx" );
				bReloadMap = true;
			}
		}
		
		// Enable nvault module
		else if ( equal( szNotLoadedModules[i], "nvault" ) )
		{
			// Make sure the module exists - only need to check this since we didn't in WC3_MissingModules
			if ( WC3_ModuleExists( "nvault_amxx" ) )
			{
				WC3_EnableModule( "nvault_amxx" );
				bReloadMap = true;
			}
		}

		// Enable Engine module
		else if ( equal( szNotLoadedModules[i], "engine" ) )
		{
			WC3_EnableModule( "engine_amxx" );
			bReloadMap = true;
		}

		// Enable Fakemeta module
		else if ( equal( szNotLoadedModules[i], "fakemeta" ) )
		{
			WC3_EnableModule( "fakemeta_amxx" );
			bReloadMap = true;
		}

		// Enable Fun module
		else if ( equal( szNotLoadedModules[i], "fun" ) )
		{
			WC3_EnableModule( "fun_amxx" );
			bReloadMap = true;
		}
	}

	if ( bReloadMap )
	{
		set_task( 2.0, "WC3_ReloadMap" );
	}
}

public WC3_EnableModule( module_name[] )
{
	new szModulesINI[256];

	get_configsdir( szModulesINI, 255 );
	add( szModulesINI, 255, "/modules.ini" );

    if ( file_exists( szModulesINI ) )
    {
        new line = 0, oldline = 0, len = 0;
        new szLine[256];
        
        // Check out every line in the file
        while ( ((line = read_file( szModulesINI, line, szLine, 255, len)) != 0) )
        {
            // Module Name Found
            if ( contain( szLine, module_name ) != -1 )
            {
                // Semicolon found in line
                if ( contain( szLine, ";" ) != -1 )
                {
                    write_file( szModulesINI, szLine[1], oldline );
                }
            }

            oldline = line;
        }
    }
}  
public WC3_ReloadMap()
{
	new szMapName[32];
	get_mapname( szMapName, 31 );
	
	server_cmd( "changelevel %s", szMapName );
}

// Checks to see if the module exists
public WC3_MissingModules( const module_name[] )
{
	new bool:bMissing = false;

	// Check cstrike module
	if ( equal( module_name, "cstrike" ) )
	{
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			bMissing = !WC3_ModuleExists( "cstrike_amxx" );
		}
	}

	// Check csx module
	else if ( equal( module_name, "csx" ) )
	{
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			bMissing = !WC3_ModuleExists( "csx_amxx" );
		}
	}

	// Check DOD Fun module
	else if ( equal( module_name, "dod" ) )
	{
		if ( g_MOD == GAME_DOD )
		{
			bMissing = !WC3_ModuleExists( "dodfun_amxx" );
		}
	}

	// Check DOD X module
	else if ( equal( module_name, "dodx" ) )
	{
		if ( g_MOD == GAME_DOD )
		{
			bMissing = !WC3_ModuleExists( "dodx_amxx" );
		}
	}

	// Check Engine module
	else if ( equal( module_name, "engine" ) )
	{
		bMissing = !WC3_ModuleExists( "engine_amxx" );
	}

	// Check Fakemeta module
	else if ( equal( module_name, "fakemeta" ) )
	{
		bMissing = !WC3_ModuleExists( "fakemeta_amxx" );
	}

	// Check Fun module
	else if ( equal( module_name, "fun" ) )
	{
		bMissing = !WC3_ModuleExists( "fun_amxx" );
	}

	return bMissing;
}


public WC3_ModuleExists( module_name[] )
{
	new szExt[16] = ".dll";

	if ( is_linux_server() )
	{
		copy( szExt, 15, "_i386.so" );
	}
	else if ( is_amd64_server() )
	{
		copy( szExt, 15, "_amd64.so" );
	}

	new szFullPath[128];

	format( szFullPath, 127, "addons/amxmodx/modules/%s%s", module_name, szExt );

	return file_exists( szFullPath );
}
