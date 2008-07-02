#define MAX_RACES		4
#define MAX_LEVELS		10

#define RACE_NONE				0
#define RACE_UNDEAD				1
#define RACE_HUMAN				2
#define RACE_ORC				3
#define RACE_ELF				4


// from shake.h
#define SCREENFADE_FRACBITS	9		
#define FFADE_IN			0x0001		// Just here so we don't pass 0 into the function
#define FFADE_OUT			0x0002		// Fade out (not in)
#define FFADE_MODULATE		0x0004		// Modulate (don't blend)
#define FFADE_STAYOUT		0x0008		// ignores the duration, stays faded out until new ScreenFade message received
#define FFADE_PURGE			0x0010		// Purges all other fades, replacing them with this one

// Player Data Indexes
#define PLAYER_RACE         0
#define PLAYER_XP           1
#define PLAYER_LEVEL        2
#define PLAYER_CHANGERACE   3
#define PLAYER_END          4

// Player Status Indexes
#define STATUS_PLANTING     0
#define STATUS_END          1

new g_PlayerData[MAXPLAYERS + 1][PLAYER_END];
new g_PlayerStatus[MAXPLAYERS + 1][STATUS_END];

// ************************
// Round State
enum RoundState
{
	RoundState_Unknown = 0,		// Unknown
	RoundState_InFreezeTime,	// During freeze time
	RoundState_InProgress,		// Round is in progress
	RoundState_End 				// Round ended
};

new RoundState:g_RoundState = RoundState:RoundState_Unknown;

// ************************
// Offsets
new OffSet_LifeState;



//**************** MENUS ****************
new Handle:g_MenuChangeRace = INVALID_HANDLE;





new bool:g_IsGameRunning = true;