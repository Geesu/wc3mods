public on_PlayerAction(){ 
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("on_PlayerAction",0)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new sArg[MAX_VAR_LENGTH], sAction[MAX_VAR_LENGTH] 
	new sName[MAX_NAME_LENGTH]  
	new iUserId, id

	read_logargv(0,sArg,MAX_VAR_LENGTH) 
	read_logargv(2,sAction,MAX_VAR_LENGTH) 
	parse_loguser(sArg,sName,MAX_NAME_LENGTH,iUserId) 
	id = find_player("k",iUserId)

	if (!WAR3_CD_installed(id))
		return PLUGIN_CONTINUE

	if(id==0)						// Prevents the server from gaining XP
		return PLUGIN_CONTINUE

#if MOD == 0
	// Bomb planted 
	if (equal(sAction,"Planted_The_Bomb")) {  

		g_fBombTime = get_gametime() + get_cvar_float("mp_c4timer")
		
		set_task((get_cvar_float("mp_c4timer") - 0.7), "on_B4TargetBombed",TASK_BOMBTIMER)

		new origin[3]
		new teamname[32]
		new player[32]
		new numplayers
		new targetorigin[3]
		new targetid
		
		get_user_origin(id, origin)
		get_user_team(id, teamname, 31)
		get_players(player, numplayers, "ae", teamname)
		g_bombCarrier = 0	

		new iXP = 3 * xpgiven[p_data[id][P_LEVEL]]												
		
		iXP = XP_give(id, iXP)
					
		if (iCvar[FT_OBJECTIVES]){				
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_PLANTING_BOMB",sName,iXP)
		}

		for (new i=0; i<numplayers; ++i){		// Includes self
			targetid=player[i]
			get_user_origin(targetid, targetorigin)
			if (get_distance(origin, targetorigin)<=iCvar[FT_XP_RADIUS]){
				iXP = iCvar[FT_BOMBPLANTERXP] + xpgiven[p_data[targetid][P_LEVEL]]	

				iXP = XP_give(targetid, iXP)

				if (iCvar[FT_OBJECTIVES]){
					get_user_name(targetid,sName,31)					
					client_print(targetid,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SUPPORTING_BOMB",sName,iXP)
				}				
			}
		}
	}
   // Bomb defused 
	else if (equal(sAction,"Defused_The_Bomb")) {
		new origin[3]
		new teamname[32]
		new player[32]
		new numplayers
		new targetorigin[3]
		new targetid
		
		get_user_origin(id, origin)
		get_user_team(id, teamname, 31)
		get_players(player, numplayers, "ae", teamname)
		
				
		new iXP = 3 *  xpgiven[p_data[id][P_LEVEL]]											
		
		iXP = XP_give(id, iXP)
				
		if (iCvar[FT_OBJECTIVES]){
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_DEFUSING_BOMB",sName,iXP)
		}

		for (new i=0; i<numplayers; ++i){		// Includes self
			targetid=player[i]
			get_user_origin(targetid, targetorigin)
			if (get_distance(origin, targetorigin)<=iCvar[FT_XP_RADIUS]){
				
				iXP = iCvar[FT_DEFUSEXP] +  xpgiven[p_data[targetid][P_LEVEL]]										
				iXP = XP_give(targetid, iXP)
							
				if (iCvar[FT_OBJECTIVES]){
					get_user_name(targetid,sName,31)						
					client_print(targetid,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SUPPORT_BOMB_D",sName,iXP)
				}					
			}
		}		
	} 
	// Bomb defusing with a kit 
	else if (equal(sAction,"Begin_Bomb_Defuse_With_Kit")) { 

		if((++p_data[id][P_DEFUSERINDEX] == 1) && (get_user_team(id) == 2) ){  // Team 1 = Terror, Team 2 = CT
			new iXP = iCvar[FT_DEFUSEXP] + xpgiven[p_data[id][P_LEVEL]]

			iXP = XP_give(id, iXP)
				
			if (iCvar[FT_OBJECTIVES]){	
				client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_ATTEMPT_BOMB_D",sName,iXP)
			}
		}               
   } 
   // Bomb defusing without a kit 
	else if (equal(sAction,"Begin_Bomb_Defuse_Without_Kit")) { 
     	
		if( (++p_data[id][P_DEFUSERINDEX] == 1) && (get_user_team(id) == 2) ){  // Team 1 = Terror, Team 2 = CT
			new iXP = xpgiven[p_data[id][P_LEVEL]]

			iXP = XP_give(id, iXP)

			if (iCvar[FT_OBJECTIVES]){
				client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_ATTEMPT_BOMB_D_BARE",id,iXP)
			}
		}
   } 
	// Spawned with the bomb 
	else if (equal(sAction,"Spawned_With_The_Bomb")) { 
		g_bombCarrier = id
	  
		new iXP = xpgiven[p_data[id][P_LEVEL]]											

		iXP = XP_give(id, iXP)

		if (iCvar[FT_OBJECTIVES]){
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SPAWNING_WITH_BOMB",sName,iXP)
		}
	} 

	// Dropped bomb 
	else if (equal(sAction,"Dropped_The_Bomb")) { 
		g_bombCarrier = 0
	  
		new iXP = -1 * xpgiven[p_data[id][P_LEVEL]]											

		iXP = XP_give(id, iXP)

		if (iCvar[FT_OBJECTIVES])
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_LOST_BOMB",sName,(-1*iXP))

   } 
	// Picked up bomb 
	else if (equal(sAction,"Got_The_Bomb")) { 
		g_bombCarrier = id
	  
		new iXP = xpgiven[p_data[id][P_LEVEL]]	

		iXP = XP_give(id, iXP)

		if (iCvar[FT_OBJECTIVES])
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_PICKING_UP_BOMB",sName,iXP)

	} 
	// Hostage touched 
	else if (equal(sAction,"Touched_A_Hostage")) { 
		new iXP = xpgiven[p_data[id][P_LEVEL]]	

		iXP = XP_give(id, iXP)
				
		if (iCvar[FT_OBJECTIVES])
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_BEGINNING_HOSTAGE_RESCUE",sName,iXP)

		g_hostageSaver = id
	} 
	// Hostage rescued 
	else if (equal(sAction,"Rescued_A_Hostage")) { 
		new origin[3]
		new teamname[32]
		new player[32]
		new numplayers
		new targetorigin[3]
		new targetid		
		
		get_user_origin(id, origin)
		get_user_team(id, teamname, 31)
		get_players(player, numplayers, "ae", teamname)
		
		new iXP = xpgiven[p_data[id][P_LEVEL]]												
			
		iXP = XP_give(id, iXP)
				
		if (iCvar[FT_OBJECTIVES])
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_RESCUING_HOSTAGE",sName,iXP)

		// Supporters and self	
		for (new i=0; i<numplayers; ++i){
			targetid=player[i]
			get_user_origin(targetid, targetorigin)
			if (get_distance(origin, targetorigin)<=iCvar[FT_XP_RADIUS]){
				iXP = iCvar[FT_HOSTAGEXP] +  xpgiven[p_data[targetid][P_LEVEL]]	

				iXP = XP_give(targetid, iXP)

				if (iCvar[FT_OBJECTIVES]){
					get_user_name(targetid,sName,31)
					client_print(targetid,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SUPPORTING_HOSTAGE_RUN",sName,iXP)
				}					
			}				
		}				
   } 

	// Hostage killed 
	else if (equal(sAction,"Killed_A_Hostage")) { 
		new iXP = -1 * xpgiven[p_data[id][P_LEVEL]]

		iXP = XP_give(id, iXP)

		if (iCvar[FT_OBJECTIVES])
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_KILLING_HOSTAGE",sName,(-1*iXP))		

	} 
	// VIP spawn 
	else if (equal(sAction,"Became_VIP")) { 
		g_vipID = id
	  
		new iXP = xpgiven[p_data[id][P_LEVEL]]										
		iXP = XP_give(id, iXP)

		if (iCvar[FT_OBJECTIVES]){
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SPAWN_AS_VIP",sName,iXP)
		}
		
	}

	// VIP assassinated 
	else if (equal(sAction,"Assassinated_The_VIP")) { 
		new sNameVIP[MAX_NAME_LENGTH] 
		get_user_name( g_vipID,sNameVIP,MAX_NAME_LENGTH) 		

		new iXP = iCvar[FT_VIP_KILL_BONUS] + xpgiven[p_data[id][P_LEVEL]]	

		iXP = XP_give(id, iXP)

		if (iCvar[FT_OBJECTIVES])
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_KILLING_VIP",sName,iXP,sNameVIP) 	
	} 
	// VIP escaped 
	else if (equal(sAction,"Escaped_As_VIP")) { 							
		new origin[3]
		new teamname[32]
		new player[32]
		new numplayers
		new targetorigin[3]
		new targetid

		get_user_origin(id, origin)
		get_user_team(id, teamname, 31)
		get_players(player, numplayers, "ae", teamname)
		
		new iXP = xpgiven[p_data[id][P_LEVEL]]

		iXP = XP_give(id, iXP)

		new nName[31]			
		if (iCvar[FT_OBJECTIVES]){	
			client_print(id,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_ESCAPED_VIP",sName,iXP)
		}

		for (new i=0; i<numplayers; ++i){		// Includes self
			targetid=player[i]
			get_user_origin(targetid, targetorigin)
			if (get_distance(origin, targetorigin)<=iCvar[FT_XP_RADIUS] && targetid!=id){
				iXP = iCvar[FT_VIP_ESCAPE_BONUS] + xpgiven[p_data[targetid][P_LEVEL]]
					
				iXP = XP_give(targetid, iXP)

				if (iCvar[FT_OBJECTIVES]){
					get_user_name(targetid,nName,31)					
					client_print(targetid,print_chat, "%s %L", g_MODclient, id,"AWARD_FOR_SUPPORTING_VIP",nName,iXP,sName)
				}				
			}
		}
	} 
#endif

	return PLUGIN_HANDLED 
}

#if MOD == 0
	public on_TargetBombed() {

		if (!warcraft3)
			return PLUGIN_CONTINUE

		set_task(0.1,"_on_TargetBombed",TASK_TARGETBOMBED)

		return PLUGIN_CONTINUE;
	}

	public _on_TargetBombed(){
		if (get_gametime() - g_fBombTime < 1.0){
			new players[32], numberofplayers, id, i
			get_players(players, numberofplayers)
			for(i=0;i<numberofplayers;i++){
				id = players[i]
				
				if( !p_data_b[id][PB_JUSTJOINED] && !is_user_alive(id) ){
					p_data_b[id][PB_DIEDLASTROUND] = true
				}
			}
		}
	}

	public on_B4TargetBombed(){
		new players[32], numberofplayers, id, i
		get_players(players, numberofplayers)
		for(i=0;i<numberofplayers;i++){
			id = players[i]
			if(is_user_alive(id)){
				saveweapons(id)
			}
		}
	}


	public on_FreezeTimeComplete() {
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("on_FreezeTimeComplete",0)
		#endif

		if (!warcraft3)
			return PLUGIN_CONTINUE

		g_freezetime = false
		
		new players[32],numberofplayers, id, i, parm[1]
		get_players(players, numberofplayers)
		
		for (i = 0; i < numberofplayers; ++i){
			id=players[i]

			WAR3_Display_Level(id, DISPLAYLEVEL_NONE)
			parm[0]=id
			set_task(0.1,"unholyspeed",TASK_UNHOLYSPEED+id,parm,1)
		}
		return PLUGIN_CONTINUE
	}

	public on_TerroristWin(){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("on_TerroristWin",0)
		#endif
		if (warcraft3==false)
			return PLUGIN_CONTINUE

		new players[32], numberofplayers,a, bool:give
		get_players(players,numberofplayers,"e","TERRORIST") 
		for (a = 0; a < numberofplayers; ++a){
			give = true
			if (iCvar[FT_CD]) {
				if (!!WAR3_CD_installed(players[a])){
					give = false
				}
			}

			if(p_data_b[players[a]][PB_JUSTJOINED] || p_data_b[players[a]][PB_CHANGINGTEAM])
				give = false

			if(give){
				new iXP = (iCvar[FT_ROUND_WIN_XP] + xpgiven[p_data[players[a]][P_LEVEL]])
				iXP = XP_give(players[a], iXP)
				if (iCvar[FT_OBJECTIVES]){				
					client_print(players[a],print_chat, "%s %L", g_MODclient, players[a],"AWARD_FOR_WINNING_ROUND",iXP)
				}				
			}
		}
		return PLUGIN_CONTINUE
	}

	public on_CTWin(){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("on_CTWin",0)
		#endif
		if (warcraft3==false)
			return PLUGIN_CONTINUE

		new players[32], numberofplayers,a, bool:give
		get_players(players,numberofplayers,"e","CT") 
		for (a = 0; a < numberofplayers; ++a){
			give = true
			if (iCvar[FT_CD]) {
				if (!!WAR3_CD_installed(players[a])){
					give = false
				}
			}

			if(p_data_b[players[a]][PB_JUSTJOINED] || p_data_b[players[a]][PB_CHANGINGTEAM])
				give = false

			if(give){
				new iXP = (iCvar[FT_ROUND_WIN_XP] + xpgiven[p_data[players[a]][P_LEVEL]])
				iXP = XP_give(players[a], iXP)

				if (iCvar[FT_OBJECTIVES]){			
					client_print(players[a],print_chat, "%s %L", g_MODclient, players[a],"AWARD_FOR_WINNING_ROUND",iXP)
				}				
			}
		}
		return PLUGIN_CONTINUE
	}
#endif

// Forwards from the CSX module and DODX module
public grenade_throw(index,greindex,wId){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("grenade_throw",index)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(g_mapDisabled)
		return PLUGIN_CONTINUE

	new szModel[64]
	entity_get_string(greindex, EV_SZ_model, szModel, 63)
#if MOD == 0
	if (greindex && equal(szModel, "models/w_hegrenade.mdl")){
#endif
#if MOD == 1
	if(greindex && (equal(szModel, "models/w_grenade.mdl") || equal(szModel, "models/w_stick.mdl"))){
#endif
		if( Verify_Skill(index, RACE_ORC, SKILL2) ){
			if( ( (p_data[index][P_ITEM2]==ITEM_GLOVES && iCvar[FT_GLOVE_ORC_DAMAGE]) || (p_data[index][P_ITEM2]!=ITEM_GLOVES) ) && is_user_alive(index) ) { 
					Create_TE_BEAMFOLLOW(greindex, g_siTrail, 20, 10, 255, 32, 32, 196)
			} 
		} 
	}
	return PLUGIN_CONTINUE
}

public client_damage(attacker,victim,damage,wpnindex,hitplace,TA){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("client_damage",victim)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if(!p_data_b[victim][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(!p_data_b[attacker][PB_ISCONNECTED])
		return PLUGIN_CONTINUE
	
	if(p_data_b[attacker][PB_MOLE] && (p_data[victim][P_ITEM2]==ITEM_PROTECTANT || p_data_b[victim][PB_WARDENBLINK])){	
		set_user_health(victim,get_user_health(victim)+damage)
		client_print(victim,print_chat,"%L",victim,"SHOT_DEFLECTED",g_MOD)
		return PLUGIN_HANDLED
	}

	if( p_data_b[victim][PB_GODMODE] ){
		if( p_data[attacker][P_ITEM] == ITEM_NECKLACE && attacker != victim ){
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

	if (victim==attacker && wpnindex==0){
		return PLUGIN_CONTINUE
	}
	else if (attacker==0){
		return PLUGIN_CONTINUE
	}

	new tempdamage = 0

	// Bot should "auto" cast his/her ultimate when attacking

	if (is_user_bot(attacker) && p_data[attacker][P_ULTIMATE]){
		cmd_Ultimate(attacker)
	}

	// **************************************************
	// Attacker Abilities
	// **************************************************

	// Undead Scourge

	if ( Verify_Race(attacker, RACE_UNDEAD) ){

		// Vampiric Aura
		if ( Verify_Skill(attacker, RACE_UNDEAD, SKILL1) ) {
			tempdamage = floatround(float(damage) * p_vampiric[p_data[attacker][P_SKILL1]-1])
			set_user_health(attacker, get_user_health(attacker) + tempdamage)

			if (get_user_health(attacker) > 100 + (p_data[attacker][P_ITEM]==ITEM_HEALTH ? iCvar[FT_HEALTH_BONUS] : 0) ){
				set_user_health(attacker, 100 + (p_data[attacker][P_ITEM]==ITEM_HEALTH ? iCvar[FT_HEALTH_BONUS] : 0) )
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
		if ( Verify_Skill(attacker, RACE_HUMAN, SKILL3) ) {
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_bash[p_data[attacker][P_SKILL3]-1] && get_user_maxspeed(victim) > 10 && !p_data_b[victim][PB_SLOWED]){		// Cannot bash if already bashed or frosted
				new normalspeed = floatround(get_user_maxspeed(victim))
				set_user_maxspeed(victim,1.0)
				p_data_b[victim][PB_STUNNED]=true
				new parm[2]
				parm[0]=victim
				parm[1]=normalspeed
				set_task(1.0,"reset_maxspeed",TASK_RESETSPEED+victim,parm,2)

				if (iglow[victim][3] < 1){
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
		if ( Verify_Skill(attacker, RACE_ORC, SKILL1)) {
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_critical[p_data[attacker][P_SKILL1]-1]){
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
		if ( Verify_Skill(attacker, RACE_ORC, SKILL2) ){		
			new bool:allow=true

			if(p_data[attacker][P_ITEM2]==ITEM_GLOVES){
				if(iCvar[FT_GLOVE_ORC_DAMAGE])
					allow=true
				else
					allow=false
			}

		#if MOD == 0
			if(g_notAllowHE)
				allow=false
			if (wpnindex == CSW_HEGRENADE && allow){
		#endif
		#if MOD == 1
			if ((wpnindex == DODW_HANDGRENADE || wpnindex == DODW_STICKGRENADE) && allow){
		#endif
				tempdamage = floatround(float(damage) * p_grenade[p_data[attacker][P_SKILL2]-1])
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
		if ( Verify_Skill(attacker, RACE_ELF, SKILL3) ){
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
		if ( Verify_Skill(attacker, RACE_BLOOD, SKILL2) ){
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

				emit_sound(attacker,CHAN_STATIC, SOUND_BANISH, 1.0, ATTN_NORM, 0, PITCH_NORM)
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
		if ( Verify_Skill(attacker, RACE_BLOOD, SKILL3) ){
			new money = floatround( p_mana[p_data[attacker][P_SKILL3]-1] * get_user_money(victim) )  

			set_user_money(attacker,get_user_money(attacker)+money,1)
			set_user_money(victim,get_user_money(victim)-money,1)

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
		if ( Verify_Skill(attacker, RACE_SHADOW, SKILL2) ) {
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_hex[p_data[attacker][P_SKILL2]-1] && p_data[victim][P_SKINCHANGED] != SKIN_HEX){
				changeskin(victim,SKIN_HEX)
				new parm[2]
				parm[0]=victim
				parm[1]=10		// 10 * 4 = 40 seconds (time hex lasts)
				_Skill_Hex(parm)

				if (iglow[victim][3] < 1){
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
	// Warden
	else if ( Verify_Race(attacker, RACE_WARDEN) ){
		
		// Shadow Strike
		if ( Verify_Skill(attacker, RACE_WARDEN, SKILL3) ){
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_shadow[p_data[attacker][P_SKILL3]-1] && p_data[attacker][P_SHADOWCOUNT]>0 && is_user_alive(victim)){
				new origin[3], attackerorigin[3]
				get_user_origin(victim,origin)
				get_user_origin(attacker,attackerorigin)
				
				if(!g_mapDisabled)
					Create_TE_SPRITETRAIL(attackerorigin, origin, g_sShadow, 50, 15, 1, 2, 6 )

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[victim][2])

				emit_sound(victim,CHAN_STATIC, SOUND_SHADOWSTRIKE, 1.0, ATTN_NORM, 0, PITCH_NORM)

				p_data[attacker][P_SHADOWCOUNT]--

				tempdamage = 10
				WAR3_damage(victim, attacker,tempdamage,CSW_SHADOW, hitplace)
			}
		}
	}

	// Crypt Lord
	else if ( Verify_Race(attacker, RACE_CRYPT) ){
		
		// Carrion Beetle
		if ( Verify_Skill(attacker, RACE_CRYPT, SKILL3) ){
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_carrion[p_data[attacker][P_SKILL3]-1]&& p_data[attacker][P_CARRIONCOUNT]>0 && is_user_alive(victim)){
				new origin[3], attackerorigin[3]
				get_user_origin(victim,origin)
				get_user_origin(attacker,attackerorigin)

				if(!g_mapDisabled)
					Create_TE_SPRITETRAIL(attackerorigin, origin, g_sBeetle, 15, 15, 1, 2, 6 )

				emit_sound(victim,CHAN_STATIC, SOUND_CARRION, 1.0, ATTN_NORM, 0, PITCH_NORM)

				p_data[attacker][P_CARRIONCOUNT]--

				tempdamage = 10
				WAR3_damage(victim, attacker,tempdamage, CSW_CARRION, hitplace)
			}
		}
		
		// Impale
		if ( Verify_Skill(attacker, RACE_CRYPT, SKILL1) ){
			new Float:randomnumber = random_float(0.0,1.0)
			if (randomnumber <= p_impale[p_data[attacker][P_SKILL1]-1]){
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
	if ( p_data[attacker][P_ITEM] == ITEM_CLAWS ){	
		WAR3_damage(victim, attacker, iCvar[FT_CLAW], wpnindex, hitplace)

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
	else if (p_data[attacker][P_ITEM] == ITEM_MASK){
		tempdamage = floatround(float(damage) * iCvar[FT_MASK_OF_DEATH])
		set_user_health(attacker, get_user_health(attacker) + tempdamage)

		if (get_user_health(attacker) > get_user_maxhealth(attacker))
			set_user_health(attacker, get_user_maxhealth(attacker)) 

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
	else if (p_data[attacker][P_ITEM] == ITEM_FROST){
		if (get_user_maxspeed(victim) > 10 && !p_data_b[victim][PB_SLOWED]){
			new normalspeed = floatround(get_user_maxspeed(victim))
			set_user_maxspeed(victim,fCvar[FT_FROST_SPEED])
			p_data_b[victim][PB_SLOWED]=true
			new parm[2]
			parm[0]=victim
			parm[1]=normalspeed
			set_task(1.0,"reset_maxspeed",TASK_RESETSPEED+victim,parm,2)

			if (iglow[victim][3] < 1){
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
	if ( Verify_Race(victim, RACE_ELF) ){

		// Evasion
		if ( Verify_Skill(victim, RACE_ELF, SKILL1) ) {
			new healthadjustment = 0
			new Float:randomnumber = random_float(0.0,1.0)

			if (randomnumber <= p_evasion[p_data[victim][P_SKILL1]-1]){
				p_data_b[victim][PB_EVADENEXTSHOT]=true
				if (get_user_health(victim) <= get_user_maxhealth(victim)){
					healthadjustment = 1024
				}
			}
			else{
				p_data_b[victim][PB_EVADENEXTSHOT]=false
				if (get_user_health(victim) > get_user_maxhealth(victim)){
					healthadjustment = -1024					
				}
			}
			if (p_data_b[victim][PB_EVADENEXTSHOT]){
				set_user_health(victim, get_user_health(victim) + damage + healthadjustment)

				if (iglow[victim][2] < 1){
					new parm[2]
					parm[0] = victim
					set_task(0.01,"glow_change",TASK_GLOW+victim,parm,2)
				}
				iglow[victim][2] += damage
				iglow[victim][0] = 0
				iglow[victim][1] = 0
				iglow[victim][3] = 0
				if (iglow[victim][2]>MAXGLOW)
					iglow[victim][2]=MAXGLOW

				Create_ScreenFade(victim, (1<<10), (1<<10), (1<<12), 0, 0, 255, iglow[victim][2])
			}
			else{
				set_user_health(victim, get_user_health(victim) + healthadjustment)
			}
		}
		
		// Thorns Aura
		if ( Verify_Skill(victim, RACE_ELF, SKILL2) ) {
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

			Create_ScreenFade(attacker, (1<<10), (1<<10), (1<<12), 255, 0, 0, iglow[attacker][0])
		}
	}

	// Crypt Lord
	else if ( Verify_Race(victim, RACE_CRYPT) ){

		// Spiked Carapace
		if ( Verify_Skill(victim, RACE_CRYPT, SKILL2) ){						
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
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("on_Death",victim)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	#if DEBUG == 1
		client_print(victim, print_chat,"*** Regular Death : %d", p_data_b[victim][PB_DIEDLASTROUND])
	#endif

	if(p_data_b[victim][PB_DIEDLASTROUND])
		return PLUGIN_CONTINUE


	if(killer > 0 )
		set_user_money(killer, get_user_money(killer)+300,1)

	WAR3_death_victim(victim, killer)
	
	XP_onDeath(victim, killer, wpnindex, headshot)

	return PLUGIN_CONTINUE
}

public on_DeathMsg(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("on_DeathMsg",read_data(2))
	#endif

	new killer = read_data(1)
	new victim = read_data(2)
	new headshot = read_data(3)
	new weapon
	get_user_attacker(victim,weapon)

	on_Death(victim, killer, weapon, headshot)
	
	return PLUGIN_CONTINUE
}

#if MOD == 0
	public on_ArmorType(id){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("on_ArmorType",id)
		#endif

		if (!warcraft3)
			return PLUGIN_CONTINUE

		if (read_data(1))
			p_data_b[id][PB_HELMET]=true
		else
			p_data_b[id][PB_HELMET]=false
		return PLUGIN_CONTINUE
	}

	public on_WeapPickup(id){ 
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("on_WeapPickup",id)
		#endif

		new weapon = read_data(1)
		if (weapon==25)
			++p_data[id][P_FLASHCOUNT]

		if (weapon==CSW_GALIL||weapon==CSW_FAMAS||weapon==CSW_M3||weapon==CSW_XM1014||weapon==CSW_MP5NAVY||weapon==CSW_TMP ||weapon==CSW_P90||weapon==CSW_MAC10||weapon==CSW_UMP45||weapon==CSW_AK47 ||weapon==CSW_SG552||weapon==CSW_M4A1||weapon==CSW_AUG||weapon==CSW_SCOUT ||weapon==CSW_AWP||weapon==CSW_G3SG1||weapon==CSW_SG550||weapon==CSW_M249)
			p_data_b[id][PB_SHIELD]=false

		return PLUGIN_CONTINUE 
	}

	public on_ShowStatus(id){
		#if ADVANCED_DEBUG == 1
			writeDebugInfo("on_ShowStatus",id)
		#endif

		if (g_freezetime){
			Create_StatusText(id, 0, "")
			return PLUGIN_HANDLED
		}

		new pid = read_data(2)

		if(get_user_team(id)==get_user_team(pid) && (p_data[pid][P_ITEM2]==ITEM_CHAMELEON || (p_data[pid][P_SKINCHANGED] == SKIN_HEX || p_data[pid][P_SKINCHANGED]==SKIN_SWITCH)) && is_user_alive(pid)){
			client_print(id,print_center,"%L",id,"HES_ON_YOUR_TEAM_DONT_SHOOT")
			client_cmd(id,"speak warcraft3/antend.wav")
		}

		// Used to show player icons
		if (g_spritesEnabled){
			if (warcraft3 && (get_user_team(id) == get_user_team(pid))){
				if (iCvar[FT_RACE_ICONS] && p_data[id][P_SHOWICONS]){
					Create_TE_PLAYERATTACHMENT(id, pid, 55, g_sRace[p_data[pid][P_RACE]], 15)
				}

				if (iCvar[FT_LEVEL_ICONS] && p_data[id][P_SHOWICONS])
					Create_TE_PLAYERATTACHMENT(id, pid, 35, g_sLevel[p_data[pid][P_LEVEL]], 16)
			}
		}

		return PLUGIN_CONTINUE
	}
#endif


public on_CurWeapon(id) {
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("on_CurWeapon",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id)){
			return PLUGIN_CONTINUE
		}
	}

	if (p_data[id][P_ITEM2]==ITEM_GLOVES){
		new wpnList[32] = 0 
		new number  = 0
		new foundNade = false 
		get_user_weapons(id,wpnList,number) 
		for (new i = 0;i < number && !foundNade;i++) { 
		#if MOD == 0
			if (wpnList[i] == CSW_HEGRENADE) 
				foundNade = true 
		#endif
		#if MOD == 1
			if (wpnList[i] == DODW_HANDGRENADE || wpnList[i] == DODW_STICKGRENADE) 
				foundNade = true 
		#endif
		}
		
		if(!p_data_b[id][PB_NADEJUSTRECEIVED] && !foundNade){
			new parm[2]
			parm[0] = id
			parm[1] = iCvar[FT_GLOVE_TIMER]

			p_data_b[id][PB_NADEJUSTRECEIVED]=true
			_Item_Glove(parm)
		}
	}

	new clipamount = 0
	new ammoamount = 0
	new weaponnum = 0
	weaponnum = get_user_weapon(id,clipamount,ammoamount)

	#if MOD == 1
		if (weaponnum==DODW_HANDGRENADE || weaponnum==DODW_STICKGRENADE){
			if( Verify_Skill(id, RACE_ORC, SKILL2) ){
				dod_set_fuse(id,FUSE_SET,2.0,FT_NEW)
			}
		}

	#endif

	// Invisibility

	if ( Verify_Skill(id, RACE_HUMAN, SKILL1)){
	#if MOD == 0
		if (weaponnum==CSW_KNIFE){
	#endif
	#if MOD == 1
		if (weaponnum==DODW_AMERKNIFE || weaponnum==DODW_GERKNIFE || weaponnum==DODW_SPADE){
	#endif
			set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,p_invisibility[p_data[id][P_SKILL1]-1]/2)
			p_data_b[id][PB_KNIFESELECTED]=true
		}
		else{
			set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,p_invisibility[p_data[id][P_SKILL1]-1])
			p_data_b[id][PB_KNIFESELECTED]=false
		}
	}
	else if (p_data[id][P_ITEM]==ITEM_CLOAK){
	#if MOD == 0
		if (weaponnum==CSW_KNIFE){
	#endif
	#if MOD == 1
		if (weaponnum==DODW_AMERKNIFE || weaponnum==DODW_GERKNIFE || weaponnum==DODW_SPADE){
	#endif
			set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,iCvar[FT_CLOAK]/2)
			p_data_b[id][PB_KNIFESELECTED]=true
		}
		else{
			set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,iCvar[FT_CLOAK])
			p_data_b[id][PB_KNIFESELECTED]=false
		}
	}
	else
		set_user_rendering(id)

	new parm[1]
	parm[0]=id
	unholyspeed(parm)

	return PLUGIN_CONTINUE
}

#if MOD == 0
public on_EndRound(){
	 #if ADVANCED_DEBUG == 1
		writeDebugInfo("on_EndRound",0)
	#endif

	if ( !warcraft3 )
		return PLUGIN_CONTINUE

	endround=true

	new players[32], numberofplayers
	new y, id
	get_players(players, numberofplayers)

	for (y = 0; y < numberofplayers; ++y){
		id = players[y]
		p_data_b[id][PB_SPAWNEDFROMITEM]=false
		p_data_b[id][PB_PLAYERSPAWNED]=false

		p_data_b[id][PB_MOLE] = false
		p_data[id][P_DEFUSERINDEX] = 0
	}

	g_freezecalled = 0
	g_buyCalled=false
	g_pheonixExistsT=0
	g_pheonixExistsCT=0
	g_randomizeCalled = false

	if( task_exists(TASK_BUYTIME) )
		remove_task(TASK_BUYTIME)

	if( task_exists(TASK_BOMBTIMER) )
		remove_task(TASK_BOMBTIMER)

	if (iCvar[MP_SAVEXP])
		XP_Save_All()

	race9_randomize()
	
	return PLUGIN_CONTINUE
}
#endif
public on_ResetHud(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("on_ResetHud",id)
	#endif

	if (warcraft3==false)
		return PLUGIN_CONTINUE

	if (iCvar[FT_CD]) {
		if (!WAR3_CD_installed(id))
			return PLUGIN_CONTINUE
	}

	if(is_user_bot(id)){
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= iCvar[FT_BOT_BUY_ITEM]){
			new num = random_num(1,2)
			if (num == 1)
				Item_Buy(id, random_num(0,8))
			else
				Item_Buy2(id, random_num(0,8))
		}
		if (randomnumber <= 0.06){
			p_data[id][P_XP]=xplevel[floatround(random_float(0.0,3.16)*random_float(0.0,3.16))]
			p_data[id][P_RACE] = random_num(1,iCvar[FT_RACES])
		}
	}

	new parm[2]
	parm[0] = id

	// Remove any reset_maxspeeds occuring (could cause a person to move during freezetime)
	if (task_exists(TASK_RESETSPEED+id)){			
		remove_task(TASK_RESETSPEED+id)
	}

	if (task_exists(TASK_LIGHTSEARCH+id)){
		remove_task(TASK_LIGHTSEARCH+id)
		p_data_b[id][PB_ISSEARCHING]=false
		icon_controller(id,ICON_HIDE)
	}

	if (task_exists(TASK_SEARCHTARGET+id)){
		remove_task(TASK_SEARCHTARGET+id)
		p_data_b[id][PB_ISSEARCHING]=false
		icon_controller(id,ICON_HIDE)
	}


	if(is_user_alive(id)){
		p_data_b[id][PB_JUSTJOINED] = false
	}

	if(p_data[id][P_RACE]==0){
		new parameter[1]
		parameter[0]=id
		set_task(0.3,"getuserinput",TASK_GETINPUT+id,parameter,1)
	}

	if(!g_randomizeCalled){
		race9_randomize()
		g_randomizeCalled = true
	}

	if(p_data_b[id][PB_GAMECOMMENCING]){
		p_data[id][P_ITEM] = 0
		p_data[id][P_ITEM2] = 0
		p_data_b[id][PB_DIEDLASTROUND]=false
		p_data_b[id][PB_GAMECOMMENCING]=false
		#if MOD == 0
			p_data[id][P_HECOUNT] = 0
			p_data[id][P_FLASHCOUNT]=0
		#endif
	}
	#if MOD == 0
		if(iCvar[FT_BUYTIME] && !g_buyCalled){
			set_task(get_cvar_float("mp_buytime")*60.0,"_WAR3_set_buytime",TASK_BUYTIME)
			g_buyCalled = true
		}
	#endif
	icon_controller(id,ICON_HIDE)

	p_data_b[id][PB_TOBEREVIVED] = false
	p_data_b[id][PB_CHANGINGTEAM] = false

	#if MOD == 1
		if(p_data[id][P_ITEM] == ITEM_ANKH){
			p_data_b[id][PB_REINCARNATION_DELAY] = false
			SKILL_Reincarnation(id)
		}
		else if( Verify_Skill(id, RACE_ORC, SKILL3) ){
			new Float:randomnumber = random_float(0.0,1.0)
			if(randomnumber <= p_ankh[p_data[id][P_SKILL3]-1])
				SKILL_Reincarnation(id)
		}
	#endif

	#if MOD == 0
		if (p_data_b[id][PB_PLAYERSPAWNED]){

			// Reset Serpent Wards
			if( Verify_Skill(id, RACE_SHADOW, SKILL3) ){
				p_data[id][P_SERPENTCOUNT] = p_serpent[p_data[id][P_SKILL3]-1]
			}

			icon_controller(id,ICON_SHOW)

			if(p_data_b[id][PB_STUNNED] || p_data_b[id][PB_SLOWED]){
				p_data_b[id][PB_STUNNED] = false
				p_data_b[id][PB_SLOWED] = false
			}

			new unholyparm[1]
			unholyparm[0]=id
			unholyspeed(unholyparm)

			if(p_data[id][P_ITEM2]!=ITEM_MOLE)
				p_data[id][P_ITEM2]=0

			p_data[id][P_ITEM]=0
			changeskin(id,SKIN_RESET)

			WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
			return PLUGIN_HANDLED
		}
	#endif

	p_data[id][P_HECOUNT]=0
	p_data[id][P_FLASHCOUNT]=0

	// Stop any cooldowns in effect	

	if (task_exists(TASK_UDELAY+id))		
		remove_task(TASK_UDELAY+id)

	// Start a new cooldown

	if (iCvar[FT_ULTIMATE_DELAY] > 0){
		p_data_b[id][PB_ULTIMATEUSED] = true
		p_data[id][P_ULTIMATEDELAY] = iCvar[FT_ULTIMATE_DELAY]
	}
	else if(p_data[id][P_ULTIMATE]){
		p_data[id][P_ULTIMATEDELAY] = 0
		p_data_b[id][PB_ULTIMATEUSED] = false
	}

	_WAR3_Ultimate_Delay(parm)

	if (p_data[id][P_RACE] == 9 && iCvar[FT_9RACERANDOM]){
		WAR3_Display_Level(id,DISPLAYLEVEL_SHOWRACE)
	}

	// User has a race selection pending, set it
	if (p_data[id][P_CHANGERACE]){
		WAR3_set_race(id, p_data[id][P_CHANGERACE])
	}	

	#if MOD == 0
		g_buyTime=true

		if (g_freezecalled==0){
			g_freezetime = true
			g_freezecalled = 1
		}

		g_hostageSaver = 0
		g_bombDefuser = 0

	#endif

	if (iCvar[FT_WARN_SUICIDE])
		p_data_b[id][PB_SUICIDEATTEMPT] = false

	// Weapon Reincarnation

	set_task(0.1, "weapon_controller", TASK_REINCARNATION+id, parm, 2)

	endround=false
	p_data_b[id][PB_ISBURNING] = false
	p_data[id][P_FLAMECOUNT]=0
	p_data_b[id][PB_WARDENBLINK] = false

	// Checks skills
	Skill_Check(id)

	// Fan of Knives Check

	if ( Verify_Skill(id, RACE_WARDEN, SKILL1) && is_user_alive(id) ){
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= p_fan[p_data[id][P_SKILL1]-1]){
			new fanparm[2]
			fanparm[0]=id
			fanparm[1]=7
			set_task(0.1,"_Item_Mole",TASK_FAN+id,fanparm,2)
		}
	}

	if (p_data_b[id][PB_RESETSKILLS]) {
		p_data[id][P_SKILL1]=0
		p_data[id][P_SKILL2]=0
		p_data[id][P_SKILL3]=0
		p_data[id][P_ULTIMATE]=0
		p_data[id][P_LEVEL] = 0
		p_data_b[id][PB_RESETSKILLS]=false
		WAR3_Display_Level(id,DISPLAYLEVEL_NONE)
	}

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
	if (skillsused < p_data[id][P_LEVEL]){
		menu_Select_Skill(id,0)
	}
	return PLUGIN_CONTINUE
}

public on_GameRestart(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("on_GameRestart",0)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	XP_Save_All()

	new players[32], numplayers, id
	get_players(players, numplayers)
	for (new i=0; i<numplayers; i++){
		id=players[i]
		p_data_b[id][PB_GAMECOMMENCING]=true

		if(task_exists(TASK_UDELAY+id))
			remove_task(TASK_UDELAY+id)
		
		if(p_data[P_ITEM2][id])
			Item_Set_Helm(id,0)

		p_data[id][P_ITEM] = 0
		p_data[id][P_ITEM2] = 0
		p_data_b[id][PB_DIEDLASTROUND] = false
		p_data[id][P_RINGS]=0
		if(iCvar[MP_SAVEXP]==0){
			p_data[id][P_LEVEL]=0
			p_data[id][P_RACE]=0
			p_data[id][P_SKILL1]=0
			p_data[id][P_SKILL2]=0
			p_data[id][P_SKILL3]=0
			p_data[id][P_ULTIMATE]=0
			p_data[id][P_XP]=0
		}
		#if MOD == 0
			p_data[id][P_HECOUNT]=0
			p_data[id][P_FLASHCOUNT]=0
		#endif
	}

	return PLUGIN_CONTINUE
}

public on_StatusIcon(id) {

	new szStatusItem[8]
	read_data(2, szStatusItem, 7)

	if ( equali(szStatusItem, "defuser") ){
		if ( read_data(1) )
			p_data_b[id][PB_DEFUSE] = true
		else
			p_data_b[id][PB_DEFUSE] = false
	}

	if ( equali(szStatusItem, "buyzone") ){
		if ( read_data(1) )
			p_data_b[id][PB_BUYZONE] = true
		else
			p_data_b[id][PB_BUYZONE] = false
	}

	return PLUGIN_CONTINUE
}