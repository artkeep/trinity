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
SDName: Assembly of Iron
Author: PrinceCreed, thanks jwladi (Nieve - FDR)
SD%Complete: 100%
SDComment:
EndScriptData */

#include "ScriptPCH.h"
#include "ulduar.h"

// Any boss
#define SPELL_SUPERCHARGE   61920
#define SPELL_BERSERK       47008   // Hard enrage, don't know the correct ID.

// Steelbreaker
#define SPELL_HIGH_VOLTAGE                    RAID_MODE(61890,63498)
#define SPELL_FUSION_PUNCH                    RAID_MODE(61903,63493)
#define SPELL_STATIC_DISRUPTION               RAID_MODE(44008,63494)
#define SPELL_OVERWHELMING_POWER              RAID_MODE(64637,61888)
#define SPELL_ELECTRICAL_CHARGE               61902

// Runemaster Molgeim
#define SPELL_SHIELD_OF_RUNES                 RAID_MODE(62274,63489)
#define SPELL_RUNE_OF_POWER                   61973
#define SPELL_RUNE_OF_POWER_VISUAL            61974
#define SPELL_RUNE_OF_DEATH                   RAID_MODE(62269, 63490)
#define SPELL_RUNE_OF_SUMMONING               62273
#define SPELL_RUNE_OF_SUMMONING_VISUAL        62019
#define SPELL_RUNE_OF_SUMMONING_SUMMON        62020
#define SPELL_LIGHTNING_BLAST                 RAID_MODE(62054,63491)

// Stormcaller Brundir
#define SPELL_CHAIN_LIGHTNING                 RAID_MODE(61879,63479)
#define SPELL_OVERLOAD                        RAID_MODE(61869,63481)
#define SPELL_LIGHTNING_WHIRL                 RAID_MODE(61915,63483)
#define SPELL_LIGHTNING_TENDRILS              RAID_MODE(61887,63486)
#define SPELL_LIGHTNING_TENDRILS_SELF_VISUAL  61883
#define SPELL_STORMSHIELD                     64187

enum eEnums
{
    EVENT_ENRAGE,
    // Steelbreaker
    EVENT_FUSION_PUNCH,
    EVENT_STATIC_DISRUPTION,
    EVENT_OVERWHELMING_POWER,
    // Molgeim
    EVENT_RUNE_OF_POWER,
    EVENT_SHIELD_OF_RUNES,
    EVENT_RUNE_OF_DEATH,
    EVENT_RUNE_OF_SUMMONING,
    EVENT_LIGHTNING_BLAST,
    // Brundir
    EVENT_CHAIN_LIGHTNING,
    EVENT_OVERLOAD,
    EVENT_LIGHTNING_WHIRL,
    EVENT_LIGHTNING_TENDRILS,
    EVENT_ENDFLIGHT,
    EVENT_FLIGHT,
    EVENT_GROUND,
    EVENT_LAND,
    EVENT_MOVE_POS,
    //rune of power
    EVENT_AURA,

    MAX_EVENT

};

enum Actions
{
    ACTION_STEELBREAKER                         = 0,
    ACTION_MOLGEIM                              = 1,
    ACTION_BRUNDIR                              = 2,
};

// Achievements
#define ACHIEVEMENT_ON_YOUR_SIDE              RAID_MODE(2945, 2946) // TODO
#define ACHIEVEMENT_CANT_WHILE_STUNNED        RAID_MODE(2947, 2948) // TODO
#define ACHIEVEMENT_CHOOSE_STEELBREAKER       RAID_MODE(2941, 2944)
#define ACHIEVEMENT_CHOOSE_MOLGEIM            RAID_MODE(2939, 2942)
#define ACHIEVEMENT_CHOOSE_BRUNDIR            RAID_MODE(2940, 2943)

//if (me->GetPower(POWER_MANA) == me->GetMaxPower(POWER_MANA)) Useful for this Achievement
//    instance->DoCompleteAchievement(ACHIEVEMENT_CANT_WHILE_STUNNED);

#define EMOTE_OVERLOAD    "Stormcaller Brundir begins to Overload!"

