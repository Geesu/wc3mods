<?
/*************************************************
*	This is the configuration file, please
*	enter the mysql information below
*
*	Details at http://war3ft.com
*
*	Credits:
*		Design set up by b4p
*		Top Banner by nightscream
*************************************************/

// 4HM Forums database
$config->dbuser 		= "root";
$config->dbpassword 	= "";
$config->dbname 		= "";
$config->dbhost 		= "localhost";

// Language
$config->lang = "en";

// Set this to true to show the user's steam ID
$config->show_steam_id = true;

// The amount to display per page
$config->limit = 100;

// What mode?
//   - "traditional"	- Like you're used to seeing, listed based on xp per race
//	 - "total"			- Will list players based on TOTAL XP over all races 
$config->mode = "traditional";

/***********************************/
/*** DO NOT EDIT BELOW THIS LINE ***/
/***********************************/
session_start();

require( "includes/ez_sql.php" );
require( "includes/functions.inc.php" );

// Create the database connection
$db = new db( $config->dbuser, $config->dbpassword, $config->dbname, $config->dbhost );

if ( !isset( $_SESSION['page_num'] ) )
{
	$_SESSION['page_num'] = 1;
}

if ( !isset( $_SESSION['race_id'] ) )
{
	$_SESSION['race_id'] = -1;
}
?>
