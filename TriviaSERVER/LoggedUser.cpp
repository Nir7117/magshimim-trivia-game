#include "LoggedUser.h"

LoggedUser::LoggedUser(std::string username) : _username(username) {}

std::string LoggedUser::getUsername() const
{
    return _username;
}
