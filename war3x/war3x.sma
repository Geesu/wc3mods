
///////////////////////////////////// Plugin Information /////

/* - AMXX Mod Script -------------------------------------- //

 *  Warcraft 3: Expansion
 *  (c) Copyright 2003-2006
 *
 *
 *  Development Team:
 *  ^^^^^^^^^^^^^^^^
 *   Ryan       ( Lead Developer )
 *   Geesu      ( aka: Pimp Daddy / OoTOAoO )
 *   Avanderik
 *   Willyumyum
 *
 *
 *  Special Thanks:
 *  ^^^^^^^^^^^^^^
 *   Spacedude  ( for inspiring this great mod idea. Some
 *                original Warcraft 3 Mod effects are still
 *                used today. )
 *   rACEmic    ( original MySQL saving implementation )
 *   Freecode   ( for helping debug v0.99+ crashing errors )
 *

 * - What is WAR3X? --------------------------------------- //

 *  Warcraft 3: Expansion expands upon the ideas of the
 *  original version of Warcraft 3 XP, written by Spacedude.
 *  Almost all of this plugin has been completely re-written
 *  from scratch. This was necessary to accomodate the
 *  changes between the two versions. This is not to
 *  discredit Spacedude in any way, however because without
 *  Spacedude, there would be no Expansion. Cheers.
 *

 * - !!DISCLAIMER!! --------------------------------------- //

 *  You may browse this source, and even modify it for your
 *  server if you would like to, but I will not allow a
 *  public release of any modified war3x source code while
 *  development resumes on the official version of the plugin.
 *
 *  Also, please ask before using (or 'borrowing') any part
 *  of this source code for your own plugin(s). A lot of hard
 *  work went into developing this script, and a simple
 *  credit to the WAR3X development team will usually suffice,
 *  but please, ASK FIRST.
 *
 *  Thank you.
 *  -Ryan
 *

 * ------------------------------------------------- End. - */



/////////////////////////////////// Plugin Configuration /////

// - WAR3X Information ------------------------------------ //

new const WAR3X_PLUGINNAME[] = "Warcraft 3: Expansion";
new const WAR3X_VERSION[]    = "v1.0.1 Official";
new const WAR3X_DATE[]       = "03/21/2006";
new const WAR3X_AUTHORS[]    = "Ryan, Geesu, Avanderik, Willyumyum";

// - Basic Setup ------------------------------------------ //

#pragma tabsize 0

#include "war3x/setup.cfg"

// - Common Includes -------------------------------------- //

#include <amxmodx>
#include <amxmisc>
#include <cstrike>
#include <fun>
#include <engine>
#include <dbi>

// - Inline Inclusions ------------------------------------ //

#include "war3x/globals.inl"

#include "war3x/language/english.inl"
#include "war3x/language.inl"

#include "war3x/include/UTIL_amx.inc"
#include "war3x/include/UTIL_cs.inc"
#include "war3x/include/UTIL_dbi.inc"
#include "war3x/include/UTIL_vexd.inc"
#include "war3x/include/UTIL_war3x.inc"

// Main Plugin Information

#include "war3x/xp.inl"

#include "war3x/shared.inl"

#include "war3x/undead.inl"
#include "war3x/human.inl"
#include "war3x/orc.inl"
#include "war3x/nightelf.inl"

#include "war3x/items.inl"
#include "war3x/ultimates.inl"
#include "war3x/upkeep.inl"
#include "war3x/save.inl"
#include "war3x/menus.inl"

#include "war3x/events.inl"
#include "war3x/commands.inl"
#include "war3x/help.inl"

// ------------------------------------------------- End. - //



///////////////////////////////////////// Core Functions /////


public plugin_precache() {

	// Get configs directory

    static AMXX_DIR[64];
	get_configsdir( AMXX_DIR, 63 );

	formatex( WAR3X_DIR, 63, "%s/war3x", AMXX_DIR );

    // Don't precache if mapname in disabledmaps.txt

    if ( Map_Disabled() )
    {
        g_bMapDisabled = true;

        new szMapName[64];
        get_mapname( szMapName, 63 );

        // Print to console

        new szMessage[256];
        formatex( szMessage, 255, "%L", LANG_SERVER, "CONSOLE_DISABLEDMAP", szMapName, WAR3X_DIR );

        server_print( "%s", szMessage );

        // Log to file

        log_amx( "%s", szMessage );

        return PLUGIN_CONTINUE;
    }

    // Check if current map restricted to short-term

    else if ( Map_Restricted() )
    {
        g_bMapRestricted = true;

        new szMapName[64];
        get_mapname( szMapName, 63 );

        // Print to console

        new szMessage[256];
        formatex( szMessage, 255, "%L", LANG_SERVER, "CONSOLE_SHORTTERMMAP", szMapName, WAR3X_DIR );

        server_print( "%s", szMessage );

        // Log to file

        log_amx( "%s", szMessage );
    }

    WAR3_precache();

    return PLUGIN_CONTINUE;
}


