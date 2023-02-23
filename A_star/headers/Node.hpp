#pragma once

#include "SFML/Graphics.hpp"
#include <map>

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

    void setNodePostitionInGrid(int x, int y);
    int x;
    int y;

    bool obstacle = false;
    bool visited = false;
    float globalGoal;
    float localGoal;

    std::vector<Node*> neighbours;
    Node* parent;
};
