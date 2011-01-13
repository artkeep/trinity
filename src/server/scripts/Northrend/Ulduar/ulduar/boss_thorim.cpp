/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
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
SDName: Thorim
SDAuthor: PrinceCreed
SD%Complete: 90
SDComments: Lightning Charge not works
EndScriptData */

#include "ScriptPCH.h"
#include "ulduar.h"
#include "ScriptedEscortAI.h"

// Thorim Spells
enum Spells
{
    SPELL_SHEAT_OF_LIGHTNING                    = 62276,
    SPELL_STORMHAMMER                           = 62042,
    SPELL_DEAFENING_THUNDER                     = 62470,
    SPELL_CHARGE_ORB                            = 62016,
    SPELL_SUMMON_LIGHTNING_ORB                  = 62391,
    SPELL_LIGHTNING_DESTRUCTION                 = 62393,
    SPELL_TOUCH_OF_DOMINION                     = 62565,
    SPELL_CHAIN_LIGHTNING_10                    = 62131,
    SPELL_CHAIN_LIGHTNING_25                    = 64390,
    SPELL_LIGHTNING_CHARGE                      = 62279,
    SPELL_LIGHTNING_RELEASE                     = 62466,
    SPELL_UNBALANCING_STRIKE                    = 62130,
    SPELL_BERSERK                               = 62560,
    // Sif
    SPELL_FROSTBOLT_VOLLEY            = 62580,
    SPELL_FROSTBOLT_VOLLEY_H        = 62604,
    SPELL_FROST_NOVA                = 62597,
    SPELL_FROST_NOVA_H                = 62605,
    SPELL_BLIZZARD                    = 62577
};

enum ThorimChests
{
    CACHE_OF_STORMS_10                          = 194312,
    CACHE_OF_STORMS_HARDMODE_10                 = 194313,
    CACHE_OF_STORMS_25                          = 194314,
    CACHE_OF_STORMS_HARDMODE_25                 = 194315
};

enum Phases
{
    PHASE_NULL = 0,
    PHASE_1,
    PHASE_2
};

enum Events
{
    EVENT_NONE,
    EVENT_STORMHAMMER,
    EVENT_CHARGE_ORB,
    EVENT_SUMMON_ADDS,
    EVENT_SUMMON_ORB,
    EVENT_CHECK,
    EVENT_BERSERK,
    EVENT_UNBALANCING_STRIKE,
    EVENT_CHAIN_LIGHTNING,
    EVENT_TRANSFER_ENERGY,
    EVENT_RELEASE_ENERGY
};

enum Yells
{
    SAY_AGGRO_1                                 = -1603270,
    SAY_AGGRO_2                                 = -1603271,
    SAY_SPECIAL_1                               = -1603272,
    SAY_SPECIAL_2                               = -1603273,
    SAY_SPECIAL_3                               = -1603274,
    SAY_JUMPDOWN                                = -1603275,
    SAY_SLAY_1                                  = -1603276,
    SAY_SLAY_2                                  = -1603277,
    SAY_BERSERK                                 = -1603278,
    SAY_WIPE                                    = -1603279,
    SAY_DEATH                                   = -1603280,
    SAY_END_NORMAL_1                            = -1603281,
    SAY_END_NORMAL_2                            = -1603282,
    SAY_END_NORMAL_3                            = -1603283,
    SAY_END_HARD_1                              = -1603284,
    SAY_END_HARD_2                              = -1603285,
    SAY_END_HARD_3                              = -1603286,
    SAY_YS_HELP                                 = -1603287
};

#define EMOTE_BARRIER      "Runic Colossus surrounds itself with a crackling Runic Barrier!"
#define EMOTE_MIGHT        "Ancient Rune Giant fortifies nearby allies with runic might!"

// Thorim Pre-Phase Adds
enum PreAdds
{
    BEHEMOTH,
    MERCENARY_CAPTAIN,
    MERCENARY_SOLDIER,
    DARK_RUNE_ACOLYTE
};

