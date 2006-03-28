
////////////////////////////////// General Configuration /////

// - Reference Values ------------------------------------- //

#define ITEM_NONE                                      0    // Item reference
#define ITEM_ANKH                                      1
#define ITEM_BOOTS                                     2
#define ITEM_CLAWS                                     3
#define ITEM_CLOAK                                     4
#define ITEM_MASK                                      5
#define ITEM_AMULET                                    6
#define ITEM_RING                                      7
#define ITEM_TOME                                      8

// - Item Constants --------------------------------------- //

#define ITEM_DROPRATE                               0.66    // (  float) percentage to drop items on death
#define ITEM_DROPLIFE                               30.0    // (  float) seconds dropped items stay on ground
#define ITEM_BUYBACK                                   2    // (integer) money given back when purchasing item and one present (1/X)

// - Item Prices ------------------------------------------ //

new const ITEM_COST[_TSI + 1] =
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

// - Item Colors ( when dropped ) ------------------------- //

new const ITEM_COLOR[_TSI + 1][3] =
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

// ------------------------------------------------- End. - //



///////////////////////////////////// Item Configuration /////

// - Ankh of Reincarnation -------------------------------- //

#define ITEM_ANKH_VALUE                              1.0    // (  float) % chance to gain items
#define ITEM_ANKH_AMMO                                 2    // (integer) number of clips to give each weapon on reincarnation

// - Boots of Speed --------------------------------------- //

#define ITEM_BOOTS_VALUE                           275.0    // (  float) max speed value

// - Claws of Attack +6 ----------------------------------- //

#define ITEM_CLAWS_VALUE                               6    // (integer) extra damage on each attack

// - Cloak of Invisibility -------------------------------- //

#define ITEM_CLOAK_VALUE                            0.60    // (  float) invisibility percentage

#define ITEM_CLOAK_COOLDOWN                          5.0    // (  float) cooldown for cloak of invisibility

// - Sobi Mask -------------------------------------------- //

#define ITEM_MASK_VALUE                             20.0    // (  float) seconds removed from ultimate cooldown

// - Amulet of Shielding ---------------------------------- //

#define ITEM_AMULET_VALUE                              1    // (integer) number of ultimates that can be blocked

new ITEM_AMULET_RGB[3]              =        {124,103,60};  // (integer) rgb of glowshell when amulet takes effect

// - Ring of Protection +5 -------------------------------- //

#define ITEM_RING_VALUE                               50    // (integer) extra armor

// - Tome of Experience ----------------------------------- //

#define ITEM_TOME_VALUE                               25    // (integer) experience

// ------------------------------------------------- End. - //


