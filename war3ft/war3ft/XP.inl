/***********************************************************************
		XP FUNCTIONS (idea from war3x)
***********************************************************************/

stock XP_give(id, iXP)
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_give", id);
	#endif

	if ( !warcraft3 || id == 0 )
	{
		return 0;
	}

	// Make sure we have the minimum amount of players
	if( get_playersnum() < get_pcvar_num( CVAR_XP_Min_Players ) )
	{
		return 0;
	}
	
	p_data[id][P_XP] += iXP;

	WAR3_Display_Level(id, DISPLAYLEVEL_SHOWGAINED);

	return iXP;
}

stock XP_onDeath(victim_id, killer_id, weapon, headshot)
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_onDeath", victim_id);
	#endif

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
			if (get_pcvar_num( XP_Show_Kill_Objectives ))
			{
				client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_TEAMMATE", iXPAwarded);
			}
		}
		else{
			if (!iCvar[MP_WEAPONXPMODIFIER])
			{
				iXP = xpgiven[p_data[victim_id][P_LEVEL]];
				iXPAwarded = XP_give(killer_id, iXP);
				if (get_pcvar_num( XP_Show_Kill_Objectives ))
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
				if (get_pcvar_num( XP_Show_Kill_Objectives ))
				{
					new szVictimName[32];
					get_user_name(victim_id, szVictimName, 31);

					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING", iXPAwarded, szVictimName);
				}
			}
			// Award XP for getting a headshot
			if( headshot )
			{
				iXP = get_pcvar_num(XP_Headshot);
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( XP_Show_Kill_Objectives ) )
				{
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_HEADSHOT", iXPAwarded);		
				}
			}
		#if MOD == 0
			// Award XP for killing the hostage saver
			if ( victim_id == g_hostageSaver )
			{
				iXP = get_pcvar_num( XP_Kill_Rescuer );
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( XP_Show_Objectives ) )
				{	
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_HOSTAGE_R", iXPAwarded);
				}
			}
			// Award XP for killing the bomb defuser
			else if ( victim_id == g_bombDefuser )
			{						
				iXP = get_pcvar_num( XP_Kill_Defuser );
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( XP_Show_Objectives ) )
				{				
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_BOMB_D", iXPAwarded);
				}	
			}
			// Award XP for killing the bomb carrier
			else if ( victim_id == g_bombCarrier )
			{						
				iXP = get_pcvar_num( XP_Kill_Bomb_Carrier );
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( XP_Show_Objectives ) )
				{
					client_print(killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_BOMB_C", iXPAwarded);
				}
			}
			// Award XP for killing the VIP
			else if ( victim_id==g_vipID )
			{
				iXP = get_pcvar_num( XP_Kill_VIP );
				iXPAwarded = XP_give(killer_id, iXP);
				if ( get_pcvar_num( XP_Show_Objectives ) )
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
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Set", 0);
	#endif

	// If we're saving XP, we want to set the max. amount of XP higher and the amount gained per kill/objective lower
	if ( iCvar[MP_SAVEXP] )
	{
		xpgiven = {6,8,10,12,14,16,18,20,24,28,32};
		xplevel = {0,100,200,400,800,1600,3200,6400,12800,25600,51200};
	}

	// Set the XP multiplier
	for( new i=0; i<11; i++ )
	{
		xplevel[i] = floatround(xplevel[i] * fCvar[MP_XPMULTIPLIER]);
	}

	XP_Set_Multiplier();
}

stock XP_Set_Multiplier()
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Set_Multiplier", 0);
	#endif

	if ( !iCvar[MP_WEAPONXPMODIFIER] )
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

