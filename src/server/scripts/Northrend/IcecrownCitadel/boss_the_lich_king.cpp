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

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "icecrown_citadel.h"
#include "Group.h"
#include "MapManager.h"

#define GOSSIP_MENU 10600
//#define GOSSIP_MENU "Long have I waited for this day, hero. Are you and your allies prepared to bring the Lich King to justice? We charge on your command!"
#define GOSSIP_START_EVENT "We are prepared, Highlord. Let us battle for the fate of Azeroth! For the light of dawn!"

enum Yells
{
    SAY_INTRO_1_KING         = -1810001,
    SAY_INTRO_2_TIRION       = -1810002,
    SAY_INTRO_3_KING         = -1810003,
    SAY_INTRO_4_TIRION       = -1810004,
    SAY_INTRO_5_KING         = -1810005,
    SAY_AGGRO                = -1810006,
    SAY_REMORSELESSS_WINTER   = -1810007,
    SAY_RANDOM_1             = -1810008,
    SAY_RANDOM_2             = -1810009,
    SAY_KILL_1               = -1810010,
    SAY_KILL_2               = -1810011,
    SAY_BERSERK              = -1810012,
    SAY_ENDING_1_KING        = -1810013,
    SAY_ENDING_2_KING        = -1810014,
    SAY_ENDING_3_KING        = -1810015,
    SAY_ENDING_4_KING        = -1810016,
    SAY_ENDING_5_TIRION      = -1810017,
    SAY_ENDING_6_KING        = -1810018,
    SAY_ENDING_8_TIRION      = -1810020,
    SAY_ENDING_9_FATHER      = -1810021,
    SAY_ENDING_10_TIRION     = -1810022,
    SAY_ENDING_11_FATHER     = -1810023,
    SAY_ENDING_12_KING       = -1810024,
    SAY_DEATH_KING           = -1810025,
    SAY_ESCAPE_FROSTMOURNE   = -1810026,
    SAY_HARVEST_SOUL         = -1810027,
    SAY_DEVOURED_FROSTMOURNE = -1810028,
    SAY_SUMMON_VALKYR        = -1810029,
    SAY_BROKEN_ARENA         = -1810030,
    SAY_10_PROZENT           = -1810031,
    SAY_EMOTE_DEFILE         = -1810032,
};
enum ePhases
{
    PHASE_1 = 1,
    PHASE_2_TRANSITION,
    PHASE_3,
    PHASE_4_TRANSITION,
    PHASE_5,
    PHASE_6_ENDING
};
enum eEvents
{
    EVENT_SPEECH = 1,
    EVENT_BERSERK,
    EVENT_CHECK_ALIVE_PLAYERS,
    EVENT_SUMMON_SHAMBLING_HORROR,
    EVENT_SUMMON_DRUDGE_GHOULS,
    EVENT_INFEST,
    EVENT_NECROTIC_PLAGUE,
    EVENT_SHADOW_TRAP,
    //Transition phase events
    EVENT_SUMMON_RAGING_SPIRIT,
    EVENT_SUMMON_ICE_SPHERE,
    EVENT_TRANSITION_PHASE_END,
    EVENT_PAIN_AND_SUFFERING,
    //Phase three events
    EVENT_SUMMON_VAL_KYR_SHADOWGUARD,
    EVENT_DEFILE,
    EVENT_SOUL_REAPER,
    //Phase five events
    EVENT_SUMMON_VILE_SPIRITS,
    EVENT_HARVEST_SOUL
};
enum Spells
{
    SPELL_SUMMON_SHAMBLING_HORROR    = 70372,
    SPELL_SUMMON_DRUDGE_GHOULS       = 70358,
    SPELL_SUMMON_ICE_SPEHERE         = 69104,
    SPELL_SUMMON_RAGING_SPIRIT       = 69200,
    SPELL_SUMMON_VALKYR              = 74361,
    SPELL_SUMMON_DEFILE              = 72762,
    SPELL_SUMMON_VILE_SPIRIT         = 70498, //????????
    SPELL_SUMMON_BROKEN_FROSTMOURNE  = 72406,
    SPELL_SUMMON_SHADOW_TRAP         = 73539,
    SPELL_INFEST                     = 70541,
    SPELL_NECROTIC_PLAGUE            = 70337, //70337 - initial cast
    SPELL_NECROTIC_PLAGUE_IMMUNITY   = 72846,
    SPELL_NECROTIC_PLAGUE_EFFECT     = 70338,
    SPELL_PLAGUE_SIPHON              = 74074,
    SPELL_REMORSELESS_WINTER          = 68981,
    SPELL_REMORSELESS_WINTER_DAMAGE   = 68983,
    SPELL_PAIN_AND_SUFFERING_10N     = 72133,
    SPELL_PAIN_AND_SUFFERING_10H     = 73789,
    SPELL_PAIN_AND_SUFFERING_25N     = 73788,
    SPELL_PAIN_AND_SUFFERING_25H     = 73790,
    //SPELL_PAIN_AND_SUFFERING         = 74115,
    //SPELL_PAIN_AND_SUFFERING_DAMAGE  = 74117,
    //SPELL_RANDOM_TALK                = 73985,
    SPELL_WINGS_OF_THE_DAMNED        = 74352,
    SPELL_SOUL_REAPER                = 69409,
    SPELL_SOUL_REAPER_HASTE_AURA     = 69410,
    SPELL_HARVEST_SOUL_TELEPORT      = 71372, //72597
    SPELL_HARVEST_SOULS              = 68980,
    SPELL_QUAKE                      = 72262,
    SPELL_CHANNEL_KING               = 71769,
    SPELL_BROKEN_FROSTMOURNE         = 72398,
    SPELL_BOOM_VISUAL                = 72726,
    SPELL_ICEBLOCK_TRIGGER           = 71614,
    SPELL_TIRION_LIGHT               = 71797,
    SPELL_FROSTMOURNE_TRIGGER        = 72405,
    SPELL_DISENGAGE                  = 61508,
    SPELL_FURY_OF_FROSTMOURNE        = 72350,
    SPELL_REVIVE                     = 72429,
    SPELL_REVIVE_EFFECT              = 72423,
    SPELL_CLONE_PLAYER               = 57507,
    SPELL_DEFILE                     = 72743,
    SPELL_DEFILE_DAMAGE              = 72754,
    SPELL_DEFILE_INCREASE            = 72756,
    SPELL_ICE_PULSE                  = 69091,
    SPELL_ICE_BURST                  = 69108,
    SPELL_LIFE_SIPHON                = 73783,
    SPELL_BURST                      = 70503,
    SPELL_VILE_SPIRIT_DISTANCE_CHECK = 70502,
    SPELL_ICE_BURST_DISTANCE_CHECK   = 69109,
    SPELL_VILE_SPIRIT_ACTIVE         = 72130,
    SPELL_RAGING_VISUAL              = 69198,
    SPELL_REMOVE_WEAPON              = 72399,
    SPELL_DROP_FROSTMOURNE           = 73017,
    SPELL_SUMMON_FROSTMOURNE_TRIGGER = 72407,
    SPELL_WMO_INTACT                 = 50176,
    SPELL_WMO_DAMAGE                 = 50177,
    SPELL_PLAY_MOVIE                 = 73159,
    SPELL_SUMMON_MENETHIL            = 72420, //caster - 38584
    SPELL_MENETHIL_VISUAL            = 72372,
    SPELL_VALKYR_CARRY_CAN_CAST      = 74506,
    SPELL_VALKYR_GRAB_PLAYER         = 68985, //74445,
    SPELL_RIDE_VEHICLE               = 46598,
    SPELL_VALKYR_TARGET_SEARCH       = 69030,
    SPELL_VALKYR_CHARGE              = 74399,
    SPELL_VALKYR_EJECT_PASSENGER     = 68576,
    SPELL_LIGHTS_BLESSING            = 71773,
    SPELL_EMOTE_SHOUT                = 73213,
    SPELL_RAGING_GHOUL_VISUAL        = 69636,
    SPELL_RISEN_WITCH_DOCTOR_SPAWN   = 69639,
    SPELL_ICE_SPHERE_VISUAL          = 69090,
    SPELL_TIRION_JUMP                = 71811,
    SPELL_CANT_RESSURECT_AURA        = 72431,
    SPELL_FROSTMOURNE_DESPAWN        = 72726,
    SPELL_SUMMON_FROSTMOURNE         = 74081,
    SPELL_SOUL_EFFECT                = 72305,

    SPELL_VILE_SPIRIT_TARGET_SEARCH  = 70501
};

enum eActions
{
    ACTION_PHASESWITCH_1        = 1, //phase 1 and 3
    ACTION_PHASESWITCH_2,            //phase 2 and 4
    ACTION_START_EVENT,
    ACTION_RESET,
    ACTION_CANCEL_ALL_TRANSITION_EVENTS,
    ACTION_DESPAWN
};
enum eSetGuid
{
    TYPE_VICTIM = 1
};
enum ePoints
{
    POINT_START_EVENT_1              = 3659700,
    POINT_PLATFORM_CENTER            = 3659701,
    POINT_PLATFORM_END               = 3659702,
    POINT_VALKYR_END                 = 3659703,
    POINT_VALKYR_ZET                 = 3659704,
    POINT_VALKYR_CONTINUE_FLYING     = 3659705
};

struct Position StartEvent[]=
{
    {465.0731f, -2123.473f, 1040.8569f},
    {462.8351f, -2123.673f, 1040.9082f},
    {461.5851f, -2123.673f, 1041.4082f},
    {445.5851f, -2123.673f, 1056.1582f},
    {436.0851f, -2123.673f, 1064.6582f}
};

struct Position MovePos[]=
{
    {461.792f, -2125.85f, 1040.860f, 0.0f}, // move
    {503.156f, -2124.51f, 1040.860f, 0.0f}, // move center X: 505.2118 Y: -2124.353 Z: 840.9403
    {490.110f, -2124.98f, 1040.860f, 0.0f}, // move tirion frostmourne
    {467.069f, -2123.58f, 1040.857f, 0.0f}, // move tirion attack
    {498.004f, 2201.57f, 1046.093f, 0.0f},  // move valkyr
    {489.297f, -2124.84f, 1040.857f, 0.0f}, //start event tirion move 1
    {503.682f, -2126.63f, 1040.940f, 0.0f}, //boss escapes after wipe
    {508.989f, -2124.55f, 1045.356f, 0.0f}, //boss levitates above the frostmourne
    {505.212f, -2124.35f, 1040.94f, 3.14159f}
};

/*struct Locations
{
    float x,y,z;
};

static Locations TeleportPoint[]=
{
    {959.996f, 212.576f, 193.843f},
    {932.537f, 231.813f, 193.838f},
    {958.675f, 254.767f, 193.822f},
    {946.955f, 201.316f, 192.535f},
    {944.294f, 149.676f, 197.551f},
    {930.548f, 284.888f, 193.367f},
    {965.997f, 278.398f, 195.777f},
};*/
typedef std::list<Player*> TPlayerList;
TPlayerList GetPlayersInTheMap(Map *pMap)
{
    std::list<Player*> players;
    const Map::PlayerList &PlayerList = pMap->GetPlayers();
    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                players.push_back(player);
    return players;
}
Player *SelectRandomPlayerInTheMap(Map *pMap)
{
    TPlayerList players = GetPlayersInTheMap(pMap);
    if (players.empty())
        return NULL;
    TPlayerList::iterator it = players.begin();
    std::advance(it, urand(0, players.size()-1));
    Player *target = *it;
    return target;
}

class boss_the_lich_king : public CreatureScript
{
    public:
        boss_the_lich_king() : CreatureScript("boss_the_lich_king") { }

        struct boss_the_lich_kingAI : public BossAI
        {
            boss_the_lich_kingAI(Creature* creature) : BossAI(creature, DATA_LICH_KING_EVENT), summons(me)
            {
                instance = me->GetInstanceScript();
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);

