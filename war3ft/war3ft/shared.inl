/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Shared Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define INVIS_CLOAK_DIVISOR		1.5

// This should be called on weapon change, on new round, when the user selects a new skill, and after an item is purchased
public SHARED_INVIS_Set( id )
{
	// Do not want to set them as invisible if the player is currently rendering
	if ( !p_data_b[id][PB_CAN_RENDER] || !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	new iInvisLevel = 0;

	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( id, SKILL_INVISIBILITY );

	// If they are Human Alliance with Invisibility lets set the level
	if ( iSkillLevel > 0 )
	{
		iInvisLevel = p_invisibility[iSkillLevel-1];
	}

	// User has a Cloak of Invisibility
	if ( ITEM_Has( id, ITEM_CLOAK ) > ITEM_NONE )
	{
		// Then we are already a little invis, lets give them a little bonus for purchasing the item
		if ( iInvisLevel > 0 )
		{
			iInvisLevel = floatround( float( iInvisLevel ) / INVIS_CLOAK_DIVISOR );
		}
		else
		{
			iInvisLevel = get_pcvar_num( CVAR_wc3_cloak );
		}
	}
	
	// If the player is holding a knife they should be more invisible
	if ( SHARED_IsHoldingKnife( id ) )
	{
		iInvisLevel /= 2;
	}

	if ( iInvisLevel )
	{
		set_user_rendering( id, kRenderFxNone, 0, 0, 0, kRenderTransTexture, iInvisLevel );
		
		p_data_b[id][PB_INVIS] = true;
	}

	// User should not be invisible
	else
	{
		set_user_rendering( id );

		p_data_b[id][PB_INVIS] = false;
	}

	return;
}

// Function will return true if their active weapon is a knife
public SHARED_IsHoldingKnife( id )
{
	new iClip, iAmmo, iWeapon;
	iWeapon = get_user_weapon( id, iClip, iAmmo );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeapon == CSW_KNIFE )
		{
			return true;
		}
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		if ( iWeapon == DODW_AMERKNIFE || iWeapon == DODW_GERKNIFE || iWeapon == DODW_SPADE )
		{
			return true;
		}
	}

	return false;
}

// Function returns true if the user has a grenade in his inventory
SHARED_HasGrenade( id )
{
	new i, bool:bNadeFound = false;
	
	// Loop through all weapons and search for a grenade
	while ( g_PlayerWeapons[id][i] && !bNadeFound )
	{
		if ( SHARED_IsGrenade( g_PlayerWeapons[id][i] ) )
		{
			bNadeFound = true;
			break;
		}

		i++;
	}

	return bNadeFound;
}

bool:SHARED_IsGrenade( iWeapon )
{
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeapon == CSW_HEGRENADE )
		{
			return true;
		}
	}
	else if ( g_MOD == GAME_DOD )
	{
		if ( iWeapon == DODW_HANDGRENADE || iWeapon == DODW_STICKGRENADE )
		{
			return true;
		}
	}

	return false;
}

// Function checks to see if the weapon id is a primary weapon (used with Night Elf's Entangle)
SHARED_IsPrimaryWeapon( iWeaponID )
{
	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeaponID == CSW_GALIL || iWeaponID == CSW_FAMAS || iWeaponID == CSW_M3 || iWeaponID == CSW_XM1014 || iWeaponID == CSW_MP5NAVY || iWeaponID == CSW_TMP || iWeaponID == CSW_P90 || iWeaponID == CSW_MAC10 || iWeaponID == CSW_UMP45 || iWeaponID == CSW_AK47 || iWeaponID == CSW_SG552 || iWeaponID == CSW_M4A1 || iWeaponID == CSW_AUG || iWeaponID == CSW_SCOUT || iWeaponID == CSW_AWP || iWeaponID == CSW_G3SG1 || iWeaponID == CSW_SG550 || iWeaponID == CSW_M249 )
		{
			return true;
		}
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		return false;
	}

	return false;
}

// Function will return the user's money
SHARED_GetUserMoney( id )
{

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		return cs_get_user_money( id );
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		return p_data[id][P_MONEY];
	}

	return 0;
}

// Function will set a user's money, if show is 1 then the difference will be displayed on screen
SHARED_SetUserMoney( id, money, show = 1 )
{

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Don't allow the user to have more than 16000
		if ( money > 16000 )
		{
			money = 16000;
		}

		cs_set_user_money( id, money, show );
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{

		if ( !XP_MinPlayers() )
		{
			return;
		}

		new parm[3];
		parm[0] = id;
		parm[1] = 1;
		parm[2] = money - SHARED_GetUserMoney( id );

		p_data[id][P_MONEY] = money;
		_DOD_showMoney( parm );
	}

	return;
}

