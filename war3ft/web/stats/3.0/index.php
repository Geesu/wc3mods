<?php

	require( "config.php" );
	
	$sql = "SELECT race_name, race_id FROM wc3_web_race WHERE race_lang = '$config->lang' ORDER BY race_id";
	$races = $db->get_results( $sql );
	
	// Determine what we want to do!
	
	// Show a player's details!
	if ( isset( $_GET['id'] ) )
	{
	
	
	}
	
	// Otherwise show the default page!
	else
	{
		$race_id		= addslashes( get_var( "race_id" ) );
		$player_name	= addslashes( get_var( "player_name" ) );
		$player_steamid	= addslashes( get_var( "player_name" ) );
		
		// Page number set?
		if ( get_var( "page_num" ) != '' )
		{
			$_SESSION['page_num'] = intval( get_var( "page_num" ) );
		}
		
		// Remember what race ID we are displaying
		if ( $race_id != '' )
		{
			$_SESSION['race_id'] = $race_id;
		}
		
		$start_limit = ($_SESSION['page_num'] - 1) * $config->limit;

		// Search by player name!
		if ( $player_name != '' )
		{
			$sql = "SELECT wc3_player.player_id, wc3_player_race.race_id, wc3_player_race.race_xp, wc3_web_race.race_name, wc3_player_extra.player_name FROM wc3_player INNER JOIN wc3_player_race ON wc3_player.player_id = wc3_player_race.player_id INNER JOIN wc3_web_race ON wc3_player_race.race_id = wc3_web_race.race_id INNER JOIN wc3_player_extra on wc3_player.player_id = wc3_player_extra.player_id WHERE wc3_player_race.race_xp > 0 AND wc3_web_race.race_lang = '$config->lang' AND wc3_player.player_name LIKE '%$player_name%' ORDER BY race_xp DESC";
		}
		
		// Search by player id!
		else if ( $player_steamid != '' )
		{
			$sql = "SELECT wc3_player.player_id, wc3_player_race.race_id, wc3_player_race.race_xp, wc3_web_race.race_name, wc3_player_extra.player_name FROM wc3_player INNER JOIN wc3_player_race ON wc3_player.player_id = wc3_player_race.player_id INNER JOIN wc3_web_race ON wc3_player_race.race_id = wc3_web_race.race_id INNER JOIN wc3_player_extra on wc3_player.player_id = wc3_player_extra.player_id WHERE wc3_player_race.race_xp > 0 AND wc3_web_race.race_lang = '$config->lang' AND wc3_player.player_steamid LIKE '%$player_steamid%' ORDER BY race_xp DESC";
		}
		
		// Show all
		else
		{
			$sql = "SELECT wc3_player.player_id, wc3_player_race.race_id, wc3_player_race.race_xp, wc3_web_race.race_name, wc3_player_extra.player_name FROM wc3_player INNER JOIN wc3_player_race ON wc3_player.player_id = wc3_player_race.player_id INNER JOIN wc3_web_race ON wc3_player_race.race_id = wc3_web_race.race_id INNER JOIN wc3_player_extra on wc3_player.player_id = wc3_player_extra.player_id WHERE wc3_player_race.race_xp > 0 AND wc3_web_race.race_lang = '$config->lang'" . ( $_SESSION['race_id'] != -1 ? " AND wc3_player_race.race_id = '" . $_SESSION['race_id'] . "'" : '' ) . " ORDER BY race_xp DESC";
		}
		
		// Determine max # of rows
		$db->query( $sql );
		$num_rows = $db->num_rows;
		
		// Get players
		$sql .= " LIMIT $start_limit, $config->limit;";
		$players = $db->get_results( $sql );
	}
	
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
	<title>Warcraft 3 Frozen Throne Statistics</title>
	<link href="main.css" rel="stylesheet" type="text/css">
</head>
<body>

<div class="cTopBanner">
	<br />
	<a href="<?php echo $_SERVER['PHP_SELF']; ?>?page_num=1"><img src="images/war3ft/top_banner.jpg" alt="Warcraft 3 Frozen Throne" style="border:0px;"></a>
</div>

<br />

