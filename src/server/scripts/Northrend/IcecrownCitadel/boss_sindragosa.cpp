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

enum Yells
{
    SAY_AGGRO            = -1666071,
    SAY_UNCHAIND_MAGIC   = -1666072,
    SAY_BLISTERING_COLD  = -1666073,
    SAY_BREATH           = -1666074,
    SAY_AIR_PHASE        = -1666075,
    SAY_PHASE_3          = -1666076,
    SAY_KILL_1           = -1666077,
    SAY_KILL_2           = -1666078,
    SAY_BERSERK          = -1666079,
    SAY_DEATH            = -1666080
};

enum Spells
{
    SPELL_FROST_AURA          = 70084,
    SPELL_CLEAVE              = 19983,
    SPELL_TAIL_SMASH          = 71077,
    SPELL_FROST_BREATH        = 73061,
    SPELL_PERMEATING_CHILL    = 70109,
    SPELL_UNCHAINED_MAGIC     = 69762, // нужно узнать механику на офе
    SPELL_ICY_TRIP_PULL       = 70117,
    SPELL_BOMB_VISUAL_1       = 64624,
    SPELL_BOMB_VISUAL_2       = 69016,
    SPELL_BLISTERING_COLD     = 71047,
    SPELL_FROST_BOMB_TRIGGER  = 69846,
    SPELL_FROST_BEACON        = 70126,
    SPELL_ICE_TOMB            = 70157,
    SPELL_FROST_BOMB          = 69845,
    SPELL_MYSTIC_BUFFED       = 70128,
    SPELL_ASPHYXATION         = 71665,
    SPELL_FROST_AURA_ADD      = 71387,
    SPELL_FROST_BREATH_ADD    = 71386,
    SPELL_ICE_BLAST           = 71376,
    SPELL_BELLOWING_ROAR      = 36922,
    SPELL_CLEAVE_ADD          = 40505,
    SPELL_TAIL_SWEEP          = 71369
    //70521
};

enum ePoints
{
    POINT_PHASE_FLY                = 1,
    POINT_PHASE_NORMAL             = 2
};

