/* AMXMOD X script. 
*
*  Warcraft 3: Frozen Throne
*  by Geesu==Pimp Daddy==OoTOAoO=
*  http://www.war3ft.com
*  http://www.4hm.net (check forums)
*
*  Credits to:
*  Spacedude (for War3 MOD)
*  Ludwig Van (for flamethrower)
*  OLO (for spectating rank info)
*  JGHG for the mole code
*  [AOL]Demandred, [AOL]LuckyJ for help coding it for steam
*  [AOL]Demandred for freezetime exploit fix
*  Everyone at amxmod.net/amxmodx.org for help
*  joecool12321 for various health related fixes
*  Tri Moon for various improvements (No Race, war3menu, etc...)
*  xeroblood for spotting some bugs for me :)
*  bad-at-this for contributing the status bar code used for godmode (big bad voodoo)
*  bad-at-this for creating race 9
*  kamikaze for...
 		help w/testing version before release
		always helping people out on the forums
		contributing code for the anti-skywalking
*  ryannotfound (war3x.net) for some of the naming conventions used (function names, constants, etc...) and functions
*  Lazarus Long for adding ALL of the sql-lite code and fine-tuning the existing MySQL code... It's so much pertier
*/

//#pragma dynamic 4096 

// Death animations for alien4: 56-65

new const WC3NAME[] =		"Warcraft 3 Frozen Throne"
new const WC3AUTHOR[] =		"Geesu==(Pimp Daddy==OoTOAoO)"
new const WC3VERSION[] =	"2.2.8"

#include <amxmodx>
#include <dbi>
#include <engine>
#include <amxmisc>
#include <fun>
#include <war3ft>

#define MOD 0							// 0 = cstrike or czero, 1 = dod
#define ADMIN_LEVEL_WC3 ADMIN_LEVEL_A	// set the admin level required for giving xp and accessing the admin menu (see amxconst.inc)
#define ADVANCED_STATS 1				// Setting this to 1 will give detailed information with psychostats (hits, damage, hitplace, etc..) for war3 abilities
#define PRECACHE_WAR3FTSOUNDS 1

// Debugging Options
#define DEBUG 0 						// Only use this when coding.. you normally don't want it
#define ADVANCED_DEBUG 0				// Prints debug information to a log file when every function is called, VERY LAGGY
#define ADVANCED_DEBUG_BOTS 1			// Print info for bots too?
#define SQL_DEBUG 0						// Prints debug information regarding SQL statments and replies

#if MOD == 0
	#include <cstrike>
#else
	#include <dodfun>
	#include <dodx>
#endif

#include "war3ft/constants.inl"
#include "war3ft/effects.inl"
#include "war3ft/XP.inl"
#include "war3ft/war3ft.inl"
#include "war3ft/events.inl"
#include "war3ft/clientCommands.inl"
#include "war3ft/items.inl"
#include "war3ft/stocks.inl"
#include "war3ft/ultimates.inl"
#include "war3ft/skills.inl"
#include "war3ft/menus.inl"
#include "war3ft/motd.inl"
#include "war3ft/language.inl"
#include "war3ft/other.inl"
#include "war3ft/admin.inl"

#if MOD == 0
	#include "war3ft/cstrike.inl"
#endif

#if MOD == 1
	#include "war3ft/dod.inl"
#endif

#if DEBUG || ADVANCED_DEBUG
	#include "war3ft/debug.inl"
#endif

