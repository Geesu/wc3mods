public lang_Set_Menus(){
#if ADVANCED_DEBUG == 1
writeDebugInfo("lang_Set_Menus",0)
#endif

new total_languages = get_langsnum()
new lang[3], i = 0, menu[128]
new curMenuId = -1, highestMenuId = -1

for(i=0;i<total_languages;i++){
get_lang(i,lang)
if(lang_exists(lang)){

format(menu,127,"%L",lang,"MENU_BUY_ITEM")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"Item_Buy")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_BUY_ITEM2")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"Item_Buy2")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_SELECT_SKILL")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_Select_Skill")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_TELEPORT_TO")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_Teleport")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_SELECT_RACE")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_Select_Race")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_WAR3_FT_MENU")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_War3menu")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_SKILLS_OPTIONS")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_Skill_Options")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_ITEM_OPTIONS")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_Item_Options")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_RACE_OPTIONS")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_Race_Options")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_ADMIN_MENU")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_Admin_Options")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_GIVE_PLAYERS_XP")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_PlayerXP_Options")
highestMenuId = curMenuId
}

format(menu,127,"%L",lang,"MENU_TEAM_XP")
curMenuId = register_menuid(menu)
if(curMenuId>highestMenuId){
register_menucmd(curMenuId,1023,"_menu_TeamXP_Options")
highestMenuId = curMenuId
}
}
}
}

public racename(race,id,race_name[],length){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("racename",id)
	#endif

	new race_lookup[128]
	format(race_lookup,127,"RACENAME_%d",race)
	format(race_name,length,"%L",id,race_lookup)
}

public shortracename(race,id,short_race_name[],length){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("shortracename",id)
	#endif

	new race_lookup[128]
	format(race_lookup,127,"SHORT_RACENAME_%d",race)
	format(short_race_name,length,"%L",id,race_lookup)
}

public itemname(item,id,item_name[],length){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("itemname",id)
	#endif

	new item_lookup[128]
	format(item_lookup,127,"ITEMNAME_%d",item)
	format(item_name,length,"%L",id,item_lookup)
}

public shortitemname(item,id,short_item_name[],length){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("shortitemname",id)
	#endif

	new item_lookup[128]
	format(item_lookup,127,"SHORT_ITEMNAME_%d",item)
	format(short_item_name,length,"%L",id,item_lookup)
}

public itemname2(item,id,item_name2[],length){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("itemname2",id)
	#endif

	new item_lookup[128]
	format(item_lookup,127,"ITEMNAME2_%d",item)
	format(item_name2,length,"%L",id,item_lookup)
}

public shortitemname2(item,id,short_item_name[],length){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("shortitemname2",id)
	#endif

	new item_lookup[128]
	format(item_lookup,127,"SHORT_ITEMNAME2_%d",item)
	format(short_item_name,length,"%L",id,item_lookup)
}


// This function will return the skill NAME, not info
public raceskill(race, skill, id, skill_name[],length){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("raceskill",id)
	#endif

#if MOD == 1
	if(race == RACE_ORC && skill == SKILL3){
		format(skill_name,length,"%L",id,"DOD_RACE3_SKILL3")
	}
	else if(race > 0 && race < 9){
#endif
#if MOD == 0
	if(race > 0 && race < 9){
#endif
		new skill_lookup[128]
		format(skill_lookup,127,"RACE%d_SKILL%d",race,skill)
		format(skill_name,length,"%L",id,skill_lookup)
	}else if(race == 9){
		new skill_lookup[128]
		format(skill_lookup,127,"RACE%d_SKILL%d",race9Options[skill],skill)
		format(skill_name,length,"%L",id,skill_lookup)
	}else if(race != 0 ){
		log_amx("[%s] ERROR, Race: %d, skill: %d not found in raceskill function",g_MOD,race,skill)
		format(skill_name,length,"")
	}
}
