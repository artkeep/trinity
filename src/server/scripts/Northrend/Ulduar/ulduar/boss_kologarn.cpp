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
#include "Vehicle.h"

enum RZnums
{
    //yells
    SAY_AGGRO                       = -1603230,
    SAY_SHOCKWAVE                   = -1603235,
    SAY_GRABB                       = -1603236,
    SAY_LEFT_DIED                   = -1603233,
    SAY_RIGHT_DIED                  = -1603234,
    SAY_SLAY_01                     = -1603231,
    SAY_SLAY_02                     = -1603232,
    SAY_BERSERK                     = -1603238, //a mettre, mode berserk? jcrois pas inutile
    SAY_DIED                        = -1603237,
    EMOTE_DIED                      = -1890032,
    EMOTE_STONE                     = -1890044,
    EMOTE_RA                        = -1890045,
    EMOTE_LA                        = -1890046,
    //kologarn
    SPELL_OVERHEAD_SMASH            = 63356,
    SPELL_OVERHEAD_SMASH_H          = 64003,
    SPELL_ONE_ARMED_SMASH           = 63573,
    SPELL_ONE_ARMED_SMASH_H         = 64006,
    SPELL_STONE_SHOUT               = 63720, //origine 63716
    SPELL_STONE_SHOUT_H             = 64004, //origine 64005
    SPELL_PETRIFYING_BREATH         = 62030,
    SPELL_PETRIFYING_BREATH_H       = 63980,
    //bLeft arm
    SPELL_SHOCKWAVE                 = 63783,
    SPELL_SHOCKWAVE_H               = 63982,
    //bRight arm
    SPELL_STONE_AURA                = 64290,
    SPELL_STONE_AURA_2              = 62056,
    SPELL_STONE_AURA_H              = 64292,
    SPELL_STONE_AURA_H_2            = 63985,
    SPELL_STONE_GRIP_VISUAL         = 63962,
    SPELL_STONE_GRIP                = 62166,
    SPELL_STONE_GRIP_H              = 63981,
    //both
    SPELL_ARM_VISUAL                = 64753,
    //rubble
    SPELL_RUMBLE                    = 63818,
    SPELL_STONE_NOVA                = 63978,
    //NPC ids
    MOB_RUBBLE                      = 33768,
    NPC_LEFT_EYE                    = 33632,
    NPC_RIGHT_EYE                   = 33802,
    //npc focus eye
    SPELL_FOCUSED_EYEBEAM_EFFECT_N      = 63346,
    SPELL_FOCUSED_EYEBEAM_PERIODIC_N    = 63347,
    SPELL_FOCUSED_EYEBEAM_EFFECT_H      = 63976,
    SPELL_FOCUSED_EYEBEAM_PERIODIC_H    = 63977,
    SPELL_FOCUSED_EYEBEAM_VISUAL_RIGHT  = 63676,
    SPELL_FOCUSED_EYEBEAM_VISUAL_LEFT   = 63702,
    //bras mort
    SPELL_ARM_DEAD_DAMAGE_KOLOGARN_N    = 63629,
	SPELL_ARM_DEAD_DAMAGE_KOLOGARN_H	= 63979
};

enum Actions
{
    ACTION_EYES,
	ACTION_RUBBLE,
	ACTION_LEFT_ARM,
    ACTION_RIGHT_ARM
};

const float LeftArmX = 1777.636841f;
const float LeftArmY = -47.970596f;
const float LeftArmZ = 448.805908f;
const float RightArmX = 1779.671753f;
const float RightArmY = 1.514701f;
const float RightArmZ = 448.810577f;

// Kologarn
class boss_kologarn : public CreatureScript
{
public:
    boss_kologarn() : CreatureScript("boss_kologarn") { }

    struct boss_kologarnAI : public BossAI
    {
        boss_kologarnAI(Creature* pCreature) : BossAI(pCreature, TYPE_KOLOGARN), vehicle(me->GetVehicleKit()), lSummons(me)
        {
            SetCombatMovement(false);
            me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_TURNING);
            emerged = false;
            ASSERT(vehicle);
        }

