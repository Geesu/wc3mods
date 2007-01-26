

new fwd_ReportKill
public fwd_register()
{
	//reportKill(killer, victim, weaponIndex, weaponName); 
	CreateMultiForward( "fwd_ReportKill", ET_IGNORE, FP_CELL, FP_CEL, FP_CELL, FP_STRING );


}

public fwd_reportKill(