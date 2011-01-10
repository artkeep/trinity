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

#define GAMEOBJECT_GIFT_OF_THE_OBSERVER 194821

enum Spells
{
    SPELL_ASCEND                    = 64487,
    SPELL_BIG_BANG                  = 64443,
    H_SPELL_BIG_BANG                = 64584,
    SPELL_COSMIC_SMASH              = 62301,
    H_SPELL_COSMIC_SMASH            = 64598,
    SPELL_PHASE_PUNCH               = 64412,
    SPELL_QUANTUM_STRIKE            = 64395,
    H_SPELL_QUANTUM_STRIKE          = 64592,
    SPELL_BLACK_HOLE_EXPLOSION      = 64122,
	H_SPELL_BLACK_HOLE_EXPLOSION    = 65108,
    SPELL_ARCANE_BARAGE             = 64599,
    H_SPELL_ARCANE_BARAGE           = 64607,
    SPELL_BLACK_HOLE_PASSIVE        = 46228,
    SPELL_BLACK_HOLE_EFFECT         = 46230,
    SPELL_PHASE_PUNCH_EFFECT        = 64417,
    SPELL_SINGULARITY               = 46238,
    SPELL_BLACK_HOLE_PHASE          = 62168,
    SPELL_BLACK_HOLE_DAMAGE         = 62169,
    SPELL_BLACK_HOLE_SPAWN_VISUAL   = 62003,
    SPELL_SUMMON_BLACK_HOLE         = 62189,
    SPELL_ALGALON_EVENT_CLIMAX      = 64580,
    SPELL_COSMIC_SMASH_EFFECT       = 62311,
    H_SPELL_COSMIC_SMASH_EFFECT     = 64596,
    SPELL_SUMMON_AZEROTH            = 64994,
    SPELL_ALGALON_EVENT_BEAM        = 64367
};

enum Creatures
{
    CREATURE_COLLAPSING_STAR        = 32955,
    CREATURE_BLACK_HOLE             = 32953,
    CREATURE_LIVING_CONSTELLATION   = 33052,
    CREATURE_DARK_MATTER            = 33089,
    CREATURE_UNLEASHED_DARK_MATTER  = 34097
};

enum Data
{
    DATA_PHASE
};

enum Yells
{
    SAY_AGGRO                                   = -1603000,
    SAY_SLAY_1                                  = -1603001,
    SAY_SLAY_2                                  = -1603002,
    SAY_ENGADED_FOR_FIRTS_TIME                  = -1603003,
    SAY_PHASE_2                                 = -1603004,
    SAY_SUMMON_COLLAPSING_STAR                  = -1603005,
    SAY_DEATH_1                                 = -1603006,
    SAY_DEATH_2                                 = -1603007,
    SAY_DEATH_3                                 = -1603008,
    SAY_DEATH_4                                 = -1603009,
    SAY_DEATH_5                                 = -1603010,
    SAY_BERSERK                                 = -1603011,
    SAY_BIG_BANG_1                              = -1603012,
    SAY_BIG_BANG_2                              = -1603013,
    SAY_TIMER_1                                 = -1603014,
    SAY_TIMER_2                                 = -1603015,
    SAY_TIMER_3                                 = -1603016,
    SAY_SUMMON_1                                = -1603017,
    SAY_SUMMON_2                                = -1603018,
    SAY_SUMMON_3                                = -1603019,
};

struct Locations
{
    float x, y, z;
};


static Locations BlackHolesPos[4]=
{
    {1595.22f, -310.01f, 417.45f},
    {1633.53f, -273.86f, 417.45f},
    {1666.99f, -307.47f, 417.45f},
    {1632.69f, -339.36f, 417.32f}
};

class boss_algalon : public CreatureScript
{
public:
    boss_algalon() : CreatureScript("boss_algalon") { }

