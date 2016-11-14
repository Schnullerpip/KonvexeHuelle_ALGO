#include "ConvexGUI.h"
#include <iostream>

void ConvexGUI::update(void* credentials)
{
	std::cout << "[ConvexGUI]::update -> updating" << std::endl;
	//handle all the events
	handleEvents();

	//clear all the window
	window.clear(sf::Color::White);

	//draw all the points
	for (auto p : *points) {
		pointRepresentation.setPosition(p->getX(), p->getY());
		window.draw(pointRepresentation);
	}

	//swap buffers and stuff -> print the actual screen
	window.display();
}

ConvexGUI::ConvexGUI(std::vector<Point*>* points)
{
	//initialize variables
	this->points = points;

	//initialize defaults
	should_close = false;
	window.create(sf::VideoMode(1000, 600), "Konvexe Hülle - Divide and Conquer");
	pointRepresentation.setRadius(5);
	pointRepresentation.setFillColor(sf::Color::White);
	pointRepresentation.setOutlineThickness(3);
	pointRepresentation.setOutlineColor(sf::Color::Black);

	//sfml customization
	window.setVerticalSyncEnabled(true);
}

void ConvexGUI::handleEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			std::cout << "[ConvexGUI]::handleEvents -> catched closing event" << std::endl;
			window.close();
			should_close = true;
		}
	}
}


bool ConvexGUI::shouldClose()
{
	return should_close;
}



ConvexGUI::~ConvexGUI()
{
}
