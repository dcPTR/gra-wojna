#include <iostream>
#include <ctime>
#include <string.h>

#define ILOSC_KART 52
#define ILOSC_GRACZY 2
#define DEBUG 0
#define GRACZ1 1
#define GRACZ2 2
#define PRAWDA 1
#define FALSZ 0
#define NIKT 404

using namespace std;

typedef struct karta{
    int kolor;
    int figura;
} Karta;

typedef struct ListaKart{
    Karta karta;
    ListaKart* next;
} listaKart_t;

typedef struct reka{
    listaKart_t* karty;
    int ilosc_kart;
} Reka;

typedef struct talia{
    Karta karty[ILOSC_KART];
} Talia;

typedef struct gracz{
    Reka reka;
} Gracz;

void tasuj(Talia* talia, int wielkoscTalii){
    Karta temp;
    int random;

    for(int i=0; i<wielkoscTalii; i++){
        do{
            random = rand()%wielkoscTalii;
        }
        while(random == i);

        temp = talia->karty[i];
        talia->karty[i] = talia->karty[random];
        talia->karty[random] = temp;
    }
}

void tasujTabliceKart(Karta karty[], int n){
    Karta temp;
    int random;
    for(int i=0; i<n; i++){
        do{
            random = rand()%n;
        }
        while(random == i);

        temp = karty[i];
        karty[i] = karty[random];
        karty[random] = temp;
    }
}

void dodajKarte(Gracz* gracz, Karta karta){
    listaKart_t** start = &(gracz->reka.karty);
    listaKart_t* nowaKarta = (listaKart_t* ) malloc(sizeof(listaKart_t));
    nowaKarta->karta = karta;
    nowaKarta->next = NULL;
    listaKart_t* biezacy = *start;

    while(biezacy->next != NULL){
        biezacy = biezacy->next;
    }
    biezacy->next = nowaKarta;
    gracz->reka.ilosc_kart++;
}

Karta pobierzKarte(Gracz* gracz){
    listaKart_t** start = &(gracz->reka.karty);
    listaKart_t* biezacy = *start;
    listaKart_t* pierwszy = biezacy->next;
    Karta pierwszaKarta = pierwszy->karta;

    if(biezacy == NULL || biezacy->next == NULL){
        cout<<"Nie mam czego pobrac Przegrales!"<<endl;
    }
    else{
        biezacy->next = biezacy->next->next;
        free(pierwszy);
        gracz->reka.ilosc_kart -= 1;
        return pierwszaKarta;
    }
}

void wyswietlListe(listaKart_t* start){
    listaKart_t* biezacy = start;
    if(biezacy == NULL || biezacy->next == NULL){
        cout<<"Lista jest pusta"<<endl;
    }
    else
    {
        while(biezacy != NULL){
            cout<<biezacy->karta.kolor<<" " << biezacy->karta.figura << " " << "--> ";
            biezacy = biezacy->next;
        }
        cout<<endl;
    }
}

void wyswietlNazwyKart(Karta karta1, Karta karta2){
    char kolor[4][6] = {"pik","kier","karo", "trefl"};
    char figura[13][6] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};

    cout<<"X: "<<kolor[karta1.kolor]<<"-"<<figura[karta1.figura]<<" vs "<<"Y: "<<kolor[karta2.kolor]<<"-"<<figura[karta2.figura]<<endl;
}

int sprawdzKoniec(const Gracz gracz[]){
    if(gracz[0].reka.ilosc_kart <= 0){
        return 1;
    }
    if(gracz[1].reka.ilosc_kart <= 0){
        return 2;
    }
    return 0;
}

