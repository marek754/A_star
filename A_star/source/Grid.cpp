#include "../headers/Grid.hpp"

Grid::Grid() {
    for (int x = 0; x < GRID_SIZE_X; x++) {
        for (int y = 0; y < GRID_SIZE_Y; y++) {
            (*nodes)[x][y].setNodePostitionInGrid(x, y);
            (*nodes)[x][y].setPosition(static_cast<float>(x * (SQUARE_SIZE + GRID_GAP)), static_cast<float>(y * (SQUARE_SIZE + GRID_GAP)));
            (*nodes)[x][y].setFillColor(sf::Color(100, 100, 100, 255));
        }
    }
    connectNodes();
};

void Grid::forEachNode(std::function<void(Node&, int&, int&)> func) {
    for (int x = 0; x < GRID_SIZE_X; x++) {
        for (int y = 0; y < GRID_SIZE_Y; y++) {
            func((*nodes)[x][y], x, y);
        }
    }
}

void Grid::forEachNode(std::function<void(Node&)> func) {
    for (int x = 0; x < GRID_SIZE_X; x++) {
        for (int y = 0; y < GRID_SIZE_Y; y++) {
            func((*nodes)[x][y]);
        }
    }
}

void Grid::connectNodes() {
    for (int x = 0; x < GRID_SIZE_X; x++) {
        for (int y = 0; y < GRID_SIZE_Y; y++) {
            //connect nodes vertically and horizontally
            if (y > 0) {
                (*nodes)[x][y].neighbours.emplace_back(&(*nodes)[x][y - 1]);
            }
            if (y < GRID_SIZE_Y - 1) {
                (*nodes)[x][y].neighbours.emplace_back(&(*nodes)[x][y + 1]);
            }
            if (x > 0) {
                (*nodes)[x][y].neighbours.emplace_back(&(*nodes)[x - 1][y]);
            }
            if (x < GRID_SIZE_X - 1) {
                (*nodes)[x][y].neighbours.emplace_back(&(*nodes)[x + 1][y]);
            }
            //connect nodes diagonally
            if (y > 0 && x > 0) {
                (*nodes)[x][y].neighbours.emplace_back(&(*nodes)[x - 1][y - 1]);
            }
            if (y < GRID_SIZE_Y - 1 && x>0) {
                (*nodes)[x][y].neighbours.emplace_back(&(*nodes)[x - 1][y + 1]);
            }
            if (y > 0 && x < GRID_SIZE_X - 1) {
                (*nodes)[x][y].neighbours.emplace_back(&(*nodes)[x + 1][y - 1]);
            }
            if (y < GRID_SIZE_Y - 1 && x < GRID_SIZE_X - 1) {
                (*nodes)[x][y].neighbours.emplace_back(&(*nodes)[x + 1][y + 1]);
            }
        }
    }
}
