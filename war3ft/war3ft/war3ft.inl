/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	All WC3 Specific functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/


#define BOT_RESPAWN			0.10			// Chance that a bot will buy a respawn scroll when dying


/***********************************************************************
		Precaching FUNCTIONS (idea from war3x)
***********************************************************************/

public WC3_Precache()
{
	
	// Precache wc3.css (if it exists!)
	if ( !file_exists( "wc3.css" ) )
	{
		log_amx( "[ERROR] Missing file 'wc3.css'" );

		set_fail_state( "A required file is missing, unable to load plugin" );
	}
	else
	{
		precache_generic( "wc3.css" );
	}

	copy( g_szSounds[SOUND_VENGEANCE]		, 63, "warcraft3/MiniSpiritPissed1.wav"				);	// Warden's Ultimate
	copy( g_szSounds[SOUND_SUICIDE]			, 63, "ambience/particle_suck1.wav"					);	// Undead's Ultimate
	copy( g_szSounds[SOUND_BANISH]			, 63, "warcraft3/banishcaster.wav"					);	// Banish
	copy( g_szSounds[SOUND_LOCUSTSWARM]		, 63, "warcraft3/locustswarmloop.wav"				);	// Locust Swarm
	copy( g_szSounds[SOUND_SHADOWSTRIKE]	, 63, "warcraft3/shadowstrikebirth.wav"				);	// Shadow Strike
	copy( g_szSounds[SOUND_CARRION]			, 63, "warcraft3/carrionswarmlaunch1.wav"			);	// Carrion Beetles
	copy( g_szSounds[SOUND_IMPALE]			, 63, "warcraft3/impalehit.wav"						);	// Impale
	copy( g_szSounds[SOUND_VOODOO]			, 63, "warcraft3/divineshield.wav"					);	// Big Bad Voodoo
	copy( g_szSounds[SOUND_FLAMESTRIKE]		, 63, "warcraft3/flamestriketargetwavenonloop1.wav"	);	// Flame Strike
	copy( g_szSounds[SOUND_ENTANGLING]		, 63, "warcraft3/entanglingrootstarget1.wav"		);	// Entangling Roots
	copy( g_szSounds[SOUND_LIGHTNING]		, 63, "warcraft3/lightningbolt.wav"					);	// Chain Lightning
	copy( g_szSounds[SOUND_TELEPORT]		, 63, "warcraft3/massteleporttarget.wav"			);	// Teleport
	copy( g_szSounds[SOUND_BLINK]			, 63, "warcraft3/blinkarrival.wav"					);	// Blink Teleport
	copy( g_szSounds[SOUND_LEVELUP]			, 63, "warcraft3/levelupcaster.wav"					);	// Level up
	copy( g_szSounds[SOUND_PICKUPITEM]		, 63, "warcraft3/pickupitem.wav"					);	// Buy / Pick up item
	copy( g_szSounds[SOUND_TOME]			, 63, "warcraft3/Tomes.wav"							);	// Tome of Experience
	copy( g_szSounds[SOUND_ULTIMATESCAN]	, 63, "turret/tu_ping.wav"							);	// Ultimate Beep
	copy( g_szSounds[SOUND_ULTIMATEREADY]	, 63, "warcraft3/resurrecttarget.wav"				);	// Ultimate Beep
	copy( g_szSounds[SOUND_ANNIHILATION]	, 63, "warcraft3/PurgeTarget1.wav"					);	// Orb of Annihilation
	copy( g_szSounds[SOUND_CONCOCTION_CAST]	, 63, "warcraft3/PossessionMissileLaunch1.wav"		);	// Unstable Concoction Cast
	copy( g_szSounds[SOUND_CONCOCTION_HIT]	, 63, "warcraft3/PossessionMissileHit1.wav"			);	// Unstable Concoction Hit
	copy( g_szSounds[SOUND_HEX]				, 63, "warcraft3/PolymorphDone.wav"					);	// Hex
	copy( g_szSounds[SOUND_IMMOLATE]		, 63, "warcraft3/ImmolationDecay1.wav"				);	// Immolate
	copy( g_szSounds[SOUND_IMMOLATE_BURNING], 63, "ambience/flameburst1.wav"				    );	// Immolate Burning

	copy( g_szSounds[SOUND_REINCARNATION]	, 63, "warcraft3/soundpack/reincarnation.wav"		);	// Weapon Reincarnation/Ankh
	copy( g_szSounds[SOUND_ANTEND]			, 63, "warcraft3/antend.wav"						);	// Sound played when you try to shoot a chameleoned teammate 
	copy( g_szSounds[SOUND_ERROR]			, 63, "warcraft3/bonus/Error.wav"					);	// Sound played on errors 

	copy( g_szSounds[SOUND_DISPELL]			, 63, "warcraft3/DispelMagicTarget.wav"				);	// Dispell an ultimate - played on caster
	copy( g_szSounds[SOUND_SPELLSHIELD]		, 63, "warcraft3/SpellShieldImpact1.wav"			);	// Dispell an ultimate - played on blocker


	// Lets make sure we have all of the sound files!!!
	new i, szTmp[128], bool:bError = false;

	for ( i = 0; i < MAX_SOUNDS; i++ )
	{

		// We only need to look for warcraft3 files (other files should be w/base HLDS installation)
		if ( containi( g_szSounds[i], "warcraft3" ) != -1 )
		{
			formatex( szTmp, 127, "sound/%s", g_szSounds[i] );

			if ( !file_exists( szTmp ) )
			{
				log_amx( "[ERROR] Missing sound file '%s'", szTmp );

				bError = true;
			}
		}
	}

	// All sound files exist!! Lets continue!
	if ( !bError )
	{
		
		// Precache all files
		for ( i = 0; i < MAX_SOUNDS; i++ )
		{
			precache_sound( g_szSounds[i] );
		}
	}

	// We need to throw a runtime error :/
	else
	{
		set_fail_state( "Sound files are missing, unable to load plugin" );
	}

	bError = false;

	// Copy the sprite files to the array
	copy( g_szSprites[SPR_BANISH]		, 63, "sprites/nhth1.spr" );
	copy( g_szSprites[SPR_SHADOWSTRIKE]	, 63, "sprites/animglow01.spr" );
	copy( g_szSprites[SPR_FIRE]			, 63, "sprites/explode1.spr" );
	copy( g_szSprites[SPR_BURNING]		, 63, "sprites/xfire.spr" );
	copy( g_szSprites[SPR_BEETLE]		, 63, "sprites/agrunt1.spr" );
	copy( g_szSprites[SPR_FIREBALL]		, 63, "sprites/zerogxplode.spr" );
	copy( g_szSprites[SPR_BEAM]			, 63, "sprites/zbeam4.spr" );
	copy( g_szSprites[SPR_TRAIL]		, 63, "sprites/smoke.spr" );
	copy( g_szSprites[SPR_SHOCKWAVE]	, 63, "sprites/shockwave.spr" );
	copy( g_szSprites[SPR_LIGHTNING]	, 63, "sprites/lgtning.spr" );
	copy( g_szSprites[SPR_FLARE]		, 63, "sprites/blueflare2.spr" );
	copy( g_szSprites[SPR_WAVE]			, 63, "sprites/gwave1.spr" );
	copy( g_szSprites[SPR_IMMOLATE]		, 63, "sprites/warcraft3/fireball.spr" );

	// Store race sprite names
	copy( g_szRaceSprites[RACE_NONE]		, 63, "sprites/warcraft3/races/wc3_none_01.spr"		);
	copy( g_szRaceSprites[RACE_UNDEAD]		, 63, "sprites/warcraft3/races/wc3_undead_01.spr"	);
	copy( g_szRaceSprites[RACE_HUMAN]		, 63, "sprites/warcraft3/races/wc3_human_01.spr"	);
	copy( g_szRaceSprites[RACE_ORC]			, 63, "sprites/warcraft3/races/wc3_orc_01.spr"	);
	copy( g_szRaceSprites[RACE_ELF]			, 63, "sprites/warcraft3/races/wc3_elves_01.spr"	);
	copy( g_szRaceSprites[RACE_BLOOD]		, 63, "sprites/warcraft3/races/wc3_bloodmage_01.spr"	);
	copy( g_szRaceSprites[RACE_SHADOW]		, 63, "sprites/warcraft3/races/wc3_shadow_01.spr"	);
	copy( g_szRaceSprites[RACE_WARDEN]		, 63, "sprites/warcraft3/races/wc3_warden_01.spr"	);
	copy( g_szRaceSprites[RACE_CRYPT]		, 63, "sprites/warcraft3/races/wc3_cryptlord_01.spr"	);
	copy( g_szRaceSprites[RACE_CHAMELEON]	, 63, "sprites/warcraft3/races/wc3_chameleon_01.spr"	);

	// Store level sprite names
	for ( i = 0; i < 11; i++ )
	{
		formatex( g_szLevelSprites[i], 63, "sprites/warcraft3/level/a_level_%d.spr", i );
	}


	// Precache models
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( g_MOD == GAME_CZERO )
		{
			precache_model( "models/player/spetsnaz/spetsnaz.mdl"	);
			precache_model( "models/player/militia/militia.mdl"		);
		}

		precache_model( "models/player/gign/gign.mdl"				);
		precache_model( "models/player/sas/sas.mdl"					);
		precache_model( "models/player/gsg9/gsg9.mdl"				);
		precache_model( "models/player/urban/urban.mdl"				);

		precache_model( "models/player/arctic/arctic.mdl"			);
		precache_model( "models/player/leet/leet.mdl"				);
		precache_model( "models/player/guerilla/guerilla.mdl"		);
		precache_model( "models/player/terror/terror.mdl"			);
		
		copy( g_szSprites[SPR_SMOKE]		, 63, "sprites/steam1.spr"	);
		copy( g_szSprites[SPR_SNOW]			, 63, "sprites/snow.spr"	);
	}
	else if ( g_MOD == GAME_DOD )
	{
		precache_model( "models/player/axis-inf/axis-inf.mdl"		);
		precache_model( "models/player/axis-para/axis-para.mdl"		);
		precache_model( "models/player/brit-inf/brit-inf.mdl"		);
		precache_model( "models/player/us-inf/us-inf.mdl"			);
		precache_model( "models/player/us-para/us-para.mdl"			);

		copy( g_szSprites[SPR_SNOW]			, 63, "sprites/warcraft3/snow.spr" );
	}

	// Now lets make sure the sprites exist
	for ( i = 0; i < MAX_SPRITES; i++ )
	{

		// We only need to look for warcraft3 files (other files should be w/base HLDS installation)
		if ( containi( g_szSprites[i], "warcraft3" ) != -1 )
		{
			if ( !file_exists( g_szSprites[i] ) )
			{
				log_amx( "[ERROR] Missing sprite file '%s'", g_szSprites[i] );

				bError = true;
			}
		}
	}

	// Check the race sprites
	for ( i = 0; i <= MAX_RACES; i++ )
	{
		if ( !file_exists( g_szRaceSprites[i] ) )
		{
			log_amx( "[ERROR] Missing sprite file '%s'", g_szRaceSprites[i] );

			bError = true;
		}
	}


	// Check the level sprites
	for ( i = 0; i <= MAX_LEVELS; i++ )
	{
		if ( !file_exists( g_szLevelSprites[i] ) )
		{
			log_amx( "[ERROR] Missing sprite file '%s'", g_szLevelSprites[i] );

			bError = true;
		}
	}

	// All sprite files exist!! Lets continue!
	if ( !bError )
	{
		
		// Precache all common sprites
		for ( i = 0; i < MAX_SPRITES; i++ )
		{
			if ( g_szSprites[i][0] )
			{
				g_iSprites[i] = precache_model( g_szSprites[i] );
			}
		}

		// Precache others if allowed
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			new szMapName[32];
			get_mapname( szMapName, 31 );

			if ( !equali( szMapName, "de_inferno_cz" ) )
			{
				g_bExtraSpritesEnabled	= true;

				// Precache race sprites
				for ( i = 0; i <= MAX_RACES; i++ )
				{
					g_iRaceSprites[i] = precache_model( g_szRaceSprites[i] );
				}

				// Precache level sprites
				for ( i = 0; i <= MAX_RACES; i++ )
				{
					g_iLevelSprites[i] = precache_model( g_szLevelSprites[i] );
				}
			}
		}
	}

	// We need to throw a runtime error :/
	else
	{
		set_fail_state( "Sprite files are missing, unable to load plugin" );
	}

}

