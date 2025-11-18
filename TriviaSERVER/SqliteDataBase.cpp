#include "SqliteDataBase.h"
#include <iostream>

SqliteDataBase::SqliteDataBase()
{
    open();
    createUsersTable();
    createQuestionsTable();
    createStatisticsTable();
}

SqliteDataBase::~SqliteDataBase()
{
    close();
}

bool SqliteDataBase::open()
{
    int result = sqlite3_open("TriviaDB.sqlite", &_db);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(_db) << std::endl;
        return false;
    }
    return true;
}

void SqliteDataBase::close()
{
    sqlite3_close(_db);
}

void SqliteDataBase::createUsersTable()
{
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY,"
        "password TEXT NOT NULL,"
        "email TEXT NOT NULL);";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(_db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to create users table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void SqliteDataBase::createQuestionsTable()
{
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS Questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            question TEXT NOT NULL,
            correct_answer TEXT NOT NULL,
            wrong_answer1 TEXT NOT NULL,
            wrong_answer2 TEXT NOT NULL,
            wrong_answer3 TEXT NOT NULL
        );
    )";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(_db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to create questions table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void SqliteDataBase::createStatisticsTable()
{
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS Statistics (
            username TEXT,
            is_correct INTEGER,
            answer_time REAL
        );
    )";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(_db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to create statistics table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

bool SqliteDataBase::doesUserExist(const std::string& username)
{
    std::string query = "SELECT 1 FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    std::cout << "[DEBUG] Checking existence of: " << username << " → rc = " << rc << std::endl;

    bool exists = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

bool SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
    std::string query = "SELECT 1 FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    std::cout << "[DEBUG] Checking match for: " << username << " + " << password << " → rc = " << rc << std::endl;

    bool match = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return match;
}

bool SqliteDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
    std::string query = "INSERT INTO users (username, password, email) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

std::vector<Question> SqliteDataBase::getQuestions(const int& amount)
{
    std::cout << "[DEBUG] GOT INTO GETQUESTIONS DB ";

    std::vector<Question> questions;
    std::string query = "SELECT question, correct_answer, wrong_answer1, wrong_answer2, wrong_answer3 FROM Questions LIMIT ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, amount);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Question q;
        q.question = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        q.correctAnswer = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        q.incorrectAnswers.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        q.incorrectAnswers.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        q.incorrectAnswers.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

        questions.push_back(q);
    }
    std::cout << "[DEBUG] getQuestions returned " << questions.size() << " questions\n";

    sqlite3_finalize(stmt);
    return questions;
}

double SqliteDataBase::getPlayerAverageAnswerTime(const std::string& username)
{
    std::string query = "SELECT AVG(answer_time) FROM Statistics WHERE username = ?;";
    sqlite3_stmt* stmt;
    double avgTime = 0;

    sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
        avgTime = sqlite3_column_double(stmt, 0);

    sqlite3_finalize(stmt);
    return avgTime;
}

int SqliteDataBase::getNumOfCorrectAnswers(const std::string& username)
{
    std::string query = "SELECT COUNT(*) FROM Statistics WHERE username = ? AND is_correct = 1;";
    sqlite3_stmt* stmt;
    int count = 0;

    sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return count;
}

int SqliteDataBase::getNumOfTotalAnswers(const std::string& username)
{
    std::string query = "SELECT COUNT(*) FROM Statistics WHERE username = ?;";
    sqlite3_stmt* stmt;
    int count = 0;

    sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return count;
}

int SqliteDataBase::getNumOfPlayerGames(const std::string& username)
{
    std::string query = "SELECT COUNT(DISTINCT rowid / 10) FROM Statistics WHERE username = ?;";
    sqlite3_stmt* stmt;
    int count = 0;

    sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return count;
}

std::vector<std::string> SqliteDataBase::getHighScores()
{
	std::vector<std::string> highScores;
	std::string query = "SELECT username, SUM(is_correct) as score FROM Statistics GROUP BY username ORDER BY score DESC LIMIT 5;";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		int score = sqlite3_column_int(stmt, 1);
		highScores.push_back("Username: " + username + ", Score: " + std::to_string(score));
	}
	sqlite3_finalize(stmt);
	return highScores;
}
int SqliteDataBase::getPlayerScore(const std::string& username)
{
    int score = 0;
    sqlite3_stmt* stmt;
    std::string sql = "SELECT score FROM Statistics WHERE username = ?";

    if (sqlite3_prepare_v2(_db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            score += sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    return score;
}
bool SqliteDataBase::insertPlayerAnswerStats(const std::string& username, bool isCorrect, double answerTime)
{
    std::string query = "INSERT INTO Statistics (username, is_correct, answer_time) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, isCorrect ? 1 : 0);
    sqlite3_bind_double(stmt, 3, answerTime);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}