public client_connect( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    WAR3_player_init( id );

    return PLUGIN_CONTINUE;
}


public client_authorized( id ) {

	// Check for steam ID pending

	new szPlayerID[32];
	get_user_authid( id, szPlayerID, 31 );

	// Then the player doesn't have a steam id, lets make them reconnect

	if ( equal( szPlayerID, "STEAM_ID_PENDING" ) )
		client_cmd( id, "reconnect" );

    return PLUGIN_CONTINUE;
}


public client_putinserver( id ) {

    if ( !g_bWar3xEnabled || g_bMapDisabled )
        return PLUGIN_CONTINUE;

    return PLUGIN_CONTINUE;
}


public client_disconnect( id ) {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    g_bPlayerConnected[id] = false;

    if ( g_bMapDisabled )
    {
        if ( g_bDisabledNotify[id] )
            g_bDisabledNotify[id] = false;

        return PLUGIN_CONTINUE;
    }

    // Add save data to queue

    if ( g_bSaveXp && !is_user_bot( id ) && g_PlayerInfo[id][CURRENT_RACE] )
    {
        // Find free slot to save Queue data in

        new bool:bSlotFound, iArrayIndex;

        while ( !bSlotFound && iArrayIndex < TOTAL_QUEUES )
        {
            if ( !g_SaveQueue_ids[iArrayIndex][0] )
            {
                Queue_Add( id, iArrayIndex, g_SaveIds[id] );
                bSlotFound = true;
            }

            iArrayIndex++;
        }
    }

    // Remove all player tasks

    Purge_Death( id );
    Purge_NewRound( id );

    // Remove player info

    Purge_PlayerInfo( id );

    return PLUGIN_CONTINUE;
}


