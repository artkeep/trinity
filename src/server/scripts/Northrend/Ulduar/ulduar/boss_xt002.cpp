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

#include "ScriptPCH.h"
#include "ulduar.h"

enum Spells
{
    SPELL_TYMPANIC_TANTRUM                      = 62776,
    SPELL_SEARING_LIGHT_10                      = 63018,
    SPELL_SEARING_LIGHT_25                      = 65121,

    SPELL_GRAVITY_BOMB_10                       = 63024,
    SPELL_GRAVITY_BOMB_25                       = 64234,

    SPELL_HEARTBREAK_10                         = 65737,
    SPELL_HEARTBREAK_25                         = 64193,

    SPELL_ENRAGE                                = 26662,

    //------------------VOID ZONE--------------------
    SPELL_VOID_ZONE_10                          = 64203,
    SPELL_VOID_ZONE_25                          = 64235,
    SPELL_VOID_ZONE                             = 46264,

    // Life Spark
    SPELL_STATIC_CHARGED_10                     = 64227,
    SPELL_STATIC_CHARGED_25                     = 64236,
    SPELL_SHOCK                                 = 64230,

    //----------------XT-002 HEART-------------------
    SPELL_EXPOSED_HEART                         = 63849,

    //---------------XM-024 PUMMELLER----------------
    SPELL_ARCING_SMASH                          = 8374,
    SPELL_TRAMPLE                               = 5568,
    SPELL_UPPERCUT                              = 10966,

    //------------------BOOMBOT-----------------------
    SPELL_BOOM                                  = 62834,

    //------------------SCRAPBOT-----------------------
    SPELL_REPAIR                                = 62832,
};

enum Timers
{
    TIMER_TYMPANIC_TANTRUM                      = 60000,
    TIMER_SEARING_LIGHT                         = 9000,
    TIMER_SPAWN_LIFE_SPARK                      = 9000,
    TIMER_GRAVITY_BOMB                          = 9000,
    TIMER_SPAWN_GRAVITY_BOMB                    = 9000,
    TIMER_HEART_PHASE                           = 33000,
    TIMER_ENRAGE                                = 600000,

    TIMER_VOID_ZONE                             = 2000,

    // Life Spark
    TIMER_SHOCK                                 = 12000,

    // Pummeller
    // Timers may be off
    TIMER_ARCING_SMASH                          = 27000,
    TIMER_TRAMPLE                               = 22000,
    TIMER_UPPERCUT                              = 17000,

    TIMER_SPAWN_ADD                             = 999999,
};

enum Creatures
{
    NPC_VOID_ZONE                               = 34001,
    NPC_LIFE_SPARK                              = 34004,
    NPC_XT002_HEART                             = 33329,
    NPC_XS013_SCRAPBOT                          = 33343,
    NPC_XM024_PUMMELLER                         = 33344,
    NPC_XE321_BOOMBOT                           = 33346,
};

enum Actions
{
    ACTION_ENTER_HARD_MODE                      = 0,
    ACTION_DISABLE_NERF_ACHI                    = 1,
    ACTION_NERF_SCRAPBOTS                       = 2
};

enum XT002Data
{
    DATA_TRANSFERED_HEALTH                      = 0,
};

enum Yells
{
    SAY_AGGRO                                   = -1603300,
    SAY_HEART_OPENED                            = -1603301,
    SAY_HEART_CLOSED                            = -1603302,
    SAY_TYMPANIC_TANTRUM                        = -1603303,
    SAY_SLAY_1                                  = -1603304,
    SAY_SLAY_2                                  = -1603305,
    SAY_BERSERK                                 = -1603306,
    SAY_DEATH                                   = -1603307,
    SAY_SUMMON                                  = -1603308,
};

#define EMOTE_TYMPANIC    "XT-002 Deconstructor begins to cause the earth to quake."
#define EMOTE_HEART       "XT-002 Deconstructor's heart is exposed and leaking energy."
#define EMOTE_REPAIR      "XT-002 Deconstructor consumes a scrap bot to repair himself!"

