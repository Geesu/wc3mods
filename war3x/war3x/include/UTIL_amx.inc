
/* - AMX Mod Include -------------------------------------------- *

 *  UTIL_amx
 *  (c) Copyright 2003-2004, Created by Ryan
 *
 *  A collection of useful functions that can be used in all amx
 *  script files. These functions are 'stock', so they will not
 *  be compiled unless used.
 *
 *
 *  Revision v1.0:
 *  --------------
 *
 *      Create_TE_PLAYERATTACHMENT()
 *      Remove_TE_PLAYERATTACHMENT( id )
 *      Create_TE_ELIGHT()
 *      Remove_TE_ELIGHT( id )
 *      Create_ScreenFade( )
 *      Remove_ScreenFade( id )
 *
 *  Enjoy!

 * -------------------------------------------------------------- */


#if defined _util_amx_inc
	#endinput
#endif
#define _util_amx_inc


/* -------------------------------------------------------------- */


#define MAX_PLAYERS             32

#define FADE_OUT                0x0000          // Fade out from max alpha
#define FADE_IN                 0x0001          // Fade in to max alpha
#define FADE_MODULATE_OUT       0x0002          // Modulate, fade out from max alpha
#define FADE_MODULATE_IN        0x0003      // Modulate, fade in to max alpha
#define FADE_ONLYONE_OUT        0x0004      // Start at max Alpha, no fade out
#define FADE_ONLYONE_IN         0x0005      // Fade in to max Alpha, no fade out
#define FADE_MODONE_OUT         0x0006      // Modulate, Start at max Alpha, no fade
#define FADE_MODONE_IN          0x0007      // Modulate, Fade in to max Alpha, no fade out

#define HUDMESSAGE_FX_FADEIN    0           // hudmessage effect types
#define HUDMESSAGE_FX_FLICKER   1
#define HUDMESSAGE_FX_WRITEOUT  2

#define ICON_HIDE               0           // StatusIcon
#define ICON_SHOW               1
#define ICON_FLASH              2

#define DURATION_1_SECOND       (1<<12)     // Used in

#define SHOWTO_ALL_BROADCAST    MAX_PLAYERS + 1     // unreliable to all
#define SHOWTO_ALL              MAX_PLAYERS + 2     // reliable to all
#define SHOWTO_ALL_PVS          MAX_PLAYERS + 3     // Ents in PVS of org
#define SHOWTO_ALL_PAS          MAX_PLAYERS + 4     // Ents in PAS of org
#define SHOWTO_ALL_PVS_R        MAX_PLAYERS + 5     // Reliable to PVS
#define SHOWTO_ALL_PAS_R        MAX_PLAYERS + 6     // Reliable to PAS

#define SEENBY_ALL              0

#define TE_BOUNCE_NULL          0           // Temp entity bounce sound types
#define TE_BOUNCE_SHELL         1
#define TE_BOUNCE_SHOTSHELL     2

#define TE_EXPLFLAG_NONE            0       // all flags clear makes default Half-Life explosion
#define TE_EXPLFLAG_NOADDITIVE      1       // sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite)
#define TE_EXPLFLAG_NODLIGHTS       2       // do not render dynamic lights
#define TE_EXPLFLAG_NOSOUND         4       // do not play client explosion sound
#define TE_EXPLFLAG_NOPARTICLES     8       // do not draw particles

#define _X                          0
#define _Y                          1

#define TOTAL_RESOLUTIONS           7

new const STEAM_RESOLUTION[TOTAL_RESOLUTIONS][2] =
{
	{640,480},
	{720,576},
	{800,600},
	{1024,768},
	{1152,864},
	{1200,960},
	{1280,1024}
};


/* -------------------------------------------------------------- */


static MSG_TYPE( Type )
{
	switch ( Type )
	{
		case SHOWTO_ALL_BROADCAST:  return MSG_BROADCAST;
		case SHOWTO_ALL:            return MSG_ALL;
		case SHOWTO_ALL_PVS:        return MSG_PVS;
		case SHOWTO_ALL_PAS:        return MSG_PAS;
		case SHOWTO_ALL_PVS_R:      return MSG_PVS_R;
		case SHOWTO_ALL_PAS_R:      return MSG_PAS_R;
	}

	return PLUGIN_HANDLED;
}


/* -------------------------------------------------------------- */