    struct boss_algalonAI : public BossAI
    {
        boss_algalonAI(Creature *c) : BossAI(c, TYPE_ALGALON)
        {
            Summon = false;
            DoCast(SPELL_SUMMON_AZEROTH);
            DoCast(SPELL_ALGALON_EVENT_BEAM);
        }

        std::list<uint64> m_lCollapsingStarGUIDList;
        std::list<uint64> m_lLivingConstellationGUIDList;

        uint8 uiPhase;
        uint32 uiAscendTimer;
        uint32 uiBigBangTimer;
        uint32 uiCosmicSmashTimer;
        uint32 uiPhasePunchTimer;
        uint32 uiQuantumStrikeTimer;
        uint32 uiCollapsingStarsTimer;
        uint32 uiLivingConstellationsTimer ;
        uint32 uiBlackHolesTimer ;
        uint32 uiPhaseTimer;
        uint32 uiStep;

        bool bHasSpawnedBlackHoles ;
        bool bIsEnraged;
        bool Summon;
		bool hasHit;
        bool hasCastedBigBang;

        void EnterCombat(Unit* who)
        {
		    _EnterCombat();
            if (Summon)
            {
                if (instance)
                    instance->SetData(TYPE_UNIVERSE_GLOBE, GO_STATE_ACTIVE);
                DoZoneInCombat(who->ToCreature());
            }
            else
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                uiStep = 1;
            }
        }

