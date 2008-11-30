
// Forwards from the CSX module and DODX module
public grenade_throw( index, greindex, wId )
{
	
	if ( !WC3_Check() )
	{
		return;
	}
	
	// If user isn't alive do nothing!
	if ( !is_user_alive( index ) )
	{
		return;
	}

	// User has gloves!  Do we need to give them another grenade?
	if ( ITEM_Has( index, ITEM_GLOVES ) > ITEM_NONE )
	{
		ITEM_Glove_Begin( index );
	}

	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( index, SKILL_CRITICALGRENADE );

	// Make sure the user has the skill and we actually have a grenade index
	if ( greindex && iSkillLevel > 0 )
	{
		
		// Then Critical Grenades are allowed
		if ( OR_CriticalGrenadeAllowed( index ) )
		{

			// Then draw it!
			if ( SHARED_IsGrenade( wId ) )
			{
				new iWidth = 3 * iSkillLevel;

				Create_TE_BEAMFOLLOW( greindex, g_iSprites[SPR_TRAIL], 20, iWidth, 255, 32, 32, 196 );
			}
		}
	}
	return;
}

// HamSandwich implementation
public EVENT_TakeDamage( iVictim, inflictor, iAttacker, Float:f_Damage, damagetype )
{

	static s_Classname[ 8 ];
	pev ( inflictor, pev_classname, s_Classname, charsmax ( s_Classname ) );


	new s_PlayerName[32], s_PlayerName2[32];

	get_user_name( iVictim, s_PlayerName, 31 );
	get_user_name( iAttacker, s_PlayerName2, 31 );

	//server_print( "%s:%d attack's %s:%d for %0.0f damage with %s:%d", s_PlayerName2, iAttacker, s_PlayerName, iVictim, f_Damage, s_Classname, inflictor );

      
/*	client_print( iAttacker, print_chat, "Damage: %f0.0, Victim: %d, Inflictor: %d (%s)", iDamage, iVictim, inflictor, s_Classname );
	client_print( iVictim, print_chat, "Damage: %f0.0, iAttacker: %d, Inflictor: %d (%s)", iDamage, iAttacker, inflictor, s_Classname );


	server_print( "Attacker: %d, Damage: %f0.0, Victim: %d, Inflictor: %d (%s)", iAttacker, iDamage, iVictim, inflictor, s_Classname );
*/
	//return HAM_SUPERCEDE


	return HAM_IGNORED;
}

public client_damage( iAttacker, iVictim, iDamage, iWeapon, iHitPlace, TA )
{

	if ( !WC3_Check() )
	{
		return;
	}

	// If they damage themself we don't care now do we ?
	if ( iVictim == iAttacker )
	{
		return;
	}
	
	g_iDamageDealt[iAttacker][iVictim] += iDamage;

	// Bot should "auto" cast his/her ultimate on random
	if ( SHARED_ValidPlayer( iAttacker ) && is_user_bot( iAttacker ) && random_num( 0, 100 ) <= ( BOT_CAST_ULT_CHANCE * 100 ) )
	{
		// Check for an actual ultimate is done in this function, why do it twice?
		cmd_Ultimate( iAttacker );
	}
	

	// We need to make sure that we have a valid attacker and the user isn't hexed
	if ( SHARED_ValidPlayer( iAttacker ) && !p_data_b[iAttacker][PB_HEXED] )
	{

		// Run the offensive spells
		UD_SkillsOffensive( iAttacker, iDamage );
		HU_SkillsOffensive( iAttacker, iVictim );
		OR_SkillsOffensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		NE_SkillsOffensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		BM_SkillsOffensive( iAttacker, iVictim, iDamage );
		SH_SkillsOffensive( iAttacker, iVictim );
		WA_SkillsOffensive( iAttacker, iVictim, iHitPlace );
		CL_SkillsOffensive( iAttacker, iVictim, iHitPlace );
	}

	// Make sure we can run the defensive skills
	if ( SHARED_ValidPlayer( iAttacker ) && !p_data_b[iVictim][PB_HEXED] )
	{
		//UD_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		//HU_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		//OR_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		NE_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace );
		BM_SkillsDefensive( iAttacker, iVictim, iDamage );
		SH_SkillsDefensive( iAttacker, iVictim );
		//WA_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
		CL_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace );
	}

	// Item abilities
	if ( SHARED_ValidPlayer( iAttacker ) )
	{
		ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
	}

	return;
}

