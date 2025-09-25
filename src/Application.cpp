#include "Application.h"
#include <iostream>

Game::Application::Application(sf::RenderWindow& window) : m_Window{&window}
{
	if (!m_Txt.loadFromFile(TXT_PATH "mam.png"))
		throw std::runtime_error("Error loading a texture from file!\n");

	if (!m_Font.openFromFile(FONT_PATH "arial.ttf"))
		throw std::runtime_error("Error loading a font from file!\n");

	m_Shape1.setRadius(100.0f);
	m_Shape1.setFillColor({0,255,0});

	m_Text = std::make_unique<sf::Text>(m_Font);
	m_Text->setCharacterSize(27);
	m_Text->setStyle(sf::Text::Bold);
	m_Text->setPosition({300.0f, 15.0f});
	m_Text->setString("Welcome to the game!");
}

void Game::Application::Input(const sf::Event::KeyPressed& keyPressed, float dt)
{
	if (keyPressed.scancode == sf::Keyboard::Scan::Num1 && !m_IsHosting)
	{
		//std::printf("You are currently hosting a game!\n");
		m_Text->setString("You are currently hosting a game!");
		m_Shape1.setOutlineColor(sf::Color::Red);
		m_Shape1.setOutlineThickness(10.0f);
		m_IsHosting = true;
	}
}

void Game::Application::Update(float dt)
{
}

void Game::Application::Render() const
{
	m_Window->clear();

	m_Window->draw(m_Shape1);
	m_Window->draw(*m_Text);

	m_Window->display();
}
