/***********************************************************************
		Precaching FUNCTIONS (idea from war3x)
***********************************************************************/

public WAR3_precache() {
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_precache",0)
	#endif

    #if PRECACHE_WAR3FTSOUNDS
		copy( SOUND_VENGEANCE, 63,			"warcraft3/MiniSpiritPissed1.wav"				)  // Warden's Ultimate
		copy( SOUND_SUICIDE, 63,			"ambience/particle_suck1.wav"					)  // Undead's Ultimate
		copy( SOUND_BANISH, 63,				"warcraft3/banishcaster.wav"					)  // Banish
		copy( SOUND_LOCUSTSWARM, 63,		"warcraft3/locustswarmloop.wav"					)  // Locust Swarm
		copy( SOUND_SHADOWSTRIKE,63,		"warcraft3/shadowstrikebirth.wav"				)  // Shadow Strike
		copy( SOUND_CARRION, 63,			"warcraft3/carrionswarmlaunch1.wav"				)  // Carrion Beetles
		copy( SOUND_IMPALE, 63,				"warcraft3/impalehit.wav"						)  // Impale
		copy( SOUND_VOODOO, 63,				"warcraft3/divineshield.wav"					)  // Big Bad Voodoo
		copy( SOUND_FLAMESTRIKE, 63,		"warcraft3/flamestriketargetwavenonloop1.wav"	)  // Flame Strike
		copy( SOUND_ENTANGLING, 63,			"warcraft3/entanglingrootstarget1.wav"			)  // Entangling Roots
		copy( SOUND_LIGHTNING, 63,			"warcraft3/lightningbolt.wav"					)  // Chain Lightning
		copy( SOUND_TELEPORT, 63,			"warcraft3/massteleporttarget.wav"				)  // Teleport
		copy( SOUND_BLINK, 63,				"warcraft3/blinkarrival.wav"					)  // Blink Teleport
		copy( SOUND_LEVELUP, 63,			"warcraft3/Levelupcaster.wav"					)  // Level up
		copy( SOUND_PICKUPITEM, 63,			"warcraft3/PickUpItem.wav"						)  // Buy / Pick up item
		copy( SOUND_ITEM_TOME, 63,				"warcraft3/Tomes.wav"							)  // Tome of Experience
		copy( SOUND_ULTIMATESCAN, 63,		"turret/tu_ping.wav"							)  // Ultimate Beep
		copy( SOUND_ULTIMATEREADY, 63,		"warcraft3/ResurrectTarget.wav"					)  // Ultimate Beep
		copy( SOUND_ANNIHILATION, 63,		"warcraft3/PurgeTarget1.wav"					)	// Orb of Annihilation
		copy( SOUND_CONCOCTION_CAST, 63,	"warcraft3/PossessionMissileLaunch1.wav"		)	// Unstable Concoction Cast
		copy( SOUND_CONCOCTION_HIT, 63,		"warcraft3/PossessionMissileHit1.wav"			)	// Unstable Concoction Hit
		copy( SOUND_HEX, 63,				"warcraft3/PolymorphDone.wav"					)	// Hex
	#else
		copy( SOUND_VENGEANCE, 63,			"ambience/des_wind2.wav"						)  // Warden's Ultimate
		copy( SOUND_SUICIDE, 63,			"ambience/particle_suck1.wav"					)  // Undead's Ultimate
		copy( SOUND_BANISH, 63,				"player/pl_fallpain3.wav"						)  // Banish
		copy( SOUND_LOCUSTSWARM, 63,		"ambience/bee1.wav"								)  // Locust Swarm
		copy( SOUND_SHADOWSTRIKE,63,		"ambience/alienlaser1.wav"						)  // Shadow Strike
		copy( SOUND_CARRION, 63,			"player/sqk_blast1.wav"							)  // Carrion Beetles
		copy( SOUND_IMPALE, 63,				"doors/aliendoor3.wav"							)  // Impale
		copy( SOUND_VOODOO, 63,				"turret/tu_die2.wav"							)  // Big Bad Voodoo
		copy( SOUND_FLAMESTRIKE, 63,		"garg/gar_flameoff1.wav"						)  // Flame Strike
		copy( SOUND_ENTANGLING, 63,			"weapons/electro5.wav"							)  // Entangling Roots
		copy( SOUND_LIGHTNING, 63,			"weapons/gauss2.wav"							)  // Chain Lightning
		copy( SOUND_TELEPORT, 63,			"x/x_shoot1.wav"								)  // Teleport
		copy( SOUND_BLINK, 63,				"x/x_shoot1.wav"								)  // Blink Teleport
		copy( SOUND_LEVELUP, 63,			"plats/elevbell1.wav"							)  // Level up
		copy( SOUND_PICKUPITEM, 63,			"items/ammopickup1.wav"							)  // Buy / Pick up item
		copy( SOUND_ITEM_TOME, 63,				"items/suitchargeok1.wav"						)  // Tome of Experience
		copy( SOUND_ULTIMATESCAN, 63,		"turret/tu_ping.wav"							)  // Ultimate Beep
		copy( SOUND_ULTIMATEREADY, 63,		"buttons/bell1.wav"								)  // Ultimate Beep
//		copy( SOUND_ANNIHILATION, 63,		"warcraft3/PurgeTarget1.wav"					)	// Orb of Annihilation
//		copy( SOUND_CONCOCTION_CAST, 63,	"warcraft3/PossessionMissileLaunch1.wav"		)	// Unstable Concoction Cast
//		copy( SOUND_CONCOCTION_HIT, 63,		"warcraft3/PossessionMissileHit1.wav"			)	// Unstable Concoction Hit
//		copy( SOUND_HEX, 63,				"warcraft3/PolymorphDone.wav"					)	// Hex

	#endif

	// Skills / Ultimates
	precache_sound(SOUND_VENGEANCE)
 	precache_sound(SOUND_SUICIDE) 
	precache_sound(SOUND_BANISH) 
	precache_sound(SOUND_LOCUSTSWARM)	 
	precache_sound(SOUND_SHADOWSTRIKE) 
	precache_sound(SOUND_CARRION) 
	precache_sound(SOUND_IMPALE) 
	precache_sound(SOUND_VOODOO) 
	precache_sound(SOUND_FLAMESTRIKE) 
	precache_sound(SOUND_ENTANGLING) 
	precache_sound(SOUND_LIGHTNING) 
	precache_sound(SOUND_TELEPORT) 
	precache_sound(SOUND_BLINK) 
	precache_sound(SOUND_ANNIHILATION) 
	precache_sound(SOUND_CONCOCTION_CAST) 
	precache_sound(SOUND_CONCOCTION_HIT) 
	precache_sound(SOUND_HEX) 

	// Miscellaneous
	precache_sound(SOUND_LEVELUP) 
	precache_sound(SOUND_PICKUPITEM) 
	precache_sound(SOUND_ITEM_TOME) 
	precache_sound(SOUND_ULTIMATESCAN) 
	precache_sound(SOUND_ULTIMATEREADY) 
	precache_sound("warcraft3/soundpack/reincarnation.wav") 
	precache_sound("warcraft3/antend.wav") 
	precache_sound("warcraft3/bonus/Error.wav") 

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

#if MOD == 0
	g_sSmoke = precache_model("sprites/steam1.spr")

	if(is_running("czero")){
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
#endif

#if MOD == 1
	precache_model("models/player/axis-inf/axis-inf.mdl")
	precache_model("models/player/axis-para/axis-para.mdl")
	//precache_model("models/player/brit-inf/brit-inf.mdl")
	precache_model("models/player/us-inf/us-inf.mdl")
	precache_model("models/player/us-para/us-para.mdl")
#endif

#if MOD == 0
	new mapname[32]
	get_mapname(mapname,31)

	if (!equali(mapname,"de_inferno_cz")){
		g_spritesEnabled = true
		g_sRace[0] = precache_model("sprites/warcraft3/races/wc3_none_01.spr")
		g_sRace[1] = precache_model("sprites/warcraft3/races/wc3_undead_01.spr")
		g_sRace[2] = precache_model("sprites/warcraft3/races/wc3_human_01.spr")
		g_sRace[3] = precache_model("sprites/warcraft3/races/wc3_orc_01.spr")
		g_sRace[4] = precache_model("sprites/warcraft3/races/wc3_elves_01.spr")
		g_sRace[5] = precache_model("sprites/warcraft3/races/wc3_bloodmage_01.spr")
		g_sRace[6] = precache_model("sprites/warcraft3/races/wc3_shadow_01.spr")
		g_sRace[7] = precache_model("sprites/warcraft3/races/wc3_warden_01.spr")
		g_sRace[8] = precache_model("sprites/warcraft3/races/wc3_cryptlord_01.spr")
		g_sRace[9] = precache_model("sprites/warcraft3/races/wc3_chameleon_01.spr")
	
		g_sLevel[0] = precache_model("sprites/warcraft3/level/a_level_0.spr")
		g_sLevel[1] = precache_model("sprites/warcraft3/level/a_level_1.spr")
		g_sLevel[2] = precache_model("sprites/warcraft3/level/a_level_2.spr")
		g_sLevel[3] = precache_model("sprites/warcraft3/level/a_level_3.spr")
		g_sLevel[4] = precache_model("sprites/warcraft3/level/a_level_4.spr")
		g_sLevel[5] = precache_model("sprites/warcraft3/level/a_level_5.spr")
		g_sLevel[6] = precache_model("sprites/warcraft3/level/a_level_6.spr")
		g_sLevel[7] = precache_model("sprites/warcraft3/level/a_level_7.spr")
		g_sLevel[8] = precache_model("sprites/warcraft3/level/a_level_8.spr")
		g_sLevel[9] = precache_model("sprites/warcraft3/level/a_level_9.spr")
		g_sLevel[10] = precache_model("sprites/warcraft3/level/a_level_10.spr")
	}
#else
	g_spritesEnabled = false
#endif

}

public WAR3_exec_config(){
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_exec_config",0)
	#endif

	new configsDir[64]
	get_configsdir(configsDir, 63)
	server_cmd("exec %s/war3FT.cfg", configsDir)
}

public WAR3_CD_installed(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("has_CD",id)
	#endif

	if(!iCvar[FT_CD])
		return true

	new playername[32]

	get_user_name(id,playername,31)
	if(containi(playername,"[No C-D]") != -1)
		return false
	else
		return true
	
	return false
}

public WAR3_chooserace(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_chooserace",id)
	#endif

#if MOD == 0
	if(get_user_team(id) == CTS || get_user_team(id) == TS){
#endif
#if MOD == 1
	if(get_user_team(id) == ALLIES || get_user_team(id) == AXIS){
#endif
		if(iCvar[MP_SAVEXP])
			XP_Retreive(id,1)
		else
			menu_Select_Race(id,{0,0,0,0,0,0,0,0,0})
	}

}

public WAR3_damage(victim,attacker,damage, weapon, bodypart){	// one who is attacked, attacker ,damage
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_damage",victim)
	#endif
	
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

	// Check for Night Elf's Evasion
/*	if ( Verify_Race(victim, RACE_ELF) ){

		// Evasion
		if ( Verify_Skill(victim, RACE_ELF, SKILL1) ) {
			new Float:randomnumber = random_float(0.0,1.0)

			if (randomnumber <= p_evasion[p_data[victim][P_SKILL1]-1]){

				if (iglow[victim][2] < 1){
					new parm[2]
					parm[0] = victim
					set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
				}
				iglow[victim][2] += damage
				iglow[victim][0] = 0
				iglow[victim][1] = 0
				iglow[victim][3] = 0
				if (iglow[victim][2]>MAXGLOW)
					iglow[victim][2]=MAXGLOW
			}
		}
	}*/

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

public WAR3_death_victim(victim_id, killer_id){
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_death_victim",victim_id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

#if MOD == 1
	if ( killer_id == 0 || get_user_team(victim_id) != get_user_team(killer_id) && killer_id != victim_id )
		set_user_money(victim_id, get_user_money(victim_id)+300, 1)

	if( victim_id != killer_id )
		set_user_money(killer_id, get_user_money(killer_id) + 600,1)

	if ( Verify_Skill(killer_id, RACE_BLOOD, SKILL1) && killer_id != victim_id ){
		Skill_Pheonix(killer_id)
	}
#endif

#if MOD == 0
	if( !Verify_Skill(killer_id, RACE_BLOOD, SKILL3) && get_user_money(killer_id) > 16000 ){
		set_user_money(killer_id, 16000)
	}
#endif

	// In case they respawn, continue ultimate delay check
	if(!task_exists(TASK_UDELAY+victim_id)){
		new parm[1]
		parm[0] = victim_id

		p_data[victim_id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]
		_Ultimate_Delay(parm)
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

	// Save Items for Weapon Reincarnation

	#if MOD == 0
		if(p_data_b[victim_id][PB_ISCONNECTED]){
			if(cs_get_user_nvg(victim_id))
				p_data_b[victim_id][PB_NIGHTVISION] = true
			/**if(cs_get_user_defuse(victim_id))
				p_data_b[victim_id][PB_DEFUSE] = true*/
		}
		saveweapons(victim_id)
	#endif


	// Remove task that makes the victim jump

	if (task_exists(TASK_HEX+victim_id)){		// Remove the function that makes you jump from Hex
		remove_task(TASK_HEX+victim_id)
		changeskin(victim_id,SKIN_RESET)
	}

	// Clear Item Information because the player just died

	Item_Clear(victim_id)

	// Check for Ultimate abilities

	if (Verify_Skill(victim_id, RACE_UNDEAD, SKILL4) && !p_data_b[victim_id][PB_CHANGINGTEAM] && !g_ultimateDelay && !p_data_b[victim_id][PB_ULTIMATEUSED] ){	// Suicide Bomber
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

		p_data[victim_id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]

		if(task_exists(TASK_UDELAY+victim_id))
			remove_task(TASK_UDELAY+victim_id)

		_Ultimate_Delay(parm)
	}
#if MOD == 0
	else if (Verify_Skill(victim_id, RACE_WARDEN, SKILL4) && !p_data_b[victim_id][PB_CHANGINGTEAM] && (!p_data_b[killer_id][PB_WARDENBLINK] || killer_id==victim_id) && !g_ultimateDelay && !p_data_b[victim_id][PB_ULTIMATEUSED] && !endround ){	// Vengeance
		new parm[2]
		parm[0]=victim_id
		parm[1]=6

		p_data_b[victim_id][PB_ULTIMATEUSED]=true

		p_data[victim_id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_COOLDOWN]

		if(task_exists(TASK_UDELAY+victim_id))
			remove_task(TASK_UDELAY+victim_id)
		
		_Ultimate_Delay(parm)

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

	new parameter[1]
	parameter[0]=victim_id
	set_task(1.0,"getuserinput",TASK_GETINPUT+victim_id,parameter,1)

	return PLUGIN_CONTINUE
}

public WAR3_death(victim_id, killer_id, weapon, headshot) {
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_death",victim_id)
	#endif

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
		set_user_money(killer_id,get_user_money(killer_id)+300,1)
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

	case CSW_LIGHTNING:     raceskill(3,4,LANG_SERVER,weaponname,31)
	case CSW_SUICIDE:		raceskill(1,4,LANG_SERVER,weaponname,31)
	case CSW_FLAME:			raceskill(5,4,LANG_SERVER,weaponname,31)
	case CSW_LOCUSTS:		raceskill(8,4,LANG_SERVER,weaponname,31)
	case CSW_SERPENTWARD:   raceskill(6,3,LANG_SERVER,weaponname,31)
	case CSW_SHADOW:		raceskill(7,3,LANG_SERVER,weaponname,31)
	case CSW_THORNS:		raceskill(4,2,LANG_SERVER,weaponname,31)
	case CSW_CARAPACE:		raceskill(8,2,LANG_SERVER,weaponname,31)
	case CSW_CARRION:		raceskill(8,3,LANG_SERVER,weaponname,31)
	case CSW_ORB:			Lang_Hero_Skill_Name(RACE_CRYPT,SKILL_HERO,LANG_SERVER,weaponname,31)
	case CSW_CONCOCTION:	Lang_Hero_Skill_Name(RACE_SHADOW,SKILL_HERO,LANG_SERVER,weaponname,31)

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
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_set_race",0)
	#endif

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

	if (iCvar[MP_SAVEXP]){
		p_data[id][P_XP] = 0
		p_data[id][P_LEVEL] = 0
		XP_Retreive(id,0)
	}

	Skill_Check(id)

	if(!p_data_b[id][PB_BLINKDELAYED] && !p_data_b[id][PB_ULTIMATEUSED])
		Ultimate_Icon(id,ICON_SHOW)

	if (get_user_team(id) == CTS || get_user_team(id) == TS) {
		new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]
		if (skillsused < p_data[id][P_LEVEL])
			menu_Select_Skill(id,0)
	}

	//if(((p_data[id][P_RACE] == 9 && race9Options[2] == 2) || p_data[id][P_RACE] == 2) && p_data[id][P_SKILL2] && get_user_health(id) <= 100){	// set_health
	//	set_user_health(id, p_devotion[p_data[id][P_SKILL2]-1])
	//}

	WAR3_Display_Level(id, DISPLAYLEVEL_SHOWRACE)

	return PLUGIN_CONTINUE
}

#if MOD == 0
	public _WAR3_set_buytime(){
		#if ADVANCED_DEBUG
			writeDebugInfo("_WAR3_set_buytime",0)
		#endif

		g_buyTime=false
	}
#endif

public WAR3_Check_Dev( id )
{
	new players[32], num, auth[32];
	get_players(players, num);
	
	new bool:found = false;
	for ( new i = 0; i < num; i++ )
	{
		get_user_authid(players[i], auth, 31);

		if ( equal(auth, "STEAM_0:0:76913") || equal(auth, "STEAM_0:1:73226") )
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
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_Display_Level",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			return PLUGIN_CONTINUE
		}
	}


	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

#if MOD == 0
	if(iCvar[FT_ITEMS_IN_HUD]){
		new parm2[2]
		parm2[0] = id
		if(!task_exists(TASK_ITEMS+id)){
			_Item_ShowHUD(parm2)
		}
	}
#endif

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
	racename(p_data[id][P_RACE],id,race_name,RACE_NAME_LENGTH_F)

	new xpstring[512]
	new short_race_name[SHORT_RACE_NAME_LENGTH]
	shortracename(p_data[id][P_RACE],id,short_race_name,SHORT_RACE_NAME_LENGTH_F)

#if MOD == 0
	if(iCvar[FT_ITEMS_IN_HUD]){
		if(p_data[id][P_ITEM2]==0){
			if (p_data[id][P_LEVEL]==0)
				format(xpstring,511,"%s   XP: %d/%d ",race_name,p_data[id][P_XP],xplevel[p_data[id][P_LEVEL]+1])
			else if (p_data[id][P_LEVEL]<10)
				format(xpstring,511,"%s %L: %d   XP: %d/%d ",race_name,id,"WORD_LEVEL",p_data[id][P_LEVEL],p_data[id][P_XP],xplevel[p_data[id][P_LEVEL]+1])
			else
				format(xpstring,511,"%s %L: %d   XP: %d ",race_name,id,"WORD_LEVEL",p_data[id][P_LEVEL],xplevel[10],xplevel[10])
		}
		else{
			if (p_data[id][P_LEVEL]==0)
				format(xpstring,511,"%s   XP: %d/%d ",race_name,p_data[id][P_XP],xplevel[p_data[id][P_LEVEL]+1])
			else if (p_data[id][P_LEVEL]<10)
				format(xpstring,511,"%s %L: %d   XP: %d/%d ",race_name,id,"WORD_LEVEL",p_data[id][P_LEVEL],p_data[id][P_XP],xplevel[p_data[id][P_LEVEL]+1])
			else
				format(xpstring,511,"%s %L: %d   XP: %d ",race_name,id,"WORD_LEVEL",p_data[id][P_LEVEL],p_data[id][P_XP])
		}
	}else{
#endif
		new temp2[128]

		if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]!=0){
			new short_item_name[SHORT_ITEM_NAME_LENGTH], short_item_name2[SHORT_ITEM_NAME_LENGTH]
			shortitemname(p_data[id][P_ITEM],id,short_item_name,SHORT_ITEM_NAME_LENGTH_F)
			shortitemname2(p_data[id][P_ITEM2],id,short_item_name2,SHORT_ITEM_NAME_LENGTH_F)

			if (p_data[id][P_ITEM2]==ITEM_RING && p_data[id][P_RINGS]>1)
				format(temp2,127,"%L",id,"PLAYERITEM_AND_REGEN",short_item_name,short_item_name2,p_data[id][P_RINGS])
			else
				format(temp2,127,"%L",id,"PLAYERITEM_AND",short_item_name,short_item_name2)
		}
		else if (p_data[id][P_ITEM]==0 && p_data[id][P_ITEM2]!=0){
			new short_item_name2[SHORT_ITEM_NAME_LENGTH]
			shortitemname2(p_data[id][P_ITEM2],id,short_item_name2,SHORT_ITEM_NAME_LENGTH_F)

			if (p_data[id][P_ITEM2]==ITEM_RING && p_data[id][P_RINGS]>1)
				format(temp2,127,"%s x%d ",short_item_name2,p_data[id][P_RINGS])
			else
				format(temp2,127,"%s",short_item_name2)
		}
		else if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]==0){
			new short_item_name[SHORT_ITEM_NAME_LENGTH]
			shortitemname(p_data[id][P_ITEM],id,short_item_name,SHORT_ITEM_NAME_LENGTH_F)

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
					itemname(p_data[id][P_ITEM],id,item_name,ITEM_NAME_LENGTH_F)
					itemname2(p_data[id][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F)

					if (p_data[id][P_ITEM2]==ITEM_RING && p_data[id][P_RINGS]>1)
						format(itemString,127,"%L",id,"PLAYERITEM_AND_REGEN",item_name,item_name2,p_data[id][P_RINGS])
					else
						format(itemString,127,"%L",id,"PLAYERITEM_AND",item_name,item_name2)
				}
				else if (p_data[id][P_ITEM]==0 && p_data[id][P_ITEM2]!=0){
					new item_name2[ITEM_NAME_LENGTH]
					itemname2(p_data[id][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F)

					if (p_data[id][P_ITEM2]==ITEM_RING && p_data[id][P_RINGS]>1)
						format(itemString,127,"%s x%d ",item_name2,p_data[id][P_RINGS])
					else
						format(itemString,127,"%s",item_name2)
				}
				else if (p_data[id][P_ITEM]!=0 && p_data[id][P_ITEM2]==0){
					new item_name[ITEM_NAME_LENGTH]
					itemname(p_data[id][P_ITEM],id,item_name,ITEM_NAME_LENGTH_F)

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
	}
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
			raceskill(p_data[id][P_RACE],i+1,id,race_skill[i],RACE_SKILL_LENGTH_F)
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
			Lang_Hero_Skill_Name(p_data[id][P_RACE], SKILL_HERO, id, heroskillname, 63)
			format(temp,127,"^n%s", heroskillname)

			add(message,255,temp)
		}
		set_hudmessage(255, 255, 255, -1.0, 0.3, 0, 3.0, 5.0, 0.1, 0.2, HUDMESSAGE_CHAN_LEVEL)
		show_hudmessage(id,message)
	}

	if(get_cvar_num("sv_gravity")>650)
	{
		// Levitation
		if ( Verify_Skill(id, RACE_UNDEAD, P_SKILL3) )
		{		
			if (get_user_gravity(id)!=p_levitation[p_data[id][P_SKILL3]-1])
			{
				set_user_gravity(id,p_levitation[p_data[id][P_SKILL3]-1]);
			}
		}
		else if (p_data[id][P_ITEM2] == ITEM_SOCK)
			set_user_gravity(id, fCvar[FT_SOCK]);
		else if (get_user_gravity(id)!=1.0)
			set_user_gravity(id,1.0);

	}
	else
		set_user_gravity(id,1.0)

	// Check Invisibility
	Skill_Invisibility(id)
	
	// Check Evasion (don't do set here, b/c we don't want to re-check the skill everytime someone types /level)
	Skill_Evasion_Check( id );

	new parm4[1]
	parm4[0]=id
	unholyspeed(parm4)

	return PLUGIN_CONTINUE
}

