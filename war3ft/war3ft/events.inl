// Forwards from the CSX module and DODX module
public grenade_throw(index,greindex,wId){
	#if ADVANCED_DEBUG
		writeDebugInfo("grenade_throw",index)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(g_mapDisabled)
		return PLUGIN_CONTINUE

	new szModel[64]
	entity_get_string(greindex, EV_SZ_model, szModel, 63)
#if MOD == 0
	if (greindex && equal(szModel, "models/w_hegrenade.mdl")){
#endif
#if MOD == 1
	if(greindex && (equal(szModel, "models/w_grenade.mdl") || equal(szModel, "models/w_stick.mdl"))){
#endif
		if( Verify_Skill(index, RACE_ORC, SKILL2) ){
			if( ( (p_data[index][P_ITEM2]==ITEM_GLOVES && iCvar[FT_GLOVE_ORC_DAMAGE]) || (p_data[index][P_ITEM2]!=ITEM_GLOVES) ) && is_user_alive(index) ) { 
					Create_TE_BEAMFOLLOW(greindex, g_siTrail, 20, 10, 255, 32, 32, 196)
			} 
		} 
	}
	return PLUGIN_CONTINUE
}

public on_Damage(victim){
	#if ADVANCED_DEBUG
		writeDebugInfo("client_damage",victim)
	#endif

	new wpnindex = 0, hitplace = 0, attacker = get_user_attacker(victim,wpnindex,hitplace)
	new damage = read_data(2)

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_data_b[victim][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(!p_data_b[attacker][PB_ISCONNECTED] && attacker > 0)
		return PLUGIN_CONTINUE

#if MOD == 0
	new inflictor = entity_get_edict(victim, EV_ENT_dmg_inflictor)
#endif
	
	#if DEBUG
		new attackerName[32]
		get_user_name(attacker, attackerName, 31)

		console_print(victim, "### on_Damage : %d by %s(%d:%d) from %d", damage, attackerName, attacker, entity_get_edict(victim, EV_ENT_dmg_inflictor), wpnindex)
	#endif

#if MOD == 0
	// Check to see if the damage was from the bomb
	if( attacker != inflictor && wpnindex != 4 && attacker != victim && inflictor > 0 ){
		
		if ( is_valid_ent ( inflictor ) )
		{
			new szClassName[64]
			entity_get_string(inflictor, EV_SZ_classname, szClassName, 63)
				
			// Technically I don't think we need to check the classname, but just in case
			if ( equali(szClassName, "grenade") || equali(szClassName, "env_explosion") ){
				wpnindex = CSW_C4
				attacker = 0

				// We need to call the death function manually b/c DeathMsg isn't broadcasted when the bomb explodes and kills someone
				if (get_user_health(victim) - damage < 0 ){
					on_Death(victim, attacker, wpnindex, 0)
				}

				#if DEBUG
					client_print(victim, print_chat, "### You were just attacked by the bomb for %d damage (%s)", damage, szClassName)
				#endif
			}
		}
	}
#endif

	if(p_data_b[attacker][PB_MOLE] && (p_data[victim][P_ITEM2]==ITEM_PROTECTANT || p_data_b[victim][PB_WARDENBLINK])){	
		set_user_actualhealth(victim,get_user_health(victim)+damage, "client_damage, protectant")
		client_print(victim,print_chat,"%L",victim,"SHOT_DEFLECTED",g_MOD)
		return PLUGIN_HANDLED
	}

	if( p_data_b[victim][PB_GODMODE] ){
		if( p_data[attacker][P_ITEM] == ITEM_NECKLACE && attacker != victim ){
			if( get_user_health(victim) - damage <= 2048 ){
				WAR3_damage(victim, attacker, 3000, wpnindex, hitplace)
			}
		}
		else{
			set_user_actualhealth(victim, get_user_health(victim) + damage, "client_damage, godmode")
		}

		return PLUGIN_CONTINUE
	}

	if (victim==attacker)
		return PLUGIN_HANDLED

	new tempdamage = 0

	// Bot should "auto" cast his/her ultimate when attacking
	
	if( attacker > 0 ){
		if (is_user_bot(attacker) && p_data[attacker][P_ULTIMATE]){
			cmd_Ultimate(attacker)
		}
	}


	// **************************************************
	// Single Victim Ability (Evasion)
	// This is here b/c we do not want war3 damage being done before we take the health away
	// **************************************************

	if ( Verify_Race(victim, RACE_ELF) ){

		// Evasion
		if ( Verify_Skill(victim, RACE_ELF, SKILL1) && p_data_b[victim][PB_EVADENEXTSHOT] ) {
			new healthadjustment = -1024
			new iHealth = get_user_health(victim)

			set_user_actualhealth(victim, iHealth + damage + healthadjustment, "client_damage, evade adjustment")
			#if DEBUG
				client_print(victim, print_chat,"### You just evaded %d damage", damage)
			#endif

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

			Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 0, 0, 255, iglow[victim][2])

			p_data_b[victim][PB_EVADENEXTSHOT] = false
		}
	}

	// **************************************************
	// Attacker Abilities
	// **************************************************

	// Undead Scourge

	if ( Verify_Race(attacker, RACE_UNDEAD) ){

		// Vampiric Aura
		if ( Verify_Skill(attacker, RACE_UNDEAD, SKILL1) ) {
			new health = get_user_health(attacker)
			new maxHealth = get_user_maxhealth(attacker)

			tempdamage = floatround(float(damage) * p_vampiric[p_data[attacker][P_SKILL1]-1])
			
			if(health < maxHealth){
				if( health + tempdamage > maxHealth )
					set_user_actualhealth(attacker, maxHealth, "client_damage, Vamp Aura - max")
				else
					set_user_actualhealth(attacker, health + tempdamage, "client_damage, Vamp Aura")
			}
			
			if (iglow[attacker][1] < 1){
				new parm[2]
				parm[0] = attacker
				set_task(0.01,"glow_change",TASK_GLOW+attacker,parm,2)
			}
			iglow[attacker][1] += tempdamage
			iglow[attacker][0] = 0
			iglow[attacker][2] = 0
			iglow[attacker][3] = 0
			if (iglow[attacker][1]>MAXGLOW)
				iglow[attacker][1]=MAXGLOW

			Create_ScreenFade(attacker, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[attacker][1])
		}
	}

	// Human Alliance
	else if ( Verify_Race(attacker, RACE_HUMAN) ){
		if ( Verify_Skill(attacker, RACE_HUMAN, SKILL3) ) {
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_bash[p_data[attacker][P_SKILL3]-1] && get_user_maxspeed(victim) > 10 && !p_data_b[victim][PB_SLOWED]){		// Cannot bash if already bashed or frosted
				new normalspeed = floatround(get_user_maxspeed(victim))
				set_user_maxspeed(victim,1.0)
				p_data_b[victim][PB_STUNNED]=true
				new parm[2]
				parm[0]=victim
				parm[1]=normalspeed
				set_task(1.0,"reset_maxspeed",TASK_RESETSPEED+victim,parm,2)

				if (iglow[victim][3] < 1){
					parm[0] = victim
					parm[1] = 0
					set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
				}
				iglow[victim][3] += 100
				iglow[victim][0] = 0
				iglow[victim][1] = 0
				iglow[victim][2] = 0
				if (iglow[victim][3]>MAXGLOW)
					iglow[victim][3]=MAXGLOW

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 255, 255, iglow[victim][3])
			}
		}
	}

	// Orcish Horde
	else if ( Verify_Race(attacker, RACE_ORC) ){

		// Critical Strike
		if ( Verify_Skill(attacker, RACE_ORC, SKILL1)) {
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_critical[p_data[attacker][P_SKILL1]-1]){
				tempdamage = floatround(float(damage) * p_data[attacker][P_SKILL1])

				WAR3_damage(victim, attacker, tempdamage, wpnindex, hitplace)
				if (iglow[victim][0] < 1){
					new parm[2]
					parm[0] = victim
					set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
				}
				iglow[victim][0] += tempdamage
				iglow[victim][1] = 0
				iglow[victim][2] = 0
				iglow[victim][3] = 0
				if (iglow[victim][0]>MAXGLOW)
					iglow[victim][0]=MAXGLOW

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 0, 0, iglow[victim][0])
			}
		}

		// Critical Grenade
		if ( Verify_Skill(attacker, RACE_ORC, SKILL2) ){		
			new bool:allow=true

			if(p_data[attacker][P_ITEM2]==ITEM_GLOVES){
				if(iCvar[FT_GLOVE_ORC_DAMAGE])
					allow=true
				else
					allow=false
			}

		#if MOD == 0
			if(g_notAllowHE)
				allow=false
			if (wpnindex == CSW_HEGRENADE && allow){
		#endif
		#if MOD == 1
			if ((wpnindex == DODW_HANDGRENADE || wpnindex == DODW_STICKGRENADE) && allow){
		#endif
				tempdamage = floatround(float(damage) * p_grenade[p_data[attacker][P_SKILL2]-1])
				WAR3_damage(victim, attacker, tempdamage, wpnindex, hitplace)
				new name[32]
				get_user_name(victim,name,31)
				if (iglow[victim][0] < 1){
					new parm[2]
					parm[0] = victim
					set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
				}
				iglow[victim][0] += tempdamage
				iglow[victim][1] = 0
				iglow[victim][2] = 0
				iglow[victim][3] = 0
				if (iglow[victim][0]>MAXGLOW)
					iglow[victim][0]=MAXGLOW

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 0, 0, iglow[victim][0])
			}

		}
	}

	// Night Elf
	else if ( Verify_Race(attacker, RACE_ELF) ){

		// Trueshot
		if ( Verify_Skill(attacker, RACE_ELF, SKILL3) ){
			tempdamage = floatround(float(damage) * p_trueshot[p_data[attacker][P_SKILL3]-1])

			WAR3_damage(victim, attacker,tempdamage,wpnindex, hitplace)
			if (iglow[victim][0] < 1){
				new parm[2]
				parm[0] = victim
				set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
			}
			iglow[victim][0] += 2*tempdamage
			iglow[victim][1] = 0
			iglow[victim][2] = 0
			iglow[victim][3] = 0
			if (iglow[victim][0]>MAXGLOW)
				iglow[victim][0]=MAXGLOW

			Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 0, 0, iglow[victim][0])
		}
	}
	
	// Blood Mage
	else if ( Verify_Race(attacker, RACE_BLOOD) ){
		if ( Verify_Skill(attacker, RACE_BLOOD, SKILL2) ){
			new Float:randomnumber = random_float(0.0,1.0)

			if (randomnumber <= p_banish[p_data[attacker][P_SKILL2]-1]){
				if(p_data[attacker][P_SKILL2] > 2){
					user_slap(victim,0)
					user_slap(victim,0)
					user_slap(victim,0)
					WAR3_damage(victim, attacker, 9, wpnindex, hitplace)
				}
				else{
					user_slap(victim,0)
					user_slap(victim,0)
					user_slap(victim,0)
					WAR3_damage(victim, attacker, 7, wpnindex, hitplace)
				}

				emit_sound(victim,CHAN_STATIC, SOUND_BANISH, 1.0, ATTN_NORM, 0, PITCH_NORM)

				if (iglow[victim][3] < 1){
					new parm[2]
					parm[0] = victim
					parm[1] = 0
					set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
				}
				iglow[victim][3] += 100
				iglow[victim][0] = 0
				iglow[victim][1] = 0
				iglow[victim][2] = 0
				if (iglow[victim][3]>MAXGLOW)
					iglow[victim][3]=MAXGLOW

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 255, 255, iglow[victim][3])
			}
		}

		// Siphon Mana
		if ( Verify_Skill(attacker, RACE_BLOOD, SKILL3) ){
			new money = floatround( p_mana[p_data[attacker][P_SKILL3]-1] * get_user_money(victim) )  

			set_user_money(attacker,get_user_money(attacker)+money,1)
			set_user_money(victim,get_user_money(victim)-money,1)

			if (iglow[attacker][1] < 1){
				new parm[2]
				parm[0] = attacker
				set_task(0.01,"glow_change",TASK_GLOW+attacker,parm,2)
			}
			iglow[attacker][1] += damage
			iglow[attacker][0] = 0
			iglow[attacker][2] = 0
			iglow[attacker][3] = 0
			if (iglow[attacker][1]>MAXGLOW)
				iglow[attacker][1]=MAXGLOW

			Create_ScreenFade(attacker, (1<<10), (1<<10), (1<<12), 144, 58, 255, iglow[attacker][1])	// Purplish color
		}
	}

	// Shadow Hunter
	else if ( Verify_Race(attacker, RACE_SHADOW) ){

		// Hex
		if ( Verify_Skill(attacker, RACE_SHADOW, SKILL2) ) {
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_hex[p_data[attacker][P_SKILL2]-1] && p_data[victim][P_SKINCHANGED] != SKIN_HEX){
				changeskin(victim,SKIN_HEX)
				new parm[2]
				parm[0]=victim
				parm[1]=10		// 10 * 4 = 40 seconds (time hex lasts)
				_Skill_Hex(parm)

				if (iglow[victim][3] < 1){
					parm[0] = victim
					parm[1] = 0
					set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
				}
				iglow[victim][3] += 100
				iglow[victim][0] = 0
				iglow[victim][1] = 0
				iglow[victim][2] = 0
				if (iglow[victim][3]>MAXGLOW)
					iglow[victim][3]=MAXGLOW

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 255, 255, iglow[victim][3])
			}
		}
	}

	// Warden
	else if ( Verify_Race(attacker, RACE_WARDEN) ){
		
		// Shadow Strike
		if ( Verify_Skill(attacker, RACE_WARDEN, SKILL3) ){
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_shadow[p_data[attacker][P_SKILL3]-1] && p_data[attacker][P_SHADOWCOUNT]>0 && is_user_alive(victim)){
				new origin[3], attackerorigin[3]
				get_user_origin(victim,origin)
				get_user_origin(attacker,attackerorigin)
				
				if(!g_mapDisabled)
					Create_TE_SPRITETRAIL(attackerorigin, origin, g_sShadow, 50, 15, 1, 2, 6 )

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[victim][2])

				emit_sound(victim,CHAN_STATIC, SOUND_SHADOWSTRIKE, 1.0, ATTN_NORM, 0, PITCH_NORM)

				p_data[attacker][P_SHADOWCOUNT]--

				tempdamage = 10
				WAR3_damage(victim, attacker,tempdamage,CSW_SHADOW, hitplace)
			}
		}
	}

	// Crypt Lord
	else if ( Verify_Race(attacker, RACE_CRYPT) ){
		
		// Orb of Annihilation
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= p_orb[p_data[attacker][P_LEVEL]]){
			new origin[3]
			get_user_origin(victim, origin)
			
			origin[2] -= 20
	
			Create_TE_SPRITE(origin, g_sWave, 10, 200)

			emit_sound(victim, CHAN_STATIC, SOUND_ANNIHILATION, 1.0, ATTN_NORM, 0, PITCH_NORM)

			WAR3_damage(victim, attacker, ORB_DAMAGE, CSW_ORB, hitplace)
		}

		// Carrion Beetle
		if ( Verify_Skill(attacker, RACE_CRYPT, SKILL3) ){
			if (random_float(0.0,1.0) <= p_carrion[p_data[attacker][P_SKILL3]-1]&& p_data[attacker][P_CARRIONCOUNT]>0 && is_user_alive(victim)){
				new origin[3], attackerorigin[3]
				get_user_origin(victim,origin)
				get_user_origin(attacker,attackerorigin)

				if(!g_mapDisabled)
					Create_TE_SPRITETRAIL(attackerorigin, origin, g_sBeetle, 15, 15, 1, 2, 6 )

				emit_sound(victim,CHAN_STATIC, SOUND_CARRION, 1.0, ATTN_NORM, 0, PITCH_NORM)

				p_data[attacker][P_CARRIONCOUNT]--

				tempdamage = 10
				WAR3_damage(victim, attacker,tempdamage, CSW_CARRION, hitplace)
			}
			
		}
		
		// Impale
		if ( Verify_Skill(attacker, RACE_CRYPT, SKILL1) ){

			if (random_float(0.0,1.0) <= p_impale[p_data[attacker][P_SKILL1]-1]){
				emit_sound(victim,CHAN_STATIC, SOUND_IMPALE, 1.0, ATTN_NORM, 0, PITCH_NORM)
				
				new param[2]
				param[0] = victim
				param[1] = 0

				_Skill_Impale(param)

				Create_ScreenShake(victim, (255<< 14), (10 << 14), (255<< 14))
			}
		}
	}

	// Item abilities

	// Claws of Attack
	if ( p_data[attacker][P_ITEM] == ITEM_CLAWS ){	
		WAR3_damage(victim, attacker, iCvar[FT_CLAW], wpnindex, hitplace)

		if (iglow[victim][0] < 1){
			new parm[2]
			parm[0] = victim
			set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
		}
		iglow[victim][0] += 2*tempdamage
		iglow[victim][1] = 0
		iglow[victim][2] = 0
		iglow[victim][3] = 0
		if (iglow[victim][0]>MAXGLOW)

		iglow[victim][0]=MAXGLOW

		Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 0, 0, iglow[victim][0])
	}

	// Mask of Death
	else if (p_data[attacker][P_ITEM] == ITEM_MASK){
		new iHealth = get_user_actualhealth(attacker)

		tempdamage = floatround(float(damage) * iCvar[FT_MASK_OF_DEATH])

		if ( iHealth + tempdamage > get_user_maxhealth(attacker) ){
			new iTotalHealth = get_user_health(attacker)

			if( iTotalHealth > 1500 )		// God Mode
				set_user_actualhealth(attacker, 2148, "client_damage, mask 1")
			else if( iTotalHealth > 500 )	// Evasion
				set_user_actualhealth(attacker, 1124, "client_damage, mask 2")
			else
				set_user_actualhealth(attacker, get_user_maxhealth(attacker), "client_damage, mask 3")
		}
		else
			set_user_actualhealth(attacker, get_user_health(attacker) + tempdamage, "client_damage, mask 4")

		if (iglow[attacker][1] < 1){
			new parm[2]
			parm[0] = attacker
			set_task(0.01,"glow_change",TASK_GLOW+attacker,parm,2)
		}
		iglow[attacker][1] += tempdamage
		iglow[attacker][0] = 0
		iglow[attacker][2] = 0
		iglow[attacker][3] = 0
		if (iglow[attacker][1]>MAXGLOW)
			iglow[attacker][1]=MAXGLOW

		Create_ScreenFade(attacker, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[attacker][1])
	}

	// Orb of Frost
	else if (p_data[attacker][P_ITEM] == ITEM_FROST){
		if (get_user_maxspeed(victim) > 10 && !p_data_b[victim][PB_SLOWED]){
			new normalspeed = floatround(get_user_maxspeed(victim))
			set_user_maxspeed(victim,fCvar[FT_FROST_SPEED])
			p_data_b[victim][PB_SLOWED]=true
			new parm[2]
			parm[0]=victim
			parm[1]=normalspeed
			set_task(1.0,"reset_maxspeed",TASK_RESETSPEED+victim,parm,2)

			if (iglow[victim][3] < 1){
				parm[0] = victim
				parm[1] = 0
				set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
			}
			iglow[victim][3] += 100
			iglow[victim][0] = 0
			iglow[victim][1] = 0
			iglow[victim][2] = 0
			if (iglow[victim][3]>MAXGLOW)
				iglow[victim][3]=MAXGLOW

			Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 255, 255, iglow[victim][3])
		}
	}

	// **************************************************
	// Victim Abilities
	// **************************************************

	// Night Elf
	if ( Verify_Race(victim, RACE_ELF) ){

		// Evasion
		if ( Verify_Skill(victim, RACE_ELF, SKILL1) ) {
			new healthadjustment = 0
			new Float:randomnumber = random_float(0.0,1.0)
			new iHealth = get_user_health(victim)
			
			#if DEBUG
				console_print(victim, "##### Evasion Chance: %f", randomnumber*100)
			#endif

			if (randomnumber <= p_evasion[p_data[victim][P_SKILL1]-1]){
				healthadjustment = 1024

				p_data_b[victim][PB_EVADENEXTSHOT] = true

				set_user_actualhealth(victim, iHealth + healthadjustment, "client_damage, evade adjustment")

				#if DEBUG
					client_print(victim, print_chat,"### You will evade the next shot", damage)
					console_print(victim,"### You will evade the next shot", damage)
				#endif

			}
		}
		
		// Thorns Aura
		if ( Verify_Skill(victim, RACE_ELF, SKILL2) && attacker > 0) {
			tempdamage = floatround(float(damage) * p_thorns[p_data[victim][P_SKILL2]-1])
			
			WAR3_damage(attacker, victim,tempdamage,CSW_THORNS, hitplace)

			if (iglow[attacker][0] < 1){
				new parm[2]
				parm[0] = attacker
				set_task(0.01,"glow_change",TASK_GLOW+attacker,parm,2)
			}
			iglow[attacker][0] += 3*tempdamage
			iglow[attacker][1] = 0
			iglow[attacker][2] = 0
			iglow[attacker][3] = 0
			if (iglow[attacker][0]>MAXGLOW)
				iglow[attacker][0]=MAXGLOW

			Create_ScreenFade(attacker, (1<<10), (1<<10), (1<<12), 255, 0, 0, iglow[attacker][0])
		}
	}

	// Blood Mage
	else if ( Verify_Race(victim, RACE_BLOOD) ){

		// Resistant Skin
		set_user_actualhealth(victim, get_user_health(victim) + floatround(float(damage) * p_resistant[p_data[victim][P_LEVEL]]), "client_damage, resistant skin")

	}

	// Shadow Hunter
	else if ( Verify_Race(victim, RACE_SHADOW) ){

		// Unstable Concoction
		new Float:randomnumber = random_float(0.0,1.0)

		if (randomnumber <= p_concoction[p_data[victim][P_LEVEL]]){
			new origin[3], k, initorigin[3], axisorigin[3]
				
			get_user_origin(victim, origin)

			emit_sound(victim,CHAN_STATIC, SOUND_CONCOCTION_CAST, 1.0, ATTN_NORM, 0, PITCH_NORM)

			initorigin[0] = origin[0]
			initorigin[1] = origin[1]
			initorigin[2] = origin[2] - 16

			axisorigin[0] = origin[0]
			axisorigin[1] = origin[1]
			axisorigin[2] = origin[2] + CONCOCTION_RADIUS

			for (k=0;k<200;k=k+25){
				Create_TE_BEAMCYLINDER(origin, initorigin, axisorigin, g_sSpriteTexture, 0, 0, 9, 20, 0, 188, 220, 255, 255, 0)

				initorigin[2] += 25
			}

			new players[32], numberofplayers, targetorigin[3], i

			if (get_user_team(victim) == CTS)
				get_players(players, numberofplayers, "ae", "TERRORIST")
			else
				get_players(players, numberofplayers, "ae", "CT")
			
			for(i=0;i<numberofplayers;i++){
				get_user_origin(players[i], targetorigin)
				if( get_distance(origin, targetorigin) <= CONCOCTION_RADIUS ){
					WAR3_damage(players[i], victim, CONCOCTION_DAMAGE, CSW_CONCOCTION, hitplace)
					emit_sound(victim,CHAN_STATIC, SOUND_CONCOCTION_HIT, 1.0, ATTN_NORM, 0, PITCH_NORM)
				}
			}
		}

	}


	// Crypt Lord
	else if ( Verify_Race(victim, RACE_CRYPT) ){

		// Spiked Carapace
		if ( Verify_Skill(victim, RACE_CRYPT, SKILL2) && attacker > 0){						
			tempdamage = floatround(float(damage) * p_spiked[p_data[victim][P_SKILL2]-1])

			WAR3_damage(attacker, victim, tempdamage, CSW_CARAPACE, hitplace)

		#if MOD == 0
			if(get_user_armor(victim)<101)
				set_user_armor(victim,get_user_armor(victim)+damage)
		#endif

			if (iglow[attacker][0] < 1){
				new parm[2]
				parm[0] = attacker
				set_task(0.01,"glow_change",TASK_GLOW+attacker,parm,2)
			}
			iglow[attacker][0] += 3*tempdamage
			iglow[attacker][1] = 0
			iglow[attacker][2] = 0
			iglow[attacker][3] = 0
			if (iglow[attacker][0]>MAXGLOW)
				iglow[attacker][0]=MAXGLOW

			Create_ScreenFade(attacker, (1<<10), (1<<10), (1<<12), 255, 0, 0, iglow[attacker][0])
		}
	}


	return PLUGIN_CONTINUE
}

