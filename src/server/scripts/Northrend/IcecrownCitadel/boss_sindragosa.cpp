/*
* Copyright (C) 2009 - 2010 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

/*
*Need correct timers
*Need add  Sindragossa fly in fly phase
*/

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "icecrown_citadel.h"
#include "../../../collision/Management/VMapFactory.h"

enum Yells
{
    SAY_AGGRO            = -1666071,
    SAY_UNCHAIND_MAGIC   = -1666072,
    SAY_BLISTERING_COLD  = -1666073,
    //SAY_BREATH           = -1666074,
    SAY_AIR_PHASE        = -1666075,
    SAY_PHASE_3          = -1666076,
    SAY_KILL_1           = -1666077,
    SAY_KILL_2           = -1666078,
    SAY_BERSERK          = -1666079,
    SAY_DEATH            = -1666080
};

enum Spells
{
    SPELL_FROST_AURA                     = 70084,
    SPELL_CLEAVE                         = 19983,
    SPELL_TAIL_SMASH                     = 71077,
    SPELL_FROST_BREATH                   = 69649,
    SPELL_PERMEATING_CHILL               = 70109,
    SPELL_CHILLED_TO_THE_BONE            = 70106,
    SPELL_INSTABILITY                    = 69766,
    SPELL_UNCHAINED_MAGIC                = 69762,
    SPELL_BACKLASH                       = 71044,
    SPELL_ICY_TRIP_PULL                  = 70117,
    SPELL_ICY_TRIP_JUMP                  = 70122,
    SPELL_BLISTERING_COLD                = 70123,
    SPELL_FROST_BOMB_TRIGGER             = 69846,
    SPELL_FROST_BEACON_EFFECT            = 69675,
    SPELL_FROST_BEACON                   = 70126,
    SPELL_FROST_BOMB_TARGETTING_VISUAL   = 70022,
    SPELL_ICE_TOMB                       = 70157,
    SPELL_FROST_BOMB                     = 69845,
    SPELL_FROST_BOMB_MISSILE             = 69846,
    SPELL_FROST_BOMB_EXPLOSION           = 71053,
    SPELL_FROST_BOMB_EXPLOSION_VISUAL    = 64624,
    //SPELL_FROST_BOMB_MISSILE             = 70521,
    SPELL_MYSTIC_BUFFET                  = 70128,
    SPELL_ASPHYXATION                    = 71665,
    SPELL_FROST_AURA_ADD                 = 71387,
    SPELL_FROST_BREATH_ADD               = 71386,
    SPELL_ICY_BLAST                      = 71376,

    SPELL_ICY_BLAST_GROUND_10N           = 69238,
    SPELL_ICY_BLAST_GROUND_10H           = 69628,
    SPELL_ICY_BLAST_GROUND_25N           = 71380,
    SPELL_ICY_BLAST_GROUND_25H           = 71381,

    SPELL_ICY_BAST_IMPACT_10N            = 71377,
    SPELL_ICY_BAST_IMPACT_10H            = 69233,
    SPELL_ICY_BAST_IMPACT_25N            = 69646,
    SPELL_ICY_BAST_IMPACT_25H            = 71377,

    SPELL_BELLOWING_ROAR                 = 36922,
    SPELL_CLEAVE_ADD                     = 40505,
    SPELL_TAIL_SWEEP                     = 71369,
    SPELL_WITHOUT_ANIMATION              = 40031
};

enum ePoints
{
    POINT_PHASE_FLY                = 1,
    POINT_PHASE_NORMAL             = 2,
    POINT_LANDING                  = 3
};

enum eEvents
{
    //Rimefang
    EVENT_FROST_BREATH = 1,
    EVENT_ICY_BLAST,
    EVENT_FLY_PHASE,
    EVENT_GROUND_PHASE,
    EVENT_ICY_BLAST_EFFECT,
    //Spinestalker
    EVENT_TAIL_SWEEP,
    EVENT_CLEAVE,
    EVENT_BELLOWING_ROAR,
    //Sindragosa
    EVENT_TAIL_SMASH,
    EVENT_ICY_GRIP,
    EVENT_BLISTERING_COLD,
    EVENT_ICE_TOMB,
    EVENT_FROST_BEACON,
    EVENT_FROST_BOMB_MARK_LANDING_POSITION,
    EVENT_FROST_BOMB_MISSILE,
    EVENT_FROST_BOMB_EXPLOSION,
    EVENT_BERSERK_NORMAL,
    EVENT_BERSERK_FLY,
    EVENT_BERSERK_THREE,
    EVENT_UNCHAINED_MAGIC,
    EVENT_BOMB,
    EVENT_ASPHYXIATION,
    EVENT_DESPAWN
};

enum ePhases 
{
	PHASE_NORMAL				= 1,
	PHASE_FLY					= 2,
    PHASE_THREE                 = 3
};

