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

/*
 * Comment: there is missing code on triggers,
 *          brann bronzebeard needs correct gossip info.
 *          requires more work involving area triggers.
 *          if reached brann speaks through his radio..
 */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "ulduar.h"
#include "Vehicle.h"

enum Spells
{
    SPELL_PURSUED                               = 62374,
    SPELL_GATHERING_SPEED                       = 62375,
    SPELL_BATTERING_RAM                         = 62376,
    SPELL_FLAME_VENTS                           = 62396,
    SPELL_MISSILE_BARRAGE                       = 62400,
    SPELL_SYSTEMS_SHUTDOWN                      = 62475,
    SPELL_OVERLOAD_CIRCUIT                      = 62399,
    SPELL_START_THE_ENGINE                      = 62472,
    SPELL_SEARING_FLAME                         = 62402,
    SPELL_BLAZE                                 = 62292,
	SPELL_TAR_PASSIVE                           = 62288,
    SPELL_SMOKE_TRAIL                           = 63575,
    SPELL_ELECTROSHOCK                          = 62522,
    SPELL_NAPALM                                = 63666,
    //TOWER Additional SPELLS
    SPELL_THORIM_S_HAMMER                       = 62911, // Tower of Storms
    SPELL_MIMIRON_S_INFERNO                     = 62909, // Tower of Flames
    SPELL_HODIR_S_FURY                          = 62533, // Tower of Frost
    SPELL_FREYA_S_WARD                          = 62906, // Tower of Nature
    SPELL_FREYA_SUMMONS                         = 62947, // Tower of Nature
    //TOWER ap & health spells
    SPELL_BUFF_TOWER_OF_STORMS                  = 65076,
    SPELL_BUFF_TOWER_OF_FLAMES                  = 65075,
    SPELL_BUFF_TOWER_OF_FR0ST                   = 65077,
    SPELL_BUFF_TOWER_OF_LIFE                    = 64482,
    //Additional Spells
    SPELL_LASH                                  = 65062,
    SPELL_FREYA_S_WARD_EFFECT_1                 = 62947,
    SPELL_FREYA_S_WARD_EFFECT_2                 = 62907,
    SPELL_AUTO_REPAIR                           = 62705,
    AURA_DUMMY_RED                              = 63772,
    AURA_DUMMY_BLUE                             = 63773,
    AURA_DUMMY_GREEN                            = 63771,
    AURA_DUMMY_CYAN                             = 63769,
    SPELL_BEAM_TARGET_STATE                     = 62898,
    SPELL_BEAM_TARGET_STATE_2                   = 62914,
    SPELL_LIQUID_PYRITE                         = 62494,
};

enum Creatures
{
    MOB_LIQUID                                  = 33189,
    MOB_CONTAINER                               = 33218,
    MOB_THORIM_BEACON                           = 33365,
    MOB_MIMIRON_BEACON                          = 33370,
    MOB_HODIR_BEACON                            = 33212,
    MOB_FREYA_BEACON                            = 33367,
    NPC_THORIM_TARGET_BEACON                    = 33364,
    NPC_MIMIRON_TARGET_BEACON                   = 33369,
    NPC_HODIR_TARGET_BEACON                     = 33108,
    NPC_FREYA_TARGET_BEACON                     = 33366,
    NPC_LOREKEEPER                              = 33686, //Hard mode starter
    NPC_BRANZ_BRONZBEARD                        = 33579,
    NPC_DELORAH                                 = 33701,
};

enum Events
{
    EVENT_NONE,
    EVENT_PURSUE,
    EVENT_MISSILE,
    EVENT_VENT,
    EVENT_SPEED,
    EVENT_SUMMON,
    EVENT_SHUTDOWN,
    EVENT_REPAIR,
    EVENT_THORIM_S_HAMMER, // Tower of Storms
    EVENT_MIMIRON_S_INFERNO, // Tower of Flames
    EVENT_HODIR_S_FURY,      // Tower of Frost
    EVENT_FREYA_S_WARD,     // Tower of Nature
};

enum Seats
{
    SEAT_PLAYER                                 = 0,
    SEAT_TURRET                                 = 1,
    SEAT_DEVICE                                 = 2,
    SEAT_CANNON                                 = 7,
};

enum Vehicles
{
    VEHICLE_SIEGE                             = 33060,
    VEHICLE_CHOPPER                           = 33062,
    VEHICLE_DEMOLISHER                        = 33109,
};

#define EMOTE_PURSUE          "Flame Leviathan pursues $N."
#define EMOTE_OVERLOAD        "Flame Leviathan's circuits overloaded."
#define EMOTE_REPAIR          "Automatic repair sequence initiated."

enum Yells
{
    SAY_AGGRO                                   = -1603060,
    SAY_SLAY                                    = -1603061,
    SAY_DEATH                                   = -1603062,
    SAY_TARGET_1                                = -1603063,
    SAY_TARGET_2                                = -1603064,
    SAY_TARGET_3                                = -1603065,
    SAY_HARDMODE                                = -1603066,
    SAY_TOWER_NONE                              = -1603067,
    SAY_TOWER_FROST                             = -1603068,
    SAY_TOWER_FLAME                             = -1603069,
    SAY_TOWER_NATURE                            = -1603070,
    SAY_TOWER_STORM                             = -1603071,
    SAY_PLAYER_RIDING                           = -1603072,
    SAY_OVERLOAD_1                              = -1603073,
    SAY_OVERLOAD_2                              = -1603074,
    SAY_OVERLOAD_3                              = -1603075,
};

enum eAchievementData
{
    ACHIEV_10_NUKED_FROM_ORBIT                  = 2915,
    ACHIEV_25_NUKED_FROM_ORBIT                  = 2917,
    ACHIEV_10_ORBITAL_BOMBARDMENT               = 2913,
    ACHIEV_25_ORBITAL_BOMBARDMENT               = 2918,
    ACHIEV_10_ORBITAL_DEVASTATION               = 2914,
    ACHIEV_25_ORBITAL_DEVASTATION               = 2916,
    ACHIEV_10_ORBIT_UARY                        = 3056,
    ACHIEV_25_ORBIT_UARY                        = 3057,
    ACHIEV_10_SHUTOUT                           = 2911,
    ACHIEV_25_SHUTOUT                           = 2912,
    ACHIEV_10_SIEGE_OF_ULDUAR                   = 2886,
    ACHIEV_25_SIEGE_OF_ULDUAR                   = 2887,
    ACHIEV_10_THREE_CAR_GARAGE                  = 2907, //no core support for using a vehicle
    ACHIEV_25_THREE_CAR_GARAGE                  = 2908, //no core support for using a vehicle
    ACHIEV_10_UNBROKEN                          = 2905,
    ACHIEV_25_UNBROKEN                          = 2906,
};

static Position Center[]=
{
    {354.8771f, -12.90240f, 409.803650f, 0.0f},
};

const Position PosSiege[5] =
{
    {-814.59f,-64.54f,429.92f,5.969f},
    {-784.37f,-33.31f,429.92f,5.096f},
    {-808.99f,-52.10f,429.92f,5.668f},
    {-798.59f,-44.00f,429.92f,5.663f},
    {-812.83f,-77.71f,429.92f,0.046f},
};

const Position PosChopper[5] =
{
    {-717.83f,-106.56f,430.02f,0.122f},
    {-717.83f,-114.23f,430.44f,0.122f},
    {-717.83f,-109.70f,430.22f,0.122f},
    {-718.45f,-118.24f,430.26f,0.052f},
    {-718.45f,-123.58f,430.41f,0.085f},
};