public plugin_init() {

    register_plugin( WAR3X_PLUGINNAME, WAR3X_VERSION, WAR3X_AUTHORS );

    if ( g_bMapDisabled )
    {
        register_event( "ResetHUD", "on_ResetHud_NOWAR3", "b" );
        return PLUGIN_CONTINUE;
    }

    // Client Commands

    register_clcmd( "war3menu", "cmd_War3menu", -1, " -- Displays the Warcraft 3 menu"                               );
    register_clcmd( "shopmenu", "cmd_Shopmenu", -1, " -- Displays the Warcraft 3 items menu"                         );
    register_clcmd( "ultimate", "cmd_Ultimate", -1, " -- Press to cast ultimate and view target skills in spectator" );
    register_clcmd( "dropitem", "cmd_DropItem", -1, " -- Press to drop item on the ground"                           );

    register_concmd( "war3x",      "WAR3_toggle",   ADMIN_RCON, " -- Enables/Disables the Warcraft 3 Plugin" );
    register_concmd( "war3x_help", "HELP_generate", ADMIN_RCON, " -- Generates War3x help files"             );

    register_concmd( "war3x_setlevel", "Cmd_SetLevel" );    // Alter level
    register_concmd( "war3x_setxp",    "Cmd_SetXp"    );    // Alter XP


    // Client commands ( events )

    register_clcmd( "kevlar",      "on_BuyArmor" );         // Buy armor
    register_clcmd( "assaultsuit", "on_BuyArmor" );         // Buy armor

    register_clcmd( "say",      "Cmd_Say" );                // Check for war3x say commands
    register_clcmd( "say_team", "Cmd_Say" );

    register_clcmd( "drop", "Cmd_Drop" );                   // Drop weapon


    // Menu commands

    register_menucmd( register_menuid( MENU_WAR3MENU_TITLE ),      1023, "_menu_War3menu"      );   // War3 Menus items
    register_menucmd( register_menuid( MENU_SELECTRACE_TITLE ),    1023, "_menu_SelectRace"    );
    register_menucmd( register_menuid( MENU_BUYITEM_TITLE ),       1023, "_menu_ItemShop"      );
    register_menucmd( register_menuid( MENU_SKILLSINFO_TITLE ),    1023, "_menu_SkillsHelp"    );
    register_menucmd( register_menuid( MENU_SELECTSKILLS_TITLE ),  1023, "_menu_SelectSkills"  );
    register_menucmd( register_menuid( MENU_PLAYEROPTIONS_TITLE ), 1023, "_menu_PlayerOptions" );
    register_menucmd( register_menuid( MENU_RACEOPTIONS_TITLE ),   1023, "_menu_RaceOptions"   );
    register_menucmd( register_menuid( MENU_HELPTOPICS_TITLE ),    1023, "_menu_HelpTopics"    );

    register_menucmd( register_menuid( MENU_VOTE_DISABLE ), (1<<0)|(1<<1), "WAR3_vote_count"       );   // War3x Voting
    register_menucmd( register_menuid( MENU_VOTE_ENABLE ),  (1<<0)|(1<<1), "WAR3_vote_count"       );

    register_menucmd( register_menuid( "BuyItem", 1 ),          (1<<0)|(1<<1), "on_BuyArmor"    );   // Buy Armor (old style)
    register_menucmd(                             -34,          (1<<0)|(1<<1), "on_BuyArmor"    );   // Buy Armor (VGUI)


    // CVARS

    register_cvar( "Warcraft_3_Expansion",      "", FCVAR_SERVER );
    register_cvar( "Warcraft_3_Expansion_Date", "", FCVAR_SERVER );

    CVAR_enabled            = register_cvar( "war3x_enabled",     "", FCVAR_SERVER );
    CVAR_save_xp            = register_cvar( "war3x_save_xp",     "", FCVAR_SERVER );

    CVAR_save_xp_sql        = register_cvar( "war3x_save_xp_sql", "" );         // System CVARs
    CVAR_save_by_ip         = register_cvar( "war3x_save_by_ip",  "" );
    CVAR_vote_allow         = register_cvar( "war3x_vote_allow",  "" );
    CVAR_vote_ratio         = register_cvar( "war3x_vote_ratio",  "" );
    CVAR_setxp_flag         = register_cvar( "war3x_setxp_flag",  "" );

    CVAR_startlevel_first   = register_cvar( "war3x_startlevel_first", "" );    // Added v1.0
    CVAR_startlevel_other   = register_cvar( "war3x_startlevel_other", "" );

    CVAR_xp_normal          = register_cvar( "war3x_xp_normal",       "" );     // Gameplay CVARs
    CVAR_xp_objective       = register_cvar( "war3x_xp_objective",    "" );
    CVAR_xp_minplayers      = register_cvar( "war3x_xp_minplayers",   "" );
    CVAR_ultimatewarmup     = register_cvar( "war3x_ultimatewarmup",  "" );
    CVAR_shopmenus          = register_cvar( "war3x_shopmenus",       "" );
    CVAR_shopzone           = register_cvar( "war3x_shopzone",        "" );
    CVAR_shoptime           = register_cvar( "war3x_shoptime",        "" );
    CVAR_ankhpistols        = register_cvar( "war3x_ankhpistols",     "" );
    CVAR_ankhautosnipers    = register_cvar( "war3x_ankhautosnipers", "" );
    CVAR_ankhgrenades       = register_cvar( "war3x_ankhgrenades",    "" );
    CVAR_statusicons        = register_cvar( "war3x_statusicons",     "" );

    CVAR_restrict_items     = register_cvar( "war3x_restrict_items",     "" );  // Restriction CVARs
    CVAR_restrict_races     = register_cvar( "war3x_restrict_races",     "" );
    CVAR_restrict_skills    = register_cvar( "war3x_restrict_skills",    "" );
    CVAR_restrict_ultimates = register_cvar( "war3x_restrict_ultimates", "" );

    CVAR_sql_host           = register_cvar( "war3x_sql_host",	"" );           // MySQL CVARs
    CVAR_sql_user           = register_cvar( "war3x_sql_user",	"" );
    CVAR_sql_pass           = register_cvar( "war3x_sql_pass",	"" );
    CVAR_sql_db             = register_cvar( "war3x_sql_db",	"" );
	CVAR_sql_table          = register_cvar( "war3x_sql_table",	"" );

    // Log events

    register_logevent( "on_Objective_Player", 3,       "1=triggered" );
    register_logevent( "on_Objective_Team",   6,       "2=triggered" );
    register_logevent( "on_World_Action",     2, "0=World triggered" );

    // Events

    register_event( "ResetHUD",    "on_ResetHud",      "b"                                                );
    register_event( "DeathMsg",    "on_Death",         "a"                                                );
    register_event( "CurWeapon",   "on_CurWeapon",     "be", "1=1",           "2!0"                       );
    register_event( "Damage",      "on_Damage",        "b",  "2!0"                                        );
    register_event( "Battery",     "on_ArmorZero",     "b",  "1=0"                                        );
    register_event( "ArmorType",   "on_ArmorType",     "be"                                               );
    register_event( "ItemPickup",  "on_ArmorPickup",   "b",  "1&item_kevlar", "1&item_assaultsuit"        );
    register_event( "WeapPickup",  "on_WeapPickup",    "b"                                                );
    register_event( "StatusValue", "on_Target_Team",   "b",  "1=1"                                        );
    register_event( "StatusValue", "on_Target_Id",     "b",  "1=2"                                        );
    register_event( "StatusValue", "on_Target_Health", "b",  "1=3"                                        );
    register_event( "StatusText",  "on_Target_Type",   "b"                                                );
    register_event( "StatusIcon",  "on_StatusIcon",    "be"                                               );
    register_event( "SetFOV",      "on_Zoom",          "be"                                               );
    register_event( "30",          "on_Intermission",  "a"                                                );
    register_event( "BarTime",     "on_BarTime",       "be"                                               );
    register_event( "HideWeapon",  "on_UseShield",     "b",  "1=66"                                       );
    register_event( "23",          "on_TargetBombed",  "a",  "1=17",          "6=-105",            "7=17" );
    register_event( "Health",      "on_Health",        "b"                                                );
    register_event( "AmmoX",       "on_ThrowGren",     "b",  "1=11",          "1=12",              "1=13" );

	// Touch Events

	register_touch( "DEATH_COIL",   "player", "UD_U_DEATHCOIL_touch" );
	register_touch( "WAR3X_ITEM",	"player", "Item_Touch" );

	// Register dictionary

	register_dictionary( "war3x.txt" );

    WAR3_set_xp();

    // Run config

    server_cmd( "exec %s/war3x.cfg", WAR3X_DIR );

    set_task( 1.0, "WAR3_map_start" );

	set_error_filter( "DEBUG_Error" );

    return PLUGIN_CONTINUE;
}


