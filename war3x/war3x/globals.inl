// Begin GLOBALS.INL


/* - Race/Skill/Item Constants ---------------------------------- */

#define VALUE_BOOTS               275.0     // (  float) max speed value

#define RACE_UNDEAD                 1     // Player race references
#define RACE_HUMAN                  2
#define RACE_ORC                    3
#define RACE_NIGHTELF               4

#define OPTION_RACEICONS            0     // Player option references
#define OPTION_HEALICONS            1
#define OPTION_DISPELLICONS         2
#define OPTION_AUTOCAST             3
#define OPTION_RESOLUTION           4

#define TOTAL_RACES                 4     // Total races
#define TOTAL_SHOPITEMS             8     // Total shopmenu items
#define TOTAL_COMMONITEMS           0     // Total common racial items
#define TOTAL_RACEITEMS             0     // Total racial shopmenu items
#define TOTAL_ULTIMATES             3     // Total ultimates for each race
#define TOTAL_SKILLSTRAINED         3     // Total trainable skills for each race
#define TOTAL_SKILLSFREE            1     // Total free skills for each race
#define TOTAL_LEVELS               10     // Total levels that can be gained ( not fully implemented )
#define TOTAL_SKILLLEVELS           3     // Total skill levels for each skill
#define TOTAL_CLASSES               6     // Total classes
#define TOTAL_OPTIONS               5     // Total options

#define TOTAL_QUEUES  32 + MAX_QUEUES     // Total queue slots

#define _TR               TOTAL_RACES     // Array Number Reference
#define _TST      TOTAL_SKILLSTRAINED
#define _TSF         TOTAL_SKILLSFREE
#define _TU           TOTAL_ULTIMATES
#define _TSI          TOTAL_SHOPITEMS
#define _TCI        TOTAL_COMMONITEMS
#define _TRI          TOTAL_RACEITEMS
#define _TC             TOTAL_CLASSES
#define _TO             TOTAL_OPTIONS
#define _TQ              TOTAL_QUEUES

#define TOTAL_SKILLS      _TST + _TSF     // Total skills for each race
#define TOTAL_SHOP2ITEMS  _TCI + _TRI     // Total items in racial shop

#define _TS              TOTAL_SKILLS
#define _TSI2        TOTAL_SHOP2ITEMS


/* - Item Configuration ----------------------------------------- */


#define ITEM_NONE                     0     // Item reference
#define ITEM_ANKH                     1
#define ITEM_BOOTS                    2
#define ITEM_CLAWS                    3
#define ITEM_CLOAK                    4
#define ITEM_MASK                     5
#define ITEM_AMULET                   6
#define ITEM_RING                     7
#define ITEM_TOME                     8


/* - Skill Configuration ---------------------------------------- */

#define SKILL_RACIAL                  0     // Generic
#define SKILL_1                       1
#define SKILL_2                       2
#define SKILL_3                       3
#define SKILL_INCENTIVE               4


/* - Ultimate Configuration ------------------------------------- */


#define ULTIMATE_DEATHCOIL            1     // Undead Ultimates
#define ULTIMATE_IMPALE               2
#define ULTIMATE_SLEEP                3

#define ULTIMATE_HOLYLIGHT            1     // Human Ultimates
#define ULTIMATE_FLAMESTRIKE          2
#define ULTIMATE_AVATAR               3

#define ULTIMATE_HEALINGWAVE          1     // Orc Ultimates
#define ULTIMATE_LIGHTNING            2
#define ULTIMATE_WINDWALK             3

#define ULTIMATE_REJUVENATION         1     // Night Elf Ultimates
#define ULTIMATE_ROOTS                2
#define ULTIMATE_SHADOWSTRIKE         3

#define ULTIMATE_1              _TS + 0     // Generic
#define ULTIMATE_2              _TS + 1
#define ULTIMATE_3              _TS + 2

new const HEAL_TARGETS[_TR][] =             // Heal targets (NOT FULLY IMPLEMENTED)
{
    "a",
    "b",
    "abcd",
    "abcd"
};

