#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"

#include "RequestHandlerFactory.h"

class MenuRequestHandler : public IRequestHandler
{
public:
    MenuRequestHandler(LoggedUser user, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& request) override;
    RequestResult handleRequest(RequestInfo request) override;

private:
    RequestResult signout(RequestInfo request);
    RequestResult getRooms(RequestInfo request);
    RequestResult getPlayersInRoom(RequestInfo request);
    RequestResult getPersonalStats(RequestInfo request);
    RequestResult getHighScore(RequestInfo request);
    RequestResult joinRoom(RequestInfo request);
    RequestResult createRoom(RequestInfo request);

    LoggedUser m_user;
    RequestHandlerFactory&  m_handlerFactory;
};