enum Yells
{
    SAY_STEELBREAKER_AGGRO                      = -1603020,
    SAY_STEELBREAKER_SLAY_1                     = -1603021,
    SAY_STEELBREAKER_SLAY_2                     = -1603022,
    SAY_STEELBREAKER_POWER                      = -1603023,
    SAY_STEELBREAKER_DEATH_1                    = -1603024,
    SAY_STEELBREAKER_DEATH_2                    = -1603025,
    SAY_STEELBREAKER_BERSERK                    = -1603026,

    SAY_MOLGEIM_AGGRO                           = -1603030,
    SAY_MOLGEIM_SLAY_1                          = -1603031,
    SAY_MOLGEIM_SLAY_2                          = -1603032,
    SAY_MOLGEIM_RUNE_DEATH                      = -1603033,
    SAY_MOLGEIM_SUMMON                          = -1603034,
    SAY_MOLGEIM_DEATH_1                         = -1603035,
    SAY_MOLGEIM_DEATH_2                         = -1603036,
    SAY_MOLGEIM_BERSERK                         = -1603037,

    SAY_BRUNDIR_AGGRO                           = -1603040,
    SAY_BRUNDIR_SLAY_1                          = -1603041,
    SAY_BRUNDIR_SLAY_2                          = -1603042,
    SAY_BRUNDIR_SPECIAL                         = -1603043,
    SAY_BRUNDIR_FLIGHT                          = -1603044,
    SAY_BRUNDIR_DEATH_1                         = -1603045,
    SAY_BRUNDIR_DEATH_2                         = -1603046,
    SAY_BRUNDIR_BERSERK                         = -1603047,
};

bool IsEncounterComplete(InstanceScript* pInstance, Creature* me)
{
   if (!pInstance || !me)
        return false;

    for (uint8 i = 0; i < 3; ++i)
    {
        uint64 guid = pInstance->GetData64(DATA_STEELBREAKER+i);
        if (!guid)
            return false;

        if(Creature *boss = (Unit::GetCreature((*me), guid)))
        {
            if (boss->isAlive())
                return false;
            continue;
        }
        else
            return false;
    }
    return true;
}

class boss_steelbreaker : public CreatureScript
{
public:
    boss_steelbreaker() : CreatureScript("boss_steelbreaker") { }

    struct boss_steelbreakerAI : public BossAI
    {
        boss_steelbreakerAI(Creature *c) : BossAI(c, TYPE_ASSEMBLY)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
        }

