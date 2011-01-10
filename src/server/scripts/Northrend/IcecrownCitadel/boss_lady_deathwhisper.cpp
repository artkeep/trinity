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

enum Spells
{
    SPELL_DEATH_AND_DECAY   = 71001,
    SPELL_DOMINATE_MIND     = 71289,
    SPELL_SHADOW_BOLT       = 71254,
    SPELL_MANA_BARRIER      = 70842,
    SPELL_TRANSFORMATION    = 70900,
    SPELL_FROST_BOLT        = 71420,
    SPELL_FROST_BOLT_VALLEY = 72905,
    SPELL_SUMMON_SHADE      = 71363,
    SPELL_INSIGNIFICANCE    = 71204,
    SPELL_BLAST             = 71494,
    SPELL_VENGEFUL_BLAST    = 71544,
    SPELL_PORT_VISUAL       = 41236,
    SPELL_SHADOW_CHANNELING = 43897,
    SPELL_TRANSFORMATION_T  = 70895,
    SPELL_EMPOWERMENT_T     = 70896,
    //SPELL_ANIMATED_DEAD   = ????? // not in DBC! Need sniffs
};

enum Yells
{
    SAY_INTRO_1        = -1631011,
    SAY_INTRO_2        = -1631012,
    SAY_INTRO_3        = -1631013,
    SAY_INTRO_4        = -1631014,
    SAY_INTRO_5        = -1631015,
    SAY_INTRO_6        = -1631016,
    SAY_INTRO_7        = -1631017,
    SAY_AGGRO          = -1631018,
    SAY_PHASE_2        = -1631019,
    EMOTE_PHASE_2      = -1631020,
    SAY_DOMINATE_MIND  = -1631021,
    SAY_EMPOWERMENT    = -1631022,
    SAY_TRANSFORMATION = -1631023,
    SAY_ANIMATE_DEAD   = -1631024,
    SAY_KILL_1         = -1631025,
    SAY_KILL_2         = -1631026,
    SAY_BERSERK        = -1631027,
    SAY_DEATH          = -1631028
};

const Position SpawnLoc[]=
{
    {-578.7066f, 2154.167f, 51.01529f, 1.692969f},
    {-598.9028f, 2155.005f, 51.01530f, 1.692969f},
    {-619.2864f, 2154.460f, 51.01530f, 1.692969f},
    {-578.6996f, 2269.856f, 51.01529f, 4.590216f},
    {-598.9688f, 2269.264f, 51.01529f, 4.590216f},
    {-619.4323f, 2268.523f, 51.01530f, 4.590216f},
    {-524.2480f, 2211.920f, 62.90960f, 3.141592f},
    {-623.0554f, 2211.326f, 51.76425f, 3.444574f}
};

class boss_lady_deathwhisper : public CreatureScript
{
    public:
        boss_lady_deathwhisper() : CreatureScript("boss_lady_deathwhisper") { }

        struct boss_lady_deathwhisperAI : public BossAI
        {
            boss_lady_deathwhisperAI(Creature* pCreature) : BossAI(pCreature, DATA_DEATHWHISPER), summons(me)
            {
                pInstance = pCreature->GetInstanceScript();
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(SPELL_FROST_BOLT, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
                bIntro = false;
                bFirstSummon = false;
            }

            void Reset()
            {
                m_uiPhase = 1;
                m_uiIntroPhase = 1;
                m_uiStage = 1;

                m_uiIntroTimer = 1000;
                m_uiSummonWaveTimer = !bFirstSummon ? 10000 : 60000;
                m_uiDeathandDecayTimer = 10000;
                m_uiFrostBoltTimer = 10000;
                m_uiFrostValleyTimer = 18000;
                m_uiShadowBoltTimer = 6000;
                m_uiBerserkTimer = 600000;
                m_uiInsignificanceTimer = 7000;
                m_uiShadeTimer = 10000;
                m_uiDominateMindTimer = 27000;
                m_uiCultTimer = urand(20000, 23000);

                me->SetLastManaUse(0xFFFFFFFF);

                DoCast(me, SPELL_SHADOW_CHANNELING);

                if (pInstance)
                    pInstance->SetData(DATA_DEATHWHISPER_EVENT, NOT_STARTED);
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoScriptText(SAY_AGGRO, me);

                me->RemoveAurasDueToSpell(SPELL_SHADOW_CHANNELING);

                if (pInstance)
                    pInstance->SetData(DATA_DEATHWHISPER_EVENT, IN_PROGRESS);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                DoScriptText(SAY_DEATH, me);

                if (pInstance)
                    pInstance->SetData(DATA_DEATHWHISPER_EVENT, DONE);
            }

            void JustReachedHome()
            {
                if(pInstance)
                    pInstance->SetData(DATA_DEATHWHISPER_EVENT, FAIL);

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

            void MoveInLineOfSight(Unit *pWho)
            {
                if (!bIntro && me->IsWithinDistInMap(pWho, 100.0f, true))
                {
                    DoScriptText(SAY_INTRO_1, me);
                    m_uiIntroTimer = 11000;
                    bIntro = true;
                }
            }

            void JustSummoned(Creature* pSummon)
            {
                DoCast(pSummon, SPELL_PORT_VISUAL);

                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 120.0f, true))
                    pSummon->AI()->AttackStart(pTarget);

                summons.Summon(pSummon);
            }

