DELETE FROM `spell_bonus_data` WHERE `entry` = 54172;
INSERT INTO `spell_bonus_data` VALUES
(54172,0,0,0,0,'Paladin - Divine Storm');

DELETE FROM `spell_dbc` WHERE `Id` = 99999;
INSERT INTO `spell_dbc` (Id, CastingTimeIndex, RangeIndex, Effect1, EffectImplicitTargetA1, EffectApplyAuraName1, SpellFamilyName, Comment) VALUES
(99999,1,1,6,1,4,10,'Divine Storm Helper (SERVERSIDE)');

DELETE FROM `spell_proc_event` WHERE `entry` = 99999;
INSERT INTO `spell_proc_event` VALUES
(99999,0,10,0,131072,0,16,0,0,100,0);
