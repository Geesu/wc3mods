/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Chameleon Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

public CHAM_Randomize()
{
	if ( get_pcvar_num( CVAR_wc3_races ) == 9 && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		new i, iNewSkill, iType;

		// Lets loop through and find some skills!
		for ( i = 0; i < 5; i++ )
		{
			iNewSkill = random_num( 0, MAX_SKILLS - 1 );
			
			// Trainable Skills
			if ( i < 3 )
			{
				iType = SKILL_TYPE_TRAINABLE;
			}

			// Ultimate
			else if ( i == 3 )
			{
				iType = SKILL_TYPE_ULTIMATE;
			}

			// Passive Ability
			else if ( i == 4 )
			{
				iType = SKILL_TYPE_PASSIVE;
			}
			
			// Loop until we find the right type of skill and while the skill exists ( we don't want a duplicate skill!! )
			while ( g_SkillType[iNewSkill] != iType || CHAM_SkillExists( iNewSkill ) )
			{
				iNewSkill = random_num( 0, MAX_SKILLS - 1 );
			}

			g_ChamSkills[i] = iNewSkill;
		}

		// Do we need to set-up the skills for certain players?
		new iPlayers[32], iNumPlayers, id;
		get_players( iPlayers, iNumPlayers );

		for ( i = 0; i < iNumPlayers; i++ )
		{
			id = iPlayers[i];

			if ( p_data[id][P_RACE] == RACE_CHAMELEON )
			{
				
				// Reset everything
				SM_ResetSkillLevels( id );
				SM_ResetSkills( id );

				// Set up the player's race again
				SM_SetPlayerRace( id, RACE_CHAMELEON );

				// Show the skill info
				WC3_ShowRaceInfo( id );
			}
		}
	} 
}

CHAM_SkillExists( skill_id )
{
	new i;

	for ( i = 0; i < 5; i++ )
	{
		if ( g_ChamSkills[i] == skill_id )
		{
			return true;
		}
	}

	return false;
}

CHAM_Configure()
{

	// Configure the Chameleon Race
	if ( !get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		g_ChamSkills[0] = get_pcvar_num( CVAR_wc3_cham_skill1 );
		g_ChamSkills[1] = get_pcvar_num( CVAR_wc3_cham_skill2 );
		g_ChamSkills[2] = get_pcvar_num( CVAR_wc3_cham_skill3 );
		g_ChamSkills[3] = get_pcvar_num( CVAR_wc3_cham_ultimate );
		g_ChamSkills[4] = get_pcvar_num( CVAR_wc3_cham_passive );

		// Lets do some error checking b/c I know some people will fuck this up...
		
		new i, bool:bError = false;
		for ( i = 0; i < 3; i++ )
		{
			if ( !CHAM_ValidSkill( g_ChamSkills[i], SKILL_TYPE_TRAINABLE ) )
			{
				log_amx( "Invalid trainable skill for cvar wc3_cham_skill%d: %d", i+1, g_ChamSkills[i] );
				bError = true;
			}
		}
		
		// Check Ultimate
		if ( !CHAM_ValidSkill( g_ChamSkills[3], SKILL_TYPE_ULTIMATE ) )
		{
			log_amx( "Invalid ultimate skill for cvar wc3_cham_ultimate: %d", g_ChamSkills[3] );
			bError = true;
		}

		// Check Passive
		if ( !CHAM_ValidSkill( g_ChamSkills[4], SKILL_TYPE_PASSIVE ) )
		{
			log_amx( "Invalid passive skill for cvar wc3_cham_passive: %d", g_ChamSkills[4] );
			bError = true;
		}

		if ( bError )
		{
			set_pcvar_num( CVAR_wc3_cham_random, 1 )
		}
	}
}

CHAM_ValidSkill( skill_id, iType )
{
	if ( skill_id < 0 || skill_id >= MAX_SKILLS )
	{
		return false;
	}

	if ( g_SkillType[skill_id] != iType )
	{
		return false;
	}

	return true;
}