public XP_Save(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Save",id)
	#endif
	
	if ( !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}

	if ( p_data[id][P_RACE] == 0 || is_user_bot(id) || !iCvar[MP_SAVEXP] || p_data[id][P_XP] == 0 )
	{
		return PLUGIN_CONTINUE;
	}
	
	// If we're saving by STEAM_ID, lets make sure the user has a steam ID
	new szPlayerID[33];
	get_user_authid(id, szPlayerID, 31);
	if( iCvar[FT_SAVEBY] == 0 && equal(szPlayerID, "STEAM_ID_PENDING") )
	{
		return PLUGIN_CONTINUE;
	}

	new szPlayerIP[20], szPlayerName[66];
	get_user_name(id, szPlayerName, 32);
	get_user_ip(id, szPlayerIP, 19);

	// Save to the database
	if ( iCvar[SV_SQL] )
	{
		// Verify we have a database connection
		if ( !XP_Check_Connection() )
		{
			return PLUGIN_CONTINUE;
		}

		// Prepare name for the query (playername is 66 in case all 33 characters are ')
		XP_AddSlashes( szPlayerName, 65 );
			
		// Save the data
		new szQuery[512];
		format(szQuery, 511, "REPLACE INTO `%s` (`playerid`, `playername`, `xp`, `race`, `skill1`, `skill2`, `skill3`, `skill4`) VALUES ('%s', '%s', %d, %d, %d, %d, %d, %d)", g_DBTableName, (iCvar[FT_SAVEBY]==2) ? szPlayerName : ((iCvar[FT_SAVEBY]==1) ? szPlayerIP : szPlayerID), szPlayerName, p_data[id][P_XP], p_data[id][P_RACE], p_data[id][P_SKILL1], p_data[id][P_SKILL2], p_data[id][P_SKILL3], p_data[id][P_ULTIMATE]);

 		new Result:res = dbi_query(sql, szQuery);
		
		// Verify we have a result
		if (res < RESULT_NONE)
		{
			client_print(id, print_chat, "%s An error has occurred when saving your race information, please contact a server administrator", g_MODclient);
			XP_DBI_Error( res, szQuery, 3 );
			return PLUGIN_CONTINUE;
		} 	
	}
	// Otherwise lets save to the vault
	else
	{		
		new szKey[128], szData[512];
		
		// Format the data for entry
		format( szData, 511, "%s %d %d %d %d %d %d %s %d %s", szPlayerID, p_data[id][P_XP], p_data[id][P_RACE], p_data[id][P_SKILL1], p_data[id][P_SKILL2], p_data[id][P_SKILL3], p_data[id][P_ULTIMATE], szPlayerIP, get_systime(), szPlayerName );
		
		// Format the vault key
		format( szKey, 127, "%s_%d", (iCvar[FT_SAVEBY]==SAVE_NAME) ? szPlayerName : ((iCvar[FT_SAVEBY]==SAVE_IP) ? szPlayerIP : szPlayerID), p_data[id][P_RACE] );
		
		// Save the data
		set_vaultdata(szKey, szData);
	}

	return PLUGIN_CONTINUE
}

