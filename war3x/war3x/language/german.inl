/* - Items ------------------------------------------------------ */


new const ITEMNAME[_TSI + 1][] =     // ITEM NAMES:
{
    "Keins",                         // None
    "Ank der Reinkanation",        // Ankh of Reincarnation
    "Stiefel der Geschwindigkeit",               // Boots of Speed
    "Klauen des Angriffs +6",           // Claws of Attack +6
    "Mantel des Schattens",             // Cloak of Shadows
    "Sobi Maske",                    // Sobi Mask
    "Amulett des Zauber Schildes",       // Amulet of Spell Shield
    "Ring des Schutzes +5",        // Ring of Protection +5
    "Rolle der Erfahrung"            // Tome of Experience
};


/* - Races ------------------------------------------------------ */


new const RACENAME[_TR][] =         // RACE NAMES:
{
    "Untote Plage",               // Undead Scourge
    "Menschen Buendnis",               // Human Alliance
    "Orkische Horde",                 // Orcish Horde
    "Nacht-Elfen Wachen"           // Night Elf Sentinels

    /*
    "Blood Elf Refugees",           // Blood Elf Refugees
    "Naga ",                        // Naga
    "Draenei Elders"                // Draenei Elders
    */
};

new const RACENAME_SHORT[_TR][] =   // RACE NAMES (short):
{
    "Untote",                       // Undead
    "Mensch",                        // Human
    "Ork",                          // Orc
    "Nacht-Elf"                     // Night Elf

    /*
    "Blood Elf",                    // Blood Elf
    "Naga",                         // Naga
    "Draenei"                       // Draenei
    */
};

new const RACEKEYNAME[_TR][] =      // RACE NAMES (save key names):
{
    "UN",                           // Undead
    "ME",                           // Human
    "OR",                           // Orc
    "NA"                            // Night Elf

    /*
    "BE",                           // Blood Elf
    "NA",                           // Naga
    "DR"                            // Draenei
    */
};


/* - Skills ----------------------------------------------------- */


new const RACE1SKILL[_TS + _TU][] =         // UNDEAD SKILLS:
{
    "Ungeweihte Aura",                          // Racial: Unholy Aura
    "Vampir Aura",                        // Vampiric Aura
    "Anti-Magie Haut",                     // Anti-Magic Shell
    "Frost Panzerung",                          // Frost Armor
    "Todes Spule",                           // Ultimate: Death Coil
    "Aufspiesen",                               // Ultimate: Impale
    "Schlafen"                                 // Ultimate: Sleep
};

new const RACE2SKILL[_TS + _TU][] =         // HUMAN SKILLS:
{
    "Kriegsausbildung",                         // War Training
    "Unsichtbarkeit",                         // Racial: Invisibility
    "Starker Schlag",                                 // Bash
    "Inneres Feuer",                           // Devotion Aura
    "Heiliges Licht",                           // Ultimate: Holy Light
    "Flammenschlag",                         // Ultimate: Flame Strike
    "Offenbarung"                                // Ultimate: Avatar
};

new const RACE3SKILL[_TS + _TU][] =         // ORC SKILLS:
{
    "Regeneration",                         // Racial: Regeneration
    "Blutgenuss",                            // Bloodlust
    "Pluverisieren",                            // Pulverize
    "Wiedergeburt",                        // Reincarnation
    "Heilende Welle",                         // Ultimate: Healing Wave
    "Ketten-Blitz",                      // Ultimate: Chain Lightning
    "Wind-Lauf"                             // Ultimate: Wind Walk
};

new const RACE4SKILL[_TS + _TU][] =         // NIGHT ELF SKILLS:
{
    "Wesen's Segnung",                    // Racial: Nature's Blessing
    "Ausweichen",                              // Evasion
    "Mond Speer",                          // Moon Glaive
    "Trueshot Aura",                        // Trueshot Aura
    "Verjuengung",                         // Ultimate: Rejuvination
    "Umschlingende Wurtzeln",                     // Ultimate: Entangling Roots
    "Schattenschlag"                         // Ultimate: Shadow Strike
};

