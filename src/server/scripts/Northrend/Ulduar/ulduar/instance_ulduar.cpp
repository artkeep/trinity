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

enum eGameObjects
{
    GO_Kologarn_CHEST_HERO  = 195047,
    GO_Kologarn_CHEST       = 195046,
    GO_Thorim_CHEST_HERO    = 194315,
    GO_Thorim_CHEST         = 194314,
    GO_Hodir_CHEST_HERO     = 194308,
    GO_Hodir_CHEST          = 194307,
    GO_Hodir_Rare_CHEST     = 194200,
    GO_Hodir_Rare_CHEST_HERO= 194201,
    GO_Freya_CHEST_HERO     = 194325,
    GO_Freya_CHEST          = 194324,
    GO_LEVIATHAN_DOOR       = 194905,
    GO_LEVIATHAN_GATE       = 194630,
	GO_DARK_IRON_PORTCULLIS	= 194560,
	GO_RUNED_STONE_DOOR_1	= 194557,
	GO_RUNED_STONE_DOOR_2	= 194558,
	GO_MIMIRON_TRAM         = 194675,
	GO_FREYA_TARGET			= 194704,
	GO_HODIR_TARGET			= 194707,
	GO_THORIM_TARGET		= 194706,
	GO_MIMIRON_TARGET		= 194705,
	GO_XT002_DOOR			= 194631,
	GO_IRON_COUNCIL_DOOR	= 194554,
	GO_ALGALON_DOOR			= 194767,
	GO_Mimiron_CHEST_HERO   = 194789,
	GO_Mimiron_CHEST        = 194956,
	GO_Mimiron_Door_1		= 194774,
	GO_Mimiron_Door_2		= 194775,
	GO_Mimiron_Door_3		= 194776,
	GO_Archivum_Door		= 194556,
	GO_Hodir_Door_1			= 194442,
	GO_Hodir_Door_2			= 194441,
	GO_Hodir_Door_3			= 194634,
	GO_Vezax_Door			= 194750,
	GO_Yogg_Saron_Door		= 194773,
	GO_Gate_of_the_Keepers	= 194255,
	GO_Big_Red_Button		= 194739,
	GO_Storm_Beacon			= 194415
};

const Position mechanoliftPos[4] = 
{
    {194.36f, -229.92f, 463.03f , 0.0f},
    {371.80f,  156.42f, 474.0f , 0.0f},
    {286.92f,  162.35f, 474.0f , 0.0f},
    {314.19f, -240.39f, 464.63f , 0.0f}
};

