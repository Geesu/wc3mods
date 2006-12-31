/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Human Alliance Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define BLINK_COOLDOWN				2.0
#define BASH_GLOW_INTENSITY			100
#define BASH_HOLD_TIME				0.7

new g_HU_DevotionAura[33];			// This will hold the amount of health the user was given as a result of devotion aura

HU_ULT_Blink( id )
{
	
	// User can't Blink when he/she's stunned
	if ( p_data_b[id][PB_STUNNED] )
	{
		WC3_StatusText( id, 0, "You can't blink when you're stunned!" );

		return;
	}
	
	new vOldLocation[3], vNewLocation[3]
	
	// Get the player's current location
	get_user_origin( id, vOldLocation );
	
	// Get where the player is looking (where the player will teleport)
	get_user_origin( id, vNewLocation, 3 );
	
	// Play the blink sound!
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_BLINK], 1.0, ATTN_NORM, 0, PITCH_NORM );

	// Make sure a nearby enemy doesn't have immunity
	if ( WC3_IsImmunePlayerNear( id, vOldLocation ) || WC3_IsImmunePlayerNear( id, vNewLocation ) )
	{
		//set_hudmessage( 255, 255, 10, -1.0, -0.4, 1, 0.5, BLINK_COOLDOWN, 0.2, 0.2 ,-1 );
		WC3_StatusText( id, 0, "%L", id, "TELEPORT_FAILED_ENEMY_IMMUNITY" );
		
		// Reset the user's ultimate
		ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) );

		return;
	}

	// Check to see if the user is teleporting where they shouldn't
	if ( SHARED_NearObjective( vNewLocation ) == OBJENT_VIP_ESCAPE )
	{
		WC3_StatusText( id, 0, "Blink failed, you cannot teleport so close to the escape zone!" );
		
		// Reset the user's ultimate
		ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) );

		return;
	}

	// If we teleport them back, make sure they don't get teleported into the ground
	vOldLocation[ZPOS] += 15;

	// Change coordinates to make sure player won't get stuck in the ground/wall
	vNewLocation[XPOS] += ( ( vNewLocation[XPOS] - vOldLocation[XPOS] > 0 ) ? -50 : 50 );
	vNewLocation[YPOS] += ( ( vNewLocation[YPOS] - vOldLocation[YPOS] > 0 ) ? -50 : 50 );
	vNewLocation[ZPOS] += 40;			

	
	// Set up some origins for some special effects!!!
	new vCenterOrigin[3], vAxisOrigin[3];
	vCenterOrigin[0]	= vOldLocation[0];
	vCenterOrigin[1]	= vOldLocation[1];
	vCenterOrigin[2]	= vOldLocation[2] + 10;
	vAxisOrigin[0]		= vOldLocation[0];
	vAxisOrigin[1]		= vOldLocation[1];
	vAxisOrigin[2]		= vOldLocation[2] + 10 + TELEPORT_RADIUS;

	// Lets create some beam cylinders!
	Create_TE_BEAMCYLINDER( vOldLocation, vCenterOrigin, vAxisOrigin, g_iSprites[SPR_SHOCKWAVE], 0, 0, 3, 60, 0, 255, 255, 255, 255, 0 );
	
	// Modify our effects a bit for another cylinder
	vCenterOrigin[2]	+= 80;
	vAxisOrigin[2]		+= 80;
	
	// And draw another cylinder!!!
	Create_TE_BEAMCYLINDER( vOldLocation, vCenterOrigin, vAxisOrigin, g_iSprites[SPR_SHOCKWAVE], 0, 0, 3, 60, 0, 255, 255, 255, 255, 0 );

	// Planting the bomb then teleporting = bad, lets stop this...
	client_cmd( id, "-use" );

	// Then lets try to slay some bitches...
	if ( get_pcvar_num( CVAR_wc3_blink_protection ) )
	{

		// Lets go ahead and make this check before they teleport...
		if ( HU_ULT_BlinkProtection( id, vNewLocation ) )
		{

			ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) );

			return;
		}
	}

	// Teleport the player!!!
	set_user_origin( id, vNewLocation );

	// Check if Blink landed you in a wall, if so, abort
	new parm[5];
	parm[0] = id;
	parm[1] = vOldLocation[0];
	parm[2] = vOldLocation[1];
	parm[3] = vOldLocation[2];
	parm[4] = vNewLocation[2];
	
	set_task( 0.1, "_HU_ULT_BlinkStuck", TASK_BLINKSTUCK + id, parm, 5 );
	
	ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) );

	ULT_Icon( id, ICON_HIDE );

	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_BLINK], 1.0, ATTN_NORM, 0, PITCH_NORM );

	return;
}

