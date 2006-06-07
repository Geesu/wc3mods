/* AMX Mod X
*   Warcraft 3 Frozen Throne
*
*  by Geesu
*  http://www.war3ft.com
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software Foundation,
*  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*  In addition, as a special exception, the author gives permission to
*  link the code of this program with the Half-Life Game Engine ("HL
*  Engine") and Modified Game Libraries ("MODs") developed by Valve,
*  L.L.C ("Valve"). You must obey the GNU General Public License in all
*  respects for all of the code used other than the HL Engine and MODs
*  from Valve. If you modify this file, you may extend this exception
*  to your version of the file, but you are not obligated to do so. If
*  you do not wish to do so, delete this exception statement from your
*  version.
*/

#pragma tabsize 0

new const WC3NAME[] =		"Warcraft 3 Frozen Throne"
new const WC3AUTHOR[] =		"Geesu"
new const WC3VERSION[] =	"3.0 RC1"
new const WC3DATE[] =		__DATE__

// Let AMX X know that we NEED these modules (as of amx x 1.75)
#pragma reqclass	xstats
#pragma reqlib		engine
#pragma reqlib		fun
#pragma reqlib		fakemeta
#pragma reqlib		nvault

#include <amxmodx>
#include <amxmisc>
#include <engine>
#include <fun>
#include <fakemeta>
#include <nvault>

#define AMXMODX_NOAUTOLOAD
#include <dbi>
#include <sqlx>
#include <cstrike>
#include <dodfun>
#include <dodx>

// Compiling Options
#define MOD						0				// 0 = cstrike or czero, 1 = dod
#define ADVANCED_STATS			1				// Setting this to 1 will give detailed information with psychostats (hits, damage, hitplace, etc..) for war3 abilities
#define SHOW_SPECTATE_INFO		1				// Show spectating information on users

// Header files that contain function declarations
#include "war3ft/dod_h.inl"
#include "war3ft/db/db_mysqlx.h"
#include "war3ft/db/db_sqlite.h"
#include "war3ft/db/db_nvault.h"

// Source Code
#include "war3ft/constants.inl"
#include "war3ft/cvar.inl"

#include "war3ft/race_orc.inl"				// Orcish Horde		- 3
#include "war3ft/race_elf.inl"				// Night Elf		- 4
#include "war3ft/race_blood.inl"			// Blood Mage		- 5
#include "war3ft/race_shadow.inl"			// Shadow Hunter	- 6
#include "war3ft/race_warden.inl"           // Warden           - 7

#include "war3ft/race_chameleon.inl"		// Chameleon		- 9

#include "war3ft/effects.inl"
#include "war3ft/XP.inl"
#include "war3ft/db/db_mysqlx.inl"
#include "war3ft/db/db_sqlite.inl"
#include "war3ft/db/db_nvault.inl"
#include "war3ft/db/db_common.inl"
#include "war3ft/war3ft.inl"
#include "war3ft/events.inl"
#include "war3ft/clientCommands.inl"
#include "war3ft/items.inl"
#include "war3ft/stocks.inl"
#include "war3ft/ultimates.inl"
#include "war3ft/skills.inl"
#include "war3ft/shared.inl"
#include "war3ft/menus.inl"
#include "war3ft/motd.inl"
#include "war3ft/language.inl"
#include "war3ft/other.inl"
#include "war3ft/admin.inl"

#include "war3ft/cstrike.inl"
#include "war3ft/dod.inl"

