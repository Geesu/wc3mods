
// CVAR Pointers
new CVAR_FT_admin_flag
new CVAR_FT_query_client
new CVAR_FT_autoxp
new CVAR_FT_show_player
new CVAR_FT_Race9_Random
new CVAR_FT_Race9_Skill1
new CVAR_FT_Race9_Skill2
new CVAR_FT_Race9_Skill3
new CVAR_FT_Race9_Ultimate
new CVAR_FT_buydead
new CVAR_FT_buytime
new CVAR_FT_buyzone
new CVAR_FT_races
new CVAR_FT_bot_buy_item
new CVAR_FT_race_icons
new CVAR_FT_level_icons
new CVAR_FT_no_orcnades
new CVAR_FT_centerhud
new CVAR_FT_saveby
new CVAR_FT_position
new CVAR_FT_glove_timer
new CVAR_FT_glove_orc_damage
new CVAR_FT_kill_objectives
new CVAR_FT_show_icons
new CVAR_FT_no_gloves_on_ka
new CVAR_FT_blink_protection
new CVAR_FT_blink_dizziness
new CVAR_FT_spec_info
new CVAR_FT_objectives
new CVAR_FT_auto_pruning
new CVAR_mp_savexp
new CVAR_mp_xpmultiplier
new CVAR_mp_weaponxpmodifier
new CVAR_sv_warcraft3
new CVAR_mp_grenadeprotection
new CVAR_DOD_startmoney
new CVAR_FT_items_in_hud

// XP CVARs
new CVAR_XP_Radius
new CVAR_XP_Win_Round
new CVAR_XP_Headshot
new CVAR_XP_Kill_Defuser
new CVAR_XP_VIP_Escape
new CVAR_XP_Kill_Bomb_Carrier
new CVAR_XP_Kill_Bomb_Planter
new CVAR_XP_Defuse_Bomb
new CVAR_XP_Rescue_Hostage
new CVAR_XP_Kill_Rescuer
new CVAR_XP_Kill_VIP
new CVAR_XP_Min_Players

// Database CVARs
new CVAR_SQL_tbname
new CVAR_SQL_dbhost
new CVAR_SQL_dbuser
new CVAR_SQL_dbpass
new CVAR_SQL_dbname
new CVAR_sv_save_end_round
new CVAR_sv_daysbeforedelete
new CVAR_sv_sql

// Ultimate CVARs
new CVAR_ULT_Entangle_Drop;
new CVAR_ULT_Delay;
new CVAR_ULT_Cooldown;
new CVAR_ULT_Blink_Protection;
new CVAR_ULT_Blink_Dizziness;

// Items
new CVAR_ITEM_Tome
new CVAR_ITEM_Claw
new CVAR_ITEM_Health

new CVAR_DOD_bootspeed
new CVAR_FT_bootspeed
new CVAR_FT_sock

new CVAR_FT_frost_speed
new CVAR_FT_mask_of_death
new CVAR_FT_cloak


