UPDATE creature_template SET ScriptName="mob_gluths_zombie" WHERE entry=16360;

-- Remove invisible state for Shadow Fissure c16129 used by Kel'Thuzad in Naxxramas and set proper baseattacktiem (by Ident thx Trazom for baseattacktime)
UPDATE `creature_template` SET `baseattacktime`=5000,`flags_extra`=`flags_extra`&~128 WHERE `entry`=16129;
-- Naxxramas
-- Delete Copse Scarabs
DELETE FROM `creature` WHERE `id`= 16698;
UPDATE `creature_template` SET `mindmg` = 400, `maxdmg` = 600, `attackpower` = ROUND((`mindmg` + `maxdmg`) / 4 * 7), `mindmg` = ROUND(`mindmg` - `attackpower` / 7), `maxdmg` = ROUND(`maxdmg` - `attackpower` / 7) WHERE `entry` = 16698;
-- Delete Gothik adds
DELETE FROM `creature` WHERE `id`IN (16124,16125,16126,16127,16148,16149,16150);
-- Faction Gothik adds
UPDATE `creature_template` SET faction_A = 21, faction_H = 21 WHERE `entry` IN (16124,16125,16126,16127,16148,16149,16150);
-- Grobbulus Wolke
UPDATE `creature_template` SET faction_A = 21, faction_H = 21 WHERE `entry` IN (16363);
-- Razevous Damage
UPDATE `creature_template` SET 
    `mindmg` = 13000, 
    `maxdmg` = 18000, 
    `attackpower` = ROUND((`mindmg` + `maxdmg`) / 4 * 7), 
    `mindmg` = ROUND(`mindmg` - `attackpower` / 7), 
    `maxdmg` = ROUND(`maxdmg` - `attackpower` / 7) 
  WHERE `entry` = 16061;
-- several mobs
DELETE FROM `creature` WHERE `id` IN (16983,16981,16982,16984);
UPDATE `creature_template` SET faction_A = 21, faction_H = 21 WHERE `entry` IN (16983,16981,16982,16984); -- Noth Skelletions
DELETE FROM `creature` WHERE `id` IN (16286);
UPDATE `creature_template` SET faction_A = 21, faction_H = 21 WHERE `entry` IN (16286); -- Lortheb Spores
DELETE FROM `creature` WHERE `id` IN (16486); -- Web Wrap
UPDATE `creature_template` SET faction_A = 21, faction_H = 21 WHERE `entry` IN (16486,17055); -- Maexxna

UPDATE creature SET deathState = 1 WHERE id = 16381;
UPDATE areatrigger_teleport SET target_map = 0, target_position_x = 3118.5715 , target_position_y = -3754.5629 , target_position_z = 133.6039, target_orientation = 4.1361 WHERE id = 4156;

UPDATE creature SET spawntimesecs = '72000' WHERE map = 533;
UPDATE gameobject SET spawntimesecs = '72000' WHERE map = 533 AND id = 181366;
UPDATE gameobject SET spawntimesecs = '72000' WHERE map = 533 AND id IN (181126,181195,181167,181235,181197,181209,181123,181120,181121,181124,181125,181170,181119,181200,181201,181202,181203,181241,181225,181228,181496,181366);

