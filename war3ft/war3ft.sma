/* AMXMOD X script. 
*
*  Warcraft 3: Frozen Throne
*  by Pimp Daddy (OoTOAoO)
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
*  kamikaze for...
 		help w/testing version before release
		always helping people out on the forums
		contributing code for the anti-skywalking
*  ryannotfound (war3x.net) for some of the naming conventions used (function names, constants, etc...) and functions
*/

#include <amxmodx>
#include <amxmisc>
#include <dbi>
#include <fun>
#include <engine>

//VERSION NAME AUTHOR
new WC3NAME[] =		"Warcraft 3 Frozen Throne"
new WC3AUTHOR[] =	"Pimp Daddy (OoTOAoO)"
new WC3VERSION[] =	"2.1.3"
//

// THESE ARE THE ONLY OPTIONS THAT NEED TO BE CHANGED DURING COMPILE TIME
#define MOD 0							// 0 = cstrike or czero, 1 = dod
#define ADMIN_LEVEL_WC3 ADMIN_LEVEL_A	// set the admin level required for giving xp and accessing the admin menu (see amxconst.inc)
#define DEBUG 1							// Only use this when coding.. you normally don't want it
#define ADVANCED_DEBUG 0				// Prints debug information to a log file when every function is called, VERY LAGGY
#define ADVANCED_DEBUG_BOTS 0			// Print info for bots too?
#define PRECACHE_WAR3FTSOUNDS 1

#if MOD == 0
	#include <cstrike>
#endif
#if MOD == 1
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

#if MOD == 1
	#include "war3ft/dod.inl"
#endif

#if DEBUG == 1 || ADVANCED_DEBUG == 1
	#include "war3ft/debug.inl"
#endif

