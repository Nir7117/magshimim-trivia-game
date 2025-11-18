#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <thread>
#include <map>
#include "IRequestHandler.h"
#include "requestStructs.h"

#define INADDR_ANY "127.0.0.1"
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
#define PORT 8826
#pragma comment (lib, "ws2_32.lib")

class RequestHandlerFactory;

class Communicator
{
public:
    Communicator(RequestHandlerFactory& handlerFactory);
    ~Communicator();

    void setHandler(SOCKET sock, IRequestHandler* handler);

    void startHandleRequests();

private:
    void bindAndListen();
    void handleNewClient(SOCKET clientSocket);
    Buffer recvMsg(SOCKET clientSocket);

    std::map<SOCKET, IRequestHandler*> m_clients;
    RequestHandlerFactory& m_handlerFactory;
    SOCKET m_serverSocket;
};