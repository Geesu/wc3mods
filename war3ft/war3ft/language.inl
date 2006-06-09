
#define TOTAL_MENUS 12


// This is the list of menu titles that are in war3ft.txt
new const MENU_NAMES[TOTAL_MENUS][] = 
{
	"MENU_BUY_ITEM",
	"MENU_BUY_ITEM2",
	"MENU_SELECT_SKILL",
	"MENU_SELECT_RACE",
	"MENU_WAR3_FT_MENU",
	"MENU_SKILLS_OPTIONS",
	"MENU_ITEM_OPTIONS",
	"MENU_RACE_OPTIONS",
	"MENU_ADMIN_MENU",
	"MENU_GIVE_PLAYERS_XP",
	"MENU_TEAM_XP",
	"MENU_RESET_XP"
};

// Callback functions for the above menu names
new const MENU_CALLBACK[TOTAL_MENUS][] = 
{
	"_menu_Shopmenu_One",
	"_menu_Shopmenu_Two",
	"_menu_Select_Skill",
	"_MENU_SelectRace",
	"_menu_War3menu",
	"_menu_Skill_Options",
	"_menu_Item_Options",
	"_menu_Race_Options",
	"_menu_Admin_Options",
	"_menu_PlayerXP_Options",
	"_menu_TeamXP_Options",
	"_menu_ResetXP"
};

/*
	Description: Function will register all menus for all languages
*/
public lang_SetMenus(){

	new total_languages = get_langsnum();
	new lang[3], menu[128];
	new curMenuId = -1, highestMenuId = -1;
	new iLang, iMenu;


	// Loop through every menu

	for ( iMenu = 0; iMenu < TOTAL_MENUS; iMenu++ )
	{
		// Register the menu names for each language

		for( iLang = 0; iLang < total_languages; iLang++ )
		{
			get_lang ( iLang, lang );

			if( lang_exists( lang ) )
			{
				formatex ( menu, 127, "%L", lang, MENU_NAMES[iMenu] );
				curMenuId = register_menuid ( menu );

				if( curMenuId > highestMenuId )
				{
					register_menucmd ( curMenuId, 1023, MENU_CALLBACK[iMenu] );
					highestMenuId = curMenuId;
				}
			}
		}// End language loop
	}// End menu loop
}

/*
	Description: Function will return the race name based on the race id and the language of the user
*/
lang_GetRaceName ( race_id, id, race_name[], len, bool:shortLookup = false )
{

	new szRaceHelper[64];
	
	if ( shortLookup == true )
	{
		formatex( szRaceHelper, 63, "SHORT_RACENAME_%d", race_id );
	}
	else
	{
		formatex( szRaceHelper, 63, "RACENAME_%d", race_id );
	}

	// Lookup the race name

	formatex( race_name, len-1, "%L", id, szRaceHelper );
}

/*
	Description: Function will return the shopmenu item name based on the item id and the language of the user
*/
lang_GetItemName ( item_id, id, item_name[], len, shop_id, bool:shortLookup = false )
{
	
	if ( item_id == 0 )
	{
		return;
	}

	new szItemHelper[64];

	if ( shortLookup == true )
	{
		if ( shop_id == 1 )
		{
			formatex( szItemHelper, 63, "SHORT_ITEMNAME_%d", item_id );
		}
		else
		{
			formatex( szItemHelper, 63, "SHORT_ITEMNAME%d_%d", shop_id, item_id );
		}
	}
	else
	{
		if ( shop_id == 1 )
		{
			formatex( szItemHelper, 63, "ITEMNAME_%d", item_id );
		}
		else
		{
			formatex( szItemHelper, 63, "ITEMNAME%d_%d", shop_id, item_id );
		}
	}

	// Lookup the race name

	formatex( item_name, len-1, "%L", id, szItemHelper );
}

