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
#include "icecrown_citadel.h"
#include "SpellAuraEffects.h"

enum Yells
{
    SAY_INTRO     = -1631000,
    SAY_AGGRO     = -1631001,
    SAY_STORM     = -1631002,
    SAY_SPIKE_1   = -1631003,
    SAY_SPIKE_2   = -1631004,
    SAY_SPIKE_3   = -1631005,
    SAY_KILL_1    = -1631006,
    SAY_KILL_2    = -1631007,
    SAY_DEATH     = -1631008,
    SAY_BERSERK   = -1631009,
    STORM_EMOTE   = -1631010
};

enum Spells
{
    SPELL_SABER_SLASH        = 69055,
    SPELL_COLD_FLAME         = 69146,
    SPELL_BONE_SPIKE         = 73142,
    SPELL_SPIKE_IMPALING     = 69065,
    SPELL_BONE_STORM         = 69076,
    SPELL_COLD_FLAME_SPAWN   = 69138,
    SPELL_COLD_FLAME_SPAWN_B = 72701
};

class npc_bone_spike : public CreatureScript
{
    public:
        npc_bone_spike() : CreatureScript("npc_bone_spike") { }

        struct npc_bone_spikeAI : public Scripted_NoMovementAI
        {
            npc_bone_spikeAI(Creature *pCreature) : Scripted_NoMovementAI(pCreature), vehicle(pCreature->GetVehicleKit())
            {
                ASSERT(vehicle);
                BoneSpikeGUID = 0;
                pInstance = pCreature->GetInstanceScript();
            }

            void SetPrisoner(const uint64& guid)
            {
                BoneSpikeGUID = guid;
                if (Player* boned = ObjectAccessor::GetPlayer(*me, BoneSpikeGUID))
                    boned->EnterVehicle(vehicle, 0);
            }

            void Reset()
            {
                BoneSpikeGUID = 0;
            }

            void JustDied(Unit* /*pKiller*/)
            {
                if (Player* boned = ObjectAccessor::GetPlayer(*me, BoneSpikeGUID))
                    boned->RemoveAurasDueToSpell(SPELL_SPIKE_IMPALING);
            }

            void KilledUnit(Unit* /*pVictim*/)
            {
                me->Kill(me);
            }

            void PassengerBoarded(Unit * who, int8 /*seatId*/, bool apply)
            {
                if (!apply)
                    return;

                who->AddAura(SPELL_SPIKE_IMPALING, who);
                m_uiAchievBonedTimer = 8000;
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!BoneSpikeGUID || !pInstance)
                    return;

                if (Player* boned = ObjectAccessor::GetPlayer(*me, BoneSpikeGUID))
                    if (!boned->HasAura(SPELL_SPIKE_IMPALING))
                        me->Kill(me);

                if (m_uiAchievBonedTimer <= uiDiff)
                {
                    pInstance->SetData(DATA_BONED, FAIL);
                    m_uiAchievBonedTimer = 8000;
                } else m_uiAchievBonedTimer -= uiDiff;
            }

        private:
            InstanceScript* pInstance;

            uint64 BoneSpikeGUID;
            uint32 m_uiAchievBonedTimer;

            Vehicle* vehicle;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_bone_spikeAI(pCreature);
        }
};

class npc_cold_flame : public CreatureScript
{
    public:
        npc_cold_flame() : CreatureScript("npc_cold_flame") { }

        struct npc_cold_flameAI : public ScriptedAI
        {
            npc_cold_flameAI(Creature *pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            void Reset()
            {
                m_uiColdFlameTimer = 900;

                SpellEntry* spell = GET_SPELL(SPELL_COLD_FLAME);
                if (spell)
                    spell->EffectRadiusIndex[0] = 16; //prevent stack damage
                DoCast(me, SPELL_COLD_FLAME);

                me->SetVisible(false);
                DoStartNoMovement(me->getVictim());

                m_uiStage = 1;
                m_uiRadius = 2;
                m_uiOwnerEntry = 0;
            }

            void IsSummonedBy(Unit* owner)
            {
                if(owner)
                {
                    if(owner->HasAura(SPELL_BONE_STORM))
                        bCrossfire = true;
                    else
                        bCrossfire = false;

                    m_uiOwnerEntry = owner->GetEntry();
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if(m_uiColdFlameTimer <= uiDiff)
                {
                    if(m_uiOwnerEntry == CREATURE_MARROWGAR)
                    {
                        if(bCrossfire)
                        {
                            float x, y;
                            me->GetNearPoint2D(x, y, m_uiRadius*m_uiStage, 0);
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                            me->GetNearPoint2D(x, y, m_uiRadius*m_uiStage, M_PI/2);
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                            me->GetNearPoint2D(x, y, m_uiRadius*m_uiStage, M_PI*(M_PI/2));
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                            me->GetNearPoint2D(x, y, m_uiRadius*m_uiStage, M_PI);
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                        }
                        else if (!bCrossfire)
                        {
                            float x, y;
                            float angle = pInstance->GetData(DATA_ANGLE / 1000);
                            me->GetNearPoint2D(x, y, m_uiRadius*m_uiStage, angle);
                            me->SummonCreature(CREATURE_COLD_FLAME, x, y, me->GetPositionZ(), angle, TEMPSUMMON_TIMED_DESPAWN, 4000);
                        }
                        ++m_uiStage;
                        m_uiColdFlameTimer = 1000;
                    }
                } else m_uiColdFlameTimer -= uiDiff;
            }

        private:
            InstanceScript* pInstance;

            uint32 m_uiColdFlameTimer;
            uint8 m_uiRadius;
            uint8 m_uiStage;
            uint32 m_uiOwnerEntry;
            bool bCrossfire;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_cold_flameAI(pCreature);
        }
};

class boss_lord_marrowgar : public CreatureScript
{
    public:
        boss_lord_marrowgar() : CreatureScript("boss_lord_marrowgar") { }