public CVAR_Init()
{
	// Ultimate modifier CVARS
	CVAR_ULT_Entangle_Drop		= register_cvar("ULT_Entangle_Drop",			"0")	//*
	CVAR_ULT_Delay				= register_cvar("ULT_Delay",					"15.0")	//*
	CVAR_ULT_Cooldown			= register_cvar("ULT_Cooldown",					"35.0")	//*
	CVAR_ULT_Blink_Protection	= register_cvar("ULT_Blink_Protection",			"1")	//*
	CVAR_ULT_Blink_Dizziness	= register_cvar("ULT_Blink_Dizziness",			"0")	//*

	// Item modifier CVARS
	CVAR_ITEM_Tome				= register_cvar( "ITEM_Tome",				"50"	);	//*

	CVAR_ITEM_Claw				= register_cvar("ITEM_Claw",					"6")
	CVAR_ITEM_Health			= register_cvar("ITEM_Health",				"15")
	
	CVAR_ITEM_Frost				= register_cvar("ITEM_Frost",			"125")
	CVAR_FT_mask_of_death		= register_cvar("FT_mask_of_death",			"0.3")
	CVAR_FT_cloak				= register_cvar("FT_cloak",					"150")
	CVAR_FT_glove_timer			= register_cvar("FT_glove_timer",			"10")
	CVAR_FT_glove_orc_damage	= register_cvar("FT_glove_orc_damage",		"0")
	CVAR_FT_sock				= register_cvar("FT_sock",					"0.5")
	CVAR_FT_no_gloves_on_ka		= register_cvar("FT_no_gloves_on_ka",		"1")



	CVAR_FT_admin_flag			= register_cvar("FT_admin_flag",			"m")
	CVAR_FT_query_client		= register_cvar("FT_query_client",			"1")
	CVAR_FT_autoxp				= register_cvar("FT_autoxp",				"0")
	CVAR_FT_show_player			= register_cvar("FT_show_player",			"1")
	CVAR_FT_Race9_Random		= register_cvar("FT_Race9_Random",			"1")
	CVAR_FT_Race9_Skill1		= register_cvar("FT_Race9_Skill1",			"1")
	CVAR_FT_Race9_Skill2		= register_cvar("FT_Race9_Skill2",			"1")
	CVAR_FT_Race9_Skill3		= register_cvar("FT_Race9_Skill3",			"1")
	CVAR_FT_Race9_Ultimate		= register_cvar("FT_Race9_Ultimate",		"1")
	CVAR_FT_buydead				= register_cvar("FT_buydead",				"1")
	CVAR_FT_buytime				= register_cvar("FT_buytime",				"0")
	CVAR_FT_buyzone				= register_cvar("FT_buyzone",				"0")
	CVAR_FT_races				= register_cvar("FT_races",					"8")
	CVAR_FT_bot_buy_item		= register_cvar("FT_bot_buy_item",			"0.33")
	CVAR_FT_race_icons			= register_cvar("FT_race_icons",			"1")
	CVAR_FT_level_icons			= register_cvar("FT_level_icons",			"1")

	register_cvar("FT_no_orcnades",				"0")
	register_cvar("FT_centerhud",				"1")
	register_cvar("FT_saveby",					"0")
	register_cvar("FT_position",				"0")
	register_cvar("FT_show_icons",				"1")
	register_cvar("FT_spec_info",				"1")
	register_cvar("FT_auto_pruning",			"0", FCVAR_SERVER)
	register_cvar("mp_savexp",					"0", FCVAR_SERVER)
	register_cvar("mp_xpmultiplier",			"1.0")
	register_cvar("mp_weaponxpmodifier",		"1")
	register_cvar("sv_warcraft3",				"1")
	register_cvar("mp_grenadeprotection",		"0"						)
	register_cvar("sv_save_end_round",			"1"						)
	register_cvar("sv_daysbeforedelete",		"31"					)
	register_cvar("sv_sql",						"0"						)

	
	// Database CVARs

	CVAR_SQL_tbname					= register_cvar( "SQL_tbname",					"war3users"		);	//*
	CVAR_SQL_dbhost					= register_cvar( "SQL_dbhost",					"127.0.0.1"		);	//*
	CVAR_SQL_dbuser					= register_cvar( "SQL_dbuser",					"root"			);	//*
	CVAR_SQL_dbpass					= register_cvar( "SQL_dbpass",					""				);	//*
	CVAR_SQL_dbname					= register_cvar( "SQL_dbname",					"amx"			);	//*

	// XP Modifier CVARs

	CVAR_XP_Min_Players				= register_cvar("XP_Min_Players",				"2"		);	//*
	CVAR_XP_Radius 					= register_cvar( "XP_Radius",					"750"	);	//*
	CVAR_XP_Win_Round				= register_cvar( "XP_Win_Round",				"35"	);	//*
	CVAR_XP_Headshot				= register_cvar( "XP_Headshot",					"9"		);	//*
	CVAR_XP_Kill_Defuser			= register_cvar( "XP_Kill_Defuser",				"25"	);	//*
	CVAR_XP_VIP_Escape				= register_cvar( "XP_VIP_Escape",				"30"	);	//*
	CVAR_XP_Kill_Bomb_Carrier		= register_cvar( "XP_Kill_Bomb_Carrier",		"25"	);	//*
	CVAR_XP_Kill_Bomb_Planter		= register_cvar( "XP_Kill_Bomb_Planter",		"15"	);	//*
	CVAR_XP_Defuse_Bomb				= register_cvar( "XP_Defuse_Bomb",				"30"	);	//*
	CVAR_XP_Rescue_Hostage			= register_cvar( "XP_Rescue_Hostage",			"25"	);	//*
	CVAR_XP_Kill_Rescuer			= register_cvar( "XP_Kill_Rescuer",				"25"	);	//*
	CVAR_XP_Kill_VIP				= register_cvar( "XP_Kill_VIP",					"25"	);	//*
	CVAR_XP_Show_Kill_Objectives	= register_cvar( "XP_Show_Kill_Objectives",		"0"		);	//*
	CVAR_XP_Show_Objectives			= register_cvar( "XP_Show_Objectives",			"1"		);	//*
	
	// MOD specific CVARs

	if ( g_MOD == GAME_DOD )
	{
		CVAR_DOD_bootspeed			= register_cvar("DOD_bootspeed",	"45.0")
		CVAR_DOD_startmoney			= register_cvar("DOD_startmoney",	"800")
	}
	else if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO  )
	{
		CVAR_FT_items_in_hud		= register_cvar("FT_items_in_hud",	"0")
		CVAR_FT_bootspeed			= register_cvar("FT_bootspeed",		"275")
	}

}

