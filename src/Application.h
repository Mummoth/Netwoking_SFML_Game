#pragma once
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Client.h"
#include "Server.h"

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

	/// @return Returns the reference to the application's Server object.
	Server& GetServer() const { return *m_Server; }

	/// @return Returns the reference to the application's Client object.
	Client& GetClient() const { return *m_Client; }

private:
	// --- System Objects ---
	sf::RenderWindow* m_Window;
	std::unique_ptr<Server> m_Server{};
	std::unique_ptr<Client> m_Client{};

	// --- Game Objects ---
	sf::Font m_Font{};
	std::unique_ptr<sf::Text> m_Text{};

	// --- Networking Objects ---
	sf::IpAddress m_ServerIp{127, 0, 0, 1};
	std::atomic<std::uint16_t> m_OtherClients{0};
};
}
