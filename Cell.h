#ifndef CELL_HPP
#define CELL_HPP

enum class EtatCellule {
    MORT,
    VIVANT,
    OBSTACLE_VIVANT,
    OBSTACLE_MORT
};

class Cell {
private:
    EtatCellule etat;

public:
    Cell() : etat(EtatCellule::MORT) {}

    void defEtatCel(bool vivant) {
        if (etat == EtatCellule::OBSTACLE_MORT || etat == EtatCellule::OBSTACLE_VIVANT) {
        }
        etat = vivant ? EtatCellule::VIVANT : EtatCellule::MORT;
    }

    void ReinitialiserCellules() {
        etat = EtatCellule::MORT;
    }

    bool estVivant() const {
        return etat == EtatCellule::VIVANT || etat == EtatCellule::OBSTACLE_VIVANT;
    }

    bool estVivantCouleur() const {
        return etat == EtatCellule::VIVANT;
    }

    void defObstacle() {
        if (etat == EtatCellule::OBSTACLE_VIVANT) {
            etat = EtatCellule::MORT; // Si c'est un obstacle vivant, on le rend mort
        }
        else if (etat == EtatCellule::OBSTACLE_MORT) {
            etat = EtatCellule::OBSTACLE_VIVANT; // Si c'est un obstacle mort, on le rend vivant
        }
        else if (etat == EtatCellule::MORT) {
            etat = EtatCellule::OBSTACLE_MORT; // On marque la cellule comme obstacle mort
        }
    }

    void defObstacle(int appui) {
        if (appui == 2) {
            etat = EtatCellule::OBSTACLE_VIVANT; // Obstacle vivant
        }
        else {
            etat = EtatCellule::OBSTACLE_MORT; // Obstacle mort
        }
    }

    bool estObstacle() const {
        return etat == EtatCellule::OBSTACLE_VIVANT || etat == EtatCellule::OBSTACLE_MORT;
    }

    bool estObstacleVivant() const {
        return etat == EtatCellule::OBSTACLE_VIVANT;
    }

    bool estModifiable() const { // Méthode pour vérifier si la cellule peut être modifiée
        return etat == EtatCellule::MORT || etat == EtatCellule::VIVANT;  // Seules les cellules mortes et vivantes peuvent être modifiées
    }
};

#endif
