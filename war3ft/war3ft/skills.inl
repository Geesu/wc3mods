

// Gives skill abilities at beginning of round and when skills are selected
public Skill_Check(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("Skill_Check",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if( p_data_b[id][PB_RESETSKILLS] ){
		p_data_b[id][PB_RESETSKILLS] = false
		
		return PLUGIN_CONTINUE
	}

	new parm[2]
	parm[0]=id

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE
	
	// Undead's Unholy Aura
	Skill_UnholyAura(id);

	// Crypt Lord's Carrion Beetles
	if ( Verify_Skill(id, RACE_CRYPT, SKILL3) ){
		p_data[id][P_CARRIONCOUNT]=2
	}
	
	// Warden's Shadow Strike
	if ( Verify_Skill(id, RACE_WARDEN, SKILL3) ){
		p_data[id][P_SHADOWCOUNT]=2
	}

	// Night Elf's Evasion
	Skill_Evasion_Set( id );

	// Human's Devotion Aura
	if ( Verify_Skill(id, RACE_HUMAN, SKILL2) ){
		set_user_health(id,p_devotion[p_data[id][P_SKILL2]-1])
	}

	// God mode, removing the reset task will prevent death (health is reset to 100 at the start of the round, so we don't want to set the health to 100-2048)
	if ( task_exists( TASK_RESETGOD+id ) && p_data_b[id][PB_GODMODE] ){
		remove_task( TASK_RESETGOD+id )
		p_data_b[id][PB_GODMODE] = false
	}

	// Shadow Hunter's Healing Wave
	if ( Verify_Skill(id, RACE_SHADOW, SKILL1) ){
		if (!task_exists(id+TASK_WAVE))
			set_task(1.0,"_Skill_Healing_Wave",TASK_WAVE+id,parm,2)
	}
	else{
		if (task_exists(id+TASK_WAVE))
			remove_task(id+TASK_WAVE)
	}

	// Shadow Hunter's Serpent Ward
	if ( Verify_Skill(id, RACE_SHADOW, SKILL3) )
		p_data[id][P_SERPENTCOUNT]=p_serpent[p_data[id][P_SKILL3]-1]
	else
		p_data[id][P_SERPENTCOUNT]=0

	Skills_Blink(id)

#if MOD == 0
	// Blood Mage's Pheonix
	if ( Verify_Skill(id, RACE_BLOOD, SKILL1) ){			
		new Float:randomnumber = random_float(0.0,1.0)
		new teamnumber = get_user_team(id)
		if (randomnumber <= p_pheonix[p_data[id][P_SKILL1]-1]){
			p_data_b[id][PB_PHOENIXCASTER] = true
			PhoenixFound[teamnumber-1]++
		}
	}
#endif

	Ultimate_Icon(id,ICON_SHOW)

	return PLUGIN_CONTINUE
}

// ****************************************
// Undead's Unholy Aura
// ****************************************
public Skill_UnholyAura( id )
{
	if( get_cvar_num("sv_gravity") > 650 )
	{
		// Levitation
		if ( Verify_Skill(id, RACE_UNDEAD, P_SKILL3) )
		{		
			if ( get_user_gravity(id) != p_levitation[p_data[id][P_SKILL3]-1] )
			{
				set_user_gravity(id, p_levitation[p_data[id][P_SKILL3]-1]);
			}
		}
		else if ( p_data[id][P_ITEM2] == ITEM_SOCK )
		{
			set_user_gravity(id, fCvar[FT_SOCK]);
		}
		else if ( get_user_gravity(id) != 1.0 )
		{
			set_user_gravity(id, 1.0);
		}

	}
	else
	{
		set_user_gravity(id, 1.0);
	}
}

// ****************************************
// Human's Invisibility
// ****************************************

public Skill_Invisibility(id)
{
	if ( p_data_b[id][PB_RENDER] && !p_data_b[id][PB_HEXED])
	{
		new clipamount = 0, ammoamount = 0, weaponnum = 0
		weaponnum = get_user_weapon(id,clipamount,ammoamount)

		if ( Verify_Skill(id, RACE_HUMAN, SKILL1) ){
		#if MOD == 0
			if (weaponnum==CSW_KNIFE){
		#endif
		#if MOD == 1
			if (weaponnum==DODW_AMERKNIFE || weaponnum==DODW_GERKNIFE || weaponnum==DODW_SPADE){
		#endif
				set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,p_invisibility[p_data[id][P_SKILL1]-1]/2)
			}
			else{
				set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,p_invisibility[p_data[id][P_SKILL1]-1])
			}
		}
		else if ( p_data[id][P_ITEM]==ITEM_CLOAK ){
		#if MOD == 0
			if (weaponnum==CSW_KNIFE){
		#endif
		#if MOD == 1
			if (weaponnum==DODW_AMERKNIFE || weaponnum==DODW_GERKNIFE || weaponnum==DODW_SPADE){
		#endif
				set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,iCvar[FT_CLOAK]/2)
			}
			else{
				set_user_rendering(id,kRenderFxNone, 0,0,0, kRenderTransTexture,iCvar[FT_CLOAK])
			}
		}
		else
			set_user_rendering(id)
	}
}

