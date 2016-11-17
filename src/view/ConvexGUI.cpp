#include "ConvexGUI.h"
#include <iostream>

/**
update method for the view, triggers a refresh/repaint/swapbuffers whatever
@param hulls optionally it rerceives a vector of ConvexHull pointers. It draws all the convex hulls (if given)
and all the points that it got at construction time (cunstructor paramter points)
for simplicity the update method also handles events
*/
void ConvexGUI::update(std::vector<ConvexHull*>* hulls)
{
	std::cout << "[ConvexGUI]::update -> updating" << std::endl;
	//handle all the events
	handleEvents();

	//clear all the window
	window.clear(sf::Color::White);

	//if convex hulls are given
	if (hulls) {
		for (auto ch : *hulls) { // for all the convex hulls
			sf::Vertex* vertices = new sf::Vertex[ch->size()+1]();
			for (int i = 0; i < ch->size(); ++i) { // add each of their points to a vertex array and draw it
				Point* p = ch->at(i);
				vertices[i] = sf::Vertex(sf::Vector2f(p->getX(), p->getY()));
			}
			if (ch->size() > 2) { //if the convex hull consists of more than two vertex, than link the first and the last vertices
				vertices[ch->size()] = sf::Vertex(sf::Vector2f(ch->at(0)->getX(), ch->at(0)->getY()));
			}
			window.draw(vertices, ch->size()+1, sf::Lines);
			delete vertices;
		}
	}


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
