// ***************************************************************************
// Author: B. Josh Becker
// Description: This page contains defines as well as the variables used
// in this plugin
// ***************************************************************************

#define TASK_ITEMS			0		// Showing items in the HUD
#define TASK_SPAWN			32		// Respawning from revive
#define TASK_ITEM_SCROLL	64		// Respawning after death from an item
#define TASK_VENGEANCE		96		// Respawning from having vengeance (warden ultimate)
//#define TASK_SETXP			128		// Sets the XP
#define TASK_EXPLOSION		160		// From Suicide Bomber
#define TASK_BEAMCYLINDER	192		// From Suicide Bomber
#define	TASK_GETINPUT		224		// Calls getuserinput function
#define TASK_COOLDOWN		256		// Ultimate Cooldown function
#define TASK_TRANSPORT		288		// Transport function for ITEM_MOLE
#define TASK_FAN			320		// Calls check_spot function
#define	TASK_WAVE			352		// Healing Wave
#define	TASK_REINCARNATION	384		// Weapon Reincarnation
#define	TASK_GLOW			416		// Calls glow_change
#define	TASK_RESETPOS		448		// Resets the user's position (1.5)
#define	TASK_JUMPER			480		// Jumper task (goomba)
#define	TASK_RESETSPEED		512		// Reset maxspeed (bash)
#define	TASK_ITEM_GLOVES	544		// Nade timer (gloves)
#define	TASK_UNHOLYSPEED	576		// Unholyspeed timer
#define	TASK_RESETGOD		608		// Reset god mode
#define	TASK_CEILING		640		// Ceiling Check function (blink)
#define	TASK_BLINKCONT		672		// Calls Blink Controller function
#define	TASK_TELEMENU		704		// Telemenu function
#define	TASK_TELEWAITSTOP	736		// Calls telewaitstop function
#define	TASK_TELEPORT		768		// Teleports the player
#define	TASK_TELEFADEIN		800		// Calls telefadein function
#define	TASK_UNSTICK		832		// "Unsticks" a stuck teleported player
#define	TASK_SEARCHTARGET	864		// Used by some ultimates
//#define TASK_WALKER			896		// Walker process
#define	TASK_WAITSTOP		928		// Used for entangling roots
#define	TASK_LIGHTNING		960		// Used for chain lightning
#define	TASK_LIGHTSEARCH	992		// Used for chain lightning
#define	TASK_LIGHTNINGNEXT	1024	// Used for chain lightning
#define TASK_SPAWNPLAYER	1056	// Used within func_spawn to spawn a player
#define TASK_GIVEITEMS		1088	// Used to give a player his items after spawning
#define	TASK_ITEM_RINGERATE	1120	// Used with rings of regeneration
#define	TASK_TESPRAY		1152	// Used with flamethrower
#define	TASK_ONFIRE			1184	// Used with flamethrower
//#define	TASK_HEALNOW		1226	// Used with Healing Waves
#define	TASK_HEALNEXT		1258	// Used with Healing Waves
#define	TASK_LIGHT			1290	// Used with serpant wards
#define	TASK_SHIELD			1322	// Used to check for shield
#define	TASK_FUNNELS		1354	// Used with Swarming Locusts
#define	TASK_HETHROWEVENT	1386	// Used for nade trails
#define	TASK_MONEYLOOP		1428	// Used for displaying the money for DOD
#define TASK_TEMPENTITY		1460
#define TASK_ANKHGIVEITEMS	1492
#define TASK_HELM			1524
#define	TASK_REINCCHECK		1556

#define TASK_BUYTIME		2000	// Checks for buytime
#define TASK_UDELAY			2001	// Ultimate Delay Function
#define	TASK_CHECKVOTES		2002	// Used for checking the votes
#define	TASK_FTCONTROLLER	2003	// Used for automatic start/stop
#define	TASK_CHECKWAR3		2004
//#define	TASK_SETLONGXP		2005
//#define	TASK_SETSHORTXP		2006
#define	TASK_SETMYSQL		2007
//#define	TASK_SETLANGUAGE	2008
#define	TASK_SETVARIABLES	2009
#define	TASK_CLONETHINK		2010
//#define	TASK_CHECKMAP		2011
#define TASK_BOIDTHINK		2012
#define TASK_TARGETBOMBED	2013
#define TASK_BOMBTIMER		2014
#define TASK_WAR3CHECK		2015	// Checks the value of sv_warcraft3 every 5 seconds (better than checking it on every function call)
#define TASK_SIPHON			2016

