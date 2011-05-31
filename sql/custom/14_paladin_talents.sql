DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = -31869;
INSERT INTO `spell_linked_spell` VALUES (-31869, -63531, 0, 'Sanctified Retribution at talent reset');

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (-20254,-20255,-20256);
INSERT INTO `spell_linked_spell` VALUES
(-20254,-63510,0,'Improved Concentration Aura at talent reset'),
(-20255,-63510,0,'Improved Concentration Aura at talent reset'),
(-20256,-63510,0,'Improved Concentration Aura at talent reset');

UPDATE `spell_group` SET `spell_id` = 63514 WHERE `id` = 1095;
