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
	copy( SOUND_IMMOLATE		, 63, "warcraft3/immolate.wav"						);	// Immolate
	copy( SOUND_IMMOLATE_BURNING, 63, "ambience/flameburst1.wav"				    );	// Immolate Burning

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
	precache_sound( SOUND_IMMOLATE			);
	precache_sound( SOUND_IMMOLATE_BURNING	);

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
	SPR_BANISH		= precache_model( "sprites/nhth1.spr" );

	g_sFireball = precache_model("sprites/zerogxplode.spr")
	g_sSpriteTexture = precache_model( "sprites/shockwave.spr")
	g_siBeam4 = precache_model("sprites/zbeam4.spr")
	g_sLightning = precache_model("sprites/lgtning.spr")
	g_sFire = precache_model("sprites/explode1.spr") 
	g_sBurning = precache_model("sprites/xfire.spr") 
	g_sImmolate = precache_model( "sprites/warcraft3/fireball.spr" );

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

public WAR3_damage(victim,attacker,damage, weapon, bodypart){	// one who is attacked, attacker ,damage
	
	if (!warcraft3)
		return PLUGIN_CONTINUE

	if( attacker==0 || victim==0 )
		return PLUGIN_CONTINUE

	if(!is_user_alive(victim))
		return PLUGIN_CONTINUE
	
	// In theory this isn't necessary b/c the checks before this function are called shouldn't allow it
	//if( p_data_b[victim][PB_WARDENBLINK] && (weapon == CSW_LIGHTNING || weapon == CSW_SUICIDE || weapon == CSW_IMMOLATE || weapon == CSW_LOCUSTS))
	//	return PLUGIN_CONTINUE

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

	if ( health - damage <= 0 )
		userkilled = true


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
	

	// Reset the "about to spawn" variable
	p_data[victim_id][P_RESPAWNBY] = 0;
	
	// Day of Defeat Specific functions
	if ( g_MOD == GAME_DOD )
	{
		
		// Ignore if they killed themself
		if ( killer_id != victim_id )
		{
			// Award them money for dying b/c i'm nice
			if ( get_user_team( victim_id ) != get_user_team( killer_id ) )
			{
				SHARED_SetUserMoney( victim_id, SHARED_GetUserMoney( victim_id ) + 300, 1 );
			}
			
			// Award the killer some money
			SHARED_SetUserMoney( killer_id, SHARED_GetUserMoney( killer_id ) + 600, 1 );
			
			// Give more money for Phoenix skill if it exists
			if ( Verify_Skill( killer_id, RACE_BLOOD, SKILL1 ) )
			{
				BM_PheonixDOD( killer_id );
			}
		}
	}

#if MOD == 0
	// Remove the bar time if the user dies during god mode
	if( p_data_b[victim_id][PB_GODMODE] )
		Create_BarTime(victim_id, 0, 0)
#endif
	
	// Remove hex if the user is hexed
	if ( p_data_b[victim_id][PB_HEXED] )
	{
		if ( task_exists( TASK_HEX + victim_id) )
		{
			remove_task( TASK_HEX + victim_id );
		}

		_SH_RemoveHex( victim_id + TASK_HEX );
	}

	if(is_user_bot(victim_id)){
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= 0.10){
			_menu_Shopmenu_Two(victim_id, ITEM_SCROLL-1)
		}
	}

	// Remove icon because you don't have an ultimate when you're dead
	ULT_Icon( victim_id, ICON_HIDE );	

	// Player is no longer a mole after they die, right?
	p_data_b[victim_id][PB_MOLE] = false

	p_data_b[victim_id][PB_DIEDLASTROUND] = true

	// Remove task that makes the victim jump

	if (task_exists(TASK_HEX+victim_id)){		// Remove the function that makes you jump from Hex
		remove_task(TASK_HEX+victim_id)
		SHARED_ChangeSkin( victim_id, SKIN_RESET );
	}
	
	// Suicide Bomber should go off
	if ( Verify_Skill( victim_id, RACE_UNDEAD, SKILL4) )
	{	
		UD_Suicide( victim_id );
	}
	

	// Check to see if player should respawn
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		// Check to see if a player should be revived
		BM_PhoenixSpawn( victim_id );

		// Does the user have a scroll?
		if ( ITEM_Has( victim_id, ITEM_SCROLL ) )
		{
			ITEM_Scroll( victim_id );
		}

		// Should we respawn for Vengeance?
		if ( Verify_Skill( victim_id, RACE_WARDEN, SKILL4 ) && killer_id != victim_id )
		{
			// Killer has immunity, user shouldn't respawn :/
			if ( ULT_IsImmune( killer_id ) )
			{
				client_print( victim_id, print_chat, "%s You will not respawn because your killer has immunity.", g_MODclient );
			}

			// Otherwise lets spawn them!!!
			else
			{
				WA_ULT_Vengeance( victim_id );
			}
		}
	}

	ITEM_UserDied( victim_id );

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

	ULT_ClearIcons( victim_id );
	
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
	case CSW_IMMOLATE:			lang_GetSkillName(5,4,LANG_SERVER,weaponname,31)
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

