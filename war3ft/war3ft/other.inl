public checkmap(){

	#if MOD ==0
		new mapname[32]
		get_mapname(mapname,31) 
		if (containi(mapname,"ka_")!=-1 || containi(mapname,"scoutzknivez")!=-1){
			g_givePistol=false
			if (get_pcvar_num( CVAR_wc3_glove_disable_ka ))
				g_giveHE=false
			else
				g_giveHE=true
		}
		else{
			g_giveHE=true
			g_givePistol=true
		}
	#endif
	
	return PLUGIN_CONTINUE
}

public changeskin(id,reset){							// Function changes your skin for ITEM_MOLE and Chameleon

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

public saveweapons(id)
{

	#if MOD == 0
		new CsArmorType:armortype
		p_data[id][P_ARMORONDEATH] = cs_get_user_armor( id, armortype );
	#endif
	
	/*for (new i=0; i<32; ++i){		// Equipment Reincarnation
		savedweapons[id][i]=0
	}

	p_data[id][P_SAVEDNUMBER] = 0;
	get_user_weapons(id,savedweapons[id],p_data[id][P_SAVEDNUMBER])
	*/

	return PLUGIN_CONTINUE
}

public glow_change(parm[]){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if ( !p_data_b[id][PB_CAN_RENDER] )		// Don't render if we shouldn't
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

	new enemy = parm[0]

	if(!p_data_b[enemy][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	p_data_b[enemy][PB_STUNNED]=false
	p_data_b[enemy][PB_SLOWED]=false

	set_user_maxspeed( enemy, float(parm[1]) )
	return PLUGIN_CONTINUE
}

public unholyspeed(parm[1]){

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
		set_user_maxspeed(id, get_pcvar_float( CVAR_wc3_frost ))
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
	else if ( Verify_Skill(id, RACE_UNDEAD, SKILL2) && !g_freezeTime && !p_data_b[id][PB_HEXED] ){              // Unholy Aura
		if (get_user_maxspeed(id)>5 && get_user_maxspeed(id)!=p_unholy[p_data[id][P_SKILL2]-1])
			set_user_maxspeed(id,(p_unholy[p_data[id][P_SKILL2]-1]))
	}
	else if ( p_data[id][P_ITEM]==ITEM_BOOTS && !g_freezeTime && !p_data_b[id][PB_HEXED] ){			// Boots of Speed
		if (get_user_maxspeed(id)!=get_pcvar_float( CVAR_wc3_boots ))
			set_user_maxspeed(id,get_pcvar_float( CVAR_wc3_boots ))
	}
#endif

	return PLUGIN_CONTINUE
}

