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