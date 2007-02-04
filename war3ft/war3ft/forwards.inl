
new fwd_ReportKill = -1;

public FWD_Register()
{
	//reportKill(killer, victim, weaponIndex, weaponName); 
	//iKiller, iVictim, iWeapon, szWpnName
	fwd_ReportKill = CreateMultiForward( "wc3_reportkill", ET_IGNORE, FP_CELL, FP_CELL, FP_CELL, FP_STRING );
}

public FWD_Destroy()
{
	DestroyForward( fwd_ReportKill );
}

