// Begin LANGUAGE.INL

#define	SHOP_COMMON			1
#define	TOTAL_MENUS			7		// Total number	of menus to	be registered

// This	is the list	of menu	titles that	are	in war3x.txt

new	const MENU_NAMES[TOTAL_MENUS][]	=
{
	"MENU_WAR3MENU_TITLE",
	"MENU_SELECTRACE_TITLE",
	"MENU_BUYITEM_TITLE",
	"MENU_SKILLSINFO_TITLE",
	"MENU_SELECTSKILLS_TITLE",
	"MENU_PLAYEROPTIONS_TITLE",
	"MENU_RACEOPTIONS_TITLE"
//	"MENU_ADMINOPTIONS_TITLE"
};

// Callback	functions for the above	menu names

new	const MENU_CALLBACK[TOTAL_MENUS][] =
{
	"_menu_War3menu",
	"_menu_SelectRace",
	"_menu_ItemShop",
	"_menu_SkillsHelp",
	"_menu_SelectSkills",
	"_menu_PlayerOptions",
	"_menu_RaceOptions"
//	"_menu_AdminOptions"
};

public LANG_RegisterMenus()	{

	new	total_languages	= get_langsnum();
	new	lang[3], menu[128];
	new	curMenuId =	-1,	highestMenuId =	-1;
	new	iLang, iMenu;

	// Loop	through	every menu

	for	( iMenu	= 0; iMenu < TOTAL_MENUS; iMenu++ )
	{
		// Register	the	menu names for each	language

		for( iLang = 0;	iLang <	total_languages; iLang++ )
		{
			get_lang ( iLang, lang );

			if(	lang_exists( lang )	)
			{
				formatex ( menu, 127, "%L",	lang, MENU_NAMES[iMenu]	);
				curMenuId =	register_menuid	( menu );

				if(	curMenuId >	highestMenuId )
				{
					register_menucmd ( curMenuId, 1023,	MENU_CALLBACK[iMenu] );
					highestMenuId =	curMenuId;
				}
			}
		} // End language loop
	} // End menu loop

	return PLUGIN_HANDLED;
}


public LANG_GetItemName( item_id, shop_id, id, item_name[],	len	) {

	new	szItemHelper[32];

	formatex( szItemHelper,	31,	"SHOP%d_ITEM%d_NAME", shop_id, item_id );
	formatex( item_name, len, "%L",	id,	szItemHelper );

	return PLUGIN_HANDLED;
}


public LANG_GetItemDesc( item_id, shop_id, id, item_desc[],	len	) {

	new	szItemHelper[32];

	formatex( szItemHelper,	31,	"SHOP%d_ITEM%d_DESC", shop_id, item_id );
	formatex( item_desc, len, "%L",	id,	szItemHelper );

	return PLUGIN_HANDLED;
}


public LANG_GetRaceName( race_id, id, race_name[], len,	bool:short_name	) {

	new	szRaceHelper[32];

	if ( short_name	)
	{
		formatex( szRaceHelper,	31,	"RACENAME%d_SHORT",	race_id	);
	}
	else
	{
		formatex( szRaceHelper,	31,	"RACENAME%d", race_id );
	}

	formatex( race_name, len, "%L",	id,	szRaceHelper );

	return PLUGIN_HANDLED;
}


public LANG_GetSkillName( race_id, skill_id, id, skill_name[], len ) {

	new	szSkillHelper[32];

	// Racial /	trained	skill

	if ( skill_id <	TOTAL_SKILLS )
	{
		formatex( szSkillHelper, 31, "RACE%d_SKILL%d", race_id,	skill_id );
	}

	// Ultimate

	else
	{
		skill_id -=	( TOTAL_SKILLS - 1 );
		formatex( szSkillHelper, 31, "RACE%d_ULTIMATE%d", race_id, skill_id	);
	}

	formatex( skill_name, len, "%L", id, szSkillHelper );

	return PLUGIN_HANDLED;
}


public LANG_GetSkillDesc( race_id, skill_id, id, skill_desc[], len ) {

	new	szSkillHelper[32];

	// Racial /	trained	skill

	if ( skill_id <	TOTAL_SKILLS )
	{
		formatex( szSkillHelper, 31, "RACE%d_SKILL%d_DESC",	race_id, skill_id );
	}

	// Ultimate

	else
	{
		skill_id -=	( TOTAL_SKILLS - 1 );
		formatex( szSkillHelper, 31, "RACE%d_ULTIMATE%d_DESC", race_id,	skill_id );
	}

	formatex( skill_desc, len, "%L", id, szSkillHelper );

	return PLUGIN_HANDLED;
}


public LANG_GetClassName( race,	id,	szClassName[], iLen	) {

	new	class_id;

	// Check for ultimate (	gives unique class )

	if ( g_PlayerInfo[id][CURRENT_ULTIMATE]	)
		class_id = g_PlayerInfo[id][CURRENT_ULTIMATE] +	2;	// Offset for ultimate-designated classname

	// Otherwise, retrive class	by level

	else
	{
		new	iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

		for	( new i	= 2; i >= 0; i-- )
		{
			if ( iLevel	>= GETCLASSCHECK[i]	)
			{
				class_id = i;
				break;
			}
		}
	}

	// Format string

	new	szHelper[31];

	formatex( szHelper,	31,	"RACE%d_CLASS%d", race,	class_id );
	formatex( szClassName, iLen, "%L", id, szHelper	);

	return PLUGIN_HANDLED;
}

// ------------------------------------------------- End. -	//
