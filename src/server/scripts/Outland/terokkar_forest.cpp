/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

/* ScriptData
SDName: Terokkar_Forest
SD%Complete: 85
SDComment: Quest support: 9889, 10009, 10873, 10896, 10898, 11096, 10052, 10051. Skettis->Ogri'la Flight
SDCategory: Terokkar Forest
EndScriptData */

/* ContentData
mob_unkor_the_ruthless
mob_infested_root_walker
mob_rotting_forest_rager
mob_netherweb_victim
npc_floon
npc_isla_starmane
npc_slim
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "Group.h"

/*######
## mob_unkor_the_ruthless
######*/

#define SAY_SUBMIT                      -1000194

#define FACTION_HOSTILE                 45
#define FACTION_FRIENDLY                35
#define QUEST_DONTKILLTHEFATONE         9889

#define SPELL_PULVERIZE                 2676
//#define SPELL_QUID9889                32174

class mob_unkor_the_ruthless : public CreatureScript
{
public:
    mob_unkor_the_ruthless() : CreatureScript("mob_unkor_the_ruthless") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_unkor_the_ruthlessAI (creature);
    }

    struct mob_unkor_the_ruthlessAI : public ScriptedAI
    {
        mob_unkor_the_ruthlessAI(Creature* creature) : ScriptedAI(creature) {}

        bool CanDoQuest;
        uint32 UnkorUnfriendly_Timer;
        uint32 Pulverize_Timer;

        void Reset()
        {
            CanDoQuest = false;
            UnkorUnfriendly_Timer = 0;
            Pulverize_Timer = 3000;
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->setFaction(FACTION_HOSTILE);
        }

        void EnterCombat(Unit* /*who*/) {}

        void DoNice()
        {
            DoScriptText(SAY_SUBMIT, me);
            me->setFaction(FACTION_FRIENDLY);
            me->SetStandState(UNIT_STAND_STATE_SIT);
            me->RemoveAllAuras();
            me->DeleteThreatList();
            me->CombatStop(true);
            UnkorUnfriendly_Timer = 60000;
        }

        void DamageTaken(Unit* done_by, uint32 &damage)
        {
            if (done_by->GetTypeId() == TYPEID_PLAYER)
                if (me->HealthBelowPctDamaged(30, damage))
            {
                if (Group* group = CAST_PLR(done_by)->GetGroup())
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* groupie = itr->getSource();
                        if (groupie &&
                            groupie->GetQuestStatus(QUEST_DONTKILLTHEFATONE) == QUEST_STATUS_INCOMPLETE &&
                            groupie->GetReqKillOrCastCurrentCount(QUEST_DONTKILLTHEFATONE, 18260) == 10)
                        {
                            groupie->AreaExploredOrEventHappens(QUEST_DONTKILLTHEFATONE);
                            if (!CanDoQuest)
                                CanDoQuest = true;
                        }
                    }
                } else
                if (CAST_PLR(done_by)->GetQuestStatus(QUEST_DONTKILLTHEFATONE) == QUEST_STATUS_INCOMPLETE &&
                    CAST_PLR(done_by)->GetReqKillOrCastCurrentCount(QUEST_DONTKILLTHEFATONE, 18260) == 10)
                {
                    CAST_PLR(done_by)->AreaExploredOrEventHappens(QUEST_DONTKILLTHEFATONE);
                    CanDoQuest = true;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (CanDoQuest)
            {
                if (!UnkorUnfriendly_Timer)
                {
                    //DoCast(me, SPELL_QUID9889);        //not using spell for now
                    DoNice();
                }
                else
                {
                    if (UnkorUnfriendly_Timer <= diff)
                    {
                        EnterEvadeMode();
                        return;
                    } else UnkorUnfriendly_Timer -= diff;
                }
            }

            if (!UpdateVictim())
                return;

            if (Pulverize_Timer <= diff)
            {
                DoCast(me, SPELL_PULVERIZE);
                Pulverize_Timer = 9000;
            } else Pulverize_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## mob_infested_root_walker
######*/

class mob_infested_root_walker : public CreatureScript
{
public:
    mob_infested_root_walker() : CreatureScript("mob_infested_root_walker") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_infested_root_walkerAI (creature);
    }

    struct mob_infested_root_walkerAI : public ScriptedAI
    {
        mob_infested_root_walkerAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() { }
        void EnterCombat(Unit* /*who*/) { }

        void DamageTaken(Unit* done_by, uint32 &damage)
        {
            if (done_by && done_by->GetTypeId() == TYPEID_PLAYER)
                if (me->GetHealth() <= damage)
                    if (rand()%100 < 75)
                        //Summon Wood Mites
                        DoCast(me, 39130, true);
        }
    };
};

/*######
## mob_skywing
######*/
class npc_skywing : public CreatureScript
{
public:
    npc_skywing() : CreatureScript("npc_skywing") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_skywingAI(creature);
    }

    struct npc_skywingAI : public npc_escortAI
    {
    public:
        npc_skywingAI(Creature* creature) : npc_escortAI(creature) {}

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (i)
            {
                case 8:
                    player->AreaExploredOrEventHappens(10898);
                    break;
            }
        }

        void EnterCombat(Unit* /*who*/) {}

        void MoveInLineOfSight(Unit* who)
        {
            if (HasEscortState(STATE_ESCORT_ESCORTING))
                return;

            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (CAST_PLR(who)->GetQuestStatus(10898) == QUEST_STATUS_INCOMPLETE)
                {
                    float Radius = 10.0f;
                    if (me->IsWithinDistInMap(who, Radius))
                    {
                        Start(false, false, who->GetGUID());
                    }
                }
            }
        }

        void Reset() {}

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);
        }
    };
};