		SummonList lSummons;
        Vehicle *vehicle;
        uint32 m_uiSpellTimer;
        uint32 m_uiStoneShoutTimer;
        uint16 m_uiCheckTimer;
        uint32 m_uiArmsTimer;
        uint32 m_uiRespawnRight;
        uint32 m_uiRespawnLeft;
        uint32 m_uiFocusedEyebeamTimer;

        bool bRight;
        bool bLeft;
        bool bRollFace;
        bool bEyes;
        bool bArms;
        bool emerged;
        uint32 m_uiRubble;

        void Reset()
        {
            _Reset();
            lSummons.DespawnAll();
            m_uiFocusedEyebeamTimer = 10000;
            m_uiArmsTimer = 1000;
            m_uiSpellTimer = urand(10000,15000);
            m_uiCheckTimer = 15000;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            bRight = true;
            bLeft = true;
            bEyes = true;
            bArms = true;
            m_uiRubble = 0;
        }

        void MoveInLineOfSight(Unit *who)
        {
            // Birth animation
            if (!emerged && me->IsWithinDistInMap(who, 35.0f) && who->GetTypeId() == TYPEID_PLAYER)
            {
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                emerged = true;
            }
            ScriptedAI::MoveInLineOfSight(who);
        }

        void KilledUnit(Unit* victim)
        {
            switch(urand(0, 1))
            {
                case 0:
				    DoScriptText(SAY_SLAY_01, me);
					break;
                case 1:
				    DoScriptText(SAY_SLAY_02, me);
					break;
            }
        }

        void DoAction(const int32 action)
	    {
		    switch(action)
		    {
		    case ACTION_RUBBLE:
			    m_uiRubble++;
			    break;
		    case ACTION_EYES:
			    bEyes = false;
			    break;
		    case ACTION_RIGHT_ARM:
			    bArms = false;
                bRight = false;
                m_uiRespawnRight = 30000;
                me->DealDamage(me, RAID_MODE(543855,2300925));
			    break;
            case ACTION_LEFT_ARM:
			    bArms = false;
                bLeft = false;
                m_uiRespawnLeft = 30000;
                me->DealDamage(me, RAID_MODE(543855,2300925));
			    break;
		    }
	    }

