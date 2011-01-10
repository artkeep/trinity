DELETE FROM `spell_proc_event` WHERE `entry` IN ('53695', '53696');
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
('53695','0','10','8388608','0','8','0','0','0','0','0'), -- Judgements of the Just (Rank 1)
('53696','0','10','8388608','0','8','0','0','0','0','0'); -- Judgements of the Just (Rank 2)
