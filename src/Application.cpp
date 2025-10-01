#include "Application.h"

#include <iostream>

Game::Application::Application(sf::RenderWindow& window) : m_Window{&window}
{
	if (!m_Txt.loadFromFile(TXT_PATH "mam.png"))
		Utils::PrintMsg("Failed loading a texture from file!", ERROR, CLIENT,
						true);

	if (!m_Font.openFromFile(FONT_PATH "arial.ttf"))
		Utils::PrintMsg("Failed loading a font from file!", ERROR, CLIENT,
						true);

	m_Shape1.setRadius(100.0f);
	m_Shape1.setFillColor({0, 255, 0});

	m_Text = std::make_unique<sf::Text>(m_Font);
	m_Text->setCharacterSize(27);
	m_Text->setStyle(sf::Text::Bold);
	m_Text->setPosition({300.0f, 15.0f});
	m_Text->setString("Welcome to the game!");

	m_ClientSocket = std::make_unique<sf::TcpSocket>();
	m_Listener.setBlocking(false);
	m_Clients.reserve(20);
}


void Game::Application::Input(const sf::Event::KeyPressed& keyPressed, float dt)
{
	if (keyPressed.scancode == sf::Keyboard::Scan::Num1 && m_CanHost
		&& !m_ServerThread.joinable())
	{
		m_Text->setString("You are currently hosting a game!");
		m_Shape1.setOutlineColor(sf::Color::Red);
		m_Shape1.setOutlineThickness(10.0f);
		m_IsHosting = true;
	} else if (keyPressed.scancode == sf::Keyboard::Scan::Num2)
		JoinServer();
}

void Game::Application::Update(float dt)
{
	if (m_IsHosting && !m_ServerThread.joinable() && m_Window->isOpen())
	{
		m_CanHost = false;
		Utils::PrintMsg("Server Starting...", INFO, SERVER);
		m_ServerThread = std::thread(&Application::RunTcpServer, this);
	}

	// Try to connect self to the self-hosted server.
	if (m_ServerThread.joinable() && !m_HasConnected)
		JoinServer();
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
	{
		Utils::PrintMsg("Listening on port " + std::to_string(ServerPort), INFO,
						SERVER);
	} else
		Utils::PrintMsg("Error binding listener socket!", ERROR, SERVER);

	// TODO:
	// [X] Handle connecting the hosting client on server start up.
	// [X] Setup retrieval of other client connections to the server.
	// [] Setup basic communication between the server and the client.

	while (m_IsHosting)
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
			} else
				Utils::PrintMsg("Error accepting connection!", ERROR, SERVER);
		} else
			m_Listener.close();


		sf::sleep(sf::milliseconds(10));
	}
}

void Game::Application::JoinServer()
{
	m_CanHost = false;

	const sf::Socket::Status status = m_ClientSocket->connect(
			m_ServerIp, ServerPort, sf::seconds(5));
	if (status != sf::Socket::Status::Done)
		Utils::PrintMsg("Error connecting to the server!", ERROR);
	else if (status == sf::Socket::Status::NotReady)
		Utils::PrintMsg("Connection timed out.");
	else if (status == sf::Socket::Status::Done)
	{
		Utils::PrintMsg("Connected to the server!");
		m_HasConnected = true;
	}
}
