/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Shared Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

#define INVIS_CLOAK_DIVISOR		1.5

// This should be called on weapon change, on new round, when the user selects a new skill
public SHARED_INVIS_Set( id )
{
	// Do not want to set them as invisible if the player is currently rendering
	if ( !p_data_b[id][PB_CAN_RENDER] )
	{
		return;
	}

	new iInvisLevel = 0;
	
	// If they are Human Alliance with Invisibility lets set the level
	if ( Verify_Skill( id, RACE_HUMAN, SKILL1 ) )
	{
		new iSkillLevel = p_data[id][P_SKILL1]-1;

		iInvisLevel = p_invisibility[iSkillLevel];
	}

	// User has a Cloak of Invisibility
	if ( p_data[id][P_ITEM] == wc3_cloak )
	{
		// Then we are already a little invis, lets give them a little bonus for purchasing the item
		if ( iInvisLevel > 0 )
		{
			iInvisLevel = floatround( float( iInvisLevel ) / INVIS_CLOAK_DIVISOR );
		}
		else
		{
			iInvisLevel = get_pcvar_num( CVAR_wc3_cloak );
		}
	}
	
	// If the player is holding a knife they should be more invisible
	if ( SHARED_IsHoldingKnife( id ) )
	{
		iInvisLevel /= 2;
	}

	if ( iInvisLevel )
	{
		set_user_rendering( id, kRenderFxNone, 0, 0, 0, kRenderTransTexture, iInvisLevel );
		
		p_data_b[id][PB_INVIS] = true;
	}

	// User should not be invisible
	else
	{
		set_user_rendering( id );

		p_data_b[id][PB_INVIS] = false;
	}

	return;
}

public SHARED_IsHoldingKnife( id )
{
	new iClip, iAmmo, iWeapon;
	iWeapon = get_user_weapon( id, iClip, iAmmo );

	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeapon == CSW_KNIFE )
		{
			return true;
		}

	}
	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
		if ( iWeapon == DODW_AMERKNIFE || iWeapon == DODW_GERKNIFE || iWeapon == DODW_SPADE )
		{
			return true;
		}
	}

	return false;
}

public SHARED_HasGrenade( id )
{
	new i, bool:bNadeFound = false;
	
	// Loop through all weapons and search for a grenade
	while ( g_PlayerWeapons[id][i] && !bNadeFound )
	{
		if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
		{
			if ( g_PlayerWeapons[id][i] == CSW_HEGRENADE )
			{
				bNadeFound = true;
			}
		}
		else if ( g_MOD == GAME_DOD )
		{
			if ( g_PlayerWeapons[id][i] == DODW_HANDGRENADE || g_PlayerWeapons[id][i] == DODW_STICKGRENADE )
			{
				bNadeFound = true;
			}
		}

		i++;
	}

	return bNadeFound;
}
