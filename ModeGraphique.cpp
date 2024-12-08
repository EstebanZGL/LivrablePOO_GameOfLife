#include "ModeGraphique.h"

// Constructeur principal pour initialiser la grille et la fenêtre
ModeGraphique::ModeGraphique(int ligne, int colonne, float Taille, int delaiMs)
    : grille(ligne, colonne, Taille), // Initialisation de la grille avec des dimensions et taille de cellule
    fenetreSFML(sf::VideoMode(colonne* Taille, ligne* Taille + 100), "Jeu de la Vie"), // Création de la fenêtre SFML
    running(false), // État du jeu (en cours ou non)
    editionGrille(true), // Mode d'édition activé
    CompteurIteration(0), // Compteur d'itérations initialisé à 0
    delai(delaiMs) { // Délai entre les itérations

    // Chargement de la police de caractères
    if (!police.loadFromFile("Roboto-Regular.ttf")) {
        throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'.");
    }

    // Configuration du texte pour afficher le nombre d'itérations
    TexteIteration.setFont(police);
    TexteIteration.setCharacterSize(20);
    TexteIteration.setFillColor(sf::Color::Black);
    TexteIteration.setPosition(5, ligne * Taille + 10); // Positionnement du texte

    // Configuration d'un autre texte pour les instructions
    toucheT.setFont(police);
    toucheT.setCharacterSize(20);
    toucheT.setFillColor(sf::Color::Black);
    toucheT.setPosition(5, ligne * Taille + 40); // Positionnement du texte
}

// Autre constructeur pour charger une grille depuis un fichier
ModeGraphique::ModeGraphique(const std::string& NomFichier, float Taille, int delaiMs)
    : grille(0, 0, Taille), // Initialisation avec une grille vide
    fenetreSFML(sf::VideoMode(800, 600), "Jeu de la Vie"), // Création de la fenêtre
    running(false),
    editionGrille(false), // Mode d'édition désactivé
    CompteurIteration(0),
    delai(delaiMs) {

    // Ouverture du fichier pour charger la grille
    grille.ouvrirFichier(NomFichier);
    fenetreSFML.create(sf::VideoMode(grille.avoirColonne() * Taille, grille.avoirLigne() * Taille + 100), "Jeu de la Vie");

    // Chargement de la police
    if (!police.loadFromFile("Roboto-Regular.ttf")) {
        throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'.");
    }

    // Configuration des textes comme précédemment
    TexteIteration.setFont(police);
    TexteIteration.setCharacterSize(20);
    TexteIteration.setFillColor(sf::Color::Black);
    TexteIteration.setPosition(5, grille.avoirLigne() * Taille + 10);

    toucheT.setFont(police);
    toucheT.setCharacterSize(20);
    toucheT.setFillColor(sf::Color::Black);
    toucheT.setPosition(5, grille.avoirLigne() * Taille + 40);
}

// Méthode pour créer une fenêtre d'affichage
void ModeGraphique::CreerFenetre(const std::string& Contenu) {
    // Chargement de la police pour le texte
    sf::Font police;
    if (!police.loadFromFile("Roboto-Regular.ttf")) {
        throw std::runtime_error("Impossible de charger la police.");
    }

    // Configuration du texte à afficher
    sf::Text texte;
    texte.setFont(police);
    texte.setString(Contenu);
    texte.setCharacterSize(30);
    texte.setFillColor(sf::Color::Black);

    // Calcul des dimensions de la fenêtre en fonction du texte
    sf::FloatRect limiteTexte = texte.getLocalBounds();
    float fenetreSFML2Largeur = limiteTexte.width + 50;
    float fenetreSFML2Hauteur = limiteTexte.height + 50;

    // Création de la nouvelle fenêtre
    sf::RenderWindow fenetreSFML2(sf::VideoMode(static_cast<unsigned int>(fenetreSFML2Largeur), static_cast<unsigned int>(fenetreSFML2Hauteur)), "Menu des touches");
    fenetreSFML2.setPosition(sf::Vector2i(50, 50));
    texte.setPosition(25, 25);

    // Boucle d'affichage de la fenêtre jusqu'à sa fermeture
    while (fenetreSFML2.isOpen()) {
        sf::Event evenement;
        while (fenetreSFML2.pollEvent(evenement)) {
            if (evenement.type == sf::Event::Closed) {
                fenetreSFML2.close(); // Ferme la fenêtre si l'événement de fermeture est détecté
            }
            else if (evenement.type == sf::Event::MouseButtonPressed) {
                fenetreSFML2.close(); // Ferme la fenêtre si un clic de souris est détecté
            }
        }

        fenetreSFML2.clear(sf::Color::White); // Nettoie la fenêtre
        fenetreSFML2.draw(texte); // Dessine le texte
        fenetreSFML2.display(); // Affiche le contenu de la fenêtre
    }
}

