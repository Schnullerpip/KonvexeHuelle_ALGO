#pragma once
#include "I_View.hpp"
#include "sfml\Graphics.hpp"

class ConvexGUI : public I_View
{
private:
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 600;
	//member
	sf::RenderWindow window;
	bool should_close;
	sf::CircleShape pointRepresentation;
	sf::Font font;
	//methods
	void handleEvents();
public:
	ConvexGUI(std::vector<Point*>* points);
	~ConvexGUI();

	void update(std::vector<ConvexHull*>* ch)override;
	bool shouldClose()override;
};