                uiPhase = 1;
                uiRandomSpeechTimer = 33000;
                uiBerserkTimer = 900000;
                uiSummonShamblingHorrorTimer = 20000;
                uiSummonDrudgeGhoulsTimer = 30000;
                uiInfestTimer = 30000;
                uiNecroticPlagueTimer = 30000;
                uiIcePulsSummonTimer = 10000;
                uiSummonSpiritTimer = 25000;
                uiSummonValkyrTimer = 5000;
                uiSoulReaperTimer = 30000;
                uiDefileTimer = 25000;
                uiInfestTimer = 40000;
                uiSummonVileSpiritTimer = 30000;
                uiHarvestSoulTimer = 70000;
                uiSummonShadowTrap = 20000;
                uiEndingTimer = 1000;
                uiStage = 1;
                uiTirionGUID = 0;

                if(!instance)
                    return;

                instance->SetData(DATA_LICH_KING_EVENT, NOT_STARTED);
                instance->SetData(DATA_BEEN_WAITING_ACHIEVEMENT, NOT_STARTED);
                instance->SetData(DATA_NECK_DEEP_ACHIEVEMENT, DONE);

                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    me->GetMotionMaster()->MovementExpired();

                if(SpellEntry* spellDefile = GET_SPELL(SPELL_SUMMON_DEFILE))
                    spellDefile->DurationIndex = 3;

                if(SpellEntry* lock = GET_SPELL(SPELL_ICEBLOCK_TRIGGER))
                    lock->Targets = 6; //target chain damage

                if(SpellEntry* reaper = GET_SPELL(SPELL_SOUL_REAPER_HASTE_AURA))
                    reaper->Targets = 1;

                if(SpellEntry* plague = GET_SPELL(SPELL_PLAGUE_SIPHON)) //hack
                    plague->Targets = 18;

                if(SpellEntry* raging = GET_SPELL(SPELL_SUMMON_RAGING_SPIRIT))
                {
                    raging->DurationIndex = 28;
                    raging->Effect[0] = 6;
                }
                if (SpellEntry *furyOfFrostmourne = GET_SPELL(SPELL_FURY_OF_FROSTMOURNE))
                {
                    furyOfFrostmourne->Effect[1] = SPELL_EFFECT_INSTAKILL;
                }
                if (SpellEntry *massResurrection = GET_SPELL(SPELL_REVIVE))
                {
                    massResurrection->EffectRadiusIndex[0] = 4;
                    massResurrection->AttributesEx3 |= SPELL_ATTR3_REQUIRE_DEAD_TARGET;
                }
                //if (SpellEntry *summonVileSpirits = GET_SPELL(SPELL_SUMMON_VILE_SPIRIT))
                //{
                //    summonVileSpirits->EffectApplyAuraName[0] = SPELL_AURA_LINKED;
                //}
                if (SpellEntry *defileDamage = GET_SPELL(SPELL_DEFILE_DAMAGE))
                {
                    defileDamage->EffectImplicitTargetA[0] = TARGET_UNIT_TARGET_ENEMY;
                    defileDamage->EffectImplicitTargetB[1] = TARGET_UNIT_TARGET_ENEMY;
                }
                //if (SpellEntry *painAndSuffering = GET_SPELL(SPELL_PAIN_AND_SUFFERING))
                //{
                //    //Disable casting auxilary spells behind the boss.
                //    painAndSuffering->Effect[0] = 0;
                //    painAndSuffering->Effect[1] = 0;
                //    painAndSuffering->EffectAmplitude[2] = 1500;
                //}
            }

            void EnterEvadeMode()
            {
                events.Reset();
                BossAI::EnterEvadeMode();
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                isSwitching = false;
                events.Reset();
                events.SetPhase(PHASE_1);
                events.ScheduleEvent(EVENT_BERSERK, 900000, PHASE_1);
                events.ScheduleEvent(EVENT_BERSERK, 900000, PHASE_2_TRANSITION);
                events.ScheduleEvent(EVENT_BERSERK, 900000, PHASE_3);
                events.ScheduleEvent(EVENT_BERSERK, 900000, PHASE_4_TRANSITION);
                events.ScheduleEvent(EVENT_BERSERK, 900000, PHASE_5);

                events.ScheduleEvent(EVENT_CHECK_ALIVE_PLAYERS, 5000, PHASE_1);
                events.ScheduleEvent(EVENT_CHECK_ALIVE_PLAYERS, 5000, PHASE_2_TRANSITION);
                events.ScheduleEvent(EVENT_CHECK_ALIVE_PLAYERS, 5000, PHASE_3);
                events.ScheduleEvent(EVENT_CHECK_ALIVE_PLAYERS, 5000, PHASE_4_TRANSITION);
                events.ScheduleEvent(EVENT_CHECK_ALIVE_PLAYERS, 5000, PHASE_5);

                events.ScheduleEvent(EVENT_SUMMON_DRUDGE_GHOULS, 10000, 0, PHASE_1);
                //About 5 seconds after the encounter starts properly Lich King will cast his first Infest
                //http://www.youtube.com/watch?v=hseFPNkaqjE
                events.ScheduleEvent(EVENT_INFEST, 5000, 0, PHASE_1);
                events.ScheduleEvent(EVENT_SUMMON_SHAMBLING_HORROR, 20000, 0, PHASE_1);
                events.ScheduleEvent(EVENT_NECROTIC_PLAGUE, 30000, 0, PHASE_1);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SHADOW_TRAP, 10000, 0, PHASE_1);
                DoScriptText(SAY_AGGRO, me);
                DoCast(me, SPELL_NECROTIC_PLAGUE_IMMUNITY);
                if(instance)
                    uiTirionGUID = instance->GetData64(GUID_TIRION);

                if(instance)
                    instance->SetData(DATA_LICH_KING_EVENT, IN_PROGRESS);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                if(!instance)
                    return;

