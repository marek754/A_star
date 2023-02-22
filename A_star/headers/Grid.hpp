#pragma once

#include "SFML/Graphics.hpp"
#include <array>
#include <memory>
#include <functional>
#include "Node.hpp"

constexpr int SQUARE_SIZE = 10;
constexpr int GRID_SIZE_X = 128;
constexpr int GRID_SIZE_Y = 64;
constexpr int GRID_SIZE = GRID_SIZE_X * GRID_SIZE_Y;
constexpr int GRID_GAP = 1;

class Grid {
public:
    Grid();

    std::unique_ptr<std::array<std::array<Node, GRID_SIZE_Y>, GRID_SIZE_X>> nodes = std::make_unique<std::array<std::array<Node, GRID_SIZE_Y>, GRID_SIZE_X>>();

    void forEachNode(std::function<void(Node&, int&, int&)> func);
    void forEachNode(std::function<void(Node&)> func);
    void connectNodes();

    Node *nodeStart = nullptr;
    Node *nodeTarget = nullptr;
};