const uint32 PRE_PHASE_ADD[]          = {32882, 32908, 32885, 33110};
#define SPELL_PRE_PRIMARY(i)            RAID_MODE(SPELL_PRE_PRIMARY_N[i],SPELL_PRE_PRIMARY_H[i])
const uint32 SPELL_PRE_PRIMARY_N[]    = {62315, 62317, 62318, 62333};
const uint32 SPELL_PRE_PRIMARY_H[]    = {62415, 62317, 62318, 62441};
#define SPELL_PRE_SECONDARY(i)          RAID_MODE(SPELL_PRE_SECONDARY_N[i],SPELL_PRE_SECONDARY_H[i])
const uint32 SPELL_PRE_SECONDARY_N[]  = {62316, 62444, 16496, 62334};
const uint32 SPELL_PRE_SECONDARY_H[]  = {62417, 62444, 16496, 62442};
#define SPELL_HOLY_SMITE                RAID_MODE(62335, 62443)

#define INCREASE_PREADDS_COUNT                     1
#define ACTION_CHANGE_PHASE                        2

// Achievements
#define ACHIEVEMENT_SIFFED              RAID_MODE(2977, 2978)
#define ACHIEVEMENT_LOSE_ILLUSION       RAID_MODE(3176, 3183)
#define ACHIEVEMENT_WHO_NEEDS_BLOODLUST RAID_MODE(2975, 2976)

// Thorim Arena Phase Adds
enum ArenaAdds
{
    DARK_RUNE_CHAMPION,
    DARK_RUNE_COMMONER,
    DARK_RUNE_EVOKER,
    DARK_RUNE_WARBRINGER,
    IRON_RING_GUARD,
    IRON_HONOR_GUARD
};

const uint32 ARENA_PHASE_ADD[]          = {32876, 32904, 32878, 32877, 32874, 32875};
#define SPELL_ARENA_PRIMARY(i)            RAID_MODE(SPELL_ARENA_PRIMARY_N[i],SPELL_ARENA_PRIMARY_H[i])
const uint32 SPELL_ARENA_PRIMARY_N[]    = {35054, 62326, 62327, 62322, 64151, 42724};
const uint32 SPELL_ARENA_PRIMARY_H[]    = {35054, 62326, 62445, 62322, 64151, 42724};
#define SPELL_ARENA_SECONDARY(i)          RAID_MODE(SPELL_ARENA_SECONDARY_N[i],SPELL_ARENA_SECONDARY_H[i])
const uint32 SPELL_ARENA_SECONDARY_N[]  = {15578, 38313, 62321, 0, 62331, 62332};
const uint32 SPELL_ARENA_SECONDARY_H[]  = {15578, 38313, 62529, 0, 62418, 62420};
#define SPELL_AURA_OF_CELERITY            62320
#define SPELL_CHARGE                      32323
#define SPELL_RUNIC_MENDING               RAID_MODE(62328, 62446)

// Runic Colossus (Mini Boss) Spells
enum RunicSpells
{
    SPELL_SMASH                                 = 62339,
    SPELL_RUNIC_BARRIER                         = 62338,
    SPELL_CHARGE_10                             = 62613,
    SPELL_CHARGE_25                             = 62614
};

// Ancient Rune Giant (Mini Boss) Spells
enum AncientSpells
{
    SPELL_RUNIC_FORTIFICATION                   = 62942,
    SPELL_RUNE_DETONATION                       = 62526,
    SPELL_STOMP_10                              = 62411,
    SPELL_STOMP_25                              = 62413
};

#define NPC_SIF                                33196