public plugin_init()
{
	register_plugin( WC3NAME, WC3VERSION, WC3AUTHOR );
	
	WC3_DetermineGame();

	gmsgDeathMsg		= get_user_msgid( "DeathMsg"	);
	gmsgScreenFade		= get_user_msgid( "ScreenFade"	);
	gmsgScreenShake		= get_user_msgid( "ScreenShake"	);
	gmsgScoreInfo		= get_user_msgid( "ScoreInfo"	);
	

	register_concmd( "playerskills"		, "CMD_Handler"		, -1 );

	register_clcmd( "war3menu"			, "CMD_Handler"		, -1 );
	register_clcmd( "changerace"		, "CMD_Handler"		, -1 );
	register_clcmd( "selectskill"		, "CMD_Handler"		, -1 );
	register_clcmd( "skillsinfo"		, "CMD_Handler"		, -1 );
	register_clcmd( "resetskills"		, "CMD_Handler"		, -1 );
	register_clcmd( "resetxp"			, "CMD_Handler"		, -1 );
	register_clcmd( "itemsinfo"			, "CMD_Handler"		, -1 );
	register_clcmd( "itemsinfo2"		, "CMD_Handler"		, -1 );
	register_clcmd( "shopmenu"			, "CMD_Handler"		, -1 );
	register_clcmd( "shopmenu2"			, "CMD_Handler"		, -1 );
	register_clcmd( "ability"			, "CMD_Handler"		, -1 );
	register_clcmd( "rings"				, "CMD_Handler"		, -1 );
	register_clcmd( "level"				, "CMD_Handler"		, -1 );
	register_clcmd( "say"				, "cmd_Say"			, -1 );
	register_clcmd( "say_team"			, "cmd_Say"			, -1 );
	register_clcmd( "war3help"			, "MOTD_War3help"	, -1 );
	register_clcmd( "ultimate"			, "cmd_Ultimate"	, -1 );
	register_clcmd( "jointeam"			, "cmd_Jointeam"	, -1 );
	register_clcmd( "fullupdate"		, "cmd_fullupdate"	, -1 );
	register_clcmd( "drop"				, "on_Drop"			, -1 );


	// Admin Commands
	register_concmd( "amx_givexp"		, "Admin_GiveXP"		, 0 , " -- Gives XP to players"				);
	register_concmd( "amx_setlevel"	    , "Admin_SetLevel"		, 0 , " -- Sets a players level"			);
	register_concmd( "amx_givemole"	    , "Admin_GiveMole"		, 0 , " -- Gives the mole item to a player"	);
	register_concmd( "amx_wc3"			, "Admin_wc3"			, 0 , " -- Enables/disables war3ft"			);
	
	// Server Admin Commands (used by external plugins)
	register_srvcmd( "amx_takexp"		, "Admin_TakeXP"	);
	register_srvcmd( "changexp"			, "changeXP"		);
	
	// Register forwards (from fakemeta)
	register_forward( FM_TraceLine		, "TRIGGER_TraceLine"	);

	register_event( "DeathMsg"			, "on_DeathMsg"		, "a"								);
	register_event( "CurWeapon"			, "on_CurWeapon"	, "be"	, "1=1"						);
	register_event( "HideWeapon"		, "on_CurWeapon"	, "b"								);
	register_event( "ResetHUD"			, "on_ResetHud"		, "b"								);
	register_event( "TextMsg"			, "on_GameRestart"	, "a"	, "2&#Game_will_restart_in" );
	register_event( "HLTV"				, "EVENT_NewRound"	, "a"	, "1=0"	,			"2=0"	);

	register_dictionary( "war3FT.txt")

	// Game Specific Initialization
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		gmsgBarTime			= get_user_msgid( "BarTime"		);
		gmsgStatusText		= get_user_msgid( "StatusText"	);
		gmsgStatusIcon		= get_user_msgid( "StatusIcon"	);

		register_logevent( "on_PlayerAction"		, 3		, "1=triggered"								);
		register_logevent( "on_FreezeTimeComplete"	, 2		, "0=World triggered"	, "1=Round_Start"	);
		register_logevent( "on_EndRound"			, 2		, "0=World triggered"	, "1=Round_End"		);

		register_event( "SendAudio"		, "on_TerroristWin"	, "a"	, "2=%!MRAD_terwin"					);
		register_event( "SendAudio"		, "on_CTWin"		, "a"	, "2=%!MRAD_ctwin"					);
		register_event( "23"			, "on_TargetBombed"	, "a"	, "1=17"	, "6=-105"	, "7=17"	);
		register_event( "ArmorType"		, "on_ArmorType"	, "be"										);
		register_event( "Battery"		, "on_Battery"		, "be"										);
		register_event( "WeapPickup"	, "on_WeapPickup"	, "b"										); 
		register_event( "StatusValue"	, "on_ShowStatus"	, "be"	, "1=2"		,"2!0"					);
		register_event( "StatusValue"	, "on_HideStatus"	, "be"	, "1=1"		,"2=0"					);
		register_event( "TextMsg"		, "on_SetSpecMode"	, "bd"	, "2&ec_Mod"						);
		register_event( "Damage"		, "on_Damage"		, "b"	, "2!0"								);
		register_event( "StatusValue"	, "on_Spectate"		, "bd"	, "1=2"								);

		// Old Style
		register_menucmd( register_menuid( "BuyItem" )	, (1<<2)	, "cmd_flash"	);
		register_menucmd( register_menuid( "BuyItem" )	, (1<<3)	, "cmd_hegren"	);

		// VGUI
		register_menucmd( -34	, (1<<2)	, "cmd_flash"	);
		register_menucmd( -34	, (1<<3)	, "cmd_hegren"	);

		// Steam
		register_clcmd( "flash"		, "cmd_flash"	);
		register_clcmd( "hegren"	, "cmd_hegren"	);

		register_menucmd( register_menuid( "Team_Select" , 1 )	, (1<<0)|(1<<1)|(1<<4)	, "cmd_Teamselect" );

		// Condition Zero
		if ( g_MOD == GAME_CZERO )
		{
			register_event("TextMsg", "on_GameRestart", "a", "2&#Game_Commencing")
		}
		// Counter-Strike
		else
		{
			register_event("TextMsg", "on_GameRestart", "a", "2&#Game_C")
		}
	}
	else if ( g_MOD == GAME_DOD )
	{
		gmsgHudText = get_user_msgid("HudText");

		register_statsfwd( XMF_SCORE	);
		register_statsfwd( XMF_DAMAGE	);

		register_event( "RoundState"	, "EVENT_DOD_EndRound"		, "a"	, "1=3"	, "1=4"	);
		register_event( "StatusValue"	, "on_StatusValue"	, "b"					);
	}
	
	// Plugin initialization procedures
	WC3_Init();
}