/* FUTURE RACES

new const RACE5SKILL[_TS + _TU][] =         // BLOOD ELF SKILLS:
{
    "Spell Immunity",                       // Racial: Spell Immunity
    "Defend",                               // Defend
    "Inner Fire",                           // Inner Fire
    "Slow",                                 // Slow
    "Heal",                                 // Ultimate: Heal
    "Flame Strike",                         // Ultimate: Flame Strike
    "Aerial Shackles"                       // Ultimate: Aerial Shackles
};

new const RACE6SKILL[_TS + _TU][] =         // NAGA SKILLS:
{
    "Spiked Shell",                         // Racial: Spiked Shell
    "Frost Arrows",                         // Frost Arrows
    "Abolish Magic",                        // Abolish Magic
    "Hardened Skin",                        // Hardened Skin
    "Mana Shield",                          // Ultimate: Mana Shield
    "Forked Lightning",                     // Ultimate: Forked Lightning
    "Parasite"                              // Ultimate: Parasite
};

new const RACE7SKILL[_TS + _TU][] =         // DRAENEI SKILLS:
{
    "Elder Invisibility",                   // Racial: Elder Invisibility
    "Critical Strike",                      // Critical Strike
    "Howl of Terror",                       // Howl of Terror
    "Liquid Fire",                          // Liquid Fire
    "Healing Wave",                         // Ultimate: Healing Wave
    "Ray of Disruption",                    // Ultimate: Ray of Disruption
    "Shadow Strike"                         // Ultimate: Shadow Strike
};

*/


/* - Skill Descriptions ----------------------------------------- */


new const RACE1SKILL_DESC[_TS + _TU][] =    // UNDEAD SKILLS:
{
    /* Unholy Aura          */  "Erhoeht die Geschwindigkeit des Spielers um <%s>. Wirkt bei allen Waffen gleich.",
    /* Vampiric Aura        */  "Spieler erhaelt <%s> des Schadens an einem Feind als Energie wieder.",
    /* Anti-Magic Shell     */  "Der Spieler ist von einer Huelle umgeben, die ihm vor <%s> aller Zaueber- /Ultimativen Zauber-Schaden schuetzt. ",
    /* Frost Armor          */  "Erstellt ein Schild aus Eis um den Spieler, wenn er eine Ruestung kauft. Das Schild fuegt <%s> zur maximalen Ruestung hinzu und man hat eine <%s> Chance einen Angreifer fuer <%s> Sekunden zu verlangsamen.",
    /* Death Coil           */  "Eine Spirale des Todes welche <%s> Schaden an lebenden feindenanrichtet, oder befreundete Untote um <%s> heilt.",
    /* Impale               */  "Spiesst einen feindlichen Player mit einem gewahltigen Hieb auf. Verursacht <%s> Schaden und und schlagt ihn hoch in die Luft und verursacht nochmals einen Fallschaden.",
    /* Sleep                */  "Ein feindlicher Spieler wird fuer <%s> Sekunden in einen Schlaf versetzt. Ein sclafender Spieler kann werder angegriffen noch selbst angreifen."
};

new const RACE2SKILL_DESC[_TS + _TU][] =    // HUMAN SKILLS:
{
    /* War Training         */  "Erhoert die Energie des Spielers um <%s>.",
    /* Invisibility         */  "Der Spieler ist <%s> weniger sichtbar wenn er ein Messer traegt. Having weapons equipped lowers this value, slightly. Wenn der Spieler angreift, angeriffen wird, oder einen Ultimativen Zuaber auspricht, wird er sichtbar fuer alle. Also ist der Spieler Aktionelos wenn er Unsichtbar ist, ausser er benutzt das Messer zum angreifen.",
    /* Bash                 */  "Es gibt eine <%s> Chance, dass die Attacke <%s> Bonus Schaden anrichtet und den Gegner fuer <%s> Sekunden betaeubt.",
    /* Devotion Aura        */  //"Der Spieler bekommt <%s> zusaetzliche Panzerung und er startet mit <%s> Ruestung jede Runde.",
    /* Inner Fire           */  "Gibt den Spieler <%s> zusaetzliche Ruestung und erhoert den Schaden um <%s> wenn die Ruestung noch besteht.",
    /* Holy Light           */  "Ein heiliges Licht, dass Mitspieler <%s> Energie heilen kann, oder <%s> Schaden an einem feindlichen Untoten Spieler anrichtet.",
    /* Flame Strike         */  "Beschwoert einen Feuer's Kranz, der Spieler brennen laesst und <%s> Schaden die Sekunde verrichtet. Sobald ein Spieler den Feuer's Kranz verlaesst, erhaelt er noch <%s> Schaden pro Sekunde fuer <%s> Sekunden. Der Feuer's Kranz besteht <%s> Sekunden.",
    /* Avatar               */  "Zeitweilig erhoet sich die Energie des Spieler's um <%s>, und gibt ihm eine Zauber-Immunitaet und einen Schadens Bonus von <%s> an allen Angreifern fuer <%s> Sekunden."
};

