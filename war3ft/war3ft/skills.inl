

// Gives skill abilities at beginning of round and when skills are selected
public Skill_Check(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("Skill_Check",id)
	#endif

	new parm[2]
	parm[0]=id

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	// Evasion
	if ( Verify_Skill(id, RACE_ELF, SKILL1) && p_data_b[id][PB_EVADENEXTSHOT] ){
		set_user_health(id,1124)
	}

	// Devotion Aura
	else if ( Verify_Skill(id, RACE_HUMAN, SKILL2) ){
		set_user_health(id,p_devotion[p_data[id][P_SKILL2]-1])
	}

	// Healing Wave
	if ( Verify_Skill(id, RACE_SHADOW, SKILL1) ){
		if (task_exists(id+TASK_WAVE))
			remove_task(id+TASK_WAVE)
		set_task(1.0,"_Skill_Healing_Wave",TASK_WAVE+id,parm,2)
	}
	else{
		if (task_exists(id+TASK_WAVE))
			remove_task(id+TASK_WAVE)
	}

	// Serpent Ward
	if ( Verify_Skill(id, RACE_SHADOW, SKILL3) )
		p_data[id][P_SERPENTCOUNT]=p_serpent[p_data[id][P_SKILL3]-1]
	else
		p_data[id][P_SERPENTCOUNT]=0

	Skills_Blink(id)
		
	// Pheonix

	if ( Verify_Skill(id, RACE_BLOOD, SKILL1) ){			
		new Float:randomnumber = random_float(0.0,1.0)
		new teamnumber = get_user_team(id)
		if (randomnumber <= p_pheonix[p_data[id][P_SKILL1]-1]){
			p_data_b[id][PB_PHEONIXCASTER] = true
			if (teamnumber==1)
				g_pheonixExistsT++
			else
				g_pheonixExistsCT++
		}
	}

	icon_controller(id,ICON_SHOW)

	return PLUGIN_CONTINUE
}



#if MOD == 1
// ****************************************
// Orc's Reincarnation ability in Day of Defeat
// ****************************************

	public SKILL_Reincarnation(id){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("SKILL_Reincarnation",id)
		#endif

		if(reincarnation[id][ZPOS] == -99999){}		// Player just joined, so lets not teleport them
		else if(p_data_b[id][PB_REINCARNATION_SKIP]){
			p_data_b[id][PB_REINCARNATION_SKIP] = false
		}
		else if(p_data_b[id][PB_REINCARNATION_DELAY]){
			client_print(id,print_chat,"%s %L", g_MODclient, id, "SKILL_REINCARNATION_SKIPPING")
			p_data_b[id][PB_REINCARNATION_DELAY] = false
		}
		else{
			new parm[2]
			parm[0] = id

			client_cmd(id, "speak warcraft3/soundpack/reincarnation.wav")

			if (iglow[id][1] < 1){
				parm[0] = id
				set_task(0.1,"glow_change",TASK_GLOW+id,parm,2)
			} 
			iglow[id][1] += 100
			iglow[id][0] = 0
			iglow[id][2] = 0
			iglow[id][3] = 0
			if (iglow[id][1]>MAXGLOW)
				iglow[id][1]=MAXGLOW

			// Screen fade green
			Create_ScreenFade(id, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[id][1])

			reincarnation[id][ZPOS]+=30

			set_user_origin(id,reincarnation[id])

			set_task(0.1,"_SKILL_Reincarnation", TASK_REINCARNATION+id,parm,1)
			set_task(2.5,"_SKILL_Reincarnation_Status", TASK_REINCCHECK+id,parm,1)
		}
	}

	public _SKILL_Reincarnation(parm[]){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("_SKILL_Reincarnation",parm[0])
		#endif

		new id = parm[0]
		new origin[3]
		get_user_origin(id,origin)

		// Failure, stuck somewhere, but them back
		if(origin[2] == reincarnation[id][2]){

			new spawnID, playersInVicinity, entList[1], i
			new ent = -1
			new Float:spawnOrigin[3]
			new Float:vicinity = 96.0		//(32x32x96)
			new bool:found = false
			if(get_user_team(id) == CTS)
				spawnID = 0
			else
				spawnID = 1

			do {
				ent = find_ent_by_class(ent,spawnEntString[spawnID])
				if (ent != 0) {
					entity_get_vector(ent,EV_VEC_origin,spawnOrigin)
					for(i=0;i<3;i++)
						origin[i] = floatround(spawnOrigin[i])

					playersInVicinity = find_sphere_class(0, "player", vicinity, entList, 1, spawnOrigin)

					if (playersInVicinity == 0)
						found = true
				}
			}
			while (ent && !found)
			
			if(found){
				for(i=0;i<3;i++)
					origin[i] = floatround(spawnOrigin[i])

				set_user_origin(id, origin)
				client_print(id,print_chat,"%s %L", g_MODclient, id, "SKILL_REINCARNATION_FAILED")
			}
			else
				set_task(0.1,"_SKILL_Reincarnation", TASK_REINCARNATION+id,parm,1)

		}
	}

	public _SKILL_Reincarnation_Status(parm[]){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("_SKILL_Reincarnation_Status",parm[0])
		#endif

		new id = parm[0]

		if(!is_user_alive(id))
			p_data_b[id][PB_REINCARNATION_DELAY] = true
		else
			p_data_b[id][PB_REINCARNATION_DELAY] = false
	}
