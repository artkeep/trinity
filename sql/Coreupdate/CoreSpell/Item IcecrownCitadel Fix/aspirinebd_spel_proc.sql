-- (56614) Погром (Уровень 5)
DELETE FROM `spell_proc_event` WHERE `entry` IN (56614);
INSERT INTO `spell_proc_event` VALUES (56614, 0x01, 0x00, 0x00000000, 0x00000000, 0x00000000, 0x00000014, 0x00000000, 0, 100, 0);
-- (16689) Хватка природы (Уровень 1)
DELETE FROM `spell_proc_event` WHERE `entry` IN (16689);
INSERT INTO `spell_proc_event` VALUES (16689, 0x08, 0x07, 0x00000000, 0x00000000, 0x00000000, 0x00000028, 0x00000000, 0, 100, 0);