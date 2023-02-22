#pragma once

#include "SFML/Graphics.hpp"

enum class NodeState {
    open,
    start,
    target,
    obstacle,
    path
};


class Node : public sf::RectangleShape {
private:
    NodeState _state;
    static std::map<NodeState, sf::Color> stateColors;
public:
    Node();
    void setState(NodeState state);
    NodeState getState();

    bool obstacle = false;
    bool visited = false;
    float globalGoal;
    float localGoal;

    std::vector<Node*> neighbours;
    Node* parent;
};