                DoScriptText(SAY_DEATH_KING, me);
                DoCast(SPELL_PLAY_MOVIE);
                instance->SetData(DATA_LICH_KING_EVENT, DONE);
                if(instance->GetData(DATA_BEEN_WAITING_ACHIEVEMENT) == DONE)
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_BEEN_WAITING_A_LONG_TIME_FOR_THIS_10,ACHIEV_BEEN_WAITING_A_LONG_TIME_FOR_THIS_25));
                if(instance->GetData(DATA_NECK_DEEP_ACHIEVEMENT) == FAIL)
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_NECK_DEEP_IN_VILE_10,ACHIEV_NECK_DEEP_IN_VILE_25));

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFEST);

                summons.DespawnAll();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;
                if (instance->GetData(DATA_LICH_KING_EVENT) == DONE)
                    return;
                switch(id)
                {
                    case POINT_PLATFORM_CENTER:
                    {
                        me->GetMotionMaster()->MovementExpired();
                        uint32 curPhase = GetPhase(events);
                        events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, 5000+2000, 0, curPhase);
                        events.ScheduleEvent(EVENT_SUMMON_ICE_SPHERE, 5000+7000, 0, curPhase);
                        events.ScheduleEvent(EVENT_SUMMON_RAGING_SPIRIT, 5000+6000, 0, curPhase);
                        events.ScheduleEvent(EVENT_TRANSITION_PHASE_END, 5000+60000, 0, curPhase);
                        DoCast(SPELL_WMO_INTACT);
                        DoScriptText(SAY_REMORSELESSS_WINTER, me);
                        DoCast(me, SPELL_REMORSELESS_WINTER);
                        break;
                    }
                }
            }

            void JustReachedHome()
            {
                if(!instance)
                    return;
                instance->SetData(DATA_LICH_KING_EVENT, FAIL);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFEST);
                instance->DoCastSpellOnPlayers(SPELL_TELEPORT_ICC_FROZEN_THRONE);

                if (Creature *tirion = ObjectAccessor::GetCreature(*me, instance->GetData64(GUID_TIRION)))
                    tirion->AI()->DoAction(ACTION_RESET);
                events.Reset();
                summons.DespawnAll();
            }

            void KilledUnit(Unit* victim)
            {
                if (uiPhase != 6 && victim->GetTypeId() == TYPEID_PLAYER)
                    DoScriptText(RAND(SAY_KILL_1, SAY_KILL_2), me);
            }

            void JustSummoned(Creature* summoned)
            {
                summons.Summon(summoned);
                switch(summoned->GetEntry())
                {
                    case NPC_ICE_SPHERE:
                        summoned->ApplySpellImmune(SPELL_REMORSELESS_WINTER_DAMAGE, IMMUNITY_ID, SPELL_REMORSELESS_WINTER_DAMAGE, true);
                        summoned->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                        summoned->CastSpell(summoned, SPELL_ICE_BURST_DISTANCE_CHECK, true);
                        summoned->CastSpell(summoned, SPELL_ICE_SPHERE_VISUAL, true);
                        summoned->SetReactState(REACT_AGGRESSIVE);
                        
                        if (Player *target = SelectRandomPlayerInTheMap(summoned->GetMap()))
                        {
                            summoned->AI()->AttackStart(target);
                            summoned->AI()->SetGUID(target->GetGUID(), TYPE_VICTIM);
                            summoned->CastSpell(target, SPELL_ICE_PULSE, true);
                        }
                        else
                        {
                            //There are no players to chase - just despawn the sphere
                            summoned->DespawnOrUnsummon();
                        }
                        break;
                    case NPC_RAGING_SPIRIT:
                        //if (Unit *victim = summoned->getVictim())
                        //    victim->CastSpell(summoned, SPELL_RAGING_VISUAL, true);
                        summoned->SetDisplayId(11649); //Some sort of spirit. Until Clone is fixed for non-minions, it's the only way to make a mob to look like something.
                        summoned->CastSpell(summoned, SPELL_NECROTIC_PLAGUE_IMMUNITY, true);
                        if (Unit *target = summoned->SelectNearbyTarget())
                            summoned->AI()->AttackStart(target);
                        break;
                    case NPC_DEFILE:
                    {
                        Position pos;
                        summoned->GetPosition(&pos);
                        pos.m_positionZ += 0.75f;
                        summoned->SetPosition(pos, true);
                        summoned->SetInCombatWithZone();
                        break;
                    }
                    case NPC_TRIGGER:
                        summoned->AI()->AttackStart(me);
                        summoned->SetVisible(false);
                        summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case NPC_FROSTMOURNE_TRIGGER:
                        summoned->CastSpell(summoned, SPELL_BROKEN_FROSTMOURNE, false);
                        summoned->CastSpell(summoned, SPELL_FROSTMOURNE_TRIGGER, false);
                        summoned->SetVisible(false);
                        break;
                    case CREAUTRE_MENETHIL:
                        DoScriptText(SAY_ENDING_9_FATHER, summoned);
                        summoned->CastSpell(summoned, SPELL_REVIVE, true);
                        summoned->CastSpell(summoned, SPELL_REVIVE_EFFECT, true);
                        summoned->CastSpell(summoned, SPELL_MENETHIL_VISUAL, true);
                        break;
                    case NPC_VALKYR:
                        summoned->CastSpell(summoned, SPELL_WINGS_OF_THE_DAMNED, true);
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                        {
                            summoned->AI()->AttackStart(target);
                            summoned->CastSpell(target, SPELL_VALKYR_TARGET_SEARCH, true);
                        }
                        else
                        {
                            //There is no target - unsummon valkyr
                            summoned->DespawnOrUnsummon();
                        }
                        break;
                    case NPC_DRUDGE_GHOUL:
                        summoned->CastSpell(summoned, SPELL_RAGING_GHOUL_VISUAL, true);
                        if (Unit *pVictim = SelectTarget(SELECT_TARGET_RANDOM))
                            summoned->AI()->AttackStart(pVictim);
                        break;
                    case NPC_SHAMBLING_HORROR:
                        summoned->CastSpell(summoned, SPELL_RISEN_WITCH_DOCTOR_SPAWN, true);
                        if (Unit *pVictim = SelectTarget(SELECT_TARGET_RANDOM))
                            summoned->AI()->AttackStart(pVictim);
                        break;
                }
            }

            void DoAction(const int32 action)
            {
                switch(action)
                {
                    case ACTION_CANCEL_ALL_TRANSITION_EVENTS:
                    {
                        events.CancelEvent(EVENT_PAIN_AND_SUFFERING);
                        events.CancelEvent(EVENT_SUMMON_ICE_SPHERE);
                        events.CancelEvent(EVENT_SUMMON_RAGING_SPIRIT);
                        events.CancelEvent(EVENT_TRANSITION_PHASE_END);
                        break;
                    }
                    case ACTION_PHASESWITCH_1:
                    {
                        uint32 nextPhase = PHASE_2_TRANSITION;
                        if (GetPhase(events) == PHASE_3)
                            nextPhase = PHASE_4_TRANSITION;
                        events.SetPhase(nextPhase);
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        SetCombatMovement(false);
                        me->SetInCombatWithZone();
                        me->GetMotionMaster()->MovePoint(POINT_PLATFORM_CENTER, MovePos[1]);
                        break;
                    }
                    case ACTION_PHASESWITCH_2:
                    {
                        if (GetPhase(events) == PHASE_2_TRANSITION)
                        {
                            events.SetPhase(PHASE_3);
                            events.ScheduleEvent(EVENT_SUMMON_VAL_KYR_SHADOWGUARD, 30000, 0, PHASE_3);
                            events.ScheduleEvent(EVENT_SOUL_REAPER, 17000, 0, PHASE_3);
                            events.ScheduleEvent(EVENT_DEFILE, 15000, 0, PHASE_3);
                            events.ScheduleEvent(EVENT_INFEST, 10000, 0, PHASE_3);
                        }
                        else
                        {
                            events.SetPhase(PHASE_5);
                            events.ScheduleEvent(EVENT_SUMMON_VILE_SPIRITS, 15000, 0, PHASE_5);
                            events.ScheduleEvent(EVENT_SOUL_REAPER, 35000, 0, PHASE_5);
                            events.ScheduleEvent(EVENT_DEFILE, 32000, 0, PHASE_5);
                            events.ScheduleEvent(EVENT_HARVEST_SOUL, 8000, 0, PHASE_5);
                        }
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveAurasDueToSpell(RAID_MODE<uint32>(SPELL_PAIN_AND_SUFFERING_10N, SPELL_PAIN_AND_SUFFERING_25N, SPELL_PAIN_AND_SUFFERING_10H, SPELL_PAIN_AND_SUFFERING_25H));
                        DoZoneInCombat(me);
                        SetCombatMovement(true);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        isSwitching = false;
                        break;
                    }
                }
            }

            void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
            {
                switch(GetPhase(events))
                {
                    case PHASE_1:
                        if(!HealthAbovePct(71) && !isSwitching)
                        {
                            isSwitching = true;
                            DoAction(ACTION_PHASESWITCH_1);
                            break;
                        }
                    case PHASE_3:
                        if(!HealthAbovePct(41) && !isSwitching)
                        {
                            isSwitching = true;
                            DoAction(ACTION_PHASESWITCH_1);
                            break;
                        }
                    case PHASE_5:
                        if(!HealthAbovePct(11) && !isSwitching)
                        {
                            isSwitching = true;
                            me->SummonCreature(NPC_TRIGGER, MovePos[6], TEMPSUMMON_CORPSE_DESPAWN, 900000);
                            summons.DespawnAll();
                            events.Reset();
                            events.SetPhase(PHASE_6_ENDING);
                        }
                        break;
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (GetPhase(events) != PHASE_6_ENDING
                    && (!UpdateVictim() || !CheckInRoom()))
                    return;

                events.Update(uiDiff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
                while (uint32 eventId = events.ExecuteEvent())
                {
                    //Remove players from threat list if they fall down
                    if (eventId == EVENT_CHECK_ALIVE_PLAYERS)
                    {
                        if (Creature *pTirion = ObjectAccessor::GetCreature(*me, instance->GetData64(GUID_TIRION)))
                        {
                            TPlayerList players = GetPlayersInTheMap(me->GetMap());
                            for (TPlayerList::iterator it = players.begin(); it != players.end(); ++it)
                                if ((*it)->GetDistanceZ(pTirion) > 10.0f)
                                {
                                    (*it)->RemoveAllAuras();
                                    me->DealDamage(*it, (*it)->GetHealth());
                                    pTirion->CastSpell(*it, SPELL_TELEPORT_ICC_FROZEN_THRONE, true);
                                }
                        }
                        events.ScheduleEvent(EVENT_CHECK_ALIVE_PLAYERS, 5000);
                    }
                    switch (GetPhase(events))
                    {
                        case PHASE_1:
                        {
                            switch (eventId)
                            {
                                case EVENT_SPEECH:
                                {
                                    DoScriptText(RAND(SAY_RANDOM_1, SAY_RANDOM_2), me);
                                    events.ScheduleEvent(EVENT_SPEECH, 33000, 0, PHASE_1);
                                    break;
                                }
                                case EVENT_SUMMON_SHAMBLING_HORROR:
                                {
                                    DoCast(SPELL_SUMMON_SHAMBLING_HORROR);
                                    events.ScheduleEvent(EVENT_SUMMON_SHAMBLING_HORROR, 35000, 0, PHASE_1);
                                    break;
                                }
                                case EVENT_SUMMON_DRUDGE_GHOULS:
                                {
                                    DoCast(SPELL_SUMMON_DRUDGE_GHOULS);
                                    events.ScheduleEvent(EVENT_SUMMON_DRUDGE_GHOULS, 30000, 0, PHASE_1);
                                    break;
                                }
                                case EVENT_INFEST:
                                {
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_INFEST))
                                        DoCast(target, SPELL_INFEST);
                                    events.ScheduleEvent(EVENT_INFEST, 20000, 0, PHASE_1);
                                    break;
                                }
                                case EVENT_NECROTIC_PLAGUE:
                                {
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -EVENT_NECROTIC_PLAGUE))
                                        DoCast(target, SPELL_NECROTIC_PLAGUE);
                                    events.ScheduleEvent(EVENT_NECROTIC_PLAGUE, 25000, 0, PHASE_1);
                                    break;
                                }
                                case EVENT_SHADOW_TRAP:
                                {
                                    ASSERT(IsHeroic());
                                    //First, try to select somebody far away from the boss
                                    Unit *target = NULL;
                                    target = SelectTarget(SELECT_TARGET_RANDOM, 0, -5.0f, true);
                                    if (!target)
                                        target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true);
                                    DoCast(target, SPELL_SUMMON_SHADOW_TRAP, true);
                                    events.ScheduleEvent(EVENT_SHADOW_TRAP, 30000, 0, PHASE_1);
                                }
                            }
                            break;
                        }
                        case PHASE_2_TRANSITION:
                        {
                            switch (eventId)
                            {
                                case EVENT_PAIN_AND_SUFFERING:
                                {
                                    if (Player *randomPlayer = SelectRandomPlayerInTheMap(me->GetMap()))
                                    {
                                        me->SetFacingToObject(randomPlayer);
                                        DoCast(randomPlayer, RAID_MODE<uint32>(SPELL_PAIN_AND_SUFFERING_10N, SPELL_PAIN_AND_SUFFERING_25N, SPELL_PAIN_AND_SUFFERING_10H, SPELL_PAIN_AND_SUFFERING_25H), true);
                                    }
                                    events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, 1500, 0, PHASE_2_TRANSITION);
                                    break;
                                }
                                case EVENT_SUMMON_RAGING_SPIRIT:
                                {
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                        DoCast(target, SPELL_SUMMON_RAGING_SPIRIT);
                                    events.ScheduleEvent(EVENT_SUMMON_RAGING_SPIRIT, RAID_MODE<uint32>(20000, 15000, 20000, 15000), 0, PHASE_2_TRANSITION);
                                    break;
                                }
                                case EVENT_SUMMON_ICE_SPHERE:
                                {
                                    DoCast(SPELL_SUMMON_ICE_SPEHERE);
                                    events.ScheduleEvent(EVENT_SUMMON_ICE_SPHERE, urand(6000, 8000), 0, PHASE_2_TRANSITION);
                                    break;
                                }
                                //case EVENT_TRANSITION_PHASE_END:
                                //{
                                //    DoAction(ACTION_PHASESWITCH_2);
                                //    break;
                                //}
                                case EVENT_BERSERK:
                                {
                                    events.Reset();
                                    DoScriptText(SAY_BERSERK, me);
                                    DoCast(me, SPELL_BERSERK2);
                                    break;
                                }
                            }
                            break;
                        }
                        case PHASE_3:
                        {
                            switch (eventId)
                            {
                                case EVENT_SUMMON_VAL_KYR_SHADOWGUARD:
                                {
                                    DoScriptText(SAY_SUMMON_VALKYR, me);
                                    DoCast(me, SPELL_SUMMON_VALKYR);
                                    events.ScheduleEvent(EVENT_SUMMON_VAL_KYR_SHADOWGUARD, urand(40000, 45000), 0, PHASE_3);
                                    break;
                                }
                                case EVENT_DEFILE:
                                {
                                    DoScriptText(SAY_EMOTE_DEFILE, me);
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                        DoCast(target, SPELL_SUMMON_DEFILE);
                                    events.ScheduleEvent(EVENT_DEFILE, 35000, 0, PHASE_3);
                                    break;
                                }
                                case EVENT_SOUL_REAPER:
                                {
                                    DoCastVictim(SPELL_SOUL_REAPER);
                                    DoCast(SPELL_SOUL_REAPER_HASTE_AURA);
                                    events.ScheduleEvent(EVENT_SOUL_REAPER, 30000, 0, PHASE_3);
                                    break;
                                }
                                case EVENT_INFEST:
                                {
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_INFEST))
                                        DoCast(target, SPELL_INFEST);
                                    events.ScheduleEvent(EVENT_INFEST, 20000, 0, PHASE_3);
                                    break;
                                }
                                case EVENT_BERSERK:
                                {
                                    DoScriptText(SAY_BERSERK, me);
                                    DoCast(me, SPELL_BERSERK2);
                                    break;
                                }
                            }
                            break;
                        }
                        case PHASE_4_TRANSITION:
                        {
                            switch (eventId)
                            {
                                case EVENT_PAIN_AND_SUFFERING:
                                {
                                    if (Player *randomPlayer = SelectRandomPlayerInTheMap(me->GetMap()))
                                    {
                                        me->SetFacingToObject(randomPlayer);
                                        DoCast(randomPlayer, RAID_MODE<uint32>(SPELL_PAIN_AND_SUFFERING_10N, SPELL_PAIN_AND_SUFFERING_25N, SPELL_PAIN_AND_SUFFERING_10H, SPELL_PAIN_AND_SUFFERING_25H), true);
                                    }
                                    events.ScheduleEvent(EVENT_PAIN_AND_SUFFERING, 1500, 0, PHASE_4_TRANSITION);
                                    break;
                                }
                                case EVENT_SUMMON_RAGING_SPIRIT:
                                {
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                        DoCast(target, SPELL_SUMMON_RAGING_SPIRIT);
                                    events.ScheduleEvent(EVENT_SUMMON_RAGING_SPIRIT, 15000, 0, PHASE_4_TRANSITION);
                                    break;
                                }
                                case EVENT_SUMMON_ICE_SPHERE:
                                {
                                    DoCast(SPELL_SUMMON_ICE_SPEHERE);
                                    events.ScheduleEvent(EVENT_SUMMON_ICE_SPHERE, urand(6000, 8000), 0, PHASE_4_TRANSITION);
                                    break;
                                }
                                //case EVENT_TRANSITION_PHASE_END:
                                //{
                                //    DoAction(ACTION_PHASESWITCH_2);
                                //    break;
                                //}
                                case EVENT_BERSERK:
                                {
                                    events.Reset();
                                    DoScriptText(SAY_BERSERK, me);
                                    DoCast(me, SPELL_BERSERK2);
                                    break;
                                }
                            }
                            break;
                        }
                        case PHASE_5:
                        {
                            switch (eventId)
                            {
                                case EVENT_DEFILE:
                                {
                                    DoScriptText(SAY_EMOTE_DEFILE, me);
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                        DoCast(target, SPELL_SUMMON_DEFILE);
                                    events.ScheduleEvent(EVENT_DEFILE, 35000, 0, PHASE_5);
                                    break;
                                }
                                case EVENT_SOUL_REAPER:
                                {
                                    DoCastVictim(SPELL_SOUL_REAPER);
                                    DoCast(SPELL_SOUL_REAPER_HASTE_AURA);
                                    events.ScheduleEvent(EVENT_SOUL_REAPER, 30000, 0, PHASE_5);
                                    break;
                                }
                                case EVENT_SUMMON_VILE_SPIRITS:
                                {
                                    DoCast(me, SPELL_SUMMON_VILE_SPIRIT);
                                    events.ScheduleEvent(EVENT_SUMMON_VILE_SPIRITS, 30000, 0, PHASE_5);
                                    break;
                                }
                                case EVENT_HARVEST_SOUL:
                                {
                                    DoScriptText(SAY_HARVEST_SOUL, me);
                                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                                        DoCast(target, SPELL_HARVEST_SOULS);
                                    events.ScheduleEvent(EVENT_HARVEST_SOUL, 75000, 0, PHASE_5);
                                    break;
                                }
                                case EVENT_BERSERK:
                                {
                                    DoScriptText(SAY_BERSERK, me);
                                    DoCast(me, SPELL_BERSERK2);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }

                if(GetPhase(events) == PHASE_6_ENDING)
                {
                    if (uiEndingTimer <= uiDiff)
                    {
                        switch(uiStage)
                        {
                            case 1:
                                me->GetMotionMaster()->MoveIdle();
                                me->SetReactState(REACT_PASSIVE);
                                me->AttackStop();
                                me->CastStop();
                                me->SetInCombatWithZone();
                                DoScriptText(SAY_10_PROZENT, me);
                                DoCast(SPELL_FURY_OF_FROSTMOURNE);
                                uiEndingTimer = 15000;
                                break;
                            case 2:
                                DoScriptText(SAY_ENDING_1_KING, me);
                                uiEndingTimer = 24000;
                                break;
                            case 3:
                                DoScriptText(SAY_ENDING_2_KING, me);
                                uiEndingTimer = 25000;
                                break;
                            case 4:
                                me->GetMotionMaster()->MovePoint(0, MovePos[1]);
                                uiEndingTimer = 4000;
                                break;
                            case 5:
                                DoScriptText(SAY_ENDING_3_KING, me);
                                DoCast(me, SPELL_CHANNEL_KING);
                                me->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                                uiEndingTimer = 28000;
                                break;
                            case 6:
                                DoScriptText(SAY_ENDING_4_KING, me);
                                uiEndingTimer = 8000;
                                break;
                            case 7:
                            {
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        DoScriptText(SAY_ENDING_5_TIRION, tirion);
                                uiEndingTimer = 11000;
                                break;
                            }
                            case 8:
                            {
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        tirion->CastSpell(tirion, SPELL_TIRION_LIGHT, true);
                                uiEndingTimer = 7000;
                                break;
                            }
                            case 9:
                            {
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        tirion->GetMotionMaster()->MovePoint(0, MovePos[2]);
                                uiEndingTimer = 1000;
                                break;
                            }
                            case 10:
                            {
                                if(uiTirionGUID)
                                {
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                    {
                                        tirion->GetMotionMaster()->MoveJump(517.482910f, -2124.905762f, 1040.861328f, 10.0f, 15.0f);
                                        tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, 375);
                                    }
                                }
                                uiEndingTimer = 1000;
                                break;
                            }
                            case 11:
                                me->RemoveAura(SPELL_CHANNEL_KING);
                                DoCast(me, SPELL_SUMMON_FROSTMOURNE);
                                DoCast(me, SPELL_SUMMON_BROKEN_FROSTMOURNE);
                                uiEndingTimer = 3000;
                                break;
                            case 12:
                                DoCast(me, SPELL_DROP_FROSTMOURNE);
                                DoCast(me, SPELL_SOUL_EFFECT);
                                DoPlaySoundToSet(me, SOUND_ENDING_7_KING);
                                uiEndingTimer = 1000;
                                break;
                            case 13:
                                DoScriptText(SAY_ENDING_6_KING, me);
                                uiEndingTimer = 3000;
                                break;
                            case 14:
                                DoCast(me, SPELL_SUMMON_FROSTMOURNE_TRIGGER);
                                uiEndingTimer = 2000;
                                break;
                            case 15:
                                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 473);
                                uiEndingTimer = 5000;
                                break;
                            case 16:
                            {
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        DoScriptText(SAY_ENDING_8_TIRION, tirion);
                                uiEndingTimer = 6000;
                                break;
                            }
                            case 17:
                            {
                                if(Creature* frostmourne = me->FindNearestCreature(NPC_FROSTMOURNE_TRIGGER, 25.0f, true))
                                    frostmourne->CastSpell(frostmourne, SPELL_SUMMON_MENETHIL, true);
                                uiEndingTimer = 3000;
                                break;
                            }
                            case 18:
                            {
                                if(Creature* father = me->FindNearestCreature(CREAUTRE_MENETHIL, 25.0f, true))
                                    DoScriptText(SAY_ENDING_11_FATHER, father);
                                uiEndingTimer = 6000;
                                break;
                            }
                            case 19:
                            {
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        DoScriptText(SAY_ENDING_10_TIRION, tirion);
                                uiEndingTimer = 5000;
                                break;
                            }
                            case 20:
                            {
                                DoScriptText(SAY_ENDING_12_KING, me);
                                me->GetMotionMaster()->MovePoint(0, MovePos[6]);
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        tirion->AI()->AttackStart(me);
                                if(Creature* father = me->FindNearestCreature(CREAUTRE_MENETHIL, 25.0f, true))
                                    father->AI()->AttackStart(me);
                                uiEndingTimer = 5000;
                                break;
                            }
                        }
                        ++uiStage;
                    } else uiEndingTimer -= uiDiff;
                }
                switch (GetPhase(events))
                {
                    case PHASE_1:
                    case PHASE_3:
                    case PHASE_5:
                        DoMeleeAttackIfReady();
                        break;
                }
            }
        private:
            InstanceScript* instance;

            uint8 uiStage;
            uint8 uiPhase;

            uint32 uiEndingTimer;
            uint32 uiSummonShamblingHorrorTimer;
            uint32 uiSummonDrudgeGhoulsTimer;
            uint32 uiSummonShadowTrap;
            uint32 uiInfestTimer;
            uint32 uiNecroticPlagueTimer;
            uint32 uiBerserkTimer;
            uint32 uiSummonValkyrTimer;
            uint32 uiSoulReaperTimer;
            uint32 uiDefileTimer;
            uint32 uiHarvestSoulTimer;
            uint32 uiSummonVileSpiritTimer;
            uint32 uiIcePulsSummonTimer;
            uint32 uiSummonSpiritTimer;
            uint32 uiRandomSpeechTimer;
            uint64 uiTirionGUID;
            bool isSwitching;
            SummonList summons;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_the_lich_kingAI(creature);
        }
};

