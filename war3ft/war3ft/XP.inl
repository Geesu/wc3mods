/***********************************************************************
		XP FUNCTIONS (idea from war3x)
***********************************************************************/
stock XP_give(id, iXP){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("XP_give",id)
	#endif

	// Check for min. players here
	new numberofplayers = get_playersnum() 

	if(numberofplayers < iCvar[FT_MIN_B4_XP])
		return 0

	p_data[id][P_XP] += iXP

	WAR3_Display_Level(id, DISPLAYLEVEL_SHOWGAINED)

	return iXP
}

stock XP_onDeath(victim_id, killer_id, weapon, headshot){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("XP_onDeath",victim_id)
	#endif

	// Don't award XP to world
	if(killer_id==0)
		return PLUGIN_CONTINUE

	new victimName[32], iXP, XP
	get_user_name(victim_id,victimName,31)

	if (killer_id && killer_id!=victim_id && victim_id){

		// He killed a teammate
		if (get_user_team(killer_id)==get_user_team(victim_id)){
			iXP = -1 * xpgiven[p_data[killer_id][P_LEVEL]]
			XP = XP_give(killer_id,iXP)
			if (iCvar[FT_KILL_OBJECTIVES]){
				client_print(killer_id,print_chat,"%L",killer_id,"AWARDED_FOR_KILLING_TEAMMATE",g_MOD,XP)
			}
		}
		else{
			if (!iCvar[MP_WEAPONXPMODIFIER]){
				iXP = xpgiven[p_data[victim_id][P_LEVEL]]
				XP = XP_give(killer_id,iXP)
				if (iCvar[FT_KILL_OBJECTIVES]){			
					client_print(killer_id,print_chat,"%L",killer_id,"AWARDED_FOR_KILLING",g_MOD,XP,victimName)
				}
			}
			else
			{
				iXP = floatround(xpgiven[p_data[victim_id][P_LEVEL]] * weaponxpmultiplier[weapon])
				XP = XP_give(killer_id,iXP)
				if (iCvar[FT_KILL_OBJECTIVES]){
					client_print(killer_id,print_chat,"%L",killer_id,"AWARDED_FOR_KILLING",g_MOD,XP,victimName)
				}
			}
		#if MOD == 0
			if( headshot && killer_id!=victim_id){
				iXP = iCvar[FT_HEADSHOT_BONUS]
				XP = XP_give(killer_id,iXP)
				if (iCvar[FT_KILL_OBJECTIVES]){
					client_print(killer_id,print_chat,"%s %L",g_MODclient, killer_id,"AWARDED_FOR_HEADSHOT", XP)			
				}
			}
			if (victim_id==g_hostageSaver){
				iXP = iCvar[FT_KILLRESCUEMANXP]
				XP = XP_give(killer_id,iXP)
				if (iCvar[FT_OBJECTIVES]){	
					client_print(killer_id,print_chat,"%s %L", g_MODclient, killer_id,"AWARDED_FOR_KILLING_HOSTAGE_R", XP)
				}
			}
			else if (victim_id==g_bombDefuser && killer_id!=g_bombDefuser){						
				iXP = iCvar[FT_DEFUSER_KILL_BONUS]
				XP = XP_give(killer_id,iXP)
				if (iCvar[FT_OBJECTIVES]){				
					client_print(killer_id,print_chat, "%s %L", g_MODclient, killer_id,"AWARDED_FOR_KILLING_BOMB_D",XP)
				}	
			}
			else if (victim_id==g_bombCarrier && killer_id!=g_bombCarrier){						
				iXP = iCvar[FT_KILL_BOMB_CARRIER_BONUS]
				XP = XP_give(killer_id,iXP)
				if (iCvar[FT_OBJECTIVES]){
					client_print(killer_id,print_chat, "%s %L", g_MODclient, killer_id,"AWARDED_FOR_KILLING_BOMB_C",XP)
				}
			}
			else if (victim_id==g_vipID && killer_id!=g_vipID){
				iXP = iCvar[FT_VIP_KILL_BONUS]
				XP = XP_give(killer_id,iXP)
				if (iCvar[FT_OBJECTIVES]){
					client_print(killer_id,print_chat, "%s %L", g_MODclient, killer_id,"AWARD_FOR_KILLING_VIP",XP)
				}
			}

		#endif
		}
	}

	return PLUGIN_CONTINUE
}

