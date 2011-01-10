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

enum Spells
{
    // Hodir
    SPELL_FROZEN_BLOWS                        = 62478,
    H_SPELL_FROZEN_BLOWS                      = 63512,
    SPELL_FLASH_FREEZE                        = 61968,
    SPELL_FLASH_FREEZE_VISUAL                 = 62148,
    SPELL_BITING_COLD                         = 48094, //62038
    SPELL_BITING_COLD_TRIGGERED               = 48095,
    SPELL_FREEZE                              = 62469,
    SPELL_ICICLE                              = 62234,
    SPELL_ICICLE_SNOWDRIFT                    = 62462,
    SPELL_BLOCK_OF_ICE                        = 61969,
    SPELL_BLOCK_OF_ICE_NPC                    = 61990,
    SPELL_FROZEN_KILL                         = 62226,
    SPELL_ICICLE_FALL                         = 69428,
    SPELL_FALL_DAMAGE                         = 62236,
    SPELL_FALL_SNOWDRIFT                      = 62460,
    SPELL_BERSERK                             = 47008,
    // Druids
    SPELL_WRATH                    = 62793,
    SPELL_STARLIGHT                = 62807,
    // Shamans
    SPELL_LAVA_BURST               = 61924,
    SPELL_STORM_CLOUD              = 65123,
    H_SPELL_STORM_CLOUD            = 65133,
    // Mage
    SPELL_FIREBALL                 = 61909,
    SPELL_MELT_ICE                 = 64528,
    SPELL_CONJURE_TOASTY_FIRE      = 62823,
    SPELL_SINGED                   = 62821,
    // Priest
    SPELL_SMITE                    = 61923,
    SPELL_GREAT_HEAL               = 62809,
    SPELL_DISPEL_MAGIC             = 63499
};

// Achievements
#define ACHIEV_CHEESE_THE_FREEZE         RAID_MODE(2961, 2962) // TODO
#define ACHIEV_COLD_IN_HERE              RAID_MODE(2967, 2968)
#define ACHIEV_THIS_CACHE_WAS_RARE       RAID_MODE(3182, 3184)
#define ACHIEV_COOLEST_FRIENDS           RAID_MODE(2963, 2965) // TODO
#define MAX_ENCOUNTER_TIME                    3 * 60 * 1000

#define ACTION_FAILED_COOLEST_FRIENDS           1

enum NPCs
{
    NPC_FLASH_FREEZE_PRE                        = 32926,
    NPC_FLASH_FREEZE                            = 32938,
    NPC_ICICLE_TARGET                           = 33174
};

enum Events
{
    EVENT_NONE,
    EVENT_FREEZE,
    EVENT_FLASH_CAST,
    EVENT_FLASH_EFFECT,
    EVENT_ICICLE,
    EVENT_BLOWS,
    EVENT_BERSERK,
    EVENT_RARE_CACHE,
    EVENT_INTENSE_COLD
};

enum Yells
{
    SAY_AGGRO                                   = -1603210,
    SAY_SLAY_1                                  = -1603211,
    SAY_SLAY_2                                  = -1603212,
    SAY_FLASH_FREEZE                            = -1603213,
    SAY_STALACTITE                              = -1603214,
    SAY_DEATH                                   = -1603215,
    SAY_BERSERK                                 = -1603216,
    SAY_YS_HELP                                 = -1603217,
    SAY_HARD_MODE_MISSED                        = -1603218,
};

#define NORMAL_COUNT 4
#define RAID_COUNT 8

#define EMOTE_FREEZE      "Hodir begins to cast Flash Freeze!"
#define EMOTE_BLOWS       "Hodir gains Frozen Blows!"

struct SummonLocation
{
    float x,y,z,o;
    uint32 entry;
};