// Kill Player Attachments

stock Remove_TE_PLAYERATTACHMENT( SeenBy,id )
{
	if ( SeenBy == SEENBY_ALL )
	{
		message_begin( MSG_ALL,SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( SeenBy ) || !g_bPlayerConnected[SeenBy] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE, SVC_TEMPENTITY,{0,0,0},SeenBy );
		}
	}

	write_byte( 125 );                  // TE_KILLPLAYERATTACHMENTS (125)
	write_byte( id );                   // entity index of player
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_PLAYERATTACHMENT( ShowTo,id,Offset,ModelIndex,Duration )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 124 );                  // TE_PLAYERATTACHMENT (124)
	write_byte( id );                   // entity index of player
	write_coord( Offset );              // vertical offset
	write_short( ModelIndex );          // model index
	write_short( Duration );            // life in 0.1's
	message_end();

	return PLUGIN_HANDLED;
}


stock Remove_TE_ELIGHT( SeenBy,id )
{
	if ( SeenBy == SEENBY_ALL )
	{
		message_begin( MSG_ALL,SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( SeenBy ) || !g_bPlayerConnected[SeenBy] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE, SVC_TEMPENTITY,{0,0,0},SeenBy );
		}
	}

	new Origin[3];
	get_user_origin( id,Origin );

	write_byte( 28 );                   // TE_ELIGHT (28)
	write_short( id );                  // entity:attachment to follow
	write_coord( Origin[0] );           // initial position (X)
	write_coord( Origin[1] );           // initial position (Y)
	write_coord( Origin[2] );           // initial position (Z)
	write_coord( 150 );                 // radius
	write_byte( 1 );                    // color (red)
	write_byte( 1 );                    // color (green)
	write_byte( 1 );                    // color (blue)
	write_byte( 1 );                    // life in 0.1's
	write_coord( 0 );                   // decay rate
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_ELIGHT( ShowTo,id,Radius,Red,Green,Blue,Life,Decay )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	new Origin[3];
	get_user_origin( id,Origin );
	Origin[2] += 75;

	write_byte( 28 );                   // TE_ELIGHT (28)
	write_short( id );                  // entity:attachment to follow
	write_coord( Origin[0] );           // initial position (X)
	write_coord( Origin[1] );           // initial position (Y)
	write_coord( Origin[2] );           // initial position (Z)
	write_coord( Radius );              // radius
	write_byte( Red );                  // color (red)
	write_byte( Green );                // color (green)
	write_byte( Blue );                 // color (blue)
	write_byte( Life );                 // life in 0.1's
	write_coord( Decay );               // decay rate
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_ScreenFade( ShowTo,Duration,HoldTime,FadeType,Red,Green,Blue,Alpha )
{
	new gmsgScreenFade = get_user_msgid( "ScreenFade" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgScreenFade );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgScreenFade,{0,0,0},ShowTo );
		}
	}

	write_short( Duration );            // fade lasts this long duration
	write_short( HoldTime );            // fade lasts this long hold time
	write_short( FadeType );            // fade type
	write_byte( Red );                  // fade red
	write_byte( Green );                // fade green
	write_byte( Blue );                 // fade blue
	write_byte( Alpha );                // fade alpha
	message_end();

	return PLUGIN_HANDLED;
}