// From ../multiplayer source/dlls/player.cpp
#define ARMOR_RATIO	 0.7	// Armor Takes 30% of the damage (was .2 in the SDK)
#define ARMOR_BONUS  0.5	// Each Point of Armor is work 1/x points of health

#define TE_BEAMPOINTS		0
#define TE_EXPLOSION		3
#define	TE_TAREXPLOSION		4
#define TE_SMOKE			5
#define TE_BEAMENTS			8
#define	TE_LAVASPLASH		10
#define	TE_TELEPORT			11
#define TE_IMPLOSION		14
#define TE_SPRITETRAIL		15
#define TE_SPRITE			17
#define TE_BEAMCYLINDER		21
#define TE_BEAMFOLLOW		22
#define TE_STREAK_SPLASH	25
#define TE_ELIGHT			28
#define TE_LARGEFUNNEL		100
#define TE_FIZZ				105		// create alpha sprites inside of entity, float upwards
#define TE_BUBBLES			113		// create alpha sprites inside of box, float upwards
#define TE_SPRAY			120
#define TE_PLAYERATTACHMENT 124

#define	EF_BRIGHTFIELD		1

#define TE_EXPLFLAG_NONE	0

#define MAX_NAME_LENGTH 32
#define MAX_VAR_LENGTH 64 
#define RACE_SKILL_LENGTH 64
#define RACE_SKILL_LENGTH_F 63
#define RACE_NAME_LENGTH 64
#define RACE_NAME_LENGTH_F 63
#define ITEM_NAME_LENGTH 64
#define ITEM_NAME_LENGTH_F 63
#define SHORT_RACE_NAME_LENGTH 16
#define SHORT_RACE_NAME_LENGTH_F 15
#define SHORT_ITEM_NAME_LENGTH 32
#define SHORT_ITEM_NAME_LENGTH_F 31

#define MAXGLOW					150
#define ULTIMATESEARCHTIME		50			// Translates to 5 seconds
#define TELEPORT_RADIUS			50
#define LIGHTNING_RANGE			500
#define EXPLOSION_MAX_DAMAGE	50
#define EXPLOSION_RANGE			300
#define EXPLOSION_BLAST_RADIUS	250
#define BLINK_COOLDOWN			2.0
#define ORB_DAMAGE				20			// Damage done by Orb of Annihilation
#define CONCOCTION_DAMAGE		15			// Damage done by Unstable Concoction
#define CONCOCTION_RADIUS		300

// CS AmmoX Types
#define AMMOX_338MAGNUM			1     // AWP
#define AMMOX_762NATO			2     // AK47, SCOUT, G3SG1
#define AMMOX_556NATOBOX		3     // MG
#define AMMOX_556NATO			4     // SG552, GALI, FAMAS, M4A1, AUG, SG550
#define AMMOX_BUCKSHOT			5     // M3, XM1014
#define AMMOX_45ACP				6     // MAC10, UMP45, USP
#define AMMOX_57MM				7     // P90, FIVESEVEN
#define AMMOX_50AE				8     // DEAGLE
#define AMMOX_357SIG			9     // P228
#define AMMOX_9MM				10     // MP5NAVY, TMP, GLOCK18, ELITE
#define AMMOX_FLASHBANG			11     // Flashbang
#define AMMOX_HEGRENADE			12     // HE Grenade
#define AMMOX_SMOKEGRENADE		13     // Smoke Grenade

/*	 Weapon IDs	 */
#define CSW_WORLDSPAWN			0 

#if !defined CSW_GALIL
	#define CSW_GALIL			14
#endif
#if !defined CSW_FAMAS
	#define CSW_FAMAS			15
#endif

#define CSW_WAR3_MIN			51

#define	CSW_LIGHTNING			51
#define	CSW_SUICIDE				52
#define	CSW_FLAME				53
#define	CSW_LOCUSTS				54
#define	CSW_SERPENTWARD			55
#define CSW_SHADOW				56
#define	CSW_THORNS				57
#define	CSW_CARAPACE			58
#define CSW_CARRION				59
#define CSW_ORB					60
#define CSW_CONCOCTION			61

#define CSW_WAR3_MAX			61

// Should be equal to (CSW_WAR3_MIN - CSW_WAR3_MAX) + 1
#define MAX_CSW_S				11

