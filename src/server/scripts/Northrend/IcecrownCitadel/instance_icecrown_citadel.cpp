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

#define MAX_ENCOUNTER      13

const Position SpawnLoc[]=
{
    {4671.521f, 2481.815f, 343.365f, 3.166f} //spawn pos
};

class instance_icecrown_citadel : public InstanceMapScript
{
    public:
        instance_icecrown_citadel() : InstanceMapScript("instance_icecrown_citadel", 631) { }

        struct instance_icecrown_citadel_InstanceMapScript : public InstanceScript
        {
            instance_icecrown_citadel_InstanceMapScript(InstanceMap* pMap) : InstanceScript(pMap)
            {
                uiDifficulty = pMap->GetDifficulty();

                uiLordMarrowgar         = 0;
                uiLadyDeathwhisper      = 0;
                uiGunship               = 0;
                uiDeathbringerSaurfang  = 0;
                uiFestergut             = 0;
                uiRotface               = 0;
                uiProfessorPutricide    = 0;
                uiPrinceValanar         = 0;
                uiPrinceKeleseth        = 0;
                uiPrinceTaldaram        = 0;
                uiBloodQueenLanathel    = 0;
                uiValithriaDreamwalker  = 0;
                uiSindragosa            = 0;
                uiLichKing              = 0;
                uiTirion                = 0;
                uiSaurfangCache         = 0;

                uiAngle                 = 0;
                uiSpawn                 = 0;
                uiBoned                 = 0;
                uiAllYouCanEat          = 0;
                uiNecroticStack         = 0;
                uiBloodCouncilController = 0;
                uiBeenWaiting           = 0;
                uiNeckDeep              = 0;
                uiIceWall1              = 0;
                uiIceWall2              = 0;
                uiMarrowgarEntrance     = 0;
                uiLadyDeathwisperTransporter = 0;
                uiOratoryDoor           = 0;
                uiSaurfangDoor          = 0;
                uiOrangeMonsterDoor     = 0;
                uiGreenMonsterDoor      = 0;
                uiProfCollisionDoor     = 0;
                uiPutricideTable        = 0;
                uiOrangePipe            = 0;
                uiGreenPipe             = 0;
                uiOozeValve             = 0;
                uiGasValve              = 0;
                uiProfDoorOrange        = 0;
                uiProfDoorGreen         = 0;
                uiRotfaceEntrance       = 0;
                uiFestergurtEntrance    = 0;
                uiProffesorDoor         = 0;
                uiBloodwingDoor         = 0;
                uiCrimsonHallDoor1      = 0;
                uiCrimsonHallDoor2      = 0;
                uiCrimsonHallDoor3      = 0;
                uiBloodQueenTransporter = 0;
                uiFrostwingDoor         = 0;
                uiDragonDoor1           = 0;
                uiDragonDoor2           = 0;
                uiDragonDoor3           = 0;
                uiRoostDoor1            = 0;
                uiRoostDoor2            = 0;
                uiRoostDoor3            = 0;
                uiRoostDoor4            = 0;
                uiValithriaTransporter  = 0;
                uiSindragossaTransporter = 0;
                uiSindragosaDoor1       = 0;
                uiSindragosaDoor2       = 0;
                uiFirstTp               = 0;
                uiMarrowgarTp           = 0;
                uiFlightWarTp           = 0;
                uiSaurfangTp            = 0;
                uiCitadelTp             = 0;
                uiSindragossaTp         = 0;
                uiLichTp                = 0;

                memset(&uiEncounter, 0, sizeof(uiEncounter));
            };

            bool IsEncounterInProgress() const
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    if (uiEncounter[i] == IN_PROGRESS)
                        return true;
                }