// Function will return an ammo name given a weapon ID
public SHARED_GetAmmoName( iWeapID, szAmmoName[], len )
{
	switch( iWeapID )
	{
		case CSW_USP, CSW_MAC10, CSW_UMP45:
		{ 
			copy( szAmmoName, len, "ammo_45acp" );
		}
		case CSW_ELITE, CSW_GLOCK18, CSW_MP5NAVY, CSW_TMP:
		{
			copy( szAmmoName, len, "ammo_9mm" );
		}
		case CSW_FIVESEVEN, CSW_P90:
		{
			copy( szAmmoName, len, "ammo_57mm" );
		}
		case CSW_DEAGLE:
		{
			copy( szAmmoName, len, "ammo_50ae" );
		}
		case CSW_P228:
		{
			copy( szAmmoName, len, "ammo_357sig" );
		}
		case CSW_SCOUT, CSW_G3SG1, CSW_AK47:
		{
			copy( szAmmoName, len, "ammo_762nato" );
		}
		case CSW_XM1014, CSW_M3:
		{
			copy( szAmmoName, len, "ammo_buckshot" );
		}
		case CSW_AUG, CSW_SG550, CSW_GALIL, CSW_FAMAS, CSW_M4A1:
		{
			copy( szAmmoName, len, "ammo_556nato" );
		}
		case CSW_AWP:
		{
			copy( szAmmoName, len, "ammo_338magnum" );
		}
		case CSW_M249:
		{
			copy( szAmmoName, len, "ammo_556natobox" );
		}
		case CSW_SG552:
		{
			copy( szAmmoName, len, "ammo_556nato" );
		}
		default:
		{
			copy( szAmmoName, len, "" );
		}
	}
}

// Function used to spawn a player
public _SHARED_Spawn( id )
{

	id -= TASK_SPAWN;
	
	// Spawning doesn't work in DOD
	if ( g_MOD == GAME_DOD )
	{
		return;
	}

	// User is no longer connected or is not on a team
	if ( !p_data_b[id][PB_ISCONNECTED] || !SHARED_IsOnTeam( id ) )
	{
		return;
	}

	// Round has ended, lets give money back if they bought a scroll
	if ( g_EndRound )
	{
		if ( p_data[id][P_RESPAWNBY] == RESPAWN_ITEM )
		{
			client_print( id, print_chat, "%s Unable to respawn because the round is over, here is your money back", g_MODclient );

			SHARED_SetUserMoney( id, SHARED_GetUserMoney( id ) + ITEM_Cost( id, ITEM_SCROLL ) );
		}

		return;
	}

	// Reset items when the user spawns!
	g_iShopMenuItems[id][ITEM_SLOT_ONE]	= ITEM_NONE;
	g_iShopMenuItems[id][ITEM_SLOT_TWO] = ITEM_NONE;
	
	if ( is_user_alive( id ) )
	{
		return;
	}

	// Save their previous weapons!
	SHARED_CopySavedWeapons( id );

	// Spawn the player
	spawn( id );
	spawn( id );

	p_data_b[id][PB_SLOWED]		= false;
	p_data_b[id][PB_STUNNED]	= false;
	p_data_b[id][PB_GIVEITEMS]	= true;
	
	// Give the user godmode for a little
	set_user_godmode( id, 1 );

	// Reset the user's skin to normal
	SHARED_ChangeSkin( id, SKIN_RESET );

	// The user should no longer be a mole!
	p_data_b[id][PB_MOLE] = false;

	set_task( 0.2, "_SHARED_Spawn_Final", TASK_SPAWNPLAYER + id );
	set_task( 0.4, "_SHARED_CS_GiveWeapons", TASK_GIVEITEMS + id );
	set_task( 1.0, "_SHARED_SpawnRemoveGod", TASK_SPAWNREMOVEGOD + id );

	return;
}

public _SHARED_SpawnRemoveGod( id )
{
	id -= TASK_SPAWNREMOVEGOD;

	// Only do this if the user is connected
	if ( p_data_b[id][PB_ISCONNECTED] )
	{
		set_user_godmode( id, 0 );
	}
}

// Function will just spawn a player again
public _SHARED_Spawn_Final( id )
{
	id -= TASK_SPAWNPLAYER;

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	spawn( id );
	
	// If we don't give them a suit then they won't have a HUD
	give_item( id, "item_suit" );
	
	// Vengeance Check
	if ( p_data[id][P_RESPAWNBY] == RESPAWN_VENGEANCE )
	{
		set_user_health( id, VENGEANCE_HEALTH );
	}
	
	return;
}