SummonLocation addLocations[]=
{
    {1983.75f, -243.36f, 432.767f, 1.57f, 32897}, // Priest 1
    {1999.90f, -230.49f, 432.767f, 1.57f, 33325}, // Druid 1
    {2010.06f, -243.45f, 432.767f, 1.57f, 33328}, // Shaman 1
    {2021.12f, -236.65f, 432.767f, 1.57f, 32893}, // Mage 1
    {2028.10f, -244.66f, 432.767f, 1.57f, 33326}, // Priest 2
    {2014.18f, -232.80f, 432.767f, 1.57f, 32901}, // Druid 2
    {1992.90f, -237.54f, 432.767f, 1.57f, 32900}, // Shaman 2
    {1976.60f, -233.53f, 432.767f, 1.57f, 33327}  // Mage 2
};

class boss_hodir : public CreatureScript
{
public:
    boss_hodir() : CreatureScript("boss_hodir") { }

    struct boss_hodir_AI : public BossAI
    {
        boss_hodir_AI(Creature *pCreature) : BossAI(pCreature, TYPE_HODIR)
        {
        }

		uint32 uiEncounterTime;
        uint32 uiCheckIntenseColdTimer;
        bool bMoreThanTwoIntenseCold;
        bool CheeseTheFreeze;
        bool CoolestFriends;
        bool RareCache;

        void Reset()
        {
            _Reset();
			summons.DespawnAll();
            me->SetReactState(REACT_DEFENSIVE);
            // Spawn NPC Helpers
            for (uint8 i = 0; i < RAID_MODE(NORMAL_COUNT, RAID_COUNT); i++)
            {
                if (Creature* pHelper = me->SummonCreature(addLocations[i].entry,addLocations[i].x,addLocations[i].y,addLocations[i].z,addLocations[i].o))
                    if (Creature *pIceBlock = me->SummonCreature(NPC_FLASH_FREEZE_PRE,addLocations[i].x,addLocations[i].y,addLocations[i].z,addLocations[i].o))
                    {
                        pIceBlock->AddThreat(pHelper, 5000000.0f);
                        pIceBlock->AddAura(SPELL_BLOCK_OF_ICE_NPC, pHelper);
                        pIceBlock->AI()->SetGUID(pHelper->GetGUID());
                        pHelper->AddThreat(me, 5000000.0f);
                    }
            }
        }

        void EnterCombat(Unit* pWho)
        {
            _EnterCombat();
            DoScriptText(SAY_AGGRO, me);
            DoZoneInCombat();
            DoStartMovement(pWho);
            DoCast(me, SPELL_BITING_COLD);
            events.ScheduleEvent(EVENT_ICICLE, 2000);
            events.ScheduleEvent(EVENT_FREEZE, 25000);
            events.ScheduleEvent(EVENT_BLOWS, urand(60000, 65000));
            events.ScheduleEvent(EVENT_FLASH_CAST, 50000);
            events.ScheduleEvent(EVENT_BERSERK, 480000);
            events.ScheduleEvent(EVENT_RARE_CACHE, 180000);
            uiEncounterTime = 0;
            uiCheckIntenseColdTimer = 2000;
            bMoreThanTwoIntenseCold = false;
            CheeseTheFreeze = true;
            CoolestFriends = true;
            RareCache = true;
        }

        void KilledUnit(Unit* victim)
        {
            if (!(rand()%5))
                DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
        }

        void JustDied(Unit *victim)
        {
            _JustDied();
            DoScriptText(SAY_DEATH, me);
            
            // Achievements
            if (instance)
            {
                // Getting Cold in Here
                if (!bMoreThanTwoIntenseCold)
                    instance->DoCompleteAchievement(ACHIEV_COLD_IN_HERE);
                // Cheese the Freeze
                if (CheeseTheFreeze)
                    instance->DoCompleteAchievement(ACHIEV_CHEESE_THE_FREEZE);
                // I Have the Coolest Friends
                if (CoolestFriends)
                    instance->DoCompleteAchievement(ACHIEV_COOLEST_FRIENDS);
                // I Could Say That This Cache Was Rare
                if (RareCache)
                {
                    instance->DoCompleteAchievement(ACHIEV_THIS_CACHE_WAS_RARE);
                    instance->SetData(TYPE_HODIR_RARE_CHEST, GO_STATE_READY);
                }
            }
        }

