/*
*	Race: Undead Scourge Functions
*/

#define EXPLOSION_MAX_DAMAGE	50
#define EXPLOSION_RANGE			300
#define EXPLOSION_BLAST_RADIUS	250


UD_Suicide( id )
{
	
	// Ultimate has been used, so we can't use it again!
	if ( !ULT_Available( id ) )
	{
		return;
	}

	// If we ult while changing teams, you will kill your old teammates, we don't want that!!
	if ( p_data_b[id][PB_CHANGINGTEAM] )
	{
		return;
	}

	// Play the undead explosion sound!!!
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_SUICIDE], 1.0, ATTN_NORM, 0, PITCH_NORM );

	new parm[5], vOrigin[3];
	get_user_origin( id, vOrigin );

	parm[0] = id;
	parm[1] = 6;
	parm[2] = vOrigin[0];
	parm[3] = vOrigin[1];
	parm[4] = vOrigin[2];

	// Set up the tasks to damage + draw effects
	set_task( 0.5, "_UD_SuicideExplode", TASK_EXPLOSION + id, parm, 5 );
	set_task( 0.5, "_UD_SuicideBlastCircles", TASK_BEAMCYLINDER + id, parm, 5 );
	
	// Create an implosion effect where they died
	Create_TE_IMPLOSION( vOrigin, 100, 20, 5 );
	
	// Set up an ultimate delay (in case the user respawns)
	ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_cooldown ) );
}


// This is only use in _UD_SuicideExplode so declaring it here should be fine
new bool:bIgnoreDmg[33] = false;

// Draw the explosions
public _UD_SuicideExplode( parm[5] )
{
	new id = parm[0];

	if ( get_pcvar_num( CVAR_wc3_psychostats ) )
	{
		new WEAPON = CSW_SUICIDE - CSW_WAR3_MIN;

		iStatsShots[id][WEAPON]++;
	}

	new vOrigin[3], vPosition[3];
	vOrigin[0] = parm[2];
	vOrigin[1] = parm[3];
	vOrigin[2] = parm[4];

	vPosition[0] = vOrigin[0] + random_num( -100, 100 );
	vPosition[1] = vOrigin[1] + random_num( -100, 100 );
	vPosition[2] = vOrigin[2] + random_num( -50, 50 );
	
	Create_TE_EXPLOSION( vOrigin, vPosition, g_iSprites[SPR_FIREBALL], (random_num(0,20) + 20), 12, TE_EXPLFLAG_NONE );
	
	// This doesn't look correct in Day of Defeat, so lets only do it for CS/CZ
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		Create_TE_Smoke( vOrigin, vPosition, g_iSprites[SPR_SMOKE], 60, 10 );
	}

	new players[32], numberofplayers;
	get_players( players, numberofplayers, "a" );

	new i, iTargetID, vTargetOrigin[3], iDamage, iDistance;
	new iMultiplier = ( EXPLOSION_MAX_DAMAGE * EXPLOSION_MAX_DAMAGE ) / EXPLOSION_RANGE;
	new iTeam = get_user_team( id );
	
	// Check all players and see if they should be damaged
	for ( i = 0; i < numberofplayers; i++ )
	{
		iTargetID = players[i];
		
		// Get origin of target
		get_user_origin( iTargetID, vTargetOrigin );

		// Get distance in b/t target and caster
		iDistance = get_distance( vOrigin, vTargetOrigin );
		
		// Make sure this user is close enough to do damage + isn't immune + isn't on the same team + isn't already immune to all damage
		if ( iDistance < EXPLOSION_RANGE && iTeam != get_user_team( iTargetID ) && !bIgnoreDmg[iTargetID] )
		{

			if ( ULT_IsImmune( iTargetID ) )
			{
				ULT_Blocked( id );
				
				bIgnoreDmg[iTargetID] = true;
			}
			
			// The user isn't immune!
			else
			{
			
				// Calculate the damage to be done
				iDamage = ( EXPLOSION_RANGE - iDistance) * iMultiplier;
				iDamage = sqroot( iDamage );
				
				// Damage them!!!!
				WC3_Damage( iTargetID, id, iDamage, CSW_SUICIDE, -1 );
				
				// Lets shake up their screen a bit
				Create_ScreenShake( iTargetID, (1<<14), (1<<13), (1<<14) );
			}
		}

		// Reset the "don't damage" rule
		if ( parm[1] - 1 <= 0 )
		{
			bIgnoreDmg[iTargetID] = false;
		}
	}

	--parm[1];
	
	// Lets keep going if we have some left!
	if ( parm[1] > 0 )
	{
		set_task( 0.1, "_UD_SuicideExplode", TASK_EXPLOSION + id, parm, 5 );
	}
}

// Draw the blast circles
public _UD_SuicideBlastCircles( parm[5] )
{
	new vOrigin[3], vPosition[3];

	vOrigin[0] = parm[2];
	vOrigin[1] = parm[3];
	vOrigin[2] = parm[4] - 16;

	vPosition[0] = vOrigin[0];
	vPosition[1] = vOrigin[1];
	vPosition[2] = vOrigin[2] + EXPLOSION_BLAST_RADIUS;

	Create_TE_BEAMCYLINDER( vOrigin, vOrigin, vPosition, g_iSprites[SPR_SHOCKWAVE], 0, 0, 6, 16, 0, 188, 220, 255, 255, 0 );

	vOrigin[2] = ( vOrigin[2] - EXPLOSION_BLAST_RADIUS ) + ( EXPLOSION_BLAST_RADIUS / 2 );

	Create_TE_BEAMCYLINDER( vOrigin, vOrigin, vPosition, g_iSprites[SPR_SHOCKWAVE], 0, 0, 6, 16, 0, 188, 220, 255, 255, 0 );
}


UD_SkillsOffensive( iAttacker, iDamage )
{
	static iSkillLevel;

	if ( !p_data_b[iAttacker][PB_ISCONNECTED] )
	{
		return;
	}

	// Vampiric Aura
	iSkillLevel = SM_GetSkillLevel( iAttacker, SKILL_VAMPIRICAURA );
	if ( iSkillLevel > 0 )
	{
		static iHealth, iMaxHealth, iBonusHealth;

		iHealth		= get_user_health( iAttacker );
		iMaxHealth	= get_user_maxhealth( iAttacker );
		iBonusHealth = floatround( float( iDamage ) * p_vampiric[iSkillLevel-1] );
		
		// Give the user health!
		if ( iHealth < iMaxHealth )
		{

			// Then give the user his maximum health
			if ( iHealth + iBonusHealth > iMaxHealth )
			{
				set_user_health( iAttacker, iMaxHealth );
			}

			// Otherwise just give iMaxHealth
			else
			{
				set_user_health( iAttacker, iHealth + iBonusHealth );
			}
		}
		
		// Make the attacker glow
		SHARED_Glow( iAttacker, 0, ( 2 * iBonusHealth ), 0, 0 );
		
		// Give the attacker a nice screen fade
		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 0, 255, 0, iBonusHealth );
	}
}
