#include <iostream>
#include "Game.h"

int main() {
    int rows, cols, delay;
    float cellSize = 20.0f;
    char choice;

    std::cout << "Bienvenue dans le Jeu de la Vie !\n";
    std::cout << "Voulez-vous (L)ire un fichier ou (E)diter manuellement ? (L/E) : ";
    std::cin >> choice;

    if (choice == 'L' || choice == 'l') {
        std::cout << "Entrez le nom du fichier : ";
        std::string filename;
        std::cin >> filename;

        try {
            Game game(filename, cellSize);
            game.start();
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << std::endl;
            return 1;
        }
    }
    else {
        std::cout << "Entrez le nombre de lignes : ";
        std::cin >> rows;
        std::cout << "Entrez le nombre de colonnes : ";
        std::cin >> cols;
        std::cout << "Entrez le temps entre chaque itération (en millisecondes) : ";
        std::cin >> delay;

        try {
            Game game(rows, cols, cellSize, delay);
            game.start();
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << std::endl;
            return 1;
        }
    }

    return 0;
}
