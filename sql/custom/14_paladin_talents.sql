DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = -31869;
INSERT INTO `spell_linked_spell` VALUES (-31869, -63531, 0, 'Sanctified Retribution at talent reset');

UPDATE `spell_group` SET `spell_id` = 63514 WHERE `id` = 1095;