// All we want to check is if the user was killed by the bomb
public client_death( iAttacker, iVictim, iWeapon, hitplace, TK )
{
	// Counter-Strike and Condition Zero Checks
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		// Check out who the inflictor was
		new iInflictor = entity_get_edict( iVictim, EV_ENT_dmg_inflictor );

		// Check to see if the death was from the bomb
		if ( !SHARED_ValidPlayer( iInflictor ) && iWeapon != CSW_HEGRENADE && iInflictor )
		{
			
			if ( is_valid_ent ( iInflictor ) )
			{
				new szClassName[64];
				entity_get_string( iInflictor, EV_SZ_classname, szClassName, 63 );

				// Check the classname of our inflictor
				if ( equali( szClassName, "grenade" ) || equali( szClassName, "env_explosion" ) )
				{
					iWeapon = CSW_C4;
					iAttacker = 0;

					// Well if this isn't set, shouldn't it be?
					if ( !p_data_b[iVictim][PB_DIEDLASTROUND] )
					{
						on_Death( iVictim, iAttacker, iWeapon, 0 );
					}
					//client_print( iVictim, print_chat, "[DEBUG] You were just killed by the bomb (%s) Alive? %d", szClassName, is_user_alive( iVictim ) );
				}
			}
		}
	}
}

public on_Death( iVictim, iAttacker, iWeaponID, iHeadshot )
{

	if ( !WC3_Check() )
	{
		return;
	}
	
	// Counter-Strike and Condition Zero Checks
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// For some reason the damage passed by explosions is not actually correct (perhaps armor adjustments weren't done yet), so lets check
		if ( is_user_alive( iVictim ) && iWeaponID == CSW_C4 )
		{
			return;
		}

		// Check for NightVision
		p_data_b[iVictim][PB_NIGHTVISION]	= (cs_get_user_nvg( iVictim )) ? true : false;

		// Check if a user had a shield on death
		p_data_b[iVictim][PB_SHIELD]		= (cs_get_user_shield( iVictim )) ? true : false;

		// See if the user has a defuse kit
		p_data_b[iVictim][PB_DEFUSE]		= (cs_get_user_defuse( iVictim )) ? true : false;

		// Save the number of flash grenades the user has
		p_data[iVictim][P_FLASHCOUNT]		= cs_get_user_bpammo( iVictim, CSW_FLASHBANG );
		
		// Check to see if weapons were silenced on death
		new iWeaponUSPEnt = find_ent_by_owner( -1, "weapon_usp", iVictim );
		new iWeaponM4A1Ent = find_ent_by_owner( -1, "weapon_m4a1", iVictim );
		
		if ( is_valid_ent( iWeaponUSPEnt ) )
		{
			p_data_b[iVictim][PB_USP_SILENCED]	= cs_get_weapon_silen( iWeaponUSPEnt ) ? true : false;
		}

		if ( is_valid_ent( iWeaponM4A1Ent ) )
		{
			p_data_b[iVictim][PB_M4A1_SILENCED]	= cs_get_weapon_silen( iWeaponM4A1Ent ) ? true : false;
		}


		// Check to see if weapons were in burst mode on death
		new iWeaponGlock18Ent = find_ent_by_owner( -1, "weapon_glock18", iVictim );
		new iWeaponFamasEnt = find_ent_by_owner( -1, "weapon_famas", iVictim );

		if ( is_valid_ent( iWeaponGlock18Ent ) )
		{
			p_data_b[iVictim][PB_GLOCK_BURST]	= cs_get_weapon_burst( iWeaponGlock18Ent ) ? true : false;
		}

		if ( is_valid_ent( iWeaponFamasEnt ) )
		{
			p_data_b[iVictim][PB_FAMAS_BURST]	= cs_get_weapon_burst( iWeaponFamasEnt ) ? true : false;
		}
	}
	
	// Day of Defeat Checks
	else if ( g_MOD == GAME_DOD )
	{

		// Save the user's origin for reincarnation
		get_user_origin( iVictim, iReincarnation[iVictim] );
	}

	WC3_Death( iVictim, iAttacker, iWeaponID, iHeadshot );
	
	return;
}

