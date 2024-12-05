#ifndef CELL_HPP // Protection contre les inclusions multiples.
#define CELL_HPP // Définition de la macro CELL_HPP pour éviter les inclusions multiples.
#ifndef OBSTACLE_CELL_HPP
#define OBSTACLE_CELL_HPP // Définition de la macro OBSTACLE_CELL_HPP qui va hériter de la classe principale CELL

class Cell {
private:
    bool alive; // Attribut pour stocker l'état de la cellule (true = vivante, false = morte).

public:
    Cell() : alive(false) {} // Constructeur par défaut, initialise alive à false (cellule morte).

    bool getAlive() const { return alive; } // Retourne l'état de la cellule (vivante ou morte).
    virtual void setAlive(bool state) { alive = state; } // Modifie l'état de la cellule avec le paramètre donné.
    virtual ~Cell() {} // Ajout d'un destructeur virtuel pour les classes dérivées
};

//#include "Cell.h" --- ca sera utile si on veut créer la classe obstacle dans un fichier apart

class ObstacleCell : public Cell {
private:
    sf::Color color; // Couleur de l'obstacle (rouge ou vert).
public:
    ObstacleCell(bool aliveState, sf::Color obstacleColor)
        : Cell(), color(obstacleColor) {
        setAlive(aliveState); // Définit l'état initial (vivant ou mort).
    }

    void setAlive(bool state) override {
        // Ne permet pas de changer l'état d'une cellule obstacle.
    }

    sf::Color getColor() const {
        return color; // Retourne la couleur de l'obstacle.
    }
};

#endif // OBSTACLE_CELL_HPP
#endif // Fin de la protection contre les inclusions multiples.
