/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Corpse.h"
#include "Player.h"
#include "Vehicle.h"
#include "MapManager.h"
#include "Transport.h"
#include "Battleground.h"
#include "WaypointMovementGenerator.h"
#include "InstanceSaveMgr.h"
#include "ObjectMgr.h"
#include "World.h"

//#define __ANTI_DEBUG__

#ifdef __ANTI_DEBUG__
#include "Chat.h"
std::string FlagsToStr(const uint32 Flags)
{
    std::string Ret="";
    if(Flags==0)
    {
        Ret="None";
        return Ret;
    }
    
    if(Flags & MOVEMENTFLAG_FORWARD)
    {   Ret+="FW "; }
    if(Flags & MOVEMENTFLAG_BACKWARD)
    {   Ret+="BW "; }
    if(Flags & MOVEMENTFLAG_STRAFE_LEFT)
    {   Ret+="STL ";    }
    if(Flags & MOVEMENTFLAG_STRAFE_RIGHT)
    {   Ret+="STR ";    }
    if(Flags & MOVEMENTFLAG_LEFT)
    {   Ret+="LF "; }
    if(Flags & MOVEMENTFLAG_RIGHT)
    {   Ret+="RI "; }
    if(Flags & MOVEMENTFLAG_PITCH_UP)
    {   Ret+="PTUP ";   }
    if(Flags & MOVEMENTFLAG_PITCH_DOWN)
    {   Ret+="PTDW ";   }
    if(Flags & MOVEMENTFLAG_WALK_MODE)
    {   Ret+="WALK ";   }
    if(Flags & MOVEMENTFLAG_ONTRANSPORT)
    {   Ret+="TRANS ";  }
    if(Flags & MOVEMENTFLAG_LEVITATING)
    {   Ret+="LEVI ";   }
    if(Flags & MOVEMENTFLAG_FLY_UNK1)
    {   Ret+="FLYUNK1 ";    }
    if(Flags & MOVEMENTFLAG_JUMPING)
    {   Ret+="JUMP ";   }
    if(Flags & MOVEMENTFLAG_UNK4)
    {   Ret+="UNK4 ";   }
    if(Flags & MOVEMENTFLAG_FALLING)
    {   Ret+="FALL ";   }
    if(Flags & MOVEMENTFLAG_SWIMMING)
    {   Ret+="SWIM ";   }
    if(Flags & MOVEMENTFLAG_FLY_UP)
    {   Ret+="FLYUP ";  }
    if(Flags & MOVEMENTFLAG_CAN_FLY)
    {   Ret+="CFLY ";   }
    if(Flags & MOVEMENTFLAG_FLYING)
    {   Ret+="FLY ";    }
    if(Flags & MOVEMENTFLAG_FLYING2)
    {   Ret+="FLY2 ";   }
    if(Flags & MOVEMENTFLAG_WATERWALKING)
    {   Ret+="WTWALK "; }
    if(Flags & MOVEMENTFLAG_SAFE_FALL)
    {   Ret+="SAFE ";   }
    if(Flags & MOVEMENTFLAG_UNK3)
    {   Ret+="UNK3 ";   }
    if(Flags & MOVEMENTFLAG_SPLINE)
    {   Ret+="SPLINE ";     }
    if(Flags & MOVEMENTFLAG_SPLINE2)
    {   Ret+="SPLINE2 ";    }
    
    return Ret;
}
#endif // __ANTI_DEBUG__

