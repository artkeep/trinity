-- [10446-10447]The_Final_Code.sql
UPDATE `gameobject_template` SET `ScriptName`='npc_mana_bomb' WHERE `entry`=184725;

-- [11085]Quest_Escape_From_Skettis.sql
-- Fix Quest Soporte Escape from Skettis http://www.wowhead.com/quest=11085
UPDATE `creature_template` SET `ScriptName`='npc_skyguard_prisoner' WHERE `entry`=23383;
DELETE FROM `script_texts` WHERE `entry` IN (-1000716,-1000717,-1000718) AND `npc_entry`=23383; 
INSERT INTO `script_texts` (`npc_entry`,`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
(23383,-1000716,'Thanks for your help. Let\'s get out of here!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'SAY_START'),
(23383,-1000717,'Let\'s keep moving. I don\'t like this place.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'SAY_DONT_LIKE'),
(23383,-1000718,'Thanks again. Sergeant Doryn will be glad to hear he has one less scout to replace this week.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'SAY_COMPLETE');

-- [10922]Quest_Digging_Through_Bones.sql
-- Fix Quest Soporte http://www.wowhead.com/quest=10922 "Digging Through Bones"
UPDATE `creature_template` SET `ScriptName`='npc_letoll' WHERE `entry`=22458;
DELETE FROM `script_texts` WHERE `entry` BETWEEN -1000531 AND -1000525 AND `npc_entry`=22458; 
INSERT INTO `script_texts` (`npc_entry`,`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
(22458,-1000526,'Shut yer yaps! I\'m gonna bang on this drum and that\'s that!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'letoll SAY_LE_SHUT'),
(22458,-1000527,'Say, do you guys hear that?',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'letoll SAY_LE_REPLY_HEAR'),
(22458,-1000528,'IN YOUR FACE! I told you there was somethin\' here!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'letoll SAY_LE_IN_YOUR_FACE'),
(22458,-1000529,'Don\'t just stand there! Help him out!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'letoll SAY_LE_HELP_HIM'),
(22458,-1000530,'%s picks up the drum.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'letoll EMOTE_LE_PICK_UP'),
(22458,-1000531,'You\'ve been a tremendous help, $r! Let\'s get out of here before more of those things show up! I\'ll let Dwarfowitz know you did the job asked of ya\' admirably.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'letoll SAY_LE_THANKS');
DELETE FROM `script_waypoint` WHERE `entry`=22458;
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) 
VALUES
(22458,0,-3739.907959,5393.691895,-4.213,5000, 'SAY_LE_KEEP_SAFE'),
(22458,1,-3733.334229,5389.243164,-5.331,0,''),
(22458,2,-3728.771729,5385.649414,-3.704,0,''),
(22458,3,-3717.267090,5379.179199,-4.400,0,''),
(22458,4,-3705.626465,5379.261719,-7.711,0,''),
(22458,5,-3688.279541,5379.716309,-9.400,0,''),
(22458,6,-3649.186523,5389.111816,-11.917,0,''),
(22458,7,-3612.791504,5392.812500,-13.655,0,''),
(22458,8,-3574.865479,5412.704590,-16.543,0,''),
(22458,9,-3564.438232,5422.615723,-16.104,0,''),
(22458,10,-3553.387695,5444.732910,-12.184,2500,'arivve dig site SAY_LE_ARRIVE'),
(22458,11,-3557.291016,5465.319336,-9.282,7500,'dig 1'),
(22458,12,-3548.102051,5453.417969,-12.282,10000,'dig 2 SAY_LE_BURIED pause'),
(22458,13,-3556.580322,5446.475098,-11.920,0,'start returning'),
(22458,14,-3564.438232,5422.615723,-16.104,0,''),
(22458,15,-3574.865479,5412.704590,-16.543,0,''),
(22458,16,-3612.791504,5392.812500,-13.655,0,''),
(22458,17,-3649.186523,5389.111816,-11.917,0,''),
(22458,18,-3688.279541,5379.716309,-9.400,0,''),
(22458,19,-3705.626465,5379.261719,-7.711,0,''),
(22458,20,-3717.267090,5379.179199,-4.400,0,''),
(22458,21,-3728.771729,5385.649414,-3.704,0,''),
(22458,22,-3733.334229,5389.243164,-5.331,0,''),
(22458,23,-3739.907959,5393.691895,-4.213,0,'');