/*######
## mob_rotting_forest_rager
######*/

class mob_rotting_forest_rager : public CreatureScript
{
public:
    mob_rotting_forest_rager() : CreatureScript("mob_rotting_forest_rager") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_rotting_forest_ragerAI (creature);
    }

    struct mob_rotting_forest_ragerAI : public ScriptedAI
    {
        mob_rotting_forest_ragerAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() { }
        void EnterCombat(Unit* /*who*/) { }

        void DamageTaken(Unit* done_by, uint32 &damage)
        {
            if (done_by->GetTypeId() == TYPEID_PLAYER)
                if (me->GetHealth() <= damage)
                    if (rand()%100 < 75)
                        //Summon Lots of Wood Mights
                        DoCast(me, 39134, true);
        }
    };
};

/*######
## mob_netherweb_victim
######*/

#define QUEST_TARGET        22459
//#define SPELL_FREE_WEBBED   38950

const uint32 netherwebVictims[6] =
{
    18470, 16805, 21242, 18452, 22482, 21285
};
class mob_netherweb_victim : public CreatureScript
{
public:
    mob_netherweb_victim() : CreatureScript("mob_netherweb_victim") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_netherweb_victimAI (creature);
    }

    struct mob_netherweb_victimAI : public ScriptedAI
    {
        mob_netherweb_victimAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() { }
        void EnterCombat(Unit* /*who*/) { }
        void MoveInLineOfSight(Unit* /*who*/) { }

        void JustDied(Unit* Killer)
        {
            if (Killer->GetTypeId() == TYPEID_PLAYER)
            {
                if (CAST_PLR(Killer)->GetQuestStatus(10873) == QUEST_STATUS_INCOMPLETE)
                {
                    if (rand()%100 < 25)
                    {
                        me->SummonCreature(QUEST_TARGET, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                        CAST_PLR(Killer)->KilledMonsterCredit(QUEST_TARGET, 0);
                    }
                    else
                        me->SummonCreature(netherwebVictims[rand()%6], 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);

                    if (rand()%100 < 75)
                        me->SummonCreature(netherwebVictims[rand()%6], 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                    me->SummonCreature(netherwebVictims[rand()%6], 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                }
            }
        }
    };
};

/*######
## npc_floon
######*/

#define GOSSIP_FLOON1           "You owe Sim'salabim money. Hand them over or die!"
#define GOSSIP_FLOON2           "Hand over the money or die...again!"

enum eFloon
{
    SAY_FLOON_ATTACK        = -1000195,

    SPELL_SILENCE           = 6726,
    SPELL_FROSTBOLT         = 9672,
    SPELL_FROST_NOVA        = 11831,

    FACTION_HOSTILE_FL      = 1738,
    QUEST_CRACK_SKULLS      = 10009
};

class npc_floon : public CreatureScript
{
public:
    npc_floon() : CreatureScript("npc_floon") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FLOON2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(9443, creature->GetGUID());
        }
        if (action == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->setFaction(FACTION_HOSTILE_FL);
            DoScriptText(SAY_FLOON_ATTACK, creature, player);
            creature->AI()->AttackStart(player);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_CRACK_SKULLS) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FLOON1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        player->SEND_GOSSIP_MENU(9442, creature->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_floonAI (creature);
    }

    struct npc_floonAI : public ScriptedAI
    {
        npc_floonAI(Creature* creature) : ScriptedAI(creature)
        {
            m_uiNormFaction = creature->getFaction();
        }

        uint32 m_uiNormFaction;
        uint32 Silence_Timer;
        uint32 Frostbolt_Timer;
        uint32 FrostNova_Timer;

        void Reset()
        {
            Silence_Timer = 2000;
            Frostbolt_Timer = 4000;
            FrostNova_Timer = 9000;
            if (me->getFaction() != m_uiNormFaction)
                me->setFaction(m_uiNormFaction);
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (Silence_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SILENCE);
                Silence_Timer = 30000;
            } else Silence_Timer -= diff;

            if (FrostNova_Timer <= diff)
            {
                DoCast(me, SPELL_FROST_NOVA);
                FrostNova_Timer = 20000;
            } else FrostNova_Timer -= diff;

            if (Frostbolt_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FROSTBOLT);
                Frostbolt_Timer = 5000;
            } else Frostbolt_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_isla_starmane
######*/
enum eIslaStarmaneData
{
    SAY_PROGRESS_1  = -1000571,
    SAY_PROGRESS_2  = -1000572,
    SAY_PROGRESS_3  = -1000573,
    SAY_PROGRESS_4  = -1000574,

    QUEST_EFTW_H    = 10052,
    QUEST_EFTW_A    = 10051,
    GO_CAGE         = 182794,
    SPELL_CAT       = 32447,
};

class npc_isla_starmane : public CreatureScript
{
public:
    npc_isla_starmane() : CreatureScript("npc_isla_starmane") { }

    struct npc_isla_starmaneAI : public npc_escortAI
    {
        npc_isla_starmaneAI(Creature* creature) : npc_escortAI(creature) {}

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();

            if (!player)
                return;

            switch (i)
            {
            case 0:
                {
                GameObject* Cage = me->FindNearestGameObject(GO_CAGE, 10);
                if (Cage)
                    Cage->SetGoState(GO_STATE_ACTIVE);
                }
                break;
            case 2: DoScriptText(SAY_PROGRESS_1, me, player); break;
            case 5: DoScriptText(SAY_PROGRESS_2, me, player); break;
            case 6: DoScriptText(SAY_PROGRESS_3, me, player); break;
            case 29:DoScriptText(SAY_PROGRESS_4, me, player);
                if (player)
                {
                    if (player->GetTeam() == ALLIANCE)
                        player->GroupEventHappens(QUEST_EFTW_A, me);
                    else if (player->GetTeam() == HORDE)
                        player->GroupEventHappens(QUEST_EFTW_H, me);
                }
                me->SetInFront(player); break;
            case 30: me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE); break;
            case 31: DoCast(me, SPELL_CAT);
                me->SetWalk(false);
                break;
            }
        }

        void Reset()
        {
            me->RestoreFaction();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (Player* player = GetPlayerForEscort())
            {
                if (player->GetTeam() == ALLIANCE)
                    player->FailQuest(QUEST_EFTW_A);
                else if (player->GetTeam() == HORDE)
                    player->FailQuest(QUEST_EFTW_H);
            }
        }
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_EFTW_H || quest->GetQuestId() == QUEST_EFTW_A)
        {
            CAST_AI(npc_escortAI, (creature->AI()))->Start(true, false, player->GetGUID());
            creature->setFaction(113);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_isla_starmaneAI(creature);
    }
};

