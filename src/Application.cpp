#include "Application.h"
#include <iostream>

Game::Application::Application(sf::RenderWindow& window) : m_Window{&window}
{
	if (!m_Txt.loadFromFile(TXT_PATH "mam.png"))
		throw std::runtime_error("Error loading a texture from file!\n");

	m_Sprite = std::make_unique<sf::Sprite>(m_Txt);

	m_Shape1.setRadius(100.0f);
	m_Shape1.setFillColor({0,255,0});
}

void Game::Application::Input(float dt) {}

void Game::Application::Update(float dt)
{
	m_Sprite->setScale({0.5f,0.5f});
	//m_Sprite->move({10.0f*dt,0.0f});
}

void Game::Application::Render() const
{
	m_Window->clear();

	//m_Window->draw(*m_Sprite);
	m_Window->draw(m_Shape1);

	m_Window->display();
}
