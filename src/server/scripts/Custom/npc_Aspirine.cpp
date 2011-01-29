 /*
 * @Authors : Intel, Bondiano updates
### TYPE ###
1 - cities
2 - eastern kingdoms
3 - kalimdor
4 - outlands
5 - northrend
6 - vanilla instance
7 - bc instance
8 - wotlk instance
9 - vanilla raid
10 - bc raid
11 - wotlk raid
12 - arena
### FACTION ###
1 - Alliance
2 - Horde
*/
#include "ScriptPCH.h"
#include <cstring>

enum eEnums
{
    SPELL_BUFF_1           = 58054, // Blessing of kings
    SPELL_BUFF_2           = 42995, // Arcane Intellect
    SPELL_BUFF_3           = 48161, // Power Word: Fortitude
    SPELL_BUFF_4           = 48469, // Mark of the Wild
    SPELL_BUFF_5           = 48102, // Scroll of Stamina
    SPELL_BUFF_6_0         = 48100, // Scroll of Intellect
    SPELL_BUFF_6_1         = 58449, // Scroll of Strenght
    SPELL_BUFF_6_2         = 58451, // Scroll of Agility
    
    SPELL_RESURRECTION_SICKNESS = 15007,
    SPELL_VISUAL_TELEPORT  = 35517,

    SOUND_1                = 5934, // Gnome greetings
    SOUND_2                = 5935, // Gnome goodbye
    SOUND_3                = 5936, // Gnome pissed
};

#define GOSSIP_NO_DEST "Упс, что-то пошло не так..."
#define GOSSIP_PREV "<- [Предыдущая страница]"
#define GOSSIP_NEXT "-> [Следующая страница]"
#define GOSSIP_MMENU "<= [Главное меню]"

// Main Menu
#define GOSSIP_MAIN_1 "Телепортируй меня!"
#define GOSSIP_MAIN_2 "Наложи на меня бафф!"
#define GOSSIP_MAIN_3 "Сними с меня маску смерти"
#define GOSSIP_MAIN_4 "Дай Exchange"

// Teleport Type Menu
#define GOSSIP_TELE_1 "Замки"
#define GOSSIP_TELE_2 "Зоны"
#define GOSSIP_TELE_3 "Подземелья"
#define GOSSIP_TELE_4 "Рейды"
#define GOSSIP_TELE_5 "Арены"

// Zone Type Menu
#define GOSSIP_MAP_0 "Восточные королевства"
#define GOSSIP_MAP_1 "Калимдор"
#define GOSSIP_MAP_2 "Запределье"
#define GOSSIP_MAP_3 "Нордскол"

// Instance/Raid Type Menu
#define GOSSIP_DUNGEON_0 "Классические подземелья"
#define GOSSIP_DUNGEON_1 "БК инсты"
#define GOSSIP_DUNGEON_2 "WoLtK инсты"

class npc_ihelp : public CreatureScript
{
    public:

        npc_ihelp() : CreatureScript("npc_aspirine") {}

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if(player->isInCombat()) // Check in combat
            {
                player->CLOSE_GOSSIP_MENU();
                creature->MonsterWhisper("Ты в бою,приходи позже", player->GetGUID());
                return true;
            }
            
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_MAIN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GOSSIP_MAIN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GOSSIP_MAIN_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, GOSSIP_MAIN_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);

            player->PlayerTalkClass->SendGossipMenu(907, creature->GetGUID());

            creature->PlayDirectSound(SOUND_1, player);

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch(action)
            {
                case GOSSIP_ACTION_INFO_DEF+1:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_MAIN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GOSSIP_MAIN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GOSSIP_MAIN_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, GOSSIP_MAIN_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
                    player->PlayerTalkClass->SendGossipMenu(907, creature->GetGUID());
                    break;
                    
                case GOSSIP_ACTION_INFO_DEF+2:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_TELE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+110);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_TELE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+62);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_TELE_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+63);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_TELE_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+64);
                    //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_TELE_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+50);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_MMENU, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                    player->SEND_GOSSIP_MENU(907, creature->GetGUID());
                    break;

                case GOSSIP_ACTION_INFO_DEF+3:
                    player->CLOSE_GOSSIP_MENU();
                    creature->CastSpell(player, SPELL_BUFF_1, true);
                    creature->CastSpell(player, SPELL_BUFF_2, true);
                    creature->CastSpell(player, SPELL_BUFF_3, true);
                    creature->CastSpell(player, SPELL_BUFF_4, true);
                    creature->CastSpell(player, SPELL_BUFF_5, true);
                    switch (player->getClass())
                    {
                        case 2:
                        case 5:
                        case 7:
                        case 8:
                        case 9:
                        case 11:
                            creature->CastSpell(player, SPELL_BUFF_6_0, true);
                            break;
                         
                        case 4:
                            creature->CastSpell(player, SPELL_BUFF_6_2, true);
                            break;
                            
                        default:
                            creature->CastSpell(player, SPELL_BUFF_6_1, true);
                            break;
                    }
                    creature->PlayDirectSound(SOUND_2, player);
                    break;

                case GOSSIP_ACTION_INFO_DEF+4:
                    player->CLOSE_GOSSIP_MENU();
                    if(player->HasAura(SPELL_RESURRECTION_SICKNESS,0))
                    {
                        creature->CastSpell(player,38588,false); // Healing effect
                        player->RemoveAurasDueToSpell(SPELL_RESURRECTION_SICKNESS);
                    }
                    creature->PlayDirectSound(SOUND_2, player);
                    break;
                    
                case GOSSIP_ACTION_INFO_DEF+5:
                    player->CLOSE_GOSSIP_MENU();
                    creature->PlayDirectSound(SOUND_2, player);
                    break;
                    
                case GOSSIP_ACTION_INFO_DEF+6:  // No tele locations
                    player->CLOSE_GOSSIP_MENU();
                    creature->PlayDirectSound(SOUND_2, player);
                    break;
                    
                case GOSSIP_ACTION_INFO_DEF+62:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_MAP_0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+120);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_MAP_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+130);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_MAP_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+140);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_MAP_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+150);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_MMENU, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                    player->SEND_GOSSIP_MENU(907, creature->GetGUID());
                    break;
                    
                case GOSSIP_ACTION_INFO_DEF+63:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_DUNGEON_0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+160);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_DUNGEON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+170);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_DUNGEON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+180);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_MMENU, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                    player->SEND_GOSSIP_MENU(907, creature->GetGUID());
                    break;
                    
                case GOSSIP_ACTION_INFO_DEF+64:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_DUNGEON_0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+190);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_DUNGEON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+200);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_DUNGEON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+210);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_MMENU, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                    player->SEND_GOSSIP_MENU(907, creature->GetGUID());
                    break;
                    
                default:
                    if ((action >= GOSSIP_ACTION_INFO_DEF+100) && (action <= GOSSIP_ACTION_INFO_DEF+300))
                    {
                        ShowDest(player, creature, action);
                    }
                    else
                    {
                        SelectDest(player, creature, action);
                    }
                    break;
            }

            return true;
        }
        
        void ShowDest(Player* player, Creature* creature, uint32 action)
        { 
            uint32 i;
            uint32 faction = 0;
        
            uint32 type = floor(action / 10) - 110;
            uint32 offset = (action - ((floor(action / 10)) * 10)) * 7;
            
            switch(player->getFaction()) // Check player's faction
            {
                case 2:
                case 5:
                case 6:
                case 8:
                case 10:
                    faction = 1; // Set it reversed, because where are checking if it is not equal
                    break;
                    
                default:
                    faction = 2;
                    break;
            }
            // tarpas
            if (player->isGameMaster()) // Show all locatoin for game master
                faction = 3; // Since we don't have faction 3 it returns everything
            
            QueryResult result = WorldDatabase.PQuery("SELECT * FROM custom_npc_tele_destination WHERE type = %u AND level <= %u AND faction != %u LIMIT %u OFFSET %u", type, player->getLevel(), faction, 7, offset);
            
            if (action - (floor(action / 10)) * 10 != 0) // Add Previous Page Button
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, GOSSIP_PREV, GOSSIP_SENDER_MAIN, action - 1);
            
            if (result)
            {
                do
                {
                    Field *fields = result->Fetch();
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, fields[1].GetString(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+300+fields[0].GetUInt32());
                    i++;
                } while (result->NextRow());
            } else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, GOSSIP_NO_DEST, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+6);
            
            result = WorldDatabase.PQuery("SELECT id FROM custom_npc_tele_destination WHERE type = %u AND level <= %u AND faction != %u LIMIT %u OFFSET %u", type, player->getLevel(), faction, 7, offset+7); // Same query but with offset +7 because we are checking next page results
            
            if ((action - (floor(action / 10)) * 10 != 9) && result)  // Add Next Page Button
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_NEXT, GOSSIP_SENDER_MAIN, action + 1);
                
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_MMENU, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1); // Main menu Button
            
            player->SEND_GOSSIP_MENU(907, creature->GetGUID());
        }
        
        void SelectDest(Player* player, Creature* creature, uint32 action)
        {
            action -= 1300;
            QueryResult result = WorldDatabase.PQuery("SELECT * FROM custom_npc_tele_destination WHERE id = %u LIMIT 1", action);
            player->CLOSE_GOSSIP_MENU();
            
            if (result)
            {
                do
                {
                    Field *fields = result->Fetch();
                    uint16 tele_map = fields[6].GetUInt32();
                    float tele_x = fields[3].GetFloat();
                    float tele_y = fields[4].GetFloat();
                    float tele_z = fields[5].GetFloat();
                    float tele_o = fields[7].GetFloat();
                    Teleport(player, tele_map, tele_x, tele_y, tele_z, tele_o);
                } while (result->NextRow());
            }
        }
        
        void Teleport(Player * const player, const uint16 &map, const float &X, const float &Y, const float &Z, const float &orient)
        {
            player->CastSpell(player, SPELL_VISUAL_TELEPORT, true);
            player->TeleportTo(map, X, Y, Z, orient);
        }
};

void AddSC_npc_aspirine()
{
    new npc_aspirine();
}