class npc_tirion_icc : public CreatureScript
{
    public:
        npc_tirion_icc() : CreatureScript("npc_tirion_icc") { }

        struct npc_tirion_iccAI : public ScriptedAI
        {
            npc_tirion_iccAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                uiIntroTimer = 1000;
                uiStage = 1;
                uiLichKingGUID = 0;
                bIntro = false;

                me->RemoveAllAuras();
                me->SetReactState(REACT_PASSIVE);
                me->SetSpeed(MOVE_RUN, 1.8f);
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->GetMotionMaster()->MovePoint(POINT_PLATFORM_CENTER, MovePos[8]);
                DoCast(me, SPELL_REVIVE, true);
                DoCast(SPELL_WMO_INTACT);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;
                switch (id)
                {
                    case POINT_PLATFORM_CENTER:
                    {
                        if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                            me->GetMotionMaster()->MovementExpired();
                        me->SetPosition(MovePos[8]);
                        me->SetOrientation(3.1416f);
                        me->SetFacing(3.1416f);
                        break;
                    }
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_START_EVENT:
                    {
                        bIntro = true;
                        if(instance)
                            uiLichKingGUID = instance->GetData64(GUID_LICH_KING);
                        break;
                    }
                    case ACTION_RESET:
                    {
                        Reset();
                        break;
                    }
                }
            }

            void SpellHit(Unit* /*caster*/, const SpellEntry * spell)
            {
                if(spell->Id == SPELL_LIGHTS_BLESSING)
                    me->RemoveAurasDueToSpell(SPELL_ICEBLOCK_TRIGGER);
            }