        bool CheckWipe()
        {
            if (instance)
            {
                if (Map* pMap = instance->instance)
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        Player* pPlayer = itr->getSource();
                        if (pPlayer->isGameMaster())
                            continue;

                        if (pPlayer->isAlive())
                            return false;
                    }
                }
            }

            return true;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (CheckWipe())
                Reset();
                
            uiEncounterTime += diff;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_FREEZE:
                        DoCastAOE(SPELL_FREEZE);
                        events.ScheduleEvent(EVENT_FREEZE, urand(30000, 35000));
                        break;
                    case EVENT_ICICLE:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            if (pTarget->isAlive())
                                DoCast(pTarget, SPELL_ICICLE);
                        events.ScheduleEvent(EVENT_ICICLE, 2000);
                        break;
                    case EVENT_FLASH_CAST:
                        DoScriptText(SAY_FLASH_FREEZE, me);
                        me->MonsterTextEmote(EMOTE_FREEZE, 0, true);
                        for (uint8 i = 0; i < RAID_MODE(2,3); ++i)
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                                if (pTarget->isAlive())
                                    pTarget->CastSpell(pTarget, SPELL_ICICLE_SNOWDRIFT, true);
                        DoCast(SPELL_FLASH_FREEZE);
                        events.DelayEvents(12000);
                        events.ScheduleEvent(EVENT_FLASH_CAST, 50000);
                        events.ScheduleEvent(EVENT_FLASH_EFFECT, 9000);
                        break;
                    case EVENT_FLASH_EFFECT:
                        DoCast(SPELL_FLASH_FREEZE_VISUAL);
                        FlashFreeze();
                        events.CancelEvent(EVENT_FLASH_EFFECT);
                        break;
                    case EVENT_BLOWS:
                        DoScriptText(SAY_STALACTITE, me);
                        me->MonsterTextEmote(EMOTE_BLOWS, 0, true);
                        DoCast(me, RAID_MODE(SPELL_FROZEN_BLOWS, H_SPELL_FROZEN_BLOWS));
                        events.ScheduleEvent(EVENT_BLOWS, urand(50000, 55000));
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        DoScriptText(SAY_BERSERK, me);
                        events.CancelEvent(EVENT_BERSERK);
                        break;
                    case EVENT_INTENSE_COLD:
                        if (!bMoreThanTwoIntenseCold)
                        {
                            std::list<HostileReference*> ThreatList = me->getThreatManager().getThreatList();
                            for (std::list<HostileReference*>::const_iterator itr = ThreatList.begin(); itr != ThreatList.end(); ++itr)
                            {
                                Unit *pTarget = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                                if (!pTarget || pTarget->GetTypeId() != TYPEID_PLAYER)
                                    continue;

                                Aura *AuraIntenseCold = pTarget->GetAura(SPELL_BITING_COLD_TRIGGERED);
                                if (AuraIntenseCold && AuraIntenseCold->GetStackAmount() > 2)
                                {
                                    bMoreThanTwoIntenseCold = true;
                                    break;
                                }
                            }
                            events.ScheduleEvent(EVENT_INTENSE_COLD, 2000);
                        }
                        else
                            events.CancelEvent(EVENT_INTENSE_COLD);
                        break;
                    case EVENT_RARE_CACHE:
                        DoScriptText(SAY_HARD_MODE_MISSED, me);
                        RareCache = false;
                        events.CancelEvent(EVENT_RARE_CACHE);
                        break;
                }
            }

            DoMeleeAttackIfReady();

        }
        
        void FlashFreeze()
        {
            DoZoneInCombat();
            std::list<HostileReference*> ThreatList = me->getThreatManager().getThreatList();
            for (std::list<HostileReference*>::const_iterator itr = ThreatList.begin(); itr != ThreatList.end(); ++itr)
            {
                if (Unit *pTarget = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                {
                    if (pTarget->HasAura(SPELL_BLOCK_OF_ICE))
                    {
                        DoCast(pTarget, SPELL_FROZEN_KILL);
                        continue;
                    }
                    else
                    {
                        if (GetClosestCreatureWithEntry(pTarget, NPC_ICICLE_TARGET, 5.0f))
                            continue;
                            
                        else 
                        {
                            if (pTarget->GetTypeId() == TYPEID_PLAYER)
                            {
                                if (Creature *pIceBlock = me->SummonCreature(NPC_FLASH_FREEZE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 105000))
                                {
                                    pIceBlock->AddAura(SPELL_BLOCK_OF_ICE, pTarget);
                                    pIceBlock->AI()->SetGUID(pTarget->GetGUID());
                                    if (pTarget->GetTypeId() == TYPEID_PLAYER)
                                        CheeseTheFreeze = false;
                                }
                            }
                            if (pTarget->GetTypeId() == TYPEID_UNIT && !pTarget->HasAura(SPELL_BLOCK_OF_ICE_NPC))
                            {
                                if (Creature *pIceBlock = me->SummonCreature(NPC_FLASH_FREEZE_PRE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 105000))
                                {
                                    pIceBlock->AddAura(SPELL_BLOCK_OF_ICE_NPC, pTarget);
                                    pIceBlock->AI()->SetGUID(pTarget->GetGUID());
                                    pTarget->ToCreature()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
                                }
                            }
                        }
                    }
                }
            }
        }
        
        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_FAILED_COOLEST_FRIENDS:
                    CoolestFriends = false;
                    break;
            }
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_hodir_AI (pCreature);
    }
};

