// **************************************************
// Shopmenu One
// **************************************************

public MENU_Shopmenu( id, iStart )
{
	if ( !WAR3_Check( id ) || !ITEM_CanBuy( id ) )
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
	else if ( iStart == 9 )
	{
		pos += format( szMenu[pos], 511-pos, "%L", id, "MENU_BUY_ITEM2" );
	}

	// Lets add the items to the menu!
	for ( i = 0; i < 9; i++ )
	{
		iItemID = iStart + i;

		lang_GetItemName( iItemID, id, szItemName, 63 );

		// These items don't exist in DOD
		if ( g_MOD == GAME_DOD && ( iItemID == ITEM_CHAMELEON  || iItemID == ITEM_SCROLL ) )
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

	pos += format( szMenu[pos], 511-pos, "^n\w0. %L", id, "EXIT_STRING" );

	show_menu( id, iKeys, szMenu, -1 );
}

public _menu_Shopmenu_One( id, iKey )
{
	if ( !WAR3_Check() || iKey == 9 )
		return PLUGIN_HANDLED;

	if ( iKey == ITEM_TOME )
		ITEM_Tome( id );

	else
		ITEM_Buy( id, iKey );

	return PLUGIN_HANDLED;
}

public _menu_Shopmenu_Two( id, iKey )
{
	if ( !WAR3_Check() || iKey == 9 )
		return PLUGIN_HANDLED;

	ITEM_Buy( id, iKey );

	return PLUGIN_HANDLED;
}

public menu_Select_Skill(id,saychat){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if (p_data[id][P_RACE] == 0){
		if (saychat==1){
			set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
			show_hudmessage(id,"%L",id,"SELECT_RACE_BEFORE_SKILLS")
		}
		else{
			console_print(id,"%L",id,"SELECT_RACE_BEFORE_SKILLS")
		}
		return PLUGIN_HANDLED
	}

	new message[256]
	new temp[128]

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]

	if (skillsused>=p_data[id][P_LEVEL]){
		if (saychat==1){
			set_hudmessage(200, 100, 0, -1.0, 0.3, 0, 1.0, 5.0, 0.1, 0.2, 2)
			show_hudmessage(id,"%L",id,"ALREADY_SELECTED_SKILL_POINTS")
		}
		else{
			console_print(id,"%L",id,"ALREADY_SELECTED_SKILL_POINTS")
		}
		return PLUGIN_HANDLED
	}

	if (is_user_bot(id)){
		new randomskill
		while (skillsused < p_data[id][P_LEVEL]){
			randomskill = random_num(1,3)
			if (p_data[id][P_ULTIMATE]==0 && p_data[id][P_LEVEL]>=6)
				p_data[id][P_ULTIMATE]=1
			else if (p_data[id][randomskill]!=3 && p_data[id][P_LEVEL]>2*p_data[id][randomskill]){
				++p_data[id][randomskill]
			}
			skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
		}
		return PLUGIN_HANDLED
	}

	format(message,255,"%L",id,"MENU_SELECT_SKILL")


	new skillcounter = 0
	new skillcurrentrace[4][64]

	while (skillcounter < 4){
		new race_skill[RACE_SKILL_LENGTH]
		lang_GetSkillName(p_data[id][P_RACE],skillcounter,id,race_skill,RACE_SKILL_LENGTH_F)
		copy(skillcurrentrace[skillcounter],63,race_skill)

		++skillcounter
	}

	skillcounter = 1
	while (skillcounter< 4){
		if (p_data[id][skillcounter]!=3){
			if (p_data[id][P_LEVEL]<=2*p_data[id][skillcounter]){
				format(temp,127,"\d")
				add(message,255,temp)
			}
			new race_skill[RACE_SKILL_LENGTH]
			lang_GetSkillName(p_data[id][P_RACE],skillcounter,id,race_skill,RACE_SKILL_LENGTH_F)

			format(temp,127,"%L",id,"LEVEL_SELECT_SKILL_FUNC",skillcounter,race_skill,p_data[id][skillcounter]+1)
			add(message,255,temp)
		}
		++skillcounter
	}
	if (p_data[id][P_ULTIMATE]==0){
		if (p_data[id][P_LEVEL]<=5){
			format(temp,127,"\d")
			add(message,255,temp)
		}
		new race_skill[RACE_SKILL_LENGTH]
		lang_GetSkillName(p_data[id][P_RACE],4,id,race_skill,RACE_SKILL_LENGTH_F)

		format(temp,127,"%L",id,"ULTIMATE_SELECT_SKILL_FUNC",race_skill)
		add(message,255,temp)
	}

	new keys = (1<<9)

	if (p_data[id][P_SKILL1]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL1] && skillsused<p_data[id][P_LEVEL])
		keys |= (1<<0)
	if (p_data[id][P_SKILL2]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL2] && skillsused<p_data[id][P_LEVEL])
		keys |= (1<<1)
	if (p_data[id][P_SKILL3]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL3] && skillsused<p_data[id][P_LEVEL])
		keys |= (1<<2)
	if (p_data[id][P_ULTIMATE]==0 && p_data[id][P_LEVEL]>=6 && skillsused<p_data[id][P_LEVEL])
		keys |= (1<<3)

	format(temp,127,"%L",id,"CANCEL_SELECT_SKILL_FUNC")
	add(message,255,temp)
	show_menu(id,keys,message,-1)
	if (saychat==1)
		return PLUGIN_CONTINUE
	return PLUGIN_HANDLED
}

