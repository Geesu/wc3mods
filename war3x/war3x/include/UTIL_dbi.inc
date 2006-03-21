
/* - AMX Mod Include -------------------------------------------- *

 *  UTIL_dbi
 *  (c) Copyright 2003-2004, Created by Ryan.
 *
 *  A collection of useful functions that are used in the war3x
 *  plugin source.
 *
 *  ( 09/25/04 ) Revised UTIL_mysql for AMXX 0.20 by Pimp Daddy
 *  ( 02/20/06 ) Added SQL Lite support - Geesu
 *
 *  Future Additions:  Along with server print... log_amx the errors...
 *
 *
 *  Enjoy!

 * -------------------------------------------------------------- */


new Sql:sql;

new g_iXPfetched[33][_TR];                  // mirrors what the database has found, to verify what data needs to be saved
new g_SaveQueue_iXPfetched[_TQ][_TR];


// sql Initialization

public mysql_init(){
	#if ADVANCED_DEBUG
		log_function("mysql_init");
	#endif

	// Check the DBI loaded variable

	if ( !g_DBILoaded )
	{
		// Fall back to vault
		set_cvar_num("war3x_save_xp_sql", 0);
	}


	// Verify the type of DBI module that is loaded is supported
	dbi_type(g_SQLtype, 15);
	if (equali(g_SQLtype, g_MySQL))
	{
		g_iSQLtype = SQL_MYSQL;
	}
	else if (equali(g_SQLtype, g_SQLite))
	{
		g_iSQLtype = SQL_SQLITE;
	}
	else
	{
		g_iSQLtype = SQL_NONE;
		log_amx("Unsupported database type found (%s), the supported databases are %s or %s", g_SQLtype, g_MySQL, g_SQLite);
		return PLUGIN_CONTINUE;
	}

    new mysqlHost[64], mysqlUser[32], mysqlPass[32], mysqlDb[32], mysqlError[128];

    get_cvar_string( "war3x_sql_host",	mysqlHost,			63 );
    get_cvar_string( "war3x_sql_user",	mysqlUser,			31 );
    get_cvar_string( "war3x_sql_pass",	mysqlPass,			31 );
    get_cvar_string( "war3x_sql_db",	mysqlDb,			31 );
	get_cvar_string( "war3x_sql_table",	g_mysqlTableName,	31 );


	// Set a default DB if it's SQLite and the user didn't supply one

	if ( g_iSQLtype == SQL_SQLITE && (strlen(mysqlDb) < 1 || equali(mysqlDb, "war3x")) )
	{
		copy( mysqlDb, 127, "addons/amxmodx/data/amxx.db" );
	}
	
    // Connect to SQL

    sql = dbi_connect( mysqlHost, mysqlUser, mysqlPass, mysqlDb, mysqlError, 128 );
	
	// Verify our database connection has worked
	if ( !Check_DBI_Connection() )
	{
		log_amx( "%s connection error: %s", g_SQLtype, mysqlError );
		return PLUGIN_CONTINUE;
	}

	log_amx( "%s connection established.", g_SQLtype );

	// Format the create table statement
	new szQuery[512];
	if ( g_iSQLtype == SQL_MYSQL )
	{
		format( szQuery, 511, "CREATE TABLE IF NOT EXISTS `%s` ( `playerid` varchar(32) NOT NULL, `race` int(2) NOT NULL, `xp` int(10) default '0', `skills` varchar(32), `lastplayed` date default '0000-00-00', PRIMARY KEY  (`playerid`,`race`) ) TYPE=MyISAM;", g_mysqlTableName );
	}
	else if ( g_iSQLtype == SQL_SQLITE )
	{
		format( szQuery, 511, "CREATE TABLE `%s` ( `playerid` varchar(32) NOT NULL DEFAULT '', `race` int(2) NOT NULL DEFAULT 0, `xp` int(10) NOT NULL DEFAULT 0, `skills` VARCHAR(32) NOT NULL DEFAULT '', `lastplayed` TIMESTAMP(14) NOT NULL DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY  (`playerid`,`race`) );", g_mysqlTableName  );
	}

	// Execute our CREATE TABLE statement if it's MySQL or the SQL LITE table doesn't exist
	if ( g_iSQLtype != SQL_SQLITE || !sqlite_table_exists(sql, g_mysqlTableName) )
	{
		new Result:ret = dbi_query(sql, szQuery)

		if ( ret < RESULT_NONE )
		{
			DB_Error( ret, szQuery, 1 );
			return PLUGIN_CONTINUE;
		}
	}

	if ( g_iSQLtype == SQL_SQLITE ) 
	{
		new szIntegrityCheck[64];
		format(szQuery, 511, "PRAGMA integrity_check");
		new Result:res = dbi_query(sql, szQuery);
		
		// Check for an error
		if ( res < RESULT_NONE )
		{
			DB_Error( res, szQuery, 2 );
			return PLUGIN_CONTINUE;
		}
		
		// Get the integrity check value
		while ( res && dbi_nextrow(res) > 0 )
		{
			dbi_result(res, "integrity_check", szIntegrityCheck, 63);
		}

		// Free the result
		dbi_free_result(res);

		// Check to make sure the integrity check passed
		if ( !equali(szIntegrityCheck, "ok") )
		{
			// Should we disable saving here?
			log_amx("SQL Lite integrity check failed");
			return PLUGIN_CONTINUE;
		}
			
		// Do some synchronous crap
		format(szQuery, 511, "PRAGMA synchronous = %d", SQLITE_SYNC_OFF);
		dbi_query(sql, szQuery);
	}

    return PLUGIN_HANDLED;
}


