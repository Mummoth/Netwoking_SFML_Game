#pragma once
#include <mutex>
#include <thread>
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"

namespace Game
{
class Client
{
public:
	Client();
	~Client();

	/// Connect the client to the desired server.
	/// @param serverIp The IP of the desired server.
	/// @param port The port of the desired server on the network.
	void Connect(sf::IpAddress serverIp, unsigned short port);
	/// Disconnect from the current server.
	void Disconnect();

	/// Start listening for server updates on a separate thread.
	void StartListening(std::atomic<std::uint16_t>& otherClients);
	/// Stop listening for server updates.
	void StopListening();
	/// @return Returns whether the client is still activity listening for
	/// server updates.
	bool IsListening() const;

	/// Update client data.
	void UpdateData(sf::Packet& packet);

	void ApplyPendingState();

	/// Render client.
	void Render(sf::RenderWindow& window) const;

	/// Move the client.
	void Move(sf::Vector2f offset);

	/// @return Returns the client's position.
	sf::Vector2f GetPosition() const;
	/// Set the client's position.
	void SetPosition(sf::Vector2f pos);

	/// @return Returns the client's size.
	sf::Vector2f GetSize() const;
	/// Set the client's size.
	void SetSize(sf::Vector2f size);

	/// @return Returns the client's rotation.
	sf::Angle GetRotation() const;
	/// Set the client's rotation.
	void SetRotation(sf::Angle angle);

	/// @return ThReturns thee client's colour.
	sf::Color GetColour() const;
	/// Set the client's colour.
	void SetColour(sf::Color colour);

	/// @return Returns the client's connection status.
	bool IsConnected() const;

	/// @return Returns the client's hosting status.
	bool CanHost() const;
	/// Set if the client can host or not.
	void SetHosting(bool b);

	/// @return Returns the client's socket.
	sf::TcpSocket& GetSocket() const;

	/// @return Returns if the client is viable or not.
	bool GetVisible() const;
	/// Set if the client render on screen or not.
	void SetVisible(bool b);

private:
	/// Monitor the client's connection status to the server.
	void StartConnectionMonitor();

	/// Listen for updates from the server.
	void Listen(std::atomic<std::uint16_t>& otherClients);

private:
	// --- Client Objects ---
	sf::RectangleShape m_Visuals{{40.f, 40.0f}};
	sf::Texture m_Txt{};
	bool m_IsVisible{false};
	std::thread m_ConnectionThread{};
	std::thread m_ListenThread{};

	// --- Networking Objects ---
	std::unique_ptr<sf::TcpSocket> m_Socket{};
	std::atomic<bool> m_IsConnected{false};
	bool m_CanHost{true};


	std::mutex m_StateMutex;
	sf::Vector2f m_PendingPos;
	std::uint32_t m_PendingColour;
	bool m_HasPendingState{false};
};
}