        struct boss_lord_marrowgarAI : public BossAI
        {
            boss_lord_marrowgarAI(Creature* pCreature) : BossAI(pCreature, DATA_MARROWGAR), summons(me)
            {
                pInstance = pCreature->GetInstanceScript();
                m_uiBoneCount = RAID_MODE(1,3,1,3);
                fBaseSpeed = me->GetSpeedRate(MOVE_RUN);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                bIntro = false;
            }

            void Reset()
            {
                m_uiSaberSlashTimer = 7000;
                m_uiBoneSpikeGraveyardTimer = 15000;
                m_uiColdFlameTimer = 10000;
                m_uiBoneStormTimer = 45000;
                m_uiBoneStormRemoveTimer = 20000;
                m_uiBerserkTimer = 600000;
                m_uiMoveTimer = 5000;

                me->SetSpeed(MOVE_RUN, fBaseSpeed, true);

                if (pInstance)
                {
                    pInstance->SetData(DATA_BONED, DONE);
                    pInstance->SetData(DATA_MARROWGAR_EVENT, NOT_STARTED);
                }
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoScriptText(SAY_AGGRO, me);

                if (pInstance)
                    pInstance->SetData(DATA_MARROWGAR_EVENT, IN_PROGRESS);

                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                if (!pInstance)
                    return;

                DoScriptText(SAY_DEATH, me);

                pInstance->SetData(DATA_MARROWGAR_EVENT, DONE);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPIKE_IMPALING);

                if(pInstance->GetData(DATA_BONED) == DONE)
                    pInstance->DoCompleteAchievement(RAID_MODE(ACHIEV_BONED_10, ACHIEV_BONED_25));

                summons.DespawnAll();
            }

            void JustReachedHome()
            {
                if (!pInstance)
                    return;

                pInstance->SetData(DATA_MARROWGAR_EVENT, FAIL);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPIKE_IMPALING);

                summons.DespawnAll();
            }

            void KilledUnit(Unit* pVictim)
            {
                if (pVictim->GetTypeId() == TYPEID_PLAYER)
                {
                    switch(rand()%1)
                    {
                        case 0: DoScriptText(SAY_KILL_1, me); break;
                        case 1: DoScriptText(SAY_KILL_2, me); break;
                    }
                }
            }

            void MoveInLineOfSight(Unit* pWho)
            {
                if (!bIntro && me->IsWithinDistInMap(pWho, 90.0f, true))
                {
                    DoScriptText(SAY_INTRO, me);
                    bIntro = true;
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (m_uiBerserkTimer <= uiDiff)
                {
                    DoScriptText(SAY_BERSERK, me);
                    DoCast(SPELL_BERSERK);
                    m_uiBerserkTimer = 600000;
                } else m_uiBerserkTimer -= uiDiff;

                if (IsHeroic() || !me->HasAura(SPELL_BONE_STORM))
                {
                    if (m_uiBoneSpikeGraveyardTimer < uiDiff)
                    {
                        for (uint8 i = 1; i <= m_uiBoneCount; ++i)
                        {
                            if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100, true, -SPELL_SPIKE_IMPALING))
                            {
                                Creature* pBone = me->SummonCreature(CREATURE_BONE_SPIKE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 999999);
                                CAST_AI(npc_bone_spike::npc_bone_spikeAI, pBone->AI())->SetPrisoner(pTarget->GetGUID());
                                pTarget->AddAura(SPELL_SPIKE_IMPALING, pTarget);
                            }
                        }
                        DoScriptText(RAND(SAY_SPIKE_1,SAY_SPIKE_2,SAY_SPIKE_3), me);
                        m_uiBoneSpikeGraveyardTimer = 15000;
                    } else m_uiBoneSpikeGraveyardTimer -= uiDiff;
                }

