stock diff(num,num2){
	if(num>num2)
		return (num-num2)
	else
		return (num2-num)
	return 0
}

stock cmd_target2(id,const arg[]) {
  new player = find_player("bl",arg)
  if (player){
    if ( player != find_player("blj",arg) ){
      console_print(id,"%L",id,"THERE_ARE_MORE")
      return 0    
    }
  }
  else if ( ( player = find_player("c",arg) )==0 && arg[0]=='#' && arg[1] )
    player = find_player("k",str_to_num(arg[1]))    
  if (!player){
    console_print(id,"%L",id,"CLIENT_NOT_FOUND")
    return 0
  }
  return player
}

stock isPrimary(weapon){
	if(weapon==CSW_GALIL||weapon==CSW_FAMAS||weapon==CSW_M3||weapon==CSW_XM1014||weapon==CSW_MP5NAVY||weapon==CSW_TMP ||weapon==CSW_P90||weapon==CSW_MAC10||weapon==CSW_UMP45||weapon==CSW_AK47||weapon==CSW_SG552||weapon==CSW_M4A1||weapon==CSW_AUG||weapon==CSW_SCOUT ||weapon==CSW_AWP||weapon==CSW_G3SG1||weapon==CSW_SG550||weapon==CSW_M249)
		return true

	return false
}

stock isSecondary(weapon){
	if(weapon==CSW_P228||weapon==CSW_ELITE||weapon==CSW_FIVESEVEN||weapon==CSW_USP||weapon==CSW_GLOCK18||weapon==CSW_DEAGLE)
		return true

	return false
}

stock find_free_spawn(iTeamNumber, Float:spawnOrigin[3], Float:spawnAngle[3]){
	new iSpawn
	if(iTeamNumber == CTS)
		iSpawn=0
	else
		iSpawn=1

	const maxSpawns = 128
	new spawnPoints[maxSpawns]
	new ent = -1, spawnsFound = 0
	do {
		ent = find_ent_by_class(ent,spawnEntString[iSpawn])
		if (ent != 0) {
			spawnPoints[spawnsFound] = ent
			spawnsFound++
		}
	}
	while (ent && spawnsFound < maxSpawns)

	new bool:foundFreeSpawn
	new Float:vicinity = 96.0		//(32x32x96)
	new i
	new playersInVicinity
	new entList[1]
	for (i = 0;i < spawnsFound && !foundFreeSpawn;i++) {
		if(spawnPoints[i] != 0){
			entity_get_vector(spawnPoints[i],EV_VEC_origin,spawnOrigin)
			playersInVicinity = find_sphere_class(0, "player", vicinity, entList, 1, spawnOrigin)
			if (playersInVicinity == 0){
				foundFreeSpawn = true
			}
			else{
				foundFreeSpawn = false
			}
		}
	}

	if(foundFreeSpawn && spawnPoints[i] != 0){
		entity_get_vector(spawnPoints[i], EV_VEC_angles, spawnAngle)

		return spawnPoints[i]
	}

	return -1
}

stock get_user_actualhealth(id){

	// Player has god mode

	if ( get_user_health(id) > 1500 )
		return get_user_health(id)-2048


	// Player will evade the next shot

	if ( get_user_health(id) > 500 )
		return get_user_health(id)-1024


	return get_user_health(id)
}

stock get_user_maxhealth(id){
	new maxHealth = 100


	// Human Devotion Skill

	if ( Verify_Skill(id, RACE_HUMAN, SKILL2) ){
		maxHealth += (p_devotion[p_data[id][P_SKILL2]-1] - 100)
	}


	// Player has a health bonus from the Periapt of Health

	if ( p_data[id][P_ITEM]==ITEM_HEALTH ){
		maxHealth += iCvar[FT_HEALTH_BONUS]
	}

	return maxHealth
}

stock Verify_Race(id, race){

	if( p_data[id][P_RACE] == race )
		return true
	else if( (p_data[id][P_RACE] == 9) ){
		if ( race9Options[1] == race )
			return true
		if ( race9Options[2] == race )
			return true
		if ( race9Options[3] == race )
			return true
		if ( race9Options[4] == race )
			return true
	}

	return false
}

stock Verify_Skill(id, race, skill){
	if( ((p_data[id][P_RACE] == 9 && race9Options[skill] == race) || p_data[id][P_RACE] == race) && p_data[id][skill] )
		return true

	return false
}

stock Status_Text(id, szMessage[], Float:fDuration, Float:iYPos){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("WAR3_status_text",0)
	#endif

	#if MOD == 1
		Create_HudText(id, szMessage, 1)
		iYPos--
	#else
		set_hudmessage(255, 208, 0, HUDMESSAGE_POS_CENTER, iYPos, 0, 6.0, fDuration, 0.1, 0.5, HUDMESSAGE_CHAN_STATUS)
		show_hudmessage(id, szMessage)
	#endif

    // Print important messages to console
    if ( fDuration >= 4.0 ){
        console_print( id, "%s %s", g_MODclient, szMessage )
    }
}

stock race9_randomize(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("race9_randomize",0)
	#endif

	if (iCvar[FT_RACES] == 9){
		if (iCvar[FT_9RACERANDOM]){
			new myintvallocal = 0
			// loop through all four skill options (3 + ultimate) pick a new race at random and update the skill
			while (myintvallocal < 4){
				race9Options[myintvallocal + 1] = random_num(1,8) 
				++myintvallocal
			}
		}
	} 
}