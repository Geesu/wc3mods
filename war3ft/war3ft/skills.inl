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

	BM_PheonixCheck( id );

	return PLUGIN_CONTINUE
}