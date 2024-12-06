#include "GraphicGame.h"

// Constructeur pour une grille vide.
GraphicGame::GraphicGame(int ligne, int colonne, float cellSize, int delayMs, bool torique)
    : grid(ligne, colonne, cellSize, torique),
    window(sf::VideoMode(colonne* cellSize, ligne* cellSize + 100), "Jeu de la Vie"),
    running(false), editing(true), iterationCount(0), delay(delayMs) {
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'.");
    }

    // Configuration des textes.
    iterationText.setFont(font);
    iterationText.setCharacterSize(20);
    iterationText.setFillColor(sf::Color::Black);
    iterationText.setPosition(10, ligne * cellSize + 10);

    toriqueText.setFont(font);
    toriqueText.setCharacterSize(20);
    toriqueText.setFillColor(sf::Color::Black);
    toriqueText.setPosition(200, ligne * cellSize + 10);
    updateToriqueText();

    fonctionText.setFont(font);
    fonctionText.setCharacterSize(20);
    fonctionText.setFillColor(sf::Color::Black);
    fonctionText.setPosition(10, ligne * cellSize + 50);
}

// Constructeur pour charger une grille depuis un fichier.
GraphicGame::GraphicGame(const std::string& filename, float cellSize, int delayMs, bool torique)
    : grid(0, 0, cellSize, torique),
    window(sf::VideoMode(800, 600), "Jeu de la Vie"),
    running(false), editing(false), iterationCount(0), delay(delayMs) {
    grid.loadFromFile(filename);
    window.create(sf::VideoMode(grid.getcolonne() * cellSize, grid.getligne() * cellSize + 60), "Jeu de la Vie");

    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'.");
    }

    iterationText.setFont(font);
    iterationText.setCharacterSize(20);
    iterationText.setFillColor(sf::Color::Black);
    iterationText.setPosition(10, grid.getligne() * cellSize + 10);

    toriqueText.setFont(font);
    toriqueText.setCharacterSize(20);
    toriqueText.setFillColor(sf::Color::Black);
    toriqueText.setPosition(200, grid.getligne() * cellSize + 10);
    updateToriqueText();

    fonctionText.setFont(font);
    fonctionText.setCharacterSize(20);
    fonctionText.setFillColor(sf::Color::Black);
    fonctionText.setPosition(10, grid.getligne() * cellSize + 50);
}

// Mise à jour du texte du mode torique.
void GraphicGame::updateToriqueText() {
    toriqueText.setString("Mode Torique : " + std::string(grid.isTorique() ? "Actif" : "Inactif"));
}

// Gestion des entrées utilisateur.
void GraphicGame::handleInput(sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    else if (event.type == sf::Event::MouseButtonPressed && editing) {
        int gridX = event.mouseButton.x / grid.getCellSize();
        int gridY = event.mouseButton.y / grid.getCellSize();
        if (event.mouseButton.button == sf::Mouse::Left) {
            grid.toggleCell(event.mouseButton.x, event.mouseButton.y);
        }
        else if (event.mouseButton.button == sf::Mouse::Middle) {
            grid.toggleObstacle(gridX, gridY);
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::P) {
            running = !running;
            editing = !running;
        }
        else if (event.key.code == sf::Keyboard::R && editing) {
            grid.clearGrid();
        }
        else if (event.key.code == sf::Keyboard::T) {
            grid.setTorique(!grid.isTorique());
            updateToriqueText();
        }
    }
}

// Lancement du jeu.
void GraphicGame::start() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleInput(event);
        }

        if (running) {
            grid.updateGrid();
            iterationCount++;
            sf::sleep(sf::milliseconds(delay));
        }

        iterationText.setString("Iterations: " + std::to_string(iterationCount));

        fonctionText.setString("clique milieu -> obstacle, P-> debut/pause, R pour reinitialiser, T torique/ non torique");

        window.clear(sf::Color::White);
        grid.draw(window);
        window.draw(iterationText);
        window.draw(toriqueText);
        window.draw(fonctionText);
        window.display();
    }
}
