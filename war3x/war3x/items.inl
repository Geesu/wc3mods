
/* - AMX Mod Script --------------------------------------------- *

 *  Warcraft 3: Expansion
 *  (c) Copyright 2003
 *
 *  Shop Item Constants/Functions

 * -------------------------------------------------------------- */



/* - Reference Constants ---------------------------------------- */


#define VALUE_ANKH                  1.0     // (  float) % chance to gain items
//#define VALUE_BOOTS               275.0     // (  float) max speed value		 - moved to globals
#define VALUE_CLAWS                   6     // (integer) extra damage on each attack
#define VALUE_CLOAK                0.60     // (  float) invisibility percentage
#define VALUE_MASK                 20.0     // (  float) seconds removed from ultimate cooldown
#define VALUE_AMULET               40.0     // (  float) seconds between absorbsions
#define VALUE_RING                   50     // (integer) extra armor
#define VALUE_TOME                   25     // (integer) experience

new const ITEMCOST[_TSI + 1] =
{
    0,              // Item: None
    2200,           // Ankh of Reincarnation
    1000,           // Boots of Speed
    800,            // Claws of Attack
    750,            // Cloak of Shadows
    600,            // Sobi Mask
    1200,           // Amulet of Spell Shield
    400,            // Ring of Protection
    4000            // Tome of Experience
};

new const ITEMCOLOR[_TSI + 1][3] =
{
    {0,0,0},        // Item: None               (   NONE )
    {255,128,0},    // Ankh of Reincarnation    ( ORANGE )
    {255,255,255},  // Boots of Speed           (  WHITE )
    {255,0,0},      // Claws of Attack          (    RED )
    {0,255,255},    // Cloak of Shadows         (   BLUE )
    {255,0,255},    // Sobi Mask                ( PURPLE )
    {255,255,0},    // Amulet of Spell Shield   ( YELLOW )
    {0,255,0},      // Ring of Protection       (  GREEN )
    {128,128,128}   // Tome of Experience       (   NONE )
};

#define ITEM_DROPRATE              0.66     // (  float) percentage to drop items on death
#define ITEM_DROPLIFE              30.0     // (  float) seconds dropped items stay on ground

#define CLOAK_COOLDOWN              5.0     // (  float) cooldown for cloak of invisibility
new AMULET_RGB[3] =         {124,103,60};   // (integer) rgb of glowshell when amulet takes effect



/* - Buy/Drop Item Functions ------------------------------------ */


// Buy Normal Item

public Item_Buy( id, iNewItem ) {
#if ADVANCED_DEBUG
	log_function("public Item_Buy( id, iNewItem ) {");
#endif

    new iOldItem = g_PlayerInfo[id][CURRENT_ITEM];

    // Insufficient Funds

    if ( cs_get_user_money( id ) + ( ITEMCOST[iOldItem] / ITEM_BUYBACK ) < ITEMCOST[iNewItem] )
    {
        client_print( id, print_center, ITEM_INSUFFICIENT_FUNDS );
        return PLUGIN_HANDLED;
    }

    // Already Equipped

    if ( g_PlayerInfo[id][CURRENT_ITEM] == iNewItem )
    {
        client_print( id, print_center, ITEM_EQUIPPED );
        return PLUGIN_HANDLED;
    }

    // Equip the Item

    g_PlayerInfo[id][CURRENT_ITEM] = iNewItem;

    if ( !Item_Equip( id, iNewItem ) )
    {
        g_PlayerInfo[id][CURRENT_ITEM] = iOldItem;
        return PLUGIN_HANDLED;
    }

    // Check for item changes

    Item_Change( id, iOldItem, iNewItem );

    if ( g_bFreezeTime )
    {
        new Float:fFreezetime = get_cvar_float( "mp_freezetime" );
        WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.0, 0.0, fFreezetime );
    }

    else
    {
        WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );
    }

    // Play Sound

    emit_sound( id, CHAN_STATIC, SOUND_PICKUPITEM, 1.0, ATTN_NORM, 0, PITCH_NORM );

    if ( iOldItem )
    {
        // Give back some cash

        new iOldCost = ITEMCOST[iOldItem] / ITEM_BUYBACK;
        new iNewCost = ITEMCOST[iNewItem];

        new iDifference = iNewCost - iOldCost;

        cs_update_money( id, -iDifference, 1 );

        // Inform player

        new szMessage[128];
        format( szMessage, 127, ITEM_BUYBACK_MESSAGE, ITEMNAME[iOldItem], iOldCost );

        client_print( id, print_chat, szMessage );
    }

    else
    {
        // Update money

        cs_update_money( id, -ITEMCOST[iNewItem], 1 );
    }

    return PLUGIN_HANDLED;
}


