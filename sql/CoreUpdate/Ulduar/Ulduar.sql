REPLACE INTO `instance_template` (map, parent, script, allowMount) VALUES ('603', '571', 'instance_ulduar', '1');

UPDATE `gameobject_template` SET `ScriptName` = 'ulduar_teleporter' WHERE `entry` = 194569;
-- Algalon
UPDATE creature_template SET scriptname="mob_collapsing_star" WHERE entry=32955;
UPDATE creature_template SET ScriptName="boss_algalon_the_observer" WHERE entry=32871;
UPDATE creature_template SET ScriptName="mob_living_constellation" WHERE entry=33052;
UPDATE creature_template SET ScriptName="mob_black_hole" WHERE entry=32953;
UPDATE creature_template SET ScriptName="mob_cosmic_smash_target" WHERE entry=33104;
UPDATE creature_template SET ScriptName="mob_dark_mattery" WHERE entry=33089;

-- Assembly of Iron
UPDATE creature_template SET ScriptName = "boss_steelbreaker" WHERE entry = 32867;
UPDATE creature_template SET ScriptName = "boss_runemaster_molgeim" WHERE entry = 32927;
UPDATE creature_template SET ScriptName = "boss_stormcaller_brundir" WHERE entry = 32857;
UPDATE creature_template SET faction_A=16, faction_H=16, scriptname='mob_lightning_elemental', difficulty_entry_1 = 33689 WHERE entry = 32958;
UPDATE creature_template SET ScriptName = "mob_rune_of_summoning" WHERE entry = 33051;
UPDATE creature_template SET ScriptName = "mob_rune_of_power" WHERE entry = 33705;

-- Auriaya
UPDATE creature_template SET ScriptName = "boss_auriaya" WHERE entry = 33515;
UPDATE creature_template SET `faction_A` = 16, `faction_H` = 16, ScriptName = 'feral_defender_trigger' WHERE `entry` = 34096;
UPDATE creature_template SET ScriptName = "mob_sanctum_sentry" WHERE entry = 34014;
UPDATE creature_template SET ScriptName = "mob_feral_defender" WHERE entry = 34035;
UPDATE creature_template SET ScriptName = "seeping_trigger" WHERE entry = 34098;

-- Flame Leviathan
UPDATE creature_template SET ScriptName = "boss_flame_leviathan" WHERE entry = 33113;
UPDATE creature_template SET ScriptName = "boss_flame_leviathan_seat" WHERE entry = 33114;
UPDATE creature_template SET ScriptName = "boss_flame_leviathan_defense_turret" WHERE entry = 33139;
UPDATE creature_template SET ScriptName = "boss_flame_leviathan_overload_device" WHERE entry = 33189;
UPDATE creature_template SET ScriptName = "boss_flame_leviathan_safety_container" WHERE entry = 33218;
UPDATE creature_template SET ScriptName = "spell_pool_of_tar" WHERE entry = 33090;
UPDATE creature_template SET ScriptName = "keeper_norgannon" WHERE entry = 33686;
UPDATE creature_template SET ScriptName = "mob_colossus" WHERE entry = 33237;
-- Correct position
DELETE FROM `creature` WHERE `id`=33113;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(33113, 603, 3, 65535, 0, 0, 435.89, -8.417, 409.886, 3.12723, 480000, 0, 0, 23009250, 0, 0, 0);
-- Delete extra Colossus
DELETE FROM creature WHERE guid IN (128936,128937);
-- Lore Keeper of Norgannon must be gossip
UPDATE creature_template SET npcflag=npcflag |1 WHERE entry=33686;
-- System Shutdown Stun (thx Bloodycore)
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=62475;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
('62475','9032','2','System Shutdown');
-- Fix Repair Station
UPDATE gameobject_template SET ScriptName='' WHERE entry=194261;
DELETE FROM areatrigger_scripts WHERE entry IN (5369, 5423);
INSERT INTO areatrigger_scripts VALUES
(5369,'at_RX_214_repair_o_matic_station'),
(5423,'at_RX_214_repair_o_matic_station');
-- This mob should be invisible
UPDATE creature_template SET flags_extra=flags_extra |128 WHERE entry=33377;

