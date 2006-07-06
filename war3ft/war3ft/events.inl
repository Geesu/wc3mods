// Forwards from the CSX module and DODX module
public grenade_throw( index, greindex, wId )
{

	if ( !WC3_Check() )
	{
		return;
	}

	if ( ITEM_Has( index, ITEM_GLOVES ) )
	{
		new Float:fTimer = get_pcvar_float( CVAR_wc3_glove_timer );

		set_task( fTimer, "_ITEM_Glove_Give", TASK_ITEM_GLOVES + index );
	}

	// Make sure the user has the skill and we actually have a grenade index
	if ( greindex && SM_VerifySkill( index, SKILL_CRITICALGRENADE ) )
	{
		
		// Then Critical Grenades are allowed
		if ( OR_CriticalGrenadeAllowed( index ) )
		{

			// Then draw it!
			if ( SHARED_IsGrenade( wId ) )
			{
				new iWidth = 3 *  p_data[index][SKILL2];

				Create_TE_BEAMFOLLOW( greindex, g_iSprites[SPR_TRAIL], 20, iWidth, 255, 32, 32, 196 );
			}
		}
	}
	return;
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
	
	// Counter-Strike and Condition Zero check only - Bomb Explosion

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		if ( iWeapon == CSW_C4 )
		{
			client_print( iVictim, print_chat, "[DEBUG] Attacked by bomb for %d damage", iDamage );
		}

		// Check out who the inflictor was
		new iInflictor = entity_get_edict( iVictim, EV_ENT_dmg_inflictor );

		// Check to see if the iDamage was from the bomb
		if ( !SHARED_ValidPlayer( iInflictor ) && iWeapon != CSW_HEGRENADE && iAttacker != iVictim && iInflictor )
		{
			
			if ( is_valid_ent ( iInflictor ) )
			{
				new szClassName[64];
				entity_get_string( iInflictor, EV_SZ_classname, szClassName, 63 );
					
				// Technically I don't think we need to check the classname, but just in case
				if ( equali( szClassName, "grenade" ) || equali( szClassName, "env_explosion" ) )
				{
					iWeapon = CSW_C4;
					iAttacker = 0;

					// We need to call the death function manually b/c DeathMsg isn't broadcasted when the bomb explodes and kills someone
					if ( get_user_health( iVictim ) - iDamage < 0 )
					{
						on_Death( iVictim, iAttacker, iWeapon, 0 );
					}

					client_print( iVictim, print_chat, "[DEBUG] You were just attacked by the bomb for %d damage (%s) Alive? %d", iDamage, szClassName, is_user_alive( iVictim ) );
				}
			}
		}
	}


	// Bot should "auto" cast his/her ultimate on random
	if ( SHARED_ValidPlayer( iAttacker) && is_user_bot( iAttacker ) && p_data[iAttacker][P_ULTIMATE] )
	{
		cmd_Ultimate( iAttacker );
	}
	

	// We need to make sure that we have a valid attacker and the user isn't hexed
	if ( SHARED_ValidPlayer( iAttacker ) && !p_data_b[iAttacker][PB_HEXED] )
	{

		// Run the offensive spells
		SM_VerifyRace( iAttacker, RACE_UNDEAD )	? UD_SkillsOffensive( iAttacker, iDamage ) : 0;
		SM_VerifyRace( iAttacker, RACE_HUMAN )	? HU_SkillsOffensive( iAttacker, iVictim ) : 0;
		SM_VerifyRace( iAttacker, RACE_ORC )		? OR_SkillsOffensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace ) : 0;
		SM_VerifyRace( iAttacker, RACE_ELF )		? NE_SkillsOffensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace ) : 0;
		SM_VerifyRace( iAttacker, RACE_BLOOD )	? BM_SkillsOffensive( iAttacker, iVictim, iDamage ) : 0;
		SM_VerifyRace( iAttacker, RACE_SHADOW )	? SH_SkillsOffensive( iAttacker, iVictim ) : 0;
		SM_VerifyRace( iAttacker, RACE_WARDEN )	? WA_SkillsOffensive( iAttacker, iVictim, iHitPlace ) : 0;
		SM_VerifyRace( iAttacker, RACE_CRYPT )	? CL_SkillsOffensive( iAttacker, iVictim, iHitPlace ) : 0;
	}

	// Make sure we can run the defensive skills
	if ( SHARED_ValidPlayer( iAttacker ) && !p_data_b[iVictim][PB_HEXED] )
	{
		//SM_VerifyRace( iVictim, RACE_UNDEAD )		? UD_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace ) : 0;
		//SM_VerifyRace( iVictim, RACE_HUMAN )		? HU_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace ) : 0;
		//SM_VerifyRace( iVictim, RACE_ORC )		? OR_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace ) : 0;
		SM_VerifyRace( iVictim, RACE_ELF )		? NE_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace ) : 0;
		SM_VerifyRace( iVictim, RACE_BLOOD )		? BM_SkillsDefensive( iAttacker, iVictim, iDamage ) : 0;
		SM_VerifyRace( iVictim, RACE_SHADOW )		? SH_SkillsDefensive( iAttacker, iVictim ) : 0;
		//SM_VerifyRace( iVictim, RACE_WARDEN )		? WA_SkillsDefensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace ) : 0;
		SM_VerifyRace( iVictim, RACE_CRYPT )		? CL_SkillsDefensive( iAttacker, iVictim, iDamage, iHitPlace ) : 0;
	}

	// Item abilities
	if ( SHARED_ValidPlayer( iAttacker ) )
	{
		ITEM_Offensive( iAttacker, iVictim, iWeapon, iDamage, iHitPlace );
	}

	return;
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

