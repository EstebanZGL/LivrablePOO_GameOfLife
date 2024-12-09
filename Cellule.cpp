#include "Cellule.h"

// Constructeur par défaut de la cellule, initialisant l'état à MORT
Cellule::Cellule() : etat(EtatCellule::MORT) {}

// Fonction pour définir l'état de la cellule (vivante ou morte)
void Cellule::defEtatCel(bool vivant) {
    // Ne pas modifier si la cellule est un obstacle
    if (etat == EtatCellule::OBSTACLE_MORT || etat == EtatCellule::OBSTACLE_VIVANT) {
        return;
    }
    // Définir l'état selon la valeur de vivant
    etat = vivant ? EtatCellule::VIVANT : EtatCellule::MORT;
}

// Fonction pour réinitialiser la cellule à l'état MORT
void Cellule::ReinitialiserCellules() {
    etat = EtatCellule::MORT; // Réinitialiser l'état
}

// Fonction pour vérifier si la cellule est vivante
bool Cellule::estVivant() const {
    return etat == EtatCellule::VIVANT || etat == EtatCellule::OBSTACLE_VIVANT; // Retourner vrai si vivante
}

// Fonction pour vérifier si la cellule est vivante sans obstacle
bool Cellule::estVivantCouleur() const {
    return etat == EtatCellule::VIVANT; // Retourner vrai si vivante
}

// Fonction pour définir la cellule comme obstacle
void Cellule::defObstacle() {
    // Changer l'état en fonction de l'état actuel
    if (etat == EtatCellule::OBSTACLE_VIVANT) {
        etat = EtatCellule::MORT; // Rendre l'obstacle non vivant
    }
    else if (etat == EtatCellule::OBSTACLE_MORT) {
        etat = EtatCellule::OBSTACLE_VIVANT; // Rendre l'obstacle vivant
    }
    else if (etat == EtatCellule::MORT) {
        etat = EtatCellule::OBSTACLE_MORT; // Définir comme obstacle mort
    }
}

// Surcharge de la fonction pour définir un obstacle selon l'appui
void Cellule::defObstacle(int appui) {
    // Définir l'état en fonction de la valeur de appui
    if (appui == 2) {
        etat = EtatCellule::OBSTACLE_VIVANT; // Obstacle vivant
    }
    else {
        etat = EtatCellule::OBSTACLE_MORT; // Obstacle mort
    }
}

// Fonction pour vérifier si la cellule est un obstacle
bool Cellule::estObstacle() const {
    return etat == EtatCellule::OBSTACLE_VIVANT || etat == EtatCellule::OBSTACLE_MORT; // Retourner vrai si obstacle
}

// Fonction pour vérifier si l'obstacle est vivant
bool Cellule::estObstacleVivant() const {
    return etat == EtatCellule::OBSTACLE_VIVANT; // Retourner vrai si obstacle vivant
}

// Fonction pour vérifier si la cellule peut être modifiée
bool Cellule::estModifiable() const {
    return etat == EtatCellule::MORT || etat == EtatCellule::VIVANT; // Retourner vrai si modifiable
}
