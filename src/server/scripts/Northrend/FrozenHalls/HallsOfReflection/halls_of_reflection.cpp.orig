/*
 * Copyright (C) 2008 - 2010 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 Myth Project <http://code.google.com/p/mythcore/>
 *
 * Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
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

 /* ScriptData
 SDName: Halls of Reflection
 SD%Complete: 0%
 SDComment: new script for tc implementation.
 SDCategory: Halls of Reflection
 EndScriptData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "halls_of_reflection.h"

enum
{
    /*INTRO - Pre Uther*/
    SAY_JAINA_INTRO_01                 = -1668001,
    SAY_SYLVANA_INTRO_01               = -1668021,
    SAY_JAINA_INTRO_02                 = -1668002,
    SAY_JAINA_INTRO_03                 = -1668003,
    SAY_SYLVANA_INTRO_03               = -1668022,
    SAY_JAINA_INTRO_04                 = -1668004,
    SAY_SYLVANA_INTRO_04               = -1668023,

    /*INTRO - Uther Dialog*/
    SAY_UTHER_A_01                     = -1668005, 
    SAY_UTHER_H_01                     = -1668024,
    SAY_JAINA_02                       = -1668006,
    SAY_SYLVANA_02                     = -1668025,
    SAY_UTHER_A_03                     = -1668007,
    SAY_UTHER_H_03                     = -1668026,
    SAY_JAINA_04                       = -1668008,
    SAY_SYLVANA_04                     = -1668027,
    SAY_UTHER_A_05                     = -1668009,
    SAY_UTHER_H_05                     = -1668028,
    SAY_JAINA_06                       = -1668010,
    SAY_SYLVANA_06                     = -1668029,
    SAY_UTHER_A_07                     = -1668011,
    SAY_UTHER_H_07                     = -1668030,
    SAY_JAINA_08                       = -1668012,
    SAY_SYLVANA_08                     = -1668031,
    SAY_UTHER_A_09                     = -1668013,
    SAY_UTHER_H_09                     = -1668032,
    SAY_JAINA_10                       = -1668014,
    SAY_UTHER_A_11                     = -1668015,
    SAY_UTHER_H_11                     = -1668033,
    SAY_JAINA_12                       = -1668017,
    SAY_SYLVANA_12                     = -1668034,
    SAY_UTHER_A_13                     = -1668016,
    SAY_UTHER_A_14                     = -1668018,
    SAY_JAINA_15                       = -1668019,

    /*INTRO - Lich King Arrive*/
    SAY_UTHER_A_16                     = -1668020,
    SAY_UTHER_H_16                     = -1668035,
    SAY_LICH_KING_17                   = -1668036,
    SAY_LICH_KING_18                   = -1668037,
    SAY_LICH_KING_19                   = -1668038,
    SAY_JAINA_20                       = -1668042,
    SAY_SYLVANA_20                     = -1668043,
    SAY_LICH_KING_A_21                 = -1594473,//
    SAY_LICH_KING_H_21                 = -1594474,//
    SAY_FALRIC_INTRO                   = -1668039,
    SAY_MARWYN_INTRO                   = -1668040,
    SAY_FALRIC_INTRO2                  = -1668041,

    /*INTRO - Pre Escape*/
    SAY_LICH_KING_AGGRO_A              = -1594477,
    SAY_LICH_KING_AGGRO_H              = -1594478,
    SAY_JAINA_AGGRO                    = -1594479,
    SAY_SYLVANA_AGGRO                  = -1594480,

    /*ESCAPE*/
    SAY_JAINA_WALL_01                  = -1594487,
    SAY_SYLVANA_WALL_01                = -1594488,
    SAY_JAINA_WALL_02                  = -1594489,
    SAY_SYLVANA_WALL_02                = -1594490,
    SAY_LICH_KING_WALL_02              = -1594491,
    SAY_LICH_KING_WALL_03              = -1594492,
    SAY_LICH_KING_WALL_04              = -1594493,
    SAY_JAINA_WALL_03                  = -1594494,
    SAY_JAINA_WALL_04                  = -1594495,
    SAY_SYLVANA_WALL_03                = -1594496,
    SAY_SYLVANA_WALL_04                = -1594497,
    SAY_JAINA_ESCAPE_01                = -1594498,
    SAY_JAINA_ESCAPE_02                = -1594499,
    SAY_SYLVANA_ESCAPE_01              = -1594500,
    SAY_SYLVANA_ESCAPE_02              = -1594501,
    SAY_JAINA_TRAP                     = -1594502,
    SAY_SYLVANA_TRAP                   = -1594503,
    SAY_MATHEAS_JAINA                  = -1594505,
    SAY_LICH_KING_END_01               = -1594506,
    SAY_LICH_KING_END_02               = -1594507,
    SAY_LICH_KING_END_03               = -1594508,

    /*SPELLS AND VISUAL EFFECTS*/
    SPELL_TAKE_FROSTMOURNE             = 72729,
    SPELL_FROSTMOURNE_DESPAWN          = 72726,
    SPELL_FROSTMOURNE_SOUNDS           = 70667,
    SPELL_CAST_VISUAL                  = 65633,  //Jaina And Sylavana cast this when summon uther.
    SPELL_BOSS_SPAWN_AURA              = 72712,  //Falric and Marwyn
    SPELL_UTHER_DESPAWN                = 70693,
    SPELL_WINTER                       = 69780,
    SPELL_FURY_OF_FROSTMOURNE          = 70063,
    SPELL_SOUL_REAPER                  = 73797,
    SPELL_RAISE_DEAD                   = 69818,
    SPELL_ICE_PRISON                   = 69708,
    SPELL_DARK_ARROW                   = 70194,
    SPELL_ICE_BARRIER                  = 69787,
    SPELL_DESTROY_ICE_WALL_01          = 69784, //Jaina
    SPELL_DESTROY_ICE_WALL_02          = 70224, 
    SPELL_DESTROY_ICE_WALL_03          = 70225, //Sylvana
    SPELL_SUMMON_ICE_WALL              = 69784,
    SPELL_SYLVANA_JUMP                 = 68339,
    SPELL_SYLVANA_STEP                 = 69087,
    SPELL_SILENCE                      = 69413,
    SPELL_LICH_KING_CAST               = 57561,
    SPELL_FROSTMOURNE_VISUAL           = 73220,
    SPELL_SHIELD_DISRUPTION            = 58291,

    FACTION                            = 2076,
};