stock XP_Set(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("XP_set",0)
	#endif

	if (iCvar[MP_SAVEXP]){
		xpgiven = {6,8,10,12,14,16,18,20,24,28,32}
		xplevel = {0,100,200,400,800,1600,3200,6400,12800,25600,51200}
	}

	for(new i=0;i<11;i++)
		xplevel[i] = floatround(xplevel[i] * fCvar[MP_XPMULTIPLIER])

	XP_Set_Multiplier()
}

stock XP_Set_Multiplier(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("set_xpmultiplier", 0)
	#endif

	if (!iCvar[MP_WEAPONXPMODIFIER])
		return PLUGIN_CONTINUE

	weaponxpmultiplier[CSW_WORLDSPAWN]		= 0.0

#if MOD == 0
	weaponxpmultiplier[CSW_USP]				= 2.5
	weaponxpmultiplier[CSW_DEAGLE]			= 2.5
	weaponxpmultiplier[CSW_GLOCK18]			= 2.5
	weaponxpmultiplier[CSW_ELITE]			= 3.5
	weaponxpmultiplier[CSW_P228]			= 2.5
	weaponxpmultiplier[CSW_FIVESEVEN]		= 2.5

	weaponxpmultiplier[CSW_XM1014]			= 1.25
	weaponxpmultiplier[CSW_M3]				= 1.5

	weaponxpmultiplier[CSW_MP5NAVY]			= 1.0
	weaponxpmultiplier[CSW_UMP45]			= 1.25
	weaponxpmultiplier[CSW_P90]				= 1.25
	weaponxpmultiplier[CSW_TMP]				= 1.5
	weaponxpmultiplier[CSW_MAC10]			= 1.5
	weaponxpmultiplier[CSW_GALIL]			= 1.15
	weaponxpmultiplier[CSW_FAMAS]			= 1.15

	weaponxpmultiplier[CSW_AWP]				= 1.0
	weaponxpmultiplier[CSW_M4A1]			= 1.0
	weaponxpmultiplier[CSW_AK47]			= 1.0
	weaponxpmultiplier[CSW_AUG]				= 1.0
	weaponxpmultiplier[CSW_SG552]			= 1.0
	weaponxpmultiplier[CSW_G3SG1]			= 1.0
	weaponxpmultiplier[CSW_SG550]			= 1.0
	weaponxpmultiplier[CSW_M249]			= 1.25
	weaponxpmultiplier[CSW_SCOUT]			= 3.0

	weaponxpmultiplier[CSW_HEGRENADE]		= 1.75
	weaponxpmultiplier[CSW_KNIFE]			= 6.0

	weaponxpmultiplier[CSW_C4]				= 2.0
	weaponxpmultiplier[CSW_SMOKEGRENADE]	= 1.0
	weaponxpmultiplier[CSW_FLASHBANG]		= 1.0
#endif
#if MOD == 1
	weaponxpmultiplier[DODW_AMERKNIFE		] = 6.0
	weaponxpmultiplier[DODW_GERKNIFE		] = 6.0
	weaponxpmultiplier[DODW_BRITKNIFE		] = 6.0
	weaponxpmultiplier[DODW_GARAND_BUTT		] = 6.0

	weaponxpmultiplier[DODW_HANDGRENADE		] = 1.75
	weaponxpmultiplier[DODW_STICKGRENADE	] = 1.75
	weaponxpmultiplier[DODW_STICKGRENADE_EX	] = 1.75
	weaponxpmultiplier[DODW_HANDGRENADE_EX	] = 1.75

	weaponxpmultiplier[DODW_COLT			] = 1.0
	weaponxpmultiplier[DODW_LUGER			] = 1.0
	weaponxpmultiplier[DODW_GARAND			] = 1.0
	weaponxpmultiplier[DODW_SCOPED_KAR		] = 1.0
	weaponxpmultiplier[DODW_THOMPSON		] = 1.0
	weaponxpmultiplier[DODW_STG44			] = 1.0
	weaponxpmultiplier[DODW_SPRINGFIELD		] = 1.0
	weaponxpmultiplier[DODW_KAR				] = 1.0
	weaponxpmultiplier[DODW_BAR				] = 1.0
	weaponxpmultiplier[DODW_MP40			] = 1.0
	weaponxpmultiplier[DODW_MG42			] = 1.0
	weaponxpmultiplier[DODW_30_CAL			] = 1.0
	weaponxpmultiplier[DODW_SPADE			] = 1.0
	weaponxpmultiplier[DODW_M1_CARBINE		] = 1.0
	weaponxpmultiplier[DODW_MG34			] = 1.0
	weaponxpmultiplier[DODW_GREASEGUN		] = 1.0
	weaponxpmultiplier[DODW_FG42			] = 1.0
	weaponxpmultiplier[DODW_K43				] = 1.0
	weaponxpmultiplier[DODW_ENFIELD			] = 1.0
	weaponxpmultiplier[DODW_STEN			] = 1.0
	weaponxpmultiplier[DODW_BREN			] = 1.0
	weaponxpmultiplier[DODW_WEBLEY			] = 1.0
	weaponxpmultiplier[DODW_BAZOOKA			] = 1.0
	weaponxpmultiplier[DODW_PANZERSCHRECK	] = 1.0
	weaponxpmultiplier[DODW_PIAT			] = 1.0
	weaponxpmultiplier[DODW_SCOPED_FG42		] = 1.0
	weaponxpmultiplier[DODW_FOLDING_CARBINE	] = 1.0
	weaponxpmultiplier[DODW_KAR_BAYONET		] = 1.0
	weaponxpmultiplier[DODW_SCOPED_ENFIELD	] = 1.0
	weaponxpmultiplier[DODW_MILLS_BOMB		] = 1.0
	weaponxpmultiplier[DODW_ENFIELD_BAYONET	] = 1.0
	weaponxpmultiplier[DODW_MORTAR			] = 1.0
	weaponxpmultiplier[DODW_K43_BUTT		] = 1.0

#endif

	weaponxpmultiplier[CSW_LIGHTNING]		= 1.0
	weaponxpmultiplier[CSW_SUICIDE]			= 1.0
	weaponxpmultiplier[CSW_FLAME]			= 1.0
	weaponxpmultiplier[CSW_LOCUSTS]			= 1.1
	weaponxpmultiplier[CSW_SERPENTWARD]		= 1.0
	weaponxpmultiplier[CSW_SHADOW]			= 1.0	
	weaponxpmultiplier[CSW_THORNS]			= 1.0
	weaponxpmultiplier[CSW_CARAPACE]		= 1.0
	weaponxpmultiplier[CSW_CARRION]			= 1.0
	weaponxpmultiplier[CSW_ORB]				= 1.0
	weaponxpmultiplier[CSW_CONCOCTION]		= 1.0

	return PLUGIN_CONTINUE
}

