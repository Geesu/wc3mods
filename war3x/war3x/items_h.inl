

/* - Reference Constants ---------------------------------------- */


#define VALUE_ANKH                  1.0     // (  float) % chance to gain items
#define VALUE_BOOTS               275.0     // (  float) max speed value		 - moved to globals
#define VALUE_CLAWS                   6     // (integer) extra damage on each attack
#define VALUE_CLOAK                0.60     // (  float) invisibility percentage
#define VALUE_MASK                 20.0     // (  float) seconds removed from ultimate cooldown
#define VALUE_AMULET                  1     // (integer) number of ultimates that can be blocked
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
    800,            // Amulet of Shielding
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
    {255,255,0},    // Amulet of Shielding      ( YELLOW )
    {0,255,0},      // Ring of Protection       (  GREEN )
    {128,128,128}   // Tome of Experience       (   NONE )
};

#define ITEM_DROPRATE              0.66     // (  float) percentage to drop items on death
#define ITEM_DROPLIFE              30.0     // (  float) seconds dropped items stay on ground

#define CLOAK_COOLDOWN              5.0     // (  float) cooldown for cloak of invisibility

new AMULET_RGB[3] =         {124,103,60};   // (integer) rgb of glowshell when amulet takes effect
