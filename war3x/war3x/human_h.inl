
//////////////////////////////////// Skill Configuration /////

// - Fortitude ( racial ) --------------------------------- //

new Float:HU_fFortitude[2]          =       {106.0,160.0};  // (  float) Fortitude (hitpoints gained)

// - Invisibility ----------------------------------------- //

new Float:HU_fInvisibility[3]       =    {0.60,0.70,0.80};  // (  float) Invisibility (percent invisibility)

#define INVISIBILITY_PRESHIFT                       10.0    // (  float) length of time before invisibility shift
#define INVISIBILITY_DURATION                       45.0    // (  float) length of time to fade from max invisibility to min
#define INVISIBILITY_SHIFTS                           15    // (integer) number of invisibility level shifts
#define INVISIBILITY_COOLDOWN                        3.0    // (  float) seconds of cooldown after attacking / being hit with invis

// - Bash ------------------------------------------------- //

new Float:HU_fBash[3]               =    {0.05,0.15,0.25};  // (  float) Bash (chance to bash)
new HU_iBashDamage[3]               =             {3,6,9};  // (integer) Bash (bonus damage)

#define BASH_KNIFEBONUS                              0.5    // (  float) bonus to apply to bash chance/damage when using knife.
#define BASH_DURATION_MIN                            0.5    // (  float) seconds player is bashed
#define BASH_DURATION_MAX                            2.5    // (  float) seconds player is bashed

new BASH_RGB[3]                     =          {96,96,96};  // (integer) RGB of bash glow shell ( when bashed )

// - Inner Fire ------------------------------------------- //

new HU_iInnerFire_armor[3]          =       {130,160,190};  // (integer) Inner Fire (max armor)
new Float:HU_fInnerFire_damage[3]   =    {0.05,0.10,0.15};  // (  float) Inner Fire (bonus damage)

// ------------------------------------------------- End. - //



///////////////////////////////// Ultimate Configuration /////

// - Holy Light ------------------------------------------- //

#define HOLYLIGHT_DAMAGE                              60    // (integer) damage
#define HOLYLIGHT_HEAL                                75    // (integer) heal
#define HOLYLIGHT_BLINDTIME                            2    // (integer) seconds to blind undead upon nuke

// - Flame Strike ----------------------------------------- //

#define FLAMESTRIKE_RADIUS                             2    // (integer) meters from center that blaze is active
#define FLAMESTRIKE_DAMAGEIN                          16    // (integer) damage / second while in flames
#define FLAMESTRIKE_DAMAGEOUT                          6    // (integer) seconds to take damage when out of flames
#define FLAMESTRIKE_DURATION                        10.0    // (integer) seconds flamestrike is active
#define FLAMESTRIKE_SINGEDURATION                      3    // (integer) seconds flamestrike is active

// - Avatar ----------------------------------------------- //

#define AVATAR_DURATION                             12.0    // (  float) seconds avatar is active
#define AVATAR_STEPSIZE                                5    // (integer) amount of health to give/remove each 0.1 second when growing/shrinking
#define AVATAR_SIZE                                  120    // (integer) size of the glowshell on the player
#define AVATAR_HEALTH                                 60    // (integer) bonus health to apply
#define AVATAR_DAMAGE                                  6    // (integer) bonus damage to apply
#define AVATAR_SPEED                               210.0    // (  float) movement speed of player while in avatar

new AVATAR_RGB[3]                   =        {48,118,122};  // (integer) RGB of bash glow shell ( when bashed )

// ------------------------------------------------- End. - //