stock Remove_ScreenFade( SeenBy )
{
	new gmsgScreenFade = get_user_msgid( "ScreenFade" );

	if ( SeenBy == SEENBY_ALL )
	{
		message_begin( MSG_ALL,gmsgScreenFade );
	}

	else
	{
		if ( is_user_bot( SeenBy ) || !g_bPlayerConnected[SeenBy] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgScreenFade,{0,0,0},SeenBy );
		}
	}

	write_short( 1 );                   // fade lasts this long duration
	write_short( 1 );                   // fade lasts this long hold time
	write_short( 0 );                   // fade type
	write_byte( 1 );                    // fade red
	write_byte( 1 );                    // fade green
	write_byte( 1 );                    // fade blue
	write_byte( 1 );                    // fade alpha
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_Damage( ShowTo,DamageSave,DamageTake,DamageType )
{
	new gmsgDamage = get_user_msgid( "Damage" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgDamage );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgDamage,{0,0,0},ShowTo );
		}
	}

	new Origin[3];
	get_user_origin( ShowTo,Origin );

	write_byte( DamageSave );           // damage save (armor)
	write_byte( DamageTake );           // damage take (health)
	write_long( DamageType );           // damage type
	write_coord( Origin[0] );           // damage origin (X)
	write_coord( Origin[1] );           // damage origin (Y)
	write_coord( Origin[2] );           // damage origin (Z)
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_DeathMsg( ShowTo,KillerId,VictimId,headshot,WeaponName[] )
{
	new gmsgDeathMsg = get_user_msgid( "DeathMsg" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgDeathMsg );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgDeathMsg,{0,0,0},ShowTo );
		}
	}

	write_byte( KillerId );             // killer index
	write_byte( VictimId );             // victim index
	write_byte( headshot );             // headshot? (1|0)
	write_string( WeaponName );         // weapon name
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_ScoreInfo( ShowTo,id,Frags,Deaths,PlayerClass,Team )
{
	new gmsgScoreInfo = get_user_msgid( "ScoreInfo" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgScoreInfo );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgScoreInfo,{0,0,0},ShowTo );
		}
	}

	write_byte( id );                   // recipient id
	write_short( Frags );               // recipient frags
	write_short( Deaths );              // recipient deaths
	write_short( PlayerClass );         // recipient class (TFC)
	write_short( Team );                // recipient team
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_SetFOV( ShowTo, iAmmount )
{
	new gmsgSetFOV = get_user_msgid( "SetFOV" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL, gmsgSetFOV );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE, gmsgSetFOV, {0,0,0}, ShowTo );
		}
	}

	write_byte( iAmmount );             // Ammount
	message_end();

	return PLUGIN_HANDLED;
}


stock Remove_SetFOV( ShowTo )
{
	new gmsgSetFOV = get_user_msgid( "SetFOV" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL, gmsgSetFOV );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE, gmsgSetFOV, {0,0,0}, ShowTo );
		}
	}

	write_byte( 90 );                   // Ammount
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_BEAMFOLLOW( ShowTo,Entity,SpriteIndex,Life,Width,Red,Green,Blue,Brightness )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 22 );                   // TE_BEAMFOLLOW (22)
	write_short( Entity );              // entity:attachment to follow
	write_short( SpriteIndex );         // sprite index
	write_byte( Life );                 // life in 0.1's
	write_byte( Width );                // line width in 0.1's
	write_byte( Red );                  // color (red)
	write_byte( Green );                // color (green)
	write_byte( Blue );                 // color (blue)
	write_byte( Brightness );           // brightness
	message_end();

	return PLUGIN_HANDLED;
}

stock Remove_TE_BEAMFOLLOW( SeenBy, id )
{
	if ( SeenBy == SEENBY_ALL )
	{
		message_begin( MSG_ALL,SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( SeenBy ) || !g_bPlayerConnected[SeenBy] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE, SVC_TEMPENTITY,{0,0,0},SeenBy );
		}
	}

	write_byte( 99 );                   // TE_KILLBEAM (99)
	write_short( id );                  // entity
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_BarTime( ShowTo,Duration )
{
	new gmsgBarTime = get_user_msgid( "BarTime" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgBarTime );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgBarTime,{0,0,0},ShowTo );
		}
	}

	write_byte( Duration );             // bar lasts this long duration
	write_byte( 0 );                    // unknown
	message_end();

	return PLUGIN_HANDLED;
}


stock Remove_BarTime( SeenBy )
{
	new gmsgBarTime = get_user_msgid("BarTime");

	if ( SeenBy == SEENBY_ALL )
	{
		message_begin( MSG_ALL,gmsgBarTime );
	}

	else
	{
		if ( is_user_bot( SeenBy ) || !g_bPlayerConnected[SeenBy] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgBarTime,{0,0,0},SeenBy );
		}
	}

	write_byte( 0 );                    // bar lasts this long duration
	write_byte( 0 );                    // unknown
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_MODEL( ShowTo,Origin[3],Velocity[3],Yaw,ModelIndex,Sound,Life )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 106 );                  // TE_MODEL
	write_coord( Origin[0] );           // initial position (X)
	write_coord( Origin[1] );           // initial position (Y)
	write_coord( Origin[2] );           // initial position (Z)
	write_coord( Velocity[0] );         // velocity (X)
	write_coord( Velocity[1] );         // velocity (Y)
	write_coord( Velocity[2] );         // velocity (Z)
	write_angle( Yaw );                 // initial yaw
	write_short( ModelIndex );          // model index
	write_byte( Sound );                // bounce sound type
	write_byte( Life );                 // life in 0.1's
	message_end();

	return PLUGIN_HANDLED;
}

