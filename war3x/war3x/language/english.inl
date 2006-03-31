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


/* - Menus ------------------------------------------------------ */

// MENU SYMBOLS KEY:
// \y = YELLOW, \w = WHITE, \d = GREY, \r = RED, \R = RIGHT ALIGN, ^n = NEW LINE

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

new const MENU_SKILLSINFO_TITLE[]       = "\ySkills Info ^n^n\w";

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


#endif

// ------------------------------------------------- End. - //