bool WorldSession::Anti__ReportCheat(const char* Reason,float Speed,const char* Op,float Val1,uint32 Val2,MovementInfo* MvInfo)
{
    if(!Reason)
    {
        sLog->outError("Anti__ReportCheat: Missing Reason parameter!");
        return false;
    }

    const char* Player = GetPlayer()->GetName();
    uint32 Acc = GetPlayer()->GetSession()->GetAccountId();
    uint32 Map = GetPlayer()->GetMapId();
    uint32 zone_id = GetPlayer()->GetZoneId();
    uint32 area_id = GetPlayer()->GetAreaId();
    float startX, startY, startZ = 0.0f;
    float endX, endY, endZ = 0.0f;
    uint32 fallTime = 0;
    uint32 t_guid = 0;
    uint32 flags = 0;

    MapEntry const* mapEntry = sMapStore.LookupEntry(GetPlayer()->GetMapId());
    AreaTableEntry const* zoneEntry = GetAreaEntryByAreaID(zone_id);
    AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(area_id);

    std::string mapName(mapEntry ? mapEntry->name[GetSessionDbcLocale()] : "<unknown>");
    std::string zoneName(zoneEntry ? zoneEntry->area_name[GetSessionDbcLocale()] : "<unknown>");
    std::string areaName(areaEntry ? areaEntry->area_name[GetSessionDbcLocale()] : "<unknown>");

    CharacterDatabase.escape_string(mapName);
    CharacterDatabase.escape_string(zoneName);
    CharacterDatabase.escape_string(areaName);

    if(!Player)
    {
        sLog->outError("Anti__ReportCheat: Player with no name?!?");
        return false;
    }

    QueryResult Res=CharacterDatabase.PQuery("SELECT speed,Val1 FROM cheaters WHERE player='%s' AND reason LIKE '%s' AND Map='%u' AND last_date >= NOW()-300",Player,Reason,Map);
    if(Res)
    {
        Field* Fields = Res->Fetch();
        
        std::stringstream Query;
        Query << "UPDATE cheaters SET count=count+1,last_date=NOW()";
        Query.precision(5);
        if(Speed>0.0f && Speed > Fields[0].GetFloat())
        {
            Query << ",speed='";
            Query << std::fixed << Speed;
            Query << "'";
        }

        if(Val1>0.0f && Val1 > Fields[1].GetFloat())
        {
            Query << ",Val1='";
            Query << std::fixed << Val1;
            Query << "'";
        }
        
        Query << " WHERE player='" << Player << "' AND reason='" << Reason << "' AND Map='" << Map << "' AND last_date >= NOW()-300 ORDER BY entry DESC LIMIT 1";
        
        CharacterDatabase.Execute(Query.str().c_str());
		
		 sWorld->SendGMText(12000,GetPlayer()->GetName(),GetPlayer()->GetGUIDLow(),Reason);
	
		
    }
    else
    {
        if(!Op)
        {   Op="";  }

        startX = GetPlayer()->GetPositionX();
        startY = GetPlayer()->GetPositionY();
        startZ = GetPlayer()->GetPositionZ();

        if(MvInfo)
        {
            fallTime = MvInfo->fallTime;
            flags = MvInfo->flags;
            t_guid = MvInfo->t_guid;

            endX = MvInfo->pos.GetPositionX();
            endY = MvInfo->pos.GetPositionY();
            endZ = MvInfo->pos.GetPositionZ();
        }

        CharacterDatabase.PExecute("INSERT INTO cheaters (player,acctid,reason,speed,count,first_date,last_date,Op,Val1,Val2,Map,mapEntry,zone_id,zoneEntry,area_id,areaEntry,Level,startX,startY,startZ,endX,endY,endZ,t_guid,flags,fallTime) "
                                   "VALUES ('%s','%u','%s','%f','1',NOW(),NOW(),'%s','%f','%u','%u','%s','%u','%s','%u','%s','%u','%f','%f','%f','%f','%f','%f','%u','%u','%u')",
                                   Player,Acc,Reason,Speed,Op,Val1,Val2,
                                   Map, mapName.c_str(),
                                   zone_id, zoneName.c_str(),
                                   area_id, areaName.c_str(),
                                   GetPlayer()->getLevel(),
                                   startX,startY,startZ,
                                   endX,endY,endZ,
                                   t_guid,flags,t_guid);
								   
					  sWorld->SendGMText(12000,GetPlayer()->GetName(),GetPlayer()->GetGUIDLow(),Reason);			    
									
    }

    if(sWorld->GetMvAnticheatKill() && GetPlayer()->isAlive())
    {
        GetPlayer()->DealDamage(GetPlayer(), GetPlayer()->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }
    if(sWorld->GetMvAnticheatKick())
    {
        GetPlayer()->GetSession()->KickPlayer();
    }
    if(sWorld->GetMvAnticheatBan() & 1)
    {
        sWorld->BanAccount(BAN_CHARACTER,Player,sWorld->GetMvAnticheatBanTime(),"Чит","Античит");
    }
    if(sWorld->GetMvAnticheatBan() & 2)
    {
        QueryResult result = LoginDatabase.PQuery("SELECT last_ip FROM account WHERE id=%u", Acc);
        if(result)
        {

            Field *fields = result->Fetch();
            std::string LastIP = fields[0].GetString();
            if(!LastIP.empty())
            {
                sWorld->BanAccount(BAN_IP,LastIP,sWorld->GetMvAnticheatBanTime(),"Cheat","Anticheat");
            }
        }
    }
    return true;
}

bool WorldSession::Anti__CheatOccurred(uint32 CurTime,const char* Reason,float Speed,const char* Op,
                                float Val1,uint32 Val2,MovementInfo* MvInfo)
{
    if(!Reason)
    {
        sLog->outError("Anti__CheatOccurred: Missing Reason parameter!");
        return false;
    }
    
    GetPlayer()->m_anti_lastalarmtime = CurTime;
    GetPlayer()->m_anti_alarmcount = GetPlayer()->m_anti_alarmcount + 1;

    if (GetPlayer()->m_anti_alarmcount > sWorld->GetMvAnticheatAlarmCount())
    {
        Anti__ReportCheat(Reason,Speed,Op,Val1,Val2,MvInfo);
        return true;
    }
    return false;
}



void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket & /*recv_data*/)
{
    sLog->outDebug("WORLD: got MSG_MOVE_WORLDPORT_ACK.");
    HandleMoveWorldportAckOpcode();
}