            void UpdateAI(const uint32 diff)
            {
                if(!bIntro || !uiLichKingGUID)
                    return;
                if (uiStage > 11)
                    return;
                if(uiIntroTimer <= diff)
                {
                    switch(uiStage)
                    {
                        case 1:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            {
                                lich->SetStandState(UNIT_STAND_STATE_STAND);
                                lich->GetMotionMaster()->MovePoint(POINT_START_EVENT_1, MovePos[0]);
                                me->SetFacingToObject(lich);
                            }
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                            uiIntroTimer = 3000;
                            break;
                        }
                        case 2:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            {
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                                DoScriptText(SAY_INTRO_1_KING, lich);
                            }
                            uiIntroTimer = 14000;
                            break;
                        }
                        case 3:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            DoScriptText(SAY_INTRO_2_TIRION, me);
                            uiIntroTimer = 9000;
                            break;
                        }
                        case 4:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            {
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_LAUGH);
                                DoScriptText(SAY_INTRO_3_KING, lich);
                            }
                            uiIntroTimer = 3000;
                            break;
                        }
                        case 5:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_POINT_NOSHEATHE);
                            uiIntroTimer = 2000;
                            break;
                        }
                        case 6:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            uiIntroTimer = 18000;
                            break;
                        }
                        case 7:
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_POINT_NOSHEATHE);
                            DoScriptText(SAY_INTRO_4_TIRION, me);
                            uiIntroTimer = 1000;
                            break;
                        case 8:
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            me->GetMotionMaster()->MovePoint(0, MovePos[3]);
                            uiIntroTimer = 2000;
                            break;
                        case 9:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->CastSpell(me, SPELL_ICEBLOCK_TRIGGER, true);
                            uiIntroTimer = 2000;
                            break;
                        }
                        case 10:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                DoScriptText(SAY_INTRO_5_KING, lich);
                            uiIntroTimer = 18000;
                            break;
                        }
                        case 11:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            {
                                lich->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                lich->SetReactState(REACT_AGGRESSIVE);
                                if(Unit* target = lich->FindNearestPlayer(100.0f))
                                    lich->AI()->AttackStart(target);
                            }
                            break;
                        }
                    }
                    ++uiStage;
                } else uiIntroTimer -= diff;
            }
            private:
                InstanceScript* instance;

                uint64 uiLichKingGUID;
                uint32 uiIntroTimer;
                uint8 uiStage;
                bool bIntro;
        };

        bool OnGossipHello(Player* player, Creature* creature)
        {
            InstanceScript* instance = creature->GetInstanceScript();
            if (!instance)
                return false;

            bool isEveryoneFriendly = true;
            const Map::PlayerList &PlayerList = creature->GetMap()->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if (Player* player = i->getSource())
                        if (!creature->IsFriendlyTo(player))
                        {
                            isEveryoneFriendly = false;
                            break;
                        }

            if (!isEveryoneFriendly)
            {
                creature->MonsterSay("Sorry, but everyone in raid should have at least friendly reputation with the Argent Crusade to participate in the final battle.", LANG_UNIVERSAL, player->GetGUID());
                return true;
            }

            if (instance->GetData(DATA_LICH_KING_EVENT) == DONE)
            {
                creature->MonsterSay("Sorry, The Lich King is already defeated on current raid lock. Clear raid lock and try again.", LANG_UNIVERSAL, player->GetGUID());
                return true;
            }

            if ((!player->GetGroup() || !player->GetGroup()->IsLeader(player->GetGUID())) && !player->isGameMaster())
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Sorry, I'm not the raid leader", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                player->SEND_GOSSIP_MENU(GOSSIP_MENU, creature->GetGUID());
                return true;
            }

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START_EVENT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->SEND_GOSSIP_MENU(GOSSIP_MENU, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
        {
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF+2:
                    creature->MonsterSay("OK, I'll wait for raid leader", LANG_UNIVERSAL, player->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+3:
                    CAST_AI(npc_tirion_icc::npc_tirion_iccAI, creature->AI())->DoAction(ACTION_START_EVENT);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                default:
                    creature->MonsterSay("You've just found a bug. Contact server admin and explain what to do to reproduce this bug", LANG_UNIVERSAL, player->GetGUID());
                    break;
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tirion_iccAI(creature);
        }
};

class npc_valkyr_icc : public CreatureScript
{
enum eEvents
{
    EVENT_MOVE_TO_PLATFORM_EDGE = 1,
    EVENT_SIPHON_LIFE,
    EVENT_CHECK_AT_PLATFORM_EDGE
};
static const float Z_FLY;
    public:
        npc_valkyr_icc() : CreatureScript("npc_valkyr_icc") { }

        struct npc_valkyr_iccAI : public ScriptedAI
        {
            npc_valkyr_iccAI(Creature* creature) : ScriptedAI(creature), vehicle(creature->GetVehicleKit())
            {
                ASSERT(vehicle);
            }

            void Reset()
            {
                float speedRate = me->GetSpeedRate(MOVE_RUN);
                me->SetSpeed(MOVE_FLIGHT, speedRate);
                me->SetSpeed(MOVE_RUN, speedRate);
                m_moveUpdatePeriod = 100;
                events.Reset();
                me->SetFlying(true);
                bCanCast = false;

                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    me->GetMotionMaster()->MovementExpired();
            }

            void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
            {
                if(!HealthAbovePct(50) && IsHeroic() && !bCanCast)
                {
                    vehicle->RemoveAllPassengers();
                    me->RemoveAllAuras();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->GetMotionMaster()->MovePoint(POINT_VALKYR_ZET, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 10);
                    events.Reset();
                }
            }

            void SpellHit(Unit *attacker, const SpellEntry *spellEntry)
            {
                if (spellEntry && spellEntry->Id == SPELL_VALKYR_GRAB_PLAYER)
                {
                    float x, y, z;
                    me->GetPosition(x, y, z);
                    me->SetPosition(x, y, Z_FLY, 0.0f, true);

                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    SetCombatMovement(false);
                    me->SetInCombatWithZone();
                    
                    me->AddUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CHARM, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_FEAR, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ROOT, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_PACIFY, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_SILENCE, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_TRANSFORM, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_SCALE, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_DISARM, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_TRANSFORM, true);
                    me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); //Set immune to DK's Death grip
                    //The Val'kyrs always choose the closest edge of the platform).
                    if (Creature *pEdgeStalker = me->FindNearestCreature(NPC_PLATFORM_DESTRUCTIBLE_EDGE_STALKER, 1000.0f, true))
                    {
                        Position edgePos;
                        me->GetPosition(&m_pos);
                        m_pos.m_positionZ = Z_FLY;
                        pEdgeStalker->GetPosition(&edgePos);
                        m_angle = m_pos.GetAngle(&edgePos);
                        me->SetFacingToObject(pEdgeStalker);
                        events.ScheduleEvent(EVENT_MOVE_TO_PLATFORM_EDGE, 1000);
                        events.ScheduleEvent(EVENT_CHECK_AT_PLATFORM_EDGE, 1000);
                    }
                    else
                    {
                        me->DespawnOrUnsummon();
                    }
                    //me->GetMotionMaster()->MovePoint(POINT_PLATFORM_END, MovePos[4]);
                }
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if(type != POINT_MOTION_TYPE)
                    return;

                if(bCanCast)
                    me->GetMotionMaster()->Clear();

                switch(id)
                {
                    case POINT_PLATFORM_END:
                    {
                        DoCast(me, SPELL_VALKYR_EJECT_PASSENGER);
                        //Fly 15 feet upward, then despawn
                        me->GetPosition(&m_pos);
                        m_pos.m_positionZ = 1055.0f;
                        me->GetMotionMaster()->MovePoint(POINT_VALKYR_END, m_pos);
                        break;
                    }
                    case POINT_VALKYR_END:
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }
                    case POINT_VALKYR_ZET:
                    {
                        events.ScheduleEvent(EVENT_SIPHON_LIFE, 3000);
                        bCanCast = true;
                        break;
                    }
                    case POINT_VALKYR_CONTINUE_FLYING:
                    {
                        me->SetPosition(m_pos);
                        events.ScheduleEvent(EVENT_MOVE_TO_PLATFORM_EDGE, m_moveUpdatePeriod);
                        break;
                    }
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                //if (!UpdateVictim() || !bCanCast)
                //    return;
                if (!me->isAlive() || me->HasUnitState(UNIT_STAT_CASTING))
                    return;
                events.Update(uiDiff);
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_AT_PLATFORM_EDGE:
                        {
                            if (me->GetDistance2d(MovePos[1].m_positionX, MovePos[1].m_positionY) > 55.0f)
                            {
                                events.Reset();
                                DoCast(me, SPELL_VALKYR_EJECT_PASSENGER);
                                //Fly 15 feet upward, then despawn
                                me->GetPosition(&m_pos);
                                m_pos.m_positionZ = 1055.0f;
                                me->GetMotionMaster()->MovePoint(POINT_VALKYR_END, m_pos);
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_AT_PLATFORM_EDGE, 1000);
                        }
                        case EVENT_MOVE_TO_PLATFORM_EDGE:
                        {
                            me->GetPosition(&m_pos);
                            if (!me->HasAuraType(SPELL_AURA_MOD_STUN))
                            {
                                float flySpeed = me->GetSpeed(MOVE_RUN) * m_moveUpdatePeriod / 10000;
                                m_pos.m_positionX += flySpeed * cosf(m_angle);
                                m_pos.m_positionY += flySpeed * sinf(m_angle);
                                m_pos.m_positionZ = Z_FLY;
                            }
                            me->SetFacing(m_angle);
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(POINT_VALKYR_CONTINUE_FLYING, m_pos);
                            me->SetPosition(m_pos);
                            break;
                        }
                        case EVENT_SIPHON_LIFE:
                        {
                            DoCastVictim(SPELL_LIFE_SIPHON);
                            events.ScheduleEvent(EVENT_SIPHON_LIFE, 3000);
                        }
                    }
                }
            }
        private:
            bool bCanCast;
            EventMap events;
            Position m_pos;
            float m_angle;
            uint32 m_moveUpdatePeriod;
            Vehicle* vehicle;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_valkyr_iccAI(creature);
        }
};
const float npc_valkyr_icc::Z_FLY = 1045.0f;

class npc_vile_spirit_icc : public CreatureScript
{
enum eEvents
{
    EVENT_BECOME_ACTIVE = 1,
    EVENT_DESPAWN,
    EVENT_MOVE_RANDOM
};
enum ePoints
{
    POINT_MOVE_NEAR_RANDOM = 1
};
public:
    static const float Z_VILE_SPIRIT;
    npc_vile_spirit_icc() : CreatureScript("npc_vile_spirit_icc") { }

    struct npc_vile_spirit_iccAI : public ScriptedAI
    {
        npc_vile_spirit_iccAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            events.ScheduleEvent(EVENT_BECOME_ACTIVE, 15000);
            //If they don't reach that player within around 30 seconds, they will despawn harmlessly.
            events.ScheduleEvent(EVENT_DESPAWN, 45000);
            SetCombatMovement(false);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlying(true);
            float x, y, z;
            me->GetPosition(x, y, z);
            me->SetPosition(x, y, npc_vile_spirit_icc::Z_VILE_SPIRIT, true);
            Position randomPos;
            float dist = 1.0f * rand_norm() * 10.0f;
            me->GetRandomNearPosition(randomPos, dist);
            randomPos.m_positionZ = Z_VILE_SPIRIT;
            me->GetMotionMaster()->MovePoint(POINT_MOVE_NEAR_RANDOM, randomPos);
            m_bActive = false;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            if (id == POINT_MOVE_NEAR_RANDOM)
            {
                me->GetMotionMaster()->MovementExpired();
                if (!m_bActive)
                    events.ScheduleEvent(EVENT_MOVE_RANDOM, 2000);
            }
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_DESPAWN:
                {
                    me->RemoveAura(SPELL_VILE_SPIRIT_DISTANCE_CHECK);
                    events.ScheduleEvent(EVENT_DESPAWN, 1000);
                    me->SetReactState(REACT_PASSIVE);
                    SetCombatMovement(false);
                    break;
                }
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            events.Update(uiDiff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_RANDOM:
                    {
                        Position randomPos;
                        float dist = 1.0f * rand_norm() * 10.0f;
                        me->GetRandomNearPosition(randomPos, dist);
                        randomPos.m_positionZ = Z_VILE_SPIRIT;
                        me->GetMotionMaster()->MovePoint(POINT_MOVE_NEAR_RANDOM, randomPos);
                        break;
                    }
                    case EVENT_BECOME_ACTIVE:
                    {
                        events.CancelEvent(EVENT_MOVE_RANDOM);
                        m_bActive = true;
                        SetCombatMovement(true);
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->GetMotionMaster()->MovementExpired();
                        DoCast(me, SPELL_VILE_SPIRIT_ACTIVE, true);
                        //DoCast(me, SPELL_VILE_SPIRIT_TARGET_SEARCH, true);
                        DoCast(me, SPELL_VILE_SPIRIT_DISTANCE_CHECK, true);
                        //Try to select random player on the platform 10 times.
                        Player *player = NULL;
                        uint8 count = 0;
                        do
                        {
                            player = SelectRandomPlayerInTheMap(me->GetMap());
                        }
                        while (player->GetDistance(me) > 100.f && (++count) < 10);
                        AttackStart(player);
                        break;
                    }
                    case EVENT_DESPAWN:
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
        bool m_bActive;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_vile_spirit_iccAI(creature);
    }
};
const float npc_vile_spirit_icc::Z_VILE_SPIRIT = 1050.0f;

//class spell_lich_king_pain_and_suffering : public SpellScriptLoader
//{
//    public:
//        spell_lich_king_pain_and_suffering() : SpellScriptLoader("spell_lich_king_pain_and_suffering") { } //74115
//
//        class spell_lich_king_pain_and_suffering_AuraScript : public AuraScript
//        {
//            PrepareAuraScript(spell_lich_king_pain_and_suffering_AuraScript)
//
//            class AnyAlivePetOrPlayerInObjectFrontalConeCheck
//            {
//                public:
//                    AnyAlivePetOrPlayerInObjectFrontalConeCheck(WorldObject const* obj) : i_obj(obj) {}
//                    bool operator()(Unit* u)
//                    {
//                        if (u->GetTypeId() != TYPEID_PLAYER)
//                            return false;
//                        if (!u->isTargetableForAttack())
//                            return false;
//                        if (!u->isAlive())
//                            return false;
//                        Position myPos, uPos;
//                        i_obj->GetPosition(&myPos);
//                        u->GetPosition(&uPos);
//                        float orientation = i_obj->GetOrientation();
//                        float angle = myPos.GetAngle(&uPos);
//                        float coneAngle = M_PI / 180 * 1.0f;
//                        angle = MapManager::NormalizeOrientation(orientation - angle);
//                        if ((0.0f <= angle) && (angle <= coneAngle / 2) ||
//                            ((2 * M_PI - coneAngle / 2) <= angle) && (angle <= (2 * M_PI)))
//                            return true;
//                        return false;
//                    }
//                private:
//                    WorldObject const* i_obj;
//            };
//            void OnPeriodic(AuraEffect const*aurEff)
//            {
//                PreventDefaultAction();
//                Unit *caster = GetCaster();
//                if (!caster || !caster->isAlive() || caster->HasUnitState(UNIT_STAT_CASTING))
//                    return;
//                AnyAlivePetOrPlayerInObjectFrontalConeCheck checker(caster);
//                std::list<Unit *> targets;
//                Trinity::UnitListSearcher<AnyAlivePetOrPlayerInObjectFrontalConeCheck> searcher(caster, targets, checker);
//
//                TypeContainerVisitor<Trinity::UnitListSearcher<AnyAlivePetOrPlayerInObjectFrontalConeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
//                TypeContainerVisitor<Trinity::UnitListSearcher<AnyAlivePetOrPlayerInObjectFrontalConeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);
//
//                CellPair p(Trinity::ComputeCellPair(caster->GetPositionX(), caster->GetPositionY()));
//                Cell cell(p);
//                cell.data.Part.reserved = ALL_DISTRICT;
//                cell.SetNoCreate();
//
//                cell.Visit(p, world_unit_searcher, *GetTarget()->GetMap(), *GetTarget(), 100.0f);
//                cell.Visit(p, grid_unit_searcher, *GetTarget()->GetMap(), *GetTarget(), 100.0f);
//
//                for (std::list<Unit*>::iterator it = targets.begin(); it != targets.end(); ++it)
//                    caster->CastSpell((*it), SPELL_PAIN_AND_SUFFERING_DAMAGE, true);
//                //Next time try to target somebody else
//                if (Player *randomTarget = SelectRandomPlayerInTheMap(caster->GetMap()))
//                    caster->SetFacingToObject(randomTarget);
//            }
//
//
//            void Register()
//            {
//                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_pain_and_suffering_AuraScript::OnPeriodic, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL); //74117
//            }
//        };
//
//        AuraScript* GetAuraScript() const
//        {
//            return new spell_lich_king_pain_and_suffering_AuraScript();
//        }
//};

class spell_lich_king_pain_and_suffering_effect : public SpellScriptLoader
{
    public:
        spell_lich_king_pain_and_suffering_effect() : SpellScriptLoader("spell_lich_king_pain_and_suffering_effect") { }