#define ACHIEVEMENT_DECONSTRUCT_FASTER        RAID_MODE(2937, 2938)
#define ACHIEVEMENT_HEARTBREAKER              RAID_MODE(3058, 3059)
#define ACHIEVEMENT_NERF_ENG                  RAID_MODE(2931, 2932)
#define ACHIEVEMENT_NERF_SCRAPBOT             RAID_MODE(2933, 2935)
#define ACHIEV_TIMED_START_EVENT              21027
#define MAX_ENCOUNTER_TIME                    205 * 1000

/************************************************
-----------------SPAWN LOCATIONS-----------------
************************************************/
//Shared Z-level
#define SPAWN_Z                                 412.0f
//Lower right
#define LR_X                                    796.0f
#define LR_Y                                    -94.0f
//Lower left
#define LL_X                                    796.0f
#define LL_Y                                    57.0f
//Upper right
#define UR_X                                    890.0f
#define UR_Y                                    -82.0f
//Upper left
#define UL_X                                    894.0f
#define UL_Y                                    62.0f

/*-------------------------------------------------------
 *
 *        XT-002 DECONSTRUCTOR
 *
 *///----------------------------------------------------
class boss_xt002 : public CreatureScript
{
public:
    boss_xt002() : CreatureScript("boss_xt002") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_xt002_AI(pCreature);
    }

    struct boss_xt002_AI : public BossAI
    {
        boss_xt002_AI(Creature *pCreature) : BossAI(pCreature, TYPE_XT002), vehicle(me->GetVehicleKit()), lSummons(me)
        {
            assert(vehicle);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
        }

        SummonList lSummons;
        Vehicle *vehicle;
        
        uint32 uiEncounterTime;
        uint32 uiSearingLightTimer;
        uint32 uiSpawnLifeSparkTimer;
        uint32 uiGravityBombTimer;
        uint32 uiSpawnGravityBombTimer;
        uint32 uiTympanicTantrumTimer;
        uint32 uiHeartPhaseTimer;
        uint32 uiSpawnAddTimer;
        uint32 uiEnrageTimer;
        uint32 uiNerfedScrapbots;
        uint32 uiNerfScrapbotsTimer;

        bool bSearingLightActive;
        bool bGravityBombActive;
        uint64 uiSearingLightTarget;

        uint64 uiGravityBombTarget;

        uint8 uiPhase;
        uint8 uiHeartExposed;
        uint32 uiSpawnPhase;
        bool bIsEnraged;

        uint32 transferHealth;
        bool bEnterHardMode;
        bool bIsHardMode;
        bool bAchievementNerf;
        bool bAchievementNerfScrapbots;

        void Reset()
        {
            _Reset();
            lSummons.DespawnAll();
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            me->ResetLootMode();

            //Makes XT-002 to cast a light bomb 10 seconds after aggro.
            uiSearingLightTimer = TIMER_SEARING_LIGHT / 2;
            uiSpawnLifeSparkTimer = TIMER_SPAWN_LIFE_SPARK;
            uiGravityBombTimer = TIMER_GRAVITY_BOMB;
            uiSpawnGravityBombTimer = TIMER_SPAWN_GRAVITY_BOMB;
            uiHeartPhaseTimer = TIMER_HEART_PHASE;
            uiSpawnAddTimer = TIMER_SPAWN_ADD;
            uiEnrageTimer = TIMER_ENRAGE;
            uiTympanicTantrumTimer = TIMER_TYMPANIC_TANTRUM / 2;
            uiNerfScrapbotsTimer = 0;
            uiNerfedScrapbots = 0;
            uiEncounterTime = 0;

            bSearingLightActive = false;
            bGravityBombActive = false;
            bIsEnraged = false;
            bIsHardMode = false;
            bEnterHardMode = false;
            bAchievementNerf = true;
            bAchievementNerfScrapbots = false;

            if (instance)
                instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_TIMED_START_EVENT);
        }

        void JustReachedHome()
        {
        }

        void EnterCombat(Unit* who)
        {
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
            _EnterCombat();

            if (instance)
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_TIMED_START_EVENT);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_ENTER_HARD_MODE:
                    if (!bIsHardMode)
                    {
                        bIsHardMode = true;

                        // Enter hard mode
                        bEnterHardMode = true;

                        // set max health
                        me->SetFullHealth();

                        // Get his heartbreak buff
                        me->CastSpell(me, RAID_MODE(SPELL_HEARTBREAK_10, SPELL_HEARTBREAK_25), true);
                    }
                    break;
                case ACTION_DISABLE_NERF_ACHI:
                    bAchievementNerf = false;
                    break;
                case ACTION_NERF_SCRAPBOTS:
                    if (!bAchievementNerfScrapbots)
                    {
                        bAchievementNerfScrapbots = true;
                       uiNerfScrapbotsTimer = 12000;
                    }
                    uiNerfedScrapbots ++;
                    if (uiNerfedScrapbots >= 20)
                        if (instance)
                            instance->DoCompleteAchievement(ACHIEVEMENT_NERF_SCRAPBOT);
                    break;
            }
        }

        void SetData(uint32 id, uint32 value)
        {
            switch(id)
            {
                case DATA_TRANSFERED_HEALTH:
                    transferHealth = value;
                    break;
            }
        }

        void KilledUnit(Unit* victim)
        {
            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2), me);
        }

        void JustDied(Unit *victim)
        {
            _JustDied();
            DoScriptText(SAY_DEATH, me);

            // Achievements
            if (instance)
            {
                // Heartbreaker
                if (bIsHardMode)
                    instance->DoCompleteAchievement(ACHIEVEMENT_HEARTBREAKER);
                // Must Deconstruct Faster
                if (uiEncounterTime <= MAX_ENCOUNTER_TIME)
                    instance->DoCompleteAchievement(ACHIEVEMENT_DECONSTRUCT_FASTER);
                // Nerf Engineering
                if (bAchievementNerf)
                    instance->DoCompleteAchievement(ACHIEVEMENT_NERF_ENG);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (bEnterHardMode)
            {
                SetPhaseOne();
                bEnterHardMode = false;
            }

            uiEncounterTime += diff;

            if (bAchievementNerfScrapbots)
            {
                if (uiNerfScrapbotsTimer <= diff)
                {
                    uiNerfedScrapbots = 0;
                    bAchievementNerfScrapbots = false;
                } else uiNerfScrapbotsTimer -= diff;
            }

            // Handles spell casting. These spells only occur during uiPhase 1 and hard mode
            if (uiPhase == 1 || bIsHardMode)
            {
                if (uiSearingLightTimer <= diff)
                {
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                    {
                        me->AddAura(RAID_MODE(SPELL_SEARING_LIGHT_10, SPELL_SEARING_LIGHT_25), pTarget);
                        uiSearingLightTarget = pTarget->GetGUID();
                    }
                    uiSpawnLifeSparkTimer = TIMER_SPAWN_LIFE_SPARK;
                    if (bIsHardMode)
                        bSearingLightActive = true;
                    uiSearingLightTimer = TIMER_SEARING_LIGHT;
                } else uiSearingLightTimer -= diff;

                if (uiGravityBombTimer <= diff)
                {
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                    {
                        me->AddAura(RAID_MODE(SPELL_GRAVITY_BOMB_10,SPELL_GRAVITY_BOMB_25), pTarget);
                        uiGravityBombTarget = pTarget->GetGUID();
                    }
                    uiGravityBombTimer = TIMER_GRAVITY_BOMB;
                    if (bIsHardMode)
                        bGravityBombActive = true;
                } else uiGravityBombTimer -= diff;

                if (uiTympanicTantrumTimer <= diff)
                {
                    DoScriptText(SAY_TYMPANIC_TANTRUM, me);
                    me->MonsterTextEmote(EMOTE_TYMPANIC, 0, true);
                    DoCast(SPELL_TYMPANIC_TANTRUM);
                    uiTympanicTantrumTimer = TIMER_TYMPANIC_TANTRUM;
                } else uiTympanicTantrumTimer -= diff;
            }

            if (!bIsHardMode)
            {
                if (uiPhase == 1)
                {
                    if (HealthBelowPct(75) && uiHeartExposed == 0)
                    {
                        exposeHeart();
                    }
                    else if (HealthBelowPct(50) && uiHeartExposed == 1)
                    {
                        exposeHeart();
                    }
                    else if (HealthBelowPct(25) && uiHeartExposed == 2)
                    {
                        exposeHeart();
                    }

                    DoMeleeAttackIfReady();
                }
                else
                {
                    //Start summoning adds
                    if (uiSpawnAddTimer <= diff)
                    {
                        DoScriptText(SAY_SUMMON, me);

                        if (uiSpawnPhase < 2)
                        {
                            // Spawn Pummeller
                            switch (rand() % 4)
                            {
                                case 0: me->SummonCreature(NPC_XM024_PUMMELLER, LR_X, LR_Y, SPAWN_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000); break;
                                case 1: me->SummonCreature(NPC_XM024_PUMMELLER, LL_X, LL_Y, SPAWN_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000); break;
                                case 2: me->SummonCreature(NPC_XM024_PUMMELLER, UR_X, UR_Y, SPAWN_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000); break;
                                case 3: me->SummonCreature(NPC_XM024_PUMMELLER, UL_X, UL_Y, SPAWN_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000); break;
                            }
                            // Spawn Boombot
                            switch (rand() % 4)
                            {
                                case 0: me->SummonCreature(NPC_XE321_BOOMBOT, float(irand(LR_X - 3, LR_X + 3)), float(irand(LR_Y - 3, LR_Y + 3)), SPAWN_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000); break;
                                case 1: me->SummonCreature(NPC_XE321_BOOMBOT, float(irand(LL_X - 3, LL_X + 3)), float(irand(LL_Y - 3, LL_Y + 3)), SPAWN_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000); break;
                                case 2: me->SummonCreature(NPC_XE321_BOOMBOT, float(irand(UR_X - 3, UR_X + 3)), float(irand(UR_Y - 3, UR_Y + 3)), SPAWN_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000); break;
                                case 3: me->SummonCreature(NPC_XE321_BOOMBOT, float(irand(UL_X - 3, UL_X + 3)), float(irand(UL_Y - 3, UL_Y + 3)), SPAWN_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000); break;
                            }
                            // Spawn Scrapbots
                            uint8 maxBots = urand(5, 8);
                            switch(rand() % 4)
                            {
                                case 0: 
                                    for (int8 n = 0; n < maxBots; n++)
                                        me->SummonCreature(NPC_XS013_SCRAPBOT, irand(LR_X - 3, LR_X + 3), irand(LR_Y - 3, LR_Y + 3), SPAWN_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000); break;
                                case 1: 
                                    for (int8 n = 0; n < maxBots; n++)
                                        me->SummonCreature(NPC_XS013_SCRAPBOT, irand(LL_X - 3, LL_X + 3), irand(LL_Y - 3, LL_Y + 3), SPAWN_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000); break;
                                case 2: 
                                    for (int8 n = 0; n < maxBots; n++)
                                        me->SummonCreature(NPC_XS013_SCRAPBOT, irand(UR_X - 3, UR_X + 3), irand(UR_Y - 3, UR_Y + 3), SPAWN_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000); break;
                                case 3: 
                                    for (int8 n = 0; n < maxBots; n++)
                                        me->SummonCreature(NPC_XS013_SCRAPBOT, irand(UL_X - 3, UL_X + 3), irand(UL_Y - 3, UL_Y + 3), SPAWN_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000); break;
                            }
                        }

                        uiSpawnPhase++;

                        uiSpawnAddTimer = urand(3000, 5000);
                    } else uiSpawnAddTimer -= diff;

                    // Is the uiPhase over?
                    if (uiHeartPhaseTimer <= diff)
                    {
                        DoScriptText(SAY_HEART_CLOSED, me);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                        SetPhaseOne();
                    }
                    else
                        uiHeartPhaseTimer -= diff;
                }
            }
            else
            {
                // Adding life sparks when searing light debuff runs out if hard mode
                if (bSearingLightActive)
                {
                    if (uiSpawnLifeSparkTimer <= diff)
                    {
                        if (Unit *pSearingLightTarget = me->GetUnit(*me, uiSearingLightTarget))
                            pSearingLightTarget->SummonCreature(NPC_LIFE_SPARK, pSearingLightTarget->GetPositionX(), pSearingLightTarget->GetPositionY(), pSearingLightTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                        uiSpawnLifeSparkTimer = TIMER_SPAWN_LIFE_SPARK;
                        bSearingLightActive = false;
                    } else uiSpawnLifeSparkTimer -= diff;
                }

                // Adding void zones when gravity bomb debuff runs out if hard mode
                if (bGravityBombActive)
                {
                    if (uiSpawnGravityBombTimer <= diff)
                    {
                        if (Unit *pGravityBombTarget = me->GetUnit(*me, uiGravityBombTarget))
                            DoCast(pGravityBombTarget, RAID_MODE(SPELL_VOID_ZONE_10, SPELL_VOID_ZONE_25));
                        uiSpawnGravityBombTimer = TIMER_SPAWN_GRAVITY_BOMB;
                        bGravityBombActive = false;
                    } else uiSpawnGravityBombTimer -= diff;
                }

                DoMeleeAttackIfReady();
            }

            //Enrage stuff
            if (!bIsEnraged)
            {
                if (uiEnrageTimer <= diff)
                {
                    DoScriptText(SAY_BERSERK, me);
                    DoCast(me, SPELL_ENRAGE);
                    bIsEnraged = true;
                } else uiEnrageTimer -= diff;
            }
        }

        void exposeHeart()
        {
            //Make untargetable
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
            me->SetReactState(REACT_PASSIVE);
            me->SetStandState(UNIT_STAND_STATE_SUBMERGED);
            me->AttackStop();
            me->CastStop();

            //Summon the heart npc
            Creature* Heart = me->SummonCreature(NPC_XT002_HEART, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 4, me->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, TIMER_HEART_PHASE);
            if (Heart){
                 Heart->EnterVehicle(me,0);
                 Heart->EnterVehicle(me,1);
            }
            // Start "end of uiPhase 2 timer"
            uiHeartPhaseTimer = TIMER_HEART_PHASE;

            //Phase 2 has offically started
            uiPhase = 2;
            uiHeartExposed++;

            //Reset the add spawning timer
            uiSpawnAddTimer = 6000;
            uiSpawnPhase = 0;

            DoScriptText(SAY_HEART_OPENED, me);
            me->MonsterTextEmote(EMOTE_HEART, 0, true);
            me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);
        }

        void SetPhaseOne()
        {
            uiSearingLightTimer = TIMER_SEARING_LIGHT / 2;
            uiGravityBombTimer = TIMER_GRAVITY_BOMB;
            uiTympanicTantrumTimer = TIMER_TYMPANIC_TANTRUM / 2;
            uiSpawnAddTimer = TIMER_SPAWN_ADD;

            if (!bIsHardMode)
                me->ModifyHealth(-((int32)transferHealth));
            else
                // Add HardMode Loot
                me->AddLootMode(LOOT_MODE_HARD_MODE_1);

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetStandState(UNIT_STAND_STATE_STAND);
            DoZoneInCombat();
            uiPhase = 1;
        }

        void JustSummoned(Creature *summon)
        {
            lSummons.Summon(summon);
            switch (summon->GetEntry())
            {
            case NPC_XE321_BOOMBOT:
            case NPC_XM024_PUMMELLER:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                {
                    summon->SetInCombatWith(target);
                    target->SetInCombatWith(summon);
                    summon->AddThreat(target, 0.0f);
                }
                summon->AI()->DoZoneInCombat();
                break ;
            case NPC_XS013_SCRAPBOT:
                summon->GetMotionMaster()->MoveChase(me);
                break ;
            }
            summon->setActive(true);
        }
    };
};

