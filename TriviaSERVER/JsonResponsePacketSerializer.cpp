#include "JsonResponsePacketSerializer.h"
#include "responseStructs.h"

#include "json.hpp"
#include "Helper.h"
#include "requestStructs.h"
#include <iostream>
#include "PlayerResult.h"


using json = nlohmann::json;

std::vector<byte> JsonResponsePacketSerializer::serializeLoginResponse(LoginResponse& response)
{
    json jsonObj;
    jsonObj["status"] = static_cast<int>(response.status);
    return createPacket(LOGIN_RESPONSE_CODE, jsonObj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeSignupResponse(SignupResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    return createPacket(SIGNUP_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeErrorResponse(ErrorResponse& response)
{
    json errorData = {
        {"message", response.message}
    };
    return createPacket(ERROR_RESPONSE_CODE, errorData);
}

std::vector<byte> JsonResponsePacketSerializer::serializeCloseRoomResponse(CloseRoomResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    return createPacket(CLOSE_ROOM_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeStartGameResponse(StartGameResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    return createPacket(115, obj);
}


std::vector<byte> JsonResponsePacketSerializer::serializeGetRoomStateResponse(GetRoomStateResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    obj["hasGameBegun"] = response.hasGameBegun;
    obj["players"] = response.players;
    obj["questionCount"] = response.questionCount;
    obj["answerTimeout"] = response.answerTimeout;
    return createPacket(GET_ROOM_STATE_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeLeaveRoomResponse(LeaveRoomResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    return createPacket(LEAVE_ROOM_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeLogoutResponse(LogoutResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    return createPacket(LOGOUT_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeGetQuestionResponse(GetQuestionResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    obj["question"] = response.question.question;

    json answersJson;
    for (const auto& answer : response.answers)
    {
        answersJson[std::to_string(answer.first)] = answer.second;
    }

    obj["answers"] = answersJson;
    obj["answerTimeout"] = response.answerTimeout;
    obj["questionCount"] = response.questionCount;

    return createPacket(GET_QUESTION_RESPONSE_CODE, obj);
}



std::vector<byte> JsonResponsePacketSerializer::serializeSubmitAnswerResponse(SubmitAnswerResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    obj["correctAnswerId"] = response.correctAnswerId;
    obj["isCorrect"] = response.isCorrect; 
    return createPacket(SUBMIT_ANSWER_RESPONSE_CODE, obj);

    return createPacket(SUBMIT_ANSWER_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeLeaveGameResponse(LeaveGameResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    return createPacket(LEAVE_GAME_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeGetGameResultsResponse(GetGameResultsResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);

    
    json resultsJson = json::array();
    for (const auto& result : response.results)
    {
        json resultJson;
        resultJson["username"] = result.username;
        resultJson["correctAnswerCount"] = result.correctAnswersCount;
        resultJson["wrongAnswerCount"] = result.wrongAnswersCount;
        resultJson["averageAnswerTime"] = result.averageAnswerTime;
        resultsJson.push_back(resultJson);
    }
    obj["results"] = resultsJson;

    return createPacket(GET_GAME_RESULTS_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::createPacket(int code, json& content)
{
    std::vector<byte> result;
    std::vector<byte> bsonData = json::to_bson(content);
    int length = static_cast<int>(bsonData.size());

    result.emplace_back(static_cast<byte>(code));
    for (int shift = 24; shift >= 0; shift -= 8)
    {
        result.emplace_back(static_cast<byte>((length >> shift) & 0xFF));
    }
    result.insert(result.end(), bsonData.begin(), bsonData.end());

    return result;
}
std::vector<byte> JsonResponsePacketSerializer::serializeCreateRoomResponse(CreateRoomResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    obj["roomId"] = response.roomId;
    obj["admin"] = response.admin;
    return createPacket(CREATE_ROOM_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(GetPlayersInRoomResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    obj["PlayersInRoom"] = response.players;
    std::cout << "[DEBUG] Players JSON: " << obj.dump() << std::endl;

    return createPacket(GET_PLAYERS_IN_ROOM_RESPONSE_CODE, obj);
}
std::vector<byte> JsonResponsePacketSerializer::serializeJoinRoomResponse(JoinRoomResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    obj["admin"] = response.admin;
    return createPacket(JOIN_ROOM_RESPONSE_CODE, obj);
}

std::vector<byte> JsonResponsePacketSerializer::serializeGetRoomsResponse(GetRoomsResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);

    obj["Rooms"] = json::array();
    for (const auto& room : response.rooms)
    {
        json roomJson;
        roomJson["id"] = room.id;
        roomJson["name"] = room.name;
        roomJson["maxPlayers"] = room.maxPlayers;
        roomJson["questionCount"] = room.questionCount;
        roomJson["answerTimeout"] = room.answerTimeout;
        roomJson["currentPlayers"] = room.currentPlayers;

        obj["Rooms"].push_back(roomJson);
    }

    return createPacket(GET_ROOMS_RESPONSE_CODE, obj);
}
std::vector<byte> JsonResponsePacketSerializer::serializeGetPersonalStatsResponse(getPersonalStatsResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    obj["statistics"] = response.statistics;
    return createPacket(PERSONAL_STATS_RESPONSE_CODE, obj);
}
std::vector<byte> JsonResponsePacketSerializer::serializeGetHighscoreResponse(getHighscoreResponse& response)
{
    json obj;
    obj["status"] = static_cast<int>(response.status);
    obj["highscores"] = response.highscores;
    return createPacket(HIGH_SCORE_RESPONSE_CODE, obj);
}
