/*
 * Copyright (C) 2008 - 2010 Trinity <http://www.trinitycore.org/>
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
#include "SpellAuraEffects.h"
#include "forge_of_souls.h"

enum Yells
{
    SAY_AGGRO           = -1632001,
    SAY_SLAY_1          = -1632002,
    SAY_SLAY_2          = -1632003,
    SAY_DEATH           = -1632004,
    SAY_SOUL_STORM      = -1632005,
    SAY_CORRUPT_SOUL    = -1632006,
};

enum Spells
{
    SPELL_MAGIC_S_BANE          = 68793,
    SPELL_SHADOW_BOLT           = 70043,
    SPELL_CORRUPT_SOUL          = 68839,
    SPELL_CONSUME_SOUL          = 68861,
    SPELL_TELEPORT              = 68988,
    SPELL_FEAR                  = 68950,
    SPELL_SOULSTORM             = 68872,
    SPELL_SOULSTORM_CHANNEL     = 69008,    // pre-fight
    SPELL_SOULSTORM_VISUAL      = 68870,    // pre-cast soulstorm
    SPELL_PURPLE_BANISH_VISUAL  = 68862     // Used by Soul Fragment (Aura)
};

enum Events
{
    EVENT_MAGIC_BANE    = 1,
    EVENT_SHADOW_BOLT   = 2,
    EVENT_CORRUPT_SOUL  = 3,
    EVENT_SOULSTORM     = 4,
    EVENT_FEAR          = 5,
};

enum CombatPhases
{
    PHASE_1 = 1,
    PHASE_2 = 2
};

class boss_bronjahm : public CreatureScript
{
    public:
        boss_bronjahm() : CreatureScript("boss_bronjahm") { }

        struct boss_bronjahmAI : public BossAI
        {
            boss_bronjahmAI(Creature* pCreature) : BossAI(pCreature, DATA_BRONJAHM)
            {
                // disable AI outside of instance
                if (!instance)
                    me->IsAIEnabled = false;
            }

            void Reset()
            {
                events.Reset();
                events.SetPhase(PHASE_1);
                events.ScheduleEvent(EVENT_SHADOW_BOLT, 2000);
                events.ScheduleEvent(EVENT_MAGIC_BANE, urand(8000, 15000));
                events.ScheduleEvent(EVENT_CORRUPT_SOUL, urand(25000, 35000), 0, PHASE_1);

                me->CastSpell(me, SPELL_SOULSTORM_CHANNEL, true);

                instance->SetBossState(DATA_BRONJAHM, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);
                me->RemoveAurasDueToSpell(SPELL_SOULSTORM_CHANNEL);

                instance->SetBossState(DATA_BRONJAHM, IN_PROGRESS);
            }

            void JustDied(Unit* /*killer*/)
            {
                DoScriptText(SAY_DEATH, me);

                instance->SetBossState(DATA_BRONJAHM, DONE);
            }

            void KilledUnit(Unit * who)
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
            {
                if (events.GetPhaseMask() & (1 << PHASE_1) && !HealthAbovePct(30))
                {
                    events.SetPhase(PHASE_2);
                    DoCast(me, SPELL_TELEPORT);
                    events.ScheduleEvent(EVENT_FEAR, urand(12000, 16000), 0, PHASE_2);
                    events.ScheduleEvent(EVENT_SOULSTORM, 700, 0, PHASE_2);
                }
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->SetReactState(REACT_PASSIVE);
                summon->GetMotionMaster()->Clear();
                summon->GetMotionMaster()->MoveFollow(me, me->GetObjectSize(), 0.0f);
                summon->CastSpell(summon, SPELL_PURPLE_BANISH_VISUAL, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_MAGIC_BANE:
                            DoCastVictim(SPELL_MAGIC_S_BANE);
                            events.ScheduleEvent(EVENT_MAGIC_BANE, urand(8000, 15000));
                            break;
                        case EVENT_SHADOW_BOLT:
                            if (!me->IsWithinMeleeRange(me->getVictim()))
                                DoCastVictim(SPELL_SHADOW_BOLT);
                            events.ScheduleEvent(EVENT_SHADOW_BOLT, 2000);
                            break;
                        case EVENT_CORRUPT_SOUL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            {
                                DoScriptText(SAY_CORRUPT_SOUL, me);
                                DoCast(target, SPELL_CORRUPT_SOUL);
                            }
                            events.ScheduleEvent(EVENT_CORRUPT_SOUL, urand(25000, 35000), 0, PHASE_1);
                            break;
                        case EVENT_SOULSTORM:
                            DoScriptText(SAY_SOUL_STORM, me);
                            me->CastSpell(me, SPELL_SOULSTORM_VISUAL, true);
                            me->CastSpell(me, SPELL_SOULSTORM, false);
                            break;
                        case EVENT_FEAR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                me->CastCustomSpell(SPELL_FEAR, SPELLVALUE_MAX_TARGETS, 1, target, false);
                            events.ScheduleEvent(EVENT_FEAR, urand(8000, 12000), 0, PHASE_2);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI *GetAI(Creature* pCreature) const
        {
            return new boss_bronjahmAI(pCreature);
        }
};

class mob_corrupted_soul_fragment : public CreatureScript
{
public:
    mob_corrupted_soul_fragment() : CreatureScript("mob_corrupted_soul_fragment") { }

    struct mob_corrupted_soul_fragmentAI : public ScriptedAI
    {
        mob_corrupted_soul_fragmentAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = me->GetInstanceScript();
        }

        InstanceScript* pInstance;

        uint32 uiCheckTimer;

        void Reset()
        {
            uiCheckTimer = 0; // first check is immediate
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiCheckTimer <= diff)
            {
                if (pInstance)
                {
                    Creature* pBronjham = Unit::GetCreature(*me, pInstance->GetData64(DATA_BRONJAHM));
                    if (pBronjham && pBronjham->isAlive())
                    {
                        if (me->IsWithinMeleeRange(pBronjham))
                        {
                            pBronjham->CastSpell(pBronjham, SPELL_CONSUME_SOUL, true);
                            me->DespawnOrUnsummon();
                        }
                        else
                        {
                            Position pos;
                            pBronjham->GetPosition(&pos);
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(0, pos);
                        }
                    }
                    else
                        me->DespawnOrUnsummon();
                }
                uiCheckTimer = 500;
            }
            else
                uiCheckTimer -= diff;
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new mob_corrupted_soul_fragmentAI(creature);
    }
};

void AddSC_boss_bronjahm()
{
    new boss_bronjahm();
    new mob_corrupted_soul_fragment();
}