class mob_icicle : public CreatureScript
{
public:
    mob_icicle() : CreatureScript("mob_icicle") { }

    struct mob_icicleAI : public ScriptedAI
    {
        mob_icicleAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->SetReactState(REACT_PASSIVE);
        }
        
        uint32 uiIcicleTimer;
        
        void UpdateAI(const uint32 diff)
        {
            if (uiIcicleTimer <= diff)
            {
                DoCast(me, SPELL_FALL_DAMAGE);
                DoCast(me, SPELL_ICICLE_FALL);
                uiIcicleTimer = 10000;
            } 
            else 
                uiIcicleTimer -= diff;
        }
        
        void Reset()
        {
            uiIcicleTimer = 4000;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_icicleAI(pCreature);
    }
};

class mob_icicle_snowdrift : public CreatureScript
{
public:
    mob_icicle_snowdrift() : CreatureScript("mob_icicle_snowdrift") { }

    struct mob_icicle_snowdriftAI : public ScriptedAI
    {
        mob_icicle_snowdriftAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->SetReactState(REACT_PASSIVE);
        }
        
        uint32 uiIcicleTimer;
        
        void UpdateAI(const uint32 diff)
        {
            if (uiIcicleTimer <= diff)
            {
                DoCast(me, SPELL_FALL_SNOWDRIFT);
                DoCast(me, SPELL_ICICLE_FALL);
                uiIcicleTimer = 10000;
            } 
            else 
                uiIcicleTimer -= diff;
        }
        
        void Reset()
        {
            uiIcicleTimer = 2000;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_icicle_snowdriftAI(pCreature);
    }
};

class mob_snowpacked_icicle : public CreatureScript
{
public:
    mob_snowpacked_icicle() : CreatureScript("mob_snowpacked_icicle") { }

    struct mob_snowpacked_icicleAI : public ScriptedAI
    {
        mob_snowpacked_icicleAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->SetReactState(REACT_PASSIVE);
        }
        
        uint32 DespawnTimer;
        
        void UpdateAI(const uint32 diff)
        {
            if (DespawnTimer <= diff)
            {
                if (GameObject *pSnowdrift = me->FindNearestGameObject(194173, 2))
                    me->RemoveGameObject(pSnowdrift, true);
                me->ForcedDespawn();
            }
            else DespawnTimer -= diff;
        }
        
        void Reset()
        {
            DespawnTimer = 12000;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mob_snowpacked_icicleAI(pCreature);
    }
};

class npc_hodir_druid : public CreatureScript
{
public:
    npc_hodir_druid() : CreatureScript("npc_hodir_druid") { }