int wojnaA(Gracz gracz[], Karta kartaGracz1, Karta kartaGracz2, Karta kartyWojna[], int* iloscKartWojna, int* iloscRuchow, int dwieKartyMaGracz){

    switch(sprawdzKoniec(gracz)){
        case 1:
            return GRACZ2; //wygrał gracz 2
        case 2:
            return GRACZ1; //wygrał gracz 1
    }

    *iloscKartWojna += 2; // dodaje te poniżej
    switch(dwieKartyMaGracz){
        case GRACZ1:
            kartyWojna[*iloscKartWojna-2] = kartaGracz1;
            kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]);
            break;
        case GRACZ2:
            kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]);
            kartyWojna[*iloscKartWojna-1] = kartaGracz2;
            break;
        default:
            kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]);
            kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]);
    }
    dwieKartyMaGracz = NIKT; // nikt

    (*iloscRuchow)++;

    switch(sprawdzKoniec(gracz)){
        case 1:
            return GRACZ2; //wygrał gracz 2
        case 2:
            return GRACZ1; //wygrał gracz 1
    }

    kartaGracz1 = pobierzKarte(&gracz[0]);
    kartaGracz2 = pobierzKarte(&gracz[1]);
    (*iloscRuchow)++;

    *iloscKartWojna += 2;

    kartyWojna[*iloscKartWojna-2] = kartaGracz1;
    kartyWojna[*iloscKartWojna-1] = kartaGracz2;

    if(kartaGracz1.figura > kartaGracz2.figura){
        tasujTabliceKart(kartyWojna, *iloscKartWojna);
        for(int i=0; i<*iloscKartWojna; i+=1){ //swoje karty
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        return 0;
    }
    else if(kartaGracz2.figura > kartaGracz1.figura){
        tasujTabliceKart(kartyWojna, *iloscKartWojna);
        for(int i=0; i<*iloscKartWojna; i+=1){ //swoje karty
            dodajKarte(&gracz[1], kartyWojna[i]);
        }
        return 0;
    }
    else{
        return wojnaA(gracz, kartaGracz1, kartaGracz2, kartyWojna, iloscKartWojna, iloscRuchow, dwieKartyMaGracz);
    }
}

int gra(Gracz gracz[], int* iloscRuchow, int wielkoscTalii){
    Karta kartaGracz1, kartaGracz2, karty1[2], karty2[2];
    int iloscKartWojna = 0, wynikWojny = 0, dwieKarty = GRACZ2, numerKartyWojna, bylaWojna = 1, temp=0;
    Karta* kartyWojna = (Karta*) malloc(wielkoscTalii*sizeof(Karta));
    srand(time(NULL));

    while(PRAWDA){
        if(bylaWojna == FALSZ){
            if(dwieKarty == GRACZ1){ // teraz pobiera dwie karty
                if(gracz[0].reka.ilosc_kart >= 2){
                    karty1[0] = pobierzKarte(&gracz[0]);
                    karty1[1] = pobierzKarte(&gracz[0]);
                    kartaGracz2 = karty2[!numerKartyWojna];
                    numerKartyWojna = rand()%2;
                    kartaGracz1 = karty1[numerKartyWojna];
                }
                else{
                    kartaGracz1 = pobierzKarte(&gracz[0]);
                    kartaGracz2 = pobierzKarte(&gracz[1]);
                }

            }
            else if(dwieKarty == GRACZ2){ // teraz pobiera dwie karty
                if(gracz[1].reka.ilosc_kart >= 2) {
                    karty2[0] = pobierzKarte(&gracz[1]);
                    karty2[1] = pobierzKarte(&gracz[1]);
                    kartaGracz1 = karty1[!numerKartyWojna];
                    numerKartyWojna = rand()%2;
                    kartaGracz2 = karty2[numerKartyWojna];
                }
                else{
                    kartaGracz1 = pobierzKarte(&gracz[0]);
                    kartaGracz2 = pobierzKarte(&gracz[1]);
                }
            }
        }
        else if(bylaWojna == PRAWDA){
            if(dwieKarty == GRACZ1){ // teraz pobiera dwie karty
                if(gracz[0].reka.ilosc_kart >= 2) {
                    karty1[0] = pobierzKarte(&gracz[0]);
                    karty1[1] = pobierzKarte(&gracz[0]);
                    kartaGracz2 = pobierzKarte(&gracz[1]);
                    numerKartyWojna = rand() % 2;
                    kartaGracz1 = karty1[numerKartyWojna];
                }
                else{
                    kartaGracz1 = pobierzKarte(&gracz[0]);
                    kartaGracz2 = pobierzKarte(&gracz[1]);
                }
            }
            else if(dwieKarty == GRACZ2){ // teraz pobiera dwie karty
                if(gracz[1].reka.ilosc_kart >= 2) {
                    kartaGracz1 = pobierzKarte(&gracz[0]);
                    karty2[0] = pobierzKarte(&gracz[1]);
                    karty2[1] = pobierzKarte(&gracz[1]);
                    numerKartyWojna = rand()%2;
                    kartaGracz2 = karty2[numerKartyWojna];
                }
                else{
                    kartaGracz1 = pobierzKarte(&gracz[0]);
                    kartaGracz2 = pobierzKarte(&gracz[1]);
                }
            }
        }

        (*iloscRuchow)++;
        if(kartaGracz1.figura > kartaGracz2.figura){
            if(rand()%2 == 0){
                dodajKarte(&gracz[0], kartaGracz1);
                dodajKarte(&gracz[0], kartaGracz2);
            }
            else{
                dodajKarte(&gracz[0], kartaGracz2);
                dodajKarte(&gracz[0], kartaGracz1);
            }
            bylaWojna = FALSZ;
        }
        else if(kartaGracz1.figura < kartaGracz2.figura){
            if(rand()%2 == 0){
                dodajKarte(&gracz[1], kartaGracz2);
                dodajKarte(&gracz[1], kartaGracz1);
            }
            else{
                dodajKarte(&gracz[1], kartaGracz1);
                dodajKarte(&gracz[1], kartaGracz2);
            }

            bylaWojna = FALSZ;
        }
        else{
            bylaWojna = PRAWDA;
            iloscKartWojna = 2;
            kartyWojna[iloscKartWojna-2] = kartaGracz1;
            kartyWojna[iloscKartWojna-1] = kartaGracz2;

            switch(dwieKarty){
                case GRACZ1:
                    kartaGracz1 = karty1[!numerKartyWojna];
                    break;
                case GRACZ2:
                    kartaGracz2 = karty2[!numerKartyWojna];
            }

            wynikWojny = wojnaA(gracz, kartaGracz1, kartaGracz2, kartyWojna, &iloscKartWojna, iloscRuchow, dwieKarty);

            switch(wynikWojny){
                case 1:
                    return GRACZ1;
                case 2:
                    return GRACZ2;
            }
        }

        dwieKarty = dwieKarty == GRACZ1 ? GRACZ2 : GRACZ1; // kolej na zmianę osoby z dwiema kartami

        if(gracz[0].reka.ilosc_kart == 0 && (bylaWojna == PRAWDA || dwieKarty == GRACZ1)){
            return 2;
        }
        else if(gracz[1].reka.ilosc_kart == 0 && (bylaWojna == PRAWDA || dwieKarty == GRACZ2)){
            return 1;
        }
    }
}

