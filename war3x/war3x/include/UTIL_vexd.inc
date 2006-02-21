
/* - AMX Mod Include -------------------------------------------- *

 *  UTIL_vexd
 *  (c) Copyright 2003-2004, Created by Ryan.
 *
 *  A collection of useful functions that can be used in all amx
 *  script files. These functions are 'stock', so they will not
 *  be compiled unless used.
 *
 *
 *  Enjoy!

 * -------------------------------------------------------------- */


#define BOTSPEED_NORMAL                0.0


/* -------------------------------------------------------------- */


// XTRAFUN - Gets the velocity of an entity (ent)

stock get_entity_velocity( ent, velocity[3] )
{
#if ADVANCED_DEBUG
	log_function("stock get_entity_velocity( ent, velocity[3] )");
#endif
    new Float:fVelocity[3];
    entity_get_vector( ent, EV_VEC_velocity, fVelocity );

    velocity[0] = floatround( fVelocity[0] );
    velocity[1] = floatround( fVelocity[1] );
    velocity[2] = floatround( fVelocity[2] );

    return ( velocity[0] && velocity[1] && velocity[2] );
}


// XTRAFUN - Sets the velocity of an entity (ent)

stock set_entity_velocity( ent, velocity[3] )
{
#if ADVANCED_DEBUG
	log_function("stock set_entity_velocity( ent, velocity[3] )");
#endif
    new Float:fVelocity[3];

    fVelocity[0] = float( velocity[0] );
    fVelocity[1] = float( velocity[1] );
    fVelocity[2] = float( velocity[2] );

    entity_set_vector( ent, EV_VEC_velocity, fVelocity );

    return ( 1 );
}


// XTRAFUN - Gets the origin of an entity (ent)

stock get_entity_origin( ent, origin[3] )
{
#if ADVANCED_DEBUG
	log_function("stock get_entity_origin( ent, origin[3] )");
#endif
    new Float:fOrigin[3];
    entity_get_vector( ent, EV_VEC_origin, fOrigin );

    origin[0] = floatround( fOrigin[0] );
    origin[1] = floatround( fOrigin[1] );
    origin[2] = floatround( fOrigin[2] );

    return ( origin[0] && origin[1] && origin[2] );
}


// XTRAFUN - Sets the origin of an entity (ent)

stock set_entity_origin(ent, origin[3])
{
#if ADVANCED_DEBUG
	log_function("stock set_entity_origin(ent, origin[3])");
#endif
    new Float:fOrigin[3];

    fOrigin[0] = float( origin[0] );
    fOrigin[1] = float( origin[1] );
    fOrigin[2] = float( origin[2] );

    entity_set_vector( ent, EV_VEC_origin, fOrigin );

    return ( 1 );
}


// XTRAFUN - Gets the velocity of a player (id)

/*	REMOVED as of AMXX 0.20
	stock get_user_velocity( id, velocity[3] )
	{
	    get_entity_velocity( id,velocity );
	    return ( velocity[0] && velocity[1] && velocity[2] );
	}
*/


// XTRAFUN - Sets the velocity of a player (id)

/*	REMOVED as of AMXX 0.20
	stock set_user_velocity( id, velocity[3] )
	{
	    set_entity_velocity( id,velocity );
	    return ( 1 );
	}
*/


// Gets velocity of an entity (ent) toward new origin with speed (fSpeed)

stock get_velocity_to_origin( ent, Float:fOrigin[3], Float:fSpeed, Float:fVelocity[3] )
{
#if ADVANCED_DEBUG
	log_function("stock get_velocity_to_origin( ent, Float:fOrigin[3], Float:fSpeed, Float:fVelocity[3] )");
#endif
    new Float:fEntOrigin[3];
    entity_get_vector( ent, EV_VEC_origin, fEntOrigin );

    // Velocity = Distance / Time

    new Float:fDistance[3];
    fDistance[0] = fOrigin[0] - fEntOrigin[0];
    fDistance[1] = fOrigin[1] - fEntOrigin[1];
    fDistance[2] = fOrigin[2] - fEntOrigin[2];

    new Float:fTime = ( vector_distance( fEntOrigin,fOrigin ) / fSpeed );

    fVelocity[0] = fDistance[0] / fTime;
    fVelocity[1] = fDistance[1] / fTime;
    fVelocity[2] = fDistance[2] / fTime;

    return ( fVelocity[0] && fVelocity[1] && fVelocity[2] );
}


// Sets velocity of an entity (ent) toward new origin with speed (fSpeed)

