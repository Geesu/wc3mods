
new g_UltimateIcons[8][32] = 
					{
						"dmg_rad",				// Undead
						"item_longjump",		// Human Alliance
						"dmg_shock",			// Orcish Horde
						"item_healthkit",		// Night Elf
						"dmg_heat",				// Blood Mage
						"suit_full",			// Shadow Hunter
						"cross",				// Warden
						"dmg_gas"				// Crypt Lord
					};

// ****************************************
// Crypt Lord's Locust Swarm
// ****************************************

Ultimate_LocustSwarm(id){

	new parm[11]
	parm[7]=id
	new players[32], numberofplayers, i, player, possibility[33], count = 0
	get_players(players, numberofplayers)
	for (i = 0; i < numberofplayers; ++i){
		player=players[i]
		if(get_user_team(player)!=get_user_team(id) && is_user_alive(player) && !p_data_b[player][PB_WARDENBLINK] && p_data[player][P_ITEM]!=ITEM_NECKLACE){
			possibility[count]=player
			count++
		}
	}
	if(count==0){
		set_hudmessage(178, 14, 41, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 5)
		show_hudmessage(id,"%L",id,"NO_VALID_TARGETS_FOUND")
		return PLUGIN_CONTINUE
	}

	while(parm[6]==0)								// Prevents target from being the server
		parm[6]=possibility[random_num(0,count)]

	new origin[3], origin2[3]
	get_user_origin(id,origin)
	get_user_origin(parm[6],origin2)
	parm[0]=origin[0]
	parm[1]=origin[1]
	parm[2]=origin[2]

	ULT_Icon( id, ICON_FLASH );

	p_data_b[id][PB_ULTIMATEUSED]=true

	drawfunnels(parm)
	return PLUGIN_CONTINUE
}

public drawfunnels(parm[]){
	new MULTIPLIER = 150 // the lower the number the faster it reaches the target
	new id = parm[6]
	new caster = parm[7]

	if(p_data[id][P_ITEM]==ITEM_NECKLACE || p_data_b[id][PB_WARDENBLINK] || !is_user_alive(id) || !p_data_b[id][PB_ISCONNECTED]){
		p_data_b[caster][PB_ULTIMATEUSED]=false
		ULT_Icon( caster, ICON_SHOW )	
		return PLUGIN_HANDLED
	}

	new origin[3], funnel[3], name[32], name2[32]
	get_user_name(id,name,31)
	get_user_name(caster,name2,31)
	get_user_origin(id,origin)

	funnel[0]=parm[0]			// Origin of the funnel
	funnel[1]=parm[1]
	funnel[2]=parm[2]
	
	Create_TE_LARGEFUNNEL(funnel, g_sSnow, 0)
	
	new xdist = diff(origin[0],funnel[0])
	new ydist = diff(origin[1],funnel[1])
	new zdist = diff(origin[2],funnel[2])	

	if(diff(origin[0],(funnel[0]-MULTIPLIER))<xdist)
		parm[0]=funnel[0]-MULTIPLIER
	else if(diff(origin[0],(funnel[0]+MULTIPLIER))<xdist)
		parm[0]=funnel[0]+MULTIPLIER
	else
		parm[0]=origin[0]

	if(diff(origin[1],(funnel[1]-MULTIPLIER))<ydist)
		parm[1]=funnel[1]-MULTIPLIER
	else if(diff(origin[1],(funnel[1]+MULTIPLIER))<ydist)
		parm[1]=funnel[1]+MULTIPLIER
	else
		parm[1]=origin[1]

	if(diff(origin[2],(funnel[2]-MULTIPLIER))<zdist)
		parm[2]=funnel[2]-MULTIPLIER
	else if(diff(origin[2],(funnel[2]+MULTIPLIER))<zdist)
		parm[2]=funnel[2]+MULTIPLIER
	else
		parm[2]=origin[2]

	if(!endround){
		if(!(xdist<50 && ydist<50 && zdist<50)){
			#if MOD == 1
				set_task(0.5,"drawfunnels",caster+TASK_FUNNELS,parm,11)
			#else
				set_task(0.1,"drawfunnels",caster+TASK_FUNNELS,parm,11)
			#endif
		}
		else{
			WAR3_damage(id, caster, 45, CSW_LOCUSTS, -1)

			emit_sound(id,CHAN_STATIC, SOUND_LOCUSTSWARM, 1.0, ATTN_NORM, 0, PITCH_NORM)

			ULT_Icon( caster, ICON_HIDE );
			
			p_data[caster][P_ULTIMATEDELAY] = get_pcvar_num( CVAR_wc3_ult_cooldown )
		}
	}
	return PLUGIN_HANDLED
}