// Méthode pour charger des structures prédéfinies depuis un fichier
void ModeGraphique::StructurePredefinies(const std::string& NomFichier) {
    std::ifstream fichier(NomFichier);
    if (!fichier) {
        std::cerr << "Erreur lors de l'ouverture du fichier : " << NomFichier << std::endl;
        return; // Retourne si le fichier ne peut pas être ouvert
    }

    structureData.cellules.clear(); // Vide les cellules précédentes
    int x, y;
    while (fichier >> x >> y) {
        structureData.cellules.emplace_back(x, y); // Ajoute les cellules chargées
        std::cout << "Cellule chargee : (" << x << ", " << y << ")" << std::endl;
    }

    fichier.close(); // Ferme le fichier
}

// Méthode pour gérer les événements de touches
void ModeGraphique::GererTouches(sf::Event& evenement) {
    if (evenement.type == sf::Event::Closed) {
        fenetreSFML.close(); // Ferme la fenêtre
    }
    else if (evenement.type == sf::Event::MouseButtonPressed && editionGrille) {
        // Obtient les coordonnées de la grille selon la position de la souris
        int gridX = evenement.mouseButton.x / grille.avoirTailleCellule();
        int gridY = evenement.mouseButton.y / grille.avoirTailleCellule();
        if (evenement.mouseButton.button == sf::Mouse::Left) {
            grille.ModifierCellule(evenement.mouseButton.x, evenement.mouseButton.y); // Modifie une cellule
        }
        else if (evenement.mouseButton.button == sf::Mouse::Middle) {
            grille.defObstacle(gridX, gridY); // Définit un obstacle
        }
        else if (evenement.mouseButton.button == sf::Mouse::Right) {
            // Applique une structure prédéfinie aux cellules
            for (const auto& cell : structureData.cellules) {
                int x = gridY + cell.second;
                int y = gridX + cell.first;
                if (x >= 0 && x < grille.avoirLigne() && y >= 0 && y < grille.avoirColonne()) {
                    grille.ModifierCellule(y * grille.avoirTailleCellule(), x * grille.avoirTailleCellule());
                }
            }
        }
    }
    // Gestion des touches pour contrôler le jeu
    else if (evenement.type == sf::Event::KeyPressed) {
        if (evenement.key.code == sf::Keyboard::P) {
            running = !running; // Met le jeu en pause ou le relance
            editionGrille = !running; // Change le mode d'édition
        }
        // Chargement de structures prédéfinies avec les touches G et H
        else if (evenement.key.code == sf::Keyboard::G && !running) {
            StructurePredefinies("glider.txt");
            std::cout << "Structure chargee. Cliquez avec le bouton droit pour l'appliquer." << std::endl;
        }
        else if (evenement.key.code == sf::Keyboard::H && !running) {
            StructurePredefinies("canon_glider.txt");
            std::cout << "Structure chargee. Cliquez avec le bouton droit pour l'appliquer." << std::endl;
        }
        else if (evenement.key.code == sf::Keyboard::C && editionGrille) {
            fenetreSFML.close(); // Ferme la fenêtre si C est pressé
        }
        else if (evenement.key.code == sf::Keyboard::R && editionGrille) {
            grille.ReinitialiserGrille(); // Réinitialise la grille
        }
        else if (evenement.key.code == sf::Keyboard::Up && editionGrille) {
            delai += 50; // Augmente le délai
        }
        else if (evenement.key.code == sf::Keyboard::Down && editionGrille) {
            if (delai > 50) {
                delai -= 50; // Diminue le délai
            }
        }
        // Affiche le menu d'instructions
        else if (evenement.key.code == sf::Keyboard::T) {
            std::string message = "Voici le menu avec les differents boutons\n"
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

            CreerFenetre(message); // Crée une fenêtre pour afficher le message
        }
    }
}

// Boucle principale pour démarrer le mode graphique
void ModeGraphique::Demarrage() {
    while (fenetreSFML.isOpen()) {
        sf::Event evenement;
        while (fenetreSFML.pollEvent(evenement)) {
            GererTouches(evenement); // Gère les événements de touches
        }

        if (running) {
            grille.GrilleMAJ(); // Met à jour la grille si le jeu est en cours
            CompteurIteration++; // Incrémente le compteur d'itérations
            sf::sleep(sf::milliseconds(delai)); // Pause pour respecter le délai
        }

        TexteIteration.setString("Iterations: " + std::to_string(CompteurIteration));
        toucheT.setString("Menu: T ");

        fenetreSFML.clear(sf::Color::White);
        grille.DessinerGrille(fenetreSFML);
        fenetreSFML.draw(TexteIteration);
        fenetreSFML.draw(toucheT);
        fenetreSFML.display();
    }
}
