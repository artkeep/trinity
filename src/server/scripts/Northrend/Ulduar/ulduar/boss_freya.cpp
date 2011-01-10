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

enum Yells
{
    SAY_AGGRO                                   = -1603180,
    SAY_AGGRO_WITH_ELDER                        = -1603181,
    SAY_SLAY_1                                  = -1603182,
    SAY_SLAY_2                                  = -1603183,
    SAY_DEATH                                   = -1603184,
    SAY_BERSERK                                 = -1603185,
    SAY_SUMMON_CONSERVATOR                      = -1603186,
    SAY_SUMMON_TRIO                             = -1603187,
    SAY_SUMMON_LASHERS                          = -1603188,
    SAY_YS_HELP                                 = -1603189,

    // Elder pBrightleaf
    SAY_BRIGHTLEAF_AGGRO                        = -1603190,
    SAY_BRIGHTLEAF_SLAY_1                       = -1603191,
    SAY_BRIGHTLEAF_SLAY_2                       = -1603192,
    SAY_BRIGHTLEAF_DEATH                        = -1603193,

    // Elder pIronbranch
    SAY_IRONBRANCH_AGGRO                        = -1603194,
    SAY_IRONBRANCH_SLAY_1                       = -1603195,
    SAY_IRONBRANCH_SLAY_2                       = -1603196,
    SAY_IRONBRANCH_DEATH                        = -1603197,

    // Elder pStonebark
    SAY_STONEBARK_AGGRO                         = -1603198,
    SAY_STONEBARK_SLAY_1                        = -1603199,
    SAY_STONEBARK_SLAY_2                        = -1603200,
    SAY_STONEBARK_DEATH                         = -1603201
};

#define EMOTE_GIFT         "A Lifebinder's Gift begins to grow!"
#define EMOTE_ALLIES       "Allies of Nature have appeared!"

#define ACHIEV_KNOCK_ON_WOOD_1      RAID_MODE(3177, 3185)
#define ACHIEV_KNOCK_ON_WOOD_2      RAID_MODE(3178, 3186)
#define ACHIEV_KNOCK_ON_WOOD_3      RAID_MODE(3179, 3187)
#define ACHIEV_BACK_TO_NATURE       RAID_MODE(2982, 2983)

enum Spells
{
    // pFreya
    SPELL_ATTUNED_TO_NATURE                     = 62519,
    SPELL_TOUCH_OF_EONAR                        = 62528,
    H_SPELL_TOUCH_OF_EONAR                      = 62892,
    SPELL_SUNBEAM                               = 62623,
    H_SPELL_SUNBEAM                             = 62872,
    SPELL_SUN_BEAM_SUMMON                       = 62450,
    SPELL_EONAR_GIFT                            = 62572,
    SPELL_NATURE_BOMB                           = 64604,
    SPELL_NATURE_BOMB_VISUAL                    = 64648,
    SPELL_SUMMON_ALLIES                         = 62678,
    SPELL_BERSERK                               = 47008,
    SPELL_FREYA_GROUND_TREMOR                   = 62437,
    H_SPELL_FREYA_GROUND_TREMOR                 = 62859,
    SPELL_FREYA_IRON_ROOTS                      = 62438,
    H_SPELL_FREYA_IRON_ROOTS                    = 62861,
    SPELL_FREYA_UNSTABLE_ENERGY                 = 62451,
    H_SPELL_FREYA_UNSTABLE_ENERGY               = 62865,
    SPELL_STONEBARKS_ESSENCE                    = 62386,
    SPELL_IRONBRANCHS_ESSENCE                   = 62387,
    SPELL_BRIGHTLEAFS_ESSENCE                   = 62385,
    SPELL_DRAINED_OF_POWER                      = 62467,
    RAID_10_0_SPELL_FREYA_CHEST                 = 62950,
    RAID_10_1_SPELL_FREYA_CHEST                 = 62952,
    RAID_10_2_SPELL_FREYA_CHEST                 = 62953,
    RAID_10_3_SPELL_FREYA_CHEST                 = 62954,
    RAID_25_0_SPELL_FREYA_CHEST                 = 62955,
    RAID_25_1_SPELL_FREYA_CHEST                 = 62956,
    RAID_25_2_SPELL_FREYA_CHEST                 = 62957,
    RAID_25_3_SPELL_FREYA_CHEST                 = 62958,
    
    // Detonating Lasher
    SPELL_DETONATE                              = 62598,
    H_SPELL_DETONATE                            = 62937,
    SPELL_FLAME_LASH                            = 62608,
    
    // Ancient Conservator
    SPELL_CONSERVATORS_GRIP                     = 62532,
    SPELL_NATURES_FURY                          = 62589,
    H_SPELL_NATURES_FURY                        = 63571,
    
    // Ancient Water Spirit
    SPELL_TIDAL_WAVE                            = 62935,
    SPELL_TIDAL_WAVE_EFFECT                     = 62654,
    H_SPELL_TIDAL_WAVE_EFFECT                   = 62936,
    
    // Storm Lasher
    SPELL_LIGHTNING_LASH                        = 62648,
    H_SPELL_LIGHTNING_LASH                      = 62939,
    SPELL_STORMBOLT                             = 62649,
    H_SPELL_STORMBOLT                           = 62938,
    
    // Snaplasher
    SPELL_HARDENED_BARK                         = 62664,
    H_SPELL_HARDENED_BARK                       = 64191,
    
    // Nature Bomb
    SPELL_NATURE_BOMB_EFFECT                    = 64587,
    H_SPELL_NATURE_BOMB_EFFECT                  = 64650,
    
    // Eonars_Gift
    SPELL_LIFEBINDERS_GIFT                      = 62584, 
    H_SPELL_LIFEBINDERS_GIFT                    = 64185,
    SPELL_PHEROMONES                            = 62619,
    SPELL_EONAR_VISUAL                          = 62579,
    
