public MENU_War3Menu( id )
{
	static pos, szMenu[256], keys;
	keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<9);
	pos = 0;

	// Add the title
	pos += formatex( szMenu[pos], 255-pos, "%L^n^n", id, "MENU_WAR3MENU" );

	// Add the actual options to the menu
	pos += formatex( szMenu[pos], 255-pos, "\w1. %L^n", id, "SKILLS_MENU" );
	pos += formatex( szMenu[pos], 255-pos, "\w2. %L^n", id, "RACE_MENU" );
	pos += formatex( szMenu[pos], 255-pos, "\w3. %L^n", id, "ITEM_MENU" );
	pos += formatex( szMenu[pos], 255-pos, "\w4. %L^n", id, "HELP" );
	pos += formatex( szMenu[pos], 255-pos, "\w5. %L^n", id, "ADMIN_MENU_TITLE" );
	pos += formatex( szMenu[pos], 255-pos, "^n\w0. %L", id, "WORD_EXIT" );

	// Display the menu
	show_menu( id, keys, szMenu, -1 );

	return;
}

public _MENU_War3Menu( id, key )
{

	if ( !WC3_Check() )
	{
		return;
	}

	switch ( key )
	{
		case 0:	menu_Skill_Options( id );
		case 1:	menu_Race_Options( id );
		case 2:	menu_Item_Options( id );
		case 3:	MOTD_War3help( id );
		case 4:	menu_Admin_Options( id );
	}
	
	return;
}