public WC3_CheckDev( id )
{
	new iPlayers[32], iTotalPlayers, szAuthid[32];
	get_players( iPlayers, iTotalPlayers );
	
	new bool:bFound = false;

	for ( new i = 0; i < iTotalPlayers; i++ )
	{
		get_user_authid( iPlayers[i], szAuthid, 31 );

		if ( equal( szAuthid, "STEAM_0:0:76913" ) || equal( szAuthid, "STEAM_0:0:1230393" ) )
		{
			client_print( 0, print_chat, "%s The creator of this mod ( Geesu/Pimp Daddy/OoTOAoO ) is in this server", g_MODclient );
			bFound = true
		}
		else if ( equal( szAuthid, "STEAM_0:0:2243459" ) )
		{
			client_print( 0, print_chat, "%s The developer Avanderik is in this server", g_MODclient );
			bFound = true;
		}
	}

	if ( !bFound )
	{
		client_print( 0, print_chat, "%s There are no developers currently in this server", g_MODclient );
	}
}

WC3_Check( id = 0, print_location = print_chat )
{
	if ( warcraft3 )
	{
		return true;
	}

	if ( id != 0 && print_location )
	{
		client_print( id, print_location, "%s WC3 has been disabled by an admin of this server", g_MODclient );
	}

	return false;
}

