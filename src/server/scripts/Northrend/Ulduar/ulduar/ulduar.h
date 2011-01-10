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

#ifndef DEF_ULDUAR_H
#define DEF_ULDUAR_H

enum eData
{
    TYPE_LEVIATHAN              ,
    TYPE_IGNIS                  ,
    TYPE_RAZORSCALE             ,
    TYPE_XT002                  ,
    TYPE_ASSEMBLY               ,
    TYPE_KOLOGARN               ,
    TYPE_AURIAYA                ,
    TYPE_MIMIRON                ,
    TYPE_HODIR                  ,
    TYPE_THORIM                 ,
    TYPE_FREYA                  ,
    TYPE_VEZAX                  ,
    TYPE_YOGGSARON              ,
    TYPE_ALGALON                ,
    MAX_ENCOUNTER               ,
    TYPE_COLOSSUS               ,
	TYPE_STONE_DOOR				,
	TYPE_RUNIC_DOOR				,
	TYPE_MIMIRON_ELEVATOR		,
	TYPE_MIMIRON_TRAM			,
	TYPE_PORTCULLIS				,
    TYPE_LEVIATHAN_HARD_MODE    ,
    TYPE_HODIR_RARE_CHEST       ,
    TYPE_ULDUAR_EVENT           ,
    TYPE_UNIVERSE_GLOBE         ,
    TYPE_HARPOON                ,
};

enum eData64
{
	DATA_LEVIATHAN              ,
    DATA_IGNIS                  ,
    DATA_RAZORSCALE             ,
    DATA_XT002                  ,
    DATA_KOLOGARN               ,
    DATA_AURIAYA                ,
	DATA_STEELBREAKER           ,
    DATA_MOLGEIM                ,
    DATA_BRUNDIR                ,
    DATA_MIMIRON                ,
    DATA_HODIR                  ,
    DATA_THORIM                 ,
    DATA_FREYA                  ,
    DATA_VEZAX                  ,
    DATA_ALGALON                ,
	DATA_RIGHT_ARM              ,
	DATA_LEFT_ARM               ,
	DATA_SENTRY_1				,
	DATA_SENTRY_2				,
	DATA_SENTRY_3				,
	DATA_SENTRY_4				,
	DATA_FERAL_DEFENDER			,
	DATA_ELDER_BRIGHTLEAF		,
	DATA_ELDER_IRONBRANCH		,
	DATA_ELDER_STONEBARK		,
	DATA_RUNIC_COLOSSUS			,
	DATA_RUNE_GIANT				,
	DATA_LEVIATHAN_MK_II		,
	DATA_VX_001					,
    DATA_AERIAL_UNIT			,
    DATA_MAGNETIC_CORE			,
	DATA_EXP_COMMANDER			,
    DATA_YS_FREYA               ,
    DATA_YS_THORIM              ,
    DATA_YS_MIMIRON             ,
    DATA_YS_HODIR               ,
	DATA_YOGG_SARON_BRAIN       ,
    DATA_YOGG_SARON             ,
};

enum eEntries
{
    NPC_LEVIATHAN               = 33113,
    NPC_IGNIS                   = 33118,
    NPC_RAZORSCALE              = 33186,
    NPC_COMMANDER               = 33210,
    NPC_XT002                   = 33293,
    NPC_STEELBREAKER            = 32867,
    NPC_MOLGEIM                 = 32927,
    NPC_BRUNDIR                 = 32857,
    NPC_KOLOGARN                = 32930,
	NPC_RIGHT_ARM               = 32934,
	NPC_LEFT_ARM                = 32933,
    NPC_AURIAYA                 = 33515,
	NPC_SANCTUM_SENTRY			= 34014,
	NPC_FERAL_DEFENDER			= 34035,
    NPC_MIMIRON                 = 33350,
    NPC_HODIR                   = 32845,
    NPC_THORIM                  = 32865,
    NPC_FREYA                   = 32906,
    NPC_VEZAX                   = 33271,
    NPC_YOGGSARON               = 33288,
    NPC_ALGALON                 = 32871,
	NPC_ELDER_BRIGHTLEAF		= 32915,
	NPC_ELDER_IRONBRANCH		= 32913,
	NPC_ELDER_STONEBARK			= 32914,
	NPC_THORIM_PHASE_TRIGGER	= 32892,
	NPC_LEVIATHAN_MK_II			= 33432,
	NPC_IRON_CONSTRUCT			= 33121,
	NPC_BRAIN_OF_YOGG_SARON		= 33890,
	NPC_YOGG_SARON				= 33288,
	NPC_SARA					= 33134,

