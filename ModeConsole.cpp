#include "ModeConsole.h"

// Constructeur pour le mode console avec dimensions de grille et d�lai
ModeConsole::ModeConsole(int ligne, int colonne, int delaiMs)
    : grille(ligne, colonne, 1.0), // Initialisation de la grille
    CompteurIteration(0), // Initialisation du compteur d'it�rations
    delai(delaiMs), // D�lai entre les it�rations
    running(true) { // �tat du jeu (en cours)

    // Ouverture du fichier de sauvegarde
    FichierSortie.open("sauvegarde.txt");
    if (!FichierSortie.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde.");
    }
}

// Constructeur pour le mode console avec chargement de fichier
ModeConsole::ModeConsole(const std::string& NomFichier, int delaiMs)
    : grille(0, 0, 1.0), // Initialisation d'une grille vide
    CompteurIteration(0), // Initialisation du compteur d'it�rations
    delai(delaiMs), // D�lai entre les it�rations
    running(true) { // �tat du jeu (en cours)

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
            std::cout << std::endl; // Nouvelle ligne apr�s chaque ligne de la grille
        }

        // Afficher le nombre d'it�rations et les instructions
        std::cout << "Iterations: " << CompteurIteration << std::endl;
        std::cout << "Entrez sur 'q' pour quitter." << std::endl;

        SauvegardeEtat(); // Sauvegarder l'�tat actuel de la grille
    }
}

// Fonction pour sauvegarder l'�tat de la grille dans un fichier
void ModeConsole::SauvegardeEtat() {
    if (FichierSortie.is_open()) { // V�rifier si le fichier est ouvert
        FichierSortie << "It�ration: " << CompteurIteration << "\n"; // �crire le num�ro d'it�ration
        // Parcourir chaque cellule pour sauvegarder son �tat
        for (int x = 0; x < grille.avoirLigne(); ++x) {
            for (int y = 0; y < grille.avoirColonne(); ++y) {
                FichierSortie << (grille.recupererCellule()[x][y].estVivant() ? "1" : "0") << " "; // 1 pour vivant, 0 pour mort
            }
            FichierSortie << "\n"; // Nouvelle ligne apr�s chaque ligne de la grille
        }
        FichierSortie << "\n"; // Ligne vide pour s�parer les �tats
    }
}

// Fonction pour g�rer les entr�es utilisateur dans un thread s�par�
void ModeConsole::ThreadUtilisateur() {
    char input;
    while (running) { // Tant que le jeu est en cours
        std::cin >> input; // Lire l'entr�e utilisateur
        if (input == 'q') {
            running = false; // Quitter le jeu
        }
        else if (input == 'p') {
            pause = !pause; // Mettre en pause ou reprendre le jeu
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorer le reste de la ligne
    }
}

// Fonction pour d�marrer la boucle principale du jeu
void ModeConsole::Demarrage() {
    std::thread ThreadUtilisateur(&ModeConsole::ThreadUtilisateur, this); // Lancer le thread pour les entr�es utilisateur
    while (running) { // Tant que le jeu est en cours
        while (!pause && running) {
            AffichageGrille(); // Afficher la grille
            grille.GrilleMAJ(); // Mettre � jour la grille
            CompteurIteration++; // Incr�menter le compteur d'it�rations
            std::this_thread::sleep_for(std::chrono::milliseconds(delai)); // Attendre le d�lai sp�cifi�
        }
    }
    ThreadUtilisateur.join(); // Attendre la fin du thread utilisateur
}
