/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	MYSQLX
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define TOTAL_TABLES		7

new const szTables[TOTAL_TABLES][] = 
{
	"CREATE TABLE IF NOT EXISTS `wc3_player` ( `player_id` int(8) unsigned NOT NULL auto_increment, `player_steamid` varchar(25) NOT NULL default '', `player_ip` varchar(20) NOT NULL default '', `player_name` varchar(35) NOT NULL default '', `time` timestamp(14) NOT NULL, PRIMARY KEY  (`player_id`), KEY `player_name` (`player_name`), KEY `player_ip` (`player_ip`), KEY `player_steamid` (`player_steamid`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_player_extra` ( `player_id` INT( 8 ) UNSIGNED NOT NULL , `player_steamid` VARCHAR( 25 ) NOT NULL , `player_ip` VARCHAR( 20 ) NOT NULL , `player_name` VARCHAR( 35 ) NOT NULL , PRIMARY KEY ( `player_id` )) TYPE=MyISAM ;",
	"CREATE TABLE IF NOT EXISTS `wc3_player_race` ( `player_id` int(8) unsigned NOT NULL default '0', `race_id` tinyint(4) unsigned NOT NULL default '0', `race_xp` int(8) default NULL, PRIMARY KEY  (`player_id`,`race_id`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_player_skill` ( `player_id` int(8) unsigned NOT NULL default '0', `skill_id` tinyint(4) unsigned NOT NULL default '0', `skill_level` tinyint(4) unsigned NOT NULL default '0', PRIMARY KEY  (`player_id`,`skill_id`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_web_race` ( `race_id` tinyint(4) unsigned NOT NULL default '0', `race_lang` char(2) NOT NULL default '', `race_name` varchar(100) default NULL, `race_description` text NOT NULL, PRIMARY KEY  (`race_id`,`race_lang`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_web_skill` ( `skill_id` tinyint(4) unsigned NOT NULL default '0', `skill_lang` char(2) NOT NULL default '', `skill_name` varchar(100) default NULL, `skill_description` text NOT NULL, `skill_type` tinyint(4) unsigned NOT NULL default '0', `skill_owner` tinyint(4) unsigned NOT NULL default '0', PRIMARY KEY  (`skill_id`,`skill_lang`) ) TYPE=MyISAM;",
	"CREATE TABLE IF NOT EXISTS `wc3_config` ( `config_id` varchar(50) NOT NULL, `config_value` varchar(255) NOT NULL, PRIMARY KEY  (`config_id`) ) TYPE=MyISAM;"
};

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
		WC3_Log( true, "[MYSQLX] Database Connection Failed: [%d] %s", iErrNum, szError );

		return;
	}

	server_print( "[WAR3FT] MySQL X database connection successful" );

	bDBAvailable = true;

	// Create tables!
	MYSQLX_CreateTables();

	// Do we need to update the skills/races ?
	MYSQLX_UpdateWebTable();

	// Do we need to run a conversion?
	MYSQLX_Convert();
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
		MYSQLX_Error( query, szQuery, 2 );

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
			MYSQLX_Error( query, szQuery, 3 );

			return;
		}

		g_iDBPlayerUniqueID[id] = SQL_GetInsertId( query );

		// Since we have the ID - lets insert extra data here...
		//  Basically insert whatever data we don't have yet on this player in the extra table 
		//  (this will only be used for the webpage)
		new szName[70], szSteamID[30], szIP[20];
		get_user_name( id, szName, 69 );
		DB_FormatString( szName, 69 );
		get_user_ip( id, szIP, 19, 1 );
		get_user_authid( id, szSteamID, 29 );

		format( szQuery, 511, "INSERT INTO `wc3_player_extra` ( `player_id` , `player_steamid` , `player_ip` , `player_name` ) VALUES ( '%d', '%s', '%s', '%s' );", g_iDBPlayerUniqueID[id], szSteamID, szIP, szName );
		query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 20 );

			return;
		}
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

	// Error checking when saving
	if ( iUniqueID <= 0 )
	{
		new szName[128];
		get_user_name( id, szName, 127 );

		WC3_Log( true, "Unable to save XP for user '%s', unique ID: %d", szName, iUniqueID );

		return;
	}

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[id][P_RACE], p_data[id][P_XP] );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		client_print( id, print_chat, "%s Error, unable to save your XP, please contact a server administrator", g_MODclient );

		MYSQLX_Error( query, szQuery, 4 );

		return;
	}

	static iCurrentLevel;

	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
		{
			iCurrentLevel = SM_GetSkillLevel( id, iSkillID, 14 );

			// Then we need to save this!
			if ( iCurrentLevel >= 0 && g_iDBPlayerSkillStore[id][iSkillID] != iCurrentLevel )
			{
				g_iDBPlayerSkillStore[id][iSkillID] = iCurrentLevel;
				format( szQuery, 511, "REPLACE INTO `wc3_player_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, iCurrentLevel );
				query = SQL_PrepareQuery( g_DBConn, szQuery );

				if ( !SQL_Execute( query ) )
				{
					client_print( id, print_chat, "%s Error, unable to save your XP, please contact a server administrator", g_MODclient );

					MYSQLX_Error( query, szQuery, 5 );

					return;
				}
			}
		}
	}

	return;
}