const Position Pos[7] =
{
    {2095.53f, -279.48f, 419.84f, 0.504f},
    {2092.93f, -252.96f, 419.84f, 6.024f},
    {2098.66f, -240.79f, 419.84f, 5.715f},
    {2113.37f, -225.85f, 419.84f, 5.221f},
    {2156.54f, -225.99f, 419.84f, 4.441f},
    {2155.10f, -254.45f, 419.84f, 3.760f},
    {2153.22f, -273.43f, 419.84f, 2.761f}
};

const Position PosCharge[7] =
{
    {2104.99f, -233.484f, 419.573f, 5.49779f},
    {2092.64f, -262.594f, 419.573f, 6.26573f},
    {2104.76f, -292.719f, 419.573f, 0.78539f},
    {2164.97f, -293.375f, 419.573f, 2.35619f},
    {2164.58f, -233.333f, 419.573f, 3.90954f},
    {2145.81f, -222.196f, 419.573f, 4.45059f},
    {2123.91f, -222.443f, 419.573f, 4.97419f}
};

class boss_thorim : public CreatureScript
{
public:
    boss_thorim() : CreatureScript("boss_thorim") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_thorimAI(pCreature);
    }

    struct boss_thorimAI : public BossAI
    {
        boss_thorimAI(Creature* pCreature) : BossAI(pCreature, TYPE_THORIM)
            , phase(PHASE_NULL), lSummons(me)
        {
            FirstTime = true;
        }

        SummonList lSummons;
        Phases phase;
        int32 PreAddsCount;
        uint8 spawnedAdds;
        uint32 EncounterTime;
        bool FirstTime;
        bool bIsHardmode;
        Creature *EnergySource;
        uint64 orbGUID[7] ;

        void Reset()
        {
            _Reset();
            lSummons.DespawnAll();
            if (!FirstTime)
                DoScriptText(SAY_WIPE, me);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
            FirstTime = true;
            PreAddsCount = 0;
            spawnedAdds = 0;
            bIsHardmode = false;
            // Spawn Pre-Phase Adds
            me->SummonCreature(PRE_PHASE_ADD[0], 2134.79f, -263.03f, 419.84f, 5.377f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            me->SummonCreature(PRE_PHASE_ADD[1], 2141.60f, -271.64f, 419.84f, 2.188f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            me->SummonCreature(PRE_PHASE_ADD[2], 2127.24f, -251.31f, 419.84f, 5.89f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            me->SummonCreature(PRE_PHASE_ADD[2], 2123.32f, -254.77f, 419.84f, 6.17f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            me->SummonCreature(PRE_PHASE_ADD[2], 2120.10f, -258.99f, 419.84f, 6.25f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            me->SummonCreature(PRE_PHASE_ADD[3], 2130.28f, -274.60f, 419.84f, 1.22f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
        }

        void KilledUnit(Unit * victim)
        {
            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2), me);
        }

        void JustDied(Unit * victim)
        {
            _JustDied();
            lSummons.DespawnAll();
            DoScriptText(SAY_DEATH, me);
            if (instance)
            {
                if (bIsHardmode)
                {
                    instance->DoCompleteAchievement(ACHIEVEMENT_LOSE_ILLUSION);
                    me->SummonGameObject(RAID_MODE(CACHE_OF_STORMS_HARDMODE_10, CACHE_OF_STORMS_HARDMODE_25), 2134.58f, -286.908f, 419.495f, 1.55988f, 0, 0, 0.7f, 0.7f, 604800);
                }
                else
                {
                    me->SummonGameObject(RAID_MODE(CACHE_OF_STORMS_10, CACHE_OF_STORMS_25), 2134.58f, -286.908f, 419.495f, 1.55988f, 0, 0, 0.7f, 0.7f, 604800);
                }

                Map* pMap = me->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    AchievementEntry const *AchievWhoNeedsBlood = GetAchievementStore()->LookupEntry(ACHIEVEMENT_WHO_NEEDS_BLOODLUST);
                    if (AchievWhoNeedsBlood)
                    {
                        Map::PlayerList const &players = pMap->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        {
                            if (itr->getSource() && itr->getSource()->isAlive() && itr->getSource()->HasAura(SPELL_AURA_OF_CELERITY) && !itr->getSource()->isGameMaster())
                            {
                                itr->getSource()->CompletedAchievement(AchievWhoNeedsBlood);
                            }
                        }
                    }
                }
            }
        }

        void JustSummoned(Creature *summon)
        {
            lSummons.Summon(summon);
            switch(summon->GetEntry())
            {
            case 32876:
            case 32904:
            case 32878:
            case 32877:
            case 32874:
            case 32875:
                if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST))
                {
                    summon->SetInCombatWith(target);
                    target->SetInCombatWith(summon);
                    summon->AddThreat(target, 0.0f);
                }
                summon->AI()->DoZoneInCombat();
                break ;
            case 33138:
                CAST_AI(npc_escortAI, (summon->AI()))->Start(true, false);
                break;
            }
        }

        void EnterCombat(Unit* pWho)
        {
            DoScriptText(RAND(SAY_AGGRO_1,SAY_AGGRO_2), me);
            _EnterCombat();

            EncounterTime = 0;
            phase = PHASE_1;
            events.SetPhase(PHASE_1);
            DoCast(me, SPELL_SHEAT_OF_LIGHTNING);
            events.ScheduleEvent(EVENT_STORMHAMMER, 40000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_CHARGE_ORB, 30000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_SUMMON_ADDS, 20000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_BERSERK, 300000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_SUMMON_ORB, 300000, 0, PHASE_1);
            events.ScheduleEvent(EVENT_CHECK, 1000, 0, PHASE_1);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);
            
            EncounterTime += diff;

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
                
            if (phase == PHASE_1)
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_CHECK:
                        {
                            bool bCanSummon = true;
                            if (instance)
                            {
                                Unit *pTarget = NULL;
                                std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
                                for (std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                                {
                                    pTarget = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                                    if (!pTarget)
                                        continue;
                                    // exclude pets & totems
                                    if (pTarget->GetTypeId() != TYPEID_PLAYER)
                                        continue;
                                    //18 yard radius minimum
                                    if (me->isInFrontInMap(pTarget, 30.0f))
                                    {
                                        bCanSummon = false;
                                        break;
                                    }
                                    pTarget = NULL;
                                }
                                if (bCanSummon)
                                {
                                    DoCast(SPELL_SUMMON_LIGHTNING_ORB);
                                    events.CancelEvent(EVENT_CHECK);
                                }

                            }
                            if (!bCanSummon)
                                events.ScheduleEvent(EVENT_CHECK, 1000, 0, PHASE_1);
                            break;
                        }
                        case EVENT_STORMHAMMER:
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_NEAREST, 0, 80, true))
                                DoCast(pTarget, SPELL_STORMHAMMER);
                            events.ScheduleEvent(EVENT_STORMHAMMER, urand(15000, 20000), 0, PHASE_1);
                            break;
                        case EVENT_CHARGE_ORB:
                            DoCastAOE(SPELL_CHARGE_ORB);
                            events.ScheduleEvent(EVENT_CHARGE_ORB, urand(15000, 20000), 0, PHASE_1);
                            break;
                        case EVENT_SUMMON_ADDS:
                            spawnAdd();
                            events.ScheduleEvent(EVENT_SUMMON_ADDS, 10000, 0, PHASE_1);
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK);
                            DoScriptText(SAY_BERSERK, me);
                            events.CancelEvent(EVENT_BERSERK);
                            break;
                        case EVENT_SUMMON_ORB:
                            DoCast(SPELL_SUMMON_LIGHTNING_ORB);
                            events.CancelEvent(EVENT_SUMMON_ORB);
                            break;
                    }
                }
            }
            else
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_UNBALANCING_STRIKE:
                            DoCastVictim(SPELL_UNBALANCING_STRIKE);
                            events.ScheduleEvent(EVENT_UNBALANCING_STRIKE, urand(15000, 20000), 0, PHASE_2);
                            break;
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                                if (pTarget->isAlive())
                                    DoCast(pTarget, RAID_MODE(SPELL_CHAIN_LIGHTNING_10, SPELL_CHAIN_LIGHTNING_25));
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(15000, 20000), 0, PHASE_2);
                            break;
                        /*case EVENT_TRANSFER_ENERGY:
                            //DoScriptText(SAY_SUMMON, me);
                            me->SummonCreature(32892, PosCharge[rand()%7], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            events.ScheduleEvent(EVENT_TRANSFER_ENERGY, 20000, 0, PHASE_2);
                            break;*/
                        case EVENT_RELEASE_ENERGY:
                            DoCast(me, SPELL_LIGHTNING_CHARGE);
                            //DoCast(EnergySource, SPELL_LIGHTNING_RELEASE);
                            events.ScheduleEvent(EVENT_RELEASE_ENERGY, 10000, 0, PHASE_2);
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK);
                            DoScriptText(SAY_BERSERK, me);
                            events.CancelEvent(EVENT_BERSERK);
                            break;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }
        
        void DoAction(const int32 action)
        {
            switch (action)
            {
                case INCREASE_PREADDS_COUNT:
                    ++PreAddsCount;
                    break;
                case ACTION_CHANGE_PHASE:
                    if (phase != PHASE_2)
                    {
                        DoScriptText(SAY_JUMPDOWN, me);
                        phase = PHASE_2;
                        events.SetPhase(PHASE_2);
                        me->RemoveAurasDueToSpell(SPELL_SHEAT_OF_LIGHTNING);
                        if (EncounterTime >= 180000)
                        {
                            me->AddAura(SPELL_TOUCH_OF_DOMINION, me);
                            bIsHardmode = false;
                        }
                        else
                        {
                            instance->DoCompleteAchievement(ACHIEVEMENT_SIFFED);
                            me->SummonCreature(NPC_SIF, 2149.27f, -260.55f, 419.69f, 2.527f) ;
                            bIsHardmode = true;
                        }
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
                        me->GetMotionMaster()->MoveJump(2134.79f, -263.03f, 419.84f, 10.0f, 20.0f);
                        me->SetInCombatWithZone();
                        events.ScheduleEvent(EVENT_UNBALANCING_STRIKE, 8000, 0, PHASE_2);
                        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 15000, 0, PHASE_2);
                        //events.ScheduleEvent(EVENT_TRANSFER_ENERGY, 20000, 0, PHASE_2);
                        events.ScheduleEvent(EVENT_RELEASE_ENERGY, 10000, 0, PHASE_2);
                        events.ScheduleEvent(EVENT_BERSERK, 300000, 0, PHASE_2);
                    }
                    break;
            }
            
            if (PreAddsCount >= 6)
            {
                // Event starts
                DoZoneInCombat();
                if(instance)
                    instance->SetData(TYPE_PORTCULLIS, GO_STATE_ACTIVE);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (id == 0)
                me->GetMotionMaster()->MoveIdle();
        }
        
        void spawnAdd()
        {
            switch(spawnedAdds)
            {
                case 0:
                    me->SummonCreature(ARENA_PHASE_ADD[0], Pos[rand()%7], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                    me->SummonCreature(ARENA_PHASE_ADD[2], Pos[rand()%7], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                    me->SummonCreature(ARENA_PHASE_ADD[3], Pos[rand()%7], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                    break;
                case 1:
                    for(uint8 n = 0; n < 7; n++)
                        me->SummonCreature(ARENA_PHASE_ADD[1], Pos[n], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                    break;
            }

            spawnedAdds++;
            if(spawnedAdds > 1)
            {
                spawnedAdds = 0;
            }
        }
    };

};

// Pre-Phase Adds
class mob_pre_phase : public CreatureScript
{
public:
    mob_pre_phase() : CreatureScript("mob_pre_phase") { }

    struct mob_pre_phaseAI : public ScriptedAI
    {
        mob_pre_phaseAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
            id = PreAdds(0);
            for (uint8 i = 0; i < 4; ++i)
                if (me->GetEntry() == PRE_PHASE_ADD[i])
                    id = PreAdds(i);
        }

        PreAdds id;
        InstanceScript* instance;
        uint32 PrimaryTimer;
        uint32 SecondaryTimer;

        void Reset()
        {
            PrimaryTimer = urand(3000, 6000);
            SecondaryTimer = urand (12000, 15000);
        }
        
        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;
                
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
                
            if (PrimaryTimer <= uiDiff)
            {
                DoCast(SPELL_PRE_PRIMARY(id));
                PrimaryTimer = urand(15000, 20000);
            }
            else PrimaryTimer -= uiDiff;
            
            if (SecondaryTimer <= uiDiff)
            {
                DoCast(SPELL_PRE_SECONDARY(id));
                SecondaryTimer = urand(4000, 8000);
            }
            else SecondaryTimer -= uiDiff;

            if (id == DARK_RUNE_ACOLYTE)
                DoSpellAttackIfReady(SPELL_HOLY_SMITE);
            else
                DoMeleeAttackIfReady();
        }
        
        void JustDied(Unit *victim)
        {
            if (Creature* pThorim = Unit::GetCreature(*me, instance->GetData64(DATA_THORIM)))
                if (pThorim->AI())
                    pThorim->AI()->DoAction(INCREASE_PREADDS_COUNT);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_pre_phaseAI(pCreature);
    }
};

// Arena Phase Adds
class mob_arena_phase : public CreatureScript
{
public:
    mob_arena_phase() : CreatureScript("mob_arena_phase") { }

    struct mob_arena_phaseAI : public ScriptedAI
    {
        mob_arena_phaseAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();        
            id = ArenaAdds(0);
            for (uint8 i = 0; i < 6; ++i)
                if (me->GetEntry() == ARENA_PHASE_ADD[i])
                    id = ArenaAdds(i);
        }

        ArenaAdds id;
        InstanceScript* instance;
        uint32 PrimaryTimer;
        uint32 SecondaryTimer;
        uint32 ChargeTimer;

        void Reset()
        {
            PrimaryTimer = urand(3000, 6000);
            SecondaryTimer = urand (7000, 9000);
            ChargeTimer = 8000;
        }
        
        void EnterCombat(Unit* pWho)
        {
            if (id == DARK_RUNE_WARBRINGER)
                DoCast(me, SPELL_AURA_OF_CELERITY);
                
            DoZoneInCombat();

            if (Unit* pTarget = SelectTarget(SELECT_TARGET_NEAREST))
                me->AI()->AttackStart(pTarget);
        }
        
        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;
                
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
                
            if (PrimaryTimer <= uiDiff)
            {
                DoCast(SPELL_ARENA_PRIMARY(id));
                PrimaryTimer = urand(3000, 6000);
            }
            else PrimaryTimer -= uiDiff;
            
            if (SecondaryTimer <= uiDiff)
            {
                DoCast(SPELL_ARENA_SECONDARY(id));
                SecondaryTimer = urand(12000, 16000);
            }
            else SecondaryTimer -= uiDiff;
            
            if (ChargeTimer <= uiDiff)
            {
                if (id == DARK_RUNE_CHAMPION)
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                        DoCast(pTarget, SPELL_CHARGE);
                ChargeTimer = 12000;
            }
            else ChargeTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_arena_phaseAI(pCreature);
    }
};