        void Reset()
        {
            _Reset();
            events.Reset();
            phase = 0;
            me->RemoveAllAuras();
            me->ResetLootMode();
                
            // Respawn
            if(Creature* Brundir = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_BRUNDIR) : 0))
            {
                if(Brundir->isDead())
                {
                    Brundir->RemoveCorpse();
                    Brundir->Respawn(true);
                    Brundir->GetMotionMaster()->MoveTargetedHome();
                }
            }
            if(Creature* Molgeim = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_MOLGEIM) : 0))
            {
                if(Molgeim->isDead())
                {
                    Molgeim->RemoveCorpse();
                    Molgeim->Respawn(true);
                    Molgeim->GetMotionMaster()->MoveTargetedHome();
                }
            }
        }

        uint32 phase;

        void EnterCombat(Unit *who)
        {
            _EnterCombat();
            if (Creature* Brundir = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_BRUNDIR) : 0))
            {
                if (Brundir->isAlive())
                {
                    Brundir->SetInCombatWithZone() ;
                    Brundir->AI()->AttackStart(me->getVictim());
                }
            }
            if (Creature* Molgeim = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_MOLGEIM) : 0))
            {
                if (Molgeim->isAlive())
                {
                    Molgeim->SetInCombatWithZone() ;
                    Molgeim->AI()->AttackStart(me->getVictim());
                }
            }
            DoScriptText(SAY_STEELBREAKER_AGGRO, me);
            DoZoneInCombat();
            DoCast(me, SPELL_HIGH_VOLTAGE);
            phase = 1;
            events.SetPhase(phase);
            events.ScheduleEvent(EVENT_ENRAGE, 900000);
            events.ScheduleEvent(EVENT_FUSION_PUNCH, 15000);
        }

        void JustDied(Unit* Killer)
        {
            DoScriptText(RAND(SAY_STEELBREAKER_DEATH_1, SAY_STEELBREAKER_DEATH_2), me);
            DoCast(SPELL_SUPERCHARGE);
        
            if(IsEncounterComplete(instance, me) && instance)
            {
                _JustDied();
                instance->DoCompleteAchievement(ACHIEVEMENT_CHOOSE_STEELBREAKER);
            }
            else 
                me->SetLootRecipient(0);
                
            if(Creature* Brundir = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_BRUNDIR) : 0))
                if(Brundir->isAlive())
                    Brundir->AI()->DoAction(ACTION_BRUNDIR);

            if(Creature* Molgeim = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_MOLGEIM) : 0))
                if(Molgeim->isAlive())
                    Molgeim->AI()->DoAction(ACTION_MOLGEIM);
        }

        void KilledUnit(Unit *who)
        {
            DoScriptText(RAND(SAY_STEELBREAKER_SLAY_1, SAY_STEELBREAKER_SLAY_2), me);

            if (phase == 3)
                DoCast(me, SPELL_ELECTRICAL_CHARGE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_ENRAGE:
                        DoScriptText(SAY_STEELBREAKER_BERSERK, me);
                        DoCast(SPELL_BERSERK);
                        break;
                    case EVENT_FUSION_PUNCH:
                        if (me->IsWithinMeleeRange(me->getVictim()))
                            DoCastVictim(SPELL_FUSION_PUNCH);
                        events.ScheduleEvent(EVENT_FUSION_PUNCH, urand(15000, 20000));
                        break;
                    case EVENT_STATIC_DISRUPTION:
                    {
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_STATIC_DISRUPTION);
                        events.ScheduleEvent(EVENT_STATIC_DISRUPTION, 20000 + (rand()%20)*1000);
                    }
                    break;
                    case EVENT_OVERWHELMING_POWER:
                        DoScriptText(SAY_STEELBREAKER_POWER, me);
                        DoCastVictim(SPELL_OVERWHELMING_POWER);
                        events.ScheduleEvent(EVENT_OVERWHELMING_POWER, RAID_MODE(60000, 35000));
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_STEELBREAKER:
                    me->SetFullHealth();
                    //me->AddAura(SPELL_SUPERCHARGE, me);
                    ++phase;
                    events.SetPhase(phase);
                    if(phase >= 2)
                        events.RescheduleEvent(EVENT_STATIC_DISRUPTION, 30000);
                    if(phase >= 3)
                    {
                        events.RescheduleEvent(EVENT_OVERWHELMING_POWER, urand(2000, 5000));
                        me->AddLootMode(LOOT_MODE_HARD_MODE_1);
                        me->AddLootMode(LOOT_MODE_HARD_MODE_2);
                    }
                    break;
            }
        }
    };
    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_steelbreakerAI (pCreature);
    }
};

class boss_runemaster_molgeim : public CreatureScript
{
public:
    boss_runemaster_molgeim() : CreatureScript("boss_runemaster_molgeim") { }

    struct boss_runemaster_molgeimAI : public BossAI
    {
        boss_runemaster_molgeimAI(Creature *c) : BossAI(c, TYPE_ASSEMBLY)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
        }