// Function will configure the various CVARs
public CVAR_Configure()
{
	// If there is no DBI module loaded than we will be unable to save XP with SQL
	if ( !g_DBILoaded && get_pcvar_num( CVAR_sv_sql ) )
	{
		set_pcvar_num( CVAR_sv_sql, 0 );
	}
	
	// We will only change the bonus' if we are saving XP
	if ( get_pcvar_num( CVAR_mp_savexp ) )
	{
		new iXPDivisor = 5;
	
		set_pcvar_num( CVAR_XP_Win_Round			, (get_pcvar_num( CVAR_XP_Win_Round ) / iXPDivisor)			);
		set_pcvar_num( CVAR_XP_Headshot				, (get_pcvar_num( CVAR_XP_Headshot ) / iXPDivisor)			);
		set_pcvar_num( CVAR_XP_Kill_Defuser			, (get_pcvar_num( CVAR_XP_Kill_Defuser ) / iXPDivisor)		);
		set_pcvar_num( CVAR_XP_VIP_Escape			, (get_pcvar_num( CVAR_XP_VIP_Escape ) / iXPDivisor)		);
		set_pcvar_num( CVAR_XP_Kill_Bomb_Carrier	, (get_pcvar_num( CVAR_XP_Kill_Bomb_Carrier ) / iXPDivisor) );
		set_pcvar_num( CVAR_XP_Kill_Bomb_Planter	, (get_pcvar_num( CVAR_XP_Kill_Bomb_Planter ) / iXPDivisor) );
		set_pcvar_num( CVAR_XP_Defuse_Bomb			, (get_pcvar_num( CVAR_XP_Defuse_Bomb ) / iXPDivisor)		);
		set_pcvar_num( CVAR_XP_Rescue_Hostage		, (get_pcvar_num( CVAR_XP_Rescue_Hostage ) / iXPDivisor)	);
		set_pcvar_num( CVAR_XP_Kill_Rescuer			, (get_pcvar_num( CVAR_XP_Kill_Rescuer ) / iXPDivisor)		);
		set_pcvar_num( CVAR_XP_Kill_VIP				, (get_pcvar_num( CVAR_XP_Kill_VIP ) / iXPDivisor)			);
		set_pcvar_num( CVAR_ITEM_Tomb				, (get_pcvar_num( CVAR_ITEM_Tomb ) / iXPDivisor)			);
	}
}