#if MOD == 1
// ****************************************
// Orc's Reincarnation ability in Day of Defeat
// ****************************************

	public SKILL_Reincarnation(id){
		#if ADVANCED_DEBUG
			writeDebugInfo("SKILL_Reincarnation",id)
		#endif

		if (!warcraft3)
			return PLUGIN_CONTINUE
		
		// User just joined or should skip reincarnation
		if ( p_data_b[id][PB_REINCARNATION_SKIP] )
		{
			p_data_b[id][PB_REINCARNATION_SKIP] = false;
			return PLUGIN_CONTINUE;
		}

		
		if(p_data_b[id][PB_REINCARNATION_DELAY])
		{
			client_print(id,print_chat,"%s %L", g_MODclient, id, "SKILL_REINCARNATION_SKIPPING")
			p_data_b[id][PB_REINCARNATION_DELAY] = false
		}
		else
		{
			new parm[2]
			parm[0] = id

			client_cmd(id, "speak warcraft3/soundpack/reincarnation.wav")

			if (iglow[id][1] < 1){
				parm[0] = id
				set_task(0.1,"glow_change",TASK_GLOW+id,parm,2)
			} 
			iglow[id][1] += 100
			iglow[id][0] = 0
			iglow[id][2] = 0
			iglow[id][3] = 0
			if (iglow[id][1]>MAXGLOW)
				iglow[id][1]=MAXGLOW

			// Screen fade green
			Create_ScreenFade(id, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[id][1])

			iReincarnation[id][ZPOS]+=30

			set_user_origin(id,iReincarnation[id])

			set_task(0.1,"_SKILL_Reincarnation", TASK_REINCARNATION+id,parm,1)
			set_task(2.5,"_SKILL_Reincarnation_Status", TASK_REINCCHECK+id,parm,1)
		}

		return PLUGIN_CONTINUE
	}

	public _SKILL_Reincarnation(parm[]){
		#if ADVANCED_DEBUG
			writeDebugInfo("_SKILL_Reincarnation",parm[0])
		#endif
		
		if (!warcraft3)
			return PLUGIN_CONTINUE

		new id = parm[0]
		new origin[3]
		get_user_origin(id,origin)

		// Failure, stuck somewhere, put them back
		if(origin[2] == iReincarnation[id][2]){

			new spawnID, playersInVicinity, entList[1], i
			new ent = -1
			new Float:spawnOrigin[3]
			new Float:vicinity = 96.0		//(32x32x96)
			new bool:found = false
			if(get_user_team(id) == CTS)
				spawnID = 0
			else
				spawnID = 1

			do {
				ent = find_ent_by_class(ent,spawnEntString[spawnID])
				if (ent != 0) {
					entity_get_vector(ent,EV_VEC_origin,spawnOrigin)
					for(i=0;i<3;i++)
						origin[i] = floatround(spawnOrigin[i])

					playersInVicinity = find_sphere_class(0, "player", vicinity, entList, 1, spawnOrigin)

					if (playersInVicinity == 0)
						found = true
				}
			}
			while (ent && !found)
			
			if(found){
				for(i=0;i<3;i++)
					origin[i] = floatround(spawnOrigin[i])

				set_user_origin(id, origin)

				client_print(id,print_chat,"%s %L", g_MODclient, id, "SKILL_REINCARNATION_FAILED")
			}
			else
				set_task(0.1,"_SKILL_Reincarnation", TASK_REINCARNATION+id,parm,1)

		}

		return PLUGIN_CONTINUE
	}

	public _SKILL_Reincarnation_Status(parm[]){
		#if ADVANCED_DEBUG
			writeDebugInfo("_SKILL_Reincarnation_Status",parm[0])
		#endif

		if (!warcraft3)
			return PLUGIN_CONTINUE

		new id = parm[0]

		if(!is_user_alive(id))
			p_data_b[id][PB_REINCARNATION_DELAY] = true
		else
			p_data_b[id][PB_REINCARNATION_DELAY] = false

		return PLUGIN_CONTINUE
	}

	// ****************************************
	// Blood Mage's Pheonix Ability in DOD
	// ****************************************

	public Skill_Pheonix(id){
		set_user_money(id, get_user_money(id) + p_pheonix[p_data[id][P_SKILL1]-1])

		new name[32], team
		get_user_name(id, name, 31)
		team = get_user_team(id)

		new players[32], numberofplayers
		new i, targetid, distancebetween, targetorigin[3], origin[3]

		get_user_origin(id, origin)
		get_players(players, numberofplayers,"a")

		new money = p_pheonix[p_data[id][P_SKILL1]-1] / 2
		for (i = 0; i < numberofplayers; ++i){
			targetid=players[i]

			if ( targetid != id && p_data_b[targetid][PB_ISCONNECTED] && get_user_team(targetid) == team){
				get_user_origin(targetid, targetorigin)
				distancebetween = get_distance(origin, targetorigin)

				if (distancebetween < iCvar[FT_HEALING_RANGE]){
					set_user_money(targetid, get_user_money(targetid) + money)
					client_print(targetid, print_chat, "%s %L", g_MODclient, targetid, "DOD_PHOENIX", money, name)
				}
			}
		}
	}
