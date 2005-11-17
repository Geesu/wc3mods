public checkmap(){
	#if ADVANCED_DEBUG
		writeDebugInfo("checkmap",0)
	#endif

	#if MOD ==0
		new mapname[32]
		get_mapname(mapname,31) 
		if (containi(mapname,"ka_")!=-1 || containi(mapname,"scoutzknivez")!=-1){
			g_givePistol=false
			if (iCvar[FT_NO_GLOVES_ON_KA])
				g_giveHE=false
			else
				g_giveHE=true
		}
		else{
			g_giveHE=true
			g_givePistol=true
		}
		if(iCvar[FT_NO_ORCNADES]){
			if (containi(mapname,"ka_")!=-1 || containi(mapname,"jail_riot")!=-1 || containi(mapname,"fy_")!=-1) 
				g_notAllowHE=true
			else
				g_notAllowHE=false
		}
	#endif
	
	return PLUGIN_CONTINUE
}

public changeskin(id,reset){							// Function changes your skin for ITEM_MOLE and Chameleon
	#if ADVANCED_DEBUG
		writeDebugInfo("changeskin",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

#if MOD == 0
	new num = 0, newSkin[32]
	if (is_running("cstrike"))
		num = random_num(0,3)
	else if (is_running("czero"))
		num = random_num(0,4)

	
	if (reset==SKIN_RESET && p_data[id][P_SKINCHANGED]==SKIN_SWITCH){
		cs_reset_user_model(id)
		p_data[id][P_SKINCHANGED]=SKIN_RESET
	}
	else if (reset==SKIN_SWITCH){
		if (get_user_team(id)==TS)
			add(newSkin,31,CTSkins[num])
		else
			add(newSkin,31,TSkins[num])

		cs_set_user_model(id,newSkin)
		p_data[id][P_SKINCHANGED]=SKIN_SWITCH
	}
#endif
	return PLUGIN_CONTINUE
}

public getuserinput(parm[1]){
	#if ADVANCED_DEBUG
		writeDebugInfo("getuserinput",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
	if (p_data[id][P_RACE] == 0){
		WAR3_chooserace(id)
	}
	else if (skillsused < p_data[id][P_LEVEL])
		menu_Select_Skill(id,0)
	else
		WAR3_Display_Level(id, DISPLAYLEVEL_NONE)

	return PLUGIN_HANDLED
}

public saveweapons(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("saveweapons",id)
	#endif

	#if MOD == 0
		p_data[id][P_ARMORONDEATH]=get_user_armor(id)
	#endif
	for (new i=0; i<32; ++i){		// Equipment Reincarnation
		savedweapons[id][i]=0
	}

	p_data[id][P_SAVEDNUMBER]=0
	get_user_weapons(id,savedweapons[id],p_data[id][P_SAVEDNUMBER])

	return PLUGIN_CONTINUE
}

public glow_change(parm[]){
	#if ADVANCED_DEBUG
		writeDebugInfo("glow_change",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if ( !p_data_b[id][PB_RENDER] )		// Don't render if we shouldn't
	{
		return PLUGIN_CONTINUE
	}
	else if ( Verify_Skill(id, RACE_HUMAN, SKILL1) ){	// Don't glow if player is invisible
		iglow[id][0] = 0
		iglow[id][1] = 0
		iglow[id][2] = 0
		iglow[id][3] = 0
	}
	else if ( p_data[id][P_ITEM]==ITEM_CLOAK ){		// Don't glow if player is invisible
		iglow[id][0] = 0
		iglow[id][1] = 0
		iglow[id][2] = 0
		iglow[id][3] = 0
	}
	else if (iglow[id][0] > 5){
		set_user_rendering(id,kRenderFxGlowShell,iglow[id][0],0,0, kRenderNormal, 16)
		iglow[id][0] -= 5
		set_task(0.2,"glow_change",TASK_GLOW+id,parm,2)
	}
	else if (iglow[id][1] > 5){
		set_user_rendering(id,kRenderFxGlowShell,0,iglow[id][1],0, kRenderNormal, 16)
		iglow[id][1] -= 5
		set_task(0.2,"glow_change",TASK_GLOW+id,parm,2)
	}
	else if (iglow[id][2] > 5){
		set_user_rendering(id,kRenderFxGlowShell,0,0,iglow[id][2], kRenderNormal, 16)
		iglow[id][2] -= 5
		set_task(0.2,"glow_change",TASK_GLOW+id,parm,2)
	}
	else if (iglow[id][3] > 5){
		set_user_rendering(id,kRenderFxGlowShell,iglow[id][3],iglow[id][3],iglow[id][3], kRenderNormal, 16)
		iglow[id][3] -= 5
		set_task(0.2,"glow_change",TASK_GLOW+id,parm,2)
	}
	else{
		iglow[id][0] = 0
		iglow[id][1] = 0
		iglow[id][2] = 0
		iglow[id][3] = 0
		set_user_rendering(id)
	}
	return PLUGIN_CONTINUE
}

public reset_maxspeed(parm[]){
	#if ADVANCED_DEBUG
		writeDebugInfo("reset_maxspeed",parm[0])
	#endif

	new enemy = parm[0]

	if(!p_data_b[enemy][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	p_data_b[enemy][PB_STUNNED]=false
	p_data_b[enemy][PB_SLOWED]=false

	set_user_maxspeed( enemy, float(parm[1]) )
	return PLUGIN_CONTINUE
}

public unholyspeed(parm[1]){
	#if ADVANCED_DEBUG
		writeDebugInfo("unholyspeed",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (p_data_b[id][PB_STUNNED]){
		set_user_maxspeed(id,1.0)
		return PLUGIN_HANDLED
	}
	else if (p_data_b[id][PB_HEXED]){
		set_user_maxspeed(id, SKILL_HEX_SPEED)
		return PLUGIN_HANDLED
	}
	else if (p_data_b[id][PB_SLOWED]){
		set_user_maxspeed(id,fCvar[FT_FROST_SPEED])
		return PLUGIN_HANDLED
	}
#if MOD == 1
	else if(entity_get_int(id,EV_INT_iuser3)){	// User is in prone position, don't change speed
		if(get_user_maxspeed(id) > 500.0)
			set_user_maxspeed(id, 50.0)

		return PLUGIN_HANDLED
	}
	else if(get_user_maxspeed(id) == 50.0 && ( p_data[id][P_ITEM] == ITEM_BOOTS || Verify_Skill(id, RACE_UNDEAD, SKILL2) ) && !p_data_b[id][PB_HEXED] ){		// User has a rocket launcher "mounted"
		set_user_maxspeed(id, 600.0)
	}
#endif
#if MOD == 0
	else if ( Verify_Skill(id, RACE_UNDEAD, SKILL2) && !g_freezetime && !p_data_b[id][PB_HEXED] ){              // Unholy Aura
		if (get_user_maxspeed(id)>5 && get_user_maxspeed(id)!=p_unholy[p_data[id][P_SKILL2]-1])
			set_user_maxspeed(id,(p_unholy[p_data[id][P_SKILL2]-1]))
	}
	else if ( p_data[id][P_ITEM]==ITEM_BOOTS && !g_freezetime && !p_data_b[id][PB_HEXED] ){			// Boots of Speed
		if (get_user_maxspeed(id)!=fCvar[FT_BOOTSPEED])
			set_user_maxspeed(id,fCvar[FT_BOOTSPEED])
	}
#endif

	return PLUGIN_CONTINUE
}

public func_spawn(parm[2]){
	#if ADVANCED_DEBUG
		writeDebugInfo("func_spawn",parm[0])
	#endif

	if (endround){
		set_user_money(parm[0],get_user_money(parm[0])+itemcost2[0],1)
		return PLUGIN_HANDLED
	}

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(get_user_team(id)==UNASSIGNED)
		return PLUGIN_CONTINUE

	p_data[id][P_ITEM]=0
	p_data[id][P_ITEM2]=0
	
	if (is_user_alive(id))
		return PLUGIN_CONTINUE

	new team[32]
	get_user_team(id,team,31)
	p_data_b[id][PB_PLAYERSPAWNED]=true
	Ultimate_Icon(id,ICON_SHOW)
	spawn(id)
	p_data_b[id][PB_SLOWED]=false
	p_data_b[id][PB_STUNNED]=false
	spawn(id)
	set_task(0.3,"spawn_player",TASK_SPAWNPLAYER+id,parm,2)
	set_task(0.5,"player_giveitems",TASK_GIVEITEMS+id,parm,2)

#if MOD == 0
	give_item(id, "weapon_knife")
#endif
	
	new money = get_user_money(id)

	#if MOD == 0
		if (money < get_cvar_num("mp_startmoney"))
			set_user_money(id,get_cvar_num("mp_startmoney"),0)
	#endif
	#if MOD == 1
		if (money < iCvar[DOD_STARTMONEY])
			set_user_money(id,iCvar[DOD_STARTMONEY],0)

	#endif
	return PLUGIN_CONTINUE
}


public player_giveitems(parm[2]){
	#if ADVANCED_DEBUG
		writeDebugInfo("player_giveitems",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	// Vengeance
	if( Verify_Skill(id, RACE_WARDEN, SKILL4) && !p_data_b[id][PB_SPAWNEDFROMITEM])
		set_user_health(id, 50)

	// Evasion
	//else if ( Verify_Skill(id, RACE_ELF, SKILL1) && p_data_b[id][PB_EVADENEXTSHOT])
	//	set_user_health(id, 1124)

	// Devotion Aura
	else if ( Verify_Skill(id, RACE_HUMAN, SKILL2))							
		set_user_health(id,p_devotion[p_data[id][P_SKILL2]-1])
	
	// Evasion
	Skill_Evasion_Set( id );

	p_data_b[id][PB_GIVEITEMS]=true
	set_task(0.1, "Skill_Reincarnation", TASK_REINCARNATION+id, parm, 2)							// Give weapons back

	give_item(id, "item_suit")

	#if MOD == 0
		give_item(id, "weapon_knife")
	#endif

	// The following code decides if pistols should be given
	#if MOD == 0
		if (!g_givePistol)
			return PLUGIN_CONTINUE

		new wpnList[32] = 0, number = 0, foundGlock = false, foundUSP = false 
		get_user_weapons(id,wpnList,number)
		for (new i = 0;i < number;i++) { 
			if (wpnList[i] == CSW_GLOCK18) 
				foundGlock = true 
			if (wpnList[i] == CSW_USP) 
				foundUSP = true 
		} 
		if (get_user_team(id)==TS){
			if(foundGlock)
				return PLUGIN_CONTINUE
			else{
				give_item(id,"weapon_glock18")
				give_item(id,"ammo_9mm")
				give_item(id,"ammo_9mm")
				give_item(id,"ammo_9mm")
			}

		}
		if (get_user_team(id)==CTS){
			if(foundUSP)
				return PLUGIN_CONTINUE
			else{
				give_item(id,"weapon_usp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
				give_item(id,"ammo_45acp")
			}
		}
	#endif
	return PLUGIN_CONTINUE

}

public spawn_player(parm[2]){
	#if ADVANCED_DEBUG
		writeDebugInfo("spawn_player",parm[0])
	#endif

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	p_data_b[id][PB_PLAYERSPAWNED]=true
	spawn(id)
	give_item(id, "item_suit")
#if MOD == 0
	give_item(id, "weapon_knife")
#endif
	return PLUGIN_CONTINUE

}