/*######
## go_skull_pile
######*/
#define GOSSIP_S_DARKSCREECHER_AKKARAI         "Summon Darkscreecher Akkarai"
#define GOSSIP_S_KARROG         "Summon Karrog"
#define GOSSIP_S_GEZZARAK_THE_HUNTRESS         "Summon Gezzarak the Huntress"
#define GOSSIP_S_VAKKIZ_THE_WINDRAGER         "Summon Vakkiz the Windrager"

class go_skull_pile : public GameObjectScript
{
public:
    go_skull_pile() : GameObjectScript("go_skull_pile") { }

    bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (sender)
        {
            case GOSSIP_SENDER_MAIN:    SendActionMenu(player, go, action); break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if ((player->GetQuestStatus(11885) == QUEST_STATUS_INCOMPLETE) || player->GetQuestRewardStatus(11885))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_S_DARKSCREECHER_AKKARAI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_S_KARROG, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_S_GEZZARAK_THE_HUNTRESS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_S_VAKKIZ_THE_WINDRAGER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        }

        player->SEND_GOSSIP_MENU(go->GetGOInfo()->questgiver.gossipID, go->GetGUID());
        return true;
    }

    void SendActionMenu(Player* player, GameObject* /*go*/, uint32 action)
    {
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                  player->CastSpell(player, 40642, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                  player->CastSpell(player, 40640, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                  player->CastSpell(player, 40632, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 4:
                  player->CastSpell(player, 40644, false);
                break;
        }
    }
};

/*######
## npc_slim
######*/

enum eSlim
{
    FACTION_CONSORTIUM  = 933
};

class npc_slim : public CreatureScript
{
public:
    npc_slim() : CreatureScript("npc_slim") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isVendor() && player->GetReputationRank(FACTION_CONSORTIUM) >= REP_FRIENDLY)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            player->SEND_GOSSIP_MENU(9896, creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(9895, creature->GetGUID());

        return true;
    }
};

