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
#include "ulduar.h"
#include "SpellAuraEffects.h"

enum Yells
{
    SAY_AGGRO                                   = -1603220,
    SAY_SLAY_1                                  = -1603221,
    SAY_SLAY_2                                  = -1603222,
    SAY_DEATH                                   = -1603223,
    SAY_SUMMON                                  = -1603224,
    SAY_SLAG_POT                                = -1603225,
    SAY_SCORCH_1                                = -1603226,
    SAY_SCORCH_2                                = -1603227,
    SAY_BERSERK                                 = -1603228,
};

enum Spells
{
    SPELL_FLAME_JETS_10                         = 62680,
    SPELL_FLAME_JETS_25                         = 63472,
    SPELL_SCORCH_10                             = 62546,
    SPELL_SCORCH_25                             = 63474,
    SPELL_SLAG_POT_10                           = 62717,
    SPELL_SLAG_POT_25                           = 63477,
    SPELL_SLAG_IMBUED_10                        = 62836,
    SPELL_SLAG_IMBUED_25                        = 63536,
    SPELL_ACTIVATE_CONSTRUCT                    = 62488,
    SPELL_STRENGTH                              = 64473,
    SPELL_GRAB                                  = 62707,
    SPELL_BERSERK                               = 47008
};

enum Events
{
    EVENT_NONE,
    EVENT_JET,
    EVENT_SCORCH,
    EVENT_SLAG_POT,
    EVENT_GRAB_POT,
    EVENT_CHANGE_POT,
    EVENT_END_POT,
    EVENT_CONSTRUCT,
    EVENT_BERSERK
};

#define EMOTE_JETS    "Ignis the Furnace Master begins to cast Flame Jets!"

enum Mobs
{
    MOB_IRON_CONSTRUCT                          = 33121,
    GROUND_SCORCH                               = 33221
};

#define ACTION_REMOVE_BUFF                        20

enum ConstructSpells
{
    SPELL_HEAT                                  = 65667,
    SPELL_MOLTEN                                = 62373,
    SPELL_BRITTLE_10                            = 62382,
    SPELL_BRITTLE_25                            = 67114,
    SPELL_SHATTER                               = 62383,
    SPELL_GROUND_10                             = 62548,
    SPELL_GROUND_25                             = 63476
};

// Achievements
#define ACHIEVEMENT_STOKIN_THE_FURNACE        RAID_MODE(2930, 2929)
#define ACHIEVEMENT_SHATTERED                 RAID_MODE(2925, 2926)
#define MAX_ENCOUNTER_TIME                    4 * 60 * 1000

// Water coords
#define WATER_1_X                                646.77
#define WATER_2_X                                526.77
#define WATER_Y                                  277.79
#define WATER_Z                                  359.88

const Position Pos[20] =
{
    {630.366f,216.772f,360.891f,3.001970f},
    {630.594f,231.846f,360.891f,3.124140f},
    {630.435f,337.246f,360.886f,3.211410f},
    {630.493f,313.349f,360.886f,3.054330f},
    {630.444f,321.406f,360.886f,3.124140f},
    {630.366f,247.307f,360.888f,3.211410f},
    {630.698f,305.311f,360.886f,3.001970f},
    {630.500f,224.559f,360.891f,3.054330f},
    {630.668f,239.840f,360.890f,3.159050f},
    {630.384f,329.585f,360.886f,3.159050f},
    {543.220f,313.451f,360.886f,0.104720f},
    {543.356f,329.408f,360.886f,6.248280f},
    {543.076f,247.458f,360.888f,6.213370f},
    {543.117f,232.082f,360.891f,0.069813f},
    {543.161f,305.956f,360.886f,0.157080f},
    {543.277f,321.482f,360.886f,0.052360f},
    {543.316f,337.468f,360.886f,6.195920f},
    {543.280f,239.674f,360.890f,6.265730f},
    {543.265f,217.147f,360.891f,0.174533f},
    {543.256f,224.831f,360.891f,0.122173f}
};

class boss_ignis : public CreatureScript
{
public:
    boss_ignis() : CreatureScript("boss_ignis") { }

    struct boss_ignis_AI : public BossAI
    {
        boss_ignis_AI(Creature *pCreature) : BossAI(pCreature, TYPE_IGNIS), vehicle(me->GetVehicleKit())
        {
            assert(vehicle);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
        }

        Vehicle *vehicle;

        uint64 SlagPotTargetGUID ;

