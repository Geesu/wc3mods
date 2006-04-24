stock diff(num,num2){
	if(num>num2)
		return (num-num2)
	else
		return (num2-num)
	return 0
}

stock find_target(id,const arg[]) {
new player = find_player("bl",arg)
if(player){
if ( player != find_player("blj",arg) ){
console_print(id,"%L",id,"THERE_ARE_MORE")
return 0
}
}
else if((player = find_player("c",arg) )==0 && arg[0]=='#' && arg[1] )
player = find_player("k",str_to_num(arg[1]))
if(!player){
console_print(id,"%L",id,"CLIENT_NOT_FOUND")
return 0
}

return player
}

stock find_free_spawn(id, iTeamNumber, Float:spawnOrigin[3], Float:spawnAngle[3]){
	new iSpawn
	if(iTeamNumber == CTS)
		iSpawn=0
	else
		iSpawn=1

	const maxSpawns = 128
	new spawnPoints[maxSpawns]
	new ent = -1, spawnsFound = 0

	/* Find all of the spawn points */
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
	
	/* Loop through all the spawn points */
	for (i = 0;i < spawnsFound && !foundFreeSpawn;i++) {

		if(spawnPoints[i] != 0 && !spawnPointsused[i]){
			/* Get the origin of the spawn point */
			entity_get_vector(spawnPoints[i],EV_VEC_origin,spawnOrigin)

			/* Determine if a player is in this vicinity */
			playersInVicinity = find_sphere_class(0, "player", vicinity, entList, 1, spawnOrigin)

			/* If not, we want to transport the player here */
			if (playersInVicinity == 0){
				foundFreeSpawn = true
				spawnPointsused[i] = true
			}
			/* Otherwise we don't */
			else{
				foundFreeSpawn = false
			}
		}
		id--

	}

	if( foundFreeSpawn && spawnPoints[i] ){
		entity_get_vector(spawnPoints[i], EV_VEC_angles, spawnAngle)

		return spawnPoints[i]
	}

	return -1
}

stock get_user_actualhealth(id){

	new health = get_user_health(id)

	// Player has god mode

	if ( health > 1500 )
		return health - 2048

	// Player will evade the next shot

	if ( health > 500 )
		return health - 1024


	return health
}

stock get_user_maxhealth(id){

	new maxHealth = 100

	// Human Devotion Skill

	if ( Verify_Skill(id, RACE_HUMAN, SKILL2) ){
		maxHealth += (p_devotion[p_data[id][P_SKILL2]-1] - 100)
	}

	// Player has a health bonus from the Periapt of Health

	if ( p_data[id][P_ITEM]==ITEM_HEALTH ){
		maxHealth += get_pcvar_num( CVAR_wc3_health );
	}

	return maxHealth
}

stock Verify_Race(id, race){

	if( id == 0 )
		return false
	
	if( p_data[id][P_RACE] == race )
		return true

	else if( (p_data[id][P_RACE] == 9) ){
		if ( g_ChamSkills[1] == race )
			return true
		if ( g_ChamSkills[2] == race )
			return true
		if ( g_ChamSkills[3] == race )
			return true
		if ( g_ChamSkills[4] == race )
			return true
	}

	return false
}

stock Verify_Skill(id, race, skill){

	if( id == 0 )
		return false

	if( ((p_data[id][P_RACE] == 9 && g_ChamSkills[skill] == race) || p_data[id][P_RACE] == race) && p_data[id][skill] )
		return true

	return false
}