public XP_Save(id){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("XP_Save",id)
	#endif

	if (is_user_bot(id) || !iCvar[MP_SAVEXP])
		return PLUGIN_CONTINUE

	if(p_data[id][P_RACE]==0)
		return PLUGIN_CONTINUE

	new playerid[32], playername[32], timet[32], ip[32]

	if (iCvar[SV_MYSQL]){
		if(mysql < SQL_OK){
			if(iSQLAttempts < SQL_ATTEMPTS)
				XP_Set_DBI()
			else if(iSQLAttempts != SQL_ATTEMPTS + 10){
				iSQLAttempts = SQL_ATTEMPTS + 10
				iCvar[MP_SAVEXP] = 0
				iCvar[SV_MYSQL] = 0
				log_amx("[%s] Unable to connect to the database server after %d tries, switching to short-term mode", g_MOD, iSQLAttempts)
				client_print(0,print_chat,"%s Unable to connect to the database server after %d tries, switching to short-term mode", g_MODclient, iSQLAttempts)
			}
			return PLUGIN_CONTINUE
		}
		new mquery[1024]
		get_user_name(id,playername,31)
		get_user_authid(id,playerid,31)
		get_user_ip(id,ip,31) 

		replace(playername, 31, "'", "")

		if(iCvar[FT_SAVEBY]==0)						// Save by steam ID
			format(mquery, 1023, "REPLACE INTO `%s` SET playerid='%s',playername='%s',xp='%d',race='%d',skill1='%d',skill2='%d',skill3='%d',skill4='%d'",mysqltablename,playerid,playername,p_data[id][P_XP], p_data[id][P_RACE],p_data[id][P_SKILL1],p_data[id][P_SKILL2],p_data[id][P_SKILL3],p_data[id][P_ULTIMATE])
		else if(iCvar[FT_SAVEBY]==1)					// Save by IP address
			format(mquery, 1023, "REPLACE INTO `%s` SET playerid='%s',playername='%s',xp='%d',race='%d',skill1='%d',skill2='%d',skill3='%d',skill4='%d'",mysqltablename,ip,playername,p_data[id][P_XP], p_data[id][P_RACE],p_data[id][P_SKILL1],p_data[id][P_SKILL2],p_data[id][P_SKILL3],p_data[id][P_ULTIMATE])
		else if(iCvar[FT_SAVEBY]==2)					// Save by Player name
			format(mquery, 1023, "REPLACE INTO `%s` SET playerid='%s',playername='%s',xp='%d',race='%d',skill1='%d',skill2='%d',skill3='%d',skill4='%d'",mysqltablename,playername,playername,p_data[id][P_XP], p_data[id][P_RACE],p_data[id][P_SKILL1],p_data[id][P_SKILL2],p_data[id][P_SKILL3],p_data[id][P_ULTIMATE])

		new Result:ret = dbi_query(mysql, mquery)

		if (ret < RESULT_NONE) {
			new err[255]
			new errNum = dbi_error(mysql, err, 254)
			log_amx("[%s] DBI XP_Save error: %s (%d)", g_MOD, err, errNum)
			return 1
		} 	

	}
	else{			// Save it to a vault :)
		new temp[128], string[512]
		get_time("%m %d %H %M",timet,31)
		get_user_authid(id,playerid,31)
		get_user_name(id,playername,31)
		get_user_ip(id,ip,31)

		format(string,511,"%s %d %d %d %d %d %d %s %s %s",playerid,p_data[id][P_XP],p_data[id][P_RACE],p_data[id][P_SKILL1],p_data[id][P_SKILL2],p_data[id][P_SKILL3],p_data[id][P_ULTIMATE],playername,ip,timet)

		if(iCvar[FT_SAVEBY]==0)						// Save by steam ID
			format(temp,127,"%s_%d",playerid,p_data[id][P_RACE])
		else if(iCvar[FT_SAVEBY]==1)					// Save by IP address
			format(temp,127,"%s_%d",ip,p_data[id][P_RACE])
		else if(iCvar[FT_SAVEBY]==2)					// Save by Player name
			format(temp,127,"%s_%d",playername,p_data[id][P_RACE])

		set_vaultdata(temp,string)
	}
	return PLUGIN_CONTINUE
}

