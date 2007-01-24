/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Crypt Lord Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define LOCUSTSWARM_DMG_MIN				30
#define LOCUSTSWARM_DMG_MAX				60
#define CARRIONBEETLE_DAMAGE			10
#define IMPALE_INTENSITY				10.0		// Intensity of impale

// Returns victim id
//  -1 if no target
//  -2 if ult blocked
CL_ULT_LocustGetTarget( id )
{
	new iTeam = get_user_team( id );

	// First thing we need to do is find an alive player that isn't immune to target :)
	new players[32], numberofplayers, i, iTargetID;
	new iTargets[33], iTotalTargets = 0;

	get_players( players, numberofplayers, "a" );

	for ( i = 0; i < numberofplayers; i++ )
	{
		iTargetID = players[i];

		if ( get_user_team( iTargetID ) != iTeam )
		{
			iTargets[iTotalTargets++] = iTargetID;
		}
	}
	
	// No victims found
	if ( iTotalTargets == 0 )
	{
		return -1;
	}

	// Lets find ourselves a victim
	new iVictim = 0, iRandomSpot;
	while ( iVictim == 0 )
	{
		iRandomSpot = random_num( 0, iTotalTargets );

		iVictim = iTargets[iRandomSpot];
	}

	// Then the user's ultimate was blocked :/
	if ( ULT_IsImmune( iVictim ) )
	{
		ULT_Blocked( id );

		// Reset the user's ultimate
		ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) );

		return -2;
	}

	return iVictim;
}

CL_ULT_LocustSwarm( id )
{

	new iVictim = CL_ULT_LocustGetTarget( id );

	// No target was found :/
	if ( iVictim == -1 )
	{
		//set_hudmessage( 178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, -1 );
		WC3_StatusText( id, TXT_BLINK_CENTER, "No valid targets found" );

		return;
	}

	// Ultimate was blocked - do nothing
	if ( iVictim == -2 )
	{
		return;
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

	if ( !is_user_alive( iVictim ) || !p_data_b[iVictim][PB_ISCONNECTED] )
	{
		client_print( iAttacker, print_chat, "%s Victim is no longer targetable for Locust Swarm, finding new target!", g_MODclient );

		// Need to find a new victim here...
		new iVictim = CL_ULT_LocustGetTarget( iAttacker );

		// No target was found :/
		if ( iVictim == -1 )
		{
			//set_hudmessage( 178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, -1 );
			WC3_StatusText( iAttacker, TXT_BLINK_CENTER, "No valid targets found" );
			
			// Set ultimate timer back to 0
			p_data[iAttacker][P_ULTIMATEDELAY] = 0;

			// Show icon + play ultimate ready sound
			ULT_IconHandler( iAttacker );

			return;
		}

		// Ultimate was blocked - do nothing
		else if ( iVictim == -2 )
		{
			return;
		}


		// Lets get the new origin of the caster
		new vCasterOrigin[3];
		get_user_origin( id, vCasterOrigin );
		parm[1] = iVictim;	// victim
		parm[2] = vCasterOrigin[0];
		parm[3] = vCasterOrigin[1];
		parm[4] = vCasterOrigin[2];

		client_print( iAttacker, print_chat, "[DEBUG] Victim is no longer targetable, try casting again!" );
		
		/*client_print( iAttacker, print_chat, "%s Victim is no longer targetable, try casting again!", g_MODclient );

		p_data[iAttacker][P_ULTIMATEDELAY] = 0;

		Ultimate_Ready( iAttacker );
		*/

		return 0;
	}

	new MULTIPLIER = 150 // the lower the number the faster it reaches the target

	new vVictimOrigin[3], vFunnel[3];
	get_user_origin( iVictim, vVictimOrigin );

	// Origin of the funnel
	vFunnel[0] = parm[2];
	vFunnel[1] = parm[3];
	vFunnel[2] = parm[4];
	
	// Draw a funnel
	Create_TE_LARGEFUNNEL( vFunnel, g_iSprites[SPR_SNOW], 0 );
	
	// Now we need to calculate where the next funnel will be drawn
	new vDist[3];
	vDist[XPOS] = CL_HLP_Diff( vVictimOrigin[0], vFunnel[0] );
	vDist[YPOS] = CL_HLP_Diff( vVictimOrigin[1], vFunnel[1] );
	vDist[ZPOS] = CL_HLP_Diff( vVictimOrigin[2], vFunnel[2] );	
	
	new i;
	for ( i = 0; i < 3; i++ )
	{
		
		if ( CL_HLP_Diff( vVictimOrigin[i], vFunnel[i] - MULTIPLIER ) < vDist[i] )
		{
			vFunnel[i] -= MULTIPLIER;
		}
		else if ( CL_HLP_Diff( vVictimOrigin[i], vFunnel[0] + MULTIPLIER ) < vDist[i] )
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
		new iDamage = random_num( LOCUSTSWARM_DMG_MIN, LOCUSTSWARM_DMG_MAX );

		WC3_Damage( iVictim, iAttacker, iDamage, CSW_LOCUSTS, -1 );

		emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_LOCUSTSWARM], 1.0, ATTN_NORM, 0, PITCH_NORM );

		ULT_ResetCooldown( iAttacker, get_pcvar_num( CVAR_wc3_ult_cooldown ) );
		
		new szName[32];
		get_user_name( iVictim, szName, 31 );

		client_print( iAttacker, print_chat, "%s Locust Swarm hit %s for %d damage!", g_MODclient, szName, iDamage );
	}
}