        void Reset()
        {
            events.Reset();
            me->RemoveAllAuras();
            phase = 0;
            // Respawn
            if(Creature* Brundir = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_BRUNDIR) : 0))
            {
                if(Brundir->isDead())
                {
                    Brundir->Respawn(true);
                    Brundir->GetMotionMaster()->MoveTargetedHome();
                }
            }
            if(Creature* Steelbreaker = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_STEELBREAKER) : 0))
            {
                if(Steelbreaker->isDead())
                {            
                    Steelbreaker->Respawn(true);
                    Steelbreaker->GetMotionMaster()->MoveTargetedHome();
                }
            }
        }

        uint32 phase;

        void EnterCombat(Unit* who)
        {
            if (Creature* Steelbreaker = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_STEELBREAKER) : 0))
            {
                if (Steelbreaker->isAlive())
                {
                    Steelbreaker->SetInCombatWithZone() ;
                    Steelbreaker->AI()->AttackStart(me->getVictim());
                }
            }
            if (Creature* Brundir = Unit::GetCreature((*me), instance ? instance->GetData64(DATA_BRUNDIR) : 0))
            {
                if (Brundir->isAlive())
                {
                    Brundir->SetInCombatWithZone() ;
                    Brundir->AI()->AttackStart(me->getVictim());
                }
            }
            DoScriptText(SAY_MOLGEIM_AGGRO, me);
            DoZoneInCombat();
            phase = 1;
            events.ScheduleEvent(EVENT_ENRAGE, 900000);
            events.ScheduleEvent(EVENT_SHIELD_OF_RUNES, 30000);
            events.ScheduleEvent(EVENT_RUNE_OF_POWER, 20000);
        }

        void JustDied(Unit* Killer)
        {
            DoScriptText(RAND(SAY_MOLGEIM_DEATH_1, SAY_MOLGEIM_DEATH_2), me);
            DoCast(SPELL_SUPERCHARGE);
        
            if(IsEncounterComplete(instance, me) && instance)
            {
                _JustDied();
                instance->DoCompleteAchievement(ACHIEVEMENT_CHOOSE_MOLGEIM);
            }
            else 
                me->SetLootRecipient(0);
                
            if(Creature* Brundir = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_BRUNDIR) : 0))
                if(Brundir->isAlive())
                    Brundir->AI()->DoAction(ACTION_BRUNDIR);

            else 
                me->SetLootRecipient(0);
                
            if(Creature* Brundir = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_BRUNDIR) : 0))
                if(Brundir->isAlive())
                    Brundir->AI()->DoAction(ACTION_BRUNDIR);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_ENRAGE:
                        DoScriptText(SAY_MOLGEIM_BERSERK, me);
                        DoCast(SPELL_BERSERK);
                        break;
                    case EVENT_RUNE_OF_POWER: // Improve target selection; random alive friendly
                    {
                        Creature* bosschoosed;
                        uint32 choice = urand(0,2);

                        if (!instance) break;
                        
                        bosschoosed = Unit::GetCreature(*me, instance->GetData64(DATA_STEELBREAKER+choice));

                        if (!bosschoosed || !bosschoosed->isAlive()) {
                            choice = ((choice == 2) ? 0 : choice++);
                            bosschoosed = Unit::GetCreature(*me, instance->GetData64(DATA_STEELBREAKER+choice));
                            if (!bosschoosed || !bosschoosed->isAlive()) {
                                choice = ((choice == 2) ? 0 : choice++);
                                bosschoosed = Unit::GetCreature(*me, instance->GetData64(DATA_STEELBREAKER+choice));
                            }
                        }

                        if (!bosschoosed || !bosschoosed->isAlive())
                            bosschoosed = Unit::GetCreature(*me, instance->GetData64(DATA_MOLGEIM));
                        
                        DoCast(bosschoosed, SPELL_RUNE_OF_POWER);
                        events.ScheduleEvent(EVENT_RUNE_OF_POWER, 35000);
                    }
                    break;
                    case EVENT_SHIELD_OF_RUNES:
                        DoCast(me, SPELL_SHIELD_OF_RUNES);
                        events.ScheduleEvent(EVENT_SHIELD_OF_RUNES, urand(60000, 80000));
                        break;
                    case EVENT_RUNE_OF_DEATH:
                    {
                        DoScriptText(SAY_MOLGEIM_RUNE_DEATH, me);
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_RUNE_OF_DEATH);
                        events.ScheduleEvent(EVENT_RUNE_OF_DEATH, 30000);
                    }
                    break;
                    case EVENT_RUNE_OF_SUMMONING:
                    {
                        DoScriptText(SAY_MOLGEIM_SUMMON, me);
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_RUNE_OF_SUMMONING);
                        events.ScheduleEvent(EVENT_RUNE_OF_SUMMONING, urand(40000, 50000));
                    }
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }

        void KilledUnit(Unit *who)
        {
            DoScriptText(RAND(SAY_MOLGEIM_SLAY_1, SAY_MOLGEIM_SLAY_2), me);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_MOLGEIM:
                    me->SetFullHealth();
                    //me->AddAura(SPELL_SUPERCHARGE, me);
                    ++phase;
                    events.SetPhase(phase);
                    events.RescheduleEvent(EVENT_SHIELD_OF_RUNES, 30000);
                    events.RescheduleEvent(EVENT_RUNE_OF_POWER, 20000);
                    if(phase >= 2)
                        events.RescheduleEvent(EVENT_RUNE_OF_DEATH, 35000);
                    if(phase >= 3)
                    {
                        events.RescheduleEvent(EVENT_RUNE_OF_SUMMONING, 15000);
                        me->AddLootMode(LOOT_MODE_HARD_MODE_1);
                    }
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_runemaster_molgeimAI (pCreature);
    }
};

