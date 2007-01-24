/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Common Ultimate Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

// This is ran every second...
public _ULT_Delay()
{
	
	// Decrement the global counter
	g_iUltimateDelay--;
	
	// Now we need to loop through all players and decrement their ultimate delay
	new players[32], numplayers, i, id;
	get_players( players, numplayers );

	for ( i = 0; i < numplayers; i++ )
	{
		id = players[i];

		// Reduce the user's ultimate delay
		p_data[id][P_ULTIMATEDELAY]--;

		// Then the user's ultimate is ready
		if ( p_data[id][P_ULTIMATEDELAY] == 0 )
		{
			ULT_IconHandler( id );
		}
	}
}

ULT_ResetCooldown( id, iTime, iHideIcon = true )
{
	p_data[id][P_ULTIMATEDELAY]		= iTime;
	
	// Hide the user's ultimate icon
	if ( iHideIcon )
	{
		ULT_Icon( id, ICON_HIDE );
	}
}

// This function will handle ultimate icons - basically shows if appropriate or hides if appropriate
ULT_IconHandler( id )
{
	// WC3 isn't running... shux
	if ( !WC3_Check() )
	{
		return;
	}
	
	new bool:bShowIcon = true;

	// User has no ultimate!
	if ( SM_GetSkillLevel( id, SM_GetSkillOfType( id, SKILL_TYPE_ULTIMATE ) ) <= 0 )
	{
		bShowIcon = false;
	}

	// In global delay - can't show icon!
	else if ( g_iUltimateDelay > 0 )
	{
		bShowIcon = false;
	}

	// User's delay is longer!
	else if ( p_data[id][P_ULTIMATEDELAY] > 0 )
	{
		bShowIcon = false;
	}

	// User is dead - don't show icon!
	else if ( !is_user_alive( id ) )
	{
		bShowIcon = false;
	}

	// User isn't connected! - don't show icon!
	else if ( !p_data_b[id][PB_ISCONNECTED] )
	{
		bShowIcon = false;
	}


	// We need to hide the user's icon!
	if ( !bShowIcon )
	{
		// In theory I could remember what was displayed when - but easy way out is this
		ULT_ClearIcons( id );
	}

	// Show the user's icon!
	else
	{
		// Play the ultimate ready sound
		client_cmd( id, "speak %s", g_szSounds[SOUND_ULTIMATEREADY] )
		
		// Give the user a graphical message that their ultimate is ready
		WC3_StatusText( id, TXT_ULTIMATE, "%L", id, "ULTIMATE_READY" );
		
		// Show their ultimate icon
		ULT_Icon( id, ICON_SHOW );
	}
}

// This function will display/flash/hide the race's ultimate icon on the screen
ULT_Icon( id, flag )
{
	
	// DOD does not support ultimate icons :/
	if ( g_MOD == GAME_DOD )
	{
		return;
	}

	// If they mysteriously lost a level - we shouldn't display the icon (i.e. admin command)
	if ( p_data[id][P_LEVEL] < MIN_ULT_LEVEL && flag != ICON_HIDE )
	{
		flag = ICON_HIDE;
	}

	new iRaceID = p_data[id][P_RACE];
	
	// If we're chameleon we need to display the icon of the ultimate we have
	if ( p_data[id][P_RACE] == RACE_CHAMELEON )
	{
		new iChamUltimate = g_ChamSkills[3];

		iRaceID = g_SkillOwner[iChamUltimate];
	}
	
	// We should only do this if we have a valid race ID
	if ( iRaceID )
	{
		new r, g, b;

		// Each race has its on color for its icon...
		switch ( iRaceID )
		{
			case 1: r=255,	g=0,	b=0;		// Undead
			case 2: r=0,	g=120,	b=120;		// Human
			case 3: r=255,	g=255,	b=255;		// Orc
			case 4: r=0,	g=0,	b=255;		// Night Elf
			case 5: r=255,	g=0,	b=0;		// Blood Mage
			case 6: r=0,	g=200,	b=200;		// Shadow Hunter
			case 7: r=255,	g=0,	b=0;		// Warden
			case 8: r=0,	g=255,	b=0;		// Crypt Lord
		}
			
		// Special circumstances should be done here
		switch ( flag )
		{
			case ICON_FLASH:
			{

				// Change colors for Suicide Bomber or Immolate
				if ( SM_GetSkillLevel( id, ULTIMATE_SUICIDE ) > 0 || SM_GetSkillLevel( id, ULTIMATE_IMMOLATE ) > 0 )
				{
					r=255, g=255, b=255;
				}
			}
		}

		// Create the status icon
		Create_StatusIcon( id, flag, g_UltimateIcons[iRaceID - 1], r, g, b );

		// Remember what icon we displayed
		if ( flag == ICON_SHOW || flag == ICON_FLASH )
		{
			g_ULT_iLastIconShown[id] = iRaceID;
		}

		// No icon shown
		else if ( flag == ICON_HIDE )
		{
			g_ULT_iLastIconShown[id] = 0;
		}
	}
}

