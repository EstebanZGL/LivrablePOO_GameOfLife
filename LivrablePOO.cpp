#include <iostream>        
#include "GraphicGame.h"   
#include "ConsoleGame.h"   
#include <fstream>         

namespace Torique {
    bool demanderModeTorique() {
        char choix;
        std::cout << "Voulez-vous activer le mode torique ? (O/N) : ";
        std::cin >> choix;
        return (choix == 'O' || choix == 'o');
    }
}

namespace ChoixGraphiqueConsole {
    char demanderModeAffichage() {
        char choix;
        std::cout << "Voulez-vous Graphique (G) ou Console (C) ? (G/C) : ";
        std::cin >> choix;
        return choix;
    }

    void lancerModeGraphique(const std::string& filename, float cellSize, int delay, bool torique) {
        try {
            GraphicGame graphicGame(filename, cellSize, delay, torique);
            graphicGame.start();
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << std::endl;
        }
    }

    void lancerModeConsole(const std::string& filename, int delay, bool torique) {
        try {
            ConsoleGame consoleGame(filename, delay, torique);
            consoleGame.start();
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << std::endl;
        }
    }
}

namespace ChoixEditer {
    void editerManuellement(int ligne, int colonne, float cellSize, int delay, bool torique) {
        try {
            GraphicGame graphicGame(ligne, colonne, cellSize, delay, torique);
            graphicGame.start();
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << std::endl;
        }
    }
}

int main() {
    int ligne, colonne, delay; // Dimensions de la grille et délai entre itérations.
    float cellSize = 20.0;     // Taille des cellules pour le mode graphique.
    char choix;                // Choix de l'utilisateur.
    bool torique = true;       // Mode torique par défaut.

    // Message de bienvenue.
    std::cout << "Bienvenue dans le Jeu de la Vie !\n";

    // Demande du mode torique.
    torique = Torique::demanderModeTorique();

    // Demande de la méthode de configuration.
    std::cout << "Voulez-vous Lire (L) un fichier ou Editer (E) manuellement ? (L/E) : ";
    std::cin >> choix;

    if (choix == 'L' || choix == 'l') {
        std::string filename;
        std::cout << "Entrez le nom du fichier : ";
        std::cin >> filename;

        // Demande du mode d'affichage.
        choix = ChoixGraphiqueConsole::demanderModeAffichage();
        std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
        std::cin >> delay;

        if (choix == 'G' || choix == 'g') {
            ChoixGraphiqueConsole::lancerModeGraphique(filename, cellSize, delay, torique);
        }
        else if (choix == 'C' || choix == 'c') {
            ChoixGraphiqueConsole::lancerModeConsole(filename, delay, torique);
        }
        else {
            std::cerr << "Choix invalide pour le mode (G/C)." << std::endl;
            return 1;
        }
    }
    else if (choix == 'E' || choix == 'e') {
        // Demande les dimensions de la grille.
        std::cout << "Entrez le nombre de lignes : ";
        std::cin >> ligne;
        std::cout << "Entrez le nombre de colonnes : ";
        std::cin >> colonne;
        std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
        std::cin >> delay;

        ChoixEditer::editerManuellement(ligne, colonne, cellSize, delay, torique);
    }
    else {
        std::cerr << "Choix invalide pour la méthode de configuration (L/E)." << std::endl;
        return 1;
    }

    return 0;
}
