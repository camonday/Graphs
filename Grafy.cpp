//
// Created by arkad on 06.05.2022.
//



#include<conio.h>
#include<string>
#include<iostream>
#include <fstream>
#include <chrono>
#include <list>


using namespace std;
using namespace std::chrono;

class Graph
{
    int krawedzie;
    int wierzcholki;
    int start;

    struct sasiad{
        int id;
        int wagaKrawedzi;
    };
    list<list<sasiad>> listaSasiedztwa;

    int **macierz;

public:
    void loadFromFile(string fileName);
    void generate(int wierz, int gestosc); //ustaw zmienna wierzcholki, na podstawie wierz i gest wylicz i ustaw zm kraw
    void show();
    void run(int mode);

private:
    void clear();
    void create();
    void addConection(int A, int B, int waga);
    void showList();
    void showMatrix();
    void runAList();
    void runAMatrix();
    void runBList();
    void runBMatrix();


};

void Graph::loadFromFile(string fileName) {
    clear();
    fstream file;
    file.open(fileName);

    file >> krawedzie >> wierzcholki;
    create();

    char test;
    file >> test;
    if(test != '\n') start = test - 48;

    int A,B,waga;
    for(int i = krawedzie; i>0; i--){
        file>>A>>B>>waga;
        addConection(A,B,waga);
    }
}

void Graph::show() {
    showMatrix();
    showList();
}

void Graph::clear() {

}


Graph myGraph;

void displayMenu(const string& info)
{
    std::cout << std::endl;
    std::cout << info << std::endl;
    std::cout << "1.Wczytaj z pliku" << std::endl;
    std::cout << "2.Wygeneruj losowo" << std::endl;
    std::cout << "3.Wyswietl" << std::endl;
    std::cout << "4.Algorytm" << std::endl;
    std::cout << "5.Testy" << std::endl;
    std::cout << "0.Powrot do menu" << std::endl;
    std::cout << "Podaj opcje:";
}

void displayGraph(){}
void createGraph(){}
void showGraph(){}

void menu_a() {
    char opt;
    string fileName;

    do {
        displayMenu("--- ALGORYTM PRIMA ---");
        opt = getche();
        std::cout << std::endl;
        switch (opt) {
            case '1': //tutaj wczytytwanie z pliku
                displayGraph();
                break;

            case '2': //tutaj losowanie
                createGraph();
                break;

            case '3': //tutaj wyswietlenie
                showGraph();
                break;

            case '4': //tutaj wykonanie algorytmu

                break;

            case '5':  //tutaj testy

                break;

        }
    } while (opt != 0);
}
void menu_b() {
    char opt;
    string fileName;

    do {
        displayMenu("--- ALGORYTM DIJKSTRY ---");
        opt = getche();
        std::cout << std::endl;
        switch (opt) {
            case '1': //tutaj wczytytwanie z pliku
                displayGraph();
                break;

            case '2': //tutaj losowanie
                createGraph();
                break;

            case '3': //tutaj wyswietlenie
                showGraph();
                break;

            case '4': //tutaj wykonanie algorytmu

                break;

            case '5':  //tutaj testy

                break;

        }
    } while (opt != 0);
}

int main(int argc, char* argv[])
{

    char option;
    do{
        std::cout << std::endl;
        std::cout << "==== MENU GLOWNE ===" << std::endl;
        std::cout << "1. Wyznaczanie minimalnego drzewa rozpinajacego (MST)" << std::endl;
        std::cout << "2. Wyznaczanie najkrotszej sciezki w grafie" << std::endl;
        std::cout << "0.Wyjscie" << std::endl;
        std::cout << "Podaj opcje:";
        option = getche();
        std::cout << std::endl;

        switch (option){
            case '1':
                menu_a();
                break;

            case '2':
                menu_b();
                break;

        }

    } while (option != '0');


    return 0;
}