enum eSindragosaActions
{
    ACTION_ICY_BLAST_IMPACT = 1
};
/*
Sindragosa casts spell 69846. Bomb flies down to the mob. When it lands, it explodes.

Position: X: 4360.1 Y: 2510.012 Z: 203.4833 O: 3.141593
Position: X: 4391.1 Y: 2476.4 Z: 203.4833 O: 3.141593
Position: X: 4394.1 Y: 2498.075 Z: 203.4833 O: 3.141593
Position: X: 4408.1 Y: 2466.511 Z: 203.4833 O: 3.141593

Position: X: 4390.1 Y: 2467.817 Z: 203.4833 O: 3.141593
Position: X: 4397.1 Y: 2483.4 Z: 203.4833 O: 3.141593

const Position SpawnLoc[]=
{
    {4523.889f, 2486.907f, 280.249f, 3.155f}, //fly pos
    {4407.439f, 2484.905f, 203.374f, 3.166f}, //center
    {4407.439f, 2484.905f, 230.374f, 3.166f}, //center Z + 30
    {4671.521f, 2481.815f, 343.365f, 3.166f} //spawn pos
};*/
const Position DragonLoc[] = 
{
    {4430.0f, 2465.0f, 204.0f, 3.166f}, //Rimefang landing position
    {4430.0f, 2505.0f, 204.0f, 3.166f}, //Spinestalker landing position
    {4430.0f, 2485.0f, 204.0f, 3.166f}  //Sindragosa landing position
};
class boss_sindragosa : public CreatureScript
{
    public:
        boss_sindragosa() : CreatureScript("boss_sindragosa") { }

        struct boss_sindragosaAI : public BossAI
        {
            boss_sindragosaAI(Creature* creature) : BossAI(creature, DATA_SINDRAGOSA)
            {
                instance = me->GetInstanceScript();
            }

            void Reset()
            {
                //events.ScheduleEvent(EVENT_FROST_BEACON, 25000, 0, PHASE_NORMAL);
                //events.ScheduleEvent(EVENT_BOMB, 7000, 0, PHASE_NORMAL);

                //uiPhase = 1;

                //uiBreathTimer = 15000;
                //uiTailSmashTimer = 15000;
                //uiBlisteringColdTimer = 30000;
                //uiMarkTimer = 25000;
                //uiBerserkTimer = 600000;
                //uiChangePhaseTimer = 110000;
                //uiUnchainedMagicTimer = 12000;
                //uiBombTimer = 7000;
                //uiCleaveTimer = 5000;

                //me->SetFlying(true);
                //SetCombatMovement(true);

                instance->SetData(DATA_ALL_YOU_CAN_EAT, DONE);

                //bMystic = false;
                bCanSwitch = false;

                if(instance)
                    instance->SetData(DATA_SINDRAGOSA_EVENT, NOT_STARTED);

                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    me->GetMotionMaster()->MovementExpired();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_LAND)
                    me->GetMotionMaster()->MovePoint(POINT_LANDING, DragonLoc[2]);
                ScriptedAI::DoAction(action);
            }