    EVENT_TOWER_OF_STORMS_DESTROYED     = 21031,
    EVENT_TOWER_OF_FROST_DESTROYED     = 21032,
    EVENT_TOWER_OF_FLAMES_DESTROYED    = 21033,
    EVENT_TOWER_OF_LIFE_DESTROYED      = 21030,

	NPC_EIVI_NIGHTFEATHER		= 33325,
	NPC_ELLIE_NIGHTFEATHER		= 32901,

	NPC_ELEMENTALIST_MAHFUUN	= 33328,
	NPC_ELEMENTALIST_AVUUN		= 32900,

	NPC_MISSY_FLAMECUFFS		= 32893,
	NPC_SISSY_FLAMECUFFS		= 33327,

	NPC_FIELD_MEDIC_PENNY		= 32897,
	NPC_FIELD_MEDIC_JESSI		= 33326,

	NPC_TOR_GREYCLOUD			= 32941,
	NPC_KAR_GREYCLOUD			= 33333,

	NPC_SPIRITWALKER_TARA		= 33332,
	NPC_SPIRITWALKER_YONA		= 32950,

	NPC_AMIRA_BLAZEWEAVER		= 33331,
	NPC_VEESHA_BLAZEWEAVER		= 32946,

	NPC_BATTLE_PRIEST_ELIZA		= 32948,
	NPC_BATTLE_PRIEST_GINA		= 33330,

	NPC_CAPTURED_MERCENARY_CAPTAIN_H	= 32907,
	NPC_CAPTURED_MERCENARY_CAPTAIN_A	= 32908,
	NPC_CAPTURED_MERCENARY_SOLDIER_H	= 32883,
	NPC_CAPTURED_MERCENARY_SOLDIER_A	= 32885,
	NPC_IRON_RING_GUARD					= 32874,
	NPC_RUNIC_COLOSSUS					= 32872,
	NPC_DARK_RUNE_ACOLYTE				= 32886,
	NPC_JORMUNGAR_BEHEMOTH				= 32882,
	NPC_ANCIENT_RUNE_GIANT				= 32873,
	NPC_IRON_HONOR_GUARD				= 33125,

    NPC_VEHICLE_SIEGE                   = 33060,
    NPC_VEHICLE_CHOPPER                 = 33062,
    NPC_VEHICLE_DEMOLISHER              = 33109,
    
	NPC_THORIM_GOSSIP					= 33242,
	NPC_HODIR_GOSSIP					= 33213,
	NPC_MIMIRON_GOSSIP					= 33244,
	NPC_FREYA_GOSSIP					= 33241,

	GO_KOLOGARN_BRIDGE			= 194232,

	GO_MIMIRON_ELEVATOR			= 194749,

	GO_TOWER_OF_STORMS			= 194377,
	GO_TOWER_OF_FLAMES			= 194371,
	GO_TOWER_OF_FROST			= 194370,
	GO_TOWER_OF_LIFE			= 194375,

	GO_BROKEN_HARPOON			= 194565,
    GO_HARPOON_GUN_1            = 194543,
    GO_HARPOON_GUN_2            = 194542,
    GO_HARPOON_GUN_3            = 194541,
    GO_HARPOON_GUN_4            = 194519,

    GO_UNIVERSE_GLOBE           = 194148,
    GO_UNIVERSE_FLOOR_1         = 194715,
    GO_UNIVERSE_FLOOR_2         = 194716,
    GO_ALGALON_SIGIL_DOOR       = 194910,
    GO_ALGALON_TRAP_DOOR        = 194253,

    MOB_MECHANOLIFT             = 33214,
};

enum eAchievementCriteria
{
    ACHIEVEMENT_CRITERIA_HOT_POCKET_10                   = 10430, //ignis
    ACHIEVEMENT_CRITERIA_HOT_POCKET_25                   = 10431,
};

enum HardModes
{
    HARD_MODE_MIMIRON                           = 1,
    HARD_MODE_HODIR                             = 2,
    HARD_MODE_THORIM                            = 4,
    HARD_MODE_FREYA                             = 8
};
#endif
