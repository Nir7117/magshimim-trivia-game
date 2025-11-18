#include "GameManager.h"
#include "Question.h"


GameManager::GameManager(SqliteDataBase& db) : m_db(db)
{
}

Question GameManager::getQuestionForUser(const std::string& username, unsigned int questionCount)
{
    GameData& game = m_games[username];

    if (game.questions.empty())
    {
        game.questions = m_db.getQuestions(questionCount);
        if (game.questions.size() > questionCount)
            game.questions.resize(questionCount);
        game.currentQuestionIndex = 0;
    }

    if (game.currentQuestionIndex >= game.questions.size())
        return Question();

    Question& q = game.questions[game.currentQuestionIndex];
    game.questionStartTime = std::chrono::high_resolution_clock::now();
    game.currentShuffledAnswers = q.getShuffledAnswers();

    return q;
}


int GameManager::submitAnswer(const std::string& username, unsigned int answerId)
{
    GameData& game = m_games[username];
    if (game.currentQuestionIndex >= game.questions.size())
        return -1;

    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = now - game.questionStartTime;
    game.totalAnswerTime += duration.count();

    const Question& q = game.questions[game.currentQuestionIndex];
    const std::vector<std::string>& shuffled = game.currentShuffledAnswers;

    int correctIndex = -1;
    for (int i = 0; i < shuffled.size(); ++i)
    {
        if (shuffled[i] == q.correctAnswer)
        {
            correctIndex = i;
            break;
        }
    }

    if (answerId == correctIndex)
        game.correctAnswers++;
    else
        game.wrongAnswers++;

    m_db.insertPlayerAnswerStats(username, answerId == correctIndex, duration.count());

    game.currentQuestionIndex++;
    return correctIndex;
}
bool GameManager::isGameOverForAll() const
{
    for (const auto& pair : m_games)
    {
        const GameData& game = pair.second;
        if (game.currentQuestionIndex < game.questions.size())
        {
            return false; 
        }
    }
    return true; 
}



std::vector<PlayerResult> GameManager::getGameResults(const std::string& username)
{
    if (!isGameOverForAll())
        return {};

    std::vector<PlayerResult> results;
    GameData& game = m_games[username];

    for (const auto& pair : m_games)
    {
        const std::string& user = pair.first;
        const GameData& game = pair.second;

        if (game.currentQuestionIndex < game.questions.size())
            continue;

        PlayerResult result;
        result.username = user;
        result.correctAnswersCount = game.correctAnswers;
        result.wrongAnswersCount = game.wrongAnswers;
        result.averageAnswerTime = (game.correctAnswers + game.wrongAnswers > 0)
            ? game.totalAnswerTime / (game.correctAnswers + game.wrongAnswers)
            : 0;

        results.push_back(result);
    }


    return results;
}

std::vector<std::string> GameManager::getShuffledAnswers(const std::string& username) const
{
    auto it = m_games.find(username);
    if (it != m_games.end())
    {
        return it->second.currentShuffledAnswers;
    }
    return {};
}