// Hook for CS/CZ
public on_DeathMsg()
{

	if ( !WC3_Check() )
	{
		return;
	}

	new iAttacker	= read_data( 1 );
	new iVictim		= read_data( 2 );
	new iHeadshot	= read_data( 3 );
	new iWeaponID;
	get_user_attacker( iVictim, iWeaponID );

	on_Death( iVictim, iAttacker, iWeaponID, iHeadshot );
	
	return;
}

new g_iLastCurWeapon[33];
public on_CurWeapon( id )
{
	// read_data(1) = isActive?
	// read_data(2) = weapon index
	// read_data(3) = ammo

	if ( !WC3_Check() )
	{
		return;
	}

	new iCurWeapon = read_data( 2 );

	// Then a weapon strip was triggered - we don't want to save now do we?
	if ( iCurWeapon == 0 && read_data( 1 ) == 0 && read_data( 3 ) == 0 )
	{
		return;
	}

	// Record the last time a shot was fired
	fLastShotFired[id] = halflife_time();
	
	// Save the user's weapons
	SHARED_SaveWeapons( id );

	// Set the fuse for the weapon in DOD
	if ( g_MOD == GAME_DOD )
	{

		// Critical Grenade
		if ( SM_GetSkillLevel( id, SKILL_CRITICALGRENADE ) > 0 && SHARED_HasGrenade( id ) )
		{
			dod_set_fuse( id, FUSE_SET, 2.0, FT_NEW );
		}
	}
	
	// We only need to run these functions if the user's weapon has changed since our last function call!
	if ( g_iLastCurWeapon[id] != iCurWeapon && SHARED_ValidPlayer( id ) )
	{
		// Check to see if we should set the player's invisibility
		SHARED_INVIS_Set( id );
		
		// Set the user's speed
		SHARED_SetSpeed( id );
	}

	g_iLastCurWeapon[id] = iCurWeapon;

	return;
}

public on_Drop( id )
{
	if ( !WC3_Check() )
	{
		return;
	}

	SHARED_SaveWeapons( id );

	return;
}

// HamSandwich implementation
public EVENT_Spawn( id )
{

	if ( !WC3_Check() )
	{
		return HAM_HANDLED;
	}

	// ResetHUD can be called when the user is not alive, lets ignore those calls
	if ( !is_user_alive( id ) )
	{
		return HAM_HANDLED;
	}

	// Store the player's team!
	g_iPlayerTeam[id] = get_user_team( id );

	// Prespawn call
	//   - Purpose is to have a spawn call that happens before everything else!
	WC3_PreSpawn( id );
	
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		
		// This is the first time the user has spawned this round
		if ( !p_data_b[id][PB_HAS_SPAWNED] )
		{	
			EVENT_PlayerInitialSpawn( id );

			p_data_b[id][PB_HAS_SPAWNED] = true;
		}
	}
	
	// Start a new session under the following conditions:
	//		- Day of Defeat - need a new session per spawn!
	//		- CSDM - rounds never end!!!
	if ( g_MOD == GAME_DOD || ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 ) )
	{
		WC3_NewSession( id );
	}

	// Should be called at the end of each spawn
	WC3_PostSpawn( id );

	return HAM_HANDLED;
}

// Function called EVERYTIME a user spawns!
public on_ResetHud( id )
{

	if ( !WC3_Check() )
	{
		return PLUGIN_CONTINUE;
	}

	// ResetHUD can be called when the user is not alive, lets ignore those calls
	if ( !is_user_alive( id ) )
	{
		return PLUGIN_CONTINUE;
	}

	// Store the player's team!
	g_iPlayerTeam[id] = get_user_team( id );

	// We're forcibly respawning the player - so lets just return
	if ( bIgnorePlayerSpawning[id] )
	{
		return PLUGIN_CONTINUE;
	}

	// Prespawn call
	//   - Purpose is to have a spawn call that happens before everything else!
	WC3_PreSpawn( id );

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		
		// This is the first time the user has spawned this round
		if ( !p_data_b[id][PB_HAS_SPAWNED] )
		{	
			EVENT_PlayerInitialSpawn( id );

			p_data_b[id][PB_HAS_SPAWNED] = true;
		}
	}
	
	// Start a new session under the following conditions:
	//		- Day of Defeat - need a new session per spawn!
	//		- CSDM - rounds never end!!!
	if ( g_MOD == GAME_DOD || ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 ) )
	{
		WC3_NewSession( id );
	}

	// Should be called at the end of each spawn
	WC3_PostSpawn( id );

	return PLUGIN_CONTINUE;
}