// Buy Tome function

public Item_BuyTome( id ) {
#if ADVANCED_DEBUG
	log_function("public Item_BuyTome( id ) {");
#endif

    // Level 10 ( no bonus )

    if ( WAR3_get_level(g_PlayerInfo[id][CURRENT_XP]) == TOTAL_LEVELS )
    {
        client_print( id, print_center, ITEM_NOBONUS );
        return PLUGIN_HANDLED;
    }

    // Insufficient Funds

    if ( cs_get_user_money( id ) < ITEMCOST[ITEM_TOME] )
    {
        client_print( id, print_center, ITEM_INSUFFICIENT_FUNDS );
        return PLUGIN_HANDLED;
    }

    new Float:fMultiplier = get_cvar_float( "war3x_xp_normal" );

    new iTomeValue = VALUE_TOME;
    new Float:fTomeValue = float( iTomeValue );

    client_print( id, print_center, ITEM_TOME_MESSAGE, floatround( fTomeValue * fMultiplier ) );

    new iOldXp = g_PlayerInfo[id][CURRENT_XP];
    new iNewXp = g_PlayerInfo[id][CURRENT_XP] + floatround( fTomeValue * fMultiplier );

    XP_Give( id, iOldXp, iNewXp );

    // Play Sound

    emit_sound( id, CHAN_STATIC, SOUND_TOME, 1.0, ATTN_NORM, 0, PITCH_NORM );

    cs_update_money( id, -ITEMCOST[ITEM_TOME], 1 );


    return PLUGIN_HANDLED;
}


public Item_Equip( id, iNewItem ) {
#if ADVANCED_DEBUG
	log_function("public Item_Equip( id, iNewItem ) {");
#endif

    new iLevel = WAR3_get_level( g_PlayerInfo[id][CURRENT_XP] );

    // Ankh of Reincarnation

    if ( iNewItem == ITEM_ANKH )
    {
        // Don't give to VIP

        if ( id == g_Vip )
        {
            client_print( id, print_center, ITEM_UNAVAILABLE );
            return ( 0 );
        }

        // Don't give to Orcs if skill is >= Ankh value

        if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_ORC && g_PlayerInfo[id][CURRENT_SKILL3] && s_Reincarnate[g_PlayerInfo[id][CURRENT_SKILL3] - 1] > VALUE_ANKH )
        {
            client_print( id, print_center, ITEM_NOBONUS );
            return ( 0 );
        }

        new szMessage[128], iLen;
        iLen += format( szMessage[iLen], 127 - iLen, ITEM_ANKH_PRIMARY );

        if ( get_cvar_num( "war3x_ankhpistols" ) )
            iLen += format( szMessage[iLen], 127 - iLen, ITEM_ANKH_SECONDARY );

        if ( get_cvar_num( "war3x_ankhgrenades" ) )
            iLen += format( szMessage[iLen], 127 - iLen, ITEM_ANKH_GRENADES );

        new Float:fAnkhValue = VALUE_ANKH;
        client_print( id, print_chat, ITEM_ANKH_MESSAGE, floatround( fAnkhValue * 100.0 ), "%", szMessage );
    }

    // Boots of Speed

    else if ( iNewItem == ITEM_BOOTS )
    {
        if ( g_PlayerInfo[id][CURRENT_RACE] == RACE_UNDEAD )
        {
            new Float:fNewSpeed = VALUE_BOOTS - SPEED_KNIFE + SUnholyAura_Get( iLevel );

            if ( fNewSpeed > CAP_SPEEDBONUS )
                fNewSpeed = CAP_SPEEDBONUS;

            if ( fNewSpeed < VALUE_BOOTS )
                fNewSpeed = VALUE_BOOTS;

            client_print( id, print_chat, ITEM_BOOTS_MESSAGE, SUnholyAura_Get( iLevel ), fNewSpeed );
        }

        else
        {
            client_print( id, print_chat, ITEM_BOOTS_MESSAGE, SPEED_KNIFE, VALUE_BOOTS );
        }

        WAR3_set_speed( id );
    }

    // Claws of Attack +6

    else if ( iNewItem == ITEM_CLAWS )
    {
        client_print( id, print_chat, ITEM_CLAWS_MESSAGE, VALUE_CLAWS );
    }

    // Cloak of Shadows

    else if ( iNewItem == ITEM_CLOAK )
    {
        client_print( id, print_chat, ITEM_CLOAK_MESSAGE, ( VALUE_CLOAK * 100.0 ), "%" );
        Invis_Set( id );
    }

    // Sobi Mask

    else if ( iNewItem == ITEM_MASK )
    {
        // Don't give mask if no ultimate

        if ( !g_PlayerInfo[id][CURRENT_ULTIMATE] )
        {
            client_print( id, print_center, ITEM_NOMASK );
            return ( 0 );
        }

        if ( g_fUltimateCooldown[id] && ( g_fBuyTime + get_cvar_float( "war3x_ultimatewarmup" ) ) < get_gametime() )
        {
            new TaskId = TASK_ULTIMATECOOLDOWN + id;
            remove_task( TaskId, 0 );

            new Float:fCurrentCooldown = get_gametime() - g_fUltimateCooldown[id];
            new Float:fRemainingCooldown = ULTIMATE_COOLDOWNDEFAULT - fCurrentCooldown;

            new Float:fTimeRemoved = fRemainingCooldown * ( VALUE_MASK / ULTIMATE_COOLDOWNDEFAULT );
            new Float:fNewCooldown = fRemainingCooldown - fTimeRemoved;

            g_fUltimateCooldown[id] -= fTimeRemoved;

            new parm_Cooldown[1];
            parm_Cooldown[0] = id;

            set_task( fNewCooldown, "Ultimate_Restore", TaskId, parm_Cooldown, 1 );
        }

        client_print( id, print_chat, ITEM_MASK_MESSAGE, ULTIMATE_COOLDOWNDEFAULT, ULTIMATE_COOLDOWNDEFAULT - VALUE_MASK );
    }

    // Amulet of Spell Shield

    else if ( iNewItem == ITEM_AMULET )
    {
        client_print( id, print_chat, ITEM_AMULET_MESSAGE, VALUE_AMULET );
    }

    // Ring of Protection +5

    else if ( iNewItem == ITEM_RING )
    {
        if ( !get_user_armor( id ) )
        {
            client_print( id, print_center, ITEM_NOARMOR );
            return ( 0 );
        }

        set_user_armor( id, get_user_armor( id ) + VALUE_RING );
        client_print( id, print_chat, ITEM_RING_MESSAGE, WAR3_get_maxarmor( id ) );
    }

    return ( 1 );
}


