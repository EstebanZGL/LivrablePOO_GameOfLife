#include <iostream> // Inclusion de la bibliothèque standard pour les entrées/sorties.
#include <vector>
#include <stdexcept>
#include "GraphicGame.h" // Inclusion de la classe pour le mode graphique.
#include "ConsoleGame.h" // Inclusion de la classe pour le mode console.
#include <fstream>


// Fonction pour vérifier la validité de la grille
bool isValidGrid(const std::vector<std::vector<int>>& grid) {
    // Exemple de vérification : la grille ne doit pas être vide
    if (grid.empty() || grid[0].empty()) {
        return false; // Grille invalide
    }

    // Vérifier que toutes les lignes ont la même longueur
    size_t rowSize = grid[0].size();
    for (const auto& row : grid) {
        if (row.size() != rowSize) {
            return false; // Grille invalide
        }
    }

    // Ajouter d'autres vérifications selon les règles de votre jeu

    return true; // Grille valide
}

// Fonction de test unitaire
void testGridValidation() {
    std::vector<std::vector<int>> validGrid = { {0, 1, 0}, {1, 0, 1}, {0, 0, 0} };
    std::vector<std::vector<int>> emptyGrid = {};
    std::vector<std::vector<int>> invalidGrid = { {0, 1}, {1, 0, 1} };

    // Test de la grille valide
    if (!isValidGrid(validGrid)) {
        std::cerr << "Echec du test : la grille valide est jugee invalide." << std::endl;
    }
    else {
        std::cout << "Test reussi : la grille valide est correcte." << std::endl;
    }

    // Test de la grille vide
    if (isValidGrid(emptyGrid)) {
        std::cerr << "Echec du test : la grille vide est jugee valide." << std::endl;
    }
    else {
        std::cout << "Test reussi : la grille vide est correcte." << std::endl;
    }

    // Test de la grille invalide
    if (isValidGrid(invalidGrid)) {
        std::cerr << "Echec du test : la grille invalide est jugee valide." << std::endl;
    }
    else {
        std::cout << "Test reussi : la grille invalide est correcte." << std::endl;
    }
}



int main() { // Point d'entree principal du programme.
    int ligne, colonne, delay; // Variables pour les dimensions de la grille et le délai entre les itérations.
    float cellSize = 20.0; // Taille des cellules pour le mode graphique.
    char choix; // Variable pour stocker le choix de l'utilisateur.

    // Affiche un message de bienvenue.
    std::cout << "Bienvenue dans le Jeu de la Vie !\n";

    // Demande à l'utilisateur s'il veut charger un fichier ou éditer manuellement.
    std::cout << "Voulez-vous Lire(L) un fichier ou Editer(E) manuellement ? (L/E) : ";
    std::cin >> choix; // Lecture du choix de l'utilisateur.

    testGridValidation(); // Ajoutez cette ligne pour exécuter les tests

    if (choix == 'L' || choix == 'l') { // Si l'utilisateur choisit de lire un fichier.
        std::cout << "Entrez le nom du fichier : ";
        std::string filename; // Nom du fichier à charger.
        std::cin >> filename; // Lecture du nom du fichier.

        // Demande à l'utilisateur de choisir entre le mode graphique ou console.
        std::cout << "Voulez-vous Graphique(G) ou Console ? (G/C) : ";
        std::cin >> choix; // Lecture du choix.


        if (choix == 'G' || choix == 'g') { // Si l'utilisateur choisit le mode graphique.
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay; // Lecture du délai entre les itérations.

            try {
                // Création et démarrage du jeu en mode graphique.
                GraphicGame graphicGame(filename, cellSize, delay);
                graphicGame.start(); // Lancement du jeu.
            }
            catch (const std::exception& e) { // Gestion des erreurs éventuelles.
                std::cerr << "Erreur : " << e.what() << std::endl; // Affiche l'erreur.
                return 1; // Termine le programme avec un code d'erreur.
            }
        }

        else { // Si l'utilisateur choisit le mode console.
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay; // Lecture du délai entre les itérations.

            try {
                // Création et démarrage du jeu en mode console.
                ConsoleGame consoleGame(filename, delay);
                consoleGame.start(); // Lancement du jeu.
            }
            catch (const std::exception& e) { // Gestion des erreurs éventuelles.
                std::cerr << "Erreur : " << e.what() << std::endl; // Affiche l'erreur.
                return 1; // Termine le programme avec un code d'erreur.
            }
        }

    }
    else { // Si l'utilisateur choisit d'éditer manuellement.
        // Demande les dimensions de la grille.
        std::cout << "Entrez le nombre de lignes : ";
        std::cin >> ligne; // Lecture du nombre de lignes.
        std::cout << "Entrez le nombre de colonnes : ";
        std::cin >> colonne; // Lecture du nombre de colonnes.
        std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
        std::cin >> delay; // Lecture du délai entre les itérations.

        try {
            // Création et démarrage du jeu en mode graphique avec une grille vide.
            GraphicGame graphicGame(ligne, colonne, cellSize, delay);
            graphicGame.start(); // Lancement du jeu.
        }
        catch (const std::exception& e) { // Gestion des erreurs éventuelles.
            std::cerr << "Erreur : " << e.what() << std::endl; // Affiche l'erreur.
            return 1; // Termine le programme avec un code d'erreur.
        }
    }

    return 0; // Retourne 0 pour indiquer que le programme s'est terminé avec succès.
}