CL_HLP_Diff( iNum, iNum2 )
{
	if ( iNum > iNum2 )
	{
		return (iNum-iNum2);
	}
	else
	{
		return (iNum2-iNum);
	}

	return 0;
}

CL_SkillsOffensive( iAttacker, iVictim, iHitPlace )
{
	static iSkillLevel;

	// Orb of Annihilation
	iSkillLevel = SM_GetSkillLevel( iAttacker, PASS_ORB );
	if ( iSkillLevel > 0 )
	{

		if ( random_float( 0.0, 1.0 ) <= p_orb[p_data[iAttacker][P_LEVEL]] )
		{
			new vVictimOrigin[3];
			get_user_origin( iVictim, vVictimOrigin );
			
			vVictimOrigin[2] -= 20;
			
			// Create the orb effect
			Create_TE_SPRITE( vVictimOrigin, g_iSprites[SPR_WAVE], 10, 200 );
			
			// Play the orb sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_ANNIHILATION], 1.0, ATTN_NORM, 0, PITCH_NORM );

			// Damage the user
			WC3_Damage( iVictim, iAttacker, ORB_DAMAGE, CSW_ORB, iHitPlace );
		}
		else if ( get_pcvar_num( CVAR_wc3_psychostats ) )
		{
			new WEAPON = CSW_ORB - CSW_WAR3_MIN;

			iStatsShots[iAttacker][WEAPON]++;
		}
	}

	// Carrion Beetles
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_CARRIONBEETLES );
	if ( iSkillLevel > 0 )
	{

		if ( random_float( 0.0, 1.0 ) <= p_carrion[iSkillLevel-1] )
		{
			new vVictimOrigin[3], vAttackerorigin[3];
			get_user_origin( iVictim, vVictimOrigin );
			get_user_origin( iAttacker, vAttackerorigin );
			
			// Create the Carrion Beetle effect
			Create_TE_SPRITETRAIL( vAttackerorigin, vVictimOrigin, g_iSprites[SPR_BEETLE], 15, 15, 1, 2, 6 );
			
			// Play the carrion beetle sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_CARRION], 1.0, ATTN_NORM, 0, PITCH_NORM );

			WC3_Damage( iVictim, iAttacker, CARRIONBEETLE_DAMAGE, CSW_CARRION, iHitPlace );
		}

		else if ( get_pcvar_num( CVAR_wc3_psychostats ) )
		{
			new WEAPON = CSW_CARRION - CSW_WAR3_MIN;

			iStatsShots[iAttacker][WEAPON]++;
		}
	}
	

	// Impale
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_IMPALE );
	if ( iSkillLevel > 0 )
	{

		if ( random_float( 0.0, 1.0 ) <= p_impale[iSkillLevel-1] )
		{

			// Play the impale sound
			emit_sound( iVictim, CHAN_STATIC, g_szSounds[SOUND_IMPALE], 1.0, ATTN_NORM, 0, PITCH_NORM );
			
			new Float:vVelocity[3];
			entity_get_vector( iVictim, EV_VEC_velocity, vVelocity );

			vVelocity[0] = random_float( 100.0, 400.0 );
			vVelocity[1] = random_float( 100.0, 400.0 );
			vVelocity[2] = random_float( 400.0, 700.0 );

			entity_set_vector( iVictim, EV_VEC_velocity, vVelocity );
		}
	}
}

CL_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace )
{
	static iSkillLevel;

	// Spiked Carapace
	iSkillLevel = SM_GetSkillLevel( iVictim, SKILL_SPIKEDCARAPACE );
	if ( iSkillLevel > 0 )
	{

		new iTemp = floatround( float( iDamage ) * p_spiked[iSkillLevel-1] );
		
		// Give the victim some armor...
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			new CsArmorType:ArmorType;
			new iCurArmor = cs_get_user_armor( iVictim, ArmorType );
			new iMaxArmor = SHARED_GetMaxArmor( iVictim );
			
			// Then set their armor to be the max
			if ( iCurArmor + iTemp > iMaxArmor )
			{
				cs_set_user_armor( iVictim, iMaxArmor, ArmorType );
			}
			
			// Just give them some bonus armor
			else
			{
				cs_set_user_armor( iVictim, iCurArmor + iTemp, ArmorType );
			}
		}
		
		if ( is_user_alive( iAttacker ) )
		{
			// Damage our attacker!
			WC3_Damage( iAttacker, iVictim, iTemp, CSW_CARAPACE, iHitPlace );

			// Make the user glow!
			SHARED_Glow( iAttacker, ( 3 * iTemp ), 0, 0, 0 );
			
			// Create a screen fade
			Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 255, 0, 0, iTemp );
		}
	}
}