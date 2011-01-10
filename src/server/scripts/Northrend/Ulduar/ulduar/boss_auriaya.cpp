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

//boss_auriaya
enum Spells
{
    //Auriaya
    SPELL_BERSERK                               = 47008 ,
    SPELL_SONIC_SCREECH                         = 64422 ,
    H_SPELL_SONIC_SCREECH                       = 64688 ,
    H_SPELL_SENTINEL_BLAST                      = 64389 ,
    SPELL_SENTINEL_BLAST                        = 64678 ,
    SPELL_TERRIFYING_SCREECH                    = 64386 ,
    SPELL_GUARDIAN_SWARM                        = 64396 ,
    //Feral Defender
    SPELL_FERAL_ESSENCE                         = 64455 ,
    SPELL_FERAL_RUSH                            = 64496 ,
    H_SPELL_FERAL_RUSH                          = 64674 ,
    SPELL_FERAL_POUNCE                          = 64478 ,
    H_SPELL_FERAL_POUNCE                        = 64669 ,
    SPELL_SLEEPING_FERAL_ESSENCE                = 64458 ,
    H_SPELL_SLEEPING_FERAL_ESSENCE              = 64676 ,
    //Sanctum Sentry
    H_SPELL_SAVAGE_POUNCE                       = 64666 ,
    SPELL_SAVAGE_POUNCE                         = 64374 ,
    SPELL_RIP_FLESH                             = 64375 ,
    H_SPELL_RIP_FLESH                           = 64667 ,
    SPELL_STRENGTH_OF_THE_PACK                  = 64369
};

enum Adds
{
    NPC_SEEPING_ESSENCE_STALKER                 = 34096
};

enum Yells
{
    SAY_AGGRO                                   = -1603050,
    SAY_SLAY_1                                  = -1603051,
    SAY_SLAY_2                                  = -1603052,
    SAY_DEATH                                   = -1603053,
    SAY_BERSERK                                 = -1603054,
};

class boss_auriaya : public CreatureScript
{
public:
    boss_auriaya() : CreatureScript("boss_auriaya") { }

    struct boss_auriaya_AI : public BossAI
    {
        boss_auriaya_AI(Creature *c) : BossAI(c, TYPE_AURIAYA), summons(me)
        {
        }

        SummonList summons;
        bool bIsBerserk;
        bool bFeralDefender;
        bool bIsCastingSentinelBlast;
        uint32 uiBerserkTimer;
        uint32 uiFeralDefenderSpawnTimer;
        uint32 uiGuardianSwarmTimer;
        uint32 uiTerrifyingScreechTimer;
        uint32 uiSonicScreechTimer;
        uint32 uiSentinelBlastTimer;

        void Reset()
        {
            _Reset();
            summons.DespawnAll();
            std::list<Creature*> lSanctumSentryList;
            GetCreatureListWithEntryInGrid(lSanctumSentryList, me, NPC_SANCTUM_SENTRY, 30.0f);
            if (!lSanctumSentryList.empty())
            {
                for(std::list<Creature*>::iterator iter = lSanctumSentryList.begin(); iter != lSanctumSentryList.end(); ++iter)
                    if (*iter)
                        if (!(*iter)->isAlive())
				        {
                            (*iter)->DisappearAndDie();
                            (*iter)->Respawn();
				        }
            }
            bIsCastingSentinelBlast = false;
            bIsBerserk = false;
            bFeralDefender = false;
            uiBerserkTimer = 600000;
            uiFeralDefenderSpawnTimer = 60000;
            uiTerrifyingScreechTimer = 30000;
            uiSonicScreechTimer = 16000;
            uiSentinelBlastTimer = 32500;
            uiGuardianSwarmTimer = urand(18000, 22000);
        }

        void JustSummoned(Creature* summoned)
        {
            if (summoned)
            {
                summons.Summon(summoned);
                if (summoned->GetEntry() == NPC_FERAL_DEFENDER)
                {
                    Unit* Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true);
                    if (!Target)
                        Target = me->getVictim();
                    summoned->AI()->AttackStart(Target);
                }
            }
        }