int main() {
    Talia talia;
    Gracz gracz[ILOSC_GRACZY];
    int iloscRuchow = 0, wygrana = 0, wielkoscTalii = 52, avg = 0;
    int ileRuchowTablica[1000] = {0};
    gracz[0].reka.karty = (listaKart_t*) malloc(sizeof(listaKart_t));
    gracz[1].reka.karty = (listaKart_t*) malloc(sizeof(listaKart_t));
    srand(time(NULL));

        for(wielkoscTalii = 20; wielkoscTalii <= 52; wielkoscTalii +=4){
            for(int ek = 0; ek<1000; ek++){
                iloscRuchow = 0;
                for(int i=0; i<wielkoscTalii; i++){
                    talia.karty[i].kolor = i/(wielkoscTalii/4);
                    talia.karty[i].figura = i%(wielkoscTalii/4)+(52-wielkoscTalii)/4;
                }
                tasuj(&talia, wielkoscTalii);

                for(int i=0; i<ILOSC_GRACZY; i++){
                    memset(gracz[i].reka.karty,0,sizeof(listaKart_t));
                    gracz[i].reka.ilosc_kart = 0;
                }

                for(int i=0; i<wielkoscTalii; i++){
                    dodajKarte(&gracz[i/(wielkoscTalii/2)], talia.karty[i]); // każdy gracz ma połowę kart
                }
                
                wygrana = gra(gracz, &iloscRuchow, wielkoscTalii); // główna funkcja
                ileRuchowTablica[ek] = iloscRuchow;
            }

            avg = 0;
            for(int i=0; i<1000; i++){
                avg += ileRuchowTablica[i];
            }
            cout<<endl;
            avg = avg/1000;
            cout<<"Wielkosc talii: "<<wielkoscTalii<<" kart."<<endl;
            cout<<"Srednio: "<<avg<<" ruchów."<<endl;
        }
    return 0;
}