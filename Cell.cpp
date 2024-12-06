#include "Cell.h"

Cell::Cell() : state(CellState::DEAD) {}

void Cell::setAlive(bool alive) {
    if (state == CellState::OBSTACLE_DEAD || state == CellState::OBSTACLE_ALIVE) {
        return;
    }
    state = alive ? CellState::ALIVE : CellState::DEAD;
}

void Cell::clearCell() {
    state = CellState::DEAD;
}

bool Cell::getAlive() const {
    return state == CellState::ALIVE || state == CellState::OBSTACLE_ALIVE;
}

bool Cell::getAliveColor() const {
    return state == CellState::ALIVE;
}

void Cell::toggleObstacle() {
    if (state == CellState::OBSTACLE_ALIVE) {
        state = CellState::DEAD; // Si c'est un obstacle vivant, on le rend mort
    }
    else if (state == CellState::OBSTACLE_DEAD) {
        state = CellState::OBSTACLE_ALIVE; // Si c'est un obstacle mort, on le rend vivant
    }
    else if (state == CellState::DEAD) {
        state = CellState::OBSTACLE_DEAD; // On marque la cellule comme obstacle mort
    }
}

void Cell::toggleObstacle(int stateInput) {
    if (stateInput == 2) {
        state = CellState::OBSTACLE_ALIVE; // Obstacle vivant
    }
    else {
        state = CellState::OBSTACLE_DEAD; // Obstacle mort
    }
}

bool Cell::isObstacle() const {
    return state == CellState::OBSTACLE_ALIVE || state == CellState::OBSTACLE_DEAD;
}

bool Cell::isObstacleAlive() const {
    return state == CellState::OBSTACLE_ALIVE;
}

bool Cell::canBeModified() const {
    return state == CellState::DEAD || state == CellState::ALIVE;  // Seules les cellules mortes peuvent être modifiées
}