// Function will check to see if a user is stuck in a wall
public _HU_ULT_BlinkStuck( parm[] )
{

	new id = parm[0]	 

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	new vOldLocation[3], vOrigin[3];

	vOldLocation[0] = parm[1];
	vOldLocation[1] = parm[2];
	vOldLocation[2] = parm[3];

	get_user_origin( id, vOrigin );
	
	// Then the user is stuck :/
	if ( parm[4] == vOrigin[2] )
	{

		//set_hudmessage( 255, 255, 10, -1.0, -0.4, 1, 0.5, BLINK_COOLDOWN, 0.2, 0.2, 5 );
		WC3_StatusText( id, 0, "%L", id, "TELEPORT_FAILED_BAD_DESTINATION" );

		set_user_origin( id, vOldLocation )	;

		ULT_ResetCooldown( id, floatround(BLINK_COOLDOWN) );
	}

	// Otherwise they teleported correctly!
	else
	{

		// Sprays white bubbles everywhere
		new vStartOrigin[3];
		vStartOrigin[0] = vOrigin[0];
		vStartOrigin[1] = vOrigin[1];
		vStartOrigin[2] = vOrigin[2] + 40;
		
		Create_TE_SPRITETRAIL( vStartOrigin, vOrigin, g_iSprites[SPR_FLARE], 30, 10, 1, 50, 10 );

		// Flash the player
		if ( get_pcvar_num( CVAR_wc3_blink_dizziness ) == 1 )
		{
			Create_ScreenFade( id, (1<<15), (1<<10), (1<<12), 0, 0, 255, 180 );
		}

		// Lets just flash them with a nice white screen :)
		else
		{
			Create_ScreenFade( id, (1<<15), (1<<10), (1<<12), 255, 255, 255, 255 );
		}
	}	
	
	return;
}