-- Freya
UPDATE creature_template SET ScriptName = "boss_freya" WHERE entry = 32906;
UPDATE creature_template SET ScriptName = "boss_elder_brightleaf" WHERE entry = 32915;
UPDATE creature_template SET `modelid1` = 11686, `modelid2` = 0, `unit_flags` = 33686022, ScriptName = 'creature_unstable_sun_beam' WHERE `entry` = 33050;
UPDATE creature_template SET `unit_flags` = 33685510, `modelid1` = 11686, `modelid2` = 0, ScriptName = 'creature_sun_beam' WHERE `entry` =33170;
UPDATE creature_template SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 393220, ScriptName = 'creature_iron_roots' WHERE `entry` = 33168;
UPDATE creature_template SET ScriptName = "boss_elder_ironbranch" WHERE entry = 32913;
UPDATE creature_template SET `unit_flags` = 393220, ScriptName = 'creature_eonars_gift' WHERE `entry` =33228;
UPDATE creature_template SET ScriptName = "boss_elder_stonebark" WHERE entry = 32914;
UPDATE creature_template SET `modelid1` = 11686, `modelid2` = 0, `unit_flags` = 33685508, ScriptName = 'creature_nature_bomb' WHERE `entry` =34129;
UPDATE creature_template SET ScriptName = 'creature_detonating_lasher' WHERE `entry` =32918;
UPDATE creature_template SET ScriptName = 'creature_ancient_conservator' WHERE `entry` =33203;
UPDATE creature_template SET `unit_flags` = 33686022, ScriptName = 'creature_healthy_spore' WHERE `entry` =33215;
UPDATE creature_template SET ScriptName = 'creature_storm_lasher' WHERE `entry` =32919;
UPDATE creature_template SET ScriptName = 'creature_snaplasher' WHERE `entry` =32916;
UPDATE creature_template SET ScriptName = 'creature_ancient_water_spirit' WHERE `entry` =33202;

-- General Vezax
UPDATE creature_template SET `mechanic_immune_mask` = 617299803, `flags_extra` = 257, ScriptName = 'boss_general_vezax' WHERE `entry` = 33271;
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, `mechanic_immune_mask` = 650854235, ScriptName = 'mob_saronite_vapors' WHERE `entry` = 33488;
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, `mechanic_immune_mask` = 650854235, ScriptName = 'mob_saronite_animus' WHERE `entry` = 33524;
UPDATE creature_template SET `modelid1` = 28470, `modelid2` = 0, ScriptName = 'mob_icicle' WHERE `entry` = 33169;
UPDATE creature_template SET `modelid1` = 28470, `modelid2` = 0, ScriptName = 'mob_icicle_snowdrift' WHERE `entry` = 33173;

-- Hodir
UPDATE creature_template SET ScriptName = "boss_hodir" WHERE entry=32845;
UPDATE creature_template SET ScriptName = 'mob_hodir_priest' WHERE `entry` IN (32897, 33326, 32948, 33330);
UPDATE creature_template SET ScriptName = 'mob_hodir_shaman' WHERE `entry` IN (33328, 32901, 33332, 32950);
UPDATE creature_template SET ScriptName = 'mob_hodir_druid' WHERE `entry` IN (33325, 32900, 32941, 33333);
UPDATE creature_template SET ScriptName = 'mob_hodir_mage' WHERE `entry` IN (32893, 33327, 33331, 32946);
UPDATE creature_template SET `modelid1` = 15880, `modelid2` = 0, ScriptName = 'toasty_fire' WHERE `entry` = 33342;

-- Ignis
UPDATE creature_template SET ScriptName = "boss_ignis" WHERE entry = 33118;
UPDATE creature_template SET ScriptName = "mob_iron_construct" WHERE entry = 33121;
UPDATE creature_template SET `modelid1` = 16925, `modelid2` = 0, ScriptName = 'mob_scorch_ground' WHERE `entry` = 33221;

-- Kologarn
UPDATE creature_template SET ScriptName = "boss_kologarn" WHERE entry = 32930;
UPDATE creature_template SET `faction_A` = 16, `faction_H` = 16, ScriptName = 'mob_focused_eyebeam' WHERE `entry` IN (33632, 33802);
UPDATE creature_template SET ScriptName = "mob_right_arm" WHERE entry = 32934;
UPDATE creature_template SET ScriptName = "mob_left_arm" WHERE entry = 32933;