/*-------------------------------------------------------
 *
 *        XT-002 HEART
 *
 *///----------------------------------------------------
class mob_xt002_heart : public CreatureScript
{
public:
    mob_xt002_heart() : CreatureScript("mob_xt002_heart") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_xt002_heartAI(pCreature);
    }

    struct mob_xt002_heartAI : public ScriptedAI
    {
        mob_xt002_heartAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_STUNNED);
            me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }

        InstanceScript* m_pInstance;
        uint32 uiExposeTimer;
        bool Exposed;

        void JustDied(Unit *victim)
        {
            if (m_pInstance)
                if (Creature* pXT002 = me->GetCreature(*me, m_pInstance->GetData64(DATA_XT002)))
                    if (pXT002->AI())
                        pXT002->AI()->DoAction(ACTION_ENTER_HARD_MODE);

            me->DespawnOrUnsummon();
        }
        
        void UpdateAI(const uint32 diff)
        {
            if (!Exposed)
            {
                if (uiExposeTimer <= diff)
                {
                    DoCast(me, SPELL_EXPOSED_HEART);
                    Exposed = true;
                }
                else uiExposeTimer -= diff;
            }
        }
        
        void Reset()
        {
            uiExposeTimer = 3000;
            Exposed = false;
        }

        void DamageTaken(Unit *pDone, uint32 &damage)
        {
            if (Creature* pXT002 = me->GetCreature(*me, m_pInstance->GetData64(DATA_XT002)))
                if (pXT002->AI())
                {
                    uint32 health = me->GetHealth();
                    if (health <= damage)
                        health = 0;
                    else
                        health -= damage;
                    pXT002->AI()->SetData(DATA_TRANSFERED_HEALTH, me->GetMaxHealth() - health);
                }
        }
    };

};

