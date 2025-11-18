#pragma once
#include <map>
#include "Room.h"
#include <WinSock2.h>

class RoomManager
{
public:
    RoomManager();

    Room& createRoom(const LoggedUser& owner, const Room& roomInfo);
    void deleteRoom(unsigned int roomId);
    std::vector<Room> getRooms() const;
    Room* getRoomById(unsigned int roomId);
    void addRoom(const Room& room);
    SOCKET getSocketByUsername(const std::string& username) const;
    void registerSocketForUser(const std::string& username, SOCKET sock);
    void removeUserFromRoom(LoggedUser user);





private:
    std::map<unsigned int, Room> m_rooms;
    unsigned int m_nextRoomId;
    std::map<std::string, SOCKET> m_userToSocket;
};
