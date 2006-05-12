
// Function will alter the table if it needs to
public DB_Upgrade()
{

	// Verify our database connection has worked
	if ( !DB_Check_Connection() )
	{
		return;
	}

	//ALTER TABLE `war3users` ADD `playerip` VARCHAR( 20 ) NOT NULL AFTER `playerid` ;
	new szQuery[256];
	format( szQuery, 255, "SHOW COLUMNS FROM `war3users` LIKE 'playerip';" );
	
	new Result:res = dbi_query( g_DB, szQuery );
	
	// We have no result from the above query - so we need to add this to the table
	if ( !res || !dbi_nextrow( res ) > 0 )
	{
		format( szQuery, 255, "ALTER TABLE `war3users` ADD `playerip` VARCHAR( 20 ) NOT NULL AFTER `playerid`;" );
		
		new Result:ret = dbi_query( g_DB, szQuery );

		if ( ret < RESULT_NONE )
		{
			DB_Error( ret, szQuery, 5 );

			return;
		}
	}
}

// Function will update the primary key if we need to
public DB_UpdateKey()
{
	
	new szQuery[256];
	
	// Format the query based on what primary key we would like
	switch ( get_pcvar_num( CVAR_wc3_save_by ) )
	{
		
		case DB_SAVEBY_STEAMID:
		{
			format( szQuery, 255, "ALTER TABLE `war3users` DROP PRIMARY KEY, ADD PRIMARY KEY ( `playerid`, `race` );" );

			copy( g_szDBKey, 31, "playerid" );
		}

		case DB_SAVEBY_IP:
		{
			format( szQuery, 255, "ALTER TABLE `war3users` DROP PRIMARY KEY, ADD PRIMARY KEY ( `playerip`, `race` );" );

			copy( g_szDBKey, 31, "playerip" );
		}

		case DB_SAVEBY_NAME:
		{
			format( szQuery, 255, "ALTER TABLE `war3users` DROP PRIMARY KEY, ADD PRIMARY KEY ( `playername`, `race` );" );

			copy( g_szDBKey, 31, "playername" );
		}
	}
	
	// Then we have a query to execute
	if ( strlen( szQuery ) > 0 )
	{
		new Result:ret = dbi_query( g_DB, szQuery );

		if ( ret < RESULT_NONE )
		{
			DB_Error( ret, szQuery, 6 );

			return;
		}
	}
}

public DB_DetermineType()
{
	// If the DBI Module isn't loaded then we can't use SQL for saving XP
	if ( !g_DBILoaded && get_pcvar_num( CVAR_wc3_save_xp_sql ) )
	{

		log_amx( "Unable to saving using DBI (sql), please enable a DBI module (sqlite or mysql) to save XP using SQL" );

		set_pcvar_num( CVAR_wc3_save_xp_sql, 0 );
	}

	
	// Then we are saving using SQL, lets determine if it's sqlite or mysql
	if ( get_pcvar_num( CVAR_wc3_save_xp_sql ) == 1 )
	{
		// Get the DB Type
		new szDBIType[16];
		dbi_type( szDBIType, 15 );
		
		// We're using MySQL
		if ( equali( szDBIType, "MySQL" ) )
		{
			g_DBType = DB_MYSQL;
			copy( g_szDBType, 15, "MySQL" );
		}

		// We're using SQLite
		else if ( equali( szDBIType, "SQLite" ) )
		{
			g_DBType = DB_SQLITE;
			copy( g_szDBType, 15, "SQLite" );
		}

		// Using an unsupported DB Type - lets default to vault
		else
		{
			log_amx( "Unsupported database type loaded: %s, the supported databases are MySQL or SQLite.", szDBIType );
		}
	}
	
	// Then we don't have a DB Type yet set up for saving! Use vault?
	if ( g_DBType == -1 )
	{

		// We can use the nvault if it's loaded
		if ( g_NVaultLoaded )
		{
			g_DBType = DB_VAULT;
		}

		// Otherwise we aren't going to be saving :/
		else
		{
			set_pcvar_num( CVAR_wc3_save_xp, 0 );

			log_amx( "Unable to save XP, please enable nvault or a sql module." );

			return;
		}
	}

	return;
}

