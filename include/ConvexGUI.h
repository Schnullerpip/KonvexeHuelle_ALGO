#pragma once
#include "I_View.hpp"
#include "sfml\Graphics.hpp"

class ConvexGUI : public I_View
{
private:
	//member
	sf::RenderWindow window;
	bool should_close;
	sf::CircleShape pointRepresentation;
	//methods
	void handleEvents();
public:
	ConvexGUI(std::vector<Point*>* points);
	~ConvexGUI();

	void update(void* credentials)override;
	bool shouldClose()override;
};