    struct npc_hodir_druidAI : public ScriptedAI
    {
        npc_hodir_druidAI(Creature *c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
        }

        InstanceScript* instance ;

        uint32 uiStarLightTimer ;

        void Reset()
        {
            if (instance->GetData(TYPE_HODIR) == NOT_STARTED)
            {
                if (Creature* pHodir = Unit::GetCreature(*me, instance->GetData64(DATA_HODIR)))
                    me->AddThreat(pHodir, 100);
                DoCast(me, SPELL_BLOCK_OF_ICE, true) ;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
            }
            uiStarLightTimer = 15000 ;
        }

        void EnterCombat(Unit* pWho)
        {
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_STUNNED))
                return;

            if (uiStarLightTimer <= diff)
            {
                DoCast(SPELL_STARLIGHT) ;
                uiStarLightTimer = urand(20000, 25000) ;
            } 
            else 
                uiStarLightTimer -= diff ;

            DoSpellAttackIfReady(SPELL_WRATH);
        }

        void JustDied(Unit *victim)
        {
            // I Have the Coolest Friends
            if (Creature* pHodir = me->FindNearestCreature(NPC_HODIR,60,true))
                if (pHodir->AI())
                    pHodir->AI()->DoAction(ACTION_FAILED_COOLEST_FRIENDS);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_hodir_druidAI(pCreature);
    }
};

class npc_hodir_shaman : public CreatureScript
{
public:
    npc_hodir_shaman() : CreatureScript("npc_hodir_shaman") { }

    struct npc_hodir_shamanAI : public ScriptedAI
    {
        npc_hodir_shamanAI(Creature *c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
        }

        InstanceScript* instance ;

        uint32 uiStormCloudTimer ;

        void Reset()
        {
            if (instance)
                if (instance->GetData(TYPE_HODIR) == NOT_STARTED)
                {
                    if (Creature* pHodir = Unit::GetCreature(*me, instance->GetData64(DATA_HODIR)))
                        me->AddThreat(pHodir, 100);
                    DoCast(me, SPELL_BLOCK_OF_ICE, true) ;
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
                }
            uiStormCloudTimer = 15000 ;
        }

        void AttackStart(Unit *who)
        {
            AttackStartCaster(who, 20);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_STUNNED))
                return;

            if (uiStormCloudTimer <= diff)
            {
                std::list<Player*> players;
                Trinity::AnyPlayerInObjectRangeCheck checker(me, 30);
                Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
                me->VisitNearbyWorldObject(30, searcher);
                if (!players.empty())
                {
                    std::list<Player*>::iterator iter = players.begin(); 
                    DoCast((*iter), SPELL_STORM_CLOUD, true);
                }
                uiStormCloudTimer = urand(20000, 25000) ;
            } 
            else 
                uiStormCloudTimer -= diff ;

            DoSpellAttackIfReady(SPELL_LAVA_BURST);
        }

        void JustDied(Unit *victim)
        {
            // I Have the Coolest Friends
            if (Creature* pHodir = me->FindNearestCreature(NPC_HODIR,60,true))
                if (pHodir->AI())
                    pHodir->AI()->DoAction(ACTION_FAILED_COOLEST_FRIENDS);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_hodir_shamanAI(pCreature);
    }
};

class npc_hodir_mage : public CreatureScript
{
public:
    npc_hodir_mage() : CreatureScript("npc_hodir_mage") { }

    struct npc_hodir_mageAI : public ScriptedAI
    {
        npc_hodir_mageAI(Creature *c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
        }

        InstanceScript* instance ;

        uint32 uiMeltIceTimer ;
        uint32 uiConjureToastyFireTimer ;

        void Reset()
        {
            if (instance->GetData(TYPE_HODIR) == NOT_STARTED)
            {
                if (Creature* pHodir = Unit::GetCreature(*me, instance->GetData64(DATA_HODIR)))
                    me->AddThreat(pHodir, 100);
                DoCast(me, SPELL_BLOCK_OF_ICE, true) ;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
            }
            uiMeltIceTimer = 15000 ;
            uiConjureToastyFireTimer = 20000 ;
        }

