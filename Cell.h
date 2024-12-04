#ifndef CELL_HPP // Protection contre les inclusions multiples.
#define CELL_HPP // D�finition de la macro CELL_HPP pour �viter les inclusions multiples.

class Cell {
private:
    bool alive; // Attribut pour stocker l'�tat de la cellule (true = vivante, false = morte).

public:
    Cell() : alive(false) {} // Constructeur par d�faut, initialise alive � false (cellule morte).

    bool getAlive() const { return alive; } // Retourne l'�tat de la cellule (vivante ou morte).
    void setAlive(bool state) { alive = state; } // Modifie l'�tat de la cellule avec le param�tre donn�.
};

#endif // Fin de la protection contre les inclusions multiples.
