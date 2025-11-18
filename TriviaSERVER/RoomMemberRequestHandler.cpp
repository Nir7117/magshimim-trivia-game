#include "RoomMemberRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include <iostream>

RoomMemberRequestHandler::RoomMemberRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_user(user), m_room(room), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}



bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& request)
{
    std::cout << "[DEBUG]Got into ROOM MEMBER ISRELEVANT with ID: " << (int)request.id << std::endl;

    return request.id == 210 || request.id == 211 || request.id == 212 || request.id == 110 || request.id == 204 || request.id == 205 || request.id == 216;
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo request)
{
    if (request.id == 216)
    {
        std::cout << "[DEBUG] Switching to GameRequestHandler for player " << m_user.getUsername() << std::endl;

        unsigned int questionCount = m_room.getQuestionCount();
        unsigned int time = m_room.getAnswerTimeout();

        RequestResult result;
        StartGameResponse response;
        response.status = 1;
        result.response = JsonResponsePacketSerializer::serializeStartGameResponse(response);

        result.newHandler = m_handlerFactory.createGameRequestHandler(m_user, questionCount, time);
        return result;
    }


    switch (request.id)
    {
    case 210:
        return startGame(request);
    case 211:
        return getRoomState(request);
    case 212:
        return leaveRoom(request);
    case 110:
        return handleRoomClosed(request);
    case 204:
        return getPlayersInRoom(request);
    case 205:
    {
        JoinRoomResponse response;
        response.status = 1;
        RequestResult result;
        result.response = JsonResponsePacketSerializer::serializeJoinRoomResponse(response);
        result.newHandler = this;
        return result;
    }
    default:
        RequestResult result;
        result.newHandler = nullptr;
        result.response = {};
        return result;
    }
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo request)
{
    std::cout << "[DEBUG] " << m_user.getUsername() << " is leaving room ID " << m_room.getId() << std::endl;
    m_room.removeUser(m_user.getUsername());

    LeaveRoomResponse response;
    response.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeLeaveRoomResponse(response);
    result.newHandler = nullptr;
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);

    return result;
}



RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo request)
{
    StartGameResponse response;
    response.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeStartGameResponse(response);
    result.newHandler = this;
    return result;
}

RequestResult RoomMemberRequestHandler::startGame(RequestInfo request)
{
    GetRoomStateResponse response;
    RoomData data = m_room.getRoomData();

    response.answerTimeout = data.answerTimeout;
    response.players = m_room.getAllUsers();
    response.questionCount = data.questionCount;
    response.status = 1;
    response.hasGameBegun = false;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeGetRoomStateResponse(response);
    result.newHandler = this;
    return result;
}

RequestResult RoomMemberRequestHandler::handleRoomClosed(RequestInfo request)
{
    LeaveRoomResponse response;
    response.status = 1;

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeLeaveRoomResponse(response);
    result.newHandler = nullptr;
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    return result;
}


RequestResult RoomMemberRequestHandler::getPlayersInRoom(RequestInfo request)
{
    GetPlayersInRoomRequest req = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(request.buffer);

    Room* room = m_handlerFactory.getRoomManager().getRoomById(req.roomId);
    GetPlayersInRoomResponse response;

    if (room != nullptr)
    {
        response.players = room->getAllUsers();
        response.status = 1;
    }
    else
    {
        response.players = {};
        response.status = 0;
    }

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(response);
    result.newHandler = this;

    return result;
}