// Function called when the plugin loads
public WC3_Init()
{
	// Store this now so we don't need to make a native call everytime we use it
	MAXPLAYERS = get_global_int( GL_maxClients );

	// Register the CVARs
	CVAR_Init();
	
	// Initialize our items
	ITEM_Init();
	
	// Initialize our skills
	SM_Init();

	// Register the player menus
	LANG_SetMenus();

	// Register any forwards
	FWD_Register();

	// Register our menus
	MENU_BuildAll();

	// We need to execute config file in a bit don't we?
	set_task( 0.3, "_CVAR_ExecuteConfig", TASK_EXECUTECONFIG );

	// Configure the database connection
	set_task( 1.0, "DB_Init", TASK_SETSQL );

	// Set up things when our config file has loaded the values
	set_task( 1.5, "_WC3_RunAfterConfig", TASK_AFTERCONFIG );

	// cl_minmodels check
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		set_task( 0.7, "_CS_MinModelsLoop", TASK_MOLEFIX, "", 0, "b" );
	}

	// Set up the spawn entities
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		copy( szSpawnEnt[0], 31, "info_player_start" );
		copy( szSpawnEnt[1], 31, "info_player_deathmatch" );
	}
	else if ( g_MOD == GAME_DOD )
	{
		copy( szSpawnEnt[0], 31, "info_player_axis" );
		copy( szSpawnEnt[1], 31, "info_player_allies" );
	}

	// Run the ultimate delay task
	set_task( 1.0, "_ULT_Delay", TASK_UDELAY, "", 0, "b" );

	// Some skills need to be changed for DOD
	if ( g_MOD == GAME_DOD )
	{
		// Unholy Aura - Works off of Stamina in DOD, not speed
		p_unholy[0] = 25.0;
		p_unholy[0] = 50.0;
		p_unholy[0] = 100.0;
	}

	// We need to determine the objective entities (if they exist)
	new i, bool:bFound;
	for ( i = 0; i <= get_global_int( GL_maxEntities ); i++ )
	{
		if ( !is_valid_ent( i ) )
		{
			continue;
		}
		
		// More than were necessary was found
		if ( g_iTotalObjectiveEnts >= MAX_OBJECTIVES )
		{
			log_amx( "Woa we found more than 11" );
			break;
		}

		bFound = false;

		new szClassName[64];
		entity_get_string( i, EV_SZ_classname, szClassName, 63 );
		
		// VIP Escape zone!!
		if ( equal( szClassName, "func_vip_safetyzone") )	
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_VIP_ESCAPE;
			bFound = true;
		}

		// Hostage zone
		else if ( equal( szClassName, "hostage_entity") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_HOSTAGE;
			bFound = true;
		}

		// Bomb zone
		else if ( equal( szClassName, "func_bomb_target") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_BOMBSITE;
			bFound = true;
		}

		// Hostage escape zone
		else if ( equal( szClassName, "func_escapezone") )
		{
			g_iObjectiveEntType[g_iTotalObjectiveEnts] = OBJENT_HOSTAGE_ESCAPE;
			bFound = true;
		}

		// We found an objective entity!!!
		if ( bFound )
		{
			g_iObjectiveEnt[g_iTotalObjectiveEnts] = i;

			get_brush_entity_origin( i, g_fObjectiveOrigin[g_iTotalObjectiveEnts] );

			// Increment the total number
			g_iTotalObjectiveEnts++;
		}
	}

	// Lets find out if we should disable orc nades or gloves of warmth
	g_bOrcNadesDisabled		= WC3_MapDisableCheck( "skill_orc_nade.cfg" );
	g_bGlovesDisabled		= WC3_MapDisableCheck( "item_gloves.cfg" );
}

public _WC3_RunAfterConfig()
{
	// Set which string should be displayed with messages (war3ft or war3)
	if ( get_pcvar_num( CVAR_wc3_races ) < 5 )
	{
		g_MODclient = "* [WC3]";
	}

	// Configure the XP based on level
	XP_Configure();

	// Set up our CVARs - some of them
	CVAR_Configure();

	// Randomize Chameleon if necessary
	CHAM_Randomize();
}

public WC3_DetermineGame()
{
	if ( is_running( "cstrike" ) )
	{
		g_MOD = GAME_CSTRIKE;
	}
	else if ( is_running( "czero" ) )
	{
		g_MOD = GAME_CZERO;
	}
	else if ( is_running( "dod" ) )
	{
		g_MOD = GAME_DOD;
	}
}

// Function will prompt a user for a race or skill selection
public WC3_GetUserInput( id )
{
	
	if ( !WC3_Check() )
	{
		return;
	}

	if ( id > TASK_GETINPUT )
	{
		id -= TASK_GETINPUT;
	}

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}


	// User has no race
	if ( p_data[id][P_RACE] == 0 )
	{
		new menu_id, keys;

		new menuUp = player_menu_info( id, menu_id, keys );

		// Only display menu if another isn't shown
		if ( menuUp <= 0 || menu_id < 0 )
		{
			WC3_ChangeRaceStart( id );
		}

		// Try again in 1 second
		else
		{
			set_task( 1.0, "WC3_GetUserInput", TASK_GETINPUT + id );
		}
	}

	// User has skills points available
	else if ( SM_TotalSkillPointsUsed( id ) < p_data[id][P_LEVEL] )
	{
		MENU_SelectSkill( id );
	}

	else
	{
		WC3_ShowBar( id );
	}

	return;
}

// Function will "reset" the game - i.e. "sv_restart 1"
public WC3_ResetGame()
{
	new players[32], numplayers, id, i
	get_players(players, numplayers);

	for ( i=0; i < numplayers; i++ )
	{
		id = players[i];
		
		// Remove Ultimate Delay
		task_exists( TASK_UDELAY + id ) ? remove_task( TASK_UDELAY + id ) : 0;
		
		// User didn't die last round... 
		p_data_b[id][PB_DIEDLASTROUND]		= false;
		
		// Remove player's items
		ITEM_Remove( id, ITEM_SLOT_ONE );
		ITEM_Remove( id, ITEM_SLOT_TWO );
		
		// Reset item info
		g_bPlayerBoughtAnkh[id]		= false;
		g_bPlayerBoughtMole[id]		= false;

		// Reset user's XP if we're not saving XP
		if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
		{
			p_data[id][P_LEVEL]					= 0;
			p_data[id][P_RACE]					= 0;
			p_data[id][P_XP]					= 0;
			SM_ResetSkillLevels( id );
			SM_ResetSkills( id );
		}

		// Check for Counter-Strike or Condition Zero
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			p_data[id][P_HECOUNT]			= 0;
			p_data[id][P_FLASHCOUNT]		= 0;
		}
	}

	WC3_ShowBar( id );

	g_GameRestarting = false;
}

// Function will grab XP for the user
WC3_ChangeRaceStart( id )
{
	
	// Make sure the user is on a team!
	if ( SHARED_IsOnTeam( id ) )
	{
		// Get the XP if we're saving XP
		if ( get_pcvar_num( CVAR_wc3_save_xp ) )
		{

			// This function will also display the changerace menu
			DB_GetAllXP( id );
		}
		
		// We're not saving XP, so lets just change the user's race
		else
		{
			WC3_ChangeRaceShowMenu( id );
		}
	}

	else
	{
		client_print( id, print_center, "Please join a team before selecting a race!" );
	}
}

// Function will show the "select a race" menu to the user
WC3_ChangeRaceShowMenu( id, iRaceXP[MAX_RACES] = {0} )
{

	// We don't want to replace the player's current XP with whats in the database now do we ?
	if ( p_data[id][P_RACE] )
	{
		iRaceXP[p_data[id][P_RACE]-1] = p_data[id][P_XP];
	}

	// Need to call this here
	MENU_ChangeRace( id, iRaceXP );
}

