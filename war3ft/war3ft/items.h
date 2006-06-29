

// Item Defines
#define MAX_SHOPMENU_ITEMS  18
#define MAX_PLAYER_ITEMS    2
#define MAX_PAGE_ITEMS      9
#define ITEM_SLOT_ONE       0
#define ITEM_SLOT_TWO       1

#define ITEM_NONE          -1
#define ITEM_ANKH		    0
#define ITEM_BOOTS		    1
#define ITEM_CLAWS		    2
#define ITEM_CLOAK		    3
#define ITEM_MASK		    4
#define ITEM_NECKLACE	    5
#define ITEM_FROST		    6
#define ITEM_HEALTH		    7
#define ITEM_TOME		    8
#define ITEM_SCROLL		    9
#define ITEM_PROTECTANT     10
#define ITEM_HELM		    11
#define ITEM_AMULET		    12
#define ITEM_SOCK		    13
#define ITEM_GLOVES		    14
#define ITEM_RING		    15
#define ITEM_CHAMELEON	    16
#define ITEM_MOLE		    17



new g_iShopMenuItems[33][MAX_PLAYER_ITEMS];			// Holds the player's current items
new g_iMultipleItems[33][2];						// Holds the number of the same item - i.e. rings
new g_iFutureShopMenu[33];							// This will hold which shopmenu we should display after the user has selected an item to remove

new bool:g_bPlayerBoughtAnkh[33];							// Set to true when a user buys an ankh
new bool:g_bPlayerBoughtMole[33];							// Set to true when a user buys a mole

new ITEM_COST[MAX_SHOPMENU_ITEMS] = {0};