/*
new const DISPELL_TARGETS[_TR][] =
{
    NOT IMPLENENTED
}
*/


/* - Miscellaneous Configuration -------------------------------- */


#define SPEED_IMMOBILIZE            0.1     // (  float) max speed when rooted / bashed (0.0 does not work)
#define SPEED_KNIFE               250.0     // (  float) max speed when wielding knife

#define HEAL_NOTIFYHEALTH           0.5     // (  float) percentage health to play blink heal icon above low health players (if healing ultimate available)
#define ARMOR_MAXFADE                96     // (integer) maximum screenfade ammount when buying armor with armor skill (0-255)

#define ITEM_BUYBACK                  2     // (integer) money given back when purchasing item and one present (1/X)

#define ANKH_AMMOCLIPS                2     // (integer) number of clips to give each weapon on reincarnation

#define CAP_SPEEDBONUS            310.0     // (  float) maximum speed attainable by means of a skill
#define CAP_INVISIBILITY           80.0     // (  float) maximum invisibility level attainable

#define DURATION_ULTIMATEPING         5     // (integer) seconds ultimate warmup is audible
#define ULTIMATE_COOLDOWNDEFAULT   45.0     // (  float) seconds of cooldown after casting ultimate

#define BOT_ULTIMATECHANCE         0.33     // (  float) chance a bot will cast ultimate (if available) when targetting
#define BOT_BUYCHANCE              0.75     // (  float) chance a bot will buy item at round start

#define LEVEL_ULTIMATE                6     // (integer) level required to get ultimate
#define LEVEL_RACIALCAP             9.0     // (  float) level when racial abilities are maximum

#define RACEOPTIONS_SKILLS            1     // (integer) level that reset skills menu is available
#define RACEOPTIONS_ULTIMATE          6     // (integer) level that reset ultimate menu is available

#define SKILLDAMAGE_KNIFE           1.0     // (  float) percent of skill damage that penetrates armor (hp damage)
#define SKILLDAMAGE_PISTOL          0.1     // (  float) percent of skill damage that penetrates armor (hp damage)
#define SKILLDAMAGE_SHOTGUN        0.25     // (  float) percent of skill damage that penetrates armor (hp damage)
#define SKILLDAMAGE_SMG            0.25     // (  float) percent of skill damage that penetrates armor (hp damage)
#define SKILLDAMAGE_RIFLE          0.35     // (  float) percent of skill damage that penetrates armor (hp damage)
#define SKILLDAMAGE_SNIPER          0.5     // (  float) percent of skill damage that penetrates armor (hp damage)
#define SKILLDAMAGE_MG              0.5     // (  float) percent of skill damage that penetrates armor (hp damage)
#define SKILLDAMAGE_GRENADE         0.5     // (  float) percent of skill damage that penetrates armor (hp damage)

#define SKILLDAMAGE_MULTIPLIER      1.5     // (  float) multiplier for armor lost on skill damage

new const GETSKILLCHECK[3] =    {1,3,5};    // (integer) levels can you get same skill upgrades
new const GETCLASSCHECK[3] =    {0,2,5};    // (integer) levels players receive new class titles


/* - Reference Constants ---------------------------------------- */


#define CURRENT_RACE                  0     // Player session array reference
#define CURRENT_XP                    1
#define CURRENT_SKILL1                2
#define CURRENT_SKILL2                3
#define CURRENT_SKILL3                4
#define CURRENT_ULTIMATE              5
#define CURRENT_ITEM                  6
#define CURRENT_KILLSTREAK            7
#define CURRENT_DEATHSTREAK           8
#define CURRENT_NEXTRACE              9
#define CURRENT_PLAYTIME             10

#define OFFSET_CURRENT_SKILLS         2     // Offset for skills in PlayerInfo array

#define CURRENT_PRIMARY               0     // Backpack array reference values ( reincarnation )
#define CURRENT_SECONDARY             1
#define CURRENT_HEGRENADE             2
#define CURRENT_FLASHBANG             3
#define CURRENT_SMOKEGRENADE          4
#define CURRENT_ARMOR                 5
#define CURRENT_ARMORTYPE             6
#define CURRENT_DEFUSE                7

