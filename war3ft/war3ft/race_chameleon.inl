/*´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.
*	Race: Chameleon Functions
´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.´¯`·.¸¸.*/

public CHAM_Randomize()
{
	if ( get_pcvar_num( CVAR_wc3_races ) == 9 && get_pcvar_num( CVAR_wc3_cham_random ) )
	{
		new myintvallocal = 0;
		// loop through all four skill options (3 + ultimate) pick a new race at random and update the skill
		while ( myintvallocal < 4 )
		{
			g_ChamSkills[myintvallocal + 1] = random_num( 1, 8 ); 
			++myintvallocal;
		}
	} 
}