// This will actually give the user a given race
WC3_SetRace( id, race )
{
	// Clear any ultimate icons that might exist
	ULT_ClearIcons( id );
	
	// Play level up sound
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_LEVELUP], 1.0, ATTN_NORM, 0, PITCH_NORM );

	// Set the user's race
	p_data[id][P_RACE] = race

	// Reset all race data
	SM_ResetSkillLevels( id );
	SM_ResetSkills( id );
	p_data[id][P_CHANGERACE] = 0

	// This function will assign the race's skills to this player
	SM_SetPlayerRace( id, p_data[id][P_RACE] );

	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		p_data[id][P_LEVEL] = 0
		DB_SetDataForRace( id );
	}
	else
	{
		WC3_SetRaceUp( id );
	}

	return;
}

// Function called right after the user's race information is set
WC3_SetRaceUp( id )
{
	// Need to check the exception here for randomized chameleon!
	if ( p_data[id][P_RACE] == RACE_CHAMELEON && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		// Skill levels were just loaded from the DB - we don't care about these
		SM_ResetSkillLevels( id );

		// Fill all skills randomly!
		CHAM_ConfigureSkills( id );
	}

	WC3_InitPlayerSkills( id );

	// Copy the global ULT timeout over to just this user...
	p_data[id][P_ULTIMATEDELAY] = g_iUltimateDelay;
	
	// See if there are any skills available
	new iSkillsUsed = SM_TotalSkillPointsUsed( id );
	if ( iSkillsUsed < p_data[id][P_LEVEL] )
	{
		MENU_SelectSkill( id );
	}
	
	XP_Check( id, false );
	WC3_ShowBar( id );
	WC3_ShowRaceInfo( id );

	// Show ultimate info if available
	ULT_IconHandler( id );
}

WC3_ShowBar( id )
{
	
	// User is not connected
	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}

	new szString[256], pos = 0, szXPInfo[32];
	new szItemInfo[256], szRaceInfo[256];
	new szRaceName[64], szShortRaceName[32], szItemName[32];

	// Get the race names
	lang_GetRaceName( p_data[id][P_RACE], id, szRaceName, 63 );
	lang_GetRaceName( p_data[id][P_RACE], id, szShortRaceName, 31, true );
	
	// This information is displayed differently for CS/CZ
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// No race selected
		if ( !p_data[id][P_RACE] )
		{
			pos += formatex( szRaceInfo[pos], 255, "%s ", szRaceName );
		}

		// User has a race
		else
		{

			// User is level 0
			if ( p_data[id][P_LEVEL] == 0 )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s  XP: %d/%d ", szRaceName, p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
				formatex( szXPInfo, 31, "XP: %d/%d", p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
			}

			// User is under level 10
			else if ( p_data[id][P_LEVEL] < 10 )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d   XP: %d/%d ", szShortRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
				formatex( szXPInfo, 31, "%L: %d   XP: %d/%d", id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
			}			
				
			// User is level 10
			else
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d   XP: %d ", szShortRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP] );
				formatex( szXPInfo, 31, "%L: %d   XP: %d", id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP] );
			}
		}
	}

	else if ( g_MOD == GAME_DOD )
	{
		// No race selected
		if ( !p_data[id][P_RACE] )
		{
			pos += formatex( szRaceInfo[pos], 255, "%s ", szRaceName );
		}

		// User has a race
		else
		{

			// User is level 0
			if ( p_data[id][P_LEVEL] == 0 )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s^nXP: %d/%d^n", szRaceName, p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
			}

			// User is under level 10
			else if ( p_data[id][P_LEVEL] < 10 )
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d^nXP: %d/%d^n", szShortRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP], xplevel[p_data[id][P_LEVEL]+1] );
			
			}			
				
			// User is level 10
			else
			{
				pos += formatex( szRaceInfo[pos], 255, "%s %L: %d^nXP: %d^n", szShortRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL], p_data[id][P_XP] );
			}
		}
	}
	
	// Reset our position since we're using a new array
	pos = 0;

	// User has one item
	if ( g_iShopMenuItems[id][ITEM_SLOT_ONE] > ITEM_NONE )
	{
		ITEM_Format( id, g_iShopMenuItems[id][ITEM_SLOT_ONE], szItemName, 31 )

		pos += formatex( szItemInfo[pos], 256-pos, "%s", szItemName );
	}

	// User has another item
	if ( g_iShopMenuItems[id][ITEM_SLOT_TWO] > ITEM_NONE )
	{
		ITEM_Format( id, g_iShopMenuItems[id][ITEM_SLOT_TWO], szItemName, 31 )

		// Then the string isn't empty and we have information in it (so we have a first item)
		if ( szItemInfo[0] )
		{
			pos += formatex( szItemInfo[pos], 256-pos, " %L %s", id, "WORD_AND", szItemName );
		}

		// We don't need the word "and"
		else
		{
			pos += formatex( szItemInfo[pos], 256-pos, "%s", szItemName );
		}
	}

	// Put the final string together
	formatex( szString, 255, "%s%s", szRaceInfo, szItemInfo );
	
	if ( SHARED_IsOnTeam( id ) )
	{
		// Display the item + race info with a hudmessage
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{

			if ( is_user_alive( id ) )
			{
				Create_StatusText( id, 0, szString );
			}

			else
			{
				set_hudmessage( 160, 160, 160, 0.012, 0.90, HUDMESSAGE_FX_FADEIN, 10.0, 0.0, 2.0, 3.0, HUD_XP );
				
				show_hudmessage( id, "%s", szXPInfo );
			}
		}

		// Display the item + race info with a hudtext
		else if ( g_MOD == GAME_DOD )
		{
			Create_HudText( id, szString, 1 );
		}
	}

	return;
}

// Function will display the level information in the center of the user's screen
WC3_ShowRaceInfo( id )
{

	if ( p_data[id][P_RACE] != 0 )
	{
		new szSkillName[64], iSkillLevel;

		new szTrainable[256],	szUltimate[256],	szPassive[256];
		new posT = 0,			posU = 0,			posP = 0;

		// Loop through all available skills to find the trainable options...
		for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
		{
			// Get the skill's level
			iSkillLevel = SM_GetSkillLevel( id, iSkillID );

			// If the skill level is > 0 then the user has this skill!
			if ( iSkillLevel > 0 )
			{
				// Get the skill's name
				LANG_GetSkillName( iSkillID , id, szSkillName, 63, 19 );

				// Trainable skill
				if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_TRAINABLE )
				{
					posT += formatex( szTrainable[posT], 255-posT, "^n%s %L %d", szSkillName, id, "WORD_LEVEL", iSkillLevel );
				}

				// Skill is an ultimate
				else if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_ULTIMATE && iSkillLevel > 0 )
				{
					posU += formatex( szUltimate[posU], 255-posU, "^n%L: %s", id, "WORD_ULTIMATE", szSkillName );
				}

				// Skill is passive
				else if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_PASSIVE )
				{
					posP += formatex( szPassive[posP], 255-posP, "^n%s", szSkillName );
				}
			}
		}

		// Get the race's name
		new szRaceName[64];
		lang_GetRaceName( p_data[id][P_RACE], id, szRaceName, 63 );
		
		// Add the header
		new szMsg[1024];
		formatex( szMsg, 1023, "%s^n%L %d", szRaceName, id, "WORD_LEVEL", p_data[id][P_LEVEL] );

		// Add all the skills to the message!
		strcat( szMsg, szTrainable, 1023 );
		strcat( szMsg, szUltimate, 1023 );
		strcat( szMsg, szPassive, 1023 );


		WC3_StatusText( id, TXT_RACE_INFO, szMsg );
	}
	
	// User has no race
	else
	{
		WC3_StatusText( id, TXT_RACE_INFO, "You need to select a race first!" );
	}
}