const Position PosDemolisher[5] =
{
    {-724.12f,-176.64f,430.03f,2.543f},
    {-766.70f,-225.03f,430.50f,1.710f},
    {-729.54f,-186.26f,430.12f,1.902f},
    {-756.01f,-219.23f,430.50f,2.369f},
    {-798.01f,-227.24f,429.84f,1.446f},
};

#define ACTION_SHUTDOWN         30

class boss_flame_leviathan : public CreatureScript
{
public:
    boss_flame_leviathan() : CreatureScript("boss_flame_leviathan") {}

    struct boss_flame_leviathanAI : public BossAI
    {
        boss_flame_leviathanAI(Creature* pCreature) : BossAI(pCreature, TYPE_LEVIATHAN), vehicle(pCreature->GetVehicleKit()), lSummons(me)
        {
            assert(vehicle);
            uiActiveTowers = 4;
            uiShutdown = 0;
            ActiveTowers = false;
            towerOfStorms = false;
            towerOfLife = false;
            towerOfFlames = false;
            towerOfFrost = false;

            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); //deathgrip
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
        }

		SummonList lSummons;

        Vehicle* vehicle;
        uint8 uiActiveTowers;
        uint8 uiShutdown;
		uint8 uiDestroyedTurrets;
        bool ActiveTowers;
        bool towerOfStorms;
        bool towerOfLife;
        bool towerOfFlames;
        bool towerOfFrost;

        void Reset()
        {
		    lSummons.DespawnAll();
            _Reset();
			assert(vehicle);
            me->SetReactState(REACT_DEFENSIVE);
            InstallAdds(true);
			uiActiveTowers = 0;
            uiDestroyedTurrets = 0;
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();
			if (instance)
            {
                DoAction(0);
                uint32 uiHardModeValue = instance->GetData(TYPE_LEVIATHAN_HARD_MODE);
                if ((uiHardModeValue & HARD_MODE_THORIM) != HARD_MODE_THORIM)
                    DoAction(1);
                if ((uiHardModeValue & HARD_MODE_MIMIRON) != HARD_MODE_MIMIRON)
                    DoAction(2);
                if ((uiHardModeValue & HARD_MODE_HODIR) != HARD_MODE_HODIR)
                    DoAction(3);
                if ((uiHardModeValue & HARD_MODE_FREYA) != HARD_MODE_FREYA)
                    DoAction(4);
            }
            me->SetReactState(REACT_AGGRESSIVE);
            events.ScheduleEvent(EVENT_PURSUE, 30*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_MISSILE, 1500);
            events.ScheduleEvent(EVENT_VENT, 20*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SPEED, 15*IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SUMMON, 1*IN_MILLISECONDS);
            InstallAdds(false);
			ActiveTower(false); //void ActiveTower
        }

        void ActiveTower(bool bReset = false)
        {
		    // if (me->GetVehicle())
             //   me->GetVehicle()->InstallAllAccessories(me->GetEntry());
            if (!bReset)
            {
                if (ActiveTowers)
                {
				    uint32 uiHardTimer = 35*IN_MILLISECONDS;
                    if (towerOfStorms)
                    {
                        me->AddAura(SPELL_BUFF_TOWER_OF_STORMS, me);
                        events.ScheduleEvent(EVENT_THORIM_S_HAMMER, uiHardTimer);
                        uiHardTimer += 35*IN_MILLISECONDS;
                    }

                    if (towerOfFlames)
                    {
                        me->AddAura(SPELL_BUFF_TOWER_OF_FLAMES, me);
                        events.ScheduleEvent(EVENT_MIMIRON_S_INFERNO, uiHardTimer);
                        uiHardTimer += 35*IN_MILLISECONDS;
                    }

                    if (towerOfFrost)
                    {
                        me->AddAura(SPELL_BUFF_TOWER_OF_FR0ST, me);
                        events.ScheduleEvent(EVENT_HODIR_S_FURY, uiHardTimer);
                        uiHardTimer += 35*IN_MILLISECONDS;
                    }

                    if (towerOfLife)
                    {
                        me->AddAura(SPELL_BUFF_TOWER_OF_LIFE, me);
                        events.ScheduleEvent(EVENT_FREYA_S_WARD, uiHardTimer);
                        uiHardTimer += 35*IN_MILLISECONDS;
                    }

                    if (!towerOfLife && !towerOfFrost && !towerOfFlames && !towerOfStorms)
                        DoScriptText(SAY_TOWER_NONE, me);
                    else
                        DoScriptText(SAY_HARDMODE, me);
                }
                else
                    DoScriptText(SAY_AGGRO, me);
            }
        }

        void InstallAdds(bool bReset = false)
        {
		    if (Vehicle* pVehicle = me->GetVehicle())
            {
                if (instance)
                    if (instance->instance->GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL)
                    {
              //          pVehicle->InstallAccessory(33114, 0);
                //        pVehicle->InstallAccessory(33114, 1);
                    }
               // pVehicle->InstallAccessory(33114, 2);
               // pVehicle->InstallAccessory(33114, 3);
            }
            if (!bReset)
            {
			    uint8 number = 0;
                std::list<Creature*> lSeats;
                me->GetCreatureListWithEntryInGrid(lSeats, 33114,17.0f);
                if (lSeats.empty())
                    return;
                for(std::list<Creature*>::const_iterator itr = lSeats.begin(); itr != lSeats.end(); itr++)
                {
				    number++;
                    Vehicle* pSeat = (*itr)->GetVehicleKit();
                    if (Creature* pTurret = (me->SummonCreature(33142, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN)))
                        pTurret->EnterVehicle(pSeat, SEAT_TURRET);

                    if (Creature* pDevice = (me->SummonCreature(33143, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN)))
                        pDevice->EnterVehicle(pSeat, SEAT_DEVICE);
                }
				if (number >= RAID_MODE(2, 4))
                    return;
            }
            else
            {
                std::list<Creature*> lSeats;
                me->GetCreatureListWithEntryInGrid(lSeats, 33114,17.0f);
                if (lSeats.empty())
                    return;
                for(std::list<Creature*>::const_iterator itr = lSeats.begin(); itr != lSeats.end(); itr++)
                {
                    Vehicle* pSeat = (*itr)->GetVehicleKit();
                    if (Unit* pTurret = (pSeat->GetPassenger(SEAT_TURRET)))
                        pTurret->RemoveFromWorld();

                    if (Unit* pDevice = (pSeat->GetPassenger(SEAT_DEVICE)))
                        pDevice->RemoveFromWorld();
                }
            }
        }

        // TODO: effect 0 and effect 1 may be on different target
        void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
        {
            if (pSpell->Id == SPELL_PURSUED)
                AttackStart(pTarget);
        }

        void JustSummoned(Creature *summon)
        {
            lSummons.Summon(summon);
        }