stock set_velocity_to_origin( ent, Float:fOrigin[3], Float:fSpeed )
{
#if ADVANCED_DEBUG
	log_function("stock set_velocity_to_origin( ent, Float:fOrigin[3], Float:fSpeed )");
#endif
    new Float:fVelocity[3];
    get_velocity_to_origin( ent, fOrigin, fSpeed, fVelocity );

    entity_set_vector( ent, EV_VEC_velocity, fVelocity );

    return ( 1 );
}


// Sets velocity of an entity (ent1) toward new entity (ent2) with speed (fSpeed)

stock set_velocity_to_ent( ent1, ent2, Float:fSpeed )
{
#if ADVANCED_DEBUG
	log_function("stock set_velocity_to_ent( ent1, ent2, Float:fSpeed )");
#endif
    new Float:fOrigin[3], Float:fVelocity[3];

    entity_get_vector( ent2, EV_VEC_origin, fOrigin );
    get_velocity_to_origin( ent1, fOrigin, fSpeed, fVelocity );

    entity_set_vector( ent1, EV_VEC_velocity, fVelocity );

    return ( 1 );
}


// Gets velocity of an entity (ent) away from origin with speed (fSpeed)

stock get_velocity_from_origin( ent, Float:fOrigin[3], Float:fSpeed, Float:fVelocity[3] )
{
#if ADVANCED_DEBUG
	log_function("stock get_velocity_from_origin( ent, Float:fOrigin[3], Float:fSpeed, Float:fVelocity[3] )");
#endif
    new Float:fEntOrigin[3];
    entity_get_vector( ent, EV_VEC_origin, fEntOrigin );

    // Velocity = Distance / Time

    new Float:fDistance[3];
    fDistance[0] = fEntOrigin[0] - fOrigin[0];
    fDistance[1] = fEntOrigin[1] - fOrigin[1];
    fDistance[2] = fEntOrigin[2] - fOrigin[2];

    new Float:fTime = ( vector_distance( fEntOrigin,fOrigin ) / fSpeed );

    fVelocity[0] = fDistance[0] / fTime;
    fVelocity[1] = fDistance[1] / fTime;
    fVelocity[2] = fDistance[2] / fTime;

    return ( fVelocity[0] && fVelocity[1] && fVelocity[2] );
}


// Sets velocity of an entity (ent) away from origin with speed (speed)

stock set_velocity_from_origin( ent, Float:fOrigin[3], Float:fSpeed )
{
#if ADVANCED_DEBUG
	log_function("stock set_velocity_from_origin( ent, Float:fOrigin[3], Float:fSpeed )");
#endif
    new Float:fVelocity[3];
    get_velocity_from_origin( ent, fOrigin, fSpeed, fVelocity )

    entity_set_vector( ent, EV_VEC_velocity, fVelocity );

    return ( 1 );
}


// Sets velocity of an entity (ent1) away from another entity (ent2) with speed (fSpeed)

stock set_velocity_from_ent( ent1, ent2, Float:fSpeed )
{
#if ADVANCED_DEBUG
	log_function("stock set_velocity_from_ent( ent1, ent2, Float:fSpeed )");
#endif
    new Float:fOrigin[3], Float:fVelocity[3];

    entity_get_vector( ent2, EV_VEC_origin, fOrigin );
    get_velocity_from_origin( ent1, fOrigin, fSpeed, fVelocity );

    entity_set_vector( ent1, EV_VEC_velocity, fVelocity );

    return ( 1 );
}


// Sets maxspeed of an entity ( corrects issues with setting maxspeed on bots )

stock set_entity_maxspeed( ent, Float:fMaxSpeed ) {
#if ADVANCED_DEBUG
	log_function("stock set_entity_maxspeed( ent, Float:fMaxSpeed ) {");
#endif

    entity_set_float( ent, EV_FL_maxspeed, fMaxSpeed );
    return ( 1 );
}


// Sets the rendering of an entity ( similar to set_user_rendering )

stock set_entity_rendering( ent, iRenderFx, iR, iG, iB, iRenderType, iAmmount )
{
#if ADVANCED_DEBUG
	log_function("stock set_entity_rendering( ent, iRenderFx, iR, iG, iB, iRenderType, iAmmount )");
#endif
    entity_set_int( ent, EV_INT_renderfx, iRenderFx );

    new Float:fRGB[3];
    fRGB[0] = float( iR );
    fRGB[1] = float( iG );
    fRGB[2] = float( iB );

    entity_set_vector( ent, EV_VEC_rendercolor, fRGB );
    entity_set_int( ent, EV_INT_rendermode, iRenderType );

    new Float:fAmmount = float( iAmmount );

    entity_set_float( ent, EV_FL_renderamt, fAmmount );

    return ( 1 );
}