#define CACHE_MONTH                   1     // Player cache array reference
#define CACHE_DAY                     2
#define CACHE_YEAR                    3
#define CACHE_ICONS                   4

#define KEY_RANDOM                    4
#define KEY_HELP                      8
#define KEY_CANCEL                    9

#define TASK_REGEN                    0     // Task id numbers
#define TASK_FROSTSLOW               32
#define TASK_GLOW                    64
#define TASK_EVASION                 96
#define TASK_INVIS                  128
#define TASK_BASH                   160
#define TASK_SLOW                   192
#define TASK_IMPALE                 224
#define TASK_STRIKE                 256
#define TASK_SLEEP                  288
#define TASK_REJUV                  320
#define TASK_ULTIMATECOOLDOWN       352
#define TASK_ANTIMAGICSHELL         384
#define TASK_ROOT                   416
#define TASK_DETONATE               448
#define TASK_FLAMESTRIKE_VICTIM     480
#define TASK_FLAMESTRIKE_CASTER     512
#define TASK_AVATAR                 544
#define TASK_MOONGLAIVE             576
#define TASK_SHADOWSTRIKE           608
#define TASK_WINDWALK               640
#define TASK_ANKH                   672
#define TASK_ULTIMATE               704
#define TASK_TARGETNAME             736
#define TASK_NOVASLOW               768

#define TASK_ULTIMATEWARMUP        1000
#define TASK_NEWROUND              1001
#define TASK_SHOWITEM              1002
#define TASK_TEMPENTITY            2000

#define RACEINFO_RACIAL               0     // Race info reference values
#define RACEINFO_SKILL1               1
#define RACEINFO_SKILL2               2
#define RACEINFO_SKILL3               3
#define RACEINFO_ULTIMATE1            4
#define RACEINFO_ULTIMATE2            5
#define RACEINFO_ULTIMATE3            6
#define RACEINFO_CLASSNAME            7

#define GLOW_R                        0     // Glow reference values
#define GLOW_G                        1
#define GLOW_B                        2
#define GLOW_ID                       0
#define GLOW_AMMOUNT                  1
#define GLOW_FADEOUT                  2
#define GLOW_MAX                    255

#define GLOW_DURATION_BASE        0.025     // Glow calc numbers
#define GLOW_DURATION_MIN           0.5
#define GLOW_DURATION_MAX           5.0

#define SLOW_FROST                    1     // Slow types
#define SLOW_ELF                      2

#define IMMOBILIZE_BASH               1     // Immobilize types
#define IMMOBILIZE_ROOT               2

#define DAMAGE_NOCHECKARMOR           0     // WAR3_damage check armor
#define DAMAGE_CHECKARMOR             1

#define ULTIMATE_TARGET_SELFONLY  (1<<1)    // Ultimate target types
#define ULTIMATE_TARGET_SELF      (1<<2)
#define ULTIMATE_TARGET_TEAM      (1<<3)
#define ULTIMATE_TARGET_ENEMY     (1<<4)
#define ULTIMATE_TARGET_NONE      (1<<5)

#define ULTIMATE_TYPE_HEAL        (1<<1)    // Ultimate types
#define ULTIMATE_TYPE_OFFENSIVE   (1<<2)
#define ULTIMATE_TYPE_DEFENSIVE   (1<<3)
#define ULTIMATE_TYPE_DISPELL     (1<<4)

#define HUDMESSAGE_POS_STATUS      0.85
#define HUDMESSAGE_POS_SERVER      0.70
#define HUDMESSAGE_POS_INFO        0.20
#define HUDMESSAGE_POS_LEVEL       0.25
#define HUDMESSAGE_POS_CENTER      -1.0

#define HUDMESSAGE_CHAN_LEVEL         1     // Level information centered @ mid-top of screen
#define HUDMESSAGE_CHAN_ITEM          2     // Race/Item Information @ bottom left of screen
#define HUDMESSAGE_CHAN_XP            3     // XP Information @ bottom left of screen
#define HUDMESSAGE_CHAN_INFO          3     // Misc info centered @ top of screen
#define HUDMESSAGE_CHAN_STATUS        4     // WAR3 status text centered @ bottom of screen

