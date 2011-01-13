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
SDName: Auriaya
Author: PrinceCreed
SD%Complete: 95%
SDComment: TODO: Savage Pounce not works properly
EndScriptData */

#include "ScriptPCH.h"
#include "ulduar.h"

// Auriaya
#define SPELL_SETINEL_BLAST                     RAID_MODE(64389, 64678)
#define SPELL_SONIC_SCREECH                     RAID_MODE(64422, 64688)
#define SPELL_TERRIFYING_SCREECH                64386
#define SPELL_SUMMON_SWARMING_GUARDIAN          64396
#define SPELL_ACTIVATE_DEFENDER                 64449
#define SPELL_DEFENDER_TRIGGER                  64448
#define SPELL_SUMMON_DEFENDER                   64447
#define SPELL_BERSERK                           47008

// Feral Defender
#define SPELL_FERAL_RUSH                        RAID_MODE(64496, 64674)
#define SPELL_FERAL_POUNCE                      RAID_MODE(64478, 64669)
#define SPELL_SEEPING_ESSENCE                   RAID_MODE(64458, 64676)
#define SPELL_SUMMON_ESSENCE                    64457
#define SPELL_FERAL_ESSENCE                     64455

// Sanctum Sentry
#define SPELL_SAVAGE_POUNCE                     RAID_MODE(64666, 64374)
#define SPELL_RIP_FLESH                         RAID_MODE(64375, 64667)
#define SPELL_STRENGHT_PACK                     64381
#define SANCTUM_SENTRY                          34014

// Achievements
#define ACHIEVEMENT_CRAZY_CAT_LADY              RAID_MODE(3006, 3007)
#define ACHIEVEMENT_NINE_LIVES                  RAID_MODE(3076, 3077)

enum Events
{
    EVENT_NONE,
    EVENT_SCREECH,
    EVENT_BLAST,
    EVENT_TERRIFYING,
    EVENT_SUMMON,
    EVENT_DEFENDER,
    EVENT_BERSERK
};

enum Yells
{
    SAY_AGGRO                                   = -1603050,
    SAY_SLAY_1                                  = -1603051,
    SAY_SLAY_2                                  = -1603052,
    SAY_DEATH                                   = -1603053,
    SAY_BERSERK                                 = -1603054,
};

#define EMOTE_FEAR        "Auriaya begins to cast Terrifying Screech."
#define EMOTE_DEFENDER    "Auriaya begins to activate the Feral Defender!"

enum Actions
{
    ACTION_CRAZY_CAT_LADY                       = 0,
    ACTION_NINE_LIVES                           = 1,
};

class boss_auriaya : public CreatureScript
{
    public:
        boss_auriaya(): CreatureScript("boss_auriaya") {}

    struct boss_auriaya_AI : public BossAI
    {
        boss_auriaya_AI(Creature *pCreature) : BossAI(pCreature, TYPE_AURIAYA)
        {
            pInstance = pCreature->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
        }
        
        InstanceScript* pInstance;
        
        Creature* Sentry[4];
        
        bool SentryDead;
        uint32 LivesCount;
        
