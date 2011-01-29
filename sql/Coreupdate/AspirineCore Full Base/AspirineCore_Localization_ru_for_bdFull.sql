set names 'utf8'; 
-- Wintergrasp
DELETE FROM `trinity_string` WHERE entry IN (756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,780,781);
REPLACE INTO `trinity_string` VALUES ('756', 'Battle begins!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Битва началась');
REPLACE INTO `trinity_string` VALUES ('757', '%s has successfully defended the fortress!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '%s успешно защитил(а) крепость!');
REPLACE INTO `trinity_string` VALUES ('758', '%s has taken over the fortress!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '%s захватил(а) крепость');
REPLACE INTO `trinity_string` VALUES ('759', 'The %s siege workshop has been damaged by the %s!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('760', 'The %s siege workshop has been destroyed by the %s!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('761', 'The %s tower has been damaged!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '%s башня повреждена');
REPLACE INTO `trinity_string` VALUES ('762', 'The %s tower has been destroyed!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '%s башня уничтожена!');
REPLACE INTO `trinity_string` VALUES ('763', 'Wintergrasp fortress is under attack!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('764', 'Wintergrasp is now under the control of the %s.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('765', 'Wintergrasp timer set to %s.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('766', 'Wintergrasp battle started.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('767', 'Wintergrasp battle finished.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('768', 'Wintergrasp info: %s controlled. Timer: %s. Wartime: %s. Number of Players: (Horde: %u, Alliance: %u)', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('769', 'Wintergrasp outdoorPvP is disabled.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('770', 'Wintergrasp outdoorPvP is enabled.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` VALUES ('771', 'You have reached Rank 1: Corporal', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы достигли Ранга 1: Капрал');
REPLACE INTO `trinity_string` VALUES ('772', 'You have reached Rank 2: First Lieutenant', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы достигли Ранга 2: Лейтенант');
REPLACE INTO `trinity_string` VALUES ('780', 'Before the Battle of  Wintergrasp left 30 minutes!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'До битвы на  Озере Ледяных Оков осталось 30 минут!');
REPLACE INTO `trinity_string` VALUES ('781', 'Before the Battle of  Wintergrasp left 10 minutes!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'До битвы на  Озере Ледяных Оков осталось 10 минут!');
REPLACE INTO `trinity_string` VALUES ('782', 'The battle for Wintergrasp  has stopped! Not enough defenders. Wintergrasp Fortress remains  Attackers.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Битва за Озеро Ледяных Оков Остановлена. Не хватает защитников. Крепость переходит атакующей  стороне.');
REPLACE INTO `trinity_string` VALUES ('783', 'The battle for Wintergrasp  has stopped! Not enough attackers. Wintergrasp Fortress remains  Defenders.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Битва за Озеро Ледяных Оков Остановлена. Не хватает нападающих. Крепость остается защитникам.');

DELETE FROM `command` WHERE name IN ('wg','wg enable','wg start','wg status','wg stop','wg switch','wg timer');
REPLACE INTO `command` VALUES ('wg', '3', 'Syntax: .wg $subcommand.');
REPLACE INTO `command` VALUES ('wg enable', '3', 'Syntax: .wg enable [on/off] Вкл/Выкл Wintergrasp outdoorPvP.');
REPLACE INTO `command` VALUES ('wg start', '3', 'Syntax: .wg start\r\nForce Wintergrasp battle старт.');
REPLACE INTO `command` VALUES ('wg status', '3', 'Syntax: .wg status\r\nWintergrasp info, defender, timer, wartime.');
REPLACE INTO `command` VALUES ('wg stop', '3', 'Syntax: .wg stop\r\nForce Wintergrasp battle стоп (No rewards).');
REPLACE INTO `command` VALUES ('wg switch', '3', 'Syntax: .wg switch\r\nSwitchs Wintergrasp defender команды.');
REPLACE INTO `command` VALUES ('wg timer', '3', 'Syntax: .wg timer $minutes\r\nИзменить текущий таймер. Min value = 1, Max value 60 (Wartime), 1440 (Not Wartime)');

-- Icecrown Citadel
-- Лорд Ребрад

UPDATE `script_texts` SET `content_loc8`='Смертные, ваш путь закончится так и не начавшись! Никто не смеет входить в Обитель Господина!' WHERE `entry`=-1631000;
UPDATE `script_texts` SET `content_loc8`='Плеть накроет этот мир волной смерти и разрушения!' WHERE `entry`=-1631001;
UPDATE `script_texts` SET `content_loc8`='ВИХРЬ КОСТЕЙ!' WHERE `entry`=-1631002;
UPDATE `script_texts` SET `content_loc8`='Костяная хватка!' WHERE `entry`=-1631003;
UPDATE `script_texts` SET `content_loc8`='Постой-ка тут!' WHERE `entry`=-1631004;
UPDATE `script_texts` SET `content_loc8`='Единственный выход - это СМЕРТЬ!' WHERE `entry`=-1631005;
UPDATE `script_texts` SET `content_loc8`='Кости, для жертвоприношений!' WHERE `entry`=-1631006;
UPDATE `script_texts` SET `content_loc8`='Сгинь в вечных муках!' WHERE `entry`=-1631007;
UPDATE `script_texts` SET `content_loc8`='Я вижу... лишь тьму' WHERE `entry`=-1631008;
UPDATE `script_texts` SET `content_loc8`='Во мне бушует ярость господина!' WHERE `entry`=-1631009;


-- Леди Смертный Шепот

UPDATE `script_texts` SET `content_loc8`='Вы здесь потому, что наделены особым знанием: вы понимаете, что на мир пало проклятье слепоты!' WHERE `entry`=-1631011;
UPDATE `script_texts` SET `content_loc8`='Вам удалось сквозь пелену лжи разглядеть источник истиной силы!' WHERE `entry`=-1631012;
UPDATE `script_texts` SET `content_loc8`='Посмотрите на свои руки, задумайтесь над нелепостью их строения!' WHERE `entry`=-1631013;
UPDATE `script_texts` SET `content_loc8`='Кожа, мускулы, кровь, что пульсирует в жилах, все это говорит о вашем несовершенстве, вашей слабости. Создатели жестоко пошутили над вами!' WHERE `entry`=-1631014;
UPDATE `script_texts` SET `content_loc8`='Чем раньше вы осознаете собственную ущербность, тем скорее выберете иной путь!' WHERE `entry`=-1631015;
UPDATE `script_texts` SET `content_loc8`='Для нашего господина нет ничего невозможного, его сила не знает границ, ничто не может сломить его волю!' WHERE `entry`=-1631016;
UPDATE `script_texts` SET `content_loc8`='Тот, кто откажется повиноваться будет уничтожен. Тот же, кто будет служить ему верой и правдой, достигнет таких высот, о которых вы не в силах даже помыслить!' WHERE `entry`=-1631017;
UPDATE `script_texts` SET `content_loc8`='Как вы смеете ступать в эти священные покои, это место станет вашей могилой!' WHERE `entry`=-1631018;
UPDATE `script_texts` SET `content_loc8`='Довольно! Пришла пора взять все в свои руки!' WHERE `entry`=-1631019;
UPDATE `script_texts` SET `content_loc8`='Прими это благословение и покажи чужакам мощь нашего господина!' WHERE `entry`=-1631022;
UPDATE `script_texts` SET `content_loc8`='Я освобождаю тебя от проклятья плоти, мой верный слуга!' WHERE `entry`=-1631023;
UPDATE `script_texts` SET `content_loc8`='Восстань и обрети истинную форму!' WHERE `entry`=-1631024;
UPDATE `script_texts` SET `content_loc8`='Ты не в силах противится моей воле!' WHERE `entry`=-1631021;
UPDATE `script_texts` SET `content_loc8`='Мне надоел этот фарс!' WHERE `entry`=-1631027;
UPDATE `script_texts` SET `content_loc8`='На все воля господина...Ваша смерть неизбежна...' WHERE `entry`=-1631028;
UPDATE `script_texts` SET `content_loc8`='Вы осознали бессмысленность своих действий?' WHERE `entry`=-1631025;
UPDATE `script_texts` SET `content_loc8`='Ступай во тьму... Вечную тьму!' WHERE `entry`=-1631026;


-- Бой на кораблях

UPDATE `script_texts` SET `content_loc8`='Благодарю духов, что они послали нам вас. "Усмиритель небес" уже улетел. Быстрее, на борт "Молота Оргрима". Ели вы поспешите, то ещё успеете его нагнать.' WHERE `entry`=-1665919;
UPDATE `script_texts` SET `content_loc8`='Вот это поможет!' WHERE `entry`=-1665920;
UPDATE `script_texts` SET `content_loc8`='Ака''Магош, храбрые войны. Солдат Альянса тут очень много.' WHERE `entry`=-1665921;
UPDATE `script_texts` SET `content_loc8`='Капитан Саурфанг будет рад видеть вас на борту "Молота Оргрима". Мы будем сдерживать Альянс, пока вы не будете готовы к взлету.' WHERE `entry`=-1665922;
UPDATE `script_texts` SET `content_loc8`='Воспряньте, сыны и дочери Орды! Сегодня мы будем биться со смертельным врагом! ЛОК''ТАР ОГАР!' WHERE `entry`=-1665925;
UPDATE `script_texts` SET `content_loc8`='Вы видите? Что это там в далеке?' WHERE `entry`=-1665926;
UPDATE `script_texts` SET `content_loc8`='БОЕВОЙ КОРАБЛЬ АЛЬЯНСА! ВСЕ НА ПАЛУБУ!' WHERE `entry`=-1665927;
UPDATE `script_texts` SET `content_loc8`='Ружейники, ОГОНЬ!' WHERE `entry`=-1665932;
UPDATE `script_texts` SET `content_loc8`='Мортирщики, заряжай!' WHERE `entry`=-1665933;
UPDATE `script_texts` SET `content_loc8`='Пехота, сержанты, в бой!' WHERE `entry`=-16659309;
UPDATE `script_texts` SET `content_loc8`='Как ты СМЕЕШЬ ступать на мой корабль? Сейчас ты умрешь.' WHERE `entry`=-1665931;
UPDATE `script_texts` SET `content_loc8`='Двигай свою калошу, не то мы ее прямо в небе взорвем! Орде тут делать нечего!' WHERE `entry`=-1665928;
UPDATE `script_texts` SET `content_loc8`='Корабль под обстрелом! Заклинателя сюда, пусть заткнет эти пушки!' WHERE `entry`=-1665934;
UPDATE `script_texts` SET `content_loc8`='Альянс повержен! Вперед, к Королю-личу!' WHERE `entry`=-1665935;
UPDATE `script_texts` SET `content_loc8`='Ты скоро поймешь, какие у нас тут дела! Кор''крон, уничтожить их!' WHERE `entry`=-1665929;

-- Саурфанг Смертоносный

UPDATE `script_texts` SET `content_loc8`='ВО ИМЯ КОРОЛЯ-ЛИЧА!' WHERE `entry`=-1631045;
UPDATE `script_texts` SET `content_loc8`='Земля обагрится вашей кровью!' WHERE `entry`=-1631046;
UPDATE `script_texts` SET `content_loc8`='Веселитесь, слуги мои!' WHERE `entry`=-1631047;
UPDATE `script_texts` SET `content_loc8`='Ты ничтожество!' WHERE `entry`=-1631048;
UPDATE `script_texts` SET `content_loc8`='Твоя душа не обретет покоя!' WHERE `entry`=-1631049;
UPDATE `script_texts` SET `content_loc8`='Я... Cтал СМЕРТЬЮ!' WHERE `entry`=-1631051;
UPDATE `script_texts` SET `content_loc8`='Я... Освободился.' WHERE `entry`=-1631052;
UPDATE `script_texts` SET `content_loc8`='Все павшие войны Орды. Все дохлые псы Альянса. Все пополнят Армию Короля-лича. Даже сейчас Валь`киры воскрешают ваших покойников, чтобы те стали частью Плети!' WHERE `entry`=-1631030;
UPDATE `script_texts` SET `content_loc8`='Сейчас всё будет ещё хуже. Идите сюда, я покажу вам, какой силой меня наделил Король-лич!' WHERE `entry`=-1631031;
UPDATE `script_texts` SET `content_loc8`='Ха-ха-ха! Дворфы...' WHERE `entry`=-1631034;
UPDATE `script_texts` SET `content_loc8`='Присоединись ко мне, отец. Перейди на мою сторону, и вместе мы разрушим этот мир во имя Плети и во славу Короля-лича!' WHERE `entry`=-1631037;
UPDATE `script_texts` SET `content_loc8`='Старый упрямец. У тебя нет шансов! Я сильнее и могущественнее, чем ты можешь представить!' WHERE `entry`=-1631039;
UPDATE `script_texts` SET `content_loc8`='Жалкий старик! Ну что ж, герои. Хотите узнать, сколь могущественна Плеть?' WHERE `entry`=-1631044;

-- Эвент Саурфанга(Мурадин, СС, Вариан)

UPDATE `script_texts` SET `content_loc8`='Один орк против мощи Альянса?' WHERE `entry`=-1631032;
UPDATE `script_texts` SET `content_loc8`='Кор''крон, выдвигайтесь! Герои, будьте начеку. Плеть только что...' WHERE `entry`=-1631036;
UPDATE `script_texts` SET `content_loc8`='Мой мальчик погиб у Врат Гнева. Я здесь, чтобы забрать его тело.' WHERE `entry`=-1631038;
UPDATE `script_texts` SET `content_loc8`='Мы назвали его Дранош - на орочьем это значит "Сердце Дренора". Я бы не позволил чернокнижникам збрать его. Он был бы в безопасности в Гарадаре под защитой старейшин.' WHERE `entry`=-1631040;
UPDATE `script_texts` SET `content_loc8`='Я пообещал его матери, когда она умирала, что пройду через Темный Портал один. неважно, умер бы я или выжил - мой сын остался бы цел. И чист...' WHERE `entry`=-1631041;
UPDATE `script_texts` SET `content_loc8`='Сегодня я исполню это обещание.' WHERE `entry`=-1631042;
UPDATE `script_texts` SET `content_loc8`='Мы похороним тебя как подобает, в Награнде, рядом с матерью и предками...' WHERE `entry`=-1666002;
UPDATE `script_texts` SET `content_loc8`='Помните о чести, герои... какой бы жестокой не была битва... никогда не трекайтесь от неё.' WHERE `entry`=-1666003;
UPDATE `script_texts` SET `content_loc8`='Мурадин защищай палубу, и приготовь солдат к штурму верхних этажей Цитадели. Я вызову из Штормграда подкрепление.' WHERE `entry`=-1665998;
UPDATE `script_texts` SET `content_loc8`='Отступи, Мурадин. Позволь пройти скорбящему отцу.' WHERE `entry`=-1631063;
UPDATE `script_texts` SET `content_loc8`='Я... Я не был у Врат Гнева. Но многое узнал от выживших солдат. Твой сын сражался достойно. Он пал смертью героя. И заслуживает погребения с почестями.' WHERE `entry`=-1631068;
UPDATE `script_texts` SET `content_loc8`='Джайна, почему ты плачешь?' WHERE `entry`=-1631070;


-- Гниломорд

UPDATE `script_texts` SET `content_loc8`='Что? Прелесть? Нееееееееееееет!!!' WHERE `entry`=-1631094;
UPDATE `script_texts` SET `content_loc8`='УУИИИИИИ!' WHERE `entry`=-1631095;
UPDATE `script_texts` SET `content_loc8`='Липучка-вонючка!' WHERE `entry`=-1631097;
UPDATE `script_texts` SET `content_loc8`='Отличные новости, народ! Слизь снова потекла!' WHERE `entry`=-1631091;
UPDATE `script_texts` SET `content_loc8`='Отличные новости, народ! Я починил трубы для подачи ядовитой слизи!' WHERE `entry`=-1631092;
UPDATE `script_texts` SET `content_loc8`='Папочка сделает новые игрушки из вас!' WHERE `entry`=-1631100;
UPDATE `script_texts` SET `content_loc8`='Я это заломал...' WHERE `entry`=-1631101;
UPDATE `script_texts` SET `content_loc8`='Папочка, не огорчайся…' WHERE `entry`=-1631103;
UPDATE `script_texts` SET `content_loc8`='Я сделал очень злую каку! Сейчас взорвется!' WHERE `entry`=-1631099;

-- Тухлопуз

UPDATE `script_texts` SET `content_loc8`='Тухлопуз, ты всегда был моим любимчиком, как и Гниломорд! Молодец, что оставил столько газа. Я его даже чувствую!' WHERE `entry`=-1631090;
UPDATE `script_texts` SET `content_loc8`='Нет! Вы убили Вонючку! Сейчас получите!' WHERE `entry`=-1631078;
UPDATE `script_texts` SET `content_loc8`='Повеселимся?' WHERE `entry`=-1631079;
UPDATE `script_texts` SET `content_loc8`='Что-то мне нехорошо...' WHERE `entry`=-1631083;
UPDATE `script_texts` SET `content_loc8`='Веселью конец!' WHERE `entry`=-1631088;
UPDATE `script_texts` SET `content_loc8`='Па-па...' WHERE `entry`=-1631089;
UPDATE `script_texts` SET `content_loc8`='Мер-твец, мер-твец, мер-твец!' WHERE `entry`=-1631087;
UPDATE `script_texts` SET `content_loc8`='Папочка! У меня получилось!' WHERE `entry`=-1631086;
UPDATE `script_texts` SET `content_loc8`='Это обычное облаго газа, только будьте осторожны, не такое уж оно и обычное!' WHERE `entry`=-1631080;


-- Профессор Мерзоцоид

UPDATE `script_texts` SET `content_loc8`='Отличные новости, народ! Я усовершенствовал штамм чумы, которая уничтожит весь Азерот!' WHERE `entry`=-1631104;
UPDATE `script_texts` SET `content_loc8`='Хм, что-то я ничего не чувствую. Что?! Это еще откуда?' WHERE `entry`=-1631107;
UPDATE `script_texts` SET `content_loc8`='На вкус, как вишенка! ОЙ! Извините!' WHERE `entry`=-1631108;
UPDATE `script_texts` SET `content_loc8`='Плохие новости, народ… Похоже, у меня ничего не выйдет…' WHERE `entry`=-1631114;


-- Кровавый Совет

UPDATE `script_texts` SET `content_loc8`='Глупые смертные. Думали, что одолели нас? Сан''лейн - непобедимые воины Короля-лича. Теперь наши силы едины!' WHERE `entry`=-1631115;
UPDATE `script_texts` SET `content_loc8`='Восстаньте, братья, и уничтожьте наших врагов!' WHERE `entry`=-1631116;
UPDATE `script_texts` SET `content_loc8`='Наксанар был досадным недоразумением! Сила Сферы позволит Валанару свершить отмщение!' WHERE `entry`=-1631132;
UPDATE `script_texts` SET `content_loc8`='И прольется кровь!' WHERE `entry`=-1631119;
UPDATE `script_texts` SET `content_loc8`='Пища для червей.' WHERE `entry`=-1631128;


-- Валитрия

UPDATE `script_texts` SET `content_loc8`='Герои, вы должны мне помочь! Мои силы... на исходе. Залечите мои раны...' WHERE `entry`=-1666063;
UPDATE `script_texts` SET `content_loc8`='Я открыла портал в Изумрудный Сон. Там вы найдете спасение, герои.' WHERE `entry`=-1666064;
UPDATE `script_texts` SET `content_loc8`='Силы возвращаются ко мне. Герои, ещё немного!' WHERE `entry`=-1666065;
UPDATE `script_texts` SET `content_loc8`='Я долго не продержусь!' WHERE `entry`=-1666066;
UPDATE `script_texts` SET `content_loc8`='Прискорбная потеря...' WHERE `entry`=-1666068;
UPDATE `script_texts` SET `content_loc8`='Простите меня! Я... не могу... оста... ВСЁ ВО ВЛАСТИ КОШМАРА!' WHERE `entry`=-1666067;
UPDATE `script_texts` SET `content_loc8`='НЕУДАЧНИКИ!' WHERE `entry`=-16660690;
UPDATE `script_texts` SET `content_loc8`='Я ИЗЛЕЧИЛАСЬ! Изера, даруй мне силу покончить с этими нечистивыми тварями.' WHERE `entry`=-1666070;


-- Синдрагоса

UPDATE `script_texts` SET `content_loc8`='Глупцы, зачем вы сюда явились! Ледяные ветра Нордскола унесут ваши души!' WHERE `entry`=-1666071;
UPDATE `script_texts` SET `content_loc8`='Трепещите, смертные, ибо ваша жалкая магия теперь бессильна!' WHERE `entry`=-1666072;
UPDATE `script_texts` SET `content_loc8`='Вы чувствуете?' WHERE `entry`=-1666073;
UPDATE `script_texts` SET `content_loc8`='Ааах! Жжется! Что это за магия?!' WHERE `entry`=-1666074;
UPDATE `script_texts` SET `content_loc8`='Сейчас вы почуствуете всю мощь нашего господина!' WHERE `entry`=-1666076;
UPDATE `script_texts` SET `content_loc8`='Погибни!' WHERE `entry`=-1666077;
UPDATE `script_texts` SET `content_loc8`='Удел смертных...' WHERE `entry`=-1666078;
UPDATE `script_texts` SET `content_loc8`='Довольно! Я устала от этих игр!' WHERE `entry`=-1666079;
UPDATE `script_texts` SET `content_loc8`='Наконец-то...свободна...' WHERE `entry`=-1666080;


-- КОРОЛЬ-ЛИЧ
UPDATE `script_texts` SET `content_loc8`='Неужели прибыли наконец хваленые силы Света? Мне бросить Ледяную Скорбь и сдаться на твою милось, Фордринг?' WHERE `entry`=-1810001;
UPDATE `script_texts` SET `content_loc8`='Мы даруем тебе быструю смерть, Артас. Более быструю, чем ты заслуживаешь за то, что замучал и погубил десятки тысяч душ.' WHERE `entry`=-1810002;
UPDATE `script_texts` SET `content_loc8`='Ты пройдешь через эти мучения сам. И будешь МОЛИТЬ о пощаде, но я не буду слушать. Твои отчаянные крики послужат доказательством моей безграничной мощи.' WHERE `entry`=-1810003;
UPDATE `script_texts` SET `content_loc8`='Да будет так. Герои, в атаку!' WHERE `entry`=-1810004;
UPDATE `script_texts` SET `content_loc8`='Я оставлю тебя в живых, чтобы ты увидел финал. Не могу допустить, чтобы величайший служитель Света пропустил рождение МОЕГО МИРА.' WHERE `entry`=-1810005;
UPDATE `script_texts` SET `content_loc8`='Я проморожу вас насквозь - и вы разлетитесь на ледяные осколки.' WHERE `entry`=-1810007;
UPDATE `script_texts` SET `content_loc8`='КОНЕЦ СВЕТА!' WHERE `entry`=-1810008;
UPDATE `script_texts` SET `content_loc8`='Склонись перед своим господином и повелителем!.' WHERE `entry`=-1810009;
UPDATE `script_texts` SET `content_loc8`='Надежда тает!' WHERE `entry`=-1810010;
UPDATE `script_texts` SET `content_loc8`='Пришел КОНЕЦ!' WHERE `entry`=-1810011;
UPDATE `script_texts` SET `content_loc8`='Встречайте трагический финал!' WHERE `entry`=-1810012;
UPDATE `script_texts` SET `content_loc8`='Сомнений нет, вы величайшие герои Азерота! Вы преодолели все препятствия, которые я воздвиг перед вами. Сильнейшие из моих слуг пали под вашим натиском, сгорели в пламени вашей ярости!' WHERE `entry`=-1810013;
UPDATE `script_texts` SET `content_loc8`='Ты отлично их обучил, Фордринг! ' WHERE `entry`=-1810014;
UPDATE `script_texts` SET `content_loc8`='Смотри, как я буду воскрешать их, и превращать в воинов Плети! Они повергнут этот мир в пучину хаоса. Азерот падет от их рук! И ты станешь первой жертвой.' WHERE `entry`=-1810015;
UPDATE `script_texts` SET `content_loc8`='Мне по душе эта ирония.' WHERE `entry`=-1810016;
UPDATE `script_texts` SET `content_loc8`='СВЕТ, ДАРУЙ МНЕ ПОСЛЕДНЕЕ БЛАГОСЛОВЛЕНИЕ... ДАЙ МНЕ СИЛЫ РАЗБИТЬ ЭТИ ОКОВЫ!' WHERE `entry`=-1810017;
UPDATE `script_texts` SET `content_loc8`='Невозможно...' WHERE `entry`=-1810018;
UPDATE `script_texts` SET `content_loc8`='Аах! Вы меня и правда ранили. Я слишком долго с вами играл! Испытайте на себе Возмездие СМЕРТИ!' WHERE `entry`=-1666080;
UPDATE `script_texts` SET `content_loc8`='И вот я стою как лев пред агнцами... И не дрожат они.' WHERE `entry`=-1810024;
UPDATE `script_texts` SET `content_loc8`='Им неведом страх.' WHERE `entry`=-1810025;
UPDATE `script_texts` SET `content_loc8`='Ну же герои, в вашей ярости моя сила!' WHERE `entry`=-1810006;
UPDATE `script_texts` SET `content_loc8`='Ледяная Скорбь жаждет крови!' WHERE `entry`=-1810027;
UPDATE `script_texts` SET `content_loc8`='Ледяная Скорбь поглотит душу вашего товарища.' WHERE `entry`=-1810028;
UPDATE `script_texts` SET `content_loc8`='Смотрите как мир рушится вокруг вас!' WHERE `entry`=-1810030;
UPDATE `script_texts` SET `content_loc8`='Валь''кира, твой господин зовет!' WHERE `entry`=-1810029;
UPDATE `script_texts` SET `content_loc8`='Хватит, Артас! Твоя ненависть не заберет больше ни одной жизни!' WHERE `entry`=-1810020;
UPDATE `script_texts` SET `content_loc8`='Вы пришли, чтобы вершить суд над Артасом? Чтобы уничтожить Короля-лича?' WHERE `entry`=-1666080;
UPDATE `script_texts` SET `content_loc8`='Вы не должны оказаться во власти Ледяной Скорби. Иначе, как и я, будете навек порабощены этим проклятым клинком!' WHERE `entry`=-1666080;
UPDATE `script_texts` SET `content_loc8`='Помогите мне уничтожить эти истерзанные души! Вместе мы вытянем силу из Ледяной Скорби и ослабим Короля-лича!' WHERE `entry`=-1666080;
UPDATE `script_texts` SET `content_loc8`='Наконец я свободен! Всё кончено, сын мой... Настал час расплаты!' WHERE `entry`=-1810021;
UPDATE `script_texts` SET `content_loc8`='Поднимитесь, Воины Света!' WHERE `entry`=-1810023;
UPDATE `script_texts` SET `content_loc8`='Король-лич падет!' WHERE `entry`=-1810022;

-- Halls of Reflection
DELETE FROM `script_texts` WHERE `entry` BETWEEN -1594540 AND -1594430;
INSERT INTO `script_texts` (`entry`,`content_default`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
(-1594473, '<need translate>', 'Глупая девчонка! Тот кого ты ищещь давно убит! Теперь он лишь призрак, слабый отзвук в моем сознании!', 17229,1,0,0, '67234'),
(-1594474, '<need translate>', 'Я не повторю прежней ошибки, Сильвана. На этот раз тебе не спастись. Ты не оправдала моего доверия и теперь тебя ждет только забвение!', 17228,1,0,0, '67234'),
-- SCENE - Hall Of Reflection (Extro) - PreEscape
(-1594477, 'Your allies have arrived, Jaina, just as you promised. You will all become powerful agents of the Scourge.', 'Твои союзники прибыли, Джайна! Как ты и обещала... Ха-ха-ха-ха... Все вы станете могучими солдатами Плети...', 17212,1,0,0, '67234'),
(-1594478, 'I will not make the same mistake again, Sylvanas. This time there will be no escape. You will all serve me in death!', 'Я не повторю прежней ошибки, Сильвана! На этот раз тебе не спастись. Вы все будите служить мне после смерти...', 17213,1,0,0, '67234'),
(-1594479, 'He is too powerful, we must leave this place at once! My magic will hold him in place for only a short time! Come quickly, heroes!', 'Он слишком силен. Мы должны выбраться от сюда как можно скорее. Моя магия задержит его ненадолго, быстрее герои...', 16644,0,0,1, '67234'),
(-1594480, 'He\'s too powerful! Heroes, quickly, come to me! We must leave this place immediately! I will do what I can do hold him in place while we flee.', 'Он слишком силен. Герои скорее, за мной. Мы должны выбраться отсюда немедленно. Я постараюсь его задержать, пока мы будем убегать.', 17058,0,0,1, '67234'),
-- SCENE - Hall Of Reflection (Extro) - Escape
(-1594481, 'Death\'s cold embrace awaits.', 'Смерть распростерла ледяные обьятия!', 17221,1,0,0, '67234'),
(-1594482, 'Rise minions, do not left them us!', 'Восстаньте прислужники, не дайте им сбежать!', 17216,1,0,0, '67234'),
(-1594483, 'Minions sees them. Bring their corpses back to me!', 'Схватите их! Принесите мне их тела!', 17222,1,0,0, '67234'),
(-1594484, 'No...', 'Надежды нет!', 17214,1,0,0, '67234'),
(-1594485, 'All is lost!', 'Смирись с судьбой.', 17215,1,0,0, '67234'),
(-1594486, 'There is no escape!', 'Бежать некуда!', 17217,1,0,0, '67234'),
(-1594487, 'I will destroy this barrier. You must hold the undead back!', 'Я разрушу эту преграду, а вы удерживайте нежить на расстоянии!', 16607,1,0,0, '67234'),
(-1594488, 'No wall can hold the Banshee Queen! Keep the undead at bay, heroes! I will tear this barrier down!', 'Никакие стены не удержат Королеву Баньши. Держите нежить на расстоянии, я сокрушу эту преграду.', 17029,1,0,0, '67234'),
(-1594489, 'Another ice wall! Keep the undead from interrupting my incantation so that I may bring this wall down!', 'Опять ледяная стена... Я разобью ее, только не дайте нежити прервать мои заклинания...', 16608,1,0,0, '67234'),
(-1594490, 'Another barrier? Stand strong, champions! I will bring the wall down!', 'Еще одна преграда. Держитесь герои! Я разрушу эту стену!', 17030,1,0,0, '67234'),
(-1594491, 'Succumb to the chill of the grave.', 'Покоритесь Леденящей смерти!', 17218,1,0,0, '67234'),
(-1594492, 'Another dead end.', 'Вы в ловушке!', 17219,1,0,0, '67234'),
(-1594493, 'How long can you fight it?', 'Как долго вы сможете сопротивляться?', 17220,1,0,0, '67234'),
(-1594494, '<need translate>', 'Он с нами играет. Я  покажу ему что бывает когда лед встречается со огнем!', 16609,0,0,0, '67234'),
(-1594495, 'Your barriers can\'t hold us back much longer, monster. I will shatter them all!', 'Твои преграды больше не задержат нас, чудовище. Я смету их с пути!', 16610,1,0,0, '67234'),
(-1594496, 'I grow tired of these games, Arthas! Your walls can\'t stop me!', 'Я устала от этих игр Артас. Твои стены не остановят меня!', 17031,1,0,0, '67234'),
(-1594497, 'You won\'t impede our escape, fiend. Keep the undead off me while I bring this barrier down!', 'Ты не помешаешь нам уйти, монстр. Сдерживайте нежить, а я уничтожу эту преграду.', 17032,1,0,0, '67234'),
(-1594498, 'There\'s an opening up ahead. GO NOW!', 'Я вижу выход, скорее!', 16645,1,0,0, '67234'),
(-1594499, 'We\'re almost there... Don\'t give up!', 'Мы почти выбрались, не сдавайтесь!', 16646,1,0,0, '67234'),
(-1594500, 'There\'s an opening up ahead. GO NOW!', 'Я вижу выход, скорее!', 17059,1,0,0, '67234'),
(-1594501, 'We\'re almost there! Don\'t give up!', 'Мы почти выбрались, не сдавайтесь!', 17060,1,0,0, '67234'),
(-1594502, 'It... It\'s a dead end. We have no choice but to fight. Steel yourself heroes, for this is our last stand!', 'Больше некуда бежать. Теперь нам придется сражаться. Это наша последняя битва!', 16647,1,0,0, '67234'),
(-1594503, 'BLASTED DEAD END! So this is how it ends. Prepare yourselves, heroes, for today we make our final stand!', 'Проклятый тупик, значит все закончится здесь. Готовьтесь герои, это наша последняя битва.', 17061,1,0,0, '67234'),
(-1594504, 'Nowhere to run! You\'re mine now...', 'Ха-ха-ха... Бежать некуда. Теперь вы мои!', 17223,1,0,0, '67234'),
(-1594505, 'Soldiers of Lordaeron, rise to meet your master\'s call!', 'Солдаты Лордерона, восстаньте по зову Господина!', 16714,1,0,0, '67234'),
(-1594506, 'The master surveyed his kingdom and found it... lacking. His judgement was swift and without mercy. Death to all!', 'Господин осмотрел свое королевство и признал его негодным! Его суд был быстрым и суровым - предать всех смерти!', 16738,1,0,0, '67234'),
-- FrostWorn General
(-1594519, 'You are not worthy to face the Lich King!', 'Вы недостойны предстать перед Королем - Личом!', 16921,1,0,0, '67234'),
(-1594520, 'Master, I have failed...', 'Господин... Я подвел вас...', 16922,1,0,0, '67234');