// Function will get data about a player's current race, and set it to whats in the DB/vault
public XP_Set_Race_Data( id )
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Set_Race_Data",id)
	#endif

	if ( !warcraft3 )
		return PLUGIN_CONTINUE;

	if ( !iCvar[MP_SAVEXP] || !id )
		return PLUGIN_CONTINUE;

	// If we're saving by STEAM_ID, lets make sure the user has a steam ID
	new szPlayerID[33];
	get_user_authid(id, szPlayerID, 31);
	if( iCvar[FT_SAVEBY] == SAVE_STEAMID && equal(szPlayerID, "STEAM_ID_PENDING") )
	{
		client_print(id, print_chat, "%s Unable to retreive race information, you have no STEAM ID, please rejoin the server.", g_MODclient);
		return PLUGIN_CONTINUE;
	}
	
	new szPlayerIP[20], szPlayerName[66];
	get_user_name(id, szPlayerName, 32);
	get_user_ip(id, szPlayerIP, 19);

	if ( iCvar[SV_SQL] )
	{
		// Verify we have a database connection
		if ( !XP_Check_Connection() )
		{
			return PLUGIN_CONTINUE;
		}

		// Prepare name for the query
		XP_AddSlashes( szPlayerName, 65 );

		new szQuery[256];
		format( szQuery, 255, "SELECT `xp`, `skill1`, `skill2`, `skill3`, `skill4` FROM `%s` WHERE (`playerid` = '%s' AND `race` = %d)", g_DBTableName, (iCvar[FT_SAVEBY]==SAVE_NAME) ? szPlayerName : ((iCvar[FT_SAVEBY]==SAVE_IP) ? szPlayerIP : szPlayerID), p_data[id][P_RACE])

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

		format( szKey, 127, "%s_%d", (iCvar[FT_SAVEBY]==SAVE_NAME) ? szPlayerName : ((iCvar[FT_SAVEBY]==SAVE_IP) ? szPlayerIP : szPlayerID), p_data[id][P_RACE] );

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

// Function will retreive the XP for a user's race and call the changerace menu function
public XP_Get( id )
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Get",id)
	#endif

	if ( !warcraft3 )
		return PLUGIN_CONTINUE;

	if ( !iCvar[MP_SAVEXP] || !id )
		return PLUGIN_CONTINUE;

	// If we're saving by STEAM_ID, lets make sure the user has a steam ID
	new szPlayerID[33];
	get_user_authid(id, szPlayerID, 31);
	if( iCvar[FT_SAVEBY] == SAVE_STEAMID && equal(szPlayerID, "STEAM_ID_PENDING") )
	{
		client_print(id, print_chat, "%s Unable to retreive race information, you have no STEAM ID, please rejoin the server.", g_MODclient);
		return PLUGIN_CONTINUE;
	}
	
	new szPlayerIP[20], szPlayerName[66];
	new iRaceXP[9]= {0,0,0,0,0,0,0,0,0};
	get_user_name(id, szPlayerName, 32);
	get_user_ip(id, szPlayerIP, 19);

	// Retreive data via SQL
	if ( iCvar[SV_SQL] )
	{
		// Make sure we have a valid SQL Connection
		if ( !XP_Check_Connection() )
		{
			return PLUGIN_CONTINUE;
		}

		// Prepare name for the query
		XP_AddSlashes( szPlayerName, 65 );
	
		new szQuery[256];
		format(szQuery, 255, "SELECT `xp`, `race` FROM `%s` WHERE (`playerid` = '%s')", g_DBTableName, (iCvar[FT_SAVEBY]==SAVE_NAME) ? szPlayerName : ((iCvar[FT_SAVEBY]==SAVE_IP) ? szPlayerIP : szPlayerID));
 		
		new Result:res = dbi_query(sql, szQuery);
		
		// Make sure we have a result
		if (res < RESULT_NONE)
		{
			client_print(id, print_chat, "%s An error has occurred when retreiving your race information, please contact a server administrator", g_MODclient);
			XP_DBI_Error( res, szQuery, 0 );
			return PLUGIN_CONTINUE;
		}

		// Loop through all of the records
		new szXP[8], szRace[2], iXP, iRace;
		while ( res && dbi_nextrow(res) > 0 )
		{
			dbi_result(res, "xp", szXP, 7);
			dbi_result(res, "race", szRace, 1);

			iXP = str_to_num( szXP );
			iRace = str_to_num( szRace );
			
			// Save the user's XP in an array
			if ( iRace > 0 && iRace < 10)
			{
				iRaceXP[iRace-1] = iXP;
			}
		}
		
		// Free the result set
		dbi_free_result(res); 
	}
	// Retrieve data via the vault
	else
	{
		new iRace, iXP, iAttempt, szKey[128], szXP[8], szData[256];
		for( iRace = 1; iRace < 10; iRace++ )
		{
			format( szKey, 127, "%s_%d", (iCvar[FT_SAVEBY]==SAVE_NAME) ? szPlayerName : ((iCvar[FT_SAVEBY]==SAVE_IP) ? szPlayerIP : szPlayerID), iRace );

			iAttempt = get_vaultdata( szKey, szData, 255 );
			
			// Only want to check if a key was found
			if ( iAttempt )
			{
				format( szXP, 7, "" );

				// Parse the vault data
				parse( szData, szPlayerID, 31, szXP, 7 );
				
				iXP = str_to_num( szXP );
				
				// Save the user's XP in an array
				if ( iRace > 0 && iRace < 10 )
				{
					iRaceXP[iRace-1] = iXP;
				}
			}
		}
	}

	// We don't want to replace the player's current XP with whats in the database now do we ?
	if ( p_data[id][P_RACE] )
	{
		iRaceXP[p_data[id][P_RACE]-1] = p_data[id][P_XP];
	}

	// Display the menu to the user
	menu_Select_Race(id, iRaceXP);

	return PLUGIN_CONTINUE;
}