void WorldSession::HandleMoveWorldportAckOpcode()
{
    // ignore unexpected far teleports
    if (!GetPlayer()->IsBeingTeleportedFar())
        return;

    // get the teleport destination
    WorldLocation &loc = GetPlayer()->GetTeleportDest();

    // possible errors in the coordinate validity check
    if (!MapManager::IsValidMapCoord(loc))
    {
        LogoutPlayer(false);
        return;
    }

    // get the destination map entry, not the current one, this will fix homebind and reset greeting
    MapEntry const* mEntry = sMapStore.LookupEntry(loc.GetMapId());
    InstanceTemplate const* mInstance = ObjectMgr::GetInstanceTemplate(loc.GetMapId());

    // reset instance validity, except if going to an instance inside an instance
    if (GetPlayer()->m_InstanceValid == false && !mInstance)
        GetPlayer()->m_InstanceValid = true;

    GetPlayer()->SetSemaphoreTeleportFar(false);

    Map * oldMap = GetPlayer()->GetMap();
    ASSERT(oldMap);
    if (GetPlayer()->IsInWorld())
    {
        sLog->outCrash("Player is still in world when teleported from map %u! to new map %u", oldMap->GetId(), loc.GetMapId());
        oldMap->Remove(GetPlayer(), false);
    }

    // relocate the player to the teleport destination
    Map * newMap = sMapMgr->CreateMap(loc.GetMapId(), GetPlayer(), 0);
    // the CanEnter checks are done in TeleporTo but conditions may change
    // while the player is in transit, for example the map may get full
    if (!newMap || !newMap->CanEnter(GetPlayer()))
    {
        sLog->outError("Map %d could not be created for player %d, porting player to homebind", loc.GetMapId(), GetPlayer()->GetGUIDLow());
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }
    else
        GetPlayer()->Relocate(&loc);

    GetPlayer()->ResetMap();
    GetPlayer()->SetMap(newMap);
	
	GetPlayer()->m_anti_TeleTime=time(NULL);

	
	
    GetPlayer()->SendInitialPacketsBeforeAddToMap();
    if (!GetPlayer()->GetMap()->Add(GetPlayer()))
    {
        sLog->outError("WORLD: failed to teleport player %s (%d) to map %d because of unknown reason!", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow(), loc.GetMapId());
        GetPlayer()->ResetMap();
        GetPlayer()->SetMap(oldMap);
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }

    // battleground state prepare (in case join to BG), at relogin/tele player not invited
    // only add to bg group and object, if the player was invited (else he entered through command)
    if (_player->InBattleground())
    {
        // cleanup setting if outdated
        if (!mEntry->IsBattlegroundOrArena())
        {
            // We're not in BG
            _player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);
            // reset destination bg team
            _player->SetBGTeam(0);
        }
        // join to bg case
        else if (Battleground *bg = _player->GetBattleground())
        {
            if (_player->IsInvitedForBattlegroundInstance(_player->GetBattlegroundId()))
                bg->AddPlayer(_player);
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    // flight fast teleport case
    if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (!_player->InBattleground())
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());
            flight->Initialize(*GetPlayer());
            return;
        }

        // battleground state prepare, stop flight
        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->CleanupAfterTaxiFlight();
    }

    // resurrect character at enter into instance where his corpse exist after add to map
    Corpse *corpse = GetPlayer()->GetCorpse();
    if (corpse && corpse->GetType() != CORPSE_BONES && corpse->GetMapId() == GetPlayer()->GetMapId())
    {
        if (mEntry->IsDungeon())
        {
            GetPlayer()->ResurrectPlayer(0.5f,false);
            GetPlayer()->SpawnCorpseBones();
        }
    }

    bool allowMount = !mEntry->IsDungeon() || mEntry->IsBattlegroundOrArena();
    if (mInstance)
    {
        Difficulty diff = GetPlayer()->GetDifficulty(mEntry->IsRaid());
        if (MapDifficulty const* mapDiff = GetMapDifficultyData(mEntry->MapID, diff))
        {
            if (mapDiff->resetTime)
            {
                if (time_t timeReset = sInstanceSaveMgr->GetResetTimeFor(mEntry->MapID, diff))
                {
                    uint32 timeleft = uint32(timeReset - time(NULL));
                    GetPlayer()->SendInstanceResetWarning(mEntry->MapID, diff, timeleft);
                }
            }
        }
        allowMount = mInstance->allowMount;
    }

    // mount allow check
    if (!allowMount)
        _player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    // update zone immediately, otherwise leave channel will cause crash in mtmap
    uint32 newzone, newarea;
    GetPlayer()->GetZoneAndAreaId(newzone, newarea);
    GetPlayer()->UpdateZone(newzone, newarea);

    // honorless target
    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);

    // in friendly area
    else if (GetPlayer()->IsPvP() && !GetPlayer()->HasFlag(PLAYER_FLAGS,PLAYER_FLAGS_IN_PVP))
        GetPlayer()->UpdatePvP(false, false);

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();
  

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAck(WorldPacket& recv_data)
{
    sLog->outDebug("MSG_MOVE_TELEPORT_ACK");
    uint64 guid;

    recv_data.readPackGUID(guid);

    uint32 flags, time;
    recv_data >> flags >> time;
    sLog->outStaticDebug("Guid " UI64FMTD, guid);
    sLog->outStaticDebug("Flags %u, time %u", flags, time/IN_MILLISECONDS);

    Unit *mover = _player->m_mover;
    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    if (!plMover || !plMover->IsBeingTeleportedNear())
        return;

    if (guid != plMover->GetGUID())
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    plMover->SetPosition(dest,true);

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    // new zone
    if (old_zone != newzone)
    {
        // honorless target
        if (plMover->pvpInfo.inHostileArea)
            plMover->CastSpell(plMover, 2479, true);

        // in friendly area
        else if (plMover->IsPvP() && !plMover->HasFlag(PLAYER_FLAGS,PLAYER_FLAGS_IN_PVP))
            plMover->UpdatePvP(false, false);
    }

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();


    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMovementOpcodes(WorldPacket & recv_data)
{
    uint16 opcode = recv_data.GetOpcode();
    recv_data.hexlike();

    Unit *mover = _player->m_mover;

    ASSERT(mover != NULL);                                  // there must always be a mover

    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (plMover && plMover->IsBeingTeleported())
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    /* extract packet */
    uint64 guid;

    recv_data.readPackGUID(guid);

    MovementInfo movementInfo;
    movementInfo.guid = guid;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data.rpos(recv_data.wpos());                   // prevent warnings spam

    // prevent tampered movement data
    if (guid != mover->GetGUID())
        return;

    if (!movementInfo.pos.IsPositionValid())
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    /* handle special cases */
    if (movementInfo.flags & MOVEMENTFLAG_ONTRANSPORT)
    {
        // transports size limited
        // (also received at zeppelin leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
        if (movementInfo.t_pos.GetPositionX() > 50 || movementInfo.t_pos.GetPositionY() > 50 || movementInfo.t_pos.GetPositionZ() > 50)
        {
            recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
            return;
        }

        if (!Trinity::IsValidMapCoord(movementInfo.pos.GetPositionX() + movementInfo.t_pos.GetPositionX(), movementInfo.pos.GetPositionY() + movementInfo.t_pos.GetPositionY(),
            movementInfo.pos.GetPositionZ() + movementInfo.t_pos.GetPositionZ(), movementInfo.pos.GetOrientation() + movementInfo.t_pos.GetOrientation()))
        {
            recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
            return;
        }

        // if we boarded a transport, add us to it
        if (plMover && !plMover->GetTransport())
        {
            float trans_rad = movementInfo.t_pos.GetPositionX()*movementInfo.t_pos.GetPositionX() + movementInfo.t_pos.GetPositionY()*movementInfo.t_pos.GetPositionY() + movementInfo.t_pos.GetPositionZ()*movementInfo.t_pos.GetPositionZ();
            if (trans_rad > 3600.0f) // transport radius = 60 yards //cheater with on_transport_flag
            {
	            return;
            }
            // elevators also cause the client to send MOVEMENTFLAG_ONTRANSPORT - just unmount if the guid can be found in the transport list
            for (MapManager::TransportSet::const_iterator iter = sMapMgr->m_Transports.begin(); iter != sMapMgr->m_Transports.end(); ++iter)
            {
                if ((*iter)->GetGUID() == movementInfo.t_guid)
                {
                    plMover->m_transport = (*iter);
                    (*iter)->AddPassenger(plMover);
                    break;
                }
            }
        }

        if (!mover->GetTransport() && !mover->GetVehicle())
        {
            GameObject *go = mover->GetMap()->GetGameObject(movementInfo.t_guid);
            if (!go || go->GetGoType() != GAMEOBJECT_TYPE_TRANSPORT)
                movementInfo.flags &= ~MOVEMENTFLAG_ONTRANSPORT;
        }
    }
    else if (plMover && plMover->GetTransport())                // if we were on a transport, leave
    {
        plMover->m_transport->RemovePassenger(plMover);
        plMover->m_transport = NULL;
        movementInfo.t_pos.Relocate(0.0f, 0.0f, 0.0f, 0.0f);
        movementInfo.t_time = 0;
        movementInfo.t_seat = -1;
    }

    // fall damage generation (ignore in flight case that can be triggered also at lags in moment teleportation to another map).
    if (opcode == MSG_MOVE_FALL_LAND && plMover && !plMover->isInFlight())
        plMover->HandleFall(movementInfo);

    if (plMover && ((movementInfo.flags & MOVEMENTFLAG_SWIMMING) != 0) != plMover->IsInWater())
    {
        // now client not include swimming flag in case jumping under water
        plMover->SetInWater(!plMover->IsInWater() || plMover->GetBaseMap()->IsUnderWater(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ()));
        if(plMover->GetBaseMap()->IsUnderWater(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ()-7.0f))
        {
            plMover->m_anti_BeginFallZ=INVALID_HEIGHT;
        }
     }
 

    // ---- anti-cheat features -->>>
  

    if (plMover && (plMover->m_transport == 0) && sWorld->GetMvAnticheatEnable() &&
        GetPlayer()->GetSession()->GetSecurity() <= sWorld->GetMvAnticheatGmLevel() &&
        GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()!=FLIGHT_MOTION_TYPE &&
       
    GetPlayer()) {
        const uint32 CurTime=getMSTime();
        if(getMSTimeDiff(GetPlayer()->m_anti_lastalarmtime,CurTime) > sWorld->GetMvAnticheatAlarmPeriod())
        {
            GetPlayer()->m_anti_alarmcount = 0;
        }
        /* I really don't care about movement-type yet (todo)
        UnitMoveType move_type;

        if (movementInfo.flags & MOVEMENTFLAG_FLYING) move_type = MOVE_FLY;
        else if (movementInfo.flags & MOVEMENTFLAG_SWIMMING) move_type = MOVE_SWIM;
        else if (movementInfo.flags & MOVEMENTFLAG_WALK_MODE) move_type = MOVE_WALK;
        else move_type = MOVE_RUN;*/

        float delta_x = GetPlayer()->GetPositionX() - movementInfo.pos.GetPositionX();
        float delta_y = GetPlayer()->GetPositionY() - movementInfo.pos.GetPositionY();
        // float delta_z = GetPlayer()->GetPositionZ() - movementInfo.pos.GetPositionZ();
        float delta = sqrt(delta_x * delta_x + delta_y * delta_y); // Len of movement-vector via Pythagoras (a^2+b^2=Len^2)
        // float tg_z = 0.0f; //tangens
        float delta_t = getMSTimeDiff(GetPlayer()->m_anti_lastmovetime,CurTime);
        
		GetPlayer()->m_anti_lastmovetime = CurTime;
        GetPlayer()->m_anti_MovedLen += delta;

        if(delta_t > 15000.0f)
        {   delta_t = 15000.0f;   }

        // Tangens of walking angel
        /*if (!(movementInfo.flags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_SWIMMING)))
        {
            //Mount hack detection currently disabled
            tg_z = ((delta !=0.0f) && (delta_z > 0.0f)) ? (atan((delta_z*delta_z) / delta) * 180.0f / M_PI) : 0.0f;
        }*/

        //antiOFF fall-damage, MOVEMENTFLAG_FALLING seted by client if player try movement when falling and unset in this case the MOVEMENTFLAG_FALLING flag.
       
		if((GetPlayer()->m_anti_BeginFallZ == INVALID_HEIGHT) &&
           (movementInfo.flags & (MOVEMENTFLAG_FALLING | MOVEMENTFLAG_PENDING_STOP)) != 0)
        {
            GetPlayer()->m_anti_BeginFallZ=(float)(movementInfo.pos.GetPositionZ());
        }

        if(GetPlayer()->m_anti_NextLenCheck <= CurTime)
        {
            // Check every 500ms is a lot more advisable then 1000ms, because normal movment packet arrives every 500ms
            uint32 OldNextLenCheck=GetPlayer()->m_anti_NextLenCheck;
            float delta_xyt=GetPlayer()->m_anti_MovedLen/(float)(getMSTimeDiff(OldNextLenCheck-500,CurTime));
            GetPlayer()->m_anti_NextLenCheck = CurTime+500;
            GetPlayer()->m_anti_MovedLen = 0.0f;
            static const float MaxDeltaXYT = sWorld->GetMvAnticheatMaxXYT();

#ifdef __ANTI_DEBUG__
            SendAreaTriggerMessage("XYT: %f ; Flags: %s",delta_xyt,FlagsToStr(movementInfo.flags).c_str());
#endif //__ANTI_DEBUG__
            
			if(delta_xyt > MaxDeltaXYT && delta<=100.0f && GetPlayer()->GetZoneId() != 2257 && GetPlayer()->GetZoneId() != 3992 && GetPlayer()->GetZoneId() != 2618)
            {
                Anti__CheatOccurred(CurTime,"Speed hack",delta_xyt,LookupOpcodeName(opcode),
                                    (float)(GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType()),
                                    (float)(getMSTimeDiff(OldNextLenCheck-500,CurTime)),&movementInfo);
            }
        }




        {
          
        }
        
      
		
        
        static const float DIFF_OVERGROUND = 10.0f;
        float Anti__GroundZ = GetPlayer()->GetMap()->GetHeight(GetPlayer()->GetPositionX(),GetPlayer()->GetPositionY(),MAX_HEIGHT);
        float Anti__FloorZ  = GetPlayer()->GetMap()->GetHeight(GetPlayer()->GetPositionX(),GetPlayer()->GetPositionY(),GetPlayer()->GetPositionZ());
        float Anti__MapZ = ((Anti__FloorZ <= (INVALID_HEIGHT+5.0f)) ? Anti__GroundZ : Anti__FloorZ) + DIFF_OVERGROUND;
        
        if(!GetPlayer()->CanFly() &&
           !GetPlayer()->GetBaseMap()->IsUnderWater(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ()-7.0f) &&
           Anti__MapZ < GetPlayer()->GetPositionZ() && Anti__MapZ > (INVALID_HEIGHT+DIFF_OVERGROUND + 5.0f))
        {
     //       static const float DIFF_AIRJUMP=25.0f; // 25 is realy high, but to many false positives...
            
			// Air-Jump-Detection definitively needs a better way to be detected...
            if((movementInfo.flags & (MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_DESCENDING | MOVEMENTFLAG_FLYING)) != 0) // Fly Hack
            {
                Anti__CheatOccurred(CurTime,"Fly hack",
                                    ((uint8)(GetPlayer()->HasAuraType(SPELL_AURA_FLY))) +
                                    ((uint8)(GetPlayer()->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED))*2),
                                    NULL,GetPlayer()->GetPositionZ()-Anti__MapZ);
            }
            
			
        }

      
    } 
	// <<---- anti-cheat features



    /* process position-change */
    WorldPacket data(opcode, recv_data.size());
    movementInfo.time = getMSTime();
    movementInfo.guid = mover->GetGUID();
    WriteMovementInfo(&data, &movementInfo);
    mover->SendMessageToSet(&data, _player);

    mover->m_movementInfo = movementInfo;

    // this is almost never true (not sure why it is sometimes, but it is), normally use mover->IsVehicle()
    if (mover->GetVehicle())
    {
        mover->SetOrientation(movementInfo.pos.GetOrientation());
        return;
    }

    mover->SetPosition(movementInfo.pos);

    if (plMover)                                            // nothing is charmed, or player charmed
    {
        plMover->UpdateFallInformationIfNeed(movementInfo, opcode);

        if (movementInfo.pos.GetPositionZ() < -500.0f)
        {
            if (!(plMover->InBattleground()
                && plMover->GetBattleground()
                && plMover->GetBattleground()->HandlePlayerUnderMap(_player)))
            {
                // NOTE: this is actually called many times while falling
                // even after the player has been teleported away
                // TODO: discard movement packets after the player is rooted
                if (plMover->isAlive())
                {
                    plMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, GetPlayer()->GetMaxHealth());
                    // pl can be alive if GM/etc
                    if (!plMover->isAlive())
                    {
                        // change the death state to CORPSE to prevent the death timer from
                        // starting in the next player update
                        plMover->KillPlayer();
                        plMover->BuildPlayerRepop();
                    }
                }

                // cancel the death timer here if started
                plMover->RepopAtGraveyard();
            }
        }
    }
}