                return false;
            }

            void OnCreatureCreate(Creature* creature)
            {
                Map::PlayerList const &players = instance->GetPlayers();
                uint32 TeamInInstance = 0;


                if (!players.isEmpty())
                {
                    if (Player* pPlayer = players.begin()->getSource())
                        TeamInInstance = pPlayer->GetTeam();
                }
                switch(creature->GetEntry())
                {
                    case CREATURE_KOR_KRON_GENERAL:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_ALLIANCE_COMMANDER, ALLIANCE);
                        break;
                    case CREATURE_TORTUNOK:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_ALANA_MOONSTRIKE, ALLIANCE);
                        break;
                    case CREATURE_GERARDO_THE_SUAVE:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_TALAN_MOONSTRIKE, ALLIANCE);
                        break;
                    case CREATURE_UVLUS_BANEFIRE:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_MALFUS_GRIMFROST, ALLIANCE);
                        break;
                    case CREATURE_IKFIRUS_THE_VILE:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_YILI, ALLIANCE);
                        break;
                    case CREATURE_VOL_GUK:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_JEDEBIA, ALLIANCE);
                        break;
                    case CREATURE_HARAGG_THE_UNSEEN:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_NIBY_THE_ALMIGHTY, ALLIANCE);
                        break;
                    case CREATURE_GARROSH_HELLSCREAM:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_KING_VARIAN_WRYNN, ALLIANCE);
                        break;
                    case CREATURE_SE_HIGH_OVERLORD_SAURFANG:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_SE_MURADIN_BRONZEBEARD, ALLIANCE);
                         break;
                    case CREATURE_SE_KOR_KRON_REAVER:
                        if (TeamInInstance == ALLIANCE)
                            creature->UpdateEntry(CREATURE_SE_SKYBREAKER_MARINE, ALLIANCE);
                        break;
                    case CREATURE_MARROWGAR:
                        uiLordMarrowgar = creature->GetGUID();
                        break;
                    case CREATURE_DEATHWHISPER:
                        uiLadyDeathwhisper = creature->GetGUID();
                        break;
                    case CREATURE_GUNSHIP:
                        uiGunship = creature->GetGUID();
                        break;
                    case CREATURE_SAURFANG:
                        uiDeathbringerSaurfang = creature->GetGUID();
                        break;
                    case CREATURE_FESTERGURT:
                        uiFestergut = creature->GetGUID();
                        break;
                    case CREATURE_ROTFACE:
                        uiRotface = creature->GetGUID();
                        break;
                    case CREATURE_PROFESSOR_PUTRICIDE:
                        uiProfessorPutricide = creature->GetGUID();
                        break;
                    case CREATURE_PRINCE_VALANAR_ICC:
                        uiPrinceValanar = creature->GetGUID();
                        break;
                    case CREATURE_PRINCE_KELESETH_ICC:
                        uiPrinceKeleseth = creature->GetGUID();
                        break;
                    case CREATURE_PRINCE_TALDARAM_ICC:
                        uiPrinceTaldaram = creature->GetGUID();
                        break;
                    case CREATURE_BLOOD_ORB_CONTROLLER:
                        uiBloodCouncilController = creature->GetGUID();
                        break;
                    case CREATURE_BLOOD_QUEEN_LANATHEL:
                        uiBloodQueenLanathel = creature->GetGUID();
                        break;
                    case CREATURE_VALITHRIA_DREAMWALKER:
                        uiValithriaDreamwalker = creature->GetGUID();
                        break;
                    case CREATURE_SINDRAGOSA:
                        uiSindragosa = creature->GetGUID();
                        break;
                    case CREATURE_LICH_KING:
                        uiLichKing = creature->GetGUID();
                        break;
                    case CREATURE_TIRION_ICC:
                        uiTirion = creature->GetGUID();
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case LORD_ICE_WALL_1:
                        uiIceWall1 = go->GetGUID();
                        break;
                    case LORD_ICE_WALL_2:
                        uiIceWall2 = go->GetGUID();
                        break;
                    case LORD_ENTRANCE:
                        uiMarrowgarEntrance = go->GetGUID();
                        break;
                    case ORATORY_ENTRANCE:
                        uiOratoryDoor = go->GetGUID();
                        break;
                    case SAURFANG_DOOR:
                        uiSaurfangDoor = go->GetGUID();
                        break;
                    case BLOODWING_DOOR:
                        uiBloodwingDoor = go->GetGUID();
                        break;
                    case FROSTWING_DOOR:
                        uiFrostwingDoor = go->GetGUID();
                        break;
                    case CRIMSONHALL_DOOR:
                        uiCrimsonHallDoor1 = go->GetGUID();
                        break;
                    case CRIMSONHALL_DOOR_1:
                        uiCrimsonHallDoor2 = go->GetGUID();
                        break;
                    case CRIMSONHALL_DOOR_2:
                        uiCrimsonHallDoor3 = go->GetGUID();
                        break;
                    case DRAGON_DOOR_1:
                        uiDragonDoor1 = go->GetGUID();
                        break;
                    case DRAGON_DOOR_2:
                        uiDragonDoor2 = go->GetGUID();
                        break;
                    case DRAGON_DOOR_3:
                        uiDragonDoor3 = go->GetGUID();
                        break;
                    case DREAMWALKER_DOOR_1:
                        uiRoostDoor1 = go->GetGUID();
                        break;
                    case DREAMWALKER_DOOR_2:
                        uiRoostDoor2 = go->GetGUID();
                        break;
                    case DREAMWALKER_DOOR_3:
                        uiRoostDoor3 = go->GetGUID();
                        break;
                    case DREAMWALKER_DOOR_4:
                        uiRoostDoor4 = go->GetGUID();
                        break;
                    case SINDRAGOSSA_DOOR_1:
                        uiSindragosaDoor1 = go->GetGUID();
                        break;
                    case SINDRAGOSSA_DOOR_2:
                        uiSindragosaDoor2 = go->GetGUID();
                        break;
                    case PROF_COLLISION_DOOR:
                        uiProfCollisionDoor = go->GetGUID();
                        if (uiEncounter[4] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case GREEN_PIPE:
                        uiGreenPipe = go->GetGUID();
                        if (uiEncounter[5] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case OOZE_VALVE:
                        uiOozeValve = go->GetGUID();
                        if (uiEncounter[5] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case PROF_DOOR_GREEN:
                        uiProfDoorGreen = go->GetGUID();
                        if (uiEncounter[5] == DONE)
                            HandleGameObject(NULL, true, go);
                        break;
                    case ORANGE_PIPE:
                        uiOrangePipe = go->GetGUID();
                        if (uiEncounter[4] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case GAS_VALVE:
                        uiGasValve = go->GetGUID();
                        if (uiEncounter[4] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case PROF_DOOR_ORANGE:
                        uiProfDoorOrange = go->GetGUID();
                        if (uiEncounter[4] == DONE)
                            HandleGameObject(NULL, true, go);
                        break;
                    case ROTFACE_DOOR:
                        uiGreenMonsterDoor = go->GetGUID();
                        if (uiEncounter[5] == DONE)
                            HandleGameObject(NULL, true, go);
                        break;
                    case FESTERGUT_DOOR:
                        uiOrangeMonsterDoor = go->GetGUID();
                        if (uiEncounter[4] == DONE)
                            HandleGameObject(NULL, true, go);
                        break;
                    case PROFESSOR_DOOR:
                        uiProffesorDoor = go->GetGUID();
                        if (uiEncounter[6] == DONE)
                            HandleGameObject(NULL, true, go);
                        break;
                    case DRINK_ME:
                        uiPutricideTable = go->GetGUID();
                        break;
                    case SAURFANG_CACHE_10_N:
                    case SAURFANG_CACHE_25_N:
                    case SAURFANG_CACHE_10_H:
                    case SAURFANG_CACHE_25_H:
                        uiSaurfangCache = go->GetGUID();
                        break;
                    case DREAMWALKER_CACHE_10_N:
                    case DREAMWALKER_CACHE_25_N:
                    case DREAMWALKER_CACHE_10_H:
                    case DREAMWALKER_CACHE_25_H:
                        uiDreamwalkerCache = go->GetGUID();
                        break;
                    case LADY_ELEVATOR:
                        uiLadyDeathwisperTransporter = go->GetGUID();
                        break;
                    case BLOODQUEEN_ELEVATOR:
                        uiBloodQueenTransporter = go->GetGUID();
                        break;
                    case VALITHRIA_ELEVATOR:
                        uiValithriaTransporter = go->GetGUID();
                        break;
                    case SINDRAGOSSA_ELEVATOR:
                        uiSindragossaTransporter = go->GetGUID();
                        break;
                    case FIRST_TELEPORT:
                        uiFirstTp = go->GetGUID();
                        if (uiEncounter[0] == DONE)
                            HandleGameObject(NULL, true, go);
                        break;
                    case LORD_TELEPORT:
                        uiMarrowgarTp = go->GetGUID();
                        if (uiEncounter[0] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case GUNSHIP_TELEPORT:
                        uiFlightWarTp = go->GetGUID();
                        if (uiEncounter[2] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case SAURFANG_TELEPORT:
                        uiSaurfangTp = go->GetGUID();
                        if (uiEncounter[4] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case CITADEL_TELEPORT:
                        uiCitadelTp = go->GetGUID();
                        if (uiEncounter[4] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                    case SINDRAGOSSA_TELEPORT:
                        uiSindragossaTp = go->GetGUID();
                        if(uiEncounter[10] == NOT_STARTED)
                            HandleGameObject(NULL, true, go);
                        break;
                    case LICH_TELEPORT:
                        uiLichTp = go->GetGUID();
                        if(uiEncounter[10] == NOT_STARTED)
                            HandleGameObject(NULL, false, go);
                        break;
                }
            }

            uint64 GetData64(uint32 identifier)
            {
                switch(identifier)
                {
                    case DATA_MARROWGAR:              return uiLordMarrowgar;
                    case DATA_DEATHWHISPER:           return uiLadyDeathwhisper;
                    case DATA_GUNSHIP_BATTLE:         return uiGunship;
                    case DATA_SAURFANG:               return uiDeathbringerSaurfang;
                    case DATA_FESTERGURT:             return uiFestergut;
                    case DATA_ROTFACE:                return uiRotface;
                    case DATA_PROFESSOR_PUTRICIDE:    return uiProfessorPutricide;
                    case DATA_PRINCE_VALANAR_ICC:     return uiPrinceValanar;
                    case DATA_PRINCE_KELESETH_ICC:    return uiPrinceKeleseth;
                    case DATA_PRINCE_TALDARAM_ICC:    return uiPrinceTaldaram;
                    case DATA_BLOOD_QUEEN_LANATHEL:   return uiBloodQueenLanathel;
                    case DATA_VALITHRIA_DREAMWALKER:  return uiValithriaDreamwalker;
                    case DATA_SINDRAGOSA:             return uiSindragosa;
                    case DATA_LICH_KING:              return uiLichKing;
                    case DATA_TIRION:                 return uiTirion;
                    case DATA_ANGLE:                  return uiAngle;
                    case DATA_BONED:                  return uiBoned;
                    case DATA_ALL_YOU_CAN_EAT:        return uiAllYouCanEat;
                    case DATA_BEEN_WAITING:           return uiBeenWaiting;
                    case DATA_NECK_DEEP:              return uiNeckDeep;
                    case DATA_BLOOD_PRINCES_CONTROL:  return uiBloodCouncilController;
                    case DATA_NECROTIC_STACK:         return uiNecroticStack;
                    case DATA_PUTRICIDE_TABLE:        return uiPutricideTable;
                }
                return 0;
            }

            void SetData(uint32 type, uint32 data)
            {
                switch(type)
                {
                    case DATA_MARROWGAR_EVENT:
                        if(data == DONE)
                        {
                            HandleGameObject(uiIceWall1, true);
                            HandleGameObject(uiIceWall2, true);
                            HandleGameObject(uiMarrowgarEntrance, true);
                            if (GameObject* FirstTp = instance->GetGameObject(uiFirstTp))
                            {
                                FirstTp->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                                FirstTp->SetGoState(GOState(0));
                            }
                            if (GameObject* MarrowgarTp = instance->GetGameObject(uiMarrowgarTp))
                            {
                                MarrowgarTp->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                                MarrowgarTp->SetGoState(GOState(0));
                            }
                        }
                        if(data == NOT_STARTED)
                        {
                            if (GameObject* FirstTp = instance->GetGameObject(uiFirstTp))
                                FirstTp->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);

                            if (GameObject* MarrowgarTp = instance->GetGameObject(uiMarrowgarTp))
                                MarrowgarTp->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                        }
                        if(data == IN_PROGRESS)
                            HandleGameObject(uiMarrowgarEntrance, false);
                        uiEncounter[0] = data;
                        break;
                    case DATA_DEATHWHISPER_EVENT:
                        if(data == DONE)
                        {
                            HandleGameObject(uiOratoryDoor, true);
                            if (GameObject* go = instance->GetGameObject(uiLadyDeathwisperTransporter))
                            {
                                go->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                                go->SetGoState(GO_STATE_READY);
                            }
                            if (GameObject* FlightWarTp = instance->GetGameObject(uiFlightWarTp))
                            {
                                FlightWarTp->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                                FlightWarTp->SetGoState(GOState(0));
                            }
                            if (GameObject* SaurfangTp = instance->GetGameObject(uiSaurfangTp))
                            {
                                SaurfangTp->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                                SaurfangTp->SetGoState(GOState(0));
                            }
                        }
                        if(data == NOT_STARTED)
                        {
                            if (GameObject* FlightWarTp = instance->GetGameObject(uiFlightWarTp))
                                FlightWarTp->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);

                            if (GameObject* SaurfangTp = instance->GetGameObject(uiSaurfangTp))
                                SaurfangTp->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);

                        }
                        if(data == IN_PROGRESS)
                            HandleGameObject(uiOratoryDoor, false);
                        uiEncounter[1] = data;
                        break;
                    /*case DATA_GUNSHIP_BATTLE_EVENT:
                        switch(data)
                        {
                            case DONE:
                                break;
                            case NOT_STARTED:
                                break;
                        }
                        uiEncounter[2] = data;
                        break;*/
                    case DATA_SAURFANG_EVENT:
                        if(data == DONE)
                        {
                            if (GameObject* pChest = instance->GetGameObject(uiSaurfangCache))
                                pChest->SetRespawnTime(pChest->GetRespawnDelay());

                            if (GameObject* CitadelTp = instance->GetGameObject(uiCitadelTp))
                            {
                                CitadelTp->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                                CitadelTp->SetGoState(GOState(0));
                            }
                            HandleGameObject(uiSaurfangDoor, true);
                            HandleGameObject(uiBloodwingDoor, true);
                            HandleGameObject(uiFrostwingDoor, true);
                        }
                        if(data == NOT_STARTED)
                        {
                            if (GameObject* CitadelTp = instance->GetGameObject(uiCitadelTp))
                                CitadelTp->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                        }
                        uiEncounter[3] = data;
                        break;
                    case DATA_FESTERGURT_EVENT:
                        if(data == DONE)
                        {
                            HandleGameObject(uiOrangeMonsterDoor, true);
                            HandleGameObject(uiOrangePipe, true);
                            HandleGameObject(uiGasValve, true);
                            if (uiEncounter[5] == DONE)
                            {
                                HandleGameObject(uiProfCollisionDoor, true);
                                if (GameObject* Oprofessordoor = instance->GetGameObject(uiProfDoorOrange))
                                {
                                    Oprofessordoor->SetGoState(GOState(2));
                                }
                            }
                        }
                        if(data == NOT_STARTED)
                        {
                            HandleGameObject(uiOrangeMonsterDoor, true);
                            HandleGameObject(uiOrangePipe, false);
                            HandleGameObject(uiGasValve, false);
                            HandleGameObject(uiProfDoorOrange, false);
                        }
                        if(data == IN_PROGRESS)
                        {
                            HandleGameObject(uiOrangeMonsterDoor, false);
                            HandleGameObject(uiOrangePipe, false);
                            HandleGameObject(uiGasValve, false);
                            HandleGameObject(uiProfDoorOrange, false);
                        }
                        uiEncounter[4] = data;
                        break;
                    case DATA_ROTFACE_EVENT:
                        if(data == DONE)
                        {
                            HandleGameObject(uiGreenMonsterDoor, true);
                            HandleGameObject(uiGreenPipe, true);
                            HandleGameObject(uiOozeValve, true);
                            if (uiEncounter[4] == DONE)
                            {
                                HandleGameObject(uiProfCollisionDoor, true);
                                if (GameObject* Gprofessordoor = instance->GetGameObject(uiProfDoorGreen))
                                {
                                    Gprofessordoor->SetGoState(GOState(2));
                                }
                            }
                        }
                        if(data == NOT_STARTED)
                        {
                            HandleGameObject(uiGreenMonsterDoor, true);
                            HandleGameObject(uiGreenPipe, false);
                            HandleGameObject(uiOozeValve, false);
                            HandleGameObject(uiProfDoorGreen, false);
                        }
                        if(data == IN_PROGRESS)
                        {
                            HandleGameObject(uiGreenMonsterDoor, false);
                            HandleGameObject(uiGreenPipe, false);
                            HandleGameObject(uiOozeValve, false);
                            HandleGameObject(uiProfDoorGreen, false);
                        }
                        uiEncounter[5] = data;
                        break;
                    case DATA_PROFESSOR_PUTRICIDE_EVENT:
                        if(data == DONE)
                            HandleGameObject(uiProffesorDoor, true);
                        if(data == NOT_STARTED)
                            HandleGameObject(uiProffesorDoor, true);
                        if(data == IN_PROGRESS)
                            HandleGameObject(uiProffesorDoor, false);
                        uiEncounter[6] = data;
                        break;
                    case DATA_BLOOD_PRINCE_COUNCIL_EVENT:
                        if(data == DONE)
                        {
                            HandleGameObject(uiCrimsonHallDoor1, true);
                            HandleGameObject(uiCrimsonHallDoor2, true);
                            HandleGameObject(uiCrimsonHallDoor3, true);
                        }
                        if(data == NOT_STARTED)
                            HandleGameObject(uiCrimsonHallDoor1, true);

                        if(data == IN_PROGRESS)
                            HandleGameObject(uiCrimsonHallDoor1, false);
                        uiEncounter[7] = data;
                        break;
                    case DATA_BLOOD_QUEEN_LANATHEL_EVENT:
                        if(data == DONE)
                        {
                            if (GameObject* go = instance->GetGameObject(uiBloodQueenTransporter))
                            {
                                go->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                                go->SetGoState(GO_STATE_READY);
                            }
                        }
                        uiEncounter[8] = data;
                        break;
                    case DATA_VALITHRIA_DREAMWALKER_EVENT:
                        if(data == DONE)
                        {
                            if (GameObject* pChest = instance->GetGameObject(uiDreamwalkerCache))
                                pChest->SetRespawnTime(pChest->GetRespawnDelay());

                            if (GameObject* SindragossaTp = instance->GetGameObject(uiSindragossaTp))
                            {
                                SindragossaTp->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                                SindragossaTp->SetGoState(GOState(0));
                            }
                            if (GameObject* go = instance->GetGameObject(uiValithriaTransporter))
                            {
                                go->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                                go->SetGoState(GO_STATE_READY);
                            }
                            if (GameObject* go = instance->GetGameObject(uiSindragossaTransporter))
                            {
                                go->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                                go->SetGoState(GO_STATE_READY);
                            }
                            HandleGameObject(uiDragonDoor1, true);
                            HandleGameObject(uiDragonDoor2, true);
                            HandleGameObject(uiDragonDoor3, true);
                        }
                        if(data == NOT_STARTED)
                        {
                            if (GameObject* SindragossaTp = instance->GetGameObject(uiSindragossaTp))
                                SindragossaTp->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                        }
                        if(data == IN_PROGRESS)
                        {
                            HandleGameObject(uiDragonDoor1, false);
                            HandleGameObject(uiRoostDoor3, true);
                            HandleGameObject(uiRoostDoor2, true);
                            if (uiDifficulty == RAID_DIFFICULTY_25MAN_NORMAL || uiDifficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                            {
                                HandleGameObject(uiRoostDoor1, true);
                                HandleGameObject(uiRoostDoor4, true);
                            }
                        }
                        uiEncounter[9] = data;
                        break;
                    case DATA_SINDRAGOSA_EVENT:
                        if(data == DONE)
                        {
                            HandleGameObject(uiSindragosaDoor1, true);
                            HandleGameObject(uiSindragosaDoor2, true);
                        }
                        uiEncounter[10] = data;
                        break;
                    case DATA_LICH_KING_EVENT:
                        uiEncounter[11] = data;
                        break;
                    case DATA_SPAWN:
                        if(data >= 2)
                            Creature* sindragosa = instance->SummonCreature(CREATURE_SINDRAGOSA, SpawnLoc[0]);
                        break;
                }

                if (data == DONE)
                    SaveToDB();
            }

            uint32 GetData(uint32 type)
            {
                switch(type)
                {
                case DATA_MARROWGAR_EVENT:
                    return uiEncounter[0];
                case DATA_DEATHWHISPER_EVENT:
                    return uiEncounter[1];
                case DATA_GUNSHIP_BATTLE_EVENT:
                    return uiEncounter[2];
                case DATA_SAURFANG_EVENT:
                    return uiEncounter[3];
                case DATA_FESTERGURT_EVENT:
                    return uiEncounter[4];
                case DATA_ROTFACE_EVENT:
                    return uiEncounter[5];
                case DATA_PROFESSOR_PUTRICIDE_EVENT:
                    return uiEncounter[6];
                case DATA_BLOOD_PRINCE_COUNCIL_EVENT:
                    return uiEncounter[7];
                case DATA_BLOOD_QUEEN_LANATHEL_EVENT:
                    return uiEncounter[8];
                case DATA_VALITHRIA_DREAMWALKER_EVENT:
                    return uiEncounter[9];
                case DATA_SINDRAGOSA_EVENT:
                    return uiEncounter[10];
                case DATA_LICH_KING_EVENT:
                    return uiEncounter[11];
                case DATA_SPAWN:
                    return uiEncounter[12];
                }
                return 0;
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "I C" << uiEncounter[0] << " " << uiEncounter[1] << " " << uiEncounter[2] << " " << uiEncounter[3]
                << " " << uiEncounter[4] << " " << uiEncounter[5] << " " << uiEncounter[6] << " " << uiEncounter[7] << " " << uiEncounter[8]
                << " " << uiEncounter[9] << " " << uiEncounter[10] << " " << uiEncounter[11];

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* in)
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;
                uint32 data0,data1,data2,data3,data4,data5,data6,data7,data8,data9,data10,data11;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3 >> data4 >> data5 >> data6 >> data7 >> data8 >> data9 >> data10 >> data11;

                if (dataHead1 == 'I' && dataHead2 == 'C')
                {
                    uiEncounter[0] = data0;
                    uiEncounter[1] = data1;
                    uiEncounter[2] = data2;
                    uiEncounter[3] = data3;
                    uiEncounter[4] = data4;
                    uiEncounter[5] = data5;
                    uiEncounter[6] = data6;
                    uiEncounter[7] = data7;
                    uiEncounter[8] = data8;
                    uiEncounter[9] = data9;
                    uiEncounter[10] = data10;
                    uiEncounter[11] = data11;

                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        loadStream >> uiEncounter[i];

                        if (uiEncounter[i] == IN_PROGRESS)
                            uiEncounter[i] = NOT_STARTED;
                    }

                } else OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            uint64 uiLordMarrowgar;
            uint64 uiLadyDeathwhisper;
            uint64 uiGunship;
            uint64 uiDeathbringerSaurfang;
            uint64 uiFestergut;
            uint64 uiRotface;
            uint64 uiProfessorPutricide;
            uint64 uiPrinceValanar;
            uint64 uiPrinceKeleseth;
            uint64 uiPrinceTaldaram;
            uint64 uiBloodQueenLanathel;
            uint64 uiValithriaDreamwalker;
            uint64 uiSindragosa;
            uint64 uiLichKing;
            uint64 uiTirion;
            uint64 uiIceWall1;
            uint64 uiIceWall2;
            uint64 uiMarrowgarEntrance;
            uint64 uiFrozenThrone;
            uint64 uiSaurfangCache;
            uint64 uiLadyDeathwisperTransporter;
            uint64 uiOratoryDoor;
            uint64 uiSaurfangDoor;
            uint64 uiOrangeMonsterDoor;
            uint64 uiGreenMonsterDoor;
            uint64 uiProfCollisionDoor;
            uint64 uiPutricideTable;
            uint64 uiOrangePipe;
            uint64 uiGreenPipe;
            uint64 uiOozeValve;
            uint64 uiGasValve;
            uint64 uiProfDoorOrange;
            uint64 uiProfDoorGreen;
            uint64 uiRotfaceEntrance;
            uint64 uiFestergurtEntrance;
            uint64 uiProffesorDoor;
            uint64 uiBloodwingDoor;
            uint64 uiCrimsonHallDoor1;
            uint64 uiCrimsonHallDoor2;
            uint64 uiCrimsonHallDoor3;
            uint64 uiBloodQueenTransporter;
            uint64 uiFrostwingDoor;
            uint64 uiDragonDoor1;
            uint64 uiDragonDoor2;
            uint64 uiDragonDoor3;
            uint64 uiRoostDoor1;
            uint64 uiRoostDoor2;
            uint64 uiRoostDoor3;
            uint64 uiRoostDoor4;
            uint64 uiValithriaTransporter;
            uint64 uiSindragossaTransporter;
            uint64 uiDreamwalkerCache;
            uint64 uiSindragosaDoor1;
            uint64 uiSindragosaDoor2;
            uint64 uiFirstTp;
            uint64 uiMarrowgarTp;
            uint64 uiFlightWarTp;
            uint64 uiSaurfangTp;
            uint64 uiCitadelTp;
            uint64 uiSindragossaTp;
            uint64 uiLichTp;
            uint64 uiBloodCouncilController;
            uint8 uiDifficulty;
            uint8 uiBoned;
            uint8 uiSpawn;
            uint8 uiAllYouCanEat;
            uint8 uiBeenWaiting;
            uint8 uiNeckDeep;
            uint8 uiNecroticStack;
            uint8 uiAngle;
            uint32 uiEncounter[MAX_ENCOUNTER];
        };

        InstanceScript* GetInstanceScript(InstanceMap* pMap) const
        {
            return new instance_icecrown_citadel_InstanceMapScript(pMap);
        }
};

void AddSC_instance_icecrown_citadel()
{
    new instance_icecrown_citadel();
}
