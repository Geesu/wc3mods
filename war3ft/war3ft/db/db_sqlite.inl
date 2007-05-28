/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	SQLite
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define TOTAL_SQLITE_TABLES		3

new const szTablesSQLite[TOTAL_SQLITE_TABLES][] = 
{
	"CREATE TABLE `wc3_player` ( `player_id` INTEGER PRIMARY KEY AUTOINCREMENT, `player_steamid` varchar(25) NOT NULL default '', `player_ip` varchar(20) NOT NULL default '', `player_name` varchar(35) NOT NULL default '', `time` timestamp(14) NOT NULL DEFAULT CURRENT_TIMESTAMP );",
	"CREATE TABLE `wc3_player_race` ( `player_id` int(8) NOT NULL default '0', `race_id` tinyint(4) NOT NULL default '0', `race_xp` int(8) default NULL, PRIMARY KEY  (`player_id`,`race_id`) );",
	"CREATE TABLE `wc3_player_skill` ( `player_id` int(8) NOT NULL default '0', `skill_id` tinyint(4) NOT NULL default '0', `skill_level` tinyint(4) NOT NULL default '0', PRIMARY KEY  (`player_id`,`skill_id`) );"
};

new const szTableNames[TOTAL_SQLITE_TABLES][] = 
{
	"wc3_player",
	"wc3_player_race",
	"wc3_player_skill"
};

// Initiate the connection to the SQLite database
SQLITE_Init()
{
	new szError[256], iErrNum;

	// Set up the tuple that will be used for threading
	g_DBTuple = SQL_MakeDbTuple( "", "", "", "war3ft" );

	// Attempt to connect
	g_DBConn = SQL_Connect( g_DBTuple, iErrNum, szError, 255 );

	if ( !g_DBConn )
	{
		WC3_Log( true, "[SQLITE] Database Connection Failed: [%d] %s", iErrNum, szError );

		return;
	}


	server_print( "[WAR3FT] SQLite database connection successful" );

	new Handle:query;

	// Create the default tables if we need to
	for ( new i = 0; i < TOTAL_SQLITE_TABLES; i++ )
	{
		query = SQL_PrepareQuery( g_DBConn, szTablesSQLite[i] );

		if ( !sqlite_TableExists( g_DBConn, szTableNames[i] ) )
		{
			if ( !SQL_Execute( query ) )
			{
				SQLITE_Error( query, szTablesSQLite[i], 1 );

				return;
			}
		}

		SQL_FreeHandle( query );
	}

	/*
		These probably should be subject to a CVAR
		Lets fine tune the database:
			"synchronous = NORMAL"	- Put back the 2.x behaviour (faster than the defalt
						  for 3.x)
			"synchronous = OFF"	- Way faster, but it might get corrupted data if a
						  server os system crash occurs
			"integrity_check"	- well it's what it says, we do have to check the
						  value it returns since it's important
		PRAGMA commands don't return anything so no need to check the result of the query
	*/	

	query = SQL_PrepareQuery( g_DBConn, "PRAGMA integrity_check" );
	
	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, "PRAGMA integrity_check", 2 );

		return;
	}
	
	// Get the integrity check value
	new szIntegrityCheck[64];
	if ( SQL_NumResults( query ) > 0 )
	{
		SQL_ReadResult( query, 0, szIntegrityCheck, 63 )
	}

	// Free the result
	SQL_FreeHandle( query );

	// Check to make sure the integrity check passed
	if ( !equali(szIntegrityCheck, "ok") )
	{
		// Should we disable saving here?
		WC3_Log( true, "[SQLITE] SQL Lite integrity check failed, disabling saving XP." );

		set_pcvar_num( CVAR_wc3_save_xp, 0 );

		return;
	}
	
	// Do some synchronous crap
	new szQuery[128];
	format( szQuery, 127, "PRAGMA synchronous = %d", SQLITE_SYNC_OFF );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		SQLITE_Error( query, szQuery, 3 );

		return;
	}

	bDBAvailable = true;
}

// Close the SQLite connection
SQLITE_Close()
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