public plugin_end() {

    if ( !g_bWar3xEnabled )
        return PLUGIN_CONTINUE;

    // Save everyone to database

    Store_Queue_All();

    if ( g_DBILoaded )
        mysql_close_all();

    return PLUGIN_CONTINUE;
}


// This functionality allows us to no longer requires a DBI module to be loaded

public plugin_natives() {

	set_module_filter( "module_filter" );
	set_native_filter( "native_filter" );

    return PLUGIN_CONTINUE;
}


public module_filter( const module[] ) {

	// Since the DBI module is missing, lets make sure we don't use it

	if ( equali( module, "dbi" ) )
	{
		// If saving XP and saving via SQL is on then lets print a message

		log_amx( "No DBI module found, defaulting to vault" );
		log_amx( "If you would like to save using SQL, please turn on a SQL module" );

		g_DBILoaded = false;

		return PLUGIN_HANDLED;
	}

	return PLUGIN_CONTINUE;
}


public native_filter( const name[], index, trap ) {

      if ( !trap )
            return PLUGIN_HANDLED;

      return PLUGIN_CONTINUE;
}


public DEBUG_Error( error_code, bool:debugging, message[] ) {

	new buffer[512];
	dbg_fmt_error(buffer, 511);

	// Hopefully this is always true?

	if ( buffer[0] )
	{
		DEBUG_Write_File( buffer );
	}

	// Just in case its not

	else
	{
		new msg[512];
		formatex( msg, 511, "Error code: %d with message: %s", error_code, message );
		DEBUG_Write_File( msg );
	}

	// Can only get a trace if plugin is running in debug mode right?

	if ( debugging )
	{
		new trace = dbg_trace_begin();
		if ( !trace )
		{
			DEBUG_Write_File( "No trace found!" );
		}

		new num = 0;
		new func[32], file[64], line;
		while ( trace )
		{
			dbg_trace_info(trace, line, func, 31, file, 63);
			new msg[256];
			formatex( msg, 255, "   [%d] %s::%s (line %d)", num++, file, func, line );
			DEBUG_Write_File( msg );
			trace = dbg_trace_next(trace);
		}
	}

	return PLUGIN_CONTINUE;
}


public DEBUG_Write_File( msg[] ) {

	new file[] = "addons/amxmodx/logs/war3x_errors.log";

	new timeStr[64];
	get_time("%m/%d/%Y - %H:%M:%S", timeStr, 63);

	//L 02/24/2006 - 16:39:46: [ENGINE] Invalid entity 114
	//L 02/24/2006 - 16:39:46: [AMXX] Displaying debug trace (plugin "war3x.amxx")
	//L 02/24/2006 - 16:39:46: [AMXX] Run time error 10: native error (native "dbg_trace_next")
	//L 02/24/2006 - 16:39:46: [AMXX]    [0] UTIL_vexd.inc::Remove_TempEnt (line 422)

	new errorMsg[256];
	formatex( errorMsg, 255, "L [WAR3X] %s: %s", timeStr, msg );

	write_file( file, errorMsg, -1 );
}


// ------------------------------------------------- End. - //
