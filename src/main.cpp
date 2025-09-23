#include <iostream>
#include <SFML/Window.hpp>

int main()
{
	sf::Window window(sf::VideoMode({ 800,600 }), "My Window");

	while (window.isOpen())
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
			}
		}
	}

	std::cout << "Hello World\n";

	return 0;
}