// Reincarnation for Day of Defeat
public SHARED_DOD_Reincarnation( id )
{

	if ( !WC3_Check() )
	{
		return PLUGIN_CONTINUE
	}
	
	// User just joined or should skip reincarnation
	if ( p_data_b[id][PB_REINCARNATION_SKIP] )
	{
		p_data_b[id][PB_REINCARNATION_SKIP] = false;
		return PLUGIN_CONTINUE;
	}

	
	if( p_data_b[id][PB_REINCARNATION_DELAY] )
	{
		client_print( id, print_chat, "%s %L", g_MODclient, id, "SKILL_REINCARNATION_SKIPPING" );
		p_data_b[id][PB_REINCARNATION_DELAY] = false;
	}
	else
	{

		client_cmd( id, "speak %s", g_szSounds[SOUND_REINCARNATION] );

		// Make the user glow!
		SHARED_Glow( id, 0, 100, 0, 0 );

		// Screen fade green
		Create_ScreenFade( id, (1<<10), (1<<10), (1<<12), 0, 255, 0, g_GlowLevel[id][1] );

		iReincarnation[id][ZPOS] += 30;

		set_user_origin( id, iReincarnation[id] );

		set_task( 0.1, "_SHARED_DOD_Reincarnation_Check", TASK_REINCARNATION + id );
		set_task( 2.5, "_SHARED_DOD_Reincarnation_Loc", TASK_REINCCHECK + id );
	}

	return PLUGIN_CONTINUE;
}

public _SHARED_DOD_Reincarnation_Check( id )
{

	id -= TASK_REINCARNATION;

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	new origin[3];
	get_user_origin( id, origin );

	// Failure, stuck somewhere, put them back
	if ( origin[2] == iReincarnation[id][2] )
	{

		new ent = SHARED_FindFreeSpawn( id, false );
		
		// Valid spawn found
		if ( ent > 0 )
		{
			new Float:fSpawnOrigin[3], vOrigin[3];
			
			// Get the origin of the spawn point
			entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );

			// Convert float vector to int vector
			FVecIVec( fSpawnOrigin, vOrigin );
			
			// Move the user
			set_user_origin( id, vOrigin );

			client_print( id, print_chat, "%s %L", g_MODclient, id, "SKILL_REINCARNATION_FAILED" )
		}

		// No Spawn found
		else
		{
			set_task( 0.1, "_SHARED_DOD_Reincarnation", TASK_REINCARNATION + id );
		}
	}

	return;
}

public _SHARED_DOD_Reincarnation_Loc( id )
{
	id -= TASK_REINCCHECK;

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}
	
	p_data_b[id][PB_REINCARNATION_DELAY] = ( is_user_alive( id ) ) ? true : false;

	return;
}

// Reincarnation for Counter-Strike and Condition Zero
public SHARED_CS_Reincarnation( id )
{

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return PLUGIN_CONTINUE;
	}

	new bool:bGiveWeapons = false;
	
	// If the give items flag is true
	if ( p_data_b[id][PB_GIVEITEMS] )
	{
		bGiveWeapons = true;
		p_data_b[id][PB_GIVEITEMS] = false;
	}
	
	// Check based on skill or if the user has an item
	if ( p_data_b[id][PB_DIEDLASTROUND] )
	{
		static iSkillLevel;
		iSkillLevel = SM_GetSkillLevel( id, SKILL_REINCARNATION );

		// Orc's Reincarnation
		if ( iSkillLevel > 0 )
		{

			if ( random_float( 0.0, 1.0 ) <= p_ankh[iSkillLevel-1] )
			{
				bGiveWeapons = true;		
			}
		}
		
		// Ankh
		if ( g_bPlayerBoughtAnkh[id] )
		{
			bGiveWeapons = true;

			g_bPlayerBoughtAnkh[id] = false;
		}	
	}

	if ( bGiveWeapons )
	{
		client_cmd( id, "speak %s", g_szSounds[SOUND_REINCARNATION] );
		
		// Make the user glow!
		SHARED_Glow( id, 0, 100, 0, 0 );

		// Screen fade green
		Create_ScreenFade( id, (1<<10), (1<<10), (1<<12), 0, 255, 0, 255 );
		
		// Give the user their weapons from last round
		set_task( 0.3, "_SHARED_CS_GiveWeapons", TASK_GIVEITEMS + id );
	}
	else
	{
		if ( !cs_get_user_nvg( id ) )
		{
			p_data_b[id][PB_NIGHTVISION] = false;
		}
	}

	return PLUGIN_CONTINUE;	
}	

