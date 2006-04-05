
/*

STATISTICS TO SHOW AT ROUND	END:
-------------------------------

	Unholy Aura					Fortitude
	  Average speed: %i%		  Bonus	Health:	%i
	Vampiric Aura				Invisibility
	  Health stolen: %d			  Invis	level: %i%
	Frost Nova					Bash
	  Damage: %i, Kills: %i		  Procs: %i, Damage: %i
	  Assists: %i				Inner Fire
	Frost Armor					  Damage: %i
	  Procs: %i	(%i%)


	Regeneration				Elune's	Grace
	  Health gained: %i			  Damage absorbed: %i
	Berserk						Evasion
	  Damage: %i				  Procs: %i, Damage: %i
	Pulverize					Nature's Blessing
	  Damage: %i, Kills: %i		  Average speed: %i%
	Pillage						Trueshot Aura
	  Money: %i, Ammo: %i		  Damage: %i
	  Grenades:	%i


	Experience:
	  Round: %i, Session: %i

	Type '/war3stats' for more
	information...

	Other things to	track:
	Kill streaks,

*/



public STATS_get_values( id, szStats[],	iMaxLen	) {

	if ( !g_PlayerInfo[id][CURRENT_RACE] )
		return PLUGIN_HANDLED;

	switch ( g_PlayerInfo[id][CURRENT_RACE]	)
	{
		case RACE_UNDEAD:
		{
			// Unholy Aura

			// Speed % increase



			// Vampiric	Aura

			if ( g_PlayerInfo[id][CURRENT_SKILL1] )
			{
				// Health stolen
			}

			// Frost Nova

			if ( g_PlayerInfo[id][CURRENT_SKILL2] )
			{
				// Damage

				// Kills

				// Assists
			}

			// Frost Armor

			if ( g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				// Procs

				// Percentage
			}
		}

		case RACE_HUMAN:
		{
			// Fortitude

			// Invisibility

			if ( g_PlayerInfo[id][CURRENT_SKILL1] )
			{
				// Ultimates avoided
			}

			// Bash

			if ( g_PlayerInfo[id][CURRENT_SKILL2] )
			{
				// Procs

				// Damage
			}

			// Inner Fire

			if ( g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				// Damage
			}
		}

		case RACE_ORC:
		{
			// Regeneration

			// Berserk

			if ( g_PlayerInfo[id][CURRENT_SKILL1] )
			{
				// Damage
			}

			// Pulverize

			if ( g_PlayerInfo[id][CURRENT_SKILL2] )
			{
				// Damage

				// Kills
			}

			// Pillage

			if ( g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				// Money

				// Ammo

				// Grenades
			}
		}

		case RACE_NIGHTELF:
		{
			// Elune's Grace

			// Damage avoided


			// Evasion

			if ( g_PlayerInfo[id][CURRENT_SKILL1] )
			{
				// Procs

				// Damage avoided
			}

			// Nature's	Blessing

			if ( g_PlayerInfo[id][CURRENT_SKILL2] )
			{
				// Speed % increase
			}

			// Trueshot	Aura

			if ( g_PlayerInfo[id][CURRENT_SKILL3] )
			{
				// Damage
			}
		}
	}

	return PLUGIN_HANDLED;
}


public STATS_round_end() {

}