    // Healthy Spore
    SPELL_HEALTHY_SPORE_VISUAL                  = 62538,
    SPELL_HEALTHY_SPORE_NE                      = 62591,
    SPELL_HEALTHY_SPORE_NW                      = 62582,
    SPELL_HEALTHY_SPORE_SE                      = 62592,
    SPELL_HEALTHY_SPORE_SW                      = 62593,
    SPELL_GROW                                  = 62559,
    SPELL_POTENT_PHEROMONES                     = 62541,
    SPELL_POTENT_PHEROMONES_AURA                = 64321,
    
    // Elder pStonebark
    SPELL_PETRIFIED_BARK                        = 62337,
    H_SPELL_PETRIFIED_BARK                      = 62933,
    SPELL_FISTS_OF_STONE                        = 62344,
    SPELL_GROUND_TREMOR                         = 62325,
    H_SPELL_GROUND_TREMOR                       = 62932,
    
    // Elder pIronbranch
    SPELL_IMPALE                                = 62310,
    H_SPELL_IMPALE                              = 62928,
    SPELL_THORN_SWARM                           = 64060,
    H_SPELL_THORN_SWARM                         = 64071,
    SPELL_IRON_ROOTS                            = 62283,
    H_SPELL_IRON_ROOTS                          = 62930,
    
    // Elder pBrightleaf
    SPELL_BRIGHTLEAF_FLUX                       = 62239,
    SPELL_UNSTABLE_ENERGY                       = 62217,
    H_SPELL_UNSTABLE_ENERGY                     = 62922,
    SPELL_SOLAR_FLARE                           = 62240,
    H_SPELL_SOLAR_FLARE                         = 62920,
    SPELL_PHOTOSYNTHESIS                        = 62209,
    SPELL_UNSTABLE_SUN_BEAM                     = 62211,
    SPELL_UNSTABLE_SUN_BEAM_SUMMON              = 62450,
    SPELL_UNSTABLE_SUN_BEAM_VISUAL              = 62216,
    SPELL_BRIGHTLEAF_FLUX_PLUS                  = 62251,
    SPELL_BRIGHTLEAF_FLUX_MINUS                 = 62252,
    SPELL_UNSTABLE_SUNBEAM_EFFECT               = 62243
};

enum FreyaNpcs
{
    NPC_SUN_BEAM                                = 33170,
    NPC_DETONATING_LASHER                       = 32918,
    NPC_ANCIENT_CONSERVATOR                     = 33203,
    NPC_ANCIENT_WATER_SPIRIT                    = 33202,
    NPC_STORM_LASHER                            = 32919,
    NPC_SNAPLASHER                              = 32916,
    NPC_NATURE_BOMB                             = 34129,
    NPC_EONARS_GIFT                             = 33228,
    NPC_HEALTHY_SPORE                           = 33215,
    NPC_UNSTABLE_SUN_BEAM                       = 33050,
    GO_NATURE_BOMB                              = 194902,
};

enum Events
{
    EVENT_NONE,
    EVENT_SUNBEAM,
    EVENT_EONAR_GIFT,
    EVENT_SUMMON_ALLIES,
    EVENT_NATURE_BOMB,
    EVENT_BRIGHTLEAF,
    EVENT_IRONBRANCH,
    EVENT_STONEBARK,
    EVENT_PHASE_2,
    EVENT_BERSERK
};

enum Actions
{
    ACTION_LASHER                               = 0,
    ACTION_ELEMENTAL                            = 1,
    ACTION_ANCIENT                              = 2,
    ACTION_ELEMENTAL_DEAD                       = 3
};

class boss_freya : public CreatureScript
{
public:
    boss_freya() : CreatureScript("boss_freya") { }

    struct boss_freyaAI : public BossAI
    {
        boss_freyaAI(Creature* pCreature) : BossAI(pCreature, TYPE_FREYA)
        {
		    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
        }

		uint8 uiSpawnOrder[3];
        uint8 uiSpawnedAdds;
        uint8 uiEldersCount;
        int32 uiElemTimer;
        uint64 Elemental[3];
        bool bCheckElementalAlive;

        void Reset()
        {
            _Reset();
			me->UpdateMaxHealth();
            if (instance)
            {
                for (uint8 data = DATA_ELDER_BRIGHTLEAF; data <= DATA_ELDER_STONEBARK; ++data)
                {
                    if (Creature *pCreature = Creature::GetCreature((*me), instance->GetData64(data)))
                    {
                        if (pCreature->isAlive())
                            pCreature->AI()->EnterEvadeMode();
                    }
                }
            }
            me->UpdateMaxHealth();
            uiSpawnedAdds = 0;
            uiEldersCount = 0;
            bCheckElementalAlive = true;
            randomizeSpawnOrder();
        }

