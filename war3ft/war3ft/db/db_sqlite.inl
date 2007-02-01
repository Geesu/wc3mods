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
	new szError[256];

	// Attempt the Connection
	g_DB = dbi_connect( "", "", "", "war3ft", szError, 255 );

	// Verify our database connection has worked
	if ( !SQLITE_Check_Connection() )
	{
		log_amx( "Error: %s", szError );

		return;
	}

	server_print( "[WAR3FT] SQLite database connection successful" );

	// Create all tables if we need to
	for ( new i = 0; i < TOTAL_SQLITE_TABLES; i++ )
	{
		// Then create it!
		if ( !sqlite_table_exists( g_DB, szTableNames[i] ) )
		{
			new Result:ret = dbi_query( g_DB, szTablesSQLite[i] );

			if ( ret < RESULT_NONE )
			{
				SQLITE_Error( ret, szTablesSQLite[i], 1 );

				return;
			}
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

	new szIntegrityCheck[64], szQuery[128];
	format(szQuery, 127, "PRAGMA integrity_check");
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
	format( szQuery, 127, "PRAGMA synchronous = %d", SQLITE_SYNC_OFF );
	dbi_query( g_DB, szQuery );
}

// Close the SQLite connection
SQLITE_Close()
{
	if ( g_DB )
	{
		dbi_close( g_DB );
	}
}

SQLITE_FetchUniqueID( id )
{
	// Verify we have a database connection
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
	new Result:res = dbi_query( g_DB, szQuery );
	
	// Verify we have a result
	if ( res < RESULT_NONE )
	{
		SQLITE_Error( res, szQuery, 3 );

		return;
	}

	// If no rows we need to insert!
	if ( res == RESULT_NONE )
	{
		// Free the last handle!
		dbi_free_result( res );

		// Insert this player!
		format( szQuery, 511, "INSERT INTO `wc3_player` ( `player_id` , `%s` , `time` ) VALUES ( NULL , '%s', julianday('now') );", szKeyName, szKey );
		res = dbi_query( g_DB, szQuery );

		// Verify we have a result
		if ( res < RESULT_NONE )
		{
			SQLITE_Error( res, szQuery, 4 );

			return;
		}

		// Free the last handle!
		dbi_free_result( res );
		
		// Now we need to get the insert ID
		format( szQuery, 511, "SELECT `player_id` FROM `wc3_player` WHERE `%s` = '%s';", szKeyName, szKey );
		res = dbi_query( g_DB, szQuery );

		// Verify we have a result
		if ( res < RESULT_NONE )
		{
			SQLITE_Error( res, szQuery, 5 );

			return;
		}
	}

	new szID[12];
	dbi_result( res, "player_id", szID, 11 );

	g_iDBPlayerUniqueID[id] = str_to_num( szID );

	// Free the last handle!
	dbi_free_result( res );
}

SQLITE_Save( id )
{
	// Verify we have a database connection
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new iUniqueID = DB_GetUniqueID( id );

	// Save the user's XP!
	new szQuery[512];
	format( szQuery, 511, "REPLACE INTO `wc3_player_race` ( `player_id` , `race_id` , `race_xp` ) VALUES ( '%d', '%d', '%d');", iUniqueID, p_data[id][P_RACE], p_data[id][P_XP] );
	new Result:res = dbi_query( g_DB, szQuery );

	// Verify we have a result
	if ( res < RESULT_NONE )
	{
		SQLITE_Error( res, szQuery, 6 );

		return;
	}

	static iCurrentLevel;

	// Now we need to save the skill levels!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		iCurrentLevel = SM_GetSkillLevel( id, iSkillID );

		// Then we need to save this!
		if ( iCurrentLevel > 0 && g_iDBPlayerSkillStore[id][iSkillID] != iCurrentLevel )
		{
			format( szQuery, 511, "REPLACE INTO `wc3_player_skill` ( `player_id` , `skill_id` , `skill_level` ) VALUES ( '%d', '%d', '%d' );", iUniqueID, iSkillID, iCurrentLevel );
			res = dbi_query( g_DB, szQuery );

			// Verify we have a result
			if ( res < RESULT_NONE )
			{
				SQLITE_Error( res, szQuery, 7 );

				return;
			}
		}
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

	new szQuery[256];
	format(szQuery, 255, "SELECT `race_id`, `race_xp` FROM `wc3_player_race` WHERE ( `player_id` = '%d' );", DB_GetUniqueID( id ) );
	
	new Result:res = dbi_query( g_DB, szQuery );

	// Verify we have a result
	if ( res < RESULT_NONE )
	{
		SQLITE_Error( res, szQuery, 8 );

		return;
	}

	// Set last saved XP to 0
	for ( new i = 0; i < MAX_RACES; i++ )
	{
		g_iDBPlayerXPInfoStore[id][i] = 0;
	}

	new iXP, iRace, szXP[8], szRace[2];

	// Loop through all of the records to find the XP data
	while ( res && dbi_nextrow( res ) > 0 )
	{
		dbi_result( res, "race_xp", szXP, 7 );
		dbi_result( res, "race_id", szRace, 1 );

		iXP		= str_to_num( szXP		);
		iRace	= str_to_num( szRace	);
		
		// Save the user's XP in an array
		if ( iRace > 0 && iRace < MAX_RACES + 1 )
		{
			g_iDBPlayerXPInfoStore[id][iRace-1] = iXP;
		}
	}

	// Free the handle
	dbi_free_result( res );

	// Call the function that will display the "select a race" menu
	WC3_ChangeRaceShowMenu( id, g_iDBPlayerXPInfoStore[id] );

	return;
}

SQLITE_SetData( id )
{
	// Verify we have a database connection
	if ( !SQLITE_Check_Connection() )
	{
		return;
	}

	new szQuery[256];
	format( szQuery, 255, "SELECT `skill_id`, `skill_level` FROM `wc3_player_skill` WHERE `player_id` = '%d';", DB_GetUniqueID( id ) );
	new Result:res = dbi_query( g_DB, szQuery );
	
	// Verify we have a result
	if ( res < RESULT_NONE )
	{
		SQLITE_Error( res, szQuery, 9 );

		return;
	}

	// Set the user's XP!
	p_data[id][P_XP] = g_iDBPlayerXPInfoStore[id][p_data[id][P_RACE]-1];

	// Reset all skill data to 0!
	for ( new iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
	{
		SM_SetSkillLevel( id, iSkillID, 0 );
	}

	new szSkillID[4], szSkillLevel[4];
	// While we have a result!
	while ( res && dbi_nextrow( res ) > 0 )
	{
		dbi_result( res, "skill_id"		, szSkillID		, 3 );
		dbi_result( res, "skill_level"	, szSkillLevel	, 3 );

		SM_SetSkillLevel( id, str_to_num( szSkillID ), str_to_num( szSkillLevel ) );
	}

	// Free the handle
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

#define SQLITE_TOTAL_PRUNE_QUERY 3
new const szPruneQuery[SQLITE_TOTAL_PRUNE_QUERY][] = 
{
	"DELETE FROM wc3_player_race  WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( (julianday(`time`) + %d) < julianday('now') ) );",
	"DELETE FROM wc3_player_skill WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( (julianday(`time`) + %d) < julianday('now') ) );",
	"DELETE FROM wc3_player WHERE player_id IN ( SELECT `player_id` FROM `wc3_player` WHERE ( (julianday(`time`) + %d) < julianday('now') ) );"
};

SQLITE_Prune()
{
	new szQuery[256];

	// Need to run all 3 queries
	for ( new i = 0; i < SQLITE_TOTAL_PRUNE_QUERY; i++ )
	{
		formatex( szQuery, 255, szPruneQuery[i], get_pcvar_num( CVAR_wc3_days_before_delete ) );
		server_print( szQuery );

		new Result:res = dbi_query( g_DB, szQuery );

		if ( res < RESULT_NONE )
		{
			SQLITE_Error( res, szQuery, 10 );

			return;
		}
	}
}

SQLITE_UpdateTimestamp( id )
{
	new szQuery[256];
	format( szQuery, 255, "UPDATE `wc3_player` SET time = julianday('now') WHERE ( `player_id` = '%d' )", g_iDBPlayerUniqueID[id] );

	new Result:ret = dbi_query( g_DB, szQuery );

	if ( ret < RESULT_NONE )
	{
		SQLITE_Error( ret, szQuery, 11 );

		return;
	}
}