        void JustDied(Unit* pKiller)
        {
		    _JustDied();
            lSummons.DespawnAll();
            if (pKiller->GetGUID() == me->GetGUID())
                return;
            if (instance)
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STONE_AURA);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STONE_AURA_2);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STONE_AURA_H);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STONE_AURA_H_2);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STONE_GRIP_VISUAL);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STONE_GRIP);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STONE_GRIP_H);
                
                if (bEyes)
                    instance->DoCompleteAchievement(RAID_MODE(2955,2956));
                if (m_uiRubble >= 25)
                    instance->DoCompleteAchievement(RAID_MODE(2959,2960));
                if (bArms)
                    instance->DoCompleteAchievement(RAID_MODE(2951,2952));
                if(0 < m_uiArmsTimer && m_uiArmsTimer < 12000)
                    instance->DoCompleteAchievement(RAID_MODE(2953,2954));

                DoScriptText(SAY_DIED, me);
                DoScriptText(EMOTE_DIED, me);
                me->GetMotionMaster()->MoveTargetedHome();
            }
        }

        void EnterCombat(Unit* who)
        {
            _EnterCombat();
            DoScriptText(SAY_AGGRO, me);
        }

        void JustSummoned(Creature *summon)
        {
            lSummons.Summon(summon);
        }

        void UpdateAI(const uint32 diff)
        {
		    //Return since we have no target
            if (!UpdateVictim())
                return;

            if (me->getVictim()->GetVehicle())
            {
                me->getThreatManager().modifyThreatPercent(me->getVictim(), -100);
                UpdateVictim();
            }

            if (!bLeft)
            {
                if (m_uiRespawnLeft < diff)
                {
                    if (Creature* leftArm = me->SummonCreature(32933, LeftArmX, LeftArmY, LeftArmZ, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                    {
                        leftArm->EnterVehicle(me, 0);
                        leftArm->SetInCombatWith(me->getVictim());
                    }
                    bLeft = true;
                    DoScriptText(EMOTE_LA, me);
                } else m_uiRespawnLeft -= diff;
            }

            if (!bRight)
            {
                if (m_uiRespawnRight < diff)
                {
                    if (Creature* rightArm = me->SummonCreature(32934, RightArmX, RightArmY, RightArmZ, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                    {
                        rightArm->EnterVehicle(me, 1);
                        rightArm->SetInCombatWith(me->getVictim());
                    }
                    bRight = true;
                    DoScriptText(EMOTE_RA, me);
                }else m_uiRespawnRight -= diff;
            }

            if (m_uiCheckTimer < diff)
            {
                if(!bRight && !bLeft)
                    m_uiArmsTimer += diff;
                else
                    m_uiArmsTimer = 0;

                if (!me->IsWithinMeleeRange(me->getVictim()) && !me->HasUnitState(UNIT_STAT_CASTING))
                {
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 5.0f))
                        me->TauntApply(pTarget);
                    else
                        DoCast(me->getVictim(), RAID_MODE(SPELL_PETRIFYING_BREATH, SPELL_PETRIFYING_BREATH_H));
                }
                m_uiCheckTimer = 500;
            }
            else 
                m_uiCheckTimer -= diff;

		     if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if (m_uiFocusedEyebeamTimer <= diff)
            {
                if (Unit* mTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 0, true))
                {
                    if (!mTarget->GetVehicle())
                    {
                        uint32 duration = urand(15000, 20000);
                        if (Creature* eye = me->SummonCreature(NPC_RIGHT_EYE, mTarget->GetPositionX()+3, mTarget->GetPositionY()+3, mTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, duration))
                        {
                            eye->AddThreat(mTarget, 999999.0f);
					        eye->GetMotionMaster()->MoveChase(mTarget, 0 ,0);
                        }
				        if (Creature* eye = me->SummonCreature(NPC_LEFT_EYE, mTarget->GetPositionX()+3, mTarget->GetPositionY()-3, mTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, duration))
                        {
                            eye->AddThreat(mTarget, 999999.0f);
					        eye->GetMotionMaster()->MoveChase(mTarget, 0, 0);
                        }
                        m_uiFocusedEyebeamTimer = urand(28000,35000);
                    }
                    else
                        m_uiFocusedEyebeamTimer = 2000;
                }
            } else m_uiFocusedEyebeamTimer -= diff; 

            if (m_uiSpellTimer < diff)
            {
                if (!bRight && !bLeft)
                    DoCast(me->getVictim(), RAID_MODE(SPELL_OVERHEAD_SMASH, SPELL_OVERHEAD_SMASH_H));
                else
                    if (bRight || bLeft)
                        DoCast(me->getVictim(), RAID_MODE(SPELL_ONE_ARMED_SMASH, SPELL_ONE_ARMED_SMASH_H));
                m_uiSpellTimer = urand(10000,15000);
            }else m_uiSpellTimer -= diff;   
            
            if (m_uiStoneShoutTimer < diff)
            {
                if (!bRight && !bLeft)
                    DoCast(me->getVictim(), RAID_MODE(SPELL_STONE_SHOUT, SPELL_STONE_SHOUT_H));
			    m_uiStoneShoutTimer = urand(400,600);
            }else m_uiStoneShoutTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_kologarnAI(pCreature);
    }
};

// Rubble
class mob_ulduar_rubble : public CreatureScript
{
public:
    mob_ulduar_rubble() : CreatureScript("mob_ulduar_rubble") { }

    struct mob_ulduar_rubbleAI : public ScriptedAI
    {
        mob_ulduar_rubbleAI(Creature* pCreature) : ScriptedAI(pCreature) 
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 m_uiStoneNovaTimer;
        uint32 m_uiDeathTimer;

        bool die;

        void Reset()
        {
            die = false;
            m_uiStoneNovaTimer = urand(8000, 12000);
            DoZoneInCombat();
        }