class npc_jaina_and_sylvana_HRintro : public CreatureScript
{
public:
    npc_jaina_and_sylvana_HRintro() : CreatureScript("npc_jaina_and_sylvana_HRintro") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        InstanceScript* m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();

        if (!m_pInstance)
            return false;

        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->CLOSE_GOSSIP_MENU();
                m_pInstance->SetData(TYPE_EVENT, 1);
                break;
        }

        if(pPlayer->GetTeam() == ALLIANCE) 
            m_pInstance->SetData(DATA_LIDER, 1);
        else
            m_pInstance->SetData(DATA_LIDER, 2);

        m_pInstance->SetData64(DATA_ESCAPE_LIDER,pCreature->GetGUID());

        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        InstanceScript* m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();

        if(pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu( pCreature->GetGUID());

        switch(pCreature->GetEntry())
        {
            case NPC_JAINA:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Леди Джайна, мы готовы к следующей миссии!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                break;
            case NPC_SYLVANA: 
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Леди Сильвана, мы готовы к следующей миссии!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);    
                //pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Поехали!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                break;
        }

        pPlayer->PlayerTalkClass->SendGossipMenu(907,pCreature->GetGUID());
        return true;
    }

    struct npc_jaina_and_sylvana_HRintroAI : public ScriptedAI
    {
        npc_jaina_and_sylvana_HRintroAI(Creature *pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* m_pInstance;

        uint32 StepTimer;
        uint32 Step;
        uint64 m_uiFrostmourneGUID;
        uint64 m_uiMainGateGUID;
        uint64 m_uiFalricGUID;
        uint64 m_uiMarwynGUID;
        Creature* pUther;
        Creature* pLichKing;
        bool Small;

        void Reset()
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            Small = false;
        }

        void StartEvent()
        {
            if(!m_pInstance)
                return
            sLog.outDebug("EventMGR: creature %u received signal %u ",me->GetEntry(),m_pInstance->GetData(TYPE_EVENT));
            m_pInstance->SetData(TYPE_PHASE, 1);
            m_pInstance->SetData(TYPE_EVENT, 0);
            Step = 1;
            StepTimer = 100;
        }

        void JumpNextStep(uint32 Time)
        {
            StepTimer = Time;
            Step++;
        }

        void Event()
        {
            switch(Step)
            {
                case 1:
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    JumpNextStep(2000);
                    break;
                case 2:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_INTRO_01, me);
                        JumpNextStep(5000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        DoScriptText(SAY_SYLVANA_INTRO_01, me);
                        JumpNextStep(8000);
                    }
                    break;
                case 3: 
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_INTRO_02, me);
                        JumpNextStep(5000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                        JumpNextStep(500);
                    break;
                case 4:
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                    me->GetMotionMaster()->MovePoint(0, 5307.031f, 1997.920f, 709.341f);
                    JumpNextStep(10000);
                    break;
                case 5:
                    if(Creature* pTarget = me->SummonCreature(NPC_ALTAR_TARGET,5309.374f,2006.788f,711.615f,1.37f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,360000))
                    {
                        me->SetUInt64Value(UNIT_FIELD_TARGET, pTarget->GetGUID());
                        pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    }
                    JumpNextStep(1000);
                    break;
                case 6:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_INTRO_03, me);
                        JumpNextStep(5000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        DoScriptText(SAY_SYLVANA_INTRO_03, me);
                        JumpNextStep(6000);
                    }
                    break;
                case 7:
                    DoCast(me, SPELL_CAST_VISUAL);
                    if(me->GetEntry() == NPC_JAINA)
                        DoScriptText(SAY_JAINA_INTRO_04, me);
                    if(me->GetEntry() == NPC_SYLVANA)
                        DoScriptText(SAY_SYLVANA_INTRO_04, me);
                    JumpNextStep(3000);
                    break;
                case 8:
                    DoCast(me, SPELL_FROSTMOURNE_SOUNDS);
                    m_uiFrostmourneGUID = m_pInstance->GetData64(GO_FROSTMOURNE);
                    if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_uiFrostmourneGUID))
                    {
                        pFrostmourne->SetGoState(GO_STATE_ACTIVE);
                    }
                    if(me->GetEntry() == NPC_JAINA)
                        JumpNextStep(12000);
                    if(me->GetEntry() == NPC_SYLVANA)
                        JumpNextStep(8000);
                    break;
                case 9:
                    if(Creature* Uther = me->SummonCreature(NPC_UTHER,5308.228f,2003.641f,709.341f,4.17f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,360000))
                    {
                        pUther = Uther;
                        Uther->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
                        me->SetUInt64Value(UNIT_FIELD_TARGET, Uther->GetGUID());
                        if(me->GetEntry() == NPC_JAINA)
                        {
                            DoScriptText(SAY_UTHER_A_01, Uther);
                            JumpNextStep(3000);
                        }
                        if(me->GetEntry() == NPC_SYLVANA)
                        {
                            DoScriptText(SAY_UTHER_H_01, Uther);
                            JumpNextStep(10000);
                        }
                    }
                    break;
                case 10:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_02, me);
                        JumpNextStep(5000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        DoScriptText(SAY_SYLVANA_02, me);
                        JumpNextStep(3000);
                    }
                    break;
                case 11:
                    if(me->GetEntry() == NPC_JAINA && pUther)
                    {
                        DoScriptText(SAY_UTHER_A_03, pUther);
                        JumpNextStep(7000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA && pUther)
                    {
                        DoScriptText(SAY_UTHER_H_03, pUther);
                        JumpNextStep(6000);
                    }
                    if(Small)
                        Step = 24;
                    break;
                case 12:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_04, me);
                        JumpNextStep(2000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        DoScriptText(SAY_SYLVANA_04, me);
                        JumpNextStep(5000);
                    }
                    break;
                case 13:
                    if(me->GetEntry() == NPC_JAINA && pUther)
                    {
                        DoScriptText(SAY_UTHER_A_05, pUther);
                        JumpNextStep(10000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA && pUther)
                    {
                        DoScriptText(SAY_UTHER_H_05, pUther); 
                        JumpNextStep(19000);
                    }
                    break;
                case 14:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_06, me);
                        JumpNextStep(6000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        DoScriptText(SAY_SYLVANA_06, me); 
                        JumpNextStep(2000);
                    }
                    break;
                case 15:
                    if(me->GetEntry() == NPC_JAINA && pUther)
                    {
                        DoScriptText(SAY_UTHER_A_07, pUther); 
                        JumpNextStep(12000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA && pUther)
                    {
                        DoScriptText(SAY_UTHER_H_07, pUther);
                        JumpNextStep(20000);
                    }
                    break;
                case 16:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_08, me); 
                        JumpNextStep(6000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        DoScriptText(SAY_SYLVANA_08, me);
                        JumpNextStep(3000);
                    }
                    break;
                case 17:
                    if(me->GetEntry() == NPC_JAINA && pUther)
                    {
                        DoScriptText(SAY_UTHER_A_09, pUther);
                        JumpNextStep(12000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA && pUther)
                    {
                        DoScriptText(SAY_UTHER_H_09, pUther);
                        JumpNextStep(11000);
                    }
                    break;
                case 18:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_10, me);
                        JumpNextStep(11000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        JumpNextStep(100);
                    }
                    break;
                case 19:
                    if(me->GetEntry() == NPC_JAINA && pUther)
                    {
                        DoScriptText(SAY_UTHER_A_11, pUther);
                        JumpNextStep(24000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA && pUther)
                    {
                        DoScriptText(SAY_UTHER_H_11, pUther);
                        JumpNextStep(9000);
                    }
                    break;
                case 20:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_12, me);
                        JumpNextStep(2000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        DoScriptText(SAY_SYLVANA_12, me);
                        JumpNextStep(2100);
                    }
                    break;
                case 21:
                    if(me->GetEntry() == NPC_JAINA && pUther)
                    {
                        DoScriptText(SAY_UTHER_A_13, pUther); 
                        JumpNextStep(5000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        JumpNextStep(100);
                    }
                    break;
                case 22:
                    if(me->GetEntry() == NPC_JAINA && pUther)
                    {
                        DoScriptText(SAY_UTHER_A_14, pUther); 
                        JumpNextStep(12000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        JumpNextStep(100);
                    }
                    break;
                case 23:
                    if(me->GetEntry() == NPC_JAINA)
                    {
                        DoScriptText(SAY_JAINA_15, me);
                        JumpNextStep(2000);
                    }
                    if(me->GetEntry() == NPC_SYLVANA)
                    {
                        JumpNextStep(100);
                    }
                    break;
                case 24:
                    if(me->GetEntry() == NPC_JAINA && pUther)
                        DoScriptText(SAY_UTHER_A_16, pUther);
                    if(me->GetEntry() == NPC_SYLVANA && pUther)
                        DoScriptText(SAY_UTHER_H_16, pUther);
                    m_uiMainGateGUID = m_pInstance->GetData64(GO_IMPENETRABLE_DOOR);
                    if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiMainGateGUID))
                    {
                        pGate->SetGoState(GO_STATE_ACTIVE);
                    }
                    if(Creature* LichKing = me->SummonCreature(NPC_LICH_KING,5362.469f,2062.342f,707.695f,3.97f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,360000))
                    {
                        pLichKing = LichKing;
                        LichKing->setActive(true);
                    }
                    JumpNextStep(1000);
                    break;
                case 25:
                    if(pUther)
                        pUther->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
                    if(pLichKing)
                    {
                        pLichKing->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pLichKing->AddUnitMovementFlag(MOVEMENTFLAG_WALKING); 
                        pLichKing->GetMotionMaster()->MovePoint(0, 5314.881f, 2012.496f, 709.341f);
                    }
                    JumpNextStep(3000);
                    break;
                case 26:
                    if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiMainGateGUID))
                        pGate->SetGoState(GO_STATE_READY);
                    JumpNextStep(7000);
                    break;
                case 27:
                    if(pUther)
                        pUther->CastSpell(pUther, SPELL_UTHER_DESPAWN, false);
                    JumpNextStep(500);
                    break;
                case 28:
                    if(pLichKing)
                        DoScriptText(SAY_LICH_KING_17, pLichKing);
                    JumpNextStep(10000);
                    break;
                case 29:
                    if(pLichKing)
                        DoScriptText(SAY_LICH_KING_18, pLichKing);
                    JumpNextStep(5000);
                    break;
                case 30:
                    if(pLichKing)
                        pLichKing->CastSpell(pLichKing, SPELL_TAKE_FROSTMOURNE, false);
                    if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_uiFrostmourneGUID))
                        pFrostmourne->SetGoState(GO_STATE_READY);
                    JumpNextStep(1500);
                    break;
                case 31:
                    if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_uiFrostmourneGUID))
                        pFrostmourne->SetPhaseMask(0, true);
                    if(pLichKing)
                        pLichKing->CastSpell(pLichKing, SPELL_FROSTMOURNE_VISUAL, false);
                    me->RemoveAurasDueToSpell(SPELL_FROSTMOURNE_SOUNDS);
                    JumpNextStep(5000);
                    break;
                case 32:
                    if(pLichKing)
                        DoScriptText(SAY_LICH_KING_19, pLichKing);
                    JumpNextStep(9000);
                    break;
                case 33:
                    m_uiFalricGUID = m_pInstance->GetData64(NPC_FALRIC);
                    m_uiMarwynGUID = m_pInstance->GetData64(NPC_MARWYN);
                    if(Creature* Falric = m_pInstance->instance->GetCreature(m_uiFalricGUID))
                    {
                        Falric->RemoveAllAuras();
                        Falric->SetVisible(true);
                        Falric->CastSpell(Falric, SPELL_BOSS_SPAWN_AURA, false);
                        Falric->GetMotionMaster()->MovePoint(0, 5283.309f, 2031.173f, 709.319f);
                    }
                    if(Creature* Marwyn = m_pInstance->instance->GetCreature(m_uiMarwynGUID))
                    {
                        Marwyn->RemoveAllAuras();
                        Marwyn->SetVisible(true);
                        Marwyn->CastSpell(Marwyn, SPELL_BOSS_SPAWN_AURA, false);
                        Marwyn->GetMotionMaster()->MovePoint(0, 5335.585f, 1981.439f, 709.319f);
                    }
                    if(pLichKing)
                        pLichKing->GetMotionMaster()->MovePoint(0, 5402.286f, 2104.496f, 707.695f);
                    JumpNextStep(600);
                    break;
                case 34:
                    if(Creature* Falric = ((Creature*)Unit::GetUnit(*me, m_uiFalricGUID)))
                        DoScriptText(SAY_FALRIC_INTRO, Falric);
                    if(Creature* Marwyn = ((Creature*)Unit::GetUnit(*me, m_uiMarwynGUID)))
                        DoScriptText(SAY_MARWYN_INTRO, Marwyn);
                    JumpNextStep(3000);
                    break;
                case 35:
                    if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiMainGateGUID))
                        pGate->SetGoState(GO_STATE_ACTIVE);
                    if(Creature* Falric = ((Creature*)Unit::GetUnit(*me, m_uiFalricGUID)))
                        DoScriptText(SAY_FALRIC_INTRO2, Falric);
                    m_pInstance->SetData(TYPE_FALRIC, SPECIAL);
                    JumpNextStep(4000);
                    break;
                case 36:
                    if(me->GetEntry() == NPC_JAINA)
                        DoScriptText(SAY_JAINA_20, me);
                    if(me->GetEntry() == NPC_SYLVANA)
                        DoScriptText(SAY_SYLVANA_20, me);
                    me->GetMotionMaster()->MovePoint(0, 5443.880f, 2147.095f, 707.695f);
                    JumpNextStep(4000);
                    break;
                case 37:
                    if(pLichKing)
                    {
                        pLichKing->GetMotionMaster()->MovementExpired(false);
                        pLichKing->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                        pLichKing->GetMotionMaster()->MovePoint(0, 5443.880f, 2147.095f, 707.695f);
                    }
                    if(me->GetEntry() == NPC_JAINA && pLichKing)
                        DoScriptText(SAY_LICH_KING_A_21, pLichKing);
                    if(me->GetEntry() == NPC_SYLVANA && pLichKing)
                        DoScriptText(SAY_LICH_KING_H_21, pLichKing);
                    JumpNextStep(8000);
                    break;
                case 38:
                    if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiMainGateGUID))
                        pGate->SetGoState(GO_STATE_READY); 
                    JumpNextStep(5000);
                    break;
                case 39:
                    me->SetVisible(false);
                    if(pLichKing)
                        pLichKing->SetVisible(false);
                    m_pInstance->SetData(TYPE_PHASE, 2);  
                    JumpNextStep(1000);
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if(!m_pInstance)
                return;

            if(m_pInstance->GetData(TYPE_EVENT) == 1
                &&  m_pInstance->GetData64(DATA_ESCAPE_LIDER) == me->GetGUID())
                StartEvent();

            if(m_pInstance->GetData(TYPE_EVENT) == 2
                &&  m_pInstance->GetData64(DATA_ESCAPE_LIDER) == me->GetGUID())
            {
                Small = true;
                StartEvent();
            }

            if(StepTimer < diff && m_pInstance->GetData(TYPE_PHASE) == 1)
                Event();
            else StepTimer -= diff;

            return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_jaina_and_sylvana_HRintroAI(pCreature);
    }

};

