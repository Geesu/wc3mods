

// Miscellaneous Configuration CVARs
new CVAR_FT_Query_Client
new CVAR_FT_Show_Player
new CVAR_FT_Buy_Dead
new CVAR_FT_Buy_Time
new CVAR_FT_Buy_Zone
new CVAR_FT_Races
new CVAR_FT_Spec_Position
new CVAR_FT_Grenade_Protection

// Icon CVARs
new CVAR_ICON_Race
new CVAR_ICON_Level

// Admin CVARs
new CVAR_ADMIN_Flag

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
new CVAR_XP_Multiplier
new CVAR_XP_Weapon_Multiplier
new CVAR_XP_Auto_Average
new CVAR_XP_Show_Kill_Objectives;
new CVAR_XP_Show_Objectives;

// XP Saving CVARs
new CVAR_SAVE_Enabled
new CVAR_SAVE_Pruning
new CVAR_SAVE_Days_Before_Delete
new CVAR_SQL_Save_By;
new CVAR_SQL_Enabled;

// Database CVARs
new CVAR_SQL_tbname
new CVAR_SQL_dbhost
new CVAR_SQL_dbuser
new CVAR_SQL_dbpass
new CVAR_SQL_dbname
new CVAR_SAVE_End_Round

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
new CVAR_ITEM_Frost
new CVAR_ITEM_Mask_Of_Death
new CVAR_ITEM_Cloak
new CVAR_ITEM_Sock
new CVAR_ITEM_Glove_Timer
new CVAR_ITEM_Glove_Orc_Damage
new CVAR_ITEM_Glove_Disable_KA

// Chameleon
new CVAR_CHAM_Random
new CVAR_CHAM_Skill1
new CVAR_CHAM_Skill2
new CVAR_CHAM_Skill3
new CVAR_CHAM_Ultimate

// DOD CVARs
#if MOD == 1
	new CVAR_DOD_Start_Money
#endif
#if MOD == 0
	new CVAR_ITEM_Boots;
#endif

// Bot CVARs
new CVAR_BOT_Buy_Item