public plugin_init(){
	#if ADVANCED_DEBUG
		writeDebugInfo("plugin_init",0)
	#endif

	#if DEBUG
		//register_srvcmd("testage", "testage")
	#endif

	gmsgDeathMsg = get_user_msgid("DeathMsg")
	gmsgScreenFade = get_user_msgid("ScreenFade")
	gmsgScreenShake = get_user_msgid("ScreenShake")
	gmsgScoreInfo = get_user_msgid("ScoreInfo")

	#if MOD == 0
		gmsgBarTime = get_user_msgid("BarTime")
		gmsgStatusText = get_user_msgid("StatusText")
		gmsgStatusIcon = get_user_msgid("StatusIcon") 
	#endif
	#if MOD == 1
		gmsgHudText = get_user_msgid("HudText")
	#endif

	#if DEBUG
		//register_message(get_user_msgid("CurWeapon"), "testing")
	#endif

	register_plugin(WC3NAME,WC3VERSION,WC3AUTHOR)
	register_cvar("War3: Frozen Throne", WC3VERSION,FCVAR_SERVER)
	new WC3AMXCVAR[32]
	format(WC3AMXCVAR,31,"%s %s", WC3NAME,WC3VERSION)
	register_cvar("amx_war3_version", WC3AMXCVAR,FCVAR_SERVER)

	register_clcmd("amx_wc3","amx_wc3_launch",-1,"Activate/Deactivate Controller & WAR3FT Plugin")		// Added by NeKo
	register_clcmd("war3menu","menu_War3menu",-1,"- Show Warcraft 3 Frozen Throne Player menu")
	register_clcmd("changerace","change_race",-1,"changerace")
	register_clcmd("selectskill","menu_Select_Skill",-1,"selectskill")
	register_clcmd("skillsinfo","MOTD_Skillsinfo",-1,"skillsinfo")
	register_clcmd("resetskills","cmd_ResetSkill",-1,"resetskills")
	register_clcmd("resetxp","XP_Reset",-1,"resetxp")
	register_clcmd("itemsinfo","MOTD_Itemsinfo",-1,"itemsinfo")
	register_clcmd("itemsinfo2","MOTD_Itemsinfo2",-1,"itemsinfo2")
	register_clcmd("war3help","MOTD_War3help",-1,"war3help")
	register_clcmd("ultimate","cmd_Ultimate",-1,"ultimate")
	register_clcmd("shopmenu","menu_Shopmenu_One",-1,"shopmenu")
	register_clcmd("shopmenu2","menu_Shopmenu_Two",-1,"shopmenu2")

	register_clcmd("ability","cmd_ability",-1,"ability")
	register_clcmd("rings","cmd_Rings",-1,"rings")
	register_clcmd("fullupdate", "cmd_fullupdate")
	register_clcmd("say","cmd_Say")
	register_clcmd("say_team","cmd_Say")
	register_clcmd("jointeam","cmd_Jointeam") 
	register_clcmd("level","cmd_Level")

	register_concmd("playerskills","MOTD_Playerskills",-1,"playerskills")

	// Admin Commands
	register_concmd("amx_givexp","Admin_GiveXP",-1,"amx_givexp")
	register_concmd("amx_savexp","Admin_SaveXP",-1,"amx_savexp")
	register_concmd("amx_givemole","Admin_GiveMole",-1,"amx_givemole")
	
	// Server Admin Commands (used by external plugins)
	register_srvcmd("amx_takexp","Admin_TakeXP")
	register_srvcmd("changexp","changeXP")
	
	#if MOD == 1
		register_statsfwd(XMF_SCORE)
		register_statsfwd(XMF_DAMAGE)

		register_event("RoundState","on_EndRound","a","1=3","1=4")

		//register_event("TextMsg","on_Spectate","a","2=#game_joined_team")
		//register_event("TextMsg","on_SetSpecMode","b","2&#Spec_Mode")

		register_event("StatusValue","on_StatusValue","b")
	#endif

	#if MOD == 0
		register_clcmd("drop", "cmd_Drop")

		register_logevent("on_PlayerAction",3,"1=triggered") 
		register_logevent("on_FreezeTimeComplete",2,"0=World triggered","1=Round_Start")
		register_logevent("on_EndRound",2,"0=World triggered","1=Round_End")

		register_event("SendAudio", "on_TerroristWin", "a", "2=%!MRAD_terwin")
		register_event("SendAudio", "on_CTWin", "a", "2=%!MRAD_ctwin")

		register_event("23", "on_TargetBombed", "a", "1=17", "6=-105", "7=17")

		register_event("ArmorType", "on_ArmorType", "be")
		register_event("WeapPickup","on_WeapPickup","b") 

		register_event("StatusValue","on_ShowStatus","be","1=2","2!0")
		register_event("StatusValue","on_HideStatus","be","1=1","2=0")  

		register_event("StatusIcon", "on_StatusIcon",  "be")

		register_event("TextMsg","on_SetSpecMode","bd","2&ec_Mod")

		register_event("Damage", "on_Damage", "b", "2!0")

		register_event("StatusValue","on_Spectate","bd","1=2")

		register_menucmd(-34,(1<<8),"cmd_Shield")

		// Old Style
		register_menucmd(register_menuid("BuyItem"),(1<<2),"cmd_flash")
		register_menucmd(register_menuid("BuyItem"),(1<<3),"cmd_hegren")

		// VGUI
		register_menucmd(-34,(1<<2),"cmd_flash")
		register_menucmd(-34,(1<<3),"cmd_hegren")

		// Steam
		register_clcmd("flash",  "cmd_flash")
		register_clcmd("hegren", "cmd_hegren")
		register_clcmd("shield", "cmd_Shield")

		register_menucmd(register_menuid("Team_Select",1),(1<<0)|(1<<1)|(1<<4),"cmd_Teamselect")

		set_task(1.0, "WAR3_Mole_Fix", TASK_MOLEFIX)
	#endif

	register_event("DeathMsg","on_DeathMsg","a")

	register_event("CurWeapon","on_CurWeapon","be","1=1")
	register_event("HideWeapon", "on_CurWeapon", "b")

	register_event("ResetHUD", "on_ResetHud", "b")


	register_event("TextMsg","on_GameRestart","a","2&#Game_will_restart_in")

	if(is_running("czero")){
		register_event("TextMsg", "on_GameRestart", "a", "2&#Game_Commencing")
	}
	else{
		register_event("TextMsg", "on_GameRestart", "a", "2&#Game_C")
	}

	// For an explanation of these variables, please see war3ft.cfg
	register_cvar("FT_query_client",			"1")
	register_cvar("FT_disable_savexp",			"0")
	register_cvar("FT_impale_intensity",		"10")
	register_cvar("FT_autoxp",					"0")
	register_cvar("FT_show_player",				"1")
	register_cvar("FT_Race9_Random",			"1")
	register_cvar("FT_Race9_Skill1",			"1")
	register_cvar("FT_Race9_Skill2",			"1")
	register_cvar("FT_Race9_Skill3",			"1")
	register_cvar("FT_Race9_Ultimate",			"1")
	register_cvar("FT_entangle_drop",			"0")
	register_cvar("FT_sock",					"0.5")
	register_cvar("FT_buydead",					"1")
	register_cvar("FT_buytime",					"0")
	register_cvar("FT_buyzone",					"0")
	register_cvar("FT_races",					"8")
	register_cvar("FT_bot_buy_item",			"0.33")
	register_cvar("FT_race_icons",				"1")
	register_cvar("FT_level_icons",				"1")
	register_cvar("FT_claw",					"6")
#if MOD == 0
	register_cvar("FT_items_in_hud",			"0")
	register_cvar("FT_bootspeed",				"275")
#endif
#if MOD == 1
	register_cvar("DOD_bootspeed",				"45.0")
	register_cvar("DOD_startmoney",				"800")
#endif
	register_cvar("FT_health_bonus",			"15")
	register_cvar("FT_frost_speed",				"125")
	register_cvar("FT_mask_of_death",			"0.3")
	register_cvar("FT_cloak",					"150")
	register_cvar("FT_CD",						"0")
	register_cvar("FT_ultimatedelay",			"15.0")
	register_cvar("FT_min_b4_XP",				"2")
	register_cvar("FT_no_orcnades",				"0")
	register_cvar("FT_centerhud",				"1")
	register_cvar("FT_saveby",					"0")
	register_cvar("FT_position",				"0")
	register_cvar("FT_glove_timer",				"10")
	register_cvar("FT_glove_orc_damage",		"0")
	register_cvar("FT_round_win_XP",			"35")
	register_cvar("FT_healing_range",			"750")
	register_cvar("FT_xp_radius",				"750")
	register_cvar("FT_ultimate_cooldown",		"35.0")
	register_cvar("FT_kill_objectives",			"0")
	register_cvar("FT_show_icons",				"1")
	register_cvar("FT_headshot_bonus",			"9")
	register_cvar("FT_defuser_kill_bonus",		"25")
	register_cvar("FT_VIP_escape_bonus",		"30")
	register_cvar("FT_kill_bomb_carrier_bonus",	"25")
	register_cvar("FT_bombplanterxp",			"15")
	register_cvar("FT_defusexp",				"30")
	register_cvar("FT_hostagexp",				"25")
	register_cvar("FT_killrescuemanxp",			"25")
	register_cvar("FT_xpbonus",					"50")
	register_cvar("FT_VIP_kill_bonus",			"25")
	register_cvar("FT_warn_suicide",			"1")
	register_cvar("FT_no_gloves_on_ka",			"1")
	register_cvar("FT_blink_radius",			"500")
	register_cvar("FT_blink_protection",		"1")
	register_cvar("FT_blink_dizziness",			"1")
	register_cvar("FT_blinkenabled",			"1")
	register_cvar("FT_spec_info",				"1")
	register_cvar("FT_objectives",				"1")
	register_cvar("FT_auto_pruning",			"0",FCVAR_SERVER)
	register_cvar("mp_savexp",					"0",FCVAR_SERVER)
	register_cvar("mp_xpmultiplier",			"1.0")
	register_cvar("mp_weaponxpmodifier",		"1")
	register_cvar("sv_warcraft3",				"1")
	register_cvar("sv_allowwar3vote",			"0")
	register_cvar("mp_grenadeprotection",		"0")
	register_cvar("sv_save_end_round",			"1")
	register_cvar("sv_daysbeforedelete",		"31")
	register_cvar("sv_mysql",					"0")
	register_cvar("sv_mysqltablename",			"war3users")
	register_cvar("FT_mysql_host",				"127.0.0.1")
	register_cvar("FT_mysql_user",				"root")
	register_cvar("FT_mysql_pass",				"")
	register_cvar("FT_mysql_db",				"amx")

	WAR3_exec_config()

	set_task(2.0, "WAR3_Set_Variables", TASK_SETVARIABLES)
	set_task(7.5, "WAR3_Check",TASK_WAR3CHECK,"",0,"b")

	register_dictionary("war3FT.txt")

	#if DEBUG
		register_concmd("test","test")
		register_concmd("test2","test2")
		register_concmd("test3","test3")
	#endif

	register_concmd("check_evasion", "Skill_Evasion_Check_C");
	register_concmd("evasion", "Skill_Evasion_Set");
	register_concmd("prune", "XP_Prune");

}