            void RandomSpawn()
            {
                switch(m_uiStage)
                {
                    case 1:
                        DoSummon(CREATURE_FANATIC, SpawnLoc[0]);
                        DoSummon(CREATURE_ADHERENT, SpawnLoc[1]);
                        DoSummon(CREATURE_FANATIC, SpawnLoc[2]);
                        ++m_uiStage;
                        break;
                    case 2:
                        DoSummon(CREATURE_ADHERENT, SpawnLoc[3]);
                        DoSummon(CREATURE_FANATIC, SpawnLoc[4]);
                        DoSummon(CREATURE_ADHERENT, SpawnLoc[5]);
                        --m_uiStage;
                        break;
                }
            }

            void DamageTaken(Unit* /*damageDealer*/, uint32& uiDamage)
            {
                if(!me->HasAura(SPELL_MANA_BARRIER) && m_uiPhase == 1)
                {
                    me->CastSpell(me, SPELL_MANA_BARRIER, true);
                }

                if(me->HasAura(SPELL_MANA_BARRIER))
                {
                    me->SetHealth(me->GetHealth() +uiDamage);
                    me->SetPower(POWER_MANA, (me->GetPower(POWER_MANA) -uiDamage));

                    if(uiDamage > me->GetPower(POWER_MANA) && m_uiPhase == 1)
                    {
                        DoScriptText(SAY_PHASE_2, me);
                        DoScriptText(EMOTE_PHASE_2, me);
                        me->SetPower(POWER_MANA, 0);
                        me->RemoveAurasDueToSpell(SPELL_MANA_BARRIER);
                        DoResetThreat();
                        m_uiPhase = 2;
                    }

                    if (IsHeroic())
                        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                }
            }

            void EmpowerCultist()
            {
                if (summons.empty())
                    return;

                std::list<Creature*> tmpList;
                for (SummonList::iterator itr = summons.begin(); itr != summons.end(); ++itr)
                    if (Creature* cre = ObjectAccessor::GetCreature(*me, *itr))
                        if (cre->isAlive() && (cre->GetEntry() == CREATURE_FANATIC || cre->GetEntry() == CREATURE_ADHERENT))
                            tmpList.push_back(cre);

                if (tmpList.empty())
                    return;

                std::list<Creature*>::iterator cultistItr = tmpList.begin();
                std::advance(cultistItr, urand(0, tmpList.size()-1));

                Creature* cultist = *cultistItr;
                DoCast(cultist, cultist->GetEntry() == CREATURE_FANATIC ? SPELL_TRANSFORMATION_T : SPELL_EMPOWERMENT_T, true);
                DoScriptText(cultist->GetEntry() == CREATURE_FANATIC ? SAY_TRANSFORMATION : SAY_EMPOWERMENT, me);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if(m_uiPhase == 1)
                    DoStartNoMovement(me->getVictim());
                else if(m_uiPhase == 2)
                    DoStartMovement(me->getVictim());

                if (m_uiIntroTimer <= uiDiff && bIntro && m_uiIntroPhase < 7)
                {
                    switch (m_uiIntroPhase)
                    {
                        case 1: DoScriptText(SAY_INTRO_2, me); m_uiIntroTimer = 10000; break;
                        case 2: DoScriptText(SAY_INTRO_3, me); m_uiIntroTimer = 7000;  break;
                        case 3: DoScriptText(SAY_INTRO_4, me); m_uiIntroTimer = 12000; break;
                        case 4: DoScriptText(SAY_INTRO_5, me); m_uiIntroTimer = 7000;  break;
                        case 5: DoScriptText(SAY_INTRO_6, me); m_uiIntroTimer = 11000; break;
                        case 6: DoScriptText(SAY_INTRO_7, me); m_uiIntroTimer = 20000; break;
                    }
                    ++m_uiIntroPhase;
                } else m_uiIntroTimer -= uiDiff;

                if (!UpdateVictim())
                    return;

                if (m_uiBerserkTimer < uiDiff)
                {
                    DoCast(me, SPELL_BERSERK);
                    DoScriptText(SAY_BERSERK, me);
                    m_uiBerserkTimer = 600000;
                } else m_uiBerserkTimer -= uiDiff;

                if (m_uiCultTimer < uiDiff)
                {
                    EmpowerCultist();
                    m_uiCultTimer = urand(20000, 23000);
                } else m_uiCultTimer -= uiDiff;

                if (m_uiDeathandDecayTimer < uiDiff)
                {
                    if(Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                        DoCast(pTarget, SPELL_DEATH_AND_DECAY);
                    m_uiDeathandDecayTimer = 11000;
                } else m_uiDeathandDecayTimer -= uiDiff;

                if (m_uiDominateMindTimer < uiDiff)
                {
                    for (uint8 i = 1; i <= RAID_MODE(0,1,1,3); ++i)
                    {
                        if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_DOMINATE_MIND))
                            DoCast(pTarget, SPELL_DOMINATE_MIND);
                    }
                    DoScriptText(SAY_DOMINATE_MIND, me);
                    m_uiDominateMindTimer = 15000;
                } else m_uiDominateMindTimer -= uiDiff;