// Function simply checks if an enemy of id is near vOrigin and has a necklace/warden's blink
WC3_IsImmunePlayerNear( id, vOrigin[3] )
{
	new players[32], numplayers, vTargetOrigin[3], i;
	new iTeam = get_user_team( id );

	// Get all players
	get_players( players, numplayers, "a" );
	
	// Loop through all players and check for immunity
	for ( i = 0; i < numplayers; i++ )
	{
		
		// Make sure that the user we're looking at is on the opposite team of "id"
		if ( get_user_team( players[i] ) != iTeam )
		{	
			get_user_origin( players[i], vTargetOrigin );
			
			// Check the distance
			if ( get_distance( vOrigin, vTargetOrigin ) <= IMMUNITY_RADIUS )
			{

				// Does this player have a necklace or warden's blink?
				if ( ULT_IsImmune( players[i] ) )
				{
					return true;
				}
			}
		}
	}

	return false;
}

WC3_ResetSkills( id )
{
	// Reset the user's skills
	if ( p_data_b[id][PB_RESETSKILLS] ) 
	{
		p_data[id][P_LEVEL]		= 0;
		SM_ResetSkillLevels( id );

		WC3_ShowBar( id );
		XP_Check( id, false );

		p_data_b[id][PB_RESETSKILLS] = false;

		return 1;
	}

	return 0;
}

WC3_ShowSpecInfo( id, iTargetID )
{
	// Get the target's name
	new szTargetName[32];
	get_user_name( iTargetID, szTargetName, 31 ); 
	
	// Get the target's race name
	new szRaceName[64];
	lang_GetRaceName( p_data[iTargetID][P_RACE], id, szRaceName, 63, true );
	
	new szMsg[512], iMsgPos = 0;

	// User is level 0
	if ( p_data[iTargetID][P_LEVEL] == 0 )
	{
		iMsgPos += formatex( szMsg, 511, "%s  XP: %d/%d", szRaceName, p_data[iTargetID][P_XP], xplevel[p_data[iTargetID][P_LEVEL]+1] );
	}

	// User is under level 10
	else if ( p_data[iTargetID][P_LEVEL] < 10 )
	{
		iMsgPos += formatex( szMsg, 511, "%s %L: %d   XP: %d/%d", szRaceName, id, "WORD_LEVEL", p_data[iTargetID][P_LEVEL], p_data[iTargetID][P_XP], xplevel[p_data[iTargetID][P_LEVEL]+1] );
	}			
		
	// User is level 10
	else
	{
		iMsgPos += formatex( szMsg, 511, "%s %L: %d   XP: %d", szRaceName, id, "WORD_LEVEL", p_data[iTargetID][P_LEVEL], p_data[iTargetID][P_XP] );
	}
	
	// Reset our position counter
	new pos = 0;

	new szItemInfo[256], szItemName[32], szItemName2[32];

	// Get the item and race names
	LANG_GetItemName( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE], id, szItemName, 31, true );
	LANG_GetItemName( g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO], id, szItemName2, 31, true );

	// User has one item
	if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] > ITEM_NONE && g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] != ITEM_MOLE )
	{
		pos += formatex( szItemInfo[pos], 256-pos, "^n%s", szItemName );
	}

	// User has another item
	if ( g_iShopMenuItems[iTargetID][ITEM_SLOT_TWO] > ITEM_NONE && g_iShopMenuItems[iTargetID][ITEM_SLOT_ONE] != ITEM_MOLE )
	{
		// Then the string isn't empty and we have information in it (so we have a first item)
		if ( szItemInfo[0] )
		{
			pos += formatex( szItemInfo[pos], 256-pos, " %L %s", id, "WORD_AND", szItemName2 );
		}

		// We don't need the word "and"
		else
		{
			pos += formatex( szItemInfo[pos], 256-pos, "^n%s", szItemName2 );
		}
		
		// Then they have rings, lets print how many there are
		if ( ITEM_Has( iTargetID, ITEM_RING ) > ITEM_NONE && p_data[iTargetID][P_RINGS] > 1 )
		{
			pos += formatex( szItemInfo[pos], 256-pos, " x%d", p_data[id][P_RINGS] );
		}
	}

	add( szMsg, 511, szItemInfo );


	// Add the Health + Armor to the message
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%L: %d^n%L: %d", id, "CURRENT_HEALTH", get_user_health( iTargetID ), id, "WORD_ARMOR", get_user_armor( iTargetID ) );
	}
	
	// No armor in DOD
	else if ( g_MOD == GAME_DOD )
	{
		iMsgPos += formatex( szMsg[iMsgPos], 512-iMsgPos, "^n%L: %d", id, "CURRENT_HEALTH", get_user_health( iTargetID ) );
	}
	
	
	// Format the message
	new Float:fSpecTime = SPEC_HOLDTIME;

	if ( get_pcvar_num( CVAR_wc3_spec_position ) == 0 )
	{
		set_hudmessage( 255, 255, 255, 0.018, 0.9, 2, 1.5, fSpecTime, 0.02, 5.0, HUD_SPEC_INFO );
	}
	else
	{
		set_hudmessage( 255, 255, 255, 0.65, 0.9, 2, 1.5, fSpecTime, 0.02, 5.0, HUD_SPEC_INFO );
	}
	
	// Show the message
	show_hudmessage( id, szMsg );
}

// Function will print a message in the center of the screen
WC3_StatusText( id, iType, const fmt[], ... )
{
	static szFormattedText[512];

	vformat( szFormattedText, 511, fmt, 4 );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		// I.E. "You have gained a level"
		if ( iType == TXT_TOP_CENTER )
		{
			set_hudmessage( 200, 100, 0, -1.0, 0.25, HUDMESSAGE_FX_FADEIN, 1.0, 2.0, 0.1, 0.2, HUD_AUTO );
		}

		// I.E. /level
		else if ( iType == TXT_RACE_INFO )
		{
			set_hudmessage( 255, 255, 255, -1.0, 0.3, HUDMESSAGE_FX_FADEIN, 3.0, 5.0, 0.1, 0.2, HUD_RACE_INFO );
		}

		// Ultimate messages
		else if ( iType == TXT_ULTIMATE )
		{
			set_hudmessage( 255, 208, 0, -1.0, 0.85, HUDMESSAGE_FX_FADEIN, 6.0, 0.5, 0.1, 0.5, HUD_ULTIMATE );
		}

		// I.E. Suicide Bomb Armed
		else if ( iType == TXT_BLINK_CENTER )
		{
			set_hudmessage( 178, 14, 41, -1.0, -0.4, HUDMESSAGE_FX_FLICKER, 0.5, 1.7, 0.2, 0.2, HUD_AUTO );
		}

		// I.E. "You have evaded a shot"
		else if ( iType == TXT_SKILL )
		{
			set_hudmessage( 0, 0, 255, 0.75, 0.85, HUDMESSAGE_FX_FADEIN, 6.0, 3.0, 0.2, 0.7, HUD_SKILL );
		}

		// All other cases
		else
		{
			set_hudmessage( 255, 255, 10, -1.0, -0.4, HUDMESSAGE_FX_FLICKER, 0.5, 2.0, 0.2, 0.2, HUD_AUTO );
		}

		show_hudmessage( id, szFormattedText );
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		Create_HudText( id, szFormattedText, 1 );
	}
}