#endif
#if MOD == 0

// ****************************************
// Orc's Reincarnation Ability in CS/CZ
// ****************************************

public _Skill_Reincarnation_Drop(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Skill_Reincarnation_Drop",0)
	#endif

	new origin[3]
	new iweapons[32] = 0, wpname[32] = 0, inum = 0
	get_user_weapons(id,iweapons,inum)

	// Disarm and Isolate Player Before Re-Incarnation
	get_user_origin(id,origin)
	origin[2] -= 2000
	set_user_origin(id,origin)
	for(new a=0;a<inum;++a){
		if(iweapons[a] != CSW_C4 && iweapons[a] != CSW_KNIFE){
			get_weaponname(iweapons[a],wpname,31)
			engclient_cmd(id,"drop",wpname)
		}
	}
	client_cmd(id,"weapon_knife")
}

public _Skill_Reincarnation_Give(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Skill_Reincarnation_Give",0)
	#endif

	new origin[3]
	new weaponname[20] = 0, ammoname[20] = 0

	get_user_origin(id,origin)
	origin[2] += 2005

	if (p_data[id][P_ARMORONDEATH]){
		if (p_data_b[id][PB_HELMET])
			give_item(id,"item_assaultsuit")
		else
			give_item(id,"item_kevlar")

		set_user_armor(id,p_data[id][P_ARMORONDEATH])
	}

	if (p_data_b[id][PB_DEFUSE])
		give_item(id,"item_thighpack")

	if (p_data_b[id][PB_NIGHTVISION])
		cs_set_user_nvg(id,1)

	if(p_data_b[id][PB_SHIELD])
		give_item(id,"weapon_shield")

	new weaponid = 0, j
	for (j=0; (j < p_data[id][P_SAVEDNUMBER]) && (j < 32); ++j){
		weaponid=savedweapons[id][j]

		if(weaponid==CSW_USP){
			weaponname="weapon_usp"
			ammoname="ammo_45acp"
		}
		else if(weaponid==CSW_ELITE){
			weaponname="weapon_elite"
			ammoname="ammo_9mm"
		}
		else if(weaponid==CSW_FIVESEVEN){
			weaponname="weapon_fiveseven"
			ammoname="ammo_57mm"
		}
		else if(weaponid==CSW_GLOCK18){
			weaponname="weapon_glock18"
			ammoname="ammo_9mm"
		}
		else if(weaponid==CSW_DEAGLE){
			weaponname="weapon_deagle"
			ammoname="ammo_50ae"
		}
		else if(weaponid==CSW_P228){
			weaponname="weapon_p228"
			ammoname="ammo_357sig"
		}
		else if (weaponid==3){
			weaponname="weapon_scout"
			ammoname="ammo_762nato"
		}
		else if (weaponid==4){
			give_item(id,"weapon_hegrenade")
		}
		else if (weaponid==5){
			weaponname="weapon_xm1014"
			ammoname="ammo_buckshot"
		}
		else if (weaponid==7){
			weaponname="weapon_mac10"
			ammoname="ammo_45acp"
		}
		else if (weaponid==8){
			weaponname="weapon_aug"
			ammoname="ammo_556nato"
		}
		else if (weaponid==9){
			give_item(id,"weapon_smokegrenade")	
		}
		else if (weaponid==12){
			weaponname="weapon_ump45"
			ammoname="ammo_45acp"
		}
		else if (weaponid==13){
			weaponname="weapon_sg550"
			ammoname="ammo_556nato"
		}
		else if (weaponid==14){
			weaponname="weapon_galil"
			ammoname="ammo_556nato"
		}
		else if (weaponid==15){
			weaponname="weapon_famas"
			ammoname="ammo_556nato"
		}
		else if (weaponid==18){
			weaponname="weapon_awp"
			ammoname="ammo_338magnum"
		}
		else if (weaponid==19){
			weaponname="weapon_mp5navy"
			ammoname="ammo_9mm"				
		}
		else if (weaponid==20){
			weaponname="weapon_m249"
			ammoname="ammo_556natobox"
		}
		else if (weaponid==21){
			weaponname="weapon_m3"
			ammoname="ammo_buckshot"
		}
		else if (weaponid==22){
			weaponname="weapon_m4a1"
			ammoname="ammo_556nato"
		}
		else if (weaponid==23){
			weaponname="weapon_tmp"
			ammoname="ammo_9mm"
		}
		else if (weaponid==24){
			weaponname="weapon_g3sg1"
			ammoname="ammo_762nato"
		}
		else if (weaponid==25){
			if(p_data[id][P_FLASHCOUNT]==1)
				give_item(id,"weapon_flashbang")
			else{
				give_item(id,"weapon_flashbang")
				give_item(id,"weapon_flashbang")
			}
		}
		else if (weaponid==27){
			weaponname="weapon_sg552"
			ammoname="ammo_556nato"
		}
		else if (weaponid==28){
			weaponname="weapon_ak47"
			ammoname="ammo_762nato"
		}
		else if (weaponid==30){
			weaponname="weapon_p90"
			ammoname="ammo_57mm"
		}
		else{
			weaponname=""
			ammoname=""
		}
		if (contain(weaponname,"weapon_")==0){	//  if no match found, 0 if match
			give_item(id,weaponname)
			give_item(id,ammoname)
			give_item(id,ammoname)
			give_item(id,ammoname)
			give_item(id,ammoname)
		}
	}

	// Restore Re-Incarnated Player to the map
	set_user_origin(id,origin)

	// This will make the player have the new reincarnated weapon selected
	// instead of the player's knife being selected
	new iweapons[32] = 0, wpname[32] = 0, inum = 0
	new bool:foundPrim = false
	//new bool:foundSec = false
	get_user_weapons(id,iweapons,inum)

	for(new a=0;a<inum;++a){
		if(isPrimary(iweapons[a])){
			get_weaponname(iweapons[a],wpname,31)
			client_cmd(id,wpname)
			foundPrim = true
			break
		}
	}

	if(!foundPrim){
		for(new a=0;a<inum;++a){
			if(isSecondary(iweapons[a])){
				get_weaponname(iweapons[a],wpname,31)
				client_cmd(id,wpname)
				//foundSec = true
				break
			}
		}
	}	
}
#endif


