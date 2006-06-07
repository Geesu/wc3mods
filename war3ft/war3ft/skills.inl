#define IMPALE_INTENSITY		10.0		// Intensity of impale


// Gives skill abilities at beginning of round and when skills are selected
public Skill_Check(id)
{

	if ( !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}

	if( p_data_b[id][PB_RESETSKILLS] )
	{
		p_data_b[id][PB_RESETSKILLS] = false;
		
		return PLUGIN_CONTINUE;
	}

	new parm[2]
	parm[0]=id

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE
	
	// Set the number of serpent wards
	if ( Verify_Skill(id, RACE_SHADOW, SKILL3) )
	{
		p_data[id][P_SERPENTCOUNT]		= p_serpent[p_data[id][P_SKILL3]-1];
	}

	// Undead's Unholy Aura
	SHARED_SetGravity(id);

	// Crypt Lord's Carrion Beetles
	if ( Verify_Skill(id, RACE_CRYPT, SKILL3) ){
		p_data[id][P_CARRIONCOUNT]=2
	}
	
	// Warden's Shadow Strike
	if ( Verify_Skill(id, RACE_WARDEN, SKILL3) ){
		p_data[id][P_SHADOWCOUNT]=2
	}

	// Human's Devotion Aura
	if ( Verify_Skill(id, RACE_HUMAN, SKILL2) ){
		set_user_health(id,p_devotion[p_data[id][P_SKILL2]-1])
	}

	// God mode, removing the reset task will prevent death (health is reset to 100 at the start of the round, so we don't want to set the health to 100-2048)
	if ( task_exists( TASK_RESETGOD+id ) && p_data_b[id][PB_GODMODE] ){
		remove_task( TASK_RESETGOD+id )
		p_data_b[id][PB_GODMODE] = false
	}

	// Shadow Hunter's Healing Wave
	if ( Verify_Skill(id, RACE_SHADOW, SKILL1) ){
		if (!task_exists(id+TASK_WAVE))
			set_task( 1.0, "_SH_HealingWave", TASK_WAVE + id );
	}
	else{
		if (task_exists(id+TASK_WAVE))
			remove_task(id+TASK_WAVE)
	}

	// Shadow Hunter's Serpent Ward
	if ( Verify_Skill(id, RACE_SHADOW, SKILL3) )
		p_data[id][P_SERPENTCOUNT]=p_serpent[p_data[id][P_SKILL3]-1]
	else
		p_data[id][P_SERPENTCOUNT]=0

	WA_Blink( id );

	BM_PheonixCheck( id );

	Ultimate_Icon(id,ICON_SHOW)

	return PLUGIN_CONTINUE
}

// ****************************************
// Blood Mage's Pheonix Ability in DOD
// ****************************************

#define BM_PHEONIX_RANGE		750

public Skill_Pheonix(id){
	SHARED_SetUserMoney(id, SHARED_GetUserMoney(id) + p_pheonix_dod[p_data[id][P_SKILL1]-1])

	new name[32], team
	get_user_name(id, name, 31)
	team = get_user_team(id)

	new players[32], numberofplayers
	new i, targetid, distancebetween, targetorigin[3], origin[3]

	get_user_origin(id, origin)
	get_players(players, numberofplayers,"a")

	new money = p_pheonix_dod[p_data[id][P_SKILL1]-1] / 2
	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]

		if ( targetid != id && p_data_b[targetid][PB_ISCONNECTED] && get_user_team(targetid) == team){
			get_user_origin(targetid, targetorigin)
			distancebetween = get_distance(origin, targetorigin)

			if (distancebetween < BM_PHEONIX_RANGE){
				SHARED_SetUserMoney(targetid, SHARED_GetUserMoney(targetid) + money)
				client_print(targetid, print_chat, "%s %L", g_MODclient, targetid, "DOD_PHOENIX", money, name)
			}
		}
	}
}

// ****************************************
// Crypt Lord's Impale
// ****************************************

public _Skill_Impale(parm[2]){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(parm[1]<4){
		new Float:iMin = -1.0 * IMPALE_INTENSITY
		new Float:iMax = IMPALE_INTENSITY

		new Float:vec_angle[3], Float:vec_v_angle[3]
		new i

		entity_get_vector(id, EV_VEC_angles, vec_angle)
		entity_get_vector(id, EV_VEC_v_angle, vec_v_angle)

		for(i=0;i<2;i++){ // Don't modify z axis (causes a roll)
			vec_angle[i] = vec_angle[i] + random_float(iMin, iMax)
			vec_v_angle[i] = vec_v_angle[i] + random_float(iMin, iMax)
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