MYSQLX_Save_T( id )
{
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( id );

	// Error checking when saving
	if ( iUniqueID <= 0 )
	{
		new szName[128];
		get_user_name( id, szName, 127 );

		WC3_Log( true, "Unable to save XP for user '%s', unique ID: %d", szName, iUniqueID );

		return;
	}

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[id][P_RACE], p_data[id][P_XP] );
	SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save_T", szQuery );

	static iCurrentLevel;

	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
		{
			iCurrentLevel = SM_GetSkillLevel( id, iSkillID, 15 );

			// Then we need to save this!
			if ( iCurrentLevel >= 0 && g_iDBPlayerSkillStore[id][iSkillID] != iCurrentLevel )
			{
				g_iDBPlayerSkillStore[id][iSkillID] = iCurrentLevel;
				format( szQuery, 511, "REPLACE INTO `wc3_player_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, iCurrentLevel );
				SQL_ThreadQuery( g_DBTuple, "_MYSQLX_Save_T", szQuery );
			}
		}
	}

	return;
}

public _MYSQLX_Save_T( failstate, Handle:query, error[], errnum, data[], size )
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
	
	new iUniqueID = DB_GetUniqueID( id );

	// Then we have a problem and cannot retreive the user's XP
	if ( iUniqueID <= 0 )
	{
		client_print( id, print_chat, "%s Unable to retreive your XP from the database, please attempt to changerace later", g_MODclient );

		WC3_Log( true, "[ERROR] Unable to retreive user's Unique ID" );

		return;
	}

	new szQuery[256];
	format(szQuery, 255, "SELECT `race_id`, `race_xp` FROM `wc3_player_race` WHERE ( `player_id` = '%d' );", iUniqueID );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		client_print( id, print_chat, "%s Error, unable to retrieve XP, please contact a server administrator", g_MODclient );

		MYSQLX_Error( query, szQuery, 6 );

		return;
	}

	// Set last saved XP to 0
	for ( new i = 0; i < MAX_RACES; i++ )
	{
		g_iDBPlayerXPInfoStore[id][i] = 0;
	}

	// Get the XP!
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

	// Call the function that will display the "select a race" menu
	WC3_ChangeRaceShowMenu( id, g_iDBPlayerXPInfoStore[id] );

	return;
}


