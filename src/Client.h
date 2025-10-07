#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"

namespace Game
{
class Client
{
public:
	Client();

	/// Connect the client to the desired server.
	/// @param serverIp The IP of the desired server.
	/// @param port The port of the desired server on the network.
	void Connect(sf::IpAddress serverIp, unsigned short port);

	/// Render client.
	void Render(sf::RenderWindow& window) const;

	/// Move the client.
	void Move(sf::Vector2f offset);

	///@return Returns the client's position.
	sf::Vector2f GetPosition() const;
	/// Set the client's position.
	void SetPosition(sf::Vector2f pos);

	///@return Returns the client's size.
	sf::Vector2f GetSize() const;
	/// Set the client's size.
	void SetSize(sf::Vector2f size);

	///@return Returns the client's rotation.
	sf::Angle GetRotation() const;
	/// Set the client's rotation.
	void SetRotation(sf::Angle angle);

	///@return ThReturns thee client's colour.
	sf::Color GetColour() const;
	/// Set the client's colour.
	void SetColour(sf::Color colour);

	///@return Returns the client's connection status.
	bool IsConnected() const;

	///@return Returns the client's hosting status.
	bool CanHost() const;
	/// Set if the client can host or not.
	void SetHosting(bool b);

	///@return Returns the client's socket.
	sf::TcpSocket& GetSocket() const;

private:
	// --- Client Objects ---
	sf::RectangleShape m_Visuals{{40.f, 40.0f}};
	sf::Texture m_Txt{};

	// --- Networking Objects ---
	std::unique_ptr<sf::TcpSocket> m_Socket{};
	bool m_IsConnected{false};
	bool m_CanHost{true};
};
}
