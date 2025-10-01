#pragma once
#include <thread>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Utils.hpp"

namespace Game
{
class Application
{
public:
	Application(sf::RenderWindow& window);
	~Application() { JoinThreads(); }

	/// Handle user input during application's lifetime.
	void Input(const sf::Event::KeyPressed& keyPressed, float dt);
	/// Handle game updates during application's lifetime.
	void Update(float dt);
	/// Render application's game objects.
	void Render() const;

	/// Whether the application is currently acting as a host.
	bool IsHosting() const { return m_IsHosting; }
	/// Set the application's hosting status.
	/// @param b Whether to make application host or not.
	void SetHosting(const bool b) { m_IsHosting = b; }

	/// Join all currently active application threads.
	void JoinThreads();

public:
	const unsigned short ServerPort = 13080;

private:
	/// Run and host a TCP server.
	void RunTcpServer();

	void JoinServer();

private:
	// --- System Objects ---
	sf::RenderWindow* m_Window;
	std::thread m_ServerThread{};

	// --- Game Objects ---
	sf::Font m_Font{};
	std::unique_ptr<sf::Text> m_Text{};
	sf::Texture m_Txt{};
	sf::CircleShape m_Shape1{};

	// --- Networking Objects ---
	std::atomic<bool> m_IsHosting{false};
	bool m_HasConnected{false};
	bool m_CanHost{true};
	sf::IpAddress m_ServerIp{127, 0, 0, 1};
	sf::TcpListener m_Listener{};
	std::unique_ptr<sf::TcpSocket> m_ClientSocket{};
	std::vector<std::unique_ptr<sf::TcpSocket>> m_Clients{};
	const short m_MaxClients{2};
};
}