        uint32 EncounterTime;
        uint32 ConstructTimer;
		bool Shattered;
        bool bHasActivate;

        void Reset()
        {
            _Reset();
			summons.DespawnAll();
            if (vehicle)
                vehicle->RemoveAllPassengers();
			bHasActivate = false;

            events.Reset();
        }

        void JustReachedHome()
        {
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();
			 Map* pMap = me->GetMap();
            if (pMap->IsDungeon())
            {
		        Map::PlayerList const &lPlayers = pMap->GetPlayers();
		        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
		        {
		           Unit* pPlayer = itr->getSource();
		           if (!pPlayer) continue;
			           if (pPlayer->isAlive())
				           if (Creature* mount = pPlayer->GetVehicleCreatureBase())
						        mount->DisappearAndDie() ;
		         }
            }
            DoScriptText(SAY_AGGRO, me);
            events.ScheduleEvent(EVENT_JET, 30000);
            events.ScheduleEvent(EVENT_SCORCH, 25000);
            events.ScheduleEvent(EVENT_SLAG_POT, 35000);
            events.ScheduleEvent(EVENT_CONSTRUCT, 15000);
            events.ScheduleEvent(EVENT_END_POT, 40000);
            events.ScheduleEvent(EVENT_BERSERK, 480000);
            EncounterTime = 0;
            ConstructTimer = 0;
            Shattered = false;
        }

        void JustDied(Unit* /*victim*/)
        {
            _JustDied();
            DoScriptText(SAY_DEATH, me);

            // Achievements
            if (instance)
            {
                // Shattered
                if (Shattered)
                    instance->DoCompleteAchievement(ACHIEVEMENT_SHATTERED);
                // Stokin' the Furnace
                if (EncounterTime <= MAX_ENCOUNTER_TIME)
                    instance->DoCompleteAchievement(ACHIEVEMENT_STOKIN_THE_FURNACE);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || !CheckInRoom())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            EncounterTime += diff;
            ConstructTimer += diff;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_JET:
                        me->MonsterTextEmote(EMOTE_JETS, 0, true);
                        DoCastAOE(RAID_MODE(SPELL_FLAME_JETS_10, SPELL_FLAME_JETS_25));
                        events.ScheduleEvent(EVENT_JET, urand(35000,40000));
                        break;
                    case EVENT_SLAG_POT:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100, true))
                        {
                            DoScriptText(SAY_SLAG_POT, me);
                            SlagPotTargetGUID = pTarget->GetGUID();
                            DoCast(pTarget, SPELL_GRAB);
                            events.DelayEvents(3000);
                            events.ScheduleEvent(EVENT_GRAB_POT, 500);
                        }
                        events.ScheduleEvent(EVENT_SLAG_POT, RAID_MODE(30000, 15000));
                        break;
                    case EVENT_GRAB_POT:
                        if(Player* SlagPotTarget = Unit::GetPlayer(*me, SlagPotTargetGUID))
					    {
						    if (SlagPotTarget && SlagPotTarget->isAlive())
						    {
							    SlagPotTarget->EnterVehicle(me, 0);
							    events.CancelEvent(EVENT_GRAB_POT);
							    events.ScheduleEvent(EVENT_CHANGE_POT, 1000);
						    }
					    }
                        break;
                    case EVENT_CHANGE_POT:
                        if(Player* SlagPotTarget = Unit::GetPlayer(*me, SlagPotTargetGUID))
					    {
						    if (SlagPotTarget && SlagPotTarget->isAlive())
						    {
							    SlagPotTarget->AddAura(RAID_MODE(SPELL_SLAG_POT_10, SPELL_SLAG_POT_25), SlagPotTarget);
							    SlagPotTarget->EnterVehicle(me, 1);
							    events.CancelEvent(EVENT_CHANGE_POT);
							    events.ScheduleEvent(EVENT_END_POT, 10000);
						    }
					    }
                        break;
                    case EVENT_END_POT:
                        if(Player* SlagPotTarget = Unit::GetPlayer(*me, SlagPotTargetGUID))
					    {
						    if (SlagPotTarget && SlagPotTarget->isAlive())
						    {
							    SlagPotTarget->ExitVehicle();
							    SlagPotTarget->CastSpell(SlagPotTarget, RAID_MODE(SPELL_SLAG_IMBUED_10, SPELL_SLAG_IMBUED_25), true);
							    SlagPotTarget = 0;
							    events.CancelEvent(EVENT_END_POT);
						    }
					    }
                        break;
                    case EVENT_SCORCH:
                        DoScriptText(RAND(SAY_SCORCH_1, SAY_SCORCH_2), me);
                        if (Unit *pTarget = me->getVictim())
                            if (Creature* pScorchTarget = me->SummonCreature(GROUND_SCORCH, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 45000))
                                me->AddAura(RAID_MODE(SPELL_GROUND_10, SPELL_GROUND_25), pScorchTarget);
                        DoCast(RAID_MODE(SPELL_SCORCH_10, SPELL_SCORCH_25));
                        events.ScheduleEvent(EVENT_SCORCH, 25000);
                        break;
                    case EVENT_CONSTRUCT:
                    {
                        if (!bHasActivate)
                        {
                            DoScriptText(SAY_SUMMON, me);
                            DoCast(me, SPELL_ACTIVATE_CONSTRUCT);
                            bHasActivate = true;
                            events.ScheduleEvent(EVENT_CONSTRUCT, 1000);
                        }
                        else
                        {
                            DoSummon(MOB_IRON_CONSTRUCT, Pos[rand()%20], 30000, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT);
                            me->AddAura(SPELL_STRENGTH,me);
                            bHasActivate = false;
                            events.ScheduleEvent(EVENT_CONSTRUCT, RAID_MODE(40000, 30000));
                        }
                        /*
					    if(Creature* construct = Creature::GetCreature(*me, instance->GetData64(DATA_RANDOM_IRON_CONSTRUCT)))
					    {
                            construct->setActive(true);
						    construct->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						    construct->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
						    DoCast(construct, SPELL_ACTIVATE_CONSTRUCT);
					    }
                        */
                        break;
					}
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        DoScriptText(SAY_BERSERK, me);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }

        void KilledUnit(Unit* /*victim*/)
        {
            if (!(rand()%5))
                DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
        }

        void JustSummoned(Creature *summon)
        {
            if (summon->GetEntry() == MOB_IRON_CONSTRUCT)
            {
                summon->SetReactState(REACT_AGGRESSIVE);
                summon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED | UNIT_FLAG_STUNNED | UNIT_FLAG_DISABLE_MOVE);
            }
            summon->AI()->AttackStart(me->getVictim());
            summon->AI()->DoZoneInCombat();
            summons.Summon(summon);
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_REMOVE_BUFF:
                    me->RemoveAuraFromStack(SPELL_STRENGTH);
                    // Shattered Achievement
                    if (ConstructTimer >= 5000)
                        ConstructTimer = 0;
                    else
                        Shattered = true;
                    break;
            }
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_ignis_AI (pCreature);
    }
};

