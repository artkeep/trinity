UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=65005;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=65013;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60442;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60493;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60487;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60529;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60436;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60537;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=67670;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=33953;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=62115;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=67672;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=33648;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=57352;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=57345;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60490;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=62114;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=59818;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=49622;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=33648;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60063;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=59787;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=60063;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=58901;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=67667;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=67653;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=33953;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=64786;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=64714;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=64738;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71404;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=64792;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=65025;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=65020;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=64742;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=65002;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=64764;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71642;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71640;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71562;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71637;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71545;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71611;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71634;
UPDATE `spell_proc_event` SET Cooldown=105 WHERE entry=71519;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71606;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71585;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71406;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=41540;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=67771;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71402;
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=67702;

-- My aditional fixes
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71645; -- Unidentifiable Organ
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71602; -- Unidentifiable Organ (HC)
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71573; -- Muradin's Spyglass
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71571; -- Muradin's Spyglass (HC)
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71578; -- Dislodged Foreign Object
UPDATE `spell_proc_event` SET Cooldown=45 WHERE entry=71576; -- Dislodged Foreign Object (HC)
UPDATE `spell_proc_event` SET Cooldown=60 WHERE entry=72415; -- Ashen Band of Unmatched(Endless) Courage
UPDATE `spell_proc_event` SET Cooldown=60 WHERE entry=72417; -- Ashen Band of Unmatched(Endless) Destruction(Wisdom)
UPDATE `spell_proc_event` SET Cooldown=60 WHERE entry=72413; -- Ashen Band of Unmatched(Endless) Vengeance(Might)

DELETE FROM `spell_proc_event` WHERE `entry` IN (75455,75474,75465,75481,75475);
INSERT INTO `spell_proc_event` VALUES
(75455, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,   0,   0,  45),
(75474, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,   0,   0,  45),
(75465, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,   0,   0,  45),
(75481, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,   0,   0,  30),
(75475, 0x00,   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,   0,   0,  30);

DELETE FROM `spell_proc_event` WHERE `entry` IN ('67712'); INSERT INTO `spell_proc_event` (`entry`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`procFlags`,`procEx`,`ppmRate`,`CustomChance`,`Cooldown`) VALUES ( 67712, 0x00, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0, 0, 2);