            void EnterCombat(Unit* /*who*/)
            {
                Cleanup();
                events.Reset();
                events.SetPhase(PHASE_NORMAL);
                events.ScheduleEvent(EVENT_CLEAVE, 5000, 0, PHASE_NORMAL);
                events.ScheduleEvent(EVENT_FROST_BREATH, 10000, 0, PHASE_NORMAL);
                events.ScheduleEvent(EVENT_TAIL_SMASH, 22000, 0, PHASE_NORMAL);
                events.ScheduleEvent(EVENT_ICY_GRIP, 30000, 0, PHASE_NORMAL);
                events.ScheduleEvent(EVENT_UNCHAINED_MAGIC, 12000, 0, PHASE_NORMAL);
                events.ScheduleEvent(EVENT_BERSERK_NORMAL, 600000, 0, PHASE_NORMAL);
                events.ScheduleEvent(EVENT_BERSERK_FLY, 600000, 0, PHASE_FLY);
                events.ScheduleEvent(EVENT_BERSERK_THREE, 600000, 0, PHASE_THREE);
                if(instance)
                    instance->SetData(DATA_SINDRAGOSA_EVENT, IN_PROGRESS);

                DoCast(me, SPELL_FROST_AURA);
                DoCast(me, SPELL_PERMEATING_CHILL);
                DoScriptText(SAY_AGGRO, me);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (!instance)
                    return;

                DoScriptText(SAY_DEATH, me);

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_UNCHAINED_MAGIC);
                instance->SetData(DATA_SINDRAGOSA_EVENT, DONE);
                if(instance->GetData(DATA_ALL_YOU_CAN_EAT) == DONE)
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_ALL_YOU_CAN_EAT_10,ACHIEV_ALL_YOU_CAN_EAT_25));
                Cleanup();
            }
            void Cleanup()
            {
                UnsummonSpecificCreaturesNearby(me, CREATURE_ICE_TOMB, 100.0f);
                UnsummonSpecificCreaturesNearby(me, CREATURE_FROST_BOMB_EXPLOSION_VISUAL, 100.0f);
                UnsummonSpecificCreaturesNearby(me, CREATURE_FROST_BOMB_LANDING_MARK, 100.0f);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_UNCHAINED_MAGIC);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ICE_TOMB);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INSTABILITY);
            }
            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    DoScriptText(RAND(SAY_KILL_1,SAY_KILL_2), me);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if(type != POINT_MOTION_TYPE)
                    return;
                switch (id)
                {
                    case POINT_LANDING:
                    {
                        me->SetHomePosition(DragonLoc[2]);
                        me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, 0x02);
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);
                        break;
                    }
                    case POINT_PHASE_FLY:
                    {
                        //events.Reset();
                        events.SetPhase(PHASE_FLY);
                        events.ScheduleEvent(EVENT_FROST_BEACON, 5000, 0, PHASE_FLY);
                        events.ScheduleEvent(EVENT_FROST_BOMB_MARK_LANDING_POSITION, 15000, 0, PHASE_FLY);
                        events.ScheduleEvent(EVENT_GROUND_PHASE, 45000, 0, PHASE_FLY);
                        me->RemoveAurasDueToSpell(SPELL_FROST_AURA);
                        break;
                    }
                    case POINT_PHASE_NORMAL:
                    {
                        //events.Reset();
                        //events.SetPhase(PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_FLY_PHASE, 110000-45000, 0, PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_CLEAVE, 5000, 0, PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_FROST_BREATH, 5000, 0, PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_TAIL_SMASH, 10000, 0, PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_ICY_GRIP, 35000, 0, PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_UNCHAINED_MAGIC, 8000, 0, PHASE_NORMAL);
                        DoCast(me, SPELL_FROST_AURA);
                        SetCombatMovement(true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, 0x02);
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);

                        if (Unit *victim = me->SelectVictim())
                            AttackStart(victim);
                        break;
                    }
                }
            }

            void JustReachedHome()
            {
                if (!instance)
                    return;
                Cleanup();
                instance->SetData(DATA_SINDRAGOSA_EVENT, FAIL);
            }

            void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
            {
                if (GetPhase(events) == PHASE_NORMAL)
                {
                    if(HealthBelowPct(86) && !bCanSwitch)
                    {
                        events.ScheduleEvent(EVENT_FLY_PHASE, 1000, 0, PHASE_NORMAL);
                        bCanSwitch = true;
                    }
                    //Only switch to third phase while on the ground
                    if(HealthBelowPct(36) && GetPhase(events) == PHASE_NORMAL)
                    {
                        DoScriptText(SAY_PHASE_3, me);
                        //events.Reset();
                        events.SetPhase(PHASE_THREE);
                        events.ScheduleEvent(EVENT_FROST_BEACON, 5000, 0, PHASE_THREE);
                        events.ScheduleEvent(EVENT_FROST_BREATH, 7000, 0, PHASE_THREE);
                        events.ScheduleEvent(EVENT_CLEAVE, 5000, 0, PHASE_THREE);
                        events.ScheduleEvent(EVENT_TAIL_SMASH, 18000, 0, PHASE_THREE);
                        events.ScheduleEvent(EVENT_UNCHAINED_MAGIC, 9000, 0, PHASE_THREE);
                        events.ScheduleEvent(EVENT_ICY_GRIP, 35000, PHASE_THREE);
                        DoCast(me, SPELL_MYSTIC_BUFFET);
                    }
                }
            }

            static uint32 GetPhase(const EventMap &em)
            {
                switch (em.GetPhaseMask())
                {
                    case 0x01: return 0;
                    case 0x02: return 1;
                    case 0x04: return 2;
                    case 0x08: return 3;
                    case 0x10: return 4;
                    case 0x20: return 5;
                    case 0x40: return 6;
                    case 0x80: return 7;
                    default:
                        return 0;
                }
           }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
                events.Update(uiDiff);
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        //Happens in any phase
                        case EVENT_BERSERK_NORMAL:
                        case EVENT_BERSERK_FLY:
                        case EVENT_BERSERK_THREE:
                        {
                            DoScriptText(SAY_BERSERK, me);
                            DoCast(me, SPELL_BERSERK);
                            events.ScheduleEvent(EVENT_BERSERK_NORMAL, 600000, 0, PHASE_NORMAL);
                            events.ScheduleEvent(EVENT_BERSERK_FLY, 600000, 0, PHASE_FLY);
                            events.ScheduleEvent(EVENT_BERSERK_THREE, 600000, 0, PHASE_THREE);
                            break;
                        }
						//Only happens in ground phase
                        case EVENT_UNCHAINED_MAGIC:
                        {
                            DoScriptText(SAY_UNCHAIND_MAGIC, me);
                            std::list<Player*> targets;
                            const std::list<HostileReference*> &threatlist = me->getThreatManager().getThreatList();
                            std::vector<Player*> manaUsers;

                            Player *pPlayer;
                            for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                                if (Unit* refTarget = (*itr)->getTarget())
                                    if (
                                        //Do not select tank - disable while in debug mode
                                        //refTarget != me->getVictim() &&
                                        refTarget->GetTypeId() == TYPEID_PLAYER)
                                    {
                                        pPlayer =  refTarget->ToPlayer();
                                        switch (pPlayer->getClass())
                                        {
                                            case CLASS_DRUID:  
                                            case CLASS_HUNTER: 
                                            case CLASS_MAGE:   
                                            case CLASS_PALADIN:
                                            case CLASS_PRIEST: 
                                            case CLASS_SHAMAN: 
                                            case CLASS_WARLOCK:
                                                //No hunters or melee hybrids were selected over 255 casts of the spell.
                                                //Perhaps it's based on total mana, requiring you to have over ~20k mana
                                                //to be a legal target? That'd rule out rets/enh/hunters. I am not certain,
                                                //but that's speculation.
                                                //http://www.tankspot.com/showthread.php?62751-Icecrown-Sindragosa&p=371547#post371547
                                                if (pPlayer->GetMaxPower(POWER_MANA) > 15000)
                                                    manaUsers.push_back(refTarget->ToPlayer());
                                                break;
                                            default:
                                                break;
                                        }
                                    }

                            uint8 maxTargets = std::min(urand(2, 4), manaUsers.size());
                            for (uint8 i = 0; i < maxTargets; ++i)
                            {
                                std::vector<Player*>::iterator itr = manaUsers.begin();
                                std::advance(itr, urand(0, manaUsers.size()-1));
                                std::swap(*manaUsers.begin(), *itr);
                            }
                            for (uint8 i = 0; i < maxTargets; ++i)
                                me->CastSpell(manaUsers[i], SPELL_UNCHAINED_MAGIC, false);
                            if (GetPhase(events) == PHASE_NORMAL)
                                events.ScheduleEvent(EVENT_UNCHAINED_MAGIC, 45000, 0, PHASE_NORMAL);
                            else
                                events.ScheduleEvent(EVENT_UNCHAINED_MAGIC, 80000, 0, PHASE_THREE);
                            break;
                        }
                        case EVENT_FROST_BREATH:
                        {
                            //DoScriptText(SAY_BREATH, me);
                            DoCastVictim(SPELL_FROST_BREATH);
                            if (GetPhase(events) == PHASE_NORMAL)
                                events.ScheduleEvent(EVENT_FROST_BREATH, 30000, 0, PHASE_NORMAL);
                            else
                                events.ScheduleEvent(EVENT_FROST_BREATH, 22000, 0, PHASE_THREE);
                            break;
                        }
                        case EVENT_CLEAVE:
                        {
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, 6000, 0, GetPhase(events));
                            break;
                        }
                        case EVENT_TAIL_SMASH:
                        {
                            DoCast(SPELL_TAIL_SMASH);
                            events.ScheduleEvent(EVENT_TAIL_SMASH, 25000, 0, GetPhase(events));
                            break;
                        }
                        case EVENT_ICY_GRIP:
                        {
                            DoCast(SPELL_ICY_TRIP_PULL);
                            events.ScheduleEvent(EVENT_BLISTERING_COLD, 500, 0, GetPhase(events));
                            break;
                        }
                        case EVENT_BLISTERING_COLD:
                        {
                            DoCast(SPELL_BLISTERING_COLD);
                            DoScriptText(SAY_BLISTERING_COLD, me);
                            if (GetPhase(events) == PHASE_NORMAL)
                                events.ScheduleEvent(EVENT_ICY_GRIP, 30000, 0, PHASE_NORMAL);
                            else
                                events.ScheduleEvent(EVENT_ICY_GRIP, 70000, 0, PHASE_THREE);
                            break;
                        }


                        //Happens in both third phase and while flying during second phase
                        case EVENT_FROST_BEACON:
                        {
                            switch (GetPhase(events))
                            {
                                case PHASE_FLY:
                                {
                                    for (uint8 i = 1; i <= RAID_MODE(2,5,2,6); ++i)
                                    {
                                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_FROST_BEACON))
                                            DoCast(target, SPELL_FROST_BEACON_EFFECT, true);
                                    }
                                    break;
                                }
                                case PHASE_THREE:
                                {
                                    //10 seconds after target gained frost beacond, it becomes ice tombed. Thus, we'll have to 
                                    //select target without ice tomb effect, instead of without frost beacon effect as above
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_ICE_TOMB))
                                        DoCast(target, SPELL_FROST_BEACON_EFFECT, true);
                                    events.ScheduleEvent(EVENT_FROST_BEACON, 15000, 0, PHASE_THREE);
                                    break;
                                }
                            }
                            break;
                        }

                        //Only happens in flying phase 
                        case EVENT_FROST_BOMB_MARK_LANDING_POSITION:
                        {
                            //Mark bomb place near random player
                            float myX, myY, myZ;
                            me->GetPosition(myX, myY, myZ);
                            Position posBombLandingSpot;
                            VMAP::IVMapManager *vMapManager = VMAP::VMapFactory::createOrGetVMapManager();
                            while (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                            {
                                target->GetRandomNearPosition(posBombLandingSpot, 25.0f);
                                if (vMapManager->isInLineOfSight(me->GetMapId(), myX, myY, myZ+2.0f
                                    , posBombLandingSpot.GetPositionX(), posBombLandingSpot.GetPositionY(), posBombLandingSpot.GetPositionZ()+2.0f))
                                    break;
                            }
                            Creature *frostBombLandingMark = DoSummon(CREATURE_TRIGGER, posBombLandingSpot, 7000, TEMPSUMMON_TIMED_DESPAWN);
                            frostBombLandingMark->CastSpell(frostBombLandingMark, SPELL_FROST_BOMB_TARGETTING_VISUAL, true);
                            events.ScheduleEvent(EVENT_FROST_BOMB_MISSILE, 2000, 0, PHASE_FLY);
                            events.ScheduleEvent(EVENT_FROST_BOMB_MARK_LANDING_POSITION, 7000, 0, PHASE_FLY);
                            break;
                        }
                        case EVENT_FROST_BOMB_MISSILE:
                        {
                            if (Unit *frostBombLandingMark = me->FindNearestCreature(CREATURE_TRIGGER, 180.0f, true))
                            {
                                float tx, ty, tz;
                                frostBombLandingMark->GetPosition(tx, ty, tz);
                                me->CastSpell(tx, ty, tz, SPELL_FROST_BOMB_MISSILE, true);
                                //DoCast(frostBombLandingMark, SPELL_FROST_BOMB_MISSILE);
                            }
                            events.ScheduleEvent(EVENT_FROST_BOMB_EXPLOSION, 2000, 0, PHASE_FLY);
                            break;
                        }
                        //case EVENT_FROST_BOMB_EXPLOSION:
                        //{
                        //    if (Unit *frostBombLandingMark = me->FindNearestCreature(CREATURE_FROST_BOMB_LANDING_MARK, 180.0f, true))
                        //    {
                        //        frostBombLandingMark->CastSpell(frostBombLandingMark, SPELL_FROST_BOMB_EXPLOSION, true);
                        //        frostBombLandingMark->CastSpell(frostBombLandingMark, SPELL_FROST_BOMB_EXPLOSION_VISUAL, true);
                        //        Position markPos;
                        //        frostBombLandingMark->GetPosition(&markPos);
                        //        frostBombLandingMark->SummonCreature(CREATURE_FROST_BOMB_EXPLOSION_VISUAL, markPos, TEMPSUMMON_TIMED_DESPAWN, 2500);
                        //        //DoCast(frostBombLandingMark, SPELL_FROST_BOMB_MISSILE);
                        //    }
                        //    break;
                        //}
                        case EVENT_GROUND_PHASE:
                        {
                            events.SetPhase(PHASE_NORMAL);
                            me->GetMotionMaster()->MovePoint(POINT_PHASE_NORMAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() - 28);
                            break;
                        }
                        case EVENT_FLY_PHASE:
                        {
                            events.SetPhase(PHASE_FLY);
                            DoScriptText(SAY_AIR_PHASE, me);
                            //me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, 0x02);
                            me->AddUnitMovementFlag(MOVEMENTFLAG_FLYING);
                            SetCombatMovement(false);
                            me->GetMotionMaster()->MovePoint(POINT_PHASE_FLY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 28);
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->SetInCombatWithZone();
                            break;
                        }
                    }
                }
                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* instance;

            //uint8 uiPhase;
            //uint8 uiBombCount;
            //uint32 uiBreathTimer;
            //uint32 uiCleaveTimer;
            //uint32 uiTailSmashTimer;
            //uint32 uiBlisteringColdTimer;
            //uint32 uiBerserkTimer;
            //uint32 uiMarkTimer;
            //uint32 uiChangePhaseTimer;
            //uint32 uiUnchainedMagicTimer;
            //uint32 uiBombTimer;

            EventMap events;
            bool bCanSwitch;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sindragosaAI(creature);
        }
};

