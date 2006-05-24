/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Common DB Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

public DB_DetermineType()
{
	// If the DBI Module isn't loaded then we can't use SQL for saving XP
	if ( !g_DBILoaded && get_pcvar_num( CVAR_wc3_save_xp_sql ) )
	{

		log_amx( "Unable to saving using DBI, please enable a DBI module (SQLite or MySQL X) to save XP using DBI" );

		set_pcvar_num( CVAR_wc3_save_xp_sql, 0 );
	}

	
	// Then we are saving using SQL, lets determine if it's sqlite or mysql
	if ( get_pcvar_num( CVAR_wc3_save_xp_sql ) == 1 )
	{
		// Get the DB Type
		new szDBIType[16];
		dbi_type( szDBIType, 15 );

		// We're using MySQL X
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
	
	// Initiate our database
	switch( g_DBType )
	{
		case DB_MYSQL:	MYSQLX_Init();
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
	
	// Save the XP	
	switch( g_DBType )
	{
		case DB_MYSQL:	MYSQLX_Save( id );
		case DB_SQLITE:	SQLITE_Save( id );
		case DB_VAULT:	NVAULT_Save( id );
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

// Function will get the user's XP for each race
public DB_GetAllXP( id, iRaceXP[MAX_RACES] )
{
	// If we're not saving XP, why do this?
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) || !id )
	{
		return;
	}
	
	// Get the XP	
	switch( g_DBType )
	{
		case DB_MYSQL:	MYSQLX_GetAllXP( id );
		case DB_SQLITE:	SQLITE_GetAllXP( id );
		case DB_VAULT:	NVAULT_GetAllXP( id );
	}

	// Copy this over
	for ( new i = 0; i < MAX_RACES; i++ )
	{
		iRaceXP[i] = g_iRaceXP[id][i];
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

	// Set the user's race information
	switch( g_DBType )
	{
		case DB_MYSQL:	MYSQLX_SetData( id );
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