public plugin_init(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("plugin_init",0)
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
		//register_message(get_user_msgid("TextMsg"), "testing")

		gmsgHudText = get_user_msgid("HudText")
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
	register_clcmd("resetskills","resetSkills",-1,"resetskills")
	register_clcmd("resetxp","amx_resetxp",-1,"resetxp")
	register_clcmd("itemsinfo","MOTD_Itemsinfo",-1,"itemsinfo")
	register_clcmd("itemsinfo2","MOTD_Itemsinfo2",-1,"itemsinfo2")
	register_clcmd("war3help","MOTD_War3help",-1,"war3help")
	register_clcmd("ultimate","ultimate",-1,"ultimate")
	register_clcmd("shopmenu","menu_Shopmenu_One",-1,"shopmenu")
	register_clcmd("shopmenu2","menu_Shopmenu_Two",-1,"shopmenu2")

	register_clcmd("ability","cmd_ability",-1,"ability")
	register_clcmd("rings","cmd_Rings",-1,"rings")
	register_clcmd("fullupdate", "cmd_fullupdate")
	register_clcmd("say","cmd_Say")
	register_clcmd("say_team","cmd_Say")
	register_clcmd("jointeam","cmd_Jointeam") 
	register_clcmd("level","cmd_Level")

	register_concmd("amx_givexp","amx_givexp",-1,"amx_givexp")
	register_concmd("amx_savexp","amx_savexp",-1,"amx_savexp")
	register_concmd("amx_givemole","amx_givemole",-1,"amx_givemole")

	register_concmd("playerskills","MOTD_Playerskills",-1,"playerskills")

	register_srvcmd("amx_takexp","amx_takexp")							// For internal use only (don't use this command)
	register_srvcmd("changexp","changeXP")								// For internal use only (don't use this command)

	#if MOD == 1
		register_statsfwd(XMF_DAMAGE)
		register_statsfwd(XMF_SCORE)
		register_event("RoundState","on_EndRound","a","1=3","1=4")

		register_event("TextMsg","on_Spectate","a","2=#game_joined_team")
		register_event("TextMsg","on_SetSpecMode","b","2&#Spec_Mode")

		register_event("StatusValue","on_StatusValue","b")
	#endif

	#if MOD == 0
		register_clcmd("drop", "cmd_Drop")

		register_logevent("on_PlayerAction",3,"1=triggered") 
		register_logevent("on_FreezeTimeComplete",2,"0=World triggered","1=Round_Start")
		register_logevent("on_EndRound",2,"0=World triggered","1=Round_End")

		register_event("SendAudio", "on_TerroristWin", "a", "2=%!MRAD_terwin")
		register_event("SendAudio", "on_CTWin", "a", "2=%!MRAD_ctwin")

		register_event("ArmorType", "on_ArmorType", "be")
		register_event("WeapPickup","on_WeapPickup","b") 

		register_event("StatusValue","on_ShowStatus","be","1=2","2!0")
		//register_event("StatusValue","on_HideStatus","be","1=1","2=0")  

		register_event("TextMsg","setSpecMode","bd","2&ec_Mod")

		register_event("StatusValue","showRank","bd","1=2")

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
	#endif

	register_event("DeathMsg","on_DeathMsg","a")

	register_event("CurWeapon","on_CurWeapon","be","1=1")
	register_event("HideWeapon", "on_CurWeapon", "b")

	register_event("ResetHUD", "on_ResetHud", "b")


	register_event("TextMsg","on_GameRestart","a","2&#Game_will_restart_in")

	if(is_running("czero")){
		register_event("TextMsg", "on_GameRestart", "a", "2&#Game_Commencing")
		register_event("TextMsg", "FT_controller", "a", "2&#Game_Commencing")
	}
	else{
		register_event("TextMsg", "on_GameRestart", "a", "2&#Game_C")
		register_event("TextMsg", "FT_controller", "a", "2&#Game_C")
	}

	// For an explanation of these variables, please see war3ft.cfg
	register_cvar("FT_Race9_Skill1",			"1")
	register_cvar("FT_Race9_Skill2",			"1")
	register_cvar("FT_Race9_Skill3",			"1")
	register_cvar("FT_Race9_Ultimate",			"1")

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
	register_cvar("FT_steam",					"1")
	register_cvar("FT_CD",						"0")
	register_cvar("FT_start",					"23",FCVAR_SERVER)
	register_cvar("FT_stop",					"7",FCVAR_SERVER)
	register_cvar("FT_control",					"0")
	register_cvar("FT_message",					"0")
	register_cvar("FT_ultimatedelay",			"15.0")
	register_cvar("FT_min_b4_XP",				"2")
	register_cvar("FT_no_orcnades",				"0")
	register_cvar("FT_centerhud",				"1")
	register_cvar("FT_saveby",					"0")
	register_cvar("FT_position",				"0")
	register_cvar("FT_9raceRandom",				"1")
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
	register_cvar("sv_mysql_auto_pruning",		"0")
	register_cvar("mp_savexp",					"0",FCVAR_SERVER)
	register_cvar("mp_xpmultiplier",			"1.0")
	register_cvar("mp_weaponxpmodifier",		"1")
	register_cvar("sv_warcraft3",				"1")
	register_cvar("sv_allowwar3vote",			"0")
	register_cvar("mp_grenadeprotection",		"0")
	register_cvar("sv_mysql_save_end_round",	"0")
	register_cvar("sv_daysbeforedelete",		"31")
	register_cvar("sv_mysql",					"0")
	register_cvar("sv_mysqltablename",			"war3users")
	register_cvar("FT_mysql_host",				"127.0.0.1")
	register_cvar("FT_mysql_user",				"root")
	register_cvar("FT_mysql_pass",				"")
	register_cvar("FT_mysql_db",				"amx")

	set_task(15.0, "FT_controller",TASK_FTCONTROLLER,"",0,"b")

	WAR3_exec_config()
   
   // Dictionary files
	register_dictionary("war3FT.txt")

	#if DEBUG == 1
		register_concmd("test","test")
	#endif
}