        class spell_lich_king_pain_and_suffering_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lich_king_pain_and_suffering_effect_SpellScript);

            //void HandleScript(SpellEffIndex effIndex)
            //{
            //    Unit *caster = GetCaster();
            //    if (!caster || !caster->isAlive())
            //    {
            //        PreventHitDefaultEffect(effIndex);
            //        return;
            //    }
            //    //Next time try to target random player
            //    if (Player *randomTarget = SelectRandomPlayerInTheMap(caster->GetMap()))
            //        caster->SetFacingToObject(randomTarget);
            //}
            class AnyAlivePetOrPlayerInObjectFrontalConeCheck
            {
                public:
                    AnyAlivePetOrPlayerInObjectFrontalConeCheck(WorldObject const* obj) : i_obj(obj) {}
                    bool operator()(Unit* u)
                    {
                        if (u->GetTypeId() != TYPEID_PLAYER)
                            return true;
                        if (!u->isTargetableForAttack())
                            return true;
                        if (!u->isAlive())
                            return true;
                        Position myPos, uPos;
                        i_obj->GetPosition(&myPos);
                        u->GetPosition(&uPos);
                        float orientation = i_obj->GetOrientation();
                        float angle = myPos.GetAngle(&uPos);
                        float coneAngle = M_PI / 180 * 15.0f;
                        angle = MapManager::NormalizeOrientation(orientation - angle);
                        if ((0.0f <= angle) && (angle <= coneAngle / 2) ||
                            ((2 * M_PI - coneAngle / 2) <= angle) && (angle <= (2 * M_PI)))
                            return false;
                        return true;
                    }
                private:
                    WorldObject const* i_obj;
            };

            void FilterTargets(std::list<Unit*>& unitList)
            {
                Unit *caster = GetCaster();
                if (!caster || !caster->isAlive())
                    return;
                unitList.remove_if(AnyAlivePetOrPlayerInObjectFrontalConeCheck(caster)); 
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_pain_and_suffering_effect_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_ENEMY);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_pain_and_suffering_effect_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_AREA_PATH);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_pain_and_suffering_effect_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_AREA_PATH);
                //OnEffect += SpellEffectFn(spell_lich_king_pain_and_suffering_effect_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lich_king_pain_and_suffering_effect_SpellScript();
        }
};

class spell_lich_king_necrotic_plague : public SpellScriptLoader
{
    public:
        spell_lich_king_necrotic_plague() : SpellScriptLoader("spell_lich_king_necrotic_plague") { } //70338

        class spell_lich_king_necrotic_plague_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_necrotic_plague_AuraScript)

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                m_stackAmount = GetStackAmount();
                if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                {
            //        //SetStackAmount(instance->GetData(DATA_NECROTIC_STACK));
            //        instance->SetData(DATA_NECROTIC_STACK, GetStackAmount());
                    if(GetStackAmount() >= 30)
                        instance->SetData(DATA_BEEN_WAITING_ACHIEVEMENT, DONE);
                }
            }
            class AnyAliveCreatureOrPlayerInObjectRangeCheck
            {
                public:
                    AnyAliveCreatureOrPlayerInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range) {}
                    bool operator()(Unit* u)
                    {
                        if (!u->isTargetableForAttack())
                            return false;
                        //Ignore the object itself
                        if (u->GetGUID() == i_obj->GetGUID())
                            return false;
                        // Check contains checks for: live, non-selectable, non-attackable flags, flight check and GM check, ignore totems, ignore pets
                        if (!(u->isAlive() && i_obj->IsWithinDistInMap(u, i_range)))
                            return false;
                        if (u->GetTypeId() == TYPEID_PLAYER)
                            return true;
                        //Ignore non-creatures (players were already checked a line above)
                        if (u->GetTypeId() != TYPEID_UNIT)
                            return false;
                        //Ignore totems
                        if (((Creature*)u)->isTotem())
                            return false;
                        //Ignore pets
                        if (u->GetOwner() && u->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                            return false;
                        //Ignore Tirion, The Lich King, Raging Spirits, too
                        uint32 entry = u->ToCreature()->GetEntry();
                        if (entry == NPC_LICH_KING || entry == NPC_TIRION_ICC || entry == NPC_RAGING_SPIRIT)
                            return false;
                        return true;
                    }
                private:
                    WorldObject const* i_obj;
                    float i_range;
            };
            //void OnPeriodic(AuraEffect const*aurEff)
            //{
            //    PreventDefaultAction();
            //    if (!(GetTarget() && GetTarget()->isAlive() && GetCaster() && GetCaster()->isAlive()))
            //        return;
            //    m_stackAmount = GetStackAmount();
            //    GetCaster()->DealDamage(GetTarget(), (uint32)aurEff->GetBaseAmount() * m_stackAmount, 0, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, aurEff->GetSpellProto(), true);
            //    //if (GetTarget() && GetTarget()->isAlive() && GetCaster() && GetCaster()->isAlive())
            //    //    GetCaster()->CastSpell(GetTarget(), SPELL_NECROTIC_PLAGUE_EFFECT, true);
            //}
            void OnRemove(AuraEffect const * aurEff, AuraEffectHandleModes mode)
            {
                CellPair p(Trinity::ComputeCellPair(GetTarget()->GetPositionX(), GetTarget()->GetPositionY()));
                Cell cell(p);
                cell.data.Part.reserved = ALL_DISTRICT;
                cell.SetNoCreate();

                Unit *anyPlayerOrCreatureInRange = NULL;
                float dist = 10.0f;
                AnyAliveCreatureOrPlayerInObjectRangeCheck checker(GetTarget(), dist);
                Unit *newTarget = NULL;
                Trinity::UnitLastSearcher<AnyAliveCreatureOrPlayerInObjectRangeCheck> searcher(GetTarget(), newTarget, checker);

                TypeContainerVisitor<Trinity::UnitLastSearcher<AnyAliveCreatureOrPlayerInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
                TypeContainerVisitor<Trinity::UnitLastSearcher<AnyAliveCreatureOrPlayerInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

                cell.Visit(p, world_unit_searcher, *GetTarget()->GetMap(), *GetTarget(), dist);
                cell.Visit(p, grid_unit_searcher, *GetTarget()->GetMap(), *GetTarget(), dist);

                uint32 stacksTransferred = GetStackAmount();
                //If target is still alive and it's a player, it means that this spell was dispelled - increase stack amount then
                //I don't know the way to find out whether it's dispelled or not
                if (!GetTarget()->isAlive())
                    ++stacksTransferred;
                else
                {
                    //Player was a target and is still alive - assume that the spell was dispelled
                    if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                        if (stacksTransferred > 1)
                            --stacksTransferred;
                }
                if (stacksTransferred < 1)
                    stacksTransferred = 1;
                uint32 spellId = aurEff->GetSpellProto()->Id;
                if (newTarget)
                {
                    Aura *appAura = newTarget->GetAura(spellId);
                    if (!appAura)
                    {
                        GetCaster()->CastSpell(newTarget, spellId, true);
                        appAura = newTarget->GetAura(spellId);
                        --stacksTransferred; //One stack is already transferred
                    }
                    if (appAura)
                    {
                        appAura->SetStackAmount(appAura->GetStackAmount() + stacksTransferred);
                        appAura->RefreshDuration();
                    }
                }
                Aura *plagueSiphon = GetCaster()->GetAura(SPELL_PLAGUE_SIPHON);
                if (!plagueSiphon)
                {
                    GetCaster()->CastSpell(GetCaster(), SPELL_PLAGUE_SIPHON, true);
                    plagueSiphon = GetCaster()->GetAura(SPELL_PLAGUE_SIPHON);
                }
                if (plagueSiphon)
                {
                    plagueSiphon->ModStackAmount(1);
                    plagueSiphon->RefreshDuration();
                }
            }

            void Register()
            {
                //OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_necrotic_plague_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectApply += AuraEffectApplyFn(spell_lich_king_necrotic_plague_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_necrotic_plague_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        private:
            uint8 m_stackAmount;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_necrotic_plague_AuraScript();
        }
};

//class spell_lich_king_defile : public SpellScriptLoader
//{
//    public:
//        spell_lich_king_defile() : SpellScriptLoader("spell_lich_king_defile") { } 
//
//        class spell_lich_king_defile_SpellScript : public SpellScript
//        {
//            PrepareSpellScript(spell_lich_king_defile_SpellScript)
//
//            bool Load()
//            {
//                m_hitCount = 0;
//                m_radius = 8.0f;
//                return true;
//            }
//
//            void FilterTargets(std::list<Unit*>& unitList)
//            {
//                LeaveOnlyPlayers(unitList);
//                Unit *caster = GetCaster();
//                for (std::list<Unit*>::iterator it = unitList.begin(); it != unitList.end(); )
//                {
//                    if ((*it)->GetDistance2d(caster) > m_radius)
//                        unitList.erase(it++);
//                    else
//                        ++it;                        
//                }
//                if (!unitList.empty())
//                    ++m_hitCount;
//            }
//
//            void HandleDamage(SpellEffIndex effIndex)
//            {
//                //PreventHitDefaultEffect(effIndex);
//                
//                Unit *caster = GetCaster();
//                if (!(caster && caster->isAlive()) && caster->GetAI())
//                    return;
//                Map *pMap = caster->GetMap();
//                //Radius increases by 10% per hit on heroic and by 5% if it's normal
//                m_radius = 8.0f + m_hitCount;
//                uint32 triggeredSpellId = SPELL_DEFILE_DAMAGE;
//                int32 triggeredSpellBaseDamage = 3000;
//                if (SpellEntry const* defileDamage = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE_DAMAGE), caster))
//                {
//                    triggeredSpellId = defileDamage->Id;
//                    triggeredSpellBaseDamage = (int32)(defileDamage->EffectBasePoints[EFFECT_0] * (1.0f + (pMap->IsHeroic() ? 0.1f : 0.05f) * m_hitCount));
//                }
//                int hitDamage = (int32)(GetSpellInfo()->EffectBasePoints[EFFECT_0] * (1.0f + (pMap->IsHeroic() ? 0.1f : 0.05f) * m_hitCount));
//                SetHitDamage(hitDamage);
//
//                if (SpellEntry const* defileIncrease = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE_INCREASE), caster))
//                {
//                    caster->CastSpell(caster, defileIncrease->Id, true);
//                    if (Aura *defileIncreaseAura = caster->GetAura(defileIncrease->Id))
//                        m_hitCount = defileIncreaseAura->GetStackAmount();
//                }
//            }
//
//            void HandleDefileIncrease(SpellEffIndex effIndex)
//            {
//                PreventHitDefaultEffect(effIndex);
//            }
//
//            void Register()
//            {
//                OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_defile_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_AREA_ENEMY_SRC);
//                OnEffect += SpellEffectFn(spell_lich_king_defile_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
//                OnEffect += SpellEffectFn(spell_lich_king_defile_SpellScript::HandleDefileIncrease, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
//            }
//        private:
//            uint32 m_hitCount;
//            float m_radius;
//        };
//
//        SpellScript* GetSpellScript() const
//        {
//            return new spell_lich_king_defile_SpellScript();
//        }
//};

