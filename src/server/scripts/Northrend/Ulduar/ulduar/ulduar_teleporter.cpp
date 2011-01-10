/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "ulduar.h"

/*
The teleporter appears to be active and stable.

- Expedition Base Camp
- Formation Grounds
- Colossal Forge
- Scrapyard
- Antechamber of Ulduar
- Shattered Walkway
- Conservatory of Life
*/

#define BASE_CAMP    200
#define GROUNDS      201
#define FORGE        202
#define SCRAPYARD    203
#define ANTECHAMBER  204
#define WALKWAY      205
#define CONSERVATORY 206
#define MIMIRON      207

class ulduar_teleporter : public GameObjectScript
{
public:
    ulduar_teleporter() : GameObjectScript("ulduar_teleporter") { }

    bool OnGossipSelect(Player *pPlayer, GameObject * /*pGO*/, uint32 sender, uint32 action)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (sender != GOSSIP_SENDER_MAIN) return true;
        if (!pPlayer->getAttackers().empty()) return true;

        switch(action)
        {
        case BASE_CAMP:
            pPlayer->TeleportTo(603, -706.122f, -92.6024f, 429.876f, 0.0f);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case GROUNDS:
            pPlayer->TeleportTo(603, 131.248f, -35.3802f, 409.804f, 0.0f);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case FORGE:
            pPlayer->TeleportTo(603, 553.233f, -12.3247f, 409.679f, 0.0f);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case SCRAPYARD:
            pPlayer->TeleportTo(603, 926.292f, -11.4635f, 418.595f, 0.0f);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case ANTECHAMBER:
            pPlayer->TeleportTo(603, 1498.09f, -24.246f, 420.967f, 0.0f);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case WALKWAY:
            pPlayer->TeleportTo(603, 1859.45f, -24.1f, 448.9f, 0.0f);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case CONSERVATORY:
            pPlayer->TeleportTo(603, 2086.27f, -24.3134f, 421.239f, 0.0f);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
		case MIMIRON:
            pPlayer->TeleportTo(603, 2517.0f, 2569.0f, 412.239f, 0.0f);
            pPlayer->CLOSE_GOSSIP_MENU();
			break;
        }

        return true;
    }

    bool OnGossipHello(Player *pPlayer, GameObject *pGO)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Expedition Base Camp", GOSSIP_SENDER_MAIN, BASE_CAMP);
        if (InstanceScript* pInstance = pGO->GetInstanceScript())
        {
            if (pInstance->GetData(TYPE_COLOSSUS) == 2) //count of 2 collossus death
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Formation Grounds", GOSSIP_SENDER_MAIN, GROUNDS);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Colossal Forge", GOSSIP_SENDER_MAIN, FORGE);
            if (pInstance->GetBossState(TYPE_XT002) == DONE)
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Scrapyard", GOSSIP_SENDER_MAIN, SCRAPYARD);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Antechamber of Ulduar", GOSSIP_SENDER_MAIN, ANTECHAMBER);
            }
            if (pInstance->GetBossState(TYPE_KOLOGARN) == DONE)
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Shattered Walkway", GOSSIP_SENDER_MAIN, WALKWAY);
            if (pInstance->GetBossState(TYPE_AURIAYA) == DONE)
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Conservatory of Life", GOSSIP_SENDER_MAIN, CONSERVATORY);
			if (pInstance->GetBossState(TYPE_AURIAYA) == DONE)
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to the Mimiron", GOSSIP_SENDER_MAIN, MIMIRON);
        }
        pPlayer->SEND_GOSSIP_MENU(pGO->GetGOInfo()->GetGossipMenuId(), pGO->GetGUID());
        return true;
    }

};

class go_call_tram : public CreatureScript
{
public:
    go_call_tram() : CreatureScript("go_call_tram") { }

    // Mimiron Tram (need update packet support)
    bool OnGossipHello(Player* pPlayer, GameObject* pGo)
    {
        InstanceScript* pInstance = pGo->GetInstanceScript();

        if (!pInstance)
            return false;

        switch(pGo->GetEntry())
        {
            case 194914:
            //case 194438:
                pInstance->SetData(TYPE_MIMIRON_TRAM, 0);
                break;
            case 194912:
            case 194437:
                pInstance->SetData(TYPE_MIMIRON_TRAM, 1);
                break;
        }

        return true;
    }
};

void AddSC_ulduar_teleporter()
{
    new ulduar_teleporter;
    new go_call_tram;
}
