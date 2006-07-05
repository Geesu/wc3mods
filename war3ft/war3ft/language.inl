/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Language Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define TOTAL_MENUS 13


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
	"MENU_RESET_XP",
	"MENU_REPLACE_ITEM"
};

// Callback functions for the above menu names
new const MENU_CALLBACK[TOTAL_MENUS][] = 
{
	"_MENU_Shopmenu1",
	"_MENU_Shopmenu2",
	"_MENU_SelectSkill",
	"_MENU_SelectRace",
	"_menu_War3menu",
	"_menu_Skill_Options",
	"_menu_Item_Options",
	"_menu_Race_Options",
	"_menu_Admin_Options",
	"_menu_PlayerXP_Options",
	"_menu_TeamXP_Options",
	"_menu_ResetXP",
	"_menu_ReplaceItem"
};

/*
	Description: Function will register all menus for all languages
*/
public LANG_SetMenus()
{
	new iTotalLanguages = get_langsnum();
	new lang[3], menu[128];
	new curMenuId = -1, highestMenuId = -1;
	new iLang, iMenu;


	// Loop through every menu

	for ( iMenu = 0; iMenu < TOTAL_MENUS; iMenu++ )
	{
		// Register the menu names for each language

		for ( iLang = 0; iLang < iTotalLanguages; iLang++ )
		{
			get_lang ( iLang, lang );

			if ( lang_exists( lang ) )
			{
				formatex ( menu, 127, "%L", lang, MENU_NAMES[iMenu] );
				curMenuId = register_menuid ( menu );

				if ( curMenuId > highestMenuId )
				{
					register_menucmd ( curMenuId, 1023, MENU_CALLBACK[iMenu] );
					highestMenuId = curMenuId;
				}
			}
		}// End language loop
	}// End menu loop
}

//Function will return the race name based on the race id and the language of the user
lang_GetRaceName ( race_id, id, race_name[], len, bool:shortLookup = false )
{

	new szRaceHelper[64];
	
	if ( shortLookup == true )
	{
		formatex( szRaceHelper, 63, "RACE_S_%d", race_id );
	}
	else
	{
		formatex( szRaceHelper, 63, "RACE_%d", race_id );
	}

	// Lookup the race name
	formatex( race_name, len-1, "%L", id, szRaceHelper );
}

//Description: Function will return the shopmenu item name based on the item id and the language of the user
LANG_GetItemName ( item_id, id, item_name[], len, bool:shortLookup = false )
{
	
	if ( item_id < 0 )
	{
		return;
	}

	new szItemHelper[64];

	if ( shortLookup == true )
	{
		formatex( szItemHelper, 63, "ITEM_S_%d", item_id );
	}
	else
	{
		formatex( szItemHelper, 63, "ITEM_%d", item_id );
	}

	// Lookup the item name
	formatex( item_name, len, "%L", id, szItemHelper );
}

//Description: Function will return the skill name based on the race id, skill id and the language of the user
LANG_GetSkillName( skill_id, id, skill_name[], len )
{	

	// Handle any game-specific instructions first
	if ( g_MOD == GAME_DOD )
	{
		// Reincarnation
		if ( skill_id == SKILL_REINCARNATION )
		{
			formatex ( skill_name, len-1, "%L", id, "SKILL_10_DOD" );
			return;
		}
	}

	// Handle everything else
	if ( skill_id < 0 || skill_id >= MAX_SKILLS )
	{
		log_amx( "Invalid skill: %d", skill_id );
		return;
	}

	new szSkillHelper[64];

	formatex ( szSkillHelper, 63, "SKILL_%d", skill_id );
	formatex ( skill_name, len-1, "%L", id, szSkillHelper );
}

