public Lang_Set_Menus(){
	#if ADVANCED_DEBUG
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
				register_menucmd(curMenuId,1023,"_menu_Shopmenu_One")
				highestMenuId = curMenuId
			}

			format(menu,127,"%L",lang,"MENU_BUY_ITEM2")
			curMenuId = register_menuid(menu)
			if(curMenuId>highestMenuId){
				register_menucmd(curMenuId,1023,"_menu_Shopmenu_Two")
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
	#if ADVANCED_DEBUG
		writeDebugInfo("racename",id)
	#endif

	new race_lookup[128]
	format(race_lookup,127,"RACENAME_%d",race)
	format(race_name,length-1,"%L",id,race_lookup)
}

public shortracename(race,id,short_race_name[],length){
	#if ADVANCED_DEBUG
		writeDebugInfo("shortracename",id)
	#endif

	new race_lookup[128]
	format(race_lookup,127,"SHORT_RACENAME_%d",race)
	format(short_race_name,length-1,"%L",id,race_lookup)
}

public itemname(item,id,item_name[],length){
	#if ADVANCED_DEBUG
		writeDebugInfo("itemname",id)
	#endif

	new item_lookup[128]
	format(item_lookup,127,"ITEMNAME_%d",item)
	format(item_name,length-1,"%L",id,item_lookup)
}

public shortitemname(item,id,short_item_name[],length){
	#if ADVANCED_DEBUG
		writeDebugInfo("shortitemname",id)
	#endif

	new item_lookup[128]
	format(item_lookup,127,"SHORT_ITEMNAME_%d",item)
	format(short_item_name,length-1,"%L",id,item_lookup)
}

public itemname2(item,id,item_name2[],length){
	#if ADVANCED_DEBUG
		writeDebugInfo("itemname2",id)
	#endif

	new item_lookup[128]
	format(item_lookup,127,"ITEMNAME2_%d",item)
	format(item_name2,length-1,"%L",id,item_lookup)
}

public shortitemname2(item,id,short_item_name[],length){
	#if ADVANCED_DEBUG
		writeDebugInfo("shortitemname2",id)
	#endif

	new item_lookup[128]
	format(item_lookup,127,"SHORT_ITEMNAME2_%d",item)
	format(short_item_name,length-1,"%L",id,item_lookup)
}