// Function will make sure the user isn't in an invalid location in a map
HU_ULT_BlinkProtection( id, vOrigin[3] )
{

	new bool:bSlay = false;
	new Float:fOrigin[3];
	
	// Convert origin to float
	IVecFVec( vOrigin, fOrigin );
	
	// User shouldn't be teleporting into the sky!
	if ( point_contents( fOrigin ) == CONTENTS_SKY )
	{
		bSlay = true;
	}

	// Get the current map name
	new szMapName[32];
	get_mapname( szMapName, 32 );

	
	// Only do these checks if we're in CS/CZ
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		if ( !bSlay )
		{
			new x = vOrigin[0];
			new y = vOrigin[1];
			new z = vOrigin[2];
			
			// Don't teleport too high
			if ( equali( szMapName, "de_dust" ) )
			{
				if ( z > 220 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "awp_assault" ) )
			{
				if( z > 520 && y > 2400 && y < 2600 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "de_dust_cz" ) )
			{
				if ( z > 220 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "de_aztec_cz" ) )
			{
				if ( z > 300 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "cs_assault_upc" ) )
			{
				if( z > 650 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "de_aztec" ) )
			{
				if( z > 300 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "de_cbble" ) || equali( szMapName, "de_cbble_cz" ) )
			{
				if ( z > 315 )
				{
					if ( ! ( ( x > -1320 && x < -1150 ) && ( y > 2600 && y < 2900 ) ) )
					{
						bSlay = true;
					}
				}           
			}

			else if ( equali( szMapName, "cs_assault" ) )
			{
				if ( z > 700 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "cs_militia" ) || equali( szMapName, "cs_militia_cz" ) )
			{
				if ( z > 500 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "cs_italy" ) )
			{
				if ( z > -220 && y < -2128 )
				{
						bSlay = true;
				}
				else if( z > 250 )
				{
					if ( ( x < -1000 && x > -1648 ) && ( y > 1900 && y < 2050 ) )
					{
						bSlay = true;
					}
					else if ( ( x < -1552 && x > -1648 ) && ( y > 1520 && y < 2050 ) )
					{
						bSlay = true;
					}
				}
			}

			else if ( equali( szMapName, "cs_italy_cz" ) )
			{
				if ( y > 2608 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "de_dust2" ) )
			{
				if ( z > 270 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "de_dust2_cz" ) )
			{
				if ( z > 270 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "fy_dustworld" ) )
			{
				if ( z > 82 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "fy_pool_day" ) )
			{
				if ( z > 190 )
				{
					bSlay = true;
				}
			}

			else if ( equali( szMapName, "as_oilrig" ) )
			{
				if ( x > 1700 )
				{
					bSlay = true;
				}
			}
		}
	}

	
	// For some reason bSlay is always true on this map, but you should be able to teleport anywhere
	if ( equali( szMapName, "cs_mice_final" ) )
	{
		bSlay = false;
	}


	// Slay the user!!!
	if ( bSlay )
	{
		client_print( id, print_chat, "%s %L", g_MODclient, id, "SLAIN_FOR_TELEPORTING" );
		user_kill( id );
	}

	return bSlay;
}

HU_DevotionAura( id )
{
	new iBonusMultiplier = SM_GetSkillLevel( id, SKILL_DEVOTION );
	
	// Then the user has devotion aura!
	if ( iBonusMultiplier > 0 )
	{
		new iNewHealth = 100 + ( iBonusMultiplier * p_devotion );

		set_user_health( id, iNewHealth );
		
		// Lets remember that we've set this!
		g_HU_DevotionAura[id] = iNewHealth - 100;
	}

	// The user doesn't have devotion aura - so lets check if they previously has it + lost it - if so we need to remove the health bonus right?
	else
	{
		// Snap they've been given some health - what a shame, we now have to remove it!
		if ( g_HU_DevotionAura[id] > 0 )
		{
			new iCurHealth = get_user_health( id );
			
			// Then give them 1 health, otherwise we'd have to kill them - and that's not nice
			if ( iCurHealth - g_HU_DevotionAura[id] <= 0 )
			{
				set_user_health( id, 1 );
			}

			// Remove bonus
			else
			{
				set_user_health( id, iCurHealth - g_HU_DevotionAura[id] );
			}

			// Lets display a message so they realize why they just lost health
			client_print( id, print_chat, "%s You lost your health bonus from devotion aura!", g_MODclient );
		}
	}
}

HU_SkillsOffensive( iAttacker, iVictim )
{
	static iSkillLevel;

	// Bash
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_BASH );
	if ( iSkillLevel > 0 )
	{

		// Cannot bash if already bashed or user is slowed
		if ( random_float( 0.0, 1.0 ) <= p_bash[iSkillLevel-1] && !SHARED_IsPlayerSlowed( iVictim ) )
		{		

			p_data_b[iVictim][PB_STUNNED] = true;
			SHARED_SetSpeed( iVictim );
			
			set_task( BASH_HOLD_TIME, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + iVictim );

			// Make the user glow!
			SHARED_Glow( iVictim, 0, 0, 0, BASH_GLOW_INTENSITY );
			
			// Create a screen fade
			Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 255, 255, 255, g_GlowLevel[iVictim][3] )
		}
	}
}