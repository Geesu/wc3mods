SM_Init()
{
	// Set up the Skill Owners
	g_SkillOwner[SKILL_VAMPIRICAURA			]	= RACE_UNDEAD;
	g_SkillOwner[SKILL_UNHOLYAURA			]	= RACE_UNDEAD;
	g_SkillOwner[SKILL_LEVITATION			]	= RACE_UNDEAD;
	g_SkillOwner[ULTIMATE_SUICIDE			]	= RACE_UNDEAD;

	g_SkillOwner[SKILL_INVISIBILITY			]	= RACE_HUMAN;
	g_SkillOwner[SKILL_DEVOTION				]	= RACE_HUMAN;
	g_SkillOwner[SKILL_BASH					]	= RACE_HUMAN;
	g_SkillOwner[ULTIMATE_BLINK				]	= RACE_HUMAN;

	g_SkillOwner[SKILL_CRITICALSTRIKE		]	= RACE_ORC;
	g_SkillOwner[SKILL_CRITICALGRENADE		]	= RACE_ORC;
	g_SkillOwner[SKILL_REINCARNATION		]	= RACE_ORC;
	g_SkillOwner[ULTIMATE_CHAINLIGHTNING	]	= RACE_ORC;

	g_SkillOwner[SKILL_EVASION				]	= RACE_ELF;
	g_SkillOwner[SKILL_THORNS				]	= RACE_ELF;
	g_SkillOwner[SKILL_TRUESHOT				]	= RACE_ELF;
	g_SkillOwner[ULTIMATE_ENTANGLE			]	= RACE_ELF;

	g_SkillOwner[SKILL_PHOENIX				]	= RACE_BLOOD;
	g_SkillOwner[SKILL_BANISH				]	= RACE_BLOOD;
	g_SkillOwner[SKILL_SIPHONMANA			]	= RACE_BLOOD;
	g_SkillOwner[ULTIMATE_IMMOLATE			]	= RACE_BLOOD;
	g_SkillOwner[PASS_RESISTANTSKIN			]	= RACE_BLOOD;

	g_SkillOwner[SKILL_HEALINGWAVE			]	= RACE_SHADOW;
	g_SkillOwner[SKILL_HEX					]	= RACE_SHADOW;
	g_SkillOwner[SKILL_SERPENTWARD			]	= RACE_SHADOW;
	g_SkillOwner[ULTIMATE_BIGBADVOODOO		]	= RACE_SHADOW;
	g_SkillOwner[PASS_UNSTABLECONCOCTION	]	= RACE_SHADOW;

	g_SkillOwner[SKILL_FANOFKNIVES			]	= RACE_WARDEN;
	g_SkillOwner[SKILL_BLINK				]	= RACE_WARDEN;
	g_SkillOwner[SKILL_SHADOWSTRIKE			]	= RACE_WARDEN;
	g_SkillOwner[ULTIMATE_VENGEANCE			]	= RACE_WARDEN;
	g_SkillOwner[PASS_HARDENEDSKIN			]	= RACE_WARDEN;

	g_SkillOwner[SKILL_IMPALE				]	= RACE_CRYPT;
	g_SkillOwner[SKILL_SPIKEDCARAPACE		]	= RACE_CRYPT;
	g_SkillOwner[SKILL_CARRIONBEETLES		]	= RACE_CRYPT;
	g_SkillOwner[ULTIMATE_LOCUSTSWARM		]	= RACE_CRYPT;
	g_SkillOwner[PASS_ORB					]	= RACE_CRYPT;


	// Set up the skill types
	g_SkillType[SKILL_VAMPIRICAURA			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_UNHOLYAURA			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_LEVITATION			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[ULTIMATE_SUICIDE			]	= SKILL_TYPE_ULTIMATE;

	g_SkillType[SKILL_INVISIBILITY			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_DEVOTION				]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_BASH					]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[ULTIMATE_BLINK				]	= SKILL_TYPE_ULTIMATE;

	g_SkillType[SKILL_CRITICALSTRIKE		]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_CRITICALGRENADE		]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_REINCARNATION			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[ULTIMATE_CHAINLIGHTNING		]	= SKILL_TYPE_ULTIMATE;

	g_SkillType[SKILL_EVASION				]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_THORNS				]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_TRUESHOT				]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[ULTIMATE_ENTANGLE			]	= SKILL_TYPE_ULTIMATE;

	g_SkillType[SKILL_PHOENIX				]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_BANISH				]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_SIPHONMANA			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[ULTIMATE_IMMOLATE			]	= SKILL_TYPE_ULTIMATE;
	g_SkillType[PASS_RESISTANTSKIN			]	= SKILL_TYPE_PASSIVE;

	g_SkillType[SKILL_HEALINGWAVE			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_HEX					]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_SERPENTWARD			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[ULTIMATE_BIGBADVOODOO		]	= SKILL_TYPE_ULTIMATE;
	g_SkillType[PASS_UNSTABLECONCOCTION		]	= SKILL_TYPE_PASSIVE;

	g_SkillType[SKILL_FANOFKNIVES			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_BLINK					]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_SHADOWSTRIKE			]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[ULTIMATE_VENGEANCE			]	= SKILL_TYPE_ULTIMATE;
	g_SkillType[PASS_HARDENEDSKIN			]	= SKILL_TYPE_PASSIVE;

	g_SkillType[SKILL_IMPALE				]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_SPIKEDCARAPACE		]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[SKILL_CARRIONBEETLES		]	= SKILL_TYPE_TRAINABLE;
	g_SkillType[ULTIMATE_LOCUSTSWARM		]	= SKILL_TYPE_ULTIMATE;
	g_SkillType[PASS_ORB					]	= SKILL_TYPE_PASSIVE;


	// Set up the skill order
	g_SkillOrder[SKILL_VAMPIRICAURA			]	= SKILL_POS_1;
	g_SkillOrder[SKILL_UNHOLYAURA			]	= SKILL_POS_2;
	g_SkillOrder[SKILL_LEVITATION			]	= SKILL_POS_3;
	g_SkillOrder[ULTIMATE_SUICIDE			]	= SKILL_POS_4;

	g_SkillOrder[SKILL_INVISIBILITY			]	= SKILL_POS_1;
	g_SkillOrder[SKILL_DEVOTION				]	= SKILL_POS_2;
	g_SkillOrder[SKILL_BASH					]	= SKILL_POS_3;
	g_SkillOrder[ULTIMATE_BLINK				]	= SKILL_POS_4;

	g_SkillOrder[SKILL_CRITICALSTRIKE		]	= SKILL_POS_1;
	g_SkillOrder[SKILL_CRITICALGRENADE		]	= SKILL_POS_2;
	g_SkillOrder[SKILL_REINCARNATION		]	= SKILL_POS_3;
	g_SkillOrder[ULTIMATE_CHAINLIGHTNING	]	= SKILL_POS_4;

	g_SkillOrder[SKILL_EVASION				]	= SKILL_POS_1;
	g_SkillOrder[SKILL_THORNS				]	= SKILL_POS_2;
	g_SkillOrder[SKILL_TRUESHOT				]	= SKILL_POS_3;
	g_SkillOrder[ULTIMATE_ENTANGLE			]	= SKILL_POS_4;

	g_SkillOrder[SKILL_PHOENIX				]	= SKILL_POS_1;
	g_SkillOrder[SKILL_BANISH				]	= SKILL_POS_2;
	g_SkillOrder[SKILL_SIPHONMANA			]	= SKILL_POS_3;
	g_SkillOrder[ULTIMATE_IMMOLATE			]	= SKILL_POS_4;
	g_SkillOrder[PASS_RESISTANTSKIN			]	= SKILL_POS_NONE;

	g_SkillOrder[SKILL_HEALINGWAVE			]	= SKILL_POS_1;
	g_SkillOrder[SKILL_HEX					]	= SKILL_POS_2;
	g_SkillOrder[SKILL_SERPENTWARD			]	= SKILL_POS_3;
	g_SkillOrder[ULTIMATE_BIGBADVOODOO		]	= SKILL_POS_4;
	g_SkillOrder[PASS_UNSTABLECONCOCTION	]	= SKILL_POS_NONE;

	g_SkillOrder[SKILL_FANOFKNIVES			]	= SKILL_POS_1;
	g_SkillOrder[SKILL_BLINK				]	= SKILL_POS_2;
	g_SkillOrder[SKILL_SHADOWSTRIKE			]	= SKILL_POS_3;
	g_SkillOrder[ULTIMATE_VENGEANCE			]	= SKILL_POS_4;
	g_SkillOrder[PASS_HARDENEDSKIN			]	= SKILL_POS_NONE;

	g_SkillOrder[SKILL_IMPALE				]	= SKILL_POS_1;
	g_SkillOrder[SKILL_SPIKEDCARAPACE		]	= SKILL_POS_2;
	g_SkillOrder[SKILL_CARRIONBEETLES		]	= SKILL_POS_3;
	g_SkillOrder[ULTIMATE_LOCUSTSWARM		]	= SKILL_POS_4;
	g_SkillOrder[PASS_ORB					]	= SKILL_POS_NONE;
}

