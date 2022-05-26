//
// Created by arkad on 06.05.2022.
//



#include<conio.h>
#include<string>
#include<iostream>
#include <fstream>
#include <chrono>
#include <list>
#include <queue>
#include <algorithm>

#define max_weight 100


using namespace std;
using namespace std::chrono;

class Graph
{
    int krawedzie;
    int wierzcholki;
    int start = 0;
    int maxKraw;
    bool czySkierowany;

    struct sasiad{
        int id;
        int wagaKrawedzi;
    };
    list<sasiad>* listaSasiedztwa = nullptr;
    list<sasiad> copyList;

    int **macierz = nullptr;

    struct krawedz{
        int poczatek, koniec, waga;
        string toString() const{
            return "\nA: " + to_string(poczatek) +"\tB: "+ to_string(koniec) +"\tWaga: "+ to_string(waga);
        }
    };
    struct PorowanajWage{
        bool operator()(krawedz const& p1, krawedz const&p2)
        {
            // return "true" if "p1" jest
            // przed "p2" w minHeap:
            return p1.waga > p2.waga;
        }
    };


public:
    void loadFromFile(string fileName, bool czySkierowany);
    void generate(int wierz, int gestosc, bool czySkierowany); //ustaw zmienna wierzcholki, na podstawie wierz i gest wylicz i ustaw zm kraw
    void show();
    void run();

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
    void addFullConnection();


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
    delete[] listaSasiedztwa;
    listaSasiedztwa = nullptr;
    delete[] macierz;
    macierz = nullptr;
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
    while(waga==0){
        waga=rand()%max_weight;
        //cout<<"\n waga byla 0 a jest "<<waga;
    }
    //lista
    listaSasiedztwa[A].push_front(sasiad{B,waga});
    //macierz
    macierz[A][B]=waga;

    if(!czySkierowany){
        listaSasiedztwa[B].push_front(sasiad{A,waga});
        macierz[B][A] = waga;
    }
    //show();
    //cout<<"\na: "<<A<<" b: "<<B<<" waga: "<<waga;
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
    maxKraw = wierzcholki * wierzcholki;      // realizacja rownania:gestosc % = 2*krawedzie/(wierzcholki*wierzcholki - wierzcholki)
    maxKraw -= wierzcholki;                   // maks ilosc wierzcholkow
    krawedzie = maxKraw * gestosc;                       // krawedzie = (w*w - w) * g / 2*100
    krawedzie /= 200;
    if(czySkierowany) krawedzie*=2;             // w grafie skierowanym moze byc 2 razy wiecej krawedzi
    else maxKraw /=2;
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
        //cout<<"\nszukam";
        //4a. wylosuj wierzcholek z listy2
        adr=list2.cbegin();
        for(int i = rand()%list2.size(); i>0; i--) ++adr;  //pamietaj o lepszym rand
        elem2 = *adr;
        //4b. wylosuj wierzcholek z listy 1, usun go z listy 1 i dodaj do listy 2
        adr = list1.cbegin();
        for(int i = rand()%list1.size(); i>0; i--) ++adr;  // jw
        elem1 = *adr;

