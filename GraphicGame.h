#ifndef GAME_HPP // Protection contre les inclusions multiples.
#define GAME_HPP // Définition de la macro GAME_HPP pour éviter les inclusions multiples.

#include <SFML/Graphics.hpp> // Inclusion de la bibliothèque SFML pour les graphiques.
#include <iostream> // Inclusion de la bibliothèque pour les entrées/sorties standard.
#include <fstream> // Inclusion pour la gestion des fichiers.
#include <vector> // Inclusion pour utiliser les vecteurs.
#include "Grid.h" // Inclusion de la classe Grid (grille).

struct StructureData { // Structure pour représenter les données à charger.
    std::vector<std::pair<int, int>> cells; // Liste de cellules à activer.
};

class GraphicGame {
private:
    Grid grid; // Instance de la grille pour le jeu.
    sf::RenderWindow window; // Fenêtre de rendu graphique SFML.
    sf::Font font; // Police utilisée pour afficher du texte.
    sf::Text iterationText; // Texte affichant le nombre d'itérations.
    sf::Text touche;
    sf::Text touche2;

    bool running; // Indique si la simulation est en cours.
    bool editing; // Mode édition pour permettre de modifier les cellules avec la souris.
    int iterationCount; // Compteur du nombre d'itérations.
    int delay; // Délai entre les mises à jour de la grille (en millisecondes).
    StructureData structureData; // Variable pour stocker les données de la structure.

public:
    GraphicGame(int ligne, int colonne, float Size, int delayMs) // Constructeur avec paramètres pour la grille.
        : grid(ligne, colonne, Size), // Initialisation de la grille.
        window(sf::VideoMode(colonne* Size, ligne* Size + 60), "Jeu de la Vie"), // Initialisation de la fenêtre.
        running(false), editing(true), iterationCount(0), delay(delayMs) { // Initialisation des variables.

        if (!font.loadFromFile("Roboto-Regular.ttf")) { // Chargement de la police.
            throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'."); // Erreur si la police n'est pas chargée.
        }

        iterationText.setFont(font); // Configuration de la police pour le texte.
        iterationText.setCharacterSize(20); // Taille du texte.
        iterationText.setFillColor(sf::Color::Black); // Couleur du texte.
        iterationText.setPosition(10, ligne * Size + 10); // Position du texte sous la grille.
    }

    GraphicGame(const std::string& filename, float Size, int delayMS) // Constructeur pour charger une grille depuis un fichier.
        : grid(0, 0, Size), // Initialisation par défaut de la grille.
        window(sf::VideoMode(800, 600), "Jeu de la Vie"), // Fenêtre avec taille par défaut.
        running(false), editing(false), iterationCount(0), delay(delayMS) { // Initialisation des variables.

        grid.loadFromFile(filename); // Chargement de la grille depuis un fichier.
        window.create(sf::VideoMode(grid.getcolonne() * Size, grid.getligne() * Size + 60), "Jeu de la Vie"); // Redimensionnement de la fenêtre selon la grille.

        if (!font.loadFromFile("Roboto-Regular.ttf")) { // Chargement de la police.
            throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'."); // Erreur si la police n'est pas chargée.
        }

        iterationText.setFont(font); // Configuration de la police pour le texte.
        iterationText.setCharacterSize(20); // Taille du texte.
        iterationText.setFillColor(sf::Color::Black); // Couleur du texte.
        iterationText.setPosition(10, grid.getligne() * Size + 10); // Position du texte sous la grille.

    
    }


    void createWindowWithText(const std::string& textContent) {
        // Créer une instance de la police
        sf::Font font;
        if (!font.loadFromFile("Roboto-Regular.ttf")) { // Assurez-vous que le fichier de police est accessible
            throw std::runtime_error("Impossible de charger la police.");
        }

        // Créer une instance de sf::Text
        sf::Text text;
        text.setFont(font);
        text.setString(textContent);
        text.setCharacterSize(30); // Taille de la police
        text.setFillColor(sf::Color::Black); // Couleur du texte

        // Calculer la taille nécessaire de la fenêtre
        sf::FloatRect textBounds = text.getLocalBounds();
        float window2Width = textBounds.width + 50; // Ajouter un peu de marge
        float window2Height = textBounds.height + 50; // Ajouter un peu de marge

        // Créer la fenêtre
        sf::RenderWindow window2(sf::VideoMode(static_cast<unsigned int>(window2Width), static_cast<unsigned int>(window2Height)), "Fenêtre avec Texte");

        // Centrer le texte dans la fenêtre
        text.setPosition(25, 25); // Positionnement avec une marge

        // Boucle principale de la fenêtre
        while (window2.isOpen()) {
            sf::Event event;
            while (window2.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window2.close(); // Fermer la fenêtre si l'utilisateur clique sur la croix
            }

            window2.clear(sf::Color::White); // Effacer la fenêtre avec un fond blanc
            window2.draw(text); // Dessiner le texte
            window2.display(); // Afficher le contenu de la fenêtre
        }
    }


