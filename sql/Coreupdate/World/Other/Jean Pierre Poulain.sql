DELETE FROM `gossip_menu_option` WHERE `menu_id`=10478; 
INSERT INTO `gossip_menu_option` (`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_script_id`) VALUES 
(10478,0,0, 'I''ll take the flight.',1,1,0,1047800); 
DELETE FROM `gossip_scripts` WHERE `id`=1047800; 
INSERT INTO `gossip_scripts` (`id`,`delay`,`command`,`datalong`,`datalong2`) VALUES 
(1047800,0,15,64795,1); 
-- Update Jean Pierre Poulain 
UPDATE `creature_template` SET `gossip_menu_id`=10478 WHERE `entry`=34244;
DELETE FROM gossip_scripts WHERE id=10478;