/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	MYSQLX
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/


// Initiate the connection to the MySQL database
MYSQLX_Init()
{
	// Determine the database information
	new szHost[64], szUser[32], szPass[32], szDB[128], szError[256], iErrNum;

	get_pcvar_string( CVAR_wc3_sql_dbhost	, szHost		, 63	);
	get_pcvar_string( CVAR_wc3_sql_dbuser	, szUser		, 31	);
	get_pcvar_string( CVAR_wc3_sql_dbpass	, szPass		, 31	);
	get_pcvar_string( CVAR_wc3_sql_dbname	, szDB			, 127	);
	get_pcvar_string( CVAR_wc3_sql_tbname	, g_DBTableName	, 63	);

	// Set up the tuple that will be used for threading
	g_DBTuple = SQL_MakeDbTuple( szHost, szUser, szPass, szDB )

	// Attempt to connect
	g_DBConn = SQL_Connect( g_DBTuple, iErrNum, szError, 255 );

	if ( !g_DBConn )
	{
		log_amx( "[MYSQLX] Database Connection Failed: [%d] %s", iErrNum, szError );

		return;
	}

	server_print( "[WAR3FT] %s database connection successful", g_szDBType );

	MYSQLX_CreateTables();

	// Set the key
	switch ( get_pcvar_num( CVAR_wc3_save_by ) )
	{
		case DB_SAVEBY_STEAMID:		copy( g_szDBKey, 31, "playerid" );
		case DB_SAVEBY_IP:			copy( g_szDBKey, 31, "playerip" );
		case DB_SAVEBY_NAME:		copy( g_szDBKey, 31, "playername" );
	}
}

// Create all of our tables!
MYSQLX_CreateTables()
{

	// Create the default table if we need to
	new szQuery[512];
	format( szQuery, 511, "CREATE TABLE IF NOT EXISTS `%s` (`playerid` VARCHAR(35) NOT NULL DEFAULT '', `playerip` VARCHAR(20) NOT NULL DEFAULT '', `playername` VARCHAR(35) NOT NULL DEFAULT '', `xp` INT(11) NOT NULL DEFAULT 0, `race` TINYINT(4) NOT NULL DEFAULT 0, `skill1` TINYINT(4) NOT NULL DEFAULT 0, `skill2` TINYINT(4) NOT NULL DEFAULT 0, `skill3` TINYINT(4) NOT NULL DEFAULT 0, `skill4` TINYINT(4) NOT NULL DEFAULT 0, `time` TIMESTAMP(14) NOT NULL, PRIMARY KEY (`playerid`, `race`))", g_DBTableName );

	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 1 );

		return;
	}

	SQL_FreeHandle( query );
}

MYSQLX_FetchUniqueID( id )
{
	// Remember how we got this ID
	g_iDBPlayerSavedBy[id] = get_pcvar_num( CVAR_wc3_save_by );

	new szKey[66], szKeyName[32];
	DB_GetKey( id, szKey, 65 );
	DB_GetKeyName( szKeyName, 31 );

	new szQuery[512];
	format( szQuery, 511, "SELECT `player_id` FROM `wc3_player` WHERE `%s` = '%s';", szKeyName, szKey );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 1 );

		return;
	}

	// If no rows we need to insert!
	if ( SQL_NumResults( query ) == 0 )
	{
		// Free the last handle!
		SQL_FreeHandle( query );

		new szKey[66], szKeyName[32];
		DB_GetKey( id, szKey, 65 );
		DB_GetKeyName( szKeyName, 31 );

		// Insert this player!
		new szQuery[512];
		format( szQuery, 511, "INSERT INTO `wc3_player` ( `player_id` , `%s` , `time` ) VALUES ( NULL , '%s', NOW() );", szKeyName, szKey );
		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 1 );

			return;
		}

		g_iDBPlayerUniqueID[id] = SQL_GetInsertId( query );
	}

	// They have been here before - store their ID
	else
	{
		g_iDBPlayerUniqueID[id] = SQL_ReadResult( query, 0 );
	}

	// Free the last handle!
	SQL_FreeHandle( query );
}