SQLITE_FetchUniqueID( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

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
		SQLITE_Error( query, szQuery, 4 );

		return;
	}

	// If no rows we need to insert!
	if ( SQL_NumResults( query ) == 0 )
	{
		// Free the last handle!
		SQL_FreeHandle( query );

		// Insert this player!
		new szQuery[512];
		format( szQuery, 511, "INSERT INTO `wc3_player` ( `player_id` , `%s` , `time` ) VALUES ( NULL , '%s', julianday('now') );", szKeyName, szKey );
		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			SQLITE_Error( query, szQuery, 5 );

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

SQLITE_Save( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( id );

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[id][P_RACE], p_data[id][P_XP] );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 6 );

		return;
	}

	static iCurrentLevel;

	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
		{
			iCurrentLevel = SM_GetSkillLevel( id, iSkillID, 16 );

			// Then we need to save this!
			if ( iCurrentLevel > 0 && g_iDBPlayerSkillStore[id][iSkillID] != iCurrentLevel )
			{
				format( szQuery, 511, "REPLACE INTO `wc3_player_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, iCurrentLevel );
				query = SQL_PrepareQuery( g_DBConn, szQuery );

				if ( !SQL_Execute( query ) )
				{
					MYSQLX_Error( query, szQuery, 6 );

					return;
				}
			}
		}
	}

	return;
}

SQLITE_Save_T( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( id );

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[id][P_RACE], p_data[id][P_XP] );
	SQL_ThreadQuery( g_DBTuple, "_SQLITE_Save_T", szQuery );

	static iCurrentLevel;

	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
		{
			iCurrentLevel = SM_GetSkillLevel( id, iSkillID, 17 );

			// Then we need to save this!
			if ( iCurrentLevel > 0 && g_iDBPlayerSkillStore[id][iSkillID] != iCurrentLevel )
			{
				format( szQuery, 511, "REPLACE INTO `wc3_player_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, iCurrentLevel );
				SQL_ThreadQuery( g_DBTuple, "_SQLITE_Save_T", szQuery );
			}
		}
	}

	return;
}

public _SQLITE_Save_T( failstate, Handle:query, error[], errnum, data[], size )
{

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		SQL_GetQueryString( query, szQuery, 255 );
		
		SQLITE_ThreadError( query, szQuery, error, errnum, failstate, 1 );
	}
}

SQLITE_GetAllXP( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( id );

	// Then we have a problem and cannot retreive the user's XP
	if ( iUniqueID <= 0 )
	{
		client_print( id, print_chat, "%s Unable to retreive your XP from the database, please attempt to changerace later", g_MODclient );

		WC3_Log( true, "[ERROR] Unable to retreive user's Unique ID" );

		return;
	}

	new szQuery[256], data[1];
	format(szQuery, 255, "SELECT `race_id`, `race_xp` FROM `wc3_player_race` WHERE ( `player_id` = '%d' );", iUniqueID );
	
	data[0] = id;

	SQL_ThreadQuery( g_DBTuple, "_SQLITE_GetAllXP", szQuery, data, 1 )	

	return;
}

// Callback function that is executed when MySQL X Thread has completed
public _SQLITE_GetAllXP( failstate, Handle:query, error[], errnum, data[], size )
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
		SQL_GetQueryString( query, szQuery, 255 );
		
		SQLITE_ThreadError( query, szQuery, error, errnum, failstate, 2 );
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

SQLITE_SetDataForRace( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new szQuery[256], data[1];
	format( szQuery, 255, "SELECT `skill_id`, `skill_level` FROM `wc3_player_skill` WHERE `player_id` = '%d';", DB_GetUniqueID( id ) );

	data[0] = id;

	SQL_ThreadQuery( g_DBTuple, "_SQLITE_SetDataForRace", szQuery, data, 1 );

	return;
}