public plugin_end()
{
	#if ADVANCED_DEBUG
		writeDebugInfo("plugin_end",0)
	#endif

	if (!warcraft3 || !iCvar[MP_SAVEXP])
		return PLUGIN_CONTINUE
	
	XP_Save_All()
	XP_Prune();
	XP_CloseDB();

	return PLUGIN_CONTINUE
}

public plugin_precache() {
	#if ADVANCED_DEBUG
		writeDebugInfo("plugin_precache",0)
	#endif

	WAR3_precache()

	return PLUGIN_CONTINUE
}

public client_putinserver(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("client_putinserver",id)
	#endif

	p_data_b[id][PB_ISCONNECTED] = true

	#if MOD == 1
		p_data[id][P_MONEY] = iCvar[DOD_STARTMONEY]
		new parm[3]
		parm[0] = id
		parm[1] = 0
		_DOD_showMoney(parm)
	#endif

	#if MOD == 0
		query_client_cvar(id, "cl_minmodels", "check_cvars") 
	#endif

}

public client_connect(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("client_connect",id)
	#endif

	client_cmd(id, "hud_centerid 0")

	p_data[id][P_RACE] = 0
	p_data[id][P_SKILL1] = 0
	p_data[id][P_SKILL2] = 0
	p_data[id][P_SKILL3] = 0
	p_data[id][P_ULTIMATE] = 0
	p_data[id][P_LEVEL] = 0
	p_data[id][P_XP] = 0
	p_data[id][P_ITEM] = 0
	p_data[id][P_ITEM2] = 0
	p_data_b[id][PB_ISBURNING] = false
	p_data[id][P_SPECMODE] = 0 
	p_data_b[id][PB_JUSTJOINED] = true
	p_data_b[id][PB_RENDER] = true

	if ( iCvar[FT_AUTOXP] && !iCvar[MP_SAVEXP] ){
		new iTotalXP
		new iNum
		for(new i = 1; i <= MAXPLAYERS; i++){
			if ( p_data[i][P_XP] > 0 ){
				iNum++
				iTotalXP += p_data[i][P_XP]
			}
		}
		if ( iNum > 0 && iTotalXP > 0 ){
			p_data[id][P_XP] = iTotalXP/iNum
		}
	}

#if MOD == 1
	reincarnation[id][ZPOS] = -99999
#endif

	#if MOD == 0
		p_data[id][P_HECOUNT] = 0
		p_data[id][P_FLASHCOUNT]=0
	#endif

	if (is_user_bot(id) && iCvar[MP_SAVEXP]){
		p_data[id][P_XP]=xplevel[floatround(random_float(0.0,3.16)*random_float(0.0,3.16))]
		p_data[id][P_RACE] = random_num(1,iCvar[FT_RACES])
		return PLUGIN_CONTINUE
	}
	return PLUGIN_CONTINUE
}


