/***********************************************************************
		XP FUNCTIONS (idea from war3x)
***********************************************************************/

stock XP_give(id, iXP)
{

	if ( !warcraft3 || id == 0 )
	{
		return 0;
	}

	// Make sure we have the minimum amount of players
	if( get_playersnum() < get_pcvar_num( CVAR_wc3_min_players ) )
	{
		return 0;
	}
	
	p_data[id][P_XP] += iXP;

	WAR3_Display_Level(id, DISPLAYLEVEL_SHOWGAINED);

	return iXP;
}

stock XP_onDeath(victim_id, killer_id, weapon, headshot)
{

	if ( !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}

	// Assuming we have a victim, a killer, and the victim is not the killer
	if ( killer_id && killer_id != victim_id && victim_id )
	{
		new iXP, iXPAwarded;

		// Attacker killed a teammate
		if ( get_user_team(killer_id) == get_user_team(victim_id) )
		{
			iXP = -1 * xpgiven[p_data[killer_id][P_LEVEL]];
			iXPAwarded = XP_give(killer_id, iXP);
			if (get_pcvar_num( CVAR_wc3_show_kill_obj ))
			{
				client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_TEAMMATE", iXPAwarded);
			}
		}
		else{
			if ( !get_pcvar_num( CVAR_wc3_xp_weap_multiplier ))
			{
				iXP = xpgiven[p_data[victim_id][P_LEVEL]];
				iXPAwarded = XP_give(killer_id, iXP);
				if (get_pcvar_num( CVAR_wc3_show_kill_obj ))
				{			
					new szVictimName[32];
					get_user_name(victim_id, szVictimName, 31);

					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING", iXPAwarded, szVictimName);
				}
			}
			else
			{
				iXP = floatround(xpgiven[p_data[victim_id][P_LEVEL]] * weaponxpmultiplier[weapon]);
				iXPAwarded = XP_give(killer_id, iXP);
				if (get_pcvar_num( CVAR_wc3_show_kill_obj ))
				{
					new szVictimName[32];
					get_user_name(victim_id, szVictimName, 31);

					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING", iXPAwarded, szVictimName);
				}
			}
			// Award XP for getting a headshot
			if( headshot )
			{
				iXP = get_pcvar_num(CVAR_wc3_headshot);
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_HEADSHOT", iXPAwarded);		
				}
			}
		#if MOD == 0
			// Award XP for killing the hostage saver
			if ( victim_id == g_hostageSaver )
			{
				iXP = get_pcvar_num( CVAR_wc3_kill_rescuer );
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
				{	
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_HOSTAGE_R", iXPAwarded);
				}
			}
			// Award XP for killing the bomb defuser
			else if ( victim_id == g_bombDefuser )
			{						
				iXP = get_pcvar_num( CVAR_wc3_kill_defuser );
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
				{				
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_BOMB_D", iXPAwarded);
				}	
			}
			// Award XP for killing the bomb carrier
			else if ( victim_id == g_bombCarrier )
			{						
				iXP = get_pcvar_num( CVAR_wc3_kill_bomb_carrier );
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
				{
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_BOMB_C", iXPAwarded);
				}
			}
			// Award XP for killing the VIP
			else if ( victim_id==g_vipID )
			{
				iXP = get_pcvar_num( CVAR_wc3_kill_vip );
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
				{
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARD_FOR_KILLING_VIP", iXPAwarded);
				}
			}
		#endif
		}
	}

	return PLUGIN_CONTINUE
}

stock XP_Set()
{

	// If we're saving XP, we want to set the max. amount of XP higher and the amount gained per kill/objective lower
	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		xpgiven = {6,8,10,12,14,16,18,20,24,28,32};
		xplevel = {0,100,200,400,800,1600,3200,6400,12800,25600,51200};
	}

	// Set the XP multiplier
	for( new i=0; i<11; i++ )
	{
		xplevel[i] = floatround(xplevel[i] * get_pcvar_float( CVAR_wc3_xp_multiplier ));
	}

	XP_Set_Multiplier();
}