stock Create_TE_BLOODSTREAM( ShowTo,Origin[3],Vector[3],Color,Speed )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 101 );                  // TE_BLOODSTREAM
	write_coord( Origin[0] );           // start position (X)
	write_coord( Origin[1] );           // start position (Y)
	write_coord( Origin[2] );           // start position (Z)
	write_coord( Vector[0] );           // spray vector (X)
	write_coord( Vector[1] );           // spray vector (Y)
	write_coord( Vector[2] );           // spray vector (Z)
	write_byte( Color );                // color
	write_byte( Speed );                // speed
	message_end();

	return PLUGIN_HANDLED;
}

stock Create_TE_WORLDDECAL( ShowTo,Origin[3],DecalIndex )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 116 );                  // TE_WORLDDECAL
	write_coord( Origin[0] );           // decal position (X)
	write_coord( Origin[1] );           // decal position (Y)
	write_coord( Origin[2] );           // decal position (Z)
	write_byte( DecalIndex );           // texture index of precached decal texture name
	message_end();

	return PLUGIN_HANDLED;
}

stock Create_TE_BLOODSPRITE( ShowTo,Origin[3],SpriteIndex1,SpriteIndex2,Color,Scale )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 115 );                  // TE_BLOODSPRITE
	write_coord( Origin[0] );           // position (X)
	write_coord( Origin[1] );           // position (Y)
	write_coord( Origin[2] );           // position (Z)
	write_short( SpriteIndex1 );        // sprite1 index
	write_short( SpriteIndex2 );        // sprite2 index
	write_byte( Color );                // color
	write_byte( Scale );                // scale
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_BEAMENTS( ShowTo,StartEntity,EndEntity,SpriteIndex,StartingFrame,FrameRate,Life,Width,Amplitude,Red,Green,Blue,Brightness,Speed )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 8 );                    // TE_BEAMENTS
	write_short( StartEntity );         // start entity
	write_short( EndEntity );           // end entity
	write_short( SpriteIndex );         // sprite index
	write_byte( StartingFrame );        // starting frame
	write_byte( FrameRate );            // frame rate in 0.1's
	write_byte( Life );                 // life in 0.1's
	write_byte( Width );                // line width in 0.1's
	write_byte( Amplitude );            // noise amplitude in 0.01's
	write_byte( Red );                  // color (red)
	write_byte( Green );                // color (green)
	write_byte( Blue );                 // color (blue)
	write_byte( Brightness );           // brightness
	write_byte( Speed );                // scroll speed in 0.1's
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_StatusIcon( ShowTo,Status,SpriteName[],Red,Green,Blue )
{
	new gmsgStatusIcon = get_user_msgid( "StatusIcon" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgStatusIcon );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgStatusIcon,{0,0,0},ShowTo );
		}
	}

	write_byte( Status );               // display status
	write_string( SpriteName );         // sprite name to display
	write_byte( Red );                  // color (red)
	write_byte( Green );                // color (green)
	write_byte( Blue );                 // color (blue)
	message_end();

	return PLUGIN_HANDLED;
}


stock Remove_StatusIcon( SeenBy,SpriteName[] )
{
	new gmsgStatusIcon = get_user_msgid( "StatusIcon" );

	if ( SeenBy == SEENBY_ALL )
	{
		message_begin( MSG_ALL,gmsgStatusIcon );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgStatusIcon,{0,0,0},SeenBy );
		}
	}

	write_byte( ICON_HIDE );            // display status
	write_string( SpriteName );         // sprite name to display
	write_byte( 0 );                    // color (red)
	write_byte( 0 );                    // color (green)
	write_byte( 0 );                    // color (blue)
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_BEAMCYLINDER( ShowTo,Origin[3],Axis[3],SpriteIndex,StartFrame,FrameRate,Life,Width,Amplitude,Red,Green,Blue,Brightness,Speed )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 21 );                   // TE_BEAMCYLINDER
	write_coord( Origin[0] );           // center position (X)
	write_coord( Origin[1] );           // center position (Y)
	write_coord( Origin[2] );           // center position (Z)
	write_coord( Origin[0] + Axis[0] ); // axis and radius (X)
	write_coord( Origin[1] + Axis[1] ); // axis and radius (Y)
	write_coord( Origin[2] + Axis[2] ); // axis and radius (Z)
	write_short( SpriteIndex );         // sprite index
	write_byte( StartFrame );           // starting frame
	write_byte( FrameRate );            // frame rate in 0.1's
	write_byte( Life );                 // life in 0.1's
	write_byte( Width );                // line width in 0.1's
	write_byte( Amplitude );            // noise amplitude in 0.01's
	write_byte( Red );                  // color (red)
	write_byte( Green );                // color (green)
	write_byte( Blue );                 // color (blue)
	write_byte( Brightness );           // brightness
	write_byte( Speed );                // scroll speed in 0.1's
	message_end();

	return PLUGIN_HANDLED;
}