<form method="POST" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<div class="cSearchBox_Outer">
		<font class="text-small">Search:</font>
		
		<div class="cSearchBox_Inner1">
			<div class="cSearchBox_Inner2">
				<div class="cSearchBox_Inner3">
					<div class="cSearchBox_Left">
						<font class="text-hdr">
							Race
						</font>
					</div>
					<div class="cSearchBox_Mid">
						<select name="race_id" class="sel-norm" style="width:125px;">
							<option value="-1">All</option>
							<?php
								foreach ( $races as $race )
								{
									echo "<option value=\"$race->race_id\"" . ( $_SESSION['race_id'] == $race->race_id ? " selected" : "" ) . ">$race->race_name</option>";
								}
							?>
						</select>
					</div>
					<div class="cSearchBox_Right">
						<input type="submit" value="Look Up" class="btn">
					</div>
				</div>
				<div class="cSearchBox_Inner3">
					<div class="cSearchBox_Left">
						<font class="text-hdr">
							Search for Player by STEAM ID
						</font>
					</div>
					<div class="cSearchBox_Mid">
						<input type="text" name="player_steamid" class="field" style="width:125px;">
					</div>
					<div class="cSearchBox_Right">
						<input type="submit" value="Look Up" class="btn">
					</div>
					
				</div>
				<div class="cSearchBox_Inner3">
					<div class="cSearchBox_Left">
						<font class="text-hdr">
							Search for Player by Name
						</font>
					</div>
					<div class="cSearchBox_Mid">
						<input type="text" name="player_name" class="field" style="width:125px;">
					</div>
					<div class="cSearchBox_Right">
						<input type="submit" value="Look Up" class="btn">
					</div>
				</div>
				<div style="clear:both;"></div>
			</div>
		</div>
</div>

<br />

<div class="cMain">

