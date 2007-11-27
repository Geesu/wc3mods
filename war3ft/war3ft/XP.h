/*
*	XP Header File
*/

// Objective Modifiers
#define DEFUSING_BOMB		10			// XP awarded when the user starts to defuse the bomb
#define DEFUSED_BOMB		20			// XP awarded when the user defuses the bomb
#define PLANTING_BOMB		10			// XP awarded when the user starts planting the bomb
#define PLANT_BOMB			20			// XP awarded when the user plants the bomb
#define SPAWN_BOMB			10			// XP awarded when the user spawns with the bomb
#define BOMB_PICKUP			10			// XP awarded when the user picks up the bomb
#define TOUCH_HOSTAGE		10			// XP awarded when the user touches a hostage
#define RESCUE_HOSTAGE		20			// XP awarded when the user rescues the hostage
#define KILL_HOSTAGE		10			// XP lost when killing a hostage
#define SPAWN_VIP			10			// XP awarded for spawning as the VIP
#define ESCAPE_VIP			20			// XP awarded for escaping as the VIP
#define OBJ_RADIUS			500			// Nearby radius to award XP for helping complete objectives

// Kill modifiers
#define KILL_HEADSHOT		10			// XP awarded for getting a headshot
#define KILL_HOSTAGE_SAVER	10			// XP awarded for killing the hostage saver
#define	KILL_DEFUSER		10			// XP awarded for killing the defuser
#define KILL_PLANTER		10			// XP awarded for killing the planter
#define KILL_BOMB_CARRIER	10			// XP awarded for killing the bomb carrier
#define KILL_VIP			20			// XP awarded for killing the VIP
#define KILL_RADIUS			250			// Nearby radius to award XP

#define WIN_ROUND			20			// XP awarded for winning the round


// Holds information about the player
enum
{
	PLR_BOMB_DEFUSER = 1,
	PLR_BOMB_PLANTER,
	PLR_HOSTAGE_RESCUER,
	PLR_VIP,
	PLR_BOMB_CARRIER,
};

new g_iPlayerRole[33];
new bool:bHasBegunPlantingOrDefusing[33];

// Holds the XP Multipliers per weapon
new Float:fWpnXPMultiplier[CSW_WAR3_MAX+1] =		{1.0};

// Amount of XP needed to gain a level
new iXPLevelShortTerm[11]				= {0,150,300,600,1000,1500,2100,2800,3400,4500,5500};
new iXPLevelSaved[11]					= {0,100,200,400,800,1600,3200,6400,12800,25600,51200};

// Amount of XP awarded when killing a user of this level
new iXPGivenShortTerm[11]				= {10,15,25,35,40,50,60,70,80,90,95};
new iXPGivenSaved[11]					= {6,8,10,12,14,16,18,20,24,28,32};