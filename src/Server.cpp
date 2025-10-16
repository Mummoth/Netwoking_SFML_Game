#include "Server.h"
#include <random>
#include "Packet.hpp"
#include "Utils.hpp"

Game::Server::Server()
{
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	m_Listener.setBlocking(false);
	m_Clients.reserve(20);
}


void Game::Server::Start(const std::optional<unsigned short> port)
{
	Utils::PrintMsg("Server Starting...", INFO, SERVER);

	m_Port = port.value_or(m_Port);
	m_ServerThread = std::thread(&Server::Run, this);
}

void Game::Server::Stop()
{
	m_IsRunning.store(false);
	if (m_ServerThread.joinable())
		m_ServerThread.join();
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

	// Main loop.
	while (m_IsRunning.load())
	{
		// Handle connecting clients.
		if (m_Clients.size() < m_MaxClients)
		{
			auto clientSocket = std::make_unique<sf::TcpSocket>();
			switch (m_Listener.accept(*clientSocket))
			{
			case sf::Socket::Status::Done:
				{
					Utils::PrintMsg("A client connected to the server!", INFO,
									SERVER);

					// Wait for client to send its colour.
					sf::Packet receivedPacket;
					sf::Color clientColour;
					if (clientSocket->receive(receivedPacket) ==
						sf::Socket::Status::Done)
					{
						std::uint8_t packetType;
						receivedPacket >> packetType;
						if (static_cast<PacketType>(packetType) ==
							PacketType::PLAYER_CONNECT)
						{
							uint32_t colourInt;
							receivedPacket >> colourInt;
							clientColour = sf::Color(colourInt);
							receivedPacket.clear();
						}
					}

					// Construct new client info object.
					ClientInfo newClient{};
					newClient.Socket = std::move(clientSocket);
					newClient.Position = {50.0f * m_Clients.size(), 50.0f};
					newClient.Colour = clientColour;
					{
						std::lock_guard lock(m_ClientMutex);
						m_Clients.push_back(std::move(newClient));
					}

					// Print out a server message.
					std::string message = "Connection accepted from ";

					// Check get incoming IP and Port and print them in a message.
					const ClientInfo& clientInfo = m_Clients.at(
							m_Clients.size() - 1);
					sf::IpAddress incomingIp = *clientInfo.Socket->
							getRemoteAddress();
					const auto port = clientInfo.Socket->getRemotePort();
					message += incomingIp.toString() + ":" + std::to_string(
							port);
					Utils::PrintMsg(message, SUCCESS, SERVER);

					// Send a welcome message to the client.
					sf::Packet welcomePacket;
					welcomePacket << static_cast<std::uint8_t>(
						PacketType::PLAYER_MESSAGE);
					welcomePacket << "Welcome to the server!";
					static_cast<void>(clientInfo.Socket->send(welcomePacket));

					// Broadcast all clients' positions & colours to everyone.
					sf::Packet updatePacket;
					updatePacket << static_cast<std::uint8_t>(
						PacketType::PLAYER_LIST);
					updatePacket << static_cast<std::uint16_t>(m_Clients.
						size());

					updatePacket << clientInfo.Colour.toInteger();
					updatePacket << clientInfo.Position.x << clientInfo.Position
							.y;

					/*for (const auto& client : m_Clients)
					{
						updatePacket << client.Colour.toInteger();
						updatePacket << client.Position.x << client.Position.y;
					}*/

					// Cast to VOID to ignore return value.
					//static_cast<void>(clientInfo.Socket->send(updatePacket));
					/*for (const auto& client : m_Clients)
						static_cast<void>(client.Socket->send(updatePacket));*/


					// Check if the server is at max capacity.
					if (m_Clients.size() >= m_MaxClients)
					{
						Utils::PrintMsg("The server reached max capacity!",
										WARNING,
										SERVER);
						m_Listener.close();
					}
					break;
				}
			case sf::Socket::Status::Error:
				Utils::PrintMsg("Error accepting connection!", ERROR, SERVER);
				break;
			}
		}


		sf::sleep(sf::milliseconds(10));
	}
}