class npc_ice_tomb : public CreatureScript
{
    public:
        npc_ice_tomb() : CreatureScript("npc_ice_tomb") { }

        struct npc_ice_tombAI: public Scripted_NoMovementAI
        {
            npc_ice_tombAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                uiTargetGUID = 0;
            }

            void SetGUID(const uint64 &guid, int32 /*id*/)
            {
                uiTargetGUID = guid;
            }

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_ASPHYXIATION, 20000);
            }

            void JustDied(Unit* killer)
            {
                if (Player* IceTomb = ObjectAccessor::GetPlayer(*me, uiTargetGUID))
                {
                    IceTomb->RemoveAurasDueToSpell(SPELL_ICE_TOMB);
                    IceTomb->RemoveAurasDueToSpell(SPELL_ASPHYXATION);
                }
            }

            void KilledUnit(Unit* victim)
            {
                me->Kill(me);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if(!uiTargetGUID)
                    return;

                if (Player* IceTomb = ObjectAccessor::GetPlayer(*me, uiTargetGUID))
                    if (!IceTomb->HasAura(SPELL_ICE_TOMB))
                        me->Kill(me);
                events.Update(uiDiff);
                if (events.ExecuteEvent() == EVENT_ASPHYXIATION)
                    if (Player* IceTomb = ObjectAccessor::GetPlayer(*me, uiTargetGUID))
                        //Occurs if a player encased in an Ice Tomb is not released within 20 seconds
                        me->CastSpell(IceTomb, SPELL_ASPHYXATION, true);
            }

        private:
            EventMap events;
            uint64 uiTargetGUID;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ice_tombAI(creature);
        }
};