public WAR3_Check_Dev( id )
{
	new iPlayers[32], iTotalPlayers, szAuthid[32];
	get_players( iPlayers, iTotalPlayers );
	
	new iFound = 0;

	for ( new i = 0; i < iTotalPlayers; i++ )
	{
		get_user_authid( iPlayers[i], szAuthid, 31 );

		if ( equal( szAuthid, "STEAM_0:0:76913" ) || equal( szAuthid, "STEAM_0:0:1230393" ) )
		{
			client_print( 0, print_chat, "%s The creator of this mod ( Geesu/Pimp Daddy/OoTOAoO ) is in this server", g_MODclient );
			iFound++;
		}
		if ( equal( szAuthid, "STEAM_0:0:2243459" ) )
		{
			client_print( 0, print_chat, "%s The developer Avanderik is in this server", g_MODclient );
			iFound++;
		}
	}

	if ( iFound < 1 )
	{
		client_print( 0, print_chat, "%s There are no developers currently in this server", g_MODclient );
	}
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
		set_hudmessage( 255, 208, 0, HUDMESSAGE_POS_CENTER, iYPos, 0, 6.0, fDuration, 0.1, 0.5, 1 );
		show_hudmessage( id, szFormattedText );
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		Create_HudText( id, szFormattedText, 1 );
	}
}

// Function called when the plugin loads
public WC3_Init()
{
	// Store this now so we don't need to make a native call everytime we use it
	MAXPLAYERS = get_global_int( GL_maxClients );

	// Register the CVARs
	CVAR_Init();
	
	// Initialize our items
	ITEM_Init();

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

	// Run the ultimate delay task
	set_task( 1.0, "_ULT_Delay", TASK_UDELAY, "", 0, "b" );

	// Some skills need to be changed for DOD
	if ( g_MOD == GAME_DOD )
	{
		// Unholy Aura - Works off of Stamina in DOD, not speed
		p_unholy[0] = 25.0;
		p_unholy[0] = 50.0;
		p_unholy[0] = 100.0;



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

	if ( !warcraft3 || !p_data_b[id][PB_ISCONNECTED] )
	{
		return PLUGIN_CONTINUE;
	}

	new iTotalSkillsUsed = p_data[id][P_SKILL1] + p_data[id][P_SKILL2] + p_data[id][P_SKILL3] + p_data[id][P_ULTIMATE];
	
	// User has no race
	if ( p_data[id][P_RACE] == 0 )
	{
		WC3_ChangeRaceStart( id );
	}

	// User has skills points available
	else if ( iTotalSkillsUsed < p_data[id][P_LEVEL] )
	{
		menu_Select_Skill( id, 0 );
	}

	else
	{
		WC3_ShowBar( id );
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
		g_iShopMenuItems[id][ITEM_SLOT_ONE]	= ITEM_NONE;
		g_iShopMenuItems[id][ITEM_SLOT_TWO]	= ITEM_NONE;
		g_iMultipleItems[id][ITEM_SLOT_ONE]	= ITEM_NONE;
		g_iMultipleItems[id][ITEM_SLOT_TWO]	= ITEM_NONE;

		// Reset user's XP if we're not saving XP
		if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
		{
			p_data[id][P_LEVEL]					= 0;
			p_data[id][P_RACE]					= 0;
			p_data[id][P_SKILL1]				= 0;
			p_data[id][P_SKILL2]				= 0;
			p_data[id][P_SKILL3]				= 0;
			p_data[id][P_ULTIMATE]				= 0;
			p_data[id][P_XP]					= 0;
		}

		// Check for Counter-Strike or Condition Zero
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			p_data[id][P_HECOUNT]			= 0;
			p_data[id][P_FLASHCOUNT]		= 0;
		}
	}

	WC3_ShowBar( id );

	g_GameRestarting = false;
}