#endif
#if MOD == 0

// ****************************************
// Orc's Reincarnation Ability in CS/CZ
// ****************************************

public Skill_Reincarnation( parm[2] )
{
	#if ADVANCED_DEBUG
		writeDebugInfo("Skill_Reincarnation",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	#if MOD == 0
		new id = parm[0]

		if(!p_data_b[id][PB_ISCONNECTED])
			return PLUGIN_CONTINUE

		new bool:reincarnate = false
		
		// Give items because of respawning...
		if(p_data_b[id][PB_GIVEITEMS]){
			reincarnate = true
			p_data_b[id][PB_GIVEITEMS]=false
		}
									
		// Equipement & Eligibility Check for Re-Incarnation
		if (p_data_b[id][PB_DIEDLASTROUND]){	// DIED LAST ROUND		
			new Float:randomnumber = random_float(0.0,1.0)   
			if ( Verify_Skill(id, RACE_ORC, SKILL3) ){
				if( randomnumber <= p_ankh[p_data[id][P_SKILL3]-1] ){
					reincarnate = true				
				}
			}
			if (p_data[id][P_ITEM]==ITEM_ANKH){
				reincarnate=true
			}	
		}

		if (reincarnate){
				client_cmd(id, "speak warcraft3/soundpack/reincarnation.wav")

				if (iglow[id][1] < 1){
					parm[0] = id
					set_task(0.1,"glow_change",TASK_GLOW+id,parm,2)
				} 
				iglow[id][1] += 100
				iglow[id][0] = 0
				iglow[id][2] = 0
				iglow[id][3] = 0
				if (iglow[id][1]>MAXGLOW)
					iglow[id][1]=MAXGLOW

				// Screen fade green
				Create_ScreenFade(id, (1<<10), (1<<10), (1<<12), 0, 255, 0, iglow[id][1])

				_Skill_Reincarnation_Drop(id)
				_Skill_Reincarnation_Give(id)
		}else{
			if(!cs_get_user_nvg(id))
				p_data_b[id][PB_NIGHTVISION]=false

		}
	#endif

	Item_Check(parm)

	return PLUGIN_HANDLED	
}	

public _Skill_Reincarnation_Drop(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Skill_Reincarnation_Drop",0)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new origin[3]
	new iweapons[32] = 0, wpname[32] = 0, inum = 0
	get_user_weapons(id,iweapons,inum)

	// Disarm and Isolate Player Before Re-Incarnation
	get_user_origin(id,origin)
	origin[2] -= 2000
	set_user_origin(id,origin)
	for(new a=0;a<inum;++a){
		if(iweapons[a] != CSW_C4 && iweapons[a] != CSW_KNIFE){
			get_weaponname(iweapons[a],wpname,31)
			engclient_cmd(id,"drop",wpname)
		}
	}
	client_cmd(id,"weapon_knife")

	return PLUGIN_CONTINUE
}

public _Skill_Reincarnation_Give(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Skill_Reincarnation_Give",0)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new origin[3]
	new weaponname[20] = 0, ammoname[20] = 0

	get_user_origin(id,origin)
	origin[2] += 2005
	
	// Give armor
	if ( p_data[id][P_ARMORONDEATH] )
	{
		// Not sure why I need to do this (shouldn't the cs_set_user_armor function do it dangit) but I do :/
		if ( g_ArmorType[id] == CS_ARMOR_KEVLAR )
		{
			give_item(id, "item_kevlar");
		}
		else if ( g_ArmorType[id] == CS_ARMOR_VESTHELM )
		{
			give_item(id, "item_assaultsuit");
		}

		cs_set_user_armor( id, p_data[id][P_ARMORONDEATH], g_ArmorType[id] );
	}
	
	// Give a defuse kit
	if ( p_data_b[id][PB_DEFUSE] )
	{
		give_item(id, "item_thighpack");
	}

	if ( p_data_b[id][PB_NIGHTVISION] )
	{
		cs_set_user_nvg(id, 1);
	}

	if( p_data_b[id][PB_SHIELD] )
	{
		give_item(id, "weapon_shield");
	}

	new weaponid = 0, j
	for (j=0; (j < p_data[id][P_SAVEDNUMBER]) && (j < 32); ++j){
		weaponid=savedweapons[id][j]

		if(weaponid==CSW_USP){
			weaponname="weapon_usp"
			ammoname="ammo_45acp"
		}
		else if(weaponid==CSW_ELITE){
			weaponname="weapon_elite"
			ammoname="ammo_9mm"
		}
		else if(weaponid==CSW_FIVESEVEN){
			weaponname="weapon_fiveseven"
			ammoname="ammo_57mm"
		}
		else if(weaponid==CSW_GLOCK18){
			weaponname="weapon_glock18"
			ammoname="ammo_9mm"
		}
		else if(weaponid==CSW_DEAGLE){
			weaponname="weapon_deagle"
			ammoname="ammo_50ae"
		}
		else if(weaponid==CSW_P228){
			weaponname="weapon_p228"
			ammoname="ammo_357sig"
		}
		else if (weaponid==3){
			weaponname="weapon_scout"
			ammoname="ammo_762nato"
		}
		else if (weaponid==4){
			give_item(id,"weapon_hegrenade")
		}
		else if (weaponid==5){
			weaponname="weapon_xm1014"
			ammoname="ammo_buckshot"
		}
		else if (weaponid==7){
			weaponname="weapon_mac10"
			ammoname="ammo_45acp"
		}
		else if (weaponid==8){
			weaponname="weapon_aug"
			ammoname="ammo_556nato"
		}
		else if (weaponid==9){
			give_item(id,"weapon_smokegrenade")	
		}
		else if (weaponid==12){
			weaponname="weapon_ump45"
			ammoname="ammo_45acp"
		}
		else if (weaponid==13){
			weaponname="weapon_sg550"
			ammoname="ammo_556nato"
		}
		else if (weaponid==14){
			weaponname="weapon_galil"
			ammoname="ammo_556nato"
		}
		else if (weaponid==15){
			weaponname="weapon_famas"
			ammoname="ammo_556nato"
		}
		else if (weaponid==18){
			weaponname="weapon_awp"
			ammoname="ammo_338magnum"
		}
		else if (weaponid==19){
			weaponname="weapon_mp5navy"
			ammoname="ammo_9mm"				
		}
		else if (weaponid==20){
			weaponname="weapon_m249"
			ammoname="ammo_556natobox"
		}
		else if (weaponid==21){
			weaponname="weapon_m3"
			ammoname="ammo_buckshot"
		}
		else if (weaponid==22){
			weaponname="weapon_m4a1"
			ammoname="ammo_556nato"
		}
		else if (weaponid==23){
			weaponname="weapon_tmp"
			ammoname="ammo_9mm"
		}
		else if (weaponid==24){
			weaponname="weapon_g3sg1"
			ammoname="ammo_762nato"
		}
		else if (weaponid==25){
			if(p_data[id][P_FLASHCOUNT]==1)
				give_item(id,"weapon_flashbang")
			else{
				give_item(id,"weapon_flashbang")
				give_item(id,"weapon_flashbang")
			}
		}
		else if (weaponid==27){
			weaponname="weapon_sg552"
			ammoname="ammo_556nato"
		}
		else if (weaponid==28){
			weaponname="weapon_ak47"
			ammoname="ammo_762nato"
		}
		else if (weaponid==30){
			weaponname="weapon_p90"
			ammoname="ammo_57mm"
		}
		else{
			weaponname=""
			ammoname=""
		}
		if (contain(weaponname,"weapon_")==0){	//  if no match found, 0 if match
			give_item(id,weaponname)
			give_item(id,ammoname)
			give_item(id,ammoname)
			give_item(id,ammoname)
			give_item(id,ammoname)
			give_item(id,ammoname)
		}
	}

	// Restore Re-Incarnated Player to the map
	set_user_origin(id,origin)

	// This will make the player have the new reincarnated weapon selected
	// instead of the player's knife being selected
	new iweapons[32] = 0, wpname[32] = 0, inum = 0
	new bool:foundPrim = false
	//new bool:foundSec = false
	get_user_weapons(id,iweapons,inum)

	for(new a=0;a<inum;++a){
		if(isPrimary(iweapons[a])){
			get_weaponname(iweapons[a],wpname,31)
			client_cmd(id,wpname)
			foundPrim = true
			break
		}
	}

	if(!foundPrim){
		for(new a=0;a<inum;++a){
			if(isSecondary(iweapons[a])){
				get_weaponname(iweapons[a],wpname,31)
				client_cmd(id,wpname)
				//foundSec = true
				break
			}
		}
	}	

	return PLUGIN_CONTINUE
}
#endif

// ****************************************
// Night Elf's Evasion
// ****************************************

// Called when a user is shot and when a user spawns
public Skill_Evasion_Set( id )
{
	// Only check to see if the user can evade if they have the skill
	if ( Verify_Skill( id, RACE_ELF, SKILL1 ) )
	{
		new Float:randomnumber = random_float(0.0, 1.0);
		
		// Only set next shot as being evaded if if they aren't already evading the next shot
		if ( randomnumber <= p_evasion[p_data[id][P_SKILL1]-1] && !p_data_b[id][PB_EVADENEXTSHOT] )
		{
			p_data_b[id][PB_EVADENEXTSHOT] = true;

		#if DEBUG
			client_print(id, print_chat, "%s You will evade the next shot", g_MODclient);
		#endif
		}
	}
	
	// Actually do the health changes in this function
	Skill_Evasion_Check( id );
}

// Called on Skill_Evasion_Set and when a user types /level
public Skill_Evasion_Check( id )
{
	new iHealth = get_user_health( id );
	
	// Make sure the user has the skill
	if ( Verify_Skill( id, RACE_ELF, SKILL1 ) )
	{
		// Give the user enough health so they can evade
		if ( p_data_b[id][PB_EVADENEXTSHOT] && iHealth < 500 && is_user_alive(id) )
		{
			set_user_health( id, iHealth + SKILL_EVASION_ADJ );

		#if DEBUG
			client_print(id, print_chat, "%s 1Health adjusted for evasion from %d to %d", g_MODclient, iHealth, (SKILL_EVASION_ADJ + iHealth));
		#endif
		}
	}
	// User doesn't have the skill
	else
	{
		// This shouldn't be true if the user doesn't have the skill right?
		if ( p_data_b[id][PB_EVADENEXTSHOT] )
		{
			p_data_b[id][PB_EVADENEXTSHOT] = false;

		#if DEBUG
			client_print(id, print_chat, "%s Evade next shot disabled", g_MODclient);
		#endif
		}
	}

	// Check if the user has too much health when they shouldn't evade the next shot
	if ( !p_data_b[id][PB_EVADENEXTSHOT] && iHealth > 500 && !p_data_b[id][PB_GODMODE] )
	{
	#if DEBUG
		client_print(id, print_chat, "%s Health adjusted for evasion from %d to %d", g_MODclient, iHealth, (SKILL_EVASION_ADJ - iHealth));
	#endif

		// Hopefully this will never kill them
		set_user_health(id, iHealth - SKILL_EVASION_ADJ);
	}

	// This should technically never occur ...
	if ( p_data_b[id][PB_EVADENEXTSHOT] && iHealth > 1500 && !p_data_b[id][PB_GODMODE] )
	{
		// Just set back to default
		set_user_health( id, 100 + SKILL_EVASION_ADJ );

	#if DEBUG
		client_print(id, print_chat, "%s Health of %d adjusted from godmode, this should not occur", g_MODclient, iHealth);
	#endif
	}
}

stock Skill_Evasion_Reset( id, damage )
{
	if ( !Verify_Skill( id, RACE_ELF, SKILL1 ) )
	{
		return;
	}

	new iHealth = get_user_health( id );
	
	if ( p_data_b[id][PB_EVADENEXTSHOT] )
	{
		set_user_health(id, iHealth + damage + (-1 * SKILL_EVASION_ADJ));

		if (iglow[id][2] < 1)
		{
			new parm[2];
			parm[0] = id;
			set_task(0.01, "glow_change", TASK_GLOW + id, parm, 2);
		}
		iglow[id][2] += damage;
		iglow[id][0] = 0;
		iglow[id][1] = 0;
		iglow[id][3] = 0;
		if (iglow[id][2]>MAXGLOW)
		{
			iglow[id][2]=MAXGLOW;
		}

		Create_ScreenFade(id, (1<<10), (1<<10), (1<<12), 0, 0, 255, iglow[id][2]);

		p_data_b[id][PB_EVADENEXTSHOT] = false;

	#if DEBUG
		client_print(id, print_chat, "%s shot evaded, health set to %d", g_MODclient, get_user_health(id));
	#endif
	}

	return;
}

#if MOD == 0
	// ****************************************
	// Blood Mage's Phoenix in cstrike/czero
	// ****************************************

	public Skill_Phoenix(id)
	{
		new vTeam = get_user_team(id)
		
		/* Make sure that we have a CT/T player that died */
		if ( vTeam == CTS || vTeam == TS )
		{
			/* Verify that a player on the victim's team has phoenix */
			if ( PhoenixFound[vTeam-1] > 0 )
			{
				new players[32], numberofplayers, i, targetid
				get_players(players, numberofplayers, "a")

				/* Loop through all the players */
				for (i = 0; i < numberofplayers; ++i)
				{
					targetid = players[i]
					/* Verify that the players are on the same team and that a caster is found */
					if ( get_user_team(targetid) == vTeam && p_data_b[targetid][PB_PHOENIXCASTER] && !p_data_b[id][PB_TOBEREVIVED] && !endround && id!=targetid && !p_data_b[id][PB_SPAWNEDFROMITEM] )
					{
						new parm[2], name[32], victimName[32], message[128]
						parm[0] = id

						p_data_b[id][PB_SPAWNEDFROMITEM] = true

						set_task(0.7,"func_spawn",TASK_SPAWN+id,parm,2)
						set_hudmessage(200, 100, 0, -0.8, 0.1, 0, 1.0, 5.0, 0.1, 0.2, 2) 
						get_user_name(targetid,name,31)
						get_user_name(id,victimName,31)

						format(message, 127, "%L",id,"HAS_REVIVED_YOU",name)
						Status_Text(id, message, 3.0, HUDMESSAGE_POS_INFO)
						client_print(id, print_chat, "%s %s", g_MODclient, message)
						format(message, 127, "%L",targetid,"YOU_HAVE_REVIVED",victimName)
						Status_Text(targetid, message, 3.0, HUDMESSAGE_POS_INFO)
						client_print(targetid, print_chat, "%s %s", g_MODclient, message)

						p_data_b[id][PB_TOBEREVIVED]=true
						PhoenixFound[vTeam-1]--
						break
					}
				}
			}
		}
	}
#endif


// ****************************************
// Shadow Hunter's Healing Wave
// ****************************************

public _Skill_Healing_Wave(parm[2]){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Skill_Healing_Wave",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if ( Verify_Skill(id, RACE_SHADOW, SKILL1) && is_user_alive(id) ){
		set_task(p_heal[p_data[id][P_SKILL1]-1],"_Skill_Healing_Wave",TASK_WAVE+id,parm,2)
	}
	
	// Prevent healing if this player is Hexed
	if ( p_data_b[id][PB_HEXED] )
		return PLUGIN_CONTINUE

	new team = get_user_team(id)

	new players[32], numberofplayers
	new i, targetid, distancebetween, targetorigin[3]
	new origin[3]

	get_user_origin(id, origin)
	get_players(players, numberofplayers,"a")

	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]

		if (p_data_b[targetid][PB_ISCONNECTED] && get_user_team(targetid) == team){
			get_user_origin(targetid, targetorigin)
			distancebetween = get_distance(origin, targetorigin)

			if (distancebetween < iCvar[FT_HEALING_RANGE]){

				get_user_origin(targetid,origin)
				
				if( get_user_actualhealth(targetid) + 1 <= get_user_maxhealth(targetid) ){

					set_user_health(targetid, get_user_health(targetid) + 1)

					Create_TE_IMPLOSION(origin, 100, 8, 1)
				}
			}
		}
	}

	return PLUGIN_CONTINUE
}


