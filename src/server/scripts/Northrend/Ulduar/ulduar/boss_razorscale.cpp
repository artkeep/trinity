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

/* ScriptData
SDName: Razorscale
SDAuthor: PrinceCreed
SD%Complete: 100
EndScriptData */

#include "ScriptPCH.h"
#include "ulduar.h"

#define SAY_GREET             -1603260
#define SAY_AGGRO_1           "Give us a moment to prepare to build the turrets."
#define SAY_AGGRO_2           "Be on the lookout! Mole machines will be surfacing soon with those nasty Iron dwarves aboard!"
#define SAY_AGGRO_3           "Ready to move out, keep those dwarves off of our backs!"
#define SAY_GROUND_PHASE      -1603261
#define SAY_TURRETS           "Fires out! Let's rebuild those turrets!"

#define GOSSIP_ITEM_1         "Activate Harpoones!"
#define EMOTE_HARPOON         "Harpoon Turret is ready for use!"
#define EMOTE_BREATH          "Razorscale takes a deep breath..."
#define EMOTE_PERMA           "Razorscale grounded permanently!"

enum Spells
{
    SPELL_FLAMEBUFFET             = 64016,
    SPELL_FIREBALL_10             = 62796,
    SPELL_FIREBALL_25             = 63815,
    SPELL_FLAME_GROUND_10         = 64709,
    SPELL_FLAME_GROUND_25         = 64734,
    SPELL_WINGBUFFET              = 62666,
    SPELL_FLAMEBREATH_10          = 63317,
    SPELL_FLAMEBREATH_25          = 64021,
    SPELL_FUSEARMOR               = 64771,
    SPELL_DEVOURINGFLAME_10       = 63236,
    SPELL_DEVOURINGFLAME_25       = 64733,
    SPELL_HARPOON_VISUAL          = 49682,  // not correct
    SPELL_HARPOON_1               = 63659,
    SPELL_HARPOON_2               = 63658,
    SPELL_HARPOON_3               = 63657,
    SPELL_HARPOON_4               = 63524,
    SPELL_HARPOON_BUFF            = 62509,
    SPELL_FLAMED                  = 62696,
    SPELL_STUN                    = 9032,
    SPELL_BERSERK                 = 47008
};

const float harpoonPosition[4][3] =
{
    {594.317f, -136.953f, 391.516998f},
    {577.449f, -136.953f, 391.517151f},
    {607.613f, -134.207f, 391.297f},
    {559.232f, -138.104f, 391.517f},
};

const Position posEng1 = {590.442322f, -130.550278f, 391.516998f, 4.789456f};
const Position posEng2 = {574.850159f, -133.687439f, 391.517151f, 4.252456f};
 
const Position pos1 = {614.975403f, -155.138214f, 391.517090f, 4.154516f};
const Position pos2 = {609.814331f, -204.968185f, 391.517090f, 5.385465f};
const Position pos3 = {563.530884f, -201.557312f, 391.517090f, 4.108194f};
const Position pos4 = {560.231260f, -153.677198f, 391.517090f, 5.402720f};

const Position RazorFlight = {588.050293f, -251.191223f, 470.535980f, 1.605303f};
const Position RazorGround = {586.966f, -175.534f, 390.516998f, 1.691704f};
const Position WATCHER1 = {629.309f, -197.959f, 391.516f, 1.691704f};
const Position WATCHER2 = {561.045f, -214.221f, 391.516f, 1.691704f};
const Position WATCHER3 = {541.707f, -166.307f, 391.516f, 1.691704f};

enum Mobs
{
    RAZORSCALE                    = 33186,
    NPC_DARK_RUNE_GUARDIAN        = 33388,
    NPC_DARK_RUNE_SENTINEL        = 33846,
    NPC_DARK_RUNE_WATCHER         = 33453,
    MOLE_MACHINE_TRIGGER          = 33245,
    NPC_ENGINEER                  = 33287,
    NPC_DEFENDER                  = 33816,
    NPC_HARPOON                   = 33184,
    MOLE_MACHINE_GOB              = 194316
};

