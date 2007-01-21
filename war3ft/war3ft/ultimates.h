// For Icon events
#define ICON_HIDE 0 
#define ICON_SHOW 1 
#define ICON_FLASH 2 

new g_UltimateIcons[8][32] = 
					{
						"dmg_rad",				// Undead
						"item_longjump",		// Human Alliance
						"dmg_shock",			// Orcish Horde
						"item_healthkit",		// Night Elf
						"dmg_heat",				// Blood Mage
						"suit_full",			// Shadow Hunter
						"cross",				// Warden
						"dmg_gas"				// Crypt Lord
					};

new g_ULT_iLastIconShown[33];				// Stores what the last icon shown was for each user (race #)