class boss_stormcaller_brundir : public CreatureScript
{
public:
    boss_stormcaller_brundir() : CreatureScript("boss_stormcaller_brundir") { }

    struct boss_stormcaller_brundirAI : public BossAI
    {
        boss_stormcaller_brundirAI(Creature *c) : BossAI(c, TYPE_ASSEMBLY)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, false);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
        }

        void Reset()
        {
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISTRACT, false);
            me->RemoveAllAuras();
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_LEVITATING | MOVEMENTFLAG_WALKING);
            events.Reset();
            phase = 0;
            // Respawn
            if(Creature* Molgeim = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_MOLGEIM) : 0))
            {
                if(Molgeim->isDead())
                {
                    Molgeim->RemoveCorpse();
                    Molgeim->Respawn(true);
                    Molgeim->GetMotionMaster()->MoveTargetedHome();
                }
            }
            if(Creature* Steelbreaker = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_STEELBREAKER) : 0))
            {
                if(Steelbreaker->isDead())
                {       
                    Steelbreaker->RemoveCorpse() ;
                    Steelbreaker->Respawn(true);
                    Steelbreaker->GetMotionMaster()->MoveTargetedHome();
                }
            }
        }

        uint32 phase;
        uint32 Position;

        void EnterCombat(Unit* who)
        {
            if (Creature* Steelbreaker = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_STEELBREAKER) : 0))
            {
                if (Steelbreaker->isAlive())
                {
                    Steelbreaker->SetInCombatWithZone() ;
                    Steelbreaker->AI()->AttackStart(me->getVictim());
                }
            }
            if (Creature* Molgeim = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_MOLGEIM) : 0))
            {
                if (Molgeim->isAlive())
                {
                    Molgeim->SetInCombatWithZone() ;
                    Molgeim->AI()->AttackStart(me->getVictim());
                }
            }
            DoScriptText(SAY_BRUNDIR_AGGRO, me);
            DoZoneInCombat();
            phase = 1;
            //events.ScheduleEvent(EVENT_MOVE_POS, 1000);
            events.ScheduleEvent(EVENT_ENRAGE, 900000);
            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 2000);
            events.ScheduleEvent(EVENT_OVERLOAD, urand(45000, 90000));
        }

        void JustDied(Unit* Killer)
        {
            DoScriptText(RAND(SAY_BRUNDIR_DEATH_1, SAY_BRUNDIR_DEATH_2), me);
            DoCast(SPELL_SUPERCHARGE);

            me->FallGround();
        
            if(IsEncounterComplete(instance, me) && instance)
            {
                _JustDied();
                instance->DoCompleteAchievement(ACHIEVEMENT_CHOOSE_BRUNDIR);
            }
            else 
                me->SetLootRecipient(0);
                
            if(Creature* Molgeim = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_MOLGEIM) : 0))
                if(Molgeim->isAlive())
                    Molgeim->AI()->DoAction(ACTION_MOLGEIM);

            if(Creature* Steelbreaker = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_STEELBREAKER) : 0))
                if(Steelbreaker->isAlive())
                    Steelbreaker->AI()->DoAction(ACTION_STEELBREAKER);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_MOVE_POS:
                        MovePos();
                        events.RescheduleEvent(EVENT_MOVE_POS, 10000);
                    break;
                    case EVENT_ENRAGE:
                        DoScriptText(SAY_BRUNDIR_BERSERK, me);
                        DoCast(SPELL_BERSERK);
                    break;
                    case EVENT_CHAIN_LIGHTNING:
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_CHAIN_LIGHTNING);
                        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(4000, 6000));
                    break;
                    case EVENT_OVERLOAD:
                        me->MonsterTextEmote(EMOTE_OVERLOAD, 0, true);
                        DoScriptText(SAY_BRUNDIR_SPECIAL, me);
                        me->GetMotionMaster()->Clear(true);
                        DoCast(SPELL_OVERLOAD);
                        events.ScheduleEvent(EVENT_OVERLOAD, urand(45000, 90000));
                    break;
                    case EVENT_LIGHTNING_WHIRL:
                        me->GetMotionMaster()->Clear(true);
                        DoCast(SPELL_LIGHTNING_WHIRL);
                        events.ScheduleEvent(EVENT_LIGHTNING_WHIRL, urand(15000, 20000));
                    break;
                    case EVENT_LIGHTNING_TENDRILS:
                        DoScriptText(SAY_BRUNDIR_FLIGHT, me);
                        DoCast(SPELL_LIGHTNING_TENDRILS);
                        me->AttackStop();
                        me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                        DoCast(SPELL_LIGHTNING_TENDRILS_SELF_VISUAL);
                        me->GetMotionMaster()->Clear(true);
                        me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), 440);
                        events.DelayEvents(35000);
                        events.ScheduleEvent(EVENT_FLIGHT, 2500);
                        events.ScheduleEvent(EVENT_ENDFLIGHT, 28000);
                        events.ScheduleEvent(EVENT_LIGHTNING_TENDRILS, 70000);
                    break;
                    case EVENT_FLIGHT:
                        me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISTRACT, true);
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            me->GetMotionMaster()->MovePoint(0, pTarget->GetPositionX(), pTarget->GetPositionY(), 440);
                        events.ScheduleEvent(EVENT_FLIGHT, 6000);
                    break;
                    case EVENT_ENDFLIGHT:
                        me->GetMotionMaster()->Clear(true);
                        me->GetMotionMaster()->MovePoint(0, 1586.920166f, 119.848984f, 440.0f);
                        events.CancelEvent(EVENT_FLIGHT);
                        events.CancelEvent(EVENT_ENDFLIGHT);
                        events.ScheduleEvent(EVENT_LAND, 4000);
                        break;
                    case EVENT_LAND:
                        me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISTRACT, false);
                        me->GetMotionMaster()->Clear(true);
                        me->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), 427.28f);
                        events.CancelEvent(EVENT_LAND);
                        events.ScheduleEvent(EVENT_GROUND, 2500);
                    break;
                    case EVENT_GROUND:
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                        me->RemoveAurasDueToSpell(SPELL_LIGHTNING_TENDRILS);
                        me->RemoveAurasDueToSpell(SPELL_LIGHTNING_TENDRILS_SELF_VISUAL);
                        events.CancelEvent(EVENT_GROUND);
                    break;
                }
            }
        }

        void KilledUnit(Unit *who)
        {
            DoScriptText(RAND(SAY_BRUNDIR_SLAY_1, SAY_BRUNDIR_SLAY_2), me);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_BRUNDIR:
                    me->SetFullHealth();
                    //me->AddAura(SPELL_SUPERCHARGE, me);
                    ++phase;
                    events.SetPhase(phase);
                    events.RescheduleEvent(EVENT_CHAIN_LIGHTNING, urand(6000, 12000));
                    events.RescheduleEvent(EVENT_OVERLOAD, 40000);
                    if(phase >= 2)
                        events.RescheduleEvent(EVENT_LIGHTNING_WHIRL, urand(15000, 20000));
                    if(phase >= 3)
                    {
                        me->AddAura(SPELL_STORMSHIELD, me);
                        events.RescheduleEvent(EVENT_LIGHTNING_TENDRILS, 20000);
                    }
                    break;
            }
        }
        
        void MovePos()
        {
            switch (Position)
            {
                case 0:
                    me->GetMotionMaster()->MovePoint(0, 1587.28f, 97.030f, me->GetPositionZ());
                    break;
                case 1:
                    me->GetMotionMaster()->MovePoint(0, 1587.18f, 121.03f, me->GetPositionZ());
                    break;
                case 2:
                    me->GetMotionMaster()->MovePoint(0, 1587.34f, 142.58f, me->GetPositionZ());
                    break;
                case 3:
                    me->GetMotionMaster()->MovePoint(0, 1587.18f, 121.03f, me->GetPositionZ());
                    break;
            }

            Position++;
            if(Position > 3)
            {
                Position = 0;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_stormcaller_brundirAI (pCreature);
    }
};

