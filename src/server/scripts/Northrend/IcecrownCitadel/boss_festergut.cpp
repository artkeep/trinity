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

#include "ScriptPCH.h"
#include "icecrown_citadel.h"

enum Yells
{
    SAY_STINKY                  = -1631078,
    SAY_AGGRO                   = -1631079,
    EMOTE_GAS_SPORE             = -1631081,
    EMOTE_WARN_GAS_SPORE        = -1631082,
    SAY_PUNGENT_BLIGHT          = -1631083,
    EMOTE_WARN_PUNGENT_BLIGHT   = -1631084,
    EMOTE_PUNGENT_BLIGHT        = -1631085,
    SAY_KILL_1                  = -1631086,
    SAY_KILL_2                  = -1631087,
    SAY_BERSERK                 = -1631088,
    SAY_DEATH                   = -1631089
};

enum Spells
{
    SPELL_PUNGENT_BLIGHT     = 69195,
    SPELL_INHALE_BLIGHT      = 69165,
    SPELL_VILE_GAS           = 69240,
    SPELL_GASTRIC_BLOAT      = 72219,
    SPELL_GAS_SPORES         = 69279,
    SPELL_UNOCULATED         = 69291,
    SPELL_GAS_FLOOD          = 71379,
    SPELL_MORTAL_WOUND       = 71127,
    SPELL_DECIMATE           = 71123,
    SPELL_PLAGUE_STENCH      = 71805,
    SPELL_GASTRIC_EXPLOSION  = 72227,
    SPELL_INOCULATED         = 69291
};

static const uint32 gaseousBlight[3]        = {69157, 69162, 69164};
static const uint32 gaseousBlightVisual[3]  = {69126, 69152, 69154};

class boss_festergut : public CreatureScript
{
    public:
        boss_festergut() : CreatureScript("boss_festergut") { }

        struct boss_festergutAI : public BossAI
        {
            boss_festergutAI(Creature* creature) : BossAI(creature, DATA_FESTERGURT)
            {
                instance = me->GetInstanceScript();
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
            }

            void Reset()
            {
                uiInhaleBlightTimer = urand(23000,29000);
                uiVileGasTimer = 20000;
                uiGasSporesTimer = 20000;
                uiGastricBloatTimer = 12000;
                uiBerserkTimer = 300000;
                uiGaseousTimer = 1000;

                uiInhaleBlightCount = 0;

                if (Creature* gasDummy = GetClosestCreatureWithEntry(me, CREATURE_ORANGE_GAS_STALKER, 100.0f, true))
                {
                    uiGasDummyGUID = gasDummy->GetGUID();
                    for (uint8 i = 0; i < 3; ++i)
                    {
                        gasDummy->RemoveAurasDueToSpell(gaseousBlight[i]);
                        gasDummy->RemoveAurasDueToSpell(gaseousBlightVisual[i]);
                    }
                }

                if (instance)
                    instance->SetData(DATA_FESTERGURT_EVENT, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);

                if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
                {
                    professor->AI()->DoAction(ACTION_FESTERGUT_COMBAT);
                    professor->AI()->DoAction(ACTION_FESTERGUT_GAS);
                }

                if (Creature* gasDummy = GetClosestCreatureWithEntry(me, CREATURE_ORANGE_GAS_STALKER, 100.0f, true))
                {
                    uiGasDummyGUID = gasDummy->GetGUID();
                    for (uint8 i = 0; i < 3; ++i)
                        gasDummy->CastSpell(gasDummy,gaseousBlight[i], true);
                }
            }

            void JustDied(Unit* /*pKiller*/)
            {
                if (!instance)
                    return;

                DoScriptText(SAY_DEATH, me);

                instance->SetData(DATA_FESTERGURT_EVENT, DONE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GASTRIC_BLOAT);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_VILE_GAS);
                me->RemoveAurasDueToSpell(SPELL_PUNGENT_BLIGHT);

                if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
                    professor->AI()->DoAction(ACTION_FESTERGUT_DEATH);

                if (Creature* gasDummy = Unit::GetCreature(*me, uiGasDummyGUID))
                    gasDummy->RemoveAllAuras();
            }

