#include "LoginManager.h"
#include "SqliteDataBase.h"

#include <iostream>

LoginManager::LoginManager(SqliteDataBase* db) : m_database(db) {}

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    if (m_database->doesUserExist(username))
        return false;

    return m_database->addNewUser(username, password, email);
}

bool LoginManager::login(const std::string& username, const std::string& password)
{
    std::cout << "[DEBUG] Login attempt: " << username << ", " << password << std::endl;

    if (!m_database->doesUserExist(username))
        return false;

    if (!m_database->doesPasswordMatch(username, password))
        return false;

    if (isUserLoggedIn(username))
        return false;

    m_loggedUsers.emplace_back(username);
    return true;
}

void LoginManager::logout(const std::string& username)
{
    m_loggedUsers.erase(
        std::remove_if(m_loggedUsers.begin(), m_loggedUsers.end(),
            [&](const LoggedUser& user) { return user.getUsername() == username; }),
        m_loggedUsers.end());
}

bool LoginManager::isUserLoggedIn(const std::string& username) const
{
    for (const auto& user : m_loggedUsers)
    {
        if (user.getUsername() == username)
            return true;
    }
    return false;
}