-- Razorscale
UPDATE `creature_template` SET `ScriptName` = 'boss_razorscale' WHERE `entry`=33186;
UPDATE `creature_template` SET `ScriptName` = 'npc_devouring_flame' WHERE `entry`=34188;
UPDATE `creature_template` SET `ScriptName` = 'npc_mole_machine_trigger' WHERE `entry`=33245;
UPDATE `creature_template` SET `ScriptName` = 'npc_expedition_commander' WHERE `entry`=33210;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkrune_guardian' WHERE `entry`=33388;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkrune_sentinel' WHERE `entry`=33846;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkrune_watcher' WHERE `entry`=33453;

-- Thorim
UPDATE creature_template SET ScriptName = "boss_thorim" WHERE entry=32865;
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, ScriptName = 'mob_pre_phase' WHERE `entry` IN (32882, 32908, 32885, 33110);
UPDATE creature_template SET `unit_flags` = 0, `faction_A` = 14, `faction_H` = 14, ScriptName = 'mob_arena_phase' WHERE `entry` IN (32876, 32904, 32878, 32877, 32874, 32875);
UPDATE creature_template SET `unit_flags` = 0, ScriptName = 'mob_runic_colossus' WHERE `entry` = 32872;
UPDATE creature_template SET `unit_flags` = 0, ScriptName = 'mob_rune_giant' WHERE `entry` = 32873;
UPDATE creature_template SET `modelid1` = 16925, `modelid2` = 0, ScriptName = 'thorim_trap_bunny' WHERE `entry` = 33725;
UPDATE creature_template SET `flags_extra` = 0, ScriptName = 'thorim_energy_source' WHERE `entry` = 32892;
UPDATE creature_template SET `dmgschool` = 4, `dmg_multiplier` = 7.5, `unit_flags` = 33587202, ScriptName = 'npc_sif' WHERE `entry` = 33196;

-- XT-002
UPDATE creature_template SET ScriptName = "boss_xt002" WHERE entry = 33293;
UPDATE creature_template SET ScriptName = "mob_xt002_heart" WHERE entry=33329;
UPDATE creature_template SET ScriptName = "mob_scrapbot" WHERE entry = 33343;
UPDATE creature_template SET ScriptName = "mob_pummeller" WHERE entry=33344;
UPDATE creature_template SET ScriptName = "mob_boombot" WHERE entry = 33346;
UPDATE creature_template SET ScriptName = "mob_void_zone" WHERE entry = 34001;
UPDATE creature_template SET ScriptName = "mob_life_spark" WHERE entry = 34004;

-- Some instance data
SET @entry=60000;
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
(@entry,'0','0','0','0','0','169','16925','0','0','Thorim Event Bunny','','','0','80','80','2','14','14','0','1','1.14286','1','1','422','586','0','642','7.5','2000','0','1','33554432','8','0','0','0','0','0','345','509','103','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','thorim_phase_trigger','11723');