/*-------------------------------------------------------
 *
 *        XS-013 SCRAPBOT
 *
 *///----------------------------------------------------
class mob_scrapbot : public CreatureScript
{
public:
    mob_scrapbot() : CreatureScript("mob_scrapbot") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_scrapbotAI(pCreature);
    }

    struct mob_scrapbotAI : public ScriptedAI
    {
        mob_scrapbotAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = me->GetInstanceScript();
            me->setActive(true);
            me->SetReactState(REACT_AGGRESSIVE);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISTRACT, true);
        }

        InstanceScript* m_pInstance;

        void Reset()
        {
            if(m_pInstance)
            {
                if(Creature* xt002 = Creature::GetCreature(*me, m_pInstance->GetData64(DATA_XT002)))
                {
                    me->AddThreat(xt002, 10000000.0f);
                    me->Attack(xt002, true);
                    AttackStart(xt002);
                }
            }
        }

        void JustDied(Unit* pKiller)
        {
            if (pKiller->GetEntry() == NPC_XE321_BOOMBOT)
                if (m_pInstance)
                    if (Creature* pXT002 = me->GetCreature(*me, m_pInstance->GetData64(DATA_XT002)))
                        pXT002->AI()->DoAction(ACTION_NERF_SCRAPBOTS);

        }

        void MoveInLineOfSight(Unit* who)
        {
            if(who->GetEntry() == NPC_XT002 && who->GetDistance2d(me->GetPositionX(), me->GetPositionY()) < 30.0f)
            {
                me->GetMotionMaster()->MovePoint(1, who->GetPositionX(), who->GetPositionY(), who->GetPositionZ());
                if(me->GetDistance2d(who->GetPositionX(), who->GetPositionY()) < 1.0f )
                {
                    me->MonsterTextEmote(EMOTE_REPAIR, 0, true);
                    who->SetHealth(who->GetHealth() + who->CountPctFromMaxHealth(1));
                    //who->ModifyHealth(int32(who->CountPctFromMaxHealth(1)));
                    who->ToCreature()->AI()->DoAction(ACTION_DISABLE_NERF_ACHI);
                    me->DisappearAndDie();
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
        }
    };

};


