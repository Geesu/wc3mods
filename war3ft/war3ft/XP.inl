/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	XP Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define XP_NEARBY_RADIUS		250

// START FROM CSX.INC - This is a hack b/c we can't include CSX and DODX in the same plugin :/
forward bomb_planting(planter);
forward bomb_planted(planter);
forward bomb_defusing(defuser);
forward bomb_defused(defuser);
// END FROM CSX.INC

// Called when a user begins defusing the bomb
public bomb_defusing( defuser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	// Make sure that this isn't called more than once per round
	if ( !bHasBegunPlantingOrDefusing[defuser] )
	{
		new iBonusXP = XP_Give( defuser, DEFUSING_BOMB );

		if ( iBonusXP != 0 )
		{
			client_print( defuser, print_chat, "%s You have been awarded %d XP for starting to defuse the bomb", g_MODclient, iBonusXP );
		}
	}

	bHasBegunPlantingOrDefusing[defuser] = true;
}

// Called when a user defuses the bomb
public bomb_defused( defuser )
{
	if ( !WC3_Check() )
	{
		return;
	}

	new iBonusXP = XP_Give( defuser, DEFUSED_BOMB );

	if ( iBonusXP != 0 )
	{
		client_print( defuser, print_chat, "%s You have been awarded %d XP for defusing the bomb", g_MODclient, iBonusXP );
	}

	g_iPlayerRole[defuser] = 0;
}

// Called when a user begins planting the bomb
public bomb_planting( planter )
{
	if ( !WC3_Check() )
	{
		return;
	}

	// Make sure that this isn't called more than once per round
	if ( !bHasBegunPlantingOrDefusing[planter] )
	{
		new iBonusXP = XP_Give( planter, PLANTING_BOMB );

		if ( iBonusXP != 0 )
		{
			client_print( planter, print_chat, "%s You have been awarded %d XP for starting to plant the bomb", g_MODclient, iBonusXP );
		}
	}

	g_iPlayerRole[planter] = PLR_BOMB_PLANTER;
	bHasBegunPlantingOrDefusing[planter] = true;
}

// Called when a user has planted the bomb
public bomb_planted( planter )
{
	if ( !WC3_Check() )
	{
		return;
	}

	new iBonusXP = XP_Give( planter, PLANT_BOMB );

	if ( iBonusXP != 0 )
	{
		client_print( planter, print_chat, "%s You have been awarded %d XP for planting the bomb", g_MODclient, iBonusXP );
	}

	g_iPlayerRole[planter] = 0;
	p_data_b[planter][PB_ISPLANTING] = false;

	// Serpent Ward Check
	SH_BombPlanted();
}

