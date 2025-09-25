#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Utils.hpp"
#include <SFML/Network.hpp>

namespace Game
{
class Application
{
public:
	Application(sf::RenderWindow& window);

	void Input(const sf::Event::KeyPressed& keyPressed, float dt);
	void Update(float dt);
	void Render() const;

	bool IsHosting() const {return m_IsHosting;}
	void SetHosting(const bool b) {m_IsHosting = b;}

public:
	const int m_ServerPort = 13080;

private:
	sf::RenderWindow* m_Window;

	sf::Font m_Font{};
	std::unique_ptr<sf::Text> m_Text{};
	sf::Texture m_Txt{};
	sf::CircleShape m_Shape1{};

	bool m_IsHosting{false};
	sf::TcpListener m_Listener{};
};
}