        void KilledUnit(Unit *pVictim)
        {
            DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2), me);
        }

        void JustSummoned(Creature* pSummon)
        {
            pSummon->SetMaxHealth(pSummon->GetMaxHealth() + (uint32)(pSummon->GetMaxHealth() * uiEldersCount * 20 / 100));
            pSummon->SetHealth(pSummon->GetMaxHealth());
        }

        void JustDied(Unit *pVictim)
        {
            _JustDied();
            DoScriptText(SAY_DEATH, me);
            
            if (instance)
            {
                // Knock on Wood
                if (uiEldersCount == 1)
                    instance->DoCompleteAchievement(ACHIEV_KNOCK_ON_WOOD_1);
                // Knock, Knock on Wood 
                if (uiEldersCount == 2)
                    instance->DoCompleteAchievement(ACHIEV_KNOCK_ON_WOOD_2);
                // Knock, Knock, Knock on Wood
                if (uiEldersCount == 3)
                    instance->DoCompleteAchievement(ACHIEV_KNOCK_ON_WOOD_3);
                // Getting Back to Nature
                if (me->HasAura(SPELL_ATTUNED_TO_NATURE))
                    if (me->GetAura(SPELL_ATTUNED_TO_NATURE, 0)->GetStackAmount() >= 25)
                        instance->DoCompleteAchievement(ACHIEV_BACK_TO_NATURE);
            }
            
            // Hard mode chest
            switch (uiEldersCount)
            {
                case 0:
                    DoCast(me, RAID_MODE(RAID_10_0_SPELL_FREYA_CHEST, RAID_25_0_SPELL_FREYA_CHEST), true);
                    break;
                case 1:
                    DoCast(me, RAID_MODE(RAID_10_1_SPELL_FREYA_CHEST, RAID_25_1_SPELL_FREYA_CHEST), true);
                    break;
                case 2:
                    DoCast(me, RAID_MODE(RAID_10_2_SPELL_FREYA_CHEST, RAID_25_2_SPELL_FREYA_CHEST), true);
                    break;
                case 3:
                    DoCast(me, RAID_MODE(RAID_10_3_SPELL_FREYA_CHEST, RAID_25_3_SPELL_FREYA_CHEST), true);
                    break;
            }
            
        }

        void EnterCombat(Unit* pWho)
        {
            DoCast(me, RAID_MODE(SPELL_TOUCH_OF_EONAR, H_SPELL_TOUCH_OF_EONAR));
            DoCast(me, SPELL_ATTUNED_TO_NATURE, true) ;
            if (Aura* attuned = me->GetAura(SPELL_ATTUNED_TO_NATURE))
                attuned->SetStackAmount(150) ;

            events.ScheduleEvent(EVENT_SUNBEAM, 20000);
            events.ScheduleEvent(EVENT_EONAR_GIFT, 30000);
            events.ScheduleEvent(EVENT_SUMMON_ALLIES, 10000);
            events.ScheduleEvent(EVENT_NATURE_BOMB, 375000);
            events.ScheduleEvent(EVENT_BERSERK, 600000);
            
            // pFreya hard mode can be triggered simply by letting the elders alive
            if (Creature* pBrightleaf = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_ELDER_BRIGHTLEAF) : 0))
                if (pBrightleaf->isAlive())
                {
                    uiEldersCount++;
                    DoCast(SPELL_BRIGHTLEAFS_ESSENCE);
                    me->AddAura(SPELL_BRIGHTLEAFS_ESSENCE, me);
                    events.ScheduleEvent(EVENT_BRIGHTLEAF, urand(15000, 30000));
                }
                
            if (Creature* pIronbranch = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_ELDER_IRONBRANCH) : 0))
                if (pIronbranch->isAlive())
                {
                    uiEldersCount++;
                    DoCast(SPELL_IRONBRANCHS_ESSENCE);
                    me->AddAura(SPELL_IRONBRANCHS_ESSENCE, me);
                    events.ScheduleEvent(EVENT_IRONBRANCH, urand(45000, 60000));
                }
                
            if (Creature* pStonebark = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_ELDER_STONEBARK) : 0))
                if (pStonebark->isAlive())
                {
                    uiEldersCount++;
                    DoCast(SPELL_STONEBARKS_ESSENCE);
                    me->AddAura(SPELL_STONEBARKS_ESSENCE, me);
                    events.ScheduleEvent(EVENT_STONEBARK, urand(35000, 45000));
                }
                
            if (uiEldersCount == 0)
                DoScriptText(SAY_AGGRO, me);
            else
                DoScriptText(SAY_AGGRO_WITH_ELDER, me);
            // each Elder left up will increase the health of both Freya and her adds. (20% per Elder)
            me->SetMaxHealth(me->GetMaxHealth() + (uint32)(me->GetMaxHealth() * uiEldersCount * 20 / 100));
            me->SetHealth(me->GetMaxHealth());
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            // Elementals must be killed within 12 seconds of each other, or they will all revive and heal
            if (bCheckElementalAlive)
                uiElemTimer = 0;
            else
            {
                uiElemTimer += diff;
                if (uiElemTimer >= 12000)
                {
                    for (uint32 i = 0; i < 3; i++)
                    {
                        if (Creature* pElem = Unit::GetCreature(*me, Elemental[i]))
                        {
                            if (pElem->isAlive())
                                pElem->SetHealth(pElem->GetMaxHealth());
                            else
                                pElem->Respawn();
                        }
                    }
                    bCheckElementalAlive = true;
                }
                bool bElemDead = true ;
                for(int i = 0 ; i < 3 ; i++)
                    if (Creature* pElem = Unit::GetCreature(*me, Elemental[i]))
                        if (pElem->isAlive())
                            bElemDead = false;
                if (bElemDead)    
                    DoAction(ACTION_ELEMENTAL);
            }
                               
            if (me->getVictim() && !me->getVictim()->GetCharmerOrOwnerPlayerOrPlayerItself())
                me->Kill(me->getVictim());
                
            events.Update(diff);
                            
            if (events.GetTimer() > 360000)
                events.CancelEvent(EVENT_SUMMON_ALLIES);
                
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_SUNBEAM:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            if (pTarget->isAlive())
                                DoCast(pTarget, RAID_MODE(SPELL_SUNBEAM, H_SPELL_SUNBEAM));
                        events.ScheduleEvent(EVENT_SUNBEAM, urand(10000, 15000));
                        break;
                    case EVENT_EONAR_GIFT:
                        me->MonsterTextEmote(EMOTE_GIFT, 0, true);
                        DoCast(SPELL_EONAR_GIFT);
                        events.ScheduleEvent(EVENT_EONAR_GIFT, urand(35000, 45000));
                        break;
                    case EVENT_SUMMON_ALLIES:
                        me->MonsterTextEmote(EMOTE_ALLIES, 0, true);
                        DoCast(me, SPELL_SUMMON_ALLIES);
                        spawnAdd();
                        events.ScheduleEvent(EVENT_SUMMON_ALLIES, 50000);
                        break;
                    case EVENT_NATURE_BOMB:
                        DoCastAOE(SPELL_NATURE_BOMB_VISUAL);
                        DoCastAOE(SPELL_NATURE_BOMB);
                        events.ScheduleEvent(EVENT_NATURE_BOMB, urand(15000, 20000));
                        break;
                    case EVENT_BRIGHTLEAF:
                        for (int8 n = 0; n < 3; n++)
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true, -SPELL_SUN_BEAM_SUMMON))
                                DoCast(pTarget, SPELL_SUN_BEAM_SUMMON);
                        events.ScheduleEvent(EVENT_BRIGHTLEAF, urand(35000, 45000));
                        break;
                    case EVENT_IRONBRANCH:
                        if (Unit* pRootTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        {
                            if (!pRootTarget->HasAura(RAID_MODE(SPELL_FREYA_IRON_ROOTS, H_SPELL_FREYA_IRON_ROOTS)))
                            {
                                pRootTarget->CastSpell(pRootTarget,RAID_MODE(SPELL_FREYA_IRON_ROOTS, H_SPELL_FREYA_IRON_ROOTS),true);
                                events.ScheduleEvent(EVENT_IRONBRANCH, urand(45000, 60000));
                            }
                            else
                                events.ScheduleEvent(EVENT_IRONBRANCH, 3000);
                        }
                        break;
                    case EVENT_STONEBARK:
                        DoCastAOE(RAID_MODE(SPELL_FREYA_GROUND_TREMOR, H_SPELL_FREYA_GROUND_TREMOR));
                        events.ScheduleEvent(EVENT_STONEBARK, urand(25000, 30000));
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        DoScriptText(SAY_BERSERK, me);
                        events.CancelEvent(EVENT_BERSERK);
                        break;
                }
            }
            DoMeleeAttackIfReady();
		}
        
        void randomizeSpawnOrder()
        {
            //Spawn order algorithm
            uiSpawnOrder[0] = 0; //Detonating Lasher
            uiSpawnOrder[1] = 1; //Elemental Adds 
            uiSpawnOrder[2] = 2; //Ancient Conservator
            
            //Swaps the entire array
            for(uint8 n = 0; n < 3; n++)
            {
                uint8 uiRandom = rand() % 2;
                uint8 uiTemp = uiSpawnOrder[uiRandom];
                uiSpawnOrder[uiRandom] = uiSpawnOrder[n];
                uiSpawnOrder[n] = uiTemp;
            }
        }


        void spawnAdd()
        {
            switch(uiSpawnedAdds)
            {
                case 0: spawnHandler(uiSpawnOrder[0]);break;
                case 1: spawnHandler(uiSpawnOrder[1]);break;
                case 2: spawnHandler(uiSpawnOrder[2]);break;
            }

            uiSpawnedAdds++;
            if (uiSpawnedAdds > 2)
                uiSpawnedAdds = 0;
        }

        void spawnHandler(uint8 add)
        {
            switch(add)
            {
                case 0:
                {
                    DoScriptText(SAY_SUMMON_LASHERS, me);
                    //Spawn 10 Detonating Lashers
                    for(uint8 n = 0; n < 10; n++)
                    {
                        //Make sure that they don't spawn in a pile
                        int8 randomX = -25 + rand() % 50;
                        int8 randomY = -25 + rand() % 50;
                        me->SummonCreature(NPC_DETONATING_LASHER, me->GetPositionX() + randomX, me->GetPositionY() + randomY, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                    }
                    break;
                }
                case 1:
                {
                    DoScriptText(SAY_SUMMON_TRIO, me);
                    //Make sure that they don't spawn in a pile
                    int8 randomX = -25 + rand() % 50;
                    int8 randomY = -25 + rand() % 50;
                    if (Creature* pElem = me->SummonCreature(NPC_SNAPLASHER, me->GetPositionX() + randomX, me->GetPositionY() + randomY, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                         Elemental[0]= pElem->GetGUID() ;
                    randomX = -25 + rand() % 50;
                    randomY = -25 + rand() % 50;
                    if (Creature* pElem = me->SummonCreature(NPC_STORM_LASHER, me->GetPositionX() + randomX, me->GetPositionY() + randomY, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                         Elemental[1]= pElem->GetGUID() ;
                    randomX = -25 + rand() % 50;
                    randomY = -25 + rand() % 50;
                    if (Creature* pElem = me->SummonCreature(NPC_ANCIENT_WATER_SPIRIT, me->GetPositionX() + randomX, me->GetPositionY() + randomY, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                         Elemental[2]= pElem->GetGUID() ;
                    break;
                }
                case 2: 
                {
                    DoScriptText(SAY_SUMMON_CONSERVATOR, me);
                    int8 randomX = -25 + rand() % 50;
                    int8 randomY = -25 + rand() % 50;
                    me->SummonCreature(NPC_ANCIENT_CONSERVATOR, me->GetPositionX() + randomX, me->GetPositionY() + randomY, me->GetPositionZ()+1, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                    break;
                }
            }
        }
        
        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_LASHER:
                    for (uint32 i = 0; i < 2; ++i)
                        me->RemoveAuraFromStack(SPELL_ATTUNED_TO_NATURE, 0, AURA_REMOVE_BY_DEFAULT);
                    break;
                case ACTION_ELEMENTAL:
                    bCheckElementalAlive = true;
                    for (uint32 i = 0; i < 30; ++i)
                        me->RemoveAuraFromStack(SPELL_ATTUNED_TO_NATURE, 0, AURA_REMOVE_BY_DEFAULT);
                    break;
                case ACTION_ANCIENT:
                    for (uint32 i = 0; i < 25; ++i)
                        me->RemoveAuraFromStack(SPELL_ATTUNED_TO_NATURE, 0, AURA_REMOVE_BY_DEFAULT);
                    break;
                case ACTION_ELEMENTAL_DEAD:
                    bCheckElementalAlive = false;
                    break;
            }
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_freyaAI(pCreature);
    }
};

/*===============================================================================================
===============================================================================================*/

class boss_elder_brightleaf : public CreatureScript
{
public:
    boss_elder_brightleaf() : CreatureScript("boss_elder_brightleaf") { }

    struct boss_elder_brightleafAI : public ScriptedAI
    {
        boss_elder_brightleafAI(Creature *c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
        }

        InstanceScript* instance;
        uint32 uiSolarFlareTimer ;
        uint32 uiUnstableSunBeamTimer ;
        uint32 uiUnstableEnergyTimer ;
        uint32 uiPhotosynthesisTimer ;
        uint32 uiFluxTimer ;
        uint32 uiUnstableSunbeamAuraTimer ;

        void Reset()
        {
            uiSolarFlareTimer = 10000 ;
            uiUnstableSunBeamTimer = 5000 ;
            uiUnstableEnergyTimer = 20000;
            uiPhotosynthesisTimer = 1000 ;
            uiUnstableSunbeamAuraTimer = 2000 ;
            uiFluxTimer = 0 ;
        }

        void EnterCombat(Unit* pWho)
        {
            DoCast(me, SPELL_BRIGHTLEAF_FLUX) ;
            DoScriptText(SAY_BRIGHTLEAF_AGGRO, me);        
        }

        void JustDied(Unit* pWho)
        {
            DoScriptText(SAY_BRIGHTLEAF_DEATH, me);
        }

        void KilledUnit(Unit* pWho)
        {
            DoScriptText(RAND(SAY_BRIGHTLEAF_SLAY_1,SAY_BRIGHTLEAF_SLAY_2), me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (uiFluxTimer<=diff)
            {
                me->RemoveAurasDueToSpell(SPELL_BRIGHTLEAF_FLUX_PLUS);
                me->RemoveAurasDueToSpell(SPELL_BRIGHTLEAF_FLUX_MINUS) ;
                switch(rand()%2)
                {
                case 0 : me->AddAura(SPELL_BRIGHTLEAF_FLUX_PLUS, me) ; break ;
                case 1 : me->AddAura(SPELL_BRIGHTLEAF_FLUX_MINUS, me) ; break ;
                }
                uiFluxTimer = 4000 ;
            }
		    else 
			    uiFluxTimer -= diff ;

            if (uiUnstableSunBeamTimer<=diff)
            {
                if (Creature* pSunbeam = me->SummonCreature(NPC_UNSTABLE_SUN_BEAM, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 20000))
                {
                    pSunbeam->CastSpell(pSunbeam, SPELL_UNSTABLE_SUN_BEAM, true) ;
                    pSunbeam->SetReactState(REACT_PASSIVE) ;
                }
                uiUnstableSunBeamTimer = 8000 ;
                uiUnstableSunbeamAuraTimer = 2000 ;
            } 
		    else 
			    uiUnstableSunBeamTimer -= diff ;

            if (uiUnstableEnergyTimer<=diff)
            {
                DoCast(RAID_MODE(SPELL_UNSTABLE_ENERGY, H_SPELL_UNSTABLE_ENERGY)) ;
                me->RemoveAurasDueToSpell(SPELL_UNSTABLE_SUN_BEAM) ;
                me->RemoveAurasDueToSpell(SPELL_UNSTABLE_SUNBEAM_EFFECT) ;
                uiUnstableEnergyTimer = 15000 ;
            } 
		    else 
			    uiUnstableEnergyTimer -= diff ;

            if (uiSolarFlareTimer<=diff)
            {
                if (me->HasAura(SPELL_BRIGHTLEAF_FLUX_PLUS))
                {
                    for(int i = 0 ; i <RAID_MODE(3, 5) ; i++)
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(pTarget,RAID_MODE(SPELL_SOLAR_FLARE, H_SPELL_SOLAR_FLARE));
                }
                else
                {
                    if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(pTarget,RAID_MODE(SPELL_SOLAR_FLARE, H_SPELL_SOLAR_FLARE));
                }
                uiSolarFlareTimer = urand(10000, 15000) ;
            } 
		    else 
			    uiSolarFlareTimer -= diff ;

            if (uiPhotosynthesisTimer<=diff)
            {
                if (me->HasAura(SPELL_UNSTABLE_SUN_BEAM) && !me->HasAura(SPELL_PHOTOSYNTHESIS))
                    DoCast(SPELL_PHOTOSYNTHESIS) ;
                else if (!me->HasAura(SPELL_UNSTABLE_SUN_BEAM) && me->HasAura(SPELL_PHOTOSYNTHESIS))
                    me->RemoveAurasDueToSpell(SPELL_PHOTOSYNTHESIS) ;
                uiPhotosynthesisTimer = 500 ;
            } 
		    else 
			    uiPhotosynthesisTimer -= diff ;

            if (uiUnstableSunbeamAuraTimer <= diff)
            {
                if (me->FindNearestCreature(NPC_UNSTABLE_SUN_BEAM, 3.0f))
                    me->AddAura(SPELL_UNSTABLE_SUNBEAM_EFFECT, me) ;
                else
                {
                    me->RemoveAurasDueToSpell(SPELL_UNSTABLE_SUNBEAM_EFFECT) ;
                    me->RemoveAurasDueToSpell(SPELL_UNSTABLE_SUN_BEAM) ;
                }
                uiUnstableSunbeamAuraTimer = 2000 ;
            } 
		    else 
			    uiUnstableSunbeamAuraTimer -= diff ;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_elder_brightleafAI(pCreature);
    }
};

class npc_sun_beam : public CreatureScript
{
public:
    npc_sun_beam() : CreatureScript("npc_sun_beam") { }

    struct npc_sun_beamAI : public Scripted_NoMovementAI
    {
        npc_sun_beamAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            DoCast(RAID_MODE(SPELL_FREYA_UNSTABLE_ENERGY, H_SPELL_FREYA_UNSTABLE_ENERGY));
            DoCast(SPELL_UNSTABLE_SUN_BEAM_VISUAL);
            me->SetReactState(REACT_PASSIVE);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_sun_beamAI(pCreature);
    }
};

class boss_elder_ironbranch : public CreatureScript
{
public:
    boss_elder_ironbranch() : CreatureScript("boss_elder_ironbranch") { }

    struct boss_elder_ironbranchAI : public ScriptedAI
    {
        boss_elder_ironbranchAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
        uint32 uiImpaleTimer;
        uint32 uiThornSwarmTimer;
        uint32 uiIronRootTimer;

        void Reset()
        {
            uiImpaleTimer = 15000;
            uiThornSwarmTimer = 20000;
            uiIronRootTimer = 8000;
        }

        void EnterCombat(Unit* pWho)
        {
            DoScriptText(SAY_IRONBRANCH_AGGRO, me);
        }
        
        void KilledUnit(Unit *pVictim)
        {
            if (!(rand()%5))
                 DoScriptText(RAND(SAY_IRONBRANCH_SLAY_1,SAY_IRONBRANCH_SLAY_2), me);
        }

        void JustDied(Unit *pVictim)
        {
            DoScriptText(SAY_IRONBRANCH_DEATH, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (uiImpaleTimer <= diff && me->IsWithinMeleeRange(me->getVictim()))
            {
                DoCastVictim(RAID_MODE(SPELL_IMPALE, H_SPELL_IMPALE));
                uiImpaleTimer = urand(15000, 20000);
            } 
		    else 
			    uiImpaleTimer -= diff;

            if (uiThornSwarmTimer <= diff)
            {
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(pTarget, RAID_MODE(SPELL_THORN_SWARM, H_SPELL_THORN_SWARM));
                uiThornSwarmTimer = urand(20000, 24000);
            } 
		    else 
			    uiThornSwarmTimer -= diff;

            if (uiIronRootTimer <= diff)
            {
                Unit* pRootTarget = SelectTarget(SELECT_TARGET_RANDOM);
                if (pRootTarget && !pRootTarget->HasAura(RAID_MODE(SPELL_IMPALE, H_SPELL_IMPALE)) && !pRootTarget->HasAura(RAID_MODE(SPELL_IRON_ROOTS, H_SPELL_IRON_ROOTS)))
                    pRootTarget->CastSpell(me, RAID_MODE(SPELL_IRON_ROOTS, H_SPELL_IRON_ROOTS), false);
                uiIronRootTimer = urand(20000, 25000);
            } 
		    else 
                uiIronRootTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_elder_ironbranchAI(pCreature);
    }
};

class npc_iron_roots : public CreatureScript
{
public:
    npc_iron_roots() : CreatureScript("npc_iron_roots") { }

    struct npc_iron_rootsAI : public Scripted_NoMovementAI
    {
        npc_iron_rootsAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            me->SetInCombatWithZone() ;
            targetGUID = 0 ;
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_NEAREST, 0, 5, true, RAID_MODE(SPELL_IRON_ROOTS, H_SPELL_IRON_ROOTS)))
            {
                targetGUID = pTarget->GetGUID() ;
            }
            if (targetGUID == 0)
                me->DisappearAndDie() ;
        }

        uint64 targetGUID ;

        void JustDied(Unit* victim)
        {
            if (Unit* pTarget = Unit::GetUnit(*me, targetGUID))
                pTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_IRON_ROOTS, H_SPELL_IRON_ROOTS)) ;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_iron_rootsAI(pCreature);
    }
};

class npc_strengthened_iron_root : public CreatureScript
{
public:
    npc_strengthened_iron_root() : CreatureScript("npc_strengthened_iron_root") { }

    struct npc_strengthened_iron_rootAI : public Scripted_NoMovementAI
    {
        npc_strengthened_iron_rootAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            me->SetInCombatWithZone() ;
            targetGUID = 0 ;
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_NEAREST, 0, 5, true, RAID_MODE(SPELL_FREYA_IRON_ROOTS, H_SPELL_FREYA_IRON_ROOTS)))
            {
                targetGUID = pTarget->GetGUID() ;
            }
            if (targetGUID == 0)
                me->DisappearAndDie() ;
        }

        uint64 targetGUID ;

        void JustDied(Unit* victim)
        {
            if (Unit* pTarget = Unit::GetUnit(*me, targetGUID))
                pTarget->RemoveAurasDueToSpell(RAID_MODE(SPELL_FREYA_IRON_ROOTS, H_SPELL_FREYA_IRON_ROOTS)) ;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_strengthened_iron_rootAI(pCreature);
    }
};

class boss_elder_stonebark : public CreatureScript
{
public:
    boss_elder_stonebark() : CreatureScript("boss_elder_stonebark") { }

    struct boss_elder_stonebarkAI : public ScriptedAI
    {
        boss_elder_stonebarkAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        uint32 uiGroundTremorTimer;
        uint32 uiFistsOfStoneTimer;
        uint32 uiPetrifiedBarkTimer;

        void Reset()
        {
            uiGroundTremorTimer = urand(5000, 10000);
            uiFistsOfStoneTimer = 25000;
            uiPetrifiedBarkTimer = 35000;
        }

        void EnterCombat(Unit* pWho)
        {
            DoScriptText(SAY_STONEBARK_AGGRO, me);
        }
        
        void KilledUnit(Unit *pVictim)
        {
            if (!(rand()%5))
                 DoScriptText(RAND(SAY_STONEBARK_SLAY_1,SAY_STONEBARK_SLAY_2), me);
        }

        void JustDied(Unit *pVictim)
        {
            DoScriptText(SAY_STONEBARK_DEATH, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if (uiGroundTremorTimer <= diff)
            {
                DoCastVictim(RAID_MODE(SPELL_GROUND_TREMOR, H_SPELL_GROUND_TREMOR));
                uiGroundTremorTimer = urand(20000, 25000);
            } 
            else 
                uiGroundTremorTimer -= diff;

            if (uiFistsOfStoneTimer <= diff)
            {
                DoCast(me, RAID_MODE(SPELL_PETRIFIED_BARK, H_SPELL_PETRIFIED_BARK));
                uiFistsOfStoneTimer = 50000;
            } 
            else 
                uiFistsOfStoneTimer -= diff;
            
            if (uiPetrifiedBarkTimer <= diff)
            {
                DoCast(me, SPELL_FISTS_OF_STONE);
                uiPetrifiedBarkTimer = 60000;
            } 
            else 
                uiPetrifiedBarkTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_elder_stonebarkAI(pCreature);
    }
};

class npc_eonar_gift : public CreatureScript
{
public:
    npc_eonar_gift() : CreatureScript("npc_eonar_gift") { }

    struct npc_eonar_giftAI : public Scripted_NoMovementAI
    {
        npc_eonar_giftAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            uiLifebindersGiftTimer = 12000;
            uiScale = 0.2;
            me->SetFloatValue(OBJECT_FIELD_SCALE_X, uiScale);
            uiScaleTimer = 200;
            DoCast(me, SPELL_PHEROMONES, true);
            DoCast(me, SPELL_EONAR_VISUAL, true);
        }

        uint32 uiLifebindersGiftTimer;
        float uiScale;
        uint32 uiScaleTimer;

        void UpdateAI(const uint32 diff)
        {
            if (uiLifebindersGiftTimer <= diff)
            {
                DoCast(me, RAID_MODE(SPELL_LIFEBINDERS_GIFT, H_SPELL_LIFEBINDERS_GIFT), true);
                uiLifebindersGiftTimer = 12000;
                me->SetFloatValue(OBJECT_FIELD_SCALE_X, 0);
                me->ForcedDespawn(1000);
            } 
            else 
                uiLifebindersGiftTimer -= diff;

            if (uiScaleTimer <= diff)
            {
                uiScale += 0.025;
                me->SetFloatValue(OBJECT_FIELD_SCALE_X, uiScale);
            } 
            else 
                uiScaleTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_eonar_giftAI(pCreature);
    }
};

class npc_nature_bomb : public CreatureScript
{
public:
    npc_nature_bomb() : CreatureScript("npc_nature_bomb") { }

    struct npc_nature_bombAI : public Scripted_NoMovementAI
    {
        npc_nature_bombAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            uiExplosionTimer = 10000;
            if (GameObject* pGo = me->SummonGameObject(GO_NATURE_BOMB, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, 0, 0, 0, 0, 0))
                natureBombGUID = pGo->GetGUID() ;
        }

        uint32 uiExplosionTimer;
        uint64 natureBombGUID ;

        void UpdateAI(const uint32 diff)
        {
            if (uiExplosionTimer <= diff)
            {
                DoCast(me, RAID_MODE(SPELL_NATURE_BOMB_EFFECT, H_SPELL_NATURE_BOMB_EFFECT));
                uiExplosionTimer = 10000;
                me->ForcedDespawn(1000);
                if (GameObject* pGo = me->GetMap()->GetGameObject(natureBombGUID))
                    pGo->Use(me);
            } 
            else 
                uiExplosionTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_nature_bombAI(pCreature);
    }
};

class npc_detonating_lasher : public CreatureScript
{
public:
    npc_detonating_lasher() : CreatureScript("npc_detonating_lasher") { }

    struct npc_detonating_lasherAI : public ScriptedAI
    {
        npc_detonating_lasherAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
        uint32 uiFlameLashTimer;
        uint32 uiSwitchTargetTimer;

        void JustDied(Unit* victim)
        {
            DoCast(me, RAID_MODE(SPELL_DETONATE, H_SPELL_DETONATE));

            if (Creature* pFreya = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_FREYA) : 0))
                pFreya->AI()->DoAction(ACTION_LASHER);
        }

        void Reset()
        {
            uiFlameLashTimer = urand(5000, 10000);
            uiSwitchTargetTimer = urand(2000, 4000);
        }

        void updateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (uiFlameLashTimer <= diff)
            {
                DoCastVictim(SPELL_FLAME_LASH);
                uiFlameLashTimer = urand(5000, 10000);
            } 
            else 
                uiFlameLashTimer -= diff;

            if (uiSwitchTargetTimer <= diff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    if (pTarget->isAlive())
                        me->Attack(pTarget, true);
                uiSwitchTargetTimer = urand(2000, 4000);
            } 
            else 
                uiSwitchTargetTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_detonating_lasherAI(pCreature);
    }
};

class npc_ancient_conservator : public CreatureScript
{
public:
    npc_ancient_conservator() : CreatureScript("npc_ancient_conservator") { }

    struct npc_ancient_conservatorAI : public ScriptedAI
    {
        npc_ancient_conservatorAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
        uint32 uiNaturesFuryTimer;
        uint32 uiSpawnHealthySporeTimer;
        uint8 uiHealthySporesSpawned;
        uint32 uiSpawnPauseTimer;
        
        void EnterCombat(Unit* pWho)
        {
            DoCast(RAND(SPELL_HEALTHY_SPORE_NE, SPELL_HEALTHY_SPORE_SE));
            DoCast(RAND(SPELL_HEALTHY_SPORE_NW, SPELL_HEALTHY_SPORE_SW));
            uiHealthySporesSpawned += 2;
            DoCast(me, SPELL_CONSERVATORS_GRIP);
        }

        void Reset()
        {
            uiNaturesFuryTimer = 5000;
            uiSpawnHealthySporeTimer = 0;
            uiHealthySporesSpawned = 0;
            uiSpawnPauseTimer = 20000;
        }

        void JustDied(Unit* victim)
        {
            if (Creature* pFreya = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_FREYA) : 0))
                pFreya->AI()->DoAction(ACTION_ANCIENT);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || (me->HasUnitState(UNIT_STAT_CASTING)))
                return;

            if (uiNaturesFuryTimer <= diff)
            {
                //Prevent casting natures fury on a pTarget that is already affected
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true, -RAID_MODE(SPELL_NATURES_FURY, H_SPELL_NATURES_FURY)))
                    DoCast(pTarget, RAID_MODE(SPELL_NATURES_FURY, H_SPELL_NATURES_FURY));
                uiNaturesFuryTimer = 5000;
            } 
            else 
                uiNaturesFuryTimer -= diff;

            if (uiSpawnHealthySporeTimer <= diff && uiHealthySporesSpawned < 10)
            {
                DoCast(RAND(SPELL_HEALTHY_SPORE_NE, SPELL_HEALTHY_SPORE_SE));
                DoCast(RAND(SPELL_HEALTHY_SPORE_NW, SPELL_HEALTHY_SPORE_SW));
                uiHealthySporesSpawned += 2;
                uiSpawnHealthySporeTimer = 2000;
            } 
            else 
                uiSpawnHealthySporeTimer -= diff;

            if (uiSpawnPauseTimer <= diff)
            {
                uiHealthySporesSpawned = 0;
                uiSpawnPauseTimer = 20000;
                uiSpawnHealthySporeTimer = 0;
            } 
            else 
                uiSpawnPauseTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_ancient_conservatorAI(pCreature);
    }
};