public XP_Retreive(id,returnrace){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("XP_Retreive",id)
	#endif

	if (!iCvar[MP_SAVEXP])
		return PLUGIN_CONTINUE

	if (!id)
		return PLUGIN_CONTINUE

	new mquery[1024]

	new playerid[32], playername[32], ip[32]
	new xp[8], race[2], skill1[2], skill2[2], skill3[2], skill4[2]
	new racexp[9]={0,0,0,0,0,0,0,0,0}
	get_user_name(id,playername,31)
	get_user_authid(id,playerid,31)
	get_user_ip(id,ip,31) 

	if (iCvar[SV_MYSQL]){
		if(mysql < SQL_OK){
			if(iSQLAttempts < SQL_ATTEMPTS)
				XP_Set_DBI()
			else if(iSQLAttempts != SQL_ATTEMPTS + 10){
				iSQLAttempts = SQL_ATTEMPTS + 10
				iCvar[MP_SAVEXP] = 0
				iCvar[SV_MYSQL] = 0
				log_amx("[%s] Unable to connect to the database server after %d tries, switching to short-term mode", g_MOD, iSQLAttempts)
				client_print(0,print_chat,"%s Unable to connect to the database server after %d tries, switching to short-term mode", g_MODclient, iSQLAttempts)
			}
			return PLUGIN_CONTINUE
		}

		if (returnrace){
			if(iCvar[FT_SAVEBY]==0)						// Save by steam ID
				format(mquery, 1023, "SELECT * FROM `%s` WHERE playerid='%s'",mysqltablename,playerid)
			else if(iCvar[FT_SAVEBY]==1)					// Save by IP address
				format(mquery, 1023, "SELECT * FROM `%s` WHERE playerid='%s'",mysqltablename,ip)
			else if(iCvar[FT_SAVEBY]==2)					// Save by Player name
				format(mquery, 1023, "SELECT * FROM `%s` WHERE playerid='%s'",mysqltablename,playername)

			new Result:res = dbi_query(mysql, mquery)

			if (res < RESULT_NONE) {
				new err[255]
				new errNum = dbi_error(mysql, err, 254)
				log_amx("[%s] DBI XP_Retreive error: %s (%d)", g_MOD, err, errNum)
				return PLUGIN_CONTINUE
			}

			while (res && dbi_nextrow(res)>0) {
				dbi_result(res, "xp", xp, 7)
				dbi_result(res, "race", race, 1)
				if (str_to_num(race)>=1 && str_to_num(race)<=9)
					racexp[str_to_num(race)-1] = str_to_num(xp)
			}

			dbi_free_result(res) 

			if (p_data[id][P_RACE])
				racexp[p_data[id][P_RACE]-1]=p_data[id][P_XP]

			menu_Select_Race(id, racexp)
		}
		else{
			if(iCvar[FT_SAVEBY]==0)						// Save by steam ID
				format(mquery, 1023, "SELECT * FROM `%s` WHERE playerid='%s' AND race='%d'",mysqltablename,playerid,p_data[id][P_RACE])
			else if(iCvar[FT_SAVEBY]==1)					// Save by IP address
				format(mquery, 1023, "SELECT * FROM `%s` WHERE playerid='%s' AND race='%d'",mysqltablename,ip,p_data[id][P_RACE])
			else if(iCvar[FT_SAVEBY]==2)					// Save by Player name
				format(mquery, 1023, "SELECT * FROM `%s` WHERE playerid='%s' AND race='%d'",mysqltablename,playername,p_data[id][P_RACE])

			new Result:res = dbi_query(mysql, mquery)

			if (res < RESULT_NONE) {
				new err[255]
				new errNum = dbi_error(mysql, err, 254)
				log_amx("[%s] DBI XP_Retreive error: %s (%d)", g_MOD, err, errNum)
				
				return PLUGIN_CONTINUE
			}

			if (res && dbi_nextrow(res)>0){
				dbi_result(res, "xp", xp, 7)
				dbi_result(res, "skill1", skill1, 1)
				dbi_result(res, "skill2", skill2, 1)
				dbi_result(res, "skill3", skill3, 1)
				dbi_result(res, "skill4", skill4, 1)				

				dbi_free_result(res) 

				p_data[id][P_XP]=str_to_num(xp)
				p_data[id][P_SKILL1]=str_to_num(skill1)
				p_data[id][P_SKILL2]=str_to_num(skill2)
				p_data[id][P_SKILL3]=str_to_num(skill3)
				p_data[id][P_ULTIMATE]=str_to_num(skill4)

				WAR3_Display_Level(id,DISPLAYLEVEL_SHOWRACE)

			}
			else{
				p_data[id][P_XP]=0
				p_data[id][P_SKILL1]=0
				p_data[id][P_SKILL2]=0
				p_data[id][P_SKILL3]=0
				p_data[id][P_ULTIMATE]=0
				WAR3_Display_Level(id,DISPLAYLEVEL_SHOWRACE)
			}
		}
	}
	else{			// Get info from a vault
		new info[512], temp[128]
		new i, attempt
		if (returnrace)
		{
			for(i=1;i<9;i++){
				format(xp,7,"")
				format(race,1,"")
				format(temp,127,"")

				if(iCvar[FT_SAVEBY]==0)			// Get info by steam ID
					format(temp,127,"%s_%d",playerid,i)
				else if(iCvar[FT_SAVEBY]==1)		// Get info by IP address
					format(temp,127,"%s_%d",ip,i)
				else if(iCvar[FT_SAVEBY]==2)		// Get info by Player name
					format(temp,127,"%s_%d",playername,i)


				attempt = get_vaultdata(temp,info,511)
				parse(info, playerid,31, xp,7, race,1, skill1,1, skill2,1, skill3,1, skill4,1)
				if (str_to_num(race)>=1 && str_to_num(race)<=9)
					racexp[str_to_num(race)-1] = str_to_num(xp)
			}
			if (p_data[id][P_RACE])
				racexp[p_data[id][P_RACE]-1]=p_data[id][P_XP]

			menu_Select_Race(id, racexp)
		}
		else{

			if(iCvar[FT_SAVEBY]==0)			// Get info by steam ID
				format(temp,127,"%s_%d",playerid,p_data[id][P_RACE])
			else if(iCvar[FT_SAVEBY]==1)		// Get info by IP address
				format(temp,127,"%s_%d",ip,p_data[id][P_RACE])
			else if(iCvar[FT_SAVEBY]==2)		// Get info by Player name
				format(temp,127,"%s_%d",playername,p_data[id][P_RACE])

			attempt = get_vaultdata(temp,info,511)
			if (attempt){
				parse(info, playerid,31, xp,7, race,1, skill1,1, skill2,1, skill3,1, skill4,1)
				p_data[id][P_XP]=str_to_num(xp)
				p_data[id][P_SKILL1]=str_to_num(skill1)
				p_data[id][P_SKILL2]=str_to_num(skill2)
				p_data[id][P_SKILL3]=str_to_num(skill3)
				p_data[id][P_ULTIMATE]=str_to_num(skill4)
				WAR3_Display_Level(id,DISPLAYLEVEL_SHOWRACE)
			}
			else{
				p_data[id][P_XP]=0
				p_data[id][P_SKILL1]=0
				p_data[id][P_SKILL2]=0
				p_data[id][P_SKILL3]=0
				p_data[id][P_ULTIMATE]=0
				WAR3_Display_Level(id,DISPLAYLEVEL_SHOWRACE)
			}
		}
	}
	return PLUGIN_CONTINUE
}