// This function will return the skill NAME, not info
public raceskill(race, skill, id, skill_name[],length){
	#if ADVANCED_DEBUG
		writeDebugInfo("raceskill",id)
	#endif

#if MOD == 1
	if(race == RACE_ORC && skill == SKILL3){
		format(skill_name,length-1,"%L",id,"DOD_RACE3_SKILL3")
	}
	else if(race > 0 && race < 9){
#endif
#if MOD == 0
	if(race > 0 && race < 9){
#endif
		new skill_lookup[64]
		format(skill_lookup,63,"RACE%d_SKILL%d",race,skill)
		format(skill_name,length-1,"%L",id,skill_lookup)
	}else if(race == RACE_CHAMELEON){
		new skill_lookup[64]
		format(skill_lookup,63,"RACE%d_SKILL%d",race9Options[skill],skill)
		format(skill_name,length-1,"%L",id,skill_lookup)
	}else if(race != 0 ){
		log_amx("[%s] ERROR, Race: %d, skill: %d not found in raceskill function",g_MOD,race,skill)
		format(skill_name,length-1,"")
	}
}

public Lang_Hero_Skill_Name(race, skill, id, skill_name[], iDescLen){
	new skill_lookup[64]
	format(skill_lookup,63,"RACE%d_SKILL5_NAME",race)
	format(skill_name,iDescLen-1,"%L",id,skill_lookup)
}

public Lang_Hero_Skill_Info(race, skill, id, skill_description[], iDescLen){
	new skill_lookup[64]
	format(skill_lookup,63,"RACE%d_SKILL5_INFO",race)

	switch(race){
		case RACE_BLOOD:	format(skill_description,iDescLen-1,"%L",id,skill_lookup,(100.0 * p_resistant[p_data[id][P_LEVEL]]))
		case RACE_SHADOW:	format(skill_description,iDescLen-1,"%L",id,skill_lookup,(100.0 * p_concoction[p_data[id][P_LEVEL]]), CONCOCTION_DAMAGE)
		case RACE_WARDEN:	format(skill_description,iDescLen-1,"%L",id,skill_lookup,(100.0 * p_harden[p_data[id][P_LEVEL]]))
		case RACE_CRYPT:	format(skill_description,iDescLen-1,"%L",id,skill_lookup,(100.0 * p_orb[p_data[id][P_LEVEL]]), ORB_DAMAGE)
	}
}

public Lang_Skill_Info(race, skill, id, skill_description[], iDescLen){

	switch(race){
		case RACE_UNDEAD:{
			switch(skill){
				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"RACE1_SKILL1_INFO",floatround(p_vampiric[0]*100),floatround(p_vampiric[1]*100),floatround(p_vampiric[2]*100))
			#if MOD == 1
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"DOD_RACE1_SKILL2_INFO")
			#else
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"RACE1_SKILL2_INFO")
			#endif
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"RACE1_SKILL3_INFO")
				case SKILL4: format(skill_description,iDescLen-1,"%L",id,"RACE1_SKILL4_INFO")
			}
		}
		case RACE_HUMAN:{
			switch(skill){

				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"RACE2_SKILL1_INFO")
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"RACE2_SKILL2_INFO",p_devotion[0],p_devotion[1],p_devotion[2])
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"RACE2_SKILL3_INFO",floatround(p_bash[0]*100),floatround(p_bash[1]*100),floatround(p_bash[2]*100))
				case SKILL4: {
					if (iCvar[FT_BLINKENABLED])
						format(skill_description,iDescLen-1,"%L",id,"RACE2_SKILL4_INFO_B")
					else
						format(skill_description,iDescLen-1,"%L",id,"RACE2_SKILL4_INFO_T")
				}
			}
		}
		case RACE_ORC:{
			switch(skill){
				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"RACE3_SKILL1_INFO",floatround(p_critical[0]*100))
			#if MOD == 1
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"DOD_RACE3_SKILL2_INFO",floatround(p_grenade[0]),floatround(p_grenade[1]),floatround(p_grenade[2]))
			#else
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"RACE3_SKILL2_INFO",floatround(p_grenade[0]),floatround(p_grenade[1]),floatround(p_grenade[2]))
			#endif
			#if MOD == 1
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"DOD_RACE3_SKILL3_INFO",floatround(p_ankh[0]*100),floatround(p_ankh[1]*100),floatround(p_ankh[2]*100))
			#else
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"RACE3_SKILL3_INFO",floatround(p_ankh[0]*100),floatround(p_ankh[1]*100),floatround(p_ankh[2]*100))
			#endif
				case SKILL4: format(skill_description,iDescLen-1,"%L",id,"RACE3_SKILL4_INFO")
			}
		}
		case RACE_ELF: {
			switch(skill){
				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"RACE4_SKILL1_INFO",floatround(p_evasion[0]*100),floatround(p_evasion[1]*100),floatround(p_evasion[2]*100))
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"RACE4_SKILL2_INFO",floatround(p_thorns[0]*100),floatround(p_thorns[1]*100),floatround(p_thorns[2]*100))
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"RACE4_SKILL3_INFO",floatround(p_trueshot[0]*100),floatround(p_trueshot[1]*100),floatround(p_trueshot[2]*100))
				case SKILL4: format(skill_description,iDescLen-1,"%L",id,"RACE4_SKILL4_INFO")
			}
		}
		case RACE_BLOOD :{
			switch(skill){
			#if MOD == 0
				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"RACE5_SKILL1_INFO",floatround(p_pheonix[0]*100),floatround(p_pheonix[1]*100),floatround(p_pheonix[2]*100))
			#endif
			#if MOD == 1
				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"DOD_RACE5_SKILL1_INFO",p_pheonix[0],p_pheonix[1],p_pheonix[2])
			#endif
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"RACE5_SKILL2_INFO",floatround(p_banish[0]*100),floatround(p_banish[1]*100),floatround(p_banish[2]*100))
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"RACE5_SKILL3_INFO",floatround(p_mana[0]*100),floatround(p_mana[1]*100),floatround(p_mana[2]*100))
				case SKILL4: format(skill_description,iDescLen-1,"%L",id,"RACE5_SKILL4_INFO")
			}
		}
		case RACE_SHADOW: {
			switch(skill){
				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"RACE6_SKILL1_INFO",floatround(p_heal[0]),floatround(p_heal[1]),floatround(p_heal[2]))
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"RACE6_SKILL2_INFO",floatround(p_hex[0]*100),floatround(p_hex[1]*100),floatround(p_hex[2]*100))
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"RACE6_SKILL3_INFO",p_serpent[0],p_serpent[1],p_serpent[2])
				case SKILL4: format(skill_description,iDescLen-1,"%L",id,"RACE6_SKILL4_INFO")
			}
		}
		case RACE_WARDEN: {
			switch(skill){
				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"RACE7_SKILL1_INFO",floatround(p_fan[0]*100),floatround(p_fan[1]*100),floatround(p_fan[2]*100))
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"RACE7_SKILL2_INFO",floatround(p_blink[0]*100),floatround(p_blink[1]*100),floatround(p_blink[2]*100))
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"RACE7_SKILL3_INFO",floatround(p_shadow[0]*100),floatround(p_shadow[1]*100),floatround(p_shadow[2]*100))
				case SKILL4: format(skill_description,iDescLen-1,"%L",id,"RACE7_SKILL4_INFO")
			}
		}
		case RACE_CRYPT: {
			switch(skill){
				case SKILL1: format(skill_description,iDescLen-1,"%L",id,"RACE8_SKILL1_INFO")
				case SKILL2: format(skill_description,iDescLen-1,"%L",id,"RACE8_SKILL2_INFO",floatround(p_spiked[0]*100),floatround(p_spiked[1]*100),floatround(p_spiked[2]*100))
				case SKILL3: format(skill_description,iDescLen-1,"%L",id,"RACE8_SKILL3_INFO",floatround(p_carrion[0]*100),floatround(p_carrion[1]*100),floatround(p_carrion[2]*100))
				case SKILL4: format(skill_description,iDescLen-1,"%L",id,"RACE8_SKILL4_INFO")
			}
		}
	}
}