// This is ran every second...
public _ULT_Delay()
{
	
	// Decrement the global counter
	g_iUltimateDelay--;

	
	// Now we need to loop through all players and decrement their ultimate delay
	new players[32], numplayers, i, id;
	get_players( players, numplayers, "a" );

	for ( i = 0; i < numplayers; i++ )
	{
		id = players[i];

		// Reduce the user's ultimate delay
		p_data[id][P_ULTIMATEDELAY]--;


		// Then the user's ultimate is ready
		if ( p_data[id][P_ULTIMATEDELAY] == 0 && g_iUltimateDelay <= 0 )
		{
			Ultimate_Ready( id );
		}
	}

	log_amx( "[DEBUG] Timer Decremented: %d", g_iUltimateDelay );
}

Ultimate_Ready( id )
{
	// Need to set this before the war3_check in case they want to restart war3ft
	p_data_b[id][PB_ULTIMATEUSED] = false;

	if ( !WAR3_Check() )
	{
		return;
	}
	
	if ( is_user_alive( id ) && p_data_b[id][PB_ISCONNECTED] && p_data[id][P_ULTIMATE] )
	{
		// Play the ultimate ready sound
		client_cmd( id, "speak %s", SOUND_ULTIMATEREADY )
		
		// Give the user a graphical message that their ultimate is ready
		WC3_Status_Text( id, 2.0, HUDMESSAGE_POS_STATUS, "%L", id, "ULTIMATE_READY" );
		
		// Show their ultimate icon
		ULT_Icon( id, ICON_SHOW );
	}
	
	return;
}

// This function will display/flash/hide the race's ultimate icon on the screen
ULT_Icon( id, flag )
{

	new iRaceID = p_data[id][P_RACE];
	
	// If we're chameleon we need to display the icon of the ultimate we have
	if ( p_data[id][P_RACE] == RACE_CHAMELEON )
	{
		iRaceID = g_ChamSkills[4];
	}
	
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
			// Change colors for Ultimate
			if ( Verify_Skill(id, RACE_UNDEAD, SKILL4) )
			{
				r=255, g=255, b=255;
			}
		}
	}

	
	// Create the status icon
	Create_StatusIcon( id, flag, g_UltimateIcons[iRaceID - 1], r, g, b );
}

ULT_ClearIcons( id )
{
	// Only have icons for CS/CZ
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		new i;
		
		// Loop through all possible icons and remove them
		for ( i = 0; i < MAX_RACES - 1; i++ )
		{
			Create_StatusIcon( id, ICON_HIDE, g_UltimateIcons[i], 0, 0, 0 )
		}
	}
}

ULT_Available( id )
{
	// User needs ult + can't have it used + can't have a delay + can't have a global delay
	if ( p_data[id][P_ULTIMATE] && !p_data_b[id][PB_ULTIMATEUSED] && !p_data[id][P_ULTIMATEDELAY] && !g_ultimateDelay )
	{
		return true;
	}

	return false;
}

ULT_IsImmune( id )
{
	return ( p_data[id][P_ITEM] == ITEM_NECKLACE || p_data_b[id][PB_WARDENBLINK] )
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
		emit_sound( id, CHAN_STATIC, SOUND_ULTIMATESCAN, 1.0, ATTN_NORM, 0, PITCH_NORM );

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
	task_exists( TASK_RESETGOD + id )		? remove_task( TASK_RESETGOD + id ) : 0;
	
	// Set this to false to stop searching ultimates (used by NE + ORC + BM ultimates)
	p_data_b[id][PB_ISSEARCHING] = false;

	// Hide their ultimate icon
	ULT_Icon( id, ICON_HIDE );
}