#pragma once
#include <string>
#include <vector>
#include "LoggedUser.h"

typedef struct
{
    unsigned int id;
    std::string name;
    unsigned int maxPlayers;
    unsigned int questionCount;
    unsigned int answerTimeout;
    unsigned int currentPlayers;
    std::string adminUsername;

} RoomData;

class Room
{
public:
    Room(unsigned int id, const std::string& name, unsigned int maxPlayers,
        unsigned int questionCount, unsigned int answerTimeout, const LoggedUser& admin);
    Room(); 

    LoggedUser getAdmin() const; 


    void addUser(const LoggedUser& user);
    void removeUser(const std::string& username);
    std::vector<std::string> getAllUsers() const;
    bool doesUserExist(const LoggedUser& user) const;

    unsigned int getId() const;
    std::string getName() const;
    unsigned int getMaxPlayers() const;
    unsigned int getQuestionCount() const;
    unsigned int getAnswerTimeout() const;

    bool isFull() const;
    RoomData getRoomData() const;

private:
    RoomData m_roomData;
    std::vector<LoggedUser> m_users;
    LoggedUser m_admin; 

};