public on_CurWeapon( id )
{

	// read_data(1) = isActive?
	// read_data(2) = weapon index
	// read_data(3) = ammo

	if ( !WC3_Check() )
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
		if ( SM_VerifySkill( id, SKILL_CRITICALGRENADE ) && SHARED_HasGrenade( id ) )
		{
			dod_set_fuse( id, FUSE_SET, 2.0, FT_NEW );
		}
	}

	// Check to see if we should set the player's invisibility
	SHARED_INVIS_Set( id );

	// Set the user's speed
	SHARED_SetSpeed( id );

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

public on_ResetHud(id)
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

	// This is the first time the user has spawned this round
	if ( !p_data_b[id][PB_HAS_SPAWNED] )
	{	
		EVENT_PlayerInitialSpawn( id );

		p_data_b[id][PB_HAS_SPAWNED] = true;
	}

	EVENT_PlayerSpawned( id );

	return PLUGIN_CONTINUE;
}

// Function is called when the user is spawned at the START of each round (called before EVENT_PlayerSpawned)
public EVENT_PlayerInitialSpawn( id )
{
	
	// Display the cheat message ONLY if they just joined!
	if ( p_data_b[id][PB_JUSTJOINED] && get_pcvar_num( CVAR_wc3_cheats ) )
	{
		client_print( id, print_chat, "%s WC3 Cheats are enabled on this server! Try typing /level10", g_MODclient );

		p_data_b[id][PB_JUSTJOINED]			= false;
	}

	// Bot options
	if ( is_user_bot(id) )
	{
		// Give the bot some random XP if we're saving XP
		if ( get_pcvar_num( CVAR_wc3_save_xp ) && !p_data[id][P_XP] )
		{
			p_data[id][P_XP] = xplevel[floatround(random_float(0.0,3.16)*random_float(0.0,3.16))];
		}

		if ( !p_data[id][P_RACE] )
		{
			p_data[id][P_RACE] = random_num(1, get_pcvar_num( CVAR_wc3_races ));
		}
	}

	// Display the new Chameleon skills for the round
	if ( p_data[id][P_RACE] == 9 && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		WC3_ShowRaceInfo( id );
	}

	WC3_CommonSpawn( id );

	return;
}

