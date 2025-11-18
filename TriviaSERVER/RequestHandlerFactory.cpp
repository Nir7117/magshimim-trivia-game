#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"
#include <iostream>

#include "RequestHandlerFactory.h"
#include "Communicator.h"

RequestHandlerFactory::RequestHandlerFactory(
    LoginManager& loginManager,
    RoomManager& roomManager,
    StatisticsManager& statisticsManager,
    GameManager& gameManager)
    : m_loginManager(loginManager),
    m_roomManager(roomManager),
    m_statisticsManager(statisticsManager),
    m_gameManager(gameManager) 
{
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return m_roomManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return m_statisticsManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return m_gameManager;
}

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this);
}

IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
    std::cout << "[DEBUG] GOT into createMenuRequestHandler" << std::endl;
    return new MenuRequestHandler(user, *this);
}

IRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room& room)
{
    std::cout << "[DEBUG] GOT into createRoomAdminRequestHandler" << std::endl;
    return new RoomAdminRequestHandler(user, room, m_roomManager, *this);
}

IRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room& room)
{
    std::cout << "[DEBUG] GOT into createRoomMemberRequestHandler" << std::endl;
    return new RoomMemberRequestHandler(user, room, m_roomManager, *this);
}

IRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, unsigned int questionCount, unsigned int time)
{
    return new GameRequestHandler(user, m_roomManager, m_gameManager, *this, questionCount, time);
}