public _SHARED_CS_GiveWeapons(id)
{

	if ( id > TASK_GIVEITEMS )
	{
		id -= TASK_GIVEITEMS;
	}

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	// Check to see if the user should have the bomb
	new bool:bGiveBomb = false;
	if ( cs_get_user_plant( id ) )
	{
		bGiveBomb = true;
	}

	// Remove all weapons
	strip_user_weapons( id );

	// Give armor
	if ( p_data[id][P_LASTARMOR] )
	{
		// g_ArmorType
		cs_set_user_armor( id, p_data[id][P_LASTARMOR], g_ArmorType[id] );
	}
	
	// Give a defuse kit
	if ( p_data_b[id][PB_DEFUSE] )
	{
		give_item(id, "item_thighpack");
	}

	if ( p_data_b[id][PB_NIGHTVISION] )
	{
		cs_set_user_nvg(id, 1);
	}

	// Give the user a bomb
	if ( bGiveBomb )
	{
		give_item( id, "weapon_c4" );

		cs_set_user_plant( id, 1, 1 );
	}
	
	new iWeapID = 0, i = 0, bool:bPrimaryFound = false;
	for ( i = 0; i < 32; i++ )
	{
		iWeapID = g_PlayerLastWeapons[id][i];

		if ( iWeapID )
		{
			if ( iWeapID != CSW_C4 && iWeapID != CSW_FLASHBANG )
			{
				new szWeaponName[32], szAmmoName[32];
				get_weaponname( iWeapID, szWeaponName, 31 );
				SHARED_GetAmmoName( iWeapID, szAmmoName, 31 );
				
				if ( contain( szWeaponName, "weapon_" ) == 0 )
				{
					give_item( id, szWeaponName );

					if ( strlen( szAmmoName ) > 0 )
					{
						give_item( id, szAmmoName );
						give_item( id, szAmmoName );
						give_item( id, szAmmoName );
						give_item( id, szAmmoName );
						give_item( id, szAmmoName );
					}
				}
			}
		}

		if ( SHARED_IsPrimaryWeapon( iWeapID ) )
		{
			new szWeaponName[32];
			get_weaponname( iWeapID, szWeaponName, 31 );

			bPrimaryFound = true;
		}
	}

	// Only give it if the user doesn't have a primary weapon
	if ( p_data_b[id][PB_SHIELD] )
	{
		if ( !bPrimaryFound )
		{
			give_item( id, "weapon_shield" );
		}
	}

	// Give them their flash bangs back
	while ( p_data[id][P_FLASHCOUNT] > 0 )
	{
		give_item( id, "weapon_flashbang" );

		p_data[id][P_FLASHCOUNT]--;
	}

	return;
}

public SHARED_SaveWeapons( id )
{
	if ( !is_user_alive( id ) )
	{
		return;
	}

	// Clear Array
	for( new i = 0; i < 32; i++ )
	{
		g_PlayerWeapons[id][i] = 0;
	}

	new num = 0;
	get_user_weapons( id, g_PlayerWeapons[id], num );
	
	return;
}

// Copy the weapons over right before reset hud is called, that way we don't lose our last round weapons when the round restarts
public SHARED_CopySavedWeapons( id )
{
	for ( new i = 0; i < 33; i++ )
	{
		for ( new j = 0; j < 32; j++ )
		{
			g_PlayerLastWeapons[i][j] = g_PlayerWeapons[i][j];
		}
	}
}