// Retrieve Player XP from Database

public get_mysql_xp( id ) {
	#if ADVANCED_DEBUG
		log_function("get_mysql_xp");
	#endif

	// Make sure we have a valid SQL Connection

	if ( !Check_DBI_Connection() )
	{
		return PLUGIN_CONTINUE;
	}


    new mysqlQuery[256];

    format( mysqlQuery, 255, "SELECT `race`, `xp` FROM `%s` WHERE (`playerid` = '%s' AND `race` > '0') ORDER BY race ASC", g_mysqlTableName, g_SaveIds[id] );

    new Result:res = dbi_query( sql, mysqlQuery );

    if ( res < RESULT_NONE )
    {
		DB_Error( res, mysqlQuery, 3 );
    }

    else
    {
        new race[3];
        new xp[11];

        while ( res && dbi_nextrow( res ) > 0 )
        {
            dbi_result( res, "xp", xp, 10 );
            dbi_result( res, "race", race, 2 );

            g_iXPtotal[id][str_to_num( race ) - 1] = str_to_num( xp );
            g_iXPfetched[id][str_to_num( race ) - 1] = str_to_num( xp );        // mirror is useful, later
        }

        dbi_free_result( res );
    }

    return PLUGIN_HANDLED;
}


// Retrieve Player Skills from Database

public get_mysql_skills( id, iRaceId, szData[64] ) {
	#if ADVANCED_DEBUG
		log_function("get_mysql_skills");
	#endif


	// Make sure we have a valid SQL Connection

	if ( !Check_DBI_Connection() )
	{
		return PLUGIN_CONTINUE;
	}

    new mysqlQuery[256];

    format( mysqlQuery, 255, "SELECT skills FROM `%s` WHERE playerid = '%s' AND race = '%d' LIMIT 1", g_mysqlTableName, g_SaveIds[id], iRaceId );

    new Result:res = dbi_query( sql, mysqlQuery );

    if ( res < RESULT_NONE )
    {
		DB_Error( res, mysqlQuery, 4 );
    }

    else
    {
        if ( res && dbi_nextrow( res ) > 0 )
        {
            dbi_result( res, "skills", szData, 31 );
        }

        dbi_free_result( res );
    }

    return PLUGIN_HANDLED;
}


// Stores all Player info to Database

public write_mysql_all( szSaveId[32], szData[32], iRaceId, iXP, bUpdate ) {
	#if ADVANCED_DEBUG
		log_function("write_mysql_all");
	#endif

	// Make sure we have a valid SQL Connection

	if ( !Check_DBI_Connection() )
	{
		return PLUGIN_CONTINUE;
	}

    new szLastPlayed[16];
    new mysqlQuery[256];

    get_time( "%Y-%m-%d", szLastPlayed, 15 );

    if ( !bUpdate )
	{
		if ( g_iSQLtype == SQL_SQLITE )
		{
			format( mysqlQuery, 255, "INSERT INTO `%s` VALUES ('%s', %d, %d, '%s', '%s')", g_mysqlTableName, szSaveId, iRaceId, iXP, szData, szLastPlayed );
		}
		else
		{
			format( mysqlQuery, 255, "INSERT DELAYED INTO `%s` VALUES ('%s', %d, %d, '%s', '%s')", g_mysqlTableName, szSaveId, iRaceId, iXP, szData, szLastPlayed );
		}
	}
    else
    {
        format( mysqlQuery, 255, "UPDATE `%s` SET xp = %d, skills = '%s', lastplayed = '%s' WHERE playerid = '%s' AND race = '%d' AND xp < %d", g_mysqlTableName, iXP, szData, szLastPlayed, szSaveId, iRaceId, iXP + 100 );
    }

    new Result:res = dbi_query( sql, mysqlQuery );

    if ( res < RESULT_NONE )
    {
		DB_Error( res, mysqlQuery, 5 );
    }

    return PLUGIN_HANDLED;
}