/***************
*  mob_lightning_elemental
*****************/

class mob_lightning_elemental : public CreatureScript
{
public:
    mob_lightning_elemental() : CreatureScript("mob_lightning_elemental") { }

    struct mob_lightning_elementalAI : public ScriptedAI 
    {
        mob_lightning_elementalAI(Creature *c) : ScriptedAI(c) {}
        
        Unit* Target;
        bool Casted;

        void Reset() 
        {
            Casted = false;
            me->GetMotionMaster()->MoveRandom(30);
        }

        void UpdateAI(const uint32 diff)
        {
            if (me->IsWithinMeleeRange(me->getVictim()) && !Casted)
            {
                me->CastSpell(me, SPELL_LIGHTNING_BLAST, true);
                me->ForcedDespawn(500);
                Casted = true;
            }
        }
    };
    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_lightning_elementalAI (pCreature);
    }
};

/***************
*  mob_rune_of_summoning
*****************/

class mob_rune_of_summoning : public CreatureScript
{
public:
    mob_rune_of_summoning() : CreatureScript("mob_rune_of_summoning") { }

    struct mob_rune_of_summoningAI : public ScriptedAI
    {
        mob_rune_of_summoningAI(Creature *c) : ScriptedAI(c)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
            me->SetReactState(REACT_PASSIVE);
        }

