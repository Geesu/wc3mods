
/*

STATISTICS TO SHOW AT ROUND END:
-------------------------------

	Unholy Aura                 Fortitude
	  Average speed: %i%          Bonus Health: %i
	Vampiric Aura               Invisibility
	  Health stolen: %d           Invis level: %i%
	Frost Nova                  Bash
	  Damage: %i, Kills: %i       Procs: %i, Damage: %i
	  Assists: %i               Inner Fire
	Frost Armor                   Damage: %i
	  Procs: %i (%i%)


	Regeneration                Elune's Grace
	  Health gained: %i           Damage absorbed: %i
	Berserk                     Evasion
	  Damage: %i                  Procs: %i, Damage: %i
	Pulverize                   Nature's Blessing
	  Damage: %i, Kills: %i       Average speed: %i%
	Pillage                     Trueshot Aura
	  Money: %i, Ammo: %i         Damage: %i
	  Grenades: %i


	Experience:
	  Round: %i, Session: %i

	Type '/war3stats' for more
	information...

	Other things to track:
	Kill streaks,

*/



public STATS_get_values( id, szStats[], iMaxLen ) {

	if ( !g_PlayerInfo[id][CURRENT_RACE] )
		return PLUGIN_HANDLED;

	new szSkillname[32];
	new race = g_PlayerInfo[id][CURRENT_RACE];

	switch ( race )
	{
		case RACE_UNDEAD:
		{
			// Unholy Aura

			LANG_GetSkillName( race, SKILL_RACIAL, id, szSkillname, 31 );

			// Speed % increase



			// Vampiric Aura

			if ( g_PlayerInfo[id][CURRENT_SKILL1] )
			{
				LANG_GetSkillName( race, SKILL_1, id, szSkillname, 31 );

				// Health stolen
			}

			// Frost Nova

			if ( g_PlayerInfo[id][CURRENT_SKILL2] )
			{
				LANG_GetSkillName( race, SKILL_2, id, szSkillname, 31 );

				// Damage

				// Kills

				// Assists
			}

			// Frost Armor

			if ( g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				LANG_GetSkillName( race, SKILL_3, id, szSkillname, 31 );

				// Procs

				// Percentage
			}
		}

		case RACE_HUMAN:
		{
			// Fortitude

			LANG_GetSkillName( race, SKILL_RACIAL, id, szSkillname, 31 );

			// Invisibility

			if ( g_PlayerInfo[id][CURRENT_SKILL1] )
			{
				LANG_GetSkillName( race, SKILL_1, id, szSkillname, 31 );

				// Ultimates avoided
			}

			// Bash

			if ( g_PlayerInfo[id][CURRENT_SKILL2] )
			{
				LANG_GetSkillName( race, SKILL_2, id, szSkillname, 31 );

				// Procs

				// Damage
			}

			// Inner Fire

			if ( g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				LANG_GetSkillName( race, SKILL_3, id, szSkillname, 31 );

				// Damage
			}
		}

		case RACE_ORC:
		{
			// Regeneration

			LANG_GetSkillName( race, SKILL_RACIAL, id, szSkillname, 31 );

			// Berserk

			if ( g_PlayerInfo[id][CURRENT_SKILL1] )
			{
				LANG_GetSkillName( race, SKILL_1, id, szSkillname, 31 );

				// Damage
			}

			// Pulverize

			if ( g_PlayerInfo[id][CURRENT_SKILL2] )
			{
				LANG_GetSkillName( race, SKILL_2, id, szSkillname, 31 );

				// Damage

				// Kills
			}

			// Pillage

			if ( g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				LANG_GetSkillName( race, SKILL_3, id, szSkillname, 31 );

				// Money

				// Ammo

				// Grenades
			}
		}

		case RACE_NIGHTELF:
		{
			// Elune's Grace

			LANG_GetSkillName( race, SKILL_RACIAL, id, szSkillname, 31 );

			// Damage avoided


			// Evasion

			if ( g_PlayerInfo[id][CURRENT_SKILL1] )
			{
				LANG_GetSkillName( race, SKILL_1, id, szSkillname, 31 );

				// Procs

				// Damage avoided
			}

			// Nature's Blessing

			if ( g_PlayerInfo[id][CURRENT_SKILL2] )
			{
				LANG_GetSkillName( race, SKILL_2, id, szSkillname, 31 );

				// Speed % increase
			}

			// Trueshot Aura

			if ( g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				LANG_GetSkillName( race, SKILL_3, id, szSkillname, 31 );

				// Damage
			}
		}
	}

	return PLUGIN_HANDLED;
}


public STATS_round_end() {

}