
///////////////////////////////////////// WAR3X	Commands /////


// Ultimate	command

public cmd_Ultimate( caster	) {

	if ( !g_bWar3xEnabled )
		return PLUGIN_HANDLED;

	new	target = g_PlayerTarget[caster];

	// Press when dead ( target	skills ) *REMOVED TEMPORARILY*

	if ( !is_user_alive( caster	) )
	{
		//HELP_motd_target(	caster,	target );
		return PLUGIN_HANDLED;
	}

	// Press when restricted

	else if	( get_pcvar_bitsum(	CVAR_restrict_ultimates	) &	WAR3_get_ult_flag( g_PlayerInfo[caster][CURRENT_RACE], g_PlayerInfo[caster][CURRENT_ULTIMATE] )	)
	{
		new	szMessage[128];
		formatex( szMessage, 127, "%L",	caster,	"ULTIMATE_RESTRICTED" );

		WAR3_status_text( caster, szMessage, 0.5 );

		return PLUGIN_HANDLED;
	}

	// Press when sleeping

	else if	( g_bPlayerSleeping[caster]	)
	{
		new	szMessage[128];
		formatex( szMessage, 127, "%L",	caster,	"ULTIMATE_NOCAST_SLEEP"	);

		WAR3_status_text( caster, szMessage, 1.0 );
	}

	// Pressing	Key	During Cooldown

	else if	( g_fUltimateCooldown[caster] && is_user_alive(	caster ) )
	{
		if ( caster	== g_Vip )
		{
			new	szMessage[128];
			formatex( szMessage, 127, "%L",	caster,	"ULTIMATE_NOCAST_VIP" );

			WAR3_status_text( caster, szMessage, 1.0 );
		}

		else if	( g_PlayerInfo[caster][CURRENT_ULTIMATE] &&	g_iCurrentRound	<= 1 )
		{
			new	szMessage[128];
			formatex( szMessage, 127, "%L",	caster,	"ULTIMATE_NOCAST_PISTOL" );

			WAR3_status_text( caster, szMessage, 1.0 );
		}

		else if	( g_PlayerInfo[caster][CURRENT_ULTIMATE] )
		{
			if ( g_bFreezeTime )
			{
				new	szMessage[128];
				formatex( szMessage, 127, "%L",	caster,	"ULTIMATE_NOTREADY"	);

				WAR3_status_text( caster, szMessage, 0.5 );
			}

			else
			{
				new	iRemainingTime;
				new	Float:fCooldownTime;

				if ( g_bUltimateWarmup )
				{
					fCooldownTime =	get_pcvar_float( CVAR_ultimatewarmup );
				}

				else
				{
					fCooldownTime =	ULTIMATE_COOLDOWNDEFAULT;
				}

				iRemainingTime = floatround( fCooldownTime - ( get_gametime() -	g_fUltimateCooldown[caster]	) );

				new	szMessage[64];
				formatex( szMessage, 63, "%L", caster, "ULTIMATE_NOTREADY_COOLDOWN", iRemainingTime	);

				WAR3_status_text( caster, szMessage, 0.5 );
			}
		}

		else
		{
			new	szMessage[128];
			formatex( szMessage, 127, "%L",	caster,	"ULTIMATE_NOTFOUND"	);

			WAR3_status_text( caster, szMessage, 0.5 );
		}

		// play	client sound

		client_cmd(	caster,	"speak warcraft3/bonus/Error.wav" );
	}

	// Cast	on self	( some ultimates )

	else if	( (	g_iChargeUltimate[caster] && Ultimate_Target( caster ) & ULTIMATE_TARGET_SELFONLY )	|| ( g_iChargeUltimate[caster] && Ultimate_Target( caster )	& ULTIMATE_TARGET_SELF ) )
		Ultimate_Cast( caster, caster );

	// Cast	on target

	else if	( target &&	!( Ultimate_Target(	caster ) & ULTIMATE_TARGET_SELFONLY	) )
		Ultimate_Cast( caster, target );

	else
	{
		if ( g_iChargeUltimate[caster] && !( Ultimate_Target( caster ) & ULTIMATE_TARGET_SELF )	)
		{
			new	szMessage[128];
			formatex( szMessage, 127, "%L",	caster,	"CANT_TARGET_SELF" );

			WAR3_status_text( caster, szMessage, 0.5 );
		}

		Ultimate_Beep( caster );
	}

	return PLUGIN_HANDLED;
}