void WorldSession::HandleForceSpeedChangeAck(WorldPacket &recv_data)
{
    uint32 opcode = recv_data.GetOpcode();
    sLog->outDebug("WORLD: Recvd %s (%u, 0x%X) opcode", LookupOpcodeName(opcode), opcode, opcode);

    /* extract packet */
    uint64 guid;
    uint32 unk1;
    float  newspeed;

    recv_data.readPackGUID(guid);

    // now can skip not our packet
    if (_player->GetGUID() != guid)
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    // continue parse packet

    recv_data >> unk1;                                      // counter or moveEvent

    MovementInfo movementInfo;
    movementInfo.guid = guid;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data >> newspeed;
    /*----------------*/

    // client ACK send one packet for mounted/run case and need skip all except last from its
    // in other cases anti-cheat check can be fail in false case
    UnitMoveType move_type;
    UnitMoveType force_move_type;

    static char const* move_type_name[MAX_MOVE_TYPE] = {  "Walk", "Run", "RunBack", "Swim", "SwimBack", "TurnRate", "Flight", "FlightBack", "PitchRate" };

    switch(opcode)
    {
        case CMSG_FORCE_WALK_SPEED_CHANGE_ACK:          move_type = MOVE_WALK;          force_move_type = MOVE_WALK;        break;
        case CMSG_FORCE_RUN_SPEED_CHANGE_ACK:           move_type = MOVE_RUN;           force_move_type = MOVE_RUN;         break;
        case CMSG_FORCE_RUN_BACK_SPEED_CHANGE_ACK:      move_type = MOVE_RUN_BACK;      force_move_type = MOVE_RUN_BACK;    break;
        case CMSG_FORCE_SWIM_SPEED_CHANGE_ACK:          move_type = MOVE_SWIM;          force_move_type = MOVE_SWIM;        break;
        case CMSG_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:     move_type = MOVE_SWIM_BACK;     force_move_type = MOVE_SWIM_BACK;   break;
        case CMSG_FORCE_TURN_RATE_CHANGE_ACK:           move_type = MOVE_TURN_RATE;     force_move_type = MOVE_TURN_RATE;   break;
        case CMSG_FORCE_FLIGHT_SPEED_CHANGE_ACK:        move_type = MOVE_FLIGHT;        force_move_type = MOVE_FLIGHT;      break;
        case CMSG_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK:   move_type = MOVE_FLIGHT_BACK;   force_move_type = MOVE_FLIGHT_BACK; break;
        case CMSG_FORCE_PITCH_RATE_CHANGE_ACK:          move_type = MOVE_PITCH_RATE;    force_move_type = MOVE_PITCH_RATE;  break;
        default:
            sLog->outError("WorldSession::HandleForceSpeedChangeAck: Unknown move type opcode: %u", opcode);
            return;
    }

    // skip all forced speed changes except last and unexpected
    // in run/mounted case used one ACK and it must be skipped.m_forced_speed_changes[MOVE_RUN} store both.
    if (_player->m_forced_speed_changes[force_move_type] > 0)
    {
        --_player->m_forced_speed_changes[force_move_type];
        if (_player->m_forced_speed_changes[force_move_type] > 0)
            return;
    }

    if (!_player->GetTransport() && fabs(_player->GetSpeed(move_type) - newspeed) > 0.01f)
    {
        if (_player->GetSpeed(move_type) > newspeed)         // must be greater - just correct
        {
            sLog->outError("%sSpeedChange player %s is NOT correct (must be %f instead %f), force set to correct value",
                move_type_name[move_type], _player->GetName(), _player->GetSpeed(move_type), newspeed);
            _player->SetSpeed(move_type,_player->GetSpeedRate(move_type),true);
        }
        else                                                // must be lesser - cheating
        {
            sLog->outBasic("Player %s from account id %u kicked for incorrect speed (must be %f instead %f)",
                _player->GetName(),_player->GetSession()->GetAccountId(),_player->GetSpeed(move_type), newspeed);
            _player->GetSession()->KickPlayer();
        }
    }
}

