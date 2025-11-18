#include "JsonRequestPacketDeserializer.h"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
	json j = json::from_bson(buffer);
	LoginRequest request;

	request.username = j["username"];
	request.password = j["password"];

	return request;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer& buffer)
{
	json j = json::from_bson(buffer);
	SignupRequest request;

	request.username = j["username"];
	request.password = j["password"];
	request.email = j["email"];

	return request;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const Buffer& buffer)
{
	json j = json::from_bson(buffer);
	GetPlayersInRoomRequest request;
	request.roomId = j["roomId"];
	return request;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer& buffer)
{
	json j = json::from_bson(buffer);
	JoinRoomRequest request;
	request.roomId = j["roomId"];
	return request;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer& buffer)
{
	json j = json::from_bson(buffer);
	CreateRoomRequest request;
	request.name = j["name"];
	request.maxPlayers = j["maxPlayers"];
	request.questionCount = j["questionCount"];
	request.answerTimeout = j["answerTimeout"];
	return request;
}
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const Buffer& buffer)
{
	json j = json::from_bson(buffer);
	GetPlayersInRoomRequest request;

	request.roomId = j["roomId"];

	return request;
}
SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(std::vector<unsigned char> buffer)
{
	json j = json::from_bson(buffer);
	SubmitAnswerRequest req;
	req.answerId = j["answerId"];
	return req;
}

LeaveGameRequest JsonRequestPacketDeserializer::deserializeLeaveGameRequest(std::vector<unsigned char> buffer)
{
	json j = json::from_bson(buffer);
	LeaveGameRequest req;
	return req;
}

GetQuestionRequest JsonRequestPacketDeserializer::deserializeGetQuestionRequest(std::vector<unsigned char> buffer)
{
	json j = json::from_bson(buffer);
	GetQuestionRequest req;
	return req;
}

GetGameResultRequest JsonRequestPacketDeserializer::deserializeGetGameResultRequest(std::vector<unsigned char> buffer)
{
	json j = json::from_bson(buffer);
	GetGameResultRequest req;
	return req;
}


