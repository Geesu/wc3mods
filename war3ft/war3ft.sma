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
new const WC3VERSION[] =	"2.3.3 RC1"
new const WC3DATE[] =		__DATE__


#include <amxmodx>
#include <amxmisc>
#include <engine>
#include <fun>
#include <fakemeta>
#include <dbi>
#include <cstrike>
#include <dodfun>
#include <dodx>

// Compiling Options
#define MOD						0				// 0 = cstrike or czero, 1 = dod
#define ADVANCED_STATS			1				// Setting this to 1 will give detailed information with psychostats (hits, damage, hitplace, etc..) for war3 abilities
#define PRECACHE_WAR3FTSOUNDS	1
#define SHOW_SPECTATE_INFO		1				// Show spectating information on users

// Header files that contain function declarations
#include "war3ft/dod_h.inl"

// Source Code
#include "war3ft/constants.inl"
#include "war3ft/cvar.inl"
#include "war3ft/effects.inl"
#include "war3ft/XP.inl"
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

#include "war3ft/race_orc.inl"
#include "war3ft/race_chameleon.inl"

#include "war3ft/cstrike.inl"
#include "war3ft/dod.inl"

public plugin_init()
{
	register_plugin(WC3NAME,WC3VERSION,WC3AUTHOR)

	// Lets determine which game is running
	WAR3_Determine_Game();

	gmsgDeathMsg		= get_user_msgid( "DeathMsg"	);
	gmsgScreenFade		= get_user_msgid( "ScreenFade"	);
	gmsgScreenShake		= get_user_msgid( "ScreenShake"	);
	gmsgScoreInfo		= get_user_msgid( "ScoreInfo"	);
	
	register_clcmd( "war3menu"			, "menu_War3menu"		, -1 );
	register_clcmd( "changerace"		, "change_race"			, -1 );
	register_clcmd( "selectskill"		, "menu_Select_Skill"	, -1 );
	register_clcmd( "skillsinfo"		, "MOTD_Skillsinfo"		, -1 );
	register_clcmd( "resetskills"		, "cmd_ResetSkill"		, -1 );
	register_clcmd( "resetxp"			, "XP_Reset"			, -1 );
	register_clcmd( "itemsinfo"			, "MOTD_Itemsinfo"		, -1 );
	register_clcmd( "itemsinfo2"		, "MOTD_Itemsinfo2"		, -1 );
	register_clcmd( "war3help"			, "MOTD_War3help"		, -1 );
	register_clcmd( "ultimate"			, "cmd_Ultimate"		, -1 );
	register_clcmd( "shopmenu"			, "menu_Shopmenu_One"	, -1 );
	register_clcmd( "shopmenu2"			, "menu_Shopmenu_Two"	, -1 );
	register_clcmd( "ability"			, "cmd_ability"			, -1 );
	register_clcmd( "rings"				, "cmd_Rings"			, -1 );
	register_clcmd( "fullupdate"		, "cmd_fullupdate"		, -1 );
	register_clcmd( "say"				, "cmd_Say"				, -1 );
	register_clcmd( "say_team"			, "cmd_Say"				, -1 );
	register_clcmd( "jointeam"			, "cmd_Jointeam"		, -1 );
	register_clcmd( "level"				, "cmd_Level"			, -1 );
	register_clcmd( "drop"				, "on_Drop"				, -1 );

	register_concmd( "playerskills"		, "MOTD_Playerskills"	, -1 );

	// Admin Commands
	register_concmd( "amx_givexp"		, "Admin_GiveXP"		, 0 , " -- Gives XP to players"				);
	register_concmd( "amx_savexp"		, "Admin_SaveXP"		, 0 , " -- Saves XP for players"			);
	register_concmd( "amx_givemole"		, "Admin_GiveMole"		, 0 , " -- Gives the mole item to a player"	);
	register_concmd( "amx_wc3"			, "ADMIN_wc3"			, 0 , " -- Enables/disables war3ft"			);
	
	// Server Admin Commands (used by external plugins)
	register_srvcmd( "amx_takexp"		, "Admin_TakeXP"	);
	register_srvcmd( "changexp"			, "changeXP"		);
	
	// Register forwards (from fakemeta)
	register_forward( FM_TraceLine		, "traceline"		);

	register_event( "DeathMsg"			, "on_DeathMsg"		,"a"								);
	register_event( "CurWeapon"			, "on_CurWeapon"	,"be"	, "1=1"						);
	register_event( "HideWeapon"		, "on_CurWeapon"	, "b"								);
	register_event( "ResetHUD"			, "on_ResetHud"		, "b"								);
	register_event( "TextMsg"			, "on_GameRestart"	,"a"	, "2&#Game_will_restart_in" );

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
	WAR3_Init();

	register_concmd( "test", "test" );
}

public test()
{
	ADMIN_Print( 0, "This is a %d fucking %s message", 5, "asdf" );
}

public plugin_end()
{
	if ( !get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		return;
	}
	
	XP_Save_All();
	XP_Prune();
	XP_CloseDB();

	return;
}