    void loadStructureFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Erreur lors de l'ouverture du fichier : " << filename << std::endl;
            return;
        }

        structureData.cells.clear(); // Réinitialise les cellules.
        int x, y;
        while (file >> x >> y) { // Supposons que le fichier contient des paires de coordonnées.
            structureData.cells.emplace_back(x, y); // Ajoute chaque paire à la structure.
            std::cout << "Cellule chargée : (" << x << ", " << y << ")" << std::endl; // Debug
        }

        file.close(); // Ferme le fichier.
    }
    /*
    void placeSmallerGrid(int startX, int startY, int smallWidth, int smallHeight) {
        for (int i = 0; i < smallHeight; ++i) {
            for (int j = 0; j < smallWidth; ++j) {
                int x = startY + i;
                int y = startX + j;

                // Vérifiez que la position est valide avant de placer la cellule
                if (x >= 0 && x < grid.getligne() && y >= 0 && y < grid.getcolonne()) {
                    grid.toggleCell(y * grid.getCellSize(), x * grid.getCellSize()); // Active la cellule
                }
            }
        }
    }
    */

    void handleInput(sf::Event& event) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed && editing) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                grid.toggleCell(event.mouseButton.x, event.mouseButton.y);
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                int gridX = event.mouseButton.x / grid.getCellSize(); // Index de la colonne
                int gridY = event.mouseButton.y / grid.getCellSize(); // Index de la ligne

                // Placer la structure chargée depuis le fichier
                for (const auto& cell : structureData.cells) {
                    int x = gridY + cell.second;
                    int y = gridX + cell.first;
                    if (x >= 0 && x < grid.getligne() && y >= 0 && y < grid.getcolonne()) {
                        grid.toggleCell(y * grid.getCellSize(), x * grid.getCellSize());
                    }
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::P) {
                running = !running;
                editing = !running;
            }
            else if (event.key.code == sf::Keyboard::G && !running) {
                loadStructureFromFile("glider.txt");
                std::cout << "Structure chargée. Cliquez avec le bouton droit pour l'appliquer." << std::endl;
            }
            else if (event.key.code == sf::Keyboard::H && !running) {
                loadStructureFromFile("canon_glider.txt");
                std::cout << "Structure chargée. Cliquez avec le bouton droit pour l'appliquer." << std::endl;
            }
            else if (event.key.code == sf::Keyboard::C && editing) {
                window.close();
            }
            else if (event.key.code == sf::Keyboard::R && editing) {
                grid.clearGrid();
            }
            else if (event.key.code == sf::Keyboard::T) {
                std::string message = "";
                createWindowWithText(message); // Appeler la fonction pour créer la fenêtre avec le texte
                
            }
        }
    }


    void start() { // Méthode principale pour démarrer le jeu.
        while (window.isOpen()) { // Boucle principale tant que la fenêtre est ouverte.
            sf::Event event;
            while (window.pollEvent(event)) { // Gestion des événements utilisateur.
                handleInput(event); // Appel de la méthode de gestion des entrées.
            }

            if (running) { // Si la simulation est en cours.
                grid.updateGrid(); // Mise à jour de la grille.
                iterationCount++; // Incrémentation du compteur d'itérations.
                sf::sleep(sf::milliseconds(delay)); // Pause pour respecter le délai configuré.
            }

            // Mise à jour du texte des instructions
            touche.setString("Pause : P    Reinitialiser : R, Objet 1 : G   Objet 2 : H    Fermer : C");

            iterationText.setString("Iterations: " + std::to_string(iterationCount)); // Mise à jour du texte d'itérations.

            window.clear(sf::Color::White); // Efface la fenêtre avec un fond blanc.
            grid.draw(window); // Dessine la grille.
            window.draw(iterationText); // Dessine le texte des itérations.
            window.draw(touche); // Dessine le texte des instructions.
            window.display(); // Affiche le contenu de la fenêtre.
        }
    }
};

#endif // Fin de la protection contre les inclusions multiples.