class npc_rimefang : public CreatureScript
{
    public:
        npc_rimefang() : CreatureScript("npc_rimefang") { }

        struct npc_rimefangAI: public ScriptedAI
        {
            npc_rimefangAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                DoAction(ACTION_NOT_LANDED);
            }

            void Reset()
            {
                events.Reset();
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_LAND:
                        me->SetVisible(true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    case ACTION_NOT_LANDED:
                        me->SetVisible(false);
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    default:
                        ScriptedAI::DoAction(action);
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.Reset();
                events.ScheduleEvent(EVENT_FROST_BREATH, 10000, 0, PHASE_NORMAL);
                events.ScheduleEvent(EVENT_FLY_PHASE, 30000, 0, PHASE_NORMAL);
                events.SetPhase(PHASE_NORMAL);
                DoCast(me, SPELL_FROST_AURA_ADD);
            }
            void JustReachedHome()
            {
                me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, 0x02);
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);
                me->SetReactState(REACT_AGGRESSIVE);
                UnsummonSpecificCreaturesNearby(me, CREATURE_ICY_BLAST, 80.0f);
            }
            void JustDied(Unit* /*killer*/)
            {
                if(instance->GetData(DATA_SINDRAGOSA_EVENT) != DONE)
                    instance->SetData(DATA_SPAWN, instance->GetData(DATA_SPAWN)+1);
            }
            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (id)
                {
                    case POINT_PHASE_FLY:
						events.ScheduleEvent(EVENT_ICY_BLAST, 100, 0, PHASE_FLY);
						events.ScheduleEvent(EVENT_GROUND_PHASE, 15000, 0, PHASE_FLY);
                        break;
                    case POINT_PHASE_NORMAL:
                        SetCombatMovement(true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, 0x02);
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);
                        if (Unit *victim = me->SelectVictim())
                            AttackStart(victim);
                        events.ScheduleEvent(EVENT_FLY_PHASE, 50000, 0, PHASE_NORMAL);
                        events.ScheduleEvent(EVENT_FROST_BREATH, 1000, 0, PHASE_NORMAL);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
                events.Update(uiDiff);
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
						//Only happens in ground phase
                        case EVENT_FROST_BREATH:
                            DoCastVictim(SPELL_FROST_BREATH_ADD);
                            events.ScheduleEvent(EVENT_FROST_BREATH, 20000, 0, PHASE_NORMAL);
                            break;
						//Only happens in flying phase
                        case EVENT_ICY_BLAST:
                            if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            {
                                Position posBeacon;
                                pTarget->GetRandomNearPosition(posBeacon, 5.0f);
                                Unit *pBeacon = DoSummon(CREATURE_ICY_BLAST, posBeacon, 60000, TEMPSUMMON_TIMED_DESPAWN);
                                DoCast(pBeacon, SPELL_ICY_BLAST);
                            }
                            //Spam Ice blast while flying
                            events.ScheduleEvent(EVENT_ICY_BLAST, 3000, 0, PHASE_FLY);
                            break;
                        case EVENT_FLY_PHASE:
							events.Reset();
							events.SetPhase(PHASE_FLY);
                            SetCombatMovement(false);
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->SetInCombatWithZone();
                            //me->SetFlying(true);
                            //me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, 0x02);
                            me->AddUnitMovementFlag(MOVEMENTFLAG_FLYING);
                            me->GetMotionMaster()->MovePoint(POINT_PHASE_FLY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 28);
                            break;
						case EVENT_GROUND_PHASE:
							events.Reset();
							events.SetPhase(PHASE_NORMAL);
							me->GetMotionMaster()->MovePoint(POINT_PHASE_NORMAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() - 28);
							break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* instance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_rimefangAI(creature);
        }
};