stock XP_Set_Multiplier()
{

	if ( get_pcvar_num( CVAR_wc3_xp_weap_multiplier ) )
	{
		return PLUGIN_CONTINUE;
	}

	weaponxpmultiplier[CSW_WORLDSPAWN]		= 0.0;

#if MOD == 0
	weaponxpmultiplier[CSW_USP]				= 2.5;
	weaponxpmultiplier[CSW_DEAGLE]			= 2.5;
	weaponxpmultiplier[CSW_GLOCK18]			= 2.5;
	weaponxpmultiplier[CSW_ELITE]			= 3.5;
	weaponxpmultiplier[CSW_P228]			= 2.5;
	weaponxpmultiplier[CSW_FIVESEVEN]		= 2.5;

	weaponxpmultiplier[CSW_XM1014]			= 1.25;
	weaponxpmultiplier[CSW_M3]				= 1.5;

	weaponxpmultiplier[CSW_MP5NAVY]			= 1.0;
	weaponxpmultiplier[CSW_UMP45]			= 1.25;
	weaponxpmultiplier[CSW_P90]				= 1.25;
	weaponxpmultiplier[CSW_TMP]				= 1.5;
	weaponxpmultiplier[CSW_MAC10]			= 1.5;
	weaponxpmultiplier[CSW_GALIL]			= 1.15;
	weaponxpmultiplier[CSW_FAMAS]			= 1.15;

	weaponxpmultiplier[CSW_AWP]				= 1.0;
	weaponxpmultiplier[CSW_M4A1]			= 1.0;
	weaponxpmultiplier[CSW_AK47]			= 1.0;
	weaponxpmultiplier[CSW_AUG]				= 1.0;
	weaponxpmultiplier[CSW_SG552]			= 1.0;
	weaponxpmultiplier[CSW_G3SG1]			= 1.0;
	weaponxpmultiplier[CSW_SG550]			= 1.0;
	weaponxpmultiplier[CSW_M249]			= 1.25;
	weaponxpmultiplier[CSW_SCOUT]			= 3.0;

	weaponxpmultiplier[CSW_HEGRENADE]		= 1.75;
	weaponxpmultiplier[CSW_KNIFE]			= 6.0;

	weaponxpmultiplier[CSW_C4]				= 2.0;
	weaponxpmultiplier[CSW_SMOKEGRENADE]	= 1.0;
	weaponxpmultiplier[CSW_FLASHBANG]		= 1.0;
#endif
#if MOD == 1
	weaponxpmultiplier[DODW_AMERKNIFE		] = 6.0;
	weaponxpmultiplier[DODW_GERKNIFE		] = 6.0;
	weaponxpmultiplier[DODW_BRITKNIFE		] = 6.0;
	weaponxpmultiplier[DODW_GARAND_BUTT		] = 6.0;

	weaponxpmultiplier[DODW_HANDGRENADE		] = 1.75;
	weaponxpmultiplier[DODW_STICKGRENADE	] = 1.75;
	weaponxpmultiplier[DODW_STICKGRENADE_EX	] = 1.75;
	weaponxpmultiplier[DODW_HANDGRENADE_EX	] = 1.75;

	weaponxpmultiplier[DODW_COLT			] = 1.0;
	weaponxpmultiplier[DODW_LUGER			] = 1.0;
	weaponxpmultiplier[DODW_GARAND			] = 1.0;
	weaponxpmultiplier[DODW_SCOPED_KAR		] = 1.0;
	weaponxpmultiplier[DODW_THOMPSON		] = 1.0;
	weaponxpmultiplier[DODW_STG44			] = 1.0;
	weaponxpmultiplier[DODW_SPRINGFIELD		] = 1.0;
	weaponxpmultiplier[DODW_KAR				] = 1.0;
	weaponxpmultiplier[DODW_BAR				] = 1.0;
	weaponxpmultiplier[DODW_MP40			] = 1.0;
	weaponxpmultiplier[DODW_MG42			] = 1.0;
	weaponxpmultiplier[DODW_30_CAL			] = 1.0;
	weaponxpmultiplier[DODW_SPADE			] = 1.0;
	weaponxpmultiplier[DODW_M1_CARBINE		] = 1.0;
	weaponxpmultiplier[DODW_MG34			] = 1.0;
	weaponxpmultiplier[DODW_GREASEGUN		] = 1.0;
	weaponxpmultiplier[DODW_FG42			] = 1.0;
	weaponxpmultiplier[DODW_K43				] = 1.0;
	weaponxpmultiplier[DODW_ENFIELD			] = 1.0;
	weaponxpmultiplier[DODW_STEN			] = 1.0;
	weaponxpmultiplier[DODW_BREN			] = 1.0;
	weaponxpmultiplier[DODW_WEBLEY			] = 1.0;
	weaponxpmultiplier[DODW_BAZOOKA			] = 1.0;
	weaponxpmultiplier[DODW_PANZERSCHRECK	] = 1.0;
	weaponxpmultiplier[DODW_PIAT			] = 1.0;
	weaponxpmultiplier[DODW_SCOPED_FG42		] = 1.0;
	weaponxpmultiplier[DODW_FOLDING_CARBINE	] = 1.0;
	weaponxpmultiplier[DODW_KAR_BAYONET		] = 1.0;
	weaponxpmultiplier[DODW_SCOPED_ENFIELD	] = 1.0;
	weaponxpmultiplier[DODW_MILLS_BOMB		] = 1.0;
	weaponxpmultiplier[DODW_ENFIELD_BAYONET	] = 1.0;
	weaponxpmultiplier[DODW_MORTAR			] = 1.0;
	weaponxpmultiplier[DODW_K43_BUTT		] = 1.0;

#endif

	weaponxpmultiplier[CSW_LIGHTNING]		= 1.0;
	weaponxpmultiplier[CSW_SUICIDE]			= 1.0;
	weaponxpmultiplier[CSW_FLAME]			= 1.0;
	weaponxpmultiplier[CSW_LOCUSTS]			= 1.1;
	weaponxpmultiplier[CSW_SERPENTWARD]		= 1.0;
	weaponxpmultiplier[CSW_SHADOW]			= 1.0;	
	weaponxpmultiplier[CSW_THORNS]			= 1.0;
	weaponxpmultiplier[CSW_CARAPACE]		= 1.0;
	weaponxpmultiplier[CSW_CARRION]			= 1.0;
	weaponxpmultiplier[CSW_ORB]				= 1.0;
	weaponxpmultiplier[CSW_CONCOCTION]		= 1.0;

	return PLUGIN_CONTINUE;
}


