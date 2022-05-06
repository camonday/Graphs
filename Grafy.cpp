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
    bool czySkierowany;

    struct sasiad{
        int id;
        int wagaKrawedzi;
    };
    list<sasiad> *listaSasiedztwa = nullptr;
    list<sasiad> copyList;

    int **macierz = nullptr;

public:
    void loadFromFile(string fileName, bool czySkierowany);
    void generate(int wierz, int gestosc, bool czySkierowany); //ustaw zmienna wierzcholki, na podstawie wierz i gest wylicz i ustaw zm kraw
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
    void makeCopyList(list<sasiad>);
    void deleteRandConnection();


};

void Graph::loadFromFile(string fileName, bool czySkierowany) {
    clear();
    fstream file;
    file.open(fileName);

    this->czySkierowany = czySkierowany;

    file >> krawedzie >> wierzcholki;
    create();

    if(czySkierowany) file>>start ;

    int A,B,waga;
    for(int i = krawedzie; i>0; i--){
        file>>A;
        file>>B;
        file>>waga;

        addConection(A,B,waga);
    }
}

void Graph::show() {
    showMatrix();
    showList();
}

void Graph::clear() {
    delete(listaSasiedztwa);
    listaSasiedztwa =NULL;
    delete(macierz);
    macierz =NULL;
}

void Graph::create() {
    listaSasiedztwa = new list<sasiad>[wierzcholki];

    macierz = new int*[wierzcholki];
    for (int i = 0; i < wierzcholki; ++i) {
        macierz[i]=new int [wierzcholki];
        for (int j =0; j<wierzcholki; ++j) macierz[i][j]=0;
    }
}

void Graph::addConection(int A, int B, int waga) {
    //lista
    listaSasiedztwa[A].push_front(sasiad{B,waga});
    //macierz
    macierz[A][B]=waga;

    if(!czySkierowany){
        listaSasiedztwa[B].push_front(sasiad{A,waga});
        macierz[B][A] = waga;
    }
}

void Graph::showList() {
    cout<<endl;
    for (int i = 0; i < wierzcholki; ++i) {
        cout <<"\n"<<i<<". ";
        makeCopyList(listaSasiedztwa[i]);

        while(!copyList.empty()){
            cout<<" "<<copyList.front().id<<'#'<<copyList.front().wagaKrawedzi;
            copyList.pop_front();
        }

    }
}

void Graph::makeCopyList(list<sasiad> first) {
    sasiad temp={0,0};

    do{
        first.push_back(temp);
        temp = first.front();
        copyList.push_front(temp);
        first.pop_front();
    }while(temp.wagaKrawedzi!=0);
    copyList.pop_front();
}

void Graph::showMatrix() {
    cout<<"\n\t";
    for (int i = 0; i < wierzcholki; ++i) {
        cout<<i<<"\t";
    }
    for (int i = 0; i < wierzcholki; ++i) {
        cout<<"\n"<<i<<"\t";
        for (int j = 0; j < wierzcholki; ++j) {
            cout<<macierz[j][i]<<"\t";
        }
    }

}

void Graph::generate(int wierz, int gestosc, bool czySkierowany) {
    this->czySkierowany=czySkierowany;
    wierzcholki=wierz;
    krawedzie=wierzcholki*wierzcholki;
    krawedzie-=wierzcholki;
    krawedzie*=(gestosc/200);
    int istKraw = 0;

    create();
    //tworzenie drzewa rozpinającego
    //1. utworz liste wierzcholkow ktorych nie ma w grafie

    //2. utworz pusta liste

    //3. pierwszy wierzchołek to 0, dodaj go do listy tych ktore sa w grafie

    //4. dopuki lista1 nie jest pusta:
        //4a. wylosuj wierzcholek z listy2
        //4b. wylosuj wierzcholek z listy 1, usun go z listy 1 i dodaj do listy 2
        //4c. wylosuj wage
        //4d. utworz polaczenie i zwieksz istniejace kraw

    //5. wyczysc listy
    //6. jesli to graf skierowany to do wierz 0 dodaj krawedz tak, by 0 był celem
    //dodaj pozostałe krawedzie
    // if 99 gest - dodaj dodatkowe 1/99 krawedzi i usun losowe 1/99 krawedzi
}


Graph myGraph;
string fileName;

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

void loadGraph(bool czySkierowany){
    cout<<"\nPodaj nazwe pliku: ";
    cin>>fileName;
    myGraph.loadFromFile(fileName,czySkierowany);
    myGraph.show();
}
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
                loadGraph(0);
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
                loadGraph(1);
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

