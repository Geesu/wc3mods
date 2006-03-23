// Begin LANGUAGE.INL

#define SHOP_COMMON		1

// Defines used for getting skills names
#define SKILL_RACIAL		1
#define SKILL_NORMAL		2
#define SKILL_ULTIMATE		3


LANG_GetItemName ( item_id, shop_id, id, item_name[], len )
{
	new szItemHelper[32];

	formatex( szItemHelper, 31, "SHOP%d_ITEM%d_NAME", shop_id, item_id );

	// Lookup the item name

	formatex( item_name, len-1, "%L", id, szItemHelper );
}

LANG_GetItemDesc ( item_id, shop_id, id, item_desc[], len )
{
	new szItemHelper[32];

	formatex( szItemHelper, 31, "SHOP%d_ITEM%d_DESC", shop_id, item_id );

	// Lookup the item name

	formatex( item_desc, len-1, "%L", id, szItemHelper );
}


LANG_GetRaceName ( race_id, id, race_name[], len, bool:short_name = false )
{
	new szRaceHelper[32];
	
	if ( short_name )
	{
		formatex( szRaceHelper, 31, "RACENAME%d_SHORT", race_id );
	}
	else
	{
		formatex( szRaceHelper, 31, "RACENAME%d", race_id );
	}

	// Lookup the race name

	formatex( race_name, len-1, "%L", id, szRaceHelper );
}

LANG_GetSkillName ( race_id, skill_id, skill_type, skill_name[], len )
{
	new szSkillHelper[32];

	if ( skill_type == SKILL_RACIAL )
	{
		formatex( szSkillHelper, 31, "RACE%d_RACIAL%d", race_id, skill_id );
	}
	else if ( skill_type == SKILL_NORMAL )
	{
		formatex( szSkillHelper, 31, "RACE%d_SKILL%d", race_id, skill_id );
	}
	else if ( skill_type == SKILL_ULTIMATE )
	{
		formatex( szSkillHelper, 31, "RACE%d_ULTIMATE%d", race_id, skill_id );
	}

	formatex( race_name, len-1, "%L", id, szSkillHelper );
}

LANG_GetSkillDesc ( race_id, skill_id, skill_type, skill_desc[], len )
{
	new szSkillHelper[32];

	if ( skill_type == SKILL_RACIAL )
	{
		formatex( szSkillHelper, 31, "RACE%d_RACIAL%d_DESC", race_id, skill_id );
	}
	else if ( skill_type == SKILL_NORMAL )
	{
		formatex( szSkillHelper, 31, "RACE%d_SKILL%d_DESC", race_id, skill_id );
	}
	else if ( skill_type == SKILL_ULTIMATE )
	{
		formatex( szSkillHelper, 31, "RACE%d_ULTIMATE%d_DESC", race_id, skill_id );
	}

	formatex( skill_desc, len-1, "%L", id, szSkillHelper );
}