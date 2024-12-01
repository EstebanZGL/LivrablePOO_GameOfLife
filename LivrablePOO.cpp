#include "Game.h"
#include <iostream>
using namespace std;

int main() {
    int rows = 20, cols = 20;
    float cellSize = 20.0f;

    cout << "Bienvenue dans le Jeu de la Vie avec SFML !\n";
    Game game(rows, cols, cellSize);
    game.start();

    return 0;
}
