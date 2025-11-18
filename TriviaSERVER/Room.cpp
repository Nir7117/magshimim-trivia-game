#include "Room.h"
#include <algorithm>

Room::Room(unsigned int id, const std::string& name, unsigned int maxPlayers,
    unsigned int questionCount, unsigned int answerTimeout,
    const LoggedUser& admin) : m_admin(admin)
{
    m_roomData.id = id;
    m_roomData.name = name;
    m_roomData.maxPlayers = maxPlayers;
    m_roomData.questionCount = questionCount;
    m_roomData.answerTimeout = answerTimeout;

    addUser(admin); 
}
LoggedUser Room::getAdmin() const
{
    return m_admin;
}

Room::Room() : m_admin(LoggedUser("default_user"))
{
    m_roomData.id = 0;
    m_roomData.name = "";
    m_roomData.maxPlayers = 0;
    m_roomData.questionCount = 0;
    m_roomData.answerTimeout = 0;
}




void Room::addUser(const LoggedUser& user)
{
    m_users.push_back(user);
}

void Room::removeUser(const std::string& username)
{
    m_users.erase(std::remove_if(m_users.begin(), m_users.end(), [&](const LoggedUser& u) {
        return u.getUsername() == username;
        }), m_users.end());
}

std::vector<std::string> Room::getAllUsers() const
{
    std::vector<std::string> names;
    for (const auto& user : m_users)
    {
        names.push_back(user.getUsername());
    }
    return names;
}

RoomData Room::getRoomData() const
{
    RoomData data = m_roomData;
    data.currentPlayers = static_cast<unsigned int>(m_users.size());
    data.adminUsername = m_admin.getUsername(); 
    return data;
}
bool Room::doesUserExist(const LoggedUser& user) const
{
    for (const auto& u : m_users)
    {
        if (u.getUsername() == user.getUsername())
            return true;
    }
    return false;
}




unsigned int Room::getId() const { return m_roomData.id; }
std::string Room::getName() const { return m_roomData.name; }
unsigned int Room::getMaxPlayers() const { return m_roomData.maxPlayers; }
unsigned int Room::getQuestionCount() const { return m_roomData.questionCount; }
unsigned int Room::getAnswerTimeout() const { return m_roomData.answerTimeout; }

bool Room::isFull() const
{
    return m_users.size() >= m_roomData.maxPlayers;
}