// ****************************************
// Shadow Hunter's Hex
// ****************************************

public _Skill_Hex(parm[2]){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Skill_Hex",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE
	
	p_data_b[id][PB_RENDER] = true
	p_data_b[id][PB_HEXED] = false
	
	/* Reset the user's speed */
	set_user_maxspeed(id, 250.0)

	set_user_rendering(id)

	emit_sound(id, CHAN_STATIC, SOUND_HEX, 1.0, ATTN_NORM, 0, PITCH_NORM)

	return PLUGIN_CONTINUE
}


// ****************************************
// Shadow Hunter's Serpent Ward
// ****************************************

public _Skill_SerpentWard(parm[5]){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Skill_SerpentWard",parm[3])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[3]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if (!is_user_alive(id))
		return PLUGIN_CONTINUE

	new origin[3], damage, start[3], end[3], red, blue, green
	origin[0]=parm[0]
	origin[1]=parm[1]
	origin[2]=parm[2]

	start[0] = origin[0]
	start[1] = origin[1]
	start[2] = origin[2] + 600
	end[0] = origin[0]
	end[1] = origin[1]
	end[2] = origin[2] - 600

#if MOD == 0
	if(parm[4]==TS){
		red = 255
		blue = 0
		green = 0
	}
	else{
		red = 0
		blue = 255
		green = 0
	}
#endif
#if MOD == 1
	if(parm[4]==AXIS){
		red = 255
		blue = 63
		green = 63
	}
	else{
		red = 76
		blue = 102
		green = 76
	}
#endif
	if(!g_mapDisabled)
		Create_TE_BEAMPOINTS(start, end, g_sLightning, 1, 5, 2, 500, 20, red, green, blue, 100, 100)

	new players[32], numberofplayers
	new i, targetid, distancebetween, targetorigin[3]

	get_players(players, numberofplayers, "a")
	
	for (i = 0; i < numberofplayers; ++i){
		targetid=players[i]
		if ( parm[4]!=get_user_team(targetid) ){
			get_user_origin(targetid,targetorigin)
			distancebetween = get_distance(origin, targetorigin)

			if ( distancebetween < 85 ){
				damage = 10
				WAR3_damage(targetid, id, damage, CSW_SERPENTWARD, -1)
				client_cmd(targetid, "speak ambience/thunder_clap.wav")
				Create_ScreenFade(targetid, (1<<10), (1<<10), (1<<12), red, green, blue, 255)
			}
		}
	}

	if (!endround)
		set_task(0.5,"_Skill_SerpentWard",TASK_LIGHT+id,parm,5)

	return PLUGIN_CONTINUE
}


