#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* database) : m_database(database)
{}

StatisticsManager::~StatisticsManager()
{
}

std::vector<std::string> StatisticsManager::getHighScore()
{
	std::vector<std::string> highScores;
	std::vector<std::string> scores = m_database->getHighScores();
	for (const auto& score : scores)
	{
		highScores.push_back(score);
	}
	return highScores;
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string username)
{
	std::vector<std::string> statistics;
	double avgTime = m_database->getPlayerAverageAnswerTime(username);
	int correctAnswers = m_database->getNumOfCorrectAnswers(username);
	int totalAnswers = m_database->getNumOfTotalAnswers(username);
	int numOfGames = m_database->getNumOfPlayerGames(username);
	statistics.push_back("Average Answer Time: " + std::to_string(avgTime));
	statistics.push_back("Correct Answers: " + std::to_string(correctAnswers));
	statistics.push_back("Total Answers: " + std::to_string(totalAnswers));
	statistics.push_back("Number of Games: " + std::to_string(numOfGames));
	return statistics;
}