// Runic Colossus (Mini Boss)
class mob_runic_colossus : public CreatureScript
{
public:
    mob_runic_colossus() : CreatureScript("mob_runic_colossus") { }

    struct mob_runic_colossusAI : public ScriptedAI
    {
        mob_runic_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
            
        uint32 BarrierTimer;
        uint32 SmashTimer;
        uint32 ChargeTimer;

        void Reset()
        {
            BarrierTimer = urand(12000, 15000);
            SmashTimer = urand (15000, 18000);
            ChargeTimer = urand (20000, 24000);
            
            me->GetMotionMaster()->MoveTargetedHome();
            // Close Runed Door
        }
        
        void UpdateAI(const uint32 uiDiff)
        {
            // I cannot find the real spell
            if (!me->IsWithinMeleeRange(me->getVictim()))
                DoCast(me, SPELL_SMASH);
        
            if (!UpdateVictim())
                return;
                
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
                
            if (BarrierTimer <= uiDiff)
            {
                me->MonsterTextEmote(EMOTE_MIGHT, 0, true);
                DoCast(me, SPELL_RUNIC_BARRIER);
                BarrierTimer = urand(35000, 45000);
            }
            else BarrierTimer -= uiDiff;
            
            if (SmashTimer <= uiDiff)
            {
                DoCast(me, SPELL_SMASH);
                SmashTimer = urand (15000, 18000);
            }
            else SmashTimer -= uiDiff;
            
            if (ChargeTimer <= uiDiff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 8, true))
                    DoCast(pTarget, RAID_MODE(SPELL_CHARGE_10, SPELL_CHARGE_25));
                ChargeTimer = 20000;
            }
            else ChargeTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        
        void JustDied(Unit *victim)
        {
            // Open Runed Door
            if (instance)
                instance->SetData(TYPE_RUNIC_DOOR, GO_STATE_ACTIVE);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_runic_colossusAI(pCreature);
    }
};