        void JustDied(Unit* pKiller)
        {
		    lSummons.DespawnAll();
            _JustDied();
            DoScriptText(SAY_DEATH, me);

            if (pKiller->IsVehicle())
                if (Vehicle* pVehicle = pKiller->GetVehicle())
                    if (Unit* pPassenger = pVehicle->GetPassenger(0))
                        if (pPassenger->GetTypeId() == TYPEID_PLAYER)
                            me->SetLootRecipient(pPassenger);

            switch (uiActiveTowers)
            {
                case 4:
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_10_ORBIT_UARY, ACHIEV_25_ORBIT_UARY));
                case 3:
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_10_NUKED_FROM_ORBIT, ACHIEV_25_NUKED_FROM_ORBIT));
                case 2:
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_10_ORBITAL_DEVASTATION, ACHIEV_25_ORBITAL_DEVASTATION));
                case 1:
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_10_ORBITAL_BOMBARDMENT, ACHIEV_25_ORBITAL_BOMBARDMENT));
            }
        }

        void SpellHit(Unit* /*caster*/, const SpellEntry* pSpell)
        {
            //if (pSpell->Id == SPELL_START_THE_ENGINE)
                //vehicle->InstallAllAccessories(me->GetEntry());

            if (pSpell->Id == SPELL_ELECTROSHOCK)
                me->InterruptSpell(CURRENT_CHANNELED_SPELL);

            if (pSpell->Id == SPELL_OVERLOAD_CIRCUIT)
                events.ScheduleEvent(EVENT_SHUTDOWN, 1000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->getThreatManager().isThreatListEmpty())
            {
                EnterEvadeMode();
                return;
            }

            if (me->GetPositionX() < 144.0f || me->GetPositionX() > 440.90f || me->GetPositionZ() < 390.0f)
            {
                EnterEvadeMode();
                return;
            }

            if (me->HasAura(SPELL_SYSTEMS_SHUTDOWN))
            {
                me->SetReactState(REACT_PASSIVE);
                me->AddUnitState(UNIT_STAT_STUNNED | UNIT_STAT_ROOT);
                return;
            }

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if (me->HasAura(SPELL_SYSTEMS_SHUTDOWN))
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                return;
            }
            else
            {
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            }

            events.Update(diff);

            uint32 eventId = events.GetEvent();

            switch(eventId)
            {
            case 0: break; // this is a must
            case EVENT_PURSUE:
                DoScriptText(RAND(SAY_TARGET_1, SAY_TARGET_2, SAY_TARGET_3), me);
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                {
                    if (pTarget->GetTypeId() == TYPEID_PLAYER || pTarget->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE))
                    {
                        me->AddAura(SPELL_PURSUED, pTarget);
                        me->MonsterTextEmote(EMOTE_PURSUE, pTarget->GetGUID(), true);
                    }
                }
                events.RepeatEvent(30*IN_MILLISECONDS);
                break;
            case EVENT_MISSILE:
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(pTarget, SPELL_MISSILE_BARRAGE);
                events.RepeatEvent(1500);
                break;
            case EVENT_VENT:
                DoCastAOE(SPELL_FLAME_VENTS);
                events.RepeatEvent(20*IN_MILLISECONDS);
                break;
            case EVENT_SPEED:
                DoCastAOE(SPELL_GATHERING_SPEED);
                events.RepeatEvent(15*IN_MILLISECONDS);
                break;
            case EVENT_SHUTDOWN:
                DoScriptText(RAND(SAY_OVERLOAD_1, SAY_OVERLOAD_2, SAY_OVERLOAD_3), me);
                InstallAdds(true);
				uiDestroyedTurrets = 0;
                me->MonsterTextEmote(EMOTE_OVERLOAD, 0, true);
                me->AddAura(SPELL_SYSTEMS_SHUTDOWN, me);
                me->RemoveAurasDueToSpell(SPELL_GATHERING_SPEED);
                events.CancelEvent(EVENT_SHUTDOWN);
                break;
            case EVENT_REPAIR:
                me->MonsterTextEmote(EMOTE_REPAIR, 0, true);
                me->ClearUnitState(UNIT_STAT_STUNNED | UNIT_STAT_ROOT);
                InstallAdds(false);
                events.CancelEvent(EVENT_REPAIR);
                break;
            case EVENT_THORIM_S_HAMMER: // Tower of Storms
                for (uint8 i = 0; i < 4; ++i)
                    DoSummon(MOB_THORIM_BEACON, me, urand(20,60), 20000, TEMPSUMMON_TIMED_DESPAWN);
                DoScriptText(SAY_TOWER_STORM, me);
                events.RepeatEvent(20000);
                break;
            case EVENT_MIMIRON_S_INFERNO: // Tower of Flames
                me->SummonCreature(MOB_MIMIRON_BEACON, 390.93f, -13.91f, 409.81f);
                DoScriptText(SAY_TOWER_FLAME, me);
                events.CancelEvent(EVENT_MIMIRON_S_INFERNO);
                break;
            case EVENT_HODIR_S_FURY:      // Tower of Frost
                for (uint8 i = 0; i < 2; ++i)
                    DoSummon(MOB_HODIR_BEACON, me, 50, 0);
                DoScriptText(SAY_TOWER_FROST, me);
                events.CancelEvent(EVENT_HODIR_S_FURY);
                break;
            case EVENT_FREYA_S_WARD:    // Tower of Nature
                DoScriptText(SAY_TOWER_NATURE, me);
                StartFreyaEvent();
                events.CancelEvent(EVENT_FREYA_S_WARD);
                break;
			case EVENT_SUMMON:
                if (summons.size() < 20) // 4seat+1turret+10lift
                    if (Creature* pLift = DoSummonFlyer(MOB_MECHANOLIFT, me, 30.0f, 50.0f, 0))
                        pLift->GetMotionMaster()->MoveRandom(100);
                events.RepeatEvent(2*IN_MILLISECONDS);
                break;
            default:
                events.PopEvent();
                break;
            }
            if (me->IsWithinMeleeRange(me->getVictim()))
                DoSpellAttackIfReady(SPELL_BATTERING_RAM);
        }

        void StartFreyaEvent()//summon these 4 on each corner wich wil spawn additional hostile mobs
        {
            me->SummonCreature(MOB_FREYA_BEACON, 377.02f, -119.10f, 409.81f);
            me->SummonCreature(MOB_FREYA_BEACON, 377.02f, 54.78f, 409.81f);
            me->SummonCreature(MOB_FREYA_BEACON, 185.62f, 54.78f, 409.81f);
            me->SummonCreature(MOB_FREYA_BEACON, 185.62f, -119.10f, 409.81f);
        }

        void DoAction(const int32 uiAction)
        {
            // Start encounter
            if (uiAction == 10)
            {
                me->SetHomePosition(354.8771f, -12.90240f, 409.803f, 0);
                me->GetMotionMaster()->MoveCharge(354.8771f, -12.90240f, 409.803f); //position center
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
                return;
            }

            if (uiAction && uiAction <= 4) // Tower destruction, debuff leviathan loot and reduce active tower
            {
                if (me->HasLootMode(31) && uiActiveTowers == 4)
                {
                    me->RemoveLootMode(LOOT_MODE_HARD_MODE_4);
                    --uiActiveTowers;
                }
                if (me->HasLootMode(15) && uiActiveTowers == 3)
                {
                    me->RemoveLootMode(LOOT_MODE_HARD_MODE_3);
                    --uiActiveTowers;
                }
                if (me->HasLootMode(7) && uiActiveTowers == 2)
                {
                    me->RemoveLootMode(LOOT_MODE_HARD_MODE_2);
                    --uiActiveTowers;
                }
                if (me->HasLootMode(3) && uiActiveTowers == 1)
                {
                    me->RemoveLootMode(LOOT_MODE_HARD_MODE_1);
                    --uiActiveTowers;
                }

				if (uiAction == ACTION_SHUTDOWN)
                {
                    uiDestroyedTurrets++;
                    if (uiDestroyedTurrets >= RAID_MODE(2, 4))
                        DoCast(me, SPELL_OVERLOAD_CIRCUIT, true);
                }
            }

            switch (uiAction)
            {
            case 0:  // Activate hard-mode setting counter to 4 towers, enable all towers apply buffs on levithian
                ActiveTowers = true;
                towerOfStorms = true;
                towerOfLife = true;
                towerOfFlames = true;
                towerOfFrost = true;
                me->SetLootMode(31);
                break;
            case 1:  // Tower of Storms destroyed
                towerOfStorms = false;
                break;
            case 2: // Tower of Flames destroyed
                towerOfFlames = false;
                break;
            case 3: // Tower of Frost destroyed
                towerOfFrost = false;
                break;
            case 4: // Tower of Nature destroyed
                towerOfLife = false;
                break;
            }
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_flame_leviathanAI (pCreature);
    }
};

