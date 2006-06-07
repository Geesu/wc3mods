// Forwards from the CSX module and DODX module
public grenade_throw( index, greindex, wId )
{

	if ( !WAR3_Check() )
	{
		return;
	}

	// Make sure the user has the skill and we actually have a grenade index
	if ( greindex && Verify_Skill( index, RACE_ORC, SKILL2 ) )
	{

		// Don't do extra damage if they have gloves
		if ( !get_pcvar_num( CVAR_wc3_glove_orc_damage ) && p_data[index][P_ITEM2] == ITEM_GLOVES )
		{
			return;
		}

		new bool:bShow = false;

		// Check for a Counter-Strike grenade
		if ( (g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO) && wId == CSW_HEGRENADE )
		{
			bShow = true;
		}

		// Check for a Day of Defeat grenade
		else if ( g_MOD == GAME_DOD && ( wId == DODW_HANDGRENADE || wId == DODW_STICKGRENADE ) )
		{
			bShow = true;
		}

		// Then draw it!
		if ( bShow )
		{
			Create_TE_BEAMFOLLOW( greindex, g_siTrail, 20, 10, 255, 32, 32, 196 );
		}
	}
	return;
}

#if MOD == 1
public client_damage(attacker,victim,damage,wpnindex,hitplace,TA){

	if (!warcraft3)
		return PLUGIN_CONTINUE


	call_damage(victim, attacker, damage, wpnindex, hitplace)

	return PLUGIN_CONTINUE
}
#endif

#if MOD == 0
public on_Damage(victim){

	if (!warcraft3)
		return PLUGIN_CONTINUE
	
	if ( victim < 1 || victim > MAXPLAYERS )
		return PLUGIN_CONTINUE

	new wpnindex = 0, hitplace = 0, attacker = get_user_attacker(victim,wpnindex,hitplace)
	new damage = read_data(2)

	call_damage(victim, attacker, damage, wpnindex, hitplace)

	return PLUGIN_CONTINUE
}
#endif

