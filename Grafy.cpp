//
// Created by arkad on 06.05.2022.
//



#include<conio.h>
#include<string>
#include<iostream>
#include <fstream>
#include <chrono>
#include <list>
#define max_weight 100


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
    bool addConection(int A, int B, int waga);
    void showList();
    void showMatrix();
    void runAList();
    void runAMatrix();
    void runBList();
    void runBMatrix();
    void makeCopyList(list<sasiad>);
    void deleteRandConnection();
    void addRandConnection(bool tryb);
    bool czyZepsuje(int elem1, int elem2);
    int losujWierzcholek() const;


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

bool Graph::addConection(int A, int B, int waga) {  //zwraca false gdy nie uda sie dodac krawedzi
    if(macierz[A][B]!=0) return false;
    //lista
    listaSasiedztwa[A].push_front(sasiad{B,waga});
    //macierz
    macierz[A][B]=waga;

    if(!czySkierowany){
        listaSasiedztwa[B].push_front(sasiad{A,waga});
        macierz[B][A] = waga;
    }
    //show();
    return true;
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
    clear();
    this->czySkierowany = czySkierowany;
    wierzcholki = wierz;
    krawedzie = wierzcholki * wierzcholki;      // realizacja rownania:gestosc % = 2*krawedzie/(wierzcholki*wierzcholki - wierzcholki)
    krawedzie -= wierzcholki;                   // maks ilosc wierzcholkow
    krawedzie *= gestosc;                       // krawedzie = (w*w - w) * g / 2*100
    krawedzie /= 200;
    if(czySkierowany) krawedzie*=2;             // w grafie skierowanym moze byc 2 razy wiecej krawedzi

    int istKraw = 0;                // potrzebne do sprawdzenia, czy mamy 99%
    create();                       //mamy wierzcholki, nie mamy krawedzi

    //TWORZENIE DRZEWA ROZPINAJACEGO
    //1. utworz liste wierzcholkow ktorych nie ma w grafie
    list<int> list1;
    for (int i = 0; i < wierzcholki; ++i) list1.push_front(i);
    int elem1;

    //2. utworz pusta liste
    list<int> list2;
    int elem2;

    _List_const_iterator<int> adr;
    int waga;

    //3. pierwszy wierzchołek to 0, dodaj go do listy tych ktore sa w grafie
    list2.push_front(list1.back());
    list1.pop_back();


    //4. dopuki lista1 nie jest pusta:
    while (!list1.empty()) {
        cout<<"\nszukam";
        //4a. wylosuj wierzcholek z listy2
        adr=list2.cbegin();
        for(int i = rand()%list2.size(); i>0; i--) adr.operator++();  //pamietaj o lepszym rand
        elem2 = *adr;
        //4b. wylosuj wierzcholek z listy 1, usun go z listy 1 i dodaj do listy 2
        adr = list1.cbegin();
        for(int i = rand()%list1.size(); i>0; i--) ++adr;  // jw
        elem1 = *adr;

        list1.remove(elem1);
        list2.push_back(elem1);
        //4c. wylosuj wage
        waga = rand()%max_weight;                               //jw
        //4d. utworz polaczenie i zwieksz istniejace kraw
        addConection(elem2, elem1, waga);
        istKraw++;
        cout<<"\n rozpinajajcy "<<list1.size();
        //showList();
        //getch();
    }
    //6. jesli to graf skierowany to do wierz 0 dodaj krawedz tak, by 0 był celem
    if(czySkierowany){
        addRandConnection(true);
        istKraw++;
    }


    //dodaj pozostałe krawedzie
    while(istKraw<krawedzie){
        addRandConnection(false);
        istKraw++;
    }
    // if 99 gest - dodaj dodatkowe 1/99 krawedzi i usun losowe 1/99 krawedzi
    if(gestosc==99){
        for (int i = istKraw/99; i >0 ; --i) {
            addRandConnection(false);
            istKraw++;
        }
        while(istKraw!=krawedzie){
            deleteRandConnection();
            istKraw--;
        }
    }
}

void Graph::addRandConnection(bool tryb) {
    int elem1, elem2, waga;
    do {
        elem1 = losujWierzcholek();
        if (tryb) elem1 = 0;
        do {
            elem2 = losujWierzcholek();
        } while (elem1 == elem2);
        waga = rand() % max_weight;
    } while(!addConection(elem2, elem1, waga));
}

void Graph::deleteRandConnection() {
    int elem1, elem2;
    do{                                         //1. Wylosuj
        do{
            elem1=losujWierzcholek();
        } while (listaSasiedztwa[elem1].size()<2);
        do{
            elem2=listaSasiedztwa[elem1].front().id;
            listaSasiedztwa[elem1].push_back(listaSasiedztwa[elem1].front());
            listaSasiedztwa[elem1].pop_front();
        } while (macierz[elem1][elem2]==0);
    } while(czyZepsuje(elem1, elem2));            //2. Sprawdz czy nie zaburzysz grafu
    //3. usun
    listaSasiedztwa[elem1].pop_back();
    macierz[elem1][elem2]=0;
    if(!czySkierowany){
       for(sasiad i = listaSasiedztwa[elem2].front(); i.id!=elem1; ){
           listaSasiedztwa[elem2].push_back(listaSasiedztwa[elem2].front());
           listaSasiedztwa[elem2].pop_front();
       }
       listaSasiedztwa[elem2].pop_back();
       macierz[elem2][elem1]=0;
    }
}

bool Graph::czyZepsuje(int elem1, int elem2) {
    //1 zsumuj wagi wiersza elem 2
    int suma = 0;
    for(int i=0; i<wierzcholki; i++){
        suma += macierz[i][elem2];
    }
    //2 odejmij od tego wage elem1:elem2
    suma -= macierz[elem1][elem2];
    //3 sprawdz czy ostrzymales wiecej niz 0
    if(suma==0) return true;
    return false;
}

int Graph::losujWierzcholek() const {
    return rand()%wierzcholki;
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
void createGraph(bool czySkierowany){
    int wierz, gest;
    cout<<"\nPodaj liczbe wierzcholkow: ";
    cin>>wierz;
    cout<<"\nPodaj gestosc: ";
    cin>>gest;
    myGraph.generate(wierz, gest, czySkierowany);
    myGraph.show();
}
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
                createGraph(0);
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
    return;
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
                createGraph(1);
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

