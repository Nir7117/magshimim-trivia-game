#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <random>

class Question
{
public:
    std::string question;
    std::string correctAnswer;
    std::vector<std::string> incorrectAnswers;

    std::vector<std::string> getShuffledAnswers() const
    {
        std::vector<std::string> allAnswers = incorrectAnswers;
        allAnswers.push_back(correctAnswer);

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(allAnswers.begin(), allAnswers.end(), g);

        return allAnswers;
    }
};
