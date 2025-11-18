#pragma once  

#include <string>  
#include <vector>  
#include <map>
#include "Room.h"  
#include "Question.h"
#include "PlayerResult.h"

typedef struct
{
	unsigned int status;
	std::string message;
} ErrorResponse;

typedef struct
{
	unsigned int status;
} LoginResponse;

typedef struct
{
	unsigned int status;
} SignupResponse;

typedef struct
{
	unsigned int status;
} CloseRoomResponse;

typedef struct
{
	unsigned int status;
} StartGameResponse;

typedef struct
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
} GetRoomStateResponse;

typedef struct
{
	unsigned int status;
} LeaveRoomResponse; // Ensure this is defined only once  

typedef struct
{
	unsigned int status;
} LogoutResponse;

typedef struct
{
	unsigned int status;
	std::vector<RoomData> rooms;
} GetRoomsResponse;

typedef struct
{
	unsigned int status;
	unsigned int roomId;
	std::string admin;
} CreateRoomResponse;

typedef struct
{
	unsigned int status;
	std::string admin;
} JoinRoomResponse;

typedef struct
{
	unsigned int status;
	std::vector<std::string> players;
} GetPlayersInRoomResponse;

typedef struct getHighscoreResponse
{
	unsigned int status;
	std::vector<std::string> highscores;
} getHighscoreResponse;

typedef struct getPersonalStatsResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
} getPersonalStatsResponse;

// ==== ADDED FOR QUESTION DB ====  


typedef struct
{
	unsigned int status;
	Question question;
	std::map<unsigned int, std::string> answers;
	unsigned int answerTimeout;      
	unsigned int questionCount;  
} GetQuestionResponse;

typedef struct LeaveGameResponse
{
	unsigned int status;
} LeaveGameResponse;

typedef struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
	bool isCorrect; 

} SubmitAnswerResponse;

typedef struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
} PlayerResults;

typedef struct GetGameResultsResponse
{
	unsigned int status;
	std::vector<PlayerResult> results;  
} GetGameResultsResponse;
