#include <iostream>        // Pour les entrées/sorties standard.
#include "GraphicGame.h"   // Classe pour le mode graphique.
#include "ConsoleGame.h"   // Classe pour le mode console.
#include <fstream>         // Pour la gestion des fichiers.

int main() {
    int ligne, colonne, delay; // Dimensions de la grille et délai entre itérations.
    float cellSize = 20.0;     // Taille des cellules pour le mode graphique.
    char choix;                // Choix de l'utilisateur.
    bool torique = true;       // Mode torique par défaut.

    // Message de bienvenue.
    std::cout << "Bienvenue dans le Jeu de la Vie !\n";

    // Choix du mode torique.
    std::cout << "Voulez-vous activer le mode torique ? (O/N) : ";
    std::cin >> choix;
    torique = (choix == 'O' || choix == 'o');

    // Demande à l'utilisateur s'il veut lire un fichier ou éditer manuellement.
    std::cout << "Voulez-vous Lire (L) un fichier ou Editer (E) manuellement ? (L/E) : ";
    std::cin >> choix;

    if (choix == 'L' || choix == 'l') { // Lecture d'un fichier.
        std::cout << "Entrez le nom du fichier : ";
        std::string filename;
        std::cin >> filename;

        // Mode graphique ou console ?
        std::cout << "Voulez-vous Graphique (G) ou Console (C) ? (G/C) : ";
        std::cin >> choix;

        if (choix == 'G' || choix == 'g') { // Mode graphique.
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay;

            try {
                // Démarrage du mode graphique.
                GraphicGame graphicGame(filename, cellSize, delay, torique);
                graphicGame.start();
            }
            catch (const std::exception& e) {
                std::cerr << "Erreur : " << e.what() << std::endl;
                return 1;
            }
        }
        else if (choix == 'C' || choix == 'c') { // Mode console.
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay;

            try {
                // Démarrage du mode console.
                ConsoleGame consoleGame(filename, delay, torique);
                consoleGame.start();
            }
            catch (const std::exception& e) {
                std::cerr << "Erreur : " << e.what() << std::endl;
                return 1;
            }
        }
        else {
            std::cerr << "Choix invalide pour le mode (G/C)." << std::endl;
            return 1;
        }

    }
    else if (choix == 'E' || choix == 'e') { // Édition manuelle.
        // Demande les dimensions de la grille.
        std::cout << "Entrez le nombre de lignes : ";
        std::cin >> ligne;
        std::cout << "Entrez le nombre de colonnes : ";
        std::cin >> colonne;
        std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
        std::cin >> delay;

        // Mode graphique pour édition manuelle.
        try {
            GraphicGame graphicGame(ligne, colonne, cellSize, delay, torique);
            graphicGame.start();
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << std::endl;
            return 1;
        }

    }
    else {
        std::cerr << "Choix invalide pour la méthode de configuration (L/E)." << std::endl;
        return 1;
    }

    return 0; // Fin du programme avec succès.
}
x