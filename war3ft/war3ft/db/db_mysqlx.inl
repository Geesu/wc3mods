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

	// Set the key
	switch ( get_pcvar_num( CVAR_wc3_save_by ) )
	{
		case DB_SAVEBY_STEAMID:		copy( g_szDBKey, 31, "playerid" );
		case DB_SAVEBY_IP:			copy( g_szDBKey, 31, "playerip" );
		case DB_SAVEBY_NAME:		copy( g_szDBKey, 31, "playername" );
	}

	// Run an upgrade if we need to
	MYSQLX_Upgrade();

	// Update the primary key if necessary
	MYSQLX_UpdateKey();
}


MYSQLX_Upgrade()
{

	new szQuery[256];
	format( szQuery, 255, "SHOW COLUMNS FROM `%s` LIKE 'playerip';", g_DBTableName );
	
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 2 );

		return;
	}

	new iRows = SQL_NumResults( query );
	SQL_FreeHandle( query );
	
	// We have no result from the above query - so we need to add this to the table
	if ( !iRows )
	{
		format( szQuery, 255, "ALTER TABLE `%s` ADD `playerip` VARCHAR( 20 ) NOT NULL AFTER `playerid`;", g_DBTableName );
		
		query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 3 );

			return;
		}

		SQL_FreeHandle( query );
	}


	return;
}

// Function will update the primary key if we need to
MYSQLX_UpdateKey()
{
	new szQuery[256];
	
	// Format the query based on what primary key we would like
	switch ( get_pcvar_num( CVAR_wc3_save_by ) )
	{
		case DB_SAVEBY_STEAMID:	format( szQuery, 255, "ALTER TABLE `war3users` DROP PRIMARY KEY, ADD PRIMARY KEY ( `playerid`, `race` );" );
		case DB_SAVEBY_IP:		format( szQuery, 255, "ALTER TABLE `war3users` DROP PRIMARY KEY, ADD PRIMARY KEY ( `playerip`, `race` );" );
		case DB_SAVEBY_NAME:	format( szQuery, 255, "ALTER TABLE `war3users` DROP PRIMARY KEY, ADD PRIMARY KEY ( `playername`, `race` );" );
	}
	
	// Then we have a query to execute
	if ( strlen( szQuery ) > 0 )
	{

		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 4 );

			return;
		}

		SQL_FreeHandle( query );
	}

	return;
}

MYSQLX_Save( id )
{
	new szPlayerIP[20], szPlayerName[66], szPlayerID[32];
	get_user_name(		id, szPlayerName	, 65 );
	get_user_ip(		id, szPlayerIP		, 19 );
	get_user_authid(	id, szPlayerID		, 31 );

	// Prepare name for the query (playername is 66 in case all 33 characters are ')
	DB_FormatString( szPlayerName, 65 );

	new iSkillLevels[4];
	iSkillLevels[0] = SM_GetSkillLevel( id, SM_GetSkillByPos( id, SKILL_POS_1 ) );
	iSkillLevels[1] = SM_GetSkillLevel( id, SM_GetSkillByPos( id, SKILL_POS_2 ) );
	iSkillLevels[2] = SM_GetSkillLevel( id, SM_GetSkillByPos( id, SKILL_POS_3 ) );
	iSkillLevels[3] = SM_GetSkillLevel( id, SM_GetSkillByPos( id, SKILL_POS_4 ) );

	// Save the data
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `%s` (`playerid`, `playerip`, `playername`, `xp`, `race`, `skill1`, `skill2`, `skill3`, `skill4`) VALUES ('%s', '%s', '%s', %d, %d, %d, %d, %d, %d)", g_DBTableName, szPlayerID, szPlayerIP, szPlayerName, p_data[id][P_XP], p_data[id][P_RACE], iSkillLevels[0], iSkillLevels[1], iSkillLevels[2], iSkillLevels[3] );
	
	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save", szQuery )

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
	new szKey[66];
	DB_GetKey( id, szKey, 65 );

	new szQuery[256], data[1];
	format(szQuery, 255, "SELECT `xp`, `race` FROM `%s` WHERE (`%s` = '%s')", g_DBTableName, g_szDBKey, szKey );
	
	data[0] = id;

	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_GetAllXP", szQuery, data, 1 )	

	return;
}

// Callback function that is executed when MySQL X Thread has completed
public _MYSQLX_GetAllXP( failstate, Handle:query, error[], errnum, data[], size )
{
	new id = data[0];
	new iRaceXP[MAX_RACES] = {0};

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
			iXP		= SQL_ReadResult( query, 0 );
			iRace	= SQL_ReadResult( query, 1 );
			
			// Save the user's XP in an array
			if ( iRace > 0 && iRace < MAX_RACES + 1 )
			{
				iRaceXP[iRace-1] = iXP;
			}

			SQL_NextRow( query );
		}

		// Free the handle
		SQL_FreeHandle( query );
	}

	// Call the function that will display the "select a race" menu
	WC3_ChangeRaceEnd( id, iRaceXP );

	return;
}

MYSQLX_SetData( id )
{
	new szKey[66];
	DB_GetKey( id, szKey, 65 );

	new szQuery[256], data[1];
	format( szQuery, 255, "SELECT `xp`, `skill1`, `skill2`, `skill3`, `skill4` FROM `%s` WHERE (`%s` = '%s' AND `race` = %d)", g_DBTableName, g_szDBKey, szKey, p_data[id][P_RACE] );

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

		new iSkillIDs[4];
		iSkillIDs[0] = SM_GetSkillByPos( id, SKILL_POS_1 );
		iSkillIDs[1] = SM_GetSkillByPos( id, SKILL_POS_2 );
		iSkillIDs[2] = SM_GetSkillByPos( id, SKILL_POS_3 );
		iSkillIDs[3] = SM_GetSkillByPos( id, SKILL_POS_4 );

		// Then we have data in the database
		if ( SQL_MoreResults( query ) )
		{
			p_data[id][P_XP]		= SQL_ReadResult( query, 0 );
			SM_SetSkillLevel( id, iSkillIDs[0], SQL_ReadResult( query, 1 ) );
			SM_SetSkillLevel( id, iSkillIDs[1], SQL_ReadResult( query, 2 ) );
			SM_SetSkillLevel( id, iSkillIDs[2], SQL_ReadResult( query, 3 ) );
			SM_SetSkillLevel( id, iSkillIDs[3], SQL_ReadResult( query, 4 ) );
		}

		// The user has no record, start them at 0
		else
		{
			p_data[id][P_XP]		= 0;
			SM_SetSkillLevel( id, iSkillIDs[0], 0 );
			SM_SetSkillLevel( id, iSkillIDs[1], 0 );
			SM_SetSkillLevel( id, iSkillIDs[2], 0 );
			SM_SetSkillLevel( id, iSkillIDs[3], 0 );
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
	SQL_FreeHandle( g_DBTuple );
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

// Prune the MySQL database
MYSQLX_Prune()
{
	new szQuery[256];
	format( szQuery, 255, "DELETE FROM `%s` WHERE DATE_SUB(CURDATE(), INTERVAL %d DAY) > time;", g_DBTableName, get_pcvar_num( CVAR_wc3_days_before_delete ) );

	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 6 );

		return;
	}
}

MYSQLX_UpdateTimestamp( id )
{
	new szKey[66];
	DB_GetKey( id, szKey, 65 );

	new szQuery[256];
	format( szQuery, 255, "UPDATE `%s` SET time = NOW() WHERE (`%s` = '%s')", g_DBTableName, g_szDBKey, szKey );

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
