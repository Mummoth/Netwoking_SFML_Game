#include <thread>
#include <SFML/Graphics.hpp>
#include "Application.h"

float CalculateDeltaTime();

int main()
{
	auto window = std::make_unique<sf::RenderWindow>();
	window->create(sf::VideoMode({800, 600}), "My Window");
	auto app = std::make_unique<Game::Application>(*window);


	while (window->isOpen())
	{
		const float deltaTime = CalculateDeltaTime();

		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				app->GetServer().Stop();
				window->close();
			}

			// Handle keyboard input.
			else if (const auto* keyPressed = event->getIf<
				sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
				{
					app->GetServer().Stop();
					window->close();
				}

				app->Input(*keyPressed, deltaTime);
			}
		}

		// Handle game logic.
		app->Update(deltaTime);
		app->Render();
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
