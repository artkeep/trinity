DELETE FROM `spell_proc_event` WHERE `entry` = 34477;
INSERT INTO `spell_proc_event` (`entry`,`cooldown`) VALUES (34477,30);

-- remove this when you find a way how to make 35079 to redirect aggro
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = -35079;
INSERT INTO `spell_linked_spell` VALUES (-35079,-34477,0,'Misdirection');