public on_Death(victim, killer, wpnindex, headshot){
	#if ADVANCED_DEBUG
		writeDebugInfo("on_Death",victim)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	#if DEBUG
		client_print(victim, print_chat,"*** Regular Death : %d", p_data_b[victim][PB_DIEDLASTROUND])
	#endif

	if(p_data_b[victim][PB_DIEDLASTROUND])
		return PLUGIN_CONTINUE


	if(killer > 0 )
		set_user_money(killer, get_user_money(killer)+300,1)

	WAR3_death_victim(victim, killer)
	
	XP_onDeath(victim, killer, wpnindex, headshot)

	return PLUGIN_CONTINUE
}

public on_DeathMsg(){
	#if ADVANCED_DEBUG
		writeDebugInfo("on_DeathMsg",read_data(2))
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new killer = read_data(1)
	new victim = read_data(2)
	new headshot = read_data(3)
	new wpnindex
	get_user_attacker(victim, wpnindex)

	on_Death(victim, killer, wpnindex, headshot)
	
	return PLUGIN_CONTINUE
}

public on_CurWeapon(id) {
	#if ADVANCED_DEBUG
		writeDebugInfo("on_CurWeapon",id)
	#endif

	// read_data(1) = isActive?
	// read_data(2) = weapon index
	// read_data(3) = ammo
	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			return PLUGIN_CONTINUE
		}
	}

	if (p_data[id][P_ITEM2]==ITEM_GLOVES){
		new wpnList[32] = 0 
		new number  = 0
		new foundNade = false 
		get_user_weapons(id,wpnList,number) 
		for (new i = 0;i < number && !foundNade;i++) { 
		#if MOD == 0
			if (wpnList[i] == CSW_HEGRENADE) 
				foundNade = true 
		#endif
		#if MOD == 1
			if (wpnList[i] == DODW_HANDGRENADE || wpnList[i] == DODW_STICKGRENADE) 
				foundNade = true 
		#endif
		}
		
		if(!p_data_b[id][PB_NADEJUSTRECEIVED] && !foundNade){
			new parm[2]
			parm[0] = id
			parm[1] = iCvar[FT_GLOVE_TIMER]

			p_data_b[id][PB_NADEJUSTRECEIVED]=true
			_Item_Glove(parm)
		}
	}

	new clipamount = 0
	new ammoamount = 0
	new weaponnum = 0
	weaponnum = get_user_weapon(id,clipamount,ammoamount)

	#if MOD == 1
		if (weaponnum==DODW_HANDGRENADE || weaponnum==DODW_STICKGRENADE){
			if( Verify_Skill(id, RACE_ORC, SKILL2) ){
				dod_set_fuse(id,FUSE_SET,2.0,FT_NEW)
			}
		}

	#endif

	// Invisibility

	if ( Verify_Skill(id, RACE_HUMAN, SKILL1)){
	#if MOD == 0
		if (weaponnum==CSW_KNIFE){
	#endif
	#if MOD == 1
		if (weaponnum==DODW_AMERKNIFE || weaponnum==DODW_GERKNIFE || weaponnum==DODW_SPADE){
	#endif
			set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,p_invisibility[p_data[id][P_SKILL1]-1]/2)
			p_data_b[id][PB_KNIFESELECTED]=true
		}
		else{
			set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,p_invisibility[p_data[id][P_SKILL1]-1])
			p_data_b[id][PB_KNIFESELECTED]=false
		}
	}
	else if (p_data[id][P_ITEM]==ITEM_CLOAK){
	#if MOD == 0
		if (weaponnum==CSW_KNIFE){
	#endif
	#if MOD == 1
		if (weaponnum==DODW_AMERKNIFE || weaponnum==DODW_GERKNIFE || weaponnum==DODW_SPADE){
	#endif
			set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,iCvar[FT_CLOAK]/2)
			p_data_b[id][PB_KNIFESELECTED]=true
		}
		else{
			set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,iCvar[FT_CLOAK])
			p_data_b[id][PB_KNIFESELECTED]=false
		}
	}
	else
		set_user_rendering(id)

	new parm[1]
	parm[0]=id
	unholyspeed(parm)

	return PLUGIN_CONTINUE
}

