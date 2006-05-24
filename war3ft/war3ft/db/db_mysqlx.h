/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	MySQL Header File
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

forward MYSQLX_Init();
forward MYSQLX_Upgrade();
forward MYSQLX_UpdateKey();
forward MYSQLX_Save( id );
forward _MYSQLX_Save( failstate, Handle:query, error[], errnum, data[], size );
forward MYSQLX_GetAllXP( id );
forward _MYSQLX_GetAllXP( failstate, Handle:query, error[], errnum, data[], size );
forward MYSQLX_SetData( id );
forward _MYSQLX_SetData( failstate, Handle:query, error[], errnum, data[], size );
forward MYSQLX_Close();
forward MYSQLX_Error( Handle:query, szQuery[], id );
forward MYSQLX_ThreadError( Handle:query, szQuery[], szError[], iErrNum, failstate, id );