// Called on certain player events
public on_PlayerAction()
{
	if ( !WC3_Check() )
	{
		return;
	}

	new sArg[64], sAction[64], sName[64];
	new iUserId, id;

	read_logargv( 0,sArg,64 );
	read_logargv( 2,sAction,64 );
	parse_loguser( sArg,sName,64,iUserId );
	id = find_player( "k", iUserId );

	if ( id == 0 )
	{
		return;
	}

	// If the round is over do not award XP!
	if ( g_EndRound )
	{
		return;
	}

	// Spawned with the bomb 
	if (equal(sAction,"Spawned_With_The_Bomb"))
	{
		g_iPlayerRole[id] = PLR_BOMB_CARRIER;
		new iBonusXP = XP_Give( id, SPAWN_BOMB );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			client_print( id, print_chat, "%s You have been awarded %d XP for spawning with the bomb", g_MODclient, iBonusXP );
		}
	}
	// Dropped bomb 
	else if (equal(sAction,"Dropped_The_Bomb"))
	{
		g_iPlayerRole[id] = 0;
		new iBonusXP = XP_Give( id, -1 * BOMB_PICKUP );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			client_print( id, print_chat, "%s You have lost %d XP for dropping the bomb", g_MODclient, -1 * iBonusXP );
		}
	}
	// Picked up bomb 
	else if (equal(sAction,"Got_The_Bomb"))
	{ 
		g_iPlayerRole[id] = PLR_BOMB_CARRIER;
		new iBonusXP = XP_Give( id, BOMB_PICKUP );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			client_print( id, print_chat, "%s You have been awarded %d XP for picking up the bomb", g_MODclient, iBonusXP );
		}
	}
	// Hostage touched 
	else if (equal(sAction,"Touched_A_Hostage"))
	{
		g_iPlayerRole[id] = PLR_HOSTAGE_RESCUER;
		new iBonusXP = XP_Give( id, TOUCH_HOSTAGE );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			client_print( id, print_chat, "%s You have been awarded %d XP for starting to rescue a hostage", g_MODclient, iBonusXP );
		}
	}
	// Hostage rescued 
	else if (equal(sAction,"Rescued_A_Hostage"))
	{
		new iBonusXP = XP_Give( id, RESCUE_HOSTAGE );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			client_print( id, print_chat, "%s You have been awarded %d XP for rescuing a hostage", g_MODclient, iBonusXP );
		}
	}
	// Hostage killed 
	else if (equal(sAction,"Killed_A_Hostage"))
	{
		new iBonusXP = XP_Give( id, -1 * KILL_HOSTAGE );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			client_print( id, print_chat, "%s You have lost %d XP for killing a hostage", g_MODclient, -1 * iBonusXP );
		}
	}
	// VIP spawn 
	else if (equal(sAction,"Became_VIP"))
	{ 
		g_iPlayerRole[id] = PLR_VIP;
		new iBonusXP = XP_Give( id, SPAWN_VIP );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			client_print( id, print_chat, "%s You have gained %d XP for spawning as the VIP", g_MODclient, iBonusXP );
		}
	}
	// VIP escaped 
	else if (equal(sAction,"Escaped_As_VIP"))
	{
		new iBonusXP = XP_Give( id, ESCAPE_VIP );

		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
		{
			client_print( id, print_chat, "%s You have gained %d XP for escaping as the VIP", g_MODclient, iBonusXP );
		}
	}
}

