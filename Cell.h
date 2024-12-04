#ifndef CELL_HPP // Protection contre les inclusions multiples.
#define CELL_HPP // Définition de la macro CELL_HPP pour éviter les inclusions multiples.

class Cell {
private:
    bool alive; // Attribut pour stocker l'état de la cellule (true = vivante, false = morte).

public:
    Cell() : alive(false) {} // Constructeur par défaut, initialise alive à false (cellule morte).

    bool getAlive() const { return alive; } // Retourne l'état de la cellule (vivante ou morte).
    void setAlive(bool state) { alive = state; } // Modifie l'état de la cellule avec le paramètre donné.
};

#endif // Fin de la protection contre les inclusions multiples.
