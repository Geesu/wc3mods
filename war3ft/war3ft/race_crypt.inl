/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Crypt Lord Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define ULT_LOCUSTSWARM_DAMAGE			45

CL_ULT_LocustSwarm( id )
{

	new iTeam = get_user_team( id );

	// First thing we need to do is find an alive player that isn't immune to target :)
	new players[32], numberofplayers, i, iTargetID;
	new iTargets[33], iTotalTargets = 0;

	get_players( players, numberofplayers, "a" );

	for ( i = 0; i < numberofplayers; i++ )
	{
		iTargetID = players[i];

		if ( get_user_team( iTargetID ) != iTeam && !ULT_IsImmune( iTargetID ) )
		{
			iTargets[iTotalTargets++] = iTargetID;
		}
	}

	// No target was found :/
	if ( iTotalTargets == 0 )
	{

		set_hudmessage( 178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, -1 );
		show_hudmessage( id, "%L", id, "NO_VALID_TARGETS_FOUND" );

		return;
	}
	
	// Lets find ourselves a victim
	new iVictim = 0, iRandomSpot;
	while ( iVictim == 0 )
	{
		iRandomSpot = random_num( 0, iTotalTargets );

		iVictim = iTargets[iRandomSpot];
	}

	// Lets get the origin of the caster
	new vCasterOrigin[3];
	get_user_origin( id, vCasterOrigin );
	
	// Flash their ultimate!
	ULT_Icon( id, ICON_FLASH );
	
	// Reset the user's ultimate - but we don't want to hide the icon yet!!
	ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ), false );

	new parm[10];
	parm[0] = id;		// caster
	parm[1] = iVictim;	// victim
	parm[2] = vCasterOrigin[0];
	parm[3] = vCasterOrigin[1];
	parm[4] = vCasterOrigin[2];

	// Start the effects!!
	_CL_ULT_LocustEffect( parm );

	return;
}

public _CL_ULT_LocustEffect( parm[] )
{
	new iAttacker	= parm[0];
	new iVictim		= parm[1];

	if ( ULT_IsImmune( iVictim ) || !is_user_alive( iVictim ) || !p_data_b[iVictim][PB_ISCONNECTED] )
	{
		client_print( iAttacker, print_chat, "%s Target is no longer targetable, try casting again!" );

		Ultimate_Ready( iAttacker );

		return;
	}

	new MULTIPLIER = 150 // the lower the number the faster it reaches the target

	new vVictimOrigin[3], vFunnel[3];
	get_user_origin( iVictim, vVictimOrigin );

	// Origin of the funnel
	vFunnel[0] = parm[2];
	vFunnel[1] = parm[3];
	vFunnel[2] = parm[4];
	
	// Draw a funnel
	Create_TE_LARGEFUNNEL( vFunnel, g_sSnow, 0 );
	
	// Now we need to calculate where the next funnel will be drawn
	new vDist[3];
	vDist[XPOS] = diff( vVictimOrigin[0], vFunnel[0] );
	vDist[YPOS] = diff( vVictimOrigin[1], vFunnel[1] );
	vDist[ZPOS] = diff( vVictimOrigin[2], vFunnel[2] );	
	
	new i;
	for ( i = 0; i < 3; i++ )
	{
		
		if ( diff( vVictimOrigin[i], vFunnel[i] - MULTIPLIER ) < vDist[i] )
		{
			vFunnel[i] -= MULTIPLIER;
		}
		else if ( diff( vVictimOrigin[i], vFunnel[0] + MULTIPLIER ) < vDist[i] )
		{
			vFunnel[i] += MULTIPLIER;
		}
		else
		{
			vFunnel[i] = vVictimOrigin[i];
		}
	}

	// Now we need to set the parms so (if/when) we call this function again it knows the next origin
	parm[2] = vFunnel[0];
	parm[3] = vFunnel[1];
	parm[4] = vFunnel[2];


	// Still not close enough to our target
	if ( !( vDist[XPOS] < 50 && vDist[YPOS] < 50 && vDist[ZPOS] < 50 ) )
	{
		
		new Float:fTime = 0.2;

		if ( g_MOD == GAME_DOD )
		{
			fTime = 0.5;
		}

		set_task( fTime, "_CL_ULT_LocustEffect", iAttacker + TASK_FUNNELS, parm, 5 );
	}

	// We're close enough, we can damage them!
	else
	{
		WAR3_damage( iVictim, iAttacker, ULT_LOCUSTSWARM_DAMAGE, CSW_LOCUSTS, -1 );

		emit_sound( iVictim, CHAN_STATIC, SOUND_LOCUSTSWARM, 1.0, ATTN_NORM, 0, PITCH_NORM );

		ULT_ResetCooldown( iVictim, get_pcvar_num( CVAR_wc3_ult_cooldown ) );
	}

	return;
}
