/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	NVault
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

// Initiate our vault "database"
NVAULT_Init()
{
	new szVaultName[32];

	// We need to open the vaults we have for each race
	for ( new i = 1; i <= MAX_RACES; i++ )
	{
		format( szVaultName, 31, "war3ft-%d", i );

		g_Vault[i] = nvault_open( szVaultName );

		if ( g_Vault[i] == -1 )
		{
			log_amx( "[NVAULT] Error when opening '%s', defaulting to short-term XP", szVaultName );

			set_pcvar_num( CVAR_wc3_save_xp, 0 );
			
			return;
		}

		// Set up default keys that should always exist
		else
		{
			nvault_pset( g_Vault[i], "war3ft_version", WC3VERSION );
		}
	}

	return;
}

// CLose the open vaults
NVAULT_Close()
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

NVAULT_Save( id )
{
	new szPlayerIP[20], szPlayerName[66], szPlayerID[32];
	new szKey[128], szData[256];
	new iRaceID = p_data[id][P_RACE];

	get_user_name(		id, szPlayerName	, 65 );
	get_user_ip(		id, szPlayerIP		, 19 );
	get_user_authid(	id, szPlayerID		, 31 );

	// Format the data for entry
	format( szData, 255, "%s %d %d %d %d %d %d %s %d %s", szPlayerID, p_data[id][P_XP], p_data[id][P_RACE], p_data[id][P_SKILL1], p_data[id][P_SKILL2], p_data[id][P_SKILL3], p_data[id][P_ULTIMATE], szPlayerIP, get_systime(), szPlayerName );
	
	// Format the vault key
	format( szKey, 127, "%s", ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_NAME ) ? szPlayerName : ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_IP ) ? szPlayerIP : szPlayerID ) ) );
	
	// Save the data
	nvault_set( g_Vault[iRaceID], szKey, szData );
}

NVAULT_GetAllXP( id )
{
	new iAttempt, szKey[66], szXP[8], szData[256], iTimestamp, szPlayerID[32], iRaceXP[MAX_RACES] = {0};

	DB_GetKey( id, szKey, 65 );

	for( new iRace = 1; iRace < 10; iRace++ )
	{
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

	// Call the function that will display the "select a race" menu
	WC3_ChangeRaceEnd( id, iRaceXP );

	return;
}

NVAULT_SetData( id )
{
	new szKey[66], szData[256], iAttempt, iTimestamp, szPlayerID[32];
	new iRace = p_data[id][P_RACE];

	DB_GetKey( id, szKey, 65 );

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

	// Set the race up
	WC3_SetRaceUp( id );

	return;
}

NVAULT_Prune()
{
	
	// This time is really "irrelevant", ideally it would be the time the FIRST entry was inserted into the vault, but this works too, since this date is before this was released
	new iStartTime	= 1149977691;
	new iEndTime	= get_systime() - ( get_pcvar_num( CVAR_wc3_days_before_delete ) * 86400 );
	
	// 86400 = 24 hours * 60 minutes * 60 seconds
	// so iEndTime = system time in seconds - ( days * hours * minutes * seconds )

	// Cycle through each vault and see if there is a record
	for ( new i = 1; i <= MAX_RACES; i++ )
	{
		// Make sure we have a valid handle
		if ( g_Vault[i] )
		{
			nvault_prune( g_Vault[i], iStartTime, iEndTime );
		}
	}
}

NVAULT_UpdateTimestamp( id )
{

	new szKey[128], szData[256], iTimestamp;
	new szPlayerIP[20], szPlayerName[66], szPlayerID[32];

	get_user_name(		id, szPlayerName	, 65 );
	get_user_ip(		id, szPlayerIP		, 19 );
	get_user_authid(	id, szPlayerID		, 31 );
	
	// Format the vault key
	format( szKey, 127, "%s", ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_NAME ) ? szPlayerName : ( ( get_pcvar_num( CVAR_wc3_save_by ) == DB_SAVEBY_IP ) ? szPlayerIP : szPlayerID ) ) );

	// Cycle through each vault and see if there is a record
	for ( new i = 1; i <= MAX_RACES; i++ )
	{
		// Make sure we have a valid handle
		if ( g_Vault[i] )
		{
			// Get the value
			if ( nvault_lookup( g_Vault[i], szKey, szData, 255, iTimestamp ) )
			{
				// Set it again - this will update the timestamp
				nvault_set( g_Vault[i], szKey, szData );
			}
		}
	}
}