// Race numbers
#define RACE_UNDEAD				1
#define RACE_HUMAN				2
#define RACE_ORC				3
#define RACE_ELF				4
#define RACE_BLOOD				5
#define RACE_SHADOW				6
#define RACE_WARDEN				7
#define RACE_CRYPT				8
#define RACE_CHAMELEON			9

#define SHOPMENU_ONE			1
#define SHOPMENU_TWO			2

#define ITEM_ANKH 1
#define ITEM_BOOTS 2
#define ITEM_CLAWS 3
#define ITEM_CLOAK 4
#define ITEM_MASK 5
#define ITEM_NECKLACE 6
#define ITEM_FROST 7
#define ITEM_HEALTH 8
#define ITEM_TOME 9

#define ITEM_SCROLL 1
#define ITEM_PROTECTANT 2
#define ITEM_HELM 3
#define ITEM_AMULET 4
#define ITEM_SOCK 5
#define ITEM_GLOVES 6
#define ITEM_RING 7
#define ITEM_CHAMELEON 8
#define ITEM_MOLE 9

// team ids 
#define UNASSIGNED 0 
#define TS 1 
#define CTS 2 
#define AUTO_TEAM 5 

// For Icon events
#define ICON_HIDE 0 
#define ICON_SHOW 1 
#define ICON_FLASH 2 

#define HUDMESSAGE_CHAN_LEVEL		1     // Level information centered @ mid-top of screen
#define HUDMESSAGE_CHAN_STATUS		1     // WAR3 status text centered @ bottom of screen
//#define HUDMESSAGE_CHAN_XP			3     // XP Information @ bottom left of screen
#define HUDMESSAGE_CHAN_INFO		2     // Misc info centered @ top of screen
#define HUDMESSAGE_CHAN_ITEM		3     // Race/Item Information @ bottom left of screen
#define HUDMESSAGE_CHAN_MONEY		4     // Race/Item Information @ bottom left of screen


#define HUDMESSAGE_CHAN_SERVER		2     // Server messages centered @ mid-bottom of screen
#define HUDMESSAGE_CHAN_DEAD		2     // Follow message (spectator)

#define HUDMESSAGE_FX_FADEIN		0         // hudmessage effect types
#define HUDMESSAGE_FX_FLICKER		1
#define HUDMESSAGE_FX_WRITEOUT		2

#define HUDMESSAGE_POS_STATUS		0.85
#define HUDMESSAGE_POS_SERVER		0.70
#define HUDMESSAGE_POS_INFO			0.20
#define HUDMESSAGE_POS_LEVEL		0.25
#define HUDMESSAGE_POS_CENTER		-1.0

#define SKIN_RESET					0
#define SKIN_SWITCH					1
#define SKIN_HEX					2

#define KEY_1						0
#define KEY_2						1
#define KEY_3						2
#define KEY_4						3
#define KEY_5						4
#define KEY_6						5
#define KEY_7						6
#define KEY_8						7
#define KEY_9						8
#define KEY_0						9

#define XPOS						0
#define YPOS						1
#define ZPOS						2

#define DISPLAYLEVEL_NONE			0
#define DISPLAYLEVEL_SHOWRACE		1
#define DISPLAYLEVEL_SHOWGAINED		2
//#define DISPLAYLEVEL_SHOWBOTH		3
#define	DISPLAYLEVEL_SHOWRACECHAT	4

#define SKILL1						1
#define SKILL2						2
#define SKILL3						3
#define SKILL4						4

#define SKILL_1						1
#define SKILL_2						2
#define SKILL_3						3
#define SKILL_ULTIMATE				4
#define SKILL_HERO					5

#define MAX_RACES					9

#define MAX_LEVELS					10

// Number of attempts to make to connect to the database if it fails (only if sv_mysql is 1)
#define	SQL_ATTEMPTS				10

// ***************************************************************************
// The following section contains defines for the player_data array, it pretty much
// stores everything.  
//		P_ is within player_data
//		PB_ is within player_data_bool array
// ***************************************************************************
	// ***************************
	//		Start of info for player_data
	// ***************************

	#define P_RACE                  0		// Race
	#define P_SKILL1                1		// Skill 1 level
	#define P_SKILL2                2		// Skill 2 level
	#define P_SKILL3                3		// Skill 4 level
	#define P_ULTIMATE              4		// Ultimate level
	#define P_LEVEL					5		// Player Level
	#define P_XP                    6		// Current XP
	#define P_ITEM                  7		// Item from shopmenu 1
	#define P_ITEM2                 8		// Item from shopmenu 2

	// Miscellaneous options
	#define P_DEFUSERINDEX			9		// Player is the defuse
	#define P_SPECMODE				10		// Used to determine if the player is spectating or not
	#define P_SHOWRACEMENU			11		// Should we show the race menu when the player respawns?
	#define P_SHOWICONS				12		// Show player icons/levels for this player?