class npc_healthy_spore : public CreatureScript
{
public:
    npc_healthy_spore() : CreatureScript("npc_healthy_spore") { }

    struct npc_healthy_sporeAI : public Scripted_NoMovementAI
    {
        npc_healthy_sporeAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            DoCast(me, SPELL_HEALTHY_SPORE_VISUAL);
            DoCast(me, SPELL_POTENT_PHEROMONES);
            DoCast(me, SPELL_GROW);
            me->ForcedDespawn(20000);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_healthy_sporeAI(pCreature);
    }
};

class npc_storm_lasher : public CreatureScript
{
public:
    npc_storm_lasher() : CreatureScript("npc_storm_lasher") { }

    struct npc_storm_lasherAI : public ScriptedAI
    {
        npc_storm_lasherAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
        uint32 uiLightningLashTimer;
        uint32 uiStormboltTimer;

        void Reset()
        {
            uiLightningLashTimer = 4000;
            uiStormboltTimer = 15000;
        }

        void JustDied(Unit* victim)
        {
            if (Creature* pFreya = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_FREYA) : 0))
                pFreya->AI()->DoAction(ACTION_ELEMENTAL_DEAD);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || (me->HasUnitState(UNIT_STAT_CASTING)))
                return;
                
            if (uiLightningLashTimer <= diff)
            {
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(pTarget, RAID_MODE(SPELL_LIGHTNING_LASH, H_SPELL_LIGHTNING_LASH));
                uiLightningLashTimer = urand(3000, 6000);
            } 
            else 
                uiLightningLashTimer -= diff;

            if (uiStormboltTimer <= diff)
            {
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(pTarget, RAID_MODE(SPELL_STORMBOLT, H_SPELL_STORMBOLT));
                uiStormboltTimer = 15000;
            } 
            else 
                uiStormboltTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_storm_lasherAI(pCreature);
    }
};