public XP_Set_DBI(){
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Set_DBI",0)
	#endif
	
	// Check the DBI loaded variable
	if ( !g_DBILoaded )
	{
		// Fall back to vault
		iCvar[SV_SQL] = 0;
	}

	// Try to open a database connection
	if ( iCvar[SV_SQL] )
	{
		// We have an attempt, lets increment our counter
		iSQLAttempts++;
		
		// Verify the type of DBI module that is loaded is supported
		dbi_type(SQLtype, 15);
		if (equali(SQLtype, g_MySQL))
		{
			iSQLtype = SQL_MYSQL;
			copy(SQLtype, strlen(g_MySQL)+1, g_MySQL);
		}
		else if (equali(SQLtype, g_SQLite))
		{
			iSQLtype = SQL_SQLITE;
			copy(SQLtype, strlen(g_SQLite)+1, g_SQLite);
		}
		else
		{
			iSQLtype = SQL_NONE;
			log_amx("Unsupported database type found (%s), the supported databases are %s or %s", SQLtype, g_MySQL, g_SQLite);
			return PLUGIN_CONTINUE;
		}

		// Determine the database information
		new szHost[64], szUser[32], szPass[32], szDB[128], szError[256];
		get_cvar_string("SQL_dbhost", szHost, 63);
		get_cvar_string("SQL_dbuser", szUser, 31);
		get_cvar_string("SQL_dbpass", szPass, 31);
		get_cvar_string("SQL_dbname", szDB, 127);
		
		// Set a default DB if it's SQLite and the user didn't supply one
		if ( iSQLtype == SQL_SQLITE && strlen(szDB) < 1 )
		{
			copy( szDB, 127, "addons/amxmodx/data/war3ft.db" );
		}

		// Attempt the Connection
		sql = dbi_connect(szHost, szUser, szPass, szDB, szError, 255);
		
		// Verify our database connection has worked
		if ( !XP_Check_Connection() )
		{
			log_amx( "Error: %s", szError );
			return PLUGIN_CONTINUE;
		}
		log_amx( "Connection to %s database successful", SQLtype );


		// Get the table name
		get_cvar_string("SQL_tbname", g_DBTableName, 63);

		// Format the create table statement
		new szQuery[512];
		if ( iSQLtype == SQL_MYSQL )
		{
			format( szQuery, 511, "CREATE TABLE IF NOT EXISTS `%s` (`playerid` VARCHAR(35) NOT NULL DEFAULT '', `playername` VARCHAR(35) NOT NULL DEFAULT '', `xp` INT(11) NOT NULL DEFAULT 0, `race` TINYINT(4) NOT NULL DEFAULT 0, `skill1` TINYINT(4) NOT NULL DEFAULT 0, `skill2` TINYINT(4) NOT NULL DEFAULT 0, `skill3` TINYINT(4) NOT NULL DEFAULT 0, `skill4` TINYINT(4) NOT NULL DEFAULT 0, `time` TIMESTAMP(14) NOT NULL, PRIMARY KEY (`playerid`, `race`))", g_DBTableName );
		}
		else if ( iSQLtype == SQL_SQLITE )
		{
			format( szQuery, 511, "CREATE TABLE `%s` (`playerid` VARCHAR(35) NOT NULL DEFAULT '', `playername` VARCHAR(35) NOT NULL DEFAULT '', `xp` INT(11) NOT NULL DEFAULT 0, `race` TINYINT(4) NOT NULL DEFAULT 0, `skill1` TINYINT(4) NOT NULL DEFAULT 0, `skill2` TINYINT(4) NOT NULL DEFAULT 0, `skill3` TINYINT(4) NOT NULL DEFAULT 0, `skill4` TINYINT(4) NOT NULL DEFAULT 0, `time` TIMESTAMP(14) NOT NULL DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY (`playerid`, `race`))", g_DBTableName );
		}

		// Execute our CREATE TABLE statement if it's MySQL or the SQL LITE table doesn't exist
		if ( iSQLtype != SQL_SQLITE || !sqlite_table_exists(sql, g_DBTableName) )
		{
			new Result:ret = dbi_query(sql, szQuery)

			if ( ret < RESULT_NONE )
			{
				XP_DBI_Error( ret, szQuery, 2 );
				return PLUGIN_CONTINUE;
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
		if ( iSQLtype == SQL_SQLITE ) 
		{
			new szIntegrityCheck[64];
			format(szQuery, 511, "PRAGMA integrity_check");
			new Result:res = dbi_query(sql, szQuery);
			
			// Check for an error
			if ( res < RESULT_NONE )
			{
				XP_DBI_Error( res, szQuery, 2 );
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
	}
	// Tell the vault what version is running (we need this for the vault conversion)
	else
	{
		set_vaultdata( "war3ft_version", WC3VERSION );
	}

	return PLUGIN_CONTINUE;
}

// Reset the user's XP to 0
public XP_Reset(id)
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Reset", id);
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE;

	p_data[id][P_LEVEL]		= 0;
	p_data[id][P_XP]		= 0;
	p_data[id][P_SKILL1]	= 0;
	p_data[id][P_SKILL2]	= 0;
	p_data[id][P_SKILL3]	= 0;
	p_data[id][P_ULTIMATE]	= 0;

	XP_Save(id);

	WAR3_Display_Level(id, DISPLAYLEVEL_NONE);

	client_print(id, print_chat, "%s %L", g_MODclient, id, "YOUR_XP_HAS_BEEN_RESET");

	return PLUGIN_CONTINUE;
}	

// This function will prune a single player (checks for the latest date and sets them all to the same)
public XP_Prune_Player(id)
{
	if ( iCvar[MP_SAVEXP] && iCvar[FT_AUTO_PRUNING] )
	{
		new szPlayerID[32], szPlayerName[32], szPlayerIP[32];
		get_user_authid(id, szPlayerID, 31);
		get_user_name(id, szPlayerName, 31);
		get_user_ip(id, szPlayerIP, 31);

		// Update all records if we're using the vault
		if ( !iCvar[SV_SQL] )
		{
			new iRace, iAttempt, szKey[128], szData[512];

			// Loop through and find the latest timestamp and re-save
			for( iRace = 1; iRace < 10; iRace++ )
			{
				format( szKey, 127, "%s_%d", (iCvar[FT_SAVEBY]==SAVE_NAME) ? szPlayerName : ((iCvar[FT_SAVEBY]==SAVE_IP) ? szPlayerIP : szPlayerID), iRace );

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
			format(szQuery, 511, "UPDATE `%s` SET `time` = NOW() WHERE `playerid` = '%s';", g_DBTableName, (iCvar[FT_SAVEBY]==2) ? szPlayerName : ((iCvar[FT_SAVEBY]==1) ? szPlayerIP : szPlayerID));

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
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Prune", 0);
	#endif

	if ( iCvar[FT_AUTO_PRUNING] )
	{
		// Vault pruning (only works with vaults created with version 2.2.8)
		if ( !iCvar[SV_SQL] )
		{
			new szVault[] = "addons/amxmodx/data/vault.ini";
			
			// Make sure the vault exists
			if ( file_exists( szVault ) )
			{
				new len, line, szText[256];
				new iCurrentTime = get_systime();
				// 86400 = 24 hours * 60 minutes * 60 seconds
				new iExpiredTime = iCurrentTime - (iCvar[SV_DAYSBEFOREDELETE] * 86400);

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

			if ( iSQLtype == SQL_MYSQL )
			{
				// Timestamp format: 20030912122142
				// Y = 2003 M = 09 D = 12 H = 12 M = 21 S = 42	
				format( query, 255, "DELETE FROM `%s` WHERE DATE_SUB(CURDATE(),INTERVAL %d DAY) > time;", g_DBTableName, iCvar[SV_DAYSBEFOREDELETE] );
			}
			else if ( iSQLtype == SQL_SQLITE )
			{
				// Timestamp format: 2003-09-12 12:21:42
				// Y = 2003 M = 09 D = 12 H = 12 M = 21 S = 42
				format( query, 255, "DELETE FROM `%s` WHERE ((julianday(`time`) + %d) < julianday('now'))", g_DBTableName, iCvar[SV_DAYSBEFOREDELETE] );
			}

			dbi_query(sql, query);

			// Vacuum the SQL LITE DB Table
			if (iSQLtype == SQL_SQLITE)
			{
				format( query, 255, "VACUUM `%s`", g_DBTableName );

				dbi_query( sql, query );
			}
		}

		log_amx("Database pruning successful, data older than %d days was removed", iCvar[SV_DAYSBEFOREDELETE]);
	}
}

// Close the database connection
public XP_CloseDB()
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_CloseDB", 0);
	#endif

	if ( iCvar[SV_SQL] )
	{
		if( sql )
		{
			dbi_close( sql );
			log_amx( "%s database connection closed", SQLtype );
		}
	}
}

// This function will save the XP for all players, but it will save the data every 0.1 seconds (reduce lag?)
public XP_Save_All()
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Save_All", 0);
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE;
	
	if ( !iCvar[MP_SAVEXP] )
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
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Save_Helper", parm[0]);
	#endif

	XP_Save( parm[0] );
}

// The id should be a unique number, so we know what function called it (useful for debugging)
public XP_DBI_Error( Result:res, query[], id )
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_DBI_Error", id);
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

// Prepares text for database insertion
public XP_AddSlashes( text[], len )
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_AddSlashes", 0);
	#endif

	replace_all( text, len, "'", "\'" );
}

// Verifies that the database connection is ok
public XP_Check_Connection()
{
	#if ADVANCED_DEBUG
		writeDebugInfo("XP_Check_Connection", 0);
	#endif

	if( sql < SQL_OK )
	{
		if( iSQLAttempts < SQL_ATTEMPTS )
		{
			log_amx( "Database connection failed, attempting to reconnect in %0.0f seconds", SQL_ATTEMPT_DELAY );
			set_task( SQL_ATTEMPT_DELAY, "XP_Set_DBI", TASK_SETSQL );
		}
		else if( iSQLAttempts >= SQL_ATTEMPTS )
		{
			iCvar[MP_SAVEXP] = 0
			iCvar[SV_SQL] = 0

			log_amx("Unable to connect to the %s database after %d attempts, switching to short-term mode", SQLtype, iSQLAttempts)
		}

		return false;
	}

	return true;
}

// Function from war3x thanks ryan!!!
public XP_get_admin_flag()
{
#if ADVANCED_DEBUG
	writeDebugInfo("XP_get_admin_flag", 0);
#endif

    new szFlags[24];
    get_cvar_string( "FT_admin_flag", szFlags, 23 );

    return ( read_flags( szFlags ) );
}