//	#define P_SPECFRIEND			13		// Determines if the player the user is looking at is on his/her team (used for icons)
	#define P_CHANGERACE			29		// Holds the value of what race to change to when the following round starts

	// Used for weapon Reincarnation
	#define P_FLASHCOUNT			14		// Number of flash grenades bought that round
	#define	P_HECOUNT				15		// Number of HE's bought that round
	#define P_SMOKECOUNT			16		// Number of smoke grenades bought that round
	#define P_ARMORONDEATH			17		// Amount of armor the player had when he/she died
	#define P_SAVEDNUMBER			19

	// Used by various ultimates/abilities
	#define P_TELEMENU				19		// Used by teleport
	#define P_CARRIONCOUNT			20		// Number of carrion beetles the player has left
	#define P_SHADOWCOUNT			21		// Number of shadow strikes the player has left
	#define P_FLAMECOUNT			22		// Number of flame shots the player has left
	#define P_SERPENTCOUNT			23		// Number of serpent wards the player has left
	#define P_RINGS					24
//	#define P_NADETIME				25
//	#define P_SERPENTTEAM			26
	#define P_SKINCHANGED			27		// Did the user's skin change?
	#define P_ULTIMATEDELAY			30
//	#define P_SIPHONMONEY			31

	// Used for DOD
	#define P_MONEY					28		// Amount of money the player has

	#define P_LAST					32
	// ***************************
	//		End of info for player_data
	// ***************************


	// ***************************
	//		Start of info for player_data_bool
	// ***************************

	// Miscellaneous
	#define PB_GAMECOMMENCING		0		// Was there a game commencing message? (change how this functions?  do we really need it to be [33]?)
	#define PB_RESETSKILLS			1		// Does the player want to reset their skills in the next round?
	#define PB_CHANGINGTEAM			2		// Is the user currently changing his/her team?
	#define PB_DIEDLASTROUND		4

	// Used for weapon Reincarnation
	#define PB_HELMET				5		// Player had a helmet when he/she died?
	#define PB_DEFUSE				6		// Player had a defuse kit when he/she died?
	#define PB_SHIELD				7		// Player had a shield when he/she died?
	#define PB_NIGHTVISION			8		// Player had nightvision when he/she died?

	// Used by various ultimates/abilities
	#define PB_GIVEITEMS			10		// Tells weapon controller function to give the player his items after respawning
	#define PB_PLAYERSPAWNED		11		// Did the player respawn from an ability?
	#define PB_SPAWNEDFROMITEM		12		// Did the player spawn from an item/teammate ability? (used to determine the health to give the player after they spawn)
	#define PB_NADEJUSTRECEIVED		13		// Used with flaming gloves... (removable?)
	#define PB_BLINKDELAYED			14		// Used to determine if the blink ability should be delayed (change this so its not [33])
	#define PB_MOLE					15		// Is the player a mole?
	#define PB_TOBEREVIVED			16		// Will this player be revived?
	#define PB_PHEONIXCASTER		17		// Did this player use his/her ability to give the team pheonix? (reviving)
	#define PB_WARDENBLINK			18		// Does this player have blink enabled?
	#define PB_SUICIDEATTEMPT		19		// Has this player made a suicide attempt for undead's ultimate?
	#define PB_ISBURNING			20		// Is the player burning from the flame strike ultimate?
	#define PB_EVADENEXTSHOT		21
	#define PB_STUNNED				22
	#define PB_SLOWED				23
	#define PB_ISSEARCHING			24
	#define PB_LIGHTNINGHIT			25
//	#define PB_HEALINGHIT			26
	#define PB_ULTIMATEUSED			27
	#define PB_KNIFESELECTED		28
	#define PB_SILENT				29
	#define PB_GODMODE				32

	#define PB_JUSTJOINED			30
	#define PB_ISCONNECTED			31
	
	#define PB_BUYZONE				33		// Player in buyzone?
	
