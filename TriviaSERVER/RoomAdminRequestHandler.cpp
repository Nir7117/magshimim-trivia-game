#include "RoomAdminRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestHandlerFactory.h"
#include <winsock.h>
#include <iostream>

RoomAdminRequestHandler::RoomAdminRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
	: m_user(user), m_room(room), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& request)
{
	return request.id == 110 || request.id == 111 || request.id == 112 || request.id == 204 || request.id == 210 || request.id == 214;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo request)
{
	std::cout << "[DEBUG] RoomAdminRequestHandler::handleRequest with ID: " << request.id << std::endl;

	if (request.id == 206)
	{
		RequestResult result;
		result.newHandler = this;
		result.response = {};
		return result;
	}

	switch (request.id)
	{
	case 110: // Close Room
		return closeRoom(request);

	case 111: // Start Game
		return startGame(request);

	case 112: // Get Room State
		return getRoomState(request);

	case 204: // Get Players
		return getPlayersInRoom(request);

	default:
		std::cout << "[DEBUG] Request ID not relevant in RoomAdminRequestHandler: " << request.id << std::endl;
		RequestResult result;
		result.newHandler = this;
		result.response = {};
		return result;
	}
}



RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo request)
{
	CloseRoomResponse adminResp;
	adminResp.status = 1;
	auto adminMsg = JsonResponsePacketSerializer::serializeCloseRoomResponse(adminResp);

	CloseRoomResponse notifyResp;
	notifyResp.status = 1;
	auto notifyMsg = JsonResponsePacketSerializer::serializeCloseRoomResponse(notifyResp);

	auto users = m_room.getAllUsers();
	std::cout << "[debug] admin closing room, notifying users:\n";
	for (auto& u : users)
	{
		if (u == m_user.getUsername()) continue;
		SOCKET s = m_roomManager.getSocketByUsername(u);
		if (s != INVALID_SOCKET)
		{
			int sent = send(s,
				reinterpret_cast<const char*>(notifyMsg.data()),
				(int)notifyMsg.size(),
				0);
			std::cout << "[debug] sent close to " << u << ", bytes=" << sent << "\n";
		}
	}

	m_roomManager.deleteRoom(m_room.getId());
	std::cout << "[debug] room deleted, returning admin to menu\n";

	RequestResult result;
	result.response = adminMsg;
	result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
	return result;
}



RequestResult RoomAdminRequestHandler::startGame(RequestInfo request)
{
	StartGameResponse response;
	response.status = 1;

	std::vector<unsigned char> serializedMsg = JsonResponsePacketSerializer::serializeStartGameResponse(response);
	std::vector<std::string> users = m_room.getAllUsers();
	unsigned int questionCount = m_room.getQuestionCount();
	unsigned int timeout = m_room.getAnswerTimeout();

	std::cout << "[DEBUG] Admin is starting the game. Sending start message to users:\n";

	for (const std::string& username : users)
	{
		std::cout << "  - Found user in room: " << username << std::endl;

		if (username != m_user.getUsername())
		{
			SOCKET sock = m_roomManager.getSocketByUsername(username);
			if (sock != INVALID_SOCKET)
			{
				int sent = send(sock, reinterpret_cast<const char*>(serializedMsg.data()), serializedMsg.size(), 0);
				if (sent == SOCKET_ERROR)
				{
					std::cout << "[ERROR] Failed to send to " << username << ", error code: " << WSAGetLastError() << std::endl;
				}
				else
				{
					std::cout << "[DEBUG] Sent start message to " << username << " (bytes sent: " << sent << ")" << std::endl;
				}
			}
			else
			{
				std::cout << "[WARNING] Could not find valid socket for user: " << username << std::endl;
			}
		}
	}

	std::cout << "[DEBUG] Switching only admin to GameRequestHandler" << std::endl;

	RequestResult result;
	result.response = serializedMsg;
	result.newHandler = m_handlerFactory.createGameRequestHandler(m_user, questionCount, timeout);
	return result;
}





RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo request)
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
RequestResult RoomAdminRequestHandler::getPlayersInRoom(RequestInfo request)
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



