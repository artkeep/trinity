UPDATE `creature_template` SET `unit_flags`='2' WHERE (`entry`='22112');

DELETE FROM spell_script_names WHERE spell_id=-47540 AND ScriptName='spell_pri_penance';
DELETE FROM spell_script_names WHERE spell_id=53005 AND ScriptName='spell_pri_penance';
DELETE FROM spell_script_names WHERE spell_id=53006 AND ScriptName='spell_pri_penance';
DELETE FROM spell_script_names WHERE spell_id=53007 AND ScriptName='spell_pri_penance';
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES
(-47540,'spell_pri_penance'),
(53005,'spell_pri_penance'),
(53006,'spell_pri_penance'),
(53007,'spell_pri_penance');