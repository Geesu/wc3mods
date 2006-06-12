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

stock get_user_maxhealth(id){

	new maxHealth = 100

	// Human Devotion Skill

	if ( Verify_Skill(id, RACE_HUMAN, SKILL2) )
		maxHealth += (p_devotion[p_data[id][P_SKILL2]-1] - 100)


	// Player has a health bonus from the Periapt of Health

	if ( ITEM_Has( id, ITEM_HEALTH ) )
		maxHealth += get_pcvar_num( CVAR_wc3_health );

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
		return 0

	if( ((p_data[id][P_RACE] == 9 && g_ChamSkills[skill] == race) || p_data[id][P_RACE] == race) && p_data[id][skill] )
		return p_data[id][skill]

	return 0
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