public plugin_end(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("plugin_end",0)
	#endif

	if (!warcraft3 || !iCvar[MP_SAVEXP])
		return PLUGIN_CONTINUE
	
	XP_Save_All()

	if (iCvar[SV_MYSQL_AUTO_PRUNING] && iCvar[SV_MYSQL] && iCvar[MP_SAVEXP]){
		new mquery[1024]
		new currentHour[3]  
		new currentMin[3]      			
		
		get_time("%H",currentHour,2)
		get_time("%M",currentMin,2)
		
		// At 5:36 AM until 5:59 AM a mapchange will trigger an auto-prune.
		if((str_to_num(currentHour) == 5) && (str_to_num(currentMin) > 35 )){
				
			// Timestamp format: 20030912122142
			// Y = 2003 M = 09 D = 12 H = 12 M = 21 S = 42	
			format(mquery, 1023, "DELETE FROM `%s` WHERE `time` + %d < now()",mysqltablename, iCvar[SV_DAYSBEFOREDELETE] * 1000000)

			new Result:ret = dbi_query(mysql, mquery)

			if (ret < RESULT_NONE) {
				new err[255]
				new errNum = dbi_error(mysql, err, 254)
				server_print("[%s] DBI prune error: %s|%d", g_MOD, err, errNum)
				log_amx("[%s] DBI prune error: %s|%d", g_MOD, err, errNum)
				return 1
			} 		
		}
	}
	if (iCvar[SV_MYSQL]){
		if(mysql)
			dbi_close(mysql)
	}
	return PLUGIN_CONTINUE
}

public plugin_modules(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("plugin_modules",0)
	#endif

	require_module("engine")
	require_module("dbi")
	#if MOD == 0
		require_module("cstrike")
		require_module("csx")
		require_module("fun")
	#endif
	#if MOD == 1
		require_module("dodfun")
		require_module("dodx")
	#endif
}