void WorldSession::HandleSetActiveMoverOpcode(WorldPacket &recv_data)
{
    sLog->outDebug("WORLD: Recvd CMSG_SET_ACTIVE_MOVER");

    uint64 guid;
    recv_data >> guid;

    if (GetPlayer()->IsInWorld())
    {
        if (Unit *mover = ObjectAccessor::GetUnit(*GetPlayer(), guid))
        {
            GetPlayer()->SetMover(mover);
            if (mover != GetPlayer() && mover->canFly())
            {
                WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 12);
                data.append(mover->GetPackGUID());
                data << uint32(0);
                SendPacket(&data);
            }
        }
        else
        {
            sLog->outError("HandleSetActiveMoverOpcode: incorrect mover guid: mover is " UI64FMTD " and should be " UI64FMTD, guid, _player->m_mover->GetGUID());
            GetPlayer()->SetMover(GetPlayer());
        }
    }
}

void WorldSession::HandleMoveNotActiveMover(WorldPacket &recv_data)
{
    sLog->outDebug("WORLD: Recvd CMSG_MOVE_NOT_ACTIVE_MOVER");

    uint64 old_mover_guid;
    recv_data.readPackGUID(old_mover_guid);

    MovementInfo mi;
    mi.guid = old_mover_guid;
    ReadMovementInfo(recv_data, &mi);

    _player->m_movementInfo = mi;
}

