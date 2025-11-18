#pragma once  
#include "IRequestHandler.h"  
#include "JsonResponsePacketSerializer.h"
#include "LoggedUser.h"  
#include "Room.h"  
#include "RoomManager.h"  

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{  
public:  
   RoomAdminRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);  
   bool isRequestRelevant(const RequestInfo& request) override;  
   RequestResult handleRequest(RequestInfo request) override;  

private:  
   Room& m_room;
   LoggedUser m_user;
   RoomManager& m_roomManager;
   RequestHandlerFactory& m_handlerFactory;
   RequestResult closeRoom(RequestInfo request);
   RequestResult startGame(RequestInfo request);
   RequestResult getRoomState(RequestInfo request);
   RequestResult getPlayersInRoom(RequestInfo request);


};