public plugin_end()
{
	if ( !WAR3_Check() )
	{
		return;
	}

	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}
	
	XP_SaveAll();
	DB_Prune();
	DB_Close();

	return;
}

public plugin_precache()
{
	WAR3_precache()
}

public client_putinserver( id )
{
	if ( !WAR3_Check() )
	{
		return;
	}

	// Check for steam ID pending
	new szPlayerID[32];
	get_user_authid( id, szPlayerID, 31 );
	
	// Then the player doesn't have a steam id, lets make them reconnect
	if ( equal(szPlayerID, "STEAM_ID_PENDING") )
	{
		client_cmd(id, "reconnect");
	}

	p_data_b[id][PB_ISCONNECTED] = true;
	
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		// Check user's cvar
		if ( !is_user_bot( id ) )
		{
			query_client_cvar( id, "cl_minmodels", "check_cvars" );
		}
	}

	// Start showing the user their money
	else if ( g_MOD == GAME_DOD )
	{
		p_data[id][P_MONEY] = get_pcvar_num( CVAR_wc3_dod_start_money );
		new parm[3];
		parm[0] = id;
		parm[1] = 0;
		_DOD_showMoney( parm );
	}

	return;
}

public client_connect( id )
{
	client_cmd( id, "hud_centerid 0" );

	p_data[id][P_RACE]					= 0;
	p_data[id][P_SKILL1]				= 0;
	p_data[id][P_SKILL2]				= 0;
	p_data[id][P_SKILL3]				= 0;
	p_data[id][P_ULTIMATE]				= 0;
	p_data[id][P_LEVEL]					= 0;
	p_data[id][P_XP]					= 0;
	p_data[id][P_ITEM]					= 0;
	p_data[id][P_ITEM2]					= 0;
	p_data_b[id][PB_ISBURNING]			= false;
	p_data[id][P_SPECMODE]				= 0;
	p_data_b[id][PB_JUSTJOINED]			= true;
	p_data_b[id][PB_CAN_RENDER]			= true;
	p_data[id][P_RESPAWNBY]				= 0;

	// Automatically set their XP if it's enabled
	if ( get_pcvar_num( CVAR_wc3_xp_auto_average ) && !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		new iTotalXP;
		new iNum, i;
		for( i = 1; i <= MAXPLAYERS; i++ )
		{
			if ( p_data[i][P_XP] > 0 )
			{
				iNum++;
				iTotalXP += p_data[i][P_XP];
			}
		}
		if ( iNum > 0 && iTotalXP > 0 )
		{
			p_data[id][P_XP] = iTotalXP/iNum;
		}
	}

	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		p_data[id][P_HECOUNT]		= 0;
		p_data[id][P_FLASHCOUNT]	= 0;
	}
	else if ( g_MOD == GAME_DOD )
	{
		// Skip Reincarnation since the user just joined
		p_data_b[id][PB_REINCARNATION_SKIP] = true;

		p_data[id][P_MONEY] = 0;
	}
	
	return;
}

