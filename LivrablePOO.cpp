#include <iostream>
#include "GraphicGame.h"
#include "ConsoleGame.h"

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



        std::cout << "Voulez-vous (G)raphique ou (C)onsole ? (G/C) : ";
        std::cin >> choice;

        if (choice == 'G' || choice == 'g') {
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay;
            try {
                GraphicGame graphicGame(filename, cellSize, delay);
                graphicGame.start();
            }
            catch (const std::exception& e) {
                std::cerr << "Erreur : " << e.what() << std::endl;
                return 1;
            }
        }
        else {
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay;
            try {
                ConsoleGame consoleGame(filename, delay);
                consoleGame.start();
            }
            catch (const std::exception& e) {
                std::cerr << "Erreur : " << e.what() << std::endl;
                return 1;
            }
        }
    }
    else {
        std::cout << "Entrez le nombre de lignes : ";
        std::cin >> rows;
        std::cout << "Entrez le nombre de colonnes : ";
        std::cin >> cols;
        std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
        std::cin >> delay;

        try {
            GraphicGame graphicGame(rows, cols, cellSize, delay);
            graphicGame.start();
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}