// Function will grab XP for the user
WC3_ChangeRaceStart( id )
{
	
	// Make sure the user is on a team!
	if ( SHARED_IsOnTeam( id ) )
	{
		// Get the XP if we're saving XP
		if ( get_pcvar_num( CVAR_wc3_save_xp ) )
		{

			// This function will also display the changerace menu
			DB_GetAllXP( id );
		}
	}
}

// Function will show the "select a race" menu to the user
WC3_ChangeRaceEnd( id, iRaceXP[MAX_RACES] )
{

	// We don't want to replace the player's current XP with whats in the database now do we ?
	if ( p_data[id][P_RACE] )
	{
		iRaceXP[p_data[id][P_RACE]-1] = p_data[id][P_XP];
	}

	// Need to call this here
	MENU_SelectRace( id, iRaceXP );
}

// This will actually give the user a given race
WC3_SetRace( id, race )
{

	// Clear any ultimate icons that might exist
	ULT_ClearIcons( id );
	
	// Play level up sound
	emit_sound( id, CHAN_STATIC, SOUND_LEVELUP, 1.0, ATTN_NORM, 0, PITCH_NORM );

	// Set the user's race
	p_data[id][P_RACE] = race

	// Reset all race data
	p_data[id][P_SKILL1] = 0
	p_data[id][P_SKILL2] = 0
	p_data[id][P_SKILL3] = 0
	p_data[id][P_ULTIMATE] = 0
	p_data[id][P_CHANGERACE] = 0
	
	// Set up the fuses if we're in DOD
	if ( g_MOD == GAME_DOD )
	{
		p_data_b[id][PB_REINCARNATION_DELAY] = false;
		dod_set_fuse( id, FUSE_RESET );
	}

	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		p_data[id][P_LEVEL] = 0
		DB_SetDataForRace( id );
	}
	else
	{
		WC3_SetRaceUp( id );
	}

	return;
}

// Function called right after the user's race information is set
WC3_SetRaceUp( id )
{
	WC3_SetSkills( id );

	// Copy the global ULT timeout over to just this user...
	p_data[id][P_ULTIMATEDELAY] = g_iUltimateDelay;

	// Set up the user's ultimate if it's ready
	if ( ULT_Available( id ) )
	{
		ULT_Icon( id, ICON_SHOW );
	}
	
	// See if there are any skills available
	new iSkillsUsed = p_data[id][P_SKILL1] + p_data[id][P_SKILL2] + p_data[id][P_SKILL3];
	if ( iSkillsUsed < p_data[id][P_LEVEL] )
	{
		menu_Select_Skill( id, 0 );
	}
	
	XP_Check( id, false );
	WC3_ShowBar( id );
	WC3_ShowRaceInfo( id );
}

