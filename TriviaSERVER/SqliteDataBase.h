
#pragma once
#include <string>
#include <vector>
#include "requestStructs.h"
#include "sqlite3.h"
#include "responseStructs.h"
#include "IDatabase.h"

class SqliteDataBase : public IDatabase
{
public:
    SqliteDataBase();
    ~SqliteDataBase();

    bool open();
    void close();

    bool doesUserExist(const std::string& username) override;
    bool doesPasswordMatch(const std::string& username, const std::string& password) override;
    bool addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

    std::vector<Question> getQuestions(const int& amount) override;


    
    double getPlayerAverageAnswerTime(const std::string& username) override;
    int getNumOfCorrectAnswers(const std::string& username) override;
    int getNumOfTotalAnswers(const std::string& username) override;
    int getNumOfPlayerGames(const std::string& username) override;
	std::vector<std::string> getHighScores() override;
    int getPlayerScore(const std::string& username) override;

    bool insertPlayerAnswerStats(const std::string& username, bool isCorrect, double answerTime);


private:
    void createUsersTable();
    void createQuestionsTable();
    void createStatisticsTable(); 

    sqlite3* _db;
};
