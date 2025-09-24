#include <functional>
#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>

#define TXT_PATH L"assets/textures/"


void RenderingThread(sf::RenderWindow& window, const std::vector<sf::Drawable*>& drawables);

int main()
{
	auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ 800, 600 }), "My Window");
	// Deactivate context in main thread.
	static_cast<void>(window->setActive(false)); //< See casting reason in RenderingThread().

	sf::Texture txt(TXT_PATH "mam.png");
	sf::Sprite sprite(txt);

	std::vector<sf::Drawable*> drawables;
	drawables.reserve(20);
	drawables.push_back(&sprite);

	std::thread renderThread(&RenderingThread, std::ref(*window), std::ref(drawables));

	sprite.scale({0.5f,0.5f});


	while (window->isOpen())
	{
		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window->close();

			// Handle keyboard input.
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				switch (keyPressed->scancode)
				{
				case sf::Keyboard::Scan::Escape:
					window->close();
				}
			}
		}
	}

	renderThread.join();
	std::cout << "Hello World\n";

	return 0;
}

void RenderingThread(sf::RenderWindow& window, const std::vector<sf::Drawable*>& drawables)
{
	// Activate the window's context.
	static_cast<void>(window.setActive(true));//< Cast to VOID to ignore discard warning.

	while (window.isOpen())
	{
		window.clear();

		// Draw here.
		for (const auto obj : drawables)
			window.draw(*obj);

		window.display();
	}
}