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
#define DB_VAULT			0
#define DB_MYSQLX			1
#define DB_SQLITE			2

// Variable Declaration
new g_szDBType[16];
new g_szDBKey[32];
new g_DBTableName[64]

// SQLX
new Handle:g_DBTuple;
new Handle:g_DBConn;

// SQLite
new Sql:g_DB;
new g_DBType = -1;

// NVault
new g_Vault[MAX_RACES+1];

// Player's Unique ID
new g_iDBPlayerUniqueID[33];
new g_iDBPlayerSavedBy[33];

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