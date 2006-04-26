#define IMPALE_INTENSITY		10.0		// Intensity of impale
#define HEALING_WAVE_RANGE		750			// Range to heal teammates


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
	Skill_UnholyAura(id);

	// Crypt Lord's Carrion Beetles
	if ( Verify_Skill(id, RACE_CRYPT, SKILL3) ){
		p_data[id][P_CARRIONCOUNT]=2
	}
	
	// Warden's Shadow Strike
	if ( Verify_Skill(id, RACE_WARDEN, SKILL3) ){
		p_data[id][P_SHADOWCOUNT]=2
	}

	// Night Elf's Evasion
	Skill_Evasion_Set( id );

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
			set_task(1.0,"_Skill_Healing_Wave",TASK_WAVE+id,parm,2)
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

	Skills_Blink(id)

#if MOD == 0
	// Blood Mage's Pheonix
	if ( Verify_Skill(id, RACE_BLOOD, SKILL1) ){			
		new Float:randomnumber = random_float(0.0,1.0)
		new teamnumber = get_user_team(id)
		if (randomnumber <= p_pheonix[p_data[id][P_SKILL1]-1]){
			p_data_b[id][PB_PHOENIXCASTER] = true
			PhoenixFound[teamnumber-1]++
		}
	}
#endif

	Ultimate_Icon(id,ICON_SHOW)

	return PLUGIN_CONTINUE
}

// ****************************************
// Undead's Unholy Aura
// ****************************************
public Skill_UnholyAura( id )
{
	if( get_cvar_num("sv_gravity") > 650 )
	{
		// Levitation
		if ( Verify_Skill(id, RACE_UNDEAD, P_SKILL3) )
		{		
			if ( get_user_gravity(id) != p_levitation[p_data[id][P_SKILL3]-1] )
			{
				set_user_gravity(id, p_levitation[p_data[id][P_SKILL3]-1]);
			}
		}
		else if ( p_data[id][P_ITEM2] == ITEM_SOCK )
		{
			set_user_gravity(id, get_pcvar_float( CVAR_wc3_sock ));
		}
		else if ( get_user_gravity(id) != 1.0 )
		{
			set_user_gravity(id, 1.0);
		}

	}
	else
	{
		set_user_gravity(id, 1.0);
	}
}

// ****************************************
// Blood Mage's Pheonix Ability in DOD
// ****************************************

public Skill_Pheonix(id){
	SHARED_SetUserMoney(id, SHARED_GetUserMoney(id) + p_pheonix[p_data[id][P_SKILL1]-1])

	new name[32], team
	get_user_name(id, name, 31)
	team = get_user_team(id)

	new players[32], numberofplayers
	new i, targetid, distancebetween, targetorigin[3], origin[3]

	get_user_origin(id, origin)
	get_players(players, numberofplayers,"a")

	new money = p_pheonix[p_data[id][P_SKILL1]-1] / 2
	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]

		if ( targetid != id && p_data_b[targetid][PB_ISCONNECTED] && get_user_team(targetid) == team){
			get_user_origin(targetid, targetorigin)
			distancebetween = get_distance(origin, targetorigin)

			if (distancebetween < HEALING_WAVE_RANGE){
				SHARED_SetUserMoney(targetid, SHARED_GetUserMoney(targetid) + money)
				client_print(targetid, print_chat, "%s %L", g_MODclient, targetid, "DOD_PHOENIX", money, name)
			}
		}
	}
}


// ****************************************
// Night Elf's Evasion
// ****************************************

// Called when a user is shot and when a user spawns
public Skill_Evasion_Set( id )
{
	// Only check to see if the user can evade if they have the skill
	if ( Verify_Skill( id, RACE_ELF, SKILL1 ) )
	{
		new Float:randomnumber = random_float(0.0, 1.0);
		
		// Only set next shot as being evaded if if they aren't already evading the next shot
		if ( randomnumber <= p_evasion[p_data[id][P_SKILL1]-1] && !p_data_b[id][PB_EVADENEXTSHOT] )
		{
			p_data_b[id][PB_EVADENEXTSHOT] = true;
		}
	}
	
	// Actually do the health changes in this function
	Skill_Evasion_Check( id );
}

