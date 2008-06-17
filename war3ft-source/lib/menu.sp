public Handler_SelectMenu(Handle:menu, MenuAction:action, param1, param2)
{
	switch (action)
	{
		case MenuAction_Select:
		{
			// Set the user's race
			g_PlayerCurrentRace[param1] = param2+1;

			decl String:race[64];		

			GetRaceName(g_PlayerCurrentRace[param1], race, 63);
		}
	}
}

BuildMenuChangeRace()
{
	if (g_MenuChangeRace != INVALID_HANDLE)
	{
		CloseHandle(g_MenuChangeRace);
		g_MenuChangeRace = INVALID_HANDLE;
	}
	
	g_MenuChangeRace = CreateMenu(Handler_SelectMenu);
	SetMenuTitle(g_MenuChangeRace, "%s", "Select Race");

	decl String:race[64];		
	for (new i = 1; i < 5; i++)
	{
		GetRaceName(i, race, 63);

		AddMenuItem(g_MenuChangeRace, race, race);
	}
	
	SetMenuExitButton(g_MenuChangeRace, true);
}