//need add correct coords
const Position SpawnLoc[]=
{
    {4523.889f, 2486.907f, 280.249f, 3.155f}, //fly pos
    {4407.439f, 2484.905f, 203.374f, 3.166f}, //center
    {4407.439f, 2484.905f, 230.374f, 3.166f}, //center Z + 30
    {4671.521f, 2481.815f, 343.365f, 3.166f} //spawn pos
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
                uiPhase = 1;

                uiBreathTimer = 15000;
                uiTailSmashTimer = 15000;
                uiBlisteringColdTimer = 30000;
                uiMarkTimer = 25000;
                uiBerserkTimer = 600000;
                uiChangePhaseTimer = 110000;
                uiUnchainedMagicTimer = 12000;
                uiBombTimer = 7000;
                uiCleaveTimer = 5000;
                uiBombCount = 0;

                me->SetFlying(true);
                SetCombatMovement(true);
                me->SetSpeed(MOVE_FLIGHT, 2.5f, true);

                bMystic = false;
                bCanSwitch = false;

                if(instance)
                    instance->SetData(DATA_SINDRAGOSA_EVENT, NOT_STARTED);

                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    me->GetMotionMaster()->MovementExpired();
            }

            void EnterCombat(Unit* /*who*/)
            {
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
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                {
                    switch(rand()%1)
                    {
                        case 0: DoScriptText(SAY_KILL_1, me); break;
                        case 1: DoScriptText(SAY_KILL_2, me); break;
                    }
                }
            }

            void JustReachedHome()
            {
                if (!instance)
                    return;

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_UNCHAINED_MAGIC);
                instance->SetData(DATA_SINDRAGOSA_EVENT, FAIL);
            }

            void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
            {
                if(uiPhase == 1)
                {
                    if(!HealthAbovePct(85) && !bCanSwitch)
                    {
                        DoScriptText(SAY_AIR_PHASE, me);
                        uiPhase = 2;
                        bCanSwitch = true;
                        SetCombatMovement(false);
                        me->GetMotionMaster()->MovePoint(POINT_PHASE_FLY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 30);
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        me->SetInCombatWithZone();
                    }
                    if(HealthAbovePct(36))
                        return;

                    DoScriptText(SAY_PHASE_3, me);
                    uiPhase = 3;
                }
            }


            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (uiBerserkTimer <= uiDiff)
                {
                    DoScriptText(SAY_BERSERK, me);
                    DoCast(me, SPELL_BERSERK);
                    uiBerserkTimer = 600000;
                } else uiBerserkTimer -= uiDiff;

                if(uiPhase == 1)
                {
                    if(uiUnchainedMagicTimer <= uiDiff)
                    {
                        for (uint8 i = 1; i <= urand(2, 4); ++i)
                        {
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, -10.0f, true, -SPELL_UNCHAINED_MAGIC))
                                DoCast(target, SPELL_UNCHAINED_MAGIC);
                        }
                        DoScriptText(SAY_UNCHAIND_MAGIC, me);
                        uiUnchainedMagicTimer = 12000;
                    } else uiUnchainedMagicTimer -= uiDiff;

                    if (uiBreathTimer <= uiDiff)
                    {
                        DoScriptText(SAY_BREATH, me);
                        DoCastVictim(SPELL_FROST_BREATH);
                        uiBreathTimer = 15000;
                    } else uiBreathTimer -= uiDiff;

                    if (uiCleaveTimer <= uiDiff)
                    {
                        DoCastVictim(SPELL_CLEAVE);
                        uiCleaveTimer = 6000;
                    } else uiCleaveTimer -= uiDiff;

                    if (uiTailSmashTimer <= uiDiff)
                    {
                        DoCast(SPELL_TAIL_SMASH);
                        uiTailSmashTimer = 15000;
                    } else uiTailSmashTimer -= uiDiff;

                    if (uiBlisteringColdTimer <= uiDiff)
                    {
                        DoCast(SPELL_ICY_TRIP_PULL);
                        DoCast(SPELL_BLISTERING_COLD);
                        DoScriptText(SAY_BLISTERING_COLD, me);
                        uiBlisteringColdTimer = 30000;
                    } else uiBlisteringColdTimer -= uiDiff;
                }

                if(uiPhase == 2)
                {
                    if (uiMarkTimer < uiDiff)
                    {
                        for (uint8 i = 1; i <= RAID_MODE(2,5,2,5); ++i)
                        {
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_FROST_BEACON))
                                DoCast(target, SPELL_FROST_BEACON);
                        }
                        uiMarkTimer = 25000;
                    } else uiMarkTimer -= uiDiff;

                    if (uiBombTimer <= uiDiff)
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 150.0f, true))
                            DoCast(target, SPELL_FROST_BOMB);
                        ++uiBombCount;
                        uiBombTimer = 7000;
                    } else uiBombTimer -= uiDiff;

                    if(uiBombCount >= RAID_MODE(4,8,4,8))
                    {
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                        SetCombatMovement(true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->AttackStop();
                        me->SetInCombatWithZone();
                        me->GetMotionMaster()->MovePoint(POINT_PHASE_NORMAL, SpawnLoc[1]);
                        uiPhase = 1;
                        uiBombCount = 0;
                    }
                }

                if(uiPhase == 3)
                {
                    if (uiMarkTimer < uiDiff)
                    {
                        for (uint8 i = 1; i <= RAID_MODE(2,5,2,5); ++i)
                        {
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_FROST_BEACON))
                                DoCast(target, SPELL_FROST_BEACON);
                        }
                        uiMarkTimer = 25000;
                    } else uiMarkTimer -= uiDiff;

                    if (uiBreathTimer <= uiDiff)
                    {
                        DoScriptText(SAY_BREATH, me);
                        DoCast(SPELL_FROST_BREATH);
                        uiBreathTimer = 15000;
                    } else uiBreathTimer -= uiDiff;

                    if(!bMystic)
                    {
                       DoCast(me, SPELL_MYSTIC_BUFFED);
                       bMystic = true;
                    }
                }

                if(!bCanSwitch || uiPhase != 1)
                    return;

                if (uiChangePhaseTimer < uiDiff)
                {
                    DoScriptText(SAY_AIR_PHASE, me);
                    uiPhase = 2;
                    SetCombatMovement(false);
                    me->GetMotionMaster()->MovePoint(POINT_PHASE_FLY, SpawnLoc[2]);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->SetInCombatWithZone();
                    uiChangePhaseTimer = 110000;
                } else uiChangePhaseTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* instance;

            uint8 uiPhase;
            uint8 uiBombCount;
            uint32 uiBreathTimer;
            uint32 uiCleaveTimer;
            uint32 uiTailSmashTimer;
            uint32 uiBlisteringColdTimer;
            uint32 uiBerserkTimer;
            uint32 uiMarkTimer;
            uint32 uiChangePhaseTimer;
            uint32 uiUnchainedMagicTimer;
            uint32 uiBombTimer;

            bool bMystic;
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

            void SetGUID(const uint64& guid)
            {
                uiTargetGUID = guid;
            }

            void Reset()
            {
                uiTargetGUID = 0;
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

            void UpdateAI(const uint32 /*uiDiff*/)
            {
                if(!uiTargetGUID)
                    return;

                if (Player* IceTomb = ObjectAccessor::GetPlayer(*me, uiTargetGUID))
                    if (!IceTomb->HasAura(SPELL_ICE_TOMB))
                        me->Kill(me);
            }

        private:
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
            }

            void Reset()
            {
                uiFrostBreathTimer = 5000;
                uiIceBlastTimer = 7000;
                me->SetFlying(true);
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_FROST_AURA_ADD);
            }

            void JustDied(Unit* /*killer*/)
            {
                if(instance)
                    if(instance->GetData(DATA_SINDRAGOSA_EVENT) != DONE)
                        instance->SetData(DATA_SPAWN, +1);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (uiFrostBreathTimer <= uiDiff)
                {
                    DoCastVictim( SPELL_FROST_BREATH_ADD);
                    uiFrostBreathTimer = 6000;
                } else uiFrostBreathTimer -= uiDiff;

                if (uiIceBlastTimer <= uiDiff)
                {
                    DoCast(me, SPELL_ICE_BLAST);
                    uiIceBlastTimer = 8000;
                } else uiIceBlastTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* instance;

            uint32 uiFrostBreathTimer;
            uint32 uiIceBlastTimer;
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
            }

            void Reset()
            {
                uiRoarTimer = 14000;
                uiCleaveTimer = 6000;
                uiSweepTimer = 7000;
                me->SetFlying(true);
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_FROST_AURA_ADD);
            }

            void JustDied(Unit* /*killer*/)
            {
                if(instance)
                    if(instance->GetData(DATA_SINDRAGOSA_EVENT) != DONE)
                        instance->SetData(DATA_SPAWN, +1);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (uiRoarTimer <= uiDiff)
                {
                    DoCastAOE(SPELL_BELLOWING_ROAR);
                    uiRoarTimer = 15000;
                } else uiRoarTimer -= uiDiff;

                if (uiCleaveTimer <= uiDiff)
                {
                    DoCastVictim( SPELL_CLEAVE_ADD);
                    uiCleaveTimer = 5000;
                } else uiCleaveTimer -= uiDiff;

                if (uiSweepTimer <= uiDiff)
                {
                    DoCast(SPELL_TAIL_SWEEP);
                    uiSweepTimer = 7000;
                } else uiSweepTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* instance;

            uint32 uiRoarTimer;
            uint32 uiCleaveTimer;
            uint32 uiSweepTimer;
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

            void OnRemove(AuraEffect const* aurEff, AuraApplication const* aurApp, AuraEffectHandleModes /*mode*/)
            {
                if(Unit* beacon = aurApp->GetBase()->GetCaster())
                {
                    if (InstanceScript* instance = aurApp->GetBase()->GetOwner()->GetInstanceScript())
                        if (Creature* sindragosa = Unit::GetCreature(*aurApp->GetBase()->GetOwner(), instance->GetData64(DATA_SINDRAGOSA)))
                            if(sindragosa->isAlive())
                                sindragosa->CastSpell(beacon, SPELL_ICE_TOMB, true);

                    beacon->CastSpell(beacon, SPELL_ASPHYXATION, true);
                    Creature* tomb = beacon->SummonCreature(CREATURE_ICE_TOMB, beacon->GetPositionX(), beacon->GetPositionY(), beacon->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN);
                    if(tomb)
                        tomb->AI()->SetGUID(beacon->GetGUID());
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_sindragosa_ice_tomb_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
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

            void OnApply(AuraEffect const* aurEff, AuraApplication const* aurApp, AuraEffectHandleModes /*mode*/)
            {
                if (InstanceScript* instance = aurApp->GetBase()->GetOwner()->GetInstanceScript())
                    if(aurApp->GetBase()->GetStackAmount() >= 5)
                        instance->SetData(DATA_ALL_YOU_CAN_EAT, FAIL);
                    else
                        instance->SetData(DATA_ALL_YOU_CAN_EAT, DONE);
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
        spell_sindragosa_unchained_magic() : SpellScriptLoader("spell_sindragosa_unchained_magic") { } //69766


        class spell_sindragosa_unchained_magic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sindragosa_unchained_magic_AuraScript)

            void OnRemove(AuraEffect const* aurEff, AuraApplication const* aurApp, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                int32 damage = (aurApp->GetBase()->GetStackAmount() * 2000);
                aurApp->GetTarget()->CastCustomSpell(71044, SPELLVALUE_BASE_POINT0, damage, aurApp->GetTarget(), true, NULL, aurEff, GetCasterGUID());
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

void AddSC_boss_sindragosa()
{
    new boss_sindragosa();
    new npc_spinestalker();
    new npc_rimefang();
    new npc_ice_tomb();
    new spell_sindragosa_ice_tomb();
    new spell_sindragosa_mystic_buffet(); //for achievement
    new spell_sindragosa_unchained_magic();
}