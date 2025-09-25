#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Utils.hpp"

namespace Game
{
class Application
{
public:
	Application(sf::RenderWindow& window);

	void Input(float dt);
	void Update(float dt);
	void Render() const;

private:
	sf::RenderWindow* m_Window;

	sf::Texture m_Txt{};
	std::unique_ptr<sf::Sprite> m_Sprite{};
	sf::CircleShape m_Shape1{};
};
}