void WorldSession::HandleDismissControlledVehicle(WorldPacket &recv_data)
{
    sLog->outDebug("WORLD: Recvd CMSG_DISMISS_CONTROLLED_VEHICLE");
    recv_data.hexlike();

    uint64 vehicleGUID = _player->GetCharmGUID();

    if (!vehicleGUID)                                        // something wrong here...
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    uint64 guid;

    recv_data.readPackGUID(guid);

    MovementInfo mi;
    mi.guid = guid;
    ReadMovementInfo(recv_data, &mi);

    _player->m_movementInfo = mi;

    _player->ExitVehicle();
}

void WorldSession::HandleChangeSeatsOnControlledVehicle(WorldPacket &recv_data)
{
    sLog->outDebug("WORLD: Recvd CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE");
    recv_data.hexlike();

    Unit* vehicle_base = GetPlayer()->GetVehicleBase();
    if (!vehicle_base)
        return;

    VehicleSeatEntry const* seat = GetPlayer()->GetVehicle()->GetSeatForPassenger(GetPlayer());
    if (!seat->CanSwitchFromSeat())
    {
        sLog->outError("HandleChangeSeatsOnControlledVehicle, Opcode: %u, Player %u tried to switch seats but current seatflags %u don't permit that.",
            recv_data.GetOpcode(), GetPlayer()->GetGUIDLow(), seat->m_flags);
        return;
    }

    switch (recv_data.GetOpcode())
    {
        case CMSG_REQUEST_VEHICLE_PREV_SEAT:
            GetPlayer()->ChangeSeat(-1, false);
            break;
        case CMSG_REQUEST_VEHICLE_NEXT_SEAT:
            GetPlayer()->ChangeSeat(-1, true);
            break;
        case CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE:
            {
                uint64 guid;        // current vehicle guid
                recv_data.readPackGUID(guid);

                ReadMovementInfo(recv_data, &vehicle_base->m_movementInfo);

                uint64 accessory;        //  accessory guid
                recv_data.readPackGUID(accessory);

                int8 seatId;
                recv_data >> seatId;

                if (vehicle_base->GetGUID() != guid)
                    return;

                if (!accessory)
                    GetPlayer()->ChangeSeat(-1, seatId > 0); // prev/next
                else if (Unit *vehUnit = Unit::GetUnit(*GetPlayer(), accessory))
                {
                    if (Vehicle *vehicle = vehUnit->GetVehicleKit())
                        if (vehicle->HasEmptySeat(seatId))
                            GetPlayer()->EnterVehicle(vehicle, seatId);
                }
            }
            break;
        case CMSG_REQUEST_VEHICLE_SWITCH_SEAT:
            {
                uint64 guid;        // current vehicle guid
                recv_data.readPackGUID(guid);

                int8 seatId;
                recv_data >> seatId;

                if (vehicle_base->GetGUID() == guid)
                    GetPlayer()->ChangeSeat(seatId);
                else if (Unit *vehUnit = Unit::GetUnit(*GetPlayer(), guid))
                    if (Vehicle *vehicle = vehUnit->GetVehicleKit())
                        if (vehicle->HasEmptySeat(seatId))
                            GetPlayer()->EnterVehicle(vehicle, seatId);
            }
            break;
        default:
            break;
    }
}