/*
	Description: Function will return the skill name based on the race id, skill id and the language of the user
*/
lang_GetSkillName( race_id, skill_id, id, skill_name[], len )
{	

	// Handle any game-specific instructions first

	if ( g_MOD == GAME_DOD && race_id == RACE_ORC && skill_id == SKILL3 )
	{
		formatex ( skill_name, len-1, "%L", id, "DOD_RACE3_SKILL3" );
	}

	// Handle everything else

	else if ( race_id > 0 && race_id < 9 )
	{
		new szSkillHelper[64];

		formatex ( szSkillHelper, 63, "RACE%d_SKILL%d", race_id, skill_id );
		formatex ( skill_name, len-1, "%L", id, szSkillHelper );
	}
	else if ( race_id == RACE_CHAMELEON )
	{
		new szSkillHelper[64];

		formatex ( szSkillHelper, 63, "RACE%d_SKILL%d", g_ChamSkills[skill_id], skill_id );
		formatex ( skill_name, len-1, "%L", id, szSkillHelper );
	}
	else if ( race_id != 0 )
	{
		log_amx ( "Race: %d, skill: %d not found", race_id, skill_id );
		
		formatex ( skill_name, len-1, "" );
	}
}

// We really should just build an HTML file when the server starts then send that to the user (like war3x)

