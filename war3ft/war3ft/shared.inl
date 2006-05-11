/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Shared Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define INVIS_CLOAK_DIVISOR		1.5

// This should be called on weapon change, on new round, when the user selects a new skill, and after an item is purchased
public SHARED_INVIS_Set( id )
{
	// Do not want to set them as invisible if the player is currently rendering
	if ( !p_data_b[id][PB_CAN_RENDER] )
	{
		return;
	}

	new iInvisLevel = 0;
	
	// If they are Human Alliance with Invisibility lets set the level
	if ( Verify_Skill( id, RACE_HUMAN, SKILL1 ) )
	{
		new iSkillLevel = p_data[id][P_SKILL1]-1;

		iInvisLevel = p_invisibility[iSkillLevel];
	}

	// User has a Cloak of Invisibility
	if ( p_data[id][P_ITEM] == ITEM_CLOAK )
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
public SHARED_HasGrenade( id )
{
	new i, bool:bNadeFound = false;
	
	// Loop through all weapons and search for a grenade
	while ( g_PlayerWeapons[id][i] && !bNadeFound )
	{
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			if ( g_PlayerWeapons[id][i] == CSW_HEGRENADE )
			{
				bNadeFound = true;
			}
		}
		else if ( g_MOD == GAME_DOD )
		{
			if ( g_PlayerWeapons[id][i] == DODW_HANDGRENADE || g_PlayerWeapons[id][i] == DODW_STICKGRENADE )
			{
				bNadeFound = true;
			}
		}

		i++;
	}

	return bNadeFound;
}

// Function checks to see if the weapon id is a primary weapon (used with Night Elf's Entangle)
SHARED_IsPrimaryWeapon( iWeaponID )
{
	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeaponID == CSW_GALIL || iWeaponID == CSW_FAMAS || iWeaponID == CSW_M3 || iWeaponID == CSW_XM1014 || iWeaponID == CSW_MP5NAVY || iWeaponID == CSW_TMP || iWeaponID == CSW_P90 || iWeaponID == CSW_MAC10 || iWeaponID == CSW_UMP45 || iWeaponID == CSW_AK47 || iWeaponID == CSW_SG552 || iWeaponID == CSW_M4A1 || iWeaponID == CSW_AUG || iWeaponID == CSW_SCOUT || iWeaponID == CSW_AWP || iWeaponID == CSW_G3SG1 || iWeaponID == CSW_SG550 || iWeaponID == CSW_M249)
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
		cs_set_user_money( id, money, show );
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		new parm[3];
		parm[0] = id;
		parm[1] = 1;
		parm[2] = money - SHARED_GetUserMoney( id );

		p_data[id][P_MONEY] = money;
		_DOD_showMoney( parm );
	}
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
public SHARED_Spawn( id )
{
	// Spawning doesn't work in DOD
	if ( g_MOD == GAME_DOD )
	{
		return PLUGIN_CONTINUE;
	}

	if( !p_data_b[id][PB_ISCONNECTED] || get_user_team( id ) == UNASSIGNED )
	{
		return PLUGIN_CONTINUE;
	}

	p_data[id][P_ITEM]	= 0;
	p_data[id][P_ITEM2]	= 0;
	
	if ( is_user_alive( id ) )
	{
		return PLUGIN_CONTINUE;
	}

	p_data_b[id][PB_PLAYERSPAWNED] = true;

	// Spawn the player
	spawn( id );
	spawn( id );

	p_data_b[id][PB_SLOWED]		= false;
	p_data_b[id][PB_STUNNED]	= false;
	p_data_b[id][PB_GIVEITEMS]	= true;

	set_task( 0.2, "_SHARED_Spawn_Final", TASK_SPAWNPLAYER + id );
	set_task( 0.4, "_SHARED_CS_GiveWeapons", TASK_GIVEITEMS + id );

	return PLUGIN_CONTINUE;
}

// Function will just spawn a player again
public _SHARED_Spawn_Final( id )
{
	id -= TASK_SPAWNPLAYER;

	if( !p_data_b[id][PB_ISCONNECTED] )
	{
		return PLUGIN_CONTINUE;
	}

	spawn( id );
	
	// If we don't give them a suit then they won't have a HUD
	give_item( id, "item_suit" );
	
	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		give_item( id, "weapon_knife" );
	}

	return PLUGIN_CONTINUE;
}

