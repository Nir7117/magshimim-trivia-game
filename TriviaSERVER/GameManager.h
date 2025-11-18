#pragma once

#include <map>
#include <vector>
#include <string>
#include <chrono>

#include "Question.h"
#include "PlayerResult.h"
#include "SqliteDataBase.h"

class GameManager
{
public:
    GameManager(SqliteDataBase& db);

    Question getQuestionForUser(const std::string& username, unsigned int questionCount);
    int submitAnswer(const std::string& username, unsigned int answerId);
    std::vector<PlayerResult> getGameResults(const std::string& username);
    std::vector<std::string> getShuffledAnswers(const std::string& username) const;
    bool isGameOverForAll() const;


private:
    struct GameData
    {
        std::vector<Question> questions;
        unsigned int currentQuestionIndex = 0;
        unsigned int correctAnswers = 0;
        unsigned int wrongAnswers = 0;
        std::chrono::high_resolution_clock::time_point questionStartTime;
        float totalAnswerTime = 0;

        std::vector<std::string> currentShuffledAnswers;
    };


    std::map<std::string, GameData> m_games;
    SqliteDataBase& m_db;
};