class npc_iron_construct : public CreatureScript
{
public:
    npc_iron_construct() : CreatureScript("npc_iron_construct") { }

    struct npc_iron_constructAI : public ScriptedAI
    {
        npc_iron_constructAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
        }

        InstanceScript* pInstance;
        bool Brittled;
		bool active;

        void Reset()
        {
		    me->SetReactState(REACT_PASSIVE);
		    me->SetUInt32Value(UNIT_NPC_EMOTESTATE,69);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE) ;
		    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE) ;
            Brittled = false;
        }

        void DamageTaken(Unit *attacker, uint32 &damage)
        {
            if (me->GetMotionMaster()->GetMotionSlotType(MOTION_SLOT_ACTIVE) == POINT_MOTION_TYPE)
                me->GetMotionMaster()->MovementExpired();

            if (me->HasAura(RAID_MODE(SPELL_BRITTLE_10, SPELL_BRITTLE_25)) && damage >= RAID_MODE(5000, 3000))
            {
                DoCast(attacker, SPELL_SHATTER, true);
                if (pInstance)
                    if (Creature *pIgnis = me->GetCreature(*me, pInstance->GetData64(DATA_IGNIS)))
                        if (pIgnis->AI())
                            pIgnis->AI()->DoAction(ACTION_REMOVE_BUFF);
                me->ForcedDespawn(500);
            }
        }

		void JustDied(Unit* pKiller)
        {
            if (pInstance)
                if (Creature *pIgnis = me->GetCreature(*me, pInstance->GetData64(DATA_IGNIS)))
                    if (pIgnis->AI())
                        pIgnis->AI()->DoAction(ACTION_REMOVE_BUFF);
        }

	    /*void SpellHit(Unit* caster,const SpellEntry *spell)
	    {
		    if(!active && spell->Id==SPELL_ACTIVATE_CONSTRUCT)
		    {
			    caster->AddAura(SPELL_STRENGTH,caster);
			    me->SetReactState(REACT_AGGRESSIVE);
			    if (caster->getVictim())
			    {
				    me->SetInCombatWith(caster->getVictim());
				    caster->getVictim()->SetInCombatWith(me);
				    me->AddThreat(caster->getVictim(), 0.0f);
                    me->GetMotionMaster()->MovePoint(1, caster->getVictim()->GetPositionX(), caster->getVictim()->GetPositionY(), caster->getVictim()->GetPositionZ()); 
			    }
			    active = true ;
			    DoZoneInCombat();
		    }
	    }*/

        void UpdateAI(const uint32 /*uiDiff*/)
        {
            Map *cMap = me->GetMap();

            if (me->HasAura(SPELL_MOLTEN) && me->HasAura(SPELL_HEAT))
                me->RemoveAura(SPELL_HEAT);

            if (Aura * aur = me->GetAura((SPELL_HEAT), GetGUID()))
            {
                if (aur->GetStackAmount() >= 10)
                {
                    me->RemoveAura(SPELL_HEAT);
                    DoCast(SPELL_MOLTEN);
                    Brittled = false;
                }
            }
            // Water pools
            if(cMap->GetId() == 603 && !Brittled && me->HasAura(SPELL_MOLTEN))
			{
                if (me->GetDistance(WATER_1_X, WATER_Y, WATER_Z) <= 18 || me->GetDistance(WATER_2_X, WATER_Y, WATER_Z) <= 18)
                {
                    DoCast(SPELL_BRITTLE_10);
                    me->RemoveAura(SPELL_MOLTEN);
                    Brittled = true;
                }
			}
            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_iron_constructAI (pCreature);
    }
};
class npc_scorch_ground : public CreatureScript
{
public:
    npc_scorch_ground() : CreatureScript("npc_scorch_ground") { }