//	#define PB_EVADENEXTWAR3DMG		34		// Should player evade next war3 damage? (NE's Evasion)
#if MOD == 1
	#define PB_REINCARNATION_DELAY	35
	#define PB_REINCARNATION_SKIP	36
#endif

	#define PB_LAST					37
	// ***************************
	//		End of info for player_data_bool
	// ***************************
// ***************************************************************************
//	End player array information
// ***************************************************************************


// Integer Array Defines
#define FT_RACE_ICONS						0
#define FT_LEVEL_ICONS						1
#define FT_SPEC_INFO						2
#define FT_MIN_B4_XP						3
#define	FT_BLINK_RADIUS						4
//#define	FT_STEAM							5
#define	MP_SAVEXP							6
#define	FT_SHOW_ICONS						7
#define	FT_HEALTH_BONUS						8
#define	FT_NO_GLOVES_ON_KA					9
#define	FT_GLOVE_ORC_DAMAGE					10
#define	FT_CLAW								11
#define	FT_CLOAK							12
#define	FT_OBJECTIVES						13
#define	FT_KILL_OBJECTIVES					14
#define	FT_ULTIMATE_DELAY					15
#define	FT_ULTIMATE_COOLDOWN				16
#define FT_RACES							17
#define FT_GLOVE_TIMER						18
#define FT_XPBONUS							19
#define FT_POSITION							20
#define FT_RACE9_RANDOM						21
#define FT_CD								22
#define FT_WARN_SUICIDE						23
#define FT_BLINKENABLED						24
#define SV_ALLOWWAR3VOTE					25
#define SV_MYSQL							26
#define SV_MYSQL_SAVE_END_ROUND				27
#define FT_SAVEBY							28
#define SV_MYSQL_AUTO_PRUNING				29
#define SV_DAYSBEFOREDELETE					30
//	#define FT_COMPETITIVE						31
#define FT_HEALING_RANGE					32
#define FT_XP_RADIUS						33
#define MP_WEAPONXPMODIFIER					34
#define FT_BLINK_PROTECTION					35
#define FT_BUYZONE							36
#define FT_BUYTIME							37
#define FT_BUYDEAD							38
#define	FT_HEADSHOT_BONUS					39
#define	FT_ROUND_WIN_XP						40
#define FT_BLINK_DIZZINESS					41
#define FT_ENTANGLE_DROP					42

#if MOD == 0
	#define	FT_ITEMS_IN_HUD						45
	#define	FT_NO_ORCNADES						46
	#define	FT_BOMBPLANTERXP					47
	#define	FT_DEFUSEXP							48
	#define	FT_HOSTAGEXP						49
	#define	FT_KILLRESCUEMANXP					50
	#define	FT_KILL_BOMB_CARRIER_BONUS			51
	#define	FT_DEFUSER_KILL_BONUS				52
	#define	FT_VIP_ESCAPE_BONUS					53
	#define	FT_VIP_KILL_BONUS					54
	#define	MP_GRENADEPROTECTION				55
#endif
#if MOD == 1
	#define FT_DOD_START_MONEY					45
#endif

// Float Array Defines
#define FT_BOT_BUY_ITEM						0
#define FT_FROST_SPEED						1
#define FT_MASK_OF_DEATH					2
#define MP_XPMULTIPLIER						3
#define AMX_VOTE_ANSWERS					4
#define	AMX_VOTEWAR3_RATIO					5
#define FT_SOCK								6
#if MOD == 0
	#define	FT_BOOTSPEED						7
#endif
#if MOD == 1
	#define DOD_BOOTSPEED						7
	#define DOD_STARTMONEY						8
#endif



// ***************************************************************************
//	Start CVAR Array Defines
// ***************************************************************************




// ***************************************************************************
//	End CVAR Array Defines
// ***************************************************************************

// ***************************************************************************
//  Start of variables
// ***************************************************************************

/* Variables for precaching sounds */

new SOUND_ANNIHILATION[64]
new SOUND_CONCOCTION_CAST[64]
new SOUND_CONCOCTION_HIT[64]