// Function will determine what type of database we should use, then initiate the appropriate "connections"
public DB_Init()
{
	
	// Then we don't want to save XP
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}
	
	// We should only need to call this once
	if ( g_DBType == -1 )
	{
		DB_DetermineType();
	}	


	// MYSQL
	if ( g_DBType == DB_MYSQL )
	{	
		iSQLAttempts++;

		// Determine the database information
		new szHost[64], szUser[32], szPass[32], szDB[128], szError[256];

		get_pcvar_string( CVAR_wc3_sql_dbhost	, szHost	, 63	);
		get_pcvar_string( CVAR_wc3_sql_dbuser	, szUser	, 31	);
		get_pcvar_string( CVAR_wc3_sql_dbpass	, szPass	, 31	);
		get_pcvar_string( CVAR_wc3_sql_dbname	, szDB		, 127	);

		// Attempt the Connection
		g_DB = dbi_connect( szHost, szUser, szPass, szDB, szError, 255 );


		// Verify our database connection has worked
		if ( !DB_Check_Connection() )
		{
			log_amx( "Error: %s", szError );

			return;
		}

		server_print( "%s database connection successful with handle %d", g_szDBType, g_DB );

		get_pcvar_string( CVAR_wc3_sql_tbname, g_DBTableName, 63 );
		
		// Create the default table if we need to
		new szQuery[512];
		format( szQuery, 511, "CREATE TABLE IF NOT EXISTS `%s` (`playerid` VARCHAR(35) NOT NULL DEFAULT '', `playerip` VARCHAR(20) NOT NULL DEFAULT '', `playername` VARCHAR(35) NOT NULL DEFAULT '', `xp` INT(11) NOT NULL DEFAULT 0, `race` TINYINT(4) NOT NULL DEFAULT 0, `skill1` TINYINT(4) NOT NULL DEFAULT 0, `skill2` TINYINT(4) NOT NULL DEFAULT 0, `skill3` TINYINT(4) NOT NULL DEFAULT 0, `skill4` TINYINT(4) NOT NULL DEFAULT 0, `time` TIMESTAMP(14) NOT NULL, PRIMARY KEY (`playerid`, `race`))", g_DBTableName );

		new Result:ret = dbi_query( g_DB, szQuery );

		if ( ret < RESULT_NONE )
		{
			DB_Error( ret, szQuery, 1 );

			return;
		}

		// Run an upgrade if we need to
		DB_Upgrade();

		// Update the primary key if necessary
		DB_UpdateKey();
	}

	// SQLITE
	else if ( g_DBType == DB_SQLITE )
	{
		iSQLAttempts++;

		new szDB[] = "addons/amxmodx/data/war3ft.db";
		new szError[256];

		// Attempt the Connection
		g_DB = dbi_connect( "", "", "", szDB, szError, 255 );

		// Verify our database connection has worked
		if ( !DB_Check_Connection() )
		{
			log_amx( "Error: %s", szError );

			return;
		}

		server_print( "%s database connection successful with handle %d", g_szDBType, g_DB );

		get_pcvar_string( CVAR_wc3_sql_tbname, g_DBTableName, 63 );

		new szQuery[512];

		// Make sure the SQLite table exists, if not, create it
		if ( !sqlite_table_exists( g_DB, g_DBTableName ) )
		{
			format( szQuery, 511, "CREATE TABLE `%s` (`playerid` VARCHAR(35) NOT NULL DEFAULT '', `playerip` VARCHAR(20) NOT NULL DEFAULT '', `playername` VARCHAR(35) NOT NULL DEFAULT '', `xp` INT(11) NOT NULL DEFAULT 0, `race` TINYINT(4) NOT NULL DEFAULT 0, `skill1` TINYINT(4) NOT NULL DEFAULT 0, `skill2` TINYINT(4) NOT NULL DEFAULT 0, `skill3` TINYINT(4) NOT NULL DEFAULT 0, `skill4` TINYINT(4) NOT NULL DEFAULT 0, `time` TIMESTAMP(14) NOT NULL DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY (`playerid`, `race`))"
				, g_DBTableName );
			new Result:ret = dbi_query( g_DB, szQuery );

			if ( ret < RESULT_NONE )
			{
				DB_Error( ret, szQuery, 2 );

				return;
			}
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

		new szIntegrityCheck[64];
		format(szQuery, 511, "PRAGMA integrity_check");
		new Result:res = dbi_query( g_DB, szQuery );
		
		// Check for an error
		if ( res < RESULT_NONE )
		{
			DB_Error( res, szQuery, 3 );
			return;
		}
		
		// Get the integrity check value
		while ( res && dbi_nextrow(res) > 0 )
		{
			dbi_result(res, "integrity_check", szIntegrityCheck, 63);
		}

		// Free the result
		dbi_free_result( res );

		// Check to make sure the integrity check passed
		if ( !equali(szIntegrityCheck, "ok") )
		{
			// Should we disable saving here?
			log_amx( "SQL Lite integrity check failed, disabling saving XP." );

			set_pcvar_num( CVAR_wc3_save_xp, 0 );

			return;
		}
			
		// Do some synchronous crap
		format( szQuery, 511, "PRAGMA synchronous = %d", SQLITE_SYNC_OFF );
		dbi_query( g_DB, szQuery );

		// Run an upgrade if we need to
		DB_Upgrade();

		// Update the primary key if necessary
		DB_UpdateKey();
	}

	// VAULT
	else if ( g_DBType == DB_VAULT )
	{
		new szVaultName[32];

		// We need to open the vaults we have for each race
		for ( new i = 1; i <= MAX_RACES; i++ )
		{
			format( szVaultName, 31, "war3ft-%d", i );

			g_Vault[i] = nvault_open( szVaultName );

			if ( g_Vault[i] == -1 )
			{
				log_amx( "Error when opening nvault '%s', defaulting to short-term XP", szVaultName );

				set_pcvar_num( CVAR_wc3_save_xp, 0 );
				
				return;
			}

			// Set up default keys that should always exist
			else
			{
				nvault_pset( g_Vault[i], "war3ft_version", WC3VERSION );
			}
		}
	}

	return;
}

