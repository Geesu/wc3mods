#include <sourcemod>
//#include <hooker>

#pragma semicolon	1
#pragma tabsize		0

// Header files
#include "lib/constants.h"

// War3FT includes
#include "lib/common.sp"
#include "lib/XP.sp"
#include "lib/player.sp"
#include "lib/event.sp"
#include "lib/menu.sp"
#include "lib/lang.sp"
#include "lib/helper.sp"
#include "lib/race_undead.sp"

public Plugin:myinfo =
{
	name = "Warcraft 3: Frozen Throne",
	author = "Geesu & Avanderik",
	description = "Warcraft 3: Frozen Throne",
	version = "0.0.0.1",
	url = "http://source.wc3mods.net/"
};
 
public OnPluginStart()
{
	LoadTranslations("war3ft.phrases");

	RegConsoleCmd("say", Command_Say);
	RegConsoleCmd("say_team", Command_Say);
	RegConsoleCmd("test", Command_Test);


	// Event Hooks
	HookEvent("player_spawn", Event_player_spawn, EventHookMode_Pre);
	HookEvent("player_death", Event_player_death, EventHookMode_Pre);
	HookEvent("player_hurt", Event_player_hurt);
	HookEvent("round_end", Event_round_end);
	HookEvent("round_start", Event_round_start);
	HookEvent("round_freeze_end", Event_round_freeze_end);

	HookEvent("bomb_beginplant", Event_bomb_beginplant);
	HookEvent("bomb_abortplant", Event_bomb_abortplant);
	HookEvent("bomb_planted", Event_bomb_planted);
	HookEvent("bomb_defused", Event_bomb_defused);
	HookEvent("bomb_exploded", Event_bomb_exploded);
	HookEvent("bomb_dropped", Event_bomb_dropped);
	HookEvent("bomb_pickup", Event_bomb_pickup);
	HookEvent("bomb_begindefuse", Event_bomb_begindefuse);
	HookEvent("bomb_abortdefuse", Event_bomb_abortdefuse);
	HookEvent("hostage_follows", Event_hostage_follows);
	HookEvent("hostage_hurt", Event_hostage_hurt);
	HookEvent("hostage_killed", Event_hostage_killed);
	HookEvent("hostage_rescued", Event_hostage_rescued);
	HookEvent("hostage_rescued_all", Event_hostage_rescued_all);
	HookEvent("hostage_stops_following", Event_hostage_stops_following);
	HookEvent("hostage_call_for_help", Event_hostage_call_for_help);
	HookEvent("vip_escaped", Event_vip_escaped);
	HookEvent("vip_killed", Event_vip_killed);

	FindOffsets();

//	RegisterHook(HK_OnTakeDamage, Event_OnTakeDamage, false);
}

public OnClientPutInServer(client)
{
    //HookEntity(HKE_CCSPlayer, client);

	// Initialize the player
	g_PlayerXP[client] = 0;
	g_PlayerCurrentRace[client] = 0;
	g_bChangeRacePending[client] = false;
}

public OnClientDisconnect(client)
{
//    UnHookPlayer(HKE_CCSPlayer, client);
}

public OnConfigsExecuted()
{
	BuildMenuChangeRace();
}


public Action:Command_Test(client, args)
{
	if (!client)
	{
		return Plugin_Continue;
	}


		new Handle:handle = StartMessageOne("Fade", client);

		if (handle != INVALID_HANDLE)
		{	
			//RoundToZero(0.3 * (1<<SCREENFADE_FRACBITS))
			BfWriteShort(handle, RoundToZero(0.3 * (1<<SCREENFADE_FRACBITS)));
			BfWriteShort(handle, RoundToZero(0.7 * (1<<SCREENFADE_FRACBITS)));
			BfWriteShort(handle, FFADE_IN|FFADE_PURGE);

			BfWriteByte(handle, 0);
			BfWriteByte(handle, 255);
			BfWriteByte(handle, 0);
			BfWriteByte(handle, 50);
			
			EndMessage();
		}

	/*new health = GetClientHealth(client);
	health+=15;
	SetClientHealth(client, health);*/

	PrintToChat(client,"Lifestate: %d, Team: %d, XP: %d, Race: %d", GetEntData(client,OffSet_LifeState,1), GetClientTeam(client), g_PlayerXP[client], g_PlayerCurrentRace[client]);



	return Plugin_Handled;	
}


public Action:Command_Say(client, args)
{
	if (!client)
	{
		return Plugin_Continue;
	}

	decl String:text[192];
	if (!GetCmdArgString(text, sizeof(text)))
	{
		return Plugin_Continue;
	}
	
	new startidx = 0;
	if(text[strlen(text)-1] == '"')
	{
		text[strlen(text)-1] = '\0';
		startidx = 1;
	}
	
	if ( strcmp(text[startidx], "changerace", false) == 0 )
	{
		// Player has no race! Display menu!
		if ( g_PlayerCurrentRace[client] == 0 )
		{
			DisplayMenu(g_MenuChangeRace, client, MENU_TIME_FOREVER);
		}
		else
		{
			// Lets wait to display the changerace menu until the next round
			g_bChangeRacePending[client] = true;
		}
	}
	
	return Plugin_Continue;	
}


FindOffsets()
{

  OffSet_LifeState=FindSendPropOffs("CAI_BaseNPC","m_lifeState");
  if(OffSet_LifeState==-1)
    SetFailState("[WC3] Error: Failed to find the LifeState offset, aborting");
}


/*
Random Notes:
	Team ID: 
		1 = Spectator
		2 = Terrorist
		3 = CT

	Lifestate
		0 = Alive
		2 = Dead
*/


