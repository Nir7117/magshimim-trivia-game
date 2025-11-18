#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <WinSock2.h> 


using byte = unsigned char;

typedef std::vector<byte> Buffer;


class IRequestHandler;

typedef struct SubmitAnswerRequest
{
    unsigned int answerId;
} SubmitAnswerRequest;

typedef struct LeaveGameRequest
{
    
} LeaveGameRequest;

typedef struct GetQuestionRequest
{
   
} GetQuestionRequest;

typedef struct GetGameResultRequest
{
    
} GetGameResultRequest;

typedef struct LoginRequest
{
    std::string username;
    std::string password;
} LoginRequest;

typedef struct SignupRequest
{
    std::string username;
    std::string password;
    std::string email;
} SignupRequest;

typedef struct RequestResult
{
    Buffer response;
    IRequestHandler* newHandler;
} RequestResult;

typedef struct RequestInfo
{
    byte id;
    std::time_t receivalTime;
    Buffer buffer;
    SOCKET sock;
} RequestInfo;

typedef struct GetPlayersInRoomRequest
{
    unsigned int status;
    unsigned int roomId;
} GetPlayersInRoomRequest;

typedef struct JoinRoomRequest
{
    unsigned int roomId;
} JoinRoomRequest;

typedef struct CreateRoomRequest
{
    std::string name;
    unsigned int maxPlayers;
    unsigned int questionCount;
    unsigned int answerTimeout;
} CreateRoomRequest;