new const RACE3SKILL_DESC[_TS + _TU][] =    // ORC SKILLS:
{
    /* Troll Regeneration   */  "Der Spieler regeneriert <%s> Energie/Panzerung alle <%s> Sekunde(n).",
    /* Bloodlust            */  "Erhoet die Laufgeschwindigkeit des Spieler, wenn er das Messer oder eine HE-Granate benutzt um <%s>, und der Schaden erhoet sich um <%s> bei einem erfolgreichem Angriff.",
    /* Pulverize            */  "Gibt einem Spieler eine Chance von <%s> einen Flaechen Effect zu verurschaen wenn sie angegriffen werden von <%s>. Die Entfernung betraegt <%s> Meter. Der Schaden wirkt sich doppelt aus, wenn der Gegner / die Gegner <%s> Meter entfernt sind.",
    /* Reincarnation        */  "Wenn der Spieler stirbt, hat er eine <%s> Chance mit allen Items beim Ableben wieder zu starten.",
    /* Healing Wave         */  "Laesst eine Heilende welle nach Vorne schiessen, die <%s> mal aufspringt. Diese gibt <%s> Energie dem ersten Spieler (Ziel). Jeder weitere Bonus ist weniger als <%s> Energie.",
    /* Chain Lightning      */  "Der Spieler schleudert einen Blitz auf seinen Gegner, der <%s> Schaden verrichtet. Der Blitz kann bis zu <%s> nahe stehenden Gegner verletzen. Jeder weiter sprung auf einen Gegner erzeugt einen kleiner Schaden von <%s>.",
    /* Wind Walk            */  "Man wird <%s> unsichtbarer und die Bewegungen sind <%s> schneller fuer <%s> Sekunden. Ein Angriff mit einem Messer gibt zusaetzlichen Schaden von <%s> , wenn der Spruch noch aktive ist. Das wechseln von Waffen, beendet den Spruch/Zauber."
};

new const RACE4SKILL_DESC[_TS + _TU][] =    // NIGHT ELF SKILLS:
{
    /* Nature's Blessing    */  "Erhoeht die Panzerung des Spielers um <%s> un erhoeht die Laufgeschwindigkeit mit einer (various?) Waffe um <%s> , wenn die Panzerung noch besteht. De Laufgeschwindigkeit mit allen Waffen ist nicht groesser als die mit dem Messer.",
    /* Evasion              */  "Es besteht eine <%s> Chance, dass der Spieler den Angriff vollkommen vermeiden kann.",
    /* Moon Glaive          */  "Gives the player a <%s> chance to strike an additional enemy player for <%s> of the initial damage within <%s> meters of the original target.",
    /* Trueshot Aura        */  "Gibt dem Spieler <%s> Schadens-Bonus bei angriffen mit einer Feuerwaffe.",
    /* Rejuvination         */  "Heilt einen gezielten freundliche Spieler <%s> Energie ueber <%s> Sekunden.",
    /* Entangling Roots     */  "Leasst Wurzel aus dem Boden wachsen, die einen Spieler gefangen halten fuer <%s> Sekunden und einen Schaden von <%s> verursachen.",
    /* Shadow Strike        */  "Schleudert einen vergiften Dolch zu dem feindlichen Spieler. Der Anfangsschaden ist <%s> und zusaetzlich <%s> Schaden ueber <%s> Sekunden. Die Laufgeschwindigkeit des Feindes wird um <%s> verringert."
};


/* - Classes ( based on level/ultimate ) ------------------------ */


new const ITEM_DESC[_TSI + 1][] =
{
    /* Item: None             */  "No Description",
    /* Reincarnation          */  "When killed, the player has a <%s> chance to spawn the next round with all items he died with.",
    /* Boots of Speed         */  "Increases the player's maximum speed by <%s>. All weapons run at the same speed.",
    /* Claws of Attack +6"    */  "Adds <%s> damage to each successful attack.",
    /* Cloak of Shadows"      */  "Makes the player <%s> invisible. If the player attacks, is attacked, or casts an ultimate, he will become visible.",
    /* Sobi Mask"             */  "Decreases ultimate cooldown of the player by <%s> seconds after every cast.",
    /* Amulet of Spell Shield */  "Blocks a harmful ultimate that an enemy casts on the player once every <%s> seconds.",
    /* Ring of Protection     */  "Player receives <%s> armor, and increases their max armor by the same ammount if armor is present.",
    /* Tome of Experience"    */  "Player instantly receives <%s> experience. Item is expended upon purchase, and does not require an item slot."
};