stock Create_TE_SPRITE( ShowTo,Origin[3],SpriteIndex,Scale,Brightness )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 17 );                   // TE_SPRITE (17)
	write_coord( Origin[0] );           // position (X)
	write_coord( Origin[1] );           // position (Y)
	write_coord( Origin[2] );           // position (Z)
	write_short( SpriteIndex );         // sprite index
	write_byte( Scale );                // scale in 0.1's
	write_byte( Brightness );           // brightness
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_BEAMPOINTS( ShowTo,Origin1[3],Origin2[3],SpriteIndex,StartFrame,FrameRate,Life,Width,Amplitude,Red,Green,Blue,Brightness,Speed )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 0 );                    // TE_BEAMPOINTS
	write_coord( Origin1[0] );          // start position (X)
	write_coord( Origin1[1] );          // start position (Y)
	write_coord( Origin1[2] );          // start position (Z)
	write_coord( Origin2[0] );          // end position (X)
	write_coord( Origin2[1] );          // end position (Y)
	write_coord( Origin2[2] );          // end position (Z)
	write_short( SpriteIndex );         // sprite index
	write_byte( StartFrame );           // starting frame
	write_byte( FrameRate );            // frame rate in 0.1's
	write_byte( Life );                 // life in 0.1's
	write_byte( Width );                // width in 0.1's
	write_byte( Amplitude );            // noise amplitude in 0.01's
	write_byte( Red );                  // color (red)
	write_byte( Green );                // color (green)
	write_byte( Blue );                 // color (blue)
	write_byte( Brightness );           // brightness
	write_byte( Speed );                // scroll speed in 0.1's
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_SPARKS( ShowTo,Origin[3] )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 9 );                    // TE_SPARKS
	write_coord( Origin[0] );          // start position (X)
	write_coord( Origin[1] );          // start position (Y)
	write_coord( Origin[2] );          // start position (Z)
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_SMOKE( ShowTo,Origin[3],SpriteIndex,Scale,FrameRate )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 5 );                    // TE_SMOKE
	write_coord( Origin[0] );           // position (X)
	write_coord( Origin[1] );           // position (Y)
	write_coord( Origin[2] );           // position (Z)
	write_short( SpriteIndex );         // sprite index
	write_byte( Scale );                // scale in 0.1's
	write_byte( FrameRate );            // framerate
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_PROJECTILE( ShowTo,Origin[3],Velocity[3],ModelIndex,Life,Owner )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 119 );                  // TE_PROJECTILE
	write_coord( Origin[0] );           // position (X)
	write_coord( Origin[1] );           // position (Y)
	write_coord( Origin[2] );           // position (Z)
	write_coord( Velocity[0] );         // velocity (X)
	write_coord( Velocity[1] );         // velocity (Y)
	write_coord( Velocity[2] );         // velocity (Z)
	write_short( ModelIndex );          // model index
	write_byte( Life );                 // life in 0.1's
	write_byte( Owner );                // owner
	message_end();

	return PLUGIN_HANDLED;
}

