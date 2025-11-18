#pragma once
#include <list>
#include "responseStructs.h"

class IDatabase
{
public:
	virtual bool doesUserExist(const std::string& username) = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
	virtual bool addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;

	virtual std::vector<Question> getQuestions(const int& amount) = 0;
	virtual double getPlayerAverageAnswerTime(const std::string& playerName) = 0;
	virtual int getNumOfCorrectAnswers(const std::string& playerName) = 0;
	virtual int getNumOfTotalAnswers(const std::string& playerName) = 0;
	virtual int getNumOfPlayerGames(const std::string& playerName) = 0;
	virtual int getPlayerScore(const std::string& playerName) = 0;
	virtual std::vector<std::string> getHighScores() = 0;
};