// Chat	commands

public Cmd_Say(	id ) {

	new	szText[32];
	read_args( szText, 31 );

	remove_quotes( szText );

	// Remove "/" from command(s)

	if ( equal(	szText,	"/", 1 ) )
		copy( szText, 31, szText[1]	);

	// War3	Vote
/*
	if ( equali( szText, "war3vote"	) || equali( szText, "wc3vote" ) ||	equali(	szText,	"war3xvote"	) || equali( szText, "wc3xvote"	) )
		WAR3_vote_start( id	);
*/
	if ( !g_bWar3xEnabled )
		return PLUGIN_CONTINUE;

	// Check for keywords

	if ( equali( szText, "changerace" )	|| equali( szText, "selectrace"	) || equali( szText, "chooserace" )	)
		menu_SelectRace( id	);

	else if	( equali( szText, "shopmenu" ) || equali( szText, "itemshop" ) || equali( szText, "buyitems" ) )
		menu_ItemShop( id );

	else if	( equali( szText, "selectskill"	) || equali( szText, "selectskills"	) )
		menu_SelectSkills( id );

	else if	( equali( szText, "playerskills" ) )
		HELP_motd_players( id );

	else if	( equali( szText, "skillinfo" )	|| equali( szText, "skillsinfo"	) || equali( szText, "skillhelp" ) || equali( szText, "skillshelp" ) )
		menu_SkillsHelp( id	);

	else if	( equali( szText, "war3help" ) || equali( szText, "wc3help"	) || equali( szText, "war3xhelp" ) || equali( szText, "wc3xhelp" ) )
		HELP_topics( id	);

	else if	( equali( szText, "level" )	|| equali( szText, "showlevel" ) )
		WAR3_hud_level(	id );

	else if	( equali( szText, "skills" ) ||	equali(	szText,	"myskills" ) )
		HELP_motd_target( id, id );

	else if	( equali( szText, "iteminfo" ) || equali( szText, "itemsinfo" )	|| equali( szText, "itemhelp" )	|| equali( szText, "itemshelp" ) )
		HELP_motd_items( id	);

	else if	( equali( szText, "war3menu" ) || equali( szText, "wc3menu"	) || equali( szText, "war3xmenu" ) || equali( szText, "wc3xmenu" ) )
		menu_War3menu( id );

	else if	( equali( szText, "options"	) || equali( szText, "playeroptions" ) || equali( szText, "war3options"	) || equali( szText, "wc3options" )	|| equali( szText, "war3xoptions" )	|| equali( szText, "wc3xoptions" ) )
		menu_PlayerOptions(	id );

	return PLUGIN_CONTINUE;
}


// Drop	weapon ( update	backpack )

public Cmd_Drop( id	) {

	if ( !g_bWar3xEnabled )
		return PLUGIN_CONTINUE;

	if ( g_bAnkhDropWeapons[id]	)
	{
		new	arg_write[1];
		arg_write[0] = id;

		new	task = TASK_ANKH + id;

		remove_task( task, 0 );
		set_task( 0.1, "ITEM_ANKH_give", task, arg_write, 1	);
	}

	return PLUGIN_CONTINUE;
}


// Drop	item

public cmd_DropItem( id	) {

	Item_Drop( id );
	return PLUGIN_HANDLED;
}


// War3x menu

public cmd_War3menu( id	) {

	menu_War3menu( id );
	return PLUGIN_HANDLED;
}


// Shop	menu

public cmd_Shopmenu( id	) {

	menu_ItemShop( id );
	return PLUGIN_HANDLED;
}


public Cmd_SetXp( id ) {

	if ( id	&& !( get_user_flags( id ) & WAR3_get_setxp_flag() ) )
	{
		client_print( id, print_console, "*	[WC3x] %L",	id,	"ERROR_NO_ACCESS" );
		return PLUGIN_HANDLED;
	}

	if ( read_argc() < 4 )
	{
		new	szErrorMsg[256];
		formatex( szErrorMsg, 255, "%s %L.^n%s %L.", WAR3X_PREFIX, id, "ERROR_ARGUMENTS", WAR3X_PREFIX,	id,	"WAR3X_SETXP_FORMAT", "war3x_setxp"	);

		status_print( id, szErrorMsg );
	}

	else
	{
		new	szArgs[128];
		read_args( szArgs, 127 );

		Cmd_SetXpLevel(	id,	"war3x_setxp", szArgs );
	}

	return PLUGIN_HANDLED;
}


