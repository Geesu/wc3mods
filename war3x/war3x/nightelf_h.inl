
/* - Skill Configuration ---------------------------------------- */


new Float:s_ElunesMagic[2]      = {0.05,0.25};                  // (racial) Elune's Grace (ultimate damage absorbsion) {level 0,level LEVEL_RACIALCAP}
new Float:s_ElunesKnife[2]      = {0.10,0.35};                  // (racial) Elune's Grace (knife damage absorbsion) {level 0,level LEVEL_RACIALCAP}
new Float:s_Evasion[3]          = {0.10,0.20,0.30};             // (skill1) Evasion (chance to evade)
new s_BlessingArmor[3]          = {115,130,145};                // (skill2) Nature's Blessing (armor)
new Float:s_BlessingSpeed[3]    = {0.1,0.2,0.3};                // (racial) Nature's Blessing (min/max speed attainable) (used to calculate % speed bonus)
new Float:s_TrueshotAura[3]     = {0.15,0.30,0.45};             // (skill3) Trueshot Aura (bonus damage)


/* - Skill Constants Configuration ------------------------------ */


#define ELUNES_NIGHTBONUS           2.0     // (  float) bonus to regular skill during night time (via moonstone)

#define EVASION_MAXDAMAGE           250     // (integer) maximum damage (ish) avoidable with evasion
#define EVASION_SHOTGAP             5.0     // (  float) min duration between evades before a consecutive shot may be evaded

#define BLESSING_MAXSPEED         260.0     // (  float) max speed of nature's blessing

new ELUNES_SHELL_RGB[3] =     {80,32,96};   // (integer) RGB of elune's grace shell ( when damage absorbed )


/* - Ultimate Configuration ------------------------------------- */


#define ROOT_DURATION               8.0     // (  float) seconds player will be rooted
#define ROOT_MAXDAMAGE               40     // (integer) total damage taken over time for duration of root

#define REJUVENATION_MAXHEALTH       75     // (integer) health gained per wave
#define REJUVENATION_WAVES            5     // (integer) number of waves
#define REJUVENATION_DURATION      16.0     // (  float) duration of effect

#define SHADOWSTRIKE_DURATION        10
#define SHADOWSTRIKE_DAMAGE          35
#define SHADOWSTRIKE_ARMOR           15
#define SHADOWSTRIKE_DOT             20
#define SHADOWSTRIKE_SPEED        120.0