public plugin_cfg(){
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("plugin_cfg",0)
	#endif

	lang_Set_Menus()
	XP_Set_DBI()
	XP_Set()
	checkmap()

	if (get_cvar_num("sv_warcraft3")==0)
		warcraft3=false
	else
		warcraft3=true

	MAXPLAYERS = get_global_int(GL_maxClients)

	new divisor

	iCvar[FT_RACE_ICONS				] =	get_cvar_num("FT_race_icons")
	iCvar[FT_LEVEL_ICONS			] =	get_cvar_num("FT_level_icons")
	iCvar[FT_SPEC_INFO				] =	get_cvar_num("FT_spec_info")
	iCvar[FT_MIN_B4_XP				] =	get_cvar_num("FT_min_b4_XP")
	iCvar[FT_BLINK_RADIUS			] =	get_cvar_num("FT_blink_radius")
	iCvar[FT_STEAM					] =	get_cvar_num("FT_steam")
	iCvar[MP_SAVEXP					] =	get_cvar_num("mp_savexp")
	iCvar[FT_SHOW_ICONS				] =	get_cvar_num("FT_show_icons")
	iCvar[FT_HEALTH_BONUS			] =	get_cvar_num("FT_health_bonus")
	iCvar[FT_NO_GLOVES_ON_KA		] = get_cvar_num("FT_no_gloves_on_ka")
	iCvar[FT_GLOVE_ORC_DAMAGE		] = get_cvar_num("FT_glove_orc_damage")
	iCvar[FT_CLAW					] = get_cvar_num("FT_claw")
	iCvar[FT_CLOAK					] = get_cvar_num("FT_cloak")
	iCvar[FT_OBJECTIVES				] = get_cvar_num("FT_objectives")
	iCvar[FT_KILL_OBJECTIVES		] = get_cvar_num("FT_kill_objectives")
	iCvar[FT_ULTIMATE_DELAY			] = get_cvar_num("FT_ultimatedelay") + 1
	iCvar[FT_ULTIMATE_COOLDOWN		] = get_cvar_num("FT_ultimate_cooldown") + 1
	iCvar[FT_RACES					] = get_cvar_num("FT_races")
	iCvar[FT_POSITION				] = get_cvar_num("FT_position")
	iCvar[FT_9RACERANDOM			] = get_cvar_num("FT_9raceRandom")
	iCvar[FT_CD						] = get_cvar_num("FT_CD")
	iCvar[FT_WARN_SUICIDE			] = get_cvar_num("FT_warn_suicide")
	iCvar[FT_BLINKENABLED			] = get_cvar_num("FT_blinkenabled")
	iCvar[SV_ALLOWWAR3VOTE			] = get_cvar_num("sv_allowwar3vote")
	iCvar[SV_MYSQL					] = get_cvar_num("sv_mysql")
	iCvar[SV_MYSQL_SAVE_END_ROUND	] = get_cvar_num("sv_mysql_save_end_round")
	iCvar[FT_SAVEBY					] = get_cvar_num("FT_saveby")
	iCvar[SV_MYSQL_AUTO_PRUNING		] = get_cvar_num("sv_mysql_auto_pruning")
	iCvar[SV_DAYSBEFOREDELETE		] = get_cvar_num("sv_daysbeforedelete")
	iCvar[FT_HEALING_RANGE			] = get_cvar_num("FT_healing_range")
	iCvar[FT_XP_RADIUS				] = get_cvar_num("FT_xp_radius")
	iCvar[FT_GLOVE_TIMER			] = get_cvar_num("FT_glove_timer")
	iCvar[FT_BUYTIME				] = get_cvar_num("FT_buytime")
	iCvar[FT_BUYZONE				] = get_cvar_num("FT_buyzone")
	iCvar[FT_BUYDEAD				] = get_cvar_num("FT_buydead")
	iCvar[FT_BLINK_DIZZINESS		] = get_cvar_num("FT_blink_dizziness")
	iCvar[MP_WEAPONXPMODIFIER		] = get_cvar_num("mp_weaponxpmodifier")
	iCvar[FT_BLINK_PROTECTION		] = get_cvar_num("FT_blink_protection")

	fCvar[FT_BOT_BUY_ITEM			] = get_cvar_float("FT_bot_buy_item")
	fCvar[FT_FROST_SPEED			] = get_cvar_float("FT_frost_speed")
	fCvar[FT_MASK_OF_DEATH			] = get_cvar_float("FT_mask_of_death")
	fCvar[MP_XPMULTIPLIER			] = get_cvar_float("mp_xpmultiplier")
	fCvar[AMX_VOTE_ANSWERS			] = get_cvar_float("amx_vote_answers")
	fCvar[AMX_VOTEWAR3_RATIO		] = get_cvar_float("amx_votewar3_ratio")
	fCvar[FT_SOCK					] = get_cvar_float("FT_sock")

	if(iCvar[MP_SAVEXP])
		divisor = 5
	else
		divisor = 1

	iCvar[FT_XPBONUS				] = get_cvar_num("FT_xpbonus") / divisor
	iCvar[FT_HEADSHOT_BONUS			] = (get_cvar_num("FT_headshot_bonus") / divisor) + 2
	iCvar[FT_ROUND_WIN_XP			] = get_cvar_num("FT_round_win_XP") / divisor

#if MOD == 0
	server_cmd("amx_statscfg off PlayerName")

	iCvar[FT_ITEMS_IN_HUD			] = get_cvar_num("FT_items_in_hud")
	iCvar[FT_NO_ORCNADES			] = get_cvar_num("FT_no_orcnades")
	iCvar[FT_BOMBPLANTERXP			] = get_cvar_num("FT_bombplanterxp") / divisor
	iCvar[FT_DEFUSEXP				] = get_cvar_num("FT_defusexp") / divisor
	iCvar[FT_HOSTAGEXP				] = get_cvar_num("FT_hostagexp") / divisor
	iCvar[FT_KILLRESCUEMANXP		] = get_cvar_num("FT_killrescuemanxp") / divisor
	iCvar[FT_KILL_BOMB_CARRIER_BONUS] = get_cvar_num("FT_kill_bomb_carrier_bonus") / divisor
	iCvar[FT_DEFUSER_KILL_BONUS		] = get_cvar_num("FT_defuser_kill_bonus") / divisor
	iCvar[FT_VIP_ESCAPE_BONUS		] = get_cvar_num("FT_VIP_escape_bonus") / divisor
	iCvar[FT_VIP_KILL_BONUS			] = get_cvar_num("FT_VIP_kill_bonus") / divisor
	iCvar[MP_GRENADEPROTECTION		] = get_cvar_num("mp_grenadeprotection")

	fCvar[FT_BOOTSPEED				] = get_cvar_float("FT_bootspeed")
#endif

#if MOD == 1
	iCvar[DOD_STARTMONEY			] = get_cvar_num("DOD_startmoney")
	
	fCvar[DOD_BOOTSPEED				] = get_cvar_float("DOD_bootspeed")
#endif

	race9Options[1] = get_cvar_num("FT_Race9_Skill1")
	race9Options[2] = get_cvar_num("FT_Race9_Skill2")
	race9Options[3] = get_cvar_num("FT_Race9_Skill3")
	race9Options[4] = get_cvar_num("FT_Race9_Ultimate")

	if(iCvar[FT_RACES] > MAX_RACES)
		iCvar[FT_RACES] = MAX_RACES

	// Set the version of war3 that is running (war3ft or classic war3?)
	if(iCvar[FT_RACES] < 5){
		g_MOD = "WAR3"
		g_MODclient = "* [WAR3]"
	}
	else{
		g_MOD = "WAR3FT"
		g_MODclient = "* [WAR3FT]"	
	}
	return PLUGIN_CONTINUE
}