class npc_spinestalker : public CreatureScript
{
    public:
        npc_spinestalker() : CreatureScript("npc_spinestalker") { }

        struct npc_spinestalkerAI: public ScriptedAI
        {
            npc_spinestalkerAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                DoAction(ACTION_NOT_LANDED);
            }

            void Reset()
            {
                events.Reset();
                me->SetStandState(UNIT_STAND_STATE_SIT);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_LAND:
                        me->SetPhaseMask(1, true); 
                        me->SendMovementFlagUpdate();
                        me->SetVisible(true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        me->SetStandState(UNIT_STAND_STATE_SIT);
                        break;
                    case ACTION_NOT_LANDED:
                        me->SetVisible(false);
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    default:
                        ScriptedAI::DoAction(action);
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.Reset();
                events.ScheduleEvent(EVENT_CLEAVE, 8000);
                events.ScheduleEvent(EVENT_TAIL_SWEEP, 10000);
                events.ScheduleEvent(EVENT_BELLOWING_ROAR, 20000);
                me->SetStandState(UNIT_STAND_STATE_STAND);
                DoCast(me, SPELL_FROST_AURA_ADD);
            }

            void JustDied(Unit* /*killer*/)
            {
                if(instance->GetData(DATA_SINDRAGOSA_EVENT) != DONE)
                    instance->SetData(DATA_SPAWN, instance->GetData(DATA_SPAWN)+1);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
                events.Update(uiDiff);
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BELLOWING_ROAR:
                        {
                            DoCastAOE(SPELL_BELLOWING_ROAR);
                            events.ScheduleEvent(EVENT_BELLOWING_ROAR, 30000);
                            break;
                        }
                        case EVENT_CLEAVE:
                        {
                            DoCastVictim(SPELL_CLEAVE_ADD);
                            events.ScheduleEvent(EVENT_CLEAVE, 5000);
                            break;
                        }
                        case EVENT_TAIL_SWEEP:
                        {
                            DoCast(SPELL_TAIL_SWEEP);
                            events.ScheduleEvent(EVENT_TAIL_SWEEP, 20000);
                            break;
                        }
                    }
                }
                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* instance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_spinestalkerAI(creature);
        }
};

