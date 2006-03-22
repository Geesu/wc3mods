/* - Skill Configuration ---------------------------------------- */


new Float:s_Fortitude[2]        = {105.0,165.0};        // (racial) Fortitude (hitpoints gained)
new Float:s_Invisibility[3]     = {0.60,0.70,0.80};     // (skill1) Invisibility (percent invisibility)
new Float:s_Bash[3]             = {0.10,0.20,0.30};     // (skill2) Bash (chance to bash)
new s_BashDamage[3]             = {3,6,9};              // (skill2) Bash (bonus damage)
new s_ifArmor[3]                = {130,160,190};        // (skill3) Inner Fire (max armor)
new Float:s_ifDamage[3]         = {0.05,0.10,0.15};     // (skill3) Inner Fire (bonus damage)


/* - Skill Constants Configuration ------------------------------ */


#define BASH_KNIFEBONUS                      0.5        // (  float) bonus to apply to bash chance/damage when using knife.
#define BASH_DURATION_MIN                    0.5        // (  float) seconds player is bashed
#define BASH_DURATION_MAX                    2.5        // (  float) seconds player is bashed

#define INVISIBILITY_PRESHIFT               10.0        // (  float) length of time before invisibility shift
#define INVISIBILITY_DURATION               45.0        // (  float) length of time to fade from max invisibility to min
#define INVISIBILITY_SHIFTS                   15        // (integer) number of invisibility level shifts
#define INVISIBILITY_COOLDOWN                3.0        // (  float) seconds of cooldown after attacking / being hit with invis

new BASH_RGB[3] =                      {96,96,96};      // (integer) RGB of bash glow shell ( when bashed )


/* - Ultimate Configuration ------------------------------------- */


#define HOLYLIGHT_DAMAGE             60     // (integer) damage
#define HOLYLIGHT_HEAL               75     // (integer) heal
#define HOLYLIGHT_BLINDTIME           2     // (integer) seconds to blind undead upon nuke

#define FLAMESTRIKE_RADIUS            2     // (integer) meters from center that blaze is active
#define FLAMESTRIKE_DAMAGEIN         16     // (integer) damage / second while in flames
#define FLAMESTRIKE_DAMAGEOUT         6     // (integer) seconds to take damage when out of flames
#define FLAMESTRIKE_DURATION       10.0     // (integer) seconds flamestrike is active
#define FLAMESTRIKE_SINGEDURATION     3     // (integer) seconds flamestrike is active

#define AVATAR_DURATION            12.0     // (  float) seconds avatar is active
#define AVATAR_STEPSIZE               5     // (integer) amount of health to give/remove each 0.1 second when growing/shrinking
#define AVATAR_SIZE                 120     // (integer) size of the glowshell on the player
#define AVATAR_HEALTH                60     // (integer) bonus health to apply
#define AVATAR_DAMAGE                 6     // (integer) bonus damage to apply
#define AVATAR_SPEED              210.0     // (  float) movement speed of player while in avatar

new AVATAR_RGB[3] =           {48,118,122};      // (integer) RGB of bash glow shell ( when bashed )