// Function is called everytime a user spawns (called after EVENT_PlayerInitialSpawn)
public EVENT_PlayerSpawned( id )
{
	// Find out if they need to choose a race or select a skill
	set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + id );

	// User isn't changing a team if they just spawned
	p_data_b[id][PB_CHANGINGTEAM]	= false;
	
	// Reset suicide attempt
	p_data_b[id][PB_SUICIDEATTEMPT] = false;
	
	// User should not be burning
	p_data_b[id][PB_ISBURNING]		= false;

	// The user should not be frozen when they spawn
	SHARED_ResetMaxSpeed( id );

	// Reset the user's skin
	SHARED_ChangeSkin( id, SKIN_RESET );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		p_data[id][P_HECOUNT]		= 0;
		p_data[id][P_FLASHCOUNT]	= 0;
		
		// If we need to give the user their weapons back, then lets
		SHARED_CS_Reincarnation( id );

		// If the user's ultimate is ready, lets show their icon!
		if ( ULT_Available( id ) )
		{
			Ultimate_Ready( id );
		}
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{

		// Should the user be reincarnated from an item?
		if ( g_bPlayerBoughtAnkh[id] )
		{
			// We don't want to skip since the user has this item
			p_data_b[id][PB_REINCARNATION_SKIP] = false;

			SHARED_DOD_Reincarnation( id );

			g_bPlayerBoughtAnkh[id] = false;
		}

		// or from a skill?
		else if ( SM_VerifySkill( id, SKILL_REINCARNATION ) )
		{
			if ( random_float( 0.0, 1.0 ) <= p_ankh[p_data[id][P_SKILL3]-1] )
			{
				SHARED_DOD_Reincarnation( id );
			}
		}
	}	

	// If the user is a bot they should have a chance to buy an item
	if ( is_user_bot( id ) )
	{
		if ( random_float( 0.0, 1.0 ) <= get_pcvar_num( CVAR_wc3_bot_buy_item ) )
		{
			( random_num( 1, 2 ) == 1 ) ? _MENU_Shopmenu1( id, random_num( 0, 8 ) ) : _MENU_Shopmenu2( id, random_num( 0, 8 ) );
		}
	}

	WC3_CommonSpawn( id );

	p_data_b[id][PB_DIEDLASTROUND]	= false;
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
		EVENT_JustBeforeSpawn( players[i] );
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
	if ( SHARED_ValidPlayer( iVictim ) )
	{
		// This is a check for ultimates that need to "search" for a target
		if ( SHARED_ValidPlayer( iAttacker ) && p_data_b[iAttacker][PB_ISSEARCHING] )
		{

			// Now lets make sure the person he's looking at is in view and isn't on the same team
			if ( get_user_team( iAttacker ) != get_user_team( iVictim ) && UTIL_EntInView( iAttacker, iVictim ) )
			{
				
				// Check to see if the user should block this ultimate!
				if ( ULT_IsImmune( iVictim ) )
				{
					ULT_Blocked( iAttacker );
				}

				// Then the user's ult should work!
				else
				{
					// Well we do have a target so lets execute the user's ultimate!!
					if ( SM_VerifySkill( iAttacker, ULTIMATE_CHAINLIGHTNING ) )
					{
						OR_ULT_ChainLightning( iAttacker, iVictim, iHitZone );
					}
					else if ( SM_VerifySkill( iAttacker, ULTIMATE_ENTANGLE ) )
					{
						NE_ULT_Entangle( iAttacker, iVictim );
					}
					else if ( SM_VerifySkill( iAttacker, ULTIMATE_IMMOLATE ) )
					{
						BM_ULT_Immolate( iAttacker, iVictim );
					}
				}

				// No longer searching since we found a target
				p_data_b[iAttacker][PB_ISSEARCHING]	= false;

				// Hide the ultimate icon
				ULT_Icon( iAttacker, ICON_HIDE );

				// Set up the user's ultimate delay
				ULT_ResetCooldown( iAttacker, get_pcvar_num( CVAR_wc3_ult_cooldown ) );
			}
		}

		// This is a nice check for Helm of Excellence
		if ( ITEM_Has( iVictim, ITEM_HELM ) )
		{
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
						ITEM_HelmRemoveCharge( iVictim );
					}
				}
				
				return FMRES_SUPERCEDE;
			}
		}

		// Check to see if this user has night elf's evasion
		if ( SM_VerifySkill( iVictim, SKILL_EVASION ) && SHARED_ValidPlayer( iAttacker ) )
		{
			// Do the check to see if we should "evade" this shot
			new Float:fTime = halflife_time();
			new Float:fDifference = fTime - fLastShotFired[iAttacker];

			if ( SHARED_ValidPlayer( iAttacker ) && fDifference < 0.1 && fDifference > 0.0 )
			{

				// Basically if friendly fire is on, we want to block ALL shots, otherwise we only block shots from enemies
				if ( !get_pcvar_num( CVAR_mp_friendlyfire ) )
				{
					if ( get_user_team( iAttacker ) == get_user_team( iVictim ) )
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
		if ( SHARED_ValidPlayer( iAttacker ) && p_data_b[iAttacker][PB_MOLE] && ITEM_Has( iVictim, ITEM_PROTECTANT ) )
		{	
			new Float:fTime = halflife_time();

			if ( fTime - fLastShotFired[iAttacker] < 0.1  )
			{
				client_print( iVictim, print_chat, "%s %L", g_MODclient, iVictim, "SHOT_DEFLECTED" );

				set_tr( TR_flFraction, 1.0 );
			}
		}
	}
	
	return FMRES_IGNORED;
}

// Function called right before the user spawns
EVENT_JustBeforeSpawn( id )
{
	
	// Remove the spectating info if necessary
	if ( g_fLastSpecDisplay[id] >= halflife_time() )
	{
		UTIL_ClearHudChannel( id, HUD_SPEC_INFO );
	}

	// Reset who the user was spectating
	g_iSpectatingID[id] = -1;

	// Reset the player's role
	g_iPlayerRole[id] = 0;
	
	// User shouldn't be a mole anymore...
	p_data_b[id][PB_MOLE] = false;

	// Reset the bomb/defusing check
	bHasBegunPlantingOrDefusing[id] = false;

	// Reset all ultimates
	ULT_Reset( id );

	// Reset certain player variables
	p_data_b[id][PB_HAS_SPAWNED]		= false;
	p_data[id][P_RESPAWNBY]				= 0;

	// Remove any reset_maxspeeds occuring (could cause a person to move during freezetime)
	task_exists( TASK_RESETSPEED + id ) ? remove_task( TASK_RESETSPEED + id ) : 0;
	
	// Save a copy of what weapons the user had the previous round (for weapon reincarnation)
	SHARED_CopySavedWeapons( id );

	// Remove any serpant wards
	( task_exists( TASK_LIGHT + id ) ) ? remove_task( TASK_LIGHT + id ) : 0;
}