class spell_sindragosa_ice_tomb : public SpellScriptLoader
{
    public:
        spell_sindragosa_ice_tomb() : SpellScriptLoader("spell_sindragosa_ice_tomb") { } //70157

        class spell_sindragosa_ice_tomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sindragosa_ice_tomb_AuraScript)

            void OnPeriodic(AuraEffect const* aurEff)
            {
                GetCaster()->CastSpell(GetTarget(), SPELL_ICE_TOMB, true);
                Creature* tomb = GetCaster()->SummonCreature(CREATURE_ICE_TOMB, GetTarget()->GetPositionX(), GetTarget()->GetPositionY(), GetTarget()->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN);
                if(tomb)
                {
                    tomb->CastSpell(tomb, SPELL_WITHOUT_ANIMATION, true);
                    tomb->SetReactState(REACT_PASSIVE);
                    tomb->AI()->SetGUID(GetTarget()->GetGUID());
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sindragosa_ice_tomb_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sindragosa_ice_tomb_AuraScript();
        }
};

class spell_sindragosa_mystic_buffet : public SpellScriptLoader
{
    public:
        spell_sindragosa_mystic_buffet() : SpellScriptLoader("spell_sindragosa_mystic_buffet") { } //70127

        class spell_sindragosa_mystic_buffet_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sindragosa_mystic_buffet_AuraScript)

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (InstanceScript* instance = GetOwner()->GetInstanceScript())
                    if(GetStackAmount() >= 5)
                        instance->SetData(DATA_ALL_YOU_CAN_EAT, FAIL);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sindragosa_mystic_buffet_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sindragosa_mystic_buffet_AuraScript();
        }
};

class spell_sindragosa_unchained_magic : public SpellScriptLoader
{
    public:
        spell_sindragosa_unchained_magic() : SpellScriptLoader("spell_sindragosa_unchained_magic") { } //69766 - instability


        class spell_sindragosa_unchained_magic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sindragosa_unchained_magic_AuraScript)

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                int32 damage = (GetStackAmount() * 2000);
                GetTarget()->CastCustomSpell(SPELL_BACKLASH, SPELLVALUE_BASE_POINT0, damage, GetTarget(), true, NULL, aurEff, GetCasterGUID());
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_sindragosa_unchained_magic_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sindragosa_unchained_magic_AuraScript();
        }
};

class spell_sindragosa_ice_tomb_effect : public SpellScriptLoader
{
    public:
        spell_sindragosa_ice_tomb_effect() : SpellScriptLoader("spell_sindragosa_ice_tomb_effect") { }


        class spell_sindragosa_ice_tomb_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sindragosa_ice_tomb_effect_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive() && GetCaster()))
                    return;

                if (GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_FROST_BEACON, true);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_sindragosa_ice_tomb_effect_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sindragosa_ice_tomb_effect_SpellScript();
        }
};

class spell_sindragosa_blistering_cold : public SpellScriptLoader
{
    public:
        spell_sindragosa_blistering_cold() : SpellScriptLoader("spell_sindragosa_blistering_cold") { }


        class spell_sindragosa_blistering_cold_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sindragosa_blistering_cold_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive() && GetCaster()))
                    return;

                GetHitUnit()->CastSpell(GetCaster(), SPELL_ICY_TRIP_JUMP, true);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_sindragosa_blistering_cold_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sindragosa_blistering_cold_SpellScript();
        }
};

class npc_icc_frostwing_mob : public CreatureScript
{
    public:
        npc_icc_frostwing_mob() : CreatureScript("npc_icc_frostwing_mob") { }

        struct npc_icc_frostwing_mobAI : public ScriptedAI
        {
            npc_icc_frostwing_mobAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }
            void JustRespawned()
            {
                if(pInstance && pInstance->GetData(DATA_SINDRAGOSA_EVENT) != DONE)
                {
                    uint32 dataType = (me->GetHomePosition().GetPositionY() < 2480.0f) ? DATA_FROSTWING_MOB_RIGHT : DATA_FROSTWING_MOB_LEFT;
                    pInstance->SetData(dataType, pInstance->GetData(dataType)+1);
                }
            }
            void JustDied(Unit* /*killer*/)
            {
                uint32 dataType = (me->GetHomePosition().GetPositionY() < 2480.0f) ? DATA_FROSTWING_MOB_RIGHT : DATA_FROSTWING_MOB_LEFT;
                pInstance->SetData(dataType, pInstance->GetData(dataType)-1);
            }
        private:
            InstanceScript *pInstance;
        };
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_icc_frostwing_mobAI(pCreature);
        }
};

