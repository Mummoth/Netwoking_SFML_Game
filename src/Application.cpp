#include "Application.h"
#include <iostream>
#include "Packet.hpp"
#include "Utils.hpp"

Game::Application::Application(sf::RenderWindow& window) : m_Window{&window}
{
	if (!m_Font.openFromFile(FONT_PATH "arial.ttf"))
		Utils::PrintMsg("Failed loading a font from file!", ERROR, CLIENT,
						true);

	m_Text = std::make_unique<sf::Text>(m_Font);
	m_Text->setCharacterSize(27);
	m_Text->setStyle(sf::Text::Bold);
	m_Text->setPosition({300.0f, 15.0f});
	m_Text->setString("Welcome to the game!");

	m_Client = std::make_unique<Client>();
	m_Server = std::make_unique<Server>();

	m_Client->SetPosition({300.0f, 250.0f});
}


void Game::Application::Input(const sf::Event::KeyPressed& keyPressed, float dt)
{
	// Handle hosting and joining a server.
	if (keyPressed.scancode == sf::Keyboard::Scan::Num1 && m_Client->CanHost()
		&& !m_Client->IsConnected())
	{
		// Start server and connect to it.
		m_Client->SetHosting(false);
		m_Server->Start();
		m_Server->WaitUntilRunning();
		// Try to connect self to the self-hosted server.
		m_Client->Connect(m_ServerIp, m_Server->GetPort());
		if (!m_Client->IsConnected())
		{
			m_Server->Stop();
			m_Client->SetHosting(true);
		}
		else
		{
			m_Client->StartListening(m_OtherClients);
			m_Text->setString("You are currently hosting a game!");
		}
	}
	else if (keyPressed.scancode == sf::Keyboard::Scan::Num2 && !m_Client->
			 IsConnected())
	{
		// Attempt to join a server.
		m_Client->SetHosting(false);
		m_Client->Connect(m_ServerIp, m_Server->GetPort());
		if (!m_Client->IsConnected())
			m_Client->SetHosting(true);
		else
		{
			m_Client->StartListening(m_OtherClients);
			m_Text->setString("You have joined a server!");
		}
	}
}

void Game::Application::Update(float dt)
{
	// Clean-up listen thread if no server.
	if ((!m_Server->IsRunning() || !m_Client->IsConnected())
		&& m_Client->IsListening())
		m_Client->StopListening();

	m_Client->ApplyPendingState();
}

void Game::Application::Render() const
{
	m_Window->clear();

	// Render main client.
	m_Client->Render(*m_Window);

	// Render application.
	m_Window->draw(*m_Text);

	// Render server-side.

	m_Window->display();
}
