#include "Client.h"
#include <random>
#include "Packet.hpp"
#include "Utils.hpp"

Game::Client::Client()
{
	if (!m_Txt.loadFromFile(TXT_PATH "mam.png"))
		Utils::PrintMsg("Failed loading a texture from file!", ERROR, CLIENT,
						true);

	// Generate a random colour.
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution randColor(0, 255);
	auto r = static_cast<uint8_t>(randColor(gen));
	auto g = static_cast<uint8_t>(randColor(gen));
	auto b = static_cast<uint8_t>(randColor(gen));
	m_Visuals.setFillColor({r, g, b});

	// Create TCP socket.
	m_Socket = std::make_unique<sf::TcpSocket>();
	m_Socket->setBlocking(false);
}

Game::Client::~Client()
{
	Disconnect();
}


void Game::Client::Connect(const sf::IpAddress serverIp,
						   const unsigned short port)
{
	// Set socket to block thread for timeout. Unblock it after.
	m_Socket->setBlocking(true);
	const sf::Socket::Status status = m_Socket->connect(
			serverIp, port, sf::seconds(5));
	m_Socket->setBlocking(false);

	switch (status)
	{
	case sf::Socket::Status::Done:
		{
			Utils::PrintMsg("Connected to the server!");
			m_IsVisible = true;
			m_IsConnected.store(true);

			sf::Packet connectPacket;
			connectPacket << static_cast<std::uint8_t>(
				PacketType::PLAYER_CONNECT);
			connectPacket << m_Visuals.getFillColor().toInteger();
			static_cast<void>(m_Socket->send(connectPacket));

			//StartConnectionMonitor();
			break;
		}
	case sf::Socket::Status::NotReady:
		Utils::PrintMsg("Connection timed out.", WARNING);
		m_IsConnected.store(false);
		break;
	case sf::Socket::Status::Error:
		Utils::PrintMsg("Error connecting to the server!", ERROR);
		m_IsConnected.store(false);
		break;
	}
}

void Game::Client::Disconnect()
{
	// Exits imminently if not connect, assuming you're already disconnected.
	if (!m_IsConnected.load())
		return;

	m_Socket->disconnect();
	m_IsConnected.store(false);
	StopListening();

	if (m_ConnectionThread.joinable())
		m_ConnectionThread.join();

	Utils::PrintMsg("Disconnected from the server.", WARNING);
}

void Game::Client::StartListening(std::atomic<std::uint16_t>& otherClients)
{
	m_ListenThread = std::thread(&Client::Listen, this, std::ref(otherClients));
}

void Game::Client::StopListening()
{
	if (m_ListenThread.joinable())
		m_ListenThread.join();
}

bool Game::Client::IsListening() const
{
	return m_ListenThread.joinable();
}

void Game::Client::UpdateData(sf::Packet& packet)
{
	std::uint32_t colourInt;
	float posX, posY;
	packet >> colourInt >> posX >> posY;

	m_Visuals.setFillColor(sf::Color{colourInt});
	m_Visuals.setPosition({posX, posY});
}

void Game::Client::ApplyPendingState()
{
	std::lock_guard lock(m_StateMutex);
	if (m_HasPendingState)
	{
		m_Visuals.setFillColor(sf::Color{m_PendingColour});
		m_Visuals.setPosition(m_PendingPos);
		m_HasPendingState = false;
	}
}

void Game::Client::StartConnectionMonitor()
{
	/*m_ConnectionThread = std::thread([this]
	{
		sf::Packet ping;
		while (m_IsConnected.load())
		{
			sf::Socket::Status status = m_Socket->receive(ping);
			if (status == sf::Socket::Status::Disconnected)
			{
				Utils::PrintMsg("Lost connection to the server.", WARNING);
				m_IsConnected.store(false);
				break;
			}

			ping.clear();
			sf::sleep(sf::milliseconds(100));
		}
	});*/
}

void Game::Client::Listen(std::atomic<std::uint16_t>& otherClients)
{
	sf::Packet receivedPacket;
	while (m_IsConnected.load())
	{
		// Check if the packet was received successfully.
		switch (m_Socket->receive(receivedPacket))
		{
		case sf::Socket::Status::Done:
			break;
		case sf::Socket::Status::NotReady:
			sf::sleep(sf::milliseconds(10));
			continue;
		case sf::Socket::Status::Partial:
			break;
		case sf::Socket::Status::Disconnected:
			m_IsConnected.store(false);
			return;
		case sf::Socket::Status::Error:
			Utils::PrintMsg("Error receiving packet!", ERROR);
			continue;
		}

		// Retrieve the packet header.
		std::uint8_t packetTypeInt;
		receivedPacket >> packetTypeInt;

		switch (static_cast<PacketType>(packetTypeInt))
		{
		case PacketType::PLAYER_LIST:
			{
				std::uint16_t others;
				receivedPacket >> others;
				otherClients.store(others);

				std::uint32_t colourInt;
				float posX, posY;
				receivedPacket >> colourInt >> posX >> posY;
				{
					std::lock_guard lock(m_StateMutex);
					m_PendingColour = colourInt;
					m_PendingPos = {posX, posY};
					m_HasPendingState = true;
				}

				break;
			}
		case PacketType::PLAYER_UPDATE:
			{
				std::uint32_t colourInt;
				float posX, posY;
				receivedPacket >> colourInt >> posX >> posY;
				{
					std::lock_guard lock(m_StateMutex);
					m_PendingColour = colourInt;
					m_PendingPos = {posX, posY};
					m_HasPendingState = true;
				}
				break;
			}
		case PacketType::PLAYER_MESSAGE:
			{
				std::string message;
				receivedPacket >> message;
				Utils::PrintMsg(message);
				break;
			}
		default:
			break;
		}

		receivedPacket.clear();
		sf::sleep(sf::milliseconds(10));
	}
}

void Game::Client::Render(sf::RenderWindow& window) const
{
	if (m_IsVisible)
		window.draw(m_Visuals);
}

void Game::Client::Move(const sf::Vector2f offset)
{
	m_Visuals.move(offset);
}

sf::Vector2f Game::Client::GetPosition() const
{
	return m_Visuals.getPosition();
}

void Game::Client::SetPosition(const sf::Vector2f pos)
{
	m_Visuals.setPosition(pos);
}

sf::Vector2f Game::Client::GetSize() const
{
	return m_Visuals.getSize();
}

void Game::Client::SetSize(const sf::Vector2f size)
{
	m_Visuals.setSize(size);
}

sf::Angle Game::Client::GetRotation() const
{
	return m_Visuals.getRotation();
}

void Game::Client::SetRotation(const sf::Angle angle)
{
	m_Visuals.setRotation(angle);
}

sf::Color Game::Client::GetColour() const
{
	return m_Visuals.getFillColor();
}

void Game::Client::SetColour(const sf::Color colour)
{
	m_Visuals.setFillColor(colour);
}

bool Game::Client::IsConnected() const
{
	return m_IsConnected.load();
}

bool Game::Client::CanHost() const
{
	return m_CanHost;
}

void Game::Client::SetHosting(const bool b)
{
	m_CanHost = b;
}

sf::TcpSocket& Game::Client::GetSocket() const
{
	return *m_Socket;
}

bool Game::Client::GetVisible() const
{
	return m_IsVisible;
}

void Game::Client::SetVisible(const bool b)
{
	m_IsVisible = b;
}