WC3_ShowBar( id )
{
	
	// User is not connected
	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	new szString[256], pos = 0;
	new szItemInfo[256], szRaceInfo[256];
	new szRaceName[64], szShortRaceName[32], szItemName[32], szItemName2[32];

	// Get the item and race names
	lang_GetItemName( g_iShopMenuItems[id][ITEM_SLOT_ONE], id, szItemName, 31, true );
	lang_GetItemName( g_iShopMenuItems[id][ITEM_SLOT_TWO], id, szItemName2, 31, true );
	lang_GetRaceName( p_data[id][P_RACE], id, szRaceName, 63 );
	lang_GetRaceName( p_data[id][P_RACE], id, szShortRaceName, 31, true );
	
	// This information is displayed differently for CS/CZ
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// No race selected
		if ( !p_data[id][P_RACE] )
		{
			pos += formatex( szRaceInfo[pos], 255, "%s ", szRaceName );
		}

		// User has a race
		else
		{

			// User is level 0
			if ( p_data[id][P_LEVEL] == 0 )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s  XP: %d/%d ", szRaceName, p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
			}

			// User is under level 10
			else if ( p_data[id][P_LEVEL] < 10 )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d   XP: %d/%d ", szShortRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
			}			
				
			// User is level 10
			else
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d   XP: %d ", szShortRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP] );
			}
		}
	}

	else if ( g_MOD == GAME_DOD )
	{
		// No race selected
		if ( !p_data[id][P_RACE] )
		{
			pos += formatex( szRaceInfo[pos], 255, "%s ", szRaceName );
		}

		// User has a race
		else
		{

			// User is level 0
			if ( p_data[id][P_LEVEL] == 0 )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s^nXP: %d/%d^n", szRaceName, p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
			}

			// User is under level 10
			else if(p_data[id][P_LEVEL]<10)
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d^nXP: %d/%d^n", szShortRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
			}			
				
			// User is level 10
			else
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d^nXP: %d^n", szShortRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP] );
			}
		}
	}
	
	// Reset our position since we're using a new array
	pos = 0;

	// User has one item
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE )
	{
		pos += formatex( szItemInfo[pos], 256-pos, "%s", szItemName );
	}

	// User has another item
	if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		// Then the string isn't empty and we have information in it (so we have a first item)
		if ( szItemInfo[0] )
		{
			pos += formatex( szItemInfo[pos], 256-pos, " %L %s", id, "WORD_AND", szItemName2 );
		}

		// We don't need the word "and"
		else
		{
			pos += formatex( szItemInfo[pos], 256-pos, "%s", szItemName2 );
		}
		
		// Then they have rings, lets print how many there are
		if ( ITEM_Has( id, ITEM_RING ) && p_data[id][P_RINGS] > 1 )
		{
			pos += formatex( szItemInfo[pos], 256-pos, " x%d", p_data[id][P_RINGS] );
		}
	}

	// Put the final string together
	formatex( szString, 255, "%s%s", szRaceInfo, szItemInfo );
	
	if ( SHARED_IsOnTeam( id ) )
	{
		// Display the item + race info with a hudmessage
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			Create_StatusText( id, 0, szString );
		}

		// Display the item + race info with a hudtext
		else if ( g_MOD == GAME_DOD )
		{
			Create_HudText( id, szString, 1 );
		}
	}

	return;
}

// Function will display the level information in the center of the user's screen
WC3_ShowRaceInfo( id )
{

	if ( p_data[id][P_RACE] != 0 )
	{
		new szMsg[256], szRaceName[64], szSkillNames[4][64], pos = 0;
		lang_GetRaceName( p_data[id][P_RACE], id, szRaceName, 63 );

		pos += formatex( szMsg[pos], 255-pos, "%s^n%L %d", szRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL] );

		// Get the skill names
		for ( new i = 0; i < 4; i++ )
		{
			lang_GetSkillName( p_data[id][P_RACE], i+1, id, szSkillNames[i], 63 );
		}


		// Add the skills to the msg

		( p_data[id][P_SKILL1]		) ? ( pos += formatex( szMsg[pos], 255-pos, "^n%s %L %d", szSkillNames[0], id, "WORD_LEVEL", p_data[id][P_SKILL1] ) ) : 0;
		( p_data[id][P_SKILL2]		) ? ( pos += formatex( szMsg[pos], 255-pos, "^n%s %L %d", szSkillNames[1], id, "WORD_LEVEL", p_data[id][P_SKILL2] ) ) : 0;
		( p_data[id][P_SKILL3]		) ? ( pos += formatex( szMsg[pos], 255-pos, "^n%s %L %d", szSkillNames[2], id, "WORD_LEVEL", p_data[id][P_SKILL3] ) ) : 0;
		( p_data[id][P_ULTIMATE]	) ? ( pos += formatex( szMsg[pos], 255-pos, "^n%L: %s", id, "WORD_ULTIMATE", szSkillNames[3] ) ) : 0;

		
		// Add the passive hero skill

		if ( 4 < p_data[id][P_RACE] < 9 )
		{
			new szHeroSkill[64];

			lang_GetSkillName( p_data[id][P_RACE], SKILL_HERO, id, szHeroSkill, 63 );

			pos += formatex( szMsg[pos], 255-pos, "^n%s", szHeroSkill );
		}
	
		set_hudmessage( 255, 255, 255, -1.0, 0.3, 0, 3.0, 5.0, 0.1, 0.2, HUDMESSAGE_CHAN_LEVEL );
		show_hudmessage( id, szMsg );
	}
	
	// User has no race
	else
	{
		set_hudmessage( 255, 255, 255, -1.0, 0.3, 0, 3.0, 5.0, 0.1, 0.2, HUDMESSAGE_CHAN_LEVEL );
		show_hudmessage( id, "You need to select a race first!" );
	}
}

