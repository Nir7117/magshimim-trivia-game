#pragma once

#include <vector>
#include <string>
#include "json.hpp" // from github
#include "responseStructs.h"

using json = nlohmann::json;
using byte = unsigned char;


class JsonResponsePacketSerializer
{
public:
    static std::vector<byte> serializeLoginResponse(LoginResponse& response);
    static std::vector<byte> serializeSignupResponse(SignupResponse& response);
    static std::vector<byte> serializeCreateRoomResponse(CreateRoomResponse& response);
    static std::vector<byte> serializeGetPersonalStatsResponse(getPersonalStatsResponse& response);
    static std::vector<byte> serializeGetHighscoreResponse(getHighscoreResponse& response);

    static std::vector<byte> serializeGetPlayersInRoomResponse(GetPlayersInRoomResponse& response);
    static std::vector<byte> serializeJoinRoomResponse(JoinRoomResponse& response);
    static std::vector<byte> serializeGetRoomsResponse(GetRoomsResponse& response);
    static std::vector<byte> serializeErrorResponse(ErrorResponse& response);
    static std::vector<byte> serializeCloseRoomResponse(CloseRoomResponse& response);
    static std::vector<byte> serializeStartGameResponse(StartGameResponse& response);
    static std::vector<byte> serializeGetRoomStateResponse(GetRoomStateResponse& response);
    static std::vector<byte> serializeLeaveRoomResponse(LeaveRoomResponse& response);
    static std::vector<byte> serializeLogoutResponse(LogoutResponse& response);

    static std::vector<byte> serializeGetQuestionResponse(GetQuestionResponse& response);
    static std::vector<byte> serializeLeaveGameResponse(LeaveGameResponse& response);
    static std::vector<byte> serializeSubmitAnswerResponse(SubmitAnswerResponse& response);
    static std::vector<byte> serializeGetGameResultsResponse(GetGameResultsResponse& response);




private:
    static std::vector<byte> createPacket(int code, json& content);
};
