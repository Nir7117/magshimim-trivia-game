#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"

class RoomMemberRequestHandler : public IRequestHandler
{
private:
    
    LoggedUser m_user;
    Room& m_room;
    RoomManager& m_roomManager;
    RequestHandlerFactory& m_handlerFactory;

public:
    RoomMemberRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& request) override;
    RequestResult handleRequest(RequestInfo request) override;

    RequestResult leaveRoom(RequestInfo request);
    RequestResult getRoomState(RequestInfo request);
    RequestResult startGame(RequestInfo request);
    RequestResult handleRoomClosed(RequestInfo request);
    RequestResult getPlayersInRoom(RequestInfo request);
};
