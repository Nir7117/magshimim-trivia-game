#pragma once

#include "IRequestHandler.h"

#include "requestStructs.h"

class RequestHandlerFactory;
class LoginRequestHandler : public IRequestHandler
{
public:
    LoginRequestHandler(RequestHandlerFactory& handlerFactory);
    virtual ~LoginRequestHandler() = default;

    bool isRequestRelevant(const RequestInfo& request) override;
    RequestResult handleRequest(RequestInfo request) override;

private:
    RequestHandlerFactory& m_handlerFactory;
};