MYSQLX_SetDataForRace( id )
{
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

	new szQuery[256];
	format( szQuery, 255, "SELECT `skill_id`, `skill_level` FROM `wc3_player_skill` WHERE `player_id` = '%d';", DB_GetUniqueID( id ) );
	new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		client_print( id, print_chat, "%s Error, unable to retrieve XP, please contact a server administrator", g_MODclient );

		MYSQLX_Error( query, szQuery, 6 );

		return;
	}

	// Set the user's XP!
	p_data[id][P_XP] = g_iDBPlayerXPInfoStore[id][p_data[id][P_RACE]-1];

	// Reset all skill data to 0!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		if ( g_SkillType[iSkillID] != SKILL_TYPE_PASSIVE )
		{
			SM_SetSkillLevel( id, iSkillID, 0, 2 );
		}
	}
	
	new iSkillID, iSkillLevel;
	// While we have a result!
	while ( SQL_MoreResults( query ) )
	{
		iSkillID = SQL_ReadResult( query, 0 );
		iSkillLevel = SQL_ReadResult( query, 1 );
		SM_SetSkillLevel( id, iSkillID, iSkillLevel, 3 );
		g_iDBPlayerSkillStore[id][iSkillID] = iSkillLevel;

		SQL_NextRow( query );
	}

	// Free the handle
	SQL_FreeHandle( query );
	
	// Set the race up
	WC3_SetRaceUp( id );

	// This user's XP has been set + retrieved! We can save now
	bDBXPRetrieved[id] = true;


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

	WC3_Log( true, "[MYSQLX] Error in querying database, location: %d", id );
	WC3_Log( true, "[MYSQLX] Message: %s (%d)", szError, iErrNum );
	WC3_Log( true, "[MYSQLX] Query statement: %s ", szQuery );

	// Free the handle
	SQL_FreeHandle( query );
}

MYSQLX_ThreadError( Handle:query, szQuery[], szError[], iErrNum, failstate, id )
{
	WC3_Log( true, "[MYSQLX] Threaded query error, location: %d", id );
	WC3_Log( true, "[MYSQLX] Message: %s (%d)", szError, iErrNum );
	WC3_Log( true, "[MYSQLX] Query statement: %s ", szQuery );

	// Connection failed
	if ( failstate == TQUERY_CONNECT_FAILED )
	{	
		WC3_Log( true, "[MYSQLX] Fail state: Connection Failed" );
	}

	// Query failed
	else if ( failstate == TQUERY_QUERY_FAILED )
	{
		WC3_Log( true, "[MYSQLX] Fail state: Query Failed" );
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

	
	new szQuery[1024], Handle:query;

	// No matter what always update the XP levels
	for ( new iLevel = 0; iLevel <= MAX_LEVELS; iLevel++ )
	{
		formatex( szQuery, 255, "REPLACE INTO `wc3_config` ( `config_id`, `config_value` ) VALUES ( 'level%d_xp', '%d' );", iLevel, XP_GetByLevel( iLevel ) );
		query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 7 );

			return;
		}
	}



	// Check to see if we even need an update!
	formatex ( szQuery, 255, "SELECT `config_value` FROM `wc3_config` WHERE `config_id` = 'version' AND `config_value` = '%s';", WC3VERSION );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 7 );

		return;
	}

	// Then we don't need to update because we already have!
	if ( SQL_NumResults( query ) == 1 )
	{
		SQL_FreeHandle( query );

		return;
	}

	// We haven't update yet! Lets store the version
	else
	{
		// Free the last handle
		SQL_FreeHandle( query );

		// Insert current version!
		formatex ( szQuery, 1023, "REPLACE INTO `wc3_config` ( `config_id` , `config_value` ) VALUES ( 'version', '%s' );", WC3VERSION );
		query = SQL_PrepareQuery( g_DBConn, szQuery );
		
		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 8 );

			return;
		}
		
		// Now lets add the language information to the DB!
		new iTotalLanguages = get_langsnum();
		new lang[3], iLang, i;
		new szName[64], szDescription[512];

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

					formatex( szQuery, 1023, "REPLACE INTO `wc3_web_race` ( `race_id` , `race_lang` , `race_name`, `race_description` ) VALUES ( '%d', '%s', '%s', '' );", i, lang, szName );

					query = SQL_PrepareQuery( g_DBConn, szQuery );

					if ( !SQL_Execute( query ) )
					{
						MYSQLX_Error( query, szQuery, 9 );

						return;
					}
				}

				// Check all skills
				for ( i = 0; i < MAX_SKILLS; i++ )
				{
					LANG_GetSkillName ( i, iLang, szName, 63, 200 );
					LANG_GetSkillInfo( i, iLang, szDescription, 255 );		// Intentionally only 255 - technically this could be all '
					DB_FormatString( szDescription, 511 );

					formatex( szQuery, 1023, "REPLACE INTO `wc3_web_skill` ( `skill_id` , `skill_lang` , `skill_name`, `skill_description`, `skill_type`, `skill_owner` ) VALUES ( '%d', '%s', '%s', '%s', '%d', '%d' );", i, lang, szName, szDescription, g_SkillType[i], g_SkillOwner[i] );
					
					WC3_Log( false, szQuery );

					query = SQL_PrepareQuery( g_DBConn, szQuery );

					if ( !SQL_Execute( query ) )
					{
						MYSQLX_Error( query, szQuery, 10 );

						return;
					}
				}
			}
		}// End language loop
	}
}

