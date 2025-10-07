#include "Client.h"
#include <random>
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
}


void Game::Client::Connect(const sf::IpAddress serverIp,
						   const unsigned short port)
{
	const sf::Socket::Status status = m_Socket->connect(
			serverIp, port, sf::seconds(5));

	sf::Packet packet;
	switch (status)
	{
	case sf::Socket::Status::Done:
		Utils::PrintMsg("Connected to the server!");
		if (m_Socket->receive(packet) == sf::Socket::Status::Done)
		{
			std::string msg;
			packet >> msg;
			Utils::PrintMsg(msg);
		}
		m_IsConnected = true;
		break;
	case sf::Socket::Status::NotReady:
		Utils::PrintMsg("Connection timed out.", WARNING);
		m_IsConnected = false;
		break;
	default:
		Utils::PrintMsg("Error connecting to the server!", ERROR);
		m_IsConnected = false;
		break;
	}
}

void Game::Client::Render(sf::RenderWindow& window) const
{
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
	return m_IsConnected;
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
