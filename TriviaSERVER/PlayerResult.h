#pragma once

#include <string>

struct PlayerResult
{
    std::string username;
    unsigned int correctAnswersCount = 0;
    unsigned int wrongAnswersCount = 0;
    float averageAnswerTime = 0;
};
