GetRaceName(iRaceID, String:szRaceName[], strLen)
{
	decl String:raceLookup[8];

	Format(raceLookup, 7, "Race %i", iRaceID);
	Format(szRaceName, strLen, "%t", raceLookup);
}