// ****************************************
// Shadow Hunter's Healing Wave
// ****************************************

public _Skill_Healing_Wave(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Skill_Healing_Wave",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (Verify_Skill(id, RACE_SHADOW, SKILL1) && !endround && is_user_alive(id)){
		set_task(p_heal[p_data[id][P_SKILL1]-1],"_Skill_Healing_Wave",TASK_HEALNOW+id,parm,2)
	}

	new players[32], numberofplayers, teamname[32]
	new i, targetid, distancebetween, targetorigin[3], heal = 1
	new origin[3]

	get_user_origin(id, origin)

	get_user_team(id, teamname, 31)

	get_players(players, numberofplayers,"ae",teamname)

	for (i = 0; i < numberofplayers; ++i){

		targetid=players[i]

		if (is_user_alive(targetid) && p_data_b[targetid][PB_ISCONNECTED]){

			get_user_origin(targetid, targetorigin)

			distancebetween = get_distance(origin, targetorigin)

			if (distancebetween < iCvar[FT_HEALING_RANGE]){

				get_user_origin(targetid,origin)

				while(heal!=0){
					if( get_user_actualhealth(targetid) + heal <= get_user_maxhealth(targetid) ){
						set_user_health(targetid, get_user_health(targetid) + heal)
					}
					heal--
				}

				Create_TE_IMPLOSION(origin, 100, 8, 1)
			}
		}
	}

	return PLUGIN_CONTINUE
}