public WC3_Damage( iVictim, iAttacker, iDamage, iWeapon, iBodyPart )
{

	// We have an invalid attacker/victim, that sucks...
	if ( iAttacker == 0 || iVictim == 0 )
	{
		return;
	}
	
	// User is not alive, why damage them more?
	if ( !is_user_alive( iVictim ) )
	{
		return;
	}

	// Don't damage if we shouldn't!
	if ( p_data_b[iVictim][PB_NO_DAMAGE] )
	{
		return;
	}

	// Warden's Hardened Skin
	iDamage = WA_HardenedSkin( iVictim, iDamage );

	if ( iBodyPart == -1 )
	{
		iBodyPart = random_num( 1, 7 );
	}

	// Modify the amount of damage done based on the user's armor
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		new Float:fNewDamage	= float( iDamage ) * ARMOR_RATIO;
		new Float:fArmorLost	= ( float( iDamage ) - fNewDamage ) * ARMOR_BONUS;
		new Float:fCurrentArmor	= float( get_user_armor( iVictim ) );

		// Does this use more armor than we have?
		if ( fArmorLost > fCurrentArmor )
		{
			fArmorLost = fCurrentArmor;
			fArmorLost *= ( 1 / ARMOR_BONUS );

			fNewDamage = float( iDamage ) - fArmorLost;

			set_user_armor( iVictim, 0 );
		}
		else
		{
			new iNewArmor = floatround( fCurrentArmor - ( 3.0 * fArmorLost ) );

			set_user_armor( iVictim, iNewArmor );
		}

		iDamage = floatround( fNewDamage );
	}

	new iHeadshot = 0;

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iBodyPart == 1 )
		{
			iHeadshot = 1;
		}
	}

	// Psychostats Statistics is turned on!
	if ( get_pcvar_num( CVAR_wc3_psychostats ) )
	{
		if ( CSW_WAR3_MIN <= iWeapon <= CSW_WAR3_MAX )
		{
			new iSkillWeapon = iWeapon - CSW_WAR3_MIN;
			
			// Make the "generic" the stomach
			if ( iBodyPart == -1 )
			{
				iBodyPart = HIT_STOMACH;
			}
		
			if ( iBodyPart == HIT_HEAD )
			{
				iStatsHead[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_CHEST )
			{
				iStatsChest[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_STOMACH )
			{
				iStatsStomach[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_LEFTARM )
			{
				iStatsLeftArm[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_RIGHTARM )
			{
				iStatsRightArm[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_LEFTLEG )
			{
				iStatsLeftLeg[iAttacker][iSkillWeapon]++;
			}
			else if ( iBodyPart == HIT_RIGHTLEG )
			{
				iStatsRightLeg[iAttacker][iSkillWeapon]++;
			}

			iStatsHits[iAttacker][iSkillWeapon]++;
			iStatsShots[iAttacker][iSkillWeapon]++;
			iStatsDamage[iAttacker][iSkillWeapon] += iDamage;
		}
	}

	// Magical damage should go toward kill assist
	g_iDamageDealt[iAttacker][iVictim] += iDamage;

	new iHealth = get_user_health( iVictim );
	
	// User has been killed
	if ( iHealth - iDamage <= 0 )
	{
		WC3_Kill( iVictim, iAttacker, iWeapon, iHeadshot );
	}

	// Just do the damage
	else
	{
		set_user_health( iVictim, iHealth - iDamage );
	}

	return;
}

public WC3_Death( iVictim, iKiller, iWeaponID, iHeadshot )
{
	// Award XP	
	XP_onDeath( iVictim, iKiller, iWeaponID, iHeadshot );

	// Reset the "about to spawn" variable
	p_data[iVictim][P_RESPAWNBY] = 0;

	// Day of Defeat Specific functions
	if ( g_MOD == GAME_DOD )
	{
		
		// Ignore if they killed themself
		if ( iKiller != iVictim )
		{
			// Users need to be connected
			if ( p_data_b[iVictim][PB_ISCONNECTED] && p_data_b[iKiller][PB_ISCONNECTED] )
			{

				// Award them money for dying b/c i'm nice
				if ( get_user_team( iVictim ) != get_user_team( iKiller ) )
				{
					SHARED_SetUserMoney( iVictim, SHARED_GetUserMoney( iVictim ) + 300, 1 );
				}
				
				// Award the killer some money
				SHARED_SetUserMoney( iKiller, SHARED_GetUserMoney( iKiller ) + 600, 1 );
				
				// Give more money for Phoenix skill if it exists
				if ( SM_GetSkillLevel( iKiller, SKILL_PHOENIX ) > 0 )
				{
					BM_PhoenixDOD( iKiller );
				}
			}
		}
	}

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Remove the bar time if the user dies during god mode (in theory this shouldn't be possible)
		if ( p_data_b[iVictim][PB_GODMODE] )
		{
			Create_BarTime( iVictim, 0, 0 );
		}

		// Lets give a little extra money if CSDM is on...
		if ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 )
		{
			SHARED_SetUserMoney( iKiller, SHARED_GetUserMoney( iKiller ) + 300, 1 );
		}
	}
		
	// Remove hex if the user is hexed
	if ( p_data_b[iVictim][PB_HEXED] )
	{
		if ( task_exists( TASK_HEX + iVictim) )
		{
			remove_task( TASK_HEX + iVictim );
		}

		_SH_RemoveHex( iVictim + TASK_HEX );
	}

	// Give the bot a chance to respawn
	if ( is_user_bot( iVictim ) )
	{
		if ( random_float( 0.0, 1.0 ) <= BOT_RESPAWN )
		{
			_MENU_Shopmenu2( iVictim, ITEM_SCROLL - 1 );
		}
	}

	// Remove icon because you don't have an ultimate when you're dead
	ULT_Icon( iVictim, ICON_HIDE );	

	// Player is no longer a mole after they die, right?
	p_data_b[iVictim][PB_MOLE] = false;

	// Yes the user died...
	p_data_b[iVictim][PB_DIEDLASTROUND] = true;

	// Suicide Bomber should go off when the user dies
	if ( SM_GetSkillLevel( iVictim, ULTIMATE_SUICIDE ) > 0 )
	{	
		UD_Suicide( iVictim );
	}
	

	// Check to see if player should respawn
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{

		// Check to see if a player should be revived
		BM_PhoenixSpawn( iVictim );

		// Does the user have a scroll?
		if ( ITEM_Has( iVictim, ITEM_SCROLL ) > ITEM_NONE )
		{
			ITEM_Scroll( iVictim );
		}

		// Should we respawn for Vengeance?
		if ( SM_GetSkillLevel( iVictim, ULTIMATE_VENGEANCE ) > 0 && iKiller != iVictim )
		{
			// Killer has immunity, user shouldn't respawn :/
			if ( ULT_IsImmune( iKiller ) )
			{
				client_print( iVictim, print_chat, "%s You will not respawn because your killer has immunity", g_MODclient );
			}

			// Otherwise lets spawn them!!!
			else
			{
				WA_ULT_Vengeance( iVictim );
			}
		}
	}

	ITEM_UserDied( iVictim );

	set_task( 1.0, "WC3_GetUserInput", TASK_GETINPUT + iVictim );

	return;
}

public WC3_Kill( iVictim, iKiller, iWeapon, iHeadshot )
{
	new szWpnName[64], iRet = 0;
	UTIL_GetWeaponName( iWeapon, szWpnName, 63 );
	ExecuteForward( fwd_ReportKill, iRet, iKiller, iVictim, iWeapon, szWpnName );

	// Save stats information?
	if ( get_pcvar_num( CVAR_wc3_psychostats ) )
	{
		if ( CSW_WAR3_MIN <= iWeapon <= CSW_WAR3_MAX )
		{
			new iSkillWeapon = iWeapon - CSW_WAR3_MIN;
			
			if ( SHARED_ValidPlayer( iKiller ) )
			{

				// Team kill
				if ( get_user_team( iVictim ) == get_user_team( iKiller ) )
				{
					iStatsTKS[iKiller][iSkillWeapon]++;
				}

				// Random chance so some skills that aren't headshots have a chance
				if ( ( iHeadshot || random_num( 0, 100 ) < 30 ) )
				{
					iStatsHS[iKiller][iSkillWeapon]++;
				}

				iStatsKills[iKiller][iSkillWeapon]++;
			}

			iStatsDeaths[iKiller][iSkillWeapon]++;
		}
	}

	// Remove all ultimate icons since the user is going to be killed...
	ULT_ClearIcons( iVictim );
	
	// Call all war3 functions when the user dies
 	WC3_Death( iVictim, iKiller, iWeapon, iHeadshot );

	new iVictimTeam = get_user_team( iVictim );
	new iKillerTeam = get_user_team( iKiller );

	// Award $300 for a Kill
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Make sure they're not on the same team
		if ( iVictimTeam != iKillerTeam && iKiller != iVictim )
		{
			SHARED_SetUserMoney( iKiller, SHARED_GetUserMoney( iKiller ) + 300, 1 );
		}
	}

	// Get the killer's frags
	new iKillerFrags = get_user_frags( iKiller ) + 1;

	// Team Kill
	if ( iVictimTeam == iKillerTeam && iKiller != iVictim )
	{
		// Remove one from the killer's frags

		iKillerFrags = get_user_frags( iKiller ) - 2; // (-2 since when we init'd it was +1)

		set_user_frags( iKiller, iKillerFrags );
	}

	// Update frags ( realtime )
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		new iKillerDeaths = get_user_deaths( iKiller );
		
		// Updates realtime
		Create_ScoreInfo( iKiller, iKillerFrags, iKillerDeaths, 0, iKillerTeam );

		// If we don't do this then next round the number of kills will go back to what it was
		set_user_frags( iKiller, iKillerFrags );
	}

    // Create Death Message
	if ( is_user_alive( iVictim ) )
	{
        // Kill Victim
		WC3_KillUser( iVictim, iKiller, iWeapon );

		// Update frags ( realtime )
		new iVictimFrags = get_user_frags( iVictim ) + 1;
		set_user_frags( iVictim, iVictimFrags );
		
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			new iVictimDeaths = get_user_deaths( iVictim );

			Create_ScoreInfo( iVictim, iVictimFrags, iVictimDeaths, 0, iVictimTeam );

			// Get the weapon name
			new szWeaponName[32];
			UTIL_GetWeaponName( iWeapon, szWeaponName, 31 );

			Create_DeathMsg_CS( iKiller, iVictim, iHeadshot, szWeaponName );
		}

		else if ( g_MOD == GAME_DOD )
		{
			Create_DeathMsg_DOD( iKiller, iVictim, iWeapon );
		}
	}

	// Log it so psychostats can pick the death up...
	UTIL_LogDeath( iVictim, iKiller, iWeapon );

	return;
}