        void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
        {
            if (uiDamage > me->GetHealth())
                if (!die)
                {
                    uiDamage = 0;
                    DoCast(me, SPELL_RUMBLE);
                    if(Creature* pKologarn = Unit::GetCreature((*me), instance->GetData64(TYPE_KOLOGARN)))
                        pKologarn->AI()->DoAction(ACTION_RUBBLE);
                    m_uiDeathTimer = 500;
                    die = true;
                }
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_uiDeathTimer < diff && die)
            {
                me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            } else m_uiDeathTimer -= diff;

            if (m_uiStoneNovaTimer < diff && !die && getDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
            {
                DoCast(me, SPELL_STONE_NOVA);
                m_uiStoneNovaTimer = urand(7000, 9000);
            } else m_uiStoneNovaTimer -= diff;

            if (!die)
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_ulduar_rubbleAI(pCreature);
    }
};

// Left Arm
class boss_left_arm : public CreatureScript
{
public:
    boss_left_arm() : CreatureScript("boss_left_arm") { }

    struct boss_left_armAI : public ScriptedAI
    {
        boss_left_armAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            Reset();
            SetCombatMovement(false);
            instance = pCreature->GetInstanceScript();
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_TURNING);
        }

        InstanceScript* instance;

        uint32 m_uiShockwaveTimer;

        void Reset()
        {
            m_uiShockwaveTimer = urand(20000,30000);
            DoCast(me, SPELL_ARM_VISUAL);
        }

        void Aggro(Unit* pWho)
        {
            if (instance)
            {
                if (Creature* pTemp = ((Creature*)Unit::GetUnit((*me), instance->GetData64(DATA_RIGHT_ARM))))
                    if (pTemp->isAlive())
                        pTemp->SetInCombatWithZone();
                if (Creature* pTemp = ((Creature*)Unit::GetUnit((*me), instance->GetData64(DATA_KOLOGARN))))
                    if (pTemp->isAlive())
                        pTemp->SetInCombatWithZone();
            }
        }