/* - Classes ( based on level/ultimate ) ------------------------ */


new const RACE1CLASS[_TC][] =       // UNDEAD CLASSES:
{
    "Messdiener",                      // Acolyte          (Level 0)
    "Ghoul",                        // Ghoul            (Level 2)
    "Abscheulichkeit",                  // Abomination      (Level 5)
    "Todesritter",                 // Death Knight     (Ultimate: Death Coil)
    "Gruft Lord",                   // Crypt Lord       (Ultimate: Impale)
    "Furcht Lord"                    // Dread Lord       (Ultimate: Sleep)
};

new const RACE2CLASS[_TC][] =       // HUMAN CLASSES:
{
    "Bauer",                      // Peasant          (Level 0)
    "Lakai",                      // Footman          (Level 2)
    "Ritter",                       // Knight           (Level 5)
    "Paladin",                      // Paladin          (Ultimate: Holy Light)
    "Blut Magier",                   // Blood Mage       (Ultimate: Flame Strike)
    "Berg Koenig"                 // Mountain King    (Ultimate: Avatar)
};

new const RACE3CLASS[_TC][] =       // ORC CLASSES:
{
    "Arbeitssklave",                         // Peon             (Level 0)
    "Grunzer",                        // Grunt            (Level 2)
    "Taure",                       // Tauren           (Level 5)
    "Schatten Jaeger",                // Shadow Hunter    (Ultimate: Healing Wave)
    "Hellseher",                     // Far Seer         (Ultimate: Chain Lightning)
    "Schwertmeister"                   // Blademaster      (Ultimate: Wind Walk)
};