/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Reincarnation for Day of Defeat
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/
public SHARED_DOD_Reincarnation( id )
{

	if ( !warcraft3 )
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

		client_cmd( id, "speak %s", SOUND_REINCARNATION );

		if ( iglow[id][1] < 1 )
		{
			new parm[2];
			parm[0] = id;
			set_task( 0.1, "glow_change", TASK_GLOW + id, parm, 2 );
		} 
		iglow[id][1] += 100;
		iglow[id][0] = 0;
		iglow[id][2] = 0;
		iglow[id][3] = 0;
		if ( iglow[id][1] > MAXGLOW )
		{
			iglow[id][1] = MAXGLOW;
		}

		// Screen fade green
		Create_ScreenFade( id, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[id][1] );

		iReincarnation[id][ZPOS] += 30;

		set_user_origin( id, iReincarnation[id] );

		set_task( 0.1, "_SHARED_DOD_Reincarnation_Check", TASK_REINCARNATION + id );
		set_task( 2.5, "_SHARED_DOD_Reincarnation_Loc", TASK_REINCCHECK + id );
	}

	return PLUGIN_CONTINUE;
}

public _SHARED_DOD_Reincarnation_Check( id )
{
	
	if ( !warcraft3 )
	{
		return;
	}

	id -= TASK_REINCARNATION;

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	new origin[3];
	get_user_origin( id, origin );

	// Failure, stuck somewhere, put them back
	if( origin[2] == iReincarnation[id][2] )
	{

		new iSpawnID = 0, playersInVicinity, entList[1], i, ent = -1;
		new Float:spawnOrigin[3];
		new const Float:vicinity = 96.0;
		new bool:bPlayerFound = false
		
		if ( get_user_team( id ) == TS )
		{
			iSpawnID = 1;
		}

		do
		{
			ent = find_ent_by_class( ent, szSpawnEnt[iSpawnID] );
			if ( ent != 0 )
			{
				entity_get_vector( ent, EV_VEC_origin, spawnOrigin );

				playersInVicinity = find_sphere_class( 0, "player", vicinity, entList, 1, spawnOrigin );

				if ( playersInVicinity == 0 )
				{
					bPlayerFound = true
				}
			}
		}
		while ( ent && !bPlayerFound )
		
		// Player is in the way, we can't spawn
		if( bPlayerFound )
		{
			for( i=0; i<3; i++ )
			{
				origin[i] = floatround( spawnOrigin[i] );
			}

			set_user_origin( id, origin );

			client_print( id, print_chat, "%s %L", g_MODclient, id, "SKILL_REINCARNATION_FAILED" )
		}
		else
		{
			set_task( 0.1, "_SHARED_DOD_Reincarnation", TASK_REINCARNATION + id );
		}

	}

	return;
}

public _SHARED_DOD_Reincarnation_Loc( id )
{
	if ( !warcraft3 )
	{
		return;
	}

	id -= TASK_REINCCHECK;

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}
	
	p_data_b[id][PB_REINCARNATION_DELAY] = ( is_user_alive( id ) ) ? true : false;

	return;
}

/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Reincarnation for Counter-Strike and Condition Zero
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/
public SHARED_CS_Reincarnation( id )
{

	if( !p_data_b[id][PB_ISCONNECTED] || !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}

	new bool:bGiveWeapons = false;
	
	// If the give items flag is true
	if( p_data_b[id][PB_GIVEITEMS] )
	{
		bGiveWeapons = true;
		p_data_b[id][PB_GIVEITEMS] = false;
	}
	
	// Check based on skill or if the user has an item
	if (p_data_b[id][PB_DIEDLASTROUND])
	{
		// Orc's Reincarnation
		if ( Verify_Skill(id, RACE_ORC, SKILL3) )
		{
			if( random_float( 0.0, 1.0 ) <= p_ankh[p_data[id][P_SKILL3]-1] )
			{
				bGiveWeapons = true;		
			}
		}
		
		// Ankh
		if ( p_data[id][P_LASTITEM] == ITEM_ANKH )
		{
			bGiveWeapons = true;
		}	
	}

	if ( bGiveWeapons )
	{
		client_cmd( id, "speak %s", SOUND_REINCARNATION );

		if (iglow[id][1] < 1)
		{
			new parm[2];
			parm[0] = id;
			set_task( 0.1, "glow_change", TASK_GLOW + id, parm, 2 );
		}

		iglow[id][1] += 100;
		iglow[id][0] = 0;
		iglow[id][2] = 0;
		iglow[id][3] = 0;

		if (iglow[id][1]>MAXGLOW)
		{
			iglow[id][1]=MAXGLOW;
		}

		// Screen fade green
		Create_ScreenFade( id, (1<<10), (1<<10), (1<<12), 0, 255, 0, 255 );
		
		// Give the user their weapons from last round
		set_task( 0.3, "_SHARED_CS_GiveWeapons", TASK_REINCARNATION + id );
	}
	else
	{
		if( !cs_get_user_nvg( id ) )
		{
			p_data_b[id][PB_NIGHTVISION] = false;
		}
	}

	return PLUGIN_CONTINUE;	
}	