// Function is called when the user is spawned at the START of each round - only WC3_PreSpawn is called first
//		TRIGGERED BY: ResetHUD
public EVENT_PlayerInitialSpawn( id )
{
	

	// Display the cheat message ONLY if they just joined!
	if ( p_data_b[id][PB_JUSTJOINED] && get_pcvar_num( CVAR_wc3_cheats ) )
	{
		client_print( id, print_chat, "%s WC3 Cheats are enabled on this server! Try typing /level10", g_MODclient );

		p_data_b[id][PB_JUSTJOINED]			= false;
	}

	// Display the new Chameleon skills for the round
	if ( p_data[id][P_RACE] == 9 && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		WC3_ShowRaceInfo( id );
	}

	WC3_NewSession( id );

	return;
}

// Function is called ONCE at the start of a new round BEFORE user's spawn
public EVENT_NewRound()
{
	if ( !WC3_Check() )
	{
		return;
	}

	// Reset the global ultimate delay
	g_iUltimateDelay = get_pcvar_num( CVAR_wc3_ult_delay );

	// User's have not spawned yet, so lets do some pre-spawn things
	new players[32], numplayers, i;
	get_players( players, numplayers );
	for ( i = 0; i < numplayers; i++ )
	{
		WC3_BeforeSpawn( players[i] );
	}
	
	// Reset which spawn points are reserved....
	_SHARED_SpawnReset();

	// If someone restarted the game, then lets reset war3
	if ( g_GameRestarting )
	{
		WC3_ResetGame();
	}
	
	// Randomize Chameleon if we need to
	CHAM_Randomize();

	// We need to determine when the buytime is up
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( get_pcvar_num( CVAR_wc3_buy_time ) )
		{
			// Remove our buytime task if it exists (from a previous round)
			( task_exists( TASK_BUYTIME ) ) ? remove_task( TASK_BUYTIME ) : 0;

			g_buyTime = true;

			set_task( get_cvar_float("mp_buytime") * 60.0, "_CS_BuyTimeOver", TASK_BUYTIME );
		}

		g_freezeTime	= true;
	}

	g_EndRound = false;
}

