#ifndef CONSOLEGAME_HPP // Protection contre les inclusions multiples.
#define CONSOLEGAME_HPP // D�finition de la macro CONSOLEGAME_HPP pour �viter les inclusions multiples.

#include <iostream> // Inclusion de la biblioth�que pour les entr�es/sorties standard.
#include "Grid.h" // Inclusion de la classe Grid (grille).
#include <thread> // Inclusion pour g�rer les threads.
#include <chrono> // Inclusion pour g�rer les d�lais temporels.
#include <limits> // Inclusion pour d�finir les limites des types num�riques.
#include <atomic> // Inclusion pour utiliser des variables atomiques.

class ConsoleGame {
private:
    Grid grid; // Instance de la grille pour le jeu.
    int iterationCount; // Compteur du nombre d'it�rations.
    int delay; // D�lai entre les mises � jour de la grille (en millisecondes).
    std::atomic<bool> running; // Variable atomique pour contr�ler l'ex�cution.

public:
    ConsoleGame(int ligne, int colonne, int delayMs) // Constructeur initialisant une grille de taille donn�e.
        : grid(ligne, colonne, 1.0), // Initialisation de la grille.
        iterationCount(0), delay(delayMs), running(true) {} // Initialisation des autres variables membres.

    ConsoleGame(const std::string& filename, int delayMs) // Constructeur initialisant une grille � partir d'un fichier.
        : grid(0, 0, 1.0), iterationCount(0), delay(delayMs), running(true) {
        grid.loadFromFile(filename); // Chargement de la grille depuis le fichier.
    }

    void displayGrid() const { // M�thode pour afficher la grille dans la console.
        printf("\033c"); // Efface la console.
        for (int x = 0; x < grid.getligne(); ++x) { // Parcours des lignes de la grille.
            for (int y = 0; y < grid.getcolonne(); ++y) { // Parcours des colonnes de la grille.
                if (grid.getCells()[x][y].getAlive()) {
                    std::cout << "\033[31m1\033[0m "; // Affiche un "1" en rouge si la cellule est vivante.
                }
                else {
                    std::cout << "0 "; // Affiche un "0" si la cellule est morte.
                }
            }
            std::cout << std::endl; // Nouvelle ligne apr�s chaque rang�e.
        }
        std::cout << "Iterations: " << iterationCount << std::endl; // Affiche le nombre d'it�rations.
        std::cout << "Entrez sur 'q' pour quitter." << std::endl; // Message pour quitter le jeu.
    }

    void inputThread() { // M�thode pour g�rer les entr�es utilisateur dans un thread s�par�.
        char input;
        while (running) { // Boucle continue tant que le jeu est en cours.
            std::cin >> input; // Lecture de l'entr�e utilisateur.
            if (input == 'q') {
                running = false; // Arr�te le jeu si 'q' est saisi.
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore le reste de la ligne.
        }
    }

    void start() { // M�thode pour d�marrer le jeu.
        std::thread inputThread(&ConsoleGame::inputThread, this); // Lancement du thread d'entr�e utilisateur.
        while (running) { // Boucle principale du jeu tant que le jeu est en cours.
            displayGrid(); // Affiche la grille.
            grid.updateGrid(); // Met � jour l'�tat de la grille.
            iterationCount++; // Incr�mente le compteur d'it�rations.
            std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Pause entre les mises � jour.
        }
        inputThread.join(); // Attend la fin du thread d'entr�e utilisateur.
    }
};

#endif // Fin de la protection contre les inclusions multiples.
