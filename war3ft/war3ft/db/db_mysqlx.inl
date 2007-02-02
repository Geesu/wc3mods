/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	MYSQLX
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define TOTAL_TABLES		5

new const szTables[TOTAL_TABLES][] = 
{
	"CREATE TABLE IF NOT EXISTS `wc3_player` ( `player_id` int(8) unsigned NOT NULL auto_increment, `player_steamid` varchar(25) NOT NULL default '', `player_ip` varchar(20) NOT NULL default '', `player_name` varchar(35) NOT NULL default '', `time` timestamp(14) NOT NULL, PRIMARY KEY  (`player_id`), KEY `player_name` (`player_name`), KEY `player_ip` (`player_ip`), KEY `player_steamid` (`player_steamid`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_player_race` ( `player_id` int(8) unsigned NOT NULL default '0', `race_id` tinyint(4) unsigned NOT NULL default '0', `race_xp` int(8) default NULL, PRIMARY KEY  (`player_id`,`race_id`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_player_skill` ( `player_id` int(8) unsigned NOT NULL default '0', `skill_id` tinyint(4) unsigned NOT NULL default '0', `skill_level` tinyint(4) unsigned NOT NULL default '0', PRIMARY KEY  (`player_id`,`skill_id`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_web_race` ( `race_id` tinyint(4) unsigned NOT NULL default '0', `race_lang` char(2) NOT NULL default '', `race_name` varchar(100) default NULL, PRIMARY KEY  (`race_id`,`race_lang`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_web_skill` ( `skill_id` tinyint(4) unsigned NOT NULL default '0', `skill_lang` char(2) NOT NULL default '', `skill_name` varchar(100) default NULL, PRIMARY KEY  (`skill_id`,`skill_lang`) ) TYPE=MyISAM;"
};

/*
-- --------------------------------------------------------

-- 
-- Table structure for table `wc3_player`
-- 

CREATE TABLE IF NOT EXISTS `wc3_player` (
  `player_id` int(8) unsigned NOT NULL auto_increment,
  `player_steamid` varchar(25) NOT NULL default '',
  `player_ip` varchar(20) NOT NULL default '',
  `player_name` varchar(35) NOT NULL default '',
  `time` timestamp(14) NOT NULL,
  PRIMARY KEY  (`player_id`),
  KEY `player_name` (`player_name`),
  KEY `player_ip` (`player_ip`),
  KEY `player_steamid` (`player_steamid`)
) TYPE=MyISAM;

-- --------------------------------------------------------

-- 
-- Table structure for table `wc3_player_race`
-- 

CREATE TABLE IF NOT EXISTS `wc3_player_race` (
  `player_id` int(8) unsigned NOT NULL default '0',
  `race_id` tinyint(4) unsigned NOT NULL default '0',
  `race_xp` int(8) default NULL,
  PRIMARY KEY  (`player_id`,`race_id`)
) TYPE=MyISAM;

-- --------------------------------------------------------

-- 
-- Table structure for table `wc3_player_skill`
-- 

CREATE TABLE IF NOT EXISTS `wc3_player_skill` (
  `player_id` int(8) unsigned NOT NULL default '0',
  `skill_id` tinyint(4) unsigned NOT NULL default '0',
  `skill_level` tinyint(4) unsigned NOT NULL default '0',
  PRIMARY KEY  (`player_id`,`skill_id`)
) TYPE=MyISAM;

-- --------------------------------------------------------

-- 
-- Table structure for table `wc3_web_race`
-- 

CREATE TABLE IF NOT EXISTS `wc3_web_race` (
  `race_id` tinyint(4) unsigned NOT NULL default '0',
  `race_lang` char(2) NOT NULL default '',
  `race_name` varchar(100) default NULL,
  PRIMARY KEY  (`race_id`,`race_lang`)
) TYPE=MyISAM;

-- --------------------------------------------------------

-- 
-- Table structure for table `wc3_web_skill`
-- 

CREATE TABLE IF NOT EXISTS `wc3_web_skill` (
  `skill_id` tinyint(4) unsigned NOT NULL default '0',
  `skill_lang` char(2) NOT NULL default '',
  `skill_name` varchar(100) default NULL,
  PRIMARY KEY  (`skill_id`, `skill_lang`)
) TYPE=MyISAM;

*/

// Initiate the connection to the MySQL database
MYSQLX_Init()
{
	// Determine the database information
	new szHost[64], szUser[32], szPass[32], szDB[128], szError[256], iErrNum;

	get_pcvar_string( CVAR_wc3_sql_dbhost	, szHost		, 63	);
	get_pcvar_string( CVAR_wc3_sql_dbuser	, szUser		, 31	);
	get_pcvar_string( CVAR_wc3_sql_dbpass	, szPass		, 31	);
	get_pcvar_string( CVAR_wc3_sql_dbname	, szDB			, 127	);

	// Set up the tuple that will be used for threading
	g_DBTuple = SQL_MakeDbTuple( szHost, szUser, szPass, szDB );

	// Attempt to connect
	g_DBConn = SQL_Connect( g_DBTuple, iErrNum, szError, 255 );

	if ( !g_DBConn )
	{
		log_amx( "[MYSQLX] Database Connection Failed: [%d] %s", iErrNum, szError );

		return;
	}

	server_print( "[WAR3FT] MySQL X database connection successful" );

	// Create tables!
	MYSQLX_CreateTables();

	// Do we need to update the skills/races ?
	MYSQLX_UpdateWebTable();

	bDBAvailable = true;
}

