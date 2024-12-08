#include <iostream> // Inclusion de la bibliothèque standard pour les entrées/sorties.
#include <vector>
#include <stdexcept>
#include "ModeGraphique.h" // Inclusion de la classe pour le mode graphique.
#include "ModeConsole.h" // Inclusion de la classe pour le mode console.
#include <fstream>


// Fonction pour vérifier la validité de la grille
bool ValidationGrille(const std::vector<std::vector<int>>& grille) {
    // Exemple de vérification : la grille ne doit pas être vide
    if (grille.empty() || grille[0].empty()) {
        return false; // Grille invalide
    }

    // Vérifier que toutes les lignes ont la même longueur
    size_t rowSize = grille[0].size();
    for (const auto& row : grille) {
        if (row.size() != rowSize) {
            return false; // Grille invalide
        }
    }

    // Ajouter d'autres vérifications selon les règles de votre jeu

    return true; // Grille valide
}

// Fonction de test unitaire
void testGrille() {
    std::vector<std::vector<int>> GrilleValide = { {0, 1, 0}, {1, 0, 1}, {0, 0, 0} };
    std::vector<std::vector<int>> GrilleVide = {};
    std::vector<std::vector<int>> GrilleInvalide = { {0, 1}, {1, 0, 1} };

    // Test de la grille valide
    if (!ValidationGrille(GrilleValide)) {
        std::cerr << "Echec du test : la grille valide est jugee invalide." << std::endl;
    }
    else {
        std::cout << "Test reussi : la grille valide est correcte." << std::endl;
    }

    // Test de la grille vide
    if (ValidationGrille(GrilleVide)) {
        std::cerr << "Echec du test : la grille vide est jugee valide." << std::endl;
    }
    else {
        std::cout << "Test reussi : la grille vide est correcte." << std::endl;
    }

    // Test de la grille invalide
    if (ValidationGrille(GrilleInvalide)) {
        std::cerr << "Echec du test : la grille invalide est jugee valide." << std::endl;
    }
    else {
        std::cout << "Test reussi : la grille invalide est correcte." << std::endl;
    }
}



int main() { // Point d'entree principal du programme.
    int ligne, colonne, delay; // Variables pour les dimensions de la grille et le délai entre les itérations.
    float TailleCellule = 20.0; // Taille des cellules pour le mode graphique.
    char choix; // Variable pour stocker le choix de l'utilisateur.

    // Affiche un message de bienvenue.
    std::cout << "Bienvenue dans le Jeu de la Vie !\n";

    // Demande à l'utilisateur s'il veut charger un fichier ou éditer manuellement.
    std::cout << "Voulez-vous Lire(L) un fichier ou Editer(E) manuellement ? (L/E) : ";
    std::cin >> choix; // Lecture du choix de l'utilisateur.


    if (choix == 'L' || choix == 'l') { // Si l'utilisateur choisit de lire un fichier.
        std::cout << "Entrez le nom du fichier : ";
        std::string NomFichier; // Nom du fichier à charger.
        std::cin >> NomFichier; // Lecture du nom du fichier.

        // Demande à l'utilisateur de choisir entre le mode graphique ou console.
        std::cout << "Voulez-vous Graphique(G) ou Console ? (G/C) : ";
        std::cin >> choix; // Lecture du choix.


        if (choix == 'G' || choix == 'g') { // Si l'utilisateur choisit le mode graphique.
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay; // Lecture du délai entre les itérations.

            try {
                // Création et démarrage du jeu en mode graphique.
                testGrille(); // Ajoutez cette ligne pour exécuter les tests
                ModeGraphique Graphique(NomFichier, TailleCellule, delay);
                Graphique.start(); // Lancement du jeu.
            }
            catch (const char* e) { // Gestion des erreurs éventuelles.
                std::cerr << "Erreur : " << e << std::endl; // Affiche l'erreur.
                return 1; // Termine le programme avec un code d'erreur.
            }
        }

        else { // Si l'utilisateur choisit le mode console.
            testGrille(); // Ajoutez cette ligne pour exécuter les tests
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay; // Lecture du délai entre les itérations.

            try {
                // Création et démarrage du jeu en mode console.
                ModeConsole Console(NomFichier, delay);
                Console.start(); // Lancement du jeu.
            }
            catch (const char* e) { // Gestion des erreurs éventuelles.
                std::cerr << "Erreur : " << e << std::endl; // Affiche l'erreur.
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
            testGrille(); // Ajoutez cette ligne pour exécuter les tests
            ModeGraphique Graphique(ligne, colonne, TailleCellule, delay);
            Graphique.start(); // Lancement du jeu.
        }
        catch (const char* e) { // Gestion des erreurs éventuelles.
            std::cerr << "Erreur : " << e << std::endl; // Affiche l'erreur.
            return 1; // Termine le programme avec un code d'erreur.
        }
    }

    return 0; // Retourne 0 pour indiquer que le programme s'est terminé avec succès.
}