// Returns 1 if iStartEnt has unobstructed path to iEndEnt

stock ent_in_view( iStartEnt, iEndEnt )
{
#if ADVANCED_DEBUG
	log_function("stock ent_in_view( iStartEnt, iEndEnt )");
#endif
    new Float:fStartOrigin[3];
    entity_get_vector( iStartEnt, EV_VEC_origin, fStartOrigin );

    new Float:fEndOrigin[3];
    entity_get_vector( iEndEnt, EV_VEC_origin, fEndOrigin );

    new Float:vReturn[3];

	// DEBUG
	/*log_amx("*** START traceline");
	log_amx("fStartOrigin(%d) (%f, %f, %f)", iStartEnt, fStartOrigin[0], fStartOrigin[1], fStartOrigin[2]);
	log_amx("fEndOrigin(%d) (%f, %f, %f)", iEndEnt, fEndOrigin[0], fEndOrigin[1], fEndOrigin[2]);
	log_amx("vReturn (%f, %f, %f)", vReturn[0], vReturn[1], vReturn[2]);*/

	// DEBUG

    new iHitEnt = trace_line( iStartEnt, fStartOrigin, fEndOrigin, vReturn );

    // Check Obstruction Hit is an Ent

    while ( iHitEnt > 0 )
    {
        if ( iHitEnt == iEndEnt )
            return ( 1 );

        entity_get_vector( iHitEnt, EV_VEC_origin, fStartOrigin );

		// DEBUG
		/*log_amx("*** START traceline: %d", iHitEnt);
		log_amx("fStartOrigin (%f, %f, %f)", fStartOrigin[0], fStartOrigin[1], fStartOrigin[2]);*/
		// DEBUG

        iHitEnt = trace_line( iHitEnt, fStartOrigin, fEndOrigin, vReturn );
    }

    // Check if Return / End Origin are the same

    if ( !vector_distance( vReturn, fEndOrigin ) )
        return ( 1 );

    return ( 0 );
}


// Predicts new origin of an entity (iEnt) after fTime

stock predict_ent_origin( iEnt, Float:fTime, Float:fNewOrigin[3] )
{
#if ADVANCED_DEBUG
	log_function("stock predict_ent_origin( iEnt, Float:fTime, Float:fNewOrigin[3] )");
#endif
    new Float:fVelocity[3];
    entity_get_vector( iEnt, EV_VEC_velocity, fVelocity );

    // Distance = Velocity * Time

    new Float:fDistance[3];
    fDistance[0] = fVelocity[0] * fTime;
    fDistance[1] = fVelocity[1] * fTime;
    fDistance[2] = fVelocity[2] * fTime;

    new Float:fOrigin[3];
    entity_get_vector( iEnt, EV_VEC_origin, fOrigin );

    fNewOrigin[0] += fOrigin[0] + fDistance[0];
    fNewOrigin[1] += fOrigin[1] + fDistance[1];
    fNewOrigin[2] += fOrigin[2] + fDistance[2];

    return ( fNewOrigin[0] && fNewOrigin[1] && fNewOrigin[2] );
}


// Gets the time it takes to travel from one origin to another at fSpeed.

stock Float:get_ent_traveltime( Float:fStartOrigin[3], Float:fEndOrigin[3], Float:fSpeed )
{
#if ADVANCED_DEBUG
	log_function("stock Float:get_ent_traveltime( Float:fStartOrigin[3], Float:fEndOrigin[3], Float:fSpeed )");
#endif
    // Time = Distance / Velocity

    new Float:fTime = ( vector_distance( fStartOrigin,fEndOrigin ) / fSpeed );

    return ( fTime );
}


// Creates an artificial temporary entity, which can be modified with entity variables

public Create_TempEnt( szClassName[32], szModel[64], Float:fOrigin[3], iMoveType, iSolidType, Float:fLife )
{
#if ADVANCED_DEBUG
	new longstring[512];
	format(longstring, 511, "public Create_TempEnt( %s, %s, Float:fOrigin[3], %d, %d, %f )", szClassName, szModel, iMoveType, iSolidType, fLife);
	log_function(longstring);
#endif
    new iEnt = create_entity( "info_target" );
    entity_set_string( iEnt, EV_SZ_classname, szClassName );

    entity_set_model( iEnt, szModel );
    entity_set_origin( iEnt, fOrigin );

    entity_set_int( iEnt, EV_INT_movetype, iMoveType );
    entity_set_int( iEnt, EV_INT_solid, iSolidType );

    Timer_TempEnt( iEnt, fLife );

    return ( iEnt );
}