// Called on Skill_Evasion_Set and when a user types /level
public Skill_Evasion_Check( id )
{
	new iHealth = get_user_health( id );
	
	// Make sure the user has the skill
	if ( Verify_Skill( id, RACE_ELF, SKILL1 ) )
	{
		// Give the user enough health so they can evade
		if ( p_data_b[id][PB_EVADENEXTSHOT] && iHealth < 500 && is_user_alive(id) )
		{
			set_user_health( id, iHealth + SKILL_EVASION_ADJ );
		}
	}
	// User doesn't have the skill
	else
	{
		// This shouldn't be true if the user doesn't have the skill right?
		if ( p_data_b[id][PB_EVADENEXTSHOT] )
		{
			p_data_b[id][PB_EVADENEXTSHOT] = false;
		}
	}

	// Check if the user has too much health when they shouldn't evade the next shot
	if ( !p_data_b[id][PB_EVADENEXTSHOT] && iHealth > 500 && !p_data_b[id][PB_GODMODE] )
	{
		// Hopefully this will never kill them
		set_user_health(id, iHealth - SKILL_EVASION_ADJ);
	}

	// This should technically never occur ...
	if ( p_data_b[id][PB_EVADENEXTSHOT] && iHealth > 1500 && !p_data_b[id][PB_GODMODE] )
	{
		// Just set back to default
		set_user_health( id, 100 + SKILL_EVASION_ADJ );

	}
}

stock Skill_Evasion_Reset( id, damage )
{
	if ( !Verify_Skill( id, RACE_ELF, SKILL1 ) )
	{
		return;
	}

	new iHealth = get_user_health( id );
	
	if ( p_data_b[id][PB_EVADENEXTSHOT] )
	{
		set_user_health(id, iHealth + damage + (-1 * SKILL_EVASION_ADJ));

		if (iglow[id][2] < 1)
		{
			new parm[2];
			parm[0] = id;
			set_task(0.01, "glow_change", TASK_GLOW + id, parm, 2);
		}
		iglow[id][2] += damage;
		iglow[id][0] = 0;
		iglow[id][1] = 0;
		iglow[id][3] = 0;
		if (iglow[id][2]>MAXGLOW)
		{
			iglow[id][2]=MAXGLOW;
		}

		Create_ScreenFade(id, (1<<10), (1<<10), (1<<12), 0, 0, 255, iglow[id][2]);

		p_data_b[id][PB_EVADENEXTSHOT] = false;

	}

	return;
}

#if MOD == 0
	// ****************************************
	// Blood Mage's Phoenix in cstrike/czero
	// ****************************************

	public Skill_Phoenix(id)
	{
		new vTeam = get_user_team(id)
		
		/* Make sure that we have a CT/T player that died */
		if ( vTeam == CTS || vTeam == TS )
		{
			/* Verify that a player on the victim's team has phoenix */
			if ( PhoenixFound[vTeam-1] > 0 )
			{
				new players[32], numberofplayers, i, targetid
				get_players(players, numberofplayers, "a")

				/* Loop through all the players */
				for (i = 0; i < numberofplayers; ++i)
				{
					targetid = players[i]
					/* Verify that the players are on the same team and that a caster is found */
					if ( get_user_team(targetid) == vTeam && p_data_b[targetid][PB_PHOENIXCASTER] && !p_data_b[id][PB_TOBEREVIVED] && !endround && id!=targetid && !p_data_b[id][PB_SPAWNEDFROMITEM] )
					{
						new parm[2], name[32], victimName[32];
						parm[0] = id

						p_data_b[id][PB_SPAWNEDFROMITEM] = true

						set_task(0.7,"func_spawn",TASK_SPAWN+id,parm,2)
						set_hudmessage(200, 100, 0, -0.8, 0.1, 0, 1.0, 5.0, 0.1, 0.2, 2) 
						get_user_name(targetid,name,31)
						get_user_name(id,victimName,31)

						client_print(id, print_chat, "%s %L", g_MODclient, id, "HAS_REVIVED_YOU", name );
						client_print(targetid, print_chat, "%s %L", g_MODclient, targetid, "YOU_HAVE_REVIVED", victimName );

						p_data_b[id][PB_TOBEREVIVED]=true
						PhoenixFound[vTeam-1]--
						break
					}
				}
			}
		}
	}