public menu_Skill_Options( id )
{

	new pos = 0, i, menu_body[512], menuitems[3][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)


	format(menuitems[0],31,"%L",id,"SELECT_SKILLS")
	format(menuitems[1],31,"%L",id,"SKILLS_INFORMATION")
	format(menuitems[2],31,"%L",id,"RESELECT_SKILLS")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_SKILLS_OPTIONS")
	for (i = 0; i<3; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menuitems[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"WORD_EXIT")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Skill_Options(id,key){
	
	if ( !WC3_Check() )
	{
		return;
	}

	switch (key){
		case 0:	MENU_SelectSkill( id );
		case 1:	MOTD_SkillsInfo( id );
		case 2:	CMD_Handle( id, "resetskills" );
		case 8: MENU_War3Menu(id)
		default: return;
	}
	return;
}

public menu_Race_Options(id){

	new pos = 0, i, menu_body[512], menuitems[4][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

	format(menuitems[0],31,"%L",id,"CHANGE_RACE")
	format(menuitems[1],31,"%L",id,"SHOW_LEVEL")
	format(menuitems[2],31,"%L",id,"RESET_XP")
	format(menuitems[3],31,"%L",id,"SHOW_PLAYER_SKILLS")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_RACE_OPTIONS")
	for (i = 0; i<4; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menuitems[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"WORD_EXIT")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Race_Options(id,key){

	if ( !WC3_Check() )
	{
		return;
	}

	switch (key){
		case 0:	WC3_ChangeRaceStart( id );
		case 1:	WC3_ShowRaceInfo( id );
		case 2:	MENU_ResetXP( id );
		case 3:	MOTD_PlayerSkills( id );
		case 8: MENU_War3Menu(id)
		default: return;
	}
	return;
}

public menu_Item_Options(id){

	new pos = 0, i, menu_body[512], menuitems[4][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<8)|(1<<9)

	format(menuitems[0],31,"%L",id,"SHOPMENU1_OPTION")
	format(menuitems[1],31,"%L",id,"SHOPMENU2_OPTION")
	format(menuitems[2],31,"%L",id,"SHOW_SHOPMENU1_INFO")
	format(menuitems[3],31,"%L",id,"SHOW_SHOPMENU2_INFO")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_ITEM_OPTIONS")
	for (i = 0; i<4; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menuitems[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"WORD_EXIT")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Item_Options(id,key){

	if ( !WC3_Check() )
	{
		return;
	}

	switch (key){
		case 0:	MENU_Shopmenu( id, 0 );
		case 1:	MENU_Shopmenu( id, 9 );
		case 2:	MOTD_ItemsInfo( id, 0 );
		case 3:	MOTD_ItemsInfo( id, 9 );
		case 8: MENU_War3Menu(id)
		default: return;
	}
	return;
}

public menu_Admin_Options(id){

    if ( id && !( get_user_flags( id ) & XP_GetAdminFlag() ) )
	{
			client_print(id,print_center,"%s %L",g_MODclient, id,"YOU_HAVE_NO_ACCESS")
			return PLUGIN_HANDLED
	}

	new pos = 0, i, menu_body[512], menuitems[3][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)

	format(menuitems[0],31,"%L",id,"GIVE_IND_XP")
	format(menuitems[1],31,"%L",id,"GIVE_MULT_XP")
	format(menuitems[2],31,"%L",id,"SAVE_ALL_XP")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_ADMIN_MENU")
	for (i = 0; i<3; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menuitems[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"WORD_EXIT")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Admin_Options(id,key){

	if ( !WC3_Check() )
	{
		return;
	}

	switch (key){
		case 0:{
			g_menuOption[id] = 1
			g_menuSettings[id] = 50
			menu_PlayerXP_Options(id,g_menuPosition[id] = 0)
		}
		case 1:{
			g_menuOption[id] = 1
			g_menuSettings[id] = 50
			menu_TeamXP_Options(id)
		}
		case 2: DB_SaveAll();
		case 8: MENU_War3Menu(id)
	}
	return;
}

public menu_PlayerXP_Options(id,pos){

	if (pos < 0){
		menu_Admin_Options(id)
		return PLUGIN_CONTINUE
	}

	get_players(g_menuPlayers[id],g_menuPlayersNum[id])
	new menuBody[512]
	new b = 0
	new i
	new name[32], team[4], title[128], back[16], exitstring[16]
	new start = pos * 7
	if (start >= g_menuPlayersNum[id])
		start = pos = g_menuPosition[id] = 0
	format(title,127,"%L",id,"MENU_GIVE_PLAYERS_XP")
	new len = format(menuBody,511, "%s\R%d/%d^n\w^n",title,pos+1,(g_menuPlayersNum[id] / 7 + ((g_menuPlayersNum[id] % 7) ? 1 : 0 )))
	new end = start + 7
	new keys = (1<<9)|(1<<7)

	if (end > g_menuPlayersNum[id])
		end = g_menuPlayersNum[id]

	for(new a = start; a < end; ++a){
		i = g_menuPlayers[id][a]
		get_user_name(i,name,31)
		get_user_team(i,team,3)
		keys |= (1<<b)
		len += format(menuBody[len],511-len,"\w%d. %s^n\w",++b,name)
	}

	format(title,127,"%L",id,"GIVE")
	len += format(menuBody[len],511-len,"^n8. %s  %d XP^n",title,g_menuSettings[id])

	format(back,15,"%L",id,"BACK_STRING")

	if (end != g_menuPlayersNum[id]){
		format(menuBody[len],511-len,"^n9. %L...^n0. %s", id,"MORE_STRING", pos ? back : back)
		keys |= (1<<8)
	}
	else{
		format(exitstring,15,"%L",id,"WORD_EXIT")
		format(menuBody[len],511-len,"^n0. %s", pos ? back : exitstring)
	}


	show_menu(id,keys,menuBody,-1)
	return PLUGIN_CONTINUE

}

public _menu_PlayerXP_Options(id,key){

	if ( !WC3_Check() )
	{
		return;
	}

	switch(key){
		case 7:{
			++g_menuOption[id]
			if (g_menuOption[id]>6){
				g_menuOption[id]=1
			}
			switch(g_menuOption[id]){
				case 1: g_menuSettings[id] = 50
				case 2: g_menuSettings[id] = 100
				case 3: g_menuSettings[id] = 500
				case 4: g_menuSettings[id] = 1000
				case 5: g_menuSettings[id] = 5000
				case 6: g_menuSettings[id] = 10000
			}
			menu_PlayerXP_Options(id,g_menuPosition[id])
		}
		case 8: menu_PlayerXP_Options(id,++g_menuPosition[id])
		case 9: return;
		default:{
			new player = g_menuPlayers[id][g_menuPosition[id] * 7 + key]
			client_print(player,print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_JUST_GAVE_YOU_XP",g_menuSettings[id])
			p_data[player][P_XP] += g_menuSettings[id]

			XP_Check( player );

			menu_PlayerXP_Options(id,g_menuPosition[id])
		}
	}
	return;
}

public menu_TeamXP_Options(id){

	new pos = 0, i, menu_body[512], menuitems[3][32], give[16]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<7)|(1<<8)|(1<<9)

	format(menuitems[0],31,"%L",id,"TERRORISTS")
	format(menuitems[1],31,"%L",id,"CT")
	format(menuitems[2],31,"%L",id,"EVERYONE")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_TEAM_XP")
	for (i = 0; i<3; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menuitems[i])
	}
	format(give,15,"%L",id,"GIVE")
	pos += format(menu_body[pos], 511-pos,"^n8. %s  %d XP^n",give,g_menuSettings[id])
	pos += format(menu_body[pos], 511-pos, "^n^n\w9. %L",id,"BACK_STRING")
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"WORD_EXIT")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_TeamXP_Options(id,key){

	if ( !WC3_Check() )
	{
		return;
	}

	switch(key){
		case 0:
		{
			ADMIN_GiveXP( id, "@TERRORIST", g_menuSettings[id] );
			menu_TeamXP_Options(id)
		}
		case 1:{
			ADMIN_GiveXP( id, "@CT", g_menuSettings[id] );
			menu_TeamXP_Options(id)
		}
		case 2:{
			ADMIN_GiveXP( id, "@ALL", g_menuSettings[id] );
			menu_TeamXP_Options(id)
		}
		case 7:{
			++g_menuOption[id]
			if (g_menuOption[id]>6){
				g_menuOption[id]=1
			}
			switch(g_menuOption[id]){
				case 1: g_menuSettings[id] = 50
				case 2: g_menuSettings[id] = 100
				case 3: g_menuSettings[id] = 500
				case 4: g_menuSettings[id] = 1000
				case 5: g_menuSettings[id] = 5000
				case 6: g_menuSettings[id] = 10000
			}
			menu_TeamXP_Options(id)
		}
		case 8: menu_Admin_Options(id)
	}
	return;
}

public MENU_ResetXP(id)
{

	static szMenu[128];
	new keys = (1<<0)|(1<<1)|(1<<9);
	
	formatex( szMenu, 127, "%L^n^n\w1. Yes^n\w2. No", id, "MENU_RESET_XP" );

	show_menu( id, keys, szMenu, -1 );

	return;
}

public _MENU_ResetXP( id, key )
{
	
	if ( !WC3_Check() )
	{
		return;
	}

	// User selected yes
	if ( key == 0 )
	{
		XP_Reset( id );
	}
	
	return;
}

// Function will display the changerace menu
public MENU_ChangeRace( id, iRaceXP[MAX_RACES] )
{
	
	new szRaceName[MAX_RACES+1][64], i, pos, iKeys = 0, szMenu[512], szXP[16];

	// Get our race names
	for ( i = 0; i < get_pcvar_num( CVAR_wc3_races ); i++ )
	{
		lang_GetRaceName( i + 1, id, szRaceName[i], 63 );
	}

	pos += formatex( szMenu[pos], 512-pos, "%L", id, "MENU_SELECT_RACE" );

	// Then add the experience column
	if ( get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		pos += formatex( szMenu[pos], 512-pos, "\R%L^n^n", id, "MENU_WORD_EXPERIENCE" );
	}
	else
	{
		pos += formatex( szMenu[pos], 512-pos, "^n^n" );
	}

	// Build the changerace menu (for every race)
	for ( i = 0; i < get_pcvar_num( CVAR_wc3_races ); i++ )
	{
		num_to_str( iRaceXP[i], szXP, 15 );
		
		// Add the "Select a Hero" message if necessary
		if ( i == 4 )
		{
			pos += format( szMenu[pos], 512-pos, "%L", id, "SELECT_HERO" );
		}
		
		// User's current race
		if ( i == p_data[id][P_RACE] - 1 )
		{
			pos += formatex( szMenu[pos], 512-pos, "\d%d. %s\d\R%s^n", i + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
		}

		// Race the user wants to change to
		else if ( i == p_data[id][P_CHANGERACE] - 1 )
		{
			pos += formatex( szMenu[pos], 512-pos, "\r%d. %s\r\R%s^n", i + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
		}

		// All other cases
		else
		{
			pos += formatex( szMenu[pos], 512-pos, "\w%d. %s\y\R%s^n", i + 1, szRaceName[i], ( (get_pcvar_num( CVAR_wc3_save_xp )) ? szXP : " " ) );
		}

		iKeys |= (1<<i);
	}

	iKeys |= (1<<i);
	
	// This is needed so we can make the Auto-Select option "0" if the number of races is 9
	if ( get_pcvar_num( CVAR_wc3_races ) == 9 )
	{
		i = -1;
	}

	pos += format( szMenu[pos], 512-pos, "%L", id, "SELECT_RACE_FOOTER", i + 1 );
	
	// Add a cancel button to the bottom
	if ( get_pcvar_num( CVAR_wc3_races ) != 9 )
	{
		iKeys |= (1<<9);

		pos += format( szMenu[pos], 512-pos, "^n\w0. %L", id, "WORD_CANCEL" );
	}
	
	// Show the menu to the user!
	show_menu( id, iKeys, szMenu, -1 );

	return;
}

public _MENU_ChangeRace( id, key )
{

	if ( !WC3_Check() )
	{
		return;
	}
	
	// User pressed 0 (cancel)
	if ( get_pcvar_num( CVAR_wc3_races ) < 9 && key - 1 == get_pcvar_num( CVAR_wc3_races ) )
	{
		return;
	}

	// Save the current race data before we change
	DB_SaveXP( id );

	new iRace, iAutoSelectKey = KEY_0;
	
	if ( get_pcvar_num( CVAR_wc3_races ) != 9 )
	{
		iAutoSelectKey = get_pcvar_num( CVAR_wc3_races )
	}
	
	// Auto select a race
	if ( key == iAutoSelectKey )
	{
		iRace = random_num( 1, get_pcvar_num( CVAR_wc3_races ) );
	}

	// Otherwise race is set
	else
	{
		iRace = key + 1;
	}

	// If this is positive we need to make sure there aren't too many races
	new iRaceLimit = get_pcvar_num( CVAR_wc3_race_limit );
	if ( iRaceLimit > 0 )
	{

		new iPlayers[32], iNumPlayers, i, iTarget;
		new iTotalRaces[MAX_RACES];
		get_players( iPlayers, iNumPlayers );

		for ( i = 0; i < iNumPlayers; i++ )
		{
			iTarget = iPlayers[i];

			if ( iTarget != id )
			{
				iTotalRaces[p_data[iTarget][P_RACE]]++;
			}
		}

		// Make sure the total races aren't greater than the limit!
		if ( iTotalRaces[iRace] >= iRaceLimit )
		{
			client_print( id, print_center, "Too many people are that race, please choose another" );

			WC3_ChangeRaceStart( id );

			return;
		}
	}

	// User currently has a race
	if ( p_data[id][P_RACE] != 0 )
	{

		// Change the user's race at the start of next round
		if ( iRace != p_data[id][P_RACE] )
		{
			client_print( id, print_center, "%L", id, "CENTER_CHANGED_NEXT" );

			p_data[id][P_CHANGERACE] = iRace;
		}

		// Do nothing
		else
		{
			p_data[id][P_CHANGERACE] = 0;
		}
	}

	// User doesn't have a race so give it to him!!!
	else
	{
		WC3_SetRace( id, iRace );
	}

	return;
}

public MENU_ReplaceItem( id )
{

	new szMenu[512] = "", pos = 0;
	new iKeys = (1<<9)|(1<<0)|(1<<1);

	// Add the menu header
	pos += format( szMenu[pos], 511-pos, "%L^n^n", id, "MENU_REPLACE_ITEM" );

	new szItemName[64], szItemName2[64];
	LANG_GetItemName( g_iShopMenuItems[id][ITEM_SLOT_ONE], id, szItemName, 63 );
	LANG_GetItemName( g_iShopMenuItems[id][ITEM_SLOT_TWO], id, szItemName2, 63 );

	// Add the items
	pos += format( szMenu[pos], 511-pos, "\w1. %s^n", szItemName );
	pos += format( szMenu[pos], 511-pos, "\w2. %s^n", szItemName2 );

	// Add the exit option
	pos += format( szMenu[pos], 511-pos, "^n\w0. %L", id, "WORD_EXIT" );

	// Show the menu
	show_menu( id, iKeys, szMenu, -1 );

	return;
}

public _menu_ReplaceItem( id, iKey )
{
	if ( !WC3_Check() || iKey == 9 )
	{
		return;
	}

	// Remove item from item slot one
	if ( iKey == 0 )
	{
		ITEM_Remove( id, ITEM_SLOT_ONE )
	}

	// Remove item from itemslot two
	else if ( iKey == 1 )
	{
		ITEM_Remove( id, ITEM_SLOT_TWO )
	}

	ITEM_Buy( id, g_iFutureItem[id] );

	return;
}

public MENU_Shopmenu( id, iStart )
{
	if ( !ITEM_CanBuy( id ) )
	{
		return;
	}

	new szMenu[512], szItemName[64], pos = 0, i, iItemID;
	new iKeys = (1<<9);

	// Add the header
	if ( iStart == 0 )
	{
		pos += format( szMenu[pos], 511-pos, "%L", id, "MENU_BUY_ITEM" );
	}

	// "Shopmenu 2"
	else if ( iStart == MAX_PAGE_ITEMS )
	{
		pos += format( szMenu[pos], 511-pos, "%L", id, "MENU_BUY_ITEM2" );
	}

	// Lets add the items to the menu!
	for ( i = 0; i < MAX_PAGE_ITEMS; i++ )
	{
		iItemID = iStart + i;

		LANG_GetItemName( iItemID, id, szItemName, 63 );

		// These items don't exist in DOD
		if ( g_MOD == GAME_DOD && ( iItemID == ITEM_SCROLL ) )
		{
			pos += format( szMenu[pos], 511-pos, "\d%d. %s\y\R%d^n", i + 1, szItemName, ITEM_COST[iItemID] );
		}

		// Everything else is allowed!
		else
		{
			pos += format( szMenu[pos], 511-pos, "\w%d. %s\y\R%d^n", i + 1, szItemName, ITEM_COST[iItemID] );
			iKeys |= (1<<i);
		}

	}

	pos += format( szMenu[pos], 511-pos, "^n\w0. %L", id, "WORD_EXIT" );

	show_menu( id, iKeys, szMenu, -1 );
}

public _MENU_Shopmenu1( id, iKey )
{
	if ( !WC3_Check() || iKey == 9 )
	{
		return;
	}

	ITEM_Buy( id, iKey );

	return;
}

public _MENU_Shopmenu2( id, iKey )
{
	if ( !WC3_Check() || iKey == 9 )
	{
		return;
	}

	iKey += MAX_PAGE_ITEMS;
	
	ITEM_Buy( id, iKey );

	return;
}

public MENU_SelectSkill( id )
{

	// User has no race, how can we select skills?!?
	if ( p_data[id][P_RACE] == 0 )
	{
		//set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
		WC3_StatusText( id, TXT_TOP_CENTER, "%L", id, "SELECT_RACE_BEFORE_SKILLS" );

		return;
	}

	// Lets make sure the user has some available skill points
	new iSkillsUsed = SM_TotalSkillPointsUsed( id );
	if ( iSkillsUsed >= p_data[id][P_LEVEL] )
	{

		//set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
		WC3_StatusText( id, TXT_TOP_CENTER, "%L", id, "ALREADY_SELECTED_SKILL_POINTS" );

		return;
	}

	// Bots don't need a menu now do they??
	if ( is_user_bot( id ) )
	{
		// No race has been selected yet!!
		if ( !SM_SkillAvailable( id ) )
		{
			return;
		}

		static iRandomSkill, iSkillLevel;

	
		// Loop while we have skills available
		while ( iSkillsUsed < p_data[id][P_LEVEL] )
		{
			iRandomSkill = SM_GetRandomSkill( id );
			iSkillLevel = SM_GetSkillLevel( id, iRandomSkill );

			
			// Give them their ultimate if we can
			if ( SM_GetSkillType( iRandomSkill ) == SKILL_TYPE_ULTIMATE && iSkillLevel == 0 && p_data[id][P_LEVEL] >= 6 )
			{
				SM_SetSkillLevel( id, iRandomSkill, iSkillLevel + 1 );
			}

			// Give them a skill if we can
			else if ( SM_GetSkillType( iRandomSkill ) == SKILL_TYPE_TRAINABLE && iSkillLevel != MAX_SKILL_LEVEL && p_data[id][P_LEVEL] > 2 * iSkillLevel )
			{
				SM_SetSkillLevel( id, iRandomSkill, iSkillLevel + 1 );
			}

			iSkillsUsed = SM_TotalSkillPointsUsed( id );
		}

		return;
	}

	// OK set up a menu!!!

	new szMsg[512], pos = 0, szSkillName[64];
	new iSkillCounter = 0, iSkillID = 0, iKeys = (1<<9), iSkillLevel;

	// Add the menu header
	pos += formatex( szMsg[pos], 512-pos, "%L", id, "MENU_SELECT_SKILL" );

	iSkillID = SM_GetSkillByPos( id, iSkillCounter );

	while ( iSkillID != -1 )
	{
		iSkillLevel = SM_GetSkillLevel( id, iSkillID );

		LANG_GetSkillName( iSkillID , id, szSkillName, 63 );
		
		// Add the trainable skills to the menu
		if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_TRAINABLE )
		{

			// Only add it to the menu if they don't have level 3 already!
			if ( iSkillLevel < MAX_SKILL_LEVEL )
			{

				// User isn't high enough of a level to select this skill yet
				if ( p_data[id][P_LEVEL] <= 2 * iSkillLevel )
				{
					pos += formatex( szMsg[pos], 512-pos, "\d" );
				}

				// Then the user can choose it!
				else
				{
					iKeys |= (1<<iSkillCounter);
				}

				pos += formatex( szMsg[pos], 512-pos, "^n%d. %s %L %d\w", iSkillCounter+1, szSkillName, id, "WORD_LEVEL", iSkillLevel + 1 );
			}
		}
		
		// Add the ultimate to the menu
		else if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_ULTIMATE )
		{

			if ( iSkillLevel < MAX_ULTIMATE_LEVEL )
			{
				// User can't choose ultimate yet :/
				if ( p_data[id][P_LEVEL] <= 5 )
				{
					pos += formatex( szMsg[pos], 512-pos, "\d" );
				}

				// Then the user is level 6 or above and can select their ultimate!
				else
				{
					iKeys |= (1<<iSkillCounter);
				}

				pos += formatex( szMsg[pos], 512-pos, "^n%d. %L: %s\w", iSkillCounter+1, id, "WORD_ULTIMATE", szSkillName );
			}
		}

		iSkillCounter++;
		iSkillID = SM_GetSkillByPos( id, iSkillCounter );
	}

	// Add the cancel button to the menu
	pos += formatex( szMsg[pos], 512-pos, "^n^n0. %L", id, "WORD_CANCEL" );

	// Show the menu!
	show_menu( id, iKeys, szMsg, -1 );

	return;
}

public _MENU_SelectSkill( id, iKey )
{

	if ( !WC3_Check() || iKey == 9 )
	{
		return;
	}

	// Determine which key was just selected
	new iSkillID = SM_GetSkillByPos( id, iKey );

	// Get the user's current skill level
	new iCurrentLevel = SM_GetSkillLevel( id, iSkillID );

	// Add one to their level!
	SM_SetSkillLevel( id, iSkillID, iCurrentLevel + 1 );

	new iSkillsUsed = SM_TotalSkillPointsUsed( id );
	
	// Then they have another skill to select!!
	if ( iSkillsUsed < p_data[id][P_LEVEL] )
	{
		MENU_SelectSkill( id );
	}

	// No more, lets show them their latest level/XP
	else
	{
		WC3_ShowBar( id );
	}

	//*****************************
	// Skill Checks
	//*****************************
	
	switch ( iSkillID )
	{
		// Human's Devotion Aura
		case SKILL_DEVOTION:
		{
			if ( is_user_alive( id ) )
			{
				set_user_health( id, get_user_health( id ) + 15 );
			}
		}

		// Shadow Hunter's Serpent Ward
		case SKILL_SERPENTWARD:
		{
			p_data[id][P_SERPENTCOUNT]++;
		}

		// Warden's Blink
		case SKILL_BLINK:
		{
			if ( !p_data_b[id][PB_WARDENBLINK] )
			{
				WA_Blink( id );
			}
		}

		// Warden's Shadow Strike
		case SKILL_SHADOWSTRIKE:
		{
			p_data[id][P_SHADOWCOUNT]--;
		}
		
		// Crypt Lord's Carrion Beetles
		case SKILL_CARRIONBEETLES:
		{
			p_data[id][P_CARRIONCOUNT]--;
		}
	}

	// User selected an ultimate
	if ( SM_GetSkillType( iSkillID ) == SKILL_TYPE_ULTIMATE )
	{
		Ultimate_Ready( id );
	}

	// Check to see if they should be more invisible
	SHARED_INVIS_Set( id );

	// Undead's Unholy Aura
	SHARED_SetGravity( id );

	// Set the user's speed
	SHARED_SetSpeed( id );

	return;
}