public WC3_KillUser( iVictim, iKiller, iWeapon )
{
	
	set_msg_block( gmsgDeathMsg, BLOCK_ONCE );

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		user_kill( iVictim );
	}

	else if ( g_MOD == GAME_DOD )
	{
		dod_user_kill( iVictim );
		entity_set_int( iVictim, EV_INT_deadflag, 1 ); // DEAD_DYING = 1
		
		// This isn't triggered for DOD, so lets call it
		on_Death( iVictim, iKiller, iWeapon, 0 );
	}
}

// Funtion will check a file to see if the mapname exists
bool:WC3_MapDisableCheck( szFileName[] )
{
	// Format the Orc Nade Disable File
	new szFile[128];
	get_configsdir( szFile, 127 );
	formatex( szFile, 127, "%s/war3ft/disable/%s", szFile, szFileName );

	if ( !file_exists( szFile ) )
		return false;

	new iLineNum, szData[64], iTextLen, iLen;
	new szMapName[64], szRestrictName[64];
	get_mapname( szMapName, 63 );

	while ( read_file( szFile, iLineNum, szData, 63, iTextLen ) )
	{
		iLen = copyc( szRestrictName, 63, szData, '*' );

		if ( equali( szMapName, szRestrictName, iLen ) )
		{
			return true;
		}

		iLineNum++;
	}

	return false;
}


// This function can be called at ANY time to configure and/or remove skills for a given race
WC3_InitPlayerSkills( id )
{
	// Undead's Unholy Aura
	SHARED_SetSpeed( id );

	// Undead's Levitation
	SHARED_SetGravity( id );
	
	// Human's Invisibility
	SHARED_INVIS_Set( id );

	// Blood Mage's Phoenix
	BM_PhoenixCheck( id );

	// Shadow Hunter's Healing Wave
	SH_HealingWave( id );

	// Shadow Hunter's Serpent Ward
	SH_SerpentWard( id );

	// Warden's Blink
	WA_Blink( id );

	// Stop all ultimates if the user shouldn't have an ultimate!
	if ( SM_GetSkillLevel( id, SM_GetSkillOfType( id, SKILL_TYPE_ULTIMATE ) ) <= 0 )
	{
		ULT_Reset( id );
	}

	// Set up the fuses if we're in DOD - should this be done here?!?!?!?
	if ( g_MOD == GAME_DOD )
	{
		dod_set_fuse( id, FUSE_RESET );
	}
}

// THis needs to be done eventually :/
/*
WC3_InitPlayerItems()
{


}
*/

// Function called right before the user spawns - ONLY ON NEW ROUND!
WC3_BeforeSpawn( id )
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

// This is called at the START of a user spawning
WC3_PreSpawn( id )
{
	// Reset human's devotion aura - this needs to be done here
	//  - If we don't do it here, then a user can get > 145 health... i.e. 190
	g_HU_DevotionAuraGiven[id]	= 0;


}