// Ancient Rune Giant (Mini Boss)
class mob_rune_giant : public CreatureScript
{
public:
    mob_rune_giant() : CreatureScript("mob_rune_giant") { }

    struct mob_rune_giantAI : public ScriptedAI
    {
        mob_rune_giantAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
            
        uint32 StompTimer;
        uint32 DetonationTimer;

        void Reset()
        {
            StompTimer = urand(10000, 12000);
            DetonationTimer = 25000;
            
            me->GetMotionMaster()->MoveTargetedHome();
            // Close Stone Door
            if (instance)
                instance->SetData(TYPE_STONE_DOOR, GO_STATE_READY);
        }
        
        void EnterCombat(Unit* pWho)
        {
            me->MonsterTextEmote(EMOTE_MIGHT, 0, true);
            DoCast(me, SPELL_RUNIC_FORTIFICATION); // need core support
        }
        
        void UpdateAI(const uint32 uiDiff)
        {    
            if (!UpdateVictim())
                return;
                
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
                
            if (StompTimer <= uiDiff)
            {
                DoCast(me, RAID_MODE(SPELL_STOMP_10, SPELL_STOMP_25));
                StompTimer = urand(10000, 12000);
            }
            else StompTimer -= uiDiff;
            
            if (DetonationTimer <= uiDiff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                    DoCast(pTarget, SPELL_RUNE_DETONATION);
                DetonationTimer = urand(10000, 12000);
            }
            else DetonationTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        
        void JustDied(Unit *victim)
        {
            // Open Stone Door
            if (instance)
                instance->SetData(TYPE_STONE_DOOR, GO_STATE_ACTIVE);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_rune_giantAI(pCreature);
    }
};

class AreaTrigger_at_thorim_phase_trigger : public AreaTriggerScript
{
    public:
        AreaTrigger_at_thorim_phase_trigger() : AreaTriggerScript("at_thorim_phase_trigger"){}

