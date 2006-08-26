/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	UTIL Functions - Basically helper functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

UTIL_GetWeaponName( iWeapon, szWpnName[], len )
{

	// Counter-Strike and Condition Zero Weapons
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		switch ( iWeapon )
		{
			case CSW_KNIFE:			copy( szWpnName, len, "knife"		);

			case CSW_USP:           copy( szWpnName, len, "usp"			);
			case CSW_GLOCK18:       copy( szWpnName, len, "glock18"		);
			case CSW_DEAGLE:        copy( szWpnName, len, "deagle"		);
			case CSW_P228:          copy( szWpnName, len, "p228"		);
			case CSW_ELITE:         copy( szWpnName, len, "elite"		);
			case CSW_FIVESEVEN:     copy( szWpnName, len, "fiveseven"	);

			case CSW_M3:            copy( szWpnName, len, "m3"			);
			case CSW_XM1014:        copy( szWpnName, len, "xm1014"		);

			case CSW_MP5NAVY:       copy( szWpnName, len, "mp5navy"		);
			case CSW_TMP:           copy( szWpnName, len, "tmp"			);
			case CSW_P90:           copy( szWpnName, len, "p90"			);
			case CSW_MAC10:         copy( szWpnName, len, "mac10"		);
			case CSW_UMP45:         copy( szWpnName, len, "ump45"		);

			case CSW_AK47:          copy( szWpnName, len, "ak47"		);
			case CSW_SG552:         copy( szWpnName, len, "sg552"		);
			case CSW_M4A1:          copy( szWpnName, len, "m4a1"		);
			case CSW_AUG:           copy( szWpnName, len, "aug"			);
			case CSW_SCOUT:         copy( szWpnName, len, "scout"		);
			case CSW_AWP:           copy( szWpnName, len, "awp"			);
			case CSW_SG550:         copy( szWpnName, len, "sg550"		);
			case CSW_G3SG1:         copy( szWpnName, len, "g3sg1"		);

			case CSW_GALI:          copy( szWpnName, len, "galil"		);
			case CSW_FAMAS:         copy( szWpnName, len, "famas"		);

			case CSW_M249:          copy( szWpnName, len, "m249"		);

			case CSW_HEGRENADE:     copy( szWpnName, len, "grenade"		);
		}
	}

	else if ( g_MOD == GAME_DOD )
	{
		switch ( iWeapon )
		{
			case DODW_AMERKNIFE:		copy( szWpnName, len, "amerknife"		);
			case DODW_GERKNIFE:			copy( szWpnName, len, "gerknife"		);
			case DODW_COLT:				copy( szWpnName, len, "colt"			);
			case DODW_LUGER:			copy( szWpnName, len, "luger"			);
			case DODW_GARAND:			copy( szWpnName, len, "garand"			);
			case DODW_SCOPED_KAR:		copy( szWpnName, len, "scopedkar"		);
			case DODW_THOMPSON:			copy( szWpnName, len, "thompson"		);
			case DODW_STG44:			copy( szWpnName, len, "mp44"			);
			case DODW_SPRINGFIELD:		copy( szWpnName, len, "spring"			);
			case DODW_KAR:				copy( szWpnName, len, "kar"				);
			case DODW_BAR:				copy( szWpnName, len, "bar"				);
			case DODW_MP40:				copy( szWpnName, len, "mp40"			);
			case DODW_HANDGRENADE:		copy( szWpnName, len, "grenade"			);
			case DODW_STICKGRENADE:     copy( szWpnName, len, "grenade2"		);
			case DODW_STICKGRENADE_EX:	copy( szWpnName, len, "grenade2"		);
			case DODW_HANDGRENADE_EX:	copy( szWpnName, len, "grenade"			);
			case DODW_MG42:				copy( szWpnName, len, "mg42"			);
			case DODW_30_CAL:			copy( szWpnName, len, "30cal"			);
			case DODW_SPADE:			copy( szWpnName, len, "spade"			);
			case DODW_M1_CARBINE:		copy( szWpnName, len, "m1carbine"		);
			case DODW_MG34:				copy( szWpnName, len, "mg34"			);
			case DODW_GREASEGUN:		copy( szWpnName, len, "greasegun"		);
			case DODW_FG42:				copy( szWpnName, len, "fg42"			);
			case DODW_K43:				copy( szWpnName, len, "k43"				);
			case DODW_ENFIELD:			copy( szWpnName, len, "enfield"			);
			case DODW_STEN:				copy( szWpnName, len, "sten"			);
			case DODW_BREN:				copy( szWpnName, len, "bren"			);
			case DODW_WEBLEY:			copy( szWpnName, len, "webley"			);
			case DODW_BAZOOKA:			copy( szWpnName, len, "bazooka"			);
			case DODW_PANZERSCHRECK:    copy( szWpnName, len, "pschreck"		);
			case DODW_PIAT:				copy( szWpnName, len, "piat"			);
			case DODW_SCOPED_FG42:		copy( szWpnName, len, "scoped_fg42"		);
			case DODW_FOLDING_CARBINE:  copy( szWpnName, len, "fcarbine"		);
			case DODW_KAR_BAYONET:		copy( szWpnName, len, "bayonet"			);
			case DODW_SCOPED_ENFIELD:   copy( szWpnName, len, "scoped_enfield"	);
			case DODW_MILLS_BOMB:		copy( szWpnName, len, "mills_bomb"		);
			case DODW_BRITKNIFE:		copy( szWpnName, len, "brit_knife"		);
			case DODW_GARAND_BUTT:		copy( szWpnName, len, "garandbutt"		);
			case DODW_ENFIELD_BAYONET:  copy( szWpnName, len, "enf_bayonet"		);
			case DODW_MORTAR:			copy( szWpnName, len, "mortar"			);
			case DODW_K43_BUTT:			copy( szWpnName, len, "k43butt"			);
		}
	}

	// These are common to both
	switch ( iWeapon )
	{
		case CSW_WORLDSPAWN:    copy( szWpnName, len, "world"													);

		case CSW_LIGHTNING:     LANG_GetSkillName( ULTIMATE_CHAINLIGHTNING	, LANG_SERVER	, szWpnName	, len	);
		case CSW_SUICIDE:		LANG_GetSkillName( ULTIMATE_SUICIDE			, LANG_SERVER	, szWpnName	, len	);
		case CSW_IMMOLATE:		LANG_GetSkillName( ULTIMATE_IMMOLATE		, LANG_SERVER	, szWpnName	, len	);
		case CSW_LOCUSTS:		LANG_GetSkillName( ULTIMATE_LOCUSTSWARM		, LANG_SERVER	, szWpnName	, len	);
		case CSW_SERPENTWARD:   LANG_GetSkillName( SKILL_SERPENTWARD		, LANG_SERVER	, szWpnName	, len	);
		case CSW_SHADOW:		LANG_GetSkillName( SKILL_SHADOWSTRIKE		, LANG_SERVER	, szWpnName	, len	);
		case CSW_THORNS:		LANG_GetSkillName( SKILL_THORNS				, LANG_SERVER	, szWpnName	, len	);
		case CSW_CARAPACE:		LANG_GetSkillName( SKILL_SPIKEDCARAPACE		, LANG_SERVER	, szWpnName	, len	);
		case CSW_CARRION:		LANG_GetSkillName( SKILL_CARRIONBEETLES		, LANG_SERVER	, szWpnName	, len	);
		case CSW_ORB:			LANG_GetSkillName( PASS_ORB					, LANG_SERVER	, szWpnName	, len	);
		case CSW_CONCOCTION:	LANG_GetSkillName( PASS_UNSTABLECONCOCTION	, LANG_SERVER	, szWpnName	, len	);
	}
}