#define HUDMESSAGE_CHAN_SERVER        2     // Server messages centered @ mid-bottom of screen
#define HUDMESSAGE_CHAN_DEAD          2     // Follow message (spectator)

#define HUDMESSAGE_FREQ_ITEM       15.0     // Frequency of RACE/LEVEL/ITEM Hudmessage

#define LEVEL_1                       0
#define LEVEL_2                       1
#define LEVEL_3                       2

#define ARMOR_AMMOUNT                 0
#define ARMOR_TYPE                    1

#define ARMORTYPE_KEVLAR              0
#define ARMORTYPE_HELMET              1

#define CSW_WORLDSPAWN                0     // Weapon ids

#if !defined CSW_GALI
   #define CSW_GALI                  14
#endif

#if !defined CSW_FAMAS
   #define CSW_FAMAS                 15
#endif

#define CSW_SHADOWSTRIKE             31
#define CSW_PULVERIZE                32
#define CSW_IMPALE                   33
#define CSW_HOLYLIGHT                34
#define CSW_LIGHTNING                35
#define CSW_ROOT                     36
#define CSW_FLAMESTRIKE              37
#define CSW_DEATHCOIL                38
#define CSW_WINDWALK                 39
#define CSW_MOONGLAIVE               40
#define CSW_FROSTNOVA                41


/* - XP Saving Constants ---------------------------------------- */


#define XP_ROUND                    0     // reference to the iXP_support/normal arrays
#define XP_TOTAL                    1


/* - Database Constants ----------------------------------------- */


#define SQL_FAIL				-1
#define SQL_NONE				0
#define SQL_MYSQL				1
#define SQL_SQLITE				2

#define SQLITE_SYNC_OFF				0
#define SQLITE_SYNC_NORMAL			1
#define SQLITE_SYNC_FULL			2

/* - Global Variables ------------------------------------------- */


new SPR_BLOODDROP;                  // Sprite Precache Ids
new SPR_BLOODSPRAY;
new SPR_BEAMFOLLOW;
new SPR_CHAINLIGHTNING;
new SPR_SMOOTHBEAM;
new SPR_ROOT;
new SPR_SHOCKWAVE;
new SPR_SMOOTHWAVE;
new SPR_FLAMEBURST;
new SPR_DEATHCOIL;

new MDL_HORNET;                     // Model Precache Ids

new ICON_UNDEAD;                    // Icon Precache Ids
new ICON_HUMAN;
new ICON_ORC;
new ICON_NIGHTELF;

new ICON_SAVEME;
new ICON_DISPELLME;

new g_Vip;                          // Objective Variables
new g_OldVip;
new g_BombCarrier;
new g_BombPlanter;
new g_BombDefuser;
new g_iHostageKills[33];

// DB Variables
new g_iSQLtype = SQL_NONE;
new g_SQLtype[16];
new bool:g_DBILoaded = true;
new const g_MySQL[] = "MySQL"
new const g_SQLite[] = "SQLite"
new g_mysqlTableName[32];

new bool:g_bTouchHostage[33];
new bool:g_bBombPlanted;            // determines whether to penalize t's when target saved.
new bool:g_bHostageRescued;         // determines whether to penalize ct's when all hosties have not been rescued.

new g_iPlayerArmor[33][2];          // stores player armor / armor type.
new g_iPlayerAmmo[33][31];          // Player ammo. Stored to calculate change on CurWeapon.
new g_iDispellHealth[33];           // stores war training bonus hps when hit with dispell

new g_PlayerInfo[33][12];           // Player session array. Holds all session data.
new g_PlayerOptions[33][_TO];       // Player session array. Holds all client-side options.

new g_PlayerBackpack[33][8];        // Player session array. Holds all weapon / item data.
new g_PlayerTarget[33];             // stores player target ID
new g_PlayerTargetTeam[33];         // stores player target TEAM
new g_TargetFormat[33][128];        // Stores format for target information