        void AttackStart(Unit *who)
        {
            AttackStartCaster(who, 20);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_STUNNED))
                return;

            if (uiMeltIceTimer <= diff)
            {
                if (Creature *pShard = me->FindNearestCreature(NPC_FLASH_FREEZE,50,true))
                {
                    DoCast(pShard, SPELL_MELT_ICE, true);
                    uiMeltIceTimer = urand(5000,10000);
                }
                uiMeltIceTimer = 5000 ;
            } 
            else 
                uiMeltIceTimer -= diff ;

            if (uiConjureToastyFireTimer <= diff)
            {
                DoCast(SPELL_CONJURE_TOASTY_FIRE) ;
                uiConjureToastyFireTimer = urand(20000, 25000) ;
            } else uiConjureToastyFireTimer -= diff ;

            DoSpellAttackIfReady(SPELL_FIREBALL);
        }

        void JustDied(Unit *victim)
        {
            // I Have the Coolest Friends
            if (Creature* pHodir = me->FindNearestCreature(NPC_HODIR,60,true))
                if (pHodir->AI())
                    pHodir->AI()->DoAction(ACTION_FAILED_COOLEST_FRIENDS);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_hodir_mageAI(pCreature);
    }
};

class npc_hodir_priest : public CreatureScript
{
public:
    npc_hodir_priest() : CreatureScript("npc_hodir_priest") { }

    struct npc_hodir_priestAI : public ScriptedAI
    {
        npc_hodir_priestAI(Creature *c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
        }

        uint32 uiSmiteTimer ;
        uint32 uiGreatHealTimer ;
        uint32 uiDispellMagicTimer ;

        InstanceScript* instance ;

        void Reset()
        {
            if (instance->GetData(TYPE_HODIR) == NOT_STARTED)
            {
                if (Creature* pHodir = Unit::GetCreature(*me, instance->GetData64(DATA_HODIR)))
                    me->AddThreat(pHodir, 100);
                DoCast(me, SPELL_BLOCK_OF_ICE, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
            }
            uiSmiteTimer = 5000 ;
            uiGreatHealTimer = 15000 ;
            uiDispellMagicTimer = 20000 ;
        }

        void AttackStart(Unit *who)
        {
            AttackStartCaster(who, 20);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_STUNNED))
            return;

            if (HealthBelowPct(35))
                DoCastAOE(SPELL_GREAT_HEAL, true);

            if (uiSmiteTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SMITE) ;
                uiSmiteTimer = urand(9000, 12000) ;
            } 
            else 
                uiSmiteTimer -= diff ;

            if (uiGreatHealTimer <= diff)
            {
                DoCastAOE(SPELL_GREAT_HEAL) ;
                uiGreatHealTimer = urand(20000, 25000) ;
            } 
            else 
                uiGreatHealTimer -= diff ;

            if (uiDispellMagicTimer <= diff)
            {
                std::list<Player*> players;
                Trinity::AnyPlayerInObjectRangeCheck checker(me, 30);
                Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
                me->VisitNearbyWorldObject(30, searcher);
                if (!players.empty())
                {
                    for (std::list<Player*>::iterator iter = players.begin(); iter != players.end(); ++iter)
                    {
                        if((*iter)->HasAura(SPELL_FREEZE))
                        {
                            DoCast((*iter),SPELL_DISPEL_MAGIC, true);
                            uiDispellMagicTimer = urand(25000,30000);
                            return;
                        }
                    }
                }
                uiDispellMagicTimer = urand(20000, 25000) ;
            } 
            else 
                uiDispellMagicTimer -= diff ;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit *victim)
        {
            // I Have the Coolest Friends
            if (Creature* pHodir = me->FindNearestCreature(NPC_HODIR,60,true))
                if (pHodir->AI())
                    pHodir->AI()->DoAction(ACTION_FAILED_COOLEST_FRIENDS);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_hodir_priestAI(pCreature);
    }
};

class toasty_fire : public CreatureScript
{
public:
    toasty_fire() : CreatureScript("toasty_fire") { }

