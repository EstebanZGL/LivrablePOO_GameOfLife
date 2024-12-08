#ifndef GAME_HPP 
#define GAME_HPP 

#include <SFML/Graphics.hpp> 
#include <iostream> 
#include <fstream> 
#include <vector> 
#include "grille.h" 

struct StructureData { // Structure pour représenter les données à charger.
    std::vector<std::pair<int, int>> cellules; // Liste de cellules à activer.
};

class ModeGraphique {
private:
    grille grille; 
    sf::RenderWindow fenetreSFML; // Fenêtre de rendu graphique SFML.
    sf::Font police; // Police utilisée pour afficher du texte.
    sf::Text TexteIteration; 
    sf::Text touche;
    sf::Text touche2;
    sf::Text toucheT;

    bool running; // Indique si la simulation est en cours.
    bool editionGrille; // Mode édition.
    int CompteurIteration; // Compteur du nombre d'itérations.
    int delai; // Délai entre les mises à jour de la grille (en millisecondes).
    StructureData structureData; // Variable pour stocker les données de la structure.

public:
    ModeGraphique(int ligne, int colonne, float Taille, int delaiMs) // Constructeur avec paramètres pour la grille.
        : grille(ligne, colonne, Taille), // Initialisation de la grille.
        fenetreSFML(sf::VideoMode(colonne* Taille, ligne* Taille + 100), "Jeu de la Vie"), // Initialisation de la fenêtre.
        running(false), editionGrille(true), CompteurIteration(0), delai(delaiMs) { // Initialisation des variables.

        if (!police.loadFromFile("Roboto-Regular.ttf")) { // Chargement de la police.
            throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'."); // Erreur si la police n'est pas chargée.
        }

        TexteIteration.setFont(police); // Configuration de la police pour le texte.
        TexteIteration.setCharacterSize(20); // Taille du texte.
        TexteIteration.setFillColor(sf::Color::Black); // Couleur du texte.
        TexteIteration.setPosition(5, ligne * Taille + 10); // Position du texte sous la grille.

        //la même chose pour la touche T
        toucheT.setFont(police); 
        toucheT.setCharacterSize(20); 
        toucheT.setFillColor(sf::Color::Black); 
        toucheT.setPosition(5, ligne * Taille + 40); 
    }

    ModeGraphique(const std::string& NomFichier, float Taille, int delaiMs) // Constructeur pour charger une grille depuis un fichier.
        : grille(0, 0, Taille), // Initialisation par défaut de la grille.
        fenetreSFML(sf::VideoMode(800, 600), "Jeu de la Vie"), // Fenêtre avec taille par défaut.
        running(false), editionGrille(false), CompteurIteration(0), delai(delaiMs) { // Initialisation des variables.

        grille.ouvrirFichier(NomFichier); // Chargement de la grille depuis un fichier.
        fenetreSFML.create(sf::VideoMode(grille.avoirColonne() * Taille  , grille.avoirLigne() * Taille + 100), "Jeu de la Vie"); // Redimensionnement de la fenêtre selon la grille.
        
        if (!police.loadFromFile("Roboto-Regular.ttf")) { // Chargement de la police.
            throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'."); // Erreur si la police n'est pas chargée.
        }

        TexteIteration.setFont(police); 
        TexteIteration.setCharacterSize(20); 
        TexteIteration.setFillColor(sf::Color::Black); 
        TexteIteration.setPosition(5, grille.avoirLigne() * Taille + 10); 

        toucheT.setFont(police); 
        toucheT.setCharacterSize(20); 
        toucheT.setFillColor(sf::Color::Black); 
        toucheT.setPosition(5, grille.avoirLigne() * Taille + 40);

    
    }


    void CreerFenetre(const std::string& Contenu) {
        sf::Font police;
        if (!police.loadFromFile("Roboto-Regular.ttf")) {
            throw std::runtime_error("Impossible de charger la police.");
        }

        sf::Text texte;
        texte.setFont(police);
        texte.setString(Contenu);
        texte.setCharacterSize(30);
        texte.setFillColor(sf::Color::Black);

        sf::FloatRect limiteTexte = texte.getLocalBounds();
        float fenetreSFML2Largeur = limiteTexte.width + 50;
        float fenetreSFML2Hauteur = limiteTexte.height + 50;

        // Créer la fenêtre
        sf::RenderWindow fenetreSFML2(sf::VideoMode(static_cast<unsigned int>(fenetreSFML2Largeur), static_cast<unsigned int>(fenetreSFML2Hauteur)), "Menu des touches");

        // Positionner la fenêtre à une position spécifique x=100, y=100
        fenetreSFML2.setPosition(sf::Vector2i(50, 50)); 

        texte.setPosition(25, 25);

        while (fenetreSFML2.isOpen()) {
            sf::Event evenement;
            while (fenetreSFML2.pollEvent(evenement)) {
                if (evenement.type == sf::Event::Closed) {
                    fenetreSFML2.close();
                }
                else if (evenement.type == sf::Event::MouseButtonPressed) {
                    fenetreSFML2.close();
                }
            }
        

            fenetreSFML2.clear(sf::Color::White);
            fenetreSFML2.draw(texte);
            fenetreSFML2.display();
        }
    }


    void StructurePredefinies(const std::string& NomFichier) {
        std::ifstream fichier(NomFichier);
        if (!fichier) {
            std::cerr << "Erreur lors de l'ouverture du fichier : " << NomFichier << std::endl;
            return;
        }

        structureData.cellules.clear(); // Réinitialise les cellules.
        int x, y;
        while (fichier >> x >> y) { // Supposons que le fichier contient des paires de coordonnées.
            structureData.cellules.emplace_back(x, y); // Ajoute chaque paire à la structure.
            std::cout << "Cellule chargee : (" << x << ", " << y << ")" << std::endl; // Debug
        }

        fichier.close(); // Ferme le fichier.
    }

