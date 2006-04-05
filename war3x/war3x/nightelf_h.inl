
//////////////////////////////////// Skill Configuration /////

// - Elune's Grace ( racial	) -----------------------------	//

new	Float:NE_fElunes_magic[2]		=		  {0.05,0.25};	// (  float) Elune's Grace (ultimate damage	absorbsion)	{level 0,level LEVEL_RACIALCAP}
new	Float:NE_fElunes_knife[2]		=		  {0.10,0.35};	// (  float) Elune's Grace (knife damage absorbsion) {level	0,level	LEVEL_RACIALCAP}

#define	ELUNES_NIGHTBONUS							 2.0	// (  float) bonus to regular skill	during night time (via moonstone)

new	ELUNES_SHELL_RGB[3]				=		   {80,32,96};	// (integer) RGB of	elune's	grace shell	( when damage absorbed )

// - Evasion ----------------------------------------------	//

new	Float:NE_fEvasion[3]			=	 {0.10,0.20,0.30};	// (  float) Evasion (chance to	evade)

#define	EVASION_MAXDAMAGE							 250	// (integer) maximum damage	(ish) avoidable	with evasion
#define	EVASION_SHOTGAP								 5.0	// (  float) min duration between evades before	a consecutive shot may be evaded

// - Nature's Blessing ------------------------------------	//

new	NE_iBlessing_armor[3]			=		{115,130,145};	// (integer) Nature's Blessing (armor)
new	Float:NE_fBlessing_speed[3]		=		{0.1,0.2,0.3};	// (  float) Nature's Blessing (min/max	speed attainable) (used	to calculate % speed bonus)

#define	BLESSING_MAXSPEED						   260.0	// (  float) max speed of nature's blessing

// - Trueshot Aura ----------------------------------------	//

new	Float:NE_fTrueshotAura[3]		=	 {0.15,0.30,0.45};	// (  float) Trueshot Aura (bonus damage)

// ------------------------------------------------- End. -	//



///////////////////////////////// Ultimate Configuration /////

// - Entangling	Roots -------------------------------------	//

#define	ROOT_DURATION								 8.0	// (  float) seconds player	will be	rooted
#define	ROOT_MAXDAMAGE								  40	// (integer) total damage taken	over time for duration of root

// - Rejuvenation -----------------------------------------	//

#define	REJUVENATION_MAXHEALTH						  75	// (integer) health	gained per wave
#define	REJUVENATION_WAVES							   5	// (integer) number	of waves
#define	REJUVENATION_DURATION						16.0	// (  float) duration of effect

// - Shadow	Strike ----------------------------------------	//

#define	SHADOWSTRIKE_DURATION						  10	// (integer) duration of damage	over time
#define	SHADOWSTRIKE_DAMAGE							  35	// (integer) initial damage
#define	SHADOWSTRIKE_ARMOR							  15	// (integer) amount	of armor to	remove from	target
#define	SHADOWSTRIKE_DOT							  20	// (integer) damage	to apply over SHADOWSTRIKE_DURATION
#define	SHADOWSTRIKE_SPEED						   120.0	// (  float) speed to set player to	when struck

// ------------------------------------------------- End. -	//