class npc_icc_icy_blast : public CreatureScript
{
    public:
        npc_icc_icy_blast() : CreatureScript("npc_icc_icy_blast") { }

        struct npc_icc_icy_blastAI : public ScriptedAI
        {
            npc_icc_icy_blastAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
                bAlreadyHit = false;
            }
            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_ICY_BLAST_EFFECT, 4500);
                me->CastSpell(me, 65686, true); //Just for visual aura
            }
            void DamageTaken(Unit* attacker, uint32& damage)
            {
                damage = 0;
                me->SetHealth(me->GetMaxHealth());
            }
            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ICY_BLAST_IMPACT:
                    {
                        bAlreadyHit = true;
                        me->RemoveAurasDueToSpell(65686); //Just for visual aura
                        DoCast(me, RAID_MODE<uint32>(SPELL_ICY_BLAST_GROUND_10N, SPELL_ICY_BLAST_GROUND_25N, SPELL_ICY_BLAST_GROUND_10H, SPELL_ICY_BLAST_GROUND_25H));
                        break;
                    }
                }
            }
            // Called when hit by a spell
            void SpellHit(Unit*, const SpellEntry* sp) 
            {
                if (bAlreadyHit) 
                    return;
                if (!sp || sp->Id != SPELL_ICY_BLAST)
                    return;
                DoAction(ACTION_ICY_BLAST_IMPACT);
            }
            void UpdateAI(const uint32 uiDiff)
            {
                events.Update(uiDiff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ICY_BLAST_EFFECT:
                            DoAction(ACTION_ICY_BLAST_IMPACT);
                            break;
                        default:
                            break;
                    }
                }
                ScriptedAI::UpdateAI(uiDiff);
            }
        private:
            EventMap events;
            InstanceScript *pInstance;
            bool bAlreadyHit;
        };
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_icc_icy_blastAI(pCreature);
        }
};
class npc_icc_frost_bomb_landing_mark : public CreatureScript
{
    public:
        npc_icc_frost_bomb_landing_mark() : CreatureScript("npc_icc_frost_bomb_landing_mark") { }

        struct npc_icc_frost_bomb_landing_markAI : public ScriptedAI
        {
            npc_icc_frost_bomb_landing_markAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }
            void Reset()
            {
                //me->SetVisible(false);
                events.Reset();
                events.ScheduleEvent(EVENT_FROST_BOMB_EXPLOSION, 500);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                events.Update(uiDiff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FROST_BOMB_EXPLOSION:
                            me->CastSpell(me, SPELL_FROST_BOMB_EXPLOSION, true);
                            me->CastSpell(me, SPELL_FROST_BOMB_EXPLOSION_VISUAL, true);
                            Position markPos;
                            me->GetPosition(&markPos);
                            me->SummonCreature(CREATURE_FROST_BOMB_EXPLOSION_VISUAL, markPos, TEMPSUMMON_TIMED_DESPAWN, 2500);
                            events.ScheduleEvent(EVENT_DESPAWN, 2500);
                            if (Creature *landingMark = me->FindNearestCreature(CREATURE_TRIGGER, 10.0f, true))
                                landingMark->DespawnOrUnsummon();
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }
                ScriptedAI::UpdateAI(uiDiff);
            }
        private:
            EventMap events;
            InstanceScript *pInstance;
        };
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_icc_frost_bomb_landing_markAI(pCreature);
        }
};class spell_icy_blast : public SpellScriptLoader
{
    public:
        spell_icy_blast() : SpellScriptLoader("spell_icc_icy_blast") { }


        class spell_icy_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icy_blast_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                uint32 effVal = uint32(GetEffectValue());
                PreventHitDefaultEffect(effIndex);
                if (!(GetHitUnit() && GetHitUnit()->isAlive() && GetHitUnit()->GetEntry() == CREATURE_ICY_BLAST))
                    return;
                if (Creature *pIcyBlast = GetHitUnit()->ToCreature())
                    pIcyBlast->AI()->DoAction(ACTION_ICY_BLAST_IMPACT);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_icy_blast_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icy_blast_SpellScript();
        }
};


void AddSC_boss_sindragosa()
{
    new boss_sindragosa();
    new npc_spinestalker();
    new npc_rimefang();
    new npc_ice_tomb();
    new spell_sindragosa_ice_tomb();
    new spell_sindragosa_mystic_buffet(); //for achievement
    new spell_sindragosa_unchained_magic();
    new spell_sindragosa_ice_tomb_effect();
    new spell_sindragosa_blistering_cold();
    new spell_icy_blast();
    new npc_icc_frostwing_mob();
    new npc_icc_icy_blast();
    new npc_icc_frost_bomb_landing_mark();
}