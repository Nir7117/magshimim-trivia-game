#include "Server.h"
#include <iostream>
#include <string>
#include "GameRequestHandler.h"


Server::Server()
	: m_loginManager(&m_database),
	m_statisticsManager(&m_database),
	m_gameManager(m_database),            
	m_handlerFactory(m_loginManager, m_roomManager, m_statisticsManager, m_gameManager),
	m_communicator(m_handlerFactory)
{
}




Server::~Server()
{
}

void Server::run()
{
	std::string clientMessage;
	std::thread th = std::thread(&Communicator::startHandleRequests, this->m_communicator);
	th.detach();

	while (clientMessage != "EXIT")
	{
		std::cin >> clientMessage;
	}
}
