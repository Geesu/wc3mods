// Amount of XP required to level
new g_XPToLevel[MAX_LEVELS+1] = {0,150,300,600,1000,1500,2100,2800,3400,4500,5500};
new g_XPAwardedByLevel[MAX_LEVELS+1] = {10,15,25,35,40,50,60,70,80,90,95};


XP_Award( client, xp )
{
	g_PlayerXP[client] += xp;

	// Check to see if they should gain a level!
}
