/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Blood Mage Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define IMMOLATE_DAMAGE     40
#define IMMOLATE_DOT_DAMAGE  5
#define IMMOLATE_DOT         4


public SH_ULT_Immolate( iCaster, iTarget )
{
	emit_sound( iCaster, CHAN_STATIC, SOUND_IMMOLATE, 0.5, ATTN_NORM, 0, PITCH_NORM );

	new vTargetOrigin[3];
	get_user_origin( iTarget, vTargetOrigin );

	Create_TE_EXPLOSION( vTargetOrigin, vTargetOrigin, g_sImmolate, 40, 24, 4 );

	Create_ScreenShake( iTarget, (10<<12), (2<<12), (5<<12) );

	WAR3_damage( iTarget, iCaster, IMMOLATE_DAMAGE, CSW_IMMOLATE, -1 )

	new parm_DoT[3];
	parm_DoT[0] = iCaster;
	parm_DoT[1] = iTarget;
	parm_DoT[2] = 0;

	new TaskId = TASK_BURNING + iTarget;
	set_task( 1.0, "SH_ULT_Immolate_DoT", TaskId, parm_DoT, 3 );
	
	return PLUGIN_HANDLED;
}

public SH_ULT_Immolate_DoT( parm_DoT[3] )
{
	new iCaster = parm_DoT[0];
    new iTarget = parm_DoT[1];
	new iCounter = parm_DoT[2];

	if ( iCounter >= IMMOLATE_DOT ) 
	{
		SH_ULT_Immolate_Remove( iTarget );
		return PLUGIN_HANDLED;
	}

    new vTargetOrigin[3];
    get_user_origin( iTarget, vTargetOrigin );

	emit_sound( iTarget, CHAN_STATIC, SOUND_IMMOLATE, 0.5, ATTN_NORM, 0, PITCH_NORM );

	Create_TE_SPRITE( vTargetOrigin, g_sFire, 5, 200 );

	WAR3_damage( iTarget, iCaster, IMMOLATE_DOT, CSW_IMMOLATE, -1 )

    if ( is_user_alive( iTarget ) )
    {
		p_data[iTarget][PB_ISBURNING] = true;

        Create_ScreenFade( iTarget, (1<<10), (1<<10), 0, 255, 108, 0, 160 );

		parm_DoT[2] = iCounter++;

		new TaskId = TASK_BURNING + iTarget;
		set_task( 1.0, "SH_ULT_Immolate_DoT", TaskId, parm_DoT, 3 );

	}

	return PLUGIN_HANDLED;
}


SH_ULT_Immolate_Remove( iTarget ) {

	p_data[iTarget][PB_ISBURNING] = false;

	new TaskId = TASK_BURNING + iTarget;
    remove_task( TaskId, 0 );

    return PLUGIN_HANDLED;
}