public Item_Change( id, iOldItem, iNewItem ) {
#if ADVANCED_DEBUG
	log_function("public Item_Change( id, iOldItem, iNewItem ) {");
#endif

    // Remove Speed Bonus if New Item is not Boots

    if ( iOldItem == ITEM_BOOTS && iNewItem != ITEM_BOOTS )
    {
        WAR3_set_speed( id );
    }

    // Remove Invisibility if New Item is not Cloak

    if ( iOldItem == ITEM_CLOAK && iNewItem != ITEM_CLOAK )
    {
        new iWeaponId, iClip, iAmmo;
        iWeaponId = get_user_weapon( id, iClip, iAmmo );

        if ( !( g_PlayerInfo[id][CURRENT_RACE] == RACE_HUMAN && g_PlayerInfo[id][CURRENT_SKILL1] && iWeaponId == CSW_KNIFE ) )
            Invis_Remove( id );
    }

    // Remove Cooldown Bonus if New Item is not Sobi Mask

    if ( iOldItem == ITEM_MASK && iNewItem != ITEM_MASK )
    {
        if ( g_fUltimateCooldown[id] )
        {
            new TaskId = TASK_ULTIMATECOOLDOWN + id;
            remove_task( TaskId, 0 );

            new Float:fCurrentCooldown = get_gametime() - ( g_fUltimateCooldown[id] + VALUE_MASK );
            new Float:fCooldownUsed = fCurrentCooldown / ( ULTIMATE_COOLDOWNDEFAULT - VALUE_MASK );
            new Float:fTimeRemoved = fCooldownUsed * ULTIMATE_COOLDOWNDEFAULT;
            new Float:fNewCooldown = ULTIMATE_COOLDOWNDEFAULT - fTimeRemoved;

            g_fUltimateCooldown[id] = get_gametime() - fTimeRemoved;

            new parm_Cooldown[1];
            parm_Cooldown[0] = id;

            set_task( fNewCooldown, "Ultimate_Restore", TaskId, parm_Cooldown, 1 );
        }
    }

    // Remove Bonus Armor if New Item is not Ring of Protection

    if ( iOldItem == ITEM_RING && iNewItem != ITEM_RING )
    {
        new iNewArmor = get_user_armor( id ) - VALUE_RING;

        if ( iNewArmor < 0 )
            iNewArmor = 0;

        set_user_armor( id, iNewArmor );
    }

    return PLUGIN_HANDLED;
}