public client_disconnect(id)
{

	// Remove the money task when a user disconnects
	if ( g_MOD == GAME_DOD )
	{
		task_exists( TASK_MONEYLOOP + id ) ? remove_task( TASK_MONEYLOOP + id ) : 0;
	}

	p_data[id][P_SPECMODE]			= 0;
	p_data_b[id][PB_ISBURNING]		= false;
	p_data_b[id][PB_DIEDLASTROUND]	= false;
	p_data_b[id][PB_JUSTJOINED]		= false;
	p_data_b[id][PB_ISCONNECTED]	= false;
	
	// Save the user's XP if we have XP to save
	if ( get_pcvar_num( CVAR_wc3_save_xp ) && !is_user_bot(id) && p_data[id][P_RACE] && p_data[id][P_XP] )
	{
		DB_SaveXP( id );
	}

#if ADVANCED_STATS
	new szWeapon[64]

	new szTeam[16], szName[32], szAuthid[32], weap;
	new iUserid = get_user_userid( id );

	if ( is_user_connected(id) )
	{
		get_user_team( id, szTeam, 15 );
	}
	get_user_name( id, szName, 31 );
	get_user_authid( id, szAuthid, 31 );

	for ( weap = CSW_WAR3_MIN; weap <=CSW_WAR3_MAX; weap++ )
	{
		format(szWeapon, 63, "")

		switch( weap )
		{
			case CSW_LIGHTNING:     lang_GetSkillName( 3				, 4				, LANG_SERVER,	szWeapon,	63 );
			case CSW_SUICIDE:		lang_GetSkillName( 1				, 4				, LANG_SERVER,	szWeapon,	63 );
			case CSW_IMMOLATE:	    lang_GetSkillName( 5				, 4				, LANG_SERVER,	szWeapon,	63 );
			case CSW_LOCUSTS:		lang_GetSkillName( 8				, 4				, LANG_SERVER,	szWeapon,	63 );
			case CSW_SERPENTWARD:   lang_GetSkillName( 6				, 3				, LANG_SERVER,	szWeapon,	63 );
			case CSW_SHADOW:		lang_GetSkillName( 7				, 3				, LANG_SERVER,	szWeapon,	63 );
			case CSW_THORNS:		lang_GetSkillName( 4				, 2				, LANG_SERVER,	szWeapon,	63 );
			case CSW_CARAPACE:		lang_GetSkillName( 8				, 2				, LANG_SERVER,	szWeapon,	63 );
			case CSW_CARRION:		lang_GetSkillName( 8				, 3				, LANG_SERVER,	szWeapon,	63 );
			case CSW_ORB:			lang_GetSkillName( RACE_CRYPT		, SKILL_HERO	, LANG_SERVER,	szWeapon,	63 );
			case CSW_CONCOCTION:	lang_GetSkillName( RACE_SHADOW		, SKILL_HERO	, LANG_SERVER,	szWeapon,	63 );
		}
		
		replace( szWeapon, 63, " ", "_" );

		new WEAPON = weap - CSW_WAR3_MIN;
		
		if ( iStatsShots[id][WEAPON] || iStatsHits[id][WEAPON] || iStatsKills[id][WEAPON] ||  iStatsHS[id][WEAPON] || iStatsTKS[id][WEAPON] || iStatsDamage[id][WEAPON] || iStatsDeaths[id][WEAPON] || iStatsHead[id][WEAPON] || iStatsChest[id][WEAPON] || iStatsStomach[id][WEAPON] || iStatsLeftArm[id][WEAPON] || iStatsRightArm[id][WEAPON] || iStatsLeftLeg[id][WEAPON] || iStatsRightLeg[id][WEAPON] )
		{

			// Counter-Strike/Condition Zero log format is different than the DOD
			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{
				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats^" (weapon ^"%s^") (shots ^"%d^") (hits ^"%d^") (kills ^"%d^") (headshots ^"%d^") (tks ^"%d^") (damage ^"%d^") (deaths ^"%d^")", szName, iUserid, szAuthid, szTeam, szWeapon, iStatsShots[id][WEAPON], iStatsHits[id][WEAPON], iStatsKills[id][WEAPON], iStatsHS[id][WEAPON], iStatsTKS[id][WEAPON], iStatsDamage[id][WEAPON], iStatsDeaths[id][WEAPON] );
			}

			// Day of Defeat log format
			else if ( g_MOD == GAME_DOD )
			{
				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats^" (weapon ^"%s^") (shots ^"%d^") (hits ^"%d^") (kills ^"%d^") (headshots ^"%d^") (tks ^"%d^") (damage ^"%d^") (deaths ^"%d^") (score ^"%d^")", szName, iUserid, szAuthid, szTeam, szWeapon, iStatsShots[id][WEAPON], iStatsHits[id][WEAPON], iStatsKills[id][WEAPON], iStatsHS[id][WEAPON], iStatsTKS[id][WEAPON], iStatsDamage[id][WEAPON], iStatsDeaths[id][WEAPON], 0 );
			}
			
			log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats2^" (weapon ^"%s^") (head ^"%d^") (chest ^"%d^") (stomach ^"%d^") (leftarm ^"%d^") (rightarm ^"%d^") (leftleg ^"%d^") (rightleg ^"%d^")", szName, iUserid, szAuthid, szTeam, szWeapon, iStatsHead[id][WEAPON], iStatsChest[id][WEAPON], iStatsStomach[id][WEAPON], iStatsLeftArm[id][WEAPON], iStatsRightArm[id][WEAPON], iStatsLeftLeg[id][WEAPON], iStatsRightLeg[id][WEAPON] );
		
			iStatsShots[id][WEAPON]		= 0;
			iStatsHits[id][WEAPON]		= 0;
			iStatsKills[id][WEAPON]		= 0;
			iStatsHS[id][WEAPON]		= 0;
			iStatsTKS[id][WEAPON]		= 0;
			iStatsDamage[id][WEAPON]	= 0;
			iStatsDeaths[id][WEAPON]	= 0;
			iStatsHead[id][WEAPON]		= 0;
			iStatsChest[id][WEAPON]		= 0;
			iStatsStomach[id][WEAPON]	= 0;
			iStatsLeftArm[id][WEAPON]	= 0;
			iStatsRightArm[id][WEAPON]	= 0;
			iStatsLeftLeg[id][WEAPON]	= 0;
			iStatsRightLeg[id][WEAPON]	= 0;
		}
	}
