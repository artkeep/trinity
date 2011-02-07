-- Фиксы к Квестам

-- Тело и дух (http://ru.wowhead.com/quest=6001/)

REPLACE INTO `quest_template` VALUES
(6001, 2, -263, 0, 10, 0, 10, 0, 1101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 5931, 0, 0, 0, 5, 15208, 1, 0, 'Body and Heart', 'Directly east of Auberdine lay many moonkin caves.  The one closest to Auberdine has a Moonkin Stone inside it.  Lunaclaw, the defender of the stone, will face you only when Cenarion Moondust is applied to it.  Lunaclaw possesses within it a strength that you must use as one of the Claw.$B$BYou''ve earned the right to use this Cenarion Moondust, $N... now show me that you''ve earned the right to possess the strength of body and the strength of heart needed as a druid!', 'Use the Cenarion Moondust on the Moonkin Stone of Auberdine to bring forth Lunaclaw.  From there, you must face Lunaclaw and earn the strength of body and heart it possesses.$B$BSpeak with Mathrengyl Bearwalker in Darnassus when you are done.', 'You have finally taken your large step into a much larger world, $N. I sense the teaching of the Great Bear Spirit within you, and I sense that you have received the strength that Lunaclaw possessed.$B$BThere are no further obstacles in your way... let me now teach you what it means to be a $C of the Claw!', NULL, 'Face Lunaclaw and earn the strength of body and heart it possesses.', 'Return to Mathrengyl Bearwalker at Cenarion Enclave in Darnassus.', '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12138, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 609, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 132300, 0, 19179, 112, 21600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = '12138',`ReqCreatureOrGOCount1` = '1' WHERE `entry` =6002;
REPLACE INTO `creature_template` VALUES
(12138, 0, 0, 0, 12138, 0, 9389, 0, 0, 0, 'Lunaclaw', '', '', 0, 12, 12, 0, 14, 14, 0, 1.14, 1.14286, 1, 0, 13, 19, 0, 5, 1.2, 2000, 2000, 1, 32768, 0, 0, 0, 0, 0, 0, 10, 15, 4, 7, 0, 12138, 0, 0, 0, 0, 0, 0, 0, 0, 3391, 18986, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 'EventAI', 0, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 11723);
REPLACE INTO `creature_template_addon` VALUES(12138, 0, 0, 0, 4097, 0, NULL);

-- Кусочки головоломки (http://ru.wowhead.com/quest=12926)

UPDATE `quest_template` SET `SrcItemId` = '40971',
`SrcItemCount` = '1' WHERE `entry` =12926;

-- Перепроклятие (http://ru.wowhead.com/quest=11712)

UPDATE `quest_template` SET `ReqCreatureOrGOId1` = '25814' WHERE `entry` =11712;
UPDATE `quest_template` SET `ReqSpellCast1` = '45980' WHERE `entry` =11712;

-- Последние почести (http://ru.wowhead.com/quest=11593)

UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` =25351;
REPLACE INTO `creature_ai_scripts` VALUES (2535100, 25351, 8, 0, 100, 0, 45474, -1, 0, 0, 33, 25351, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'q11593 The Honored Dead');
REPLACE INTO `conditions` VALUES (13, 0, 45474, 0, 18, 1, 25351, 0, 0, '', NULL);
UPDATE `quest_template` SET `ReqSpellCast1`=45474 WHERE `entry`=11593;
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = '25351' WHERE `entry` =11593;

-- На этот раз ты нарвался, Кул (http://ru.wowhead.com/quest=14096) и (http://ru.wowhead.com/quest=14142)
UPDATE `quest_template` SET `ReqCreatureOrGOId1`='34956', `ReqSpellCast1`='0', `ReqCreatureOrGOId2`='34716' WHERE `entry`='14096';
UPDATE `quest_template` SET `ReqCreatureOrGOId1`='34956', `ReqSpellCast1`='0', `ReqCreatureOrGOId2`='34716' WHERE `entry`='14142'; 