// Verifies that the database connection is ok
public DB_Check_Connection()
{
	
	if ( g_DB < SQL_OK )
	{
		// Lets try to re-connect in a certain amount of time
		if ( iSQLAttempts < SQL_ATTEMPTS )
		{
			if ( !task_exists( TASK_SETSQL ) )
			{
				log_amx( "Database connection failed, attempting to reconnect in %0.0f seconds", SQL_ATTEMPT_DELAY );

				set_task( SQL_ATTEMPT_DELAY, "DB_Init", TASK_SETSQL );
			}
		}

		// We have reached the maximum amount of attempts, lets default to short-term XP
		else if( iSQLAttempts >= SQL_ATTEMPTS )
		{
			set_pcvar_num( CVAR_wc3_save_xp, 0 );
			set_pcvar_num( CVAR_wc3_save_xp_sql, 0 );

			log_amx( "Unable to connect to the %s database after %d attempts, switching to short-term mode", g_szDBType, iSQLAttempts )
		}

		return false;
	}

	return true;
}

// Close the database connection(s)
public DB_Close()
{
	
	// Then we shouldn't have a connection to close
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}
	
	if ( g_DBType == DB_MYSQL || g_DBType == DB_SQLITE )
	{
		if ( g_DB )
		{
			dbi_close( g_DB );
		}
	}

	else if ( g_DBType == DB_VAULT )
	{

		// Cycle through each vault and close it
		for ( new i = 1; i <= MAX_RACES; i++ )
		{
			// Make sure we have a valid handle
			if ( g_Vault[i] )
			{
				nvault_close( g_Vault[i] );
			}
		}
	}

	return;
}

// Prune the database of old records
public DB_Prune()
{


	return;
}

// Save the user's XP
public DB_SaveXP( id )
{
	
	if ( !WAR3_Check() )
	{
		return;
	}

	if ( p_data[id][P_RACE] == 0 || is_user_bot(id) || !get_pcvar_num( CVAR_wc3_save_xp ) || p_data[id][P_XP] == 0 )
	{
		return;
	}
	
	// These will be used in either case
	new szPlayerIP[20], szPlayerName[66], szPlayerID[32];
	get_user_name(		id, szPlayerName	, 65 );
	get_user_ip(		id, szPlayerIP		, 19 );
	get_user_authid(	id, szPlayerID		, 31 );
	
	// Check for MySQL or SQLite connection
	if ( g_DBType == DB_MYSQL || g_DBType == DB_SQLITE )
	{

		// Verify we have a database connection
		if ( !DB_Check_Connection() )
		{
			return;
		}
		
		// Prepare name for the query (playername is 66 in case all 33 characters are ')
		DB_FormatString( szPlayerName, 65 );

		// Save the data
		new szQuery[512];
		format( szQuery, 511, "REPLACE INTO `%s` (`playerid`, `playerip`, `playername`, `xp`, `race`, `skill1`, `skill2`, `skill3`, `skill4`) VALUES ('%s', '%s', '%s', %d, %d, %d, %d, %d, %d)", g_DBTableName, szPlayerID, szPlayerIP, szPlayerName, p_data[id][P_XP], p_data[id][P_RACE], p_data[id][P_SKILL1], p_data[id][P_SKILL2], p_data[id][P_SKILL3], p_data[id][P_ULTIMATE] );

		new Result:res = dbi_query( g_DB, szQuery );
		
		// Verify we have a result
		if ( res < RESULT_NONE )
		{
			client_print( id, print_chat, "%s An error has occurred when saving your race information, please contact a server administrator", g_MODclient );
			DB_Error( res, szQuery, 4 );

			return;
		}
	}
	
	// Saving via vault
	else if ( g_DBType == DB_VAULT )
	{

		new szKey[128], szData[512];
		new iRaceID = p_data[id][P_RACE];
		
		// Format the data for entry
		format( szData, 511, "%s %d %d %d %d %d %d %s %d %s", szPlayerID, p_data[id][P_XP], p_data[id][P_RACE], p_data[id][P_SKILL1], p_data[id][P_SKILL2], p_data[id][P_SKILL3], p_data[id][P_ULTIMATE], szPlayerIP, get_systime(), szPlayerName );
		
		// Format the vault key
		format( szKey, 127, "%s", ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_NAME ) ? szPlayerName : ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_IP ) ? szPlayerIP : szPlayerID ) ) );
		
		// Save the data
		nvault_set( g_Vault[iRaceID], szKey, szData );
	}

	return;
}

