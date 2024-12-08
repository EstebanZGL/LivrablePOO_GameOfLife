#define CONSOLEGAME_HPP 

#include <iostream> 
#include "grille.h" 
#include <thread> 
#include <chrono> 
#include <limits> 
#include <atomic> 
#include <fstream> 


class ModeConsole { // Classe représentant le jeu en mode console.
private:

    grille grille; // Instance de la grille représentant l'état du jeu.
    int iterationCount; // Compteur d'itérations.
    int delay; // Délai entre chaque mise à jour de la grille (en millisecondes).
    std::atomic<bool> running; // Variable atomique pour contrôler l'exécution du jeu (thread-safe).
    std::ofstream outputFile; // Flux de fichier pour sauvegarder les états du jeu.
    bool pausing=0;

public:
    // Constructeur pour initialiser le jeu avec une grille vide.
    ModeConsole(int ligne, int colonne, int delayms)
        : grille(ligne, colonne, 1.0), 
        iterationCount(0), delay(delayms), running(true) { 
        outputFile.open("sauvegarde.txt"); // Ouvre un fichier pour sauvegarder les états.
        if (!outputFile.is_open()) { // Vérifie si le fichier s'est bien ouvert.
            throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde."); 
        }
    }

    // Constructeur pour initialiser le jeu à partir d'un fichier existant.
    ModeConsole(const std::string& NomFichier, int delayms)
        : grille(0, 0, 1.0f), iterationCount(0), delay(delayms), running(true) { 
        grille.loadFromFile(NomFichier); 
        outputFile.open("sauvegarde.txt"); // Ouvre un fichier pour sauvegarder les états.
        if (!outputFile.is_open()) { // Vérifie si le fichier s'est bien ouvert.
            throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde.");
        }
    }

    // Destructeur pour fermer le fichier de sortie si nécessaire.
    ~ModeConsole() {
        if (outputFile.is_open()) { 
            outputFile.close(); 
        }
    }

    // Méthode pour afficher la grille dans la console.
    void displayGrid() {
        if (!pausing) {
            printf("\033c"); // Efface la console.
            for (int x = 0; x < grille.getligne(); ++x) {
                for (int y = 0; y < grille.getcolonne(); ++y) {
                    if (grille.getCells()[x][y].getAlive() && !grille.getCells()[x][y].isObstacleAlive()) { // Si la cellule est vivante.
                        std::cout << "\033[33m1\033[0m "; // Affiche "1" en jaune 033=1 en ESC.
                    }
                    else if (grille.getCells()[x][y].isObstacleAlive()) {
                        std::cout << "\033[32mX\033[0m "; //Affiche "X" en vert pour l'obstacle vivant
                    }
                    else if (grille.getCells()[x][y].isObstacle() && !grille.getCells()[x][y].isObstacleAlive()) {
                        std::cout << "\033[31mX\033[0m "; //Affiche "X" en rouge pour l'obstacle mort
                    }
                    else {
                        std::cout << "0 ";
                    }
                }
                std::cout << std::endl;
            }


            std::cout << "Iterations: " << iterationCount << std::endl;
            std::cout << "Entrez sur 'q' pour quitter." << std::endl;

            // Sauvegarde l'état actuel de la grille dans le fichier.
            saveCurrentState();
        }
    }

    // Méthode pour sauvegarder l'état actuel de la grille dans un fichier.
    void saveCurrentState() {
        if (outputFile.is_open()) { 
            outputFile << "Itération: " << iterationCount << "\n"; 
            for (int x = 0; x < grille.getligne(); ++x) { 
                for (int y = 0; y < grille.getcolonne(); ++y) { 
                    outputFile << (grille.getCells()[x][y].getAlive() ? "1" : "0") << " ";

                }
                outputFile << "\n"; 
            }

            outputFile << "\n"; 
        }
    }

    // Méthode exécutée dans un thread séparé pour gérer les entrées utilisateur.
    void inputThread() {
        char input; 
        while (running) { // Boucle continue tant que le jeu est en cours.
            std::cin >> input; 
            if (input == 'q') { 
                running = false; 

            }
            else if (input == 'p') {
                pausing = !pausing;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');// Ignore le reste de la ligne pour éviter les problèmes d'entrée.
        }
    }


    // Méthode pour démarrer le jeu.
    void start() {
        // Lancement du thread pour gérer les entrées utilisateur.
        std::thread inputThread(&ModeConsole::inputThread, this);
        while (running) { 

            displayGrid(); 
            grille.updateGrid(); 
            iterationCount++; 
            std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Pause entre les mises à jour.
        }

        inputThread.join(); // Attend que le thread d'entrée utilisateur se termine.
    }
};

