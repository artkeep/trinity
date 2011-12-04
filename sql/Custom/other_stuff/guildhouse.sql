DELETE FROM creature_template WHERE `entry`=13;
INSERT INTO `creature_template` VALUES ('13', '0', '0', '0', '0', '0', '26789', '0', '0', '0', 'Beltez', 'Guildhouse Keeper', '', '0', '80', '80', '0', '35', '35', '1', '1.48', '1.14286', '0.75', '0', '181', '189', '0', '158', '1', '1400', '1900', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '1', '3', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', 'guildmaster', '0');



-- ----------------------------
-- Table structure for guildhouses
-- ----------------------------
CREATE TABLE `guildhouses` (
  `id` int(8) unsigned NOT NULL auto_increment,
  `guildId` bigint(20) NOT NULL default '0',
  `x` double NOT NULL,
  `y` double NOT NULL,
  `z` double NOT NULL,
  `map` int(11) NOT NULL,
  `comment` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=42 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records
-- ----------------------------
INSERT INTO `guildhouses` VALUES ('1', '0', '16222', '16266', '14.2', '1', 'GM Island');
INSERT INTO `guildhouses` VALUES ('2', '0', '-10711', '2483', '8', '1', 'Tauren village at Veiled Sea (Silithus)');
INSERT INTO `guildhouses` VALUES ('3', '0', '-8323', '-343', '146', '0', 'Fishing outside an Northshire Abbey (Elwynn Forest');
INSERT INTO `guildhouses` VALUES ('4', '0', '7368', '-1560', '163', '1', 'Troll Village in mountains (Darkshore)');
INSERT INTO `guildhouses` VALUES ('5', '0', '-4151', '-1400', '198', '0', 'Dwarven village outside Ironforge (Wetlands)');
INSERT INTO `guildhouses` VALUES ('6', '0', '-1840', '-4233', '2.14', '0', 'Dwarven village (Arathi Highlands, Forbidding Sea)');
INSERT INTO `guildhouses` VALUES ('8', '0', '-723', '-1076', '179', '1', 'Tauren camp (Mulgore, Red Rock)');
INSERT INTO `guildhouses` VALUES ('9', '0', '-206', '1666', '80', '0', 'Shadowfang Keep an outside instance (Silverpine Forest)');
INSERT INTO `guildhouses` VALUES ('10', '0', '-6374', '1262', '7', '0', 'Harbor house outside Stormwind (Elwynn Forest)');
INSERT INTO `guildhouses` VALUES ('11', '0', '-8640', '580', '96', '0', 'Empty jail between canals (Stormwind) DELETE');
INSERT INTO `guildhouses` VALUES ('12', '0', '-4844', '-1066', '502', '0', 'Old Ironforge');
INSERT INTO `guildhouses` VALUES ('13', '0', '-4863', '-1658', '503.5', '0', 'Ironforge Airport');
INSERT INTO `guildhouses` VALUES ('14', '0', '1146', '-165', '313', '37', 'Azshara Crater instance (Alliance entrance)');
INSERT INTO `guildhouses` VALUES ('15', '0', '-123', '858', '298', '37', 'Azshara Crater instance (Horde entrance)');
INSERT INTO `guildhouses` VALUES ('16', '0', '4303', '-2760', '16.8', '0', 'Quel\'Thalas Tower');
INSERT INTO `guildhouses` VALUES ('17', '0', '-6161', '-790', '423', '0', 'Crashed gnome airplane (between Dun Morogh and Searing Gorge)');
INSERT INTO `guildhouses` VALUES ('18', '0', '-11790', '-1640', '54.7', '0', 'Zul\'Gurub an outside instance (Stranglethorn Vale)');
INSERT INTO `guildhouses` VALUES ('19', '0', '-11805', '-4754', '6', '1', 'Goblin village (Tanaris, South Seas)');
INSERT INTO `guildhouses` VALUES ('20', '0', '-9296', '670', '132', '0', 'Villains camp outside an Stormwind (Elwynn Forest)');
INSERT INTO `guildhouses` VALUES ('21', '0', '3414', '-3380', '142.2', '0', 'Stratholm an outside instance');
INSERT INTO `guildhouses` VALUES ('22', '0', '4654', '-3772', '944', '1', 'Kalimdor Hyjal (Aka World Tree)');
INSERT INTO `guildhouses` VALUES ('23', '0', '2176', '-4766', '55', '1', 'The Ring of Valor (Aka. Orgrimmar Arena)');
INSERT INTO `guildhouses` VALUES ('24', '0', '1951.512085', '1530.475586', '247.288147', '1', 'Stonetalon Logging Camp');
INSERT INTO `guildhouses` VALUES ('25', '0', '2813.660645', '2248.552979', '215.524643', '1', 'Stonetalon Ruins');
INSERT INTO `guildhouses` VALUES ('28', '0', '9725.27', '-21.43', '20.03', '1', 'Teldrassil Furbold camp');
INSERT INTO `guildhouses` VALUES ('29', '0', '-3855', '-3479', '579', '0', 'Wetlands mountain camp');
INSERT INTO `guildhouses` VALUES ('30', '0', '-5362', '-2540', '485', '0', 'Ortell\'s Hideout');
INSERT INTO `guildhouses` VALUES ('31', '0', '-12865', '-1396', '115', '0', 'Stranglethorn Secret Cave');
INSERT INTO `guildhouses` VALUES ('32', '0', '-11073', '-1956', '39', '0', 'Karazhan Smiley');
INSERT INTO `guildhouses` VALUES ('33', '0', '-11084', '-1801', '53', '0', 'Well of the Forgotten (Aka. Karazhan Crypt or Lower Karazhan)');
INSERT INTO `guildhouses` VALUES ('34', '0', '1683.235474', '286.458801', '-45.451775', '0', 'Undercity Top Tier');
INSERT INTO `guildhouses` VALUES ('35', '0', '-8521.3', '599.5', '101.399338', '0', 'Stormwind Cut-Throat Alley');
INSERT INTO `guildhouses` VALUES ('36', '0', '-5933', '452', '509', '0', 'Forgotten gnome camp');
INSERT INTO `guildhouses` VALUES ('37', '0', '-920.231323', '7096.489258', '170.35289', '530', 'Outland Nagrand : Newton\'s Remains');
INSERT INTO `guildhouses` VALUES ('38', '0', '-2140.501953', '9142.6875', '137.041855', '530', 'Outland Nagrand : Tomb');
INSERT INTO `guildhouses` VALUES ('39', '0', '-483.401794', '7461.944824', '186.120987', '530', 'Outland Nagrand: Challe\'s Home for Little Tykes');
INSERT INTO `guildhouses` VALUES ('40', '0', '2387.753906', '3191.757324', '152.669388', '530', 'Outland Netherstorm: Nova\'s Shrine');
