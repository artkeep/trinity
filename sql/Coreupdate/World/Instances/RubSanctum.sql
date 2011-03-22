-- sound / text
DELETE FROM `script_texts` WHERE `entry` BETWEEN -1666406 AND -1666000;

-- xerestrasza
INSERT INTO `script_texts` (`npc_entry`, `entry`, `content_default`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
(40429, '-1666000','Help! I am trapped within this tree! I require aid!','Спасите! Я под этим деревом. Мне нужна помощь!','17490','6','0','0','SAY_XERESTRASZA_YELL_1'),
(40429, '-1666001','Thank you! I could have not held out for much longer. A terrible thing has happened here.','Спасибо! Без вас я бы долго не продержалась... Здесь произошли страшные события...','17491','0','0','0','SAY_XERESTRASZA_YELL_2'),
(40429, '-1666002','We believed that the Sanctum was well fortified, but we were not prepareted for the nature of this assault.','Святилище считалось неприступным, но до сих пор оно не подвергалось такому штурму...','17492','0','0','0','SAY_XERESTRASZA_SAY_1'),
(40429, '-1666003','The Black Dragonkin materialized from thin air, and set upon us before we could react.','Черные драконы явились из ниоткуда. Мы даже не успели понять что происходит...','17493','0','0','0','SAY_XERESTRASZA_SAY_2'),
(40429, '-1666004','We did not stand a chance. As my brethren perished around me, I managed to retreat here and bar the entrance.','Силы были неравны, мои братья гибли один за другим. А я спряталась здесь и запечатала вход.','17494','0','0','0','SAY_XERESTRASZA_SAY_3'),
(40429, '-1666005','They slaughtered us with cold efficiency, but the true focus of their interest seemed to be the eggs kept here in the sanctum.','Нас убивали с расчетливой жестокостью, но основной целью врага была кладка яиц в святилище.','17495','0','0','0','SAY_XERESTRASZA_SAY_4'),
(40429, '-1666006','The commander of the forces on the ground here is a cruel brute named Zarithrian. But I fear there are greater powers at work.',' Атакой руководил кровожадный Заритриан, но, боюсь, тут замешано и более могущественное зло.','17496','0','0','0','SAY_XERESTRASZA_SAY_5'),
(40429, '-1666007','In their initial assault I caught a glimpse of their true leader, a fearsome full-grown Twilight Dragon.','В самом начале я ощутила присутствие их настоящего лидера - огромного сумеречного дракона.','17497','0','0','0','SAY_XERESTRASZA_SAY_6'),
(40429, '-1666008','I know not the extent of their plans heroes, but I know this: they cannot be allowed to succeed!','Герои, мне не ведомо чего добиваются эти захватчики. Одно я знаю точно - их нужно остановить!','17498','0','0','0','SAY_XERESTRASZA_SAY_7'),

-- Halion
(39863, '-1666100','Meddlesome insects, you\'re too late! The Ruby Sanctum is lost.','Назойливая мошкара! Вы опоздали. Рубиновое святилище пало!','17499','6','0','0','SAY_HALION_SPAWN'),
(39863, '-1666101','Your world teeters on the brink of annihilation. You will all bear witness to the coming of a new age of destruction!','Этот мир вот-вот соскользнет в бездну. Вам выпала честь узреть начало эры РАЗРУШЕНИЯ!','17500','6','0','0','SAY_HALION_AGGRO'),
(39863, '-1666102','Another hero falls.','Сколько еще таких героев?','17501','6','0','0','SAY_HALION_SLAY_1'),
(39863, '-1666103','Ha Ha Ha!','','17502','6','0','0','SAY_HALION_SLAY_2'),
(39863, '-1666104','Relish this victory mortals, for it will be your last. This world will burn with the Master\'s return!','Это ваша последняя победа. Насладитесь сполна ее вкусом. Ибо когда вернется мой господин, этот мир сгинет в огне!','17503','6','0','0','SAY_HALION_DEATH'),
(39863, '-1666105','Not good enough!','Надоело!','17504','6','0','0','SAY_HALION_BERSERK'),
(39863, '-1666106','The heavens burn!','Небеса в огне!','17505','6','0','0','SAY_HALION_SPECIAL_1'),
(39863, '-1666107','Beware the shadow!','','17506','6','0','0','SAY_HALION_SPECIAL_2'),
(39863, '-1666108','You will find only suffering within the realm of Twilight. Enter if you dare.','Вы найдете только тьму в мире Сумерек. Входите, если посмеете.','17507','6','0','0','SAY_HALION_PHASE_2'),
(39863, '-1666109','I am the light AND the darkness! Cower mortals before the Herald of Deathwing!','Я есть свет и я есть тьма! Трепещите, ничтожные, перед посланником Смертокрыла!','17508','6','0','0','SAY_HALION_PHASE_3'),
(39863, '-1666110','<need translate>','Во вращающихся сферах пульсирует темная энергия!','0','3','0','0',''),
(39863, '-1666111','<need translate>','Ваши союзники протолкнули Халиона дальше в физический мир!','0','3','0','0',''),
(39863, '-1666112','<need translate>','Ваши союзники протолкнули Халиона дальше в реальный мир!','0','3','0','0',''),
(39863, '-1666113','<need translate>','Находясь в покое в одном из миров, Халион восстанавливает жизненные силы.','0','3','0','0',''),

-- Zarthrian
(39746, '-1666200','Alexstrasza has chosen capable allies. A pity that I must end you!','Алекстраза выбрала достойных союзников... Жаль, что придется ПРИКОНЧИТЬ ВАС!','17512','6','0','0','SAY_ZARITHRIAN_AGGRO'),
(39746, '-1666201','You thought you stood a chance?','Глупо было и надеяться!','17513','6','0','0','SAY_ZARITHRIAN_SLAY_1'),
(39746, '-1666202','It\'s for the best.','Все только к лучшему!','17514','6','0','0','SAY_ZARITHRIAN_SLAY_2'),
(39746, '-1666203','Halion! I\'m...aah!','ХАЛИОН! Я...','17515','6','0','0','SAY_ZARITHRIAN_DEATH'),
(39746, '-1666204','Turn them to ash, minions!','Слуги! Обратите их в пепел!','17516','6','0','0','SAY_ZARITHRIAN_SPECIAL_1'),

-- baltharus
(39751, '-1666300','Ah, the entertainment has arrived...','А-а-а, цирк приехал.','17520','6','0','0','SAY_BALTHARUS_AGGRO'),
(39751, '-1666301','Baltharus leaves no survivors!','Балтар не оставляет живых!','17521','6','0','0','SAY_BALTHARUS_SLAY_1'),
(39751, '-1666302','This world has enough heroes!','В мире хватает героев и без тебя...','17522','6','0','0','SAY_BALTHARUS_SLAY_2'),
(39751, '-1666303','I...didn\'t see that coming...','Как… это могло произойти?..','17523','1','0','0','SAY_BALTHARUS_DEATH'),
(39751, '-1666304','Twice the pain and half the fun!','Вдвое сильнее страдание.','17524','6','0','0','SAY_BALTHARUS_SPECIAL_1'),
(39751, '-1666305','Your power wanes, ancient one! Soon, you will join your friends!','Твоя сила на исходе, Древнейшая! Скоро ты присоединишься к своим друзьям!','17525','6','0','0','SAY_BALTHARUS_YELL'),

-- saviana
(39747, '-1666400','You will suffer for this intrusion...','Ваш-ш-ши муки с-cтанут платой за это вторжение!','17528','6','0','0','SAY_SAVIANA_AGGRO'),
(39747, '-1666401','As it should be!','Так и должно быть!','17529','6','0','0','SAY_SAVIANA_SLAY_1'),
(39747, '-1666402','Halion will be pleased...','Халион будет доволен!','17530','6','0','0','SAY_SAVIANA_SLAY_2'),
(39747, '-1666403','<screaming>','О...','17531','6','0','0','SAY_SAVIANA_DEATH'),
(39747, '-1666404','Burn in the master\'s flame!','Горите в огне хозяина!','17532','6','0','0','SAY_SAVIANA_SPECIAL_1'),
(39747, '-1666405','<need translate>','|3-3(%s) впадает в исступление!','0','3','0','0','');

-- Ruby sanctum
UPDATE `instance_template` SET `Script`='instance_ruby_sanctum' WHERE `map`=724;
-- Halion
UPDATE `creature_template` SET `ScriptName`='boss_halion_real', `AIName` ='' WHERE `entry`=39863;
UPDATE `creature_template` SET `ScriptName`='boss_halion_twilight', `AIName` ='' WHERE `entry`=40142;
UPDATE `creature_template` SET `ScriptName`='mob_halion_meteor', `AIName` ='' WHERE `entry` = 40029;
UPDATE `creature_template` SET `ScriptName`='mob_halion_flame', `AIName` ='' WHERE `entry` IN (40041);
UPDATE `creature_template` SET `ScriptName`='mob_halion_control', `AIName` ='' WHERE `entry` IN (40146);
UPDATE `creature_template` SET `ScriptName`='mob_halion_orb', `AIName` ='' WHERE `entry` IN (40083,40100);
UPDATE `creature_template` SET `ScriptName`='mob_orb_rotation_focus', `AIName` ='' WHERE `entry` = 40091;
UPDATE `creature_template` SET `ScriptName`='mob_orb_carrier', `AIName` ='' WHERE `entry` = 40081;
UPDATE `creature_template` SET `ScriptName`='mob_fiery_combustion', `AIName` ='' WHERE `entry` = 40001;
UPDATE `creature_template` SET `ScriptName`='mob_soul_consumption', `AIName` ='' WHERE `entry` = 40135;
UPDATE `creature_template` SET `ScriptName`='', `AIName` ='' WHERE `entry` IN (40143, 40144, 40145);

-- spell_halion_fiery_combustion 74562
DELETE FROM `spell_script_names` WHERE `spell_id`=74562 AND `ScriptName`='spell_halion_fiery_combustion';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (74562,'spell_halion_fiery_combustion');

-- spell_halion_soul_consumption 74792
DELETE FROM `spell_script_names` WHERE `spell_id`=74792 AND `ScriptName`='spell_halion_soul_consumption';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (74792,'spell_halion_soul_consumption');


UPDATE `gameobject_template` SET `data10` = 74807, `faction` = '0', `ScriptName` = 'go_halion_portal_twilight' WHERE `gameobject_template`.`entry` IN (202794,202795);
UPDATE `gameobject_template` SET `faction` = '0', `ScriptName` = 'go_halion_portal_real' WHERE `gameobject_template`.`entry` IN (202796);

-- Baltharus
UPDATE `creature_template` SET `ScriptName`='boss_baltharus', `AIName` ='', `dmg_multiplier` = 80  WHERE `entry`=39751;
UPDATE `creature_template` SET `ScriptName`='mob_baltharus_clone', `AIName` ='', `dmg_multiplier` = 80  WHERE `entry`=39899;

-- zarithrian
UPDATE `creature_template` SET `ScriptName`='boss_zarithrian', `AIName` ='' WHERE `entry`=39746;
UPDATE `creature` SET `position_x` = '3008.552734',`position_y` = '530.471680',`position_z` = '89.195290',`orientation` = '6.16' WHERE `id` = 39746;
UPDATE `creature_template` SET `ScriptName`='mob_flamecaller_ruby', `AIName` ='' WHERE `entry`=39814;

-- Saviana Ragefire
UPDATE `creature_template` SET `ScriptName`='boss_ragefire', `AIName` ='' WHERE `entry`=39747;
DELETE FROM `conditions` WHERE `SourceEntry`=74455;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES 
(13,0,74455,0,18,1,39747,0,0, '', 'Ragefire - Conflagration');

-- Xerestrasza
UPDATE `creature_template` SET `ScriptName`='mob_xerestrasza', `AIName` ='' WHERE `entry`=40429;