public CVAR_Init()
{
	
	// Admin CVARs

	CVAR_ADMIN_Flag					= register_cvar("ADMIN_Flag",				"m")

	// Miscellaneous Configuration CVARs

	CVAR_FT_Query_Client			= register_cvar("FT_Query_Client",			"1")
	CVAR_FT_Show_Player				= register_cvar("FT_Show_Player",			"1")
	CVAR_FT_Buy_Dead				= register_cvar("FT_Buy_Dead",				"1")
	CVAR_FT_Buy_Time				= register_cvar("FT_Buy_Time",				"0")
	CVAR_FT_Buy_Zone				= register_cvar("FT_Buy_Zone",				"0")
	CVAR_FT_Races					= register_cvar("FT_Races",					"8", FCVAR_SERVER)
	CVAR_FT_Spec_Position			= register_cvar("FT_Spec_Position",				"0")
	CVAR_FT_Grenade_Protection		= register_cvar("FT_Grenade_Protection",		"0"						)

	// Icon CVARs

	CVAR_ICON_Race					= register_cvar("ICON_Race",			"1")
	CVAR_ICON_Level					= register_cvar("ICON_Level",			"1")

	// Saving CVARs

	CVAR_SAVE_Enabled				= register_cvar("SAVE_Enabled",					"0", FCVAR_SERVER)
	CVAR_SAVE_Pruning				= register_cvar("SAVE_Pruning",			"0", FCVAR_SERVER)
	CVAR_SAVE_Days_Before_Delete	= register_cvar("SAVE_Expire_Time",			"31"						)
	CVAR_SAVE_End_Round				= register_cvar("SAVE_End_Round",		"1"					)
	CVAR_SQL_Enabled				= register_cvar("SQL_Enabled",						"0"						)
	CVAR_SQL_Save_By				= register_cvar("SQL_Save_By",					"0")


	// Ultimate CVARs

	CVAR_ULT_Entangle_Drop			= register_cvar( "ULT_Entangle_Drop",			"0")
	CVAR_ULT_Delay					= register_cvar( "ULT_Delay",					"15.0")
	CVAR_ULT_Cooldown				= register_cvar( "ULT_Cooldown",					"35.0")
	CVAR_ULT_Blink_Protection		= register_cvar( "ULT_Blink_Protection",			"1")
	CVAR_ULT_Blink_Dizziness		= register_cvar( "ULT_Blink_Dizziness",			"0")

	// Item CVARs

	CVAR_ITEM_Tome					= register_cvar( "ITEM_Tome",				"50"	);
	CVAR_ITEM_Claw					= register_cvar( "ITEM_Claw",				"6");
	CVAR_ITEM_Health				= register_cvar( "ITEM_Health",				"15");
	CVAR_ITEM_Frost					= register_cvar( "ITEM_Frost",				"125")
	CVAR_ITEM_Mask_Of_Death			= register_cvar( "ITEM_Mask_Of_Death",			"0.3")
	CVAR_ITEM_Cloak					= register_cvar( "ITEM_Cloak",					"150")
	CVAR_ITEM_Sock					= register_cvar( "ITEM_sock",					"0.5")
	CVAR_ITEM_Glove_Timer			= register_cvar( "ITEM_Glove_Timer",			"10")
	CVAR_ITEM_Glove_Orc_Damage		= register_cvar( "ITEM_Glove_Orc_Damage",		"0")
	CVAR_ITEM_Glove_Disable_KA		= register_cvar( "ITEM_Glove_Disable_KA",		"1")

	// Chameleon Race CVARs

	CVAR_CHAM_Random				= register_cvar( "CHAM_Random",			"1")
	CVAR_CHAM_Skill1				= register_cvar( "CHAM_Skill1",			"1")
	CVAR_CHAM_Skill2				= register_cvar( "CHAM_Skill2",			"1")
	CVAR_CHAM_Skill3				= register_cvar( "CHAM_Skill3",			"1")
	CVAR_CHAM_Ultimate				= register_cvar( "CHAM_Ultimate",		"1")
	
	// Database CVARs

	CVAR_SQL_tbname					= register_cvar( "SQL_tbname",					"war3users"		);
	CVAR_SQL_dbhost					= register_cvar( "SQL_dbhost",					"127.0.0.1"		);
	CVAR_SQL_dbuser					= register_cvar( "SQL_dbuser",					"root"			);
	CVAR_SQL_dbpass					= register_cvar( "SQL_dbpass",					""				);
	CVAR_SQL_dbname					= register_cvar( "SQL_dbname",					"amx"			);

	// XP Modifier CVARs

	CVAR_XP_Min_Players				= register_cvar( "XP_Min_Players",				"2"		);
	CVAR_XP_Radius 					= register_cvar( "XP_Radius",					"750"	);
	CVAR_XP_Win_Round				= register_cvar( "XP_Win_Round",				"35"	);
	CVAR_XP_Headshot				= register_cvar( "XP_Headshot",					"9"		);
	CVAR_XP_Kill_Defuser			= register_cvar( "XP_Kill_Defuser",				"25"	);
	CVAR_XP_VIP_Escape				= register_cvar( "XP_VIP_Escape",				"30"	);
	CVAR_XP_Kill_Bomb_Carrier		= register_cvar( "XP_Kill_Bomb_Carrier",		"25"	);
	CVAR_XP_Kill_Bomb_Planter		= register_cvar( "XP_Kill_Bomb_Planter",		"15"	);
	CVAR_XP_Defuse_Bomb				= register_cvar( "XP_Defuse_Bomb",				"30"	);
	CVAR_XP_Rescue_Hostage			= register_cvar( "XP_Rescue_Hostage",			"25"	);
	CVAR_XP_Kill_Rescuer			= register_cvar( "XP_Kill_Rescuer",				"25"	);
	CVAR_XP_Kill_VIP				= register_cvar( "XP_Kill_VIP",					"25"	);
	CVAR_XP_Show_Kill_Objectives	= register_cvar( "XP_Show_Kill_Objectives",		"0"		);
	CVAR_XP_Show_Objectives			= register_cvar( "XP_Show_Objectives",			"1"		);
	CVAR_XP_Multiplier				= register_cvar( "XP_Multiplier",				"1.0", FCVAR_SERVER);
	CVAR_XP_Weapon_Multiplier		= register_cvar( "XP_Weapon_Multiplier",		"1")
	CVAR_XP_Auto_Average			= register_cvar( "CVAR_XP_Auto_Average",		"0")
	
	// Bot CVARs

	CVAR_BOT_Buy_Item				= register_cvar("BOT_Buy_Item",			"0.33")

	// MOD specific CVARs

	#if MOD == 1
		CVAR_DOD_Start_Money		= register_cvar("DOD_Start_Money",	"800")
	#endif
	#if MOD == 0
		CVAR_ITEM_Boots				= register_cvar("ITEM_Boots",		"275")
	#endif

}

// Function will configure the various CVARs
public CVAR_Configure()
{
	// If there is no DBI module loaded than we will be unable to save XP with SQL
	if ( !g_DBILoaded && get_pcvar_num( CVAR_SQL_Enabled ) )
	{
		set_pcvar_num( CVAR_SQL_Enabled, 0 );
	}
	
	// We will only change the bonus' if we are saving XP
	if ( get_pcvar_num( CVAR_SAVE_Enabled ) )
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
		set_pcvar_num( CVAR_ITEM_Tome				, (get_pcvar_num( CVAR_ITEM_Tome ) / iXPDivisor)			);
	}

	// Configure the Chameleon Race
	if ( !get_pcvar_num( CVAR_CHAM_Random ) )
	{
		race9Options[1] = get_pcvar_num( CVAR_CHAM_Skill1 );
		race9Options[2] = get_pcvar_num( CVAR_CHAM_Skill2 );
		race9Options[3] = get_pcvar_num( CVAR_CHAM_Skill3 );
		race9Options[4] = get_pcvar_num( CVAR_CHAM_Ultimate );
	}
}
