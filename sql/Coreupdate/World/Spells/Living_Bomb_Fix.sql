DELETE FROM `spell_bonus_data` WHERE `entry` IN ('44457','44461');
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
('44457','0','0.2','0','0','Mage - Living Bomb'),
('44461','0.4','0','0','0','Mage - Living Bomb');

DELETE FROM `spell_proc_event` WHERE `entry` IN ('44449','44469','44470','44471','44472','44445','44446','44448');
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
('44449','0','3','551686903','233544','0','0','2','0','0','0'), -- Burnout (Rank 1)
('44469','0','3','551686903','233544','0','0','2','0','0','0'), -- Burnout (Rank 2)
('44470','0','3','551686903','233544','0','0','2','0','0','0'), -- Burnout (Rank 3)
('44471','0','3','551686903','233544','0','0','2','0','0','0'), -- Burnout (Rank 4)
('44472','0','3','551686903','233544','0','0','2','0','0','0'), -- Burnout (Rank 5)
('44445','0','3','19','69632','0','0','0','0','0','0'), -- Hot Streak (Rank 1)
('44446','0','3','19','69632','0','0','0','0','0','0'), -- Hot Streak (Rank 2)
('44448','0','3','19','69632','0','0','0','0','0','0'); -- Hot Streak (Rank 3)