new SOUND_VENGEANCE[64]
new SOUND_SUICIDE[64]
new SOUND_BANISH[64]
new SOUND_LOCUSTSWARM[64]
new SOUND_SHADOWSTRIKE[64]
new SOUND_CARRION[64]
new SOUND_IMPALE[64]
new SOUND_VOODOO[64]
new SOUND_FLAMESTRIKE[64]
new SOUND_ENTANGLING[64]
new SOUND_LIGHTNING[64]
new SOUND_TELEPORT[64]
new SOUND_BLINK[64]
new SOUND_LEVELUP[64]
new SOUND_PICKUPITEM[64]
new SOUND_ITEM_TOME[64]
new SOUND_ULTIMATESCAN[64]
new SOUND_ULTIMATEREADY[64]

// Race9 Setup 
new race9Options[5] = {0,1,1,1,1}		// the value is what race that skill should be copied from so race9Options[1] = 1 means that skill1 is undead skill1 this means you can not have skill 1 form more than one race.
										// this default setting of 0,1,1,1,1 will make the 9th race have all the undead skills 
										// note that the first value does not do anything.

new p_data[33][P_LAST]					// Contains player data
new bool:p_data_b[33][PB_LAST]			// Contains player data of type boolean

new iCvar[56]							// Contains integer CVAR data (loaded at plugin start)
new Float:fCvar[9]						// Contains float CVAR data (loaded at plugin start)

#if ADVANCED_STATS
	new iStatsHead[33][MAX_CSW_S]
	new iStatsChest[33][MAX_CSW_S]
	new iStatsStomach[33][MAX_CSW_S]
	new iStatsLeftArm[33][MAX_CSW_S]
	new iStatsRightArm[33][MAX_CSW_S]
	new iStatsLeftLeg[33][MAX_CSW_S]
	new iStatsRightLeg[33][MAX_CSW_S]
	new iStatsKills[33][MAX_CSW_S]
	new iStatsDeaths[33][MAX_CSW_S]
	new iStatsHS[33][MAX_CSW_S]
	new iStatsTKS[33][MAX_CSW_S]
	new iStatsShots[33][MAX_CSW_S]
	new iStatsHits[33][MAX_CSW_S]
	new iStatsDamage[33][MAX_CSW_S]
#endif

new MAXPLAYERS

// Used by admin menu
new g_menuPosition[33]
new g_menuPlayers[33][32]
new g_menuPlayersNum[33]
new g_menuOption[33]
new g_menuSettings[33]

#if MOD == 0	
	new gmsgStatusText
	new gmsgBarTime

	new Float:g_fBombTime
	new g_hostageSaver
	new g_bombCarrier
	new g_bombDefuser
	new g_vipID = 0
	new bool:g_freezetime = false
	new g_freezecalled = 0
	new bool:g_buyTime
	new bool:g_buyCalled = false
	new bool:g_givePistol
	new bool:g_giveHE
	new bool:g_notAllowHE = false

	new CTSkins[5][]={"sas","gsg9","urban","gign","spetsnaz"}
	new TSkins[5][]={"arctic","leet","guerilla","terror","militia"}
	new spawnEntString[2][] = {"info_player_start","info_player_deathmatch"}
#endif
#if MOD == 1
	//new gmsgObject
	//new gmsgClientAreas
	new gmsgHudText
	//new AlliedSkins[2][]={"us-inf","us-para"}
	//new AxisSkins[2][]={"axis-inf","axis-para"}
	new spawnEntString[2][] = {"info_player_axis","info_player_allies"}

	new reincarnation[33][3]
#endif

// Captured Messages
new gmsgDeathMsg
new gmsgScreenFade
new gmsgScreenShake
new gmsgScoreInfo
new gmsgStatusIcon

new bool:g_mapDisabled = false
new bool:g_randomizeCalled = false
new bool:g_spritesEnabled = false
new Float:g_ultimateDelay = 0.0
new g_lastAnnounce = -1

// Abilities
new g_pheonixExistsT = 0
new g_pheonixExistsCT = 0
new bool:spawnPointsused[33] = false

// Sprites
new g_sShadow
new g_sSnow
new g_sFire 
new g_sBurning 
new g_sBeetle
new g_sFireball
new g_siBeam4
new g_siTrail
new g_sSpriteTexture
new g_sLightning
new g_sFlare
new g_sWave

#if MOD != 1
	new g_sRace[10]
	new g_sLevel[11]
	new g_sSmoke
#endif

new Sql:mysql
new mysqltablename[64]
new iSQLAttempts = 0

new g_MOD[8] = "WAR3FT"
new g_MODclient[15] = "* [WAR3FT]"

new teleportid[33][32]

new FT_control
new	FT_start
new	FT_stop
new	FT_message