    struct toasty_fireAI : public Scripted_NoMovementAI
    {
        toasty_fireAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
        }
        
        void Reset()
        {
            DoCast(me, SPELL_SINGED);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new toasty_fireAI(pCreature);
    }
};

class mob_flash_freeze : public CreatureScript
{
public:
    mob_flash_freeze() : CreatureScript("mob_flash_freeze") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_flash_freezeAI(pCreature);
    }

    struct mob_flash_freezeAI : public ScriptedAI
    {
        mob_flash_freezeAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_STUNNED | UNIT_FLAG_PACIFIED);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true);  // Death Grip
            DoCast(me, SPELL_BLOCK_OF_ICE_NPC, true);
        }
        
        uint32 uiIceBlockTimer;
        uint64 uiTargetGUID;
        
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->getVictim()->GetTypeId() == TYPEID_PLAYER || me->getVictim()->HasAura(SPELL_BLOCK_OF_ICE_NPC))
                return;

            if (uiIceBlockTimer <= diff)
            {
                if (Unit* pTarget = Unit::GetUnit(*me, uiTargetGUID))
                    me->AddAura(SPELL_BLOCK_OF_ICE_NPC, pTarget);
                uiIceBlockTimer = 30000;
            } 
            else uiIceBlockTimer -= diff;
        }

        void SetGUID(const uint64& guid, int32 /*type*/ = 0)
        {
            uiTargetGUID = guid;
        }
        
        void DamageTaken(Unit* pKiller, uint32 &damage)
        {
            if (pKiller && pKiller->GetTypeId() == TYPEID_PLAYER)
                if (Creature* pHodir = me->FindNearestCreature(NPC_HODIR,60,true))
                    if (!pHodir->isInCombat())
                        pHodir->AI()->DoZoneInCombat();
        }

        void JustDied(Unit* pKiller)
        {
            if (Unit* pTarget = Unit::GetUnit(*me, uiTargetGUID))
            {
                if (pTarget->isAlive() && pTarget->HasAura(SPELL_BLOCK_OF_ICE_NPC))
                {
                    pTarget->RemoveAurasDueToSpell(SPELL_BLOCK_OF_ICE_NPC) ;
                    pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
                    if (Creature* pHodir = me->FindNearestCreature(NPC_HODIR,60,true))
                    {
                        pTarget->SetInCombatWith(pHodir);
                        pTarget->Attack(pHodir, true);
                        if (pTarget->GetTypeId() == TYPEID_UNIT)
                            pTarget->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                    }
                }
                if (pTarget->isAlive() && pTarget->HasAura(SPELL_BLOCK_OF_ICE))
                    pTarget->RemoveAurasDueToSpell(SPELL_BLOCK_OF_ICE) ;
            }
        }
        
        void Reset()
        {
            uiIceBlockTimer = 30000;
        }
    };
};

class spell_block_of_ice : public SpellScriptLoader
{
    public:
        spell_block_of_ice() : SpellScriptLoader("spell_block_of_ice") { }

    class spell_block_of_ice_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_block_of_ice_AuraScript)

        void ApplyEffect(AuraEffect const* /*aurEff*/, AuraApplication const* aurApp, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pTarget = aurApp->GetTarget())
            {
                pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS, true);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS_DEST, true);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL, true);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
            }

        }

        void RemoveEffect(AuraEffect const* /*aurEff*/, AuraApplication const* aurApp, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* pTarget = aurApp->GetTarget())
            {
                pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS, false);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL_TOWARDS_DEST, false);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL, false);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, false);
                pTarget->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, false);
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_block_of_ice_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_block_of_ice_AuraScript::RemoveEffect, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_block_of_ice_AuraScript();
    }
};

void AddSC_boss_hodir()
{
    new npc_hodir_shaman;
    new npc_hodir_druid;
    new npc_hodir_mage;
    new npc_hodir_priest;
    new boss_hodir;
    new mob_icicle;
    new mob_icicle_snowdrift;
    new toasty_fire;
    new mob_flash_freeze;
    new mob_snowpacked_icicle;
    new spell_block_of_ice;
}