    bool OnTrigger(Player* pPlayer, AreaTriggerEntry const* trigger)
    {
        if (!pPlayer->isGameMaster())
        {
            std::list<Creature*> ThorimList;
            pPlayer->GetCreatureListWithEntryInGrid(ThorimList, NPC_THORIM, 100.0f);
            if (!ThorimList.empty())
                for (std::list<Creature*>::const_iterator itr = ThorimList.begin(); itr != ThorimList.end(); ++itr)
                    if (Creature* pThorim = *itr)
                        if (pThorim->AI())
                            pThorim->AI()->DoAction(ACTION_CHANGE_PHASE);
        }
        return true;
    }
};

class npc_thorim_lightning_orb : public CreatureScript
{
public:
    npc_thorim_lightning_orb() : CreatureScript("npc_thorim_lightning_orb") { }

    struct npc_thorim_lightning_orbAI : public npc_escortAI
    {
        npc_thorim_lightning_orbAI(Creature* pCreature) : npc_escortAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->SetReactState(REACT_PASSIVE);
            AddWaypoint(1, 2132.67f, -439.24f, 438.24f);
            AddWaypoint(2, 2167.96f, -440.52f, 438.24f);
            AddWaypoint(3, 2198.93f, -433.88f, 419.95f);
            AddWaypoint(4, 2212.62f, -433.50f, 413.11f);
            AddWaypoint(5, 2227.28f, -416.90f, 412.13f);
            AddWaypoint(6, 2227.28f, -262.93f, 412.17f);
            AddWaypoint(7, 2183.47f, -263.29f, 414.73f);
            AddWaypoint(8, 2169.61f, -263.45f, 419.29f);
            AddWaypoint(9, 2134.68f, -262.86f, 419.84f);
            Start(true, true);
        }