class npc_jaina_and_sylvana_HRextro : public CreatureScript
{
public:
    npc_jaina_and_sylvana_HRextro() : CreatureScript("npc_jaina_and_sylvana_HRextro") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        InstanceScript* m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->CLOSE_GOSSIP_MENU();
                ((npc_jaina_and_sylvana_HRextroAI*)pCreature->AI())->Start(false,true);
                pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pCreature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                pCreature->setActive(true);

                if(m_pInstance)
                {
                    m_pInstance->SetData64(DATA_ESCAPE_LIDER, pCreature->GetGUID());
                    m_pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
                    m_pInstance->SetData(TYPE_PHASE, 5);
                }
                return true;
            default:
                return false;
        }
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        InstanceScript*   m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();

        if(!m_pInstance)
            return false;

        if(m_pInstance->GetData(TYPE_LICH_KING) == DONE)
            return false;

        if(pCreature->isQuestGiver())
           pPlayer->PrepareQuestMenu( pCreature->GetGUID());

        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Побежали!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }

    struct npc_jaina_and_sylvana_HRextroAI : public npc_escortAI
    {
        npc_jaina_and_sylvana_HRextroAI(Creature *pCreature) : npc_escortAI(pCreature)
        {
            m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* m_pInstance;

        uint32 CastTimer;
        uint32 StepTimer;
        uint32 Step;
        int32 HoldTimer;
        uint32 Count;
        bool Fight;
        bool Event;
        bool PreFight;
        bool WallCast;
        uint64 m_uiLichKingGUID;
        uint64 m_uiLiderGUID;
        uint64 m_uiIceWallGUID;
        uint64 m_uipWallTargetGUID;
        Creature* pLichKing;
        uint32 m_chestID;

        void Reset()
        {  
            if(!m_pInstance)
                return;

            if(m_pInstance->GetData(TYPE_LICH_KING) == IN_PROGRESS)
                return;

            Step = 0;
            StepTimer = 500;
            Fight = true;
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            m_uipWallTargetGUID = 0;

            if(me->GetEntry() == NPC_JAINA_OUTRO)
            {
                me->CastSpell(me, SPELL_ICE_BARRIER, false);
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
            }
            if(m_pInstance->GetData(TYPE_LICH_KING) == DONE)
                me->SetVisible(false);

        }

        void AttackStart(Unit* who)
        {
            if(!who)
                return;

            if(me->GetEntry() != NPC_SYLVANA_OUTRO)
                return;

            if(m_pInstance->GetData(TYPE_LICH_KING) == IN_PROGRESS || Fight != true)
                return;

            npc_escortAI::AttackStart(who);

        }

        void JustDied(Unit* pKiller)
        {
            if(!m_pInstance)
                return;
            m_pInstance->SetData(TYPE_LICH_KING, FAIL);
        }

        void WaypointReached(uint32 i)
        {
            switch(i)
            {
                case 3:
                    m_pInstance->SetData(TYPE_ICE_WALL_01, IN_PROGRESS);
                    if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ICE_WALL_1)))
                    {
                        pGate->SetGoState(GO_STATE_READY);
                        m_uiIceWallGUID = pGate->GetGUID();
                    }
                    break;
                case 4:
                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                        DoScriptText(SAY_JAINA_WALL_01, me);
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        DoScriptText(SAY_SYLVANA_WALL_01, me);
                    CastTimer = 1000;
                    HoldTimer = 30000;
                    SetEscortPaused(true);
                    if (Creature *pWallTarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
                    {
                        m_uipWallTargetGUID = pWallTarget->GetGUID();
                        pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if(me->GetEntry() == NPC_JAINA_OUTRO)
                            me->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_01, false);
                    }
                    WallCast = true;
                    break;
                case 6:
                    m_pInstance->SetData(TYPE_ICE_WALL_02, IN_PROGRESS);
                    if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
                    {
                        if(pWallTarget->isAlive())
                        {
                            pWallTarget->ForcedDespawn();
                            m_uipWallTargetGUID = 0;
                        }
                    }
                    break;
                case 8:
                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                        DoScriptText(SAY_JAINA_WALL_02, me);
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        DoScriptText(SAY_SYLVANA_WALL_02, me);
                    CastTimer = 1000;
                    HoldTimer = 30000;
                    SetEscortPaused(true);
                    if (Creature *pWallTarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
                    {
                        m_uipWallTargetGUID = pWallTarget->GetGUID();
                        pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if(me->GetEntry() == NPC_JAINA_OUTRO)
                            me->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_01, false);
                    }
                    WallCast = true;
                    break;
                case 9:
                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                        DoScriptText(SAY_JAINA_ESCAPE_01, me);
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        DoScriptText(SAY_SYLVANA_ESCAPE_01, me);
                    break;
                case 11:
                    m_pInstance->SetData(TYPE_ICE_WALL_03, IN_PROGRESS);
                    if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
                    {
                        if(pWallTarget->isAlive())
                        {
                            pWallTarget->ForcedDespawn();
                            m_uipWallTargetGUID = 0;
                        }
                    }
                    break;
                case 12:
                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                        DoScriptText(SAY_JAINA_WALL_03, me);
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        DoScriptText(SAY_SYLVANA_WALL_03, me);
                    CastTimer = 1000;
                    HoldTimer = 30000;
                    SetEscortPaused(true);
                    if (Creature *pWallTarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
                    {
                        m_uipWallTargetGUID = pWallTarget->GetGUID();
                        pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if(me->GetEntry() == NPC_JAINA_OUTRO)
                            me->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_01, false);
                    }
                    WallCast = true;
                    break;
                case 13:
                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                        DoScriptText(SAY_JAINA_ESCAPE_02, me);
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        DoScriptText(SAY_SYLVANA_ESCAPE_02, me);
                    break;
                case 15:
                    m_pInstance->SetData(TYPE_ICE_WALL_04, IN_PROGRESS);
                    if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
                    {
                        if(pWallTarget->isAlive())
                        {
                            pWallTarget->ForcedDespawn();
                            m_uipWallTargetGUID = 0;
                        }
                    }
                    break;
                case 16:
                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                        DoScriptText(SAY_JAINA_WALL_04, me);
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        DoScriptText(SAY_SYLVANA_WALL_04, me);
                    CastTimer = 1000;
                    HoldTimer = 30000;
                    SetEscortPaused(true);
                    if (Creature *pWallTarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
                    {
                        m_uipWallTargetGUID = pWallTarget->GetGUID();
                        pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if(me->GetEntry() == NPC_JAINA_OUTRO)
                            me->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_01, false);
                    }
                    WallCast = true;
                    break;
                case 19:
                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                        DoScriptText(SAY_JAINA_TRAP, me);
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        DoScriptText(SAY_SYLVANA_TRAP, me);
                    break;          
                case 20:
                    if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
                    {
                        if(pWallTarget->isAlive())
                        {
                            pWallTarget->ForcedDespawn();
                            m_uipWallTargetGUID = 0;
                        }
                    }
                    SetEscortPaused(true);
                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                    break;
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            if(!who || !m_pInstance)
                return;

            if(who->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* pPlayer = (Player *)who;

            if(pPlayer->GetTeam() == ALLIANCE && me->GetEntry() == NPC_SYLVANA_OUTRO)
                return;

            if(pPlayer->GetTeam() == HORDE && me->GetEntry() == NPC_JAINA_OUTRO)
                return;

            if(me->IsWithinDistInMap(who, 50.0f)
                && m_pInstance->GetData(TYPE_FROST_GENERAL) == DONE
                && m_pInstance->GetData(TYPE_PHASE) == 3)
            {
                pPlayer = (Player *)who;
                Event = true;
                me->setFaction(FACTION);
                m_pInstance->SetData(TYPE_PHASE, 4);
            }
        }

        void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
        {
            if(!m_pInstance)
                return;

            if(m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            {
                uiDamage = 0;
                return;
            }

            if(m_pInstance->GetData(TYPE_LICH_KING) == IN_PROGRESS && WallCast == true)
            {
                HoldTimer = HoldTimer + 100;
                return;
            }
        }

        void JumpNextStep(uint32 Time)
        {
            StepTimer = Time;
            Step++;
        }

        void Intro()
        {
            switch(Step)
            {
                case 0:
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                    m_uiLichKingGUID = m_pInstance->GetData64(BOSS_LICH_KING);
                    pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGUID);
                    JumpNextStep(100);
                    break;
                case 1:
                    if(pLichKing)
                    {
                        pLichKing->SetPhaseMask(65535, true);
                        if(me->GetEntry() == NPC_JAINA_OUTRO)
                            DoScriptText(SAY_LICH_KING_AGGRO_A, pLichKing);
                        if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                            DoScriptText(SAY_LICH_KING_AGGRO_H, pLichKing);
                        pLichKing->AI()->AttackStart(me);
                        me->AI()->AttackStart(pLichKing);
                    }
                    JumpNextStep(3000);
                    break; 
                case 2:
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                    {
                        Fight = false;
                        if(pLichKing)
                            me->GetMotionMaster()->MovePoint(0, (me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ());
                        JumpNextStep(3000);
                    }
                    else
                        JumpNextStep(100);
                    break;
                case 3:
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        Fight = true;
                    JumpNextStep(100);
                    break;
                case 4:
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                    {
                        if(pLichKing)
                            me->CastSpell(pLichKing, SPELL_SYLVANA_STEP, false);
                        JumpNextStep(3000);
                    }
                    else
                        JumpNextStep(100);
                    break;
                case 5:
                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                    {
                        Fight = false;
                        if(pLichKing)
                            me->GetMotionMaster()->MovePoint(0, (me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ());
                        JumpNextStep(3000);
                    }
                    else
                        JumpNextStep(12000);
                    break;
                case 6:
                    Fight = true;

                    if(pLichKing)
                        me->CastSpell(pLichKing, me->GetEntry() == NPC_JAINA_OUTRO ? SPELL_ICE_PRISON : SPELL_DARK_ARROW,true);

                    JumpNextStep(2500);
                    break; 
                case 7:
                    if(pLichKing && !pLichKing->HasAura(SPELL_ICE_PRISON))
                        pLichKing->CastSpell(pLichKing,me->GetEntry() == NPC_JAINA_OUTRO ? SPELL_ICE_PRISON : SPELL_DARK_ARROW,true);

                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                    me->AttackStop();

                    if(me->GetEntry() == NPC_JAINA_OUTRO)
                    {
                        me->RemoveAurasDueToSpell(SPELL_ICE_BARRIER);
                        DoScriptText(SAY_JAINA_AGGRO, me);
                    }

                    if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                        DoScriptText(SAY_SYLVANA_AGGRO, me);

                    JumpNextStep(3000);
                    break; 
                case 8:
                    me->GetMotionMaster()->MovePoint(0, 5577.187f, 2236.003f, 733.012f);
                    if(pLichKing)
                        me->SetUInt64Value(UNIT_FIELD_TARGET, pLichKing->GetGUID());
                    JumpNextStep(10000);
                    break; 
                case 9:
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    Count = 1;
                    JumpNextStep(100);
                    break; 
            }
        }

        void Outro()
        {
            switch(Step)
            {
                case 10:
                    me->CastSpell(me, SPELL_SHIELD_DISRUPTION,false);
                    me->RemoveAurasDueToSpell(SPELL_SILENCE);
                    JumpNextStep(6000);
                    break;
                case 11:
                    if(GameObject* pCave = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_CAVE)))
                        pCave->SetGoState(GO_STATE_READY);
                    me->CastSpell(me, SPELL_SHIELD_DISRUPTION,false);
                    me->GetMotionMaster()->MovePoint(0, 5258.911328f,1652.112f,784.295166f);
                    JumpNextStep(10000);
                    break;
                case 12:
                    m_pInstance->SetData(TYPE_LICH_KING, DONE);
                    JumpNextStep(10000);
                    break;
                case 13:
                    JumpNextStep(20000);
                    break;
                case 14:
                    me->GetMotionMaster()->MovePoint(0, 5240.66f, 1646.93f, 784.302f);
                    JumpNextStep(5000);
                    break;
                case 15:
                    me->SetOrientation(0.68f);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    QuestCompleate();
                    JumpNextStep(5000);
                    break;
            }
        }

        void QuestCompleate()
        {
            if(m_pInstance)
            {
                Map::PlayerList const &PlayerList = m_pInstance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    i->getSource()->KilledMonsterCredit(36955, 0);
                    i->getSource()->KilledMonsterCredit(37554, 0);
                }
            }
        }

        void UpdateEscortAI(const uint32 diff)
        {
            if(!m_pInstance || !Event)
                return;

            DoMeleeAttackIfReady();

            if(m_pInstance->GetData(TYPE_PHASE) == 4)
            {
                if(StepTimer < diff)
                    Intro();
                else
                    StepTimer -= diff;
            }

            if(m_pInstance->GetData(TYPE_LICH_KING) == SPECIAL
                && m_pInstance->GetData(TYPE_PHASE) != 6)       //End Cinematic
            {
                m_pInstance->SetData(TYPE_PHASE, 6);
                Step = 10;
            }

            if (m_pInstance->GetData(TYPE_PHASE) == 6)
            {
                if(StepTimer < diff)
                    Outro();
                else
                    StepTimer -= diff;
                return;
            }

            if(WallCast == true && CastTimer < diff)
            {
                if(me->GetEntry() == NPC_SYLVANA_OUTRO)
                {
                   if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
                        me->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_03, false);
                    CastTimer = 1000;
                }
            }
            else
                CastTimer -= diff;

            if (WallCast == true && HoldTimer < 10000 && ( m_pInstance->GetData(DATA_SUMMONS) == 0 || !me->isInCombat()))
            {
                WallCast = false;
                me->InterruptNonMeleeSpells(false);
                SetEscortPaused(false);
                if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiIceWallGUID))
                    pGate->SetGoState(GO_STATE_ACTIVE); 
                ++Count;
                switch(Count)
                {
                    case 2:
                        if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ICE_WALL_2)))
                        {
                            pGate->SetGoState(GO_STATE_READY);
                            if(pLichKing && pLichKing->isAlive())
                                DoScriptText(SAY_LICH_KING_WALL_02, pLichKing);
                            m_uiIceWallGUID = pGate->GetGUID();
                        }
                        break;
                    case 3:
                        if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ICE_WALL_3)))
                        {
                            pGate->SetGoState(GO_STATE_READY);
                            if(pLichKing && pLichKing->isAlive())
                                DoScriptText(SAY_LICH_KING_WALL_03, pLichKing);
                            m_uiIceWallGUID = pGate->GetGUID();
                        }
                        break;
                    case 4:
                        if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ICE_WALL_4)))
                        {
                            pGate->SetGoState(GO_STATE_READY);
                            if(pLichKing && pLichKing->isAlive())
                                DoScriptText(SAY_LICH_KING_WALL_04, pLichKing);
                            m_uiIceWallGUID = pGate->GetGUID();
                        }
                        break;
                    case 5:
                        if(pLichKing && pLichKing->isAlive())
                        {
                            pLichKing->RemoveAurasDueToSpell(SPELL_WINTER);
                            pLichKing->SetSpeed(MOVE_WALK, 2.5f, true);
                            Step = 0;
                        }
                        break;
                }
            }
            else
            {
                HoldTimer -= diff;
                if (HoldTimer <= 0)
                    HoldTimer = 0;
            }

            return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_jaina_and_sylvana_HRextroAI(pCreature);
    }
};

