/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
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
#include "SharedDefines.h"
#include "LFG.h"
#include "LFGMgr.h"
#include "Group.h"
#include "Player.h"

void LFGPlayerState::Clear()
{
    rolesMask = LFG_ROLE_MASK_NONE;
    update = true;
    m_state = LFG_STATE_NONE;
    m_flags = LFG_MEMBER_FLAG_NONE | LFG_MEMBER_FLAG_CHARINFO |
              LFG_MEMBER_FLAG_COMMENT | LFG_MEMBER_FLAG_UNK1 |
              LFG_MEMBER_FLAG_GROUP |
              LFG_MEMBER_FLAG_UNK2  |
              LFG_MEMBER_FLAG_UNK3  |
              LFG_MEMBER_FLAG_BIND;

    m_DungeonsList.clear();
    m_LockMap.clear();
    SetComment("<no comment>");
}

LFGLockStatusMap* LFGPlayerState::GetLockMap()
{
    if (update)
    {
        m_LockMap.clear();
        m_LockMap = sLFGMgr.GetPlayerLockMap(m_player);
        update = false;
    }
    return &m_LockMap;
};

void LFGPlayerState::SetRoles(uint8 roles)
{
    if (roles & LFG_ROLE_MASK_TANK)
        rolesMask = LFGRoleMask(rolesMask | LFG_ROLE_MASK_TANK); 
    else
        rolesMask = LFGRoleMask(rolesMask & ~LFG_ROLE_MASK_TANK); 

    if (roles & LFG_ROLE_MASK_HEALER)
        rolesMask = LFGRoleMask(rolesMask | LFG_ROLE_MASK_HEALER); 
    else
        rolesMask = LFGRoleMask(rolesMask & ~LFG_ROLE_MASK_HEALER); 

    if (roles & LFG_ROLE_MASK_DAMAGE)
        rolesMask = LFGRoleMask(rolesMask | LFG_ROLE_MASK_DAMAGE); 
    else
        rolesMask = LFGRoleMask(rolesMask & ~LFG_ROLE_MASK_DAMAGE); 

    if (rolesMask != LFG_ROLE_MASK_NONE)
        m_flags |= LFG_MEMBER_FLAG_ROLES;
    else
        m_flags &= ~LFG_MEMBER_FLAG_ROLES;

};

void LFGPlayerState::SetComment(std::string comment)
{
    m_comment.clear();
    m_comment.append(comment);
};

LFGType LFGPlayerState::GetType()
{
    if (m_DungeonsList.empty())
        return LFG_TYPE_NONE;
    else
        return LFGType((*m_DungeonsList.begin())->type);
};

void LFGGroupState::Clear()
{
    queued = false;
    update = true;
    status = LFG_STATUS_NOT_SAVED;
    dungeonEntry = 0;
    kicks = 0;
    kickActive = false;
    m_DungeonsList.clear();
    m_flags = LFG_MEMBER_FLAG_NONE |
              LFG_MEMBER_FLAG_COMMENT | 
              LFG_MEMBER_FLAG_BIND;
}