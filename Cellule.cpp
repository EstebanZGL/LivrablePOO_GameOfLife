#include "Cellule.h"

// Constructeur par d�faut de la cellule, initialisant l'�tat � MORT
Cellule::Cellule() : etat(EtatCellule::MORT) {}

// Fonction pour d�finir l'�tat de la cellule (vivante ou morte)
void Cellule::defEtatCel(bool vivant) {
    // Ne pas modifier si la cellule est un obstacle
    if (etat == EtatCellule::OBSTACLE_MORT || etat == EtatCellule::OBSTACLE_VIVANT) {
        return;
    }
    // D�finir l'�tat selon la valeur de vivant
    etat = vivant ? EtatCellule::VIVANT : EtatCellule::MORT;
}

// Fonction pour r�initialiser la cellule � l'�tat MORT
void Cellule::ReinitialiserCellules() {
    etat = EtatCellule::MORT; // R�initialiser l'�tat
}

// Fonction pour v�rifier si la cellule est vivante
bool Cellule::estVivant() const {
    return etat == EtatCellule::VIVANT || etat == EtatCellule::OBSTACLE_VIVANT; // Retourner vrai si vivante
}

// Fonction pour v�rifier si la cellule est vivante sans obstacle
bool Cellule::estVivantCouleur() const {
    return etat == EtatCellule::VIVANT; // Retourner vrai si vivante
}

// Fonction pour d�finir la cellule comme obstacle
void Cellule::defObstacle() {
    // Changer l'�tat en fonction de l'�tat actuel
    if (etat == EtatCellule::OBSTACLE_VIVANT) {
        etat = EtatCellule::MORT; // Rendre l'obstacle non vivant
    }
    else if (etat == EtatCellule::OBSTACLE_MORT) {
        etat = EtatCellule::OBSTACLE_VIVANT; // Rendre l'obstacle vivant
    }
    else if (etat == EtatCellule::MORT) {
        etat = EtatCellule::OBSTACLE_MORT; // D�finir comme obstacle mort
    }
}

// Surcharge de la fonction pour d�finir un obstacle selon l'appui
void Cellule::defObstacle(int appui) {
    // D�finir l'�tat en fonction de la valeur de appui
    if (appui == 2) {
        etat = EtatCellule::OBSTACLE_VIVANT; // Obstacle vivant
    }
    else {
        etat = EtatCellule::OBSTACLE_MORT; // Obstacle mort
    }
}

// Fonction pour v�rifier si la cellule est un obstacle
bool Cellule::estObstacle() const {
    return etat == EtatCellule::OBSTACLE_VIVANT || etat == EtatCellule::OBSTACLE_MORT; // Retourner vrai si obstacle
}

// Fonction pour v�rifier si l'obstacle est vivant
bool Cellule::estObstacleVivant() const {
    return etat == EtatCellule::OBSTACLE_VIVANT; // Retourner vrai si obstacle vivant
}

// Fonction pour v�rifier si la cellule peut �tre modifi�e
bool Cellule::estModifiable() const {
    return etat == EtatCellule::MORT || etat == EtatCellule::VIVANT; // Retourner vrai si modifiable
}