public XP_Save_All(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("XP_Save_All",0)
	#endif

	if (warcraft3){
		new players[32], numofplayers, id, i
		get_players(players, numofplayers)
		if(iCvar[SV_MYSQL]){
			if (iCvar[SV_MYSQL_SAVE_END_ROUND]){
				for (i=0; i<numofplayers; i++){
					id = players[i]
					XP_Save(id)
				}
			}
		}
		else{
			for (i=0; i<numofplayers; i++){
				id = players[i]
				XP_Save(id)
			}
		}
	}
	return PLUGIN_CONTINUE
}

public XP_Client_Save(id,show){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("XP_client_save",id)
	#endif

	if (iCvar[MP_SAVEXP]){
		if(show)
			client_print(id,print_chat, "%s XP saved.",g_MODclient)
		XP_Save(id)
	}	
	else if (show)
		client_print(id,print_chat,"%s %L",g_MODclient, id,"XP_IS_NOT_SAVED")
	
	return PLUGIN_CONTINUE
}

public XP_Set_DBI(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("XP_Set_DBI",0)
	#endif

	if (iCvar[SV_MYSQL]){
		get_cvar_string("sv_mysqltablename",mysqltablename,63)
		new mhost[64], muser[32], mpass[32], mdb[32], primarykey[128]
		new mquery[512]
		format (mquery, 511, "CREATE TABLE IF NOT EXISTS `%s` ( `playerid` VARCHAR(35) NOT NULL, `playername` VARCHAR(35) NOT NULL, `xp` INT(11) NOT NULL, `race` TINYINT(4) NOT NULL, `skill1` TINYINT(4), `skill2` TINYINT(4), `skill3` TINYINT(4), `skill4` TINYINT(4), `time` TIMESTAMP( 14 ) NOT NULL, PRIMARY KEY (playerid,race))",mysqltablename)

		get_cvar_string("FT_mysql_host",mhost,63)
		get_cvar_string("FT_mysql_user",muser,31)
		get_cvar_string("FT_mysql_pass",mpass,31)
		get_cvar_string("FT_mysql_db",mdb,31)

		new err[255], errNum = 0
		mysql = dbi_connect(mhost,muser,mpass,mdb)
		if (mysql < SQL_OK) {
			errNum = dbi_error(mysql, err, 254)
			log_amx("[%s] DBI XP_Set_DBI error: %s|%d", g_MOD, err, errNum)

			if(iSQLAttempts < SQL_ATTEMPTS){
				new Float:delay = 25.0
				log_amx("[%s] Will attempt to re-connect to the MySQL database in %f seconds", g_MOD, delay)
				set_task(delay, "XP_Set_DBI", TASK_SETMYSQL)
			}
			iSQLAttempts++
			return 1
		}
		else
			server_print("[%s] Connection to MySQL Database successful (%d)", g_MOD, mysql)

		new Result:ret = dbi_query(mysql, mquery)

		if (ret < RESULT_NONE) {
			errNum = dbi_error(mysql, err, 254)
			log_amx("[%s] DBI XP_Set_DBI error: %s|%d", g_MOD, err, errNum)
			return 1
		}

		format(mquery, 511, "show index from `%s`", mysqltablename)

		new Result:res = dbi_query(mysql, mquery)

		if (res <= RESULT_NONE) {
			errNum = dbi_error(mysql, err, 254)
			log_amx("[%s] DBI XP_Set_DBI error: %s|%d", g_MOD, err, errNum)
			return 1
		}

		while (res && dbi_nextrow(res)>0) {
			dbi_result(res, "Column_name", primarykey, 127)
		}

		dbi_free_result(res)

		if (contain(primarykey,"playerid")){	// contain() returns 0 if the two strings match exactly
			format(mquery, 511, "ALTER TABLE `%s` DROP PRIMARY KEY , ADD PRIMARY KEY ( `playerid` , `race` )", mysqltablename)
			new Result:ret2 = dbi_query(mysql, mquery)
			if (ret2 < RESULT_NONE) {
				errNum = dbi_error(mysql, err, 254)
				log_amx("[%s] DBI XP_Set_DBI error: %s|%d", g_MOD, err, errNum)
				return 1
			}
		}
	}

	return PLUGIN_CONTINUE
}