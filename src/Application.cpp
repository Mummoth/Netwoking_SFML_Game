#include "Application.h"

#include <iostream>

Game::Application::Application(sf::RenderWindow& window) : m_Window{&window}
{
	if (!m_Txt.loadFromFile(TXT_PATH "mam.png"))
		throw std::runtime_error("Error loading a texture from file!\n");

	if (!m_Font.openFromFile(FONT_PATH "arial.ttf"))
		throw std::runtime_error("Error loading a font from file!\n");

	m_Shape1.setRadius(100.0f);
	m_Shape1.setFillColor({0, 255, 0});

	m_Text = std::make_unique<sf::Text>(m_Font);
	m_Text->setCharacterSize(27);
	m_Text->setStyle(sf::Text::Bold);
	m_Text->setPosition({300.0f, 15.0f});
	m_Text->setString("Welcome to the game!");

	m_Socket = std::make_unique<sf::TcpSocket>();
	m_Listener.setBlocking(false);
}


void Game::Application::Input(const sf::Event::KeyPressed& keyPressed, float dt)
{
	if (keyPressed.scancode == sf::Keyboard::Scan::Num1 && !m_IsHosting)
	{
		m_Text->setString("You are currently hosting a game!");
		m_Shape1.setOutlineColor(sf::Color::Red);
		m_Shape1.setOutlineThickness(10.0f);
		m_IsHosting = true;
	}
}

void Game::Application::Update(float dt)
{
	if (m_IsHosting && !m_ServerThread.joinable() && m_Window->isOpen())
	{
		Utils::PrintMsg("Server Starting...");
		m_ServerThread = std::thread(&Application::RunTcpServer, this);
	}
}

void Game::Application::Render() const
{
	m_Window->clear();

	m_Window->draw(m_Shape1);
	m_Window->draw(*m_Text);

	m_Window->display();
}

void Game::Application::JoinThreads()
{
	if (m_ServerThread.joinable())
	{
		m_IsHosting = false;
		m_ServerThread.join();
	}
}

void Game::Application::RunTcpServer()
{
	if (m_Listener.listen(ServerPort) == sf::Socket::Status::Done)
		Utils::PrintMsg("Listening on port " + std::to_string(ServerPort));
	else
		Utils::PrintMsg("Error binding listener socket!", ERROR);

	// TODO:
	// . Handle connecting the hosting client on server start up.
	// . Setup retrieval of other client connections to the server.
	// . Setup basic communication between the server and the client.


	/*sf::Socket::Status status = m_Listener.accept(*m_Socket);
	if (status == sf::Socket::Status::Done)
	{
		const std::string message = "Connection accepted from ";
		// Check get incoming IP and Port and print them in a message.
		const sf::IpAddress incomingIp = *m_Socket->getRemoteAddress();
		const unsigned short port = m_Socket->getRemotePort();
		Utils::PrintMsg(
				message + incomingIp.toString() + ":" + std::to_string(port),
				SUCCESS);
	}*/

	while (m_IsHosting)
	{
		sf::sleep(sf::milliseconds(10));
	}
}
