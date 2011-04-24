DROP TABLE IF EXISTS `character_xp_rates`;
CREATE TABLE `character_xp_rates` (
        `guid`  INT(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier' ,
        `start_time`  TIMESTAMP NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT 'Absolute start date, the bonus will be granted' ,
        `end_time`  TIMESTAMP NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT 'Absolute end date, the bonus is expired' ,
        `kill_xp_rate`  INT(10) UNSIGNED NOT NULL DEFAULT 1 COMMENT 'Experience Multiplier' ,
        `quest_xp_rate`  INT(10) UNSIGNED NOT NULL DEFAULT 1 COMMENT 'Experience Multiplier' ,
        `explore_xp_rate`  INT(10) UNSIGNED NOT NULL DEFAULT 1 COMMENT 'Experience Multiplier' ,
PRIMARY KEY (`guid`)
) ENGINE=INNODB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
 
-- Copy all players in characters into character_xp_rate with default values
INSERT IGNORE INTO `character_xp_rates` (`guid`) SELECT `guid` FROM `characters`;