public client_disconnect(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("client_disconnect",id)
	#endif

	#if MOD == 0
		p_data[id][P_HECOUNT]=0
		p_data[id][P_FLASHCOUNT]=0
	#endif
	#if MOD == 1
		if(task_exists(TASK_MONEYLOOP+id))		// Remove the money task when a user disconnects
			remove_task(TASK_MONEYLOOP+id)
		p_data[id][P_MONEY] = 0
	#endif
	p_data[id][P_SPECMODE] = 0
	p_data_b[id][PB_ISBURNING] = false
	p_data_b[id][PB_DIEDLASTROUND]=false
	p_data_b[id][PB_JUSTJOINED] = false
	p_data_b[id][PB_ISCONNECTED] = false

	new i=0
	for (i=0; i<32; ++i){		// Equipment Reincarnation
		savedweapons[id][i]=0
	}

	if (iCvar[MP_SAVEXP] && !is_user_bot(id) && p_data[id][P_RACE] && p_data[id][P_XP])
		XP_Save(id)

#if ADVANCED_STATS
	new szWeapon[64]

	new szTeam[16], szName[32], szAuthid[32]
	new iUserid = get_user_userid( id )
	if ( is_user_connected(id) )
	{
		get_user_team(id, szTeam, 15 )
	}
	get_user_name(id, szName ,31 )
	get_user_authid(id, szAuthid , 31 )

	for(new weap = CSW_WAR3_MIN; weap <=CSW_WAR3_MAX; weap++){
		format(szWeapon, 63, "")

		switch( weap ){
			case CSW_LIGHTNING:     raceskill(3,4,LANG_SERVER,szWeapon,63)
			case CSW_SUICIDE:		raceskill(1,4,LANG_SERVER,szWeapon,63)
			case CSW_FLAME:			raceskill(5,4,LANG_SERVER,szWeapon,63)
			case CSW_LOCUSTS:		raceskill(8,4,LANG_SERVER,szWeapon,63)
			case CSW_SERPENTWARD:   raceskill(6,3,LANG_SERVER,szWeapon,63)
			case CSW_SHADOW:		raceskill(7,3,LANG_SERVER,szWeapon,63)
			case CSW_THORNS:		raceskill(4,2,LANG_SERVER,szWeapon,63)
			case CSW_CARAPACE:		raceskill(8,2,LANG_SERVER,szWeapon,63)
			case CSW_CARRION:		raceskill(8,3,LANG_SERVER,szWeapon,63)
			case CSW_ORB:			Lang_Hero_Skill_Name(RACE_CRYPT,SKILL_HERO,LANG_SERVER,szWeapon,63)
			case CSW_CONCOCTION:	Lang_Hero_Skill_Name(RACE_SHADOW,SKILL_HERO,LANG_SERVER,szWeapon,63)
		}
		
		replace(szWeapon, 63, " ", "_")

		new WEAPON = weap - CSW_WAR3_MIN
		
		if ( iStatsShots[id][WEAPON] || iStatsHits[id][WEAPON] || iStatsKills[id][WEAPON] ||  iStatsHS[id][WEAPON] || iStatsTKS[id][WEAPON] || iStatsDamage[id][WEAPON] || iStatsDeaths[id][WEAPON] || iStatsHead[id][WEAPON] || iStatsChest[id][WEAPON] || iStatsStomach[id][WEAPON] || iStatsLeftArm[id][WEAPON] || iStatsRightArm[id][WEAPON] || iStatsLeftLeg[id][WEAPON] || iStatsRightLeg[id][WEAPON] ){
			// Save Statistics For War3 Abilities (allows for detailed reports with psychostats)
			#if MOD == 0
				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats^" (weapon ^"%s^") (shots ^"%d^") (hits ^"%d^") (kills ^"%d^") (headshots ^"%d^") (tks ^"%d^") (damage ^"%d^") (deaths ^"%d^")",
					szName,iUserid,szAuthid,szTeam,szWeapon,iStatsShots[id][WEAPON],iStatsHits[id][WEAPON],iStatsKills[id][WEAPON], iStatsHS[id][WEAPON],iStatsTKS[id][WEAPON],iStatsDamage[id][WEAPON],iStatsDeaths[id][WEAPON])
			#endif
			#if MOD == 1
				log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats^" (weapon ^"%s^") (shots ^"%d^") (hits ^"%d^") (kills ^"%d^") (headshots ^"%d^") (tks ^"%d^") (damage ^"%d^") (deaths ^"%d^") (score ^"%d^")",
					szName,iUserid,szAuthid,szTeam,szWeapon,iStatsShots[id][WEAPON],iStatsHits[id][WEAPON],iStatsKills[id][WEAPON], iStatsHS[id][WEAPON],iStatsTKS[id][WEAPON],iStatsDamage[id][WEAPON],iStatsDeaths[id][WEAPON],0)
			#endif
			log_message("^"%s<%d><%s><%s>^" triggered ^"weaponstats2^" (weapon ^"%s^") (head ^"%d^") (chest ^"%d^") (stomach ^"%d^") (leftarm ^"%d^") (rightarm ^"%d^") (leftleg ^"%d^") (rightleg ^"%d^")",
				szName,iUserid,szAuthid,szTeam,szWeapon,iStatsHead[id][WEAPON],iStatsChest[id][WEAPON],iStatsStomach[id][WEAPON],  iStatsLeftArm[id][WEAPON],iStatsRightArm[id][WEAPON],iStatsLeftLeg[id][WEAPON],iStatsRightLeg[id][WEAPON])
		
			iStatsShots[id][WEAPON] = 0
			iStatsHits[id][WEAPON] = 0
			iStatsKills[id][WEAPON] = 0
			iStatsHS[id][WEAPON] = 0
			iStatsTKS[id][WEAPON] = 0
			iStatsDamage[id][WEAPON] = 0
			iStatsDeaths[id][WEAPON] = 0
			iStatsHead[id][WEAPON] = 0
			iStatsChest[id][WEAPON] = 0
			iStatsStomach[id][WEAPON] = 0
			iStatsLeftArm[id][WEAPON] = 0
			iStatsRightArm[id][WEAPON] = 0
			iStatsLeftLeg[id][WEAPON] = 0
			iStatsRightLeg[id][WEAPON] = 0
		}
	}