// Sets the user's speed, should be called after freezetime, on weapon change and after a speed modifying skill has been called
public SHARED_SetSpeed( id )
{

	if ( id >= TASK_UNHOLYSPEED )
	{
		id -= TASK_UNHOLYSPEED;
	}

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}
	
	// We should NOT change the user's speed during freezetime
	if ( g_freezeTime && ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO ) )
	{
		return;
	}

	// User is stunned, we shouldn't change their speed
	else if ( p_data_b[id][PB_STUNNED] )
	{
		set_user_maxspeed( id, 1.0 );

		return;
	}

	// User is hexed, they should be slowed
	else if ( p_data_b[id][PB_HEXED] )
	{
		set_user_maxspeed( id, SH_HEX_SPEED );

		return;
	}

	// User is slowed
	else if ( p_data_b[id][PB_SLOWED] )
	{
		set_user_maxspeed( id, get_pcvar_float( CVAR_wc3_frost ) );
		
		return;
	}
	
	// Day of Defeat specific checks
	else if ( g_MOD == GAME_DOD )
	{
		
		// User is in the prone position so we shouldn't change their speed
		if ( entity_get_int( id, EV_INT_iuser3 ) )
		{
			// When prone the maxspeed should be 50, never let it be different than this
			if ( get_user_maxspeed( id ) > 500.0 )
			{
				set_user_maxspeed( id, 50.0 );
			}

			return;
		}

		// User has a rocket launcher "mounted", we let users w/unholy aura + boots of speed run faster with it
		else if ( get_user_maxspeed( id ) == 50.0 && ( ITEM_Has( id, ITEM_BOOTS ) > ITEM_NONE || SM_GetSkillLevel( id, SKILL_UNHOLYAURA ) > 0 ) )
		{
			set_user_maxspeed( id, 600.0 );

			return;
		}
	}

	// Counter-Strike and Condition Zero specific checks
	else if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		static iSkillLevel;
		iSkillLevel = SM_GetSkillLevel( id, SKILL_UNHOLYAURA );

		// Unholy Aura bonus
		if ( iSkillLevel > 0 )
		{
			// Give them the bonus
			set_user_maxspeed( id, p_unholy[iSkillLevel-1] );

			return;
		}

		// Boots of Speed bonus
		else if ( ITEM_Has( id, ITEM_BOOTS ) > ITEM_NONE )
		{
			new iClip, iAmmo;
			new iWeapon = get_user_weapon( id, iClip, iAmmo );

			// Give them the bonus
			if ( g_iPlayerRole[id] == PLR_VIP )
			{
				set_user_maxspeed( id, CS_SPEED_VIP * ITEM_BOOT_INCREASE );
			}
			// Player zoomed in
			else if ( g_bPlayerZoomed[id] )
			{
				set_user_maxspeed( id, CS_WEAPON_SPEED_ZOOM[iWeapon] * ITEM_BOOT_INCREASE );
			}
			// Regular
			else
			{
				set_user_maxspeed( id, CS_WEAPON_SPEED[iWeapon] * ITEM_BOOT_INCREASE );
			}

			return;
		}
	}

	// We would never get here unless we didn't hit any if statement from above
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Here we want to reset the user's speed
		new iClip, iAmmo;
		new iWeapon = get_user_weapon( id, iClip, iAmmo );
		
		if ( g_iPlayerRole[id] == PLR_VIP )
		{
			set_user_maxspeed( id, CS_SPEED_VIP );
		}
		else if ( g_bPlayerZoomed[id] )
		{
			set_user_maxspeed( id, CS_WEAPON_SPEED_ZOOM[iWeapon] );
		}
		else
		{
			set_user_maxspeed( id, CS_WEAPON_SPEED[iWeapon] );
		}
	}
	else if ( g_MOD == GAME_DOD )
	{
		set_user_maxspeed( id, 600.0 );
	}

	return;
}

public SHARED_ResetMaxSpeed( id )
{

	if ( id >= TASK_RESETSPEED )
	{
		id -= TASK_RESETSPEED;
	}


	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}
	
	// User should no longer be stunned
	p_data_b[id][PB_STUNNED]	= false;

	// User should no longer be slowed
	p_data_b[id][PB_SLOWED]		= false;

	SHARED_SetSpeed( id );

	return;
}

// Returns true if the user is hexed/stunned/slowed
SHARED_IsPlayerSlowed( id )
{

	if ( p_data_b[id][PB_STUNNED] || p_data_b[id][PB_SLOWED] )
	{
		return true;
	}

	return false;
}

// Function changes your skin for ITEM_MOLE and Chameleon
public SHARED_ChangeSkin( id, iFlag )
{
	
	// Don't change a bots model - it just confuses the bots!
	if ( is_user_bot( id ) )
	{
		return;
	}

	new szSkin[32];
	
	// Reset the user's model
	if ( iFlag == SKIN_RESET && p_data_b[id][PB_SKINSWITCHED] )
	{

		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			cs_reset_user_model( id );
		}
		else if ( g_MOD == GAME_DOD )
		{
			dod_clear_model( id );
		}

		p_data_b[id][PB_SKINSWITCHED] = false;
	}
	
	// Switch the user's skin to the opposing team
	else if ( iFlag == SKIN_SWITCH )
	{
		new iTeam = get_user_team( id );
		
		// For Counter-Strike or Condition Zero
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			new iModelNum = random_num( 0, 3 );
			
			// Condition Zero has one more model!
			if ( g_MOD == GAME_CZERO )
			{
				iModelNum = random_num( 0, 4 );
			}
			
			// Save which skin we're going to use
			if ( iTeam == TEAM_CT )
			{
				add( szSkin, 31, SKIN_T[iModelNum] );
			}
			else
			{
				add( szSkin, 31, SKIN_CT[iModelNum] );
			}

			cs_set_user_model( id, szSkin );

		}

		// For Day of Defeat
		else if ( g_MOD == GAME_DOD )
		{
			new iModelNum = random_num( 0, 1 );

			if ( iTeam == ALLIES )
			{
				add( szSkin, 31, SKIN_AXIS[iModelNum] );
			}
			else
			{
				
				// Then we should use a British model
				if ( dod_get_map_info( MI_ALLIES_TEAM ) == 1 )
				{
					add( szSkin, 31, SKIN_BRIT[0] );
				}
				
				// Otherwise use American model
				else
				{
					add( szSkin, 31, SKIN_ALLIES[iModelNum] );
				}
			}
			
			dod_set_model( id, szSkin );
		}

		p_data_b[id][PB_SKINSWITCHED] = true;
	}

	return;
}

