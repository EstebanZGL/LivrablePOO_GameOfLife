#ifndef CELL_H
#define CELL_H

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
    Cell();
    void setAlive(bool alive);
    void clearCell();
    bool getAlive() const;
    bool getAliveColor() const;
    void toggleObstacle();
    void toggleObstacle(int stateInput);
    bool isObstacle() const;
    bool isObstacleAlive() const;
    bool canBeModified() const;
};

#endif
