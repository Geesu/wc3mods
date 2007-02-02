
new fwd_ReportKill;

public fwd_register()
{
	//reportKill(killer, victim, weaponIndex, weaponName); 
	//iKiller, iVictim, iWeapon, szWpnName
	fwd_ReportKill = CreateMultiForward( "wc3_reportkill", ET_IGNORE, FP_CELL, FP_CELL, FP_CELL, FP_STRING );
}

public fwd_destroy()
{
	DestroyForward( fwd_ReportKill );
}