stock Create_TE_IMPLOSION( ShowTo, Origin[3], Radius, Count, Life )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 14 );                   // TE_IMPLOSION
	write_coord( Origin[0] );           // position (X)
	write_coord( Origin[1] );           // position (Y)
	write_coord( Origin[2] );           // position (Z)
	write_byte ( Radius );              // radius
	write_byte ( Count );               // count
	write_byte ( Life );                // life in 0.1's
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_EXPLODEMODEL( ShowTo, Origin[3], Velocity, ModelIndex, Count, Life )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 107 );                  // TE_EXPLODEMODEL (107)
	write_coord( Origin[0] );           // origin (X)
	write_coord( Origin[1] );           // origin (Y)
	write_coord( Origin[2] );           // origin (Z)
	write_coord( Velocity );            // velocity
	write_short( ModelIndex );          // model index
	write_short( Count );               // count
	write_byte( Life );                 // life in 0.1's
	message_end();

	return PLUGIN_HANDLED;
}

stock Create_TE_EXPLOSION( ShowTo, Origin[3], SpriteIndex, Scale, FrameRate, Flags )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 3 );                    // TE_EXPLOSION
	write_coord( Origin[0] );           // position (X)
	write_coord( Origin[1] );           // position (Y)
	write_coord( Origin[2] );           // position (Z)
	write_short( SpriteIndex );         // sprite index
	write_byte( Scale );                // scale in 0.1's
	write_byte( FrameRate );            // framerate
	write_byte( Flags );                // flags
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TE_SPRITETRAIL( ShowTo, Start[3], End[3], SpriteIndex, Count, Life, Scale, Velocity, Random )
{
	if ( ShowTo > MAX_PLAYERS )         // Show to all ( w/ specified type )
	{
		message_begin( MSG_TYPE( ShowTo ),SVC_TEMPENTITY );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,SVC_TEMPENTITY,{0,0,0},ShowTo );
		}
	}

	write_byte( 15 );                   // TE_SPRITETRAIL (15)
	write_coord( Start[0] );            // start position (X)
	write_coord( Start[1] );            // start position (Y)
	write_coord( Start[2] );            // start position (Z)
	write_coord( End[0] );              // end position (X)
	write_coord( End[1] );              // end position (Y)
	write_coord( End[2] );              // end position (Z)
	write_short( SpriteIndex );         // sprite index
	write_byte( Count );                // count
	write_byte( Life );                 // life in 0.1's
	write_byte( Scale );                // scale in 0.1's
	write_byte( Velocity );             // velocity along vector in 10's
	write_byte( Random );               // randomness of velocity in 10's
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_StatusText( ShowTo, LineNumber, StatusText[] )
{
	new gmsgStatusText = get_user_msgid( "StatusText" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL, gmsgStatusText );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE, gmsgStatusText, {0,0,0}, ShowTo );
		}
	}

	write_byte( LineNumber );           // line number of status bar text
	write_string( StatusText );         // status bar text
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_ScreenShake( ShowTo,Amplitude,Duration,Frequency )
{
	new gmsgShake = get_user_msgid( "ScreenShake" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgShake );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgShake,{0,0,0},ShowTo );
		}
	}

	write_short( Amplitude );           // amplitude
	write_short( Duration );            // duration
	write_short( Frequency );           // frequency
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_HideWeapon( ShowTo,Ammount )
{
	new gmsgHideWeapon = get_user_msgid( "HideWeapon" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgHideWeapon );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgHideWeapon,{0,0,0},ShowTo );
		}
	}

	write_byte( Ammount );              // Hide Ammount
	message_end();

	return PLUGIN_HANDLED;
}


stock Create_TextMsg( ShowTo,Destination,Message[64] )
{
	new gmsgTextMsg = get_user_msgid( "TextMsg" );

	if ( ShowTo > MAX_PLAYERS )         // Force Reliable
	{
		message_begin( MSG_ALL,gmsgTextMsg );
	}

	else
	{
		if ( is_user_bot( ShowTo ) || !g_bPlayerConnected[ShowTo] )
			return PLUGIN_HANDLED;

		else
		{
			message_begin( MSG_ONE,gmsgTextMsg,{0,0,0},ShowTo );
		}
	}

	write_byte( Destination );          // destination
	write_string( Message );            // message
	write_string( "" );                 // submessage1
	write_string( "" );                 // submessage2
	write_string( "" );                 // submessage3
	write_string( "" );                 // submessage4
	message_end();

	return PLUGIN_HANDLED;
}


/* -------------------------------------------------------------- */


// Returns an integer token found after iToken'th cChar