public on_ResetHud(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("on_ResetHud",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id))
			return PLUGIN_CONTINUE
	}

	if(is_user_bot(id)){
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= iCvar[FT_BOT_BUY_ITEM]){
			new num = random_num(1,2)
			if (num == 1)
				_menu_Shopmenu_One(id, random_num(0,8))
			else
				_menu_Shopmenu_Two(id, random_num(0,8))
		}
		if (randomnumber <= 0.06){
			p_data[id][P_XP]=xplevel[floatround(random_float(0.0,3.16)*random_float(0.0,3.16))]
			p_data[id][P_RACE] = random_num(1,iCvar[FT_RACES])
		}
	}

	new parm[2]
	parm[0] = id

	// Remove any reset_maxspeeds occuring (could cause a person to move during freezetime)
	if (task_exists(TASK_RESETSPEED+id)){			
		remove_task(TASK_RESETSPEED+id)
	}

	// The user should not be frozen at the start of the round	
	p_data_b[id][PB_STUNNED] = false
	p_data_b[id][PB_SLOWED] = false

	// Remove Hex
	if (task_exists(TASK_JUMPER+id)){
		changeskin(id,SKIN_RESET)
		remove_task(TASK_JUMPER+id)
	}
	
	if (task_exists(TASK_LIGHTSEARCH+id)){
		remove_task(TASK_LIGHTSEARCH+id)
		p_data_b[id][PB_ISSEARCHING]=false
		icon_controller(id,ICON_HIDE)
	}

	if (task_exists(TASK_SEARCHTARGET+id)){
		remove_task(TASK_SEARCHTARGET+id)
		p_data_b[id][PB_ISSEARCHING]=false
		icon_controller(id,ICON_HIDE)
	}


	if(is_user_alive(id)){
		p_data_b[id][PB_JUSTJOINED] = false
	}

	if(p_data[id][P_RACE]==0){
		new parameter[1]
		parameter[0]=id
		set_task(0.3,"getuserinput",TASK_GETINPUT+id,parameter,1)
	}

	if(!g_randomizeCalled){
		race9_randomize()
		g_randomizeCalled = true
	}

	if(p_data_b[id][PB_GAMECOMMENCING]){
		p_data[id][P_ITEM] = 0
		p_data[id][P_ITEM2] = 0
		p_data_b[id][PB_DIEDLASTROUND]=false
		p_data_b[id][PB_GAMECOMMENCING]=false
		#if MOD == 0
			p_data[id][P_HECOUNT] = 0
			p_data[id][P_FLASHCOUNT]=0
		#endif
	}
	#if MOD == 0
		if(iCvar[FT_BUYTIME] && !g_buyCalled){
			set_task(get_cvar_float("mp_buytime")*60.0,"_WAR3_set_buytime",TASK_BUYTIME)
			g_buyCalled = true
		}
	#endif
	icon_controller(id,ICON_HIDE)

	p_data_b[id][PB_TOBEREVIVED] = false
	p_data_b[id][PB_CHANGINGTEAM] = false

	#if MOD == 1
		if(p_data[id][P_ITEM] == ITEM_ANKH){
			p_data_b[id][PB_REINCARNATION_DELAY] = false
			SKILL_Reincarnation(id)
		}
		else if( Verify_Skill(id, RACE_ORC, SKILL3) ){
			new Float:randomnumber = random_float(0.0,1.0)
			if(randomnumber <= p_ankh[p_data[id][P_SKILL3]-1])
				SKILL_Reincarnation(id)
		}
	#endif

	#if MOD == 0
		if (p_data_b[id][PB_PLAYERSPAWNED]){

			// Reset Serpent Wards
			if( Verify_Skill(id, RACE_SHADOW, SKILL3) ){
				p_data[id][P_SERPENTCOUNT] = p_serpent[p_data[id][P_SKILL3]-1]
			}

			icon_controller(id,ICON_SHOW)

			if(p_data_b[id][PB_STUNNED] || p_data_b[id][PB_SLOWED]){
				p_data_b[id][PB_STUNNED] = false
				p_data_b[id][PB_SLOWED] = false
			}

			new unholyparm[1]
			unholyparm[0]=id
			unholyspeed(unholyparm)

			if(p_data[id][P_ITEM2]!=ITEM_MOLE)
				p_data[id][P_ITEM2]=0

			p_data[id][P_ITEM]=0
			changeskin(id,SKIN_RESET)

			WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
			return PLUGIN_HANDLED
		}
	#endif

	p_data[id][P_HECOUNT]=0
	p_data[id][P_FLASHCOUNT]=0

	// Stop any cooldowns in effect	

	if (task_exists(TASK_UDELAY+id))		
		remove_task(TASK_UDELAY+id)

	// Start a new cooldown

	if (iCvar[FT_ULTIMATE_DELAY] > 0){
		p_data_b[id][PB_ULTIMATEUSED] = true
		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_DELAY]
	}
	else if(p_data[id][P_ULTIMATE]){
		p_data[id][P_ULTIMATEDELAY] = 0
		p_data_b[id][PB_ULTIMATEUSED] = false
	}

	_Ultimate_Delay(parm)

	if (p_data[id][P_RACE] == 9 && iCvar[FT_RACE9_RANDOM]){
		WAR3_Display_Level(id,DISPLAYLEVEL_SHOWRACE)
	}

	// User has a race selection pending, set it
	if (p_data[id][P_CHANGERACE]){
		WAR3_set_race(id, p_data[id][P_CHANGERACE])
	}	

	#if MOD == 0
		g_buyTime=true

		if (g_freezecalled==0){
			g_freezetime = true
			g_freezecalled = 1
		}

		g_hostageSaver = 0
		g_bombDefuser = 0

	#endif

	if (iCvar[FT_WARN_SUICIDE])
		p_data_b[id][PB_SUICIDEATTEMPT] = false

	// Weapon Reincarnation

	set_task(0.1, "weapon_controller", TASK_REINCARNATION+id, parm, 2)

	endround=false
	p_data_b[id][PB_ISBURNING] = false
	p_data[id][P_FLAMECOUNT]=0
	p_data_b[id][PB_WARDENBLINK] = false

	// Checks skills
	Skill_Check(id)

	// Fan of Knives Check

	if ( Verify_Skill(id, RACE_WARDEN, SKILL1) && is_user_alive(id) ){
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= p_fan[p_data[id][P_SKILL1]-1]){
			new fanparm[2]
			fanparm[0]=id
			fanparm[1]=7
			set_task(0.1,"_Item_Mole",TASK_FAN+id,fanparm,2)
		}
	}

	if (p_data_b[id][PB_RESETSKILLS]) {
		p_data[id][P_SKILL1]=0
		p_data[id][P_SKILL2]=0
		p_data[id][P_SKILL3]=0
		p_data[id][P_ULTIMATE]=0
		p_data[id][P_LEVEL] = 0
		p_data_b[id][PB_RESETSKILLS]=false
		WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
	}

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
	if (skillsused < p_data[id][P_LEVEL]){
		menu_Select_Skill(id,0)
	}
	return PLUGIN_CONTINUE
}