// Function will get the user's XP for each race
public DB_GetAllXP( id, iRaceXP[MAX_RACES] )
{
	// If we're not saving XP, why do this?
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) || !id )
	{
		return;
	}
	
	// These will be used in either case
	new szPlayerIP[20], szPlayerName[66], szPlayerID[32];
	get_user_name(		id, szPlayerName	, 65 );
	get_user_ip(		id, szPlayerIP		, 19 );
	get_user_authid(	id, szPlayerID		, 31 );

	// Open the database based on what DB Type we have
	if ( g_DBType == DB_MYSQL || g_DBType == DB_SQLITE )
	{

		// Verify we have a database connection
		if ( !DB_Check_Connection() )
		{
			return;
		}
	
		// Prepare name for the query
		DB_FormatString( szPlayerName, 65 );
	
		new szQuery[256];
		format(szQuery, 255, "SELECT `xp`, `race` FROM `%s` WHERE (`%s` = '%s')", g_DBTableName, g_szDBKey, ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_NAME ) ? szPlayerName : ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_IP ) ? szPlayerIP : szPlayerID ) ) );
		
		new Result:res = dbi_query( g_DB, szQuery );
		
		// Make sure we have a result
		if ( res < RESULT_NONE )
		{
			client_print( id, print_chat, "%s An error has occurred when retreiving your race information, please contact a server administrator", g_MODclient );
			
			DB_Error( res, szQuery, 7 );
			
			return;
		}

		// Loop through all of the records to find the XP data
		new szXP[8], szRace[2], iXP, iRace;
		while ( res && dbi_nextrow( res ) > 0 )
		{
			dbi_result( res, "xp", szXP, 7 );
			dbi_result( res, "race", szRace, 1 );

			iXP		= str_to_num( szXP		);
			iRace	= str_to_num( szRace	);
			
			// Save the user's XP in an array
			if ( iRace > 0 && iRace < MAX_RACES + 1 )
			{
				iRaceXP[iRace-1] = iXP;
			}
		}
		
		// Free the result set
		dbi_free_result( res ); 
	}

	else if ( g_DBType == DB_VAULT )
	{
		new iAttempt, szKey[128], szXP[8], szData[256], iTimestamp;

		for( new iRace = 1; iRace < 10; iRace++ )
		{
			format( szKey, 127, "%s", ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_NAME ) ? szPlayerName : ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_IP ) ? szPlayerIP : szPlayerID ) ) );

			iAttempt = nvault_lookup( g_Vault[iRace], szKey, szData, 255, iTimestamp );

			// Only want to check if a key was found
			if ( iAttempt )
			{
				format( szXP, 7, "" );

				// Parse the vault data
				parse( szData, szPlayerID, 31, szXP, 7 );

				// Save the user's XP in an array
				if ( iRace > 0 && iRace < MAX_RACES + 1 )
				{
					iRaceXP[iRace-1] = str_to_num( szXP );
				}
			}
		}
	}


	// We don't want to replace the player's current XP with whats in the database now do we ?
	if ( p_data[id][P_RACE] )
	{
		iRaceXP[p_data[id][P_RACE]-1] = p_data[id][P_XP];
	}

	return;
}