class instance_ulduar : public InstanceMapScript
{
public:
    instance_ulduar() : InstanceMapScript("instance_ulduar", 603) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_ulduar_InstanceMapScript(pMap);
    }

    struct instance_ulduar_InstanceMapScript : public InstanceScript
    {
        instance_ulduar_InstanceMapScript(Map* pMap) : InstanceScript(pMap), KologarnChest(NULL), ThorimChest(NULL), HodirChest(NULL) { Initialize(); };

        uint32 uiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;
        uint8 uiLeviathanDoorsFlag;
        uint8 uiBrokenHarpoonsFlag;
        uint8 uiHarpoonGunsFlag;

        uint64 uiLeviathanGUID;
        uint64 uiIgnisGUID;
        uint64 uiRazorscaleGUID;
        uint64 uiExpCommanderGUID;
        uint64 uiXT002GUID;
        uint64 uiSteelbreakerGUID;
        uint64 uiMolgeimGUID;
        uint64 uiBrundirGUID;
        uint64 uiKologarnGUID;
        uint64 uiAuriayaGUID;
        uint64 uiMimironGUID;
        uint64 uiHodirGUID;
        uint64 uiThorimGUID;
        uint64 uiFreyaGUID;
        uint64 uiVezaxGUID;
        uint64 uiYoggSaronGUID;
        uint64 uiAlgalonGUID;
        uint64 uiLeviathanDoor[7];
        uint64 uiLeviathanGateGUID;

        uint64 uiRightArmGUID;
        uint64 uiLeftArmGUID;
        uint64 uiKologarnLootGUID;
        uint64 uiKologarnBridgeGUID;
        uint64 uiSentryGUID1;
        uint64 uiSentryGUID2;
        uint64 uiSentryGUID3;
        uint64 uiSentryGUID4;
        uint64 uiFeralDefenderGUID;
        uint64 uiBrightleafGUID ;
        uint64 uiIronbranchGUID ;
        uint64 uiStonebarkGUID ;
        uint64 uiRunicColossusGUID ;
        uint64 uiAncientRuneGiantGUID ;
        uint64 uiRunicDoor ;
        uint64 uiStoneDoor ;
        uint64 uiDarkIronProtcullis ;
        uint64 uiLeviathanMKIIGUID ;
        uint64 uiVX001;
        uint64 uiAerialUnit;
        uint64 uiMagneticCore;
        uint64 uiMimironElevatorGUID ;
        uint64 uiMimironTramGUID ;
        uint64 uiMimironTargetGUID ;
        uint64 uiHodirTargetGUID    ;
        uint64 uiThorimTargetGUID ;
        uint64 uiFreyaTargetGUID ;
        uint64 uiXT002Door ;
        uint64 uiIronCouncilDoor ;
        uint64 uiAlgalonDoor ;
        uint64 uiMimironDoors[3];
        uint64 uiArchivumDoor ;
        uint64 uiHodirDoor1;
        uint64 uiHodirDoor2;
        uint64 uiHodirDoor3;
        uint64 uiVezaxDoor;
        uint64 uiYoggSaronDoor;
        uint64 uiKeepersGate;
        uint64 uiBigRedButton;
        uint64 uiYoggSaronBrainGUID;
        uint64 uiYoggSaron;
        uint64 uiSaraGUID;
        uint32 uiLeviathanHardMode;
        uint32 uiUlduarEvent;
        uint64 uiThorimImageGUID;
        uint64 uiHodirImageGUID;
        uint64 uiMimironImageGUID;
        uint64 uiFreyaImageGUID;
        uint64 uiFreyaYS;
        uint64 uiThorimYS;
        uint64 uiMimironYS;
        uint64 uiHodirYS;
        uint64 uiAlgalonTrapDoorGUID;
        uint64 uiAlgalonGlobeGUID;
        uint64 uiUniverseFloor1GUID;
        uint64 uiUniverseFloor2GUID;
        uint64 uiThorimLightningFieldGUID;
        uint32 uiSummonTimer;
        std::list<uint64> thorimNPCList ;
        std::list<uint64> vehicleList ;
        std::list<uint64> beaconGOList ;
        std::list<uint64> ironConstructList;
        uint64 uiBrokenHarpoons[4];
        uint64 uiHarpoonGuns[4];
        uint64 uiUniverFloor1GUID;
        uint64 uiUniverFloor2GUID;
        GameObject* KologarnChest, *ThorimChest, *HodirChest, *HodirRareChest;

        void Initialize()
        {
            SetBossNumber(MAX_ENCOUNTER);
            uiLeviathanGUID         = 0;
            uiIgnisGUID             = 0;
            uiRazorscaleGUID        = 0;
            uiExpCommanderGUID      = 0;
            uiXT002GUID             = 0;
            uiKologarnGUID          = 0;
            uiAuriayaGUID           = 0;
            uiMimironGUID           = 0;
            uiHodirGUID             = 0;
            uiThorimGUID            = 0;
            uiFreyaGUID             = 0;
            uiVezaxGUID             = 0;
            uiYoggSaronGUID         = 0;
            uiAlgalonGUID           = 0;
            KologarnChest           = 0;
            ThorimChest             = 0;
            HodirChest              = 0;
            uiRightArmGUID          = 0;
            uiLeftArmGUID           = 0;
            uiKologarnLootGUID      = 0;
            uiKologarnBridgeGUID    = 0;
            uiLeviathanGateGUID     = 0;
            uiLeviathanDoorsFlag    = 0;
            uiFeralDefenderGUID     = 0;
            uiBrightleafGUID        = 0;
            uiIronbranchGUID        = 0;
            uiStonebarkGUID         = 0;
            uiRunicColossusGUID     = 0;
            uiAncientRuneGiantGUID  = 0;
            uiRunicDoor             = 0;
            uiStoneDoor             = 0;
            uiLeviathanMKIIGUID     = 0;
            uiVX001                 = 0;
            uiAerialUnit            = 0;
            uiMagneticCore          = 0;
            uiMimironElevatorGUID   = 0;
            uiMimironTramGUID       = 0;
            uiMimironTargetGUID     = 0;
            uiHodirTargetGUID       = 0;
            uiThorimTargetGUID      = 0;
            uiFreyaTargetGUID       = 0;
            uiXT002Door             = 0;
            uiIronCouncilDoor       = 0;
            uiMolgeimGUID           = 0;
            uiSteelbreakerGUID      = 0;
            uiBrundirGUID           = 0;
            uiDarkIronProtcullis    = 0;
            uiMimironDoors[0]       = 0;
            uiMimironDoors[1]       = 0;
            uiMimironDoors[2]       = 0;
            uiArchivumDoor          = 0;
            uiHodirDoor1            = 0;
            uiHodirDoor2            = 0;
            uiHodirDoor3            = 0;
            uiVezaxDoor             = 0;
            uiYoggSaronDoor         = 0;
            uiKeepersGate           = 0;
            uiBigRedButton          = 0;
            uiYoggSaronBrainGUID    = 0;
            uiSaraGUID              = 0;
            uiYoggSaronGUID         = 0;
            uiLeviathanHardMode     = 0;
            uiThorimImageGUID       = 0;
            uiHodirImageGUID        = 0;
            uiMimironImageGUID      = 0;
            uiFreyaImageGUID        = 0;
            uiFreyaYS               = 0;
            uiThorimYS              = 0;
            uiMimironYS             = 0;
            uiHodirYS               = 0;
            uiUlduarEvent           = 0;
            uiAlgalonTrapDoorGUID   = 0;
            uiAlgalonGlobeGUID      = 0;
            uiUniverseFloor1GUID    = 0;
            uiUniverseFloor2GUID    = 0;
            uiAlgalonTrapDoorGUID   = 0;
            uiSummonTimer           = 0;
            uiBrokenHarpoonsFlag    = 0;
            uiHarpoonGunsFlag       = 0;
            uiThorimLightningFieldGUID= 0;
 
            uiUniverFloor1GUID=0;
            uiUniverFloor2GUID=0;

            memset(&uiEncounter, 0, sizeof(uiEncounter));
            memset(&uiLeviathanDoor, 0, sizeof(uiLeviathanDoor));
			memset(&uiBrokenHarpoons, 0, sizeof(uiBrokenHarpoons));
            memset(&uiHarpoonGuns, 0, sizeof(uiHarpoonGuns));
        }

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
                case NPC_LEVIATHAN:
                    uiLeviathanGUID = creature->GetGUID();
                    break;
                case NPC_IGNIS:
                    uiIgnisGUID = creature->GetGUID();
                    break;
                case NPC_RAZORSCALE:
                    uiRazorscaleGUID = creature->GetGUID();
                    break;
                case NPC_XT002:
                    uiXT002GUID = creature->GetGUID();
                    break;
                case NPC_KOLOGARN:
                    uiKologarnGUID = creature->GetGUID();
                    break;
				case NPC_RIGHT_ARM:
                    uiRightArmGUID = creature->GetGUID();
                    break;
			    case NPC_LEFT_ARM:
                    uiLeftArmGUID = creature->GetGUID();
                    break;
                case NPC_AURIAYA:
                    uiAuriayaGUID = creature->GetGUID();
                    break;
				case NPC_FERAL_DEFENDER:
				    uiFeralDefenderGUID = creature->GetGUID();
				    break;
                case NPC_MIMIRON:
                    uiMimironGUID = creature->GetGUID();
                    break;
                case NPC_HODIR:
                    uiHodirGUID = creature->GetGUID();
                    break;
                case NPC_THORIM:
                    uiThorimGUID = creature->GetGUID();
                    break;
                case NPC_FREYA:
                    uiFreyaGUID = creature->GetGUID();
                    break;
                case NPC_VEZAX:
                    uiVezaxGUID = creature->GetGUID();
                    break;
                case NPC_YOGGSARON:
                    uiYoggSaronGUID = creature->GetGUID();
                    break;
                case NPC_ALGALON:
                    uiAlgalonGUID = creature->GetGUID();
                    break;
				case NPC_ELDER_BRIGHTLEAF:
                    uiBrightleafGUID = creature->GetGUID();
                    break;
			    case NPC_ELDER_IRONBRANCH:
                    uiIronbranchGUID = creature->GetGUID();
                    break;
			    case NPC_ELDER_STONEBARK:
                    uiStonebarkGUID = creature->GetGUID();
                    break;
			    case NPC_TOR_GREYCLOUD:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_EIVI_NIGHTFEATHER, HORDE);
                    break;
			    case NPC_KAR_GREYCLOUD:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_ELLIE_NIGHTFEATHER, HORDE);
                    break;
			    case NPC_SPIRITWALKER_TARA:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_ELEMENTALIST_MAHFUUN, HORDE);
                    break;
			    case NPC_SPIRITWALKER_YONA:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_ELEMENTALIST_AVUUN, HORDE);
                    break;
			    case NPC_AMIRA_BLAZEWEAVER:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_MISSY_FLAMECUFFS, HORDE);
                    break;
			    case NPC_VEESHA_BLAZEWEAVER:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_SISSY_FLAMECUFFS, HORDE);
                    break;
			    case NPC_BATTLE_PRIEST_ELIZA:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_FIELD_MEDIC_PENNY, HORDE);
                    break;
			    case NPC_BATTLE_PRIEST_GINA:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_FIELD_MEDIC_JESSI, HORDE);
                    break;
			    case NPC_CAPTURED_MERCENARY_CAPTAIN_A:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_CAPTURED_MERCENARY_CAPTAIN_H, HORDE);
				    thorimNPCList.push_back(creature->GetGUID()) ;
				    break ;
			    case NPC_CAPTURED_MERCENARY_SOLDIER_A:
                    if (TeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_CAPTURED_MERCENARY_SOLDIER_H, HORDE);
				    thorimNPCList.push_back(creature->GetGUID()) ;
				    break ;
			    case NPC_RUNIC_COLOSSUS:
				    uiRunicColossusGUID = creature->GetGUID() ;
			    case NPC_ANCIENT_RUNE_GIANT:
				    uiAncientRuneGiantGUID = creature->GetGUID() ;
			    case NPC_JORMUNGAR_BEHEMOTH:
			    case NPC_DARK_RUNE_ACOLYTE:
			    case NPC_IRON_RING_GUARD:
			    case NPC_IRON_HONOR_GUARD:
			    case NPC_THORIM_PHASE_TRIGGER:
				    thorimNPCList.push_back(creature->GetGUID()) ;
				    break ;
			    case NPC_LEVIATHAN_MK_II:
				    uiLeviathanMKIIGUID = creature->GetGUID() ;
				    break ;
			    case 33651: 
				    uiVX001 = creature->GetGUID(); 
				    break;
			    case 33670: 
				    uiAerialUnit = creature->GetGUID(); 
				    break;
			    case 34068: 
				    uiMagneticCore = creature->GetGUID(); 
				    break;
			    case NPC_MOLGEIM:
				    uiMolgeimGUID = creature->GetGUID() ;
				    break ;
			    case NPC_STEELBREAKER:
				    uiSteelbreakerGUID = creature->GetGUID() ;
				    break ;
			    case NPC_BRUNDIR:
				    uiBrundirGUID = creature->GetGUID() ;
				    break ;
			    case NPC_IRON_CONSTRUCT:
				    //ironConstructList.push_back(pCreature->GetGUID()) ;
				    break ;
			    case NPC_COMMANDER: 
				    uiExpCommanderGUID = creature->GetGUID();
				    break;
			    case NPC_BRAIN_OF_YOGG_SARON:
				    uiYoggSaronBrainGUID = creature->GetGUID();
				    break;
			    case NPC_SARA:
				    uiSaraGUID = creature->GetGUID();
				    break;
                case NPC_THORIM_GOSSIP:
                    uiThorimImageGUID = creature->GetGUID();
                    if (uiEncounter[TYPE_VEZAX] != DONE)
                        creature->SetVisible(false);
                    break;
                case NPC_HODIR_GOSSIP:
                    uiHodirImageGUID = creature->GetGUID();
                    if (uiEncounter[TYPE_VEZAX] != DONE)
                        creature->SetVisible(false);
                    break;
                case NPC_MIMIRON_GOSSIP:
                    uiMimironImageGUID = creature->GetGUID();
                    if (uiEncounter[TYPE_VEZAX] != DONE)
                        creature->SetVisible(false);
                    break;
                case NPC_FREYA_GOSSIP:
                    uiFreyaImageGUID = creature->GetGUID();
                    if (uiEncounter[TYPE_VEZAX] != DONE)
                        creature->SetVisible(false);
                    break;
                case NPC_VEHICLE_SIEGE:
                case NPC_VEHICLE_DEMOLISHER:
                case NPC_VEHICLE_CHOPPER:
                    vehicleList.push_back(creature->GetGUID()) ;
                    break;
                case 33410: 
                    uiFreyaYS = creature->GetGUID(); 
                    break;
                case 33413: 
                    uiThorimYS = creature->GetGUID(); 
                    break;
                case 33412: 
                    uiMimironYS = creature->GetGUID(); 
                    break;
                case 33411: 
                    uiHodirYS = creature->GetGUID(); 
                    break;
            }
         }

        void OnGameObjectCreate(GameObject* pGo)
        {
            switch(pGo->GetEntry())
            {
                case GO_Kologarn_CHEST_HERO:
					KologarnChest = pGo;
					break;
                case GO_Kologarn_CHEST:
					KologarnChest = pGo;
					break;
                case GO_Hodir_CHEST_HERO:
					HodirChest = pGo;
					break;
                case GO_Hodir_CHEST:
					HodirChest = pGo;
					break;
                case GO_Hodir_Rare_CHEST:
					HodirRareChest = pGo;
					break;
                case GO_Hodir_Rare_CHEST_HERO:
					HodirRareChest = pGo;
					break;
			    case GO_KOLOGARN_BRIDGE:
				    uiKologarnBridgeGUID = pGo->GetGUID();
                    if (uiEncounter[TYPE_KOLOGARN] == DONE)
                        HandleGameObject(uiKologarnBridgeGUID,false);
                    break;
                case GO_LEVIATHAN_DOOR:
                    uiLeviathanDoor[uiLeviathanDoorsFlag] = pGo->GetGUID();
                    HandleGameObject(NULL, true, pGo);
                    uiLeviathanDoorsFlag++;
                    if (uiLeviathanDoorsFlag == 7)
                        uiLeviathanDoorsFlag =0;
                    break;
                case GO_LEVIATHAN_GATE:
                    uiLeviathanGateGUID = pGo->GetGUID();
                    HandleGameObject(NULL, false, pGo);
                    break;
			    case GO_RUNED_STONE_DOOR_1:
				    uiRunicDoor = pGo->GetGUID() ;
				    break ;
			    case GO_RUNED_STONE_DOOR_2:
				    uiStoneDoor = pGo->GetGUID() ;
				    break ;
			    case GO_DARK_IRON_PORTCULLIS:
				    uiDarkIronProtcullis = pGo->GetGUID() ;
				    break ;
			    case GO_MIMIRON_ELEVATOR:
				    uiMimironElevatorGUID = pGo->GetGUID() ;
				    break ;
			    case GO_MIMIRON_TRAM:
				    uiMimironTramGUID = pGo->GetGUID() ;
				    break ;
			    case GO_FREYA_TARGET:
				    uiFreyaTargetGUID = pGo->GetGUID() ;
				    break ;
			    case GO_THORIM_TARGET:
				    uiThorimTargetGUID = pGo->GetGUID() ;
				    break ;
			    case GO_HODIR_TARGET:
				    uiHodirTargetGUID = pGo->GetGUID() ;
				    break ;
			    case GO_MIMIRON_TARGET:
				    uiMimironTargetGUID = pGo->GetGUID() ;
				    break ;
			    case GO_XT002_DOOR:
				    uiXT002Door = pGo->GetGUID() ;
				    break ;
			    case GO_IRON_COUNCIL_DOOR:
				    uiIronCouncilDoor = pGo->GetGUID() ;
				    break ;
			    case GO_ALGALON_DOOR:
				    uiAlgalonDoor = pGo->GetGUID() ;
                    if (uiEncounter[TYPE_ALGALON] == DONE)
                        HandleGameObject(uiAlgalonDoor, true);
				    break ;
			    case GO_BROKEN_HARPOON:
				    if (uiBrokenHarpoonsFlag < 4)
                    {
                        uiBrokenHarpoons[uiBrokenHarpoonsFlag] = pGo->GetGUID();
                        uiBrokenHarpoonsFlag++;
                    }
				    break ;
                case GO_HARPOON_GUN_1:
                case GO_HARPOON_GUN_2:
                case GO_HARPOON_GUN_3:
                case GO_HARPOON_GUN_4:
				    if (uiHarpoonGunsFlag < 4)
                    {
                        uiHarpoonGuns[uiHarpoonGunsFlag] = pGo->GetGUID();
                        uiHarpoonGunsFlag++;
                    }
				    break ;
			    case GO_Mimiron_Door_1:
				    uiMimironDoors[0] = pGo->GetGUID();
				    break ;
			    case GO_Mimiron_Door_2:
				    uiMimironDoors[1] = pGo->GetGUID();
				    break ;
			    case GO_Mimiron_Door_3:
				    uiMimironDoors[2] = pGo->GetGUID();
				    break ;
			    case GO_Archivum_Door:
				    uiArchivumDoor = pGo->GetGUID();
				    if (uiEncounter[4] == DONE)
					    HandleGameObject(uiArchivumDoor,true);
				    break;
			    case GO_Hodir_Door_1:
				    uiHodirDoor1 = pGo->GetGUID();
				    break ;
			    case GO_Hodir_Door_2:
				    uiHodirDoor2 = pGo->GetGUID();
				    if (uiEncounter[TYPE_HODIR] == DONE)
					    HandleGameObject(uiHodirDoor2,true);
				    break ;
			    case GO_Hodir_Door_3:
				    uiHodirDoor3 = pGo->GetGUID();
				    if (uiEncounter[TYPE_HODIR] == DONE)
					    HandleGameObject(uiHodirDoor3,true);
				    break ;
			    case GO_Vezax_Door:
				    uiVezaxDoor = pGo->GetGUID();
				    if (uiEncounter[11] == DONE)
					    HandleGameObject(uiVezaxDoor,true);
				    break;
			    case GO_Yogg_Saron_Door:
				    uiYoggSaronDoor = pGo->GetGUID();
				    break;
			    case GO_Gate_of_the_Keepers:
				    uiKeepersGate = pGo->GetGUID();
				    break;
			    case GO_Big_Red_Button:
				    uiBigRedButton = pGo->GetGUID();
				    break;
			    case GO_Storm_Beacon:
				    beaconGOList.push_back(pGo->GetGUID());
				    break;
                case GO_UNIVERSE_GLOBE:
                    uiAlgalonGlobeGUID = pGo->GetGUID();
                    break;
                case GO_UNIVERSE_FLOOR_1:
                    uiUniverFloor1GUID = pGo->GetGUID();
                    break;
                case GO_UNIVERSE_FLOOR_2:
                    uiUniverFloor2GUID = pGo->GetGUID();
                    break;
                case GO_ALGALON_TRAP_DOOR:
                    uiAlgalonTrapDoorGUID = pGo->GetGUID();
                    break;
            }
        }

        void ProcessEvent(GameObject* /*go*/, uint32 uiEventId)
        {
            // Flame Leviathan's Tower Event triggers
           Creature* pFlameLeviathan = instance->GetCreature(NPC_LEVIATHAN);

            if (pFlameLeviathan && pFlameLeviathan->isAlive()) //No leviathan, no event triggering ;)
                switch(uiEventId)
                {
                    case EVENT_TOWER_OF_STORMS_DESTROYED:
					    HandleGameObject(uiThorimTargetGUID,false);
                        uiLeviathanHardMode = uiLeviathanHardMode | HARD_MODE_THORIM;
                        //pFlameLeviathan->AI()->DoAction(1);
                        break;
                    case EVENT_TOWER_OF_FROST_DESTROYED:
                        HandleGameObject(uiHodirTargetGUID,false);
                        uiLeviathanHardMode = uiLeviathanHardMode | HARD_MODE_HODIR;
                        //pFlameLeviathan->AI()->DoAction(3);
                        break;
                    case EVENT_TOWER_OF_FLAMES_DESTROYED:
                        HandleGameObject(uiMimironTargetGUID,false);
                        uiLeviathanHardMode = uiLeviathanHardMode | HARD_MODE_MIMIRON;
                        //pFlameLeviathan->AI()->DoAction(2);
                        break;
                    case EVENT_TOWER_OF_LIFE_DESTROYED:
                        HandleGameObject(uiFreyaTargetGUID,false);
                        uiLeviathanHardMode = uiLeviathanHardMode | HARD_MODE_FREYA;
                        //pFlameLeviathan->AI()->DoAction(4);
                        break;
                }
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (type)
            {
            case TYPE_LEVIATHAN:
                if (state == IN_PROGRESS)
                {
                    HandleGameObject(uiLeviathanDoor[0],false);
                    HandleGameObject(uiLeviathanDoor[1],false);
                    HandleGameObject(uiLeviathanDoor[2],false);
                    HandleGameObject(uiLeviathanDoor[3],false);
                    HandleGameObject(uiLeviathanDoor[4],false);
                    HandleGameObject(uiLeviathanDoor[5],false);
                    HandleGameObject(uiLeviathanDoor[6],false);
                }
                else
                {
                    HandleGameObject(uiLeviathanDoor[0],true);
                    HandleGameObject(uiLeviathanDoor[1],true);
                    HandleGameObject(uiLeviathanDoor[2],true);
                    HandleGameObject(uiLeviathanDoor[3],true);
                    HandleGameObject(uiLeviathanDoor[4],true);
                    HandleGameObject(uiLeviathanDoor[5],true);
                    HandleGameObject(uiLeviathanDoor[6],true);
                }
                if(state == NOT_STARTED)
			    {
				    if(!vehicleList.empty())
				    {
					    for(std::list<uint64>::iterator itr = vehicleList.begin(); itr != vehicleList.end(); ++itr)
					    {
						    if(Creature* npc = instance->GetCreature(*itr))
						    {
							    npc->DisappearAndDie() ;
							    npc->Respawn() ;
						    }
					    }
				    }
                }
                break;
            case TYPE_RAZORSCALE:
            if (state == NOT_STARTED || state == FAIL)
                {
                    for (uint8 i = 0 ; i < 4 ; i++)
                    {
                        if (GameObject* pHarpoon = instance->GetGameObject(uiBrokenHarpoons[i]))
                            pHarpoon->SetPhaseMask(1, true);
                        if (GameObject* pHarpoon = instance->GetGameObject(uiHarpoonGuns[i]))
                        {
                            pHarpoon->SetPhaseMask(256, true);
                            pHarpoon->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                        }
                    }
                }
                break;
			case TYPE_XT002:
                if (state == IN_PROGRESS)
                    HandleGameObject(uiXT002Door,false);
                else
                    HandleGameObject(uiXT002Door,true);
                break;
            case TYPE_ASSEMBLY:
            if (state == IN_PROGRESS)
				    HandleGameObject(uiIronCouncilDoor,false);
                else
                {
				    if (uiEncounter[4] == DONE)
					    HandleGameObject(uiArchivumDoor,true);
				    HandleGameObject(uiIronCouncilDoor,true);
                }
                break;
            case TYPE_KOLOGARN:
                if (state == DONE)
                    {
                    if (KologarnChest)
                        KologarnChest->SetRespawnTime(KologarnChest->GetRespawnDelay());
                    HandleGameObject(uiKologarnBridgeGUID,false);
                }
                break;
            case TYPE_MIMIRON:
			    if(state == NOT_STARTED)
			    {
				    if(Creature* leviathan = instance->GetCreature(uiLeviathanMKIIGUID))
				    {
					    leviathan->RemoveCorpse() ;
					    leviathan->Respawn() ;
					    leviathan->GetMotionMaster()->MoveTargetedHome();
				    }
				    if (GameObject* bigRedButton = instance->GetGameObject(uiBigRedButton))
					    bigRedButton->SetGoState(GO_STATE_READY);
			    }
			    if (state == IN_PROGRESS)
                {
                    HandleGameObject(uiMimironDoors[0],false);
                    HandleGameObject(uiMimironDoors[1],false);
                    HandleGameObject(uiMimironDoors[2],false);
                }
                else
                {
                    HandleGameObject(uiMimironDoors[0],true);
                    HandleGameObject(uiMimironDoors[1],true);
                    HandleGameObject(uiMimironDoors[2],true);
                }
			    OpenKeepersGate();
                break;
            case TYPE_HODIR:
                if (state == DONE && HodirChest)
                {
				    HodirChest->SetRespawnTime(HodirChest->GetRespawnDelay());
                    HandleGameObject(uiHodirDoor2,true);
                    HandleGameObject(uiHodirDoor3,true);
                    HandleGameObject(uiHodirDoor1,true);
                }
			    if(state == NOT_STARTED)
                    HandleGameObject(uiHodirDoor1,true);
			    if (state == IN_PROGRESS)
                    HandleGameObject(uiHodirDoor1,false);
			    OpenKeepersGate();
                break;
            case TYPE_THORIM:
                if(state == NOT_STARTED)
			    {
				    if(!thorimNPCList.empty())
				    {
					    for(std::list<uint64>::iterator itr = thorimNPCList.begin(); itr != thorimNPCList.end(); ++itr)
					    {
						    if(Creature* npc = instance->GetCreature(*itr))
						    {
							    npc->DisappearAndDie() ;
							    npc->Respawn() ;
						    }
					    }
				    }
				    SetData(TYPE_RUNIC_DOOR, GO_STATE_READY);
				    SetData(TYPE_STONE_DOOR, GO_STATE_READY);
				    SetData(TYPE_PORTCULLIS, GO_STATE_READY);
			    }
			    OpenKeepersGate();
                break;
            case TYPE_FREYA:
                OpenKeepersGate();
                break;
             case TYPE_VEZAX:
			    if (state == DONE)
                {
				    HandleGameObject(uiVezaxDoor,true);
				    if (Creature* pFreyaGossip = instance->GetCreature(uiFreyaImageGUID))
                        pFreyaGossip->SetVisible(true);
				    if (Creature* pThorimGossip = instance->GetCreature(uiThorimImageGUID))
                        pThorimGossip->SetVisible(true);
				    if (Creature* pHodirGossip = instance->GetCreature(uiHodirImageGUID))
                        pHodirGossip->SetVisible(true);
				    if (Creature* pMimironGossip = instance->GetCreature(uiMimironImageGUID))
                        pMimironGossip->SetVisible(true);
                }
                break;
            case TYPE_YOGGSARON:
			    if (state == IN_PROGRESS)
				    HandleGameObject(uiYoggSaronDoor, false);
                else
                {
				    if (state == NOT_STARTED)
					    if(Creature* sara = instance->GetCreature(uiSaraGUID))
						    if (!sara->isAlive())
							    sara->Respawn(true);
				    HandleGameObject(uiYoggSaronDoor, true);
                }
                break;
            case TYPE_ALGALON:
		        if (state == IN_PROGRESS)
			        HandleGameObject(uiAlgalonDoor, false);
                else
                {
			        HandleGameObject(uiAlgalonDoor, true);
                    HandleGameObject(uiAlgalonGlobeGUID, false);
                    HandleGameObject(uiAlgalonTrapDoorGUID, false);
                }
                break;
            }
            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch(type)
            {
			    case TYPE_HARPOON:
                    if (data == DONE)
                    {
                        for (uint8 i = 0 ; i < 4 ; i++)
                        {
                            if (GameObject* pHarpoon = instance->GetGameObject(uiBrokenHarpoons[i]))
                                pHarpoon->SetPhaseMask(1, true);
                            if (GameObject* pHarpoon = instance->GetGameObject(uiHarpoonGuns[i]))
                            {
                                Position pHarpoonPosition;
                                pHarpoon->GetPosition(&pHarpoonPosition);
                                if (Creature* pHarpoonFireState = instance->SummonCreature(33282, pHarpoonPosition, 0, 10000))
                                    pHarpoon->CastSpell(pHarpoonFireState, 62696);
                                pHarpoon->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                                pHarpoon->SetPhaseMask(256, true);                                
                            }
                        }
                    }
                    if (data == IN_PROGRESS)
                    {
                        for (uint8 i = 0 ; i < 4 ; i++)
                        {
                            if (GameObject* pHarpoon = instance->GetGameObject(uiBrokenHarpoons[i]))
                            {
                                if (pHarpoon->GetPhaseMask() == 1)
                                {
                                    pHarpoon->SetPhaseMask(256, true);
                                    if (GameObject* pHarpoon = instance->GetGameObject(uiHarpoonGuns[i]))
                                        pHarpoon->SetPhaseMask(1, true);
                                    break;
                                }
                            }
                        }
                    }
                    break;
                case TYPE_ALGALON:
                    if (data == TO_BE_DECIDED)
                    {
                        if (Creature* pAlgalon = instance->GetCreature(uiAlgalonGUID))
                            pAlgalon->ForcedDespawn(360000);
                        HandleGameObject(uiAlgalonDoor,true);
                    }
                    break;
                case TYPE_COLOSSUS:
                    uiEncounter[TYPE_COLOSSUS] = data;
                    if (data == 2)
                    {
                        if (Creature* pBoss = instance->GetCreature(uiLeviathanGUID))
                            pBoss->AI()->DoAction(10);
                        if (GameObject* pGate = instance->GetGameObject(uiLeviathanGateGUID))
                            pGate->SetGoState(GO_STATE_ACTIVE);
                    }
                    break;
                case TYPE_RUNIC_DOOR:
                    if (data == GO_STATE_READY)
					    HandleGameObject(uiRunicDoor,false);
                    else
					    HandleGameObject(uiRunicDoor,true);
                    break;
				case TYPE_STONE_DOOR:
                    if (data == GO_STATE_READY)
					    HandleGameObject(uiStoneDoor,false);
                    else
					    HandleGameObject(uiStoneDoor,true);
                    break;
			    case TYPE_PORTCULLIS:
                    if (data == GO_STATE_READY)
					    HandleGameObject(uiDarkIronProtcullis,false);
                    else
					    HandleGameObject(uiDarkIronProtcullis,true);
                    break;
			    case TYPE_MIMIRON_ELEVATOR:
				    if (GameObject* elevator = instance->GetGameObject(uiMimironElevatorGUID))
					    elevator->SetGoState(GOState(data));
                    break;
			    case TYPE_MIMIRON_TRAM:
				    if (GameObject* MimironTram = instance->GetGameObject(uiMimironTramGUID))
				    {
					    if (MimironTram && instance)
					    {
						    // Load Mimiron Tram (unfortunally only server side)
						    instance->LoadGrid(2307.0f, 284.632f);
    	                
						    if (data == 0)
							    MimironTram->SetGoState(GO_STATE_READY);
						    if (data == 1)
							    MimironTram->SetGoState(GO_STATE_ACTIVE);
    	                    
						    // Send movement update to players
						    if (Map* pMap = MimironTram->GetMap())
							    if (pMap->IsDungeon())
							    {
								    Map::PlayerList const &PlayerList = pMap->GetPlayers();

								    if (!PlayerList.isEmpty())
									    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
										    if (i->getSource())
										    {
											    UpdateData data;
											    WorldPacket pkt;
											    MimironTram->BuildValuesUpdateBlockForPlayer(&data, i->getSource());
											    data.BuildPacket(&pkt);
											    i->getSource()->GetSession()->SendPacket(&pkt);
										    }
							    }
					    }
				    }
                    break;
                case TYPE_LEVIATHAN_HARD_MODE:
                    uiLeviathanHardMode = data;
                    break;
                case TYPE_HODIR_RARE_CHEST:
                    if (HodirRareChest && data == GO_STATE_READY)
                    {
                        HodirRareChest->RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_UNK1);
                        HodirRareChest->AddLootMode(2);
                        HodirRareChest->SetRespawnTime(HodirChest->GetRespawnDelay());
                    }
                    break;
                case TYPE_ULDUAR_EVENT:
                    uiUlduarEvent = data;
                    break;
                case TYPE_UNIVERSE_GLOBE:
                    if (data = GO_STATE_ACTIVE)
                    {
                        HandleGameObject(uiAlgalonGlobeGUID, true);
                        HandleGameObject(uiAlgalonTrapDoorGUID, true);
                    }
                    break;
            }
        }

        uint64 GetData64(uint32 data)
        {
            switch(data)
            {
                case DATA_LEVIATHAN:            return uiLeviathanGUID;
                case DATA_IGNIS:                return uiIgnisGUID;
                case DATA_RAZORSCALE:           return uiRazorscaleGUID;
                case DATA_XT002:                return uiXT002GUID;
                case DATA_KOLOGARN:             return uiKologarnGUID;
			    case DATA_LEFT_ARM:				return uiLeftArmGUID;
			    case DATA_RIGHT_ARM:			return uiRightArmGUID;
                case DATA_AURIAYA:              return uiAuriayaGUID;
                case DATA_MIMIRON:              return uiMimironGUID;
                case DATA_HODIR:                return uiHodirGUID;
                case DATA_THORIM:               return uiThorimGUID;
                case DATA_FREYA:                return uiFreyaGUID;
                case DATA_VEZAX:                return uiVezaxGUID;
                case DATA_ALGALON:              return uiAlgalonGUID;

                case DATA_YS_FREYA:             return uiFreyaYS;
                case DATA_YS_THORIM:            return uiThorimYS;
                case DATA_YS_MIMIRON:           return uiMimironYS;
                case DATA_YS_HODIR:             return uiHodirYS;
			    case DATA_YOGG_SARON:			return uiYoggSaronGUID;
			    case DATA_YOGG_SARON_BRAIN:		return uiYoggSaronBrainGUID;

                // razorscale expedition commander
                case DATA_EXP_COMMANDER:        return uiExpCommanderGUID;
                // Assembly of Iron
                case DATA_STEELBREAKER:         return uiSteelbreakerGUID;
                case DATA_MOLGEIM:              return uiMolgeimGUID;
                case DATA_BRUNDIR:              return uiBrundirGUID;

			    // Hodir
			    case DATA_RUNIC_COLOSSUS:		return uiRunicColossusGUID;
			    case DATA_RUNE_GIANT:			return uiAncientRuneGiantGUID;

			    case DATA_LEVIATHAN_MK_II:		return uiLeviathanMKIIGUID ;

			    case DATA_VX_001:				return uiVX001;
	            case DATA_AERIAL_UNIT:          return uiAerialUnit;
	            case DATA_MAGNETIC_CORE:        return uiMagneticCore;

			    // Freya
			    case DATA_ELDER_BRIGHTLEAF:		return uiBrightleafGUID ;
			    case DATA_ELDER_IRONBRANCH:		return uiIronbranchGUID ;
			    case DATA_ELDER_STONEBARK:		return uiStonebarkGUID ;
            }

            return 0;
        }

        bool CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* /*source*/, Unit const* /*target*/, uint32 /*miscvalue1*/)
        {
            switch (criteria_id)
            {
                case ACHIEVEMENT_CRITERIA_HOT_POCKET_10:
                    return true;
                case ACHIEVEMENT_CRITERIA_HOT_POCKET_25:
                    return true;
                default:
                    break;
            }
            return false;
        }

        uint32 GetData(uint32 type)
        {
            switch(type)
            {
                case TYPE_COLOSSUS:
                    return uiEncounter[type];
				case TYPE_LEVIATHAN_HARD_MODE:
                    return uiLeviathanHardMode;
                case TYPE_ULDUAR_EVENT:
                    return uiUlduarEvent;
            }

            return 0;
        }

		void OpenKeepersGate()
	    {
		    if (uiEncounter[7] == DONE && uiEncounter[8] == DONE && uiEncounter[9] == DONE && uiEncounter[10] == DONE)
			    HandleGameObject(uiKeepersGate,true);
	    }

        void Update(uint32 diff)
        {
            if (!instance->HavePlayers())
                return;

            /*
            if (uiSummonTimer <= diff)
            {
                for (uint8 i = 0 ; i < 4 ; i++)
                    if (Creature* pLift = instance->SummonCreature(MOB_MECHANOLIFT, mechanoliftPos[i]))
                        pLift->AI()->SetData(0, i);
                uiSummonTimer = 3000;
            } else uiSummonTimer -= diff ;*/
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "U U " << GetBossSaveData() << " " << uiEncounter[14];

            OUT_SAVE_INST_DATA_COMPLETE;
            return saveStream.str();
        }

        void Load(const char* strIn)
        {
            if (!strIn)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(strIn);

            char dataHead1, dataHead2;

            std::istringstream loadStream(strIn);
            loadStream >> dataHead1 >> dataHead2;

            if (dataHead1 == 'U' && dataHead2 == 'U')
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                        tmpState = NOT_STARTED;
					sLog->outError("Load : %u",tmpState);
                    SetBossState(i, EncounterState(tmpState));
                }
				loadStream >> uiEncounter[14]; //colossus pre leviathan
            }
            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };

};

void AddSC_instance_ulduar()
{
    new instance_ulduar();
}