public SHARED_SetGravity( id )
{

	// Can't set gravity if user isn't connected!
	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	// If gravity is less than this, lets not change per-user b/c it BLOWS ASS in game

	if ( CVAR_sv_gravity == 0 || get_pcvar_num( CVAR_sv_gravity ) > 650 )
	{
		static iSkillLevel;
		iSkillLevel = SM_GetSkillLevel( id, SKILL_LEVITATION );

		new Float:fGravityLevel = 1.0;

		// Our gravity level bonus because of undead's levitation
		if ( iSkillLevel > 0 )
		{
			fGravityLevel = p_levitation[iSkillLevel-1];
		}

		// Set the user's gravity based on the item
		if ( ITEM_Has( id, ITEM_SOCK ) > ITEM_NONE )
		{
			
			// User has levitation + sock, give them an extra bonus
			if ( fGravityLevel > 0.0 )
			{
				fGravityLevel /= 2.0;

			}

			// User just has sock
			else
			{
				fGravityLevel = get_pcvar_float( CVAR_wc3_sock );
			}
		}

		// Set the user's gravity!
		set_user_gravity( id, fGravityLevel );
	}

	return;
}

public SHARED_IsOnTeam( id )
{
	new iTeam = get_user_team( id );

	// Counter-Strike or Condition Zero Check
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iTeam == CTS || iTeam == TS )
		{
			return true;
		}
	}

	// Day of Defeat Check
	else if ( g_MOD == GAME_DOD )
	{
		if ( iTeam == ALLIES || iTeam == AXIS )
		{
			return true;
		}
	}

	return false;
}

// Reset our reserved spawn points
public _SHARED_SpawnReset()
{
	new i;

	for ( i = 0; i < TOTAL_SPAWNS; i++ )
	{
		g_iSpawnReserved[i] = 0;
	}
}

// Determine if this entity is reserved for another player
bool:SHARED_SpawnReserved( ent )
{
	new i;

	for ( i = 0; i < g_iSpawnInc; i++ )
	{
		if ( g_iSpawnReserved[i] == ent )
		{
			return true;
		}
	}

	return false;
}

// Find a free spawn!
SHARED_FindFreeSpawn( id, bImmunityCheck = false, bReverseTeam = false )
{

	new iPlayersInVicinity, iSpawnID, iEntList[1], vOrigin[3];
	new ent = -1;
	new Float:fSpawnOrigin[3];
	new Float:fVicinity = 96.0;
	new bool:bFound = false;
	new bool:bImmunityNear = false;
	
	new iTeam = get_user_team( id );

	// Reverse the team IDs (i.e. Mole would want this)
	if ( bReverseTeam )
	{
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			iTeam = ( ( iTeam == TEAM_CT ) ? TEAM_T : TEAM_CT );
		}
		else if ( g_MOD == GAME_DOD )
		{
			iTeam = ( ( iTeam == AXIS ) ? ALLIES : AXIS );
		}
	}

	// Need to determine which spawn point to look for based on the user's team
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		iSpawnID = ( ( iTeam == TEAM_CT ) ? 0 : 1 );
	}
	else if ( g_MOD == GAME_DOD )
	{
		iSpawnID = ( ( iTeam == AXIS ) ? 0 : 1 );
	}

	// Loop through each ent until we find a spawn entity that we want
	do {
		ent = find_ent_by_class( ent, szSpawnEnt[iSpawnID] );
		
		// Valid ent found
		if ( ent != 0 )
		{
			entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );
			
			// Convert float vector to int vector
			FVecIVec( fSpawnOrigin, vOrigin );

			// Check to see if there are players in this spawn
			iPlayersInVicinity = find_sphere_class( 0, "player", fVicinity, iEntList, 1, fSpawnOrigin );
			
			// We have a free spawn!!
			if ( iPlayersInVicinity == 0 )
			{

				// Make sure it isn't reserved
				if ( !SHARED_SpawnReserved( ent ) )
				{

					// Then we need to reserve it :)
					g_iSpawnReserved[g_iSpawnInc++] = ent;

					// We need to make sure there isn't anyone nearby that is immune
					if ( bImmunityCheck )
					{
						// Immune found
						if ( WC3_IsImmunePlayerNear( id, vOrigin ) )
						{
							bImmunityNear = true;
						}

						// We're clear!
						else
						{
							bImmunityNear = false;
							bFound = true;
						}
					}
					
					// We have a free spawn we can quit!
					else
					{
						bFound = true;
					}
				}
			}
		}
	}
	while ( ent && !bFound && !bImmunityNear )
	
	// Reset the spawn points...
	if ( !task_exists( TASK_RESETSPAWNS ) )
	{
		set_task( 0.3, "_SHARED_SpawnReset", TASK_RESETSPAWNS );
	}

	// Failed, nothing found
	if ( !bFound )
	{
		// Return a different value so they know the reason for failing
		if ( bImmunityCheck && bImmunityNear )
		{
			return -2;
		}

		return -1;
	}
	
	// Otherwise we found something!
	return ent;
}

