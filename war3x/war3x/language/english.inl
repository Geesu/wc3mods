// Begin ENGLISH.INL

#if !defined __WAR3X_LANGUAGE__
#define __WAR3X_LANGUAGE__


/* - Items ------------------------------------------------------ */

new const RACEKEYNAME[_TR][] =      // RACE NAMES (save key names):
{
    "UD",                           // Undead
    "HU",                           // Human
    "OR",                           // Orc
    "NE"                            // Night Elf

    /*
    "BE",                           // Blood Elf
    "NA",                           // Naga
    "DR"                            // Draenei
    */
};


/* - Classes ( based on level/ultimate ) ------------------------ */


new const RACE1CLASS[_TC][] =       // UNDEAD CLASSES:
{
    "Acolyte",                      // Acolyte          (Level 0)
    "Ghoul",                        // Ghoul            (Level 2)
    "Abomination",                  // Abomination      (Level 5)
    "Death Knight",                 // Death Knight     (Ultimate: Death Coil)
    "Crypt Lord",                   // Crypt Lord       (Ultimate: Impale)
    "Dread Lord"                    // Dread Lord       (Ultimate: Sleep)
};

new const RACE2CLASS[_TC][] =       // HUMAN CLASSES:
{
    "Peasant",                      // Peasant          (Level 0)
    "Footman",                      // Footman          (Level 2)
    "Knight",                       // Knight           (Level 5)
    "Paladin",                      // Paladin          (Ultimate: Holy Light)
    "Blood Mage",                   // Blood Mage       (Ultimate: Flame Strike)
    "Mountain King"                 // Mountain King    (Ultimate: Avatar)
};

new const RACE3CLASS[_TC][] =       // ORC CLASSES:
{
    "Peon",                         // Peon             (Level 0)
    "Grunt",                        // Grunt            (Level 2)
    "Tauren",                       // Tauren           (Level 5)
    "Shadow Hunter",                // Shadow Hunter    (Ultimate: Healing Wave)
    "Far Seer",                     // Far Seer         (Ultimate: Chain Lightning)
    "Blademaster"                   // Blademaster      (Ultimate: Wind Walk)
};

new const RACE4CLASS[_TC][] =       // NIGHT ELF CLASSES:
{
    "Wisp",                         // Wisp             (Level 0)
    "Archer",                       // Archer           (Level 2)
    "Hunter",                       // Hunter           (Level 5)
    "Druid",                        // Druid            (Ultimate: Rejuvination)
    "Keeper",                       // Keeper           (Ultimate: Entangling Roots)
    "Warden"                        // Warden           (Ultimate: Shadow Strike)
};

/* FUTURE RACES

new const RACE5CLASS[_TC][] =       // BLOOD ELF CLASSES:
{
    "Worker",                       // Worker           (Level 0)
    "Swordsman",                    // Swordsman        (Level 2)
    "Spell Breaker",                // Spell Breaker    (Level 5)
    "Priest",                       // Priest           (Ultimate: Heal)
    "Blood Mage",                   // Blood Mage       (Ultimate: Flame Strike)
    "Dragonhawk"                    // Dragonhawk       (Ultimate: Aerial Shackles)
};

new const RACE6CLASS[_TC][] =       // NAGA CLASSES:
{
    "Slave",                        // Slave            (Level 0)
    "Reaver",                       // Reaver           (Level 2)
    "Myrmidon",                     // Myrmidon         (Level 5)
    "Sea Witch",                    // Sea Witch        (Ultimate: Mana Shield)
    "Sea Witch",                    // Sea Witch        (Ultimate: Forked Lightning)
    "Siren"                         // Siren            (Ultimate: Parasite)
};

new const RACE7CLASS[_TC][] =       // DRAENEI CLASSES:
{
    "Laborer",                      // Laborer          (Level 0)
    "Stalker",                      // Stalker          (Level 2)
    "Assassin",                     // Assassin         (Level 5)
    "Seer",                         // Seer             (Ultimate: Healing Wave)
    "Harbinger",                    // Harbinger        (Ultimate: Ray of Disruption)
    "Elder Sage"                    // Elder Sage       (Ultimate: Shadow Strike)
};

*/


/* - Status Messages -------------------------------------------- */


new const STATUS_PREFIX[]               = "* [WC3x]  S T A T U S   T E X T :  ";
new const WAR3X_PREFIX[]                = "* [WC3x]";