#endif

	return PLUGIN_CONTINUE
}



public client_PreThink(id){

	if(p_data_b[id][PB_ISCONNECTED]){

		if(is_user_alive(id)){
#if MOD == 0
			if ( Verify_Skill(id, RACE_UNDEAD, SKILL3) && !p_data_b[id][PB_STUNNED] && !p_data_b[id][PB_SLOWED]){
				new Float:vel[3]
				entity_get_vector(id, EV_VEC_velocity, vel)
				new Float:length = vector_length(vel)
				if (length < 180.0){
					entity_set_int(id, EV_INT_flTimeStepSound, 999)
				}
				else if (entity_get_int(id, EV_INT_flTimeStepSound) > 500){
					entity_set_int(id, EV_INT_flTimeStepSound, 200)
				}
			}
#endif
			if(p_data_b[id][PB_SILENT]){
				entity_set_int(id, EV_INT_flTimeStepSound, 999)
			}
		}
/*		if(!p_data_b[id][PB_DIEDLASTROUND] && (get_user_team(id) == CTS || get_user_team(id) == TS)){	// Then player is dead
			if(entity_get_int(id,EV_INT_deadflag)>0 && !p_data_b[id][PB_DIEDLASTROUND]){
				//client_death(0,id,-1,0,0)
				//WAR3_death_victim(id, 0)
				//p_data_b[id][PB_DIEDLASTROUND] = true
			}
		}*/
	#if MOD == 1
		if( Verify_Skill(id, RACE_UNDEAD, SKILL2) || p_data[id][P_ITEM] == ITEM_BOOTS){
			// They have a rocket launcher "deployed" or are using their stamina
			new prone = entity_get_int(id,EV_INT_iuser3)
			new Float:maxspeed = entity_get_float(id,EV_FL_maxspeed)
			if((maxspeed==50.0 && !prone) || entity_get_float(id,EV_FL_fuser4)<100.0 || (maxspeed>500.0 && prone)){		
				new parm[1]
				parm[0] = id
				unholyspeed(parm)
			}
			if ( Verify_Skill(id, RACE_UNDEAD, SKILL2) ){
				if(entity_get_float(id,EV_FL_fuser4) < p_unholy[p_data[id][P_SKILL2]-1]){
					entity_set_float(id,EV_FL_fuser4, p_unholy[p_data[id][P_SKILL2]-1])
				}
			}
		}
		if(p_data[id][P_ITEM] == ITEM_BOOTS && entity_get_float(id,EV_FL_fuser4) < fCvar[DOD_BOOTSPEED]){
			entity_set_float(id,EV_FL_fuser4, fCvar[DOD_BOOTSPEED])
		}
	#endif
	}
}