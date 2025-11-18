#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "Helper.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <WinSock2.h> 


static const unsigned int IFACE = 0;

Communicator::Communicator(RequestHandlerFactory& handlerFactory) :
    m_handlerFactory(handlerFactory)
{
    m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET)
        throw std::exception(__FUNCTION__ " - socket");
}

Communicator::~Communicator()
{
    try
    {
        ::closesocket(m_serverSocket);
    }
    catch (...) {}
}

void Communicator::startHandleRequests()
{
    bindAndListen();
}

void Communicator::bindAndListen()
{
    sockaddr_in sa = { 0 };
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = IFACE;
    sa.sin_port = htons(PORT); // Using port 8826

    if (::bind(m_serverSocket, (SOCKADDR*)&sa, sizeof(sa)) == SOCKET_ERROR)
        throw std::exception(__FUNCTION__ " - Bind");

    std::cout << "Binded" << std::endl;

    if (::listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
        throw std::exception(__FUNCTION__ " - Listen");

    std::cout << "Listening..." << std::endl;

    while (true)
    {
        std::cout << "Waiting for client connection....." << std::endl;

        SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);

        if (clientSocket == INVALID_SOCKET)
            throw std::exception(__FUNCTION__);

        std::cout << "Client accepted!" << std::endl;

        IRequestHandler* clientHandle = m_handlerFactory.createLoginRequestHandler();
        m_clients.insert(std::pair<SOCKET, IRequestHandler*>(clientSocket, clientHandle));

        std::thread th(&Communicator::handleNewClient, this, clientSocket);
        th.detach();
    }
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
    while (true)
    {
        Buffer buffer = recvMsg(clientSocket);

        if (buffer.size() < 5)
            continue;

        int sizeBuff = Helper::ByteToInt(buffer);
        if (buffer.size() < sizeBuff + 5)
            continue;

        Buffer subBuff(buffer.begin() + 5, buffer.begin() + sizeBuff + 5);
        RequestInfo reqInfo = { (int)buffer[0], std::time(0), subBuff, clientSocket };

        std::cout << "[SERVER DEBUG] Got request ID: " << reqInfo.id << std::endl;

        if (m_clients.find(clientSocket) == m_clients.end() || m_clients[clientSocket] == nullptr)
            break;

        IRequestHandler* handler = m_clients[clientSocket];

        std::cout << "[DEBUG] current handler type: " << typeid(*handler).name() << std::endl;

        // 🔍 נוודא שהבקשה רלוונטית לפני שאנחנו מפעילים את ה-handler
        if (!handler->isRequestRelevant(reqInfo))
        {
            std::cout << "[DEBUG] Request not relevant. ID: " << reqInfo.id << std::endl;
            continue;
        }

        // ✅ רק עכשיו מטפלים בבקשה
        RequestResult res;
        try
        {
            res = handler->handleRequest(reqInfo);
        }
        catch (const std::exception& e)
        {
            std::cerr << "[EXCEPTION] in handleRequest: " << e.what() << std::endl;
            continue;
        }

        if (res.newHandler != nullptr && res.newHandler != handler)
        {
            std::cout << "[DEBUG] Swapping handler" << std::endl;
            delete handler;
            m_clients[clientSocket] = res.newHandler;
        }

        handler = m_clients[clientSocket];
        std::cout << "[DEBUG] NEW current handler type: " << typeid(*handler).name() << std::endl;

        Helper::send_Msg(clientSocket, res.response);
    }
}


Buffer Communicator::recvMsg(SOCKET clientSocket)
{
    Buffer buffer(1024);
    int res = recv(clientSocket, reinterpret_cast<char*>(buffer.data()), 1024, 0);

    if (res == INVALID_SOCKET)
        std::cout << "Error receiving message" << std::endl;
    else
    {
        std::cout << "[SERVER DEBUG] Raw received buffer (" << res << " bytes): ";
        for (int i = 0; i < res; ++i)
        {
            std::cout << std::hex << std::uppercase << (int)buffer[i] << " ";
        }
        std::cout << std::dec << std::endl;

        buffer.resize(res);
    }
       

    return buffer;
}
void Communicator::setHandler(SOCKET sock, IRequestHandler* handler)
{
    if (m_clients.find(sock) != m_clients.end())
    {
        delete m_clients[sock]; 
        m_clients[sock] = handler;
    }
}