public iGettok( szData[64], iToken, cChar ) {

	new szTokenData[64], szTempData[64];
	copy( szTempData, 63, szData );

	for ( new i = 1; i <= iToken; i++ )
	{
		new szLeft[64], szRight[64];
		new iLen = copyc( szLeft, 63, szTempData, cChar ) + 1;

		copy( szRight, 63, szTempData[iLen] );
		parse( szLeft, szLeft, 63 );
		parse( szRight, szRight, 63 );

		szTempData = szRight;
		szTokenData = szLeft;
	}

	if ( !szTokenData[0] )
		return ( 0 );

	new iValue = str_to_num( szTokenData );
	return ( iValue );
}


// Prints szMessage to server (id=0), or client num.

public status_print( id, szMessage[] ) {

	if ( id > 0 && id <= MAX_PLAYERS )
		client_print( id, print_console, "%s", szMessage );

	else
	{
		server_print( "%s", szMessage );
	}

	return PLUGIN_HANDLED;
}


public get_pcvar_bitsum( pointer ) {

	new szFlags[64];
	get_pcvar_string( pointer, szFlags, 63 );

	return ( read_flags( szFlags ) );
}


public sqrt( iNum ) {       // Written by Spacedude

	new iDiv = iNum;
	new iResult = 1;

	// end when iDiv <= iResult
	// take mean value as new divisor

	while ( iDiv > iResult )
	{
		iDiv = ( iDiv + iResult ) / 2;
		iResult = iNum / iDiv;
	}

	return ( iDiv );
}


// Returns letter matching iLetterNum

public get_letter( iLetterNum, szLetter[2] ) {

	new bool:bUpper;

	if ( iLetterNum > 26 )
	{
		bUpper = true;
		iLetterNum -= 26;
	}

	switch ( iLetterNum )
	{
		case 1:  copy( szLetter, 1, "a" );
		case 2:  copy( szLetter, 1, "b" );
		case 3:  copy( szLetter, 1, "c" );
		case 4:  copy( szLetter, 1, "d" );
		case 5:  copy( szLetter, 1, "e" );
		case 6:  copy( szLetter, 1, "f" );
		case 7:  copy( szLetter, 1, "g" );
		case 8:  copy( szLetter, 1, "h" );
		case 9:  copy( szLetter, 1, "i" );
		case 10: copy( szLetter, 1, "j" );
		case 11: copy( szLetter, 1, "k" );
		case 12: copy( szLetter, 1, "l" );
		case 13: copy( szLetter, 1, "m" );
		case 14: copy( szLetter, 1, "n" );
		case 15: copy( szLetter, 1, "o" );
		case 16: copy( szLetter, 1, "p" );
		case 17: copy( szLetter, 1, "q" );
		case 18: copy( szLetter, 1, "r" );
		case 19: copy( szLetter, 1, "s" );
		case 20: copy( szLetter, 1, "t" );
		case 21: copy( szLetter, 1, "u" );
		case 22: copy( szLetter, 1, "v" );
		case 23: copy( szLetter, 1, "w" );
		case 24: copy( szLetter, 1, "x" );
		case 25: copy( szLetter, 1, "y" );
		case 26: copy( szLetter, 1, "z" );
	}

	if ( bUpper )
		strtoupper( szLetter );

	return PLUGIN_HANDLED;
}


// Glow Durations

public Float:glow_duration( iCalcNum ) {

	// Min = 0.5 secs / Max = 5.0 secs

	new Float:fCalcNum = float( iCalcNum );
	new Float:fBaseDuration = GLOW_DURATION_BASE;

	new Float:fDuration = ( fBaseDuration * fCalcNum );

	if ( fDuration < GLOW_DURATION_MIN )
		fDuration = GLOW_DURATION_MIN;

	if ( fDuration > GLOW_DURATION_MAX )
		fDuration = GLOW_DURATION_MAX;

	return ( fDuration );
}


public AMX_get_opposite( iNum ) {

	if ( iNum == 0 )
		iNum = 1;

	else if ( iNum == 1 )
		iNum = 0;

	return ( iNum );
}


public is_weapon_fired( iNewAmmo, iOldAmmo ) {

	if ( iNewAmmo < iOldAmmo )
		return ( 1 );

	return ( 0 );
}

// Distance formula (metric) - from statsx
stock Float:distance( iDistance ) {
  return float( iDistance ) * 0.0254
}


// ------------------------------------------------- End. - //