    struct mob_scorch_targetAI : public Scripted_NoMovementAI
    {
        mob_scorch_targetAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
        }

        void Reset()
        {
            if (me->GetDistance(WATER_1_X, WATER_Y, WATER_Z) <= 18 || me->GetDistance(WATER_2_X, WATER_Y, WATER_Z) <= 18)
                me->DisappearAndDie();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_scorch_targetAI(pCreature);
    }
};

enum SlagPotSpells
{
    SPELL_SLAG_POT                           = 62717,
    SPELL_SLAG_POT_DAMAGE                    = 65722,
    SPELL_SLAG_IMBUED                        = 62836
};

class spell_ignis_slag_pot : public SpellScriptLoader
{
    public:
        spell_ignis_slag_pot() : SpellScriptLoader("spell_ignis_slag_pot") { }

        class spell_ignis_slag_pot_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ignis_slag_pot_AuraScript)
            bool Validate(SpellEntry const * /*spellEntry*/)
            {
                if (!sSpellStore.LookupEntry(SPELL_SLAG_POT_DAMAGE))
                    return false;
                if (!sSpellStore.LookupEntry(SPELL_SLAG_POT))
                    return false;
                if (!sSpellStore.LookupEntry(SPELL_SLAG_IMBUED))
                    return false;
                return true;
            }

            void HandleEffectPeriodic(AuraEffect const * aurEff)
            {
                Unit* aurEffCaster = aurEff->GetCaster();
                if (!aurEffCaster)
                    return;

                Unit * target = GetTarget();
                aurEffCaster->CastSpell(target, SPELL_SLAG_POT_DAMAGE, true);
                if (target->isAlive() && !GetDuration())
                     target->CastSpell(target, SPELL_SLAG_IMBUED, true);
            }

			void ApplyEffect(AuraEffect const* /*aurEff*/, AuraApplication const* aurApp, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* pTarget = aurApp->GetTarget())
                    pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_SCHOOL_DAMAGE, true);
            }

            void RemoveEffect(AuraEffect const* /*aurEff*/, AuraApplication const* aurApp, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* pTarget = aurApp->GetTarget())
                    pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_SCHOOL_DAMAGE, false);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ignis_slag_pot_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
				OnEffectApply += AuraEffectApplyFn(spell_ignis_slag_pot_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_ignis_slag_pot_AuraScript::RemoveEffect, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ignis_slag_pot_AuraScript();
        }
};

void AddSC_boss_ignis()
{
    new boss_ignis();
    new npc_iron_construct();
    new npc_scorch_ground();
    new spell_ignis_slag_pot();
}
