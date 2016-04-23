#include "Game.h"

Game::Game()
{
	window::renderWindow = new sf::RenderWindow(sf::VideoMode(window::WIDTH, window::HEIGHT), window::NAME);
	events = new sf::Event();
	test = new sf::RectangleShape(sf::Vector2f(100, 100));
	test->setFillColor(sf::Color(0, 255, 0));
	test->setPosition(sf::Vector2f(window::WIDTH * 0.5, window::HEIGHT * 0.5));
	check = false;
}

Game::~Game()
{
	delete window::renderWindow;
	delete events;
	delete test;
}

void Game::start()
{
	lh.start("test.lua");

	while (window::renderWindow->isOpen())
	{
		handleEvents();
		window::renderWindow->clear();
		if(check)
			window::renderWindow->draw(*test);
		window::renderWindow->display();
	}
}

void Game::handleEvents()
{
	while (window::renderWindow->pollEvent(*events))
	{
		switch (events->type)
		{
		case sf::Event::Closed:
			window::renderWindow->close();
			break;
		case sf::Event::KeyPressed:
			handleKeyboardEvents();
			check = !check;
			break;
		}
	}
}

void Game::handleKeyboardEvents()
{
	switch (events->key.code)
	{
	case sf::Keyboard::Space:
		lh.load("test2.lua");
		window::renderWindow->draw(*test);
		break;
	case sf::Keyboard::Escape:
		window::renderWindow->close();
		break;
	}
}
