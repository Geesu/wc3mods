// Begin LANGUAGE.INL

#define SHOP_COMMON			1

// Defines used for getting skills names
#define SKILL_T_RACIAL		1
#define SKILL_T_TRAINED		2
#define SKILL_T_ULTIMATE	3


LANG_GetItemName( item_id, shop_id, id, item_name[], len )
{
	new szItemHelper[32];

	formatex( szItemHelper, 31, "SHOP%d_ITEM%d_NAME", shop_id, item_id );
	formatex( item_name, len, "%L", id, szItemHelper );
}

LANG_GetItemDesc( item_id, shop_id, id, item_desc[], len )
{
	new szItemHelper[32];

	formatex( szItemHelper, 31, "SHOP%d_ITEM%d_DESC", shop_id, item_id );
	formatex( item_desc, len, "%L", id, szItemHelper );
}

LANG_GetRaceName( race_id, id, race_name[], len, bool:short_name = false )
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

	formatex( race_name, len, "%L", id, szRaceHelper );
}

LANG_GetSkillNameHelper( race_id, skill_id, id, skill_name[], len )
{
	new newSkillId = 0;
	new skill_type = 0;

	switch ( skill_id )
	{
		case 0: {newSkillId = 1; skill_type = SKILL_T_RACIAL;}
		case 1: {newSkillId = 1; skill_type = SKILL_T_TRAINED;}
		case 2: {newSkillId = 2; skill_type = SKILL_T_TRAINED;}
		case 3: {newSkillId = 3; skill_type = SKILL_T_TRAINED;}
		case 4: {newSkillId = 1; skill_type = SKILL_T_ULTIMATE;}
		case 5: {newSkillId = 2; skill_type = SKILL_T_ULTIMATE;}
		case 6: {newSkillId = 3; skill_type = SKILL_T_ULTIMATE;}
	}


	LANG_GetSkillName( race_id, newSkillId, skill_type, id, skill_name, len )
}

LANG_GetSkillName( race_id, skill_id, skill_type, id, skill_name[], len )
{
	new szSkillHelper[32];

	if ( skill_type == SKILL_T_RACIAL )
	{
		formatex( szSkillHelper, 31, "RACE%d_RACIAL%d", race_id, skill_id );
	}
	else if ( skill_type == SKILL_T_TRAINED )
	{
		formatex( szSkillHelper, 31, "RACE%d_SKILL%d", race_id, skill_id );
	}
	else if ( skill_type == SKILL_T_ULTIMATE )
	{
		formatex( szSkillHelper, 31, "RACE%d_ULTIMATE%d", race_id, skill_id );
	}

	formatex( skill_name, len, "%L", id, szSkillHelper );
}

LANG_GetSkillDesc( race_id, skill_id, skill_type, id, skill_desc[], len )
{
	new szSkillHelper[32];

	if ( skill_type == SKILL_T_RACIAL )
	{
		formatex( szSkillHelper, 31, "RACE%d_RACIAL%d_DESC", race_id, skill_id );
	}
	else if ( skill_type == SKILL_T_TRAINED )
	{
		formatex( szSkillHelper, 31, "RACE%d_SKILL%d_DESC", race_id, skill_id );
	}
	else if ( skill_type == SKILL_T_ULTIMATE )
	{
		formatex( szSkillHelper, 31, "RACE%d_ULTIMATE%d_DESC", race_id, skill_id );
	}

	server_print(szSkillHelper)

	formatex( skill_desc, len, "%L", id, szSkillHelper );
}