        void JustDied(Unit* pKiller)
        {
            if (!instance)
                return;

            if (Creature* pTemp = me->FindNearestCreature(NPC_KOLOGARN, 30.0f))
                if (pTemp->isAlive()) 
                {
                    //pTemp->CastSpell(pTemp, RAID_MODE(SPELL_ARM_DEAD_DAMAGE_KOLOGARN_N, SPELL_ARM_DEAD_DAMAGE_KOLOGARN_H), true);
				    pTemp->AI()->DoAction(ACTION_LEFT_ARM);
				    DoScriptText(SAY_LEFT_DIED, pTemp);
                }
            for (uint8 i = 0 ; i < 5 ; i++)
                if (Creature* pTemp = me->SummonCreature(MOB_RUBBLE, LeftArmX, LeftArmY, LeftArmZ, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    {
                        pTemp->AddThreat(pTarget,0.0f);
                        pTemp->AI()->AttackStart(pTarget);
                    }
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
             if (!UpdateVictim())
                    return;

            if (m_uiShockwaveTimer < diff)
            {
                me->SetInCombatWithZone();
                if (Creature* pKologarn = me->FindNearestCreature(NPC_KOLOGARN, 30))
                    DoScriptText(SAY_SHOCKWAVE, pKologarn);
                DoCast(me, RAID_MODE(SPELL_SHOCKWAVE, SPELL_SHOCKWAVE_H));
                m_uiShockwaveTimer = urand(20000,30000);
            }else m_uiShockwaveTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_left_armAI(pCreature);
    }
};

// Right Arm
class boss_right_arm : public CreatureScript
{
public:
    boss_right_arm() : CreatureScript("boss_right_arm") { }

    struct boss_right_armAI : public ScriptedAI
    {
        boss_right_armAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            SetCombatMovement(false);
            instance = pCreature->GetInstanceScript();
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_TURNING);
        }

        InstanceScript* instance;

        uint32 m_uiStoneGripTimer;
        uint32 gripdmg;
        uint32 freedmg;
        uint64 uiGripTarget;

        bool grip;

        void Reset()
        {
            m_uiStoneGripTimer = 20000;
            uiGripTarget = 0;
            gripdmg = 0;
            freedmg = 0;
            grip = false;
            DoCast(me, SPELL_ARM_VISUAL);
        }

        void Aggro(Unit* pWho)
        {
            if (instance)
            {
                if (Creature* pTemp = ((Creature*)Unit::GetUnit((*me), instance->GetData64(DATA_LEFT_ARM))))
                    if (pTemp->isAlive())
                        pTemp->SetInCombatWithZone();
                if (Creature* pTemp = ((Creature*)Unit::GetUnit((*me), instance->GetData64(DATA_KOLOGARN))))
                    if (pTemp->isAlive())
                        pTemp->SetInCombatWithZone();
            }
        }

        void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
        {
            if (grip)
            {
                gripdmg += uiDamage;
                if (gripdmg > RAID_MODE(100000, 480000) || uiDamage > me->GetHealth())
                {
                    if (Unit* pGripTarget = Unit::GetUnit(*me, uiGripTarget))
                    {
                        if (pGripTarget->HasAura(RAID_MODE(SPELL_STONE_AURA,SPELL_STONE_AURA_H)))
                        {
                            pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_GRIP, SPELL_STONE_GRIP_H)); 
                            pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_AURA_2, SPELL_STONE_AURA_H_2));
                            pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_AURA, SPELL_STONE_AURA_H));
                            pGripTarget->ExitVehicle();
                            pGripTarget->GetMotionMaster()->MoveJump(1767.80f, -18.38f, 448.808f, 10.0f, 10.0f);
                        }
                    }
                    grip = false;
                    gripdmg = 0;
                }
            }
        }

        void KilledUnit(Unit* pVictim)
        {
            if (pVictim && pVictim->GetGUID() == uiGripTarget)
            {
                pVictim->ExitVehicle();
                pVictim->GetMotionMaster()->MoveJump(1767.80f, -18.38f, 448.808f, 10.0f, 10.0f);
                grip = false;
                gripdmg = 0;
            }
        }

        void JustDied(Unit* pKiller)
        {
            if (!instance)
                return;
            if (Creature* pTemp = me->FindNearestCreature(NPC_KOLOGARN, 30.0f))
                if (pTemp->isAlive())
                {
                    //pTemp->CastSpell(pTemp, RAID_MODE(SPELL_ARM_DEAD_DAMAGE_KOLOGARN_N, SPELL_ARM_DEAD_DAMAGE_KOLOGARN_H), true);
                    pTemp->AI()->DoAction(ACTION_RIGHT_ARM);
				    DoScriptText(SAY_RIGHT_DIED, pTemp);
                }
            for (uint8 i = 0 ; i < 5 ; i++)
                if (Creature* pTemp = me->SummonCreature(MOB_RUBBLE, RightArmX, RightArmY, RightArmZ, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    {
                        pTemp->AddThreat(pTarget,0.0f);
                        pTemp->AI()->AttackStart(pTarget);
                    }
                }
            if (Unit* pGripTarget = Unit::GetUnit(*me, uiGripTarget))
            {
                if (pGripTarget->HasAura(RAID_MODE(SPELL_STONE_AURA,SPELL_STONE_AURA_H)))
                {
                    pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_GRIP, SPELL_STONE_GRIP_H)); 
                    pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_AURA_2, SPELL_STONE_AURA_H_2));
                    pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_AURA, SPELL_STONE_AURA_H));
                    pGripTarget->ExitVehicle();
                    pGripTarget->GetMotionMaster()->MoveJump(1767.80f, -18.38f, 448.808f, 10.0f, 10.0f);
                }
            }
            me->GetMotionMaster()->MoveTargetedHome();
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                    return;

            if (m_uiStoneGripTimer < diff)
            {
                if (Unit* pGripTarget = Unit::GetUnit(*me, uiGripTarget))
                {
                    pGripTarget->ExitVehicle();
                    pGripTarget->GetMotionMaster()->MoveJump(1767.80, -18.38, 448.808, 10, 10);

                    if (pGripTarget->HasAura(RAID_MODE(SPELL_STONE_AURA,SPELL_STONE_AURA_H)))
                    {
                        pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_GRIP, SPELL_STONE_GRIP_H)); 
                        pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_AURA_2, SPELL_STONE_AURA_H_2));
                        pGripTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_STONE_AURA, SPELL_STONE_AURA_H));
                        pGripTarget->ExitVehicle();
                        pGripTarget->GetMotionMaster()->MoveJump(1767.80f, -18.38f, 448.808f, 10.0f, 10.0f);
                    }
                }
                grip = false;
                uiGripTarget = 0;
                gripdmg = 0;
                me->SetInCombatWithZone();
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0, true))
                {
                    if (Creature* pKologarn = me->FindNearestCreature(NPC_KOLOGARN, 30.0f))
                    {
                        DoScriptText(SAY_GRABB, pKologarn);
                        DoScriptText(EMOTE_STONE, pKologarn);
                    }
                    //DoCast(target, SPELL_STONE_GRIP);
                    me->AddAura(SPELL_STONE_GRIP_VISUAL, target);
                    me->AddAura(RAID_MODE(SPELL_STONE_AURA, SPELL_STONE_AURA_H), target);
                    me->AddAura(RAID_MODE(SPELL_STONE_AURA_2, SPELL_STONE_AURA_H_2), target);
                    me->AddAura(64708, target);
                    target->EnterVehicle(me, 0);
                    uiGripTarget = target->GetGUID();
                    grip = true;
                    gripdmg = 0;        
                }
                m_uiStoneGripTimer = 30000;
            }else m_uiStoneGripTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_right_armAI(pCreature);
    }
};

