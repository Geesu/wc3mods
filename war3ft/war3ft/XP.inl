/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	XP Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

XP_give(id, iXP)
{

	if ( !WAR3_Check() || !id )
	{
		return 0;
	}

	// Make sure we have the minimum amount of players
	if( get_playersnum() < get_pcvar_num( CVAR_wc3_min_players ) )
	{
		return 0;
	}
	
	p_data[id][P_XP] += iXP;
	
	XP_Check( id );

	return iXP;
}

// Function will verify if the user's level/information is correct and checks to see if the user has gained a level
XP_Check( id, bShowGained = true )
{
	new iOldLevel = p_data[id][P_LEVEL];
	
	// Make sure the user doesn't have negative experience
	if ( p_data[id][P_XP] < 0 )
	{
		p_data[id][P_XP] = 0;
	}

	// Determine what the user's level should be
	for ( new i = 0; i <= MAX_LEVELS; i++ )
	{
		// User has enough XP to advance to the next level
		if ( p_data[id][P_XP] >= xplevel[i] )
		{
			p_data[id][P_LEVEL] = i;
		}

		// On this increment the user doesn't have enough XP to advance to the next level
		else
		{
			break;
		}
	}

	// User gained a level
	if ( p_data[id][P_LEVEL] > iOldLevel && p_data[id][P_RACE] != 0 && bShowGained )
	{
		new szMsg[128];
		formatex( szMsg, 127, "%L", id, "YOU_GAINED_A_LEVEL" );

		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			set_hudmessage( 200, 100, 0, -1.0, 0.25, 0, 1.0, 2.0, 0.1, 0.2, HUDMESSAGE_CHAN_INFO );
			show_hudmessage( id, szMsg );
		}
		else if ( g_MOD == GAME_DOD )
		{
			Create_HudText( id, szMsg, 1 );
		}

		emit_sound( id, CHAN_STATIC, SOUND_LEVELUP, 1.0, ATTN_NORM, 0, PITCH_NORM );
	}

	// We might need to lower the skills the user has ( can occur if you load XP info from a database and the XP multiplier has changed)
	new iSkillsUsed = p_data[id][P_SKILL1] + p_data[id][P_SKILL2] + p_data[id][P_SKILL3] + p_data[id][P_ULTIMATE];

	while ( iSkillsUsed > p_data[id][P_LEVEL] )
	{

		// Remove the user's ultimate
		if ( p_data[id][P_ULTIMATE] == 1 && p_data[id][P_LEVEL] < 6 )
		{
			p_data[id][P_ULTIMATE] = 0;
		}

		// Remove first skill
		else if ( p_data[id][P_SKILL1] >= p_data[id][P_SKILL2] && p_data[id][P_SKILL1] >= p_data[id][P_SKILL3] )
		{
			--p_data[id][P_SKILL1];
		}

		// Remove second skill
		else if ( p_data[id][P_SKILL2] >= p_data[id][P_SKILL1] && p_data[id][P_SKILL2] >= p_data[id][P_SKILL3] )
		{
			--p_data[id][P_SKILL2];
		}

		// Remove third skill
		else if ( p_data[id][P_SKILL3] >= p_data[id][P_SKILL1] && p_data[id][P_SKILL3] >= p_data[id][P_SKILL2] )
		{
			--p_data[id][P_SKILL3];
		}

		iSkillsUsed = p_data[id][P_SKILL1] + p_data[id][P_SKILL2] + p_data[id][P_SKILL3] + p_data[id][P_ULTIMATE];
	}

	WC3_ShowBar( id );
}