void WorldSession::HandleEnterPlayerVehicle(WorldPacket &data)
{
    // Read guid
    uint64 guid;
    data >> guid;

    if (Player* pl=ObjectAccessor::FindPlayer(guid))
    {
        if (!pl->GetVehicleKit())
            return;
        if (!pl->IsInRaidWith(_player))
            return;
        if (!pl->IsWithinDistInMap(_player,INTERACTION_DISTANCE))
            return;
        _player->EnterVehicle(pl);
    }
}

void WorldSession::HandleEjectPassenger(WorldPacket &data)
{
    Vehicle* vehicle = _player->GetVehicleKit();
    if (!vehicle)
    {
        sLog->outError("HandleEjectPassenger: Player %u is not in a vehicle!", GetPlayer()->GetGUIDLow());
        return;
    }

    uint64 guid;
    data >> guid;

    if (IS_PLAYER_GUID(guid))
    {
        Player *plr = ObjectAccessor::FindPlayer(guid);
        if (!plr)
        {
            sLog->outError("Player %u tried to eject player %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        if (!plr->IsOnVehicle(vehicle->GetBase()))
        {
            sLog->outError("Player %u tried to eject player %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(plr);
        ASSERT(seat);
        if (seat->IsEjectable())
            plr->ExitVehicle();
        else
            sLog->outError("Player %u attempted to eject player %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
    }

    else if (IS_CREATURE_GUID(guid))
    {
        Unit *unit = ObjectAccessor::GetUnit(*_player, guid);
        if (!unit) // creatures can be ejected too from player mounts
        {
            sLog->outError("Player %u tried to eject creature guid %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        if (!unit->IsOnVehicle(vehicle->GetBase()))
        {
            sLog->outError("Player %u tried to eject unit %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(unit);
        ASSERT(seat);
        if (seat->IsEjectable())
        {
            ASSERT(GetPlayer() == vehicle->GetBase());
            unit->ExitVehicle();
            unit->ToCreature()->DespawnOrUnsummon(1000);
            ASSERT(!unit->IsOnVehicle(vehicle->GetBase()));
        }
        else
            sLog->outError("Player %u attempted to eject creature GUID %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
    }
    else
        sLog->outError("HandleEjectPassenger: Player %u tried to eject invalid GUID "UI64FMTD, GetPlayer()->GetGUIDLow(), guid);
}

void WorldSession::HandleRequestVehicleExit(WorldPacket &recv_data)
{
    sLog->outDebug("WORLD: Recvd CMSG_REQUEST_VEHICLE_EXIT");
    recv_data.hexlike();

    if (Vehicle* vehicle = GetPlayer()->GetVehicle())
    {
        if (VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(GetPlayer()))
        {
            if (seat->CanEnterOrExit())
                GetPlayer()->ExitVehicle();
            else
                sLog->outError("Player %u tried to exit vehicle, but seatflags %u (ID: %u) don't permit that.",
                    GetPlayer()->GetGUIDLow(), seat->m_ID, seat->m_flags);
        }
    }
}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recv_data*/)
{
    WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 8);
    data << uint64(GetPlayer()->GetGUID());

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveKnockBackAck(WorldPacket & recv_data)
{
    sLog->outDebug("CMSG_MOVE_KNOCK_BACK_ACK");

    uint64 guid;                                            // guid - unused
    recv_data.readPackGUID(guid);

    recv_data.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    ReadMovementInfo(recv_data, &movementInfo);
}

void WorldSession::HandleMoveHoverAck(WorldPacket& recv_data)
{
    sLog->outDebug("CMSG_MOVE_HOVER_ACK");

    uint64 guid;                                            // guid - unused
    recv_data.readPackGUID(guid);

    recv_data.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleMoveWaterWalkAck(WorldPacket& recv_data)
{
    sLog->outDebug("CMSG_MOVE_WATER_WALK_ACK");

    uint64 guid;                                            // guid - unused
    recv_data.readPackGUID(guid);

    recv_data.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& recv_data)
{
    if (!_player->isAlive() || _player->isInCombat())
        return;

    uint64 summoner_guid;
    bool agree;
    recv_data >> summoner_guid;
    recv_data >> agree;

    _player->SummonIfPossible(agree);
}
