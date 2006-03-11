/* - Globals ---------------------------------------- */

new bool:bNewMap = true;			// Used to print a "map has loaded" line in the debug file



/* - Debugging Functions -------------------------------------------- */

public log_function( info[] ){

	if ( bNewMap ) {
		new szMapName[32];
		new szString[128];

		get_mapname( szMapName, 31 );
		format( szString, 127, "******** A new map has loaded (%s)", szMapName );

		bNewMap = false;

		write_file( "addons/amxmodx/logs/war3x.log", szString, -1 );
	}

	write_file( "addons/amxmodx/logs/war3x.log", info, -1 );
	
	return PLUGIN_CONTINUE;
}