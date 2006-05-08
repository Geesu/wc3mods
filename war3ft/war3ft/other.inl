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