new bool:g_bPlayerConnected[33];    // Stores whether player is connected ( message-sensitive var )
new bool:g_bGotDefuser[33];         // true when player has defuse kit

new bool:g_bArmorDepleted[33];      // stores whether armor depleted or not
new bool:g_bNewTargetType[33];      // Stores if new target type aquired

new g_ChainJumps[33][10];           // Stores ids of players affected by chain ultimates

new g_PlayerRooted[33];             // Player effect arrays. Stores misc information
new g_PlayerOnFire[33];
new g_PlayerSingeing[33];
new g_PlayerImpaled[33];
new g_PlayerStruck[33];
new g_PlayerRejuv[33];

new bool:g_bPlayerRegen[33];        // Player effect arrays
new bool:g_bPlayerCantMove[33];
new bool:g_bPlayerSlowed[33];
new bool:g_bPlayerGlowing[33];
new bool:g_bPlayerZoomed[33];
new bool:g_bPlayerDispelled[33];
new bool:g_bPlayerBashed[33];
new bool:g_bPlayerFrosted[33];
new bool:g_bPlayerNova[33];
new bool:g_bPlayerSleeping[33];
new bool:g_bPlayerWalk[33];
new bool:g_bPlayerInvis[33];
new bool:g_bPlayerAvatarGrow[33];

new bool:g_bEvadeNextShot[33];      // Misc skill arrays
new bool:g_bInvisCooldown[33];
new bool:g_bFlameStrikeCast[33];
new bool:g_bPlayerRespawned[33];

new g_iHealingWaveHealth[33];       // Skill ammount arrays
new g_iRejuvHealth[33];
new g_iWindwalkDamage[33];
new g_iEvasionDamage[33];
new g_iPlayerAvatar[33];

new Float:g_fEvasionTime[33];
new Float:g_fAmuletTime[33];

new bool:g_bPlayerSaveMe[33];
new bool:g_bPlayerDispellMe[33];

/* - Skill Variables -------------------------------------------- */


new playerSkill1Info[33][2];        // Player skill data. Used in newround skill data hudmessage
new playerSkill2Info[33][2];
new playerSkill3Info[33][2];
new playerRoundXp[33][2];

new bool:g_bAnkhDropWeapons[33];    // Store whether to give ankh items after weapon drop

new g_iCurrentRound;                // Current round
new bool:g_bFreezeTime;             // Stores whether freezetime is active
new bool:g_bRoundEnd;               // Stores whether endround cooldown is in effect

new Float:g_fBuyTime;               // Stores gametime for round start to get accurate buytime for war3x_shoptime
new Float:g_fBombTime;              // Stores gametime of bomb explosion
new Float:g_fUltimateCooldown[33];  // Stores gametime of ultimate cast (for time remaining)
new Float:g_fCommandFlood[33];     // Stores anti-flood time for ultimate command.

new g_iChargeUltimate[33];          // Countdown when charging ultimate

new bool:g_bUltimateWarmup;         // Stores whether min ultimate timer has been reached
new bool:g_bChangeUltimate[33];     // Stores whether ultimate skill has been changed in-round.
new bool:g_bInBuyZone[33];          // determines whether player is in buyzone

new bool:g_bSaveXp;                 // stores value of war3x_save_xp at round beginning
new g_SaveIds[33][32];              // stores player save id (auth/ip)

new bool:g_bWar3xEnabled;           // stores whether war3x is enabled for current map
new bool:g_bMapRestricted;          // stores whether map is restricted to Long-Term XP
new bool:g_bMapDisabled;            // stores whether war3x is disabled on current map
new bool:g_bDisabledNotify[33];     // stores whether player has been notified of disabling

new g_RestrictedSkills[64];         // stores restricted skills for current map
//new g_bRestrictedRace;              // stores whether or not player is informed their current race is restricted on map
//new g_bRestrictedSkills;            // stores whether or not player is informed their current race is restricted on map
//new g_bRestrictedUltimate;          // stores whether or not player is informed their current race is restricted on map

new g_iVoteCounts[2];               // stores votes for/against
new Float:g_fVoteRunning;           // stores vote end time
new Float:g_fVoteRatio;             // stores current ratio

