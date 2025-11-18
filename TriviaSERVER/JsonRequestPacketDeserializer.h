#pragma once
#include "requestStructs.h"
#include "IRequestHandler.h"

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(const Buffer& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const Buffer& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const Buffer& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const Buffer& buffer);
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(std::vector<unsigned char> buffer);
	static LeaveGameRequest deserializeLeaveGameRequest(std::vector<unsigned char> buffer);
	static GetQuestionRequest deserializeGetQuestionRequest(std::vector<unsigned char> buffer);
	static GetGameResultRequest deserializeGetGameResultRequest(std::vector<unsigned char> buffer);


};

