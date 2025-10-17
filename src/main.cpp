#include <SFML/Graphics.hpp>

float CalculateDeltaTime();

int main()
{
	const auto window = std::make_unique<sf::RenderWindow>();
	window->create(sf::VideoMode({800, 600}), "My Window");
	window->setFramerateLimit(60);


	while (window->isOpen())
	{
		const float deltaTime = CalculateDeltaTime();

		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window->close();
			}

			// Handle keyboard input.
			else if (const auto* keyPressed = event->getIf<
				sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
				{
					window->close();
				}
			}
		}
	}

	return 0;
}

/// Calculate the delta between the current and last frame.
float CalculateDeltaTime()
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<float> delta = currentTime - lastTime;

	lastTime = currentTime;

	return delta.count();
}
