-- [Culling of Stratholme] Instance by John Holiver
-- Much work done, the instance is playable with this
-- Although there're things that aren't implemented yet
-- * Random placement of risen zombies after MalGanis transform some
-- * Recovery of the city in case of Reset()
-- * Heroic timer for Infinite Corruptor spawn
-- Known bugs (that if i knew where to fix i would do it):
-- * Jaina get spawned 2 times on the beggining

SET @Meathook :=26529;
SET @Salramm :=26530;
SET @Epoch :=26532;
SET @MalGanis :=26533;
SET @Infinite :=32273;
SET @Arthas :=26499;
SET @Uther :=26528;
SET @Jaina :=26497;

DELETE FROM `script_texts` WHERE `entry` <= -1595000 and `entry` >= -1595999;
REPLACE INTO `script_texts` (`npc_entry`,`entry`,`content_default`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
-- Bosses
-- Chrono Lord Epoch
(@Epoch,-1595000, 'Prince Arthas Menethil, on this day, a powerful darkness has taken hold of your soul. The death you are destined to visit upon others will this day be your own.' ,13408,1,0,0, 'epoch SAY_INTRO | culling SAY_PHASE314' ),
(@Epoch,-1595001, 'We''ll see about that, young prince.' ,13409,1,0,0, 'epoch SAY_AGGRO' ),
(@Epoch,-1595002, 'Tick tock, tick tock...' ,13410,1,0,0, 'epoch SAY_TIME_WARP_1' ),
(@Epoch,-1595003, 'Not quick enough!' ,13411,1,0,0, 'epoch SAY_TIME_WARP_2'),
(@Epoch,-1595004, 'Let''s get this over with. ' ,13412,1,0,0, 'epoch SAY_TIME_WARP_3' ),
(@Epoch,-1595005, 'There is no future for you.' ,13413,1,0,0, 'epoch SAY_SLAY_1' ),
(@Epoch,-1595006, 'This is the hour of our greatest triumph!' ,13414,1,0,0, 'epoch SAY_SLAY_2' ),
(@Epoch,-1595007, 'You were destined to fail.' ,13415,1,0,0, 'epoch SAY_SLAY_3' ),
(@Epoch,-1595008, '' ,13416,0,0,0, 'epoch SAY_DEATH' ),
-- Mal'Ganis
(@MalGanis,-1595009, 'Yes, this is the beginning. I''ve been waiting for you, young prince. I am Mal''Ganis.' ,14410,0,0,1, 'mal_ganis SAY_INTRO_1 | culling SAY_PHASE206' ),
(@MalGanis,-1595010, 'As you can see, your people are now mine. I will now turn this city household by household, until the flame of life has been snuffed out... forever.' ,14411,0,0,1, 'mal_ganis SAY_INTRO_2 | culling SAY_PHASE207' ),
(@MalGanis,-1595011, 'This will be a fine test...Prince Arthas...' ,14413,1,0,0, 'mal_ganis SAY_AGGRO' ),
(@MalGanis,-1595012, 'All too easy...' ,14416,1,0,0, 'mal_ganis SAY_KILL_1' ),
(@MalGanis,-1595013, 'The dark lord is displeased with your interference...' ,14417,1,0,0, 'mal_ganis SAY_KILL_2' ),
(@MalGanis,-1595014, 'It is Prince Arthas I want... not you...' ,14418,1,0,0, 'mal_ganis SAY_KILL_3' ),
(@MalGanis,-1595015, 'Anak''Keri...' ,14422,1,0,0, 'mal_ganis SAY_SLAY_1' ),
(@MalGanis,-1595016, 'My onslaught will wash over the Lich King''s forces...' ,14423,1,0,0, 'mal_ganis SAY_SLAY_2' ),
(@MalGanis,-1595017, 'Your death is in vain, tiny mortal...' ,14424,1,0,0, 'mal_ganis SAY_SLAY_3' ),
(@MalGanis,-1595018, 'Your time has come to an end!' ,14425,1,0,0, 'mal_ganis SAY_SLAY_4' ),
(@MalGanis,-1595019, 'Time out...' ,14414,1,0,0, 'mal_ganis SAY_SLEEP_1' ),
(@MalGanis,-1595020, 'You seem...tired...' ,14415,1,0,0, 'mal_ganis SAY_SLEEP_2' ),
(@MalGanis,-1595021, 'I spent too much time in that weak little shell...' ,14426,1,0,0, 'mal_ganis SAY_30HEALTH' ),
(@MalGanis,-1595022, '(Eredun) I AM MAL''GANIS! I AM ETERNAL!' ,14427,1,0,0, 'mal_ganis SAY_15HEALTH' ),
(@MalGanis,-1595023, 'ENOUGH! I waste my time here...I must gather my strength on the home world...' ,14428,1,0,0, 'mal_ganis SAY_ESCAPE_SPEECH_1' ),
(@MalGanis,-1595024, 'You''ll never defeat the Lich King without my forces! I''ll have my revenge...on him, AND you...' ,14429,1,0,0, 'mal_ganis SAY_ESCAPE_SPEECH_2' ),
(@MalGanis,-1595025, 'Your journey has just begun, young prince. Gather your forces and meet me in the artic land of Northrend. It is there that we shall settle the score between us. It is there that your true destiny will unfold.' ,14412,1,0,1, 'mal_ganis SAY_OUTRO' ),
-- Meathook
(@Meathook,-1595026, 'Play time!' ,13428,1,0,0, 'meathook SAY_AGGRO' ),
(@Meathook,-1595027, 'Boring...' ,13430,1,0,0, 'meathook SAY_SLAY_1' ),
(@Meathook,-1595028, 'Why you stop moving?' ,13431,1,0,0, 'meathook SAY_SLAY_2' ),
(@Meathook,-1595029, 'Get up! Me not done!' ,13432,1,0,0, 'meathook SAY_SLAY_3' ),
(@Meathook,-1595030, 'New toys!' ,13429,1,0,0, 'meathook SAY_SPAWN' ),
(@Meathook,-1595031, 'This... not fun...' ,13433,1,0,0, 'meathook SAY_DEATH' ),
-- Salramm the Fleshcrafter 
(@Salramm,-1595032, 'Ah, the entertainment has arrived!' ,13472,1,0,0, 'salramm SAY_AGGRO' ),
(@Salramm,-1595033, 'You are too late, champion of Lordaeron. The dead shall have their day.' ,13471,1,0,0, 'salramm SAY_SPAWN' ),
(@Salramm,-1595034, 'The fun is just beginning!' ,13473,1,0,0, 'salramm SAY_SLAY_1' ),
(@Salramm,-1595035, 'Aah, quality materials!' ,13474,1,0,0, 'salramm SAY_SLAY_2' ),
(@Salramm,-1595036, 'Don''t worry, I''ll make good use of you.' ,13475,1,0,0, 'salramm SAY_SLAY_3' ),
(@Salramm,-1595037, 'You only advance... the master''s plan...' ,13483,1,0,0, 'salramm SAY_DEATH' ),
(@Salramm,-1595038, 'BOOM! Hahahahah...' ,13478,1,0,0, 'salramm SAY_EXPLODE_GHOUL_1' ),
(@Salramm,-1595039, 'Blood... destruction... EXHILARATING!' ,13479,1,0,0, 'salramm SAY_EXPLODE_GHOUL_2' ),
(@Salramm,-1595040, 'I want a sample...' ,13480,1,0,0, 'salramm SAY_STEAL_FLESH_1' ),
(@Salramm,-1595041, 'Such strength... it must be mine!' ,13481,1,0,0, 'salramm SAY_STEAL_FLESH_2' ),
(@Salramm,-1595042, 'Your flesh betrays you.' ,13482,1,0,0, 'salramm SAY_STEAL_FLESH_3' ),
(@Salramm,-1595043, 'Say hello to some friends of mine.' ,13476,1,0,0, 'salramm SAY_SUMMON_GHOULS_1' ),
(@Salramm,-1595044, 'Come, citizen of Stratholme! Meet your saviors.' ,13477,1,0,0, 'salramm SAY_SUMMON_GHOULS_2' ),
-- The Infinite Corruptor 
-- Couldn't find the value from the audios for `sound` column, not added.
(@Infinite,-1595045, 'How dare you interfere with our work here!' ,0,1,0,0, 'infinite SAY_AGGRO' ),
(@Infinite,-1595046, 'My work here is finished!' ,0,1,0,0, 'infinite SAY_FAIL' ),
(@Infinite,-1595047, 'My time... has run out...' ,0,1,0,0, 'infinite SAY_DEATH' ),
-- First Act - Uther and Jaina Dialog
-- Outside the city, Arthas, Uther and Jaina dialog
(@Arthas,-1595070, 'Glad you could make it, Uther.' ,12828,0,0,1, 'culling SAY_PHASE101' ),
(@Uther,-1595071, 'Watch your tone with me, boy. You may be the prince, but I''m still your superior as a paladin!' ,12839,0,0,25, 'culling SAY_PHASE102' ),
(@Arthas,-1595072, 'As if I could forget. Listen, Uther, there''s something about the plague you should know... ' ,12829,0,0,0, 'culling SAY_PHASE103' ),
(@Arthas,-1595073, 'Oh, no. We''re too late. These people have all been infected! They may look fine now, but it''s just a matter of time before they turn into the undead!' ,12830,0,0,1, 'culling SAY_PHASE104' ),
(@Uther,-1595074, 'What?' ,12840,0,0,5, 'culling SAY_PHASE105' ),
(@Arthas,-1595075, 'This entire city must be purged.' ,12831,0,0,1, 'culling SAY_PHASE106' ),
(@Uther,-1595076, 'How can you even consider that? There''s got to be some other way.' ,12841,0,0,1, 'culling SAY_PHASE107' ),
(@Arthas,-1595077, 'Damn it, Uther! As your future king, I order you to purge this city!' ,12832,1,0,5, 'culling SAY_PHASE108' ),
(@Uther,-1595078, 'You are not my king yet, boy! Nor would I obey that command even if you were!' ,12842,1,0,22, 'culling SAY_PHASE109' ),
(@Arthas,-1595079, 'Then I must consider this an act of treason.' ,12833,0,0,0, 'culling SAY_PHASE110' ),
(@Uther,-1595080, 'Treason? Have you lost your mind, Arthas?' ,12843,0,0,5, 'culling SAY_PHASE111' ),
(@Arthas,-1595081, 'Have I? Lord Uther, by my right of succession and the sovereignty of my crown, I hereby relieve you of your command and suspend your paladins from service.' ,12834,0,0,1, 'culling SAY_PHASE112' ),
(@Jaina,-1595082, 'Arthas! You can''t just--' ,12837,0,0,1, 'culling SAY_PHASE113' ),
(@Arthas,-1595083, 'It''s done! Those of you who have the will to save this land, follow me! The rest of you... get out of my sight!' ,12835,0,0,0, 'culling SAY_PHASE114' ),
(@Uther,-1595084, 'You''ve just crossed a terrible threshold, Arthas.' ,12844,0,0,25, 'culling SAY_PHASE115' ),
(@Arthas,-1595085, 'Jaina?' ,12836,0,0,1, 'culling SAY_PHASE116' ),
(@Jaina,-1595086, 'I''m sorry, Arthas. I can''t watch you do this.' ,12838,0,0,1, 'culling SAY_PHASE117' ),
(@Arthas,-1595087, 'Take position here, and I will lead a small force inside Stratholme to begin the culling. We must contain and purge the infected for the sake of all Lordaeron!' ,14327,0,0,1, 'culling SAY_PHASE118' ),
-- Second Act - City Streets
-- Arthas enters the city and dialog with Mal'Ganis
-- After defeating 2 bosses he runs to Town Hall
(@Arthas,-1595088, 'Everyone looks ready. Remember, these people are all infected with the plague and will die soon. We must purge Stratholme to protect the remainder of Lordaeron from the Scourge. Let''s go.' ,14293,0,0,1, 'culling SAY_PHASE201' ),
(0,-1595089, 'Prince Arthas, may the light be praised! Many people in the town have begun to fall seriously ill, can you help us?' ,0,0,0,1, 'culling SAY_PHASE202' ),
(@Arthas,-1595090, 'I can only help you with a clean death.' ,14294,0,0,0, 'culling SAY_PHASE203' ),
(0,-1595091, 'What? This can''t be!' ,0,0,0,0, 'culling SAY_PHASE204' ),
(@Arthas,-1595092, 'That was just the beginning.' ,14295,0,0,1, 'culling SAY_PHASE205' ),
(@Arthas,-1595093, 'I won''t allow it, Mal''Ganis! Better that these people die by my hand than serve as your slaves in death!' ,14296,0,0,5, 'culling SAY_PHASE208' ),
(@Arthas,-1595094, 'Mal''ganis will send out some of his Scourge minions to interfere with us. Those of you with the strongest steel and magic shall go forth and destroy them. I will lead the rest of my forces in purging Stratholme of the infected.' ,14885,0,0,1, 'culling SAY_PHASE209' ),
(@Arthas,-1595095, 'Champions, meet me at town hall at once! We must take the fight to Mal''Ganis.' ,14297,0,0,1, 'culling SAY_PHASE210' ),
-- Third Act - Town Hall
(@Arthas,-1595096, 'Follow me. I know the way trought.' ,14298,0,0,1, 'culling SAY_PHASE301' ),
(0,-1595097, 'Ah, you''ve finally arrived Prince Arthas. You''re just in the nick of time.' ,0,0,0,1, 'culling SAY_PHASE302' ),
(@Arthas,-1595098, 'Yes, I''m glad i could get you before the plague.' ,14299,0,0,1, 'culling SAY_PHASE303' ),
(@Arthas,-1595099, 'What is this sorcery?' ,14300,0,0,0, 'culling SAY_PHASE304' ),
(0,-1595100, 'There''s no need for you to understand, Arthas. All you need to do is die!' ,0,0,0,0, 'culling SAY_PHASE305' ),
(@Arthas,-1595101, 'Mal''Ganis appears to have more than scourge in his arsenal. We should make haste.' ,14301,0,0,0, 'culling SAY_PHASE306' ),
(@Arthas,-1595102, 'More vile sorcery! Be ready for anything.' ,14302,0,0,0, 'culling SAY_PHASE307' ),
(@Arthas,-1595103, 'Let''s move on.' ,14303,0,0,0, 'culling SAY_PHASE308' ),
(@Arthas,-1595104, 'Watch your backs... they have us surrounded in this hall.' ,14304,0,0,0, 'culling SAY_PHASE309' ),
(@Arthas,-1595105, 'One less obstacle to deal with.' ,0,0,0,0, 'culling SAY_PHASE310' ),
(@Arthas,-1595106, 'Mal''Ganis is not making this easy...' ,14305,0,0,0, 'culling SAY_PHASE311' ),
(@Arthas,-1595107, 'They''re very persistent.' ,14306,0,0,0, 'culling SAY_PHASE312' ),
(@Arthas,-1595108, 'What else can be put in my way?' ,14307,0,0,0, 'culling SAY_PHASE313' ),
(@Arthas,-1595109, 'I do what i must for Lordaeron. And neither your words nor your actions will stop me.' ,14309,0,0,5, 'culling SAY_PHASE315' ),
-- Fourth Act - Fire Corridor
(@Arthas,-1595110, 'The quickest path to Mal''Ganis lays behind that bookshelf ahead.' ,14308,0,0,0, 'culling SAY_PHASE401' ),
(@Arthas,-1595111, 'This will only take a moment.' ,14310,0,0,0, 'culling SAY_PHASE402' ),
(@Arthas,-1595112, 'I believe that secret passage still works!' ,14311,0,0,0, 'culling SAY_PHASE403' ),
(@Arthas,-1595113, 'Let''s move trought here as quickly as possible. If the undead don''t kill us the fires might.' ,14312,0,0,0, 'culling SAY_PHASE404' ),
(@Arthas,-1595114, 'Rest a moment and clear your lungs. But we must move again soon.' ,14313,0,0,0, 'culling SAY_PHASE405' ),
(@Arthas,-1595115, 'That''s enough, we must move again. Mal''Ganis awaits.' ,14314,0,0,0, 'culling SAY_PHASE406' ),
(@Arthas,-1595116, 'At last some good luck, Market Row has not caught fire yet. Mal''Ganis is supposed to be on Crusader Square which is just ahead. Tell me when you''re ready to move forth.' ,14315,0,0,0, 'culling SAY_PHASE407' ),
-- Fifth Act - Mal'Ganis Fight
(@Arthas,-1595117, 'Justice will be done!' ,14316,0,0,0, 'culling SAY_PHASE501' ),
(@Arthas,-1595118, 'We''re going to finish this right now Mal''Ganis... just you and me!' ,14317,0,0,0, 'culling SAY_PHASE502' ),
(@Arthas,-1595119, 'I''ll hunt you to the ends of the earth if I have to! Do you hear me? To the ends of the earth!' ,14318,1,0,5, 'culling SAY_PHASE503' ),
(@Arthas,-1595120, 'You performed well this day. Anything that Mal''Ganis has left behind is yours. Take it as your reward. I must now begin plans for an expedition to Northrend.' ,14319,0,0,5, 'culling SAY_PHASE504' );

-- script_texts for boss_epoch_hunter have wrong comment (epoch instead of epoch_hunter)
-- This generates a "conflict" with boss_epoch script_texts just introduced
-- The following queries eliminate script_texts comment conflict from boss_epoch and boss_epoch_hunter
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_ENTER1' where `entry`=-1560013;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_ENTER2' where `entry`=-1560014;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_ENTER3' where `entry`=-1560015;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_AGGRO1' where `entry`=-1560016;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_AGGRO2' where `entry`=-1560017;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_SLAY1' where `entry`=-1560018;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_SLAY2' where `entry`=-1560019;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_BREATH1' where `entry`=-1560020;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_BREATH2' where `entry`=-1560021;
UPDATE script_texts SET `comment`= 'epoch_hunter SAY_DEATH' where `entry`=-1560022;

-- Arthas Gossip Menu
DELETE FROM `npc_text` WHERE `ID` in (100000,100001,100002,100003,100004,100005);
REPLACE INTO `npc_text` (`ID`,`text0_0`,`text0_1`,`prob0`) VALUES
(100001, 'Are you ready to do whatever it must be done to protect Lordaeron?', 'Are you ready to do whatever it must be done to protect Lordaeron?',1),
(100002, 'You must be one of the brave soldiers who have been fighting the Scourge forces of Mal\'Ganis. Well done.', 'You must be one of the brave soldiers who have been fighting the Scourge forces of Mal\'Ganis. Well done.',1),
(100003, 'Gather your senses quickly, we must press on.', 'Gather your senses quickly, we must press on.',1),
(100004, 'This isn\'t getting any easier. Stratholme is burning. We must brave the Scourge and the flames to reach Mal\'Ganis. Prepare yourselves.', 'This isn\'t getting any easier. Stratholme is burning. We must brave the Scourge and the flames to reach Mal\'Ganis. Prepare yourselves.',1),
(100005, 'Are you ready to face Mal\'Ganis with me?', 'Are you ready to face Mal\'Ganis with me?',1);

-- script_waypoints for Arthas movements inside Culling
DELETE FROM script_waypoint WHERE entry=26499;
REPLACE INTO script_waypoint VALUES
(26499,0,1903.167,1291.573,143.32,0, 'culling Bridge WP1' ),
(26499,1,1911.087,1314.263,150.026,0, 'culling Bridge WP2' ),
(26499,2,1902.959,1295.127,143.388,10000, 'culling Bridge WP3' ),
(26499,3,1913.726,1287.407,141.927,0, 'culling Bridge WP4' ),
(26499,4,1990.833,1293.391,145.467,0, 'culling Bridge WP5' ),
(26499,5,1997.003,1317.776,142.963,0, 'culling Bridge WP6' ),
(26499,6,2019.631,1326.084,142.929,0, 'culling Bridge WP7' ),
(26499,7,2026.469,1287.088,143.596,0, 'culling Bridge WP8' ),
(26499,8,2050.660,1287.333,142.671,0, 'culling Bridge WP9' ),
(26499,9,2081.447,1287.770,141.324,0, 'culling Streets WP1' ),
(26499,10,2087.689,1280.344,140.730,0, 'culling Streets WP2' ),
(26499,11,2092.154,1276.645,140.520,0, 'culling Streets WP3' ),
(26499,12,2099.876,1280.21,138.55,0, 'culling Streets WP4' ),
(26499,13,2120.757,1286.97,136.343,0, 'culling Streets WP5' ),
(26499,14,2165.073,1279.338,133.40,0, 'culling Streets WP6' ),
(26499,15,2186.441,1234.445,136.524,0, 'culling Streets WP7' ),
(26499,16,2210.385,1207.550,136.259,0, 'culling Streets WP8' ),
(26499,17,2243.594,1177.705,137.144,0, 'culling Streets WP9' ),
(26499,18,2286.883,1177.262,137.631,0, 'culling Streets WP10' ),
(26499,19,2320.374,1179.954,133.926,0, 'culling Streets WP11' ),
(26499,20,2365.626,1194.838,131.974,0, 'culling Streets WP12' ),
(26499,21,2366.559,1197.759,132.382,0, 'culling House WP1' ),
(26499,22,2394.011,1205.148,134.125,0, 'culling House WP2' ),
(26499,23,2395.854,1206.345,134.039,0, 'culling House WP3' ),
(26499,24,2442.023,1219.205,133.999,0, 'culling House WP3' ),
(26499,25,2447.105,1191.182,148.076,0, 'culling House WP4' ),
(26499,26,2444.250,1190.510,148.076,0, 'culling House WP5' ),
(26499,27,2418.487,1196.059,148.076,0, 'culling House WP6' ),
(26499,28,2401.221,1191.705,148.076,0, 'culling House WP7' ),
(26499,29,2409.205,1157.425,148.190,0, 'culling House WP8' ),
(26499,30,2417.584,1121.026,148.082,0, 'culling House WP9' ),
(26499,31,2423.123,1119.431,148.076,0, 'culling House WP10' ),
(26499,32,2447.833,1112.714,148.076,0, 'culling House WP11' ),
(26499,33,2457.133,1120.941,150.008,0, 'culling House WP12' ),
(26499,34,2459.694,1127.012,150.008,0, 'culling House WP13' ),
(26499,35,2469.617,1122.274,150.008,0, 'culling House WP14' ),
(26499,36,2470.437,1122.794,150.008,3000, 'culling House WP15' ),
(26499,37,2471.662,1123.077,150.035,3000, 'culling House WP16' ),
(26499,38,2483.183,1125.042,149.905,0, 'culling Secret WP1' ),
(26499,39,2487.867,1099.760,144.858,0, 'culling Secret WP2' ),
(26499,40,2498.270,1101.929,144.599,0, 'culling Secret WP3' ),
(26499,41,2492.114,1128.238,139.967,0, 'culling Secret WP4' ),
(26499,42,2500.286,1130.183,139.982,0, 'culling Room WP1' ),
(26499,43,2503.010,1119.241,139.978,0, 'culling Room WP2' ),
(26499,44,2517.820,1122.645,132.066,0, 'culling Room WP3' ),
(26499,45,2540.479,1129.061,130.868,7000, 'culling Fire Street WP1' ),
(26499,46,2568.619,1157.794,126.906,0, 'culling Fire Street WP2' ),
(26499,47,2556.074,1222.058,125.412,20000, 'culling Fire Street WP3' ),
(26499,48,2521.531,1295.209,130.573,0, 'culling Fire Street WP4' ),
(26499,49,2504.362,1348.667,132.944,0, 'culling Fire Street WP5' ),
(26499,50,2450.594,1431.544,131.361,0, 'culling Fire Street WP6' ),
(26499,51,2353.485,1404.839,128.531,0, 'culling Market WP1' ),
(26499,52,2329.882,1406.273,128.013,0, 'culling Market WP2' ),
(26499,53,2329.882,1406.273,128.013,12000, 'culling Market WP3' ),
(26499,54,2327.391,1412.475,127.692,0, 'culling Market WP4' ),
(26499,55,2303.016,1480.070,128.139,0, 'culling Crusader WP1' ),
(26499,56,2296.665,1502.362,128.362,0, 'culling Crusader WP2' );

-- ScriptName placement
update `creature_template` set `AIName`='', `Scriptname`='npc_arthas' where `entry` in (26499);
update `creature_template` set `AIName`='', `Scriptname`='boss_salramm' where `entry` in (26530);
update `creature_template` set `AIName`='', `Scriptname`='boss_meathook' where `entry` in (26529);
update `creature_template` set `AIName`='', `Scriptname`='boss_epoch' where `entry` in (26532);
update `creature_template` set `AIName`='', `Scriptname`='boss_mal_ganis' where `entry` in (26533);
update `instance_template` set `script` = 'instance_culling_of_stratholme' WHERE map=595;

-- Culling of Stratholme - Fire Street mobs
-- Source: http://www.youtube.com/watch?v=dFg7rtEyQS0
DELETE FROM `creature` WHERE guid >= 4459039 AND guid <= 4459247;
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(4459039, 27729, 595, 1, 1, 0, 0, 2576.53, 1174.19, 125.799, 4.5887, 60, 0, 0, 50400, 0, 0, 0),
(4459041, 27729, 595, 1, 1, 0, 0, 2552.65, 1200.16, 127.728, 4.90608, 60, 0, 0, 50400, 0, 0, 0),
(4459050, 27729, 595, 1, 1, 0, 0, 2565.16, 1217.81, 125.181, 4.94848, 60, 0, 0, 50400, 0, 0, 0),
(4459052, 27734, 595, 1, 1, 0, 0, 2545.56, 1240.36, 125.993, 5.14326, 60, 0, 0, 50400, 0, 0, 0),
(4459054, 28201, 595, 1, 1, 0, 0, 2538.96, 1264.22, 126.334, 5.16831, 60, 0, 0, 104264, 0, 0, 0),
(4459058, 28249, 595, 1, 1, 0, 0, 2527.91, 1292.33, 130.62, 4.9412, 60, 0, 0, 47144, 0, 0, 0),
(4459060, 27729, 595, 1, 1, 0, 0, 2501.27, 1351.21, 132.653, 5.13843, 60, 0, 0, 50400, 0, 0, 0),
(4459062, 27729, 595, 1, 1, 0, 0, 2488.69, 1372.49, 130.305, 5.13782, 60, 0, 0, 50400, 0, 0, 0),
(4459072, 27734, 595, 1, 1, 0, 0, 2477.79, 1379.03, 129.347, 5.28836, 60, 0, 0, 50400, 0, 0, 0),
(4459074, 27736, 595, 1, 1, 0, 0, 2466.5, 1410.46, 130.728, 5.28334, 60, 0, 0, 104264, 0, 0, 0),
(4459076, 27729, 595, 1, 1, 0, 0, 2443.01, 1431.24, 131.233, 5.21811, 60, 0, 0, 50400, 0, 0, 0),
(4459078, 27731, 595, 1, 1, 0, 0, 2515.91, 1281.52, 129.724, 5.14721, 60, 0, 0, 15750, 7988, 0, 0),
(4459089, 27731, 595, 1, 1, 0, 0, 2570.3, 1202.13, 125.54, 5.08855, 60, 0, 0, 15750, 7988, 0, 0),
(4459091, 27737, 595, 1, 1, 0, 0, 2560.54, 1169.48, 127.601, 4.47637, 60, 0, 0, 15000, 0, 0, 0),
(4459093, 27737, 595, 1, 1, 0, 0, 2566.6, 1166.85, 126.888, 4.47637, 60, 0, 0, 15000, 0, 0, 0),
(4459095, 27737, 595, 1, 1, 0, 0, 2570.14, 1159.86, 126.678, 4.34914, 60, 0, 0, 15000, 0, 0, 0),
(4459097, 27737, 595, 1, 1, 0, 0, 2562.99, 1158.59, 127.53, 3.29278, 60, 0, 0, 15000, 0, 0, 0),
(4459099, 27737, 595, 1, 1, 0, 0, 2578.47, 1156.7, 126.45, 4.59026, 60, 0, 0, 15000, 0, 0, 0),
(4459101, 27737, 595, 1, 1, 0, 0, 2579.08, 1167.59, 125.964, 3.08543, 60, 0, 0, 15000, 0, 0, 0),
(4459103, 27737, 595, 1, 1, 0, 0, 2569.55, 1178.06, 126.012, 3.42787, 60, 0, 0, 15000, 0, 0, 0),
(4459105, 27737, 595, 1, 1, 0, 0, 2571.66, 1148.78, 126.802, 4.46381, 60, 0, 0, 15000, 0, 0, 0),
(4459107, 27737, 595, 1, 1, 0, 0, 2577.25, 1140.56, 126.423, 3.47892, 60, 0, 0, 15000, 0, 0, 0),
(4459109, 27737, 595, 1, 1, 0, 0, 2561.47, 1181.77, 126.608, 4.10724, 60, 0, 0, 15000, 0, 0, 0),
(4459111, 27737, 595, 1, 1, 0, 0, 2566.87, 1193.3, 125.781, 5.83118, 60, 0, 0, 15000, 0, 0, 0),
(4459113, 27737, 595, 1, 1, 0, 0, 2574.31, 1198.34, 126.013, 4.98295, 60, 0, 0, 15000, 0, 0, 0),
(4459115, 27737, 595, 1, 1, 0, 0, 2562.47, 1201.81, 125.812, 4.3138, 60, 0, 0, 15000, 0, 0, 0),
(4459117, 27737, 595, 1, 1, 0, 0, 2563.46, 1213.01, 125.297, 4.78425, 60, 0, 0, 15000, 0, 0, 0),
(4459119, 27737, 595, 1, 1, 0, 0, 2553.6, 1212.3, 126.796, 4.78425, 60, 0, 0, 15000, 0, 0, 0),
(4459121, 27737, 595, 1, 1, 0, 0, 2558.2, 1224.74, 125.492, 4.79284, 60, 0, 0, 15000, 0, 0, 0),
(4459123, 27737, 595, 1, 1, 0, 0, 2548.17, 1225.98, 126.643, 4.79598, 60, 0, 0, 15000, 0, 0, 0),
(4459125, 27737, 595, 1, 1, 0, 0, 2551.95, 1222.84, 126.168, 5.17612, 60, 0, 0, 15000, 0, 0, 0),
(4459127, 27737, 595, 1, 1, 0, 0, 2555.51, 1237.51, 125.6, 4.67032, 60, 0, 0, 15000, 0, 0, 0),
(4459129, 27737, 595, 1, 1, 0, 0, 2538.3, 1238.15, 126.915, 5.01589, 60, 0, 0, 15000, 0, 0, 0),
(4459131, 27737, 595, 1, 1, 0, 0, 2552.39, 1248.25, 125.248, 4.6389, 60, 0, 0, 15000, 0, 0, 0),
(4459133, 27737, 595, 1, 1, 0, 0, 2535.45, 1250.57, 126.922, 5.32927, 60, 0, 0, 15000, 0, 0, 0),
(4459135, 27737, 595, 1, 1, 0, 0, 2541.02, 1254.04, 126.255, 5.32927, 60, 0, 0, 15000, 0, 0, 0),
(4459137, 27737, 595, 1, 1, 0, 0, 2547.55, 1258.28, 126.261, 4.73237, 60, 0, 0, 15000, 0, 0, 0),
(4459139, 27737, 595, 1, 1, 0, 0, 2547.09, 1252.75, 126.039, 3.85272, 60, 0, 0, 15000, 0, 0, 0),
(4459141, 27737, 595, 1, 1, 0, 0, 2531.8, 1260.09, 128.033, 5.66463, 60, 0, 0, 15000, 0, 0, 0),
(4459143, 27737, 595, 1, 1, 0, 0, 2544.61, 1267.88, 126.823, 5.28764, 60, 0, 0, 15000, 0, 0, 0),
(4459145, 27737, 595, 1, 1, 0, 0, 2512.76, 1299.76, 130.888, 4.63986, 60, 0, 0, 15000, 0, 0, 0),
(4459147, 27737, 595, 1, 1, 0, 0, 2509.8, 1294.61, 130.906, 4.63986, 60, 0, 0, 15000, 0, 0, 0),
(4459149, 27737, 595, 1, 1, 0, 0, 2511.91, 1289, 130.43, 6.05594, 60, 0, 0, 15000, 0, 0, 0),
(4459151, 27737, 595, 1, 1, 0, 0, 2516.37, 1292.37, 130.617, 6.05594, 60, 0, 0, 15000, 0, 0, 0),
(4459153, 27737, 595, 1, 1, 0, 0, 2519.41, 1297.28, 130.629, 5.77319, 60, 0, 0, 15000, 0, 0, 0),
(4459155, 27737, 595, 1, 1, 0, 0, 2523.65, 1296.02, 130.752, 4.83543, 60, 0, 0, 15000, 0, 0, 0),
(4459157, 27737, 595, 1, 1, 0, 0, 2522.49, 1286.49, 129.486, 4.27465, 60, 0, 0, 15000, 0, 0, 0),
(4459159, 27737, 595, 1, 1, 0, 0, 2519.52, 1280.15, 128.742, 4.27465, 60, 0, 0, 15000, 0, 0, 0),
(4459161, 27737, 595, 1, 1, 0, 0, 2519.54, 1273.62, 128.344, 5.78026, 60, 0, 0, 15000, 0, 0, 0),
(4459163, 27737, 595, 1, 1, 0, 0, 2523.45, 1275.68, 128.598, 5.78026, 60, 0, 0, 15000, 0, 0, 0),
(4459165, 27737, 595, 1, 1, 0, 0, 2525.14, 1278.75, 128.335, 5.78026, 60, 0, 0, 15000, 0, 0, 0),
(4459167, 27737, 595, 1, 1, 0, 0, 2531.5, 1277.13, 127.74, 5.12288, 60, 0, 0, 15000, 0, 0, 0),
(4459169, 27737, 595, 1, 1, 0, 0, 2528.62, 1273.54, 127.77, 3.99191, 60, 0, 0, 15000, 0, 0, 0),
(4459171, 27737, 595, 1, 1, 0, 0, 2526.9, 1269.35, 128.169, 5.03099, 60, 0, 0, 15000, 0, 0, 0),
(4459173, 27737, 595, 1, 1, 0, 0, 2531.23, 1268.16, 127.537, 0.475684, 60, 0, 0, 15000, 0, 0, 0),
(4459175, 27737, 595, 1, 1, 0, 0, 2536.96, 1272.03, 126.867, 5.91064, 60, 0, 0, 15000, 0, 0, 0),
(4459177, 27737, 595, 1, 1, 0, 0, 2529.73, 1285.64, 129.017, 5.23755, 60, 0, 0, 15000, 0, 0, 0),
(4459179, 27737, 595, 1, 1, 0, 0, 2533.89, 1286.98, 129.064, 5.27111, 60, 0, 0, 15000, 0, 0, 0),
(4459181, 27737, 595, 1, 1, 0, 0, 2493.52, 1339.58, 133.68, 0.261969, 60, 0, 0, 15000, 0, 0, 0),
(4459183, 27737, 595, 1, 1, 0, 0, 2493.94, 1345.46, 133.344, 0.00985568, 60, 0, 0, 15000, 0, 0, 0),
(4459185, 27737, 595, 1, 1, 0, 0, 2507.38, 1350.97, 133.011, 5.63331, 60, 0, 0, 15000, 0, 0, 0),
(4459187, 27737, 595, 1, 1, 0, 0, 2503.66, 1356.53, 132.098, 4.13005, 60, 0, 0, 15000, 0, 0, 0),
(4459189, 27737, 595, 1, 1, 0, 0, 2495.64, 1355.89, 132.006, 4.13005, 60, 0, 0, 15000, 0, 0, 0),
(4459191, 27737, 595, 1, 1, 0, 0, 2498.26, 1359.81, 131.282, 5.13222, 60, 0, 0, 15000, 0, 0, 0),
(4459193, 27737, 595, 1, 1, 0, 0, 2490.52, 1356.36, 131.711, 5.13222, 60, 0, 0, 15000, 0, 0, 0),
(4459195, 27737, 595, 1, 1, 0, 0, 2497.23, 1368.5, 130.975, 4.66098, 60, 0, 0, 15000, 0, 0, 0),
(4459197, 27737, 595, 1, 1, 0, 0, 2492.11, 1366.28, 130.792, 4.66098, 60, 0, 0, 15000, 0, 0, 0),
(4459199, 27737, 595, 1, 1, 0, 0, 2484.4, 1365.99, 130.027, 4.66098, 60, 0, 0, 15000, 0, 0, 0),
(4459201, 27737, 595, 1, 1, 0, 0, 2492.39, 1375.37, 130.36, 0.17479, 60, 0, 0, 15000, 0, 0, 0),
(4459203, 27737, 595, 1, 1, 0, 0, 2482.88, 1372.34, 129.654, 2.99908, 60, 0, 0, 15000, 0, 0, 0),
(4459205, 27737, 595, 1, 1, 0, 0, 2488.04, 1383.27, 130.238, 5.32857, 60, 0, 0, 15000, 0, 0, 0),
(4459207, 27737, 595, 1, 1, 0, 0, 2491.47, 1387.06, 130.568, 5.32857, 60, 0, 0, 15000, 0, 0, 0),
(4459209, 27737, 595, 1, 1, 0, 0, 2481.7, 1391.46, 130.048, 4.57538, 60, 0, 0, 15000, 0, 0, 0),
(4459211, 27737, 595, 1, 1, 0, 0, 2474.76, 1392.42, 129.655, 4.57538, 60, 0, 0, 15000, 0, 0, 0),
(4459213, 27737, 595, 1, 1, 0, 0, 2477.2, 1400.52, 130.455, 5.91369, 60, 0, 0, 15000, 0, 0, 0),
(4459215, 27737, 595, 1, 1, 0, 0, 2465.15, 1404.28, 130.293, 4.79136, 60, 0, 0, 15000, 0, 0, 0),
(4459217, 27737, 595, 1, 1, 0, 0, 2470.86, 1409.25, 130.984, 5.28773, 60, 0, 0, 15000, 0, 0, 0),
(4459219, 27737, 595, 1, 1, 0, 0, 2471.3, 1415.37, 131.513, 4.34525, 60, 0, 0, 15000, 0, 0, 0),
(4459221, 27737, 595, 1, 1, 0, 0, 2457.37, 1416.8, 130.267, 5.09924, 60, 0, 0, 15000, 0, 0, 0),
(4459223, 27737, 595, 1, 1, 0, 0, 2463.16, 1420.69, 130.922, 0.332655, 60, 0, 0, 15000, 0, 0, 0),
(4459225, 27737, 595, 1, 1, 0, 0, 2461.38, 1428.33, 131.14, 2.1053, 60, 0, 0, 15000, 0, 0, 0),
(4459227, 27737, 595, 1, 1, 0, 0, 2457.31, 1430.58, 131.346, 3.83317, 60, 0, 0, 15000, 0, 0, 0),
(4459229, 27737, 595, 1, 1, 0, 0, 2455.66, 1424, 130.795, 3.45383, 60, 0, 0, 15000, 0, 0, 0),
(4459231, 27737, 595, 1, 1, 0, 0, 2451.11, 1423.59, 130.626, 2.6056, 60, 0, 0, 15000, 0, 0, 0),
(4459233, 27737, 595, 1, 1, 0, 0, 2450.49, 1430.84, 131.274, 0.586337, 60, 0, 0, 15000, 0, 0, 0),
(4459235, 27737, 595, 1, 1, 0, 0, 2450.3, 1432.57, 131.469, 1.98042, 60, 0, 0, 15000, 0, 0, 0),
(4459237, 27737, 595, 1, 1, 0, 0, 2445.13, 1437.02, 131.758, 3.54965, 60, 0, 0, 15000, 0, 0, 0),
(4459239, 27737, 595, 1, 1, 0, 0, 2439.99, 1433.47, 131.366, 4.58402, 60, 0, 0, 15000, 0, 0, 0),
(4459241, 27737, 595, 1, 1, 0, 0, 2434.27, 1428.67, 131.239, 5.71263, 60, 0, 0, 15000, 0, 0, 0),
(4459243, 27737, 595, 1, 1, 0, 0, 2438.5, 1425.95, 130.855, 5.71263, 60, 0, 0, 15000, 0, 0, 0),
(4459245, 27737, 595, 1, 1, 0, 0, 2440.8, 1423.15, 130.543, 5.71263, 60, 0, 0, 15000, 0, 0, 0),
(4459247, 27737, 595, 1, 1, 0, 0, 2446.09, 1423.63, 130.578, 4.94757, 60, 0, 0, 15000, 0, 0, 0);

-- Arthas placed in front of Stratholme bridge
DELETE FROM `creature` WHERE (`guid`=4456649);
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (4456649, 26499, 595, 1, 0, 0, 1920.87, 1287.12, 142.935, 6.25562, 25, 0, 0, 44100, 7988, 0, 0);

-- Doors inside Culling redone
DELETE FROM `gameobject_template` WHERE `entry` in (188686,187711,187723);
REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `castBarCaption`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES 
(188686, 0, 7831, 'Doodad_LD_hidden_door_room01', '', 1375, 6553632, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(187711, 0, 3631, 'Crusaders\' Square Gate', '', 1375, 6553632, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(187723, 0, 3631, 'Crusaders\' Square Gate', '', 1375, 6553632, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

-- Delete all bosses and creatures that could be spawned in the instance
DELETE FROM `creature` WHERE (`id`=27744);
DELETE FROM `creature` WHERE (`id`=26530);
DELETE FROM `creature` WHERE (`id`=26529);
DELETE FROM `creature` WHERE (`id`=26532);
DELETE FROM `creature` WHERE (`id`=26533);

-- Some fixes to make Chest work
UPDATE `gameobject_template` SET `flags`='4' WHERE `gameobject_template`.`entry` in (190663,193597);
DELETE FROM `gameobject` WHERE `gameobject`.`id` in (190663,193597);
REPLACE INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(67603, 190663, 595, 1, 1, 2288.35, 1498.73, 128.414, -0.994837, 0, 0, 0, 1, 180, 0, 1),
(500000, 193597, 595, 2, 1, 2288.35, 1498.73, 128.414, -0.994837, 0, 0, 0, 1, 180, 0, 1);

-- Some updates that know i really don't know if are needed anymore
-- UPDATE `creature_template` SET `minhealth`=8600, `maxhealth`=8600 where `entry` in (31127, 31126, 28167, 28169);
UPDATE `creature_template` SET `faction_A`=35, `faction_H`=35 where `entry` in (31127, 31126, 28167, 28169);
-- UPDATE `creature_template` SET `minhealth`=15000, `maxhealth`=15000 where `entry` in (27737);
UPDATE `creature_template` SET `MovementType` = 2 WHERE `creature_template`.`entry` = 26528;
UPDATE `creature` SET `spawntimesecs`='3600' where `id` in (31127, 31126, 28167, 28169);
UPDATE `gameobject` SET `state`='1' where `id`=187711;