public _menu_Select_Skill(id,key){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]

	if (key == KEY_1 && p_data[id][P_SKILL1]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL1] && skillsused<p_data[id][P_LEVEL])
		++p_data[id][P_SKILL1]
	else if (key == KEY_2 && p_data[id][P_SKILL2]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL2] && skillsused<p_data[id][P_LEVEL])
		++p_data[id][P_SKILL2]
	else if (key == KEY_3 && p_data[id][P_SKILL3]!=3 && p_data[id][P_LEVEL]>2*p_data[id][P_SKILL3] && skillsused<p_data[id][P_LEVEL])
		++p_data[id][P_SKILL3]
	else if (key == KEY_4 && p_data[id][P_ULTIMATE]==0 && p_data[id][P_LEVEL]>=6 && skillsused<p_data[id][P_LEVEL]){
		p_data[id][P_ULTIMATE]=1
	}
	else if (key == KEY_0)
		return PLUGIN_HANDLED

	skillsused = p_data[id][P_SKILL1]+p_data[id][P_SKILL2]+p_data[id][P_SKILL3]+p_data[id][P_ULTIMATE]
	if (skillsused < p_data[id][P_LEVEL])
		menu_Select_Skill(id,0)
	else
		WC3_ShowBar( id );

	// Serpent Ward Chosen
	if ( Verify_Skill(id, RACE_SHADOW, SKILL3) && key == KEY_3 ){
		p_data[id][P_SERPENTCOUNT]++
	}
	// Carrion Beetle Chosen
	if ( Verify_Skill(id, RACE_CRYPT, SKILL3) &&  key == KEY_3 ){
		if( p_data[id][P_CARRIONCOUNT] < 3 ){
			p_data[id][P_CARRIONCOUNT]++
		}
	}
	// Shadow Strike Chosen
	if ( Verify_Skill(id, RACE_WARDEN, SKILL3) && key == KEY_3 ){
		if (p_data[id][P_SHADOWCOUNT] < 3 ){
			p_data[id][P_SHADOWCOUNT] = 2
		}
	}
	// Devotion Aura Chosen
	if ( Verify_Skill(id, RACE_HUMAN, SKILL2) && key == KEY_2 && is_user_alive(id)){
		if(p_data[id][P_SKILL2]==1)
			set_user_health(id,get_user_health(id) + (p_devotion[0] - 100))
		else if(p_data[id][P_SKILL2]==2)
			set_user_health(id,get_user_health(id) + (p_devotion[1] - p_devotion[0]))
		else if(p_data[id][P_SKILL2]==3)
			set_user_health(id,get_user_health(id) + (p_devotion[2] - p_devotion[1]))
	}

	// Check to see if they should be more invisible
	SHARED_INVIS_Set( id );

	return PLUGIN_HANDLED
}

public menu_War3menu(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new pos = 0, i, menu_body[512], menuitems[5][32]
	new keys = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<9)

	format(menuitems[0],31,"%L",id,"SKILLS_MENU")
	format(menuitems[1],31,"%L",id,"RACE_MENU")
	format(menuitems[2],31,"%L",id,"ITEM_MENU")
	format(menuitems[3],31,"%L",id,"HELP")
	format(menuitems[4],31,"%L",id,"ADMIN_MENU_TITLE")

	pos += format(menu_body[pos], 511-pos, "%L^n^n",id,"MENU_WAR3_FT_MENU")
	for (i = 0; i<5; i++){
		pos += format(menu_body[pos], 511-pos, "\w%d. %s^n",i+1,menuitems[i])
	}
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_HANDLED
}

public _menu_War3menu(id,key){

	switch (key){
		case 0:	menu_Skill_Options(id)
		case 1:	menu_Race_Options(id)
		case 2:	menu_Item_Options(id)
		case 3:	MOTD_War3help(id)
		case 4:	menu_Admin_Options(id)
		default:	return PLUGIN_HANDLED
	}
	
	return PLUGIN_HANDLED
}

