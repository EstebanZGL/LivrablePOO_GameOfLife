#ifndef CELL_HPP
#define CELL_HPP

class Cell {
private:
    bool alive;

public:
    Cell() : alive(false) {}

    bool getAlive() const { return alive; }
    void setAlive(bool state) { alive = state; }
};

#endif
