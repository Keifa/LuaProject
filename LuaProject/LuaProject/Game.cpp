#include "Game.h"

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(window::WIDTH, window::HEIGHT), window::NAME);
	events = new sf::Event();
}

Game::~Game()
{
	delete window;
	delete events;
}

void Game::start()
{
	lh.start("test.lua");

	while (window->isOpen())
	{
		while (window->pollEvent(*events))
		{
			switch (events->type)
			{
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::KeyPressed:
				lh.load("test2.lua");
				break;
			}
		}

		window->clear();
		
		window->display();
	}
}