/***** NEW SKILL SET UP BELOW!! *****/

SM_SetPlayerRace( id, iRace )
{
	static i;

	// Loop through all possible skills to find all valid skills for this race
	for ( i = 0; i < MAX_SKILLS; i++ )
	{

		// Valid skill found, assign it to this player
		if ( g_SkillOwner[i] == iRace )
		{
			g_bPlayerSkills[id][i] = true;
		}
	}
}

// Function will reset all of the user's skill levels
SM_ResetSkillLevels( id )
{
	static i;

	for ( i = 0; i < MAX_SKILLS; i++ )
	{
		g_PlayerSkillLevel[id][i] = 0;
	}
}

// Function will reset what skills the user has
SM_ResetSkills( id )
{
	static i;

	for ( i = 0; i < MAX_SKILLS; i++ )
	{
		g_bPlayerSkills[id][i] = false;
	}
}

// Function will return the total number of skill points used
SM_TotalSkillsUsed( id )
{
	static i, iTotalSkillsUsed;
	iTotalSkillsUsed = 0;

	for ( i = 0; i < MAX_SKILLS; i++ )
	{
		iTotalSkillsUsed += g_PlayerSkillLevel[id][i];
	}

	return iTotalSkillsUsed;
}

// Function will return the skill ID number based on the position (i.e. used after a skill is selected)
SM_GetSkillByPos( id, iPos )
{
	static iCurrentSkill, i;
	iCurrentSkill = 1;

	for ( i = 0; i < MAX_SKILLS; i++ )
	{
		if ( g_bPlayerSkills[id][i] )
		{
			if ( iCurrentSkill == iPos )
			{
				return i;
			}

			iCurrentSkill++;
		}
	}

	// Technically we should NEVER be here
	log_error( AMX_ERR_NONE, "No valid skill found for position %d with race %d", iPos, p_data[id][P_RACE] );

	return -1;
}