        void Reset()
        {
            SetDespawnAtFar(false);
            DoCast(SPELL_LIGHTNING_DESTRUCTION);
        }

        void WaypointReached(uint32 i)
        {
            switch(i)
            {
                case 9:
                    me->DespawnOrUnsummon();
                    break;
            }
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_thorim_lightning_orbAI(creature);
    }
};

class npc_sif : public CreatureScript
{
public:
    npc_sif() : CreatureScript("npc_sif") { }

    struct npc_sifAI : public ScriptedAI
    {
        npc_sifAI(Creature *c) : ScriptedAI(c) 
        {
        }

        uint32 frostboltVolleyTimer ;
        uint32 frostNovaTimer ;
        uint32 blizzardTimer ;

        void Reset()
        {
            frostboltVolleyTimer = urand(3000, 6000) ;
            frostNovaTimer = urand(15000, 18000) ;
            blizzardTimer = urand(10000, 13000) ;
        }

        void EnterCombat(Unit* who)
        {
        }
        void KilledUnit(Unit* victim)
        {
        }

        void JustDied(Unit *victim)
        {
        }

        void MoveInLineOfSight(Unit* who) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (frostboltVolleyTimer <= diff)
            {
                DoCast(RAID_MODE(SPELL_FROSTBOLT_VOLLEY, SPELL_FROSTBOLT_VOLLEY_H));
                frostboltVolleyTimer = urand(20000, 25000) ;
            } else frostboltVolleyTimer -= diff ;

            if (frostNovaTimer <= diff)
            {
                DoCast(RAID_MODE(SPELL_FROST_NOVA, SPELL_FROST_NOVA_H));
                frostNovaTimer = urand(15000, 18000) ;
            } else frostNovaTimer -= diff ;

            if (blizzardTimer <= diff)
            {
                DoCast(SPELL_BLIZZARD);
                blizzardTimer = urand(60000, 70000) ;
            } else blizzardTimer -= diff ;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_sifAI(pCreature);
    }
};

void AddSC_boss_thorim()
{
    new boss_thorim;
    new mob_pre_phase;
    new mob_arena_phase;
    new mob_runic_colossus;
    new mob_rune_giant;
    new npc_sif;
    new npc_thorim_lightning_orb;
    new AreaTrigger_at_thorim_phase_trigger;
}
