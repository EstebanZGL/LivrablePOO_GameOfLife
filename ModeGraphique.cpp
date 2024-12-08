#include "ModeGraphique.h"

// Constructeur principal pour initialiser la grille et la fen�tre
ModeGraphique::ModeGraphique(int ligne, int colonne, float Taille, int delaiMs)
    : grille(ligne, colonne, Taille), // Initialisation de la grille avec des dimensions et taille de cellule
    fenetreSFML(sf::VideoMode(colonne* Taille, ligne* Taille + 100), "Jeu de la Vie"), // Cr�ation de la fen�tre SFML
    running(false), // �tat du jeu (en cours ou non)
    editionGrille(true), // Mode d'�dition activ�
    CompteurIteration(0), // Compteur d'it�rations initialis� � 0
    delai(delaiMs) { // D�lai entre les it�rations

    // Chargement de la police de caract�res
    if (!police.loadFromFile("Roboto-Regular.ttf")) {
        throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'.");
    }

    // Configuration du texte pour afficher le nombre d'it�rations
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
    fenetreSFML(sf::VideoMode(800, 600), "Jeu de la Vie"), // Cr�ation de la fen�tre
    running(false),
    editionGrille(false), // Mode d'�dition d�sactiv�
    CompteurIteration(0),
    delai(delaiMs) {

    // Ouverture du fichier pour charger la grille
    grille.ouvrirFichier(NomFichier);
    fenetreSFML.create(sf::VideoMode(grille.avoirColonne() * Taille, grille.avoirLigne() * Taille + 100), "Jeu de la Vie");

    // Chargement de la police
    if (!police.loadFromFile("Roboto-Regular.ttf")) {
        throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'.");
    }

    // Configuration des textes comme pr�c�demment
    TexteIteration.setFont(police);
    TexteIteration.setCharacterSize(20);
    TexteIteration.setFillColor(sf::Color::Black);
    TexteIteration.setPosition(5, grille.avoirLigne() * Taille + 10);

    toucheT.setFont(police);
    toucheT.setCharacterSize(20);
    toucheT.setFillColor(sf::Color::Black);
    toucheT.setPosition(5, grille.avoirLigne() * Taille + 40);
}

// M�thode pour cr�er une fen�tre d'affichage
void ModeGraphique::CreerFenetre(const std::string& Contenu) {
    // Chargement de la police pour le texte
    sf::Font police;
    if (!police.loadFromFile("Roboto-Regular.ttf")) {
        throw std::runtime_error("Impossible de charger la police.");
    }

    // Configuration du texte � afficher
    sf::Text texte;
    texte.setFont(police);
    texte.setString(Contenu);
    texte.setCharacterSize(30);
    texte.setFillColor(sf::Color::Black);

    // Calcul des dimensions de la fen�tre en fonction du texte
    sf::FloatRect limiteTexte = texte.getLocalBounds();
    float fenetreSFML2Largeur = limiteTexte.width + 50;
    float fenetreSFML2Hauteur = limiteTexte.height + 50;

    // Cr�ation de la nouvelle fen�tre
    sf::RenderWindow fenetreSFML2(sf::VideoMode(static_cast<unsigned int>(fenetreSFML2Largeur), static_cast<unsigned int>(fenetreSFML2Hauteur)), "Menu des touches");
    fenetreSFML2.setPosition(sf::Vector2i(50, 50));
    texte.setPosition(25, 25);

    // Boucle d'affichage de la fen�tre jusqu'� sa fermeture
    while (fenetreSFML2.isOpen()) {
        sf::Event evenement;
        while (fenetreSFML2.pollEvent(evenement)) {
            if (evenement.type == sf::Event::Closed) {
                fenetreSFML2.close(); // Ferme la fen�tre si l'�v�nement de fermeture est d�tect�
            }
            else if (evenement.type == sf::Event::MouseButtonPressed) {
                fenetreSFML2.close(); // Ferme la fen�tre si un clic de souris est d�tect�
            }
        }

        fenetreSFML2.clear(sf::Color::White); // Nettoie la fen�tre
        fenetreSFML2.draw(texte); // Dessine le texte
        fenetreSFML2.display(); // Affiche le contenu de la fen�tre
    }
}

// M�thode pour charger des structures pr�d�finies depuis un fichier
void ModeGraphique::StructurePredefinies(const std::string& NomFichier) {
    std::ifstream fichier(NomFichier);
    if (!fichier) {
        std::cerr << "Erreur lors de l'ouverture du fichier : " << NomFichier << std::endl;
        return; // Retourne si le fichier ne peut pas �tre ouvert
    }

    structureData.cellules.clear(); // Vide les cellules pr�c�dentes
    int x, y;
    while (fichier >> x >> y) {
        structureData.cellules.emplace_back(x, y); // Ajoute les cellules charg�es
        std::cout << "Cellule chargee : (" << x << ", " << y << ")" << std::endl;
    }

    fichier.close(); // Ferme le fichier
}

// M�thode pour g�rer les �v�nements de touches
void ModeGraphique::GererTouches(sf::Event& evenement) {
    if (evenement.type == sf::Event::Closed) {
        fenetreSFML.close(); // Ferme la fen�tre
    }
    else if (evenement.type == sf::Event::MouseButtonPressed && editionGrille) {
        // Obtient les coordonn�es de la grille selon la position de la souris
        int gridX = evenement.mouseButton.x / grille.avoirTailleCellule();
        int gridY = evenement.mouseButton.y / grille.avoirTailleCellule();
        if (evenement.mouseButton.button == sf::Mouse::Left) {
            grille.ModifierCellule(evenement.mouseButton.x, evenement.mouseButton.y); // Modifie une cellule
        }
        else if (evenement.mouseButton.button == sf::Mouse::Middle) {
            grille.defObstacle(gridX, gridY); // D�finit un obstacle
        }
        else if (evenement.mouseButton.button == sf::Mouse::Right) {
            // Applique une structure pr�d�finie aux cellules
            for (const auto& cell : structureData.cellules) {
                int x = gridY + cell.second;
                int y = gridX + cell.first;
                if (x >= 0 && x < grille.avoirLigne() && y >= 0 && y < grille.avoirColonne()) {
                    grille.ModifierCellule(y * grille.avoirTailleCellule(), x * grille.avoirTailleCellule());
                }
            }
        }
    }
    // Gestion des touches pour contr�ler le jeu
    else if (evenement.type == sf::Event::KeyPressed) {
        if (evenement.key.code == sf::Keyboard::P) {
            running = !running; // Met le jeu en pause ou le relance
            editionGrille = !running; // Change le mode d'�dition
        }
        // Chargement de structures pr�d�finies avec les touches G et H
        else if (evenement.key.code == sf::Keyboard::G && !running) {
            StructurePredefinies("glider.txt");
            std::cout << "Structure chargee. Cliquez avec le bouton droit pour l'appliquer." << std::endl;
        }
        else if (evenement.key.code == sf::Keyboard::H && !running) {
            StructurePredefinies("canon_glider.txt");
            std::cout << "Structure chargee. Cliquez avec le bouton droit pour l'appliquer." << std::endl;
        }
        else if (evenement.key.code == sf::Keyboard::C && editionGrille) {
            fenetreSFML.close(); // Ferme la fen�tre si C est press�
        }
        else if (evenement.key.code == sf::Keyboard::R && editionGrille) {
            grille.ReinitialiserGrille(); // R�initialise la grille
        }
        else if (evenement.key.code == sf::Keyboard::Up && editionGrille) {
            delai += 50; // Augmente le d�lai
        }
        else if (evenement.key.code == sf::Keyboard::Down && editionGrille) {
            if (delai > 50) {
                delai -= 50; // Diminue le d�lai
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

            CreerFenetre(message); // Cr�e une fen�tre pour afficher le message
        }
    }
}

// Boucle principale pour d�marrer le mode graphique
void ModeGraphique::Demarrage() {
    while (fenetreSFML.isOpen()) {
        sf::Event evenement;
        while (fenetreSFML.pollEvent(evenement)) {
            GererTouches(evenement); // G�re les �v�nements de touches
        }

        if (running) {
            grille.GrilleMAJ(); // Met � jour la grille si le jeu est en cours
            CompteurIteration++; // Incr�mente le compteur d'it�rations
            sf::sleep(sf::milliseconds(delai)); // Pause pour respecter le d�lai
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