// Callback function once MySQL X Thread has completed
public _SQLITE_SetDataForRace( failstate, Handle:query, error[], errnum, data[], size )
{
	new id = data[0];

	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		SQL_GetQueryString( query, szQuery, 255 );
		
		SQLITE_ThreadError( query, szQuery, error, errnum, failstate, 3 );
	}

	// Query successful, we can do stuff!
	else
	{
		// Set the user's XP!
		if ( !SHARED_ValidPlayer( id ) || !p_data_b[id][PB_ISCONNECTED] || p_data[id][P_RACE] <=0 || p_data[id][P_RACE] > MAX_RACES )
		{
			WC3_Log( true, "[ERROR] WTF MATE?!? %d", id );

			return;
		}

		p_data[id][P_XP] = g_iDBPlayerXPInfoStore[id][p_data[id][P_RACE]-1];

		// Reset all skill data to 0!
		for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
		{
			if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
			{
				SM_SetSkillLevel( id, iSkillID, 0, 4 );
			}
		}

		// While we have a result!
		while ( SQL_MoreResults( query ) )
		{
			SM_SetSkillLevel( id, SQL_ReadResult( query, 0 ), SQL_ReadResult( query, 1 ), 5 );
			
			SQL_NextRow( query );
		}

		// Free the handle
		SQL_FreeHandle( query );
		
		// Set the race up
		WC3_SetRaceUp( id );

		// This user's XP has been retrieved! We can save now
		bDBXPRetrieved[id] = true;
	}

	return;
}

// Verifies that the database connection is ok
SQLITE_Check_Connection()
{
	if ( !bDBAvailable )
	{
		return false;
	}

	return true;
}

#define SQLITE_TOTAL_PRUNE_QUERY 2

SQLITE_Prune()
{
	new const szPruneQuery[SQLITE_TOTAL_PRUNE_QUERY][] = 
	{
		"DELETE FROM wc3_player_race  WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( (julianday(`time`) + %d) < julianday('now') ) );",
		"DELETE FROM wc3_player_skill WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( (julianday(`time`) + %d) < julianday('now') ) );"
	};

	new szQuery[256];

	// Need to run all 3 queries
	for ( new i = 0; i < MYSQL_TOTAL_PRUNE_QUERY; i++ )
	{
		formatex( szQuery, 255, szPruneQuery[i], get_pcvar_num( CVAR_wc3_days_before_delete ) );

		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 6 );

			return;
		}
	}
}

SQLITE_UpdateTimestamp( id )
{
	// Make sure our connection is working
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new szKey[66];
	DB_GetKey( id, szKey, 65 );

	new szQuery[256];
	format( szQuery, 255, "UPDATE `wc3_player` SET time = julianday('now') WHERE ( `player_id` = '%d' )", g_iDBPlayerUniqueID[id] );

	SQL_ThreadQuery( g_DBTuple, "_SQLITE_UpdateTimestamp", szQuery );	
}

public _SQLITE_UpdateTimestamp( failstate, Handle:query, error[], errnum, data[], size )
{
	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		SQL_GetQueryString( query, szQuery, 255 );
		
		SQLITE_ThreadError( query, szQuery, error, errnum, failstate, 4 );
	}

	// Query successful, we can do stuff!
	else
	{
		// Free the handle
		SQL_FreeHandle( query );
	}

	return;
}

// The id should be a unique number, so we know what function called it (useful for debugging)
SQLITE_Error( Handle:query, szQuery[], id )
{
	new szError[256];
	new iErrNum = SQL_QueryError( query, szError, 255 );

	WC3_Log( true, "[SQLITE] Error in querying database, location: %d", id );
	WC3_Log( true, "[SQLITE] Message: %s (%d)", szError, iErrNum );
	WC3_Log( true, "[SQLITE] Query statement: %s ", szQuery );

	// Free the handle
	SQL_FreeHandle( query );
}

SQLITE_ThreadError( Handle:query, szQuery[], szError[], iErrNum, failstate, id )
{
	WC3_Log( true, "[SQLITE] Threaded query error, location: %d", id );
	WC3_Log( true, "[SQLITE] Message: %s (%d)", szError, iErrNum );
	WC3_Log( true, "[SQLITE] Query statement: %s ", szQuery );

	// Connection failed
	if ( failstate == TQUERY_CONNECT_FAILED )
	{	
		WC3_Log( true, "[SQLITE] Fail state: Connection Failed" );
	}

	// Query failed
	else if ( failstate == TQUERY_QUERY_FAILED )
	{
		WC3_Log( true, "[SQLITE] Fail state: Query Failed" );
	}

	// Free the handle
	SQL_FreeHandle( query );
}