MYSQLX_Save( id )
{
	new iUniqueID = DB_GetUniqueID( id );

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[id][P_RACE], p_data[id][P_XP] );
	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save", szQuery );

	new iCurrentLevel;

	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		iCurrentLevel = SM_GetSkillLevel( id, iSkillID );

		// Then we need to save this!
		if ( iCurrentLevel > 0 && g_iDBPlayerSkillStore[id][iSkillID] != iCurrentLevel )
		{
			new szQuery[512];
			format( szQuery, 511, "REPLACE INTO `wc3_player_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, iCurrentLevel );
			SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save", szQuery );
		}
	}

	return;
}

public _MYSQLX_Save( failstate, Handle:query, error[], errnum, data[], size )
{

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 1 );
	}
}

MYSQLX_GetAllXP( id )
{
	new szQuery[256], data[1];
	format(szQuery, 255, "SELECT `race_id`, `race_xp` FROM `wc3_player_race` WHERE ( `player_id` = '%d' );", DB_GetUniqueID( id ) );
	
	data[0] = id;

	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_GetAllXP", szQuery, data, 1 )	

	return;
}

// Callback function that is executed when MySQL X Thread has completed
public _MYSQLX_GetAllXP( failstate, Handle:query, error[], errnum, data[], size )
{
	new id = data[0];

	// Set last saved XP to 0
	for ( new i = 0; i < MAX_RACES; i++ )
	{
		g_iDBPlayerXPInfoStore[id][i] = 0;
	}

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 2 );
	}

	// Query successful, we can do stuff!
	else
	{
		new iXP, iRace;

		// Loop through all of the records to find the XP data
		while ( SQL_MoreResults( query ) )
		{
			iRace	= SQL_ReadResult( query, 0 );
			iXP		= SQL_ReadResult( query, 1 );
			
			// Save the user's XP in an array
			if ( iRace > 0 && iRace < MAX_RACES + 1 )
			{
				g_iDBPlayerXPInfoStore[id][iRace-1] = iXP;
			}

			SQL_NextRow( query );
		}

		// Free the handle
		SQL_FreeHandle( query );
	}

	// Call the function that will display the "select a race" menu
	WC3_ChangeRaceShowMenu( id, g_iDBPlayerXPInfoStore[id] );

	return;
}

MYSQLX_SetData( id )
{
	new szQuery[256], data[1];
	format( szQuery, 255, "SELECT `skill_id`, `skill_level` FROM `wc3_player_skill` WHERE `player_id` = '%d';", DB_GetUniqueID( id ) );

	data[0] = id;

	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_SetData", szQuery, data, 1 );

	return;
}

// Callback function once MySQL X Thread has completed
public _MYSQLX_SetData( failstate, Handle:query, error[], errnum, data[], size )
{
	new id = data[0];

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 3 );
	}

	// Query successful, we can do stuff!
	else
	{
		// Set the user's XP!
		p_data[id][P_XP] = g_iDBPlayerXPInfoStore[id][p_data[id][P_RACE]-1];

		// Reset all skill data to 0!
		for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
		{
			SM_SetSkillLevel( id, iSkillID, 0 );
		}

		// While we have a result!
		while ( SQL_MoreResults( query ) )
		{
			SM_SetSkillLevel( id, SQL_ReadResult( query, 0 ), SQL_ReadResult( query, 1 ) );
			
			SQL_NextRow( query );
		}

		// Free the handle
		SQL_FreeHandle( query );
		
		// Set the race up
		WC3_SetRaceUp( id );
	}

	return;
}

MYSQLX_Close()
{
	if ( g_DBTuple )
	{
		SQL_FreeHandle( g_DBTuple );
	}

	if ( g_DBConn )
	{
		SQL_FreeHandle( g_DBConn );
	}
}

// The id should be a unique number, so we know what function called it (useful for debugging)
MYSQLX_Error( Handle:query, szQuery[], id )
{
	new szError[256];
	new iErrNum = SQL_QueryError( query, szError, 255 );

	log_amx( "[MYSQLX] Error in querying database, location: %d", id );
	log_amx( "[MYSQLX] Message: %s (%d)", szError, iErrNum );
	log_amx( "[MYSQLX] Query statement: %s ", szQuery );

	// Free the handle
	SQL_FreeHandle( query );
}

