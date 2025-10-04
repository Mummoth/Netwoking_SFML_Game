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
	m_Server = std::make_unique<Server>();
}


void Game::Application::Input(const sf::Event::KeyPressed& keyPressed, float dt)
{
	if (keyPressed.scancode == sf::Keyboard::Scan::Num1)
	{
		m_Text->setString("You are currently hosting a game!");
		m_Shape1.setOutlineColor(sf::Color::Red);
		m_Shape1.setOutlineThickness(10.0f);

		// Start server and connect to it.
		m_CanHost = false;
		m_Server->Start();
		m_Server->WaitUntilRunning();
		// Try to connect self to the self-hosted server.
		m_HasConnected = m_Server->Join(*m_ClientSocket, m_ServerIp);
		if (!m_HasConnected)
		{
			m_Server->Stop();
			m_CanHost = true;
		}
	}
	else if (keyPressed.scancode == sf::Keyboard::Scan::Num2)
	{
		m_Text->setString("You have joined a server!");

		// Attempt to join a server.
		m_CanHost = false;
		m_HasConnected = m_Server->Join(*m_ClientSocket, m_ServerIp);
		if (!m_HasConnected)
			m_CanHost = true;
	}
}

void Game::Application::Update(float dt)
{}

void Game::Application::Render() const
{
	m_Window->clear();

	m_Window->draw(m_Shape1);
	m_Window->draw(*m_Text);

	m_Window->display();
}