// Log the death of a user
UTIL_LogDeath( iVictim, iKiller, iWeapon )
{
	new szKillerName[32], szVictimName[32];
	new szKillerTeam[32], szVictimTeam[32];
	new szKillerAuth[32], szVictimAuth[32];
	new szWeaponName[32];

	new iKillerUserID = get_user_userid( iKiller );
	new iVictimUserID = get_user_userid( iVictim );

	UTIL_GetWeaponName( iWeapon, szWeaponName, 31 );

	get_user_name(		iKiller	, szKillerName	, 31 );
	get_user_name(		iVictim	, szVictimName	, 31 );
	get_user_team(		iKiller	, szKillerTeam	, 31 );
	get_user_team(		iVictim	, szVictimTeam	, 31 );
	get_user_authid(	iKiller	, szKillerAuth	, 31 );
	get_user_authid(	iVictim	, szVictimAuth	, 31 );

	log_message("^"%s<%d><%s><%s>^" killed ^"%s<%d><%s><%s>^" with ^"%s^"", szKillerName, iKillerUserID, szKillerAuth, szKillerTeam, szVictimName, iVictimUserID, szVictimAuth, szVictimTeam, szWeaponName );
}

stock UTIL_EntInView( id, iEnt )
{
	new Float:vPlayerOrigin[3];
	entity_get_vector( id, EV_VEC_origin, vPlayerOrigin );

	new Float:vEntOrigin[3];
	entity_get_vector( iEnt, EV_VEC_origin, vEntOrigin );

	new Float:vReturn[3];

	new iHitEnt = trace_line( id, vPlayerOrigin, vEntOrigin, vReturn );

	if ( iHitEnt == iEnt )
	{
		return 1;
	}
	
	return 0;
}

UTIL_ClearHudChannel( id, iChannel )
{
	set_hudmessage( 0, 0, 0, 0.0, 0.0, 0, 0.0, 0.0, 0.0, 0.0, iChannel );
	show_hudmessage( id, "" );
}

stock UTIL_CanSeePlayer( id, iTargetID )
{
	new Float:vOrigin[3];
	entity_get_vector( iTargetID, EV_VEC_origin, vOrigin );

	return is_in_viewcone( id, vOrigin );
}