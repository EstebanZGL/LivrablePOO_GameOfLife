#include <iostream> // Inclusion de la biblioth�que standard pour les entr�es/sorties.
#include "GraphicGame.h" // Inclusion de la classe pour le mode graphique.
#include "ConsoleGame.h" // Inclusion de la classe pour le mode console.

int main() { // Point d'entr�e principal du programme.
    int ligne, colonne, delay; // Variables pour les dimensions de la grille et le d�lai entre les it�rations.
    float cellSize = 20.0; // Taille des cellules pour le mode graphique.
    char choix; // Variable pour stocker le choix de l'utilisateur.

    // Affiche un message de bienvenue.
    std::cout << "Bienvenue dans le Jeu de la Vie !\n";

    // Demande � l'utilisateur s'il veut charger un fichier ou �diter manuellement.
    std::cout << "Voulez-vous Lire(L) un fichier ou Editer(E) manuellement ? (L/E) : ";
    std::cin >> choix; // Lecture du choix de l'utilisateur.

    if (choix == 'L' || choix == 'l') { // Si l'utilisateur choisit de lire un fichier.
        std::cout << "Entrez le nom du fichier : ";
        std::string filename; // Nom du fichier � charger.
        std::cin >> filename; // Lecture du nom du fichier.

        // Demande � l'utilisateur de choisir entre le mode graphique ou console.
        std::cout << "Voulez-vous Graphique(G) ou Console ? (G/C) : ";
        std::cin >> choix; // Lecture du choix.


        if (choix == 'G' || choix == 'g') { // Si l'utilisateur choisit le mode graphique.
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay; // Lecture du d�lai entre les it�rations.

            try {
                // Cr�ation et d�marrage du jeu en mode graphique.
                GraphicGame graphicGame(filename, cellSize, delay);
                graphicGame.start(); // Lancement du jeu.
            }
            catch (const std::exception& e) { // Gestion des erreurs �ventuelles.
                std::cerr << "Erreur : " << e.what() << std::endl; // Affiche l'erreur.
                return 1; // Termine le programme avec un code d'erreur.
            }
        }

        else { // Si l'utilisateur choisit le mode console.
            std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
            std::cin >> delay; // Lecture du d�lai entre les it�rations.

            try {
                // Cr�ation et d�marrage du jeu en mode console.
                ConsoleGame consoleGame(filename, delay);
                consoleGame.start(); // Lancement du jeu.
            }
            catch (const std::exception& e) { // Gestion des erreurs �ventuelles.
                std::cerr << "Erreur : " << e.what() << std::endl; // Affiche l'erreur.
                return 1; // Termine le programme avec un code d'erreur.
            }
        }
        
    }
    else { // Si l'utilisateur choisit d'�diter manuellement.
        // Demande les dimensions de la grille.
        std::cout << "Entrez le nombre de lignes : ";
        std::cin >> ligne; // Lecture du nombre de lignes.
        std::cout << "Entrez le nombre de colonnes : ";
        std::cin >> colonne; // Lecture du nombre de colonnes.
        std::cout << "Entrez le temps entre chaque iteration (en millisecondes) : ";
        std::cin >> delay; // Lecture du d�lai entre les it�rations.

        try {
            // Cr�ation et d�marrage du jeu en mode graphique avec une grille vide.
            GraphicGame graphicGame(ligne, colonne, cellSize, delay);
            graphicGame.start(); // Lancement du jeu.
        }
        catch (const std::exception& e) { // Gestion des erreurs �ventuelles.
            std::cerr << "Erreur : " << e.what() << std::endl; // Affiche l'erreur.
            return 1; // Termine le programme avec un code d'erreur.
        }
    }

    return 0; // Retourne 0 pour indiquer que le programme s'est termin� avec succ�s.
}