XP_onDeath( iVictim, iAttacker, iWeaponIndex, iHeadshot )
{

	// We don't want to give XP to the world
	if ( iAttacker == 0 )
	{
		return;
	}

	// We don't want to give XP to suiciders
	else if ( iAttacker == iVictim )
	{
		return;
	}
	
	new iLevel = p_data[iVictim][P_LEVEL]
	new iBonusXP, iXP = floatround( XP_GivenByLevel( iLevel ) * fWpnXPMultiplier[iWeaponIndex] );

	// Check for a team kill
	if ( get_user_team( iAttacker ) == get_user_team( iVictim ) )
	{
		// Remove XP since he killed his teammate
		iBonusXP = XP_Give( iAttacker, -1 * iXP );

		// This message should be displayed no matter what XP_kill_objectives is, b/c it's a no-no
		if ( iBonusXP != 0 )
		{
			client_print( iAttacker, print_chat, "%s You have lost %d XP for killing a teammate", g_MODclient, -1 * iBonusXP );
		}

	}

	// Otherwise the player killed the other team
	else
	{
		// Award XP for a kill
		iBonusXP = XP_Give( iAttacker, iXP );
		
		if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
		{
			client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the enemy", g_MODclient, iBonusXP );
		}

		// User had a headshot?  Give bonus XP!
		if ( iHeadshot )
		{
			iBonusXP = XP_Give( iAttacker, KILL_HEADSHOT );
			
			if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
			{
				client_print( iAttacker, print_chat, "%s You have been awarded %d XP for getting a headshot", g_MODclient, iBonusXP );
			}
		}

		new iAssistLevel, iAssistXP, iVictimMaxHealth;
		new Float:fMultiplier;

		// Award XP for other people doing damage to this victim
		for ( new i = 0; i < MAXPLAYERS; i++ )
		{
			// Then this player dealt some damage to this player this round
			if ( g_iDamageDealt[i][iVictim] > 0 && iAttacker != i )
			{
				iVictimMaxHealth = get_user_maxhealth( iVictim );
				fMultiplier = float( g_iDamageDealt[i][iVictim] ) / float( iVictimMaxHealth );
				
				iAssistLevel = p_data[i][P_LEVEL];
				iAssistXP = XP_GivenByLevel( iAssistLevel );

				// Need a ratio of XP to award to person who dealt damage
				iBonusXP = XP_Give( i, ( floatround( float( iAssistXP ) * fMultiplier ) / 2 ) );

				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					client_print( i, print_chat, "%s You have been awarded %d XP for a kill assist!", g_MODclient, iBonusXP );
				}
			}
		}

		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			
			// User killed a hostage rescuer
			if ( g_iPlayerRole[iVictim] == PLR_HOSTAGE_RESCUER )
			{
				iBonusXP = XP_Give( iAttacker, KILL_HOSTAGE_SAVER );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the hostage rescuer", g_MODclient, iBonusXP );
				}
			}
			// User killed the bomb defuser
			else if ( g_iPlayerRole[iVictim] == PLR_BOMB_DEFUSER )
			{
				iBonusXP = XP_Give( iAttacker, KILL_DEFUSER );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the bomb defuser", g_MODclient, iBonusXP );
				}
			}
			// User killed the bomb planter
			else if ( g_iPlayerRole[iVictim] == PLR_BOMB_PLANTER )
			{
				iBonusXP = XP_Give( iAttacker, KILL_PLANTER );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the bomb planter", g_MODclient, iBonusXP );
				}
			}
			// User killed the bomb carrier
			else if ( g_iPlayerRole[iVictim] == PLR_BOMB_CARRIER )
			{
				iBonusXP = XP_Give( iAttacker, KILL_BOMB_CARRIER );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_kill_obj ) )
				{
					client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the bomb carrier", g_MODclient, iBonusXP );
				}
			}
			// user killed the VIP
			else if ( g_iPlayerRole[iVictim] == PLR_VIP )
			{
				iBonusXP = XP_Give( iAttacker, KILL_VIP );
				
				if ( iBonusXP != 0 && get_pcvar_num( CVAR_wc3_show_objectives ) )
				{
					client_print( iAttacker, print_chat, "%s You have been awarded %d XP for killing the VIP", g_MODclient, iBonusXP );
				}
			}
		}
		

		else if ( g_MOD == GAME_DOD )
		{


		}

		// Player died, so lets reset their data
		g_iPlayerRole[iVictim] = 0;
	}
}


XP_WinRound( iWinner )
{
	
	new players[32], numberofplayers, i, id;
	get_players( players, numberofplayers );

	for ( i = 0; i < numberofplayers; i++ )
	{
		id = players[i];

		// Is the user on the correct team?
		if ( get_user_team( id ) == iWinner )
		{

			new iBonusXP = XP_Give( id, WIN_ROUND );

			if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
			{				
				client_print( id, print_chat, "%s %L", g_MODclient, id, "AWARD_FOR_WINNING_ROUND", iBonusXP );
			}
		}
	}
}

XP_GetByLevel( iLevel )
{
	if ( iLevel < 0 || iLevel > 10 )
	{
		return 0;
	}

	// Change based on XP multiplier!
	new Float:fXPMult = get_pcvar_float( CVAR_wc3_xp_multiplier );

	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return floatround( iXPLevelSaved[iLevel] * fXPMult );
	}

	return floatround( iXPLevelShortTerm[iLevel] * fXPMult );
}


XP_GivenByLevel( iLevel )
{
	if ( iLevel < 0 || iLevel > 10 )
	{
		return 0;
	}

	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return iXPGivenSaved[iLevel];
	}

	return iXPGivenShortTerm[iLevel];
}

bool:XP_MinPlayers()
{
	new iNum;

	// Don't ignore bots
	if ( get_pcvar_num( CVAR_wc3_ignore_bots ) == 0 )
	{
		iNum = get_playersnum();
	}

	// Ignore bots
	else
	{
		new players[32];
		get_players( players, iNum, "ch" ); 
	}

	if ( iNum < get_pcvar_num( CVAR_wc3_min_players ) )
	{
		return false;
	}

	return true;
}