public call_damage(victim, attacker, damage, wpnindex, hitplace){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_data_b[victim][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if( attacker <= 0 || attacker > MAXPLAYERS || !p_data_b[attacker][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

#if MOD == 0
	new inflictor = entity_get_edict(victim, EV_ENT_dmg_inflictor)

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

				/*#if DEBUG
					client_print(victim, print_chat, "### You were just attacked by the bomb for %d damage (%s)", damage, szClassName)
				#endif*/
			}
		}
	}
#endif

	if(p_data_b[attacker][PB_MOLE] && p_data[victim][P_ITEM2]==ITEM_PROTECTANT){	
		set_user_health(victim,get_user_health(victim)+damage)
		client_print(victim,print_chat,"%s %L",g_MODclient,victim,"SHOT_DEFLECTED")
		return PLUGIN_HANDLED
	}

	if( p_data_b[victim][PB_GODMODE] ){
		if( ( p_data_b[attacker][PB_WARDENBLINK] ) && attacker != victim ){
			if( get_user_health(victim) - damage <= 2048 ){
				WAR3_damage(victim, attacker, 3000, wpnindex, hitplace)
			}
		}
		else{
			set_user_health(victim, get_user_health(victim) + damage)
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
	// Attacker Abilities
	// **************************************************

	// Undead Scourge

	if ( Verify_Race(attacker, RACE_UNDEAD) ){

		// Vampiric Aura
		if ( Verify_Skill(attacker, RACE_UNDEAD, SKILL1) && !p_data_b[attacker][PB_HEXED] ) {
			new health = get_user_health(attacker)
			new maxHealth = get_user_maxhealth(attacker)

			tempdamage = floatround(float(damage) * p_vampiric[p_data[attacker][P_SKILL1]-1])
			
			if(health < maxHealth){
				if( health + tempdamage > maxHealth )
					set_user_health(attacker, maxHealth)
				else
					set_user_health(attacker, health + tempdamage)
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

		// Bash - cannot occur if caster is hexed (hex disables abilities)
		if ( Verify_Skill(attacker, RACE_HUMAN, SKILL3) && !p_data_b[attacker][PB_HEXED] )
		{
			new Float:randomnumber = random_float( 0.0, 1.0 );

			// Cannot bash if already bashed or user is slowed
			if ( randomnumber <= p_bash[p_data[attacker][P_SKILL3]-1] && !SHARED_IsPlayerSlowed( victim ) )
			{		

				p_data_b[victim][PB_STUNNED] = true;
				SHARED_SetSpeed( victim );
				
				set_task( 1.0, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + victim );
				
				if (iglow[victim][3] < 1){
					new parm[2];
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
		if ( Verify_Skill(attacker, RACE_ORC, SKILL1) && !p_data_b[attacker][PB_HEXED]) {
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= CRITICAL_STRIKE_CHANCE){
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
		if ( Verify_Skill(attacker, RACE_ORC, SKILL2) && !p_data_b[attacker][PB_HEXED] ){		
			new bool:allow=true

			if(p_data[attacker][P_ITEM2]==ITEM_GLOVES){
				if(get_pcvar_num( CVAR_wc3_glove_orc_damage ))
					allow=true
				else
					allow=false
			}

		#if MOD == 0
			if (wpnindex == CSW_HEGRENADE && allow){
		#endif
		#if MOD == 1
			if ((wpnindex == DODW_HANDGRENADE || wpnindex == DODW_STICKGRENADE) && allow){
		#endif
				new iMaxHealth = get_user_maxhealth( victim );

				tempdamage = floatround( damage * p_grenade[p_data[attacker][P_SKILL2]-1] )

				if ( tempdamage + damage >= iMaxHealth )
					tempdamage = iMaxHealth - ( damage + 1 );

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
		if ( Verify_Skill(attacker, RACE_ELF, SKILL3) && !p_data_b[attacker][PB_HEXED] ){
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

		// Banish
		if ( Verify_Skill(attacker, RACE_BLOOD, SKILL2) && !p_data_b[attacker][PB_HEXED] ){
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
		if ( Verify_Skill(attacker, RACE_BLOOD, SKILL3) && !p_data_b[attacker][PB_HEXED] ){
			new money = floatround( p_mana[p_data[attacker][P_SKILL3]-1] * SHARED_GetUserMoney(victim) )  

			SHARED_SetUserMoney(attacker,SHARED_GetUserMoney(attacker)+money,1)
			SHARED_SetUserMoney(victim,SHARED_GetUserMoney(victim)-money,1)

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
		if ( Verify_Skill(attacker, RACE_SHADOW, SKILL2) && !p_data_b[attacker][PB_HEXED] ) {
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_hex[p_data[attacker][P_SKILL2]-1] && !p_data_b[victim][PB_HEXED]){

				/*  Set the rendering of the player */
				set_user_rendering(victim, kRenderFxDistort, 0, 0, 0, kRenderTransTexture, 0)
				
				/* Do not allow other renderings to take place like invisibility */
				p_data_b[victim][PB_CAN_RENDER] = false;
				
				p_data_b[victim][PB_HEXED] = true
				p_data_b[victim][PB_SLOWED] = true;
					
				/* Set the user's speed */
				SHARED_SetSpeed( victim );

				/* Hex will only last for 10 seconds */
				new parm[2]
				parm[0]=victim
				set_task( SH_HEX_LENGTH ,"_SH_RemoveHex", TASK_HEX + victim );

				emit_sound(victim, CHAN_STATIC, SOUND_HEX, 1.0, ATTN_NORM, 0, PITCH_NORM)

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 255, 255, 255, iglow[victim][3])
			}
		}
	}

	// Warden
	else if ( Verify_Race(attacker, RACE_WARDEN) ){
		
		// Shadow Strike
		if ( Verify_Skill(attacker, RACE_WARDEN, SKILL3) && !p_data_b[attacker][PB_HEXED] ){
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_shadow[p_data[attacker][P_SKILL3]-1]){
				if ( p_data[attacker][P_SHADOWCOUNT]>0 && is_user_alive(victim) ){
					new origin[3], attackerorigin[3]
					get_user_origin(victim,origin)
					get_user_origin(attacker,attackerorigin)
					
					Create_TE_SPRITETRAIL(attackerorigin, origin, g_sShadow, 50, 15, 1, 2, 6 )

					Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[victim][2])

					emit_sound(victim,CHAN_STATIC, SOUND_SHADOWSTRIKE, 1.0, ATTN_NORM, 0, PITCH_NORM)

					p_data[attacker][P_SHADOWCOUNT]--

					tempdamage = 10
					WAR3_damage(victim, attacker,tempdamage,CSW_SHADOW, hitplace)
				}
			}
		#if ADVANCED_STATS
			else{
				new WEAPON = CSW_SHADOW - CSW_WAR3_MIN
				iStatsShots[attacker][WEAPON]++
			}
		#endif
		}
	}

	// Crypt Lord
	else if ( Verify_Race(attacker, RACE_CRYPT) ){
		
		// Orb of Annihilation
		if ( !p_data_b[attacker][PB_HEXED] )
		{
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_orb[p_data[attacker][P_LEVEL]]){
				new origin[3]
				get_user_origin(victim, origin)
				
				origin[2] -= 20
		
				Create_TE_SPRITE(origin, g_sWave, 10, 200)

				emit_sound(victim, CHAN_STATIC, SOUND_ANNIHILATION, 1.0, ATTN_NORM, 0, PITCH_NORM)

				WAR3_damage(victim, attacker, ORB_DAMAGE, CSW_ORB, hitplace)
			}
		#if ADVANCED_STATS
			else{
				new WEAPON = CSW_ORB - CSW_WAR3_MIN
				iStatsShots[attacker][WEAPON]++
			}
		#endif
		}

		// Carrion Beetle
		if ( Verify_Skill(attacker, RACE_CRYPT, SKILL3) && !p_data_b[attacker][PB_HEXED] ){
			if (random_float(0.0,1.0) <= p_carrion[p_data[attacker][P_SKILL3]-1]){
				if ( p_data[attacker][P_CARRIONCOUNT]>0 && is_user_alive(victim) ){
					new origin[3], attackerorigin[3]
					get_user_origin(victim,origin)
					get_user_origin(attacker,attackerorigin)

					Create_TE_SPRITETRAIL(attackerorigin, origin, g_sBeetle, 15, 15, 1, 2, 6 )

					emit_sound(victim,CHAN_STATIC, SOUND_CARRION, 1.0, ATTN_NORM, 0, PITCH_NORM)

					p_data[attacker][P_CARRIONCOUNT]--

					tempdamage = 10
					WAR3_damage(victim, attacker,tempdamage, CSW_CARRION, hitplace)
				}
			}
		#if ADVANCED_STATS
			else{
				new WEAPON = CSW_CARRION - CSW_WAR3_MIN
				iStatsShots[attacker][WEAPON]++
			}
		#endif
		}
		
		// Impale
		if ( Verify_Skill(attacker, RACE_CRYPT, SKILL1) && !p_data_b[attacker][PB_HEXED] ){

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
	if ( p_data[attacker][P_ITEM] == ITEM_CLAWS && !p_data_b[attacker][PB_HEXED] ){	
		WAR3_damage(victim, attacker, get_pcvar_num( CVAR_wc3_claw ), wpnindex, hitplace)

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
	else if ( p_data[attacker][P_ITEM] == ITEM_MASK && !Verify_Skill(attacker, RACE_UNDEAD, SKILL1) && !p_data_b[attacker][PB_HEXED] ){
		new iHealth = get_user_actualhealth(attacker)

		tempdamage = floatround(float(damage) * get_pcvar_num( CVAR_wc3_mask ))

		if ( iHealth + tempdamage > get_user_maxhealth(attacker) ){
			set_user_health(attacker, get_user_maxhealth(attacker))
		}
		else
			set_user_health(attacker, get_user_health(attacker) + tempdamage)

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
	else if ( p_data[attacker][P_ITEM] == ITEM_FROST && !p_data_b[attacker][PB_HEXED] )
	{
		// Only slow them if they aren't slowed/stunned already
		if ( !SHARED_IsPlayerSlowed( victim ) )
		{

			p_data_b[victim][PB_SLOWED]	= true;
			SHARED_SetSpeed( victim );

			set_task( 1.0, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + victim );

			if (iglow[victim][3] < 1){
				new parm[2];
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
	if ( Verify_Race(victim, RACE_ELF) )
	{
		
		// Thorns Aura
		if ( Verify_Skill(victim, RACE_ELF, SKILL2) && attacker > 0 && !p_data_b[victim][PB_HEXED] ) {
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

			Create_ScreenFade(attacker, (1<<10), (1<<10), (1<<12), 0, 0, 255, iglow[attacker][0])
		}
	}

	// Blood Mage
	else if ( Verify_Race(victim, RACE_BLOOD) ){
		
		if ( !p_data_b[victim][PB_HEXED] )
		{
			// Resistant Skin
			set_user_health(victim, get_user_health(victim) + floatround(float(damage) * p_resistant[p_data[victim][P_LEVEL]]))
		}
	}

	// Shadow Hunter
	else if ( Verify_Race(victim, RACE_SHADOW) )
	{
		// Unstable Concoction Check
		SH_Concoction( victim, attacker );
	}


	// Crypt Lord
	else if ( Verify_Race(victim, RACE_CRYPT) ){

		// Spiked Carapace
		if ( Verify_Skill(victim, RACE_CRYPT, SKILL2) && attacker > 0 && !p_data_b[victim][PB_HEXED] ){						
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

	if (!warcraft3)
		return PLUGIN_CONTINUE

#if MOD == 0
	/* For some reason the damage passed by explosions is not actually correct
		(perhaps armor adjustments weren't done yet), so lets check */
	if ( is_user_alive(victim) && wpnindex == CSW_C4 )
		return PLUGIN_CONTINUE
	
	// Can't remember why this is here...
	if(p_data_b[victim][PB_DIEDLASTROUND])
	{
		return PLUGIN_CONTINUE;
	}

#endif

	WAR3_death_victim(victim, killer)
	
	XP_onDeath(victim, killer, wpnindex, headshot)

	// If we're playing DOD we need to get the user's origin when they die
	#if MOD == 1
		get_user_origin(victim, iReincarnation[victim]);
	#endif

	#if MOD == 0
		// Check for NightVision
		p_data_b[victim][PB_NIGHTVISION] = (cs_get_user_nvg( victim )) ? true : false;

		// Check if a user had a shield on death
		p_data_b[victim][PB_SHIELD] = (cs_get_user_shield( victim )) ? true : false;

		// See if the user has a defuse kit
		p_data_b[victim][PB_DEFUSE] = (cs_get_user_defuse( victim )) ? true : false;
	#endif
	
	return PLUGIN_CONTINUE
}

public on_DeathMsg(){

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

	// read_data(1) = isActive?
	// read_data(2) = weapon index
	// read_data(3) = ammo

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	// Record the last time a shot was fired
	fLastShotFired[id] = halflife_time();
	
	SHARED_SaveWeapons( id );

	ITEM_Glove_Check( id );

	#if MOD == 1
		new clipamount = 0, ammoamount = 0, weaponnum = 0
		weaponnum = get_user_weapon(id,clipamount,ammoamount)

		if (weaponnum==DODW_HANDGRENADE || weaponnum==DODW_STICKGRENADE){
			if( Verify_Skill(id, RACE_ORC, SKILL2) ){
				dod_set_fuse(id,FUSE_SET,2.0,FT_NEW)
			}
		}

	#endif

	// Check to see if we should set the player's invisibility
	SHARED_INVIS_Set( id );

	// Set the user's speed
	SHARED_SetSpeed( id );

	return PLUGIN_CONTINUE
}


public on_GameRestart(){

	if ( !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}

	XP_SaveAll()

	g_GameRestarting = true;

	return PLUGIN_CONTINUE
}


public on_WeapPickup(id){ 

	if ( !WAR3_Check() )
	{
		return;
	}

	SHARED_SaveWeapons( id );

	return;
}

public on_Drop(id)
{
	if ( !WAR3_Check() )
	{
		return;
	}

	SHARED_SaveWeapons( id );

	return;
}

public on_ResetHud(id)
{

	if ( !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}
	
	// Then this is the first call of the new round
	if ( endround )
	{
		EVENT_NewRound();
	}

	// ResetHUD can be called when the user is not alive, lets ignore those calls
	if ( !is_user_alive( id ) )
	{
		return PLUGIN_CONTINUE;
	}

	// This is the first time the user has spawned this round
	if ( !p_data_b[id][PB_HAS_SPAWNED] )
	{	
		EVENT_PlayerInitialSpawn( id );

		p_data_b[id][PB_HAS_SPAWNED] = true;
	}

	EVENT_PlayerSpawned( id );

	return PLUGIN_CONTINUE;
}

// Function is called when the user is spawned at the START of each round (called before EVENT_PlayerSpawned)
public EVENT_PlayerInitialSpawn( id )
{

	// Bot options
	if ( is_user_bot(id) )
	{
		// Give the bot some random XP if we're saving XP
		if ( get_pcvar_num( CVAR_wc3_save_xp ) && !p_data[id][P_XP] )
		{
			p_data[id][P_XP] = xplevel[floatround(random_float(0.0,3.16)*random_float(0.0,3.16))];
		}

		if ( !p_data[id][P_RACE] )
		{
			p_data[id][P_RACE] = random_num(1, get_pcvar_num( CVAR_wc3_races ));
		}
	}

	// We only want to do this here for CS/CZ... in DOD it should be done on every spawn
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Reset user skills if we need to (returns 1 if skills were reset)
		if ( WC3_ResetSkills( id ) )
		{
			return;
		}
	}

	// Start a new cooldown
	new iUltDelay = get_pcvar_num( CVAR_wc3_ult_delay );
	if ( iUltDelay > 0)
	{
		p_data_b[id][PB_ULTIMATEUSED]	= true;
		p_data[id][P_ULTIMATEDELAY]		= iUltDelay;
	}
	else if ( p_data[id][P_ULTIMATE] )
	{
		p_data[id][P_ULTIMATEDELAY]		= 0;
		p_data_b[id][PB_ULTIMATEUSED]	= false;
	}
	_ULT_Delay( id );
	
	// We only want to do this here for CS/CZ... in DOD it should be done on every spawn
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// User has a race selection pending, set it
		if ( p_data[id][P_CHANGERACE] )
		{
			WC3_SetRace( id, p_data[id][P_CHANGERACE] );
		}
	}

	// Display the new Chameleon skills for the round
	if ( p_data[id][P_RACE] == 9 && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		WC3_ShowRaceInfo( id );
	}

	return;
}

// Function is called everytime a user spawns (called after EVENT_PlayerInitialSpawn)
public EVENT_PlayerSpawned( id )
{
	// Find out if they need to choose a race or select a skill
	set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + id );
	
	// Check to see if they should still have some items that they lost b/c of respawning (i.e. ankh + respawn scroll)
	ITEM_GiveItemBackFromDeath( id );

	// User isn't changing a team if they just spawned
	p_data_b[id][PB_CHANGINGTEAM]	= false;
	
	// Reset suicide attempt
	p_data_b[id][PB_SUICIDEATTEMPT] = false;
	
	// User should not be burning
	p_data_b[id][PB_ISBURNING]		= false;

	// Reset Blink
	p_data_b[id][PB_WARDENBLINK]	= false;

	// We only want to do this here for DOD
	if ( g_MOD == GAME_DOD )
	{
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
	}

	// Set the user's Invisibility
	SHARED_INVIS_Set( id );

	// User's need their skillz
	Skill_Check( id );

	// The user should not be frozen when they spawn
	SHARED_ResetMaxSpeed( id );

	// Reset the user's skin
	SHARED_ChangeSkin( id, SKIN_RESET );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		p_data[id][P_HECOUNT]		= 0;
		p_data[id][P_FLASHCOUNT]	= 0;
		
		// If we need to give the user their weapons back, then lets
		SHARED_CS_Reincarnation( id );
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{

		// Should the user be reincarnated ??
		if ( g_ItemLastOwned[0][id] == ITEM_ANKH )
		{
			// We don't want to skip since the user has this item
			p_data_b[id][PB_REINCARNATION_SKIP] = false;

			SHARED_DOD_Reincarnation( id );
		}
		else if ( Verify_Skill(id, RACE_ORC, SKILL3) )
		{
			if ( random_float( 0.0, 1.0 ) <= p_ankh[p_data[id][P_SKILL3]-1] )
			{
				SHARED_DOD_Reincarnation( id );
			}

		}
	}

	// Check the user's items to see if something should be done
	//ITEM_Check( id );
	
	// Should the user mole from fan of knives or an item?
	if ( g_ItemLastOwned[1][id] == ITEM_MOLE || ( Verify_Skill(id, RACE_WARDEN, SKILL1) && random_float(0.0,1.0) <= p_fan[p_data[id][P_SKILL1]-1] ) )
	{
		set_task( 0.1, "_SHARED_Mole", TASK_MOLE + id );
	}


	// If the user is a bot they should have a chance to buy an item
	if ( is_user_bot( id ) )
	{
		if ( random_float(0.0,1.0) <= get_pcvar_num( CVAR_wc3_bot_buy_item ) )
		{
			( random_num( 1, 2 ) == 1 ) ? _menu_Shopmenu_One( id, random_num( 0, 8 ) ) : _menu_Shopmenu_Two( id, random_num( 0, 8 ) );
		}
	}

	p_data_b[id][PB_DIEDLASTROUND]	= false;
}

// Function is called ONCE at the start of a new round BEFORE user's spawn
public EVENT_NewRound()
{
	
	// User's have not spawned yet, so lets do some pre-spawn things
	new players[32], numplayers, i;
	get_players( players, numplayers, "a" );
	for ( i = 0; i < numplayers; i++ )
	{
		EVENT_JustBeforeSpawn( players[i] );
	}

	// If someone restarted the game, then lets reset war3
	if ( g_GameRestarting )
	{
		WC3_ResetGame();
	}
	
	// Randomize Chameleon if we need to
	CHAM_Randomize();

	// have "fake" ultimate delay
	g_iUltimateDelay = get_pcvar_num( CVAR_wc3_ult_delay );
	_ULT_Delay( 0 );

	// We need to determine when the buytime is up
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( get_pcvar_num( CVAR_wc3_buy_time ) )
		{
			g_buyTime = true;
			set_task( get_cvar_float("mp_buytime") * 60.0, "_WAR3_set_buytime", TASK_BUYTIME );
		}

		if ( !g_freezeCalled )
		{
			g_freezeTime	= true;
			g_freezeCalled	= true;
		}

		g_hostageSaver		= 0;
		g_bombDefuser		= 0;
	}

	endround = false;
}

// Called when a user looks somewhere
public TRIGGER_TraceLine( Float:v1[3], Float:v2[3], noMonsters, pentToSkip )
{
	new iAttacker = pentToSkip;
	new iVictim = get_tr(TR_pHit);
	new iHitZone = (1 << get_tr(TR_iHitgroup));
	

	// Make sure we have a valid victim
	if ( 0 < iVictim <= MAXPLAYERS )
	{
		// This is a check for ultimates that need to "search" for a target
		if ( 0 < iAttacker <= MAXPLAYERS && p_data_b[iAttacker][PB_ISSEARCHING] )
		{

			// Now lets make sure the person he's looking at isn't immune and isn't on the same team
			if ( get_user_team( iAttacker ) != get_user_team( iVictim ) && !ULT_IsImmune( iVictim ) )
			{

				// Well we do have a target so lets execute the user's ultimate!!

				switch ( p_data[iAttacker][P_RACE] )
				{
					case RACE_ORC:		OR_ULT_ChainLightning( iAttacker, iVictim, iHitZone );
					case RACE_ELF:		NE_ULT_Entangle( iAttacker, iVictim );
					case RACE_BLOOD:	BM_ULT_Immolate( iAttacker, iVictim );
				}
				
				// No longer searching since we found a target
				p_data_b[iAttacker][PB_ISSEARCHING]	= false;

				// Ultimate has been used
				p_data_b[iAttacker][PB_ULTIMATEUSED]	= true;
				
				// Hide the ultimate icon
				Ultimate_Icon( iAttacker, ICON_HIDE );

				// Set up the user's ultimate delay
				p_data[iAttacker][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown );
				_ULT_Delay( iAttacker );
			}
		}

		// This is a nice check for Helm of Excellence
		if ( p_data[iVictim][P_ITEM2] == ITEM_HELM )
		{
			// If its a headshot then we want to block it
			if ( iHitZone & (1 << 1) )
			{
				set_tr( TR_flFraction, 1.0 );
				
				// Do the check to see if we should flash the screen orange
				new Float:time = halflife_time();
				if ( 0 < iAttacker <= MAXPLAYERS && time - fLastShotFired[iAttacker] < 0.1 )
				{
					Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 0, 0, 255, 150 );
				}
				
				return FMRES_SUPERCEDE;
			}
		}

		// This is a check for the big bad voodoo ultimate -
		if ( p_data_b[iVictim][PB_GODMODE] )
		{
			set_tr( TR_flFraction, 1.0 );

			return FMRES_SUPERCEDE;
		}
		
		// Check to see if this user has night elf's evasion
		if ( Verify_Skill( iVictim, RACE_ELF, SKILL1 ) )
		{
			// Do the check to see if we should "evade" this shot
			new Float:time = halflife_time();
			if ( 0 < iAttacker <= MAXPLAYERS && time - fLastShotFired[iAttacker] < 0.1 )
			{

				// Basically if friendly fire is on, we want to block ALL shots, otherwise we only block shots from enemies
				if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
				{
					if ( get_user_team( iAttacker ) == get_user_team( iVictim ) )
					{
						return FMRES_IGNORED;
					}
				}
				
				// Then we should evade this shot!
				if ( NE_Evasion( iVictim, iHitZone ) )
				{
					set_tr( TR_flFraction, 1.0 );

					client_print( iVictim, print_chat, "[DEBUG] Shot blocked by evasion!!!" );

					return FMRES_SUPERCEDE;
				}
			}
		}
	}
	
	return FMRES_IGNORED;
}

// Function called right before the user's spawn
EVENT_JustBeforeSpawn( id )
{
	
	// Save the items the user had from the previous round
	ITEM_Save( id );

	// Remove the user's items
	ITEM_ResetCurrent( id );

	// Reset all ultimates
	ULT_Reset( id );

	// Reset certain player variables
	p_data_b[id][PB_HAS_SPAWNED]		= false;
	p_data[id][P_RESPAWNBY]				= 0;
	p_data_b[id][PB_JUSTJOINED]			= false;

	// Remove any reset_maxspeeds occuring (could cause a person to move during freezetime)
	task_exists( TASK_RESETSPEED + id ) ? remove_task( TASK_RESETSPEED + id ) : 0;
	
	// Save a copy of what weapons the user had the previous round (for weapon reincarnation)
	SHARED_CopySavedWeapons( id );
}