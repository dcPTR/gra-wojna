#include <iostream>
#include <ctime>
#include <string.h>

#define ILOSC_KART 52
#define ILOSC_GRACZY 2
#define DEBUG 0
#define PRAWDA 1
#define FALSZ 0
#define GRACZ1 1
#define GRACZ2 2
#define BLAD 404
#define REMIS 99

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

int sprawdzKoniec(const Gracz gracz[]){
    if(gracz[0].reka.ilosc_kart <= 0){
        return 1;
    }
    if(gracz[1].reka.ilosc_kart <= 0){
        return 2;
    }
    return 0;
}

int wojnaA(Gracz gracz[], Karta kartaGracz1, Karta kartaGracz2, Karta kartyWojna[], int* iloscKartWojna, int* iloscRuchow){
    switch(sprawdzKoniec(gracz)){
        case 1:
            return 2; //wygra?? gracz 2
        case 2:
            return 1; //wygra?? gracz 1
    }


    *iloscKartWojna += 2; // dodaje te poni??ej
    kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]);
    kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]);
    (*iloscRuchow)++;

    switch(sprawdzKoniec(gracz)){
        case 1:
            return GRACZ2; //wygra?? gracz 2
        case 2:
            return GRACZ1; //wygra?? gracz 1
    }

    kartaGracz1 = pobierzKarte(&gracz[0]);
    kartaGracz2 = pobierzKarte(&gracz[1]);
    (*iloscRuchow)++;

    *iloscKartWojna += 2;
    kartyWojna[*iloscKartWojna-2] = kartaGracz1;
    kartyWojna[*iloscKartWojna-1] = kartaGracz2;

    if(kartaGracz1.figura > kartaGracz2.figura){
        for(int i=0; i<*iloscKartWojna; i+=2){ //swoje karty
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        for(int i=1; i<*iloscKartWojna; i+=2){ //karty przeciwnika
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        return 0;
    }
    else if(kartaGracz2.figura > kartaGracz1.figura){
        for(int i=1; i<*iloscKartWojna; i+=2){ //swoje karty
            dodajKarte(&gracz[1], kartyWojna[i]);
        }
        for(int i=0; i<*iloscKartWojna; i+=2){ //karty przeciwnika
            dodajKarte(&gracz[1], kartyWojna[i]);
        }
        return 0;
    }
    else{
        return wojnaA(gracz, kartaGracz1, kartaGracz2, kartyWojna, iloscKartWojna, iloscRuchow);
    }
}

int wojnaB(Gracz gracz[], Karta kartaGracz1, Karta kartaGracz2, Karta kartyWojna[], int* iloscKartWojna, int* iloscRuchow, int dobranoKarty = 0){
    int koniec;
    // ZACZYNALEM Z JEDN?? KART?? (TERAZ MAM 0)
    koniec = sprawdzKoniec(gracz);
    if(koniec){
        if(dobranoKarty == FALSZ){
            switch(koniec){
                case 1: // graczowi numer 1 sko??czy??y si?? karty
                    *iloscKartWojna += 2;
                    kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]); // najpierw na STOS przegranego (GRACZA 1)
                    if(gracz[1].reka.ilosc_kart == 0) return GRACZ2;
                    kartaGracz1 = pobierzKarte(&gracz[1]);
                    (*iloscRuchow)++;
                    if(gracz[1].reka.ilosc_kart == 0) return GRACZ2;
                    kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[1]); // POTEM NA SW??J (GRACZA 2)
                    if(gracz[1].reka.ilosc_kart == 0) return GRACZ2;
                    kartaGracz2 = pobierzKarte(&gracz[1]);
                    (*iloscRuchow)++;
                    if(gracz[1].reka.ilosc_kart == 0) return GRACZ2;
                    *iloscKartWojna += 2;
                    kartyWojna[*iloscKartWojna-2] = kartaGracz1;
                    kartyWojna[*iloscKartWojna-1] = kartaGracz2;
                    break;
                case 2: // graczowi numer 2 sko??czy??y si?? karty
                    *iloscKartWojna += 2;
                    kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]); // najpierw na STOS przegranego (GRACZA 2)
                    if(gracz[0].reka.ilosc_kart == 0) return GRACZ1;
                    kartaGracz2 = pobierzKarte(&gracz[0]);
                    (*iloscRuchow)++;
                    if(gracz[0].reka.ilosc_kart == 0) return GRACZ1;
                    kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[0]); // POTEM NA SW??J (GRACZA 1)
                    if(gracz[0].reka.ilosc_kart == 0) return GRACZ1;
                    kartaGracz1 = pobierzKarte(&gracz[0]);
                    (*iloscRuchow)++;
                    if(gracz[0].reka.ilosc_kart == 0) return GRACZ1;
                    *iloscKartWojna += 2;
                    kartyWojna[*iloscKartWojna-2] = kartaGracz1;
                    kartyWojna[*iloscKartWojna-1] = kartaGracz2;
                    break;
            }
            dobranoKarty = PRAWDA;
        }
        else{
            switch(koniec){
                case 1:
                    return GRACZ2; //wygra?? gracz 2
                case 2:
                    return GRACZ1; //wygra?? gracz 1
            }
        }
    }
    else if(gracz[0].reka.ilosc_kart+1 == 2 || gracz[1].reka.ilosc_kart+1 == 2){
        // KA??DY WYK??ADA ZAKRYTE KARTY
        *iloscKartWojna += 2;
        kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]);
        kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]);
        (*iloscRuchow)++;
        if(dobranoKarty == FALSZ){ // NIE DOBRANO JESZCZE KART PRZECIWNIKA
            if(gracz[0].reka.ilosc_kart == 0){ // NIE MA JU?? KART
                if(gracz[1].reka.ilosc_kart == 0) return REMIS; //REMIS
                kartaGracz1 = pobierzKarte(&gracz[1]);
                if(gracz[1].reka.ilosc_kart == 0) return 2;
                kartaGracz2 = pobierzKarte(&gracz[1]);
                (*iloscRuchow)++;
                if(gracz[1].reka.ilosc_kart == 0) return 2;
            }
            else if(gracz[1].reka.ilosc_kart == 0){ // NIE MA JU?? KART
                if(gracz[0].reka.ilosc_kart == 0) return REMIS; //REMIS
                kartaGracz2 = pobierzKarte(&gracz[0]);
                if(gracz[0].reka.ilosc_kart == 0) return 1;
                kartaGracz1 = pobierzKarte(&gracz[0]);
                (*iloscRuchow)++;
                if(gracz[0].reka.ilosc_kart == 0) return 1;
            }
            *iloscKartWojna += 2;
            kartyWojna[*iloscKartWojna-2] = kartaGracz1;
            kartyWojna[*iloscKartWojna-1] = kartaGracz2;
            dobranoKarty = PRAWDA;
        }
        else{
            if(gracz[0].reka.ilosc_kart == 0){
                return GRACZ2; // WYGRYWA DRUGI GRACZ
            }
            if(gracz[0].reka.ilosc_kart == 1){
                return GRACZ1; // WYGRYWA PIERWSZY GRACZ
            }
        }
    }
    else{ // GRAMY NORMALNIE
        *iloscKartWojna += 2;
        kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]);
        kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]);
        (*iloscRuchow)++;

        kartaGracz1 = pobierzKarte(&gracz[0]);
        kartaGracz2 = pobierzKarte(&gracz[1]);
        (*iloscRuchow)++;
        *iloscKartWojna += 2;

        kartyWojna[*iloscKartWojna-2] = kartaGracz1;
        kartyWojna[*iloscKartWojna-1] = kartaGracz2;
    }

    if(kartaGracz1.figura > kartaGracz2.figura){
        for(int i=0; i<*iloscKartWojna; i+=2){ //swoje karty
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        for(int i=1; i<*iloscKartWojna; i+=2){ //karty przeciwnika
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        return FALSZ; //nie ma ko??ca gry
    }
    else if(kartaGracz2.figura > kartaGracz1.figura){
        for(int i=1; i<*iloscKartWojna; i+=2){ //swoje karty
            dodajKarte(&gracz[1], kartyWojna[i]);
        }
        for(int i=0; i<*iloscKartWojna; i+=2){ //karty przeciwnika
            dodajKarte(&gracz[1], kartyWojna[i]);
        }
        return FALSZ; //nie ma ko??ca gry
    }
    else{
        return wojnaB(gracz, kartaGracz1, kartaGracz2, kartyWojna, iloscKartWojna, iloscRuchow, dobranoKarty);
    }
}

int gra(Gracz gracz[], int* iloscRuchow, int wielkoscTalii, char rodzajWojny){
    Karta kartaGracz1, kartaGracz2;
    int iloscKartWojna = 0, wynikWojny = 0;

    Karta* kartyWojna = (Karta*) malloc(wielkoscTalii*sizeof(Karta));

    while(true){
        kartaGracz1 = pobierzKarte(&gracz[0]);
        kartaGracz2 = pobierzKarte(&gracz[1]);
        (*iloscRuchow)++;

        if(kartaGracz1.figura > kartaGracz2.figura){
            dodajKarte(&gracz[0], kartaGracz1);
            dodajKarte(&gracz[0], kartaGracz2);
        }
        else if(kartaGracz1.figura < kartaGracz2.figura){
            dodajKarte(&gracz[1], kartaGracz2);
            dodajKarte(&gracz[1], kartaGracz1);
        }
        else{
            iloscKartWojna = 2;
            kartyWojna[iloscKartWojna-2] = kartaGracz1;
            kartyWojna[iloscKartWojna-1] = kartaGracz2;

            switch(rodzajWojny){
                case 'A':
                    wynikWojny = wojnaA(gracz, kartaGracz1, kartaGracz2, kartyWojna, &iloscKartWojna, iloscRuchow);
                    break;
                case 'B':
                    wynikWojny = wojnaB(gracz, kartaGracz1, kartaGracz2, kartyWojna, &iloscKartWojna, iloscRuchow);
                    break;
                default:
                    cout<<"Podaj poprawny wariant wojny."<<endl;
                    return BLAD;
            }
            switch(wynikWojny){
                case 1:
                    return GRACZ1;
                case 2:
                    return GRACZ2;
            }
        }
        ;
        if(gracz[0].reka.ilosc_kart == 0){
            return 2;
        }
        else if(gracz[1].reka.ilosc_kart == 0){
            return 1;
        }
    }
}

int main() {
    Talia talia;
    char kolor[4][6] = {"pik","kier","karo", "trefl"};
    char figura[13][6] = {"2","3","4","5","6","7","8","9","10","walet","dama","krol","as"};
    Gracz gracz[ILOSC_GRACZY];
    int iloscRuchow = 0, wygrana = 0;
    int ileRuchowTablica[1000] = {0};
    double avg;
    char rodzajWojny = 'B';

    gracz[0].reka.karty = (listaKart_t*) malloc(sizeof(listaKart_t));
    gracz[1].reka.karty = (listaKart_t*) malloc(sizeof(listaKart_t));
    srand(time(NULL));

    for(int wielkoscTalii = 20; wielkoscTalii <= 52; wielkoscTalii += 4){
        for(int ek=0; ek<1000; ek++){
            iloscRuchow = 0;
            for(int i=0; i<wielkoscTalii; i++) {
                talia.karty[i].kolor = i / (wielkoscTalii / 4);
                talia.karty[i].figura = i % (wielkoscTalii / 4) + (52 - wielkoscTalii) / 4;
            }
            tasuj(&talia, wielkoscTalii);

            //TESTOWANIE
            /*
    int t1[ILOSC_KART] = {2, 2, 0, 0, 2, 9, 3, 5, 2, 8, 2, 5, 2, 10, 3, 6, 0, 3, 3, 2, 1, 10, 2, 1, 1, 7, 0, 6, 3, 12, 1, 11, 0, 2, 1, 0, 3, 11, 0, 1, 0, 10, 2, 6, 0, 9, 1, 6, 2, 4, 2, 0};
    int t2[ILOSC_KART] = {0, 7, 3, 0, 3, 8, 2, 7, 2, 12, 1, 1, 3, 7, 3, 3, 3, 10, 1, 5, 0, 5, 1, 2, 2, 11, 0, 8, 1, 4, 1, 3, 1, 12, 0, 11, 3, 9, 3, 1, 0, 12, 1, 9, 3, 4, 2, 3, 1, 8, 0, 4};

    for(int i=0; i<26; i++){
        talia.karty[i].kolor = t1[i*2];
        talia.karty[i].figura = t1[i*2+1];
    }
    for(int i=26; i<52; i++){
        talia.karty[i].kolor = t2[(i-26)*2];
        talia.karty[i].figura = t2[(i-26)*2+1];
    }*/
            //WY??EJ

            for(int i=0; i<ILOSC_GRACZY; i++){

                memset(gracz[i].reka.karty,0,sizeof(listaKart_t));
                gracz[i].reka.ilosc_kart = 0;
            }


            for(int i=0; i<wielkoscTalii; i++){
                dodajKarte(&gracz[i/(wielkoscTalii/2)], talia.karty[i]); // ka??dy gracz ma po??ow?? kart
            }

            wygrana = gra(gracz, &iloscRuchow, wielkoscTalii, rodzajWojny); // g????wna funkcja

            if(wygrana == GRACZ1){
                if(DEBUG) cout<<endl<<"Koniec gry. Wygra?? gracz 1."<<endl;
            }
            else if(wygrana == GRACZ2){
                if(DEBUG)cout<<endl<<"Koniec gry. Wygra?? gracz 2."<<endl;
            }
            else if(wygrana == REMIS){
                cout<<endl<<"REMIS"<<endl;
            }
            else{
                cout<<"BLAD"<<endl;
            }

            ileRuchowTablica[ek] = iloscRuchow;
        }

        avg = 0;
        for(int i=0; i<1000; i++){
            avg += ileRuchowTablica[i];
        }
        cout<<endl;
        avg = avg/1000;
        cout<<"Wielkosc talii: "<<wielkoscTalii<<" kart."<<endl;
        cout<<"Srednio: "<<avg<<" ruch??w."<<endl;
    }

    //    WY??WIETL KARTY
    //    for(int i=0; i<ILOSC_KART; i++){
    //        cout<<kolor[talia.karty[i].kolor]<<" "<<figura[talia.karty[i].figura]<<endl;
    //    }

    return 0;
}