new const CANT_TARGET[]                 = "Unable to target player.";
new const CANT_TARGET_SELF[]            = "Unable to target Self.";
new const CANT_TARGET_HOLYLIGHT[]       = "Must target friendly living players or enemy Undead players.";
new const CANT_TARGET_DEATHCOIL[]       = "Must target friendly Undead players or enemy living players.";
new const CANT_TARGET_AIR[]             = "Your target must be on the ground.";
new const CANT_TARGET_SLEEP[]           = "Unable to target sleeping players.";
new const CANT_TARGET_INVIS[]           = "Unable to target invisible players.";

new const FULLHEALTH_TARGET[]           = "Target is at full health.";
new const FULLHEALTH_SELF[]             = "You are at full health.";

new const HEAL_TARGET[]                 = "%s has restored %d of your hitpoints.";
new const HEAL_SELF[]                   = "You have restored %d of your own hitpoints.";
new HEAL_CAST[]							= "You have cast a heal on your teammate(s).";

new const BLOCK_ULTIMATE_C[]            = "Your ultimate has been blocked.";
new const BLOCK_ULTIMATE_T[]            = "You have blocked a harmful ultimate.";

new const CAST_HOLYLIGHT[]              = "%s has cast Holy Light on you. You take %d damage.";
new const CAST_FLAMESTRIKE[]            = "%s conjures a pillar of fire around you.";
new const CAST_CHAINLIGHTNING[]         = "%s has cast Chain lightning on you. You take %d damage.";
new const CAST_CHAINLIGHTNING_JUMP[]    = "%s's Chain Lightning has jumped to you. You take %d damage";
new const CAST_ROOTS[]                  = "You have been rooted by %s.";
new const CAST_SHADOWSTRIKE[]           = "%s's Shadowstrike has overwhelmed you.";
new const CAST_SLEEP[]                  = "%s has put you to sleep.";
new const CAST_WINDWALK[]               = "Your surprise attack dealt %d damage.";
new const CAST_AVATAR[]                 = "Your body surges with strength!";

new const FINISH_SLEEP[]                = "You no longer feel sleepy.";
new const FINISH_WINDWALK[]             = "Your walk has ended.";
new const FINISH_AVATAR[]               = "Your strength diminishes.";

new const DAMAGE_FROSTNOVA[]            = "%s's Frost Nova hits you for %d damage.";
new const DAMAGE_PULVERIZE[]            = "You have been Pulverized for %d damage.";
new const DAMAGE_ELUNE[]                = "Elune's Grace has absorbed %d damage.";
new const DAMAGE_EVASION[]              = "You successfully avoided %d damage.";

new const REJUVINATE_SELF[]             = "You cast Rejuvinate on yourself.";
new const REJUVINATE_TARGET[]           = "%s has cast Rejuvinate on you.";
new const REJUVINATE_ALREADY[]          = "Target is already Rejuvinating.";

new const ARMOR_MESSAGE_UD[]            = "A shield of frost surrounds you.";
new const ARMOR_MESSAGE_HU[]            = "An inner fire burns within you.";
new const ARMOR_MESSAGE_OR[]            = "You and your armor are one.";
new const ARMOR_MESSAGE_NE[]            = "You are blessed by the touch of nature.";

new const ARMOR_MESSAGE_FADE_UD[]       = "Your frost armor fades.";
new const ARMOR_MESSAGE_FADE_HU[]       = "Your inner fire fades.";
new const ARMOR_MESSAGE_FADE_OR[]       = "Your armor fades.";
new const ARMOR_MESSAGE_FADE_NE[]       = "You no longer feel blessed.";

new const DISPELL_SHADOWSTRIKE[]        = "%s has dispelled your Shadow Strike.";
new const DISPELL_ROOTS[]               = "%s has dispelled your Entangling Roots.";
new const DISPELL_GENERIC[]             = "All negative effects on you have been dispelled.";

new const ULTIMATE_READY[]              = "Ultimate is ready.";
new const ULTIMATE_NOTREADY[]           = "Ultimate not ready.";
new const ULTIMATE_NOTREADY_COOLDOWN[]  = "Ultimate not ready.^n(%d seconds remaining)";
new const ULTIMATE_NOTFOUND[]           = "Ultimate not found.";
new const ULTIMATE_RESTRICTED[]         = "Ultimate is restricted on this map.";
new const ULTIMATE_NOCAST_VIP[]         = "VIP can not cast ultimates!";
new const ULTIMATE_NOCAST_PISTOL[]      = "Can not cast ultimates in pistol round.";
new const ULTIMATE_NOCAST_AIR[]         = "You must be on the ground to cast ultimates.";
new const ULTIMATE_NOCAST_SLEEP[]       = "Can not cast ultimates when sleeping.";