LANG_GetSkillInfo( skill_id, id, skill_description[], len )
{
	switch ( skill_id )
	{
		// Undead Scourge
		case SKILL_VAMPIRICAURA:		formatex ( skill_description, len, "%L", id, "SKILL_I_0" );
		case SKILL_UNHOLYAURA:			formatex ( skill_description, len, "%L", id, ( (g_MOD == GAME_DOD) ? "SKILL_I_1_DOD" : "SKILL_I_1" ) );
		case SKILL_LEVITATION:			formatex ( skill_description, len, "%L", id, "SKILL_I_2" );
		case ULTIMATE_SUICIDE:			formatex ( skill_description, len, "%L", id, "SKILL_I_3" );

		// Human Alliance
		case SKILL_INVISIBILITY:		formatex ( skill_description, len, "%L", id, "SKILL_I_4" );
		case SKILL_DEVOTION:			formatex ( skill_description, len, "%L", id, "SKILL_I_5" );
		case SKILL_BASH:				formatex ( skill_description, len, "%L", id, "SKILL_I_6" );
		case ULTIMATE_BLINK:			formatex ( skill_description, len, "%L", id, "SKILL_I_7" );

		// Orcish Horde
		case SKILL_CRITICALSTRIKE:		formatex ( skill_description, len, "%L", id, "SKILL_I_8" );
		case SKILL_CRITICALGRENADE:		formatex ( skill_description, len, "%L", id, "SKILL_I_9" );
		case SKILL_REINCARNATION:		formatex ( skill_description, len, "%L", id, "SKILL_I_10" );
		case ULTIMATE_CHAINLIGHTNING:	formatex ( skill_description, len, "%L", id, "SKILL_I_11" );

		// Night Elf
		case SKILL_EVASION:				formatex ( skill_description, len, "%L", id, "SKILL_I_12" );
		case SKILL_THORNS:				formatex ( skill_description, len, "%L", id, "SKILL_I_13" );
		case SKILL_TRUESHOT:			formatex ( skill_description, len, "%L", id, "SKILL_I_14" );
		case ULTIMATE_ENTANGLE:			formatex ( skill_description, len, "%L", id, "SKILL_I_15" );

		// Blood Mage
		case SKILL_PHOENIX:				formatex ( skill_description, len, "%L", id, ( (g_MOD == GAME_DOD) ? "SKILL_I_16_DOD" : "SKILL_I_16" ) );
		case SKILL_BANISH:				formatex ( skill_description, len, "%L", id, "SKILL_I_17" );
		case SKILL_SIPHONMANA:			formatex ( skill_description, len, "%L", id, "SKILL_I_18" );
		case ULTIMATE_IMMOLATE:			formatex ( skill_description, len, "%L", id, "SKILL_I_19" );
		case PASS_RESISTANTSKIN:		formatex ( skill_description, len, "%L", id, "SKILL_I_20" );

		// Shadow Hunter
		case SKILL_HEALINGWAVE:			formatex ( skill_description, len, "%L", id, "SKILL_I_21" );
		case SKILL_HEX:					formatex ( skill_description, len, "%L", id, "SKILL_I_22" );
		case SKILL_SERPENTWARD:			formatex ( skill_description, len, "%L", id, "SKILL_I_23" );
		case ULTIMATE_BIGBADVOODOO:		formatex ( skill_description, len, "%L", id, "SKILL_I_24" );
		case PASS_UNSTABLECONCOCTION:	formatex ( skill_description, len, "%L", id, "SKILL_I_25" );

		// Warden
		case SKILL_FANOFKNIVES:			formatex ( skill_description, len, "%L", id, "SKILL_I_26" );
		case SKILL_BLINK:				formatex ( skill_description, len, "%L", id, "SKILL_I_27" );
		case SKILL_SHADOWSTRIKE:		formatex ( skill_description, len, "%L", id, "SKILL_I_28" );
		case ULTIMATE_VENGEANCE:		formatex ( skill_description, len, "%L", id, "SKILL_I_29" );
		case PASS_HARDENEDSKIN:			formatex ( skill_description, len, "%L", id, "SKILL_I_30" );

		// Crypt Lord
		case SKILL_IMPALE:				formatex ( skill_description, len, "%L", id, "SKILL_I_31" );
		case SKILL_SPIKEDCARAPACE:		formatex ( skill_description, len, "%L", id, "SKILL_I_32" );
		case SKILL_CARRIONBEETLES:		formatex ( skill_description, len, "%L", id, "SKILL_I_33" );
		case ULTIMATE_LOCUSTSWARM:		formatex ( skill_description, len, "%L", id, "SKILL_I_34" );
		case PASS_ORB:					formatex ( skill_description, len, "%L", id, "SKILL_I_35" );
	}
}