class spell_lich_king_defile : public SpellScriptLoader
{
    public:
        spell_lich_king_defile() : SpellScriptLoader("spell_lich_king_defile") { } 

        class spell_lich_king_defile_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_defile_AuraScript)

            void OnPeriodic(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                Unit *caster = GetCaster();
                if (!(caster && caster->isAlive()) && caster->GetAI())
                    return;
                Map *pMap = caster->GetMap();
                //Radius increases by 10% per hit on heroic and by 5% if it's normal
                m_radius = 8.0f + m_hitCount;
                //Find targest
                std::list<Unit *> targets;
                Trinity::AnyUnfriendlyVisibleUnitInObjectRangeCheck checker(caster, caster, m_radius); 

                Trinity::UnitListSearcher<Trinity::AnyUnfriendlyVisibleUnitInObjectRangeCheck> searcher(caster, targets, checker);

                TypeContainerVisitor<Trinity::UnitListSearcher<Trinity::AnyUnfriendlyVisibleUnitInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
                TypeContainerVisitor<Trinity::UnitListSearcher<Trinity::AnyUnfriendlyVisibleUnitInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

                CellPair p(Trinity::ComputeCellPair(caster->GetPositionX(), caster->GetPositionY()));
                Cell cell(p);
                cell.data.Part.reserved = ALL_DISTRICT;
                cell.SetNoCreate();

                cell.Visit(p, world_unit_searcher, *pMap, *caster, 100.0f);
                cell.Visit(p, grid_unit_searcher, *pMap, *caster, 100.0f);

                if (targets.empty())
                    return;
                uint32 triggeredSpellId = SPELL_DEFILE_DAMAGE;
                int32 triggeredSpellBaseDamage = 3000;
                if (SpellEntry const* defileDamage = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE_DAMAGE), caster))
                {
                    triggeredSpellId = defileDamage->Id;
                    triggeredSpellBaseDamage = (int32)(defileDamage->EffectBasePoints[EFFECT_0] * (1.0f + (pMap->IsHeroic() ? 0.1f : 0.05f) * m_hitCount));
                }

                bool increaseRadius = false;
                uint64 ownerGuid = (caster->GetOwner() ? caster->GetOwner()->GetGUID() : 0);
                Unit *curVictim = NULL;
                for (std::list<Unit*>::iterator it = targets.begin(); it != targets.end(); ++it)
                {
                    curVictim = *it;
                    if (curVictim->GetGUID() == ownerGuid)
                        continue;
                    if (curVictim->GetTypeId() != TYPEID_PLAYER)
                        continue;
                    if (curVictim->GetDistance2d(caster) > m_radius)
                        continue;
                    caster->CastCustomSpell(triggeredSpellId, SPELLVALUE_BASE_POINT0, triggeredSpellBaseDamage, curVictim, true, NULL, NULL, caster->GetGUID());
                    increaseRadius = true;
                }
                if (!increaseRadius)
                    return;
                if (SpellEntry const* defileIncrease = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE_INCREASE), caster))
                {
                    caster->CastSpell(caster, defileIncrease->Id, true);
                    if (Aura *defileIncreaseAura = caster->GetAura(defileIncrease->Id))
                        m_hitCount = defileIncreaseAura->GetStackAmount();
                    else
                        ++m_hitCount;
                }
            }

            void Register()
            {
                m_hitCount = 0;
                m_radius = 0.0f;
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_defile_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        private:
            uint32 m_hitCount;
            float m_radius;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_defile_AuraScript();
        }
};

class spell_lich_king_infection : public SpellScriptLoader
{
    public:
        spell_lich_king_infection() : SpellScriptLoader("spell_lich_king_infection") { } //70541


        class spell_lich_king_infection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_infection_AuraScript)

            void HandleTick(AuraEffect const*aurEff)
            {
                if (!GetTarget()->isAlive() || GetTarget()->GetHealthPct() >= 90)
                {
                    //Aura::ApplicationMap &appMap = const_cast<Aura::ApplicationMap&>(aurEff->GetBase()->GetApplicationMap());
                    //Aura::ApplicationMap::iterator it = appMap.find(GetTarget()->GetGUID());
                    //if (it != appMap.end())
                    //    appMap.erase(it);
                    PreventDefaultAction();
                    GetTarget()->RemoveAurasDueToSpell(aurEff->GetSpellProto()->Id);
                }
            }
            void OnCalcAmount(AuraEffect const* aurEff, int32 & amount, bool & canBeRecalculated)
            {
                amount = (int32)(1000.0f * powf(1.15f, aurEff->GetTickNumber()));
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_infection_AuraScript::HandleTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_lich_king_infection_AuraScript::OnCalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_infection_AuraScript();
        }
};

class spell_lich_king_valkyr_summon : public SpellScriptLoader
{
    public:
        spell_lich_king_valkyr_summon() : SpellScriptLoader("spell_lich_king_valkyr_summon") { } //74361


        class spell_lich_king_valkyr_summon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_valkyr_summon_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    uint32 spawnMod = caster->GetMap()->GetSpawnMode();
                    GetTargetApplication()->GetBase()->SetDuration(spawnMod == 1 || spawnMod == 3 ? 3000 : 1000);
                }
            }

            void HandleTriggerSpell(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                if (Unit* caster = GetCaster())
                {
                    uint32 triggerSpellId = GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                    float x, y, z;
                    caster->GetPosition(x, y, z);
                    caster->CastSpell(x, y, z + 6, triggerSpellId, true, NULL, NULL, GetCasterGUID(), caster);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_valkyr_summon_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                OnEffectApply += AuraEffectApplyFn(spell_lich_king_valkyr_summon_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_valkyr_summon_AuraScript();
        }
};

class spell_lich_king_vile_spirit_summon : public SpellScriptLoader
{
    public:
        spell_lich_king_vile_spirit_summon() : SpellScriptLoader("spell_lich_king_vile_spirit_summon") { }


        class spell_lich_king_vile_spirit_summon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_vile_spirit_summon_AuraScript);

            void OnPeriodic(AuraEffect const*aurEff)
            {
                PreventDefaultAction();
                Unit *caster = GetCaster();
                if (!caster || !caster->isAlive())
                    return;
                InstanceScript *instance = caster->GetInstanceScript();
                if (instance)
                {
                    uint32 spawnMod = caster->GetMap()->GetSpawnMode();
                    uint32 maxSummoned;
                    if (spawnMod == 1 || spawnMod == 3)
                        maxSummoned = 10;
                    else
                        maxSummoned = 8;
                    if (aurEff->GetTickNumber() >= maxSummoned)
                        return;
                }
                Position pos;
                caster->GetRandomNearPosition(pos, 13.0f);
                pos.m_positionZ = npc_vile_spirit_icc::Z_VILE_SPIRIT;
                uint32 triggeredSpell = aurEff->GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                caster->CastSpell(pos.m_positionX, pos.m_positionY, pos.m_positionZ, triggeredSpell, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_vile_spirit_summon_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_vile_spirit_summon_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_LINKED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_vile_spirit_summon_AuraScript();
        }
};

class spell_lich_king_vile_spirit_summon_visual : public SpellScriptLoader
{
    public:
        spell_lich_king_vile_spirit_summon_visual() : SpellScriptLoader("spell_lich_king_vile_spirit_summon_visual") { }


        class spell_lich_king_vile_spirit_summon_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_vile_spirit_summon_visual_AuraScript);

            void OnPeriodic(AuraEffect const*aurEff)
            {
                PreventDefaultAction();
                Unit *caster = GetCaster();
                if (!caster || !caster->isAlive())
                    return;
                Position pos;
                caster->GetRandomNearPosition(pos, 13.0f);
                pos.m_positionZ = npc_vile_spirit_icc::Z_VILE_SPIRIT;
                uint32 triggeredSpell = aurEff->GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                caster->CastSpell(pos.m_positionX, pos.m_positionY, pos.m_positionZ, triggeredSpell, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_vile_spirit_summon_visual_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_vile_spirit_summon_visual_AuraScript();
        }
};


class spell_lich_king_winter : public SpellScriptLoader
{
    public:
        spell_lich_king_winter() : SpellScriptLoader("spell_lich_king_winter") { } //68981


        class spell_lich_king_winter_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_winter_AuraScript)

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (UnitAI *pBossAI = caster->GetAI())
                        pBossAI->DoAction(ACTION_CANCEL_ALL_TRANSITION_EVENTS);
                    caster->CastSpell(caster, SPELL_QUAKE, true);
                    DoScriptText(SAY_BROKEN_ARENA, caster);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_winter_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_winter_AuraScript();
        }
};

class spell_lich_king_quake : public SpellScriptLoader
{
    public:
        spell_lich_king_quake() : SpellScriptLoader("spell_lich_king_quake") { } //72262


        class spell_lich_king_quake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_quake_AuraScript)

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if(!GetCaster() || !GetOwner())
                    return;

                GetTargetApplication()->GetBase()->GetCaster()->CastSpell(GetTargetApplication()->GetBase()->GetCaster(), SPELL_WMO_DAMAGE, true);
                if (InstanceScript* instance = GetTarget()->GetInstanceScript())
                    if (Creature* lichKing = Unit::GetCreature(*GetTarget(), instance->GetData64(GUID_LICH_KING)))
                        lichKing->AI()->DoAction(ACTION_PHASESWITCH_2);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_quake_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_quake_AuraScript();
        }
};

class spell_vile_spirit_distance_check : public SpellScriptLoader
{
    public:
        spell_vile_spirit_distance_check() : SpellScriptLoader("spell_vile_spirit_distance_check") { }


        class spell_vile_spirit_distance_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_vile_spirit_distance_check_SpellScript);


            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if(Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, SPELL_BURST, true);

                    if (InstanceScript* instance = caster->GetInstanceScript())
                        instance->SetData(DATA_NECK_DEEP_ACHIEVEMENT, FAIL);
                    caster->GetAI()->DoAction(ACTION_DESPAWN);
                }
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_vile_spirit_distance_check_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_vile_spirit_distance_check_SpellScript();
        }
};

class spell_ice_burst_distance_check : public SpellScriptLoader
{
    public:
        spell_ice_burst_distance_check() : SpellScriptLoader("spell_ice_burst_distance_check") { }


        class spell_ice_burst_distance_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ice_burst_distance_check_SpellScript);


            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if (GetHitUnit()->GetTypeId() != TYPEID_PLAYER)
                    return;

                if(Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_ICE_BURST, true);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_ice_burst_distance_check_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ice_burst_distance_check_SpellScript();
        }
};

class spell_lich_king_play_movie : public SpellScriptLoader
{
    public:
        spell_lich_king_play_movie() : SpellScriptLoader("spell_lich_king_play_movie") { }


        class spell_lich_king_play_movie_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lich_king_play_movie_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if (GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
                    GetHitUnit()->ToPlayer()->SendMovieStart(MOVIE_ID_ARTHAS_DEATH);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_lich_king_play_movie_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lich_king_play_movie_SpellScript();
        }
};

