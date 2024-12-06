#ifndef CELL_HPP
#define CELL_HPP

enum class CellState {
    DEAD,
    ALIVE,
    OBSTACLE_ALIVE,
    OBSTACLE_DEAD
};

class Cell {
private:
    CellState state;

public:
    Cell() : state(CellState::DEAD) {}

    void setAlive(bool alive) {
        if (state == CellState::OBSTACLE_DEAD || state == CellState::OBSTACLE_ALIVE) {
            return;
        }
        state = alive ? CellState::ALIVE : CellState::DEAD;
    }

    void clearCell() {
        state = CellState::DEAD;
    }

    bool getAlive() const {
        return state == CellState::ALIVE || state == CellState::OBSTACLE_ALIVE;
    }

    bool getAliveColor() const {
        return state == CellState::ALIVE;
    }

    void toggleObstacle() {
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

    void toggleObstacle(int stateInput) {
        if (stateInput == 2) {
            state = CellState::OBSTACLE_ALIVE; // Obstacle vivant
        }
        else {
            state = CellState::OBSTACLE_DEAD; // Obstacle mort
        }
    }

    bool isObstacle() const {
        return state == CellState::OBSTACLE_ALIVE || state == CellState::OBSTACLE_DEAD;
    }

    bool isObstacleAlive() const {
        return state == CellState::OBSTACLE_ALIVE;
    }

    bool canBeModified() const { // Méthode pour vérifier si la cellule peut être modifiée
        return state == CellState::DEAD || state == CellState::ALIVE;  // Seules les cellules mortes peuvent être modifiées
    }
};

#endif