public plugin_precache()
{
	WAR3_precache()
}

public client_putinserver(id)
{
	// Check for steam ID pending
	new szPlayerID[32];
	get_user_authid( id, szPlayerID, 31 );
	
	// Then the player doesn't have a steam id, lets make them reconnect
	if ( equal(szPlayerID, "STEAM_ID_PENDING") )
	{
		client_cmd(id, "reconnect");
	}
	// Update all XP records to the current timestamp
	else
	{
		XP_Prune_Player(id);
	}

	p_data_b[id][PB_ISCONNECTED] = true;

	#if MOD == 1
		p_data[id][P_MONEY] = get_pcvar_num( CVAR_wc3_dod_start_money );
		new parm[3];
		parm[0] = id;
		parm[1] = 0;
		_DOD_showMoney( parm );
	#endif
	#if MOD == 0
		if ( !is_user_bot(id) )
		{
			query_client_cvar( id, "cl_minmodels", "check_cvars" );
		}
	#endif
}

public client_connect(id)
{
	
	client_cmd( id, "hud_centerid 0" );

	p_data[id][P_RACE]			= 0;
	p_data[id][P_SKILL1]		= 0;
	p_data[id][P_SKILL2]		= 0;
	p_data[id][P_SKILL3]		= 0;
	p_data[id][P_ULTIMATE]		= 0;
	p_data[id][P_LEVEL]			= 0;
	p_data[id][P_XP]			= 0;
	p_data[id][P_ITEM]			= 0;
	p_data[id][P_ITEM2]			= 0;
	p_data_b[id][PB_ISBURNING]	= false;
	p_data[id][P_SPECMODE]		= 0;
	p_data_b[id][PB_JUSTJOINED] = true;
	p_data_b[id][PB_CAN_RENDER]		= true;
	
	// These were on disconnect, might as well do them on connect
	p_data[id][P_HECOUNT]		= 0;
	p_data[id][P_FLASHCOUNT]	= 0;

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

	#if MOD == 1
		// Skip Reincarnation since the user just joined
		p_data_b[id][PB_REINCARNATION_SKIP] = true;
	#endif
	#if MOD == 0
		p_data[id][P_HECOUNT]		= 0;
		p_data[id][P_FLASHCOUNT]	= 0;
	#endif
	
	// Give the bot a random amount of XP
	if ( is_user_bot(id) && get_pcvar_num( CVAR_wc3_save_xp ) )
	{
		p_data[id][P_XP] = xplevel[floatround(random_float(0.0,3.16)*random_float(0.0,3.16))];
		p_data[id][P_RACE] = random_num(1, get_pcvar_num( CVAR_wc3_races ));
	}

	return PLUGIN_CONTINUE;
}

public client_disconnect(id)
{
	#if MOD == 1
		// Remove the money task when a user disconnects

		if( task_exists( TASK_MONEYLOOP+id ) )
		{
			remove_task( TASK_MONEYLOOP+id );
		}
		p_data[id][P_MONEY] = 0;
	#endif

	p_data[id][P_SPECMODE]			= 0;
	p_data_b[id][PB_ISBURNING]		= false;
	p_data_b[id][PB_DIEDLASTROUND]	= false;
	p_data_b[id][PB_JUSTJOINED]		= false;
	p_data_b[id][PB_ISCONNECTED]	= false;
	
	// Reset Equipment Reincarnation
	new i=0;
	for (i=0; i<32; ++i)
	{
		savedweapons[id][i] = 0;
	}
	
	// Save the user's XP if we have XP to save

	if (get_pcvar_num( CVAR_wc3_save_xp ) && !is_user_bot(id) && p_data[id][P_RACE] && p_data[id][P_XP])
	{
		XP_Save( id );
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
			case CSW_FLAME:			lang_GetSkillName( 5				, 4				, LANG_SERVER,	szWeapon,	63 );
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

			// Save Statistics For War3 Abilities (allows for detailed reports with psychostats)

			#if MOD == 0
				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats^" (weapon ^"%s^") (shots ^"%d^") (hits ^"%d^") (kills ^"%d^") (headshots ^"%d^") (tks ^"%d^") (damage ^"%d^") (deaths ^"%d^")", szName,iUserid,szAuthid,szTeam,szWeapon,iStatsShots[id][WEAPON],iStatsHits[id][WEAPON],iStatsKills[id][WEAPON], iStatsHS[id][WEAPON],iStatsTKS[id][WEAPON],iStatsDamage[id][WEAPON],iStatsDeaths[id][WEAPON])
			#endif
			#if MOD == 1
				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats^" (weapon ^"%s^") (shots ^"%d^") (hits ^"%d^") (kills ^"%d^") (headshots ^"%d^") (tks ^"%d^") (damage ^"%d^") (deaths ^"%d^") (score ^"%d^")", szName,iUserid,szAuthid,szTeam,szWeapon,iStatsShots[id][WEAPON],iStatsHits[id][WEAPON],iStatsKills[id][WEAPON], iStatsHS[id][WEAPON],iStatsTKS[id][WEAPON],iStatsDamage[id][WEAPON],iStatsDeaths[id][WEAPON],0)
			#endif
			log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats2^" (weapon ^"%s^") (head ^"%d^") (chest ^"%d^") (stomach ^"%d^") (leftarm ^"%d^") (rightarm ^"%d^") (leftleg ^"%d^") (rightleg ^"%d^")", szName,iUserid,szAuthid,szTeam,szWeapon,iStatsHead[id][WEAPON],iStatsChest[id][WEAPON],iStatsStomach[id][WEAPON],  iStatsLeftArm[id][WEAPON],iStatsRightArm[id][WEAPON],iStatsLeftLeg[id][WEAPON],iStatsRightLeg[id][WEAPON])
		
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