UPDATE `creature_template` SET mechanic_immune_mask = 113983323 WHERE `entry` IN (15956,15953,15952,16028,15931,15932,15928,16061,16060,16064,16065,16062,16063,15954,15936,16011,15989,15990);
UPDATE `creature_template` SET maxlevel = 83, minlevel = 83  WHERE `entry` IN (15956,15953,15952,16028,15931,15932,15928,15930,15929,16061,16060,16064,16065,16062,16063,15954,15936,16011,15989,15990);
UPDATE `creature_template` SET maxlevel = 73, minlevel = 73 WHERE `entry` = 16363;
-- *******************************************************************************************
-- Doors
-- *******************************************************************************************
-- Spawns
-- Delete existing Spawns
DELETE FROM gameobject WHERE id IN (181126,181195,181167,181235,181197,181209,181123,181120,181121,181124,181125,181170,181119,181200,181201,181202,181203,181241,181225,181228,181496);
INSERT INTO `gameobject` (`guid`, `id`, `map`,`spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
-- 181126 Anub'Rekhan Door
('333000','181126','533','1','3202.7','-3476.08','287.061','0','0','0','0','0','6','0','1'),
-- 181195 Anub'Rekhan Gate
('333001','181195','533','1','3143.53','-3572.74','286','5.49778','0','0','0.382687','-0.923878','6','0','1'),
-- 181167 Grand Widow Faerlina Door
('333002','181167','533','1','3120.78','-3791','273.93','0','0','0','0','0','6','0','1'),
-- 181235 Grand Widow Faerlina - Web
('333003','181235','533','1','3322.53','-3695.4','259','3.14159','0','0','1','3.13916e-007','6','0','0'),
-- 181209 Maexxna - Outer Web Door 
('333004','181209','533','1','3425.68','-3846.1','309.109','3.14159','0','0','1','3.13916e-007','6','0','1'),
-- 181197 Maexxna - Inner Web Door
('333005','181197','533','1','3446.72','-3860.3','308.76','3.14159','0','0','0','0','6','0','1'),
-- 181124 Vaccuum - Enter Gate
('333006','181124','533','1','2765.75','-3384.38','267.685','0','0','0','0','1','6','0','1'),
-- 181125 Vaccuum - Exit Gate
('333007','181125','533','1','2617.74','-3336.81','267.684','0','0','0','0','1','25','0','1'),
-- 181170 Vaccuum - Combat Gate
('333008','181170','533','1','2692.04','-3361.4','267.684','0','0','0','0','1','6','0','1'),
-- 181119 Deathknight Door
('333009','181119','533','1','2587.76','-3017.16','240.5','3.14159','0','0','1','3.13916e-007','6','0','1'),
-- 181200 Noth - Entry Door
('333010','181200','533','1','2737.83','-3489.78','262.107','0','0','0','0','1','25','0','0'),
-- 181201 Noth - Exit Door
('333011','181201','533','1','2684.37','-3559.83','261.944','1.57079','0','0','0.707105','0.707109','25','0','1'),
-- 181202 Heigan the Unclean - Entry Door
('333012','181202','533','1','2823.47','-3685.07','274.079','0','0','0','0','1','25','0','0'),
-- 181203 Heigan the Unclean - Exit Door
('333013','181203','533','1','2771.42','-3737.02','273.619','1.57079','0','0','0.707105','0.707109','25','0','1'),
-- 181496 Heigan the Unclean - Exit Gate
('333020','181496','533','1','2909.82','-3817.88','273.926','0','0','0','0','1','6','0','1'),
-- 181241 Loatheb - Entrance Door
('333014','181241','533','1','2909.73','-3947.75','273.924','0','0','0','0','1','6','0','1'),
-- 181123 Patchwork - Exit Door
('333015','181123','533','1','3318.27','-3254.42','293.786','1.57079','0','0','0.707105','0.707109','6','0','1'),
-- 181120 Gluth - Exit Door
('333016','181120','533','1','3338.63','-3101.36','297.0','3.14159','0','0','1','1.26759e-006','6','0','1'),
-- 181121 Thaddius Door
('333017','181121','533','1','3424.77','-3014.72','295.608','0','0','0','0','1','25','0','1'),
-- 181225 Frostwyrm Waterfall Door
('333018','181225','533','1','3537.32','-5160.48','143.619','4.50667','0','0','0.775973','-0.630766','25','0','1'),
-- 181228 KelThuzad Door 
('333019','181228','533','1','3635.34','-5090.31','143.206','1.37','0','0','0.632673','0.774419','6','0','1');

UPDATE gameobject_template SET size = 1.2 WHERE entry = 181201;
UPDATE gameobject_template SET faction = 14 WHERE entry IN (181126,181195,181167,181235,181197,181209,181123,181120,181121,181124,181125,181170,181119,181200,181201,181202,181203,181241,181225,181228,181496,181366);