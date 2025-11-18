#pragma once
#include <vector>
#include <string>
#include "LoggedUser.h"
#include "SqliteDataBase.h"

class LoginManager
{
public:
    LoginManager(SqliteDataBase* db);
    bool signup(const std::string& username, const std::string& password, const std::string& email);
    bool login(const std::string& username, const std::string& password);
    void logout(const std::string& username);
    bool isUserLoggedIn(const std::string& username) const;

private:
    SqliteDataBase* m_database;
    std::vector<LoggedUser> m_loggedUsers;
};
