/*
*	MySQL Header File
*/

forward MYSQLX_Init();
forward MYSQLX_Upgrade();
forward MYSQLX_UpdateKey();
forward MYSQLX_Save( id );
forward MYSQLX_Save_T( id );
forward MYSQLX_GetAllXP( id );
forward MYSQLX_SetDataForRace( id );
forward MYSQLX_Close();
forward MYSQLX_Error( Handle:query, szQuery[], id );
forward MYSQLX_ThreadError( Handle:query, szQuery[], szError[], iErrNum, failstate, id );
forward	MYSQLX_Prune();
forward MYSQLX_UpdateTimestamp( id );