        uint32 SummonTimer;

        void Reset()
        {
            SummonTimer = 1500;
            me->ForcedDespawn(12500);
            DoCast(me, SPELL_RUNE_OF_SUMMONING_VISUAL);
        }

        void UpdateAI(const uint32 uiDiff)
        {

            if (SummonTimer <= uiDiff)
            {
                DoCast(me, SPELL_RUNE_OF_SUMMONING_SUMMON);
                SummonTimer = 1500;
            } 
            else SummonTimer -= uiDiff;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_rune_of_summoningAI (pCreature);
    }
};

/***************
*  mob_rune_of_power
*****************/

class mob_rune_of_power : public CreatureScript
{
public:
    mob_rune_of_power() : CreatureScript("mob_rune_of_power") { }

    struct mob_rune_of_powerAI : public Scripted_NoMovementAI
    {
        mob_rune_of_powerAI(Creature *c) : Scripted_NoMovementAI(c)  
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->SetReactState(REACT_PASSIVE);
        }
        
        void Reset()
        {
            DoCast(me, SPELL_RUNE_OF_POWER_VISUAL, true);
            me->ForcedDespawn(35000);
        }

        /*
        void MoveInLineOfSight(Unit *who)
        {
            if (who->IsWithinDist(me, 5.0f))
                me->AddAura(64320, who);
        }
        */
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_rune_of_powerAI (pCreature);
    }
};

void AddSC_boss_assembly_of_iron()
{
    new boss_steelbreaker();
    new boss_runemaster_molgeim();
    new boss_stormcaller_brundir();
    new mob_lightning_elemental();
    new mob_rune_of_summoning();
    new mob_rune_of_power();
}
