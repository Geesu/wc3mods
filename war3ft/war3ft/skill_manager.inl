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
}

// This function will return the skill ID when given a race and a skill position
// Position 0 = Skill 1
// Position 1 = Skill 2
// Position 2 = Skill 3
// Position 3 = Ultimate
// Position 4 = Passive Ability (if it exists)
SM_GetSkill( iRace, iSkillPos, iType = -1 )
{
	
	new iSkillID;
	
	// This is quite "simple" for Chameleon
	if ( iRace == RACE_CHAMELEON )
	{
		return g_ChamSkills[iSkillPos];
	}

	else
	{
		for ( iSkillID = 0; iSkillID < MAX_SKILLS; iSkillID++ )
		{
			if ( g_SkillOwner[iSkillID] == iRace )
			{
				// Are we searching for a type ?
				if ( iType != -1 )
				{
					if ( g_SkillType[iSkillID] == iType )
					{
						iSkillPos--;
					}
				}

				// Not looking for a particular skill type
				else
				{
					iSkillPos--;
				}
			}
			
			// Then skill was found, return it!
			if ( iSkillPos < 0 )
			{
				return iSkillID;
			}
		}
	}

	return -1;
}

// This function will return the position of where the skill resides in the p_data array (i.e. SKILL_1, SKILL_2, SKILL_3, SKILL_ULTIMATE, etc...)
// Of course this doesn't work for chameleon - DUH!!
SM_GetSkillPos( skill_id )
{
	new iRaceID		= g_SkillOwner[skill_id];
	new i, iSkillPos = 0;

	for ( i = 0; i < MAX_SKILLS; i++ )
	{
		
		if ( g_SkillOwner[i] == iRaceID )
		{
			iSkillPos++;
		}

		if ( i == skill_id )
		{
			return iSkillPos;
		}
	}

	return -1;
}

SM_VerifySkill( id, skill_id )
{

	if ( id == 0 )
	{
		return 0;
	}

	// Determine which race the skill belongs to
	new iRaceID		= g_SkillOwner[skill_id];
	new iSkillPos	= SM_GetSkillPos( skill_id );

	// In theory this should never occur, unless someone edits the source
	if ( iSkillPos == -1 )
	{
		log_amx( "Skill %d is dangling - no skill position found", skill_id );

		return 0;
	}
	
	if ( p_data[id][P_RACE] == iRaceID && p_data[id][iSkillPos] )
	{
		return p_data[id][iSkillPos];
	}

	// Chameleon Check
	else if ( p_data[id][P_RACE] == RACE_CHAMELEON )
	{
		// Find the skill in the chameleon array
		iSkillPos = CHAM_FindSkill( skill_id );

		if ( iSkillPos != -1 && p_data[id][iSkillPos] )
		{
			return p_data[id][iSkillPos];
		}
	}

	return 0;
}

SM_VerifyRace( id, race_id )
{

	if ( id == 0 )
	{
		return false;
	}

	if ( p_data[id][P_RACE] == race_id )
	{
		return true;
	}

	// Chameleon Check
	else if ( p_data[id][P_RACE] == RACE_CHAMELEON )
	{
		new i, iSkillID;

		for ( i = 0; i < 5; i++ )
		{
			iSkillID = g_ChamSkills[i];

			if ( g_SkillOwner[iSkillID] == race_id )
			{
				return true;
			}
		}
	}

	return false;
}