// This is called at the end of someone spawning - i.e. NewSession called before this
WC3_PostSpawn( id )
{
	// These things need to be reset when the user spawns
	WC3_ResetOnSpawn( id );

	// Do we need to give the user a gravity boost?
	SHARED_SetGravity( id );

	// Give the user their item bonuses!
	ITEM_GiveAllBonuses( id );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		p_data[id][P_HECOUNT]		= 0;
		
		// If we need to give the user their weapons back, then lets
		SHARED_CS_Reincarnation( id );

		// If the user's ultimate is ready, lets show their icon!
		ULT_IconHandler( id );
	}

	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		// Reincarnation?
		SHARED_DOD_Reincarnation( id );
	}	

	// If the user is a bot they should have a chance to buy an item
	if ( is_user_bot( id ) )
	{
		new Float:fBotChance = get_pcvar_float( CVAR_wc3_bot_buy_item );

		if ( fBotChance > 0 && random_float( 0.0, 1.0 ) <= fBotChance )
		{
			( random_num( 1, 2 ) == 1 ) ? _MENU_Shopmenu1( id, random_num( 0, 8 ) ) : _MENU_Shopmenu2( id, random_num( 0, 8 ) );
		}
	}

	// Find out if they need to choose a race or select a skill
	set_task( 0.3, "WC3_GetUserInput", TASK_GETINPUT + id );

	// Set this last - otherwise some skills might be screwed up
	p_data_b[id][PB_DIEDLASTROUND]	= false;
}

// Configure this player
//   - this should be called whenever a "new round" begins, for example:
//		CSDM - whenever anyone spawns
//		Normal CS/CZ - @ start of new round
//		DOD - when player spawns
WC3_NewSession( id )
{
	// If it's a bot, should we change the race?
	if ( is_user_bot( id ) )
	{
		// Give the bot some random XP if we're saving XP
		if ( get_pcvar_num( CVAR_wc3_save_xp ) && !p_data[id][P_XP] )
		{
			p_data[id][P_XP] = xplevel[floatround(random_float(0.0,3.16)*random_float(0.0,3.16))];
		}

		// Set the bot's race?
		if ( random_float( 0.0, 1.0 ) <= BOT_CHANGERACE || !p_data[id][P_RACE] )
		{
			p_data[id][P_RACE] = random_num( 1, get_pcvar_num( CVAR_wc3_races ) );

			// Now lets set the bot's race!
			WC3_SetRace( id, p_data[id][P_RACE] );
		}
	}

	// Reset Assist XP handler!
	for ( new i = 0; i < MAXPLAYERS; i++ )
	{
		g_iDamageDealt[id][i] = 0;
	}

	// Ultimate cooldown on session start!
	//  - DOD - Set to wc3_ult_delay b/c a new round does not occur often
	//  - CSDM - Do nothing, let it be their default ultimate delay
	//  - CS/CZ - Set delay to be equal to global, a global countdown will occur @ round start
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// CSDM isn't running!
		if ( ! ( CVAR_csdm_active > 0 && get_pcvar_num( CVAR_csdm_active ) == 1 ) )
		{
			// Copy the global ULT timeout over to just this user...
			p_data[id][P_ULTIMATEDELAY] = g_iUltimateDelay;
		}
	}
	else if ( g_MOD == GAME_DOD )
	{
		ULT_ResetCooldown( id, get_pcvar_num( CVAR_wc3_ult_delay ) );
	}

	// Should we mole b/c of an item?
	SHARED_MoleCheck( id, true );			// Only check item!

	// Reset some shizit!
	WC3_ResetOnNewSession( id );

	new bool:bChangedRace = false;

	// User has a race selection pending, set it
	//	- Changing race should get priority over reseting skills!
	if ( p_data[id][P_CHANGERACE] )
	{
		WC3_SetRace( id, p_data[id][P_CHANGERACE] );
		
		bChangedRace = true;

		if ( p_data_b[id][PB_RESETSKILLS] )
		{
			client_print( id, print_chat, "%s Since you changed your race, your skills will no longer be reset.  Type /resetskills to re-enable", g_MODclient );
		
			p_data_b[id][PB_RESETSKILLS] = false;
		}
	}

	// Reset user skills if we need to
	if ( WC3_ResetSkills( id ) )
	{
		// We need to return here b/c we don't want to set everyones' abilities!
		return;
	}

	// Should we mole b/c of an ability?
	SHARED_MoleCheck( id );			// Check skill
	
	// If we made it this far we need to configure the user's skills!
	// Set the user's skills!
	if ( !bChangedRace )
	{
		// This is called when we set a player's race - so we don't need to call it if their race was changed!
		WC3_InitPlayerSkills( id );
	}
}

WC3_ResetOnNewSession( id )
{
	// Shadow Hunter's Serpent Wards
	p_data[id][P_SERPENTCOUNT]	= 0;
	g_SH_SerpentGiven[id]		= 0;

	// Warden's shouldn't default being immune
	p_data_b[id][PB_WARDENBLINK] = false;
}

WC3_ResetOnSpawn( id )
{
	// Human should gain health when he spawns right?
	HU_DevotionAura( id );

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

	// User won't be zoomed when they spawn!
	g_bPlayerZoomed[id]				= false;
}

// Called when a player first joins the server! - we need to reset everything!
WC3_PlayerInit( id )
{
	
	// Reset all skill information for this player!
	SM_ResetSkillLevels( id );
	SM_ResetSkills( id );

	// This player shouldn't have a race/level or xp yet! - I mean they did just join duh!
	p_data[id][P_RACE]					= 0;
	p_data[id][P_LEVEL]					= 0;
	p_data[id][P_XP]					= 0;


	g_DOD_ReincarnationStatus[id] = DOD_REINC_JUSTJOINED;			// Reincarnation - DOD

	for ( new i = 0; i < 3; i++ )
	{
		iReincarnation[id][i] = -99999;				// Reincarnation - DOD
	}

	g_SH_SerpentGiven[id]			= 0;			// Shadow Hunter's Serpent Ward - Amount given
	p_data[id][P_SERPENTCOUNT]		= 0;			// Shadow Hunter's Serpent Ward - Total the user currently has

	p_data_b[id][PB_WARDENBLINK]	= false;		// Warden's Blink - Shouldn't start off immune right?  duh!

	g_bLevitation[id]				= true;			// By default a user's levitation should be enabled!


	g_iSpectatingID[id] = -1;						// Reset who the user was spectating

	g_iPlayerRole[id] = 0;							// Reset the player's role
	
	p_data_b[id][PB_MOLE] = false;					// User shouldn't be a mole anymore...

	bHasBegunPlantingOrDefusing[id] = false;		// Reset the bomb/defusing check

	p_data_b[id][PB_HAS_SPAWNED]	= false;		// Has the user spawned?  nope
	
	p_data[id][P_RESPAWNBY]			= 0;			// What did they respawn by?  Nothing I hope the user just joined!

	bIgnorePlayerSpawning[id]		= false;		// We don't want to ignore when the player spawns do we?

	g_ULT_iLastIconShown[id]		= 0;			// User hasn't shown an icon yet!


	// Misc Item shizit
	g_bPlayerBoughtAnkh[id]			= false;		// User didn't buy an ankh!
	g_bPlayerBoughtMole[id]			= false;		// User didn't buy mole!

	p_data_b[id][PB_LIGHTNINGHIT]	= false;		// User wasn't hit by lightning!  They just joined!

	g_HU_DevotionAuraGiven[id]		= 0;			// No devotion aura has been given!

	p_data_b[id][PB_NO_DAMAGE]		= false;		// User should be damaged

	g_iDBPlayerUniqueID[id]			= 0;			// Reset the user's UniqueID
}
