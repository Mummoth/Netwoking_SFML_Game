#include "Server.h"
#include "Utils.hpp"

Game::Server::Server()
{
	m_Listener.setBlocking(false);
	m_Clients.reserve(20);
}


void Game::Server::Start(const std::optional<unsigned short> port)
{
	Utils::PrintMsg("Server Starting...", INFO, SERVER);

	m_Port = port.value_or(m_Port);
	m_Thread = std::thread(&Server::Run, this);
}

void Game::Server::Stop()
{
	m_IsRunning.store(false);
	if (m_Thread.joinable())
		m_Thread.join();
}

void Game::Server::Run()
{
	// Signal that the server is running.
	m_IsRunning.store(true, std::memory_order_release);
	Utils::PrintMsg("Server is Running!", SUCCESS, SERVER);
	m_IsRunning.notify_one();

	if (m_Listener.listen(m_Port) == sf::Socket::Status::Done)
	{
		Utils::PrintMsg("Listening on port " + std::to_string(m_Port), INFO,
						SERVER);
	}
	else
		Utils::PrintMsg("Error binding listener socket!", ERROR, SERVER);

	// TODO:
	// [X] Handle connecting the hosting client on server start up.
	// [X] Setup retrieval of other client connections to the server.
	// [] Setup basic communication between the server and the client.

	// Main loop.
	while (m_IsRunning.load())
	{
		// Handle connecting clients.
		if (m_Clients.size() < m_MaxClients)
		{
			auto newClient = std::make_unique<sf::TcpSocket>();
			if (m_Listener.accept(*newClient) == sf::Socket::Status::Done)
			{
				Utils::PrintMsg("A client connected to the server!", INFO,
								SERVER);
				m_Clients.push_back(std::move(newClient));
				const auto socketPos = m_Clients.size() - 1;

				std::string message = "Connection accepted from ";

				// Check get incoming IP and Port and print them in a message.
				sf::IpAddress incomingIp = *m_Clients.at(socketPos)->
						getRemoteAddress();
				const auto port = m_Clients.at(socketPos)->getRemotePort();
				message += incomingIp.toString() + ":" + std::to_string(port);
				Utils::PrintMsg(message, SUCCESS, SERVER);

				if (m_Clients.size() >= m_MaxClients)
					Utils::PrintMsg("The server reached max capacity!", INFO,
									SERVER);
			}
			else
				Utils::PrintMsg("Error accepting connection!", ERROR, SERVER);
		}
		else
			m_Listener.close();


		sf::sleep(sf::milliseconds(10));
	}
}

bool Game::Server::Join(sf::TcpSocket& client, const sf::IpAddress serverIp,
						const std::optional<unsigned short> port)
{
	m_Port = port.value_or(m_Port); //< Check to what server to connect to.

	const sf::Socket::Status status = client.connect(
			serverIp, m_Port, sf::seconds(5));

	switch (status)
	{
	case sf::Socket::Status::Done:
		Utils::PrintMsg("Connected to the server!");
		return true;
	case sf::Socket::Status::NotReady:
		Utils::PrintMsg("Connection timed out.", WARNING);
		return false;
	default:
		Utils::PrintMsg("Error connecting to the server!", ERROR);
		return false;
	}
}