<?php

	// We're simply listing all players
	if ( isset( $players ) )
	{
?>
	<div class="cPlayers_Outer">
		<div class="text-small" style="float:left;text-align:left;width:49%;">
			Players
		</div>
		<div class="text-small" style="float:left;text-align:right;width:49%;">
			Goto: <select name="page_num" class="sel-norm" onchange="this.form.submit();">
			<?php
				for ( $i = 1; $i <= ceil( $num_rows / $config->limit ); $i++ )
				{
					echo "<option value=\"$i\"" . ( $_SESSION['page_num'] == $i ? " selected" : "" ) . ">$i</option>\r\n";
				}
			?>
			</select> 
		</div>
		<div style="clear: both;"></div>

		
		<div class="cPlayers_Inner1">
			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="tbl-players">
				<tr class="tbl-hdr">
					<td>Rank</td>
					<td>Player Name</td>
					<td>XP</td>
					<td>Race</td>
				</tr>
				<tr style="height:4px; background-color:#000000; font-size:4px;"><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>


<?php
		$i = ( ( $_SESSION['page_num'] - 1 ) * 100 ) + 1;
	
		foreach ( $players as $player )
		{
			echo "<tr class=\"" . ( ( $i % 2 == 0 ) ? "tbl-shade2" : "tbl-shade3" ) . "\"><td>" . $i++ . "</td><td><b><a href=\"" . $_SERVER['PHP_SELF'] . "?id=$player->player_id\">" . htmlspecialchars( $player->player_name ) . "</a></b></td><td>$player->race_xp</td><td>$player->race_name</td></tr>\n\r";
		}
?>
			</table>
		</div>
	</div>
<?php
	}
	
	// We're showing a player
	else if ( isset( $_GET['id'] ) )
	{
		// Get all the skill names + descriptions!
		getSkills();
		
		// Get all the race names + descriptions!
		getRaces();
		
		// Get the config information!
		getPluginConfig();
		
		// Lets get this player's information!
		$sql = "SELECT player_name, player_steamid FROM wc3_player_extra WHERE `player_id` = '" . (int) $_GET['id'] . "';";
		$p_data = $db->get_row( $sql );
		
		// Get the player's skill information
		$p_skill_data = getPlayerSkillData( $player_id );

		// Race version of war3ft!
		if ( !isRaceLess() )
		{
			// Get race data!
			$sql = "SELECT wc3_player_race.race_id, wc3_player_race.race_xp FROM wc3_player_race WHERE `player_id` = '" . (int) $_GET['id'] . "';";
			$p_race_data = $db->get_results( $sql );		
		
		}
		
		// Otherwise assume there are no races!
		else
		{
		
		}
?>
<div class="cPlayer_Outer">
	<div style="float:left;">
		<font class="text-small">Player Information:</font>
	</div>
	<div style="float:right;">
		<font size="1"><a href="<?=$_SERVER['PHP_SELF']; ?>">< Back</a></font>
	</div>
	<div style="float:clear;"></div>
	&nbsp;
	
	<div class="cSearchBox_Inner1">
		<div class="cSearchBox_Inner2">
			<span class="text-PlrHdr">
				<center>
				<?php
					// Print player's name
					echo $p_data->player_name . "'s Race Information<br>";
					
					// Show steam id?
					echo ( ( $config->show_steam_id && strlen( $p_data->player_steamid ) > 0 ) ? ( "(" . $p_data->player_steamid . ")" ) : "" );
				?>
				</center>
			
			</span>
		</div>
	</div>
</div>

<br>
<?php
	
	// Raced version
	if ( !isRaceLess() )
	{
		// Loop through and display each race!
		for ( $i = 0; $i < count( $p_race_data ); $i++ )
		{
			$div_align = ( isEven( $i ) ? "left" : "right" );
			$race_level = getRaceLevelByXP( $p_race_data[$i]->race_xp );
?>
<?=( ( isEven( $i ) ) ? "<div class=\"cPlayer_DataOuter\">\n\r" : "" ) ; ?>

	<div class="cPlayer_DataOuter2" style="float:<?php echo $div_align; ?>">
		<div class="cPlayer_DataOuter3">
			<div class="cPlayer_DataInner1">
				<div class="cPlayer_DataInner2">
					<center>
						<b><?=getRaceName( $p_race_data[$i]->race_id ); ?></b><br>
						Level <?=$race_level; ?><br>
						XP <?=$p_race_data[$i]->race_xp; ?><br>
					</center>
					<br>
					<center>
						<table>
							<tr>
								<td align="center" style="padding-bottom: 5px;"><u>Skill</u></td>
								<td align="center"><u>Level</u></td>
								<td width="60%"><u>Description</u></td>
							</tr>
						<?php
							
							// Get the skills for this race
							$skills = getSkillsByRace( $p_race_data[$i]->race_id );
							
							foreach( $skills as $skill )
							{
								// Figure out what the skill level is so we can determine the appropriate image!
								$skill_level = isset( $p_skill_data[$skill->skill_id] ) ? $p_skill_data[$skill->skill_id] : 0;
								
								// Trainable skill
								if ( $skill->skill_type == 1 )
								{
									$skill_level_img = "<img src=\"images/level" . $skill_level . ".gif\" alt=\"Level $skill_level\" title=\"Level $skill_level\">";
								}
								
								// Passive skill
								else if ( $skill->skill_type == 2 )
								{
									$skill_level_img = "<img src=\"images/passive" . $race_level . ".gif\"  alt=\"Level $race_level\" title=\"Level $race_level\">";
									$skill->skill_description = "Passive, $skill->skill_description";
								}
								
								// Ultimate
								else if ( $skill->skill_type == 3 )
								{
									$skill_level_img = "<img src=\"images/ultimate" . $skill_level . ".gif\"  alt=\"Level $skill_level\" title=\"Level $skill_level\">";
									$skill->skill_description = "Ultimate, $skill->skill_description";
								}
								
						?>
							<tr>
								<td style="padding-top: 5px;">
									<center>
										<img src="images/war3ft/<?=$skill->skill_id; ?>.gif" alt="<?=$skill->skill_name; ?>" title="<?=$skill->skill_name; ?>"><br>
										<span class="text-small">
											<?=$skill->skill_name; ?>
										</span>
									</center>
								</td>
								<td><?=$skill_level_img; ?></td>
								<td style="padding:5px; text-align: left;"><?=$skill->skill_description; ?></td>
							</tr>
						<?php	
							}
						?>
						</table>
					</center>
				</div>
			</div>
		</div>
	</div>
<?=( ( !isEven( $i ) ) ? "<div style=\"clear:both;\"></div>\n\r" : "" ) ; ?>

<?=( ( !isEven( $i ) ) ? "</div><br>\n\r" : "" ) ; ?>
<?php
		} //endfor
	} //endif
?>
</div>

<?php
	
	}
?>
</div>

<div class="cBottom">
	<br /><font size="1">Powered by <a href="http://wc3mods.net/" target="_blank">Warcraft 3 Mods</a> v1.2</font><br /><br />
</div>
</form>
</body>
</html>