/*
* Copyright (C) 2010 Easy for TrinityCore <http://trinity-core.ru/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ScriptPCH.h"

#define MSG_GOSSIP_TEXT_MOUNT           "Прокат ездовых животных"
#define MSG_HI_LEVEL                    "У вас слишком большой уровень..."
#define MSG_MOUTED                      "Вы уже на средстве передвижения."
#define MOUNT_SPELL_MOUNT               64657


class npc_mount : public CreatureScript
{
    public:
    npc_mount() : CreatureScript("npc_mount") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MSG_GOSSIP_TEXT_MOUNT, GOSSIP_SENDER_MAIN, 1);
        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
	    if (uiSender != GOSSIP_SENDER_MAIN)
            return false;

        pPlayer->PlayerTalkClass->ClearMenus();

	    switch (uiAction)
	    {
		    case 1:
                if (pPlayer->getLevel()>80)
                {
                    pCreature->MonsterWhisper(MSG_HI_LEVEL, pPlayer->GetGUID());
                    pPlayer->CLOSE_GOSSIP_MENU();
			        break;
                }
                if (pPlayer->IsMounted())
                {
                    pCreature->MonsterWhisper(MSG_MOUTED, pPlayer->GetGUID());
                    pPlayer->CLOSE_GOSSIP_MENU();
			        break;
                }
                pPlayer->AddAura(MOUNT_SPELL_MOUNT, pPlayer);
			    pPlayer->CLOSE_GOSSIP_MENU();
			    break;
		    default:
			    pPlayer->CLOSE_GOSSIP_MENU();
	    }
        return true;
    }
};

void AddSC_npc_mount()
{
    new npc_mount;
}