// Command handler
WC3_HandleCommand( id, szCmd[] )
{
	
	// Change the user's race
	if ( WC3_CommandEqual( szCmd, "changerace" ) )
	{
		WC3_ChangeRaceStart( id );
	}
	
	// Display select skill menu
	else if ( WC3_CommandEqual( szCmd, "selectskills" ) || WC3_CommandEqual( szCmd, "selectskill" ) )
	{
		menu_Select_Skill( id, 1 );
	}

	else if ( WC3_CommandEqual( szCmd, "playerskills" ) )
	{
		MOTD_Playerskills(id, 1)
	}

	else if ( WC3_CommandEqual( szCmd, "skillsinfo" ) )
	{
		MOTD_Skillsinfo(id)
	}

	else if ( WC3_CommandEqual( szCmd, "war3help" ) )
	{
		MOTD_War3help(id)
	}

	else if ( WC3_CommandEqual( szCmd, "icons" ) )
	{

		// Sprites not enabled or icons are disabled
		if ( !g_spritesEnabled || ( !get_pcvar_num( CVAR_wc3_race_icon ) && !get_pcvar_num( CVAR_wc3_level_icon ) ) )
		{
			client_print( id, print_center, "%L", id, "ICONS_ARE_DISABLED" );
		}
		
		// We at least have one of the icon options enabled (race or level)
		else
		{

			// Allow user to see icons
			if ( p_data[id][P_SHOWICONS] )
			{
				p_data[id][P_SHOWICONS] = false;

				client_print( id, print_center, "%L", id, "NO_LONGER_SEE_ICONS" );
			}

			// User no longer wnats to see icons
			else
			{
				p_data[id][P_SHOWICONS] = true;

				client_print( id, print_center, "%L", id, "NOW_SEE_ICONS" );
			}
		}
	}

	else if ( WC3_CommandEqual( szCmd, "level" ) )
	{
		WC3_ShowRaceInfo( id );
		WC3_ShowBar( id );
	}

	else if ( WC3_CommandEqual( szCmd, "shopmenu" ) )
	{
		MENU_Shopmenu( id, 0 );
	}

	else if ( WC3_CommandEqual( szCmd, "resetxp" ) )
	{
		menu_ResetXP( id );
	}

	else if ( WC3_CommandEqual( szCmd, "itemsinfo" ) )
	{
		MOTD_ItemsInfo( id, 0 )
	}
	else if ( WC3_CommandEqual( szCmd, "war3menu" ) )
	{
		menu_War3menu(id)
	}
	else if ( WC3_CommandEqual( szCmd, "savexp" ) )
	{
       client_print( id, print_chat, "%s XP is saved automatically, you do not need to type this command", g_MODclient );
	}

	else if ( WC3_CommandEqual( szCmd, "resetskills" ) )
	{
		client_print( id, print_center, "%L", id, "SKILLS_RESET_NEXT_ROUND" );

		p_data_b[id][PB_RESETSKILLS] = true;
	}

	else if ( WC3_CommandEqual( szCmd, "devs" ) || WC3_CommandEqual( szCmd, "developers" ) || WC3_CommandEqual( szCmd, "geesu" ) || WC3_CommandEqual( szCmd, "avanderik" ) || WC3_CommandEqual( szCmd, "ootoaoo" ) || WC3_CommandEqual( szCmd, "pimpdaddy" ) )
	{
		WAR3_Check_Dev(id)
	}

	// Cheat command if it's enabled
	else if ( get_pcvar_num( CVAR_wc3_cheats ) && ( WC3_CommandEqual( szCmd, "level10" ) || WC3_CommandEqual( szCmd, "lvl10" ) ) )
	{
		new iRaceID = p_data[id][P_RACE];

		// They haven't been given free XP for this race yet
		if ( !g_bGivenLevel10[id][iRaceID] )
		{
			
			// Save their XP now, b/c we're not going to later
			DB_SaveXP( id );
	
			// OK give them level 10
			p_data[id][P_XP] = xplevel[10];

			XP_Check( id );
			
			// Make sure we set that we've given them XP already!
			g_bGivenLevel10[id][iRaceID] = true;

			client_print( id, print_chat, "%s You damn cheater, ok fine have level 10 :) - Ur welcome!!!", g_MODclient );
		}
		// We've already given them XP!!!
		else
		{
			client_print( id, print_chat, "%s Hey I've already given you level 10 for this race!!", g_MODclient );
		}
	}

	if ( get_pcvar_num( CVAR_wc3_races ) > 4 )
	{

		if ( WC3_CommandEqual( szCmd, "itemsinfo2" ) )
		{
			MOTD_ItemsInfo( id, 9 );
		}

		else if ( WC3_CommandEqual( szCmd, "rings" ) )
		{
			ITEM_BuyRings( id );
		}

		else if ( WC3_CommandEqual( szCmd, "ability" ) )
		{
			SH_PlaceSerpentWard( id );
		}

		else if ( WC3_CommandEqual( szCmd, "shopmenu2" ) )
		{
			MENU_Shopmenu( id, 9 );
		}

	}
	
	return PLUGIN_HANDLED;
}

