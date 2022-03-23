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
#define REMIS 99
#define BLAD 404

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
    else{ //do poprawy
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

void zakryte(){
    cout<<"X: zakryte Y: zakryte"<<endl;
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
    cout<<"Wojna"<<endl;
    switch(sprawdzKoniec(gracz)){
        case 1:
            return GRACZ2; //wygrał gracz 2
        case 2:
            return GRACZ1; //wygrał gracz 1
    }

    *iloscKartWojna += 2;
    kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]);
    kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]);
    (*iloscRuchow)++;
    zakryte();

    switch(sprawdzKoniec(gracz)){
        case 1:
            return GRACZ2; //wygrał gracz 2
        case 2:
            return GRACZ1; //wygrał gracz 1
    }

    kartaGracz1 = pobierzKarte(&gracz[0]);
    kartaGracz2 = pobierzKarte(&gracz[1]);
    (*iloscRuchow)++;
    wyswietlNazwyKart(kartaGracz1, kartaGracz2);

    *iloscKartWojna += 2;
    kartyWojna[*iloscKartWojna-2] = kartaGracz1;
    kartyWojna[*iloscKartWojna-1] = kartaGracz2;

    if(kartaGracz1.figura > kartaGracz2.figura){
        cout<<"Gracz 1 wygrywa wojne"<<endl;
        for(int i=0; i<*iloscKartWojna; i+=2){ //swoje karty
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        for(int i=1; i<*iloscKartWojna; i+=2){ //karty przeciwnika
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        return 0;
    }
    else if(kartaGracz2.figura > kartaGracz1.figura){
        cout<<"Gracz 2 wygrywa wojne"<<endl;
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
    cout<<"Wojna"<<endl;

    // ZACZYNALEM Z JEDNĄ KARTĘ (TERAZ MAM 0)
    koniec = sprawdzKoniec(gracz);
    if(koniec){
        if(dobranoKarty == FALSZ){
            switch(koniec){
                case 1: // graczowi numer 1 skończyły się karty
                    cout<<"Gracz 2 kładzie dwie karty na stosie Gracza 1"<<endl;
                    *iloscKartWojna += 2;
                    kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]); // najpierw na STOS przegranego (GRACZA 1)
                    if(gracz[1].reka.ilosc_kart == 0) return 2;
                    kartaGracz1 = pobierzKarte(&gracz[1]);
                    (*iloscRuchow)++;
                    cout<<"Gracz 2 kładzie dwie karty na swoim stosie"<<endl;
                    if(gracz[1].reka.ilosc_kart == 0) return 2;
                    kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[1]); // POTEM NA SWÓJ (GRACZA 2)
                    if(gracz[1].reka.ilosc_kart == 0) return 2;
                    kartaGracz2 = pobierzKarte(&gracz[1]);
                    (*iloscRuchow)++;
                    if(gracz[1].reka.ilosc_kart == 0) return 2;
                    zakryte();
                    wyswietlNazwyKart(kartaGracz1, kartaGracz2);
                    *iloscKartWojna += 2;
                    kartyWojna[*iloscKartWojna-2] = kartaGracz1;
                    kartyWojna[*iloscKartWojna-1] = kartaGracz2;
                    break;
                case 2: // graczowi numer 2 skończyły się karty
                    cout<<"Gracz 1 kładzie dwie karty na stosie Gracza 2"<<endl;
                    *iloscKartWojna += 2;
                    kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]); // najpierw na STOS przegranego (GRACZA 2)
                    if(gracz[0].reka.ilosc_kart == 0) return 1;
                    kartaGracz2 = pobierzKarte(&gracz[0]);
                    (*iloscRuchow)++;
                    cout<<"Gracz 1 kładzie dwie karty na swoim stosie"<<endl;
                    if(gracz[0].reka.ilosc_kart == 0) return 1;
                    kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[0]); // POTEM NA SWÓJ (GRACZA 1)
                    if(gracz[0].reka.ilosc_kart == 0) return 1;
                    kartaGracz1 = pobierzKarte(&gracz[0]);
                    (*iloscRuchow)++;
                    if(gracz[0].reka.ilosc_kart == 0) return 1;
                    zakryte();
                    wyswietlNazwyKart(kartaGracz1, kartaGracz2);
                    *iloscKartWojna += 2;
                    kartyWojna[*iloscKartWojna-2] = kartaGracz1;
                    kartyWojna[*iloscKartWojna-1] = kartaGracz2;
                    break;
            }
            dobranoKarty = 1;
        }
        else{
            switch(koniec){
                case 1:
                    return GRACZ2; //wygrał gracz 2
                case 2:
                    return GRACZ1; //wygrał gracz 1
            }
        }
    }
    else if(gracz[0].reka.ilosc_kart+1 == 2 || gracz[1].reka.ilosc_kart+1 == 2){
        // KAŻDY WYKŁADA ZAKRYTE KARTY
        *iloscKartWojna += 2;
        kartyWojna[*iloscKartWojna-2] = pobierzKarte(&gracz[0]);
        kartyWojna[*iloscKartWojna-1] = pobierzKarte(&gracz[1]);
        (*iloscRuchow)++;
        zakryte();
        if(dobranoKarty == FALSZ){ // NIE DOBRANO JESZCZE KART PRZECIWNIKA
            if(DEBUG) cout<<"dalej"<<endl;
            if(gracz[0].reka.ilosc_kart == 0){ // NIE MA JUŻ KART
                cout<<"Gracz 2 dokłada kartę na stos Gracz 1"<<endl;
                if(DEBUG) cout<<"Gracz[0] nie ma już kart!!!!!"<<endl;
                if(gracz[1].reka.ilosc_kart == 0) return REMIS; //REMIS
                kartaGracz1 = pobierzKarte(&gracz[1]);
                if(gracz[1].reka.ilosc_kart == 0) return GRACZ2;
                kartaGracz2 = pobierzKarte(&gracz[1]);
                (*iloscRuchow)++;
                wyswietlNazwyKart(kartaGracz1, kartaGracz2);
                if(gracz[1].reka.ilosc_kart == 0) return GRACZ2;
            }
            else if(gracz[1].reka.ilosc_kart == 0){ // NIE MA JUŻ KART
                if(DEBUG) cout<<"Gracz[1]] nie ma już kart!!!!!"<<endl;
                cout<<"Gracz 2 dokłada kartę na stos Gracz 1"<<endl;
                if(gracz[0].reka.ilosc_kart == 0) return REMIS; //REMIS
                kartaGracz2 = pobierzKarte(&gracz[0]);
                if(gracz[0].reka.ilosc_kart == 0) return GRACZ1;
                kartaGracz1 = pobierzKarte(&gracz[0]);
                (*iloscRuchow)++;
                wyswietlNazwyKart(kartaGracz1, kartaGracz2);
                if(gracz[0].reka.ilosc_kart == 0) return GRACZ1;
            }
            *iloscKartWojna += 2;
            kartyWojna[*iloscKartWojna-2] = kartaGracz1;
            kartyWojna[*iloscKartWojna-1] = kartaGracz2;
            wyswietlNazwyKart(kartaGracz1, kartaGracz2);
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
        zakryte();

        kartaGracz1 = pobierzKarte(&gracz[0]);
        kartaGracz2 = pobierzKarte(&gracz[1]);
        (*iloscRuchow)++;
        wyswietlNazwyKart(kartaGracz1, kartaGracz2);
        *iloscKartWojna += 2;

        kartyWojna[*iloscKartWojna-2] = kartaGracz1;
        kartyWojna[*iloscKartWojna-1] = kartaGracz2;
    }

    if(kartaGracz1.figura > kartaGracz2.figura){
        cout<<"Gracz 1 wygrywa wojne"<<endl;
        for(int i=0; i<*iloscKartWojna; i+=2){ //swoje karty
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        for(int i=1; i<*iloscKartWojna; i+=2){ //karty przeciwnika
            dodajKarte(&gracz[0], kartyWojna[i]);
        }
        return FALSZ; //KONIEC WOJNY, ale nie koniec gry
    }
    else if(kartaGracz2.figura > kartaGracz1.figura){
        cout<<"Gracz 2 wygrywa wojne"<<endl;
        for(int i=1; i<*iloscKartWojna; i+=2){ //swoje karty
            dodajKarte(&gracz[1], kartyWojna[i]);
        }
        for(int i=0; i<*iloscKartWojna; i+=2){ //karty przeciwnika
            dodajKarte(&gracz[1], kartyWojna[i]);
        }
        return FALSZ; //KONIEC WOJNY, ale nie koniec gry
    }
    else{
        return wojnaB(gracz, kartaGracz1, kartaGracz2, kartyWojna, iloscKartWojna, iloscRuchow, dobranoKarty);
    }
}

int gra(Gracz gracz[], int* iloscRuchow, int wielkoscTalii, char rodzajWojny){
    Karta kartaGracz1, kartaGracz2;
    int iloscKartWojna = 0, wynikWojny = 0;

    Karta* kartyWojna = (Karta*) malloc(wielkoscTalii*sizeof(Karta));

    while(PRAWDA){
        kartaGracz1 = pobierzKarte(&gracz[0]);
        kartaGracz2 = pobierzKarte(&gracz[1]);
        (*iloscRuchow)++;
        wyswietlNazwyKart(kartaGracz1, kartaGracz2);

        if(kartaGracz1.figura > kartaGracz2.figura){
            dodajKarte(&gracz[0], kartaGracz1);
            dodajKarte(&gracz[0], kartaGracz2);
        }
        else if(kartaGracz1.figura < kartaGracz2.figura){
            dodajKarte(&gracz[1], kartaGracz2);
            dodajKarte(&gracz[1], kartaGracz1);
        }
        else{
            if(DEBUG) cout<<"WOJNA"<<endl;
            iloscKartWojna = 2; //nie trzeba czyścić kartyWojny
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

        if(gracz[0].reka.ilosc_kart == 0){
            return GRACZ2;
        }
        else if(gracz[1].reka.ilosc_kart == 0){
            return GRACZ1;
        }
    }
}

void menu(int* wielkoscTalii, char* rodzajWojny){
    cout<<"*******GRA*******"<<endl;
    while(PRAWDA) {
        cout<<"Podaj wielkosc talii: "<<endl;
        cin>>*wielkoscTalii;
        if( ((*wielkoscTalii % 4) == 0) && *wielkoscTalii >= 4 && *wielkoscTalii <= 52) break;
        cout<<"Podaj liczbe w zakresie od 4 do 52, ktora jest podzielna przez 4."<<endl;
    }
    while(PRAWDA) {
        cout<<"Podaj wariant wojny: "<<endl;
        cin>>*rodzajWojny;
        if(*rodzajWojny == 'A' || *rodzajWojny == 'B') break;
        cout<<"Możliwe warianty A lub B..."<<endl;
    }
}

int main() {
    Talia talia;
    char kolor[4][6] = {"pik","kier","karo", "trefl"};
    char figura[13][6] = {"2","3","4","5","6","7","8","9","10","walet","dama","krol","as"};
    Gracz gracz[ILOSC_GRACZY];
    int iloscRuchow = 0, wygrana = 0, wielkoscTalii = 52;
    char rodzajWojny = 'B';

    gracz[0].reka.karty = (listaKart_t*) malloc(sizeof(listaKart_t));
    gracz[1].reka.karty = (listaKart_t*) malloc(sizeof(listaKart_t));
    srand(time(NULL));

    menu(&wielkoscTalii, &rodzajWojny);

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

    // WYSWIETL KARTY W GRZE
    /*
    for(int i=0; i<wielkoscTalii; i++){
        cout<<kolor[talia.karty[i].kolor]<<"-"<<figura[talia.karty[i].figura]<<", ";
    }
    cout<<endl;*/
    wygrana = gra(gracz, &iloscRuchow, wielkoscTalii, rodzajWojny); // główna funkcja

    if(wygrana == GRACZ1){
        cout<<endl<<"Koniec gry. Wygrał gracz 1."<<endl;
    }
    else if(wygrana == GRACZ2){
        cout<<endl<<"Koniec gry. Wygrał gracz 2."<<endl;
    }
    else if(wygrana == REMIS){
        cout<<endl<<"REMIS"<<endl;
    }
    else{
        cout<<"BLAD"<<endl;
    }
    return 0;
}