enum DarkRuneSpells
{
    SPELL_CHAIN_LIGHTNING_10      = 64758,
    SPELL_CHAIN_LIGHTNING_25      = 64759,
    SPELL_LIGHTNING_BOLT_10       = 63809,
    SPELL_LIGHTNING_BOLT_25       = 64696,

    SPELL_STORMSTRIKE             = 64757,

    SPELL_BATTLE_SHOUT_10         = 46763,
    SPELL_BATTLE_SHOUT_25         = 64062,
    SPELL_HEROIC_STRIKE           = 45026,
    SPELL_WHIRLWIND_10            = 63807,
    SPELL_WHIRLWIND_25            = 63808,
};

#define ACHIEVEMENT_QUICK_SHAVE     RAID_MODE(2919, 2921)

#define ACTION_EVENT_START          1
#define ACTION_GROUND_PHASE         2

enum Phases
{
    PHASE_NULL = 0,
    PHASE_PERMAGROUND,
    PHASE_GROUND,
    PHASE_FLIGHT
};

enum Events
{
    EVENT_NONE,
    EVENT_BERSERK,
    EVENT_BREATH,
    EVENT_BUFFET,
    EVENT_HARPOON,
    EVENT_FIREBALL,
    EVENT_FLIGHT,
    EVENT_DEVOURING,
    EVENT_FLAME,
    EVENT_LAND,
    EVENT_GROUND,
    EVENT_FUSE,
    EVENT_SUMMON
};