new const RACE4CLASS[_TC][] =       // NIGHT ELF CLASSES:
{
    "Irrlicht",                         // Wisp             (Level 0)
    "Bogenschuetze",                       // Archer           (Level 2)
    "Jaeger",                       // Hunter           (Level 5)
    "Druide",                        // Druid            (Ultimate: Rejuvination)
    "Hueter",                       // Keeper           (Ultimate: Entangling Roots)
    "Waechter"                        // Warden           (Ultimate: Shadow Strike)
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

new const CANT_TARGET[]                 = "Nicht im Stande den Spieler zu markieren.";
new const CANT_TARGET_SELF[]            = "Nicht im Stande dich selbst zu markieren.";
new const CANT_TARGET_HOLYLIGHT[]       = "Du musst befreundete lebende Spieler markieren oder feindliche Untoten Spieler.";
new const CANT_TARGET_DEATHCOIL[]       = "Du musst befreundete Unotetn Spieler makieren oder lebende feindliche Spieler.";
new const CANT_TARGET_AIR[]             = "Dein Ziel muss auf dem Boden sein.";
new const CANT_TARGET_SLEEP[]           = "Nicht im Stande schlafende Spieler zu makieren.";
new const CANT_TARGET_INVIS[]           = "Nicht im Stande unsichtbare Spieler zu makieren.";

new const FULLHEALTH_TARGET[]           = "Das Ziel ist bei bester Gesundheit.";
new const FULLHEALTH_SELF[]             = "Du bist bei bester Gesundheit";

new const HEAL_TARGET[]                 = "%s von deiner Energie %d wurden wiederhergestellt.";
new const HEAL_SELF[]                   = "Du hast %d von deiner Energie wiederhergestellt.";

new const BLOCK_ULTIMATE_C[]            = "Dein Ultimativer Zauber wurde blockiert.";
new const BLOCK_ULTIMATE_T[]            = "Du hast einen Ultimativen Zauber blockiert.";

new const CAST_HOLYLIGHT[]              = "%s hat Heiliges Licht auf dich gezaubert. Du hast %d Schaden bekommen.";
new const CAST_FLAMESTRIKE[]            = "%s beschwoerte einen Feuer's Kreis auf dich.";
new const CAST_CHAINLIGHTNING[]         = "%s hat einen Ketten-Blitz auf dich gezaubert. Du hast %d Schaden bekommen.";
new const CAST_CHAINLIGHTNING_JUMP[]    = "%s's Chain Lightning has jumped to you. Du hast %d Schaden bekommen.";
new const CAST_ROOTS[]                  = "Du wurdest verwurzelt von %s.";
new const CAST_SHADOWSTRIKE[]           = "%s's Schattenschlag hat dich uebrwaeltigt you.";
new const CAST_SLEEP[]                  = "%s hat dich in einen Schlaf versetzt.";
new const CAST_WINDWALK[]               = "Dein Ueberraschungsangriff verursachte %d Schaden.";
new const CAST_AVATAR[]                 = "Dein Koerper nimmt an Kraft zu!";

new const FINISH_SLEEP[]                = "Du bist nicht laenger im Schlaf.";
new const FINISH_WINDWALK[]             = "Dein Lauf ist beendet.";
new const FINISH_AVATAR[]               = "Du kommst zu alter Staerke zurueck.";

new const DAMAGE_MOONGLAIVE[]           = "Ein Mond Speer traff dich mit %d Schaden.";
new const DAMAGE_PULVERIZE[]            = "Du wurdest pulverisiert, erhaltener Schaden: %d .";
new const DAMAGE_ANTIMAGICSHELL[]       = "Deine Haut hat %d Schaden absorbiert.";
new const DAMAGE_EVASION[]              = "Du hast erfolgreich einen Schaden von %d vermieden.";

new const REJUVINATE_SELF[]             = "Du zauberst Verjuengen auf dich selbst.";
new const REJUVINATE_TARGET[]           = "%s hat Verjuengen auf dich gezaubert.";
new const REJUVINATE_ALREADY[]          = "Das Ziel besitzt bereits den Zauebr Verjuengung.";

new const ARMOR_MESSAGE_UD[]            = "Ein Schild aus Eis/Frost umgibt dich.";
new const ARMOR_MESSAGE_HU[]            = "Ein Inneres Feuer brennt in dir.";
new const ARMOR_MESSAGE_OR[]            = "Du und deine Panzerung seit eins.";
new const ARMOR_MESSAGE_NE[]            = "Du wurdest von der Natur gesegnet.";

new const ARMOR_MESSAGE_FADE_UD[]       = "Dein Eis/frost Panzerung ist verblast.";
new const ARMOR_MESSAGE_FADE_HU[]       = "Dein Inneres feuer ist verblasst.";
new const ARMOR_MESSAGE_FADE_OR[]       = "Deine Panzerung verblasst.";
new const ARMOR_MESSAGE_FADE_NE[]       = "Du fuehlst dich nicht mehr gesegnet.";

new const DISPELL_SHADOWSTRIKE[]        = "%s hat deinen Schatten-Schlag vermieden.";
new const DISPELL_ROOTS[]               = "%s hat deine umschliegenden Wurzel vermieden.";
new const DISPELL_GENERIC[]             = "Alle negativen Auswirkungen auf dich sind vorbei.";

new const ULTIMATE_READY[]              = "Der Ultimative zauber ist bereit.";
new const ULTIMATE_NOTREADY[]           = "Der Ultimative zauber ist nicht bereit.";
new const ULTIMATE_NOTREADY_COOLDOWN[]  = "Der Ultimative zauber ist nicht bereit.^n(%d sekunden verbleiben)";
new const ULTIMATE_NOTFOUND[]           = "Du hast keinen Ultimativen Zauber.";
new const ULTIMATE_RESTRICTED[]         = "Der Ultimative Zauber ist auf der Map tapu.";
new const ULTIMATE_NOCAST_VIP[]         = "VIP's koennen keinen Ultimativen zauber ausprechen!";
new const ULTIMATE_NOCAST_PISTOL[]      = "Du kannst keine Ultimativen Zauber in Pistolen-Runden einzusetzen.";
new const ULTIMATE_NOCAST_AIR[]         = "Du musst auf dem Boden stehen, um Ultimativen Zauber einzusetzen.";
new const ULTIMATE_NOCAST_SLEEP[]       = "Du kannst waerend du schlaefst keine Ultimativen zauber aussprechen.";

new const INFO_GAINLEVEL[]              = "Du bist nun ein Level hoeher! Du hast Level %d erreicht!";
new const INFO_NEWRACENEXTROUND[]       = "Du startest naechste Runde als %s.";
new const INFO_NEWRACENOW[]             = "Du aenderst deine Rasse als %s...";
new const INFO_NOSKILLPOINTS[]          = "Du hast bereits alle Skill-Punkte eingesetzt!";

new const INFO_RESTRICTEDMAP_HUD[]      = "Current Map is restricted to Short-Term XP.";

new const INFO_FOLLOW_NORACE[]          = "Keine Rasse zur Zeit ausgewaehlt";
new const INFO_FOLLOW_MESSAGE_1[]       = "%s Level %i   XP:%i/%i (%i%s)";
new const INFO_FOLLOW_MESSAGE_2[]       = "Item: %s   Rucksack: %s";
new const INFO_FOLLOW_MESSAGE_3[]       = "Energie: %i   Panzerung: %i   Geld: %i";


/* - Chat Messages ---------------------------------------------- */


new const ITEM_INSUFFICIENT_FUNDS[]     = "Du hast nicht genug Geld!";
new const ITEM_EQUIPPED[]               = "Du besitzt das Item schon!";
new const ITEM_UNAVAILABLE[]            = "Item nicht verfuegbar";
new const ITEM_NOBONUS[]                = "Item gibt keinen Bonus mehr!";
new const ITEM_NOARMOR[]                = "Du musst erst eine Panzerung kaufen!";
new const ITEM_NOMASK[]                 = "Du hast keinen Ultimativen Zauber!";
new const ITEM_DROP[]                   = "Das Item wurde fallen gelassen!";
new const ITEM_PICKUP[]                 = "Das Item traegst du nun!";
new const ITEM_NODROP[]                 = "Du traegst kein Item!";

new const ITEM_ANKH_MESSAGE[]           = "* [WC3x] Du hast eine Chnace von %d%s das du naechste Runde deine Ausruestung wieder bekommst. (%s)";
new const ITEM_ANKH_PRIMARY[]           = "primary";
new const ITEM_ANKH_SECONDARY[]         = ", sidearm";
new const ITEM_ANKH_GRENADES[]          = ", grenades";
new const ITEM_ANKH_NOITEMS[]           = "Keine Items vorhanden!";

new const ITEM_BOOTS_MESSAGE[]          = "* [WC3x] Dein Schnelligkeit is von %0.1f auf %0.1f angestiegen.";
new const ITEM_CLAWS_MESSAGE[]          = "* [WC3x] Alle deine Angreifer erhalten einen zusaetzlichen Schaden von %d .";
new const ITEM_CLOAK_MESSAGE[]          = "* [WC3x] You are now less visible by %0.0f%s, and untargetable by ultimates when invisible.";
new const ITEM_MASK_MESSAGE[]           = "* [WC3x] Deine Zeitspanne zum einsetzen von Ultimativen Zauber ist von %0.1f sek. auf %0.1f gefallen.";
new const ITEM_AMULET_MESSAGE[]         = "* [WC3x] Du kannst jetzt Ultimative Zauber alle %0.0f sekunden blockieren.";
new const ITEM_RING_MESSAGE[]           = "* [WC3x] Dein Maximum an Panzerung ist auf %d gestiegen.";
new const ITEM_TOME_MESSAGE[]           = "* You gain %d experience from the tome. *";
new const ITEM_BUYBACK_MESSAGE[]        = "* [WC3x] Du verkaufst dein Item: %s fuer $%d!";

new const VIP_ANKH_MESSAGE[]            = "* [WC3x] Deine Waffe wirst du wieder bekommen, wenn du icht mehr VIP bist.";
new const VIP_NOSKILLS[]                = "* [WC3x] The VIP does not have access to skills or ultimates.";

new const INFO_SHOP_DISABLED[]          = "Der Server hat die Shopmenu's gesperrt!";
new const INFO_SHOP_BUYZONE[]           = "Du musst in der Kauf-Zone seien!";

new const INFO_CURRENTSKILLS[]          = "* [WC3x] Aktuelle Skills: %s (%0.0f%s)%s";
new const INFO_SKILLDETAILS[]           = "* [WC3x] Schreibe '/skills' in den Chat um mehr Informationen ueber Skills zu bekommen.";

new const INFO_RESETSKILLS[]            = "* [WC3x] Deine Skills wurden erfolgreich zurueck gesetzt.";
new const INFO_RESETULTIMATE[]          = "* [WC3x] Dein Ultimativer Zauber wurde erfolgreich zurueck gesetzt.";
new const INFO_RESETXP[]                = "* [WC3x] Deine Erfahrung von %s wurde erfolgreich zurueck gesetzt.";

new const INFO_RESTRICTEDMAP_CHAT[]     = "* [WC3x] Current map is restricted to Short-Term XP. Don't Worry, your Long-Term XP has not been deleted.";
new const INFO_DISABLEDMAP_CHAT[]       = "* [WC3x] Warcraft 3: Expansion ist auf dieser Map deaktiviert.";
new const INFO_GENERATEHELP_CHAT[]      = "* [WC3x] Hilfe Dateien wurden erstellt.";

new const XP_SUPPORT_HEALMSG[]          = "* [WC3x] Du erhaelst %d Erfahrung fuer die Wiederherstellung von %d Energie an deinen Team-Spieler(n).";
new const XP_SUPPORT_HEALMSG_MAX[]      = "* [WC3x] Du hast %d Energie an deinen Team-Spieler(n) wiederhergestellt.";
//new const XP_SUPPORT_DISPELLMSG[]     = "* [WC3x] Du erhaelst %d Erfahrung fuer das entfernen eines negativen Effektes %d an deine(n/m) Team-Spieler(n).";


/* - Enable / Disable ------------------------------------------- */

// CONSOLE

new const WAR3X_FORMAT[]               = "Nicht korrektes Format - gebrauche: warcraft3 < on | off >";
new const WAR3_enableD[]               = "* [WC3x] Warcraft 3: Expansion ist aktiviert.";
new const WAR3_disableD[]              = "* [WC3x] Warcraft 3: Expansion ist deaktiviert.";
new const WAR3_enableD_ALREADY[]       = "* [WC3x] Warcraft 3: Expansion ist bereits aktiviert.";
new const WAR3_disableD_ALREADY[]      = "* [WC3x] Warcraft 3: Expansion ist bereits deaktiviert.";

// HUD

new const WAR3_enableD_HUD[]           = "Warcraft 3: Expansion wurde aktiviert^nRunde wird neu gestartet";
new const WAR3_disableD_HUD[]          = "Warcraft 3: Expansion wurde deaktiviert^nRunde wird neu gestartet";

// CHAT

new const WAR3X_VOTE_NOVOTE[]           = "* [WC3x] Abstimmungen sind gerade deaktiviert.";
new const WAR3X_VOTE_ALREADY[]          = "* [WC3x] Es ist bereits eine Abstimmung am laufen.";
new const WAR3X_VOTE_NOTALLOWED[]       = "* [WC3x] Abstimmungen sind gerade nicht erlaubt.";
new const WAR3X_WAR3_vote_startED[]     = "* [WC3x] %s startet Warcraft 3: Expansion Abstimmung...";

new const WAR3X_VOTE_FOR[]              = "fuer";
new const WAR3X_VOTE_AGAINST[]          = "dagegen";

new const WAR3X_VOTE_ENABLED[]          = "Aktiviert.";
new const WAR3X_VOTE_DISABLED[]         = "Deaktiviert.";
new const WAR3X_VOTE_REMAINENABLED[]    = "Remains enabled.";
new const WAR3X_VOTE_REMAINDISABLED[]   = "Remains disabled.";

new const WAR3X_VOTE_FAILED[]           = "* [WC3x] Abstimmung fehlgeschlagen (ja '%d')|(nnein '%d')|(gebraucht '%.2f')";
new const WAR3X_VOTE_SUCCESS[]          = "* [WC3x] Abstimmung erfolgreich (ja '%d')|(nein '%d')|(gebraucht '%.2f')";
new const WAR3X_VOTE_TIE[]              = "* [WC3x] Abstimmung binden (ja '%d')|(nein '%d')";


/* - Menus ------------------------------------------------------ */

// MENU SYMBOLS KEY:
// \y = YELLOW, \w = WHITE, \d = GREY, \r = RED, \R = RIGHT ALIGN, ^n = NEW LINE

new const MENU_VOTE_DISABLE[]           = "\yDeaktivere Warcraft3: Expansion?\w^n^n1.  Ja^n2.  Nein";
new const MENU_VOTE_ENABLE[]            = "\yAktiviere Warcraft3: Expansion?\w^n^n1.  Ja^n2.  Nein";

new const MENU_WAR3MENU_TITLE[]         = "\yWarcraft 3 Menu ^n^n\w";
new const MENU_WAR3MENU_1[]             = "1. Rasse waehlen^n";
new const MENU_WAR3MENU_2[]             = "2. Skills waehlen^n";
new const MENU_WAR3MENU_3[]             = "3. Items kaufen^n";
new const MENU_WAR3MENU_4[]             = "4. Zeige Level/Skills^n";
new const MENU_WAR3MENU_5[]             = "5. Spieler Skills^n";
new const MENU_WAR3MENU_6[]             = "6. Spieler Optionen^n";
new const MENU_WAR3MENU_7[]             = "7. Rassen Optionen^n";
new const MENU_WAR3MENU_8[]             = "\d8. Admin Optionen\w^n";
new const MENU_WAR3MENU_9[]             = "9. Hilfe^n^n";

new const MENU_SELECTRACE_TITLE[]       = "\yRasse Waehlen ^n^n";

new const MENU_SELECTSKILLS_TITLE[]     = "\ySkills waehlen ^n";
new const MENU_SELECTSKILLS_9[]         = "9. Hilfe (Skill Info)";

new const MENU_BUYITEM_TITLE[]          = "\yItems kaufen \R$   Kosten ^n^n";
new const MENU_BUYITEM_9[]              = "9. Hilfe (Item Info)";

new const MENU_SKILLSINFO_TITLE[]       = "\ySkills Info ^n^n\w";

new const MENU_PLAYEROPTIONS_TITLE[]    = "\ySpieler Optionen ^n^n\w";
new const MENU_PLAYEROPTIONS_1[]        = "1. Ziege Rassen Symbole";
new const MENU_PLAYEROPTIONS_2[]        = "2. Zeige Energie Symbole";
new const MENU_PLAYEROPTIONS_3[]        = "3. Zeige Dispell Symbole";
new const MENU_PLAYEROPTIONS_4[]        = "4. Autom. Zauber auf Teamspieler";
new const MENU_PLAYEROPTIONS_5[]        = "5. Hudmessage Position";

new const MENU_RACEOPTIONS_TITLE[]      = "\yRassen Optionen ^n";
new const MENU_RACEOPTIONS_TITLE2[]     = "\d%s^n^n";
new const MENU_RACEOPTIONS_1[]          = "1. Skills zurueck setzen \R\d( Leve l %d+ )^n";
new const MENU_RACEOPTIONS_2[]          = "2. Ultimate Zuaber zurueck setzen \R\d( Leve l %d+ )^n";
new const MENU_RACEOPTIONS_5[]          = "5. \rErfahrung zurueck setzen^n";

new const MENU_HELPTOPICS_TITLE[]       = "\yHilfe ^n^n\w";
new const MENU_HELPTOPICS_1[]           = "1. Hilfe zu den Skills^n";
new const MENU_HELPTOPICS_2[]           = "2. Hilfe zu den Items^n^n";
new const MENU_HELPTOPICS_9[]           = "9. Was ist WAR3X?^n^n";

new const MENU_CANCEL[]                 = "0. Abbrechen";


/* - MOTD Windows ----------------------------------------------- */


new const WAR3X_MOTD_HELP[]             = "Warcraft 3: Expansion";
new const WAR3X_MOTD_ITEMS[]            = "Item Informationen";
new const WAR3X_MOTD_PLAYERSKILLS[]     = "Spieler Skills";
new const WAR3X_MOTD_TARGETSKILLS[]     = "Spieler Skills ( %s )";

new const MOTD_TS_CURRENTSKILLS[]       = "Aktuelle Skills";
new const MOTD_TS_CURRENTULTIMATE[]     = "Aktuelle Ultimative Zauber";
new const MOTD_TS_NOSKILL[]             = "(Keine Punkte wurdem dem Skill zugewiesen.)";

new const HELP_HEADER_RACIAL[]          = "<h3>Rassen Faehigkeit</h3>Dieser Skill steigt automatisch mit jedem Level.";
new const HELP_HEADER_SKILLS[]          = "<h3>Trainierbare Skills</h3>Dieser Skill kann durch das steigen im Level trainiert werden.";
//new const HELP_HEADER_BONUS[]         = "<h3>Zusatz Faehigkeit</h3>Dieser Skills eignet man sich automatisch bei Level %d an und steigt mit jedem Level.";
new const HELP_HEADER_ULTIMATES[]       = "<h3>Ultimative Zauber</h3>Einer dieser Skills kann mit Level %d+ ausgewaehlt werden. Um einen ultimativen zauebr zu benutzen, schreibe in die Console '<i>bind Taste ultimate</i>' .";
new const HELP_HEADER_ITEMS[]           = "<h3>Uebersicht</h3>Man kann Items kaufen in einem Shop, den man auch im war3menu findet (schreibe einfach war3menu). Du kannst auch eine Taste belegen, schreibe dazu in die Console '<i>bind Taste shopmenu</i>' .^n";


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


/* - Help Files ------------------------------------------------- */


new const LANGUAGE_COMMON_LEVEL[]           = "Level";
new const LANGUAGE_COMMON_RACIAL[]          = "Rasse";
new const LANGUAGE_COMMON_CLASS[]           = "Klasse";
new const LANGUAGE_COMMON_SKILLS[]          = "Skills";

new const LANGUAGE_COMMON_ON[]              = "AN";
new const LANGUAGE_COMMON_OFF[]             = "AUS";


/* -------------------------------------------------------------- */