public Remove_TempEnt( parm_iEnt[1] )
{
#if ADVANCED_DEBUG
	new string[128];
	format(string, 127, "public Remove_TempEnt( %d ) %d", parm_iEnt[0], is_valid_ent(parm_iEnt[0]));
	log_function(string);
	
	if ( is_valid_ent(parm_iEnt[0]) )
	{
		new iOwner = entity_get_edict( parm_iEnt[0], EV_ENT_owner );
		new Float:vVector[3];
		new szClassName[64], szGlobalName[64], szModel[64], szTarget[64];

		entity_get_vector( parm_iEnt[0], EV_VEC_origin, vVector );
		entity_get_string( parm_iEnt[0], EV_SZ_classname, szClassName, 63 );
		entity_get_string( parm_iEnt[0], EV_SZ_globalname, szGlobalName, 63 );
		entity_get_string( parm_iEnt[0], EV_SZ_model, szModel, 63 );
		entity_get_string( parm_iEnt[0], EV_SZ_target, szTarget, 63 );

		new chars[256];
		format(chars, 255, "%d:%s:%s:%s:%s (%f, %f, %f)", iOwner, szClassName, szGlobalName, szModel, szTarget, vVector[0], vVector[1], vVector[2] );
		log_function(chars);
	}
#endif
	new iEnt = parm_iEnt[0];
	new szClassName[64];
	entity_get_string( iEnt, EV_SZ_classname, szClassName, 63 );

	if ( is_war3_entity(iEnt, szClassName) )
	{
		//log_amx("[Remove_TempEnt] Removed entity %d with class name %s", iEnt, szClassName);
		remove_entity( iEnt );
	}
	else
	{
		log_amx("[Remove_TempEnt] Entity not removed, crash prevented: entity %d with class name %s", iEnt, szClassName);
	}

    return PLUGIN_HANDLED;
}


public Remove_TempEnts( szClassname[32], iOwner )
{
#if ADVANCED_DEBUG
	new string[128];
	format(string, 127, "public Remove_TempEnts( %s, %d )", szClassname, iOwner);
	log_function(string);
#endif

    new iEntity = find_ent_by_class( -1, szClassname );

    while ( iEntity > 0 )
    {
        if ( !iOwner || entity_get_edict( iEntity, EV_ENT_owner ) == iOwner )
        {
            new iTaskId = TASK_TEMPENTITY + iEntity;
            remove_task( iTaskId, 0 );
			
			log_amx("[Remove_TempEnts] Removed entity %d with class name %s", iEntity, szClassname);
            remove_entity( iEntity );

            iEntity = find_ent_by_class( -1, szClassname );
        }
        else
        {
            iEntity = find_ent_by_class( iEntity, szClassname );
        }
    }

    return PLUGIN_HANDLED;
}


public Create_TempSprite( szClassName[32], szModel[64], Float:fOrigin[3], iMoveType, iSolidType, Float:fFramerate, Float:fScale, Float:fLife )
{
#if ADVANCED_DEBUG
	new longstring[512];
	format(longstring, 511, "public Create_TempSprite( %s, %s, Float:fOrigin[3], %d, %d, %f, %f, %f )", szClassName, szModel, iMoveType, iSolidType, fFramerate, fScale, fLife);
	log_function(longstring);
#endif
    new iSprite = create_entity( "env_sprite" );

    entity_set_model( iSprite, szModel );
    entity_set_float( iSprite, EV_FL_framerate, fFramerate );

    DispatchSpawn( iSprite );

    entity_set_origin( iSprite, fOrigin );
    entity_set_string( iSprite, EV_SZ_classname, szClassName );

    entity_set_int( iSprite, EV_INT_movetype, iMoveType );
    entity_set_int( iSprite, EV_INT_solid, iSolidType );
    entity_set_float( iSprite, EV_FL_scale, fScale );

    set_entity_rendering( iSprite, kRenderFxNone, 0, 0, 0, kRenderTransAdd, 255 );

    Timer_TempEnt( iSprite, fLife );

    return ( iSprite );
}


public Timer_TempEnt( iEnt, Float:fLife ) {
#if ADVANCED_DEBUG
	log_function("public Timer_TempEnt( iEnt, Float:fLife ) {");
#endif

    if ( fLife > 0.0 )
    {
        new parm_iEnt[1];
        parm_iEnt[0] = iEnt;

        set_task( fLife, "Remove_TempEnt", TASK_TEMPENTITY + iEnt, parm_iEnt, 1 );
    }

    return PLUGIN_HANDLED;
}

// End of UTIL_VEXD.INC