// Called when a user looks somewhere
public TRIGGER_TraceLine( Float:v1[3], Float:v2[3], noMonsters, pentToSkip )
{
	if ( !WC3_Check() )
	{
		return FMRES_IGNORED;
	}

	new iAttacker = pentToSkip;
	new iVictim = get_tr(TR_pHit);
	new iHitZone = (1 << get_tr(TR_iHitgroup));
	
	// Make sure we have a valid victim
	if ( SHARED_ValidPlayer( iVictim ) && p_data_b[iVictim][PB_ISCONNECTED] )
	{
		// We need to have a valid player!
		if ( SHARED_ValidPlayer( iAttacker ) )
		{
			// This is a check for ultimates that need to "search" for a target
			if ( p_data_b[iAttacker][PB_ISSEARCHING] )
			{

				// Now lets make sure the person he's looking at is in view and isn't on the same team
				if ( get_user_team( iAttacker ) != get_user_team( iVictim ) ) //&& UTIL_EntInView( iAttacker, iVictim ) )
				{
					
					// Check to see if the user should block this ultimate!
					if ( !g_EndRound && ULT_CanUserBlockUlt( iVictim ) )
					{
						ULT_RemoveCharge( iVictim, 0 );
						ULT_Blocked( iAttacker );
					}

					// Then the user's ult should work!
					else
					{
						// Well we do have a target so lets execute the user's ultimate!!
						if ( SM_GetSkillLevel( iAttacker, ULTIMATE_CHAINLIGHTNING ) > 0 )
						{
							OR_ULT_ChainLightning( iAttacker, iVictim, iHitZone );
						}
						else if ( SM_GetSkillLevel( iAttacker, ULTIMATE_ENTANGLE ) > 0 )
						{
							NE_ULT_Entangle( iAttacker, iVictim );
						}
						else if ( SM_GetSkillLevel( iAttacker, ULTIMATE_IMMOLATE ) > 0 )
						{
							BM_ULT_Immolate( iAttacker, iVictim );
						}
					}

					// No longer searching since we found a target
					p_data_b[iAttacker][PB_ISSEARCHING]	= false;

					// Set up the user's ultimate delay
					ULT_ResetCooldown( iAttacker, get_pcvar_num( CVAR_wc3_ult_cooldown ) );
				}
			}

			// This is a nice check for Helm of Excellence
			if ( ITEM_Has( iVictim, ITEM_HELM ) > ITEM_NONE )
			{
				// Friendly fire is off! - This means same team shouldn't remove a charge!
				if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
				{
					if ( g_iPlayerTeam[iAttacker] == g_iPlayerTeam[iVictim] )
					{
						return FMRES_IGNORED;
					}
				}

				// If its a headshot then we want to block it
				if ( iHitZone & (1 << 1) )
				{
					set_tr( TR_flFraction, 1.0 );
					
					// Make sure we have a valid attacker!
					if ( SHARED_ValidPlayer( iAttacker ) )
					{
						// Do the check to see if we should flash the screen orange
						new Float:fTime = halflife_time();
						new Float:fDifference = fTime - fLastShotFired[iAttacker];

						if ( fDifference < 0.1 && fDifference > 0.0 )
						{
							Create_ScreenFade( iVictim, (1<<10), (1<<10), (1<<12), 0, 0, 255, 150 );

							// Lets remove a charge from the helm!
							ITEM_RemoveCharge( iVictim, ITEM_HELM );
						}
					}
					
					return FMRES_SUPERCEDE;
				}
			}

			// Check to see if this user has night elf's evasion
			if ( SM_GetSkillLevel( iVictim, SKILL_EVASION ) > 0 )
			{
				// Do the check to see if we should "evade" this shot
				new Float:fTime = halflife_time();
				new Float:fDifference = fTime - fLastShotFired[iAttacker];

				if ( SHARED_ValidPlayer( iAttacker ) && fDifference < 0.1 && fDifference > 0.0 )
				{

					// Friendly fire is off! - This means we shouldn't evade since no damage will be done!
					if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
					{
						if ( g_iPlayerTeam[iAttacker] == g_iPlayerTeam[iVictim] )
						{
							return FMRES_IGNORED;
						}
					}
					
					// Then we should evade this shot!
					if ( NE_Evasion( iVictim, iHitZone ) )
					{
						set_tr( TR_flFraction, 1.0 );
						
						WC3_StatusText( iVictim, TXT_SKILL, "You have evaded a shot!" );

						return FMRES_SUPERCEDE;
					}
				}
			}
			
			// Mole protectant
			if ( p_data_b[iAttacker][PB_MOLE] && ITEM_Has( iVictim, ITEM_PROTECTANT ) > ITEM_NONE )
			{	
				new Float:fTime = halflife_time();

				if ( fTime - fLastShotFired[iAttacker] < 0.1  )
				{
					client_print( iVictim, print_chat, "%s %L", g_MODclient, iVictim, "SHOT_DEFLECTED" );

					set_tr( TR_flFraction, 1.0 );

					return FMRES_SUPERCEDE;
				}
			}

			// Check for Big Bad Voodoo's ultimate!
			if ( p_data_b[iVictim][PB_GODMODE] )
			{
				
				new bool:bBlockDamage = true;

				// Do we allow this person to be attacked by this player?
				if ( p_data_b[iAttacker][PB_BIGBAD_ATTACKER] )
				{
					bBlockDamage = false;
				}

				// Check to see if immunity is available for the attacker
				else if ( ULT_CanUserBlockUlt( iAttacker ) )
				{
					// Remove charge and display message to attacker
					ULT_RemoveCharge( iAttacker, 1 );

					// Display message about user's ultimate being blocked!
					ULT_Blocked( iVictim );

					// This user can attack someone with big bad voodoo!
					p_data_b[iAttacker][PB_BIGBAD_ATTACKER] = true;

					// Reset the attacker dmg
					set_task( get_pcvar_float( CVAR_wc3_ult_cooldown ), "_SH_ResetBigBadAttacker", TASK_BIGBADATTACKER + iAttacker );
				}

				// Block the damage!
				if ( bBlockDamage )
				{
					set_tr( TR_flFraction, 1.0 );

					return FMRES_SUPERCEDE;
				}
			}
		}
	}
	
	return FMRES_IGNORED;
}