new iglow[33][4]
new savedweapons[33][32]
new bool:warcraft3
new bool:endround

// Constants for Abilities
new const Float:p_vampiric[3] =			{0.15,0.30,0.45}		// Vampiric Aura			(skill 1)
#if MOD == 0
	new const Float:p_unholy[3] =		{265.0,285.0,300.0}		// Unholy Aura				(skill 2)
#endif
#if MOD == 1
	new const Float:p_unholy[3] =		{25.0,50.0,100.0}		// Unholy Aura				(skill 2)
#endif
new const Float:p_levitation[3] =		{0.8,0.6,0.4}			// Levitation				(skill 3)

new const p_invisibility[3] =			{200,150,100}			// Invisibility				(skill 1)
new const p_devotion[3] =				{115,130,145}			// Devotion Aura			(skill 2)
new const Float:p_bash[3] =				{0.15,0.30,0.45}		// Bash						(skill 3)

new const Float:p_critical[3] =			{0.15,0.15,0.15}		// Critical Strike			(skill 1)
new const Float:p_grenade[3] =			{2.0,3.0,4.0}			// Critical Grenade			(skill 2)
new const Float:p_ankh[3] =				{0.333,0.666,1.0}		// Equipment reincarnation	(skill 3)

new const Float:p_evasion[3] =			{0.1,0.2,0.3}			// Evasion					(skill 1)
new const Float:p_thorns[3] =			{0.05,0.1,0.15}			// Thorns Aura				(skill 2)
new const Float:p_trueshot[3] =			{0.15,0.3,0.45}			// Trueshot Aura			(skill 3)

new const Float:p_pheonix[3] =			{0.333,0.666,1.0}		// Pheonix					(skill 1)
new const Float:p_banish[3] =			{0.07,0.13,0.20}		// Banish					(skill 2)
new const Float:p_mana[3] =				{0.02,0.04,0.08}		// Siphon Mana				(skill 3)
new const Float:p_resistant[11] =		{0.0, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50}	// Resistant Skin		(Skill 4)

new const Float:p_heal[3] =				{6.0,4.0,2.0}			// Healing Wave				(skill 1)
new const Float:p_hex[3] =				{0.05,0.10,0.15}		// Hex						(skill 2)
new const p_serpent[3] =				{1,2,3}					// Serpent Ward				(skill 3)
new const Float:p_concoction[11] =		{0.0, 0.01, 0.02, 0.04, 0.06, 0.08, 0.10, 0.13, 0.15, 0.17, 0.20}	// Unstable Concoction	(Skill 4)

new const Float:p_fan[3] =				{0.05,0.10,0.15}		// Fan of Knives			(skill 1)
new const Float:p_blink[3] =			{0.333,0.666,1.0}		// Blink					(skill 2)
new const Float:p_shadow[3] =			{0.15,0.30,0.45}		// Shadow Strike			(skill 3)
new const Float:p_harden[11] =			{0.0, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50}	// Hardened Skin		(Skill 4)

new const Float:p_impale[3] =			{0.5,0.10,0.20}			// Impale					(skill 1)
new const Float:p_spiked[3] =			{0.05,0.1,0.15}			// Spiked Carapace			(skill 2)
new const Float:p_carrion[3] =			{0.15,0.25,0.45}		// Carrion Beetle			(skill 3)
new const Float:p_orb[11] =				{0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.15}	// Orb of Annihilation	(Skill 4)


new xplevel[11] =						{0,150,300,600,1000,1500,2100,2800,3400,4500,5500}
new xpgiven[11] =						{10,15,25,35,40,50,60,70,80,90,95}

new Float:weaponxpmultiplier[62] =		{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0}

new const itemcost[9] =					{1500,1500,1000,800,2000,800,2000,1000,4000}
#if MOD == 0
	new const itemcost2[9] =				{5000,1500,550,1500,1500,1750,1000,9000,16000}
#endif
#if MOD == 1
	new const itemcost2[9] =				{800,1500,550,1500,1500,1750,1000,9000,16000}
#endif

new MOTD_header[] = "<html><head><style type=^"text/css^">#title{font-family:^"Verdana, Arial, Helvetica, sans-serif^";color:#00FF00;text-align:center;font-weight:bold;}#s{text-indent:35px;width:650px;}ul{margin-top:0px;margin-bottom:10px;}</style></head><body bgcolor=#000000 text=#FFB000>"