ULT_ClearIcons( id )
{
	// Only have icons for CS/CZ
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Remove the last icon that was shown!
		if ( g_ULT_iLastIconShown[id] > 0 )
		{
			Create_StatusIcon( id, ICON_HIDE, g_UltimateIcons[g_ULT_iLastIconShown[id]-1], 0, 0, 0 );
		}
		
		/*
		// Loop through all possible icons and remove them
		for ( new i = 0; i < MAX_RACES - 1; i++ )
		{
			Create_StatusIcon( id, ICON_HIDE, g_UltimateIcons[i], 0, 0, 0 )
		}*/
	}
}

ULT_Available( id )
{

	// User needs ult + can't have it used + can't have a delay + can't have a global delay
	if ( p_data[id][P_ULTIMATEDELAY] <= 0 && g_iUltimateDelay <= 0 )
	{
		new iSkillID = SM_GetSkillOfType( id, SKILL_TYPE_ULTIMATE );
		new iSkillLevel = SM_GetSkillLevel( id, iSkillID );
		
		if ( iSkillLevel > 0 )
		{
			return true;
		}
	}

	return false;
}

ULT_IsImmune( id )
{

	new bool:bBlocked = false;

	if ( p_data_b[id][PB_WARDENBLINK] )
	{
		bBlocked = true;
	}

	else if ( ITEM_Has( id, ITEM_NECKLACE ) > ITEM_NONE )
	{
		ITEM_RemoveCharge( id, ITEM_NECKLACE );

		bBlocked = true;
	}

	if ( bBlocked )
	{
		client_print( id, print_chat, "%s You have blocked an enemy's ultimate!", g_MODclient );

		// Play ultimate blocked sound
		emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_SPELLSHIELD], 1.0, ATTN_NORM, 0, PITCH_NORM );
	}

	return bBlocked;
}

ULT_Blocked( id )
{
	// Maybe play a sound here instead?
	client_print( id, print_chat, "%s Your ultimate has been blocked", g_MODclient );

	// Play ultimate blocked sound
	emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_DISPELL], 1.0, ATTN_NORM, 0, PITCH_NORM );
}

public _ULT_Ping( parm[] )
{
	new id = parm[0];
	new iTimeLeft = parm[1];
	
	// Decrement our timer
	parm[1]--;
	
	// User died or diconnected
	if ( !p_data_b[id][PB_ISCONNECTED] || !is_user_alive( id ) )
	{
		p_data_b[id][PB_ISSEARCHING] = false;
	}
	
	// This is the last "playing" of the sound, no target was found :/
	if ( iTimeLeft == 0 )
	{
		p_data_b[id][PB_ISSEARCHING] = false;

		ULT_Icon( id, ICON_SHOW );
	}
	
	// Then we need to play the sound + flash their icon!
	if ( p_data_b[id][PB_ISSEARCHING] )
	{
		// Flash their ultimate icon
		ULT_Icon( id, ICON_FLASH );

		// Play the ping sound
		emit_sound( id, CHAN_STATIC, g_szSounds[SOUND_ULTIMATESCAN], 1.0, ATTN_NORM, 0, PITCH_NORM );

		set_task( 1.0, "_ULT_Ping", TASK_ULTPING + id, parm, 2 );
	}

	return;
}

// Function called before user's spawn
ULT_Reset( id )
{
	// Remove any explosions that are occuring, we don't want them carried over to the next round
	task_exists( TASK_EXPLOSION + id )		? remove_task( TASK_EXPLOSION + id ) : 0;
	task_exists( TASK_BEAMCYLINDER + id )	? remove_task( TASK_BEAMCYLINDER + id ) : 0;

	// Stop Crypt Lord's Ultimate
	task_exists( TASK_FUNNELS + id )		? remove_task( TASK_FUNNELS + id ) : 0;

	// Stop the ultimate cooldowns since they will reset at round start
	task_exists( TASK_UDELAY + id )			? remove_task( TASK_UDELAY + id ) : 0;
	
	// Reset Big Bad Voodoo
	if ( task_exists( TASK_RESETGOD + id ) )
	{
		remove_task( TASK_RESETGOD + id );

		SH_Ult_Remove( id );
	}
	
	// Set this to false to stop searching ultimates (used by NE + ORC + BM ultimates)
	p_data_b[id][PB_ISSEARCHING] = false;

	// Hide their ultimate icon
	ULT_Icon( id, ICON_HIDE );
}