MYSQLX_ThreadError( Handle:query, szQuery[], szError[], iErrNum, failstate, id )
{
	log_amx( "[MYSQLX] Threaded query error, location: %d", id );
	log_amx( "[MYSQLX] Message: %s (%d)", szError, iErrNum );
	log_amx( "[MYSQLX] Query statement: %s ", szQuery );

	// Connection failed
	if ( failstate == TQUERY_CONNECT_FAILED )
	{	
		log_amx( "[MYSQLX] Fail state: Connection Failed" );
	}

	// Query failed
	else if ( failstate == TQUERY_QUERY_FAILED )
	{
		log_amx( "[MYSQLX] Fail state: Query Failed" );
	}

	// Free the handle
	SQL_FreeHandle( query );
}

MYSQLX_UpdateTimestamp( id )
{
	new szKey[66];
	DB_GetKey( id, szKey, 65 );

	new szQuery[256];
	format( szQuery, 255, "UPDATE `wc3_player` SET time = NOW() WHERE ( `player_id` = '%d' );", DB_GetUniqueID( id ) );

	server_print( "Updating: '%s'", szQuery );

	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_UpdateTimestamp", szQuery );	
}

public _MYSQLX_UpdateTimestamp( failstate, Handle:query, error[], errnum, data[], size )
{
	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 5 );
	}

	// Query successful, we can do stuff!
	else
	{
		// Free the handle
		SQL_FreeHandle( query );
	}

	return;
}

// Prune the MySQL database
/*MYSQLX_Prune()
{
	new szQuery[256];
	format( szQuery, 255, "DELETE FROM `%s` WHERE DATE_SUB(CURDATE(), INTERVAL %d DAY) > time;", g_DBTableName, get_pcvar_num( CVAR_wc3_days_before_delete ) );

	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 6 );

		return;
	}
}*/

/*MYSQLX_FetchUniqueID( id )
{
	client_print( id, print_chat, "[DEBUG] MYSQLX_FetchUniqueID" );
	server_print( "[DEBUG] MYSQLX_FetchUniqueID %d", id );

	new szKey[66], szKeyName[32];
	DB_GetKey( id, szKey, 65 );
	DB_GetKeyName( szKeyName, 31 );

	new parm[1];
	parm[0] = id;

	new szQuery[512];
	format( szQuery, 511, "SELECT `player_id` FROM `wc3_players` WHERE `%s` = '%s';", szKeyName, szKey );
	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_FetchUniqueID", szQuery, parm, 1 );
}

public _MYSQLX_FetchUniqueID( failstate, Handle:query, error[], errnum, data[], size )
{

	server_print( "[DEBUG] _MYSQLX_FetchUniqueID %d", data[0] );
	client_print( data[0], print_chat, "[DEBUG] _MYSQLX_FetchUniqueID" );

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 1 );
	}
	
	// Query successful
	else
	{
		// If no rows we need to insert!
		if ( SQL_NumResults( query ) == 0 )
		{
			new szKey[66], szKeyName[32];
			DB_GetKey( data[0], szKey, 65 );
			DB_GetKeyName( szKeyName, 31 );

			// Insert this player!
			new szQuery[512];
			format( szQuery, 511, "INSERT INTO `wc3_players` ( `player_id` , `%s` , `time` ) VALUES ( NULL , '%s', NOW() );", szKeyName, szKey );
			SQL_ThreadQuery( g_DBTuple, "_MYSQLX_InsertNewPlayer", szQuery, data, 1 );
		}

		// They have been here before - store their ID
		else
		{
			client_print( data[0], print_chat, "[DEBUG]-- _MYSQLX_FetchUniqueID" );
			server_print( "[DEBUG]-- _MYSQLX_FetchUniqueID %d", data[0] );

			g_iDBPlayerUniqueID[data[0]] = SQL_ReadResult( query, 0 );
		}

		// Free the last handle!
		SQL_FreeHandle( query );
	}

}

public _MYSQLX_InsertNewPlayer( failstate, Handle:query, error[], errnum, data[], size )
{
	client_print( data[0], print_chat, "[DEBUG] _MYSQLX_InsertNewPlayer" );
	server_print( "[DEBUG] _MYSQLX_InsertNewPlayer %d", data[0] );

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 1 );
	}
	
	// Query successful
	else
	{
		g_iDBPlayerUniqueID[data[0]] = SQL_GetInsertId( query );

		SQL_FreeHandle( query );
	}
}*/