    void GererTouches(sf::Event& evenement) {
        if (evenement.type == sf::Event::Closed) {
            fenetreSFML.close();
        }
        else if (evenement.type == sf::Event::MouseButtonPressed && editionGrille) {
            int gridX = evenement.mouseButton.x / grille.avoirTailleCellule(); // Index de la colonne
            int gridY = evenement.mouseButton.y / grille.avoirTailleCellule(); // Index de la ligne
            if (evenement.mouseButton.button == sf::Mouse::Left) {
                grille.ModifierCellule(evenement.mouseButton.x, evenement.mouseButton.y);
            }
            else if (evenement.mouseButton.button == sf::Mouse::Middle) {
                // Action pour la molette de la souris
                
                grille.defObstacle(gridX, gridY); // Toggle l'état obstacle
            }
            else if (evenement.mouseButton.button == sf::Mouse::Right) {
                int gridX = evenement.mouseButton.x / grille.avoirTailleCellule(); // Index de la colonne
                int gridY = evenement.mouseButton.y / grille.avoirTailleCellule(); // Index de la ligne

                // Placer la structure chargée depuis le fichier
                for (const auto& cell : structureData.cellules) {
                    int x = gridY + cell.second;
                    int y = gridX + cell.first;
                    if (x >= 0 && x < grille.avoirLigne() && y >= 0 && y < grille.avoirColonne()) {
                        grille.ModifierCellule(y * grille.avoirTailleCellule(), x * grille.avoirTailleCellule());
                    }
                }
            }
        }
        // fonctionnement des touches
        else if (evenement.type == sf::Event::KeyPressed) { // utilisation de P pour le départ ou l'arret
            if (evenement.key.code == sf::Keyboard::P) {
                running = !running;
                editionGrille = !running;
            }
            else if (evenement.key.code == sf::Keyboard::G && !running) { // utilisation de G pour créer un glider
                StructurePredefinies("glider.txt");
                std::cout << "Structure chargee. Cliquez avec le bouton droit pour l'appliquer." << std::endl;
            }
            else if (evenement.key.code == sf::Keyboard::H && !running) { // utilisation de H pour créer un canon a glider
                StructurePredefinies("canon_glider.txt");
                std::cout << "Structure chargee. Cliquez avec le bouton droit pour l'appliquer." << std::endl;
            }
            else if (evenement.key.code == sf::Keyboard::C && editionGrille) { //utilisation de C pour fermer la fenêtre 
                fenetreSFML.close();
            }
            else if (evenement.key.code == sf::Keyboard::R && editionGrille) { //utilisation de R pour reset la page
                grille.ReinitialiserGrille();
            }
            else if (evenement.key.code == sf::Keyboard::Up && editionGrille) { //utilisation de la touche Up
				delai += 50; // Incrémentation du délai.
			}
            else if (evenement.key.code == sf::Keyboard::Down && editionGrille) { //utilisation de la touche Down
                if (delai > 50) {
                    delai -= 50; // Décrémentation du délai.
                }
            }
            else if (evenement.key.code == sf::Keyboard::T) { // utilisation de T pour afficher le menu
                
                std::string message =
                    "Voici le menu avec les differents boutons\n"
                    "pour faire fonctionner le jeu : \n"
                    "\n"
                    "- Click droit >> change l'etat d'une cellule \n"
                    "- Click gauche >> place la construction \n"
                    "- Click molette >> place un obstacle \n"
                    "- Mettre sur pause ou relancer >> P\n"
                    "- Reinitialiser la grille >> R \n"
                    "- Delai actuel : " + std::to_string(delai) +
                    "\n- Delai + >> Fleche Haut\n"
                    "- Delai - >> Fleche Bas\n"
                    "- Charger construction 1 >> G\n"
                    "- Charger construction 2 >> H\n"
                    "- Fermer la fenetre de la grille >> C";

                CreerFenetre(message); // Appeler la fonction pour créer la fenêtre avec le texte
                
            }
        }
    }


    void Demarrage() { // Méthode principale pour démarrer le jeu.
        while (fenetreSFML.isOpen()) { // Boucle principale tant que la fenêtre est ouverte.
            sf::Event evenement;
            while (fenetreSFML.pollEvent(evenement)) { // Gestion des événements utilisateur.
                GererTouches(evenement); // Appel de la méthode de gestion des entrées.
            }

            if (running) { // Si la simulation est en cours.
                grille.GrilleMAJ(); // Mise à jour de la grille.
                CompteurIteration++; // Incrémentation du compteur d'itérations.
                sf::sleep(sf::milliseconds(delai)); // Pause pour respecter le délai configuré.
            }


            TexteIteration.setString("Iterations: " + std::to_string(CompteurIteration)); // Mise à jour du texte d'itérations.
            toucheT.setString("Menu: T ");//affichage T

            fenetreSFML.clear(sf::Color::White); // Efface la fenêtre avec un fond blanc.
            grille.DessinerGrille(fenetreSFML); // Dessine la grille.
            fenetreSFML.draw(TexteIteration); // Dessine le texte des itérations.
            fenetreSFML.draw(toucheT); //dessiner touche T
            fenetreSFML.display(); // Affiche le contenu de la fenêtre.
        }
    }
};

#endif 
