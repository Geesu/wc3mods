
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
	new Float:fOrigin[3];

	fOrigin[0] = float( origin[0] );
	fOrigin[1] = float( origin[1] );
	fOrigin[2] = float( origin[2] );

	entity_set_vector( ent, EV_VEC_origin, fOrigin );

	return ( 1 );
}


// XTRAFUN - Gets the velocity of a player (id)

/*  REMOVED as of AMXX 0.20
	stock get_user_velocity( id, velocity[3] )
	{
		get_entity_velocity( id,velocity );
		return ( velocity[0] && velocity[1] && velocity[2] );
	}
*/


// XTRAFUN - Sets the velocity of a player (id)

/*  REMOVED as of AMXX 0.20
	stock set_user_velocity( id, velocity[3] )
	{
		set_entity_velocity( id,velocity );
		return ( 1 );
	}
*/


// Gets velocity of an entity (ent) toward new origin with speed (fSpeed)

stock get_velocity_to_origin( ent, Float:fOrigin[3], Float:fSpeed, Float:fVelocity[3] )
{
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
	new Float:fVelocity[3];
	get_velocity_to_origin( ent, fOrigin, fSpeed, fVelocity );

	entity_set_vector( ent, EV_VEC_velocity, fVelocity );

	return ( 1 );
}


// Sets velocity of an entity (ent1) toward new entity (ent2) with speed (fSpeed)

stock set_velocity_to_ent( ent1, ent2, Float:fSpeed )
{
	new Float:fOrigin[3], Float:fVelocity[3];

	entity_get_vector( ent2, EV_VEC_origin, fOrigin );
	get_velocity_to_origin( ent1, fOrigin, fSpeed, fVelocity );

	entity_set_vector( ent1, EV_VEC_velocity, fVelocity );

	return ( 1 );
}


// Gets velocity of an entity (ent) away from origin with speed (fSpeed)

stock get_velocity_from_origin( ent, Float:fOrigin[3], Float:fSpeed, Float:fVelocity[3] )
{
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
	new Float:fVelocity[3];
	get_velocity_from_origin( ent, fOrigin, fSpeed, fVelocity )

	entity_set_vector( ent, EV_VEC_velocity, fVelocity );

	return ( 1 );
}


// Sets velocity of an entity (ent1) in the opposite direction of another entity (ent2) with speed (fSpeed)

stock set_velocity_from_ent( ent1, ent2, Float:fSpeed )
{
	new Float:fOrigin[3], Float:fVelocity[3];

	entity_get_vector( ent2, EV_VEC_origin, fOrigin );
	get_velocity_from_origin( ent1, fOrigin, fSpeed, fVelocity );

	entity_set_vector( ent1, EV_VEC_velocity, fVelocity );

	return ( 1 );
}


// Sets maxspeed of an entity ( corrects issues with setting maxspeed on bots )

stock set_entity_maxspeed( ent, Float:fMaxSpeed ) {

	entity_set_float( ent, EV_FL_maxspeed, fMaxSpeed );
	return ( 1 );
}


// Sets the rendering of an entity ( similar to set_user_rendering )

stock set_entity_rendering( ent, iRenderFx, iR, iG, iB, iRenderType, iAmmount )
{
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
	new Float:fStartOrigin[3];
	entity_get_vector( iStartEnt, EV_VEC_origin, fStartOrigin );

	new Float:fEndOrigin[3];
	entity_get_vector( iEndEnt, EV_VEC_origin, fEndOrigin );

	new Float:vReturn[3];

	new iHitEnt = trace_line( iStartEnt, fStartOrigin, fEndOrigin, vReturn );

	// Check Obstruction Hit is an Ent

	while ( iHitEnt > 0 )
	{
		if ( iHitEnt == iEndEnt )
			return ( 1 );

		entity_get_vector( iHitEnt, EV_VEC_origin, fStartOrigin );

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
	// Time = Distance / Velocity

	new Float:fTime = ( vector_distance( fStartOrigin,fEndOrigin ) / fSpeed );

	return ( fTime );
}




// ------------------------------------------------- End. - //