        void KilledUnit(Unit *victim)
        {
            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2), me);
        }

        void Reset()
        {
            _Reset();
            uiPhase = 1;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            uiPhaseTimer = 0;
            uiAscendTimer = 360000;
            uiQuantumStrikeTimer = 4000 + rand()%10000;
            uiCollapsingStarsTimer = 15000;
            uiLivingConstellationsTimer = 30000 ;
            uiBigBangTimer = 90000;
            uiPhasePunchTimer = 15000;
            uiCosmicSmashTimer = urand(30000, 60000);
            bIsEnraged = false;
            bHasSpawnedBlackHoles = false ;
            hasHit = false ;
            DespawnCollapsingStar() ;
            DespawnLivingConstellation() ;
        }

        void DespawnCollapsingStar()
        {
            if (m_lCollapsingStarGUIDList.empty())
                return;

            for (std::list<uint64>::const_iterator itr = m_lCollapsingStarGUIDList.begin(); itr != m_lCollapsingStarGUIDList.end(); ++itr)
            {
                if (Creature* pTemp = Unit::GetCreature(*me, *itr))
                {
                    if (pTemp->isAlive())
                        pTemp->ForcedDespawn();
                }
            }
            m_lCollapsingStarGUIDList.clear();
        }

		void DespawnLivingConstellation()
        {
            if (m_lLivingConstellationGUIDList.empty())
                return;

            for(std::list<uint64>::const_iterator itr = m_lLivingConstellationGUIDList.begin(); itr != m_lLivingConstellationGUIDList.end(); ++itr)
            {
                if (Creature* pTemp = Unit::GetCreature(*me, *itr))
                {
                    if (pTemp->isAlive())
                        pTemp->ForcedDespawn();
                }
            }
            m_lCollapsingStarGUIDList.clear();
        }

        void JustSummoned(Creature* pSummoned)
        {
            if (pSummoned->GetEntry() == CREATURE_COLLAPSING_STAR)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    pSummoned->AI()->AttackStart(pTarget);
                m_lCollapsingStarGUIDList.push_back(pSummoned->GetGUID());
            }
			if (pSummoned->GetEntry() == CREATURE_LIVING_CONSTELLATION)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    pSummoned->AI()->AttackStart(pTarget);
                m_lLivingConstellationGUIDList.push_back(pSummoned->GetGUID());
            }
        }

        void JumpToNextStep(uint32 time)
        {
            uiPhaseTimer = time ;
            uiStep ++ ;
        }

        void SpellHitTarget(Unit* who, const SpellEntry* spell)
        {
            if(spell->Id == RAID_MODE(SPELL_BIG_BANG,H_SPELL_BIG_BANG))
                hasHit = true ;
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if (uiPhase == 1)
            {
                if (!Summon)
                {
                    if (uiPhaseTimer <= diff)
                    {
                        switch(uiStep)
                        {
                            case 1:
                                DoScriptText(SAY_SUMMON_1, me);
                                JumpToNextStep(6000);
                                break;
                            case 2:
                                DoScriptText(SAY_SUMMON_2, me);
                                JumpToNextStep(5000);
                                break;
                            case 3:
                                DoScriptText(SAY_SUMMON_3, me);
                                JumpToNextStep(9000);
                                break;
                            case 4:
                                DoScriptText(SAY_ENGADED_FOR_FIRTS_TIME, me);
                                JumpToNextStep(8000);
                                break;
                            case 5:
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                me->SetReactState(REACT_AGGRESSIVE);
                                Summon = true;
								if (instance)
                                    instance->SetData(TYPE_UNIVERSE_GLOBE, GO_STATE_ACTIVE);
                                break;
                        }
                    } else uiPhaseTimer -= diff;

                    return;
                }

                if (uiCollapsingStarsTimer <= diff)
                {
                    DoScriptText(SAY_SUMMON_COLLAPSING_STAR, me);
                    for (uint8 i = 0 ; i < 4 ; i++)
                    {
                        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                                me->SummonCreature(CREATURE_COLLAPSING_STAR,target->GetPositionX()+15.0f,target->GetPositionY()+15.0f,target->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN, 100000);
                    }
                    uiCollapsingStarsTimer = 90000 ;
                } else uiCollapsingStarsTimer -= diff ;

                if (uiLivingConstellationsTimer <= diff)
                {
                    for (uint8 i = 0 ; i < 3 ; i++)
                    {
                        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                                me->SummonCreature(CREATURE_LIVING_CONSTELLATION,target->GetPositionX()+15.0f,target->GetPositionY()+15.0f,target->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN, 100000);
                    }
                    uiLivingConstellationsTimer = 90000 ;
                } else uiLivingConstellationsTimer -= diff ;

                if (HealthBelowPct(20))
                {
                    uiPhase = 2;
                    uiBlackHolesTimer = 5000 ;
                    DespawnCollapsingStar();
                    DespawnLivingConstellation();
                    DoScriptText(SAY_PHASE_2, me);
                }
            }
            if (uiPhase == 2)
            {
                if(!bHasSpawnedBlackHoles)
                {
                    if (uiBlackHolesTimer <= diff)
                    {
                        for (uint8 i = 0 ; i < 4 ; i++)
                        {
                            if(Creature* blackHole = me->SummonCreature(CREATURE_BLACK_HOLE,BlackHolesPos[i].x, BlackHolesPos[i].y, BlackHolesPos[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 100000))
                                blackHole->AI()->SetData(DATA_PHASE, uiPhase) ;
                            bHasSpawnedBlackHoles = true ;
                        }
                        uiBlackHolesTimer = 100000 ;
                    } else uiBlackHolesTimer -= diff ;
                }

                if (HealthBelowPct(2))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAllAuras();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    Summon = false ;
                    uiStep = 1;
                }

                if (!Summon)
                {
                    if (uiPhaseTimer <= diff)
                    {
                        switch(uiStep)
                        {
                            case 1:
                                DoScriptText(SAY_DEATH_1, me);
                                JumpToNextStep(3000);
                                break;
                            case 2:
                                DoScriptText(SAY_DEATH_2, me);
                                JumpToNextStep(3000);
                                break;
                            case 3:
                                DoScriptText(SAY_DEATH_3, me);
                                JumpToNextStep(3000);
                                break;
                            case 4:
                                DoScriptText(SAY_DEATH_4, me);
                                JumpToNextStep(3000);
                                break;
                            case 5:
                                DoScriptText(SAY_DEATH_5, me);
                                JumpToNextStep(3000);
                                break;
                            case 6:
                                me->SummonGameObject(GAMEOBJECT_GIFT_OF_THE_OBSERVER, 1634.258667f, -295.101166f, 417.321381f,0,0,0,0,0,-10);
                                me->DisappearAndDie() ;
                                _JustDied();
                                break;
                        }
                    } else uiPhaseTimer -= diff;

                    return;
                }
            }

            if (!hasCastedBigBang)
            {
                if (uiBigBangTimer <= diff)
                {
                    DoScriptText(RAND(SAY_BIG_BANG_1,SAY_BIG_BANG_2), me);
                    DoCast(me->getVictim(), RAID_MODE(SPELL_BIG_BANG,H_SPELL_BIG_BANG));
                    hasCastedBigBang = true ;
                    uiBigBangTimer = 9000;
                } else uiBigBangTimer -= diff;
            }
            else
            {
                if (uiBigBangTimer <= diff)
                {
                    if(!hasHit)
                        uiAscendTimer = 0 ;
                    hasHit = false ;
                    hasCastedBigBang = false ;
                    uiBigBangTimer = 81000;
                } else uiBigBangTimer -= diff;
            }


            if (uiQuantumStrikeTimer <= diff)
            {
                DoCast(me->getVictim(), RAID_MODE(SPELL_QUANTUM_STRIKE,H_SPELL_QUANTUM_STRIKE));

                uiQuantumStrikeTimer = urand(4000, 14000);
            } else uiQuantumStrikeTimer -= diff;

            if (uiPhasePunchTimer <= diff)
            {
                DoCast(me->getVictim(),SPELL_PHASE_PUNCH);
                if(me->getVictim())
                    if(Aura* phasePunch = me->getVictim()->GetAura(SPELL_PHASE_PUNCH))
                    {
                        if(phasePunch->GetStackAmount() == 5)
                        {
                            me->AddAura(SPELL_PHASE_PUNCH_EFFECT, me->getVictim());
                            me->getVictim()->RemoveAurasDueToSpell(SPELL_PHASE_PUNCH) ;
                        }
                    }
                uiPhasePunchTimer = 15000;
            } else uiPhasePunchTimer -= diff;

            if (uiCosmicSmashTimer <= diff)
            {
                if (instance)
                {
                    for (uint8 i = 0 ; i < RAID_MODE(1, 3) ; i++)
                    {
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, RAID_MODE(SPELL_COSMIC_SMASH, H_SPELL_COSMIC_SMASH));
                    }
                }
                uiCosmicSmashTimer = 25000;
            } else uiCosmicSmashTimer -= diff;

            if (uiAscendTimer  <= diff)
            {
                DoCast(me, SPELL_ASCEND);
                if(instance)
                {
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLACK_HOLE_DAMAGE);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLACK_HOLE_PHASE);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHASE_PUNCH_EFFECT);
                }
                DoScriptText(SAY_BERSERK, me);
                uiAscendTimer = urand(360000,365000);
            } else uiAscendTimer -= diff;

            // Don't attack current target if he's not visible for us.
            if(me->getVictim() && me->getVictim()->GetPhaseMask() != 1)
                me->getThreatManager().modifyThreatPercent(me->getVictim(), -100);

            DoMeleeAttackIfReady();

			EnterEvadeIfOutOfCombatArea(diff);
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_algalonAI(pCreature);
    }
};