/*-------------------------------------------------------
 *
 *        XM-024 PUMMELLER
 *
 *///----------------------------------------------------
class mob_pummeller : public CreatureScript
{
public:
    mob_pummeller() : CreatureScript("mob_pummeller") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_pummellerAI(pCreature);
    }

    struct mob_pummellerAI : public ScriptedAI
    {
        mob_pummellerAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
            me->setActive(true);
        }

        InstanceScript* m_pInstance;
        uint32 uiArcingSmashTimer;
        uint32 uiTrampleTimer;
        uint32 uiUppercutTimer;

        void Reset()
        {
            uiArcingSmashTimer = TIMER_ARCING_SMASH;
            uiTrampleTimer = TIMER_TRAMPLE;
            uiUppercutTimer = TIMER_UPPERCUT;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->IsWithinMeleeRange(me->getVictim()))
            {
                if (uiArcingSmashTimer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_ARCING_SMASH);
                    uiArcingSmashTimer = TIMER_ARCING_SMASH;
                } else uiArcingSmashTimer -= diff;

                if (uiTrampleTimer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_TRAMPLE);
                    uiTrampleTimer = TIMER_TRAMPLE;
                } else uiTrampleTimer -= diff;

                if (uiUppercutTimer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_UPPERCUT);
                    uiUppercutTimer = TIMER_UPPERCUT;
                } else uiUppercutTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };

};