public Item_Create( id, iItemNum ) {
#if ADVANCED_DEBUG
	log_function("public Item_Create( id, iItemNum ) {");
#endif

    new Float:fOrigin[3];
    entity_get_vector( id, EV_VEC_origin, fOrigin );

    new iEnt = Create_TempEnt( "WAR3X_ITEM", "models/sphere.mdl", fOrigin, MOVETYPE_TOSS, SOLID_TRIGGER, ITEM_DROPLIFE );

    entity_set_int( iEnt, EV_INT_rendermode, kRenderTransColor );
    entity_set_int( iEnt, EV_INT_renderfx, kRenderFxGlowShell );

    new Float:fColors[3];
    fColors[0] = float( ITEMCOLOR[iItemNum][GLOW_R] );
    fColors[1] = float( ITEMCOLOR[iItemNum][GLOW_G] );
    fColors[2] = float( ITEMCOLOR[iItemNum][GLOW_B] );

    entity_set_vector( iEnt, EV_VEC_rendercolor, fColors );
    entity_set_float( iEnt, EV_FL_renderamt, 150.0 );

    entity_set_int( iEnt, EV_ENT_owner, iItemNum );

    return ( iEnt );
}


public Item_Drop( id ) {
#if ADVANCED_DEBUG
	log_function("public Item_Drop( id ) {");
#endif

    if ( !g_PlayerInfo[id][CURRENT_ITEM] )
    {
        client_print( id, print_center, ITEM_NODROP );
        return PLUGIN_CONTINUE;
    }

    // Remove Item

    new iOldItem = g_PlayerInfo[id][CURRENT_ITEM];
    g_PlayerInfo[id][CURRENT_ITEM] = ITEM_NONE;

    Item_Change( id, iOldItem, ITEM_NONE );

    WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );
    client_print( id, print_center, ITEM_DROP );

    // Create Item

    new iEnt = Item_Create( id, iOldItem );

    // Toss Item

    new Float:fOrigin[3];
    entity_get_vector( id, EV_VEC_origin, fOrigin );

    new iaimOrigin[3];
    get_user_origin( id, iaimOrigin, 3 );

    new Float:faimOrigin[3];
    faimOrigin[0] = float( iaimOrigin[0] );
    faimOrigin[1] = float( iaimOrigin[1] );
    faimOrigin[2] = fOrigin[2];

    set_velocity_to_origin( iEnt, faimOrigin, 500.0 );

    return PLUGIN_HANDLED;
}


public Item_Pickup( id, iItemEnt, iNewItem ) {
#if ADVANCED_DEBUG
	log_function("public Item_Pickup( id, iItemEnt, iNewItem ) {");
#endif

    // Check if restricted

    if ( get_cvar_bitsum( "war3x_restrict_items" ) & WAR3_get_item_flag( iNewItem ) )
        return PLUGIN_HANDLED;

    g_PlayerInfo[id][CURRENT_ITEM] = iNewItem;

    if ( !Item_Equip( id, iNewItem ) )
    {
        g_PlayerInfo[id][CURRENT_ITEM] = ITEM_NONE;
        return PLUGIN_HANDLED;
    }

    Item_Change( id, ITEM_NONE, iNewItem );

    WAR3_hud_item( id, HUDMESSAGE_FX_FADEIN, 10.0, 0.1, 2.0, 3.0 );
    client_print( id, print_center, ITEM_PICKUP );

    // Play Sound

    emit_sound( id, CHAN_STATIC, SOUND_PICKUPITEM, 1.0, ATTN_NORM, 0, PITCH_NORM );

    remove_entity( iItemEnt );

    return PLUGIN_HANDLED;
}


