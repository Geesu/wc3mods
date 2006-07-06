/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Day of Defeat only functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

public EVENT_DOD_EndRound()
{
	
	if ( !WAR3_Check() )
	{
		return;
	}
	
	// The round has ended
	g_EndRound = true;

	// Allies = 3, Axis = 4
	new iWinner = read_data( 1 );
	new iTeam = -1;

	// Allies won
	if ( iWinner == 3 )
	{
		iTeam = ALLIES;
	}

	// Axis won!
	else if ( iWinner == 4 )
	{
		iTeam = AXIS;
	}

	XP_WinRound( iTeam );

	return;
}

// Called when a user scores!
public client_score( index, score, total )
{

	if ( !WAR3_Check() )
	{
		return;
	}

	new iBonusMoney = score * 1000;

	// Award the user money
	SHARED_SetUserMoney( index, SHARED_GetUserMoney( index ) + iBonusMoney, 1 );

	// Award the user XP
	new iXP = score * xpgiven[p_data[index][P_LEVEL]];

	iXP = XP_Give( index, iXP );

	if ( get_pcvar_num( CVAR_wc3_show_objectives ) )
	{
		client_print( index, print_chat, "%s %L", g_MODclient, index, "DOD_AWARDED_XP_OBJECTIVE", iXP );
	}

	return;
}

public _DOD_showMoney(parm[3])
{

	if ( !WAR3_Check() )
	{
		return;
	}
	
	new id = parm[0];

	if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		return;
	}
	
	new Float:fTime = 0.7;

	if ( SHARED_IsOnTeam( id ) )
	{

		new szMsg[128];

		// Set up the hud message
		set_hudmessage( 255, 255, 255, 0.5, 0.0, 2, 1.5, 40.0, 0.1, 0.5, HUD_DOD_MONEY );

		// Should we show the change in money?
		if ( parm[1] == 1 )
		{
			// Lost Money
			if ( parm[2] < 0 )
			{
				formatex( szMsg, 127, "$%d^n%d", p_data[id][P_MONEY], parm[2] );
			}

			// Gained Money
			else
			{
				formatex( szMsg, 127, "$%d^n+%d", p_data[id][P_MONEY], parm[2] );
			}

			show_hudmessage( id, szMsg );
			
			// Remove task if it exists
			( task_exists( TASK_MONEYLOOP + id ) ) ? remove_task( TASK_MONEYLOOP + id ) : 0;
			
			// Don't show the message for 2 seconds so the user notices the change in money
			fTime = 2.0
		}

		// Just show the money like usual
		else
		{
			format( szMsg, 127, "$%d", p_data[id][P_MONEY] );

			show_hudmessage( id, szMsg );
		}
	}

	parm[1] = 0;
	parm[2] = 0;

	set_task( fTime, "_DOD_showMoney", TASK_MONEYLOOP+id, parm, 3 );

	return;
}