/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	SQLite Header File
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

forward SQLITE_Init();
forward SQLITE_Upgrade();
forward SQLITE_Close();
forward SQLITE_Save( id );
forward SQLITE_GetAllXP( id );
forward SQLITE_SetData( id );
forward SQLITE_Check_Connection();
forward SQLITE_Error( Result:res, query[], id );
forward SQLITE_Prune();
forward SQLITE_UpdateTimestamp( id );
forward SQLITE_FetchUniqueID( id );