
//////////////////////////////////// Skill Configuration /////

// - Unholy Aura ( racial ) ------------------------------- //

new Float:UD_fUnholyAura_speed[2]   = {SPEED_KNIFE,300.0};  // (  float) Unholy Aura (260.0 = knife speed)
new Float:UD_fUnholyAura_gravity[2] =          {1.0,0.75};  // (  float) Unholy Aura (gravity percentage)

// - Vampiric Aura ---------------------------------------- //

new Float:UD_fVampiricAura[3]       =    {0.15,0.30,0.45};  // (  float) Vampiric Aura (health percent)

#define VAMPIRIC_KNIFEBONUS                         0.50    // (  float) Bonus to apply to vampiric aura when using knife.

// - Frost Nova ------------------------------------------- //

new UD_iFrostNova_damage[3]         =           {6,12,18};  // (integer) Frost Nova (damage dealt)
new UD_iFrostNova_range[3]          =             {6,7,8};  // (integer) Frost Nova (range of blast)

new FROSTNOVA_RGB[3]                =          {35,63,93};  // (integer) RGB of frost nova shell ( when slowed )

#define FROSTNOVA_SLOWSPEED                         60.0    // (  float) max speed when slowed by frost nova
#define FROSTNOVA_SLOWDURATION_MIN                   1.0    // (  float) min duration in seconds player is slowed by frost nova ( minimum = 1 second )
#define FROSTNOVA_SLOWDURATION_MAX                   6.0    // (  float) max duration in seconds player is slowed by frost nova ( minimum = 1 second )

// - Frost Armor ------------------------------------------ //

new UD_iFrostArmor_armor[3]         =       {110,120,130};  // (integer) Frost Armor (armor amount)
new Float:UD_fFrostArmor_slow[3]    =       {0.1,0.2,0.3};  // (  float) Frost Armor (chance to slow)

new FROSTARMOR_RGB[3]               =          {23,42,62};  // (integer) RGB of frost slow shell ( when slowed )

#define FROSTARMOR_ARMOR                              15    // (integer) armor removed when player slowed with frost armor
#define FROSTARMOR_SLOWSPEED                       150.0    // (  float) max speed when slowed by frost armor
#define FROSTARMOR_SLOWDURATION                      3.0    // (  float) seconds player is slowed by frost armor

// ------------------------------------------------- End. - //



///////////////////////////////// Ultimate Configuration /////

// - Death Coil ------------------------------------------- //

#define DEATHCOIL_HEAL                                75    // (integer) amount of health to give friendly undead target
#define DEATHCOIL_DAMAGE                              60    // (integer) amount of damage to deal living enemy target
#define DEATHCOIL_VELOCITY                         800.0    // (  float) velocity of death coil towards target
#define DEATHCOIL_DURATION                           3.0    // (  float) life of deathcoil before removal

new DEATHCOIL_SHELL_RGB[3]          =         {160,255,0};  // (integer) RGB of glow shell over skull
new DEATHCOIL_TRAIL_RGB[3]          =          {96,224,0};  // (integer) RGB of beamfollow trail from skull

// - Impale ----------------------------------------------- //

#define IMPALE_DAMAGE                                 15    // (integer) initial impale damage
#define IMPALE_ARMORDAMAGE                            20    // (integer) initial impale damage
#define IMPALE_VELOCITY                              630    // (integer) upward velocity (620 ~ 30 damage)
#define IMPALE_MINDISTANCE                           200    // (integer) any impale that doesnt span this distance will assume ceiling collision
#define IMPALE_COLLISIONDAMAGE                        20    // (integer) damage dealt for collision with ceiling

// - Sleep ------------------------------------------------ //

#define SLEEP_DURATION_MIN                           8.0    // (  float) min duration player is disabled
#define SLEEP_DURATION_MAX                          12.0    // (  float) max duration player is disabled
#define SLEEP_MOVETIME                               0.8    // (  float) duration after sleep is done that player is unattackable

// ------------------------------------------------- End. - //