public SHARED_MoleCheck( id )
{
	new parm[2];
	parm[1] = 0;

	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( id, SKILL_FANOFKNIVES );

	// Mole from Fan of Knives?
	if ( iSkillLevel > 0 && random_float( 0.0, 1.0 ) <= p_fan[iSkillLevel-1] )
	{
		parm[1] = 1;
	}
	
	// Mole from an item?
	if ( parm[1] == 0 )
	{
		if ( g_bPlayerBoughtMole[id] )
		{
			parm[1] = 2;

			g_bPlayerBoughtMole[id] = false;
		}
	}

	// OK then lets mole!!
	if ( parm[1] > 0 )
	{
		parm[0] = id;
		
		set_task( 0.1, "_SHARED_Mole", TASK_MOLE + id, parm, 2 );
	}
}

public _SHARED_Mole( parm[2] )
{
	
	new id = parm[0];

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	// Lets search for a new spawn (ignore immunity, reverse team ids)	
	new ent = SHARED_FindFreeSpawn( id, false, true );
	
	// Free spawn found!!
	if ( ent > 0 )
	{
		new vOrigin[3], Float:fSpawnOrigin[3];
		
		// Get the origin of the spawn
		entity_get_vector( ent, EV_VEC_origin, fSpawnOrigin );
		
		// Convert float vector to int vector
		FVecIVec( fSpawnOrigin, vOrigin );
		
		// Change the user's skin
		SHARED_ChangeSkin( id, SKIN_SWITCH );
		
		// Move the user
		set_user_origin( id, vOrigin );

		// Shake the user's screen
		Create_ScreenShake( id, (255<< 14), (10 << 14), (255<< 14) );
		
		// User is a mole
		p_data_b[id][PB_MOLE] = true;
		
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			if ( !g_bMoleBuyZoneDisabled )
			{
				// Give the user a buyzone!
				SHARED_CreateBuyZone( id );

				// Display to target they can buy
				client_print( id, print_center, "Quick buy some items!" );
			}
		}
	}

	// No spawn found
	else
	{
		// Moving b/c of item - if no spot then give the user his/her money back!
		if ( parm[1] == 2 )
		{
			SHARED_SetUserMoney( id, SHARED_GetUserMoney( id ) + ITEM_Cost( id, ITEM_MOLE ), 1 );

			client_print( id, print_chat, "%s %L", g_MODclient, id, "NO_SPOT_MOLE_MONEY" );
		}

		// Moving b/c of skill
		else
		{
			client_print( id, print_chat, "%s %L", g_MODclient, id, "NO_SPOT_TO_MOLE" )
		}
	}

	return; 
} 

bool:SHARED_ValidPlayer( id )
{
	if ( id < 1 || id > MAXPLAYERS )
	{
		return false;
	}

	return true;
}

#define MAXGLOW					150

SHARED_Glow( id, iRed, iGreen, iBlue, iAll )
{
	
	// Not allowed to glow right now...
	if ( !p_data_b[id][PB_CAN_RENDER] )
	{
		return;
	}
		
	// Don't glow if invisible
	else if ( SM_GetSkillLevel( id, SKILL_INVISIBILITY ) > 0 || ITEM_Has( id, ITEM_CLOAK ) > ITEM_NONE )
	{
		return;
	}

	// Only glow if the task doesn't exist!
	else if ( task_exists( TASK_GLOW + id ) )
	{
		return;
	}
	
	if ( iAll )
	{
		g_GlowLevel[id][0]	= 0;
		g_GlowLevel[id][1]	= 0;
		g_GlowLevel[id][2]	= 0;
		g_GlowLevel[id][3]	+= iAll;
	}
	else if ( iRed )
	{
		g_GlowLevel[id][0]	+= iRed;
		g_GlowLevel[id][1]	= 0;
		g_GlowLevel[id][2]	= 0;
		g_GlowLevel[id][3]	= 0;
	}
	else if ( iGreen )
	{
		g_GlowLevel[id][0]	= 0;
		g_GlowLevel[id][1]	+= iGreen;
		g_GlowLevel[id][2]	= 0;
		g_GlowLevel[id][3]	= 0;
	}
	else if ( iBlue )
	{
		g_GlowLevel[id][0]	= 0;
		g_GlowLevel[id][1]	= 0;
		g_GlowLevel[id][2]	+= iBlue;
		g_GlowLevel[id][3]	= 0;
	}

	// Lets make sure its not over the max!
	g_GlowLevel[id][0] = ( ( g_GlowLevel[id][0] > MAXGLOW ) ? MAXGLOW : g_GlowLevel[id][0] );
	g_GlowLevel[id][1] = ( ( g_GlowLevel[id][1] > MAXGLOW ) ? MAXGLOW : g_GlowLevel[id][1] );
	g_GlowLevel[id][2] = ( ( g_GlowLevel[id][2] > MAXGLOW ) ? MAXGLOW : g_GlowLevel[id][2] );
	g_GlowLevel[id][3] = ( ( g_GlowLevel[id][3] > MAXGLOW ) ? MAXGLOW : g_GlowLevel[id][3] );
	

	_SHARED_Glow( id );
}