class boss_razorscale : public CreatureScript
{
public:
    boss_razorscale() : CreatureScript("boss_razorscale") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_razorscaleAI (pCreature);
    }

    struct boss_razorscaleAI : public BossAI
    {
        boss_razorscaleAI(Creature *pCreature) : BossAI(pCreature, TYPE_RAZORSCALE)
            , phase(PHASE_NULL)
        {
            // Do not let Razorscale be affected by Battle Shout buff
            me->ApplySpellImmune(0, IMMUNITY_ID, RAID_MODE(SPELL_BATTLE_SHOUT_10, SPELL_BATTLE_SHOUT_25), true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 49560, true); // Death Grip jump effect
        }

        Phases phase;

        uint32 EnrageTimer;
        uint32 FlyCount;

        uint8 hitCount;
        uint8 currentHarpoon;

        //uint64 Harpoon[4];
        bool PermaGround;
        bool Enraged;

        void Reset()
        {
            _Reset();
            /*for(uint8 i = 0 ; i < RAID_MODE(2, 4) ; i++)
                if (Creature* Harp = Unit::GetCreature(*me, Harpoon[i]))
                {
                    Harp->DisappearAndDie();
                    Harpoon[i] = 0;
                }*/
            currentHarpoon = 0;
            PermaGround = false;
            Enraged = false;
            hitCount = 0;
            me->SetFlying(true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        void EnterCombat(Unit* who)
        {
            _EnterCombat();
            me->SetSpeed(MOVE_FLIGHT, 3.0f, true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
            phase = PHASE_GROUND;
            events.SetPhase(PHASE_GROUND);
            FlyCount = 0;
            EnrageTimer = 15*60*1000; // Enrage in 15 min
            Enraged = false;
            events.ScheduleEvent(EVENT_FLIGHT, 0, 0, PHASE_GROUND);
            DoZoneInCombat();
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
        }

        void JustDied(Unit* Killer)
        {
            _JustDied();

            // Achievements
            if (instance)
            {
                // A Quick Shave
                if (FlyCount <= 2)
                    instance->DoCompleteAchievement(ACHIEVEMENT_QUICK_SHAVE);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (HealthBelowPct(50) && !PermaGround)
                EnterPermaGround();

            if (EnrageTimer <= diff && !Enraged)
            {
                DoCast(me, SPELL_BERSERK);
                Enraged = true;
            }
            else EnrageTimer -= diff;

            if (phase == PHASE_GROUND)
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_FLIGHT:
                            phase = PHASE_FLIGHT;
                            events.SetPhase(PHASE_FLIGHT);
                            me->SetFlying(true);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->RemoveAllAuras();
                            me->GetMotionMaster()->MovePoint(1,RazorFlight);
                            events.ScheduleEvent(EVENT_FIREBALL, 7000, 0, PHASE_FLIGHT);
                            events.ScheduleEvent(EVENT_DEVOURING, 10000, 0, PHASE_FLIGHT);
                            events.ScheduleEvent(EVENT_SUMMON, 5000, 0, PHASE_FLIGHT);
                            events.ScheduleEvent(EVENT_GROUND, 50000, 0, PHASE_FLIGHT);
                            ++FlyCount;
                            return;
                        case EVENT_LAND:
                            me->SetFlying(false);
                            me->CastSpell(me, SPELL_STUN, true);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            if (Creature *pCommander = me->GetCreature(*me, instance->GetData64(DATA_EXP_COMMANDER)))
                                pCommander->AI()->DoAction(ACTION_GROUND_PHASE);
                            //events.ScheduleEvent(EVENT_HARPOON, 0, 0, PHASE_GROUND);
                            events.ScheduleEvent(EVENT_BREATH, 30000, 0, PHASE_GROUND);
                            events.ScheduleEvent(EVENT_BUFFET, 33000, 0, PHASE_GROUND);
                            events.ScheduleEvent(EVENT_FLIGHT, 35000, 0, PHASE_GROUND);
                            return;
                        case EVENT_BREATH:
                            me->MonsterTextEmote(EMOTE_BREATH, 0, true);
                            DoCastAOE(RAID_MODE(SPELL_FLAMEBREATH_10, SPELL_FLAMEBREATH_25));
                            //events.CancelEvent(EVENT_HARPOON);
                            events.CancelEvent(EVENT_BREATH);
                            return;
                        case EVENT_BUFFET:
                            if (instance)
                                instance->SetData(TYPE_HARPOON, DONE);
                            /*for(uint8 i = 0 ; i < RAID_MODE(2, 4) ; i++)
                                if (Creature* Harp = Unit::GetCreature(*me, Harpoon[i]))
                                {
                                    Harp->CastSpell(Harp, SPELL_FLAMED, true);
                                    Harp->DisappearAndDie();
                                }*/
                            currentHarpoon = 0;
                            DoCastAOE(SPELL_WINGBUFFET);
                            events.CancelEvent(EVENT_BUFFET);
                            return;
                    }
                }
            }
            if (phase == PHASE_PERMAGROUND)
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_FLAME:
                            DoCastAOE(SPELL_FLAMEBUFFET);
                            events.ScheduleEvent(EVENT_FLAME, 10000, 0, PHASE_PERMAGROUND);
                            return;
                        case EVENT_BREATH:
                            me->MonsterTextEmote(EMOTE_BREATH, 0, true);
                            DoCastVictim(RAID_MODE(SPELL_FLAMEBREATH_10, SPELL_FLAMEBREATH_25));
                            events.ScheduleEvent(EVENT_BREATH, 20000, 0, PHASE_PERMAGROUND);
                            return;
                        case EVENT_FIREBALL:
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                                DoCast(pTarget, RAID_MODE(SPELL_FIREBALL_10, SPELL_FIREBALL_25));
                            events.ScheduleEvent(EVENT_FIREBALL, 3000, 0, PHASE_PERMAGROUND);
                            return;
                        case EVENT_DEVOURING:
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                                DoCast(pTarget, SPELL_DEVOURINGFLAME_10);
                            events.ScheduleEvent(EVENT_DEVOURING, 10000, 0, PHASE_PERMAGROUND);
                            return;
                        case EVENT_BUFFET:
                            DoCastAOE(SPELL_WINGBUFFET);
                            events.CancelEvent(EVENT_BUFFET);
                            return;
                        case EVENT_FUSE:
                            DoCastVictim(SPELL_FUSEARMOR);
                            events.ScheduleEvent(EVENT_FUSE, 10000, 0, PHASE_PERMAGROUND);
                            return;
                    }
                }

                DoMeleeAttackIfReady();
            }
            else
            {
                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_GROUND:
                            if (currentHarpoon < RAID_MODE(2, 4))
                            {
                                if (instance)
                                    instance->SetData(TYPE_HARPOON, IN_PROGRESS);
                                /*if (Creature* Harp = me->SummonCreature(NPC_HARPOON, harpoonPosition[currentHarpoon][0], harpoonPosition[currentHarpoon][1], harpoonPosition[currentHarpoon][2], 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 0))
                                {
                                    Harpoon[currentHarpoon] = Harp->GetGUID();
                                    Harp->MonsterTextEmote(EMOTE_HARPOON, 0, true);
                                }*/
                                me->MonsterTextEmote(EMOTE_HARPOON, 0, true);
                                currentHarpoon++;
                                events.ScheduleEvent(EVENT_GROUND, 30000, 0, PHASE_FLIGHT);
                            }
                            return;
                        case EVENT_FIREBALL:
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                                DoCast(pTarget, RAID_MODE(SPELL_FIREBALL_10, SPELL_FIREBALL_25));
                            events.ScheduleEvent(EVENT_FIREBALL, 3000, 0, PHASE_FLIGHT);
                            return;
                        case EVENT_DEVOURING:
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                                DoCast(pTarget, SPELL_DEVOURINGFLAME_10);
                            events.ScheduleEvent(EVENT_DEVOURING, 10000, 0, PHASE_FLIGHT);
                            return;
                        case EVENT_SUMMON:
                            SummonAdds();
                            events.ScheduleEvent(EVENT_SUMMON, 40000, 0, PHASE_FLIGHT);
                            return;
                    }
                }
            }
        }

        void SpellHit(Unit *caster, const SpellEntry *spell)
        {
            if(spell->Id == SPELL_HARPOON_1
                || spell->Id == SPELL_HARPOON_2
                || spell->Id == SPELL_HARPOON_3
                || spell->Id == SPELL_HARPOON_4)
            {
                DoCast(SPELL_HARPOON_BUFF);
                hitCount++ ;
                if (hitCount == RAID_MODE(2, 4))
                {
                    WorldPacket heart;
                    me->BuildHeartBeatMsg(&heart);
                    me->SendMessageToSet(&heart, false);
                    me->GetMotionMaster()->MovePoint(100, RazorGround);
                    phase = PHASE_GROUND;
                    events.SetPhase(PHASE_GROUND);
                    events.ScheduleEvent(EVENT_LAND, 10000, 0, PHASE_GROUND);
                    hitCount = 0;
                }
            }
        }

        void MovementInform(uint32 uiType, uint32 uiId)
        {
            if (uiType != POINT_MOTION_TYPE)
                return;

            if (uiId == 100)
                events.RescheduleEvent(EVENT_LAND, 500, 0, PHASE_GROUND);
        }

        void EnterPermaGround()
        {
            me->MonsterTextEmote(EMOTE_PERMA, 0, true);
            phase = PHASE_PERMAGROUND;
            events.SetPhase(PHASE_PERMAGROUND);
            me->SetFlying(false);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            me->RemoveAurasDueToSpell(SPELL_STUN);
            me->SetSpeed(MOVE_FLIGHT, 1.0f, true);
            PermaGround = true;
            DoCastAOE(RAID_MODE(SPELL_FLAMEBREATH_10, SPELL_FLAMEBREATH_25));
            events.ScheduleEvent(EVENT_FLAME, 15000, 0, PHASE_PERMAGROUND);
            events.RescheduleEvent(EVENT_DEVOURING, 15000, 0, PHASE_PERMAGROUND);
            events.RescheduleEvent(EVENT_BREATH, 20000, 0, PHASE_PERMAGROUND);
            events.RescheduleEvent(EVENT_FIREBALL, 3000, 0, PHASE_PERMAGROUND);
            events.RescheduleEvent(EVENT_DEVOURING, 6000, 0, PHASE_PERMAGROUND);
            events.RescheduleEvent(EVENT_BUFFET, 2500, 0, PHASE_PERMAGROUND);
            events.RescheduleEvent(EVENT_FUSE, 5000, 0, PHASE_PERMAGROUND);
        }

        void SummonAdds()
        {
            // Adds will come in waves from mole machines. One mole can spawn a Dark Rune Watcher
            // with 1-2 Guardians, or a lone Sentinel. Up to 4 mole machines can spawn adds at any given time.
            uint8 random = RAID_MODE(2, 3);
            for (uint8 i = 0; i < random; ++i)
            {
                float x = irand(540, 640);   // Safe range is between 500 and 650
                float y = irand(-230, -195); // Safe range is between -235 and -145
                float z = 391.5f;                  // Ground level
                me->SummonCreature(MOLE_MACHINE_TRIGGER, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
            }
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_EVENT_START:
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoZoneInCombat();
                    break;
            }
        }
    };

};