-- Algalon
REPLACE INTO script_texts
(entry, content_default, sound, TYPE, LANGUAGE, emote, COMMENT) VALUES
(-1615172, "Trans-location complete. Commencing planetary analysis of Azeroth.", 15405, 0, 0, 0, "Algalon: Intro 1"),
(-1615173, "Stand back, mortals. I am not here to fight you.", 15406, 0, 0, 0, "Algalon: Intro 2"),
(-1615174, "It is in the universe's best interest to re-originate this planet should my analysis find systemic corruption. Do not interfere.", 15407, 0, 0, 0, "Algalon: Intro 3"),
(-1615175, "See your world through my eyes: A universe so vast as to be immeasurable - incomprehensible even to your greatest minds.", 15390, 1, 0, 0, "Algalon: Engaged for the first time"),
(-1615176, "Your actions are illogical. All possible results for this encounter have been calculated. The Pantheon will receive the Observer's message regardless of outcome.", 15386, 1, 0, 0, "Algalon: Aggro"),
(-1615177, "The stars come to my aid.", 15392, 1, 0, 0, "Algalon: Summoning Collapsing Stars"),
(-1615178, "Witness the fury of cosmos!", 15396, 1, 0, 0, "Algalon: Big Bang 1"),
(-1615179, "Behold the tools of creation!", 15397, 1, 0, 0, "Algalon: Big Bang 2"),
(-1615180, "Beware!", 15391, 1, 0, 0, "Algalon: Phase 2"),
(-1615181, "Loss of life, unavoidable.", 15387, 1, 0, 0, "Algalon: Killing a player 1"),
(-1615182, "I do what I must.", 15388, 1, 0, 0, "Algalon: Killing a player 2"),
(-1615183, "You are... out of time.", 15394, 1, 0, 0, "Algalon: Berserk"),
(-1615184, "Analysis complete. There is partial corruption in the planet's life-support systems as well as complete corruption in most of the planet's defense mechanisms.", 15398, 0, 0, 0, "Algalon: Despawn 1"),
(-1615185, "Begin uplink: Reply Code: 'Omega'. Planetary re-origination requested.", 15399, 0, 0, 0, "Algalon: Despawn 2"),
(-1615186, "Farewell, mortals. Your bravery is admirable, for such flawed creatures.", 15400, 0, 0, 0, "Algalon: Despawn 3"),
(-1615187, "I have seen worlds bathed in the Makers' flames. Their denizens fading without so much as a whimper. Entire planetary systems born and raised in the time that it takes your mortal hearts to beat once. Yet all throughout, my own heart, devoid of emotion... of empathy. I... have... felt... NOTHING! A million, million lives wasted. Had they all held within them your tenacity? Had they all loved life as you do?", 15393, 0, 0, 0, "Algalon: Defeated 1"),
(-1615188, "Perhaps it is your imperfection that which grants you free will. That allows you to persevere against cosmically calculated odds. You prevailed where the Titans' own perfect creations have failed.", 15401, 0, 0, 0, "Algalon: Defeated 2"),
(-1615189, "I've rearranged the reply code. Your planet will be spared. I cannot be certain of my own calculations anymore.", 15402, 0, 0, 0, "Algalon: Defeated 3"),
(-1615190, "I lack the strength to transmit the signal. You must hurry. Find a place of power close to the skies.", 15403, 0, 0, 0, "Algalon: Defeated 4"),
(-1615191, "Do not worry about my fate $N. If the signal is not transmitted in time re-origination will proceed regardless. Save. Your. World.", 15403, 0, 0, 0, "Algalon: Defeated 5");



