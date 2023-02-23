#include "../headers/PathFinder.hpp"

PathFinder::PathFinder() {
	window.create(sf::VideoMode(GRID_SIZE_X * (SQUARE_SIZE + GRID_GAP), GRID_SIZE_Y * (SQUARE_SIZE + GRID_GAP)), "A*");
	window.setFramerateLimit(144);;
}

enum class SelectionMode {
	startSelection,
	targetSelection,
	obstacleSelection
};

void PathFinder::handleEvents() {
	sf::Event event;

	auto handleLeftClickEvent = [&](Node &node) {
		sf::Vector2i mousePosition(event.mouseButton.x, event.mouseButton.y);
		sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
		if (node.getGlobalBounds().contains(worldPosition)) {
			switch (node.getState()) {
			case NodeState::start:
				node.setState(NodeState::open);
				startIsSet = false;
				break;
			case NodeState::target:
				node.setState(NodeState::open);
				targetIsSet = false;
				break;
			case NodeState::open:
				if (!startIsSet) {
					node.setState(NodeState::start);
					grid.nodeStart = &node;
					startIsSet = true;
				}
				else if (!targetIsSet) {
					node.setState(NodeState::target);
					grid.nodeTarget = &node;
					targetIsSet = true;
				}
				else {
					selectingObstacles = true;
				}
				break;
			case NodeState::path:
				node.setState(NodeState::obstacle);
				node.obstacle = true;
			}
		}
	};

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (!selectingObstacles) {
					grid.forEachNode(handleLeftClickEvent);
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right) {
				deselectingObstacles = true;
				selectingObstacles = false;
			}
		}
		if (event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (selectingObstacles) {
					selectingObstacles = false;
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right) {
				if (deselectingObstacles) {
					deselectingObstacles = false;
				}
			}
		}
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Enter) {
				selectingObstacles = false;
				deselectingObstacles = false;
				if (startIsSet && targetIsSet) {
					findPath();
				}
			}
			else if (event.key.code == sf::Keyboard::Space) {
				grid.forEachNode([&](Node& node) {
					node.setState(NodeState::open);
					});
				grid.nodeStart = nullptr;
				grid.nodeTarget = nullptr;
				startIsSet = false;
				targetIsSet = false;
			}
		}
	}
}

void PathFinder::render() {
	window.clear(sf::Color(40, 40, 40, 255));
	grid.forEachNode([&](Node& node) {
		window.draw(node);
		});
	window.display();
}

void PathFinder::selectOrDeselectObstacles() {
	grid.forEachNode([&](Node &node) {
		sf::Vector2f worldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		if (node.getGlobalBounds().contains(worldPosition)) {
			if (selectingObstacles && (node.getState() == NodeState::open || node.getState() == NodeState::path)) {
				node.setState(NodeState::obstacle);
				node.obstacle = true;
			}
			else if (deselectingObstacles && node.getState() == NodeState::obstacle) {
				node.setState(NodeState::open);
				node.obstacle = false;
			}
		}
	});
}

void PathFinder::mainLoop() {
	while(window.isOpen()) {
		handleEvents();
		if (selectingObstacles || deselectingObstacles) {
			selectOrDeselectObstacles();
		}
		render();
	}
}

class NodeComparator {
public:
	bool operator() (const Node* a, const Node* b) const {
		return a->globalGoal > b->globalGoal;
	}
};