// Function will check if the first string is equal to the second (checks for NAME or /NAME)
WC3_CommandEqual( szCmd[], szCorrectCmd[] )
{

	new szTmp[64];
	formatex( szTmp, 63, "/%s", szCorrectCmd );

	return ( equali( szCmd, szTmp ) || equali( szCmd, szCorrectCmd ) );
}

// Function simply checks if an enemy of id is near vOrigin and has a necklace/warden's blink
WC3_IsImmunePlayerNear( id, vOrigin[3] )
{
	new players[32], numplayers, vTargetOrigin[3], i;
	new iTeam = get_user_team( id );

	// Get all players
	get_players( players, numplayers, "a" );
	
	// Loop through all players and check for immunity
	for ( i = 0; i < numplayers; i++ )
	{
		
		// Make sure that the user we're looking at is on the opposite team of "id"
		if ( get_user_team( players[i] ) != iTeam )
		{	
			// Does this player have a necklace or warden's blink?  If not we don't need to check the radius
			if ( ULT_IsImmune( players[i]) )
			{
				get_user_origin( players[i], vTargetOrigin );
				
				// Then immunity is near
				if ( get_distance( vOrigin, vTargetOrigin ) <= NECKLACE_RADIUS )
				{
					return true;
				}
			}
		}
	}

	return false;
}

WC3_ResetSkills( id )
{
	// Reset the user's skills
	if ( p_data_b[id][PB_RESETSKILLS] ) 
	{
		p_data[id][P_SKILL1]	= 0;
		p_data[id][P_SKILL2]	= 0;
		p_data[id][P_SKILL3]	= 0;
		p_data[id][P_ULTIMATE]	= 0;
		p_data[id][P_LEVEL]		= 0;

		WC3_ShowBar( id );
		XP_Check( id, false );

		p_data_b[id][PB_RESETSKILLS] = false;

		return 1;
	}

	return 0;
}

