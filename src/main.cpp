#include <thread>
#include <SFML/Graphics.hpp>
#include "Application.h"


//void HandleEvents(sf::RenderWindow& window, Game::Application& app, float dt);
float CalculateDeltaTime();

int main()
{
	auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ 800, 600 }), "My Window");
	auto app = std::make_unique<Game::Application>(*window);


	while (window->isOpen())
	{
		const float deltaTime = CalculateDeltaTime();

		//HandleEvents(*window, *app, deltaTime);

		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window->close();

			// Handle keyboard input.
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
					window->close();

				app->Input(*keyPressed, deltaTime);
			}
		}

		// Handle game logic.
		app->Update(deltaTime);
		app->Render();
	}

	return 0;
}

/*void HandleEvents(sf::RenderWindow& window, Game::Application& app, float dt)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();

		// Handle keyboard input.
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			switch (keyPressed->scancode)
			{
			case sf::Keyboard::Scan::Escape:
				window.close();
			}

			app.Input(*keyPressed, dt);

		}
	}
}*/

float CalculateDeltaTime()
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<float> delta = currentTime - lastTime;

	lastTime = currentTime;

	return delta.count();
}
