#ifndef CELLULE_H
#define CELLULE_H

enum class EtatCellule {
    MORT,
    VIVANT,
    OBSTACLE_VIVANT,
    OBSTACLE_MORT
};

class Cellule {
private:
    EtatCellule etat;

public:
    Cellule();
    void defEtatCel(bool vivant);
    void ReinitialiserCellules();
    bool estVivant() const;
    bool estVivantCouleur() const;
    void defObstacle();
    void defObstacle(int appui);
    bool estObstacle() const;
    bool estObstacleVivant() const;
    bool estModifiable() const;
};

#endif 