// Function will verify if the user's level/information is correct and checks to see if the user has gained a level
XP_Check( id, bShowGained = true )
{
	new iOldLevel = p_data[id][P_LEVEL], i;
	
	// Make sure the user doesn't have negative experience
	if ( p_data[id][P_XP] < 0 )
	{
		p_data[id][P_XP] = 0;
	}

	// Determine what the user's level should be
	for ( i = 0; i <= MAX_LEVELS; i++ )
	{
		// User has enough XP to advance to the next level
		if ( p_data[id][P_XP] >= XP_GetByLevel( i ) )
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
		WC3_StatusText( id, TXT_TOP_CENTER, "%L", id, "YOU_GAINED_A_LEVEL" );
		
		// Only play the sound if the user is alived!!
		if ( is_user_alive( id ) )
		{
			emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_LEVELUP], 1.0, ATTN_NORM, 0, PITCH_NORM );
		}
	}

	// We might need to lower the skills the user has ( can occur if you load XP info from a database and the XP multiplier has changed )
	new iSkillsUsed = SM_TotalSkillPointsUsed( id );
	new iSkillID, iSkillLevel;
	new bool:bSkillRemoved = false;

	while ( iSkillsUsed > p_data[id][P_LEVEL] )
	{
		// We might need to remove the ult first!
		if ( p_data[id][P_LEVEL] < MIN_ULT_LEVEL )
		{
			new iUltimateID = SM_GetSkillOfType( id, SKILL_TYPE_ULTIMATE );
			
			// Then we need to remove this!
			if ( SM_GetSkillLevel( id, iUltimateID, 0 ) == 1 )
			{
				// Remove the ult
				SM_SetSkillLevel( id, iUltimateID, 0, 0 );

				bSkillRemoved = true;

				// Clear ult icon!
				ULT_ClearIcons( id );

				// Get our current skills used
				iSkillsUsed = SM_TotalSkillPointsUsed( id );

				// Restart the loop!
				continue;
			}
		}

		iSkillID = SM_GetRandomSkill( id );
		iSkillLevel = SM_GetSkillLevel( id, iSkillID, 1 );

		// We don't want a passive skill or a skill that is level 0 so keep trying
		while ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_PASSIVE || iSkillLevel == 0 )
		{
			iSkillLevel = SM_GetSkillLevel( id, iSkillID, 2 );
			iSkillID = SM_GetRandomSkill( id );
		}
		
		// OK at this point we have a valid skill, lets remove a level!
		SM_SetSkillLevel( id, iSkillID, iSkillLevel - 1, 1 );

		bSkillRemoved = true;

		// Get our current skills used
		iSkillsUsed = SM_TotalSkillPointsUsed( id );
	}
	
	// Need to reset all skill data for the given race if we've removed a skill
	if ( bSkillRemoved )
	{
		WC3_InitPlayerSkills( id );
	}

	// OK lets check the total skills the user has, and maybe show them the selectskills menu
	new iTotalSkillsUsed = SM_TotalSkillPointsUsed( id );

	if ( iTotalSkillsUsed < p_data[id][P_LEVEL] )
	{
		MENU_SelectSkill( id );
	}

	WC3_ShowBar( id );
}

