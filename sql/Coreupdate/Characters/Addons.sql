SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for addons
-- ----------------------------
CREATE TABLE `addons` (
  `name` varchar(120) NOT NULL DEFAULT '',
  `crc` int(32) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Addons';

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `addons` VALUES ('Blizzard_AchievementUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_ArenaUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_AuctionUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_BarbershopUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_BattlefieldMinimap', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_BindingUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_Calendar', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_CombatLog', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_CombatText', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_DebugTools', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_GMChatUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_GMSurveyUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_GlyphUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_GuildBankUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_InspectUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_ItemSocketingUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_MacroUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_RaidUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_TalentUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_TimeManager', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_TokenUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_TradeSkillUI', '1276933997');
INSERT INTO `addons` VALUES ('Blizzard_TrainerUI', '1276933997');