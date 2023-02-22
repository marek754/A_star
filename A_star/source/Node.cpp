#include "../headers/Node.hpp"
#include "../headers/Grid.hpp"

std::map<NodeState, sf::Color> Node::stateColors{
	{NodeState::open, sf::Color(100, 100, 100, 255)},
	{NodeState::start, sf::Color::Green},
	{NodeState::target, sf::Color::Red},
	{NodeState::obstacle, sf::Color::Black},
	{NodeState::path, sf::Color::Magenta}
};

Node::Node() : sf::RectangleShape({SQUARE_SIZE, SQUARE_SIZE}) {
	this->setState(NodeState::open);
}

inline void Node::setState(NodeState state) {
    _state = state;
    this->setFillColor(stateColors[state]);
}

NodeState Node::getState() {
    return _state;
}