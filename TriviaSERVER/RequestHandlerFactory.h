#pragma once

#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "GameManager.h"
#include "LoggedUser.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "Communicator.h"

class RequestHandlerFactory
{
public:
    RequestHandlerFactory(LoginManager& loginManager, RoomManager& roomManager, StatisticsManager& statisticsManager, GameManager& gameManager);

    LoginManager& getLoginManager();
    RoomManager& getRoomManager();
    StatisticsManager& getStatisticsManager();
    GameManager& getGameManager();
    IRequestHandler* createLoginRequestHandler();
    IRequestHandler* createMenuRequestHandler(LoggedUser user);
    IRequestHandler* createRoomAdminRequestHandler(LoggedUser user, Room& room);
    IRequestHandler* createRoomMemberRequestHandler(LoggedUser user, Room& room);
    IRequestHandler* createGameRequestHandler(LoggedUser user, unsigned int questionCount, unsigned int time);

private:
    LoginManager& m_loginManager;
    RoomManager& m_roomManager;
    StatisticsManager& m_statisticsManager;
    GameManager& m_gameManager;
};