        void Reset()
        {
            _Reset();
            
            Sentry[0] = me->SummonCreature(SANCTUM_SENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            Sentry[0]->GetMotionMaster()->MoveFollow(me, 0.5, 1.5, MOTION_SLOT_ACTIVE);
            Sentry[1] = me->SummonCreature(SANCTUM_SENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            Sentry[1]->GetMotionMaster()->MoveFollow(me, 0.5, -1.5, MOTION_SLOT_ACTIVE);
            if (getDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
            {
                Sentry[2] = me->SummonCreature(SANCTUM_SENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                Sentry[2]->GetMotionMaster()->MoveFollow(me, -2.5, 1.5, MOTION_SLOT_ACTIVE);
                Sentry[3] = me->SummonCreature(SANCTUM_SENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                Sentry[3]->GetMotionMaster()->MoveFollow(me, -2.5, -1.5, MOTION_SLOT_ACTIVE);
            }
        }

        void EnterCombat(Unit* who)
        {
            _EnterCombat();
            DoScriptText(SAY_AGGRO,me);
            SentryDead = false;
            LivesCount = 0;
            events.ScheduleEvent(EVENT_SCREECH, 60000);
            events.ScheduleEvent(EVENT_BLAST, 35500);
            events.ScheduleEvent(EVENT_TERRIFYING, 35000);
            events.ScheduleEvent(EVENT_DEFENDER, 65000);
            events.ScheduleEvent(EVENT_SUMMON, 120000);
            events.ScheduleEvent(EVENT_BERSERK, 600000);
        }
        void KilledUnit(Unit* victim)
        {
            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
        }

        void JustDied(Unit *victim)
        {
            _JustDied();
            DoScriptText(SAY_DEATH, me);
            
            // Achievements
            if (pInstance)
            {
                // Crazy Cat Lady
                if (!SentryDead)
                    pInstance->DoCompleteAchievement(ACHIEVEMENT_CRAZY_CAT_LADY);
                // Nine Lives
                if (LivesCount >= 9)
                    pInstance->DoCompleteAchievement(ACHIEVEMENT_NINE_LIVES);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_SCREECH:
                        DoCast(SPELL_SONIC_SCREECH);
                        events.ScheduleEvent(EVENT_SCREECH, 25000);
                        break;
                    case EVENT_TERRIFYING:
                        me->MonsterTextEmote(EMOTE_FEAR, 0, true);
                        DoCast(SPELL_TERRIFYING_SCREECH);
                        events.ScheduleEvent(EVENT_TERRIFYING, 35000);
                        break;
                    case EVENT_BLAST:
                        DoCastAOE(SPELL_SETINEL_BLAST);
                        events.ScheduleEvent(EVENT_BLAST, 35000);
                        break;
                    case EVENT_DEFENDER:
                        me->MonsterTextEmote(EMOTE_DEFENDER, 0, true);
                        DoCast(SPELL_DEFENDER_TRIGGER);
                        DoCast(SPELL_ACTIVATE_DEFENDER);
                        events.CancelEvent(EVENT_DEFENDER);
                        break;
                    case EVENT_SUMMON:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                            DoCast(pTarget, SPELL_SUMMON_SWARMING_GUARDIAN);
                        events.ScheduleEvent(EVENT_SUMMON, 35000);
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        DoScriptText(SAY_BERSERK, me);
                        events.CancelEvent(EVENT_BERSERK);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
        
        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_CRAZY_CAT_LADY:
                    SentryDead = true;
                    break;
                case ACTION_NINE_LIVES:
                    LivesCount++;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_auriaya_AI (pCreature);
    };
};

class feral_defender_trigger : public CreatureScript
{
    public:
        feral_defender_trigger(): CreatureScript("feral_defender_trigger") {}

    struct feral_defender_triggerAI : public ScriptedAI
    {
        feral_defender_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_PACIFIED);
        }

        InstanceScript* m_pInstance;
        int32 SummomTimer;

        void Reset()
        {
            SummomTimer = 3000;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (SummomTimer <= int32(uiDiff))
            {
                DoCast(me, SPELL_SUMMON_DEFENDER);
                SummomTimer = 30000;
            } 
            else SummomTimer -= uiDiff;
        }

        void JustSummoned(Creature *summon)
        {
            summon->AI()->DoAction(0);
            summon->AI()->DoZoneInCombat();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new feral_defender_triggerAI(pCreature);
    };
};

class mob_sanctum_sentry : public CreatureScript
{
    public:
        mob_sanctum_sentry(): CreatureScript("mob_sanctum_sentry") {}

    struct mob_sanctum_sentryAI : public ScriptedAI
    {
        mob_sanctum_sentryAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        int32 RipTimer;
        int32 PounceTimer;
        int32 CheckTimer;

        void Reset()
        {
            RipTimer = urand(4000, 8000);
            PounceTimer = urand(20000, 30000);
            CheckTimer = 1000;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (RipTimer <= int32(uiDiff))
            {
                DoCastVictim(SPELL_RIP_FLESH);
                RipTimer = urand(14000, 18000);
            } else RipTimer -= uiDiff;
            
            if (PounceTimer <= int32(uiDiff))
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                {
                    me->AddThreat(pTarget, 100.0f);
                    me->AI()->AttackStart(pTarget);
                    DoCast(pTarget, SPELL_SAVAGE_POUNCE);
                }
                PounceTimer = urand(20000, 30000);
            } else PounceTimer -= uiDiff;
            
            // Increases the damage of all Sanctum Sentries within 10 yards by 30%
            uint8 aura = -1;
            if (CheckTimer < int32(uiDiff))
            {
                std::list<Creature*> Sanctum;
                GetCreatureListWithEntryInGrid(Sanctum, me, SANCTUM_SENTRY, 100.0f);
                for(std::list<Creature*>::iterator itr = Sanctum.begin(); itr != Sanctum.end(); ++itr)
                {
                    if(Creature *c = *itr)
                    {
                        if (c->isAlive())
                            if (c->IsWithinDistInMap(me, 10))
                                aura++;
                    }
                }
                me->RemoveAurasDueToSpell(SPELL_STRENGHT_PACK);
                while(aura !=0)
                {
                    me->AddAura(SPELL_STRENGHT_PACK, me);
                    aura=aura-1;
                }
                CheckTimer = 2000;
            } else CheckTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        
        void JustDied(Unit* victim)
        {
            if (pInstance)
                if (Creature *pAuriaya = me->GetCreature(*me, pInstance->GetData64(DATA_AURIAYA)))
                    if (pAuriaya->AI())
                        pAuriaya->AI()->DoAction(ACTION_CRAZY_CAT_LADY);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_sanctum_sentryAI(pCreature);
    };
};

class mob_feral_defender : public CreatureScript
{
    public:
        mob_feral_defender(): CreatureScript("mob_feral_defender") {}

    struct mob_feral_defenderAI : public ScriptedAI
    {
        mob_feral_defenderAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }
        
        InstanceScript* pInstance;
        int32 PounceTimer;
        int32 RushTimer;
        int32 RessTimer;
        int32 Lifes;

        void Reset()
        {
            PounceTimer = 5000;
            RushTimer = 12000;
            Lifes = 8;
            RessTimer = 999999;
            
            me->SetAuraStack(SPELL_FERAL_ESSENCE, me, Lifes);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                me->DespawnOrUnsummon();
                
            if (pInstance && pInstance->GetBossState(TYPE_AURIAYA) != IN_PROGRESS)
                me->DespawnOrUnsummon();

            if (PounceTimer <= int32(uiDiff))
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                {
                    me->AddThreat(pTarget, 100.0f);
                    me->AI()->AttackStart(pTarget);
                    DoCast(pTarget, SPELL_FERAL_POUNCE);
                }
                PounceTimer = urand(15000, 20000);
            } 
            else PounceTimer -= uiDiff;
            
            if (RushTimer <= int32(uiDiff))
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                {
                    me->AddThreat(pTarget, 100.0f);
                    me->AI()->AttackStart(pTarget);
                    DoCast(pTarget, SPELL_FERAL_RUSH);
                }
                RushTimer = urand(15000, 20000);
            } 
            else RushTimer -= uiDiff;
            
            if (RessTimer <= int32(uiDiff))
            {
                me->SetStandState(UNIT_STAND_STATE_STAND);
                for (uint8 i = 0; i < Lifes; ++i)
                    DoCast(me, SPELL_FERAL_ESSENCE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                me->SetReactState(REACT_AGGRESSIVE);
                RessTimer = 999999;
                DoZoneInCombat();
            } else RessTimer -= uiDiff;
            
            DoMeleeAttackIfReady();
        }
        
        void DamageTaken(Unit* pKiller, uint32 &damage)
        {
            if(damage >= me->GetHealth())
            {
                if (me->HasAura(SPELL_FERAL_ESSENCE))
                {
                    damage = 0;
                    DoCast(me, SPELL_SUMMON_ESSENCE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetStandState(UNIT_STAND_STATE_DEAD);
                    me->SetHealth(me->GetMaxHealth());
                    me->RemoveAllAuras();
                    me->AttackStop();
                    Lifes--;
                    PounceTimer = 35000;
                    RushTimer = 42000;
                    RessTimer = 30000;        
                }
                
                // must be placed here to make the achievement work
                if (Creature *pAuriaya = me->GetCreature(*me, pInstance->GetData64(DATA_AURIAYA)))
                    if (pAuriaya->AI())
                        pAuriaya->AI()->DoAction(ACTION_NINE_LIVES);
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_feral_defenderAI(pCreature);
    };
};


class seeping_trigger : public CreatureScript
{
    public:
        seeping_trigger(): CreatureScript("seeping_trigger") {}

    struct seeping_triggerAI : public ScriptedAI
    {
        seeping_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            pInstance = pCreature->GetInstanceScript();
        }
        
        InstanceScript* pInstance;
        
        void Reset()
        {
            me->ForcedDespawn(600000);
            DoCast(me, SPELL_SEEPING_ESSENCE);
        }
        
        void UpdateAI(const uint32 uiDiff)
        {
            if (pInstance && pInstance->GetBossState(TYPE_AURIAYA) != IN_PROGRESS)
                me->DespawnOrUnsummon();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new seeping_triggerAI(pCreature);
    };
};

void AddSC_boss_auriaya()
{
    new boss_auriaya();
    new feral_defender_trigger();
    new mob_sanctum_sentry();
    new mob_feral_defender();
    new seeping_trigger();
}
