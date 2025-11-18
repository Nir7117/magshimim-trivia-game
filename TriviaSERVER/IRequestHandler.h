#pragma once
#include "requestStructs.h" 

class IRequestHandler
{
public:
    virtual ~IRequestHandler() = default;
    virtual bool isRequestRelevant(const RequestInfo& request) = 0;
    virtual RequestResult handleRequest(RequestInfo request) = 0;
};