//#define BOSS_DEBUG

class boss_flame_leviathan_seat : public CreatureScript
{
public:
    boss_flame_leviathan_seat() : CreatureScript("boss_flame_leviathan_seat") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_flame_leviathan_seatAI (pCreature);
    }

    struct boss_flame_leviathan_seatAI : public PassiveAI
    {
        boss_flame_leviathan_seatAI(Creature* pCreature) : PassiveAI(pCreature), vehicle(pCreature->GetVehicleKit())
        {
            assert(vehicle);
    #ifdef BOSS_DEBUG
            me->SetReactState(REACT_AGGRESSIVE);
    #endif
        }

        Vehicle* vehicle;

    #ifdef BOSS_DEBUG
        void MoveInLineOfSight(Unit *who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER && CAST_PLR(who)->isGameMaster()
                && !who->GetVehicle() && vehicle->GetPassenger(SEAT_TURRET))
                who->EnterVehicle(vehicle, SEAT_PLAYER);
        }
    #endif

        void PassengerBoarded(Unit* who, int8 seatId, bool apply)
        {
            if (!me->GetVehicle())
                return;

            if (seatId == SEAT_PLAYER)
            {
                if (!apply)
                    return;
                else
                    DoScriptText(SAY_PLAYER_RIDING,me);

                if (Creature* pTurret = CAST_CRE(vehicle->GetPassenger(SEAT_TURRET)))
                {
                    pTurret->setFaction(me->GetVehicleBase()->getFaction());
                    pTurret->SetUInt32Value(UNIT_FIELD_FLAGS, 0); // unselectable
                    pTurret->AI()->AttackStart(who);
                }
                if (Unit* pDevice = CAST_CRE(vehicle->GetPassenger(SEAT_DEVICE)))
                {
                    pDevice->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    pDevice->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                }
            }
            //else //throw passenger bugged, when fixed uncomment this part.
            //    if (seatId == SEAT_TURRET)
            //    {
            //        if (apply)
            //            return;
            //        if (Unit* device = vehicle->GetPassenger(SEAT_DEVICE))
            //        {
            //            device->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            //            device->SetUInt32Value(UNIT_FIELD_FLAGS, 0); // unselectable
            //        }
            //    }
        }
    };

};

class boss_flame_leviathan_defense_cannon : public CreatureScript
{
public:
    boss_flame_leviathan_defense_cannon() : CreatureScript("boss_flame_leviathan_defense_cannon") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_flame_leviathan_defense_cannonAI (pCreature);
    }

    struct boss_flame_leviathan_defense_cannonAI : public ScriptedAI
    {
        boss_flame_leviathan_defense_cannonAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 NapalmTimer;

        void Reset ()
        {
            NapalmTimer = 5000;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (NapalmTimer <= diff)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                {
                    if(CanAIAttack(pTarget))
                        DoCast(pTarget,SPELL_NAPALM,true);
                }
                NapalmTimer = 5000;
            }
            else
                NapalmTimer-=diff;
        }
    };
};

class boss_flame_leviathan_defense_turret : public CreatureScript
{
public:
    boss_flame_leviathan_defense_turret() : CreatureScript("boss_flame_leviathan_defense_turret") { }

    struct boss_flame_leviathan_defense_turretAI : public TurretAI
    {
        boss_flame_leviathan_defense_turretAI(Creature *c) : TurretAI(c) {}

        void DamageTaken(Unit* who, uint32 &damage)
        {
            if (!CanAIAttack(who))
                damage = 0;
        }

		void JustDied(Unit* pKiller)
        {
            if (Unit* pFlameLeviathan = me->GetVehicleBase())
                if (pFlameLeviathan->GetTypeId() == TYPEID_UNIT)
                    pFlameLeviathan->ToCreature()->AI()->DoAction(ACTION_SHUTDOWN);
        }

        bool CanAIAttack(const Unit *who) const
        {
            if (who->GetTypeId() != TYPEID_PLAYER || !who->GetVehicle() || who->GetVehicleBase()->GetEntry() != 33114)
                return false;
            return true;
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_flame_leviathan_defense_turretAI (pCreature);
    }
};

class boss_flame_leviathan_overload_device : public CreatureScript
{
public:
    boss_flame_leviathan_overload_device() : CreatureScript("boss_flame_leviathan_overload_device") { }

    struct boss_flame_leviathan_overload_deviceAI : public PassiveAI
    {
        boss_flame_leviathan_overload_deviceAI(Creature* pCreature) : PassiveAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript *instance;

        void DoAction(const int32 param)
        {
            if (param == EVENT_SPELLCLICK)
            {
                if (me->GetVehicle())
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    if (Unit* pPlayer = me->GetVehicle()->GetPassenger(SEAT_PLAYER))
                    {
                        pPlayer->ExitVehicle();
                        if (Unit* leviathan = me->GetVehicleBase())
                        {
                            pPlayer->GetMotionMaster()->MoveKnockbackFrom(me->GetVehicleBase()->GetPositionX(), me->GetVehicleBase()->GetPositionY(), 30, 30);
                            leviathan->CastSpell(pPlayer, SPELL_SMOKE_TRAIL, true);
                        }
                    }
                }
            }
        }
    };

	CreatureAI* GetAI_boss_flame_leviathan_overload_device(Creature* pCreature)
    {
        return new boss_flame_leviathan_overload_deviceAI (pCreature);
    }
};

class boss_flame_leviathan_safety_container : public CreatureScript
{
public:
    boss_flame_leviathan_safety_container() : CreatureScript("boss_flame_leviathan_safety_container") { }

    struct boss_flame_leviathan_safety_containerAI : public PassiveAI
    {
        boss_flame_leviathan_safety_containerAI(Creature* pCreature) : PassiveAI(pCreature)
        {
        }

        void JustDied(Unit* /*killer*/)
        {
            float x,y,z;
            me->GetPosition(x,y,z);
            z = me->GetMap()->GetHeight(x, y, z);
            me->GetMotionMaster()->MovePoint(0,x,y,z);
            me->GetMap()->CreatureRelocation(me, x,y,z,0);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new boss_flame_leviathan_safety_containerAI(pCreature);
    }
};

class npc_mechanolift : public CreatureScript
{
public:
    npc_mechanolift() : CreatureScript("npc_mechanolift") { }

    struct npc_mechanoliftAI : public npc_escortAI
    {
        npc_mechanoliftAI(Creature* pCreature) : npc_escortAI(pCreature), vehicle(pCreature->GetVehicleKit())
        {
            assert(vehicle);
			me->SetReactState(REACT_PASSIVE);
        }

