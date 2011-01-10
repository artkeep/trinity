﻿DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (37107,37158,37774,37582,21925,37496,37587,37584,37588,37587,37498,37583,37779,36522,36499,36478,36666,36551,36564,36620,36516);
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_inverse_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`action2_type`,`action2_param1`,`action2_param2`,`action2_param3`,`action3_type`,`action3_param1`,`action3_param2`,`action3_param3`,`comment`) VALUES
 ('3777410', '37774', '0', '0', '100', '7', '11000', '11000', '20000', '45000', '11', '22746', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Cast Cone of Cold'),
 ('3777407', '37774', '0', '0', '100', '2', '0', '0', '0', '0', '21', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Stop Movement on Aggro'),
 ('3777411', '37774', '4', '0', '100', '2', '0', '0', '0', '0', '11', '51779', '1', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Cast Frostfire Bolt and Set Phase 1 on Aggro'),
 ('3777408', '37774', '3', '0', '100', '2', '15', '0', '0', '0', '21', '1', '0', '0', '22', '5', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Start Movement and Set Phase 2 when Mana is at 15%'),
 ('3777409', '37774', '0', '6', '100', '3', '0', '0', '1500', '3000', '11', '51779', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Cast Frostfire Bolt above 15% Mana (Phase 1)'),
 ('3777414', '37774', '3', '5', '100', '2', '100', '28', '0', '0', '21', '0', '0', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Set Ranged Movement and Set Phase 1 when Mana is above 28% (Phase 2)'),
 ('3777403', '37774', '9', '0', '100', '3', '30', '50', '0', '0', '22', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Start Movement Beyond 30 Yards'),
 ('3777402', '37774', '7', '0', '100', '2', '0', '0', '0', '0', '22', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - On Evade set Phase to 0'),
 ('3777400', '37774', '0', '0', '100', '4', '0', '0', '0', '0', '21', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Stop Movement on Aggro'),
 ('3777404', '37774', '4', '0', '100', '4', '0', '0', '0', '0', '11', '70616', '1', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Cast Frostfire Bolt and Set Phase 1 on Aggro'),
 ('3777406', '37774', '3', '0', '100', '4', '15', '0', '0', '0', '21', '1', '0', '0', '22', '5', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Start Movement and Set Phase 2 when Mana is at 15%'),
 ('3777413', '37774', '0', '6', '100', '5', '0', '0', '1500', '3000', '11', '70616', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Cast Frostfire Bolt above 15% Mana (Phase 1)'),
 ('3777401', '37774', '3', '5', '100', '4', '100', '28', '0', '0', '21', '0', '0', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Set Ranged Movement and Set Phase 1 when Mana is above 28% (Phase 2)'),
 ('3777412', '37774', '9', '0', '100', '5', '30', '50', '0', '0', '22', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - Start Movement Beyond 30 Yards'),
 ('3777405', '37774', '7', '0', '100', '4', '0', '0', '0', '0', '22', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Elandra - On Evade set Phase to 0'),
 ('3758209', '37582', '0', '0', '100', '7', '11000', '11000', '20000', '45000', '11', '22746', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Cast Cone of Cold'),
 ('3758210', '37582', '0', '0', '100', '2', '0', '0', '0', '0', '21', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Stop Movement on Aggro'),
 ('3758211', '37582', '4', '0', '100', '2', '0', '0', '0', '0', '11', '51779', '1', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Cast Frostfire Bolt and Set Phase 1 on Aggro'),
 ('3758212', '37582', '3', '0', '100', '2', '15', '0', '0', '0', '21', '1', '0', '0', '22', '5', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Start Movement and Set Phase 2 when Mana is at 15%'),
 ('3758215', '37582', '0', '6', '100', '3', '0', '0', '1500', '3000', '11', '51779', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Cast Frostfire Bolt above 15% Mana (Phase 1)'),
 ('3758202', '37582', '3', '5', '100', '2', '100', '28', '0', '0', '21', '0', '0', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Set Ranged Movement and Set Phase 1 when Mana is above 28% (Phase 2)'),
 ('3758214', '37582', '9', '0', '100', '3', '30', '50', '0', '0', '22', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Start Movement Beyond 30 Yards'),
 ('3758213', '37582', '7', '0', '100', '2', '0', '0', '0', '0', '22', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - On Evade set Phase to 0'),
 ('3758207', '37582', '0', '0', '100', '4', '0', '0', '0', '0', '21', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Stop Movement on Aggro'),
 ('3758206', '37582', '4', '0', '100', '4', '0', '0', '0', '0', '11', '70616', '1', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Cast Frostfire Bolt and Set Phase 1 on Aggro'),
 ('3758205', '37582', '3', '0', '100', '4', '15', '0', '0', '0', '21', '1', '0', '0', '22', '5', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Start Movement and Set Phase 2 when Mana is at 15%'),
 ('3758200', '37582', '0', '6', '100', '5', '0', '0', '1500', '3000', '11', '70616', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Cast Frostfire Bolt above 15% Mana (Phase 1)'),
 ('3758201', '37582', '3', '5', '100', '4', '100', '28', '0', '0', '21', '0', '0', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Set Ranged Movement and Set Phase 1 when Mana is above 28% (Phase 2)'),
 ('3758208', '37582', '9', '0', '100', '5', '30', '50', '0', '0', '22', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Start Movement Beyond 30 Yards'),
 ('3758203', '37582', '7', '0', '100', '4', '0', '0', '0', '0', '22', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - On Evade set Phase to 0'),
 ('3758204', '37582', '0', '0', '100', '7', '12000', '12000', '19000', '24000', '11', '22645', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Archmage Koreln - Casts Frost Nova'),
 ('2192507', '21925', '0', '0', '100', '7', '15000', '15500', '25500', '30000', '11', '34017', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Avatar of Sathal - Casts Rain of Chaos'),
 ('2192506', '21925', '0', '0', '100', '6', '0', '0', '0', '0', '21', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Avatar of Sathal - Stop Movement on Aggro'),
 ('2192505', '21925', '4', '0', '100', '6', '0', '0', '0', '0', '11', '12471', '1', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Avatar of Sathal - Cast Frostbolt and Set Phase 1 on Aggro'),
 ('2192504', '21925', '3', '0', '100', '6', '15', '0', '0', '0', '21', '1', '0', '0', '22', '5', '0', '0', '0', '0', '0', '0', 'Avatar of Sathal - Start Movement and Set Phase 2 when Mana is at 15%'),
 ('2192503', '21925', '0', '6', '100', '7', '0', '0', '1500', '3000', '11', '12471', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Avatar of Sathal - Cast FrostBolt above 15% Mana (Phase 1)'),
 ('2192502', '21925', '3', '5', '100', '6', '100', '28', '0', '0', '21', '0', '0', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Avatar of Sathal - Set Ranged Movement and Set Phase 1 when Mana is above 28% (Phase 2)'),
 ('2192501', '21925', '9', '0', '100', '7', '30', '50', '0', '0', '22', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Avatar of Sathal - Start Movement Beyond 30 Yards'),
 ('2192500', '21925', '7', '0', '100', '6', '0', '0', '0', '0', '22', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Avatar of Sathal - On Evade set Phase to 0'),
 ('3749601', '37496', '2', '0', '100', '7', '10', '0', '10300', '12900', '11', '69350', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Raise Dead When Below 10% HP'),
 ('3749600', '37496', '0', '0', '100', '7', '10000', '14000', '20500', '22900', '11', '69413', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Strangulating'),
 ('3758703', '37587', '2', '0', '100', '7', '10', '0', '10300', '12900', '11', '69350', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Raise Dead When Below 10% HP'),
 ('3758701', '37587', '0', '0', '100', '7', '10000', '14000', '20500', '22900', '11', '69413', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Strangulating'),
 ('3758400', '37584', '2', '0', '100', '7', '10', '0', '10300', '12900', '11', '69350', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Raise Dead When Below 10% HP'),
 ('3758401', '37584', '0', '0', '100', '7', '10000', '14000', '20500', '22900', '11', '69413', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Strangulating'),
 ('3758800', '37588', '2', '0', '100', '7', '10', '0', '10300', '12900', '11', '69350', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Raise Dead When Below 10% HP'),
 ('3758801', '37588', '0', '0', '100', '7', '10000', '14000', '20500', '22900', '11', '69413', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Strangulating'),
 ('3758700', '37587', '2', '0', '100', '7', '10', '0', '10300', '12900', '11', '69350', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Raise Dead When Below 10% HP'),
 ('3758702', '37587', '0', '0', '100', '7', '10000', '14000', '20500', '22900', '11', '69413', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Strangulating'),
 ('3749802', '37498', '2', '0', '100', '7', '10', '0', '10300', '12900', '11', '69350', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Raise Dead When Below 10% HP'),
 ('3749803', '37498', '0', '0', '100', '7', '10000', '14000', '20500', '22900', '11', '69413', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Coliseum Champion  - Cast Strangulating'),
 ('3758316', '37583', '4', '0', '100', '2', '0', '0', '0', '0', '11', '31942', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Dark Ranger Kalira - Cast Multi-Shot on Aggro'),
 ('3758317', '37583', '4', '0', '100', '4', '0', '0', '0', '0', '11', '70513', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Dark Ranger Kalira - Cast Multi-Shot on Aggro'),
 ('3777900', '37779', '4', '0', '100', '2', '0', '0', '0', '0', '11', '31942', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Dark Ranger Loralen - Cast Multi-Shot on Aggro'),
 ('3777901', '37779', '4', '0', '100', '4', '0', '0', '0', '0', '11', '70513', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Dark Ranger Loralen - Cast Multi-Shot on Aggro'),
 ('3652200', '36522', '0', '0', '100', '3', '8000', '10000', '16000', '22000', '11', '69088', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soul Horror - Casts Soul Strike'),
 ('3652201', '36522', '0', '0', '100', '5', '8000', '10000', '16000', '22000', '11', '70211', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soul Horror - Casts Soul Strike'),
 ('3649902', '36499', '0', '0', '100', '3', '12000', '15000', '19000', '24000', '11', '69060', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Reaper  - Casts Frost Nova'),
 ('3649901', '36499', '0', '0', '100', '5', '12000', '15000', '19000', '24000', '11', '70209', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Reaper  - Casts Frost Nova'),
 ('3649900', '36499', '0', '0', '100', '7', '10000', '14000', '22000', '29000', '11', '69058', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Reaper - Casts Shadow Lance'),
 ('3647800', '36478', '0', '0', '100', '7', '9000', '14000', '25000', '30000', '11', '69056', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Watchman  - Casts Shroud of Runes'),
 ('3647801', '36478', '1', '0', '100', '7', '1000', '1000', '1800000', '1800000', '11', '69053', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Watchman - Casts Unholy Rage on Spawn'),
 ('3666600', '36666', '0', '0', '100', '7', '10000', '14000', '27000', '30000', '11', '69633', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Spectral Warden  - Casts Spectral Warden'),
 ('3666602', '36666', '0', '0', '100', '3', '15000', '17000', '29000', '35000', '11', '69148', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Spectral Warden - Cast Wail of Souls'),
 ('3666601', '36666', '0', '0', '100', '5', '15000', '17000', '29000', '35000', '11', '70210', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Spectral Warden - Cast Wail of Souls'),
 ('3655100', '36551', '0', '0', '100', '3', '15000', '17000', '29000', '35000', '11', '68895', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Spiteful Apparition - Cast Spite'),
 ('3655101', '36551', '0', '0', '100', '5', '15000', '17000', '29000', '35000', '11', '70212', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Spiteful Apparition - Cast Spite'),
 ('3656401', '36564', '0', '0', '100', '3', '15000', '17000', '29000', '35000', '11', '69080', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Bonecaster - Cast Bone Volley'),
 ('3656404', '36564', '0', '0', '100', '5', '15000', '17000', '29000', '35000', '11', '70206', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Bonecaster - Cast Bone Volley'),
 ('3656402', '36564', '0', '0', '100', '3', '5000', '7000', '17000', '19000', '11', '69069', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Bonecaster - Cast Shield of Bones'),
 ('3656405', '36564', '0', '0', '100', '5', '5000', '7000', '17000', '19000', '11', '70207', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Bonecaster - Cast Shield of Bones'),
 ('3656403', '36564', '4', '0', '100', '2', '0', '0', '0', '0', '11', '31942', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Bonecaster - Cast Raise Dead'),
 ('3656400', '36564', '4', '0', '100', '4', '0', '0', '0', '0', '11', '70513', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Bonecaster - Cast Raise Dead'),
 ('3662002', '36620', '2', '0', '100', '3', '55', '0', '10300', '12900', '11', '69066', '4', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept  - Cast Drain Life When Below 25% HP'),
 ('3662004', '36620', '2', '0', '100', '5', '55', '0', '10300', '12900', '11', '70213', '4', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept  - Cast Drain Life When Below 25% HP'),
 ('3662000', '36620', '0', '0', '100', '7', '5000', '7000', '29000', '35000', '11', '69562', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adep - Cast Raise Dead'),
 ('3662001', '36620', '14', '0', '90', '2', '320', '40', '0', '0', '11', '69564', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adep - Casts Shadow Mend on Friendlies'),
 ('3662003', '36620', '14', '0', '90', '4', '320', '40', '0', '0', '11', '70205', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adep - Casts Shadow Mend on Friendlies'),
 ('3651609', '36516', '0', '0', '100', '2', '0', '0', '0', '0', '21', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Stop Movement on Aggro'),
 ('3651611', '36516', '4', '0', '100', '2', '0', '0', '0', '0', '11', '69068', '1', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Cast Shadow Bolt and Set Phase 1 on Aggro'),
 ('3651612', '36516', '3', '0', '100', '2', '15', '0', '0', '0', '21', '1', '0', '0', '22', '5', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Start Movement and Set Phase 2 when Mana is at 15%'),
 ('3651613', '36516', '0', '6', '100', '3', '0', '0', '1500', '3000', '11', '69068', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Shadow Bolt above 15% Mana (Phase 1)'),
 ('3651600', '36516', '3', '5', '100', '2', '100', '28', '0', '0', '21', '0', '0', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Set Ranged Movement and Set Phase 1 when Mana is above 28% (Phase 2)'),
 ('3651610', '36516', '9', '0', '100', '3', '30', '50', '0', '0', '22', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Start Movement Beyond 30 Yards'),
 ('3651608', '36516', '7', '0', '100', '2', '0', '0', '0', '0', '22', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - On Evade set Phase to 0'),
 ('3651615', '36516', '0', '0', '100', '4', '0', '0', '0', '0', '21', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Stop Movement on Aggro'),
 ('3651607', '36516', '4', '0', '100', '4', '0', '0', '0', '0', '11', '70208', '1', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Cast Shadow Bolt and Set Phase 1 on Aggro'),
 ('3651606', '36516', '3', '0', '100', '4', '15', '0', '0', '0', '21', '1', '0', '0', '22', '5', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Start Movement and Set Phase 2 when Mana is at 15%'),
 ('3651604', '36516', '0', '6', '100', '5', '0', '0', '1500', '3000', '11', '70208', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Cast Shadow Bolt above 15% Mana (Phase 1)'),
 ('3651603', '36516', '3', '5', '100', '4', '100', '28', '0', '0', '21', '0', '0', '0', '22', '6', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Set Ranged Movement and Set Phase 1 when Mana is above 28% (Phase 2)'),
 ('3651602', '36516', '9', '0', '100', '5', '30', '50', '0', '0', '22', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - Start Movement Beyond 30 Yards'),
 ('3651601', '36516', '7', '0', '100', '4', '0', '0', '0', '0', '22', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adept - On Evade set Phase to 0'),
 ('3651616', '36516', '0', '0', '100', '7', '5000', '7000', '29000', '35000', '11', '69562', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Adep - Cast Raise Dead'),
 ('3651614', '36516', '0', '0', '100', '7', '12000', '15000', '19000', '24000', '11', '69128', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Animator  - Casts Soul Siphon'),
 ('3651605', '36516', '0', '0', '100', '7', '10000', '14000', '22000', '28000', '11', '69131', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Soulguard Animator  - Casts Soul Sickness');

UPDATE creature_template SET  AIName='EventAI' WHERE entry IN   (37107,37158,37774,37582,21925,37496,37587,37584,37588,37587,37498,37583,37779,36522,36499,36478,36666,36551,36564,36620,36516);
