#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "MenuRequestHandler.h"
#include "LoggedUser.h"
#include <iostream>
#include "Helper.h"
#include "RequestHandlerFactory.h"
#include "SqliteDataBase.h"
#include "Codes.h"



LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory)
    : m_handlerFactory(handlerFactory) {}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& request)
{
    std::cout << "[DEBUG]Got into LOGIN ISRELEVANT with ID: " << (int)request.id << std::endl;

    return request.id == LOGIN_CODE || request.id == SIGNUP_CODE;
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo request)
{
    RequestResult res;
    res.newHandler = nullptr;

    try
    {
        if (request.id == LOGIN_CODE)
        {
            LoginRequest loginReq = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);
            std::cout << "username: " << loginReq.username << " password: " << loginReq.password << std::endl;

            bool success = m_handlerFactory.getLoginManager().login(loginReq.username, loginReq.password);
            std::cout << "[DEBUG] login() success = " << success << std::endl;

            LoginResponse response;
            response.status = success ? 1 : 0;
            res.response = JsonResponsePacketSerializer::serializeLoginResponse(response);
            std::cout << "[DEBUG] serialized login response with status: " << response.status << std::endl;

            if (success)
            {
                
                LoggedUser loggedUser(loginReq.username);
                m_handlerFactory.getRoomManager().registerSocketForUser(loggedUser.getUsername(), request.sock);

                res.newHandler = m_handlerFactory.createMenuRequestHandler(loggedUser);
                std::cout << "[DEBUG] Switched to MenuRequestHandler" << std::endl;

            }
            else
            {
                res.newHandler = this; 
            }

        }

        else if (request.id == SIGNUP_CODE)
        {
            SignupRequest signupReq = JsonRequestPacketDeserializer::deserializeSignupRequest(request.buffer);
            std::cout << "username: " << signupReq.username << " password: " << signupReq.password << " email: " << signupReq.email << std::endl;

            bool success = m_handlerFactory.getLoginManager().signup(signupReq.username, signupReq.password, signupReq.email);
            std::cout << "[DEBUG] signup success = " << success << std::endl;

            SignupResponse response;
            response.status = success ? 1 : 0;
            res.response = JsonResponsePacketSerializer::serializeSignupResponse(response);
            res.newHandler = this;
        }

        else
        {
            throw std::exception("Invalid request id");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception in handleRequest: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Error: " + std::string(e.what());
        res.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
        res.newHandler = this;
    }

    return res;
}

