#if MOD == 1
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

public _Skill_Hex(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Skill_Hex",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	client_cmd(id,"+jump;wait;-jump")

	parm[1]--

	Create_ScreenFade(id, (1<<10), (1<<10), (1<<12), 144, 58, 255, iglow[id][3])

	if(!endround && is_user_alive(id) && parm[1]>0)
		set_task(4.0,"_Skill_Hex",TASK_JUMPER+id,parm,2)
	
	return PLUGIN_CONTINUE
}

public _Skill_Impale(parm[2]){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("_Skill_Impale",parm[0])
	#endif

	new id = parm[0]

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
}