/*-------------------------------------------------------
 *
 *        XE-321 BOOMBOT
 *
 *///----------------------------------------------------
class mob_boombot : public CreatureScript
{
public:
    mob_boombot() : CreatureScript("mob_boombot") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_boombotAI(pCreature);
    }

    struct mob_boombotAI : public ScriptedAI
    {
        mob_boombotAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
            me->setActive(true);
        }

        InstanceScript* m_pInstance;

        void Reset()
        {
        }
        
        void UpdateAI(const uint32 diff)
        {
            if (HealthBelowPct(50))
            {
                //Explosion
                DoCast(me, SPELL_BOOM);
            }

            if (!UpdateVictim())
                return;
        }
    };

};


/*-------------------------------------------------------
 *
 *        VOID ZONE
 *
 *///----------------------------------------------------
class mob_void_zone : public CreatureScript
{
public:
    mob_void_zone() : CreatureScript("mob_void_zone") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_void_zoneAI(pCreature);
    }

    struct mob_void_zoneAI : public ScriptedAI
    {
        mob_void_zoneAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_PACIFIED);
        }

        InstanceScript* m_pInstance;
        uint32 uiVoidZoneTimer;

        void Reset()
        {
            uiVoidZoneTimer = TIMER_VOID_ZONE;
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiVoidZoneTimer <= diff)
            {
                DoCast(SPELL_VOID_ZONE);
                uiVoidZoneTimer = TIMER_VOID_ZONE;
            } else uiVoidZoneTimer -= diff;
        }
    };

};


