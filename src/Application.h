#pragma once
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Server.h"
#include "Utils.hpp"

namespace Game
{
class Application
{
public:
	Application(sf::RenderWindow& window);

	/// Handle user input during application's lifetime.
	void Input(const sf::Event::KeyPressed& keyPressed, float dt);
	/// Handle game updates during application's lifetime.
	void Update(float dt);
	/// Render application's game objects.
	void Render() const;

	/// @return The reference to the application's Server object.
	Server& GetServer() const { return *m_Server; }

private:
	// --- System Objects ---
	sf::RenderWindow* m_Window;
	std::unique_ptr<Server> m_Server{};

	// --- Game Objects ---
	sf::Font m_Font{};
	std::unique_ptr<sf::Text> m_Text{};
	sf::Texture m_Txt{};
	sf::CircleShape m_Shape1{};

	// --- Networking Objects ---
	bool m_HasConnected{false};
	bool m_CanHost{true};
	sf::IpAddress m_ServerIp{127, 0, 0, 1};
	std::unique_ptr<sf::TcpSocket> m_ClientSocket{};
};
}
