#pragma once

#include "Grid.hpp"
#include <string>
#include <queue>

class PathFinder {
public:
	PathFinder();
	void mainLoop();
private:
	sf::RenderWindow window;
	Grid grid;

	std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();

	bool startIsSet = false;
	bool targetIsSet = false;
	bool selectingObstacles = false;
	bool deselectingObstacles = false;

	void handleEvents();
	void selectOrDeselectObstacles();
	void render();
	bool findPath();
	void drawPath();

	inline float square(float a);
	float heuristic(Node* a, Node* b);
	bool checkIfDiagonal(const Node* current, const Node* next);
};