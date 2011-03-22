DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (12129,36561);
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
('1212901','12129','0','0','100','7','7800','8200','9400','9600','11','15284','1','1','0','0','0','0','0','0','0','0','Onyxian Warder - Cast Cleave'),
('1212902','12129','0','0','100','7','9200','9300','8400','8500','11','20203','0','1','0','0','0','0','0','0','0','0','Onyxian Warder - Cast Fire Nova'),
('1212903','12129','0','0','100','7','11500','11700','23400','23600','11','12097','1','1','0','0','0','0','0','0','0','0','Onyxian Warder - Cast Pierce Armor'),
('1212904','12129','0','0','100','7','12600','12800','5800','6000','11','18958','1','1','0','0','0','0','0','0','0','0','Onyxian Warder - Cast Flame Lash'),
('1212905','12129','0','0','100','7','9200','9300','8400','8500','11','68969','0','1','0','0','0','0','0','0','0','0','Onyxian Warder - Cast Fire Nova'),
('1212906','12129','0','0','100','7','12600','12800','5800','6000','11','69308','1','1','0','0','0','0','0','0','0','0','Onyxian Warder - Cast Flame Lash'),
(3656101,36561,0,0,75,7,15000,15000,30000,30000,11,68958,0,0,0,0,0,0,0,0,0,0, 'Onyxia Lair Guard - Cast Blast Nova'),
(3656102,36561,0,0,90,7,5000,5000,20000,20000,11,68960,1,0,0,0,0,0,0,0,0,0, 'Onyxia Lair Guard - Cast Ignite Weapon'),
(3656103,36561,0,0,80,7,7000,7000,9000,11000,11,15284,1,0,0,0,0,0,0,0,0,0, 'Onyxia Lair Guard - Cast Cleave' );

UPDATE creature_template SET  AIName='EventAI' WHERE entry IN (12129,36561);