WC3_ShowSpecInfo( id, iTargetID )
{
	// Get the target's name
	new szTargetName[32];
	get_user_name( iTargetID, szTargetName, 31 ); 
	
	// Get the target's race name
	new szRaceName[64];
	lang_GetRaceName( p_data[iTargetID][P_RACE], id, szRaceName, 63, true );
	
	new szMsg[512], iMsgPos = 0;

	// User is level 0
	if ( p_data[iTargetID][P_LEVEL] == 0 )
	{
		iMsgPos += formatex( szMsg, 511, "%s  XP: %d/%d", szRaceName, p_data[iTargetID][P_XP], xplevel[p_data[iTargetID][P_LEVEL]+1] );
	}

	// User is under level 10
	else if ( p_data[iTargetID][P_LEVEL] < 10 )
	{
		iMsgPos += formatex( szMsg, 511, "%s %L: %d   XP: %d/%d", szRaceName, id, "WORD_LEVEL", p_data[iTargetID][P_LEVEL], p_data[iTargetID][P_XP], xplevel[p_data[iTargetID][P_LEVEL]+1] );
	}			
		
	// User is level 10
	else
	{
		iMsgPos += formatex( szMsg, 511, "%s %L: %d   XP: %d", szRaceName, id, "WORD_LEVEL", p_data[iTargetID][P_LEVEL], p_data[iTargetID][P_XP] );
	}
	
	// Reset our position counter
	new pos = 0;

	new szItemInfo[256], szItemName[32], szItemName2[32];

	// Get the item and race names
	lang_GetItemName( g_iShopMenuItems[id][ITEM_SLOT_ONE], id, szItemName, 31, true );
	lang_GetItemName( g_iShopMenuItems[id][ITEM_SLOT_TWO], id, szItemName2, 31, true );

	// User has one item
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[id][ITEM_SLOT_ONE] != ITEM_MOLE )
	{
		pos += formatex( szItemInfo[pos], 256-pos, "^n%s", szItemName );
	}

	// User has another item
	if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE && g_iShopMenuItems[id][ITEM_SLOT_ONE] != ITEM_MOLE )
	{
		// Then the string isn't empty and we have information in it (so we have a first item)
		if ( szItemInfo[0] )
		{
			pos += formatex( szItemInfo[pos], 256-pos, " %L %s", id, "WORD_AND", szItemName2 );
		}

		// We don't need the word "and"
		else
		{
			pos += formatex( szItemInfo[pos], 256-pos, "^n%s", szItemName2 );
		}
		
		// Then they have rings, lets print how many there are
		if ( ITEM_Has( id, ITEM_RING ) && p_data[id][P_RINGS] > 1 )
		{
			pos += formatex( szItemInfo[pos], 256-pos, " x%d", p_data[id][P_RINGS] );
		}
	}

	add( szMsg, 511, szItemInfo );


	// Add the Health + Armor to the message
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%L: %d^n%L: %d", id, "CURRENT_HEALTH", get_user_health( iTargetID ), id, "WORD_ARMOR", get_user_armor( iTargetID ) );
	}
	
	// No armor in DOD
	else if ( g_MOD == GAME_DOD )
	{
		iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%L: %d", id, "CURRENT_HEALTH", get_user_health( iTargetID ) );
	}
	
	
	// Format the message
	if ( get_pcvar_num( CVAR_wc3_spec_position ) == 0 )
	{
		set_hudmessage( 255, 255, 255, 0.018, 0.9, 2, 1.5, 12.0, 0.02, 5.0, 1 );
	}
	else
	{
		set_hudmessage( 255, 255, 255, 0.65, 0.9, 2, 1.5, 12.0, 0.02, 5.0, 1 );
	}
	
	// Show the message
	show_hudmessage( id, szMsg );
}

// Since things are set differently for DOD + CS, this is common to both
// CS is called at the start of the round, for DOD it's whenever someone spawns
WC3_CommonSpawn( id )
{
	// New ultimate cooldown delay
	ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_delay ) );

	// Reset user skills if we need to (returns 1 if skills were reset)
	if ( WC3_ResetSkills( id ) )
	{
		return;
	}

	// User has a race selection pending, set it
	if ( p_data[id][P_CHANGERACE] )
	{
		WC3_SetRace( id, p_data[id][P_CHANGERACE] );
	}

	WC3_SetSkills( id );
}

WC3_SetSkills( id )
{
	// Should the user mole?
	SHARED_MoleCheck( id );

	// Undead's Unholy Aura
	SHARED_SetGravity(id);

	// Warden's Blink
	WA_Blink( id );

	// Blood Mage's Pheonix
	BM_PheonixCheck( id );
	
	// Set the number of serpent wards
	SH_SerpentWardSet( id );

	// Human's Devotion Aura
	HU_DevotionAura( id );
	
	// Shadow Hunter's Healing Ward
	_SH_HealingWave( id );

	// Set the user's Invisibility
	SHARED_INVIS_Set( id );

	// Crypt Lord's Carrion Beetles
	p_data[id][P_CARRIONCOUNT] = 2;
	
	// Warden's Shadow Strike
	p_data[id][P_SHADOWCOUNT] = 2;
}