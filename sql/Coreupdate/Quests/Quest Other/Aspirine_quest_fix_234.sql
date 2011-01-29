DELETE FROM `creature_questrelation` WHERE `quest` = 13625;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 13625;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 13625;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (33647, 13625);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 33647;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 13625;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 13625;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (33647, 13625);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=33647;
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 33229, `ReqCreatureOrGOId2` = 33243, `ReqCreatureOrGOId3` = 33272, `ReqSpellCast1` = 62544, `ReqSpellCast2` = 62575, `ReqSpellCast3` = 63010 WHERE `entry` = 13625;

DELETE FROM `creature_questrelation` WHERE `quest` = 13677;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 13677;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 13677;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (33659, 13677);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 33659;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 13677;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 13677;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (33659, 13677);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=33659;
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 33229, `ReqCreatureOrGOId2` = 33243, `ReqCreatureOrGOId3` = 33272, `ReqSpellCast1` = 62544, `ReqSpellCast2` = 62575, `ReqSpellCast3` = 63010 WHERE `entry` = 13677;

DELETE FROM `creature_questrelation` WHERE `quest` = 13828;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 13828;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 13828;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (33625, 13828);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 33625;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 13828;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 13828;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (33625, 13828);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=33625;
UPDATE `quest_template` SET `ReqCreatureOrGOId2` = 33229, `ReqSpellCast2` = 62544 WHERE `entry` = 13828;

DELETE FROM `creature_questrelation` WHERE `quest` = 13829;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 13829;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 13829;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (33542, 13829);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 33542;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 13829;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 13829;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (33542, 13829);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=33542;
UPDATE `quest_template` SET `ReqCreatureOrGOId2` = 33229, `ReqSpellCast2` = 62544 WHERE `entry` = 13829;

DELETE FROM `creature_questrelation` WHERE `quest` = 13835;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 13835;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 13835;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (33647, 13835);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 33647;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 13835;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 13835;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (33647, 13835);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=33647;
UPDATE `quest_template` SET `ReqCreatureOrGOId2` = 33243, `ReqSpellCast2` = 62575 WHERE `entry` = 13835;


DELETE FROM `creature_questrelation` WHERE `quest` = 13837;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 13837;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 13837;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (33646, 13837);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 33646;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 13837;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 13837;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (33646, 13837);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=33646;
UPDATE `quest_template` SET `ReqCreatureOrGOId2` = 33272, `ReqSpellCast2` = 63010 WHERE `entry` = 13837;


DELETE FROM `creature_questrelation` WHERE `quest` = 13838;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 13838;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 13838;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (33659, 13838);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 33659;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 13838;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 13838;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (33659, 13838);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=33659;
UPDATE `quest_template` SET `ReqCreatureOrGOId2` = 33243, `ReqSpellCast2` = 62575 WHERE `entry` = 13838;

DELETE FROM `creature_questrelation` WHERE `quest` = 13839;
DELETE FROM `gameobject_questrelation` WHERE `quest` = 13839;
UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 13839;
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (33658, 13839);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 33658;
DELETE FROM `creature_involvedrelation` WHERE `quest` = 13839;
DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 13839;
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (33658, 13839);
UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=33658;
UPDATE `quest_template` SET `ReqCreatureOrGOId2` = 33272, `ReqSpellCast2` = 63010 WHERE `entry` = 13839;