        Vehicle* vehicle;

        uint32 MoveTimer;

		void WaypointReached(uint32 uiPointId)
        {
        }

        void SetData(uint32 id, uint32 data)
        {
            switch(data)
            {
            case 0:
                AddWaypoint(1, 194.36f, -40.58f, 474.0f, 0);
                AddWaypoint(2, 28.48f, -40.58f, 474.0f, 0);
                AddWaypoint(3, 17.37f, 291.41f, 490.35f, 0);
                break;
            case 1:
                AddWaypoint(1, 364.40f, -8.60f, 474.0f, 0);
                AddWaypoint(2, 443.16f, -8.60f, 474.0f, 0);
                AddWaypoint(3, 443.16f, -212.47f, 474.0f, 0);
                break;
            case 2:
                AddWaypoint(1, 293.95f, 50.60f, 474.0f, 0);
                AddWaypoint(2, 112.22f, 50.60f, 474.0f, 0);
                AddWaypoint(3, 112.22f, 273.87f, 474.0f, 0);
                AddWaypoint(4, 301.54f, 273.87f, 474.0f, 0);
                AddWaypoint(5, 356.09f, 181.31f, 490.0f, 0);
                break;
            case 3:
                AddWaypoint(1, 312.88f, -178.93f, 478.74f, 0);
                AddWaypoint(2, 311.91f, -179.99f, 478.42f, 0);
                AddWaypoint(3, 361.68f, -20.14f, 474.0f, 0);
                AddWaypoint(4, 437.25f, -18.57f, 474.0f, 0);
                AddWaypoint(5, 443.16f, -212.47f, 474.0f, 0);
                break;
            }
            SetDespawnAtEnd(true);
            Start(false,true);
        }

        void Reset ()
        {
            MoveTimer = 0;
        }

        void JustDied(Unit* pKiller)
        {
            if (Vehicle* vehicle = me->GetVehicleKit())
                if (!vehicle->m_Seats.empty())
                    for (uint8 i=0 ; i< vehicle->m_Seats.size() ; i++)
                        if (Unit* passenger = vehicle->GetPassenger(i))
                            if (passenger->GetTypeId() == TYPEID_UNIT)
                                passenger->ToCreature()->DisappearAndDie();
            if (Creature* pLiquid = DoSummon(MOB_LIQUID, me, 0))
            {
                pLiquid->CastSpell(pLiquid, SPELL_LIQUID_PYRITE, true);
                pLiquid->GetMotionMaster()->MoveFall(410);
            }
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_mechanoliftAI(pCreature);
    }
};

class spell_pool_of_tar : public CreatureScript
{
public:
    spell_pool_of_tar() : CreatureScript("spell_pool_of_tar") { }

    struct spell_pool_of_tarAI : public PassiveAI
    {
        spell_pool_of_tarAI(Creature* pCreature) : PassiveAI(pCreature)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			me->AddAura(SPELL_TAR_PASSIVE, me);
			me->SetReactState(REACT_PASSIVE);
        }

        void DamageTaken(Unit * /*who*/, uint32 &damage)
        {
            damage = 0;
        }

        void SpellHit(Unit* /*caster*/, const SpellEntry* pSpell)
        {
            if (pSpell->SchoolMask & SPELL_SCHOOL_MASK_FIRE && !me->HasAura(SPELL_BLAZE))
                me->CastSpell(me, SPELL_BLAZE, true);
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new spell_pool_of_tarAI (pCreature);
    }
};

class npc_colossus : public CreatureScript
{
public:
    npc_colossus() : CreatureScript("npc_colossus") { }

    struct npc_colossusAI : public ScriptedAI
    {
        npc_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript *instance;

        void JustDied(Unit* /*Who*/)
        {
            if (me->GetHomePosition().IsInDist(Center,50.f))
                instance->SetData(TYPE_COLOSSUS,instance->GetData(TYPE_COLOSSUS)+1);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (!UpdateVictim())
                return;
            DoMeleeAttackIfReady() ;
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_colossusAI(pCreature);
    }
};

class npc_thorims_hammer : public CreatureScript
{
public:
    npc_thorims_hammer() : CreatureScript("npc_thorims_hammer") { }

    struct npc_thorims_hammerAI : public ScriptedAI
    {
        npc_thorims_hammerAI(Creature* pCreature) : ScriptedAI (pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			me->SetReactState(REACT_AGGRESSIVE);
        }

        uint32 uiThorimHammerTimer;
        uint32 uiTargetChangeTimer;

        void Reset ()
        {
		    DoCast(AURA_DUMMY_BLUE);
            uiThorimHammerTimer = urand(4000, 8000);
            uiTargetChangeTimer = 2000;
            DoZoneInCombat();
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiThorimHammerTimer <= diff)
            {
                if (Unit* targetBeacon = me->SummonCreature(NPC_THORIM_TARGET_BEACON, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+70, 0, TEMPSUMMON_TIMED_DESPAWN, 25000))
                    targetBeacon->CastSpell(me, SPELL_THORIM_S_HAMMER, true);
                uiThorimHammerTimer = 999999;
                me->ForcedDespawn(2000);
            } else uiThorimHammerTimer -= diff;

            if (!UpdateVictim())
                return;

			if (uiTargetChangeTimer <= diff)
            {
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    me->GetMotionMaster()->MoveChase(pTarget);
                uiTargetChangeTimer = urand(2000, 4000);
            } else uiTargetChangeTimer -= diff;
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new  npc_thorims_hammerAI(pCreature);
    }
};

class npc_mimirons_inferno : public CreatureScript
{
public:
    npc_mimirons_inferno() : CreatureScript("npc_mimirons_inferno") { }

    struct npc_mimirons_infernoAI : public npc_escortAI
    {
        npc_mimirons_infernoAI(Creature* pCreature) : npc_escortAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);
			Start(false,true,0,NULL,false,true);
        }

        void WaypointReached(uint32 /*i*/)
        {
        }

        void Reset()
        {
		    DoCast(AURA_DUMMY_RED);
            infernoTimer = 2000;
        }

        uint32 infernoTimer;

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if(infernoTimer <= diff)
            {
                if (Unit* targetBeacon = me->SummonCreature(NPC_MIMIRON_TARGET_BEACON, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+70, 0, TEMPSUMMON_TIMED_DESPAWN, 25000))
                    targetBeacon->CastSpell(me, SPELL_MIMIRON_S_INFERNO, true);
                infernoTimer = 3000;
            }
            else
                infernoTimer -= diff;
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_mimirons_infernoAI(pCreature);
    }
};

class npc_hodirs_fury : public CreatureScript
{
public:
    npc_hodirs_fury() : CreatureScript("npc_hodirs_fury") { }

    struct npc_hodirs_furyAI : public ScriptedAI
    {
        npc_hodirs_furyAI(Creature* pCreature) : ScriptedAI (pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
        }

        uint32 uiTargetChangeTimer;
        uint32 uiHodirFuryTimer;

        void Reset()
        {
            DoCast(AURA_DUMMY_CYAN);
            uiTargetChangeTimer = 2000;
            DoZoneInCombat();
        }