bool PathFinder::findPath() {
	Node* const start = grid.nodeStart;
	Node* const target = grid.nodeTarget;

	std::unordered_map<Node*, float> distances;
	std::priority_queue<Node*, std::vector<Node*>, NodeComparator> queue;

	grid.forEachNode([&](Node& node) {
		node.visited = false;
		node.globalGoal = INFINITY;
		node.localGoal = INFINITY;
		node.parent = nullptr;
		if (node.getState() == NodeState::path) {
			node.setState(NodeState::open);
		}
		});

	start->localGoal = 0.f;
	start->globalGoal = heuristic(start, target);
	queue.push(start);

	float possiblyLowerGoal;
	Node* current = nullptr;
	while (!queue.empty()) {
		current = queue.top();
		queue.pop();
		current->visited = true;
		if (current == target) {
			break;
		}
		for (Node *neighbour: current->neighbours) {
			if (neighbour->visited || neighbour->obstacle || checkIfDiagonal(current, neighbour)) {
				continue;
			}
			possiblyLowerGoal = current->localGoal + heuristic(current, neighbour);
			if (possiblyLowerGoal < neighbour->localGoal) {
				neighbour->parent = current;
				neighbour->localGoal = possiblyLowerGoal;
				neighbour->globalGoal = neighbour->localGoal + heuristic(neighbour, target);
				distances[neighbour] = neighbour->globalGoal;
				queue.push(neighbour);
			}
		}
	}
	drawPath();
	return true;
}

void PathFinder::drawPath() {
	if (grid.nodeTarget != nullptr) {
		Node* node = grid.nodeTarget->parent;
		while (node->parent != nullptr) {
			node->setState(NodeState::path);
			node = node->parent;
		}
	}
}

inline float PathFinder::square(float a) {
	return a * a;
}

float PathFinder::heuristic(Node* a, Node* b) {
	return std::sqrtf(PathFinder::square(a->getPosition().x - b->getPosition().x) + square(a->getPosition().y - b->getPosition().y));
}

bool isInGrid(int x, int y) {
	return x >= 0 && x < GRID_SIZE_X &&
		y >= 0 && y < GRID_SIZE_Y;
}

bool PathFinder::checkIfDiagonal(const Node* current, const Node* next) {
	const int dx = next->x - current->x;
	const int dy = next->y - current->y;

	if (dx * dy != 0 && isInGrid(current->x + dx, current->y) && isInGrid(current->x, current->y + dy)) {
		const Node *nodeA = &grid.nodes->at(current->x + dx).at(current->y);
		const Node *nodeB = &grid.nodes->at(current->x).at(current->y + dy);
		return nodeA->obstacle && nodeB->obstacle;
	}
	return false;
}

/*
bool PathFinder::checkIfDiagonal(const Node* current, const Node* next) {
	if (current->x < 0 || current->x >= GRID_SIZE_X ||
		current->y < 0 || current->y >= GRID_SIZE_Y ||
		next->x < 0 || next->x >= GRID_SIZE_X ||
		next->y < 0 || next->y >= GRID_SIZE_Y) {
		return true;
	}
	// top left
	if(current->x == next->x - 1 && current->y == next->y - 1 &&
		isInGrid(current->x - 1, current->y) &&
		grid.nodes->at(current->x - 1).at(current->y).obstacle &&
		isInGrid(current->x, current->y - 1) &&
		grid.nodes->at(current->x).at(current->y - 1).obstacle) {
			return true;
	}
	// top right
	if (current->x == next->x + 1 && current->y == next->y - 1 &&
		isInGrid(current->x, current->y + 1) &&
		grid.nodes->at(current->x).at(current->y + 1).obstacle &&
		isInGrid(current->x + 1, current->y) &&
		grid.nodes->at(current->x + 1).at(current->y).obstacle) {
		return true;
	}
	// bottom left
	if (current->x == next->x - 1 && current->y == next->y + 1 &&
		isInGrid(current->x - 1, current->y) &&
		grid.nodes->at(current->x - 1).at(current->y).obstacle &&
		isInGrid(current->x, current->y + 1) &&
		grid.nodes->at(current->x).at(current->y + 1).obstacle) {
		return true;
	}
	// bottom right
	if (current->x == next->x + 1 && current->y == next->y + 1 &&
		isInGrid(current->x + 1, current->y) &&
		grid.nodes->at(current->x + 1).at(current->y).obstacle &&
		isInGrid(current->x, current->y + 1) &&
		grid.nodes->at(current->x).at(current->y + 1).obstacle) {
		return true;
	}
	return false;
}*/