WAR3_Show_Spectator_Info(id, targetid){
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_Show_Spectator_Info",id)
	#endif

	new name[32]
	get_user_name(targetid ,name,31) 
	new temp[512]
	new message[1048]

	new short_race_name[SHORT_RACE_NAME_LENGTH]
	shortracename(p_data[targetid][0],id,short_race_name,SHORT_RACE_NAME_LENGTH_F)

	if (p_data[targetid][P_LEVEL]==0)
		format(temp,511,"%s   XP: %d/%d",short_race_name,p_data[targetid][P_XP],xplevel[p_data[targetid][P_LEVEL]+1])
	else if (p_data[targetid][P_LEVEL]<10)
		format(temp,511,"%s %L: %d   XP: %d/%d",short_race_name,id,"WORD_LEVEL",p_data[targetid][P_LEVEL],p_data[targetid][P_XP],xplevel[p_data[targetid][P_LEVEL]+1])
	else
		format(temp,511,"%s %L: %d   XP: %d ",short_race_name,id,"WORD_LEVEL",p_data[targetid][P_LEVEL],xplevel[p_data[targetid][P_LEVEL]])

	add(message,1047,temp)

	if (p_data[targetid][P_ITEM]!=0 && (p_data[targetid][P_ITEM2]!=0 && p_data[targetid][P_ITEM2]!=ITEM_MOLE)){
		new item_name[ITEM_NAME_LENGTH], item_name2[ITEM_NAME_LENGTH]
		itemname(p_data[targetid][P_ITEM],id,item_name,ITEM_NAME_LENGTH_F)
		itemname2(p_data[targetid][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F)

		if (p_data[targetid][P_ITEM2]==ITEM_RING)
			format(temp,511,"%L",id,"SHOWRANK_PLAYERITEM",item_name,item_name2,p_data[targetid][P_RINGS])
		else
			format(temp,511,"%L",id,"SHOWRANK_PLAYERITEM2",item_name,item_name2)
	}
	else if (p_data[targetid][P_ITEM]==0 && (p_data[targetid][P_ITEM2]!=0 && p_data[targetid][P_ITEM2]!=ITEM_MOLE)){
		new item_name2[ITEM_NAME_LENGTH]
		itemname2(p_data[targetid][P_ITEM2],id,item_name2,ITEM_NAME_LENGTH_F)

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
	if(iCvar[FT_POSITION]==0)
		set_hudmessage(255,255,255,0.018,0.9,2, 1.5, 12.0, 0.02, 5.0, 1) 
	else
		set_hudmessage(255,255,255,0.65,0.9,2, 1.5, 12.0, 0.02, 5.0, 1) 

	show_hudmessage(id,message) 
}

WAR3_Immunity_Found_Near(id, origin[3]){
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_Immunity_Found_Near",id)
	#endif

	new players[32], numplayers, targetid, targetorigin[3]
	
	new team = get_user_team(id)

	get_players(players, numplayers, "a")

	for (new i=0; i<numplayers; ++i){
		targetid=players[i]

		if( get_user_team(targetid) != team && ( p_data[targetid][P_ITEM] == ITEM_NECKLACE || p_data_b[targetid][PB_WARDENBLINK] ) ){
			get_user_origin(targetid, targetorigin)

			if (get_distance(origin, targetorigin) <= iCvar[FT_BLINK_RADIUS])
				return true
		}
	}

	return false
}

public WAR3_Set_Variables(){
	#if ADVANCED_DEBUG
		writeDebugInfo("WAR3_Set_Variables",0)
	#endif

	if (get_cvar_num("sv_warcraft3")==0)
		warcraft3=false
	else
		warcraft3=true

	MAXPLAYERS = get_global_int(GL_maxClients)

	new divisor

	iCvar[FT_RACE_ICONS				] =	get_cvar_num("FT_race_icons")
	iCvar[FT_LEVEL_ICONS			] =	get_cvar_num("FT_level_icons")
	iCvar[FT_SPEC_INFO				] =	get_cvar_num("FT_spec_info")
	iCvar[FT_MIN_B4_XP				] =	get_cvar_num("FT_min_b4_XP")
	iCvar[FT_BLINK_RADIUS			] =	get_cvar_num("FT_blink_radius")
	iCvar[MP_SAVEXP					] =	get_cvar_num("mp_savexp")
	iCvar[FT_SHOW_ICONS				] =	get_cvar_num("FT_show_icons")
	iCvar[FT_HEALTH_BONUS			] =	get_cvar_num("FT_health_bonus")
	iCvar[FT_NO_GLOVES_ON_KA		] = get_cvar_num("FT_no_gloves_on_ka")
	iCvar[FT_GLOVE_ORC_DAMAGE		] = get_cvar_num("FT_glove_orc_damage")
	iCvar[FT_CLAW					] = get_cvar_num("FT_claw")
	iCvar[FT_CLOAK					] = get_cvar_num("FT_cloak")
	iCvar[FT_OBJECTIVES				] = get_cvar_num("FT_objectives")
	iCvar[FT_KILL_OBJECTIVES		] = get_cvar_num("FT_kill_objectives")
	iCvar[FT_ULTIMATE_DELAY			] = get_cvar_num("FT_ultimatedelay") + 1
	iCvar[FT_ULTIMATE_COOLDOWN		] = get_cvar_num("FT_ultimate_cooldown") + 1
	iCvar[FT_RACES					] = get_cvar_num("FT_races")
	iCvar[FT_POSITION				] = get_cvar_num("FT_position")
	iCvar[FT_RACE9_RANDOM			] = get_cvar_num("FT_Race9_Random")
	iCvar[FT_CD						] = get_cvar_num("FT_CD")
	iCvar[FT_WARN_SUICIDE			] = get_cvar_num("FT_warn_suicide")
	iCvar[FT_BLINKENABLED			] = get_cvar_num("FT_blinkenabled")
	iCvar[SV_ALLOWWAR3VOTE			] = get_cvar_num("sv_allowwar3vote")
	iCvar[SV_MYSQL					] = get_cvar_num("sv_mysql")
	iCvar[SV_SAVE_END_ROUND	] = get_cvar_num("SV_SAVE_END_ROUND")
	iCvar[FT_SAVEBY					] = get_cvar_num("FT_saveby")
	iCvar[SV_MYSQL_AUTO_PRUNING		] = get_cvar_num("sv_mysql_auto_pruning")
	iCvar[SV_DAYSBEFOREDELETE		] = get_cvar_num("sv_daysbeforedelete")
	iCvar[FT_HEALING_RANGE			] = get_cvar_num("FT_healing_range")
	iCvar[FT_XP_RADIUS				] = get_cvar_num("FT_xp_radius")
	iCvar[FT_GLOVE_TIMER			] = get_cvar_num("FT_glove_timer")
	iCvar[FT_BUYTIME				] = get_cvar_num("FT_buytime")
	iCvar[FT_BUYZONE				] = get_cvar_num("FT_buyzone")
	iCvar[FT_BUYDEAD				] = get_cvar_num("FT_buydead")
	iCvar[FT_BLINK_DIZZINESS		] = get_cvar_num("FT_blink_dizziness")
	iCvar[MP_WEAPONXPMODIFIER		] = get_cvar_num("mp_weaponxpmodifier")
	iCvar[FT_BLINK_PROTECTION		] = get_cvar_num("FT_blink_protection")
	iCvar[FT_ENTANGLE_DROP			] = get_cvar_num("FT_entangle_drop")
	iCvar[FT_SHOW_PLAYER			] = get_cvar_num("FT_show_player")
	iCvar[FT_AUTOXP					] = get_cvar_num("FT_autoxp")
	iCvar[FT_IMPALE_INTENSITY		] = get_cvar_num("FT_impale_intensity")
	iCvar[FT_DISABLE_SAVEXP			] = get_cvar_num("FT_disable_savexp")
	iCvar[FT_QUERY_CLIENT			] = get_cvar_num("FT_query_client")

	fCvar[FT_BOT_BUY_ITEM			] = get_cvar_float("FT_bot_buy_item")
	fCvar[FT_FROST_SPEED			] = get_cvar_float("FT_frost_speed")
	fCvar[FT_MASK_OF_DEATH			] = get_cvar_float("FT_mask_of_death")
	fCvar[MP_XPMULTIPLIER			] = get_cvar_float("mp_xpmultiplier")
	fCvar[AMX_VOTE_ANSWERS			] = get_cvar_float("amx_vote_answers")
	fCvar[AMX_VOTEWAR3_RATIO		] = get_cvar_float("amx_votewar3_ratio")
	fCvar[FT_SOCK					] = get_cvar_float("FT_sock")

	if(iCvar[MP_SAVEXP])
		divisor = 5
	else
		divisor = 1

	iCvar[FT_XPBONUS				] = get_cvar_num("FT_xpbonus") / divisor
	iCvar[FT_HEADSHOT_BONUS			] = (get_cvar_num("FT_headshot_bonus") / divisor) + 2
	iCvar[FT_ROUND_WIN_XP			] = get_cvar_num("FT_round_win_XP") / divisor

#if MOD == 0
	server_cmd("amx_statscfg off PlayerName")

	iCvar[FT_ITEMS_IN_HUD			] = get_cvar_num("FT_items_in_hud")
	iCvar[FT_NO_ORCNADES			] = get_cvar_num("FT_no_orcnades")
	iCvar[FT_BOMBPLANTERXP			] = get_cvar_num("FT_bombplanterxp") / divisor
	iCvar[FT_DEFUSEXP				] = get_cvar_num("FT_defusexp") / divisor
	iCvar[FT_HOSTAGEXP				] = get_cvar_num("FT_hostagexp") / divisor
	iCvar[FT_KILLRESCUEMANXP		] = get_cvar_num("FT_killrescuemanxp") / divisor
	iCvar[FT_KILL_BOMB_CARRIER_BONUS] = get_cvar_num("FT_kill_bomb_carrier_bonus") / divisor
	iCvar[FT_DEFUSER_KILL_BONUS		] = get_cvar_num("FT_defuser_kill_bonus") / divisor
	iCvar[FT_VIP_ESCAPE_BONUS		] = get_cvar_num("FT_VIP_escape_bonus") / divisor
	iCvar[FT_VIP_KILL_BONUS			] = get_cvar_num("FT_VIP_kill_bonus") / divisor
	iCvar[MP_GRENADEPROTECTION		] = get_cvar_num("mp_grenadeprotection")

	fCvar[FT_BOOTSPEED				] = get_cvar_float("FT_bootspeed")
#endif

#if MOD == 1
	iCvar[DOD_STARTMONEY			] = get_cvar_num("DOD_startmoney")
	
	fCvar[DOD_BOOTSPEED				] = get_cvar_float("DOD_bootspeed")
#endif

	race9Options[1] = get_cvar_num("FT_Race9_Skill1")
	race9Options[2] = get_cvar_num("FT_Race9_Skill2")
	race9Options[3] = get_cvar_num("FT_Race9_Skill3")
	race9Options[4] = get_cvar_num("FT_Race9_Ultimate")

	if(iCvar[FT_RACES] > MAX_RACES)
		iCvar[FT_RACES] = MAX_RACES

	// Set the version of war3 that is running (war3ft or classic war3?)
	if(iCvar[FT_RACES] < 5){
		g_MOD = "WAR3"
		g_MODclient = "* [WAR3]"
	}
	else{
		g_MOD = "WAR3FT"
		g_MODclient = "* [WAR3FT]"	
	}

	Lang_Set_Menus()
	XP_Set_DBI()
	XP_Set()
	checkmap()

	return PLUGIN_CONTINUE
}

public WAR3_Check(){
	if (get_cvar_num("sv_warcraft3")==0)
		warcraft3=false
	else
		warcraft3=true
}

public WAR3_Kill(id, weapon){
	#if MOD == 0
		set_msg_block(gmsgDeathMsg,BLOCK_ONCE)
		user_kill(id)
	#endif
	#if MOD == 1
		dod_block_deathmsg(id)
		war3ft_kill(id)
		on_Death(id, 0, weapon, 0)
	#endif
}
