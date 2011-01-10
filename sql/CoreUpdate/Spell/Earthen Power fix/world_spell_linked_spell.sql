DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN ('3600');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
('3600','199999','0','Earthbind');