/*====================================================================================
====================================================================================*/

class npc_expedition_commander : public CreatureScript
{
public:
    npc_expedition_commander() : CreatureScript("npc_expedition_commander") { }

    struct npc_expedition_commanderAI : public ScriptedAI
    {
        npc_expedition_commanderAI(Creature* pCreature) : ScriptedAI(pCreature), summons(me)
        {
            pInstance = pCreature->GetInstanceScript();
            greet = false;
        }

        InstanceScript* pInstance;
        SummonList summons;

        uint32 uiTimer;
        bool greet;
        uint64 m_uiCommanderGUID;
        uint8  uiPhase;
        uint64 engineer[2];
        uint64 defender[4];

        void Initialize()
        {
            uiTimer =0;
            uiPhase = 0;
            engineer[0] = 0;
            engineer[1] = 0;
            defender[0] = 0;
            defender[1] = 0;
            defender[2] = 0;
            defender[3] = 0;
        }

        void Reset()
        {
            uiTimer = 0;
            greet = false;
        }

        void MoveInLineOfSight(Unit *who)
        {
            if (!greet && me->IsWithinDistInMap(who, 10.0f) && who->GetTypeId() == TYPEID_PLAYER)
            {
                DoScriptText(SAY_GREET, me);
                greet = true;
            }
            ScriptedAI::MoveInLineOfSight(who);
        }