// ****************************************
// Warden's Blink
// ****************************************

public Skills_Blink(id){
	#if ADVANCED_DEBUG
		writeDebugInfo("Skills_Blink",id)
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	if ( Verify_Skill(id, RACE_WARDEN, SKILL2) ){			// Blink
		new Float:randomnumber = random_float(0.0,1.0)
		if (randomnumber <= p_blink[p_data[id][P_SKILL2]-1])
			p_data_b[id][PB_WARDENBLINK]=true	
		else
			p_data_b[id][PB_WARDENBLINK]=false
	}
	else
		p_data_b[id][PB_WARDENBLINK]=false

	return PLUGIN_CONTINUE
}


// ****************************************
// Crypt Lord's Impale
// ****************************************

public _Skill_Impale(parm[2]){
	#if ADVANCED_DEBUG
		writeDebugInfo("_Skill_Impale",parm[0])
	#endif

	if (!warcraft3)
		return PLUGIN_CONTINUE

	new id = parm[0]

	if(!p_data_b[id][PB_ISCONNECTED])
		return PLUGIN_CONTINUE

	if(parm[1]<4){
		new Float:iMin = -1.0 * float(iCvar[FT_IMPALE_INTENSITY])
		new Float:iMax = float(iCvar[FT_IMPALE_INTENSITY])

		new Float:vec_angle[3], Float:vec_v_angle[3]
		new i

		entity_get_vector(id, EV_VEC_angles, vec_angle)
		entity_get_vector(id, EV_VEC_v_angle, vec_v_angle)

		for(i=0;i<2;i++){ // Don't modify z axis (causes a roll)
			vec_angle[i] = vec_angle[i] + random_float(iMin, iMax)
			vec_v_angle[i] = vec_v_angle[i] + random_float(iMin, iMax)
		}

		entity_set_int(id, EV_INT_fixangle, 1)
		entity_set_vector(id, EV_VEC_angles, vec_angle)
		entity_set_vector(id, EV_VEC_v_angle, vec_v_angle)
		parm[1]++
		set_task(0.1, "_Skill_Impale", 0, parm, 2)
	}
	else{
		entity_set_int(id, EV_INT_fixangle, 1)
	}

	return PLUGIN_CONTINUE
}