        void EnterCombat(Unit* who)
        {
            _EnterCombat();
			std::list<Creature*> lSanctumSentryList;
            GetCreatureListWithEntryInGrid(lSanctumSentryList, me, NPC_SANCTUM_SENTRY, 30.0f);
            if (!lSanctumSentryList.empty())
            {
                for(std::list<Creature*>::iterator iter = lSanctumSentryList.begin(); iter != lSanctumSentryList.end(); ++iter)
                    if (*iter)
                    {
                        if (!(*iter)->isAlive())
				        {
                            (*iter)->DisappearAndDie();
                            (*iter)->Respawn();
				        }
                        (*iter)->SetInCombatWithZone();
                    }
            }
            DoScriptText(SAY_AGGRO,me);
        }

        void KilledUnit(Unit* victim)
        {
            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2), me);
        }

        void JustDied(Unit *victim)
        {
		    _JustDied();
            summons.DespawnAll();
            DoScriptText(SAY_DEATH, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!bIsBerserk)
            {
                if (uiBerserkTimer <= diff)
                {
                    DoScriptText(SAY_BERSERK, me);
                    DoCast(me, SPELL_BERSERK, true);
                    bIsBerserk = true;
                } 
                else 
                    uiBerserkTimer -= diff;
            }

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if (!bFeralDefender)
            {
                if (uiFeralDefenderSpawnTimer <= diff)
                {
                    me->SummonCreature(NPC_FERAL_DEFENDER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN);
                    bFeralDefender = true;
                } 
                else 
                    uiFeralDefenderSpawnTimer -= diff;
            }

            if (uiGuardianSwarmTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                {
                    DoCast(target, SPELL_GUARDIAN_SWARM);
                }
                uiGuardianSwarmTimer = urand(26000, 34000);
            }
            else
                uiGuardianSwarmTimer -= diff;

            if (uiTerrifyingScreechTimer <= diff)
            {
                DoCast(SPELL_TERRIFYING_SCREECH);
                uiTerrifyingScreechTimer = 30000;
            } 
            else 
                uiTerrifyingScreechTimer -= diff;

            if (uiSentinelBlastTimer <= diff)
            {
                if (!bIsCastingSentinelBlast)
                {
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
                    DoCast(RAID_MODE(SPELL_SENTINEL_BLAST, H_SPELL_SENTINEL_BLAST));
                    uiSentinelBlastTimer = 4000;
                    bIsCastingSentinelBlast = true;
                }
                else
                {
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
                    uiSentinelBlastTimer = 26000;
                    bIsCastingSentinelBlast = false;
                }
            } 
            else 
                uiSentinelBlastTimer -= diff;

            if (uiSonicScreechTimer <= diff)
            {
                DoCast(me->getVictim(),RAID_MODE(SPELL_SONIC_SCREECH, H_SPELL_SONIC_SCREECH));
                uiSonicScreechTimer = 30000;
            } 
            else 
                uiSonicScreechTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_auriaya_AI (pCreature);
    }
};

class npc_feral_defender : public CreatureScript
{
public:
    npc_feral_defender() : CreatureScript("npc_feral_defender") { }

    struct npc_feral_defender_AI : public ScriptedAI
    {
        npc_feral_defender_AI(Creature *c) : ScriptedAI(c), summons(me) {}

        SummonList summons;
        InstanceScript* instance;
        uint32 uiAttackTimer;
        uint32 uiReviveTimer;
        uint32 uiNbDeath;
        uint32 uiNbAuras;
        bool bFakeDeath;

        void Reset()
        {
            summons.DespawnAll();
            uiNbDeath = 0;
            uiNbAuras = 9;
            me->AddAura(SPELL_FERAL_ESSENCE, me);
            me->SetAuraStack(SPELL_FERAL_ESSENCE, me, uiNbAuras);
            uiAttackTimer = 3000;
            bFakeDeath = false;
        }

        void EnterCombat(Unit* who)
        {
        }
        void KilledUnit(Unit* victim)
        {
        }

