/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
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

enum Creatures
{
    CREATURE_LOTTERY                          = 1000000,
};

enum Data
{
    DATA_LOTTERY_EVENT,
};

enum Data64
{
    DATA_LOTERRY,
};

enum Events
{
    EVENT_NONE,

    // LoTTery
    EVENT_BURNING_BREATH,
    EVENT_BURNING_FURY,
    EVENT_FLAME_CINDER_A,
    EVENT_METEOR_FISTS_A,
    EVENT_METEOR_FISTS_B,

};

enum Spells
{
    // Spells
    SPELL_BURNING_BREATH                        = 66665,
    SPELL_BURNING_BREATH_H                      = 67328,
    SPELL_BURNING_FURY                          = 66721,
    SPELL_FLAME_CINDER_A                        = 66684,
    SPELL_FLAME_CINDER_A_H                      = 67332,
    SPELL_FLAME_CINDER_B                        = 66681, 
    SPELL_METEOR_FISTS_A                        = 66725,
    SPELL_METEOR_FISTS_A_H                      = 66765,
    SPELL_METEOR_FISTS_B                        = 67333,
    SPELL_METEOR_FISTS_B_H                      = 68161,
};

enum Yells
{
	SAY_BURNING_FURY                              = -1999970,
    SAY_AGGRO                                     = -1999971,
    SAY_SLAY_1                                    = -1999972,
    SAY_SLAY_2                                    = -1999973,
    SAY_SLAY_3                                    = -1999974,
    SAY_DEATH                                     = -1999975,
	SAY_RANDOM_1                                  = -1999976,
	SAY_RANDOM_2                                  = -1999977,
	SAY_RANDOM_3                                  = -1999978,
	SAY_RANDOM_4                                  = -1999979,
	SAY_RANDOM_5                                  = -1999980,
	SAY_RANDOM_6                                  = -1999981,
	SAY_RANDOM_7                                  = -1999982,
	SAY_RANDOM_8                                  = -1999983,
	SAY_RANDOM_9                                  = -1999984,
	SAY_RANDOM_10                                  = -1999985,
};

class boss_lottery : public CreatureScript
{
public:
    boss_lottery() : CreatureScript("boss_lottery") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_lotteryAI (pCreature);
    }

    struct boss_lotteryAI : public ScriptedAI
    {
        boss_lotteryAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
        }

        InstanceScript *pInstance;
        EventMap events;

        void Reset()
        {
            events.Reset();

            if (pInstance)
                pInstance->SetData(DATA_LOTTERY_EVENT, NOT_STARTED);
        }

        void KilledUnit(Unit * /*victim*/)
        {
            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2,SAY_SLAY_3), me);
        }

        void JustDied(Unit* /*Killer*/)
        {
            DoScriptText(SAY_DEATH, me);
        }

        void EnterCombat(Unit * /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
            DoZoneInCombat();

            DoCast(me, SPELL_BURNING_FURY);
            events.ScheduleEvent(EVENT_BURNING_FURY, 20000);    // TODO check timer
            events.ScheduleEvent(EVENT_BURNING_BREATH, 15000);  // 1st after 15sec, then every 45sec
            events.ScheduleEvent(EVENT_METEOR_FISTS_A, 75000);  // 1st after 75sec, then every 45sec
            events.ScheduleEvent(EVENT_FLAME_CINDER_A, 30000);  // TODO check timer

            if (pInstance)
                pInstance->SetData(DATA_LOTTERY_EVENT, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

			events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_BURNING_FURY:
                        DoCast(me, SPELL_BURNING_FURY);
						DoScriptText(RAND(SAY_RANDOM_1,SAY_RANDOM_2,SAY_RANDOM_3,SAY_RANDOM_4,SAY_RANDOM_5,SAY_RANDOM_6,SAY_RANDOM_7,SAY_RANDOM_8,SAY_RANDOM_9,SAY_RANDOM_10), me);
                        events.ScheduleEvent(EVENT_BURNING_FURY, 20000);
                        return;
                    case EVENT_BURNING_BREATH:
                        DoCast(me, RAID_MODE(SPELL_BURNING_BREATH,SPELL_BURNING_BREATH_H));
                        events.ScheduleEvent(EVENT_BURNING_BREATH, 45000);
                        return;
                    case EVENT_METEOR_FISTS_A:
                        DoCast(me, RAID_MODE(SPELL_METEOR_FISTS_A,SPELL_METEOR_FISTS_A_H));
                        events.ScheduleEvent(EVENT_METEOR_FISTS_B, 1500);
                        return;
                    case EVENT_METEOR_FISTS_B:
                        DoCast(me, RAID_MODE(SPELL_METEOR_FISTS_B,SPELL_METEOR_FISTS_B_H));
                        DoScriptText(SAY_BURNING_FURY, me);
						events.ScheduleEvent(EVENT_METEOR_FISTS_A, 45000);
                        return;
                    case EVENT_FLAME_CINDER_A:
                        DoCast(me, RAID_MODE(SPELL_FLAME_CINDER_A,SPELL_FLAME_CINDER_A_H));
                        events.ScheduleEvent(EVENT_FLAME_CINDER_A, 30000);
                        return;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

};


void AddSC_boss_lottery()
{
    new boss_lottery();
}