public Item_DropDead( id, iItemNum ) {
#if ADVANCED_DEBUG
	log_function("public Item_DropDead( id, iItemNum ) {");
#endif

    // Create Item

    new iEnt = Item_Create( id, iItemNum );

    // Toss Item ( give player's velocity and then some )

    new Float:fVelocity[3];
    entity_get_vector( id, EV_VEC_velocity, fVelocity );

    fVelocity[0] *= 1.2;
    fVelocity[1] *= 1.2;
    fVelocity[2] *= 1.2;

    entity_set_vector( iEnt, EV_VEC_velocity, fVelocity );

    return PLUGIN_HANDLED;
}


/* - Item Functions --------------------------------------------- */


// Claws of Attack +6

public IClaws_Damage( iAttackerId, iVictimId, iWeaponId, iHeadshot ) {
#if ADVANCED_DEBUG
	log_function("public IClaws_Damage( iAttackerId, iVictimId, iWeaponId, iHeadshot ) {");
#endif

    // Add to bonus damage array

    WAR3_damage( iAttackerId, iVictimId, iWeaponId, VALUE_CLAWS, iHeadshot, DAMAGE_CHECKARMOR );

    if ( is_user_alive( iVictimId ) )
    {
        // Screen Fade

        new iFadeAlpha = VALUE_CLAWS * 3;
        Create_ScreenFade( iVictimId, (1<<10), (1<<10), FADE_OUT, 255, 0, 0, iFadeAlpha );
    }

    return PLUGIN_HANDLED;
}


// Cloak of Invisibility

public ICloak_Set( id ) {
#if ADVANCED_DEBUG
	log_function("public ICloak_Set( id ) {");
#endif

    g_bPlayerInvis[id] = true;

    new Float:fInvis = ( 1.0 - VALUE_CLOAK ) * 255.0;
    new iInvis = floatround( fInvis );

    set_user_rendering( id, kRenderFxNone, 0, 0, 0, kRenderTransTexture, iInvis );

    Invis_Icon( id, ICON_SHOW );

    return PLUGIN_HANDLED;
}


// Amulet of Spell Shield

public IAmulet_Ready( cIndex, tIndex ) {
#if ADVANCED_DEBUG
	log_function("public IAmulet_Ready( cIndex, tIndex ) {");
#endif

    if ( g_PlayerInfo[tIndex][CURRENT_ITEM] == ITEM_AMULET && get_gametime() - g_fAmuletTime[tIndex] >= VALUE_AMULET )
    {
        IAmulet_Block( cIndex, tIndex );
        return ( 1 );
    }

    return ( 0 );
}

public IAmulet_Block( cIndex, tIndex ) {
#if ADVANCED_DEBUG
	log_function("public IAmulet_Block( cIndex, tIndex ) {");
#endif

    g_fAmuletTime[tIndex] = get_gametime();

    // Message

    new szMessage[128], szCasterName[32];
    get_user_name( cIndex, szCasterName, 31 );

    format( szMessage, 127, BLOCK_ULTIMATE_T );
    WAR3_status_text( tIndex, szMessage, 3.0 );

    format( szMessage, 127, BLOCK_ULTIMATE_C );
    WAR3_status_text( cIndex, szMessage, 3.0 );

    // Glow

    Glow_Set( tIndex, 1.0, AMULET_RGB, 48 );

    // Sparks

    for ( new i = 0; i < 8; i++ )
    {
        new iOrigin[3];
        get_user_origin( tIndex, iOrigin );

        iOrigin[0] += random_num( -20, 20 );
        iOrigin[1] += random_num( -20, 20 );
        iOrigin[2] += random_num( -20, 20 );

        Create_TE_SPARKS( SHOWTO_ALL_BROADCAST, iOrigin );
    }

    return PLUGIN_HANDLED;
}

public Item_Touch( iToucherId, iPlayerId )
{
#if ADVANCED_DEBUG
	log_function("public Item_Touch( iToucherId, iPlayerId )");
#endif

	// Sanity checks
    if ( iPlayerId < 1 || iPlayerId > 32 || iToucherId < 1 || !is_user_alive( iPlayerId ) )
        return PLUGIN_CONTINUE;

    if ( !g_PlayerInfo[iPlayerId][CURRENT_ITEM] )
    {
        // Make sure on ground

        if ( get_entity_flags( iToucherId ) & FL_ONGROUND )
        {
            new iItemNum = entity_get_int( iToucherId, EV_ENT_owner );
            Item_Pickup( iPlayerId, iToucherId, iItemNum );
        }
    }

	return PLUGIN_CONTINUE;
}
