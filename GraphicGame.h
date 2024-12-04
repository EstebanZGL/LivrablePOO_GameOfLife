#ifndef GAME_HPP // Protection contre les inclusions multiples.
#define GAME_HPP // Définition de la macro GAME_HPP pour éviter les inclusions multiples.

#include <SFML/Graphics.hpp> // Inclusion de la bibliothèque SFML pour les graphiques.
#include <iostream> // Inclusion de la bibliothèque pour les entrées/sorties standard.
#include "Grid.h" // Inclusion de la classe Grid (grille).

class GraphicGame {
private:
    Grid grid; // Instance de la grille pour le jeu.
    sf::RenderWindow window; // Fenêtre de rendu graphique SFML.
    sf::Font font; // Police utilisée pour afficher du texte.
    sf::Text iterationText; // Texte affichant le nombre d'itérations.

    bool running; // Indique si la simulation est en cours.
    bool editing; // Mode édition pour permettre de modifier les cellules avec la souris.
    int iterationCount; // Compteur du nombre d'itérations.
    int delay; // Délai entre les mises à jour de la grille (en millisecondes).

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

    void handleInput(sf::Event& event) { // Méthode pour gérer les entrées utilisateur.
        if (event.type == sf::Event::Closed) { // Si l'utilisateur ferme la fenêtre.
            window.close(); // Ferme la fenêtre.
        }
        else if (event.type == sf::Event::MouseButtonPressed && editing) { // Si un clic est détecté en mode édition.
            if (event.mouseButton.button == sf::Mouse::Left) { // Si c'est un clic gauche.
                grid.toggleCell(event.mouseButton.x, event.mouseButton.y); // Modifie l'état de la cellule cliquée.
            }
        }
        else if (event.type == sf::Event::KeyPressed) { // Si une touche est pressée.
            if (event.key.code == sf::Keyboard::P) { // Si la touche 'P' est pressée.
                running = !running; // Alterne entre démarrer/arrêter la simulation.
                editing = !running; // Le mode édition est actif si la simulation est arrêtée.
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

            iterationText.setString("Iterations: " + std::to_string(iterationCount)); // Mise à jour du texte d'itérations.

            window.clear(sf::Color::White); // Efface la fenêtre avec un fond blanc.
            grid.draw(window); // Dessine la grille.
            window.draw(iterationText); // Dessine le texte des itérations.
            window.display(); // Affiche le contenu de la fenêtre.
        }
    }
};

#endif // Fin de la protection contre les inclusions multiples.