XP_Configure()
{

	// Configure based on weapon multiplier

	// Counter-Strike and Condition Zero weapons
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		fWpnXPMultiplier[CSW_USP			]	= 2.5;
		fWpnXPMultiplier[CSW_DEAGLE			]	= 2.5;
		fWpnXPMultiplier[CSW_GLOCK18		]	= 2.5;
		fWpnXPMultiplier[CSW_ELITE			]	= 3.5;
		fWpnXPMultiplier[CSW_P228			]	= 2.5;
		fWpnXPMultiplier[CSW_FIVESEVEN		]	= 2.5;

		fWpnXPMultiplier[CSW_XM1014			]	= 1.25;
		fWpnXPMultiplier[CSW_M3				]	= 1.5;

		fWpnXPMultiplier[CSW_MP5NAVY		]	= 1.0;
		fWpnXPMultiplier[CSW_UMP45			]	= 1.25;
		fWpnXPMultiplier[CSW_P90			]	= 1.25;
		fWpnXPMultiplier[CSW_TMP			]	= 1.5;
		fWpnXPMultiplier[CSW_MAC10			]	= 1.5;
		fWpnXPMultiplier[CSW_GALIL			]	= 1.15;
		fWpnXPMultiplier[CSW_FAMAS			]	= 1.15;

		fWpnXPMultiplier[CSW_AWP			]	= 1.0;
		fWpnXPMultiplier[CSW_M4A1			]	= 1.0;
		fWpnXPMultiplier[CSW_AK47			]	= 1.0;
		fWpnXPMultiplier[CSW_AUG			]	= 1.0;
		fWpnXPMultiplier[CSW_SG552			]	= 1.0;
		fWpnXPMultiplier[CSW_G3SG1			]	= 1.0;
		fWpnXPMultiplier[CSW_SG550			]	= 1.0;
		fWpnXPMultiplier[CSW_M249			]	= 1.25;
		fWpnXPMultiplier[CSW_SCOUT			]	= 3.0;

		fWpnXPMultiplier[CSW_HEGRENADE		]	= 1.75;
		fWpnXPMultiplier[CSW_KNIFE			]	= 6.0;

		fWpnXPMultiplier[CSW_C4				]	= 2.0;
		fWpnXPMultiplier[CSW_SMOKEGRENADE	]	= 1.0;
		fWpnXPMultiplier[CSW_FLASHBANG		]	= 1.0;
	}

	// Day of Defeat weapons
	else if ( g_MOD == GAME_DOD )
	{
		fWpnXPMultiplier[DODW_AMERKNIFE			] = 6.0;
		fWpnXPMultiplier[DODW_GERKNIFE			] = 6.0;
		fWpnXPMultiplier[DODW_BRITKNIFE			] = 6.0;
		fWpnXPMultiplier[DODW_GARAND_BUTT		] = 6.0;

		fWpnXPMultiplier[DODW_HANDGRENADE		] = 1.75;
		fWpnXPMultiplier[DODW_STICKGRENADE		] = 1.75;
		fWpnXPMultiplier[DODW_STICKGRENADE_EX	] = 1.75;
		fWpnXPMultiplier[DODW_HANDGRENADE_EX	] = 1.75;

		fWpnXPMultiplier[DODW_COLT				] = 1.0;
		fWpnXPMultiplier[DODW_LUGER				] = 1.0;
		fWpnXPMultiplier[DODW_GARAND			] = 1.0;
		fWpnXPMultiplier[DODW_SCOPED_KAR		] = 1.0;
		fWpnXPMultiplier[DODW_THOMPSON			] = 1.0;
		fWpnXPMultiplier[DODW_STG44				] = 1.0;
		fWpnXPMultiplier[DODW_SPRINGFIELD		] = 1.0;
		fWpnXPMultiplier[DODW_KAR				] = 1.0;
		fWpnXPMultiplier[DODW_BAR				] = 1.0;
		fWpnXPMultiplier[DODW_MP40				] = 1.0;
		fWpnXPMultiplier[DODW_MG42				] = 1.0;
		fWpnXPMultiplier[DODW_30_CAL			] = 1.0;
		fWpnXPMultiplier[DODW_SPADE				] = 1.0;
		fWpnXPMultiplier[DODW_M1_CARBINE		] = 1.0;
		fWpnXPMultiplier[DODW_MG34				] = 1.0;
		fWpnXPMultiplier[DODW_GREASEGUN			] = 1.0;
		fWpnXPMultiplier[DODW_FG42				] = 1.0;
		fWpnXPMultiplier[DODW_K43				] = 1.0;
		fWpnXPMultiplier[DODW_ENFIELD			] = 1.0;
		fWpnXPMultiplier[DODW_STEN				] = 1.0;
		fWpnXPMultiplier[DODW_BREN				] = 1.0;
		fWpnXPMultiplier[DODW_WEBLEY			] = 1.0;
		fWpnXPMultiplier[DODW_BAZOOKA			] = 1.0;
		fWpnXPMultiplier[DODW_PANZERSCHRECK		] = 1.0;
		fWpnXPMultiplier[DODW_PIAT				] = 1.0;
		fWpnXPMultiplier[DODW_SCOPED_FG42		] = 1.0;
		fWpnXPMultiplier[DODW_FOLDING_CARBINE	] = 1.0;
		fWpnXPMultiplier[DODW_KAR_BAYONET		] = 1.0;
		fWpnXPMultiplier[DODW_SCOPED_ENFIELD	] = 1.0;
		fWpnXPMultiplier[DODW_MILLS_BOMB		] = 1.0;
		fWpnXPMultiplier[DODW_ENFIELD_BAYONET	] = 1.0;
		fWpnXPMultiplier[DODW_MORTAR			] = 1.0;
		fWpnXPMultiplier[DODW_K43_BUTT			] = 1.0;
	}

	fWpnXPMultiplier[CSW_WORLDSPAWN		]	= 0.0;
	fWpnXPMultiplier[CSW_LIGHTNING		]	= 1.0;
	fWpnXPMultiplier[CSW_SUICIDE		]	= 1.0;
	fWpnXPMultiplier[CSW_IMMOLATE		]	= 1.0;
	fWpnXPMultiplier[CSW_LOCUSTS		]	= 1.0;
	fWpnXPMultiplier[CSW_SERPENTWARD	]	= 1.0;
	fWpnXPMultiplier[CSW_SHADOW			]	= 1.0;	
	fWpnXPMultiplier[CSW_THORNS			]	= 1.0;
	fWpnXPMultiplier[CSW_CARAPACE		]	= 1.0;
	fWpnXPMultiplier[CSW_CARRION		]	= 1.0;
	fWpnXPMultiplier[CSW_ORB			]	= 1.0;
	fWpnXPMultiplier[CSW_CONCOCTION		]	= 1.0;
}