new g_iLevelXp[11];                 // (integer) stores xp required for levelups
new Float:g_fWeaponXp[42];


/* - Saving Globals --------------------------------------------- */

new g_iPlayerRounds[33];            // stores total rounds player has been playing (will be used for multiple things)

new g_iXPtotal[33][_TR];            // total xp for all races stored here ( this is where we store fetched xp )

new g_iXPkill[33][2];               // stores xp gained from kills for current round/map
new g_iXPobjective[33][2];          // stores xp gained from objectives for current round/map
new g_iXPsupport[33][2];            // stores xp gained from support skills for current round/map

new bool:g_bXPfetched[33];          // true when xp has been fetched from database

new bool:g_bStoreXp[33];            // store player xp to db ( false if data unchanged, saves a query )
new bool:g_bStoreSkills[33];        // store player skills to db ( false if data unchanged, saves a query )
new bool:g_bStoreOptions[33];       // store player options to db ( false if data unchanged, saves a query )

new g_SaveQueue_iTotal;             // stores total queues

new g_SaveQueue_ids[_TQ][32];       // holds authid of players (match with g_SaveQueue_iXP)
new g_SaveQueue_iXP[64][_TR];       // holds xp for all races.
new g_SaveQueue_iInfo[64][_TS + 3]; // holds skills for race to be saved.
new g_SaveQueue_iOptions[64][_TO];  // holds options for all races.


/* - Stats Variables -------------------------------------------- */


#define STATS_ACTIVE                // Stores how many times skill(s) activate
#define STATS_TOTAL                 // Stores damage totals (to calculate % with STATS_ACTIVE)
#define STATS_AMMOUNT               // Stores skill ammount(s)
#define STATS_KILLS                 // Stores skill kill count
/*
stock rStats_Skill_1[33][4];
stock rStats_Skill_2[33][4];
#if ADVANCED_DEBUG
	log_function("stock rStats_Skill_1[33][4];");
#endif
stock rStats_Skill_3[33][4];
stock rStats_Ultimate[33][4];
#if ADVANCED_DEBUG
	log_function("stock rStats_Skill_3[33][4];");
#endif
stock rStats_Experience[33][4];
//stock rStats_Item[33][TOTAL_ITEMS];
#if ADVANCED_DEBUG
	log_function("stock rStats_Experience[33][4];");
#endif

stock tStats_Skill_1[33][4];
stock tStats_Skill_2[33][4];
#if ADVANCED_DEBUG
	log_function("stock tStats_Skill_1[33][4];");
#endif
stock tStats_Skill_3[33][4];
stock tStats_Ultimate[33][4];
#if ADVANCED_DEBUG
	log_function("stock tStats_Skill_3[33][4];");
#endif
stock tStats_Experience[33][4];
//stock tStats_Item[33][TOTAL_ITEMS];
#if ADVANCED_DEBUG
	log_function("stock tStats_Experience[33][4];");
#endif
*/


/* - Sound Precaching Variables --------------------------------- */


new SOUND_LEVELUP[64];
new SOUND_PICKUPITEM[64];
new SOUND_TOME[64];
new SOUND_ULTIMATESCAN[64];
new SOUND_ULTIMATEREADY[64];
new SOUND_IMPALE[64];
new SOUND_DEATHCOIL[64];
new SOUND_SLEEP[64];
new SOUND_HOLYLIGHT[64];
new SOUND_FLAMESTRIKE[64];
new SOUND_FLAMESTRIKE_BURN[64];
new SOUND_AVATAR[64];
new SOUND_PULVERIZE[64];
new SOUND_HEALINGWAVE[64];
new SOUND_CHAINLIGHTNING[64];
new SOUND_WINDWALK[64];
new SOUND_REJUVENATION[64];
new SOUND_ROOTS[64];
new SOUND_SHADOWSTRIKE[64];


/* -------------------------------------------------------------- */


new const HTML_HEADER[] = "<html><head><link rel='stylesheet' href='war3x.css'></head><body><pre>";


/* -------------------------------------------------------------- */

// End of GLOBALS.INL