/*########
####npc_akuno
#####*/

enum eAkuno
{
    QUEST_ESCAPING_THE_TOMB = 10887,
    NPC_CABAL_SKRIMISHER    = 21661
};

class npc_akuno : public CreatureScript
{
public:
    npc_akuno() : CreatureScript("npc_akuno") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_ESCAPING_THE_TOMB)
        {
            if (npc_akunoAI* pEscortAI = CAST_AI(npc_akuno::npc_akunoAI, creature->AI()))
                pEscortAI->Start(false, false, player->GetGUID());

            if (player->GetTeamId() == TEAM_ALLIANCE)
                creature->setFaction(FACTION_ESCORT_A_NEUTRAL_PASSIVE);
            else
                creature->setFaction(FACTION_ESCORT_H_NEUTRAL_PASSIVE);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_akunoAI(creature);
    }

    struct npc_akunoAI : public npc_escortAI
    {
        npc_akunoAI(Creature* creature) : npc_escortAI(creature) {}

        void WaypointReached(uint32 i)
        {
            Player* player = GetPlayerForEscort();

            if (!player)
                return;

            switch (i)
            {
                case 3:
                    me->SummonCreature(NPC_CABAL_SKRIMISHER, -2795.99f, 5420.33f, -34.53f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(NPC_CABAL_SKRIMISHER, -2793.55f, 5412.79f, -34.53f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    break;
                case 11:
                    if (player && player->GetTypeId() == TYPEID_PLAYER)
                        player->GroupEventHappens(QUEST_ESCAPING_THE_TOMB, me);
                    break;
            }
        }

        void JustSummoned(Creature* summon)
        {
            summon->AI()->AttackStart(me);
        }
    };
};

/*######
## npc_skyguard_prisoner
## Quest Soporte Escape from Skettis http://www.wowhead.com/quest=11085
######*/

enum SPrisoner
{
    SAY_START                     = -1000716,
    SAY_DONT_LIKE                 = -1000717,
    SAY_COMPLETE                  = -1000718,
    GO_PRISONER_CAGE              = 185952,
    QUEST_ESCAPE_FROM_SKETTIS     = 11085,
};

class npc_skyguard_prisoner : public CreatureScript
{
public:
    npc_skyguard_prisoner(): CreatureScript ("npc_skyguard_prisoner") { }
    
    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const * pQuest)
    {
        if (pQuest->GetQuestId() == QUEST_ESCAPE_FROM_SKETTIS)
        {
            if (GameObject* pGo = pCreature->FindNearestGameObject(GO_PRISONER_CAGE, 10.0f))
                pGo->UseDoorOrButton();
            
            if (npc_skyguard_prisoner::npc_skyguard_prisonerAI* pEscortAI = CAST_AI(npc_skyguard_prisoner::npc_skyguard_prisonerAI, pCreature->AI()))
                pEscortAI->StartEvent(pPlayer, pQuest);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_skyguard_prisonerAI(pCreature);
    }

    struct npc_skyguard_prisonerAI : public npc_escortAI 
    {

        npc_skyguard_prisonerAI(Creature *c) : npc_escortAI(c) { }

        void Reset() { }
        
        uint32 CalculateWaypointID()
        {
            switch (me->GetGUIDLow())
            {
            case 1189307:
                return 1;
                break;
            case 1189309:
                return 2;
                break;
            default:
                return 3;
                break;
            }
        }

        void StartEvent(Player* pPlayer, const Quest* pQuest)
        {
            switch (CalculateWaypointID())
            {
            case 1:
                AddWaypoint(0, -4108.25f, 3032.18f, 344.799f, 3000);
                AddWaypoint(1, -4114.41f, 3036.73f, 344.039f);
                AddWaypoint(2, -4126.41f, 3026.07f, 344.156f);
                AddWaypoint(3, -4145.17f, 3029.69f, 337.423f);
                AddWaypoint(4, -4173.69f, 3035.72f, 343.346f);
                AddWaypoint(5, -4173.70f, 3047.37f, 343.888f);
                AddWaypoint(6, -4183.47f, 3060.62f, 344.157f, 3000);
                AddWaypoint(7, -4179.13f, 3090.20f, 323.971f, 30000);
                Start(false, false, pPlayer->GetGUID(), pQuest);
                break;
            case 2:
                AddWaypoint(0, -3718.81f, 3787.24f, 302.890f, 3000);
                AddWaypoint(1, -3714.44f, 3780.35f, 302.075f);
                AddWaypoint(2, -3698.33f, 3788.04f, 302.171f);
                AddWaypoint(3, -3679.36f, 3780.25f, 295.077f);
                AddWaypoint(4, -3654.82f, 3770.43f, 301.291f);
                AddWaypoint(5, -3656.07f, 3757.31f, 301.985f);
                AddWaypoint(6, -3648.83f, 3743.07f, 302.173f, 3000);
                AddWaypoint(7, -3659.16f, 3714.94f, 281.576f, 30000);
                Start(false, false, pPlayer->GetGUID(), pQuest);
                break;
            case 3:
                AddWaypoint(0, -3671.51f, 3385.36f, 312.956f, 3000);
                AddWaypoint(1, -3677.74f, 3379.05f, 312.136f);
                AddWaypoint(2, -3667.52f, 3366.45f, 312.233f);
                AddWaypoint(3, -3672.87f, 3343.52f, 304.994f);
                AddWaypoint(4, -3679.35f, 3319.01f, 311.419f);
                AddWaypoint(5, -3692.93f, 3318.69f, 312.081f);
                AddWaypoint(6, -3704.08f, 3309.56f, 312.233f, 3000);
                AddWaypoint(7, -3733.99f, 3315.77f, 292.093f, 30000);
                Start(false, false, pPlayer->GetGUID(), pQuest);
                break;
            }
            return;
        }

        void WaypointReached(uint32 uiPointId)
        {
            if (Player* pPlayer = GetPlayerForEscort())
            {
                switch (uiPointId)
                {
                case 0:
                    DoScriptText(SAY_START, me);
                    break;
                case 6:
                    DoScriptText(SAY_DONT_LIKE, me);
                    break;
                case 7:
                    DoScriptText(SAY_COMPLETE, me);
                    if (Player* pPlayer = GetPlayerForEscort())
                        pPlayer->GroupEventHappens(QUEST_ESCAPE_FROM_SKETTIS, me);
                    break;
                }
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            npc_escortAI::UpdateAI(uiDiff);

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_mana_bomb
## http://www.wowhead.com/quest=10446 The Final Code (Alliance)
## http://www.wowhead.com/quest=10447 The Final Code (Horde)
######*/

enum eManaBomb
{
    SAY_COUNT_1                 = -1000472,
    SAY_COUNT_2                 = -1000473,
    SAY_COUNT_3                 = -1000474,
    SAY_COUNT_4                 = -1000475,
    SAY_COUNT_5                 = -1000476,
    SPELL_MANA_BOMB_LIGHTNING   = 37843,
    SPELL_MANA_BOMB_EXPL        = 35513,
    NPC_MANA_BOMB_EXPL_TRIGGER  = 20767,
    NPC_MANA_BOMB_KILL_TRIGGER  = 21039
};

class npc_mana_bomb : public CreatureScript
{
public:
    npc_mana_bomb() : CreatureScript("npc_mana_bomb") { }

    bool OnGossipHello(Player* pPlayer, GameObject* pGo)
    {
        if (Creature* pCreature = GetClosestCreatureWithEntry(pGo, NPC_MANA_BOMB_EXPL_TRIGGER, INTERACTION_DISTANCE))
        {
            if (npc_mana_bombAI* pBombAI = dynamic_cast<npc_mana_bombAI*>(pCreature->AI()))
                pBombAI->DoTrigger(pPlayer, pGo);
        }

        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mana_bombAI (pCreature);
    }

    struct npc_mana_bombAI : public ScriptedAI
    {
        npc_mana_bombAI(Creature* pCreature) : ScriptedAI(pCreature) 
        { 
            Reset(); 
        }

        GameObject* pManaBomb;

        bool m_bIsActivated;
        uint32 m_uiEventTimer;
        uint32 m_uiEventCounter;

        void Reset()
        {
            pManaBomb = NULL;
            m_bIsActivated = false;
            m_uiEventTimer = 1000;
            m_uiEventCounter = 0;
        }

        void DoTrigger(Player* pPlayer, GameObject* pGo)
        {
            if (m_bIsActivated)
                return;
            m_bIsActivated = true;
            pPlayer->KilledMonsterCredit(NPC_MANA_BOMB_KILL_TRIGGER,me->GetGUID());
            pManaBomb = pGo;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!m_bIsActivated)
                return;

            if (m_uiEventTimer < uiDiff)
            {
                m_uiEventTimer = 1000;

                if (m_uiEventCounter < 10)
                    me->CastSpell(me, SPELL_MANA_BOMB_LIGHTNING, false);

                switch(m_uiEventCounter)
                {
                    case 5:
                        if (pManaBomb)
                            pManaBomb->SetGoState(GO_STATE_ACTIVE);
                        DoScriptText(SAY_COUNT_1, me);
                        break;
                    case 6:
                        DoScriptText(SAY_COUNT_2, me);
                        break;
                    case 7:
                        DoScriptText(SAY_COUNT_3, me);
                        break;
                    case 8:
                        DoScriptText(SAY_COUNT_4, me);
                        break;
                    case 9:
                        DoScriptText(SAY_COUNT_5, me);
                        break;
                    case 10:
                        me->CastSpell(me, SPELL_MANA_BOMB_EXPL, false);
                        break;
                    case 30:
                        if (pManaBomb)
                            pManaBomb->SetGoState(GO_STATE_READY);                            
                        Reset();
                        break;
                }

                ++m_uiEventCounter;
            }
            else
                m_uiEventTimer -= uiDiff;
        }
    };
};

/*######
## npc_letoll
## http://www.wowhead.com/quest=10922 "Digging Through Bones"
######*/

enum NpcLetoll
{
    SAY_LE_START                    = -1000511,
    SAY_LE_KEEP_SAFE                = -1000512,
    SAY_LE_NORTH                    = -1000513,
    SAY_LE_ARRIVE                   = -1000514,
    SAY_LE_BURIED                   = -1000515,
    SAY_LE_ALMOST                   = -1000516,
    SAY_LE_DRUM                     = -1000517,
    SAY_LE_DRUM_REPLY               = -1000518,
    SAY_LE_DISCOVERY                = -1000519,
    SAY_LE_DISCOVERY_REPLY          = -1000520,
    SAY_LE_NO_LEAVE                 = -1000521,
    SAY_LE_NO_LEAVE_REPLY1          = -1000522,
    SAY_LE_NO_LEAVE_REPLY2          = -1000523,
    SAY_LE_NO_LEAVE_REPLY3          = -1000524,
    SAY_LE_NO_LEAVE_REPLY4          = -1000525,
    SAY_LE_SHUT                     = -1000526,
    SAY_LE_REPLY_HEAR               = -1000527,
    SAY_LE_IN_YOUR_FACE             = -1000528,
    SAY_LE_HELP_HIM                 = -1000529,
    EMOTE_LE_PICK_UP                = -1000530,
    SAY_LE_THANKS                   = -1000531,
    QUEST_DIGGING_BONES             = 10922,
    NPC_RESEARCHER                  = 22464,
    NPC_BONE_SIFTER                 = 22466,
    MAX_RESEARCHER                  = 4
};

class npc_letoll : public CreatureScript
{
public:
    npc_letoll() : CreatureScript("npc_letoll") { }

    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
    {
        if (pQuest->GetQuestId() == QUEST_DIGGING_BONES)
        {
            if (npc_letollAI* pEscortAI = dynamic_cast<npc_letollAI*>(pCreature->AI()))
            {
                DoScriptText(SAY_LE_START, pCreature);
                pCreature->setFaction(FACTION_ESCORT_N_NEUTRAL_PASSIVE);

                pEscortAI->Start(false, false, pPlayer->GetGUID(), pQuest, true);
            }
        }        
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_letollAI(pCreature);
    }

    struct npc_letollAI : public npc_escortAI
    {
        npc_letollAI(Creature* pCreature) : npc_escortAI(pCreature)
        {
            m_uiEventTimer = 5000;
            m_uiEventCount = 0;
            Reset();
        }
        
        std::list<Creature*> m_lResearchersList;
        uint32 m_uiEventTimer;
        uint32 m_uiEventCount;

        void Reset() {}

        void SetFormation()
        {
            uint32 uiCount = 0;

            for (std::list<Creature*>::iterator itr = m_lResearchersList.begin(); itr != m_lResearchersList.end(); ++itr)
            {
                float fAngle = uiCount < MAX_RESEARCHER ? M_PI/MAX_RESEARCHER - (uiCount*2*M_PI/MAX_RESEARCHER) : 0.0f;

                if ((*itr)->isAlive() && !(*itr)->isInCombat())
                    (*itr)->GetMotionMaster()->MoveFollow(me, 2.5f, fAngle);

                ++uiCount;
            }
        }

        Creature* GetAvailableResearcher(uint8 uiListNum)
        {
            if (!m_lResearchersList.empty())
            {
                uint8 uiNum = 1;

                for (std::list<Creature*>::iterator itr = m_lResearchersList.begin(); itr != m_lResearchersList.end(); ++itr)
                {
                    if (uiListNum && uiListNum != uiNum)
                    {
                        ++uiNum;
                        continue;
                    }

                    if ((*itr)->isAlive() && (*itr)->IsWithinDistInMap(me, 20.0f))
                        return (*itr);
                }
            }
            return NULL;
        }

        void JustStartedEscort()
        {
            m_uiEventTimer = 5000;
            m_uiEventCount = 0;

            m_lResearchersList.clear();

            float x, y, z;
            me->GetPosition(x, y, z);

            CellCoord pair(Trinity::ComputeCellCoord(x, y));
            Cell cell(pair);
            cell.SetNoCreate();

            Trinity::AllCreaturesOfEntryInRange check(me, NPC_RESEARCHER, 25);
            Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(me,m_lResearchersList, check);
            TypeContainerVisitor<Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange>, GridTypeMapContainer> cSearcher(searcher);
            cell.Visit(pair, cSearcher, *me->GetMap(), *me, 25);

            if (!m_lResearchersList.empty())
                SetFormation();
        }

        void WaypointReached(uint32 uiPointId)
        {
            Player* pPlayer = GetPlayerForEscort();

            if (!pPlayer)
                return;

            switch(uiPointId)
            {
                case 0:
                    if (pPlayer)
                        DoScriptText(SAY_LE_KEEP_SAFE, me, pPlayer);
                    break;
                case 1:
                    DoScriptText(SAY_LE_NORTH, me);
                    break;
                case 10:
                    DoScriptText(SAY_LE_ARRIVE, me);
                    break;
                case 12:
                    DoScriptText(SAY_LE_BURIED, me);
                    SetEscortPaused(true);
                    break;
                case 13:
                    DoScriptText(EMOTE_LE_PICK_UP, me);
                    if (pPlayer) 
                    {
                        DoScriptText(SAY_LE_THANKS, me, pPlayer);
                        pPlayer->GroupEventHappens(QUEST_DIGGING_BONES, me);
                    }
                    SetRun();
                    break;
            }
        }

        void Aggro(Unit* pWho)
        {
            if (pWho->isInCombat() && pWho->GetTypeId() == TYPEID_UNIT && pWho->GetEntry() == NPC_BONE_SIFTER)
                DoScriptText(SAY_LE_HELP_HIM, me);
        }

        void JustSummoned(Creature* pSummoned)
        {
            Player* pPlayer = GetPlayerForEscort();

            if (pPlayer && pPlayer->isAlive())
                pSummoned->AI()->AttackStart(pPlayer);
            else
                pSummoned->AI()->AttackStart(me);
        }

        void UpdateEscortAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
            {
                if (HasEscortState(STATE_ESCORT_PAUSED))
                {
                    if (m_uiEventTimer < uiDiff)
                    {
                        m_uiEventTimer = 7000;

                        switch(m_uiEventCount)
                        {
                            case 0:
                                DoScriptText(SAY_LE_ALMOST, me);
                                break;
                            case 1:
                                DoScriptText(SAY_LE_DRUM, me);
                                break;
                            case 2:
                                if (Creature* pResearcher = GetAvailableResearcher(0))
                                    DoScriptText(SAY_LE_DRUM_REPLY, pResearcher);
                                break;
                            case 3:
                                DoScriptText(SAY_LE_DISCOVERY, me);
                                break;
                            case 4:
                                if (Creature* pResearcher = GetAvailableResearcher(0))
                                    DoScriptText(SAY_LE_DISCOVERY_REPLY, pResearcher);
                                break;
                            case 5:
                                DoScriptText(SAY_LE_NO_LEAVE, me);
                                break;
                            case 6:
                                if (Creature* pResearcher = GetAvailableResearcher(1))
                                    DoScriptText(SAY_LE_NO_LEAVE_REPLY1, pResearcher);
                                break;
                            case 7:
                                if (Creature* pResearcher = GetAvailableResearcher(2))
                                    DoScriptText(SAY_LE_NO_LEAVE_REPLY2, pResearcher);
                                break;
                            case 8:
                                if (Creature* pResearcher = GetAvailableResearcher(3))
                                    DoScriptText(SAY_LE_NO_LEAVE_REPLY3, pResearcher);
                                break;
                            case 9:
                                if (Creature* pResearcher = GetAvailableResearcher(4))
                                    DoScriptText(SAY_LE_NO_LEAVE_REPLY4, pResearcher);
                                break;
                            case 10:
                                DoScriptText(SAY_LE_SHUT, me);
                                break;
                            case 11:
                                if (Creature* pResearcher = GetAvailableResearcher(0))
                                    DoScriptText(SAY_LE_REPLY_HEAR, pResearcher);
                                break;
                            case 12:
                                DoScriptText(SAY_LE_IN_YOUR_FACE, me);
                                me->SummonCreature(NPC_BONE_SIFTER, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                                break;
                            case 13:
                                SetEscortPaused(false);
                                break;
                        }

                        ++m_uiEventCount;
                    }
                    else
                        m_uiEventTimer -= uiDiff;
                }

                return;
            }

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_terokkar_forest()
{
    new mob_unkor_the_ruthless();
    new mob_infested_root_walker();
    new mob_rotting_forest_rager();
    new mob_netherweb_victim();
    new npc_floon();
    new npc_isla_starmane();
    new go_skull_pile();
    new npc_skywing();
    new npc_slim();
    new npc_akuno();
    new npc_skyguard_prisoner();
    new npc_mana_bomb();
    new npc_letoll();
}
