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

/* Strips specified gun or C4 from a specified player or bot.
* Don't try to strip grenades, shield or knife with this function.
* Returns true on success, false on failure.
* If user have no weapon with given wid, then wname would be used.
* Engine module required (proting to the fakemeta is possible).
* By VEN. Use at your own risk. Provided as is (no warranties) */
stock bool:strip_user_gun(id, wid = 0, const wname[] = "") {
    if (!is_user_alive(id))
        return false

    new ent_name[24]
    if (wid && user_has_weapon(id, wid))
        get_weaponname(wid, ent_name, 23)
    else if (wname[0])
        copy(ent_name, 23, wname)
    else
        return false

    new ent_weap = find_ent_by_owner(-1, ent_name, id)
    if (!ent_weap)
        return false

    engclient_cmd(id, "drop", ent_name)

    new ent_box = entity_get_edict(ent_weap, EV_ENT_owner)
    if (!ent_box || ent_box == id)
        return false

    entity_set_int(ent_box, EV_INT_flags, entity_get_int(ent_box, EV_INT_flags)|FL_KILLME)
    entity_set_int(ent_weap, EV_INT_flags, entity_get_int(ent_weap, EV_INT_flags)|FL_KILLME)

    return true
} 

// Function checks to see if the weapon is a pistol
stock SHARED_IsSecondaryWeapon( iWeaponID )
{
	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeaponID == CSW_ELITE || iWeaponID == CSW_FIVESEVEN || iWeaponID == CSW_USP || iWeaponID == CSW_GLOCK18 || iWeaponID == CSW_DEAGLE || iWeaponID == CSW_P90 )
		{
			return true;
		}
	}
	
	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{

	}


	return false;
}