                if (m_uiPhase == 1)
                {
                    if (m_uiShadowBoltTimer < uiDiff)
                    {
                        if(Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                            DoCast(pTarget, SPELL_SHADOW_BOLT);
                        m_uiShadowBoltTimer = 6000;
                    } else m_uiShadowBoltTimer -= uiDiff;

                    if (m_uiSummonWaveTimer < uiDiff)
                    {
                        if (Difficulty() == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty() == RAID_DIFFICULTY_10MAN_HEROIC)
                            RandomSpawn();
                        else
                        {
                            DoSummon(CREATURE_FANATIC, SpawnLoc[0]);
                            DoSummon(CREATURE_ADHERENT, SpawnLoc[1]);
                            DoSummon(CREATURE_FANATIC, SpawnLoc[2]);
                            DoSummon(CREATURE_ADHERENT, SpawnLoc[3]);
                            DoSummon(CREATURE_FANATIC, SpawnLoc[4]);
                            DoSummon(CREATURE_ADHERENT, SpawnLoc[5]);
                            DoSummon(RAND(CREATURE_FANATIC,CREATURE_ADHERENT), SpawnLoc[6]);
                        }
                        bFirstSummon = false ? true : true;
                        m_uiSummonWaveTimer = !bFirstSummon ? 10000 : 60000;
                    } else m_uiSummonWaveTimer -= uiDiff;
                }

                if (m_uiPhase == 2)
                {
                    if (m_uiFrostBoltTimer < uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_FROST_BOLT);
                        m_uiFrostBoltTimer = 10000;
                    } else m_uiFrostBoltTimer -= uiDiff;

                    if (m_uiFrostValleyTimer < uiDiff)
                    {
                        DoCast(SPELL_FROST_BOLT_VALLEY);
                        m_uiFrostValleyTimer = 19000;
                    } else m_uiFrostValleyTimer -= uiDiff;

                    if (m_uiInsignificanceTimer < uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_INSIGNIFICANCE);
                        m_uiInsignificanceTimer = 8000;
                    } else m_uiInsignificanceTimer -= uiDiff;

                    if (m_uiSummonWaveTimer < uiDiff)
                    {
                        if (Difficulty() == RAID_DIFFICULTY_10MAN_HEROIC)
                            DoSummon(RAND(CREATURE_FANATIC,CREATURE_ADHERENT), SpawnLoc[6]);
                        if (Difficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
                            RandomSpawn();
                        m_uiSummonWaveTimer = 60000;
                    } else m_uiSummonWaveTimer -= uiDiff;

                    if (m_uiShadeTimer < uiDiff)
                    {
                        DoCast(SPELL_SUMMON_SHADE);
                        m_uiShadeTimer = 15000;
                    } else m_uiShadeTimer -= uiDiff;

                    DoMeleeAttackIfReady();
                }
            }

        private:
            InstanceScript* pInstance;

            uint8 m_uiPhase;
            uint8 m_uiIntroPhase;
            uint8 m_uiStage;
            uint32 m_uiDominateMindTimer;
            uint32 m_uiSummonWaveTimer;
            uint32 m_uiDeathandDecayTimer;
            uint32 m_uiFrostBoltTimer;
            uint32 m_uiFrostValleyTimer;
            uint32 m_uiShadowBoltTimer;
            uint32 m_uiBerserkTimer;
            uint32 m_uiInsignificanceTimer;
            uint32 m_uiShadeTimer;
            uint32 m_uiCultTimer;
            uint32 m_uiIntroTimer;

            bool bIntro;
            bool bFirstSummon;

            SummonList summons;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_lady_deathwhisperAI(pCreature);
        }
};

class npc_shade : public CreatureScript
{
    public:
        npc_shade() : CreatureScript("npc_shade") { }

        struct npc_shadeAI : public ScriptedAI
        {
            npc_shadeAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetInCombatWithZone();
                DoCast(me, SPELL_BLAST);
            }

            void SpellHitTarget(Unit * /*pTarget*/, const SpellEntry *spell)
            {
                if (spell->Id == SPELL_VENGEFUL_BLAST)
                    me->ForcedDespawn();
            }

            void UpdateAI(const uint32 /*uiDiff*/)
            {
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_shadeAI(pCreature);
        }
};

void AddSC_boss_lady_deathwhisper()
{
    new boss_lady_deathwhisper();
    new npc_shade();
}
