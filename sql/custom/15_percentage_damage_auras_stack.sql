-- remove already existing wrong rules
DELETE FROM `spell_group` WHERE `id` IN (1055,1056,1057);
DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1057;

DELETE FROM `spell_group` WHERE `id` IN (2000,2001,2002,2003);
INSERT INTO `spell_group` VALUES
-- Percentage Damage Increase
(2000,63531), -- Sanctified Retribution
(2001,31579), -- Arcane Empowerment
(2002,75593), -- Ferocious Inspiration
(2003,-2000),
(2003,-2001),
(2003,-2002);

DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 2003;
INSERT INTO `spell_group_stack_rules` VALUES (2003,1);
