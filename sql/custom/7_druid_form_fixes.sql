delete from `spell_required` where `spell_id` = 9634;
insert into `spell_required` values (9634,5487);
delete from `spell_ranks` where `first_spell_id` = 5487;
insert into `spell_ranks` values (5487,5487,1),(5487,9634,2);
delete from `spell_ranks` where `first_spell_id` = 33943;
insert into `spell_ranks` values (33943,33943,1),(33943,40120,2);