public DB_SetDataForRace( id )
{

	// If we're not saving XP, why do this?
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) || !id )
	{
		return;
	}

	// These will be used in either case
	new szPlayerIP[20], szPlayerName[66], szPlayerID[32];
	get_user_name(		id, szPlayerName	, 65 );
	get_user_ip(		id, szPlayerIP		, 19 );
	get_user_authid(	id, szPlayerID		, 31 );

	if ( g_DBType == DB_MYSQL || g_DBType == DB_SQLITE )
	{

		// Verify we have a database connection
		if ( !DB_Check_Connection() )
		{
			return;
		}

		// Prepare name for the query
		DB_FormatString( szPlayerName, 65 );

		new szQuery[256];
		format( szQuery, 255, "SELECT `xp`, `skill1`, `skill2`, `skill3`, `skill4` FROM `%s` WHERE (`%s` = '%s' AND `race` = %d)", g_DBTableName, g_szDBKey, ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_NAME ) ? szPlayerName : ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_IP ) ? szPlayerIP : szPlayerID ) ), p_data[id][P_RACE] );

		new Result:res = dbi_query( g_DB, szQuery );
		
		// Verify we have a result
		if ( res < RESULT_NONE )
		{
			client_print( id, print_chat, "%s An error has occurred when retreiving your race information, please contact a server administrator", g_MODclient );
			
			DB_Error( res, szQuery, 8 );

			return;
		}
		
		// Then we have data in the database
		if ( res && dbi_nextrow( res ) > 0 )
		{
			new szXP[8], szSkill1[2], szSkill2[2], szSkill3[2], szSkill4[2];

			dbi_result( res, "xp"		, szXP		, 7 );
			dbi_result( res, "skill1"	, szSkill1	, 1 );
			dbi_result( res, "skill2"	, szSkill2	, 1 );
			dbi_result( res, "skill3"	, szSkill3	, 1 );
			dbi_result( res, "skill4"	, szSkill4	, 1 );			

			p_data[id][P_XP]		= str_to_num( szXP		);
			p_data[id][P_SKILL1]	= str_to_num( szSkill1	);
			p_data[id][P_SKILL2]	= str_to_num( szSkill2	);
			p_data[id][P_SKILL3]	= str_to_num( szSkill3	);
			p_data[id][P_ULTIMATE]	= str_to_num( szSkill4	);
		}

		// The user has no record, start them at 0
		else
		{
			p_data[id][P_XP]		= 0;
			p_data[id][P_SKILL1]	= 0;
			p_data[id][P_SKILL2]	= 0;
			p_data[id][P_SKILL3]	= 0;
			p_data[id][P_ULTIMATE]	= 0;
		}
		
		// Free the result
		dbi_free_result( res );
	}

	else if ( g_DBType == DB_VAULT )
	{

		new szKey[128], szData[256], iAttempt, iTimestamp;
		new iRace = p_data[id][P_RACE];

		format( szKey, 127, "%s", ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_NAME ) ? szPlayerName : ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_IP ) ? szPlayerIP : szPlayerID ) ) );

		iAttempt = nvault_lookup( g_Vault[iRace], szKey, szData, 255, iTimestamp );
		
		// Then we found a record in the vault
		if ( iAttempt )
		{
			new szXP[8], szSkill1[2], szSkill2[2], szSkill3[2], szSkill4[2], szRace[2];

			// Parse the vault entry
			parse( szData, szPlayerID, 31, szXP, 7, szRace, 1, szSkill1, 1, szSkill2, 1, szSkill3, 1, szSkill4, 1 );

			p_data[id][P_XP]		= str_to_num(szXP);
			p_data[id][P_SKILL1]	= str_to_num(szSkill1);
			p_data[id][P_SKILL2]	= str_to_num(szSkill2);
			p_data[id][P_SKILL3]	= str_to_num(szSkill3);
			p_data[id][P_ULTIMATE]	= str_to_num(szSkill4);
		}

		// No record was found, lets start them at 0
		else
		{
			p_data[id][P_XP]		= 0;
			p_data[id][P_SKILL1]	= 0;
			p_data[id][P_SKILL2]	= 0;
			p_data[id][P_SKILL3]	= 0;
			p_data[id][P_ULTIMATE]	= 0;
		}
	}

	return;
}

// The id should be a unique number, so we know what function called it (useful for debugging)
public DB_Error( Result:res, query[], id )
{

	// Get the error message and log it
	new szError[256];
	new iError = dbi_error( g_DB, szError, 255 );
	log_amx( "Error in querying database, location: %d", id );
	log_amx( "Message: %s (%d)", szError, iError );
	log_amx( "Query statement: %s ", query );

	// Free the result
	if ( res > RESULT_FAILED )
	{
		dbi_free_result( res );
	}
}

// Prepares text for database insertion
public DB_FormatString( text[], len )
{
	replace_all( text, len, "'", "\'" );
}