lang_GetSkillInfo( race_id, skill_id, id, skill_description[], len )
{

	switch( race_id )
	{

		// Undead Scourge

		case RACE_UNDEAD:
		{
			switch( skill_id )
			{
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "RACE1_SKILL1_INFO",floatround(p_vampiric[0]*100), floatround(p_vampiric[1]*100), floatround(p_vampiric[2]*100) );
			#if MOD == 1
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "DOD_RACE1_SKILL2_INFO" );
			#endif
			#if MOD == 0
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "RACE1_SKILL2_INFO" );
			#endif
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "RACE1_SKILL3_INFO" );
				case SKILL4: formatex ( skill_description, len-1, "%L", id, "RACE1_SKILL4_INFO" );
			}
		}

		// Human Alliance

		case RACE_HUMAN:
		{
			switch(skill_id)
			{
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "RACE2_SKILL1_INFO" );
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "RACE2_SKILL2_INFO", p_devotion[0], p_devotion[1], p_devotion[2] );
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "RACE2_SKILL3_INFO", floatround(p_bash[0]*100), floatround(p_bash[1]*100), floatround(p_bash[2]*100) );
				case SKILL4: formatex ( skill_description, len-1, "%L", id, "RACE2_SKILL4_INFO");
			}
		}

		// Orcish Horde

		case RACE_ORC:
		{
			switch(skill_id)
			{
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "RACE3_SKILL1_INFO", floatround(p_critical[0]*100) );
			#if MOD == 1
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "DOD_RACE3_SKILL2_INFO", floatround(p_grenade[0]), floatround(p_grenade[1]), floatround(p_grenade[2]) );
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "DOD_RACE3_SKILL3_INFO", floatround(p_ankh[0]*100), floatround(p_ankh[1]*100), floatround(p_ankh[2]*100) );
			#endif
			#if MOD == 0
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "RACE3_SKILL2_INFO", floatround(p_grenade[0]), floatround(p_grenade[1]), floatround(p_grenade[2]) );
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "RACE3_SKILL3_INFO", floatround(p_ankh[0]*100), floatround(p_ankh[1]*100), floatround(p_ankh[2]*100) );
			#endif
				case SKILL4: formatex ( skill_description, len-1, "%L", id, "RACE3_SKILL4_INFO" );
			}
		}

		// Night Elf

		case RACE_ELF:
		{
			switch(skill_id)
			{
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "RACE4_SKILL1_INFO", floatround(p_evasion[0]*100), floatround(p_evasion[1]*100), floatround(p_evasion[2]*100) );
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "RACE4_SKILL2_INFO", floatround(p_thorns[0]*100), floatround(p_thorns[1]*100), floatround(p_thorns[2]*100) );
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "RACE4_SKILL3_INFO", floatround(p_trueshot[0]*100), floatround(p_trueshot[1]*100), floatround(p_trueshot[2]*100) );
				case SKILL4: formatex ( skill_description, len-1, "%L", id, "RACE4_SKILL4_INFO" );
			}
		}

		// Blood Mage

		case RACE_BLOOD:
		{
			switch(skill_id)
			{
			#if MOD == 1
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "DOD_RACE5_SKILL1_INFO", p_pheonix_dod[0], p_pheonix_dod[1], p_pheonix_dod[2] );
			#endif
			#if MOD == 0
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "RACE5_SKILL1_INFO", floatround(p_pheonix[0]*100), floatround(p_pheonix[1]*100), floatround(p_pheonix[2]*100) );
			#endif
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "RACE5_SKILL2_INFO", floatround(p_banish[0]*100), floatround(p_banish[1]*100), floatround(p_banish[2]*100) );
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "RACE5_SKILL3_INFO", floatround(p_mana[0]*100), floatround(p_mana[1]*100), floatround(p_mana[2]*100) );
				case SKILL4: formatex ( skill_description, len-1, "%L", id, "RACE5_SKILL4_INFO" );
				case SKILL5: formatex ( skill_description, len-1, "%L", id, "RACE5_SKILL5_INFO", (100.0 * p_resistant[p_data[id][P_LEVEL]]) );
			}
		}

		// Shadow Hunter

		case RACE_SHADOW:
		{
			switch(skill_id)
			{
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "RACE6_SKILL1_INFO", floatround(p_heal[0]), floatround(p_heal[1]), floatround(p_heal[2]) );
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "RACE6_SKILL2_INFO", floatround(p_hex[0]*100), floatround(p_hex[1]*100), floatround(p_hex[2]*100) );
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "RACE6_SKILL3_INFO", p_serpent[0], p_serpent[1], p_serpent[2])
				case SKILL4: formatex ( skill_description, len-1, "%L", id, "RACE6_SKILL4_INFO" );
				case SKILL5: formatex ( skill_description, len-1, "%L", id, "RACE6_SKILL5_INFO",(100.0 * p_concoction[p_data[id][P_LEVEL]]), SH_CONCOCTION_DAMAGE)
			}
		}

		// Warden

		case RACE_WARDEN:
		{
			switch(skill_id)
			{
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "RACE7_SKILL1_INFO",  floatround(p_fan[0]*100), floatround(p_fan[1]*100), floatround(p_fan[2]*100) );
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "RACE7_SKILL2_INFO", floatround(p_blink[0]*100), floatround(p_blink[1]*100), floatround(p_blink[2]*100) );
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "RACE7_SKILL3_INFO", floatround(p_shadow[0]*100), floatround(p_shadow[1]*100), floatround(p_shadow[2]*100) );
				case SKILL4: formatex ( skill_description, len-1, "%L", id, "RACE7_SKILL4_INFO" );
				case SKILL5: formatex ( skill_description, len-1, "%L", id, "RACE7_SKILL5_INFO", (100.0 * p_harden[p_data[id][P_LEVEL]]) );
			}
		}

		// Crypt Lord

		case RACE_CRYPT:
		{
			switch(skill_id)
			{
				case SKILL1: formatex ( skill_description, len-1, "%L", id, "RACE8_SKILL1_INFO", floatround(p_impale[0]*100), floatround(p_impale[1]*100), floatround(p_impale[2]*100) );
				case SKILL2: formatex ( skill_description, len-1, "%L", id, "RACE8_SKILL2_INFO", floatround(p_spiked[0]*100), floatround(p_spiked[1]*100), floatround(p_spiked[2]*100) );
				case SKILL3: formatex ( skill_description, len-1, "%L", id, "RACE8_SKILL3_INFO", floatround(p_carrion[0]*100), floatround(p_carrion[1]*100), floatround(p_carrion[2]*100) );
				case SKILL4: formatex ( skill_description, len-1, "%L", id, "RACE8_SKILL4_INFO" );
				case SKILL5: formatex ( skill_description, len-1, "%L", id, "RACE8_SKILL5_INFO", (100.0 * p_orb[p_data[id][P_LEVEL]]), ORB_DAMAGE)
			}
		}
	}
}