class npc_focused_eye_left : public CreatureScript
{
public:
    npc_focused_eye_left() : CreatureScript("npc_focused_eye_left") { }

    struct npc_focused_eye_leftAI : public ScriptedAI
    {
        npc_focused_eye_leftAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;

        void Reset()
        {
            me->SetDisplayId(11686);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoCast(me, RAID_MODE(SPELL_FOCUSED_EYEBEAM_PERIODIC_N, SPELL_FOCUSED_EYEBEAM_PERIODIC_H));
            DoCast(me, RAID_MODE(SPELL_FOCUSED_EYEBEAM_EFFECT_N, SPELL_FOCUSED_EYEBEAM_EFFECT_H));
            DoCast(me, SPELL_FOCUSED_EYEBEAM_VISUAL_LEFT);
            me->ClearUnitState(UNIT_STAT_CASTING) ;
        }

        void SpellHitTarget(Unit *pTarget, const SpellEntry *spell)
        {
            if (instance)
                if (Creature* pKologarn = Unit::GetCreature((*me), instance->GetData64(TYPE_KOLOGARN)))
                    pKologarn->AI()->DoAction(ACTION_EYES);
        }

        void UpdateAI(const uint32 diff)
        {
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_focused_eye_leftAI(pCreature);
    }
};

class npc_focused_eye_right : public CreatureScript
{
public:
    npc_focused_eye_right() : CreatureScript("npc_focused_eye_right") { }

    struct npc_focused_eye_rightAI : public ScriptedAI
    {
        npc_focused_eye_rightAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;

        void Reset()
        {
            me->SetDisplayId(11686);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoCast(me, RAID_MODE(SPELL_FOCUSED_EYEBEAM_PERIODIC_N, SPELL_FOCUSED_EYEBEAM_PERIODIC_H));
            DoCast(me, RAID_MODE(SPELL_FOCUSED_EYEBEAM_EFFECT_N, SPELL_FOCUSED_EYEBEAM_EFFECT_H));
            DoCast(me, SPELL_FOCUSED_EYEBEAM_VISUAL_RIGHT);
            me->ClearUnitState(UNIT_STAT_CASTING) ;
        }

        void SpellHitTarget(Unit *pTarget, const SpellEntry *spell)
        {
            if (instance)
                if (Creature* pKologarn = Unit::GetCreature((*me), instance->GetData64(TYPE_KOLOGARN)))
                    pKologarn->AI()->DoAction(ACTION_EYES);
        }

        void UpdateAI(const uint32 diff)
        {
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_focused_eye_rightAI(pCreature);
    }
};

void AddSC_boss_kologarn()
{
    new boss_kologarn;
    new boss_left_arm;
    new boss_right_arm;
    new mob_ulduar_rubble;
    new npc_focused_eye_right;
    new npc_focused_eye_left;
}