class mob_collapsing_star : public CreatureScript
{
public:
    mob_collapsing_star() : CreatureScript("mob_collapsing_star") { }

    struct mob_collapsing_starAI : public ScriptedAI
    {
        mob_collapsing_starAI(Creature *pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 healthPercent ;
        uint32 uiHealthLostTimer ;

        void Reset()
        {
            healthPercent = me->GetMaxHealth()/100 ;
            uiHealthLostTimer = 1000 ;
        }

        void JustDied(Unit* who)
        {
            me->CastSpell(me, RAID_MODE(SPELL_BLACK_HOLE_EXPLOSION, H_SPELL_BLACK_HOLE_EXPLOSION), true);
            if(Creature* blackHole = me->SummonCreature(CREATURE_BLACK_HOLE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN, 27000))
                blackHole->AI()->SetData(DATA_PHASE, 1) ;
        }

        void SpellHitTarget(Unit* target, const SpellEntry* spell)
        {
            if(spell->Id == RAID_MODE(SPELL_BLACK_HOLE_EXPLOSION, H_SPELL_BLACK_HOLE_EXPLOSION))
                me->DealDamage(target, RAID_MODE(urand(16088, 16912),urand(20475, 21525)), 0, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            // Don't attack current target if he's not visible for us.
            if(me->getVictim() && me->getVictim()->GetPhaseMask() != 1)
                me->getThreatManager().modifyThreatPercent(me->getVictim(), -100);

            if (uiHealthLostTimer <= diff)
            {
                if (me->GetHealth() > healthPercent)
                    me->SetHealth(me->GetHealth()-healthPercent);
                else
                    me->DealDamage(me, me->GetHealth());
                uiHealthLostTimer = 1000 ;
            } else uiHealthLostTimer -= diff ;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_collapsing_starAI(pCreature);
    }
};

class mob_black_hole : public CreatureScript
{
public:
    mob_black_hole() : CreatureScript("mob_black_hole") { }

    struct mob_black_holeAI : public ScriptedAI
    {
        mob_black_holeAI(Creature *pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint8 uiPhase ;
        uint32 uiDarkMatterSpawnTimer ;

        void SetData(uint32 id, uint32 value)
        {
            if (id == DATA_PHASE)
            {
                uiPhase = value ;
                if(uiPhase == 2)
                    uiDarkMatterSpawnTimer = 30000 ;
            }
        }

        void SpellHitTarget(Unit* target, const SpellEntry* spell)
        {
            if (spell->Id == SPELL_SINGULARITY)
            {
                if(target && target->GetTypeId() == TYPEID_PLAYER)
                {
                    me->AddAura(SPELL_BLACK_HOLE_PHASE, target) ;
                    me->AddAura(SPELL_BLACK_HOLE_DAMAGE, target) ;
                }
            }
        }

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            DoCast(SPELL_BLACK_HOLE_SPAWN_VISUAL) ;
            DoCast(SPELL_BLACK_HOLE_PASSIVE) ;
            uiDarkMatterSpawnTimer = urand(1000, 4000) ;
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiDarkMatterSpawnTimer <= diff)
            {
                if(uiPhase == 2)
                {
                    if(Creature* darkMatter = me->SummonCreature(CREATURE_UNLEASHED_DARK_MATTER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000))
                        darkMatter->SetInCombatWithZone() ;
                }
                if(Creature* darkMatter = me->SummonCreature(CREATURE_DARK_MATTER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000))
                {
                    darkMatter->SetPhaseMask(16, true) ;
                    darkMatter->SetInCombatWithZone() ;
                }
                uiDarkMatterSpawnTimer = 30000 ;
            } else uiDarkMatterSpawnTimer -= diff ;

            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_black_holeAI(pCreature);
    }
};

class mob_living_constellation : public CreatureScript
{
public:
    mob_living_constellation() : CreatureScript("mob_living_constellation") { }

    struct mob_living_constellationAI : public ScriptedAI
    {
        mob_living_constellationAI(Creature *pCreature) : ScriptedAI(pCreature)
        {
        }

        uint32 uiArcaneBarrageTimer ;

        void Reset()
        {
            uiArcaneBarrageTimer = urand(5000, 8000) ;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            // Don't attack current target if he's not visible for us.
            if(me->getVictim() && me->getVictim()->GetPhaseMask() != 1)
                me->getThreatManager().modifyThreatPercent(me->getVictim(), -100);

            if (uiArcaneBarrageTimer <= diff)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, RAID_MODE(SPELL_ARCANE_BARAGE, H_SPELL_ARCANE_BARAGE)) ;
                uiArcaneBarrageTimer = urand(9000, 12000) ;
            } else uiArcaneBarrageTimer -= diff ;

            if (Creature* blackHole = me->FindNearestCreature(CREATURE_BLACK_HOLE, 5.0f))
            {
                me->DisappearAndDie() ;
                blackHole->ForcedDespawn() ;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_living_constellationAI(pCreature);
    }
};

class mob_algalon_stalker_asteroid_target : public CreatureScript
{
public:
    mob_algalon_stalker_asteroid_target() : CreatureScript("mob_algalon_stalker_asteroid_target") { }

    struct mob_algalon_stalker_asteroid_targetAI : public Scripted_NoMovementAI
    {
        mob_algalon_stalker_asteroid_targetAI(Creature *pCreature) : Scripted_NoMovementAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            SetCombatMovement(false);
        }

        uint32 uiCosmicSmashTimer ;

        void Reset()
        {
            uiCosmicSmashTimer = 6000 ;
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiCosmicSmashTimer <= diff)
            {
                Unit *pTarget = NULL;
                uint32 damage ;
                std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
                std::vector<Unit *> target_list;
                for (std::list<HostileReference *>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {
                    pTarget = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                    if (!pTarget)
                        continue;

                    if (pTarget->GetPhaseMask() != 1)
                        continue;

                    if(me->GetDistance2d(pTarget) <= 1.0f)
                        damage = RAID_MODE(43000, 55000) ;
                    else
                        damage = RAID_MODE(float(43000)/me->GetDistance2d(pTarget), float(55000)/me->GetDistance2d(pTarget)) ;
                    me->SendSpellNonMeleeDamageLog(pTarget,RAID_MODE(SPELL_COSMIC_SMASH_EFFECT, H_SPELL_COSMIC_SMASH_EFFECT),damage,SPELL_SCHOOL_MASK_FIRE,0,0,false,0,false);
                    me->DealDamage(pTarget, damage, 0, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FIRE);
                }
                me->DisappearAndDie();
            } else uiCosmicSmashTimer -= diff ;

            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_algalon_stalker_asteroid_targetAI(pCreature);
    }
};

enum eAccess
{
    ITEM_CELESTIAL_PLANETARIUM_KEY             = 45796,
    ITEM_HEROIC_CELESTIAL_PLANETARIUM_KEY      = 45798,
};

class go_celestial_planetarium_access : public CreatureScript
{
public:
    go_celestial_planetarium_access() : CreatureScript("go_celestial_planetarium_access") { }

    bool OnGossipHello(Player* pPlayer, GameObject* pGo)
    {
        InstanceScript* instance = pGo->GetInstanceScript();

        if (!instance)
            return false;

        switch(pGo->GetEntry())
        {
            case 194628:
                if (pPlayer->HasItemCount(ITEM_CELESTIAL_PLANETARIUM_KEY, 1))
                    instance->SetData(TYPE_ALGALON, TO_BE_DECIDED);
                pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                break;
            case 194752:
                if (pPlayer->HasItemCount(ITEM_HEROIC_CELESTIAL_PLANETARIUM_KEY, 1))
                    instance->SetData(TYPE_ALGALON, TO_BE_DECIDED);
                pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                break;
        }

        return true;
    }
};

void AddSC_boss_algalon()
{
    new boss_algalon();
    new mob_collapsing_star();
	new mob_black_hole();
    new mob_living_constellation();
    new mob_algalon_stalker_asteroid_target();
    new go_celestial_planetarium_access();
}