public _SHARED_CS_GiveWeapons(id)
{
	if ( id > TASK_REINCARNATION )
	{
		id -= TASK_REINCARNATION;
	}

	if ( !warcraft3 || !p_data_b[id][PB_ISCONNECTED] )
	{
		return PLUGIN_CONTINUE;
	}

	// Remove all weapons
	strip_user_weapons( id );

	give_item( id, "weapon_knife" );

	// Give armor
	if ( p_data[id][P_ARMORONDEATH] )
	{
		// g_ArmorType
		cs_set_user_armor( id, p_data[id][P_ARMORONDEATH], g_ArmorType[id] );
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

	if ( p_data_b[id][PB_SHIELD] )
	{
		give_item(id, "weapon_shield");
	}
	
	new iWeapID = 0, i = 0;
	for ( i = 0; i < 32; i++ )
	{
		iWeapID = g_PlayerLastWeapons[id][i];

		if ( iWeapID )
		{
			if ( iWeapID != CSW_C4 && iWeapID != CSW_KNIFE )
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
	}

	return PLUGIN_CONTINUE;
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
	
	new szWeaponName[32];
	for( new i = 0; i < 32; i++ )
	{
		if ( g_PlayerWeapons[id][i] )
		{
			get_weaponname( g_PlayerWeapons[id][i], szWeaponName, 31 );
		}
	}

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
	if ( id > TASK_UNHOLYSPEED )
	{
		id -= TASK_UNHOLYSPEED;
	}

	if( !p_data_b[id][PB_ISCONNECTED] )
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
		set_user_maxspeed( id, SKILL_HEX_SPEED );

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
			if( get_user_maxspeed( id ) > 500.0 )
			{
				set_user_maxspeed( id, 50.0 );
			}

			return;
		}

		// User has a rocket launcher "mounted", we let users w/unholy aura + boots of speed run faster with it
		else if ( get_user_maxspeed( id ) == 50.0 && ( p_data[id][P_ITEM] == ITEM_BOOTS || Verify_Skill( id, RACE_UNDEAD, SKILL2 ) ) )
		{
			set_user_maxspeed( id, 600.0 );

			return;
		}
	}

	// Counter-Strike and Condition Zero specific checks
	else if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		// Unholy Aura bonus
		if ( Verify_Skill( id, RACE_UNDEAD, SKILL2 ) )
		{
			// Give them the bonus
			set_user_maxspeed( id, p_unholy[p_data[id][P_SKILL2]-1] );

			return;
		}

		// Boots of Speed bonus
		else if ( p_data[id][P_ITEM] == ITEM_BOOTS )
		{

			// Give them the bonus
			set_user_maxspeed( id, get_pcvar_float( CVAR_wc3_boots ) );

			return;
		}
	}

	// We would never get here unless we didn't hit any if statement from above
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		set_user_maxspeed( id, 250.0 );
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


	if( !p_data_b[id][PB_ISCONNECTED] )
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
public SHARED_ChangeSkin( id, reset )
{

	if ( !warcraft3 || g_MOD == GAME_DOD )
	{
		return;
	}

	new szSkin[32];
	
	// Reset the user's model
	if ( reset == SKIN_RESET && p_data[id][P_SKINCHANGED] == SKIN_SWITCH )
	{
		cs_reset_user_model( id );
		p_data[id][P_SKINCHANGED] = SKIN_RESET;
	}
	
	// Switch the user's skin to the opposing team
	else if ( reset == SKIN_SWITCH )
	{
		new iModelNum = random_num( 0, 3 );

		if ( g_MOD == GAME_CZERO )
		{
			iModelNum = random_num( 0, 4 );
		}

		if ( get_user_team( id ) == TS )
		{
			add( szSkin, 31, CTSkins[iModelNum] );
		}
		else
		{
			add( szSkin, 31, TSkins[iModelNum] );
		}

		cs_set_user_model( id, szSkin );

		p_data[id][P_SKINCHANGED] = SKIN_SWITCH;
	}

	return;
}

public SHARED_SetGravity( id )
{
	
	// If gravity is less than this, lets not change per-user b/c it BLOWS ASS in game

	if ( get_cvar_num("sv_gravity") > 650 )
	{
		// Set the user's gravity based on the item
		if ( p_data[id][P_ITEM2] == ITEM_SOCK )
		{
			set_user_gravity( id, get_pcvar_float( CVAR_wc3_sock ) );
		}

		// Set the user's gravity based on undead's levitation
		else if ( Verify_Skill( id, RACE_UNDEAD, P_SKILL3 ) )
		{
			if ( get_user_gravity( id ) != p_levitation[p_data[id][P_SKILL3]-1] )
			{
				set_user_gravity( id, p_levitation[p_data[id][P_SKILL3]-1] );
			}
		}
		else
		{
			set_user_gravity( id, 1.0 );
		}
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