/*-------------------------------------------------------
 *
 *        LIFE SPARK
 *
 *///----------------------------------------------------
class mob_life_spark : public CreatureScript
{
public:
    mob_life_spark() : CreatureScript("mob_life_spark") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_life_sparkAI(pCreature);
    }

    struct mob_life_sparkAI : public ScriptedAI
    {
        mob_life_sparkAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* m_pInstance;
        uint32 uiShockTimer;

        void Reset()
        {
            DoCast(me, RAID_MODE(SPELL_STATIC_CHARGED_10, SPELL_STATIC_CHARGED_25));
            uiShockTimer = 0; // first one is immediate.
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_pInstance->GetData(TYPE_XT002) != IN_PROGRESS)
                me->DespawnOrUnsummon();

            if (!UpdateVictim())
                return;

            if (uiShockTimer <= diff)
            {
                if (me->IsWithinMeleeRange(me->getVictim()))
                {
                    DoCast(me->getVictim(), SPELL_SHOCK);
                    uiShockTimer = TIMER_SHOCK;
                }
            }
            else uiShockTimer -= diff;
        }
    };

};

void AddSC_boss_xt002()
{
    new boss_xt002;
    new mob_xt002_heart;
    new mob_scrapbot;
    new mob_pummeller;
    new mob_boombot;
    new mob_void_zone;
    new mob_life_spark;
}