public Cmd_SetLevel( id	) {

	if ( id	&& !( get_user_flags( id ) & WAR3_get_setxp_flag() ) )
	{
		client_print( id, print_console, "*	[WC3x] %L",	id,	"ERROR_NO_ACCESS" );
		return PLUGIN_HANDLED;
	}

	if ( read_argc() < 4 )
	{
		new	szErrorMsg[256];
		formatex( szErrorMsg, 255, "%s %L.^n%s %L.", WAR3X_PREFIX, id, "ERROR_ARGUMENTS", WAR3X_PREFIX,	id,	"WAR3X_SETXP_FORMAT", "war3x_setlevel" );

		status_print( id, szErrorMsg );
	}

	else
	{
		new	szArgs[128];
		read_args( szArgs, 127 );

		Cmd_SetXpLevel(	id,	"war3x_setlevel", szArgs );
	}

	return PLUGIN_HANDLED;
}


public Cmd_SetXpLevel( id, szCommand[16], szArgs[128] )	{

	// Grab	arguments

	new	szPlayerName[32], szRaceName[16], szNewValue[16];
	parse( szArgs, szPlayerName, 31, szRaceName, 15, szNewValue, 15	);

	// Get target player id

	new	target = find_player( "b", szPlayerName	);

	if ( !target )
	{
		new	szErrorMsg[128];
		formatex( szErrorMsg, 127, "%s %L",	WAR3X_PREFIX, id, "WAR3X_SETXP_WARNING_NOPLAYER", szPlayerName );

		status_print( id, szErrorMsg );

		return PLUGIN_HANDLED;
	}

	else if	( !g_PlayerInfo[target][CURRENT_RACE] )
	{
		new	szErrorMsg[128];
		formatex( szErrorMsg, 127, "%s %L",	WAR3X_PREFIX, id, "WAR3X_SETXP_WARNING_NORACE",	szPlayerName );

		status_print( id, szErrorMsg );

		return PLUGIN_HANDLED;
	}

	get_user_name( target, szPlayerName, 31	);

	// Check for valid race

	new	race;
	new	szRaceNameShort[16];

	for	( new i	= 0; i < TOTAL_RACES; i++ )
	{
		LANG_GetRaceName( i, id, szRaceNameShort, 15, true );

		if ( equali( szRaceName, szRaceNameShort, 1	) )
		{
			race = i;
			break;
		}
	}

	if ( !race )
	{
		new	szError[64];
		formatex( szError, 63, "%s %L",	WAR3X_PREFIX, id, "WAR3X_SETXP_ERROR_RACENAME" );

		status_print( id, szError );

		return PLUGIN_HANDLED;
	}

	// Get new values

	new	bool:bAdd;
	new	iNewValue, iOldValue;

	new	szAdminName[32];
	get_user_name( id, szAdminName,	31 );

	if ( equali( szNewValue, "+", 1	) )
		bAdd = true;

	iNewValue =	str_to_num(	szNewValue );

	// war3x_setxp

	if ( equal(	szCommand, "war3x_setxp" ) )
	{
		// Get old value

		iOldValue =	g_iXPtotal[target][race];

		if ( iNewValue == iOldValue	&& !bAdd )
		{
			new	szErrorMsg[128];
			formatex( szErrorMsg, 127, "%s %L",	WAR3X_PREFIX, id, "WAR3X_SETXP_WARNING_VALUES" );

			status_print( id, szErrorMsg );

			return PLUGIN_HANDLED;
		}

		// Set message(s)

		if ( bAdd )
		{
			client_print( target, print_chat, "%s %L", WAR3X_PREFIX, id, "WAR3X_SETXP_XP_ADD", szAdminName,	iNewValue, szRaceNameShort );
			iNewValue =	iOldValue +	iNewValue;
		}

		else if	( iNewValue	< 0	)
		{
			client_print( target, print_chat, "%s %L", WAR3X_PREFIX, id, "WAR3X_SETXP_XP_REMOVE", szAdminName, iNewValue * -1, szRaceNameShort );
			iNewValue =	iOldValue +	iNewValue;
		}

		else
		{
			client_print( target, print_chat, "%s %L", WAR3X_PREFIX, id, "WAR3X_SETXP_XP_CHANGE", szAdminName, szRaceNameShort,	iNewValue );
		}
	}

	// war3x_setlevel

	else if	( equal( szCommand,	"war3x_setlevel" ) )
	{
		// Get old value

		iOldValue =	WAR3_get_level(	g_iXPtotal[target][race] );

		if ( iNewValue == iOldValue	&& !bAdd )
		{
			new	szErrorMsg[128];
			formatex( szErrorMsg, 127, "%s %L",	WAR3X_PREFIX, id, "WAR3X_SETXP_WARNING_VALUES" );

			status_print( id, szErrorMsg );

			return PLUGIN_HANDLED;
		}

		// Set message(s)

		if ( bAdd )
		{
			client_print( target, print_chat, "%s %L", WAR3X_PREFIX, id, "WAR3X_SETXP_LEVEL_ADD", szAdminName, iNewValue, szRaceNameShort );
			iNewValue =	iOldValue +	iNewValue;
		}

		else if	( iNewValue	< 0	)
		{
			client_print( target, print_chat, "%s %L", WAR3X_PREFIX, id, "WAR3X_SETXP_LEVEL_REMOVE", szAdminName, iNewValue	* -1, szRaceNameShort );
			iNewValue =	iOldValue +	iNewValue;
		}

		else
		{
			client_print( target, print_chat, "%s %L", WAR3X_PREFIX, id, "WAR3X_SETXP_LEVEL_CHANGE", szAdminName, szRaceNameShort, iNewValue );
		}

		// Convert to XP values

		iOldValue =	g_iLevelXp[iOldValue];
		iNewValue =	g_iLevelXp[iNewValue];

	}

	// Error checking

	if ( iNewValue > g_iLevelXp[TOTAL_LEVELS] )
		iNewValue =	g_iLevelXp[TOTAL_LEVELS];

	if ( iNewValue < 0 )
		iNewValue =	0;

	// Change XP

	g_iXPtotal[target][race] = iNewValue;

	// Reset skills/ultimates if current race

	if ( g_PlayerInfo[target][CURRENT_RACE]	== race	+ 1	)
	{
		g_PlayerInfo[target][CURRENT_XP] = iNewValue;
		WAR3_hud_item( target, HUDMESSAGE_FX_FADEIN, 10.0, 0.1,	2.0, 3.0 );

		// Check if	current	level less than	old	level

		if ( WAR3_get_level( iNewValue ) < WAR3_get_level( iOldValue ) )
		{
			// Reset all skills	and	ultimate

			Reset_Skills( target );
			Reset_Ultimate(	target );

			// Show	skill selection	menu

			if ( WAR3_get_level( iNewValue ) )
				menu_SelectSkills( target );
		}

		else if	( WAR3_get_level( iNewValue	) )
		{
			WAR3_check_level( target, iOldValue, iNewValue );
			WAR3_check_skills( target );
		}
	}

	if ( g_bSaveXp )
		g_bStoreXp[target] = true;

	// Log usage

	if ( equal(	szCommand, "war3x_setxp" ) )
		WAR3_log_setxp(	id,	"war3x_setxp", szPlayerName, szRaceNameShort, szNewValue );

	else if	( equal( szCommand,	"war3x_setlevel" ) )
	{
		WAR3_log_setxp(	id,	"war3x_setlevel", szPlayerName,	szRaceNameShort, szNewValue	);
	}

	// Echo	to player that issued command

	new	szConfirm[256];
	formatex( szConfirm, 255, "%s %L", WAR3X_PREFIX, id, "WAR3X_SETXP_CONFIRM",	szPlayerName, szRaceNameShort, iOldValue, iNewValue	);

	status_print( id, szConfirm	);

	return PLUGIN_HANDLED;
}

// ------------------------------------------------- End. -	//