class npc_lich_king_hr : public CreatureScript
{
public:
    npc_lich_king_hr() : CreatureScript("npc_lich_king_hr") { }

    struct npc_lich_king_hrAI : public ScriptedAI
    {
        npc_lich_king_hrAI(Creature *pCreature) : ScriptedAI(pCreature)
        {
            Reset();
        }

        void Reset()
        {
        }

        void JustDied(Unit* pKiller)
        {
        }

        void AttackStart(Unit* who) 
        { 
            return;
        }

        void UpdateAI(const uint32 diff)
        {
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lich_king_hrAI(pCreature);
    }
};

enum GENERAL_EVENT
{
    SAY_AGGRO                    = -1594519,
    SAY_DEATH                    = -1594520,

    SPELL_SHIELD_THROWN          = 69222,
    SPELL_SPIKE                  = 59446   // this is not right spell!
};

class npc_frostworn_general : public CreatureScript
{
public:
    npc_frostworn_general() : CreatureScript("npc_frostworn_general") { }

    struct npc_frostworn_generalAI : public ScriptedAI
    {
        npc_frostworn_generalAI(Creature *pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* m_pInstance;

        uint32 m_uiShieldTimer;
        uint32 m_uiSpikeTimer;

        void Reset()
        {
            if (!m_pInstance)
                return;
            m_uiShieldTimer = 5000;
            m_uiSpikeTimer = 14000;
            m_pInstance->SetData(TYPE_FROST_GENERAL, NOT_STARTED);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void JustDied(Unit* pKiller)
        {
            if (!m_pInstance)
                return;
            DoScriptText(SAY_DEATH, me);
            m_pInstance->SetData(TYPE_FROST_GENERAL, DONE);
        }

        void MoveInLineOfSight(Unit* pWho) 
        {
            if (!m_pInstance)
                return;

            if (me->getVictim())
                return;

            if (pWho->GetTypeId() != TYPEID_PLAYER
                || m_pInstance->GetData(TYPE_MARWYN) != DONE
                || !me->IsWithinDistInMap(pWho, 20.0f)
                ) return;

            if (Player* pPlayer = (Player*)pWho)
                if (pPlayer->isGameMaster())
                    return;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            AttackStart(pWho);
        }

        void EnterCombat(Unit* pVictim)
        {
            if (!m_pInstance)
                return;
            DoScriptText(SAY_AGGRO, me);
            m_pInstance->SetData(TYPE_FROST_GENERAL, IN_PROGRESS);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if(!UpdateVictim())
                return;

            if(m_uiShieldTimer < uiDiff)
            {
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                   DoCast(pTarget,SPELL_SHIELD_THROWN);
                m_uiShieldTimer = urand(8000, 12000);
            }
            else
                m_uiShieldTimer -= uiDiff;

            if (m_uiSpikeTimer < uiDiff) 
            {
                if(Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_SPIKE);
                m_uiSpikeTimer = urand(15000, 20000);
            }
            else
                m_uiSpikeTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_frostworn_generalAI(pCreature);
    }
};


void AddSC_halls_of_reflection()
{
    new npc_jaina_and_sylvana_HRintro();
    new npc_jaina_and_sylvana_HRextro();
    new npc_lich_king_hr();
    new npc_frostworn_general();
}
