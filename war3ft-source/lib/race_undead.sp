/*
    * Vampiric Aura: Gives you a percentage of the damage you do in attack back as health
    * Unholy Aura: Gives you a speed boost, also all weapons make you go at the same speed
    * Levitation: Allows you to jump higher by reducing your gravity
    * Ultimate, Suicide Bomber: When you die you will explode killing nearby enemies and regenerate 
*/

/*
new const Float:p_vampiric[3] =			{0.10,0.20,0.30}		// Vampiric Aura			(skill 1)
new Float:p_unholy[3] =					{265.0,285.0,300.0}		// Unholy Aura				(skill 2)
new Float:p_unholy_dod[3] =				{33.3,66.6,100.0}		// Unholy Aura				(skill 2)
new const Float:p_levitation[3] =		{0.8,0.6,0.4}			// Levitation				(skill 3)
*/

new const Float:UD_VampiricAura[3] = {0.10,0.20,0.30};

//UD_OnTakeDamage(client, &inflictor, &attacker, &Float:Damage, &DamageType, &AmmoType)
UD_OnTakeDamage(victim, attacker, Damage)
{
	if ( g_PlayerCurrentRace[attacker] == RACE_UNDEAD )
	{
		//PrintToChat(attacker, "[WC3-Pre] Health: %d", GetClientHealth( attacker ));

		new bonus_health = RoundToCeil( float(Damage) * UD_VampiricAura[2] );
		new current_health = GetClientHealth( attacker );
		new max_health = GetClientMaxHealth( attacker );


		if ( bonus_health + current_health > max_health )
		{
			SetClientHealth( attacker, max_health );
		}
		else
		{
			SetClientHealth( attacker, current_health + bonus_health );
		}

		//PrintToChat(attacker, "[WC3-Post] Health: %d", GetClientHealth( attacker ));



		/*
				// Make the attacker glow
		SHARED_Glow( iAttacker, 0, ( 2 * iBonusHealth ), 0, 0 );
		
		// Give the attacker a nice screen fade
		Create_ScreenFade( iAttacker, (1<<10), (1<<10), (1<<12), 0, 255, 0, iBonusHealth );
		*/

		new Handle:handle = StartMessageOne("Fade", attacker);

		if (handle != INVALID_HANDLE)
		{
			BfWriteShort(handle, RoundToZero(0.3 * (1<<SCREENFADE_FRACBITS)));
			BfWriteShort(handle, RoundToZero(0.7 * (1<<SCREENFADE_FRACBITS)));
			BfWriteShort(handle, FFADE_IN|FFADE_PURGE);

			BfWriteByte(handle, 0);
			BfWriteByte(handle, 255);
			BfWriteByte(handle, 0);
			BfWriteByte(handle, bonus_health);
			
			EndMessage();
		}
	}

	return 0;
}



//SetEntityGravity


/*
void DBUtils::ScreenFade( int EntIdx, const color32 &color, float duration, float fadeHold, int flags )
{
	static int msgid = UserMessageIndex("Fade");

	MRecipientFilter mrf(m_Engine);
	mrf.AddPlayer(EntIdx);

	bf_write *bf;
	bf = m_Engine->UserMessageBegin( &mrf, msgid );
		bf->WriteShort( FixedUnsigned16( duration, 1<<SCREENFADE_FRACBITS ) );				// fade lasts this long
		bf->WriteShort( FixedUnsigned16( fadeHold, 1<<SCREENFADE_FRACBITS ) );				// fade lasts this long
		bf->WriteShort( flags );			// fade type (in / out)
		bf->WriteByte( color.r );				// fade red
		bf->WriteByte( color.g );				// fade green
		bf->WriteByte( color.b );				// fade blue
		bf->WriteByte( color.a );				// fade alpha
	m_Engine->MessageEnd();
}*/