class npc_snaplasher : public CreatureScript
{
public:
    npc_snaplasher() : CreatureScript("npc_snaplasher") { }

    struct npc_snaplasherAI : public ScriptedAI
    {
        npc_snaplasherAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
            DoCast(me, SPELL_HARDENED_BARK);
        }

        InstanceScript* instance;

        void JustDied(Unit* victim)
        {
            if (Creature* pFreya = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_FREYA) : 0))
                pFreya->AI()->DoAction(ACTION_ELEMENTAL_DEAD);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_snaplasherAI(pCreature);
    }
};

class npc_ancient_water_spirit : public CreatureScript
{
public:
    npc_ancient_water_spirit() : CreatureScript("npc_ancient_water_spirit") { }

    struct npc_ancient_water_spiritAI : public ScriptedAI
    {
        npc_ancient_water_spiritAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
        uint32 uiTidalWaveTimer;

        void Reset()
        {
            uiTidalWaveTimer = 10000;
        }

        void JustDied(Unit* victim)
        {
            if (Creature* pFreya = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_FREYA) : 0))
                pFreya->AI()->DoAction(ACTION_ELEMENTAL_DEAD);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (uiTidalWaveTimer <= diff)
            {
                DoCastVictim(SPELL_TIDAL_WAVE);
                DoCastVictim(RAID_MODE(SPELL_TIDAL_WAVE_EFFECT, H_SPELL_TIDAL_WAVE_EFFECT));
                uiTidalWaveTimer = 15000;
            } 
            else 
                uiTidalWaveTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_ancient_water_spiritAI(pCreature);
    }
};

void AddSC_boss_freya()
{
    new boss_freya;
    new boss_elder_brightleaf;
    new npc_sun_beam;
    new boss_elder_ironbranch;
    new npc_iron_roots;
    new npc_strengthened_iron_root;
    new boss_elder_stonebark;
    new npc_eonar_gift;
    new npc_nature_bomb;
    new npc_detonating_lasher;
    new npc_ancient_conservator;
    new npc_healthy_spore;
    new npc_storm_lasher;
    new npc_snaplasher;
    new npc_ancient_water_spirit;
}