class spell_valkyr_carry_can_cast : public SpellScriptLoader
{
    public:
        spell_valkyr_carry_can_cast() : SpellScriptLoader("spell_valkyr_carry_can_cast") { }


        class spell_valkyr_carry_can_cast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_valkyr_carry_can_cast_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit *caster = GetCaster(), *target = GetHitUnit();
                if (!(target && target->isAlive() && caster))
                    return;

                if (target->GetTypeId() == TYPEID_PLAYER)
                {
                    target->CastSpell(caster, SPELL_VALKYR_GRAB_PLAYER, true);
                }
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_valkyr_carry_can_cast_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_valkyr_carry_can_cast_SpellScript();
        }
};

class spell_valkyr_target_search : public SpellScriptLoader
{
    public:
        spell_valkyr_target_search() : SpellScriptLoader("spell_valkyr_target_search") { }


        class spell_valkyr_target_search_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_valkyr_target_search_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit *target = GetHitUnit(), *caster = GetCaster();
                if (!(target && target->isAlive() && caster))
                    return;

                if (target->GetTypeId() == TYPEID_PLAYER)
                {
                    caster->CastSpell(target, SPELL_VALKYR_CHARGE, true);
                    caster->CastSpell(target, SPELL_VALKYR_CARRY_CAN_CAST, true);
                }
            }

            void FilterTargets(std::list<Unit*>& unitList)
            {
                LeaveOnlyPlayers(unitList);
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_valkyr_target_search_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_AREA_ENEMY_SRC);
                OnEffect += SpellEffectFn(spell_valkyr_target_search_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_valkyr_target_search_SpellScript();
        }
};

class spell_valkyr_eject_passenger : public SpellScriptLoader
{
    public:
        spell_valkyr_eject_passenger() : SpellScriptLoader("spell_valkyr_eject_passenger") { }


        class spell_valkyr_eject_passenger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_valkyr_eject_passenger_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetCaster() && GetCaster()->IsVehicle()))
                    return;

                if (Vehicle* vehicle = GetCaster()->GetVehicleKit())
                    vehicle->RemoveAllPassengers();
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_valkyr_eject_passenger_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_valkyr_eject_passenger_SpellScript();
        }
};

class spell_vile_spirit_target_search : public SpellScriptLoader
{
    public:
        spell_vile_spirit_target_search() : SpellScriptLoader("spell_vile_spirit_target_search") { }


        class spell_vile_spirit_target_search_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_vile_spirit_target_search_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive() && GetCaster()))
                    return;

                if (GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
                    GetCaster()->AddThreat(GetHitUnit(), 1000.0f);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_vile_spirit_target_search_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_vile_spirit_target_search_SpellScript();
        }
};

class npc_shambling_horror_icc: public CreatureScript
{
    enum eEvents
    {
        EVENT_ENRAGE = 1,
        EVENT_SHOCKWAVE
    };
    enum eSpells
    {
        SPELL_SHOCKWAVE                  = 72149,
        SPELL_ENRAGE                     = 72143,
        SPELL_FRENZY                     = 28747
    };
public:
    npc_shambling_horror_icc(): CreatureScript("npc_shambling_horror_icc") { }
    struct npc_shambling_horror_iccAI: public ScriptedAI
    {
        npc_shambling_horror_iccAI(Creature *creature): ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }
        
        void EnterCombat(Unit* who)
        {
            events.Reset();
            events.ScheduleEvent(EVENT_ENRAGE, 15000);
            events.ScheduleEvent(EVENT_SHOCKWAVE, 10000);
            isFrenzied = false;
        }

        void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
        {
            if (IsHeroic())
                if (HealthBelowPct(20) && !isFrenzied)
                {
                    isFrenzied = true;
                    DoCast(me, SPELL_FRENZY);
                }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ENRAGE:
                    {
                        DoCast(me, SPELL_ENRAGE);
                        events.ScheduleEvent(EVENT_ENRAGE, urand(20000, 30000));
                        break;
                    }
                    case EVENT_SHOCKWAVE:
                    {
                        DoCast(me->getVictim(), SPELL_SHOCKWAVE);
                        events.ScheduleEvent(EVENT_SHOCKWAVE, 20000);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        bool isFrenzied;
        InstanceScript *instance;
        EventMap events;
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_shambling_horror_iccAI(creature);
    }
};

class npc_raging_spirit_icc: public CreatureScript
{
    enum eEvents
    {
        EVENT_SOUL_SHRIEK = 1
    };
    enum eSpells
    {
        SPELL_SOUL_SHRIEK                = 69242
    };
public:
    npc_raging_spirit_icc(): CreatureScript("npc_raging_spirit_icc") { }
    struct npc_raging_spirit_iccAI: public ScriptedAI
    {
        npc_raging_spirit_iccAI(Creature *creature): ScriptedAI(creature) { }
        
        void EnterCombat(Unit* who)
        {
            events.Reset();
            events.ScheduleEvent(EVENT_SOUL_SHRIEK, 15000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SOUL_SHRIEK:
                    {
                        DoCast(me->getVictim(), SPELL_SOUL_SHRIEK);
                        events.ScheduleEvent(EVENT_SOUL_SHRIEK, 10000);
                        break;
                    }
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_raging_spirit_iccAI(creature);
    }
};

class spell_lich_king_tirion_mass_resurrection : public SpellScriptLoader
{
    public:
        spell_lich_king_tirion_mass_resurrection() : SpellScriptLoader("spell_lich_king_tirion_mass_resurrection") { } 

        class spell_lich_king_tirion_mass_resurrection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lich_king_tirion_mass_resurrection_SpellScript)

            //void MassResurrect(SpellEffIndex effIndex)
            //{
            //    PreventHitDefaultEffect(effIndex);
            //    InstanceScript *instance = GetCaster()->GetInstanceScript();
            //    if (!instance)
            //        return;
            //    instance->DoCastSpellOnPlayers(SPELL_REVIVE_EFFECT);
            //}

            void FilterTargets(std::list<Unit*>& unitList)
            {
                unitList.clear();
                const Map::PlayerList &PlayerList = GetCaster()->GetMap()->GetPlayers();

                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* player = i->getSource())
                            unitList.push_back(player);
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_lich_king_tirion_mass_resurrection_SpellScript::FilterTargets, EFFECT_0, TARGET_SRC_CASTER);
                //OnEffect += SpellEffectFn(spell_lich_king_tirion_mass_resurrection_SpellScript::MassResurrect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lich_king_tirion_mass_resurrection_SpellScript();
        }
};

class npc_ice_sphere_icc : public CreatureScript
{
    enum eEvents
    {
        EVENT_MOVE_FORWARD = 1
    };
public:
    npc_ice_sphere_icc() : CreatureScript("npc_ice_sphere_icc") { }

    struct npc_ice_sphere_iccAI : public ScriptedAI
    {
        npc_ice_sphere_iccAI(Creature* creature) : ScriptedAI(creature), m_victimGuid(0) { }

        void Reset()
        {
            events.ScheduleEvent(EVENT_MOVE_FORWARD, 2000);
            SetCombatMovement(false);
            me->SetReactState(REACT_PASSIVE);
        }

        void SetGUID(const uint64& guid, int32 type = 0)
        {
            if (type == TYPE_VICTIM)
                m_victimGuid = guid;
        }

        void JustDied(Unit* /*pKiller*/)
        {
            if (Unit *victim = ObjectAccessor::GetUnit(*me, m_victimGuid))
                victim->RemoveAurasDueToSpell(SPELL_ICE_PULSE, me->GetGUID());
            me->DespawnOrUnsummon();
        }

        void KilledUnit(Unit* victim)
        {
            victim->RemoveAurasDueToSpell(SPELL_ICE_PULSE, me->GetGUID());
        }

        void UpdateAI(const uint32 uiDiff)
        {
            events.Update(uiDiff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_FORWARD:
                    {
                        if (Unit *pVictim = ObjectAccessor::GetUnit(*me, m_victimGuid))
                            if (pVictim->isAlive() && pVictim->isTargetableForAttack())
                            {
                                me->SetFacingToObject(pVictim);
                                pVictim->GetPosition(&m_victimPos);
                                me->GetPosition(&m_newPos);
                                me->MovePosition(m_newPos, 0.20f, 0.0f);
                                me->SetPosition(m_newPos);
                            }
                            else
                            {
                                if (Player *newVictim = SelectRandomPlayerInTheMap(me->GetMap()))
                                {
                                    m_victimGuid = newVictim->GetGUID();
                                    AttackStart(newVictim);
                                    me->CastSpell(newVictim, SPELL_ICE_PULSE, true);
                                }
                                else
                                    me->DespawnOrUnsummon();
                            }
                        events.ScheduleEvent(EVENT_MOVE_FORWARD, 100);
                    }
                    default:
                        break;
                }
            }
        }
    private:
        EventMap events;
        Position m_victimPos, m_newPos;
        uint64 m_victimGuid;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ice_sphere_iccAI(creature);
    }
};

class npc_defile_icc : public CreatureScript
{

public:
    npc_defile_icc() : CreatureScript("npc_defile_icc") { }

    struct npc_defile_iccAI : public Scripted_NoMovementAI
    {
        npc_defile_iccAI(Creature* creature) : Scripted_NoMovementAI(creature), alreadyReset(false), m_hitNumber(0)
        {
        }

        void JustRespawned()
        {
            me->SetInCombatWithZone();
            me->SetReactState(REACT_PASSIVE);
        }

        //void UpdateDefileAura()
        //{
        //    ++m_hitNumber;
        //    m_radiusMod = (int32)(((float)m_hitNumber / 60) * 0.9f + 0.1f) * 10000 * 2 / 3;
        //    if (SpellEntry const* defileAuraSpellEntry = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE), me))
        //        me->CastCustomSpell(defileAuraSpellEntry->Id, SPELLVALUE_RADIUS_MOD, m_radiusMod, me, true, NULL, NULL, me->GetGUID()); 
        //}

        void Reset()
        {
            if (!alreadyReset)
            {
                if (SpellEntry const* defileAuraSpellEntry = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE), me))
                    DoCast(me, defileAuraSpellEntry->Id, true);
                //UpdateDefileAura();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                alreadyReset = true;
            }
        }

        //void SpellHitTarget(Unit* target, SpellEntry const* spell)
        //{
        //    if (spell->Id == SPELL_DEFILE_DAMAGE)
        //    {
        //        AttackStart(target);
        //        if (SpellEntry const* defileIncrease = sSpellMgr->GetSpellForDifficultyFromSpell(sSpellStore.LookupEntry(SPELL_DEFILE_INCREASE), me))
        //            DoCast(me, defileIncrease->Id, true);
        //        UpdateDefileAura();
        //    }
        //}
        //Disallow melee
        void UpdateAI(const uint32 uiDiff) {}
    private:
        bool alreadyReset;
        int32 m_hitNumber, m_radiusMod;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_defile_iccAI(creature);
    }
};

void AddSC_boss_lichking()
{
    new boss_the_lich_king();
    new npc_tirion_icc();
    new npc_valkyr_icc();
    new npc_vile_spirit_icc();
    new npc_shambling_horror_icc();
    new npc_raging_spirit_icc();
    new npc_ice_sphere_icc();
    new npc_defile_icc();
    new spell_lich_king_necrotic_plague();
    new spell_lich_king_infection();
    new spell_lich_king_valkyr_summon();
    new spell_lich_king_vile_spirit_summon();
    new spell_lich_king_vile_spirit_summon_visual();
    new spell_lich_king_winter();
    new spell_vile_spirit_distance_check();
    //new spell_lich_king_pain_and_suffering();
    new spell_lich_king_pain_and_suffering_effect();
    new spell_ice_burst_distance_check();
    new spell_lich_king_quake();
    new spell_lich_king_play_movie();
    new spell_valkyr_carry_can_cast();
    new spell_valkyr_target_search();
    new spell_valkyr_eject_passenger();
    new spell_vile_spirit_target_search();
    new spell_lich_king_defile();
    new spell_lich_king_tirion_mass_resurrection();
}