// Stores all Player XP to Database

public write_mysql_xp( szSaveId[32], iRaceId, iXP, bUpdate ) {
	#if ADVANCED_DEBUG
		log_function("write_mysql_xp");
	#endif

	// Make sure we have a valid SQL Connection

	if ( !Check_DBI_Connection() )
	{
		return PLUGIN_CONTINUE;
	}

    new szLastPlayed[16];
    new mysqlQuery[256];

    get_time( "%Y-%m-%d", szLastPlayed, 15 );

    if ( !bUpdate )
        format( mysqlQuery, 255, "INSERT INTO `%s` VALUES ('%s', %d, %d, '0,0,0,0', '%s')", g_mysqlTableName, szSaveId, iRaceId, iXP, szLastPlayed );

    else
    {
        format( mysqlQuery, 255, "UPDATE `%s` SET xp = %d, lastplayed = '%s' WHERE playerid = '%s' AND race = %d AND xp < %d", g_mysqlTableName, iXP, szLastPlayed, szSaveId, iRaceId, iXP + 100 );
    }

    new Result:res = dbi_query( sql, mysqlQuery );

    if ( res < RESULT_NONE )
    {
		DB_Error( res, mysqlQuery, 6 );
    }

    return PLUGIN_HANDLED;
}


// Stores all Player options to Database

public write_mysql_options( szSaveId[32], szData[32] ) {
	#if ADVANCED_DEBUG
		log_function("write_mysql_options");
	#endif

	// Make sure we have a valid SQL Connection

	if ( !Check_DBI_Connection() )
	{
		return PLUGIN_CONTINUE;
	}

    new szLastPlayed[16];
    new mysqlQuery[256];

    get_time( "%Y-%m-%d", szLastPlayed, 15 );

    format( mysqlQuery, 255, "REPLACE INTO `%s` VALUES ('%s', 0, 0, '%s', '%s')", g_mysqlTableName, szSaveId, szData, szLastPlayed );

    new Result:res = dbi_query( sql, mysqlQuery );

    if ( res < RESULT_NONE )
    {
		DB_Error( res, mysqlQuery, 7 );
    }

    return PLUGIN_HANDLED;
}


// Stores all Player skills to Database

public write_mysql_skills( szSaveId[32], iRaceId, szData[32] ) {
	#if ADVANCED_DEBUG
		log_function("write_mysql_skills");
	#endif

	// Make sure we have a valid SQL Connection

	if ( !Check_DBI_Connection() )
	{
		return PLUGIN_CONTINUE;
	}

    new szLastPlayed[16];
    new mysqlQuery[256];

    get_time( "%Y-%m-%d", szLastPlayed, 15 );

    format( mysqlQuery, 255, "UPDATE `%s` SET skills = '%s', lastplayed = '%s' WHERE playerid = '%s' AND race = %d", g_mysqlTableName, szData, szLastPlayed, szSaveId, iRaceId );

    new Result:res = dbi_query( sql, mysqlQuery );

    if ( res < RESULT_NONE )
    {
		DB_Error( res, mysqlQuery, 8 );
    }

    return PLUGIN_HANDLED;
}


public mysql_close_all() {
	#if ADVANCED_DEBUG
		log_function("mysql_close_all");
	#endif

	// Make sure we have a valid SQL Connection

	if ( !Check_DBI_Connection() )
	{
		return PLUGIN_CONTINUE;
	}

    dbi_close( sql );

    return PLUGIN_HANDLED;
}


// The id should be a unique number, so we know what function called it (useful for debugging)

public DB_Error( Result:res, query[], id )
{
	#if ADVANCED_DEBUG
		log_function("DB_Error");
	#endif

	// Get the error message and log it
	new szError[255];
	new iError = dbi_error(sql, szError, 254);
	log_amx("Error in querying database, location: %d", id);
	log_amx("Message: %s (%d)", szError, iError);
	log_amx("Query statement: %s ", query);

	// Free the result
	if ( res > RESULT_FAILED )
	{
		dbi_free_result(res);
	}
}


// Verifies that the database connection is ok

public Check_DBI_Connection()
{
	#if ADVANCED_DEBUG
		log_function("Check_DBI_Connection");
	#endif

	if( sql < SQL_OK )
	{
		log_amx("Unable to connect to the %s database, switching to short-term mode", g_SQLtype)
		set_cvar_num("war3x_save_xp_sql",	0);
		set_cvar_num("war3x_save_xp",		0);

		return false;
	}

	return true;
}

// End of UTIL_DBI.INC