        list1.remove(elem1);
        list2.push_back(elem1);
        //4c. wylosuj wage
        waga = rand()%max_weight;                               //jw
        waga++;
        //4d. utworz polaczenie i zwieksz istniejace kraw
        addConection(elem2, elem1, waga);
        istKraw++;
        //cout<<"\n rozpinajajcy "<<list1.size();
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
        /*while(istKraw!=maxKraw) {           //czy nie lepiej dodac krawedzie tam, gdzie w matrycy jest 0
            addRandConnection(false);
            istKraw++;
        }
         */
         addFullConnection();              //w. alternatywna
         //cout<<"\ndodalem do 100%";
         istKraw=maxKraw;
        while(istKraw!=krawedzie){
            deleteRandConnection();
            //cout<<"\nusuwam";
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
    sasiad* check1;
    sasiad* check2 = nullptr;
    do{                                         //1. Wylosuj
        do{
            elem1=losujWierzcholek();
        } while (listaSasiedztwa[elem1].size()<2);
        //cout<<"\nwylosowalem A usuwanego";
        check1 = &listaSasiedztwa[elem1].front();
        do{
            if(check1==check2){
                //cout<<"\n Czy zepsujepowinno byc true";
                break;
            }
            check2=&listaSasiedztwa[elem1].front();
            elem2=listaSasiedztwa[elem1].front().id;
            listaSasiedztwa[elem1].push_back(listaSasiedztwa[elem1].front());
            listaSasiedztwa[elem1].pop_front();

        } while (macierz[elem1][elem2]==0);
       // cout<<"\nWylosowalem B usuwanego";
    } while(czyZepsuje(elem1, elem2));            //2. Sprawdz czy nie zaburzysz grafu
    //3. usun
    //cout<<"\nusunolem ";
    listaSasiedztwa[elem1].pop_back();
    //cout<<" 1. ";
    macierz[elem1][elem2]=0;
   // cout<<" 2. ";
    if(!czySkierowany){
        //cout<<" 3. ";
       for(sasiad i = listaSasiedztwa[elem2].front(); i.id!=elem1; i=listaSasiedztwa[elem2].front() ){
           listaSasiedztwa[elem2].push_back(listaSasiedztwa[elem2].front());
           listaSasiedztwa[elem2].pop_front();
       }
        //cout<<" 4. ";
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
    //cout<<"\nczy zepsuje 1: "<<suma;
    //2 odejmij od tego wage elem1:elem2
    suma -= macierz[elem1][elem2];
    //cout<<"\nczy zepsuje 2: "<<suma;
    //3 sprawdz czy ostrzymales wiecej niz 0
    if(suma==0) return true;
    return false;
}

int Graph::losujWierzcholek() const {
    return rand()%wierzcholki;
}

void Graph::run() {
    if(czySkierowany){
        runBList();
        runBMatrix();
    }
    else{
        runAList();
        runAMatrix();
    }
}

void Graph::runAList() { //Prim for a list
    list<krawedz> krawedzieMST;
    list<int> wierzcholkiMST;
    priority_queue<krawedz, vector<krawedz>,PorowanajWage> sterta;
    krawedz check{};
    int koniec;
    int sum=0;

    //krawedzieMST.push_front(krawedz{start,start,0});
    wierzcholkiMST.push_front(start);
    for (auto const& i : listaSasiedztwa[start]) {
        sterta.push(krawedz{start,i.id,i.wagaKrawedzi});
    }

    while(wierzcholkiMST.size()!=wierzcholki){
        check = sterta.top();
        sterta.pop();
        koniec = check.koniec;
        if(std::find(wierzcholkiMST.begin(), wierzcholkiMST.end(), koniec)==wierzcholkiMST.end()){
            if(koniec!=wierzcholkiMST.back()){
                //dodaj krawedz check do drzewwa rozpinajacego
                krawedzieMST.push_back(check);
                wierzcholkiMST.push_front(koniec);
                for (auto const& i : listaSasiedztwa[koniec]) {
                    sterta.push(krawedz{koniec,i.id,i.wagaKrawedzi});
                }
            }
        }
    }
    cout<<"\n---DLA LISTY---";
    for (krawedz i : krawedzieMST) {
        cout<<i.toString();
        sum+=i.waga;
    }
    cout<<"\nSuma wag: "<<sum;

}

void Graph::runAMatrix() {
    list<krawedz> krawedzieMST;
    list<int> wierzcholkiMST;
    priority_queue<krawedz, vector<krawedz>,PorowanajWage> sterta;
    krawedz check{};
    int koniec;
    int sum=0;

    //krawedzieMST.push_front(krawedz{start,start,0});
    wierzcholkiMST.push_front(start);
    for (int i=0; i<wierzcholki; i++) {
        if(macierz[start][i]!=0){
            sterta.push(krawedz{start,i,macierz[start][i]});
        }
    }

    while(wierzcholkiMST.size()!=wierzcholki){
        check = sterta.top();
        sterta.pop();
        koniec = check.koniec;
        if(std::find(wierzcholkiMST.begin(), wierzcholkiMST.end(), koniec)==wierzcholkiMST.end()){
            if(koniec!=wierzcholkiMST.back()){
                //dodaj krawedz check do drzewwa rozpinajacego
                krawedzieMST.push_back(check);
                wierzcholkiMST.push_front(koniec);
                for (int i=0; i<wierzcholki; i++) {
                    if(macierz[koniec][i]!=0){
                        sterta.push(krawedz{koniec,i,macierz[koniec][i]});
                    }
                }
            }
        }
    }

    cout<<"\n\n---DLA MACIERZY---";
    for (krawedz i : krawedzieMST) {
        cout<<i.toString();
        sum+=i.waga;
    }
    cout<<"\nSuma wag: "<<sum;
}

void Graph::addFullConnection() {
    for (int i = 0; i < wierzcholki; ++i) {
        for(int j=0; j< wierzcholki; ++j){
            if(macierz[i][j]==0 && i!=j){ //i!j poniewaz nie ma krawedzi z tylko jednym wierzcholkiem
                addConection(i,j,rand()%max_weight);
                cout<<"\ndodalem uzupelniajace";
            }
        }
    }
}

void Graph::runBList() {
    struct wierzcholekSP {
            int id;
            int p;  // p — poprzednik
            int d;  // d - droga
    };
    list<int> nieprzebadane;
    wierzcholekSP tablica[wierzcholki];
    wierzcholekSP min{};
    int wyswietl;

    for (int i=0; i<wierzcholki; i++) {
        nieprzebadane.push_front(i);
        tablica[i] = wierzcholekSP{i,INT_MAX,INT_MAX};
        //cout<<"\n"<<tablica[i].d<<" "<<i;
    }
    tablica[start].d=0;

    while(!nieprzebadane.empty()) {
        //wybierz wierzcholek o najmnijeszym d
        min = tablica[nieprzebadane.front()];
        for (auto const i : nieprzebadane) {
            if (min.d > tablica[i].d) min=tablica[i];
        }
        //relaksacja sasiadow
        for (auto const i : listaSasiedztwa[min.id]) {
            if((i.wagaKrawedzi+min.d)<tablica[i.id].d){
                tablica[i.id].d = i.wagaKrawedzi+min.d;
                tablica[i.id].p = min.id;
            }
        }
        //usun wierzcholek z nieprzebadanych
        //1) przesun min na poczatek
        while(nieprzebadane.front()!=min.id) {
            nieprzebadane.push_back(nieprzebadane.front());
            nieprzebadane.pop_front();
        }
        //2) usun
        nieprzebadane.pop_front();
    }

    //wyswietl
    cout<<"Poczatek = "<<start;
    for (int i=0; i<wierzcholki; i++) {

        if(i!=start){
        wyswietl = tablica[i].p;
        cout<<"\nwierzcholek: "<<i<<" waga: "<<tablica[i].d<<"\ndroga: ";

            nieprzebadane.push_front(wyswietl);
            while (wyswietl != start) {
                wyswietl = tablica[wyswietl].p;
                nieprzebadane.push_front(wyswietl);
            }
            while(!nieprzebadane.empty()){
                cout<<nieprzebadane.front()<< " ";
                nieprzebadane.pop_front();
            }
            cout<<i;
        }

    }
}

void Graph::runBMatrix() {
    struct wierzcholekSP {
        int id;
        int p;  // p — poprzednik
        int d;  // d - droga
    };
    list<int> nieprzebadane;
    wierzcholekSP tablica[wierzcholki];
    wierzcholekSP min{};
    int wyswietl;

    for (int i=0; i<wierzcholki; ++i) {
        nieprzebadane.push_front(i);
        tablica[i] = wierzcholekSP{i,INT_MAX,INT_MAX};
    }
    tablica[start].d=0;

    cout<<"\n---MACIERZ---\n";

    while(!nieprzebadane.empty()) {
        //wybierz wierzcholek o najmnijeszym d
        min = tablica[nieprzebadane.front()];
        for (auto const i : nieprzebadane) {
            if (min.d > tablica[i].d) min=tablica[i];
        }
        //relaksacja sasiadow
        for (int i=0; i<wierzcholki; ++i) {
            if(0<macierz[min.id][i] && (macierz[min.id][i]+min.d)<tablica[i].d){
               /* cout<<"\nRelaksacja\n";
                cout<<"A: "<<min.id<<" B: "<<i;
                cout<<"\ndroga a: "<<min.d<<" krawedz ab: "<<macierz[min.id][i]<<" razem: "<<macierz[min.id][i]+min.d;
                cout<<" droga b: "<<tablica[i].d; */
                tablica[i].d = macierz[min.id][i]+min.d;
                tablica[i].p = min.id;
                //cout<<"\nTeraz droga b: "<<tablica[i].d;


            }
        }
        //usun wierzcholek z nieprzebadanych
        //1) przesun min na poczatek
        while(nieprzebadane.front()!=min.id) {
            nieprzebadane.push_back(nieprzebadane.front());
            nieprzebadane.pop_front();
        }
        //2) usun
        nieprzebadane.pop_front();
    }

    //wyswietl
    cout<<"\nPoczatek: "<<start;
    for (auto const i : tablica) {
        if(i.id != start) {
            wyswietl = i.p;
            cout << "\nwierzcholek: " << i.id << "waga: " << i.d << "\ndroga: " ;
            nieprzebadane.push_front(wyswietl);
            while (wyswietl != start) {
                wyswietl = tablica[wyswietl].p;
                nieprzebadane.push_front(wyswietl);
            }
            while(!nieprzebadane.empty()){
                cout<<nieprzebadane.front()<< " ";
                nieprzebadane.pop_front();
            }
        }
    }
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
void showGraph(){
    myGraph.show();
}
void runGraph(){
    myGraph.run();
}

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
                runGraph();
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
                runGraph();
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

