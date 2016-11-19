#include "ConvexGUI.h"
#include <iostream>
#include <sstream>

/**
update method for the view, triggers a refresh/repaint/swapbuffers whatever
@param hulls optionally it rerceives a vector of ConvexHull pointers. It draws all the convex hulls (if given)
and all the points that it got at construction time (cunstructor paramter points)
for simplicity the update method also handles events
*/
void ConvexGUI::update(std::vector<ConvexHull*>* hulls)
{
	//std::cout << "[ConvexGUI]::update -> updating" << std::endl;
	//handle all the events
	handleEvents();

	//clear all the window
	window.clear(sf::Color::White);

	//draw a coordinate system - so points can be associated
	int offset = 15;
	sf::Vertex y_axis[2] = {sf::Vertex(sf::Vector2f(offset*2, offset), sf::Color::Black), sf::Vertex(sf::Vector2f(offset*2, SCREEN_HEIGHT - offset), sf::Color::Black)};
	sf::Vertex x_axis[2] = {sf::Vertex(sf::Vector2f(offset*2, SCREEN_HEIGHT - offset), sf::Color::Black), sf::Vertex(sf::Vector2f(SCREEN_WIDTH -offset, SCREEN_HEIGHT -offset), sf::Color::Black)};
	window.draw(y_axis, 2, sf::Lines);
	window.draw(x_axis, 2, sf::Lines);

	int x_step = 10;
	int y_step = x_step;
	for (int i = 0; i < SCREEN_WIDTH; i += x_step) {;
		sf::Text txt;
		std::ostringstream ss;
		txt.setFont(font);
		txt.setCharacterSize(12);
		txt.setPosition(offset + i*x_step, SCREEN_HEIGHT - offset + 2);
		ss << x_step * i;
		txt.setString(ss.str());
		txt.setFillColor(sf::Color::Black);
		window.draw(txt);
	}
	for (int i = SCREEN_HEIGHT; i > 0; i -= y_step) {
		sf::Text txt;
		std::ostringstream ss;
		txt.setFont(font);
		txt.setCharacterSize(12);
		txt.setPosition(2, y_step*i);
		ss << SCREEN_HEIGHT - y_step * i;
		txt.setString(ss.str());
		txt.setFillColor(sf::Color::Black);
		window.draw(txt);
	}

	//draw all the points
	pointRepresentation.setOrigin(sf::Vector2f(pointRepresentation.getRadius(), pointRepresentation.getRadius()));
	for (auto p : *points) {
		pointRepresentation.setPosition(p->getX(), SCREEN_HEIGHT - p->getY());
		window.draw(pointRepresentation);
	}

	//if convex hulls are given
	if (hulls) {
		for (auto ch : *hulls) { // for all the convex hulls
			sf::Vertex* vertices = new sf::Vertex[ch->size() + ((ch->size() <= 2) ? 0 : 1)]();
			for (int i = 0; i < ch->size(); ++i) { // add each of their points to a vertex array and draw it
				Point* p = ch->at(i);
				vertices[i] = sf::Vertex(sf::Vector2f(p->getX(), SCREEN_HEIGHT - p->getY()), sf::Color::Black);
			}
			if (ch->size() > 2) { //if the convex hull consists of more than two vertex, than link the first and the last vertices
				vertices[ch->size()] = sf::Vertex(sf::Vector2f(ch->at(0)->getX(), SCREEN_HEIGHT - ch->at(0)->getY()), sf::Color::Black);
			}
			window.draw(vertices, ch->size()+ ((ch->size() <= 2) ? 0 : 1), sf::LineStrip);
			delete vertices;
		}
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
	window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Konvexe Hülle - Divide and Conquer");
	pointRepresentation.setRadius(5);
	pointRepresentation.setFillColor(sf::Color::White);
	pointRepresentation.setOutlineThickness(3);
	pointRepresentation.setOutlineColor(sf::Color::Black);

	if (!font.loadFromFile("fonts/Roboto-Bold.ttf")) {
		std::cout << "[ConvexGUI]::ConvexGUI -> couldnt load Roboto-Bold.ttf file!" << std::endl;
	}

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

int ConvexGUI::getInput() {
	//synchronous wait for user input
	bool wait = true;
	sf::Event event;
	while (wait) {
		while (window.pollEvent(event)) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
				wait = false;
			}
			if (event.type == sf::Event::Closed) {
				should_close = true;
				window.close();
				wait = false;
			}
		}
	}
	return event.key.code;
}


bool ConvexGUI::shouldClose()
{
	return should_close;
}



ConvexGUI::~ConvexGUI()
{
}