        void MoveInLineOfSight(Unit* pWho)
        {
            if (me->GetDistance2d(pWho->GetPositionX(), pWho->GetPositionY()) < 3.0f && pWho->GetTypeId() == TYPEID_PLAYER)
            {
                if (Unit* targetBeacon = me->SummonCreature(NPC_HODIR_TARGET_BEACON, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+70, 0, TEMPSUMMON_TIMED_DESPAWN, 25000))
                    targetBeacon->CastSpell(me, SPELL_HODIR_S_FURY, true);
                uiHodirFuryTimer = urand(7000, 10000);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiHodirFuryTimer > diff)
                uiHodirFuryTimer -= diff;

            if (!UpdateVictim())
                return;

			if (uiTargetChangeTimer <= diff)
            {
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    me->GetMotionMaster()->MoveChase(pTarget);
                uiTargetChangeTimer = urand(4000, 7000);
            } else uiTargetChangeTimer -= diff;
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_hodirs_furyAI(pCreature);
    }
};

class npc_freyas_ward : public CreatureScript
{
public:
    npc_freyas_ward() : CreatureScript("npc_freyas_ward") { }

    struct npc_freyas_wardAI : public ScriptedAI
    {
        npc_freyas_wardAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        uint32 summonTimer ;

        void Reset()
        {
            summonTimer = urand(7000, 12000) ;
            DoCast(AURA_DUMMY_GREEN);
        }

        void UpdateAI(const uint32 diff)
        {
            if(summonTimer <= diff)
            {
                if (Unit* targetBeacon = me->SummonCreature(NPC_FREYA_TARGET_BEACON, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+70, 0, TEMPSUMMON_TIMED_DESPAWN, 25000))
                    targetBeacon->CastSpell(me, SPELL_FREYA_S_WARD, true);
                summonTimer = 20000 ;
            }
            else
                summonTimer -= diff ;

            if (!UpdateVictim())
                return;
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_freyas_wardAI(pCreature);
    }
};

class npc_freya_ward_summon : public CreatureScript
{
public:
    npc_freya_ward_summon() : CreatureScript("npc_freya_ward_summon") { }

    struct npc_freya_ward_summonAI : public ScriptedAI
    {
        npc_freya_ward_summonAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pCreature->GetMotionMaster()->MoveRandom(100);
        }

        uint32 lashTimer ;

        void Reset()
        {
		    DoZoneInCombat();
            lashTimer = 5000 ;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if(lashTimer <= diff)
            {
                DoCast(SPELL_LASH);
                lashTimer = 20000;
            }
            else
                lashTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_freya_ward_summonAI (pCreature);
    }
};

//npc lore keeper
enum Event
{
    NPC_DELLORAH        = 33701,
    NPC_RHYDAN            = 33696,
    NPC_PENTARUS        = 33624,
    NPC_GARDIEN            = 33686,
    NPC_BRANN            = 33579,
};

#define GOSSIP_ITEM_1  "Activate secondary defensive systems"
#define GOSSIP_ITEM_2  "Confirmed"

#define SAY_DELLORAH_1  "J'ai entendu parler d'un gardien du savoir d'Ulduar dont la description vous correspondrait. Est-ce que vous avez un role similaire ?"
#define SAY_KEEPER_1    "J'ai ete construit comme structure de stockage des informations essentielles concernant ce complexe. Mes fonctions principales incluent la notification du statut des systemes de defense frontaux et l'evaluation du statut de l'entite que ce complexe doit emprisonner."
#define SAY_DELLORAH_2  "Systemes de defense frontaux ? Est-ce qu'il y a quelque chose que Brann devrait savoir avant d'envoyer des gens à l'assault du complexe ?"
#define SAY_DELLORAH_3  "Pouvez-vous detailler la nature de ces systemes de defense ?"
#define SAY_KEEPER_2    "L'acces à l'interieur du complexe est actuellement restreint. Les postes defensifs primaires sont actifs. Les postes defensifs secondaires sont actuellement non-actifs."
#define SAY_KEEPER_3    "Compromission du complexe detectee. Protocole de sécurite d'urgence active. Requete acceptee."
#define SAY_DELLORAH_4  "Compris. Au moins, nous n'avons pas besoin de nous occuper des positions orbitales."
#define SAY_DELLORAH_5  "Rhydian, il faut avertir Brann et l'archimage Pentarus de l'existence de ces defenses, sur-le-champ."
#define SAY_KEEPER_4    "Les postes de defense primaires incluent des assemblages de fer et des balises tempetes qui genereront des assemblages supplementaires selon les besoins. Les systemes secondaires incluent l'installation de defense orbitale."
#define SAY_DELLORAH_6  "Et vous avez parle d'une entite prisonniere ? Quels sont la nature et le statut de cette entite ?"
#define SAY_KEEPER_5    "Entite designee : Yogg-Saron. Niveau de securite compromis. Statut operationnel de la prison inconnu. Contact preventif des gardiens impossible."
#define SAY_DELLORAH_7  "Yogg-Saron est ici ? Ne dites plus rien, on a deja les mains pleines."
#define SAY_DELLORAH_8  "Mais... Mais qu'avez-vous fait, $N ? Brann ! BRAAAANNNN !"
#define SAY_DELLORAH_9  "Brann ! $N vient d'activer le systeme de defense orbitale ! Si on ne sort pas vite d'ici, on va se faire carboniser !"

class npc_lorekeeper : public CreatureScript
{
public:
    npc_lorekeeper() : CreatureScript("npc_lorekeeper") { }

    struct npc_lorekeeperAI : public ScriptedAI
    {
        npc_lorekeeperAI(Creature* pCreature) : ScriptedAI(pCreature), summons(me)
        {
            instance = pCreature->GetInstanceScript();
			uiStep = 0;
        }

        InstanceScript* instance;

		SummonList summons;
	    bool bStepping;
        uint32 uiStep;
        uint32 uiPhaseTimer;
        uint32 uiGossipStep;

        uint64 uiDellorahGUID;
        uint64 uiPlayerGUID;

