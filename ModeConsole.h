#ifndef CONSOLEGAME_H 
#define CONSOLEGAME_H 

#include <iostream> 
#include "grille.h" 
#include <thread> 
#include <chrono> 
#include <limits> 
#include <atomic> 
#include <fstream> 

class ModeConsole {
private:
    grille grille;
    int CompteurIteration;
    int delai;
    std::atomic<bool> running;
    std::ofstream FichierSortie;
    bool pause = 0;

public:
    ModeConsole(int ligne, int colonne, int delaiMs);
    ModeConsole(const std::string& NomFichier, int delaiMs);
    ~ModeConsole();

    void AffichageGrille();
    void SauvegardeEtat();
    void ThreadUtilisateur();
    void Demarrage();
};

#endif // CONSOLEGAME_H
