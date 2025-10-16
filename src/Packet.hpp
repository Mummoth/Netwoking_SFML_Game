#pragma once
#include <cstdint>

namespace Game
{
enum class PacketType : std::uint8_t
{
	PLAYER_CONNECT, // Cli -> Ser: Client data when connecting to the server.
	PLAYER_LIST, // Ser -> Cli: Number of clients and data related to updates.
	PLAYER_UPDATE, // Ser -> Cli: Data related to updates.
	PLAYER_MESSAGE, // Ser -> Cli: Messages sent from server to client.
};
}
