<?php

	$skill_data 	= null;
	$race_data	 	= null;
	$config_data	= null;
	
	function getSkills()
	{
		global $skill_data, $db, $config;
		
		// Query the DB!
		$sql = "SELECT wc3_web_skill.skill_id, wc3_web_skill.skill_name, wc3_web_skill.skill_description, wc3_web_skill.skill_type, wc3_web_skill.skill_owner FROM wc3_web_skill WHERE `skill_lang` = '$config->lang';";
		$data = $db->get_results( $sql );
		
		// Add them to the array based on their ID
		$skill_data = array();
		for ( $i = 0; $i < count( $data ); $i++ )
		{
			$skill_data[(int) $data[$i]->skill_id] = $data[$i];
		}
	}
	
	function getSkillsByRace( $race_id )
	{
		global $skill_data;
	
		$skills = array();
		
		for ( $i = 0; $i < count( $skill_data ); $i++ )
		{
			if ( $skill_data[$i]->skill_owner == $race_id )
			{
				array_push( $skills, $skill_data[$i] );
			}
		}
		
		return $skills;
	}
	
	function getRaces()
	{
		global $race_data, $db, $config;
		
		// Query the DB!
		$sql = "SELECT wc3_web_race.race_id, wc3_web_race.race_name, wc3_web_race.race_description FROM wc3_web_race WHERE `race_lang` = '$config->lang';";
		$data = $db->get_results( $sql );
		
		// Add them to the array based on their ID
		$race_data = array();
		for ( $i = 0; $i < count( $data ); $i++ )
		{
			$race_data[(int) $data[$i]->race_id] = $data[$i];
		}
	}
	
	function getRaceName( $id )
	{
		global $race_data;
		
		return $race_data[$id]->race_name;
	}
	
	function getPluginConfig()
	{
		global $config_data, $db, $config;
		
		// Query the DB!
		$sql = "SELECT wc3_config.config_id, wc3_config.config_value FROM wc3_config;";
		$data = $db->get_results( $sql );
		
		// Add them to the array based on their ID
		$config_data = array();
		for ( $i = 0; $i < count( $data ); $i++ )
		{
			$config_data[$data[$i]->config_id] = $data[$i]->config_value;
		}
	}
	
	function isRaceLess()
	{
		global $config_data;
		
		if ( !isset( $config_data->raceless ) || $config_data->raceless == 0 )
		{
			return false;
		}
		
		return true;
	}
	
	// Function will organize the array based on the skill ID
	function getPlayerSkillData( $player_id )
	{
		global $db, $config;
		
		$sql = "SELECT wc3_player_skill.skill_id, wc3_player_skill.skill_level FROM wc3_player_skill WHERE `player_id` = '" . (int) $_GET['id'] . "';";
		$data = $db->get_results( $sql );
		
		// Add them to the array based on their ID
		$p_skill_data = array();
		for ( $i = 0; $i < count( $data ); $i++ )
		{
			$p_skill_data[(int) $data[$i]->skill_id] = $data[$i]->skill_level;
		}
		
		return $p_skill_data;
	}
	
	function getRaceLevelByXP( $xp )
	{
		global $config_data;
		
		$iLevel = 0;
		
		for ( $i = 0; $i <= 10; $i++ )
		{
			if ( $xp >= $config_data['level' . $i . '_xp'] )
			{
				$iLevel = $i;
			}
			else
			{
				break;
			}
		}		

		return $iLevel;
	}
	
	function isEven( $num )
	{
		return ( $num % 2 == 0 );
	}
	
	function get_var( $var )
	{
		return isset( $_GET[$var] ) ? $_GET[$var] : ( isset( $_POST[$var] ) ? $_POST[$var] : '' );
	}

?>