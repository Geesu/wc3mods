
//////////////////////////////////// Skill Configuration /////

// - Regeneration ( racial ) ------------------------------ //

new Float:OR_fRegeneration[2]       =           {2.8,0.8};  // (  float) Regeneration (regeneration rate)

#define REGENERATION_AMOUNT                            1    // (integer) health gained each cycle

// - Berserk ---------------------------------------------- //

new Float:OR_fBerserk_damage[3]		=       {0.3,0.8,1.2};	// (  float) Berserk (bonus % damage based on health)
new Float:OR_fBerserk_speed[3]		= {280.0,300.0,320.0};	// (  float) Max possible speed with Berserk at health 0

#define BERSERK_HEALTH				                  75	// (integer) lowest health to start giving bonus

// - Pulverize -------------------------------------------- //

new Float:OR_fPulverize_range[3]	=       {4.0,5.0,6.0};	// (  float) Pulverize (distance from detonation)
new Float:OR_fPulverize_bonus[3]	=       {1.0,2.0,3.0};	// (  float) Pulverize (bonus damage)

#define PULVERIZE_ARMOR                             0.25    // (  float) % armor removed on pulverize based on damage

new PULVERIZE_RING_RGB[3]          =        {255,255,192};  // (integer) RGB of pulverize rings at max level

// - Pillage ---------------------------------------------- //

new Float:OR_fPillage[3]            =    {0.30,0.60,0.90};  // (  float) Pillage (percent chance)

#define PILLAGE_KNIFEBONUS                           0.5    // (  float) bonus to apply to pillage chance/money stolen when using knife.
//#define PILLAGE_AMMO (will replace later)         0.30    // (  float) % damage used to calculate ammo stolen.
#define PILLAGE_AMMO                                   1    // (integer) number of ammo clips to award player.
#define PILLAGE_MONEY                                3.0    // (  float) % damage used to calculate money stolen.

enum {                                                      // (integer) Pillage type index
    INDEX_MONEY = 1,
    INDEX_AMMO,
    INDEX_GRENADE
};

// ------------------------------------------------- End. - //



///////////////////////////////// Ultimate Configuration /////

// - Healing Wave ----------------------------------------- //

#define HEALINGWAVE_HEAL                              60    // (integer) initial heal
#define HEALINGWAVE_RANGE                             12    // (integer) max range of jump targets
#define HEALINGWAVE_MULTIPLIER                         3    // (integer) jump health multiplier (1/X)
#define HEALINGWAVE_JUMPS                              6    // (integer) maximum healing wave jumps

// - Chain Lightning -------------------------------------- //

#define CHAINLIGHTNING_DAMAGE                         65    // (integer) initial damage
#define CHAINLIGHTNING_RANGE                          12    // (integer) max range of jump targets
#define CHAINLIGHTNING_MULTIPLIER                      3    // (integer) jump damage multiplier (1/X)
#define CHAINLIGHTNING_JUMPS                           6    // (integer) maximum chain lightning jumps

// - Wind Walk -------------------------------------------- //

#define WINDWALK_INVISIBILITY                         28    // (integer) visibility at peak of windwalk
#define WINDWALK_SPEED                             350.0    // (  float) max speed when windwalking
#define WINDWALK_DURATION                             10    // (integer) seconds windwalk is active
#define WINDWALK_PREWALKTIME                           1    // (integer) seconds it takes to become fully invisible
#define WINDWALK_DAMAGE                               75    // (integer) damage on successful windwalk knife attack
#define WINDWALK_FOV                                  50    // (integer) amount to pull back FOV

// ------------------------------------------------- End. - //