        void DoAction(const int32 uiAction)
        {
            // Start encounter
            if (uiAction == 0)
            {
                for (int32 i = 0; i < RAID_MODE(2, 5); ++i)
                    DoSummon(VEHICLE_SIEGE, PosSiege[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                for (int32 i = 0; i < RAID_MODE(2, 5); ++i)
                    DoSummon(VEHICLE_CHOPPER, PosChopper[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                for (int32 i = 0; i < RAID_MODE(2, 5); ++i)
                    DoSummon(VEHICLE_DEMOLISHER, PosDemolisher[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                return;
            }
        }

		void Reset() 
        {   
            uiPhaseTimer = 1000;  
            uiGossipStep = 0;
            bStepping = false;
        }

        void SetGUID(const uint64 &guid, int32 /*id*/ = 0)
        {
            uiPlayerGUID = guid;
        }

        void JustSummoned(Creature *summon)
        {
            summons.Summon(summon);
        }

        void JumpToNextStep(uint32 uiTimer)
        {
            uiPhaseTimer = uiTimer;
            ++uiStep;
        }
        
        void WaypointReached(uint32 uiPointId)
        {
        }

	    void SetData(uint32 id, uint32 data)
	    {
		    switch(id)
		    {
		    case TYPE_ULDUAR_EVENT:
			    switch(data)
			    {
			    case 3:
				    uiStep = 16;
                    if (Creature * pDellorah = me->FindNearestCreature(NPC_DELLORAH, 20.0f))
                        uiDellorahGUID = pDellorah->GetGUID();
				    bStepping = true;
				    break;
			    case 1:
				    uiStep = 0;
                    if (Creature * pDellorah = me->FindNearestCreature(NPC_DELLORAH, 20.0f))
                        uiDellorahGUID = pDellorah->GetGUID();
				    bStepping = true;
				    break;
			    }
			    break;
		    default:
			    break;
		    }
	    }

	    uint32 GetData(uint32 id)
	    {
		    return uiStep ;
	    }

        void UpdateAI(const uint32 diff)
        {
            if(bStepping)
            {
                if (uiPhaseTimer <= diff)
                {
                    switch(uiStep)
                    {
                        case 0:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                                pDellorah->MonsterSay(SAY_DELLORAH_1, LANG_UNIVERSAL, 0);
                            JumpToNextStep(5000);
                            break;
                        case 1:
                            me->MonsterSay(SAY_KEEPER_1, LANG_UNIVERSAL, 0);
                            JumpToNextStep(8500);
                            break;
                        case 2:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                                pDellorah->MonsterSay(SAY_DELLORAH_2, LANG_UNIVERSAL, 0);
                            JumpToNextStep(4500);
                            break;
                        case 3:
                            me->MonsterSay(SAY_KEEPER_2, LANG_UNIVERSAL, 0);
                            JumpToNextStep(5000);
                            break;
                        case 4:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                                pDellorah->MonsterSay(SAY_DELLORAH_3, LANG_UNIVERSAL, 0);
                            JumpToNextStep(3500);
                            break;
                        case 5:
                            me->MonsterSay(SAY_KEEPER_3, LANG_UNIVERSAL, 0);
                            JumpToNextStep(3500);
                            break;
                        case 6:
                            me->MonsterSay(SAY_KEEPER_4, LANG_UNIVERSAL, 0);
                            JumpToNextStep(8000);
                            break;
                        case 7:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                                pDellorah->MonsterSay(SAY_DELLORAH_4, LANG_UNIVERSAL, 0);
                            JumpToNextStep(4000);
                            break;
                        case 8:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                                pDellorah->MonsterSay(SAY_DELLORAH_5, LANG_UNIVERSAL, 0);
                            JumpToNextStep(4000);
                            break;
                        case 9:
                            if (Creature* pRhydan = me->FindNearestCreature(NPC_RHYDAN, 20.0f))
                                pRhydan->GetMotionMaster()->MovePoint(0, -720.596f,-54.139f,429.84f);
                            JumpToNextStep(4000);
                            break;
                        case 10:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                                pDellorah->MonsterSay(SAY_DELLORAH_6, LANG_UNIVERSAL, 0);
                            JumpToNextStep(4000);
                            break;
                        case 11:
                            me->MonsterSay(SAY_KEEPER_5, LANG_UNIVERSAL, 0);
                            JumpToNextStep(5000);
                            break;
                        case 12:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                            {
                                pDellorah->MonsterSay(SAY_DELLORAH_7, LANG_UNIVERSAL, 0);
                                bStepping = false;
                            }
						    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            break;
                        case 13:
                        case 14:
                        case 15:
                        case 16:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                            {
                                pDellorah->MonsterYell(SAY_DELLORAH_8, LANG_UNIVERSAL, uiPlayerGUID);
                                pDellorah->GetMotionMaster()->MovePoint(0, -720.596f,-54.139f,429.84f);
                            }
                            JumpToNextStep(3000);
                            break;
                        case 17:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                                pDellorah->MonsterYell(SAY_DELLORAH_9, LANG_UNIVERSAL, uiPlayerGUID);
                            JumpToNextStep(4000);
                        case 18:
                            if (Creature* pDellorah = Unit::GetCreature(*me, uiDellorahGUID))
                                pDellorah->ForcedDespawn();
                            if (Creature* pSphere = me->FindNearestCreature(33721, 20.0f))
                            {
                                me->ForcedDespawn();
                                pSphere->ForcedDespawn();
                            }
						    if (Creature* pBran = me->FindNearestCreature(NPC_BRANN, 200.0f))
						    {
							    pBran->AI()->SetData(TYPE_ULDUAR_EVENT, 4);
							    pBran->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
						    }
                            bStepping = false;
                            break;
                    }
                } else uiPhaseTimer -= diff;
            }

		    if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        InstanceScript* instance = pCreature->GetInstanceScript();

        if (pCreature->AI()->GetData(TYPE_ULDUAR_EVENT) == 0 && instance->GetData(TYPE_LEVIATHAN) != DONE)
	    {
		    pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		    pCreature->AI()->SetData(TYPE_ULDUAR_EVENT, 1);
	    }

        if (instance && instance->GetData(TYPE_LEVIATHAN) != DONE && pPlayer && pCreature->AI()->GetData(TYPE_ULDUAR_EVENT) == 12)
        {
            pPlayer->PrepareGossipMenu(pCreature);

            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,GOSSIP_ITEM_1,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        }
        return true;
    }
    //enable hardmode
    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        InstanceScript* instance = pCreature->GetInstanceScript();
        switch(uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            if (pPlayer)
            {
                pPlayer->PrepareGossipMenu(pCreature);
                //instance->instance->LoadGrid(364,-16); //make sure leviathan is loaded


                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,GOSSIP_ITEM_2,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            }
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            if (pPlayer)
                pPlayer->CLOSE_GOSSIP_MENU();

            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pCreature->AI()->SetData(TYPE_ULDUAR_EVENT, 3);
            pCreature->AI()->SetGUID(pPlayer->GetGUID());

            if (Creature* pLeviathan = instance->instance->GetCreature(instance->GetData64(DATA_LEVIATHAN)))
            {
                if (instance)
                    instance->SetData(TYPE_LEVIATHAN_HARD_MODE, HARD_MODE_MIMIRON + HARD_MODE_HODIR + HARD_MODE_THORIM + HARD_MODE_FREYA);
                //pLeviathan->AI()->DoAction(0); //enable hard mode activating the 4 additional events spawning additional vehicles
                //pCreature->SetVisible(false);
                //pCreature->AI()->DoAction(0); // spawn the vehicles
                //pCreature->SetVisible(false);
                //if (Creature* Delorah = pCreature->FindNearestCreature(NPC_DELORAH, 1000, true))
                //    pPlayer->PlayerTalkClass->ClearMenus();
                //{
                //    if (Creature* Branz = pCreature->FindNearestCreature(NPC_BRANZ_BRONZBEARD, 1000, true))
                //    {
                //        Delorah->GetMotionMaster()->MovePoint(0, Branz->GetPositionX()-4, Branz->GetPositionY(), Branz->GetPositionZ());
                        //TODO DoScriptText(xxxx, Delorah, Branz); when reached at branz
                //    }
                //}
            }
            break;
        }
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_lorekeeperAI (pCreature);
    }
};

class go_ulduar_tower : public GameObjectScript
{
public:
    go_ulduar_tower() : GameObjectScript("go_ulduar_tower") { }

    void OnDestroyed(Player* /*pPlayer*/, GameObject* pGO, uint32 /*value*/)
    {
        InstanceScript* instance = pGO->GetInstanceScript();
        if (pGO->GetGOValue()->building.health == 0)
        {
            switch(pGO->GetEntry())
            {
            case GO_TOWER_OF_STORMS:
                instance->ProcessEvent(pGO, EVENT_TOWER_OF_STORMS_DESTROYED);
                break;
            case GO_TOWER_OF_FLAMES:
                instance->ProcessEvent(pGO, EVENT_TOWER_OF_FLAMES_DESTROYED);
                break;
            case GO_TOWER_OF_FROST:
                instance->ProcessEvent(pGO, EVENT_TOWER_OF_FROST_DESTROYED);
                break;
            case GO_TOWER_OF_LIFE:
                instance->ProcessEvent(pGO, EVENT_TOWER_OF_LIFE_DESTROYED);
                break;
            }
        }
    }
};

class at_RX_214_repair_o_matic_station : public AreaTriggerScript
{
public:
    at_RX_214_repair_o_matic_station() : AreaTriggerScript("at_RX_214_repair_o_matic_station") { }

    bool OnTrigger(Player* pPlayer, const AreaTriggerEntry* /*pAt*/)
    {
        InstanceScript* pInstance = pPlayer->GetInstanceScript();
        if (pInstance)
            if (pInstance->GetBossState(TYPE_LEVIATHAN) != IN_PROGRESS)
            {
                if(Creature* vehicle = pPlayer->GetVehicleCreatureBase())
                {
                    if(!vehicle->HasAura(SPELL_AUTO_REPAIR))
                    {
                        pPlayer->MonsterTextEmote(EMOTE_REPAIR, pPlayer->GetGUID(), true);
                        pPlayer->CastSpell(vehicle, SPELL_AUTO_REPAIR, true);
                    }
                }
            }
        return true;
    }
};

#define GOSSIP_ITEM_BRANN_3         "We are ready"
#define SAY_BRANN_1                 "Pentarus, vous l'avez entendue. Que vos mages levent le bouclier pour laisser passer ces braves ames !"
#define SAY_PENTARUS_1              "Bien sur, Brann, nous allons faire tomber ce bouclier pour un moment."
#define SAY_BRANN_2                 "OK ! C'est parti. Tous a vos machines, j'vous causerai d'ici avec la radio !"
#define SAY_PENTARUS_2              "Mages du Kirin Tor, au signal de Brann, levez le bouclier ! Defendez la plate-forme et nos allies de vos vies ! Pour Dalaran !"
#define SAY_BRANN_3                 "Nos alliés sont prets. Levez le bouclier et faites place !"
#define SAY_BRANN_4                 "On a aperçu des nains de fer qui sortaient des fortins à la base des piliers juste devant vous ! Detruisez les fortins, ça les obligera à s'replier !"

class npc_brann_ulduar : public CreatureScript
{
public:
    npc_brann_ulduar() : CreatureScript("npc_brann_ulduar") { }

    struct npc_brann_ulduarAI : public npc_escortAI
    {
        npc_brann_ulduarAI(Creature* pCreature) : npc_escortAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
        
        bool bSteppingBrann;
        uint32 uiStepBrann;
        uint32 uiPhaseTimerBrann;
        
        void Reset() 
        {   
            uiPhaseTimerBrann = 1000;  
            bSteppingBrann = false;
            uiStepBrann = 0;
        }

        void JumpToNextStepBrann(uint32 uiTimerBrann)
        {
            uiPhaseTimerBrann = uiTimerBrann;
            ++uiStepBrann;
        }
        
        void WaypointReached(uint32 uiPointId)
        {
        }

	    void SetData(uint32 id, uint32 data)
	    {
		    switch(id)
		    {
		        case TYPE_ULDUAR_EVENT:
			        switch(data)
			        {
			            case 4:
				        uiPhaseTimerBrann = 0;
				        bSteppingBrann = true;
				        uiStepBrann    = 0;
				        break;
			        }
			        break;
		        default:
                    break;
		    }
	    }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if(bSteppingBrann)
            {
                if (uiPhaseTimerBrann <= diff)
                {
                    switch(uiStepBrann)
                    {
                        case 0:
                            me->MonsterYell(SAY_BRANN_1, LANG_UNIVERSAL, 0);
                            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            JumpToNextStepBrann(3000);
                            break;
                        case 1:
                            if (Unit* pPentarus = me->FindNearestCreature(NPC_PENTARUS, 20.0f))
                                pPentarus->MonsterYell(SAY_PENTARUS_1, LANG_UNIVERSAL, 0);
                            JumpToNextStepBrann(3000);
                            break;
                        case 2:
                            me->MonsterSay(SAY_BRANN_2, LANG_UNIVERSAL, 0);
                            JumpToNextStepBrann(8000);
                            break;
                        case 3:
                            if (Unit* pIngenieur = me->FindNearestCreature(33626, 50.0f))
							    pIngenieur->GetMotionMaster()->MovePoint(0, -777.336f,-45.084f,429.843f);
                            if (Unit* pBataille = me->FindNearestCreature(33662, 50.0f))
                            {
							    pBataille->GetMotionMaster()->MovePoint(0, -686.287f,-67.053f,427.960f);
							    pBataille->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                            }
                            if (Unit* pMage = me->FindNearestCreature(33672, 50.0f))
                            {
							    pMage->GetMotionMaster()->MovePoint(0, -701.350f,-51.397f,429.483f);
							    pMage->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                            }
                            if (Unit* pPentarus = me->FindNearestCreature(NPC_PENTARUS, 20.0f))
                            {
							    pPentarus->GetMotionMaster()->MovePoint(0, -686.287f,-34.389f,427.960f);
							    pPentarus->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                            }
                            me->GetMotionMaster()->MovePoint(0, -673.477f,-52.912f,426.130f);
                            JumpToNextStepBrann(1500);
                            break;
                        case 4:
                            if (Unit* pPentarus = me->FindNearestCreature(NPC_PENTARUS, 50.0f))
                                pPentarus->MonsterYell(SAY_PENTARUS_2, LANG_UNIVERSAL, 0);
                            JumpToNextStepBrann(5000);
                            break;
                        case 5:
                            me->MonsterYell(SAY_BRANN_3, LANG_UNIVERSAL, 0);
                            JumpToNextStepBrann(4000);
                            break;
                        case 6:
                            if (me->FindNearestGameObject(194484, 250.0f))
                            {
                                me->CastSpell(me,69900,true);
                                me->FindNearestGameObject(194484, 250.0f)->RemoveFromWorld();
                            }
                            JumpToNextStepBrann(1500);
                            break;
                        case 7:
                            me->MonsterSay(SAY_BRANN_4, LANG_UNIVERSAL, 0);
                            bSteppingBrann = false;
                            break;
                    }
                } else uiPhaseTimerBrann -= diff;

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        }
	};

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,GOSSIP_ITEM_BRANN_3,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF);
        pPlayer->SEND_GOSSIP_MENU(100003, pCreature->GetGUID());
        
	    return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF:
			    pCreature->AI()->SetData(TYPE_ULDUAR_EVENT, 4);
			    pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
        }
        return true;
    }

	CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_brann_ulduarAI(pCreature);
    }
};

void AddSC_boss_flame_leviathan()
{
    new boss_flame_leviathan;
    new boss_flame_leviathan_seat;
    new boss_flame_leviathan_defense_turret;
    new boss_flame_leviathan_overload_device;
    new boss_flame_leviathan_safety_container;
    new npc_mechanolift;
    new spell_pool_of_tar;
    new npc_colossus;
    new npc_thorims_hammer;
    new npc_mimirons_inferno;
    new npc_hodirs_fury;
    new npc_freyas_ward;
    new npc_freya_ward_summon;
    new npc_lorekeeper;
    new go_ulduar_tower;
    new at_RX_214_repair_o_matic_station;
    new npc_brann_ulduar;
}