            void JustReachedHome()
            {
                if (!instance)
                    return;

                instance->SetData(DATA_FESTERGURT_EVENT, FAIL);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GASTRIC_BLOAT);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_VILE_GAS);
                me->RemoveAurasDueToSpell(SPELL_PUNGENT_BLIGHT);
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

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (uiGastricBloatTimer < uiDiff)
                {
                    DoCast(me->getVictim(), SPELL_GASTRIC_BLOAT);
                    uiGastricBloatTimer = 13000;
                } else uiGastricBloatTimer -= uiDiff;

                if (uiInhaleBlightTimer < uiDiff)
                {
                    if (Creature* gasDummy = Unit::GetCreature(*me, uiGasDummyGUID))
                    {
                        if(uiInhaleBlightCount == 3)
                        {
                            DoScriptText(EMOTE_WARN_PUNGENT_BLIGHT, me);
                            DoCast(me, SPELL_PUNGENT_BLIGHT);
                            DoScriptText(SAY_PUNGENT_BLIGHT, me);
                            uiInhaleBlightCount = 0;

                            if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
                                professor->AI()->DoAction(ACTION_FESTERGUT_GAS);
                        }
                        else
                        {
                            DoCast(me, SPELL_INHALE_BLIGHT);
                            gasDummy->RemoveAurasDueToSpell(gaseousBlight[uiInhaleBlightCount]);
                            gasDummy->RemoveAurasDueToSpell(gaseousBlightVisual[uiInhaleBlightCount]);
                            ++uiInhaleBlightCount;
                        }
                    }
                    uiInhaleBlightTimer = urand(23000, 29000);
                } else uiInhaleBlightTimer -= uiDiff;

                if (uiVileGasTimer < uiDiff)
                {
                    for (uint8 i = 1; i <= 3; i++)
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_VILE_GAS, true);
                    uiVileGasTimer = 20000;
                } else uiVileGasTimer -= uiDiff;

                if (uiGasSporesTimer < uiDiff)
                {
                    for (uint8 i = 1; i <= RAID_MODE(2,3,2,3); ++i)
                        if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1))
                            target->CastSpell(target, SPELL_GAS_SPORES, true);
                    DoScriptText(EMOTE_GAS_SPORE, me);
                    DoScriptText(EMOTE_WARN_GAS_SPORE, me);
                    uiGasSporesTimer = 20000;
                } else uiGasSporesTimer -= uiDiff;

                if(uiBerserkTimer < uiDiff)
                {
                    DoCast(me, SPELL_BERSERK);
                    DoScriptText(SAY_BERSERK, me);
                    uiBerserkTimer = 300000;
                } else uiBerserkTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* instance;

            uint32 uiInhaleBlightTimer;
            uint32 uiGasSporesTimer;
            uint32 uiVileGasTimer;
            uint32 uiGastricBloatTimer;
            uint32 uiBerserkTimer;
            uint32 uiGaseousTimer;
            uint64 uiGasDummyGUID;
            uint8 uiInhaleBlightCount;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_festergutAI(creature);
        }
};

class npc_stinky_icc : public CreatureScript
{
    public:
        npc_stinky_icc() : CreatureScript("npc_stinky_icc") { }

        struct npc_stinky_iccAI : public ScriptedAI
        {
            npc_stinky_iccAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                uiDecimateTimer = 23000;
                uiMortalTimer = urand(8000, 10000);

                DoCast(me, SPELL_PLAGUE_STENCH);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if(uiDecimateTimer < uiDiff)
                {
                    DoCast(me->getVictim(), SPELL_DECIMATE);
                    uiDecimateTimer = 23000;
                } else uiDecimateTimer -= uiDiff;

                if(uiMortalTimer < uiDiff)
                {
                    DoCast(me->getVictim(), SPELL_MORTAL_WOUND);
                    uiMortalTimer = urand(8000,10000);
                } else uiMortalTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*who*/)
            {
                uint64 festergutGUID = instance ? instance->GetData64(DATA_FESTERGURT) : 0;
                if (Creature *festergut = me->GetCreature(*me, festergutGUID))
                    DoScriptText(SAY_STINKY, festergut);
            }
        private:
            InstanceScript* instance;
            uint32 uiDecimateTimer;
            uint32 uiMortalTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_stinky_iccAI(creature);
        }
};

class spell_festergut_gastric_bloat : public SpellScriptLoader
{
    public:
        spell_festergut_gastric_bloat() : SpellScriptLoader("spell_festergut_gastric_bloat") { }


        class spell_festergut_gastric_bloat_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_festergut_gastric_bloat_SpellScript);
            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Aura const* aura = GetHitUnit()->GetAura(GetSpellInfo()->Id);
                if (!(aura && aura->GetStackAmount() == 10))
                    return;

                SpellEntry const* spellInfo = sSpellStore.LookupEntry(SPELL_GASTRIC_EXPLOSION);
                if (!spellInfo)
                    return;

                spellInfo = sSpellMgr->GetSpellForDifficultyFromSpell(spellInfo, GetCaster());
                GetHitUnit()->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                GetHitUnit()->CastSpell(GetHitUnit(), spellInfo, true);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_festergut_gastric_bloat_SpellScript::HandleScript, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_festergut_gastric_bloat_SpellScript();
        }
};

class spell_festergut_blighted_spores : public SpellScriptLoader
{
    public:
        spell_festergut_blighted_spores() : SpellScriptLoader("spell_festergut_blighted_spores") { }

        class spell_festergut_blighted_spores_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_festergut_blighted_spores_AuraScript);

            void ExtraEffect(AuraEffect const* /*aurEff*/, AuraApplication const* aurApp, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster()->IsAIEnabled || GetCaster()->GetTypeId() != TYPEID_UNIT)
                    return;

                aurApp->GetTarget()->CastSpell(aurApp->GetTarget(), SPELL_INOCULATED, true);
                GetCaster()->CastSpell(aurApp->GetTarget(), SPELL_INOCULATED, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_festergut_blighted_spores_AuraScript::ExtraEffect, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_festergut_blighted_spores_AuraScript();
        }
};

void AddSC_boss_festergut()
{
    new boss_festergut();
    new npc_stinky_icc();
    new spell_festergut_gastric_bloat();
    new spell_festergut_blighted_spores();
}