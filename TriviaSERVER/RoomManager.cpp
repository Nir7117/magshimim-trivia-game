#include "RoomManager.h"
#include <winsock.h>


RoomManager::RoomManager() : m_nextRoomId(1) {}

Room& RoomManager::createRoom(const LoggedUser& owner, const Room& roomInfo)
{
    Room newRoom(m_nextRoomId++, roomInfo.getName(), roomInfo.getMaxPlayers(),
        roomInfo.getQuestionCount(), roomInfo.getAnswerTimeout(), owner);

    m_rooms.insert({ newRoom.getId(), newRoom });
    return m_rooms[newRoom.getId()];
}

void RoomManager::deleteRoom(unsigned int roomId)
{
    m_rooms.erase(roomId);
}

std::vector<Room> RoomManager::getRooms() const
{
    std::vector<Room> result;
    for (const auto& entry : m_rooms)
    {
        result.push_back(entry.second);
    }
    return result;
}

Room* RoomManager::getRoomById(unsigned int roomId)
{
    auto it = m_rooms.find(roomId);
    if (it != m_rooms.end())
        return &(it->second);
    return nullptr;
}

void RoomManager::addRoom(const Room& room)
{
    m_rooms[room.getId()] = room;
}

void RoomManager::registerSocketForUser(const std::string& username, SOCKET sock)
{
    m_userToSocket[username] = sock;
}
void RoomManager::removeUserFromRoom(LoggedUser user)
{
    for (auto& pair : m_rooms)
    {
        Room& room = pair.second;
        if (room.doesUserExist(user))
        {
            room.removeUser(user.getUsername());
        }

    }
}


SOCKET RoomManager::getSocketByUsername(const std::string& username) const
{
    auto it = m_userToSocket.find(username);
    if (it != m_userToSocket.end())
    {
        return it->second;
    }
    return INVALID_SOCKET;
}