public on_GameRestart(){
	#if ADVANCED_DEBUG
		writeDebugInfo("on_GameRestart",0)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	XP_Save_All()

	new players[32], numplayers, id
	get_players(players, numplayers)
	for (new i=0; i<numplayers; i++){
		id=players[i]
		p_data_b[id][PB_GAMECOMMENCING]=true

		if(task_exists(TASK_UDELAY+id))
			remove_task(TASK_UDELAY+id)
		
		if(p_data[P_ITEM2][id])
			Item_Set_Helm(id,0)

		p_data[id][P_ITEM] = 0
		p_data[id][P_ITEM2] = 0
		p_data_b[id][PB_DIEDLASTROUND] = false
		p_data[id][P_RINGS]=0
		if(iCvar[MP_SAVEXP]==0){
			p_data[id][P_LEVEL]=0
			p_data[id][P_RACE]=0
			p_data[id][P_SKILL1]=0
			p_data[id][P_SKILL2]=0
			p_data[id][P_SKILL3]=0
			p_data[id][P_ULTIMATE]=0
			p_data[id][P_XP]=0
		}
		#if MOD == 0
			p_data[id][P_HECOUNT]=0
			p_data[id][P_FLASHCOUNT]=0
		#endif
	}

	return PLUGIN_CONTINUE
}