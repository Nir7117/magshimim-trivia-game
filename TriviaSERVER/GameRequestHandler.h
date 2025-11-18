#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"
#include "requestStructs.h"

class GameRequestHandler : public IRequestHandler
{
public:
    GameRequestHandler(LoggedUser user, RoomManager& roomManager, GameManager& gameManager, RequestHandlerFactory& handlerFactory, unsigned int questionCount, unsigned int time);
    bool isRequestRelevant(const RequestInfo& info) override;
    RequestResult handleRequest(RequestInfo info) override;

private:
    LoggedUser m_user;
    RoomManager& m_roomManager;
    GameManager& m_gameManager;
    RequestHandlerFactory& m_handlerFactory;
    unsigned int m_questionCount;
    unsigned int m_answerTimeout;
};
