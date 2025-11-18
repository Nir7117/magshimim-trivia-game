#include "MenuRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "requestStructs.h"
#include "responseStructs.h"



#include <iostream>

MenuRequestHandler::MenuRequestHandler(LoggedUser user, RequestHandlerFactory& handlerFactory)
    : m_user(user), m_handlerFactory(handlerFactory){}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& request)
{
	std::cout << "[DEBUG]Got into ISRELEVANT with ID: " << (int)request.id << std::endl;
    return request.id == 200 || request.id == 201 || request.id == 202 ||
        request.id == 203 || request.id == 204 || request.id == 205 ||
        request.id == 206 || request.id == 207 || request.id == 208;
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo request)
{
	std::cout << "[DEBUG]Got into handleREQUEST wiht request with ID: " << (int)request.id << std::endl;

	switch (request.id)
	{
	case 202:
		return signout(request);
	case 203:
		return getRooms(request);
	case 204:
		return getPlayersInRoom(request);
	case 205:
		return joinRoom(request);
	case 206:
		return createRoom(request);
	case 207:
		return getHighScore(request);
	case 208:
		return getPersonalStats(request);
		
	default:
		break;
	}

    RequestResult result;
    result.newHandler = nullptr;
    result.response = {};
    return result;
}

RequestResult MenuRequestHandler::signout(RequestInfo request)
{
	LogoutResponse response;
	response.status = 103;
	RequestResult result;
	result.newHandler = nullptr;
    result.response = {};
    return result;
}



RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo request)
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
		response.status = 0;
	}

	RequestResult result;
	result.response = JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(response);

	if (room != nullptr && room->getAdmin().getUsername() == m_user.getUsername())
	{
		std::cout << "[DEBUG] Promoting to RoomAdminRequestHandler from getPlayersInRoom" << std::endl;
		result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, *room);
	}
	else
	{
		result.newHandler = this;
	}

	return result;
}


RequestResult MenuRequestHandler::getPersonalStats(RequestInfo request)
{
	getPersonalStatsResponse response;
	response.status = 1;
	response.statistics = m_handlerFactory.getStatisticsManager().getUserStatistics(m_user.getUsername());

	RequestResult result;
	result.newHandler = this;
	result.response = JsonResponsePacketSerializer::serializeGetPersonalStatsResponse(response);
	std::cout << "[DEBUG] Sending personal stats response, status: " << response.status << std::endl;

	return result;
}


RequestResult MenuRequestHandler::getHighScore(RequestInfo request)
{
	getHighscoreResponse response;
	response.status = 1;
	response.highscores = m_handlerFactory.getStatisticsManager().getHighScore();

	RequestResult result;
	result.newHandler = this;
	result.response = JsonResponsePacketSerializer::serializeGetHighscoreResponse(response);
	return result;
}


RequestResult MenuRequestHandler::joinRoom(RequestInfo request)
{
	JoinRoomRequest req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(request.buffer);
	Room* room = m_handlerFactory.getRoomManager().getRoomById(req.roomId);

	JoinRoomResponse response;
	RequestResult result;

	if (room && !room->isFull())
	{
		room->addUser(m_user);
		response.status = 1;
		response.admin = room->getAdmin().getUsername();

		
		result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, *room);
		std::cout << "[DEBUG] Promoted to RoomMemberRequestHandler after joinRoom()" << std::endl;
	}
	else
	{
		response.status = 0;
		result.newHandler = this;
	}

	result.response = JsonResponsePacketSerializer::serializeJoinRoomResponse(response);
	return result;
}

RequestResult MenuRequestHandler::createRoom(RequestInfo request)
{
	CreateRoomRequest roomReq = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(request.buffer);

	Room tempRoom(0, roomReq.name, roomReq.maxPlayers, roomReq.questionCount, roomReq.answerTimeout, m_user);
	Room& createdRoom = m_handlerFactory.getRoomManager().createRoom(m_user, tempRoom);

	CreateRoomResponse response;
	response.status = 1;
	response.roomId = createdRoom.getId();
	response.admin = m_user.getUsername();

	RequestResult result;
	result.response = JsonResponsePacketSerializer::serializeCreateRoomResponse(response);
	result.newHandler = this; 

	return result;
}











RequestResult MenuRequestHandler::getRooms(RequestInfo request)
{
	std::cout << "[DEBUG] Entered getRooms()" << std::endl;

	GetRoomsResponse response;
	response.status = 1;

	const auto& allRooms = m_handlerFactory.getRoomManager().getRooms();
	std::cout << "[DEBUG] Total rooms in system: " << allRooms.size() << std::endl;

	for (const Room& room : allRooms)
	{
		std::cout << "[DEBUG] Room '" << room.getName()
			<< "' | Players: " << room.getAllUsers().size()
			<< "/" << room.getMaxPlayers()
			<< " | isFull = " << (room.isFull() ? "true" : "false")
			<< std::endl;

		if (!room.isFull())
		{
			RoomData data = room.getRoomData();
			std::cout << "[DEBUG] → Added room to response: " << data.name << std::endl;
			response.rooms.push_back(data);
		}
		else
		{
			std::cout << "[DEBUG] → Room skipped (full): " << room.getName() << std::endl;
		}
	}

	std::cout << "[DEBUG] Total rooms in response: " << response.rooms.size() << std::endl;

	RequestResult result;
	result.newHandler = this;
	result.response = JsonResponsePacketSerializer::serializeGetRoomsResponse(response);
	return result;
}
