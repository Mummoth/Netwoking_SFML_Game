#pragma once
#include <mutex>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

namespace Game
{
struct ClientInfo
{
	std::unique_ptr<sf::TcpSocket> Socket;
	sf::Color Colour;
	sf::Vector2f Position;
};

class Server
{
public:
	Server();
	~Server() { Stop(); }

	/// Start the TCP server.
	/// @param port An optional assigning of a different server port, if NULL,
	/// it uses the server's default port.
	void Start(std::optional<unsigned short> port = std::nullopt);

	/// Stop the server and join any loose threads.
	void Stop();

	/// Run and host a TCP server.
	void Run();

	/// Render server object.
	//void Render(sf::RenderWindow& window);

	/// @return If the server is actively running or not.
	bool IsRunning() const { return m_IsRunning.load(); }

	/// Wait for until the server starts running.
	void WaitUntilRunning() const { m_IsRunning.wait(false); }

	/// @return The server's port.
	unsigned short GetPort() const { return m_Port; }

private:
	// --- Networking Objects ---
	sf::TcpListener m_Listener{};
	unsigned short m_Port{13080};
	std::vector<ClientInfo> m_Clients{};
	const size_t m_MaxClients{1};

	// --- Multi-threading Objects ---
	std::thread m_Thread{};
	std::atomic<bool> m_IsRunning{false};
	std::mutex m_ClientMutex{};
};
}