#define MYSQL_TOTAL_PRUNE_QUERY 2

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
		"DELETE FROM wc3_player_race  WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( DATE_SUB(CURDATE(), INTERVAL %d DAY) > time ) );",
		"DELETE FROM wc3_player_skill WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( DATE_SUB(CURDATE(), INTERVAL %d DAY) > time ) );"
	};
	new szQuery[256];

	// Need to run all 3 queries
	for ( new i = 0; i < MYSQL_TOTAL_PRUNE_QUERY; i++ )
	{
		formatex( szQuery, 255, szPruneQuery[i], get_pcvar_num( CVAR_wc3_days_before_delete ) );

		new Handle:query = SQL_PrepareQuery( g_DBConn, szQuery );

		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 11 );

			return;
		}
	}
}

#define MYSQL_TOTAL_CONVERSION_QUERY 2
MYSQLX_Convert()
{
	// Make sure our connection is working
	if ( !MYSQLX_Check_Connection() )
	{
		return;
	}

	new const szConversionQuery[MYSQL_TOTAL_CONVERSION_QUERY][] = 
	{
		"INSERT INTO wc3_player select '', playerid, playerip, playername, time FROM `war3users` GROUP BY playerid;",
		"INSERT INTO wc3_player_race select wc3_player.player_id, war3users.race, war3users.xp FROM `wc3_player`, `war3users` WHERE wc3_player.player_steamid=war3users.playerid;"
	};
	
	new szQuery[256], Handle:query;


	// Check to see if the table even exists!
	new szDB[128];
	get_pcvar_string( CVAR_wc3_sql_dbname	, szDB			, 127	);
	formatex ( szQuery, 255, "SELECT count(*) FROM information_schema.tables WHERE table_schema = '%s' AND table_name = 'war3users';", szDB );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 12 );

		return;
	}

	// Weird this shouldn't occur as we should always have a count!
	if ( SQL_NumResults( query ) <= 0 )
	{
		return;
	}

	// Find out if the count is 1, if it's not then we need to return
	if ( SQL_ReadResult( query, 0 ) != 1 )
	{
		return;
	}

	// Now we need to check to see if the conversion has already been ran
	formatex ( szQuery, 255, "SELECT `config_value` FROM `wc3_config` WHERE `config_id` = 'sql_conversion' AND `config_value` = '1';" );
	query = SQL_PrepareQuery( g_DBConn, szQuery );

	if ( !SQL_Execute( query ) )
	{
		MYSQLX_Error( query, szQuery, 13 );

		return;
	}

	// Then we don't need to run the conversion because we already have!
	if ( SQL_NumResults( query ) == 1 )
	{
		SQL_FreeHandle( query );

		return;
	}

	// We haven't ran the conversion yet - run it!
	else
	{
		// Free the last handle
		SQL_FreeHandle( query );

		// Insert that we ran the conversion!
		formatex ( szQuery, 255, "REPLACE INTO `wc3_config` ( `config_id` , `config_value` ) VALUES ( 'sql_conversion', '1' );" );
		query = SQL_PrepareQuery( g_DBConn, szQuery );
		
		if ( !SQL_Execute( query ) )
		{
			MYSQLX_Error( query, szQuery, 14 );

			return;
		}

		// Need to run all the queries
		for ( new i = 0; i < MYSQL_TOTAL_CONVERSION_QUERY; i++ )
		{
			new Handle:query = SQL_PrepareQuery( g_DBConn, szConversionQuery[i] );

			if ( !SQL_Execute( query ) )
			{
				MYSQLX_Error( query, szQuery, 15 );

				return;
			}
		}

		WC3_Log( true, "[MYSQLX] MySQL Conversion to 3.0 DB Format ran successfully" );
	}
}