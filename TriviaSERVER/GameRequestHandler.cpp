#include "GameRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "Codes.h"
#include <iostream>


GameRequestHandler::GameRequestHandler(LoggedUser user, RoomManager& roomManager, GameManager& gameManager, RequestHandlerFactory& handlerFactory, unsigned int questionCount, unsigned int time)
    : m_user(user), m_roomManager(roomManager), m_gameManager(gameManager), m_handlerFactory(handlerFactory), m_questionCount(questionCount), m_answerTimeout(time)
{
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& info)
{
    return info.id == LEAVE_GAME_REQUEST_CODE ||
        info.id == GET_QUESTION_REQUEST_CODE ||
        info.id == SUBMIT_ANSWER_REQUEST_CODE ||
        info.id == GET_GAME_RESULTS_REQUEST_CODE ||
        info.id == 204 || info.id == 5 || info.id == 6;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo info)
{
    RequestResult result;
    

     if (info.id == 204)
    {
    std::cout << "[DEBUG] Ignoring getPlayersInRoom (204) during game" << std::endl;

    ErrorResponse err;
    err.status = 1;
    err.message = "Not available during game.";

    result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
    result.newHandler = this;
    return result;
    }

    if (info.id == LEAVE_GAME_REQUEST_CODE)
    {
        m_roomManager.removeUserFromRoom(m_user);

        LeaveGameResponse response{ 1 };
        result.response = JsonResponsePacketSerializer::serializeLeaveGameResponse(response);
        result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    }
    else if (info.id == GET_QUESTION_REQUEST_CODE)
    {
        auto question = m_gameManager.getQuestionForUser(m_user.getUsername(), m_questionCount);

        GetQuestionResponse response;
        response.status = 1;
        response.question = question;

        const auto& shuffled = m_gameManager.getShuffledAnswers(m_user.getUsername());
        for (int i = 0; i < shuffled.size(); i++)
        {
            response.answers[i] = shuffled[i];
        }

        response.answerTimeout = m_answerTimeout;
        response.questionCount = m_questionCount;

        result.response = JsonResponsePacketSerializer::serializeGetQuestionResponse(response);
        result.newHandler = this;
    }





    else if (info.id == SUBMIT_ANSWER_REQUEST_CODE)
    {
        auto req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
        int correctAnswer = m_gameManager.submitAnswer(m_user.getUsername(), req.answerId);

        SubmitAnswerResponse response;
        response.status = 1;
        response.correctAnswerId = correctAnswer;
        response.isCorrect = (req.answerId == correctAnswer); 

        result.response = JsonResponsePacketSerializer::serializeSubmitAnswerResponse(response);
        result.newHandler = this;
    }


    else if (info.id == GET_GAME_RESULTS_REQUEST_CODE)
    {
        auto results = m_gameManager.getGameResults(m_user.getUsername());
        std::cout << "[DEBUG] Results size: " << results.size() << std::endl;

        GetGameResultsResponse response;
        response.status = results.empty() ? 0 : 1;
        response.results = results;

        result.response = JsonResponsePacketSerializer::serializeGetGameResultsResponse(response);
        result.newHandler = this;
    }

    return result;
}