DELETE FROM `script_texts` WHERE `entry` IN (-1603268,-1603267,-1603266);
INSERT INTO `script_texts` (`npc_entry`, `entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
(33186, -1603268, 'Razorscale grounded permanently!', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,2,0,0, 'EMOTE_PERMA'),
(33186, -1603267, 'Razorscale takes a deep breath...', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,2,0,0, 'EMOTE_BREATH'),
(33287, -1603266, 'Harpoon Turret is ready for use!', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,2,0,0, 'EMOTE_HARPOON');

DELETE FROM `script_texts` WHERE `entry` IN (-1603265,-1603264,-1603263,-1603262);
INSERT INTO `script_texts` (`npc_entry`,`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
(33287, -1603265, 'Fires out! Let''s rebuild those turrets!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1,0,0, 'SAY_TURRETS'),
(33287, -1603264, 'Ready to move out, keep those dwarves off of our backs!', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1,0,0, 'SAY_AGGRO_3'),
(33287, -1603263, 'Be on the lookout! Mole machines will be surfacing soon with those nasty Iron dwarves aboard!', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1,0,0, 'SAY_AGGRO_2'),
(33287, -1603262, 'Give us a moment to prepare to build the turrets.', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1,0,0, 'SAY_AGGRO_1');

-- Razorscale emotes
UPDATE `script_texts` SET `content_default` = "Welcome, champions! All of our attempts at grounding her have failed. We could use a hand in bring her down with these harpoon guns.", `type`=0  WHERE `entry`=-1603260;
UPDATE `script_texts` SET `content_default` = "Move! Quickly! She won't remain grounded for long.", `type`=1  WHERE `entry`=-1603261;




-- Yogg Saron-- 
UPDATE `creature_template` SET `unit_flags` = 0, `type_flags` = 0, `mechanic_immune_mask` = 650854235, `ScriptName` = 'boss_sara' WHERE `entry` = 33134;
UPDATE creature_model_info SET bounding_radius = 0.465, combat_reach = 45 WHERE modelid = 29117;
UPDATE `creature` SET `spawndist` = 0 WHERE `id` = 33134;
-- Ominous cloud
DELETE FROM `creature` WHERE `id`=33292;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33947654, `type_flags` = 0, `speed_walk` = 0.2, modelid1 = 28549, modelid2 = 0, `ScriptName` = 'npc_ominous_cloud' WHERE `entry` IN (33292, 32406);
-- Guardian of Yogg-Saron
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `faction_A` = 16, `faction_H` = 16, `mindmg` = 123, `maxdmg` = 123, `attackpower` = 123, `dmg_multiplier` = 123, `baseattacktime` = 1800, `mechanic_immune_mask` = 1234, `ScriptName` = 'npc_guardian_yoggsaron' WHERE `entry` = 33136;
UPDATE creature_model_info SET bounding_radius = 0.62, combat_reach = 1.5 WHERE modelid = 28465;
-- Yogg-Saron
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 131076, `mechanic_immune_mask` = 650854235, `ScriptName` = 'boss_yoggsaron' WHERE `entry` = 33288;
UPDATE creature_model_info SET bounding_radius = 0.755, combat_reach = 18 WHERE modelid = 28817;
UPDATE `creature_template` SET  modelid1 = 16946, modelid2 = 0, `minlevel` = 80, `maxlevel` = 80, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33685510, `speed_walk` = 2, `ScriptName` = 'npc_death_orb' WHERE `entry` = 33882;
-- Brain of Yogg-Saron
UPDATE `creature_template` SET `difficulty_entry_1` = 33954, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 131076, `mechanic_immune_mask` = 650854235, `ScriptName` = 'boss_brain_yoggsaron' WHERE `entry` = 33890;
UPDATE creature_model_info SET bounding_radius = 0.755, combat_reach = 30 WHERE modelid = 28951;
-- Illusions
UPDATE `creature_template` SET modelid1 = 15880, modelid2 = 0, `minlevel` = 82, `maxlevel` = 82, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33685510, `ScriptName` = 'npc_laughing_skull' WHERE `entry` = 33990;
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `faction_A` = 7, `faction_H` = 7, `mechanic_immune_mask` = 650854235, `ScriptName` = 'npc_illusion' WHERE `entry` IN (33433, 33716, 33717, 33719, 33720, 33567);
UPDATE creature_model_info SET bounding_radius = 0.306, combat_reach = 1.5 WHERE modelid = 28621;
UPDATE creature_model_info SET bounding_radius = 1, combat_reach = 10 WHERE modelid IN (2718, 1687, 2717, 12869);
-- Influence Tentacle
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `faction_A` = 16, `faction_H` = 16, `mindmg` = 123, `maxdmg` = 123, `attackpower` = 123, `dmg_multiplier` = 12 WHERE `entry` = 33943;
UPDATE creature_model_info SET bounding_radius = 0.306, combat_reach = 1.5 WHERE modelid = 28813;
-- Fake npcs
UPDATE `creature_template` SET `unit_flags` = 518, `type_flags` = 0, `flags_extra` = 2 WHERE `entry` IN (33436, 33437, 33536, 33535, 33495, 33523, 33441, 33442);
-- Whispers
UPDATE `script_texts` SET `type` = 4 WHERE `entry` IN (-1603317, -1603340, -1603339);
-- Descend into madness portal
UPDATE `creature_template` SET `ScriptName` = 'npc_descend_into_madness' WHERE `entry` = 34072;
-- Flee to the Surface
UPDATE `gameobject_template` SET `data10` = 63992 WHERE `entry` = 194625;
DELETE FROM `gameobject` WHERE `id` = 194625;
INSERT INTO `gameobject` VALUES
(NULL, 194625, 603, 3, 1, 1996.41, -0.070, 240.59, 0, 0, 0, 1, 0, 300, 0, 1),
(NULL, 194625, 603, 3, 1, 1949.63, -26.07, 241.25, 0, 0, 0, 1, 0, 300, 0, 1),
(NULL, 194625, 603, 3, 1, 1995.03, -52.98, 241.02, 0, 0, 0, 1, 0, 300, 0, 1);
-- Cancel Illusion Room Aura
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 63992;
INSERT INTO `spell_linked_spell` VALUES
(63992, -63988, 0, "Cancel Illusion Room Aura");
-- Dragon blood
DELETE FROM gameobject WHERE id = 194462;
INSERT INTO gameobject VALUES (NULL, 194462, 603, 3, 1, 2104.35, -25.3753, 242.647, 0, 0, 0, 0, -1, 300, 0, 1);
-- Remove spawned mobs
DELETE FROM `creature` WHERE `id`=34137 AND position_x = 1921.84;
-- Portals coordinates
DELETE FROM `spell_target_position` WHERE `id` IN (63989, 63992, 63997, 63998);
INSERT INTO `spell_target_position` VALUES
(63989, 603, 1953.91, 21.91, 239.71, 2.08),
(63992, 603, 1980.28, -25.59, 329.40, 3.14),
(63997, 603, 2042.02, -25.54, 239.72, 6.28),
(63998, 603, 1948.44, -82.04, 239.99, 4.18);
-- Tentacles
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 131076, `type_flags` = 0, `ScriptName` = "npc_constrictor_tentacle" WHERE `entry` = 33983;
UPDATE creature_model_info SET bounding_radius = 0.306, combat_reach = 1.5 WHERE modelid = 28815;
-- Squeeze stun
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (64125, 64126);
INSERT INTO `spell_linked_spell` VALUES
(64125, 9032, 2, "Squeeze stun"),
(64126, 9032, 2, "Squeeze stun");
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 4, `type_flags` = 0, `mindmg` = 123, `maxdmg` = 123, `attackpower` = 123, `dmg_multiplier` = 123, `baseattacktime` = 1800, `ScriptName` = 'npc_crusher_tentacle' WHERE `entry` = 33966;
UPDATE creature_model_info SET bounding_radius = 0.985, combat_reach = 5 WHERE modelid = 28814;
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `unit_flags` = 131076, `ScriptName` = 'npc_corruptor_tentacle' WHERE `entry` = 33985;
-- Immortal Guardian
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `mindmg` = 123, `maxdmg` = 132, `attackpower` = 123, `dmg_multiplier` = 12, `type_flags` = 0, `ScriptName` = 'npc_immortal_guardian' WHERE `entry` = 33988;
UPDATE creature_model_info SET bounding_radius = 0.92, combat_reach = 4 WHERE modelid = 29024;
DELETE FROM `conditions` WHERE `SourceEntry` = 64173;
INSERT INTO `conditions` (SourceTypeOrReferenceId,SourceEntry,ConditionTypeOrReference,ConditionValue1,ConditionValue2) VALUES 
(13, 64173, 18, 1, 33966),
(13, 64173, 18, 1, 33983),
(13, 64173, 18, 1, 33985);
-- Remove area stun
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (64059, 65238);
INSERT INTO `spell_linked_spell` VALUES
(64059, 65238, 0, "Shattered illusion remove"),
(65238, -64173, 1, "Shattered illusion remove");
DELETE FROM `conditions` WHERE `SourceEntry` = 64172;
INSERT INTO `conditions` (SourceTypeOrReferenceId,SourceEntry,ConditionTypeOrReference,ConditionValue1,ConditionValue2) VALUES 
(13, 64172, 18, 1, 33988);
UPDATE `creature` SET `MovementType`='0' WHERE `guid`='131859';

-- immune mask to 25 man version of razorscale
UPDATE `creature_template` SET `mechanic_immune_mask`=650854235 WHERE `entry`=33724;
-- immune mask to 10 man version of razorscale
UPDATE `creature_template` SET `mechanic_immune_mask`=650854235 WHERE `entry`=33186;
-- add equipment to 25 man version adds
UPDATE `creature_template` SET `equipment_id`=574 WHERE `entry`=33850;
-- add equipment to 25 man version adds
UPDATE `creature_template` SET `equipment_id`=1863 WHERE `entry`=33852;
-- add equipment to 25 man version adds
UPDATE `creature_template` SET `equipment_id`=1787 WHERE `entry`=33851;
-- disable move, set not selectable, set non attackable 
UPDATE `creature_template` SET `unit_flags`=33554438 WHERE `entry`=33184;

DELETE FROM `creature` WHERE `id` IN (33186,33233,33282);
-- using same guid that razorscale had before
REPLACE INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(137611,33186,603,3,1,0,0,587.073,-203.205,441.237,1.60532,604800,0,0,3555975,0,0,0);

UPDATE creature_template SET ScriptName="mole_machine_trigger" WHERE entry=33245; 