public _SHARED_Glow( id )
{
	
	if ( id >= TASK_GLOW )
	{
		id -= TASK_GLOW;
	}

	// User is no longer connected, so lets not continue this!
	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}
	
	new iRed	= g_GlowLevel[id][0];
	new iGreen	= g_GlowLevel[id][1];
	new iBlue	= g_GlowLevel[id][2];
	new iAll	= g_GlowLevel[id][3];

	// Then we want to glow
	if ( iRed || iGreen || iBlue )
	{

		g_GlowLevel[id][0] = ( ( iRed > 5 )		? iRed - 5		: 0 );
		g_GlowLevel[id][1] = ( ( iGreen > 5 )	? iGreen - 5	: 0 );
		g_GlowLevel[id][2] = ( ( iBlue > 5 )	? iBlue - 5		: 0 );

		set_user_rendering( id, kRenderFxGlowShell, iRed, iGreen, iBlue, kRenderNormal, 16 );
	}

	else if ( iAll )
	{
		g_GlowLevel[id][3] = ( ( iAll > 5 )		? iAll - 5		: 0 );
		
		set_user_rendering( id, kRenderFxGlowShell, iAll, iAll, iAll, kRenderNormal, 16 );
	}

	// No more glowing!
	else
	{
		set_user_rendering( id );

		return;
	}

	set_task( 0.2, "_SHARED_Glow", TASK_GLOW + id );

	return;
}

SHARED_GetMaxArmor( id )
{
	id--;	// Need this or a dumb compiler warning :/

	return 100;
}

// Returns if an origin is near an objective (returns which objective)
SHARED_NearObjective( vOrigin[3] )
{

	new i, Float:fOrigin[3];
	
	// Convert vector to float
	IVecFVec( vOrigin, fOrigin );

	// Check the distances
	for ( i = 0; i < g_iTotalObjectiveEnts; i++ )
	{
		new Float:fDistance = vector_distance( fOrigin, g_fObjectiveOrigin[i] );

		if ( fDistance < 250.0 )
		{
			return g_iObjectiveEntType[i];
		}
	}

	return -1;
}

// This will create a buyzone on a target and remove it when buytime is complete
SHARED_CreateBuyZone( id )  
{       
	new iEnt = create_entity( "func_buyzone" );  

	new Float:vPlayerOrigin[3];  
	entity_get_vector( id, EV_VEC_origin, vPlayerOrigin );  
	entity_set_vector( iEnt, EV_VEC_origin, vPlayerOrigin );  

	DispatchSpawn( iEnt );  

	new Float:fDuration = ( get_pcvar_float( CVAR_mp_buytime ) * 60.0 );

	set_task( fDuration, "_SHARED_RemoveBuyZone", TASK_REMOVEBUYZONE + iEnt );  

	return;       
}  

// This will remove the buyzone if it exists
public _SHARED_RemoveBuyZone( iEnt )  
{
	if ( iEnt >= TASK_REMOVEBUYZONE )
	{ 	 
		iEnt -= TASK_REMOVEBUYZONE; 	 
	}

	// Make sure we're removing a valid entity!
	if ( is_valid_ent( iEnt ) )
	{
		remove_entity( iEnt );  
	}

	return;  
}

stock SHARED_ForceWeaponChange( id )
{
	new iAmmo, iClip, szWeaponName[32], i;
	new num = 0, iWeapons[32];

	new iCurWeapon = get_user_weapon( id, iAmmo, iClip );

	// We need a valid current weapon to do this!
	if ( iCurWeapon != 0 )
	{
		// Get all weapons the user is holding
		get_user_weapons( id, iWeapons, num );

		// Loop through all weapons
		for ( i = 0; i < num; i++ )
		{
			
			// Find a weapon the player isn't currently holding
			if ( iWeapons[i] != iCurWeapon && iWeapons[i] > 0 )
			{
				get_weaponname( iWeapons[i], szWeaponName, 31 );

				client_cmd( id, szWeaponName );

				break;
			}
		}

		// Switch back to the user's current weapon
		get_weaponname( iCurWeapon, szWeaponName, 31 );
		client_cmd( id, szWeaponName );
	}

	return;
}