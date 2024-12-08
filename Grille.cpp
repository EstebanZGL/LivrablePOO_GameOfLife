#include "Grille.h"
#include <iostream>

// Fonction pour compter les voisins vivants d'une cellule
int grille::CompterVoisins(int x, int y) const {
    int compte = 0; // Initialisation du compteur de voisins
    // Parcourir les cellules adjacentes
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // Ignorer la cellule elle-même

            // Calculer les coordonnées des voisins avec un wrap-around
            int nx = (x + dx + ligne) % ligne;
            int ny = (y + dy + colonne) % colonne;

            compte += cellules[nx][ny].estVivant(); // Incrémenter si le voisin est vivant
        }
    }
    return compte; // Retourner le nombre de voisins vivants
}

// Fonction pour obtenir la taille d'une cellule
float grille::avoirTailleCellule() const {
    return TailleCellule; // Retourner la taille de la cellule
}

// Constructeur de la grille avec dimensions et taille de cellule
grille::grille(int lig, int col, float taille)
    : ligne(lig), colonne(col), TailleCellule(taille), cellules(lig, std::vector<Cellule>(col)) {
}

// Fonction pour récupérer les cellules de la grille
const std::vector<std::vector<Cellule>>& grille::recupererCellule() const {
    return cellules; // Retourner la référence des cellules
}

// Fonction pour ouvrir un fichier et initialiser la grille
void grille::ouvrirFichier(const std::string& NomFichier) {
    std::ifstream fichier(NomFichier);
    if (fichier.is_open()) {
        fichier >> ligne >> colonne; // Lire les dimensions de la grille
        cellules.resize(ligne, std::vector<Cellule>(colonne)); // Redimensionner le vecteur de cellules

        // Initialiser les cellules à partir du fichier
        for (int x = 0; x < ligne; ++x) {
            for (int y = 0; y < colonne; ++y) {
                int etat;
                if (fichier >> etat) {
                    // Définir l'état de la cellule selon la valeur lue
                    if (etat == 1) {
                        cellules[x][y].defEtatCel(true); // Cellule vivante
                    }
                    else if (etat == 0) {
                        cellules[x][y].defEtatCel(false); // Cellule morte
                    }
                    else if (etat == 2 || etat == 3) {
                        cellules[x][y].defObstacle(); // Définir comme obstacle
                    }
                    else {
                        cellules[x][y].defEtatCel(false); // Par défaut, cellule morte
                    }
                }
            }
            fichier.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer le reste de la ligne
        }

        fichier.close(); // Fermer le fichier
    }
    else {
        throw std::runtime_error("Impossible d'ouvrir le fichier."); // Erreur si le fichier ne s'ouvre pas
    }
}

// Fonction pour obtenir le nombre de colonnes
int grille::avoirColonne() const {
    return colonne; // Retourner le nombre de colonnes
}

// Fonction pour obtenir le nombre de lignes
int grille::avoirLigne() const {
    return ligne; // Retourner le nombre de lignes
}

// Fonction pour définir un obstacle à une position donnée
void grille::defObstacle(int y, int x) {
    if (x >= 0 && x < ligne && y >= 0 && y < colonne) {
        cellules[x][y].defObstacle(); // Définir la cellule comme obstacle
    }
}

// Fonction pour modifier l'état d'une cellule en fonction de la position de la souris
void grille::ModifierCellule(int sourisX, int sourisY) {
    int x = sourisY / TailleCellule; // Calculer la position Y
    int y = sourisX / TailleCellule; // Calculer la position X
    if (x >= 0 && x < ligne && y >= 0 && y < colonne) {
        if (cellules[x][y].estModifiable()) { // Vérifier si la cellule est modifiable
            bool etatActuel = cellules[x][y].estVivant(); // État actuel de la cellule
            cellules[x][y].defEtatCel(!etatActuel); // Inverser l'état de la cellule
        }
    }
}

// Fonction pour mettre à jour la grille selon les règles du jeu
void grille::GrilleMAJ() {
    std::vector<std::vector<Cellule>> next = cellules; // Créer une copie de l'état actuel
    for (int x = 0; x < ligne; ++x) {
        for (int y = 0; y < colonne; ++y) {
            int voisins = CompterVoisins(x, y); // Compter les voisins vivants
            // Appliquer les règles de mise à jour
            if (cellules[x][y].estVivant()) {
                next[x][y].defEtatCel(voisins == 2 || voisins == 3); // Règle pour les cellules vivantes
            }
            else {
                next[x][y].defEtatCel(voisins == 3); // Règle pour les cellules mortes
            }
        }
    }
    cellules = next; // Mettre à jour l'état de la grille
}

// Fonction pour réinitialiser la grille
void grille::ReinitialiserGrille() {
    std::cout << "Reset de la grille" << std::endl; // Message de réinitialisation
    std::vector<std::vector<Cellule>> next = cellules; // Créer une copie de l'état actuel
    for (int x = 0; x < ligne; ++x) {
        for (int y = 0; y < colonne; ++y) {
            next[x][y].ReinitialiserCellules(); // Réinitialiser chaque cellule
        }
    }
    cellules = next; // Mettre à jour l'état de la grille
}

// Fonction pour dessiner la grille dans une fenêtre SFML
void grille::DessinerGrille(sf::RenderWindow& fenetreSFML) const {
    sf::RectangleShape FormeCellule(sf::Vector2f(TailleCellule, TailleCellule)); // Créer la forme de la cellule
    for (int x = 0; x < ligne; ++x) {
        for (int y = 0; y < colonne; ++y) {
            FormeCellule.setPosition(static_cast<float>(y) * TailleCellule, static_cast<float>(x) * TailleCellule); // Positionner la cellule

            // Déterminer la couleur de la cellule selon son état
            if (cellules[x][y].estVivantCouleur()) {
                FormeCellule.setFillColor(sf::Color::Black); // Cellule vivante
            }
            else if (cellules[x][y].estObstacleVivant()) {
                FormeCellule.setFillColor(sf::Color::Green); // Obstacle vivant
            }
            else if (cellules[x][y].estObstacle()) {
                FormeCellule.setFillColor(sf::Color::Red); // Obstacle non vivant
            }
            else {
                FormeCellule.setFillColor(sf::Color::White); // Cellule morte
            }
            fenetreSFML.draw(FormeCellule); // Dessiner la cellule
        }
    }

    sf::RectangleShape Posligne(sf::Vector2f(colonne * TailleCellule, 1)); // Ligne horizontale
    Posligne.setFillColor(sf::Color::Black); // Couleur de la ligne

    // Dessiner les lignes horizontales
    for (int i = 1; i < ligne; ++i) {
        Posligne.setPosition(0, static_cast<float>(i) * TailleCellule);
        fenetreSFML.draw(Posligne); // Dessiner la ligne
    }

    Posligne.setSize(sf::Vector2f(1, ligne * TailleCellule)); // Ligne verticale

    // Dessiner les lignes verticales
    for (int i = 1; i < colonne; ++i) {
        Posligne.setPosition(static_cast<float>(i) * TailleCellule, 0);
        fenetreSFML.draw(Posligne); // Dessiner la ligne
    }
}