// Returns the user's level for a certain skill
SM_GetSkillLevel( id, skill_id )
{
	if ( !SM_IsValidSkill( skill_id ) )
	{
		log_error( AMX_ERR_NONE, "Invalid skill: %d", skill_id );

		return 0;
	}

	// User doesn't have this skill
	if ( !g_bPlayerSkills[id][skill_id] )
	{
		return 0;
	}

	// If it's a passive skill, we'll just return the player's current level
	if ( g_SkillType[skill_id] == SKILL_TYPE_PASSIVE )
	{
		return p_data[id][P_LEVEL];
	}

	return g_PlayerSkillLevel[id][skill_id];
}

// Set the user's skill level for a given skill
SM_SetSkillLevel( id, skill_id, iLevel )
{
	if ( !SM_IsValidSkill( skill_id ) )
	{
		log_amx( "Invalid skill: %d", skill_id );

		return;
	}

	// User doesn't have this skill
	if ( !g_bPlayerSkills[id][skill_id] )
	{
		return;
	}
	
	// We shouldn't be setting a passive skill's level!
	if ( g_SkillType[skill_id] == SKILL_TYPE_PASSIVE )
	{
		log_error( AMX_ERR_NONE, "Setting a passive skill's level %d to %d", skill_id, iLevel );

		return;
	}

	g_PlayerSkillLevel[id][skill_id] = iLevel;

	return;
}


// Checks to see if a skill ID is valid
bool:SM_IsValidSkill( skill_id )
{
	if ( skill_id >= 0 && skill_id < MAX_SKILLS )
	{
		return true;
	}

	return false;
}

// Function will get a random skill for the user's current skills (great for bots)
SM_GetRandomSkill( id )
{
	static iRandomSkill;
	
	// Initial condition selected
	iRandomSkill = random_num( 0, MAX_SKILLS - 1 );

	while ( !g_bPlayerSkills[id][iRandomSkill] )
	{
		iRandomSkill = random_num( 0, MAX_SKILLS - 1 );
	}

	return iRandomSkill;
}

// Function will simply return the skill type
SM_GetSkillType( skill_id )
{
	if ( !SM_IsValidSkill( skill_id ) )
	{
		log_amx( "Invalid skill: %d", skill_id );

		return 0;
	}

	return g_SkillType[skill_id];
}

// Returns the total selectable skills that this user currently has
SM_TotalSelectableSkills( id )
{
	static i, iTotal;
	iTotal = 0;

	for ( i = 0; i < MAX_SKILLS; i++ )
	{
		if ( g_SkillType[i] == SKILL_TYPE_TRAINABLE || g_SkillType[i] == SKILL_TYPE_ULTIMATE )
		{
			if ( g_bPlayerSkills[id][i] )
			{
				iTotal++;
			}
		}
	}

	return iTotal;
}

// Function will return a skill of a certain type
SM_GetSkillOfType( id, type, iStart = 0 )
{
	static i;

	for ( i = iStart; i < MAX_SKILLS; i++ )
	{
		if ( g_bPlayerSkills[id][i] && g_SkillType[i] == type )
		{
			return i;
		}
	}

	return -1;
}

bool:SM_IsValidRace( iRace )
{
	if ( 1 <= iRace <= get_pcvar_num( CVAR_wc3_races ) )
	{
		return true;
	}

	return false;
}