// Reset the user's XP to 0
public XP_Reset(id)
{

	p_data[id][P_LEVEL]		= 0;
	p_data[id][P_XP]		= 0;
	SM_ResetSkillLevels( id );

	// Save after resetting the user's XP
	DB_SaveXP( id, true );

	WC3_ShowBar( id );

	client_print( id, print_chat, "%s %L", g_MODclient, id, "YOUR_XP_HAS_BEEN_RESET" );

	// Reset the skills...

	SHARED_INVIS_Set( id );

	// Undead's Unholy Aura
	SHARED_SetGravity( id );

	// Set the user's speed
	SHARED_SetSpeed( id );

	return PLUGIN_CONTINUE;
}	

// Function from war3x thanks ryan!!!
public XP_GetAdminFlag()
{

    new szFlags[24];
    get_pcvar_string( CVAR_wc3_admin_flag, szFlags, 23 );

    return ( read_flags( szFlags ) );
}

stock XP_Give( id, iBonusXP )
{

	if ( !WC3_Check() || !id )
	{
		return 0;
	}

	// Make sure we have the minimum amount of players
	if ( !XP_MinPlayers() )
	{
		return 0;
	}

	// Bonus calculated by:
	// Bonus XP * (lvl of player/10 + 1.0)
	// I.E. if Player is level 10, then it will be Bonus XP * 2.0
	
	if ( iBonusXP != 0 )
	{
		new Float:fCurrentLevel = float( p_data[id][P_LEVEL] );
		new Float:iLevelMultiplier = ( fCurrentLevel / 10.0 ) + 1.0;
		new iRealBonusXP = floatround(iLevelMultiplier * iBonusXP);
		
		p_data[id][P_XP] += iRealBonusXP;

		XP_Check( id );

		return iRealBonusXP;
	}

	return 0;
}

