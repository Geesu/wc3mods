/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Common DB Header File
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

// Type of SQlite database synchronization
#define SQLITE_SYNC_OFF				0
#define SQLITE_SYNC_NORMAL			1
#define SQLITE_SYNC_FULL			2

// Based on what we're saving by (FT_saveby)
#define DB_SAVEBY_STEAMID			0
#define DB_SAVEBY_IP				1
#define DB_SAVEBY_NAME				2

// Used with g_DBType
#define DB_SQLITE			0
#define DB_MYSQLX			1

// Lets us know that the DB is ready
new bool:bDBAvailable = false;

// SQLX
new Handle:g_DBTuple;
new Handle:g_DBConn;

// SQLite
//new Sql:g_DB;
new g_DBType = -1;

// Player's Unique ID
new g_iDBPlayerUniqueID[33];
new g_iDBPlayerSavedBy[33];

// Lets us store what level the skill was the last time we saved (so we don't save more than necessary)
new g_iDBPlayerSkillStore[33][MAX_SKILLS];
new g_iDBPlayerXPInfoStore[33][MAX_RACES];

// Function Declarations
forward DB_DetermineType();
forward DB_Init();
forward DB_Close();
forward DB_SaveXP( id );
forward DB_GetKey( id, szKey[], len );
forward DB_GetKeyName( szKeyName[], len );
forward DB_GetAllXP( id );
forward DB_SetDataForRace( id );
forward DB_FormatString( text[], len );
forward DB_UpdateTimestamp( id );
forward DB_Prune();
forward DB_FetchUniqueID( id );
forward DB_GetUniqueID( id );