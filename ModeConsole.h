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
    int CompteurIteration; // Compteur d'itérations.
    int delai; // Délai entre chaque mise à jour de la grille (en millisecondes).
    std::atomic<bool> running; // Variable atomique pour contrôler l'exécution du jeu (thread-safe).
    std::ofstream FichierSortie; // Flux de fichier pour sauvegarder les états du jeu.
    bool pause=0;

public:
    // Constructeur pour initialiser le jeu avec une grille vide.
    ModeConsole(int ligne, int colonne, int delaiMs)
        : grille(ligne, colonne, 1.0), 
        CompteurIteration(0), delai(delaiMs), running(true) { 
        FichierSortie.open("sauvegarde.txt"); // Ouvre un fichier pour sauvegarder les états.
        if (!FichierSortie.is_open()) { // Vérifie si le fichier s'est bien ouvert.
            throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde."); 
        }
    }

    // Constructeur pour initialiser le jeu à partir d'un fichier existant.
    ModeConsole(const std::string& NomFichier, int delaiMs)
        : grille(0, 0, 1.0f), CompteurIteration(0), delai(delaiMs), running(true) { 
        grille.ouvrirFichier(NomFichier); 
        FichierSortie.open("sauvegarde.txt"); // Ouvre un fichier pour sauvegarder les états.
        if (!FichierSortie.is_open()) { // Vérifie si le fichier s'est bien ouvert.
            throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde.");
        }
    }

    // Destructeur pour fermer le fichier de sortie si nécessaire.
    ~ModeConsole() {
        if (FichierSortie.is_open()) { 
            FichierSortie.close(); 
        }
    }

    // Méthode pour afficher la grille dans la console.
    void AffichageGrille() {
        if (!pause) {
            printf("\033c"); // Efface la console.
            for (int x = 0; x < grille.avoirLigne(); ++x) {
                for (int y = 0; y < grille.avoirColonne(); ++y) {
                    if (grille.recupererCellule()[x][y].estVivant() && !grille.recupererCellule()[x][y].estObstacleVivant()) { // Si la cellule est vivante.
                        std::cout << "\033[33m1\033[0m "; // Affiche "1" en jaune 033=1 en ESC.
                    }
                    else if (grille.recupererCellule()[x][y].estObstacleVivant()) {
                        std::cout << "\033[32mX\033[0m "; //Affiche "X" en vert pour l'obstacle vivant
                    }
                    else if (grille.recupererCellule()[x][y].estObstacle() && !grille.recupererCellule()[x][y].estObstacleVivant()) {
                        std::cout << "\033[31mX\033[0m "; //Affiche "X" en rouge pour l'obstacle mort
                    }
                    else {
                        std::cout << "0 ";
                    }
                }
                std::cout << std::endl;
            }


            std::cout << "Iterations: " << CompteurIteration << std::endl;
            std::cout << "Entrez sur 'q' pour quitter." << std::endl;

            // Sauvegarde l'état actuel de la grille dans le fichier.
            SauvegardeEtat();
        }
    }

    // Méthode pour sauvegarder l'état actuel de la grille dans un fichier.
    void SauvegardeEtat() {
        if (FichierSortie.is_open()) { 
            FichierSortie << "Itération: " << CompteurIteration << "\n"; 
            for (int x = 0; x < grille.avoirLigne(); ++x) { 
                for (int y = 0; y < grille.avoirColonne(); ++y) { 
                    FichierSortie << (grille.recupererCellule()[x][y].estVivant() ? "1" : "0") << " ";

                }
                FichierSortie << "\n"; 
            }

            FichierSortie << "\n"; 
        }
    }

    // Méthode exécutée dans un thread séparé pour gérer les entrées utilisateur.
    void ThreadUtilisateur() {
        char input; 
        while (running) { // Boucle continue tant que le jeu est en cours.
            std::cin >> input; 
            if (input == 'q') { 
                running = false; 
            }
            else if (input == 'p') {
                pause = !pause;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');// Ignore le reste de la ligne pour éviter les problèmes d'entrée.
        }
    }


    // Méthode pour démarrer le jeu.
    void Demarrage() {
        // Lancement du thread pour gérer les entrées utilisateur.
        std::thread ThreadUtilisateur(&ModeConsole::ThreadUtilisateur, this);
        while (running) { 
            AffichageGrille(); 
            grille.GrilleMAJ(); 
            CompteurIteration++; 
            std::this_thread::sleep_for(std::chrono::milliseconds(delai)); // Pause entre les mises à jour.
        }
        ThreadUtilisateur.join(); // Attend que le thread d'entrée utilisateur se termine.
    }
};