// Function will get data about a player's current race, and set it to whats in the DB/vault
public XP_Set_Race_Data( id )
{

	if ( !warcraft3 )
		return PLUGIN_CONTINUE;

	if ( !get_pcvar_num( CVAR_wc3_save_xp ) || !id )
		return PLUGIN_CONTINUE;

	// If we're saving by STEAM_ID, lets make sure the user has a steam ID
	new szPlayerID[33];
	get_user_authid(id, szPlayerID, 31);
	if( get_pcvar_float( CVAR_wc3_save_by) == SAVE_STEAMID && equal(szPlayerID, "STEAM_ID_PENDING") )
	{
		client_print(id, print_chat, "%s Unable to retreive race information, you have no STEAM ID, please rejoin the server.", g_MODclient);
		return PLUGIN_CONTINUE;
	}
	
	new szPlayerIP[20], szPlayerName[66];
	get_user_name(id, szPlayerName, 32);
	get_user_ip(id, szPlayerIP, 19);

	if ( get_pcvar_num( CVAR_wc3_save_xp_sql ) )
	{
		// Verify we have a database connection
		if ( !XP_Check_Connection() )
		{
			return PLUGIN_CONTINUE;
		}

		// Prepare name for the query
		DB_FormatString( szPlayerName, 65 );

		new szQuery[256];
		format( szQuery, 255, "SELECT `xp`, `skill1`, `skill2`, `skill3`, `skill4` FROM `%s` WHERE (`playerid` = '%s' AND `race` = %d)", g_DBTableName, (get_pcvar_float( CVAR_wc3_save_by)==SAVE_NAME) ? szPlayerName : ((get_pcvar_float( CVAR_wc3_save_by)==SAVE_IP) ? szPlayerIP : szPlayerID), p_data[id][P_RACE])

		new Result:res = dbi_query(sql, szQuery)
		
		// Verify we have a result
		if (res < RESULT_NONE)
		{
			client_print(id, print_chat, "%s An error has occurred when retreiving your race information, please contact a server administrator", g_MODclient);
			XP_DBI_Error( res, szQuery, 1 );
			return PLUGIN_CONTINUE;
		}
		
		// Then we have data in the database
		if ( res && dbi_nextrow(res)>0 )
		{
			new szXP[8], szSkill1[2], szSkill2[2], szSkill3[2], szSkill4[2];
			dbi_result(res, "xp", szXP, 7);
			dbi_result(res, "skill1", szSkill1, 1);
			dbi_result(res, "skill2", szSkill2, 1);
			dbi_result(res, "skill3", szSkill3, 1);
			dbi_result(res, "skill4", szSkill4, 1);			

			p_data[id][P_XP]		= str_to_num(szXP);
			p_data[id][P_SKILL1]	= str_to_num(szSkill1);
			p_data[id][P_SKILL2]	= str_to_num(szSkill2);
			p_data[id][P_SKILL3]	= str_to_num(szSkill3);
			p_data[id][P_ULTIMATE]	= str_to_num(szSkill4);
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
		
		dbi_free_result(res);

	}
	// Retreive data from the vault
	else
	{
		new szKey[128], szData[256], iAttempt;

		format( szKey, 127, "%s_%d", (get_pcvar_float( CVAR_wc3_save_by)==SAVE_NAME) ? szPlayerName : ((get_pcvar_float( CVAR_wc3_save_by)==SAVE_IP) ? szPlayerIP : szPlayerID), p_data[id][P_RACE] );

		iAttempt = get_vaultdata(szKey, szData, 255);
		
		// Then we found a record in the vault
		if ( iAttempt )
		{
			new szXP[8], szSkill1[2], szSkill2[2], szSkill3[2], szSkill4[2], szRace[2];

			// Parse the vault entry
			parse(szData, szPlayerID, 31, szXP, 7, szRace, 1, szSkill1, 1, szSkill2, 1, szSkill3, 1, szSkill4, 1);

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
	
	// Display the race information to the user
	WAR3_Display_Level(id, DISPLAYLEVEL_SHOWRACE);

	return PLUGIN_CONTINUE;
}


public XP_Set_DBI(){
	

	return PLUGIN_CONTINUE;
}

// Reset the user's XP to 0
public XP_Reset(id)
{

	if (!warcraft3)
		return PLUGIN_CONTINUE;

	p_data[id][P_LEVEL]		= 0;
	p_data[id][P_XP]		= 0;
	p_data[id][P_SKILL1]	= 0;
	p_data[id][P_SKILL2]	= 0;
	p_data[id][P_SKILL3]	= 0;
	p_data[id][P_ULTIMATE]	= 0;

	DB_SaveXP(id);

	WAR3_Display_Level(id, DISPLAYLEVEL_NONE);

	client_print(id, print_chat, "%s %L", g_MODclient, id, "YOUR_XP_HAS_BEEN_RESET");

	return PLUGIN_CONTINUE;
}	

// This function will prune a single player (checks for the latest date and sets them all to the same)
public XP_Prune_Player(id)
{
	if ( get_pcvar_num( CVAR_wc3_save_xp ) && get_pcvar_num( CVAR_wc3_save_pruning ) )
	{
		new szPlayerID[32], szPlayerName[32], szPlayerIP[32];
		get_user_authid(id, szPlayerID, 31);
		get_user_name(id, szPlayerName, 31);
		get_user_ip(id, szPlayerIP, 31);

		// Update all records if we're using the vault
		if ( !get_pcvar_num( CVAR_wc3_save_xp_sql ) )
		{
			new iRace, iAttempt, szKey[128], szData[512];

			// Loop through and find the latest timestamp and re-save
			for( iRace = 1; iRace < 10; iRace++ )
			{
				format( szKey, 127, "%s_%d", (get_pcvar_float( CVAR_wc3_save_by)==SAVE_NAME) ? szPlayerName : ((get_pcvar_float( CVAR_wc3_save_by)==SAVE_IP) ? szPlayerIP : szPlayerID), iRace );

				iAttempt = get_vaultdata( szKey, szData, 511 );
				
				// Only want to check if a key was found
				if ( iAttempt )
				{
					new szXP[8], szRace[2], szSkill1[2], szSkill2[2], szSkill3[2], szSkill4[2], szIP[32];

					format( szXP, 7, "" );

					// Get the data that is in the vault
					parse( szData, szPlayerID, 31, szXP, 7, szRace, 1, szSkill1, 1, szSkill2, 1, szSkill3, 1, szSkill4, 1, szIP, 31);
					
					// Re-save the data with the current timestamp
					format( szData, 511, "%s %d %d %d %d %d %d %s %d %s", szPlayerID, szXP, szSkill1, szSkill2, szSkill3, szSkill4, szIP, get_systime(), szPlayerName );
					
					// Save the data
					set_vaultdata(szKey, szData);
				}
			}
		}
		// Saving with SQL, lets do some sweet ass query thing that updates all of them to the current timestamp, we don't want 8 queries do we?
		else
		{
			new szQuery[512];
			format(szQuery, 511, "UPDATE `%s` SET `time` = NOW() WHERE `playerid` = '%s';", g_DBTableName, (get_pcvar_float( CVAR_wc3_save_by)==2) ? szPlayerName : ((get_pcvar_float( CVAR_wc3_save_by)==1) ? szPlayerIP : szPlayerID));

			new Result:res = dbi_query(sql, szQuery);
			
			// Verify we have a result
			if (res < RESULT_NONE)
			{
				XP_DBI_Error( res, szQuery, 3 );
				return;
			} 
		}
	}

	return;
}

// Prune the database of old records
public XP_Prune()
{

	if ( get_pcvar_num( CVAR_wc3_save_pruning ) )
	{
		// Vault pruning (only works with vaults created with version 2.2.8)
		if ( !get_pcvar_num( CVAR_wc3_save_xp_sql ) )
		{
			new szVault[] = "addons/amxmodx/data/vault.ini";
			
			// Make sure the vault exists
			if ( file_exists( szVault ) )
			{
				new len, line, szText[256];
				new iCurrentTime = get_systime();
				// 86400 = 24 hours * 60 minutes * 60 seconds
				new iExpiredTime = iCurrentTime - (get_pcvar_num( CVAR_wc3_days_before_delete ) * 86400);

				// Check every line in the vault
				while ( (line = read_file(szVault, line, szText, 255, len)) != 0 )
				{
					// The first 2 lines are not actual data, so lets skip them
					if ( line > 2 )
					{
						new szID[32], szAuthID[32], szXP[8], szRace[2], szSkill1[2], szSkill2[2], szSkill3[2], szSkill4[2], szIP[32], szTimestamp[32];
						parse(szText, szID, 31, szAuthID, 31, szXP, 7, szRace, 1, szSkill1, 1, szSkill2, 1, szSkill3, 1, szSkill4, 1, szIP, 31, szTimestamp, 31);
						
						// Verify its the new vault timestamp
						if ( strlen(szTimestamp) > 2 )
						{
							new iUserTimestamp = str_to_num( szTimestamp );
							
							server_print( "[VAULT] Checking  %d < %d", iUserTimestamp, iExpiredTime );
							if ( iUserTimestamp < iExpiredTime )
							{
								server_print( "[VAULT] Expired removing %s", szID );
							}
						}
					}
				} 
			}
		}
		// MySQL/SQLLite pruning
		else
		{
			new query[256];

			if ( g_DBType == DB_MYSQL )
			{
				// Timestamp format: 20030912122142
				// Y = 2003 M = 09 D = 12 H = 12 M = 21 S = 42	
				format( query, 255, "DELETE FROM `%s` WHERE DATE_SUB(CURDATE(),INTERVAL %d DAY) > time;", g_DBTableName, get_pcvar_num( CVAR_wc3_days_before_delete ) );
			}
			else if ( g_DBType == DB_SQLITE )
			{
				// Timestamp format: 2003-09-12 12:21:42
				// Y = 2003 M = 09 D = 12 H = 12 M = 21 S = 42
				format( query, 255, "DELETE FROM `%s` WHERE ((julianday(`time`) + %d) < julianday('now'))", g_DBTableName, get_pcvar_num( CVAR_wc3_days_before_delete ) );
			}

			dbi_query(sql, query);

			// Vacuum the SQL LITE DB Table
			if ( g_DBType == DB_SQLITE )
			{
				format( query, 255, "VACUUM `%s`", g_DBTableName );

				dbi_query( sql, query );
			}
		}

		log_amx("Database pruning successful, data older than %d days was removed", get_pcvar_num( CVAR_wc3_days_before_delete ));
	}
}

// Close the database connection
public XP_CloseDB()
{

	if ( get_pcvar_num( CVAR_wc3_save_xp_sql ) )
	{
		if( sql )
		{
			dbi_close( sql );
			log_amx( "%s database connection closed", g_szDBType );
		}
	}
}

// This function will save the XP for all players, but it will save the data every 0.1 seconds (reduce lag?)
public XP_Save_All()
{

	if (!warcraft3)
		return PLUGIN_CONTINUE;
	
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
		return PLUGIN_CONTINUE;

	new players[32], numofplayers, parm[1];
	get_players( players, numofplayers );
	new Float:time = 0.0;

	for ( new i=0; i < numofplayers; i++ )
	{
		parm[0] = players[i];

		set_task( time, "XP_Save_Helper", TASK_SAVE_ALL + players[i], parm[0], 1 );

		time += 0.1;
	}

	return PLUGIN_CONTINUE;
}

// Helper function to call save for XP_Save_All
public XP_Save_Helper( parm[1] )
{

	DB_SaveXP( parm[0] );
}

// Verifies that the database connection is ok
public XP_Check_Connection()
{

	if( sql < SQL_OK )
	{
		if( iSQLAttempts < SQL_ATTEMPTS )
		{
			log_amx( "Database connection failed, attempting to reconnect in %0.0f seconds", SQL_ATTEMPT_DELAY );
			set_task( SQL_ATTEMPT_DELAY, "XP_Set_DBI", TASK_SETSQL );
		}
		else if( iSQLAttempts >= SQL_ATTEMPTS )
		{
			set_pcvar_num( CVAR_wc3_save_xp, 0 )
			set_pcvar_num( CVAR_wc3_save_xp_sql, 0 )

			log_amx("Unable to connect to the %s database after %d attempts, switching to short-term mode", g_szDBType, iSQLAttempts)
		}

		return false;
	}

	return true;
}

// Function from war3x thanks ryan!!!
public XP_get_admin_flag()
{

    new szFlags[24];
    get_pcvar_string( CVAR_wc3_admin_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}