// ****************************************
// Shadow Hunter's Hex
// ****************************************

public _Skill_Hex(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Skill_Hex",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	client_cmd(id,"+jump;wait;-jump")

	parm[1]--

	Create_ScreenFade(id, (1<<10), (1<<10), (1<<12), 82, 245, 235, 110)

	if(!endround && is_user_alive(id) && parm[1]>0)
		set_task(4.0,"_Skill_Hex",TASK_JUMPER+id,parm,2)
	
	return PLUGIN_CONTINUE
}


// ****************************************
// Shadow Hunter's Serpent Ward
// ****************************************

public _Skill_SerpentWard(parm[6]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Skill_SerpentWard",parm[5])
	#endif

	new id = parm[5]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (!is_user_alive(id))
		return PLUGIN_CONTINUE

	new origin[3], damage, start[3], end[3], red, blue, green
	origin[0]=parm[0]
	origin[1]=parm[1]
	origin[2]=parm[2]

	start[0] = origin[0]
	start[1] = origin[1]
	start[2] = origin[2] + 600
	end[0] = origin[0]
	end[1] = origin[1]
	end[2] = origin[2] - 600

#if MOD == 0
	if(p_data[id][P_SERPENTTEAM]==TS){
		red = 255
		blue = 0
		green = 0
	}
	else{
		red = 0
		blue = 255
		green = 0
	}
#endif
#if MOD == 1
	if(p_data[id][P_SERPENTTEAM]==AXIS){
		red = 255
		blue = 63
		green = 63
	}
	else{
		red = 76
		blue = 102
		green = 76
	}
#endif
	if(!g_mapDisabled)
		Create_TE_BEAMPOINTS(start, end, g_sLightning, 1, 5, 2, 500, 20, red, green, blue, 100, 100)


	new players[32]
	new numberofplayers
	get_players(players, numberofplayers)
	new i
	new targetid = 0
	new distancebetween = 0
	new targetorigin[3]
	
	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]
		if (p_data[id][P_SERPENTTEAM]!=get_user_team(targetid) &&is_user_alive(targetid)){
			get_user_origin(targetid,targetorigin)
			distancebetween=sqroot((origin[1]-targetorigin[1])*(origin[1]-targetorigin[1])+(origin[0]-targetorigin[0])*(origin[0]-targetorigin[0]))
			if ((distancebetween < 85)){
				damage = 10
				WAR3_damage(targetid, id, damage, CSW_SERPENTWARD, 0)

				client_cmd(targetid, "speak ambience/thunder_clap.wav")

				Create_ScreenFade(targetid, (1<<10), (1<<10), (1<<12), red, green, blue, 255)
			}
		}
	}

	if (!endround)
		set_task(0.5,"_Skill_SerpentWard",TASK_LIGHT+id,parm,6)

	return PLUGIN_CONTINUE
}


// ****************************************
// Warden's Blink
// ****************************************

public Skills_Blink(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("Skills_Blink",id)
	#endif

	if ( Verify_Skill(id, RACE_WARDEN, SKILL2) ){			// Blink
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= p_blink[p_data[id][P_SKILL2]-1])
			p_data_b[id][PB_WARDENBLINK]=true	
		else
			p_data_b[id][PB_WARDENBLINK]=false
	}
	else
		p_data_b[id][PB_WARDENBLINK]=false
}


// ****************************************
// Crypt Lord's Impale
// ****************************************

public _Skill_Impale(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Skill_Impale",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(parm[1]<4){
		new Float:vec_angle[3], Float:vec_v_angle[3]
		new i

		entity_get_vector(id, EV_VEC_angles, vec_angle)
		entity_get_vector(id, EV_VEC_v_angle, vec_v_angle)

		for(i=0;i<2;i++){ // Don't modify z axis (causes a roll)
			vec_angle[i] = vec_angle[i] + random_float(-10.0,10.0)
			vec_v_angle[i] = vec_v_angle[i] + random_float(-10.0,10.0)
		}

		entity_set_int(id, EV_INT_fixangle, 1)
		entity_set_vector(id, EV_VEC_angles, vec_angle)
		entity_set_vector(id, EV_VEC_v_angle, vec_v_angle)
		parm[1]++
		set_task(0.1, "_Skill_Impale", 0, parm, 2)
	}
	else{
		entity_set_int(id, EV_INT_fixangle, 1)
	}

	return PLUGIN_CONTINUE
}