public menu_Skill_Options(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

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
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Skill_Options(id,key){

	switch (key){
		case 0:	menu_Select_Skill(id,1)
		case 1:	MOTD_Skillsinfo(id)
		case 2:	WC3_HandleCommand( id, "resetskills" );
		case 8: menu_War3menu(id)
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_Race_Options(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

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
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Race_Options(id,key){

	switch (key){
		case 0:	WC3_ChangeRaceStart( id );
		case 1:	WC3_ShowRaceInfo( id );
		case 2:	menu_ResetXP(id)
		case 3:	MOTD_Playerskills(id, 1)
		case 8: menu_War3menu(id)
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_Item_Options(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

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
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Item_Options(id,key){

	switch (key){
		case 0:	MENU_Shopmenu( id, 0 );
		case 1:	MENU_Shopmenu( id, 9 );
		case 2:	MOTD_ItemsInfo( id, 0 );
		case 3:	MOTD_ItemsInfo( id, 9 );
		case 8: menu_War3menu(id)
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_Admin_Options(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

    if ( id && !( get_user_flags( id ) & XP_get_admin_flag() ) )
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
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_Admin_Options(id,key){

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
		case 2: XP_SaveAll()
		case 8: menu_War3menu(id)
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_PlayerXP_Options(id,pos){

	if (!warcraft3)
		return PLUGIN_CONTINUE

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
		format(exitstring,15,"%L",id,"EXIT_STRING")
		format(menuBody[len],511-len,"^n0. %s", pos ? back : exitstring)
	}


	show_menu(id,keys,menuBody,-1)
	return PLUGIN_CONTINUE

}

public _menu_PlayerXP_Options(id,key){

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
		case 9: return PLUGIN_HANDLED
		default:{
			new player = g_menuPlayers[id][g_menuPosition[id] * 7 + key]
			client_print(player,print_chat,"%s %L",g_MODclient, id,"THE_ADMIN_JUST_GAVE_YOU_XP",g_menuSettings[id])
			p_data[player][P_XP] += g_menuSettings[id]

			XP_Check( player );

			menu_PlayerXP_Options(id,g_menuPosition[id])
		}
	}
	return PLUGIN_HANDLED
}

public menu_TeamXP_Options(id){

	if (!warcraft3)
		return PLUGIN_CONTINUE

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
	pos += format(menu_body[pos], 511-pos, "^n\w0. %L",id,"EXIT_STRING")
	show_menu(id,keys,menu_body,-1)

	return PLUGIN_CONTINUE
}

public _menu_TeamXP_Options(id,key){

	switch(key){
		case 0:{
			_Admin_GiveXP(id, "@TERRORIST", g_menuSettings[id])
			menu_TeamXP_Options(id)
		}
		case 1:{
			_Admin_GiveXP(id, "@CT", g_menuSettings[id])
			menu_TeamXP_Options(id)
		}
		case 2:{
			_Admin_GiveXP(id, "@ALL", g_menuSettings[id])
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
		case 9: return PLUGIN_HANDLED
		default: return PLUGIN_HANDLED
	}
	return PLUGIN_HANDLED
}

public menu_ResetXP(id)
{

	if (!warcraft3)
		return PLUGIN_CONTINUE;

	new szMenu[512];
	new keys = (1<<0)|(1<<1)|(1<<9);
	
	format( szMenu, 511, "%L^n^n\w1. Yes^n\w2. No", id, "MENU_RESET_XP" );

	show_menu(id, keys, szMenu, -1);

	return PLUGIN_CONTINUE;
}

public _menu_ResetXP( id, key )
{
	
	// User selected yes
	if ( key == 0 )
	{
		XP_Reset(id);
	}
	
	return;
}

// Function will display the changerace menu
public MENU_SelectRace( id, iRaceXP[MAX_RACES] )
{
	
	if ( !WAR3_Check( id ) )
	{
		return;
	}

	new szRaceName[MAX_RACES+1][64], i, pos, iKeys = 0, szMenu[512], szXP[16];

	// Get our race names
	for ( i = 0; i < get_pcvar_num( CVAR_wc3_races ); i++ )
	{
		lang_GetRaceName( i + 1, id, szRaceName[i], 63 );
	}

	pos += formatex( szMenu[pos], 512-pos, "%L", id, "MENU_SELECT_RACE" );
	pos += formatex( szMenu[pos], 512-pos, "%L", id, "SELECT_RACE_TITLE" );

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

public _MENU_SelectRace( id, key )
{

	if ( !WAR3_Check() )
	{
		return;
	}
	
	// User pressed 0 (cancel)
	if( get_pcvar_num( CVAR_wc3_races ) < 9 && key-1 == get_pcvar_num( CVAR_wc3_races ) )
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