new const INFO_GAINLEVEL[]              = "You gained a level! You are now level %d!";
new const INFO_NEWRACENEXTROUND[]       = "You will spawn next round as %s.";
new const INFO_NEWRACENOW[]             = "Switching your race to %s...";
new const INFO_NOSKILLPOINTS[]          = "You have already spent all your skill points!";

new const INFO_RESTRICTEDMAP_HUD[]      = "Current Map is restricted to Short-Term XP.";

new const INFO_FOLLOW_NORACE[]          = "Race not yet Selected";
new const INFO_FOLLOW_MESSAGE_1[]       = "%s Level %i   XP:%i/%i (%i%s)";
new const INFO_FOLLOW_MESSAGE_2[]       = "Item: %s   Backpack: %s";
new const INFO_FOLLOW_MESSAGE_3[]       = "Health: %i   Armor: %i   Money: %i";


/* - Chat Messages ---------------------------------------------- */


new const ITEM_INSUFFICIENT_FUNDS[]     = "You have Insufficient Funds!";
new const ITEM_EQUIPPED[]               = "Item already Equipped!";
new const ITEM_UNAVAILABLE[]            = "Item Unavailable";
new const ITEM_NOBONUS[]                = "Item no longer gives bonus!";
new const ITEM_NOARMOR[]                = "You must buy armor first!";
new const ITEM_NOMASK[]                 = "You don't have an ultimate!";
new const ITEM_DROP[]                   = "Item has been dropped!";
new const ITEM_PICKUP[]                 = "Item has been equipped!";
new const ITEM_NODROP[]                 = "No item(s) equipped!";

new const ITEM_ANKH_MESSAGE[]           = "* [WC3x] You have a %d%s chance of reincarnating your weapons next round. (%s)";
new const ITEM_ANKH_PRIMARY[]           = "primary";
new const ITEM_ANKH_SECONDARY[]         = ", sidearm";
new const ITEM_ANKH_GRENADES[]          = ", grenades";
new const ITEM_ANKH_NOITEMS[]           = "No items to reincarnate!";

new const ITEM_BOOTS_MESSAGE[]          = "* [WC3x] Your maximum speed has increased by %0.1f%%.";
new const ITEM_CLAWS_MESSAGE[]          = "* [WC3x] All of your attacks will deal an additional %d damage.";
new const ITEM_CLOAK_MESSAGE[]          = "* [WC3x] You are now less visible by %0.0f%s, and untargetable by ultimates when invisible.";
new const ITEM_MASK_MESSAGE[]           = "* [WC3x] Your ultimate cooldown has decreased from %0.1f seconds to %0.1f.";
new const ITEM_AMULET_MESSAGE[]         = "* [WC3x] You are now able to block %d harmful ultimate(s) cast on you.";
new const ITEM_RING_MESSAGE[]           = "* [WC3x] Your maximum armor has increased to %d.";
new const ITEM_TOME_MESSAGE[]           = "* You gain %d experience from the tome. *";
new const ITEM_BUYBACK_MESSAGE[]        = "* [WC3x] You sold your %s for $%d!";

new const ITEM_AMULET_DESTROYED[]       = "* [WC3x] Your Amulet has been destroyed.";

new const VIP_ANKH_MESSAGE[]            = "* [WC3x] Your weapons will be restored when you are no longer the VIP.";
new const VIP_NOSKILLS[]                = "* [WC3x] The VIP does not have access to skills or ultimates.";

new const INFO_SHOP_DISABLED[]          = "Server has disabled Shop menus!";
new const INFO_SHOP_VIP[]               = "VIP can not buy items!";
new const INFO_SHOP_BUYZONE[]           = "You must be in a buy zone!";

new const INFO_NOTCONNECTED[]           = "You must wait until next round.";

new const INFO_LEVEL_FIRST[]            = "^n* [WC3x] Your first selected race has automatically been given level %d!^n";
new const INFO_LEVEL_OTHER[]            = "^n* [WC3x] Your selected race has been given level %d.^n";

new const INFO_CURRENTSKILLS[]          = "* [WC3x] Current Skills: %s (%0.0f%s)%s";
new const INFO_SKILLDETAILS[]           = "* [WC3x] Type '/skills' in chat for detailed skill information.";

