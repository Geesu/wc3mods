/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	SQLite
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

// Initiate the connection to the SQLite database
SQLITE_Init()
{
	iSQLAttempts++;

	new szDB[] = "addons/amxmodx/data/war3ft.db";
	new szError[256];

	// Attempt the Connection
	g_DB = dbi_connect( "", "", "", szDB, szError, 255 );

	// Verify our database connection has worked
	if ( !SQLITE_Check_Connection() )
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
			SQLITE_Error( ret, szQuery, 1 );

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
		SQLITE_Error( res, szQuery, 2 );
		return;
	}
	
	// Get the integrity check value
	while ( res && dbi_nextrow(res) > 0 )
	{
		dbi_result( res, "integrity_check", szIntegrityCheck, 63 );
	}

	// Free the result
	dbi_free_result( res );

	// Check to make sure the integrity check passed
	if ( !equali(szIntegrityCheck, "ok") )
	{
		// Should we disable saving here?
		log_amx( "[SQLITE] SQL Lite integrity check failed, disabling saving XP." );

		set_pcvar_num( CVAR_wc3_save_xp, 0 );

		return;
	}
		
	// Do some synchronous crap
	format( szQuery, 511, "PRAGMA synchronous = %d", SQLITE_SYNC_OFF );
	dbi_query( g_DB, szQuery );

	// Run an upgrade if we need to
	SQLITE_Upgrade();

}

SQLITE_Upgrade()
{

	new szQuery[256];
	//format( szQuery, 255, "SELECT sql FROM sqlite_master" );
	format( szQuery, 255, "SELECT * FROM sqlite_master WHERE type = 'table' AND name = '%s';", g_DBTableName );

	new Result:res = dbi_query( g_DB, szQuery );
	
	// We have a result, lets check to see if 'playerip' is in the result
	new szResultData[512];
	new bool:bFound = false;

	if ( res && dbi_nextrow( res ) > 0 )
	{
		for ( new i = 1; i <= dbi_num_fields( res ); i++ )
		{
			dbi_result( res, "sql", szResultData, 511 );

			if ( containi( szResultData, "playerip" ) != -1 )
			{
				bFound = true;
			}
		}
	}

	// Free the result
	dbi_free_result( res );

	
	// We didn't find the field, we need to add it
	if ( !bFound )
	{
		format( szQuery, 255, "ALTER TABLE `%s` ADD `playerip` VARCHAR( 20 ) NOT NULL DEFAULT '';", g_DBTableName );
		
		new Result:ret = dbi_query( g_DB, szQuery );

		if ( ret < RESULT_NONE )
		{
			SQLITE_Error( ret, szQuery, 3 );

			return;
		}
	}

	return;
}

// Close the SQLite connection
SQLITE_Close()
{
	if ( g_DB )
	{
		dbi_close( g_DB );
	}
}

SQLITE_Save( id )
{
	// Verify we have a database connection
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}
	
	new szPlayerIP[20], szPlayerName[66], szPlayerID[32];
	get_user_name(		id, szPlayerName	, 65 );
	get_user_ip(		id, szPlayerIP		, 19 );
	get_user_authid(	id, szPlayerID		, 31 );

	// Prepare name for the query (playername is 66 in case all 33 characters are ')
	DB_FormatString( szPlayerName, 65 );

	// Save the data
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `%s` (`playerid`, `playerip`, `playername`, `xp`, `race`, `skill1`, `skill2`, `skill3`, `skill4`) VALUES ('%s', '%s', '%s', %d, %d, %d, %d, %d, %d)", g_DBTableName, szPlayerID, szPlayerIP, szPlayerName, p_data[id][P_XP], p_data[id][P_RACE], p_data[id][P_SKILL1], p_data[id][P_SKILL2], p_data[id][P_SKILL3], p_data[id][P_ULTIMATE] );

	new Result:res = dbi_query( g_DB, szQuery );
	
	// Verify we have a result
	if ( res < RESULT_NONE )
	{
		SQLITE_Error( res, szQuery, 4 );

		return;
	}

	return;
}

SQLITE_GetAllXP( id )
{
	// Verify we have a database connection
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new szKey[66];
	DB_GetKey( id, szKey, 65 );

	new szQuery[256];
	format(szQuery, 255, "SELECT `xp`, `race` FROM `%s` WHERE (`%s` = '%s')", g_DBTableName, g_szDBKey, szKey );
	
	new Result:res = dbi_query( g_DB, szQuery );
	
	// Make sure we have a result
	if ( res < RESULT_NONE )
	{
		SQLITE_Error( res, szQuery, 5 );
		
		return;
	}

	// Loop through all of the records to find the XP data
	new szXP[8], szRace[2], iXP, iRace, iRaceXP[MAX_RACES] = {0};
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

	// Call the function that will display the "select a race" menu
	WC3_ChangeRaceEnd( id, iRaceXP );

	return;
}

SQLITE_SetData( id )
{
	// Verify we have a database connection
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new szKey[66];
	DB_GetKey( id, szKey, 65 );

	new szQuery[256];
	format( szQuery, 255, "SELECT `xp`, `skill1`, `skill2`, `skill3`, `skill4` FROM `%s` WHERE (`%s` = '%s' AND `race` = %d)", g_DBTableName, g_szDBKey, szKey );

	new Result:res = dbi_query( g_DB, szQuery );
	
	// Verify we have a result
	if ( res < RESULT_NONE )
	{
		SQLITE_Error( res, szQuery, 6 );

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

	// Set the race up
	WC3_SetRaceUp( id );

	return;
}

// Verifies that the database connection is ok
SQLITE_Check_Connection()
{
	
	if ( g_DB < SQL_OK )
	{
		// Lets try to re-connect in a certain amount of time
		if ( iSQLAttempts < SQL_ATTEMPTS )
		{
			if ( !task_exists( TASK_SETSQL ) )
			{
				log_amx( "[SQLITE] Database connection failed, attempting to reconnect in %0.0f seconds", SQL_ATTEMPT_DELAY );

				set_task( SQL_ATTEMPT_DELAY, "SQLITE_Init", TASK_SETSQL );
			}
		}

		// We have reached the maximum amount of attempts, lets default to short-term XP
		else if( iSQLAttempts >= SQL_ATTEMPTS )
		{
			set_pcvar_num( CVAR_wc3_save_xp, 0 );
			set_pcvar_num( CVAR_wc3_save_xp_sql, 0 );

			log_amx( "[SQLITE] Unable to connect to the %s database after %d attempts, switching to short-term mode", g_szDBType, iSQLAttempts )
		}

		return false;
	}

	return true;
}

// The id should be a unique number, so we know what function called it (useful for debugging)
SQLITE_Error( Result:res, query[], id )
{

	// Get the error message and log it
	new szError[256];
	new iError = dbi_error( g_DB, szError, 255 );
	log_amx( "[SQLITE] Error in querying database, location: %d", id );
	log_amx( "[SQLITE] Message: %s (%d)", szError, iError );
	log_amx( "[SQLITE] Query statement: %s ", query );

	// Free the result
	if ( res > RESULT_FAILED )
	{
		dbi_free_result( res );
	}
}
