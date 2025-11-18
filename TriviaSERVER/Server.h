#pragma once
#include "GameManager.h"
#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "SqliteDataBase.h"
#include <thread>

class Server
{
public:
    Server();
    ~Server();

    void run();

private:
    GameManager m_gameManager;
    SqliteDataBase m_database;
    LoginManager m_loginManager;
    RoomManager m_roomManager;
    StatisticsManager m_statisticsManager;
    RequestHandlerFactory m_handlerFactory;
    Communicator m_communicator;
    
};