XP_onDeath( victim_id, killer_id, weapon, headshot )
{

	// Assuming we have a victim, a killer, and the victim is not the killer
	if ( killer_id && killer_id != victim_id && victim_id )
	{
		new iXP, iXPAwarded;

		// Attacker killed a teammate
		if ( get_user_team( killer_id ) == get_user_team( victim_id ) )
		{
			iXP = -1 * xpgiven[p_data[killer_id][P_LEVEL]];
			iXPAwarded = XP_give( killer_id, iXP );

			if ( get_pcvar_num( CVAR_wc3_show_kill_obj ) )
			{
				client_print( killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_TEAMMATE", iXPAwarded );
			}
		}

		// User didn't kill a teammate, so we can give some XP
		else
		{
			// We have no weapon multiplier - same XP awarded for each weapon
			if ( !get_pcvar_num( CVAR_wc3_xp_weap_multiplier ) )
			{
				iXP = xpgiven[p_data[victim_id][P_LEVEL]];
				iXPAwarded = XP_give( killer_id, iXP );

				if ( get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{			
					new szVictimName[32];
					get_user_name( victim_id, szVictimName, 31 );

					client_print( killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING", iXPAwarded, szVictimName );
				}
			}

			// Give bonus XP based on the weapon used
			else
			{
				iXP = floatround( xpgiven[p_data[victim_id][P_LEVEL]] * weaponxpmultiplier[weapon] );
				iXPAwarded = XP_give( killer_id, iXP );
				
				// Display a message to the user
				if ( get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					new szVictimName[32];
					get_user_name( victim_id, szVictimName, 31 );

					client_print( killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING", iXPAwarded, szVictimName );
				}
			}

			// Bonus XP for getting a headshot
			if ( headshot )
			{
				iXP = get_pcvar_num( CVAR_wc3_headshot );
				iXPAwarded = XP_give( killer_id, iXP );
				
				// Display a message to the user
				if ( get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					client_print( killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_HEADSHOT", iXPAwarded );		
				}
			}

			// Counter-Strike and Condition Zero specific awards

			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{

				// Award XP for killing the hostage saver
				if ( victim_id == g_hostageSaver )
				{
					iXP = get_pcvar_num( CVAR_wc3_kill_rescuer );
					iXPAwarded = XP_give( killer_id, iXP );

					// Display a message to the user
					if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
					{	
						client_print( killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_HOSTAGE_R", iXPAwarded );
					}
				}

				// Award XP for killing the bomb defuser
				else if ( victim_id == g_bombDefuser )
				{						
					iXP = get_pcvar_num( CVAR_wc3_kill_defuser );
					iXPAwarded = XP_give( killer_id, iXP );

					// Display a message to the user
					if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
					{				
						client_print( killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_BOMB_D", iXPAwarded );
					}	
				}

				// Award XP for killing the bomb carrier
				else if ( victim_id == g_bombCarrier )
				{						
					iXP = get_pcvar_num( CVAR_wc3_kill_bomb_carrier );
					iXPAwarded = XP_give( killer_id, iXP );

					// Display a message to the user
					if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
					{
						client_print( killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARDED_FOR_KILLING_BOMB_C", iXPAwarded );
					}
				}

				// Award XP for killing the VIP
				else if ( victim_id == g_vipID )
				{
					iXP = get_pcvar_num( CVAR_wc3_kill_vip );
					iXPAwarded = XP_give( killer_id, iXP );

					// Display a message to the user
					if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
					{
						client_print( killer_id, print_chat, "%s %L", g_MODclient, killer_id, "AWARD_FOR_KILLING_VIP", iXPAwarded );
					}
				}
			}// end cstrike/czero check
		}// user didn't kill teammate
	}// valid killer/victim

	return;
}

XP_Configure()
{

	// If we're saving XP, we want to set the max. amount of XP higher and the amount gained per kill/objective lower
	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		xpgiven = {6,8,10,12,14,16,18,20,24,28,32};
		xplevel = {0,100,200,400,800,1600,3200,6400,12800,25600,51200};
	}

	// Set the XP multiplier
	for ( new i = 0; i < 11; i++ )
	{
		xplevel[i] = floatround( xplevel[i] * get_pcvar_float( CVAR_wc3_xp_multiplier ) );
	}

	// Configure based on weapon multiplier

	// Counter-Strike and Condition Zero weapons
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
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
	}

	// Day of Defeat weapons
	else if ( g_MOD == GAME_DOD )
	{
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
	}

	weaponxpmultiplier[CSW_WORLDSPAWN]		= 0.0;
	weaponxpmultiplier[CSW_LIGHTNING]		= 1.0;
	weaponxpmultiplier[CSW_SUICIDE]			= 1.0;
	weaponxpmultiplier[CSW_IMMOLATE]			= 1.0;
	weaponxpmultiplier[CSW_LOCUSTS]			= 1.0;
	weaponxpmultiplier[CSW_SERPENTWARD]		= 1.0;
	weaponxpmultiplier[CSW_SHADOW]			= 1.0;	
	weaponxpmultiplier[CSW_THORNS]			= 1.0;
	weaponxpmultiplier[CSW_CARAPACE]		= 1.0;
	weaponxpmultiplier[CSW_CARRION]			= 1.0;
	weaponxpmultiplier[CSW_ORB]				= 1.0;
	weaponxpmultiplier[CSW_CONCOCTION]		= 1.0;
}

// Reset the user's XP to 0
public XP_Reset(id)
{

	p_data[id][P_LEVEL]		= 0;
	p_data[id][P_XP]		= 0;
	p_data[id][P_SKILL1]	= 0;
	p_data[id][P_SKILL2]	= 0;
	p_data[id][P_SKILL3]	= 0;
	p_data[id][P_ULTIMATE]	= 0;

	DB_SaveXP( id );
	
	WC3_ShowBar( id );

	client_print( id, print_chat, "%s %L", g_MODclient, id, "YOUR_XP_HAS_BEEN_RESET" );

	return PLUGIN_CONTINUE;
}	

// This function will prune a single player (checks for the latest date and sets them all to the same)

/*
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
*/


// This function will save the XP for all players, but it will save the data every 0.1 seconds (reduce lag?)
public XP_SaveAll()
{

	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}

	new Float:time = 0.0;
	new players[32], numofplayers;
	get_players( players, numofplayers );

	for ( new i=0; i < numofplayers; i++ )
	{
		set_task( time, "XP_SaveHelper", TASK_SAVE_ALL + players[i] );

		time += 0.1;
	}

	return;
}

// Helper function to call save for XP_SaveAll
public XP_SaveHelper( id )
{
	id -= TASK_SAVE_ALL;

	DB_SaveXP( id );
}


// Function from war3x thanks ryan!!!
public XP_get_admin_flag()
{

    new szFlags[24];
    get_pcvar_string( CVAR_wc3_admin_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}