new const INFO_RESETSKILLS[]            = "* [WC3x] Your skills have been successfully reset.";
new const INFO_RESETULTIMATE[]          = "* [WC3x] Your ultimate has been successfully reset.";
new const INFO_RESETXP[]                = "* [WC3x] Your %s experience has been successfully reset.";

new const INFO_RESTRICTEDMAP_CHAT[]     = "* [WC3x] Current map is restricted to Short-Term XP. Don't Worry, your Long-Term XP has not been deleted.";
new const INFO_DISABLEDMAP_CHAT[]       = "* [WC3x] Warcraft 3: Expansion has been disabled for the current map.";
new const INFO_GENERATEHELP_CHAT[]      = "* [WC3x] Help files have been generated.";

new const XP_SUPPORT_HEALMSG[]          = "* [WC3x] You have received %d XP for restoring %d health on your teammate(s).";
new const XP_SUPPORT_HEALMSG_MAX[]      = "* [WC3x] You have restored %d health on your teammate(s).";
//new const XP_SUPPORT_DISPELLMSG[]       = "* [WC3x] You have received %d XP for dispelling %d negative effects from your teammate(s).";


/* - Enable / Disable ------------------------------------------- */

// CONSOLE

new const WAR3X_FORMAT[]                = "Incorrect Format - Usage: warcraft3 < on | off >";
new const WAR3_ENABLED[]               = "* [WC3x] Warcraft 3: Expansion is now enabled.";
new const WAR3_DISABLED[]              = "* [WC3x] Warcraft 3: Expansion is now disabled.";
new const WAR3_ENABLED_ALREADY[]       = "* [WC3x] Warcraft 3: Expansion is already enabled.";
new const WAR3_DISABLED_ALREADY[]      = "* [WC3x] Warcraft 3: Expansion is already disabled.";

// HUD

new const WAR3_ENABLED_HUD[]           = "Warcraft 3: Expansion has been enabled^nRound Restarting";
new const WAR3_DISABLED_HUD[]          = "Warcraft 3: Expansion has been disabled^nRound Restarting";

// CHAT

new const WAR3X_VOTE_NOVOTE[]           = "* [WC3x] Voting is currently disabled.";
new const WAR3X_VOTE_ALREADY[]          = "* [WC3x] There is already a vote in progress.";
new const WAR3X_VOTE_NOTALLOWED[]       = "* [WC3x] Voting is not allowed at this time.";
new const WAR3X_WAR3_vote_startED[]          = "* [WC3x] %s started Warcraft 3: Expansion vote...";

new const WAR3X_VOTE_FOR[]              = "for";
new const WAR3X_VOTE_AGAINST[]          = "against";

new const WAR3X_VOTE_ENABLED[]          = "Enabled.";
new const WAR3X_VOTE_DISABLED[]         = "Disabled.";
new const WAR3X_VOTE_REMAINENABLED[]    = "Remains enabled.";
new const WAR3X_VOTE_REMAINDISABLED[]   = "Remains disabled.";

new const WAR3X_VOTE_FAILED[]           = "* [WC3x] Vote failed (yes '%d')|(no '%d')|(needed '%.2f')";
new const WAR3X_VOTE_SUCCESS[]          = "* [WC3x] Vote Succesful (yes '%d')|(no '%d')|(needed '%.2f')";
new const WAR3X_VOTE_TIE[]              = "* [WC3x] Vote Tie (yes '%d')|(no '%d')";


/* - Menus ------------------------------------------------------ */

// MENU SYMBOLS KEY:
// \y = YELLOW, \w = WHITE, \d = GREY, \r = RED, \R = RIGHT ALIGN, ^n = NEW LINE

new const MENU_VOTE_DISABLE[]           = "\yDisable Warcraft3: Expansion?\w^n^n1.  Yes^n2.  No";
new const MENU_VOTE_ENABLE[]            = "\yEnable Warcraft3: Expansion?\w^n^n1.  Yes^n2.  No";

new const MENU_WAR3MENU_TITLE[]         = "\yWarcraft 3 Menu ^n^n\w";
new const MENU_WAR3MENU_1[]             = "1. Select Race^n";
new const MENU_WAR3MENU_2[]             = "2. Select Skills^n";
new const MENU_WAR3MENU_3[]             = "3. Buy Items^n";
new const MENU_WAR3MENU_4[]             = "4. Show Level/Skills^n";
new const MENU_WAR3MENU_5[]             = "5. Player Skills^n";
new const MENU_WAR3MENU_6[]             = "6. Player Options^n";
new const MENU_WAR3MENU_7[]             = "7. Reset Skills/Ultimate/XP...^n";
new const MENU_WAR3MENU_8[]             = "\d8. Admin Options\w^n";
new const MENU_WAR3MENU_9[]             = "9. Help Topics^n^n";

