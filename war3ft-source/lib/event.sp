public Action:Event_player_spawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event,"userid"));

	if(client && IsClientAlive(client) && GetClientTeam(client)!=0)
	{

		// Player needs to choose a race!
		if(g_PlayerData[client][PLAYER_RACE] == 0 || g_PlayerData[client][PLAYER_CHANGERACE])
		{
			DisplayMenu(g_MenuChangeRace, client, MENU_TIME_FOREVER);

			// Reset so we don't try to change the user's race next round!
			g_PlayerData[client][PLAYER_CHANGERACE] = false;
		}
	}

	return Plugin_Continue;
}

/* short 	 userid
short 	attacker
string 	weapon
bool 	headshot*/
public Action:Event_player_death(Handle:event, const String:name[], bool:dontBroadcast)
{

	if (!IsGameRunning())
	{
		return;
	}


	new victim = GetClientOfUserId(GetEventInt(event,"userid"));
	new attacker = GetClientOfUserId(GetEventInt(event,"attacker"));

	// Award experience to the attacker
	new victim_level = g_PlayerData[victim][PLAYER_LEVEL];
	new xp = g_XPAwardedByLevel[victim_level];
	XP_Award( attacker, xp );

	PrintToChat( attacker, "[WC3] You have been awarded %d XP!", xp );
}

/*public Action:Event_OnTakeDamage(client, &inflictor, &attacker, &Float:Damage, &DamageType, &AmmoType)
{
	new returnVal = 0;

	PrintToServer("client: %d, Inflictor: %d, Attacker: %d, Damage: %0.0f", client, inflictor, attacker, Damage);

	if ( inflictor > 0 )
	{
		returnVal += UD_OnTakeDamage(client, inflictor, attacker, Damage, DamageType, AmmoType);
	}
	//Damage = 0.0;
	
	if ( returnVal > 0 )
	{
		return Plugin_Handled;
	}

	PrintToServer("victim: %d, inflictor: %d, Attacker: %d, Damage: %d", client, inflictor, attacker, Damage);

	return Plugin_Continue;
}*/

/*
short 	 userid
short 	attacker
byte 	health
byte 	armor
string 	weapon
byte 	dmg_health
byte 	dmg_armor
byte 	hitgroup
*/
public Action:Event_player_hurt(Handle:event, const String:name[], bool:dontBroadcast)
{
	new victim = GetClientOfUserId(GetEventInt(event,"userid"));
	new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	new dmg_health = GetEventInt(event, "dmg_health");

	//PrintToServer("victim: %d, Attacker: %d, Damage: %d", victim, attacker, dmg_health);

	UD_OnTakeDamage(victim, attacker, dmg_health);


	/*new client = GetClientOfUserId(GetEventInt(event,"userid"));

	new health = GetEventInt(event, "health");
	new armor = GetEventInt(event, "armor");
	new dmg_health = GetEventInt(event, "dmg_health");
	new dmg_armor = GetEventInt(event, "dmg_armor");
	new attacker = GetEventInt(event, "attacker");

	new String:weapon[32];
	GetEventString(event, weapon, "weapon", 31);

	PrintToChat(client, "health: %d", health);
	PrintToChat(client, "armor: %d", armor);
	PrintToChat(client, "dmg_health: %d", dmg_health);
	PrintToChat(client, "dmg_armor: %d", dmg_armor);
	PrintToChat(client, "attacker: %d", attacker);
	PrintToChat(client, "weapon: %s", weapon);*/
}

public Event_round_end(Handle:event,const String:name[],bool:dontBroadcast)
{
	g_RoundState = RoundState:RoundState_End;
}

public Event_round_freeze_end(Handle:event,const String:name[],bool:dontBroadcast)
{
	g_RoundState = RoundState:RoundState_InProgress;
}

public Event_round_start(Handle:event,const String:name[],bool:dontBroadcast)
{
	g_RoundState = RoundState:RoundState_InFreezeTime;
}

public Event_bomb_beginplant(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event,"userid"));
	g_PlayerStatus[client][STATUS_PLANTING] = true;
}

public Event_bomb_abortplant(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event,"userid"));
	g_PlayerStatus[client][STATUS_PLANTING] = false;
}

public Event_bomb_planted(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event,"userid"));
	// Award xp
}

public Event_bomb_defused(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event,"userid"));
	// Award xp
}

public Event_bomb_exploded(Handle:event, const String:name[], bool:dontBroadcast)
{
	// T's won, give them xp
}

public Event_bomb_dropped(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event,"userid"));
	// dunno what happens =(
}

public Event_bomb_pickup(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event,"userid"));
	// give xp
}



