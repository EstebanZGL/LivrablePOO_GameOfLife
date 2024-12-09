#include "ModeConsole.h"

// Constructeur pour le mode console avec dimensions de grille et délai
ModeConsole::ModeConsole(int ligne, int colonne, int delaiMs)
    : grille(ligne, colonne, 1.0), // Initialisation de la grille
    CompteurIteration(0), // Initialisation du compteur d'itérations
    delai(delaiMs), // Délai entre les itérations
    running(true) { // État du jeu (en cours)

    // Ouverture du fichier de sauvegarde
    FichierSortie.open("sauvegarde.txt");
    if (!FichierSortie.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde.");
    }
}

// Constructeur pour le mode console avec chargement de fichier
ModeConsole::ModeConsole(const std::string& NomFichier, int delaiMs)
    : grille(0, 0, 1.0), // Initialisation d'une grille vide
    CompteurIteration(0), // Initialisation du compteur d'itérations
    delai(delaiMs), // Délai entre les itérations
    running(true) { // État du jeu (en cours)

    // Ouverture du fichier pour charger la grille
    grille.ouvrirFichier(NomFichier);
    // Ouverture du fichier de sauvegarde
    FichierSortie.open("sauvegarde.txt");
    if (!FichierSortie.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde.");
    }
}

// Destructeur pour fermer le fichier de sauvegarde
ModeConsole::~ModeConsole() {
    if (FichierSortie.is_open()) {
        FichierSortie.close(); // Fermeture du fichier si ouvert
    }
}

// Fonction pour afficher la grille dans la console
void ModeConsole::AffichageGrille() {
    if (!pause) { // Afficher seulement si le jeu n'est pas en pause
        printf("\033c"); // Effacer la console
        // Parcourir chaque cellule de la grille
        for (int x = 0; x < grille.avoirLigne(); ++x) {
            for (int y = 0; y < grille.avoirColonne(); ++y) {
                // Conditions pour afficher les cellules
                if (grille.recupererCellule()[x][y].estVivant() && !grille.recupererCellule()[x][y].estObstacleVivant()) {
                    std::cout << "\033[33m1\033[0m "; // Cellule vivante orange
                }
                else if (grille.recupererCellule()[x][y].estObstacleVivant()) {
                    std::cout << "\033[32mX\033[0m "; // Obstacle vivant vert
                }
                else if (grille.recupererCellule()[x][y].estObstacle() && !grille.recupererCellule()[x][y].estObstacleVivant()) {
                    std::cout << "\033[31mX\033[0m "; // Obstacle non vivant rouge
                }
                else {
                    std::cout << "0 "; // Cellule morte
                }
            }
            std::cout << std::endl; // Nouvelle ligne après chaque ligne de la grille
        }

        // Afficher le nombre d'itérations et les instructions
        std::cout << "Iterations: " << CompteurIteration << std::endl;
        std::cout << "Entrez sur 'q' pour quitter." << std::endl;

        SauvegardeEtat(); // Sauvegarder l'état actuel de la grille
    }
}

// Fonction pour sauvegarder l'état de la grille dans un fichier
void ModeConsole::SauvegardeEtat() {
    if (FichierSortie.is_open()) { // Vérifier si le fichier est ouvert
        FichierSortie << "Itération: " << CompteurIteration << "\n"; // Écrire le numéro d'itération
        // Parcourir chaque cellule pour sauvegarder son état
        for (int x = 0; x < grille.avoirLigne(); ++x) {
            for (int y = 0; y < grille.avoirColonne(); ++y) {
                FichierSortie << (grille.recupererCellule()[x][y].estVivant() ? "1" : "0") << " "; // 1 pour vivant, 0 pour mort
            }
            FichierSortie << "\n"; // Nouvelle ligne après chaque ligne de la grille
        }
        FichierSortie << "\n"; // Ligne vide pour séparer les états
    }
}

// Fonction pour gérer les entrées utilisateur dans un thread séparé
void ModeConsole::ThreadUtilisateur() {
    char input;
    while (running) { // Tant que le jeu est en cours
        std::cin >> input; // Lire l'entrée utilisateur
        if (input == 'q') {
            running = false; // Quitter le jeu
        }
        else if (input == 'p') {
            pause = !pause; // Mettre en pause ou reprendre le jeu
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer le reste de la ligne
    }
}

// Fonction pour démarrer la boucle principale du jeu
void ModeConsole::Demarrage() {
    std::thread ThreadUtilisateur(&ModeConsole::ThreadUtilisateur, this); // Lancer le thread pour les entrées utilisateur
    while (running) { // Tant que le jeu est en cours
        while (!pause && running) {
            AffichageGrille(); // Afficher la grille
            grille.GrilleMAJ(); // Mettre à jour la grille
            CompteurIteration++; // Incrémenter le compteur d'itérations
            std::this_thread::sleep_for(std::chrono::milliseconds(delai)); // Attendre le délai spécifié
        }
    }
    ThreadUtilisateur.join(); // Attendre la fin du thread utilisateur
}