new const MENU_SELECTRACE_TITLE[]       = "\ySelect Race ^n^n";

new const MENU_SELECTSKILLS_TITLE[]     = "\ySelect Skills ^n";
new const MENU_SELECTSKILLS_9[]         = "9. Help (skill info)";

new const MENU_BUYITEM_TITLE[]          = "\yBuy Item \R$   Cost ^n^n";
new const MENU_BUYITEM_9[]              = "9. Help (item info)";

new const MENU_SKILLSINFO_TITLE[]       = "\ySkills Info ^n^n\w";

new const MENU_PLAYEROPTIONS_TITLE[]    = "\yPlayer Options ^n^n\w";
new const MENU_PLAYEROPTIONS_1[]        = "1. Show Race Icons";
new const MENU_PLAYEROPTIONS_2[]        = "2. Show Health Icons";
new const MENU_PLAYEROPTIONS_3[]        = "3. Show Dispell Icons";
new const MENU_PLAYEROPTIONS_4[]        = "4. Auto-Cast on Teammates";
new const MENU_PLAYEROPTIONS_5[]        = "5. Hudmessage Position";

new const MENU_RACEOPTIONS_TITLE[]      = "\yRace Options ^n";
new const MENU_RACEOPTIONS_TITLE2[]     = "\d%s^n^n";
new const MENU_RACEOPTIONS_1[]          = "1. Reset Skills \R\d( Leve l %d+ )^n";
new const MENU_RACEOPTIONS_2[]          = "2. Reset Ultimate \R\d( Leve l %d+ )^n";
new const MENU_RACEOPTIONS_5[]          = "5. \rReset Experience^n";

new const MENU_HELPTOPICS_TITLE[]       = "\yHelp Topics ^n^n\w";
new const MENU_HELPTOPICS_1[]           = "1. Skills Help^n";
new const MENU_HELPTOPICS_2[]           = "2. Items Help^n^n";
new const MENU_HELPTOPICS_9[]           = "9. What is WAR3X?^n^n";

new const MENU_CANCEL[]                 = "0. Cancel";


/* - MOTD Windows ----------------------------------------------- */


new const WAR3X_MOTD_HELP[]             = "Warcraft 3: Expansion";
new const WAR3X_MOTD_ITEMS[]            = "Item Information";
new const WAR3X_MOTD_PLAYERSKILLS[]     = "Player Skills";
new const WAR3X_MOTD_TARGETSKILLS[]     = "Player Skills ( %s )";

new const MOTD_TS_CURRENTSKILLS[]       = "Current Skills";
new const MOTD_TS_CURRENTULTIMATE[]     = "Current Ultimate";
new const MOTD_TS_NOSKILL[]             = "(No points have been allocated to this skill.)";
new const MOTD_TS_ALLLEVELS[]           = "All Race Levels";

new const HELP_HEADER_RACIAL[]          = "<h3>Racial Ability</h3>This skill automatically increases with each level.";
new const HELP_HEADER_SKILLS[]          = "<h3>Trainable Skills</h3>These skills can be upgraded when you gain levels.";
//new const HELP_HEADER_BONUS[]           = "<h3>Bonus Ability</h3>This skill is automatically aquired at level %d, and increases with each level.";
new const HELP_HEADER_ULTIMATES[]       = "<h3>Ultimates</h3>One of these skills can be chosen at level %d+. To use an ultimate, bind a key by typing '<i>bind _KEY_ ultimate</i>' in console.";
new const HELP_HEADER_ITEMS[]           = "<h3>Overview</h3>Items below can be purchased via the Shop Menu, which is accessable from the main menu. You may also choose to bind a key by typing '<i>bind _KEY_ shopmenu</i>' in console.^n";


/* - XP Messages ------------------------------------------------ */


