/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Common DB Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

public DB_DetermineType()
{

	new iDatabaseType = get_pcvar_num( CVAR_wc3_save_xp_db );

	// See if we can save with SQLite
	if ( iDatabaseType == DB_SQLITE )
	{
		
		// Then we can't save w/this!!!
		if ( !LibraryExists( "dbi", LibType_Class )  )
		{
			log_amx( "[WARNING] Unable to saving using SQLite, please enable the sqlite module" );
			log_amx( "[WARNING] Saving will be done using NVault" );

			set_pcvar_num( CVAR_wc3_save_xp_db, 0 );
		}
		
		// OK we can save
		else
		{
			// Get the DB Type
			new szDBIType[16];
			dbi_type( szDBIType, 15 );

			// We're using SQLite
			if ( equali( szDBIType, "SQLite" ) )
			{
				g_DBType = DB_SQLITE;
				copy( g_szDBType, 15, "SQLite" );
			}

			// Using an unsupported DB Type - lets default to vault
			else
			{
				log_amx( "[WARNING] Unsupported database type loaded: %s, please enable the sqlite module", szDBIType );
				log_amx( "[WARNING] Saving will be done using NVault" );

				set_pcvar_num( CVAR_wc3_save_xp_db, 0 );
			}
		}
	}

	// See if we can save with MySQL
	if ( iDatabaseType == DB_MYSQLX )
	{
	
		// Then we can't save using mysql
		if ( !LibraryExists( "sqlx", LibType_Class ) )
		{
			log_amx( "[WARNING] Unable to saving using MySQL, please enable the mysql module" );
			log_amx( "[WARNING] Saving will be done using NVault" );

			set_pcvar_num( CVAR_wc3_save_xp_db, 0 );
		}

		// OK we can save!
		else
		{
			g_DBType = DB_MYSQLX;
			copy( g_szDBType, 15, "MySQLX" );
		}
	}
	
	// Then we don't have a DB Type yet set up for saving! Use vault?
	if ( g_DBType == -1 )
	{

		// We can use the nvault if it's loaded
		if ( LibraryExists( "nvault" , LibType_Library ) )
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

	// Initiate our database
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_Init();
		case DB_SQLITE:	SQLITE_Init();
		case DB_VAULT:	NVAULT_Init();
	}

	return;
}

// Close the database connection(s)
public DB_Close()
{
	
	// Then we shouldn't have a connection to close
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}

	// Close our connections
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_Close();
		case DB_SQLITE:	SQLITE_Close();
		case DB_VAULT:	NVAULT_Close();
	}

	return;
}

// Save the user's XP
public DB_SaveXP( id )
{
	
	if ( id >= TASK_SAVE )
	{
		id -= TASK_SAVE;
	}

	if ( !WC3_Check() )
	{
		return;
	}

	new iRaceID = p_data[id][P_RACE];

	if ( iRaceID == 0 || !get_pcvar_num( CVAR_wc3_save_xp ) || p_data[id][P_XP] == 0 )
	{
		return;
	}

	// Allow bot's XP to be saved if we're saving by name
	if ( is_user_bot( id ) && get_pcvar_num( CVAR_wc3_save_by ) != DB_SAVEBY_NAME )
	{
		return;
	}

	// We don't want to save their XP if they cheated and got level 10!!!
	if ( g_bGivenLevel10[id][iRaceID] )
	{
		return;
	}
	
	// Save the XP	
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_Save( id );
		case DB_SQLITE:	SQLITE_Save( id );
		case DB_VAULT:	NVAULT_Save( id );
	}

	return;
}

// This function will save the XP for all players
public DB_SaveAll()
{

	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}

	new players[32], numofplayers, i;
	get_players( players, numofplayers );

	for ( i = 0; i < numofplayers; i++ )
	{
		DB_SaveXP( players[i] );
	}

	return;
}

// Function will return the appropriate key for a user
DB_GetKey( id, szKey[], len )
{

	switch( get_pcvar_num( CVAR_wc3_save_by ) )
	{
		case DB_SAVEBY_NAME:
		{
			get_user_name( id, szKey, len );

			DB_FormatString( szKey, len );
		}

		case DB_SAVEBY_IP:		get_user_ip( id, szKey, len );
		case DB_SAVEBY_STEAMID:	get_user_authid( id, szKey, len );
	}
}

// Function will return the keyname
DB_GetKeyName( szKeyName[], len )
{
	switch( get_pcvar_num( CVAR_wc3_save_by ) )
	{
		case DB_SAVEBY_NAME:	copy( szKeyName, len, "player_name" );
		case DB_SAVEBY_IP:		copy( szKeyName, len, "player_ip" );
		case DB_SAVEBY_STEAMID:	copy( szKeyName, len, "player_steamid" );
	}
}

// Function will get the user's XP for each race
public DB_GetAllXP( id )
{
	// If we're not saving XP, why do this?
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) || !id )
	{
		return;
	}
	
	// Get the XP	
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_GetAllXP( id );
		case DB_SQLITE:	SQLITE_GetAllXP( id );
		case DB_VAULT:	NVAULT_GetAllXP( id );
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

	// Set the user's race information
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_SetData( id );
		case DB_SQLITE:	SQLITE_SetData( id );
		case DB_VAULT:	NVAULT_SetData( id );
	}

	return;
}

// Prepares text for database insertion
public DB_FormatString( text[], len )
{
	replace_all( text, len, "'", "\'" );
}

DB_UpdateTimestamp( id )
{

	// If we're not saving XP, why do this?
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) || !id )
	{
		return;
	}

	// Update the user's timestamp for each race
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_UpdateTimestamp( id );
		case DB_SQLITE:	SQLITE_UpdateTimestamp( id );
		case DB_VAULT:	NVAULT_UpdateTimestamp( id );
	}

	return;
}

DB_Prune()
{
	
	// If we're not saving or pruning is disabled, we don't want to be here
	if ( !get_pcvar_num( CVAR_wc3_save_pruning ) || !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}

	// Prune the DB
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_Prune();
		case DB_SQLITE:	SQLITE_Prune();
		case DB_VAULT:	NVAULT_Prune();
	}

	return;
}

DB_FetchUniqueID( id )
{
	// If we're not saving XP, why do this?
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) || !id )
	{
		return;
	}

	client_print( id, print_chat, "[DEBUG] DB_FetchUniqueID" );

	// Update the user's timestamp for each race
	switch( g_DBType )
	{
		case DB_MYSQLX:	MYSQLX_FetchUniqueID( id );
		//case DB_SQLITE:	SQLITE_UpdateTimestamp( id );
		//case DB_VAULT:	NVAULT_UpdateTimestamp( id );
	}

	return;
}

DB_GetUniqueID( id )
{
	// Then we need to determine this player's ID!
	if ( g_iDBPlayerUniqueID[id] <= 0 || get_pcvar_num( CVAR_wc3_save_by ) != g_iDBPlayerSavedBy[id] )
	{
		DB_FetchUniqueID( id );
	}

	return g_iDBPlayerUniqueID[id];
}