#endif


// ****************************************
// Shadow Hunter's Healing Wave
// ****************************************

public _Skill_Healing_Wave(parm[2]){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if ( Verify_Skill(id, RACE_SHADOW, SKILL1) && is_user_alive(id) ){
		set_task(p_heal[p_data[id][P_SKILL1]-1],"_Skill_Healing_Wave",TASK_WAVE+id,parm,2)
	}
	
	// Prevent healing if this player is Hexed
	if ( p_data_b[id][PB_HEXED] )
		return PLUGIN_CONTINUE

	new team = get_user_team(id)

	new players[32], numberofplayers
	new i, targetid, distancebetween, targetorigin[3]
	new origin[3]

	get_user_origin(id, origin)
	get_players(players, numberofplayers,"a")

	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]

		if (p_data_b[targetid][PB_ISCONNECTED] && get_user_team(targetid) == team){
			get_user_origin(targetid, targetorigin)
			distancebetween = get_distance(origin, targetorigin)

			if (distancebetween < HEALING_WAVE_RANGE){

				get_user_origin(targetid,origin)
				
				if( get_user_actualhealth(targetid) + 1 <= get_user_maxhealth(targetid) ){

					set_user_health(targetid, get_user_health(targetid) + 1)

					Create_TE_IMPLOSION(origin, 100, 8, 1)
				}
			}
		}
	}

	return PLUGIN_CONTINUE
}


// ****************************************
// Shadow Hunter's Hex
// ****************************************

public _Skill_Hex(parm[2]){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE
	
	p_data_b[id][PB_CAN_RENDER] = true
	p_data_b[id][PB_HEXED] = false
	
	/* Reset the user's speed */
	set_user_maxspeed(id, 250.0)

	set_user_rendering(id)

	emit_sound(id, CHAN_STATIC, SOUND_HEX, 1.0, ATTN_NORM, 0, PITCH_NORM)

	return PLUGIN_CONTINUE
}


// ****************************************
// Shadow Hunter's Serpent Ward
// ****************************************

public _Skill_SerpentWard(parm[5]){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[3]

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
	if(parm[4]==TS){
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
	if(parm[4]==AXIS){
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

	new players[32], numberofplayers
	new i, targetid, distancebetween, targetorigin[3]

	get_players(players, numberofplayers, "a")
	
	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]
		if ( parm[4]!=get_user_team(targetid) ){
			get_user_origin(targetid,targetorigin)
			distancebetween = get_distance(origin, targetorigin)

			if ( distancebetween < 85 ){
				damage = 10
				WAR3_damage(targetid, id, damage, CSW_SERPENTWARD, -1)
				client_cmd(targetid, "speak ambience/thunder_clap.wav")
				Create_ScreenFade(targetid, (1<<10), (1<<10), (1<<12), red, green, blue, 255)
			}
		}
	}

	if (!endround)
		set_task(0.5,"_Skill_SerpentWard",TASK_LIGHT+id,parm,5)

	return PLUGIN_CONTINUE
}


// ****************************************
// Warden's Blink
// ****************************************

public Skills_Blink(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if ( Verify_Skill(id, RACE_WARDEN, SKILL2) ){			// Blink
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= p_blink[p_data[id][P_SKILL2]-1])
			p_data_b[id][PB_WARDENBLINK]=true	
		else
			p_data_b[id][PB_WARDENBLINK]=false
	}
	else
		p_data_b[id][PB_WARDENBLINK]=false

	return PLUGIN_CONTINUE
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