        void MovementInform(uint32 uiType, uint32 uiId)
        {
            if (uiType != POINT_MOTION_TYPE)
                return;
        }

        void JustSummoned(Creature *summon)
        {
            summons.Summon(summon);
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
                case ACTION_GROUND_PHASE:
                    DoScriptText(SAY_GROUND_PHASE, me);
                    break;
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            ScriptedAI::UpdateAI(uiDiff);
            if (uiTimer <= uiDiff)
            {
                switch(uiPhase)
                {
                    case 0:
                        break;
                    case 1:
                        if (pInstance)
                            pInstance->SetData(TYPE_RAZORSCALE, IN_PROGRESS);
                        summons.DespawnAll();
                        uiTimer = 1000;
                        uiPhase = 2;
                        break;
                    case 2:
                        if (Creature* eng = me->SummonCreature(NPC_ENGINEER, 591.951477f, -95.968292f, 391.516998f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        {
                            engineer[0] = eng->GetGUID();
                            eng->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            eng->SetSpeed(MOVE_RUN, 0.5f);
                            eng->SetHomePosition(posEng1);
                            eng->GetMotionMaster()->MoveTargetedHome();
                        }
                        if (Creature* eng = me->SummonCreature(NPC_ENGINEER, 591.951477f, -95.968292f, 391.516998f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        {
                            engineer[1] = eng->GetGUID();
                            eng->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            eng->SetSpeed(MOVE_RUN, 0.5f);
                            eng->SetHomePosition(posEng2);
                            eng->GetMotionMaster()->MoveTargetedHome();
                            eng->MonsterYell(SAY_AGGRO_1, LANG_UNIVERSAL, 0);
                        }
                        uiPhase = 3;
                        uiTimer = 14000;
                        break;
                    case 3:
                        uiPhase = 4;
                        break;
                    case 4:
                        if(Creature* def = me->SummonCreature(NPC_DEFENDER, 600.75f, -104.85f, 391.516998f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        {
                            defender[0] = def->GetGUID();
                            def->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            def->SetHomePosition(pos1);
                            def->GetMotionMaster()->MoveTargetedHome();
                            def->SetReactState(REACT_AGGRESSIVE);
                        }
                        if(Creature* def = me->SummonCreature(NPC_DEFENDER, 596.38f, -110.26f, 391.516998f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        {
                            defender[1] = def->GetGUID();
                            def->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            def->SetHomePosition(pos2);
                            def->GetMotionMaster()->MoveTargetedHome();
                            def->SetReactState(REACT_AGGRESSIVE);
                        }
                        if(Creature* def = me->SummonCreature(NPC_DEFENDER, 566.47f, -103.63f, 391.516998f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        {
                            defender[2] = def->GetGUID();
                            def->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            def->SetHomePosition(pos3);
                            def->GetMotionMaster()->MoveTargetedHome();
                            def->SetReactState(REACT_AGGRESSIVE);
                        }
                        if(Creature* def = me->SummonCreature(NPC_DEFENDER, 570.41f, -108.79f, 391.516998f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000))
                        {
                            defender[3] = def->GetGUID();
                            def->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                            def->SetHomePosition(pos4);
                            def->GetMotionMaster()->MoveTargetedHome();
                            def->SetReactState(REACT_AGGRESSIVE);
                        }
                        uiPhase = 5;
                        break;
                    case 5:
                        if(Creature* eng = Unit::GetCreature(*me, engineer[0]))
                            eng->HandleEmoteCommand(EMOTE_STATE_USESTANDING);
                        if(Creature* eng = Unit::GetCreature(*me, engineer[1]))
                            eng->HandleEmoteCommand(EMOTE_STATE_USESTANDING);
                        for (uint8 i = 0 ; i < 4 ; i++)
                            if(Creature* def = Unit::GetCreature(*me, defender[i]))
                                def->HandleEmoteCommand(EMOTE_STATE_READY2H);
                        me->MonsterYell(SAY_AGGRO_2, LANG_UNIVERSAL, 0);
                        uiTimer = 16000;
                        uiPhase = 6;
                        break;
                    case 6:
                        if (Creature *pRazorscale = me->GetCreature(*me, pInstance->GetData64(DATA_RAZORSCALE)))
                            if (pRazorscale->AI())
                                pRazorscale->AI()->DoAction(ACTION_EVENT_START);
                        if(Creature* eng = Unit::GetCreature(*me, engineer[0]))
                            eng->MonsterYell(SAY_AGGRO_3, LANG_UNIVERSAL, 0);
                        uiPhase =7;
                        break;
                }
                if (!UpdateVictim())
                    return;
                    
                DoMeleeAttackIfReady();
            }
            else uiTimer -= uiDiff;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_expedition_commanderAI(pCreature);
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        InstanceScript *pInstance = pCreature->GetInstanceScript();
        
        if (pInstance && pPlayer && pInstance->GetData(TYPE_RAZORSCALE) != IN_PROGRESS && pInstance->GetData(TYPE_RAZORSCALE) != DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,GOSSIP_ITEM_1,GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF);
            pPlayer->SEND_GOSSIP_MENU(13853, pCreature->GetGUID());
        }else
            pPlayer->SEND_GOSSIP_MENU(13910, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF:
                if (pPlayer)
                    pPlayer->CLOSE_GOSSIP_MENU();
                CAST_AI(npc_expedition_commander::npc_expedition_commanderAI, (pCreature->AI()))->uiPhase = 1;
                break;
        }
        return true;
    }

};


class npc_devouring_flame : public CreatureScript
{
public:
    npc_devouring_flame() : CreatureScript("npc_devouring_flame") { }

    struct npc_devouring_flameAI : public ScriptedAI
    {
        npc_devouring_flameAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->SetReactState(REACT_PASSIVE);
            DoCast(me, 64704);
        }

        void Reset()
        {
            DoCast(me, RAID_MODE(SPELL_FLAME_GROUND_10, SPELL_FLAME_GROUND_25));
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new npc_devouring_flameAI(pCreature);
    }
};


class npc_darkrune_watcher : public CreatureScript
{
public:
    npc_darkrune_watcher() : CreatureScript("npc_darkrune_watcher") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_darkrune_watcherAI (pCreature);
    }

    struct npc_darkrune_watcherAI : public ScriptedAI
    {
        npc_darkrune_watcherAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* m_pInstance;

        uint32 ChainTimer;
        uint32 LightTimer;

        void Reset()
        {
            ChainTimer = urand(3000, 6000);
            LightTimer = urand(1000, 3000);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (ChainTimer <= uiDiff)
            {
                DoCast(RAID_MODE(SPELL_CHAIN_LIGHTNING_10, SPELL_CHAIN_LIGHTNING_25));
                ChainTimer = urand(6000, 9000);
            } else ChainTimer -= uiDiff;

            if (LightTimer <= uiDiff)
            {
                DoCast(RAID_MODE(SPELL_LIGHTNING_BOLT_10, SPELL_LIGHTNING_BOLT_25));
                LightTimer = urand(4000, 7000);
            } else LightTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

};


class npc_darkrune_guardian : public CreatureScript
{
public:
    npc_darkrune_guardian() : CreatureScript("npc_darkrune_guardian") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_darkrune_guardianAI (pCreature);
    }

    struct npc_darkrune_guardianAI : public ScriptedAI
    {
        npc_darkrune_guardianAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* m_pInstance;

        uint32 StormTimer;

        void Reset()
        {
            StormTimer = urand(3000, 6000);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (StormTimer <= uiDiff)
            {
                DoCast(SPELL_STORMSTRIKE);
                StormTimer = urand(4000, 8000);
            }
            else StormTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

};


class npc_darkrune_sentinel : public CreatureScript
{
public:
    npc_darkrune_sentinel() : CreatureScript("npc_darkrune_sentinel") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_darkrune_sentinelAI (pCreature);
    }

    struct npc_darkrune_sentinelAI : public ScriptedAI
    {
        npc_darkrune_sentinelAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* m_pInstance;

        uint32 HeroicTimer;
        uint32 WhirlTimer;
        uint32 ShoutTimer;

        void Reset()
        {
            HeroicTimer = urand(4000, 8000);
            WhirlTimer = urand(9000, 12000);
            ShoutTimer = urand(15000, 25000);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (HeroicTimer <= uiDiff)
            {
                DoCast(SPELL_HEROIC_STRIKE);
                HeroicTimer = urand(4000, 6000);
            }
            else HeroicTimer -= uiDiff;

            if (WhirlTimer <= uiDiff)
            {
                DoCast(RAID_MODE(SPELL_WHIRLWIND_10, SPELL_WHIRLWIND_25));
                WhirlTimer = urand(15000, 20000);
            } else WhirlTimer -= uiDiff;

            if (ShoutTimer <= uiDiff)
            {
                DoCast(me, RAID_MODE(SPELL_BATTLE_SHOUT_10, SPELL_BATTLE_SHOUT_25));
                ShoutTimer = urand(30000, 40000);
            }
            else ShoutTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

};

class mole_machine_trigger : public CreatureScript
{
public:
    mole_machine_trigger() : CreatureScript("mole_machine_trigger") { }

    struct mole_machine_triggerAI : public ScriptedAI
    {
        mole_machine_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = pCreature->GetInstanceScript();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_PACIFIED);
            me->SetVisible(false);
        }

        InstanceScript* m_pInstance;
        GameObject* MoleMachine;
        uint32 uiSummonTimer;

        void Reset()
        {
            MoleMachine = me->SummonGameObject(MOLE_MACHINE_GOB, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), urand(0,6), 0, 0, 0, 0, 300);
            if (MoleMachine)
                MoleMachine->SetGoState(GO_STATE_ACTIVE);
            uiSummonTimer = 6000;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (uiSummonTimer <= uiDiff)
            {
                float x = me->GetPositionX();
                float y = me->GetPositionY();
                float z = me->GetPositionZ();
                
                // One mole can spawn a Dark Rune Watcher with 1-2 Guardians, or a lone Sentinel
                if (!(rand()%2))
                {
                    me->SummonCreature(NPC_DARK_RUNE_WATCHER, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                    uint8 random = urand(1,2);
                    for (uint8 i = 0; i < random; ++i)
                        me->SummonCreature(NPC_DARK_RUNE_GUARDIAN, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                }
                else
                    me->SummonCreature(NPC_DARK_RUNE_SENTINEL, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                    
                uiSummonTimer = 15000;
            } 
            else uiSummonTimer -= uiDiff;
        }
        
        void JustSummoned(Creature *summon)
        {
            summon->AI()->DoAction(0);
            summon->AI()->DoZoneInCombat();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const    
    {
        return new mole_machine_triggerAI(pCreature);
    }
};

class npc_razorscale_harpoon : public CreatureScript
{
public:
    npc_razorscale_harpoon() : CreatureScript("npc_razorscale_harpoon") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        std::list<Creature*> RazorList;
        pCreature->GetCreatureListWithEntryInGrid(RazorList,NPC_RAZORSCALE,1000.0f);
        if (!RazorList.empty())
        {
            for (std::list<Creature*>::const_iterator itr = RazorList.begin(); itr != RazorList.end(); ++itr)
            {
                if (Creature* pRazor = *itr)
                {
                    pRazor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pCreature->CastSpell(pRazor, SPELL_HARPOON_1, true);
                    pRazor->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }
        }
        return true;
    }
};

class go_razorscale_harpoon : public GameObjectScript
{
public:
    go_razorscale_harpoon() : GameObjectScript("go_razorscale_harpoon") { }

    bool OnGossipHello(Player* pPlayer, GameObject* pGo)
    {
        std::list<Creature*> RazorList;
        pGo->GetCreatureListWithEntryInGrid(RazorList,NPC_RAZORSCALE,1000.0f);
        if (!RazorList.empty())
        {
            for (std::list<Creature*>::const_iterator itr = RazorList.begin(); itr != RazorList.end(); ++itr)
            {
                if (Creature* pRazor = *itr)
                {
                    pRazor->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pPlayer->CastSpell(pRazor, SPELL_HARPOON_1, true);
                    pRazor->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                }
            }
        }
        return true;
    }
};

void AddSC_boss_razorscale()
{
    new npc_expedition_commander;
    new boss_razorscale;
    new npc_devouring_flame;
    new npc_darkrune_watcher;
    new npc_darkrune_guardian;
    new npc_darkrune_sentinel;
    new mole_machine_trigger;
    new npc_razorscale_harpoon;
    new go_razorscale_harpoon;
}