                if (!me->HasAura(SPELL_BONE_STORM))
                {
                    if (m_uiBoneStormTimer <= uiDiff)
                    {
                        DoCast(SPELL_BONE_STORM);
                        DoScriptText(SAY_STORM, me);
                        DoScriptText(STORM_EMOTE, me);
                        DoStartNoMovement(me->getVictim());
                        me->SetSpeed(MOVE_RUN, fBaseSpeed*3.0f, true);
                        m_uiBoneStormTimer = 45000; //bone storm 30 second + other spell casting time
                    } else m_uiBoneStormTimer -= uiDiff;

                    if (m_uiColdFlameTimer <= uiDiff)
                    {
                        if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, -8.0f, true))
                        {
                            pInstance->SetData(DATA_ANGLE, (me->GetAngle(pTarget)*1000));
                            DoCast(pTarget, SPELL_COLD_FLAME_SPAWN);
                        }
                        else
                        {
                            pInstance->SetData(DATA_ANGLE, (me->GetAngle(me->getVictim())*1000));
                            DoCast(me->getVictim(), SPELL_COLD_FLAME_SPAWN);
                        }
                        m_uiColdFlameTimer = 10000;
                    } else m_uiColdFlameTimer -= uiDiff;

                    if (m_uiSaberSlashTimer <= uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_SABER_SLASH);
                        m_uiSaberSlashTimer = 7000;
                    } else m_uiSaberSlashTimer -= uiDiff;
                }
                else
                {
                    if (m_uiBoneStormRemoveTimer <= uiDiff)
                    {
                        me->RemoveAurasDueToSpell(SPELL_BONE_STORM);
                        DoStartMovement(me->getVictim());
                        me->SetSpeed(MOVE_RUN, fBaseSpeed, true);
                        m_uiBoneStormRemoveTimer = RAID_MODE(20000,30000,20000,30000);
                    } else m_uiBoneStormRemoveTimer -= uiDiff;

                    if(m_uiMoveTimer <= uiDiff)
                    {
                        float x, y, z;
                        if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        {
                            pTarget->GetPosition(x, y, z);
                            me->GetMotionMaster()->MovePoint(0, x, y, z);
                        }
                        DoCast(SPELL_COLD_FLAME_SPAWN_B);
                        m_uiMoveTimer = 5000;
                    } else m_uiMoveTimer -= uiDiff;
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* pInstance;

            uint32 m_uiSaberSlashTimer;
            uint32 m_uiBoneSpikeGraveyardTimer;
            uint32 m_uiBoneStormTimer;
            uint32 m_uiBoneStormRemoveTimer;
            uint32 m_uiColdFlameTimer;
            uint32 m_uiBerserkTimer;
            uint32 m_uiMoveTimer;
            uint8 m_uiBoneCount;
            float fBaseSpeed;
            bool bIntro;

            SummonList summons;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_lord_marrowgarAI(pCreature);
        }
};

class spell_lord_marrowgar_bone_storm : public SpellScriptLoader
{
    public:
        spell_lord_marrowgar_bone_storm() : SpellScriptLoader("spell_lord_marrowgar_bone_storm") { }


        class spell_lord_marrowgar_bone_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lord_marrowgar_bone_storm_SpellScript)

            void onEffect(SpellEffIndex /*effIndex*/)
            {
                int32 dmg = GetHitDamage();
                float distance = GetHitUnit()->GetExactDist2d(GetCaster());
                float distVariant = distance >= 20.0f ? 4 : (10.0f/3.0f);
                if (distance < 5)
                    distance = 5; //prevent exploit

                SetHitDamage(int32(dmg * distVariant / distance));
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_lord_marrowgar_bone_storm_SpellScript::onEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }

            bool Load()
            {
                if (GetCaster()->GetEntry() != CREATURE_MARROWGAR)
                    return false;
                return true;
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lord_marrowgar_bone_storm_SpellScript();
        }
};

void AddSC_boss_lord_marrowgar()
{
    new npc_bone_spike();
    new npc_cold_flame();
    new boss_lord_marrowgar();
    new spell_lord_marrowgar_bone_storm();

    if (VehicleSeatEntry* vehSeat = const_cast<VehicleSeatEntry*>(sVehicleSeatStore.LookupEntry(6206)))
        vehSeat->m_flags |= 0x400;
}