public plugin_precache() {
	#if ADVANCED_DEBUG == 1
		writeDebugInfo("plugin_precache",0)
	#endif

	WAR3_precache()

	return PLUGIN_CONTINUE
}

public client_putinserver(id){
	#if ADVANCED_DEBUG == 1
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

}

public client_connect(id){
	#if ADVANCED_DEBUG == 1
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
#if MOD == 1
	reincarnation[id][ZPOS] = -99999
#endif

	// If a player has a helm, the new player shouldn't be able to hit him in the head
	for (new j = 1; j <= MAXPLAYERS; j++){
		if(p_data_b[j][PB_ISCONNECTED]){
			if(p_data[P_ITEM2][j] == ITEM_HELM)
				set_user_hitzones(id, j, 253)
		}
	}

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
	#if ADVANCED_DEBUG == 1
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
		if( Verify_Skill(id, RACE_UNDEAD, SKILL2) ){
			// They have a rocket launcher "deployed" or are using their stamina
			new prone = entity_get_int(id,EV_INT_iuser3)
			new Float:maxspeed = entity_get_float(id,EV_FL_maxspeed)
			if((maxspeed==50.0 && !prone) || entity_get_float(id,EV_FL_fuser4)<100.0 || (maxspeed>500.0 && prone)){		
				new parm[1]
				parm[0] = id
				unholyspeed(parm)
			}
			if(entity_get_float(id,EV_FL_fuser4) < p_unholy[p_data[id][P_SKILL2]-1]){
				entity_set_float(id,EV_FL_fuser4, p_unholy[p_data[id][P_SKILL2]-1])
			}
		}
		if(p_data[id][P_ITEM] == ITEM_BOOTS && entity_get_float(id,EV_FL_fuser4) < fCvar[DOD_BOOTSPEED]){
			entity_set_float(id,EV_FL_fuser4, fCvar[DOD_BOOTSPEED])
		}
	#endif
	}
}