// Verifies that the database connection is ok
MYSQLX_Check_Connection()
{
	
	if ( !bDBAvailable )
	{
		return false;
	}

	return true;
}

// Create all of our tables!
MYSQLX_CreateTables()
{
	new Handle:query;

	// Create the default tables if we need to
	for ( new i = 0; i < TOTAL_TABLES; i++ )
	{
		query = SQL_PrepareQuery( g_DBConn, szTables[i] );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szTables[i], 1 );

			return;
		}

		SQL_FreeHandle( query );
	}
}

MYSQLX_FetchUniqueID( id )
{
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
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
		MYSQLX_Error( query, szQuery, 1 );

		return;
	}

	// If no rows we need to insert!
	if ( SQL_NumResults( query ) == 0 )
	{
		// Free the last handle!
		SQL_FreeHandle( query );

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
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( id );

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[id][P_RACE], p_data[id][P_XP] );
	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save", szQuery );

	static iCurrentLevel;

	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		iCurrentLevel = SM_GetSkillLevel( id, iSkillID );

		// Then we need to save this!
		if ( iCurrentLevel > 0 && g_iDBPlayerSkillStore[id][iSkillID] != iCurrentLevel )
		{
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
		SQL_GetQueryString( query, szQuery, 255 );
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 1 );
	}
}

MYSQLX_GetAllXP( id )
{
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

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
		SQL_GetQueryString( query, szQuery, 255 );
		
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
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

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
		SQL_GetQueryString( query, szQuery, 255 );
		
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
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

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
		SQL_GetQueryString( query, szQuery, 255 );
		
		MYSQLX_ThreadError( query, szQuery, error, errnum, failstate, 4 );
	}

	// Query successful, we can do stuff!
	else
	{
		// Free the handle
		SQL_FreeHandle( query );
	}

	return;
}

MYSQLX_UpdateWebTable()
{
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

	new iTotalLanguages = get_langsnum();
	new lang[3], iLang, i;
	new szQuery[256], szName[64];

	// Loop through all languages
	for ( iLang = 0; iLang < iTotalLanguages; iLang++ )
	{
		get_lang ( iLang, lang );
		
		// We have a valid language
		if ( lang_exists( lang ) )
		{

			// Check all races
			for ( i = 1; i <= MAX_RACES; i++ )
			{
				lang_GetRaceName ( i, iLang, szName, 63 );

				formatex( szQuery, 255, "REPLACE INTO `wc3_web_race` ( `race_id` , `race_lang` , `race_name` ) VALUES ( '%d', '%s', '%s' );", i, lang, szName );

				SQL_ThreadQuery( g_DBTuple, "_MYSQLX_UpdateWebTable", szQuery );	
			}

			// Check all skills
			for ( i = 0; i < MAX_SKILLS; i++ )
			{
				LANG_GetSkillName ( i, iLang, szName, 63, 200 );

				formatex( szQuery, 255, "REPLACE INTO `wc3_web_skill` ( `skill_id` , `skill_lang` , `skill_name` ) VALUES ( '%d', '%s', '%s' );", i, lang, szName );

				SQL_ThreadQuery( g_DBTuple, "_MYSQLX_UpdateWebTable", szQuery );	
			}

		}
	}// End language loop
}

public _MYSQLX_UpdateWebTable( failstate, Handle:query, error[], errnum, data[], size )
{
	// Error during the query
	if ( failstate )
	{
		new szQuery[256];
		SQL_GetQueryString( query, szQuery, 255 );
		
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

#define MYSQL_TOTAL_PRUNE_QUERY 3

// Prune the MySQL database
MYSQLX_Prune()
{
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

	new const szPruneQuery[MYSQL_TOTAL_PRUNE_QUERY][] = 
	{
		"DELETE FROM wc3_player_race  WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( DATE_SUB(CURDATE(), INTERVAL %d DAY) > time );",
		"DELETE FROM wc3_player_skill WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( DATE_SUB(CURDATE(), INTERVAL %d DAY) > time );",
		"DELETE FROM wc3_player WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( DATE_SUB(CURDATE(), INTERVAL %d DAY) > time );"
	};
	new szQuery[256];

	// Need to run all 3 queries
	for ( new i = 0; i < MYSQL_TOTAL_PRUNE_QUERY; i++ )
	{
		formatex( szQuery, 255, szPruneQuery[i], get_pcvar_num( CVAR_wc3_days_before_delete ) );
		server_print( szQuery );

		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 6 );

			return;
		}
	}
}

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