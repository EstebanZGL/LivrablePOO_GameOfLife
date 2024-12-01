#ifndef CELL_HPP
#define CELL_HPP

class Cell {
private:
    bool isAlive;

public:
    Cell() : isAlive(false) {}

    void setAlive(bool state) { isAlive = state; }
    bool getAlive() const { return isAlive; }
};

#endif