public client_PreThink(id)
{

	if ( !warcraft3 )
	{
		return PLUGIN_CONTINUE;
	}

	if( p_data_b[id][PB_ISCONNECTED] )
	{

		if( is_user_alive(id) )
		{
	#if MOD == 0
			if ( Verify_Skill(id, RACE_UNDEAD, SKILL3) && !p_data_b[id][PB_STUNNED] && !p_data_b[id][PB_SLOWED])
			{
				new Float:vel[3];
				entity_get_vector( id, EV_VEC_velocity, vel );
				new Float:length = vector_length( vel );
				if (length < 180.0)
				{
					entity_set_int( id, EV_INT_flTimeStepSound, 999 );
				}
				else if (entity_get_int(id, EV_INT_flTimeStepSound) > 500)
				{
					entity_set_int( id, EV_INT_flTimeStepSound, 200 );
				}
			}
	#endif
			if(p_data_b[id][PB_SILENT])
			{
				entity_set_int( id, EV_INT_flTimeStepSound, 999 );
			}
		}
	#if MOD == 1
		if( Verify_Skill(id, RACE_UNDEAD, SKILL2) || p_data[id][P_ITEM] == ITEM_BOOTS)
		{
			// They have a rocket launcher "deployed" or are using their stamina
			new prone = entity_get_int( id, EV_INT_iuser3 );
			new Float:maxspeed = entity_get_float( id, EV_FL_maxspeed );
			if( ( maxspeed == 50.0 && !prone ) || entity_get_float( id, EV_FL_fuser4 ) < 100.0 || ( maxspeed > 500.0 && prone ) )
			{		
				new parm[1];
				parm[0] = id;
				unholyspeed( parm );
			}
			if ( Verify_Skill(id, RACE_UNDEAD, SKILL2) )
			{
				if( entity_get_float( id, EV_FL_fuser4 ) < p_unholy[p_data[id][P_SKILL2]-1] )
				{
					entity_set_float( id, EV_FL_fuser4, p_unholy[p_data[id][P_SKILL2]-1] );
				}
			}
		}
		if( p_data[id][P_ITEM] == ITEM_BOOTS && entity_get_float(id,EV_FL_fuser4) < DOD_BOOT_SPEED )
		{
			entity_set_float( id, EV_FL_fuser4, DOD_BOOT_SPEED );
		}
	#endif
	}

	return PLUGIN_CONTINUE;
}

// This functionality allows us to no longer requires a DBI module to be loaded
public plugin_natives()
{
	set_module_filter("module_filter");
	set_native_filter("native_filter");
}

public module_filter(const module[])
{
	// Then we obviously don't want to save XP via mysql or SQLite now do we?  Lets default to vault and print an error message
	if ( equali( module, "dbi" ) )
	{
		g_DBILoaded = false;
		log_amx("[ERROR] No DBI module found, defaulting to vault");

		MODULE_Enable( "mysql_amxx" );

		return PLUGIN_HANDLED;
	}
	else if ( equali( module, "dod" ) )
	{
		return PLUGIN_HANDLED;
	}
	else if ( equali( module, "dodx" ) )
	{
		return PLUGIN_HANDLED;
	}
	else if ( equali( module, "cstrike" ) )
	{
		return PLUGIN_HANDLED;
	}

	return PLUGIN_CONTINUE;
}

public native_filter(const name[], index, trap)
{
      if (!trap)
            return PLUGIN_HANDLED;

      return PLUGIN_CONTINUE;
}

public MODULE_Enable( module_name[] )
{

	new fp = fopen( "addons/amxmodx/configs/modules.ini", "r+t" );
	new data[128];

	while( !feof( fp ) )
	{
		fgets( fp, data, 63 );
		
		server_print( "Searching %s", data );

		// Module Name Found
		if ( contain( data, module_name ) != -1 )
		{
			// Semicolon before module name found
			if ( contain( data, ";" ) != -1 )
			{
				// Lets go to the start of the line we're on so we can remove the ;
				fseek( fp, strlen(data), SEEK_CUR );
				
				// Remove the ; from the line
				replace( data, strlen(data), ";" , "" );

				server_print( "%s", data );
				
				new len = strlen( data );
				for ( new i = 0; i < len; i++ )
				{
					fputc( fp, data[i] );
					server_print( "%c", data[i] );
				}

				server_print( "done" );
			}
		}
	} 

	fclose( fp );
}