        void DamageTaken(Unit *done_by, uint32 &damage)
        {
            if (damage > me->GetHealth())
            {
                if (Creature* pTrigger = me->SummonCreature(NPC_SEEPING_ESSENCE_STALKER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0,TEMPSUMMON_MANUAL_DESPAWN))
                    pTrigger->CastSpell(pTrigger, RAID_MODE(SPELL_SLEEPING_FERAL_ESSENCE, H_SPELL_SLEEPING_FERAL_ESSENCE), true);
                uiNbDeath++;
                if (uiNbDeath < 9)
                {
                    uiNbAuras--;
                    bFakeDeath = true;
                    uiReviveTimer = 30000;
                    me->InterruptNonMeleeSpells(false);
                    me->SetHealth(0);
                    me->StopMoving();
                    me->ClearComboPointHolders();
                    me->RemoveAllAurasOnDeath();
                    me->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
                    me->ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, false);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->ClearAllReactives();
                    me->SetUInt64Value(UNIT_FIELD_TARGET,0);
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveIdle();
                    me->SetStandState(UNIT_STAND_STATE_DEAD);
                }
                else
                {
                    JustDied(done_by);
                }
            }
            else return;
        }

        void JustDied(Unit *killer)
        {
        }

        void JustSummoned(Creature* summoned)
        {
            if (summoned)
            {
                summons.Summon(summoned);
            }
        }

        void MoveInLineOfSight(Unit* who) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!bFakeDeath)
            {
                if (uiAttackTimer < diff)
                {
                    switch(rand()%2)
                    {
                    case 0 :
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 200, true))
                            DoCast(target, RAID_MODE(SPELL_FERAL_RUSH, H_SPELL_FERAL_RUSH));
                    case 1 :
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 200, true))
                            DoCast(target, RAID_MODE(SPELL_FERAL_POUNCE, H_SPELL_FERAL_POUNCE));
                    }
                    uiAttackTimer = 6000;
                } 
                else 
                    uiAttackTimer -= diff;

                DoMeleeAttackIfReady();
            }    
            else
            {
                if (uiReviveTimer <= diff)
                {                
                    Unit* Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true);
                    if (!Target)
                        Target = me->getVictim();
                    DoResetThreat();
                    AttackStart(Target);
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveChase(Target);
                    me->AddThreat(Target, 0.0f);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetHealth(me->GetMaxHealth());
                    me->SetStandState(UNIT_STAND_STATE_STAND);
                    me->AddAura(SPELL_FERAL_ESSENCE, me);
                    me->SetAuraStack(SPELL_FERAL_ESSENCE, me, uiNbAuras);
                    bFakeDeath = false;
                } else uiReviveTimer -= diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_feral_defender_AI (pCreature);
    }
};

class npc_sanctum_sentry : public CreatureScript
{
public:
    npc_sanctum_sentry() : CreatureScript("npc_sanctum_sentry") { }

    struct npc_sanctum_sentryAI : public ScriptedAI
    {
        npc_sanctum_sentryAI(Creature *c) : ScriptedAI(c) {}

        uint32 uiSavagePounceTimer;
        uint32 uiRipFleshTimer;

        void Reset()
        {
            uiSavagePounceTimer = urand(4000, 7000);
            uiRipFleshTimer = urand(9000, 13000);
        }

        void EnterCombat(Unit* who)
        {
            DoCast(me, SPELL_STRENGTH_OF_THE_PACK);
        }

        void KilledUnit(Unit* victim)
        {
        }

        void JustDied(Unit *victim)
        {
        }

        void MoveInLineOfSight(Unit* who) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (uiSavagePounceTimer <= diff)
            {
                DoCast(me->getVictim(), RAID_MODE(SPELL_SAVAGE_POUNCE, H_SPELL_SAVAGE_POUNCE));
                uiSavagePounceTimer = urand(9000, 13000);
            } 
            else 
                uiSavagePounceTimer -= diff;

            if (uiRipFleshTimer <= diff)
            {
                DoCast(me->getVictim(), RAID_MODE(SPELL_RIP_FLESH, H_SPELL_RIP_FLESH));
                uiRipFleshTimer = urand(9000, 13000);
            } 
            else 
                uiRipFleshTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_sanctum_sentryAI (pCreature);
    }
};

void AddSC_boss_auriaya()
{
    new boss_auriaya();
	new npc_feral_defender();
    new npc_sanctum_sentry();
}