new const XP_KILLSTREAK_MESSAGE[]           = "* [WC3x] You have gained %d bonus XP for killing %d players in a row!";
new const XP_KILLVIP_MESSAGE[]              = "* [WC3x] You have gained %d bonus XP for killing the VIP!";
new const XP_KILLPLANTER_MESSAGE[]          = "* [WC3x] You have gained %d bonus XP for killing a player planting the bomb!";
new const XP_KILLDEFUSER_MESSAGE[]          = "* [WC3x] You have gained %d bonus XP for killing a player defusing the bomb!";
new const XP_HOSTAGE_KILLRESCUER_MESSAGE[]  = "* [WC3x] You have gained %d bonus XP for killing a player attempting to rescue hostages!";
new const XP_TEAMKILL_MESSAGE[]             = "* [WC3x] You have lost %d XP for killing a teammate!";

new const XP_HOSTAGE_TOUCH_MESSAGE[]        = "* [WC3x] You received %d XP for beginning to escort a hostage to safety!";
new const XP_HOSTAGE_SHARE_MESSAGE[]        = "* [WC3x] You receive %d XP for escorting hostages to the rescue zone!";
new const XP_KILLHOSTAGE_MESSAGE[]          = "* [WC3x] You have lost %d XP for killing too many hostages.";
new const XP_BOMBPICKUP_MESSAGE[]           = "* [WC3x] You have the bomb! You will receive %d bonus XP for planting it!";
new const XP_BOMBPLANT_MESSAGE[]            = "* [WC3x] You receive %d XP for planting the bomb!";
new const XP_BOMBPLANT_SHARE_MESSAGE[]      = "* [WC3x] You receive %d XP for supporting the bomb planting effort!";
new const XP_DEFUSE_MESSAGE[]               = "* [WC3x] You receive %d XP for defusing the bomb!";
new const XP_DEFUSE_SHARE_MESSAGE[]         = "* [WC3x] You receive %d XP for supporting the bomb defusing effort!";
new const XP_VIPSPAWN_MESSAGE[]             = "* [WC3x] You are the VIP! You will receive %d bonus XP for escaping!";
new const XP_VIPESCAPE_MESSAGE[]            = "* [WC3x] You receive %d XP for escaping to safety!";
new const XP_VIPESCAPE_SHARE_MESSAGE[]      = "* [WC3x] You receive %d XP for escorting the VIP to safety!";

new const XP_TARGETBOMBED_MESSAGE[]         = "* [WC3x] All Terrorists receive %d XP for successfully bombing the target!";
new const XP_TARGETSAVED_CT_MESSAGE[]       = "* [WC3x] All CTs receive %d XP for saving the target!";
new const XP_TARGETSAVED_T_MESSAGE[]        = "* [WC3x] You have lost %d XP for not attempting to complete your objective.";
new const XP_VIPNOTESCAPE_T_MESSAGE[]       = "* [WC3x] All Terrorists receive %d XP for preventing the VIP from escaping!";
new const XP_VIPNOTESCAPE_VIP_MESSAGE[]     = "* [WC3x] You have lost %d XP for not completing your objective.";
new const XP_HOSTAGENOTRESCUED_T_MESSAGE[]  = "* [WC3x] All Terrorists receive %d XP for preventing the hostages from being rescued!";
new const XP_HOSTAGENOTRESCUED_CT_MESSAGE[] = "* [WC3x] You have lost %d XP for not attempting to complete your objective.";
new const XP_ALLHOSTAGESRESCUED_MESSAGE[]   = "* [WC3x] All CTs receive %d XP for rescuing all hostages!";

new const XP_MAX_MESSAGE[]                  = "* [WC3x] Your XP is already at maximum!";
new const XP_MAX_BOMBPICKUP_MESSAGE[]       = "* [WC3x] You have the bomb! Help your teammates gain XP by planting it!";
new const XP_MAX_VIPSPAWN_MESSAGE[]         = "* [WC3x] You are the VIP! Help your teammates gain XP by escaping!";


/* - Upkeep Messages -------------------------------------------- */


new const UPKEEP_MESSAGE[]                  = "* [WC3x] Your team is now at %s.";


/* - Help Files ------------------------------------------------- */


new const LANGUAGE_COMMON_LEVEL[]           = "Level";
new const LANGUAGE_COMMON_RACIAL[]          = "Racial";
new const LANGUAGE_COMMON_CLASS[]           = "Class";
new const LANGUAGE_COMMON_SKILLS[]          = "Skills";

new const LANGUAGE_COMMON_ON[]              = "ON";
new const LANGUAGE_COMMON_OFF[]             = "OFF";


/* -------------------------------------------------------------- */



#endif


// ------------------------------------------------- End. - //
