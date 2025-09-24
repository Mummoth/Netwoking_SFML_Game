#include "Application.h"
#include <iostream>

Game::Application::Application(sf::RenderWindow& window) : m_Window{window}
{
	m_Drawables.reserve(25);

	if (!m_Txt.loadFromFile(TXT_PATH "mam.png"))
		std::cerr << "Error loading a texture from file!\n";

	m_Sprite = std::make_unique<sf::Sprite>(m_Txt);
	m_Drawables.push_back(m_Sprite.get());
}

void Game::Application::Input(float dt) {}

void Game::Application::Update(float dt)
{
	m_Sprite->setScale({0.5f,0.5f});
	m_Sprite->move({10.0f*dt,0.0f});
}

void Game::Application::Render() const
{
	m_Window.clear();

	for (const auto obj : m_Drawables)
		m_Window.draw(*obj);

	m_Window.display();
}