#endif
}



public client_PreThink( id )
{
	if ( !WAR3_Check() )
	{
		return;
	}

	if ( p_data_b[id][PB_ISCONNECTED] )
	{
		if ( is_user_alive( id ) )
		{
			
			// Counter-Strike or Condition Zero
			if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
			{

				// This is used so we can't hear the undead's footsteps at level 3
				if ( Verify_Skill( id, RACE_UNDEAD, SKILL3 ) && !p_data_b[id][PB_STUNNED] && !p_data_b[id][PB_SLOWED] )
				{
					new Float:vel[3];
					entity_get_vector( id, EV_VEC_velocity, vel );

					// When this is true, the user is walking...  lets hope :P
					if ( vector_length( vel ) < 180.0 )
					{
						entity_set_int( id, EV_INT_flTimeStepSound, 999 );
					}

					// Otherwise if we just set it on the previous tick we need to set it back
					else if ( entity_get_int(id, EV_INT_flTimeStepSound) > 500 )
					{
						entity_set_int( id, EV_INT_flTimeStepSound, 200 );
					}
				}
			}

			// Day of Defeat
			else if ( g_MOD == GAME_DOD )
			{

				// Set the user's speed
				SHARED_SetSpeed( id );
				
				// Give the user more stamina
				if( p_data[id][P_ITEM] == ITEM_BOOTS && entity_get_float( id, EV_FL_fuser4 ) < DOD_BOOT_SPEED )
				{
					entity_set_float( id, EV_FL_fuser4, DOD_BOOT_SPEED );
				}
			}

			// Amulet of the Cat
			if( p_data[id][P_ITEM2] == ITEM_AMULET )
			{
				entity_set_int( id, EV_INT_flTimeStepSound, 999 );
			}
		}
	}

	return;
}

// This functionality allows us to no longer requires a DBI module to be loaded
public plugin_natives()
{
	set_module_filter("module_filter");
	set_native_filter("native_filter");
}

public module_filter( const module[] )
{
	WC3_DetermineGame();

	copy( szNotLoadedModules[iTotalNotLoadedModules++], 31, module );
	
	// The required module doesn't even exist!!!
	if ( WC3_MissingModules( module ) )
	{
		log_amx( "Please verify the module '%s' exists in your modules folder, otherwise war3ft will not start.", ( equal( module, "dod" ) ) ? "sqlite" : module );
		
		return PLUGIN_CONTINUE;
	}

	// Check to make sure the modules are enabled
	if ( !task_exists( TASK_CHECKMODULES ) )
	{
		set_task( 0.3, "_WC3_CheckModules", TASK_CHECKMODULES );
	}

	return PLUGIN_HANDLED;
}

public native_filter( const name[], index, trap )
{
	if ( !trap )
	{
		return PLUGIN_HANDLED;
	}

	return PLUGIN_CONTINUE;
}