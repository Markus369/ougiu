/**
 *  Skjelett/grunnlag for oblig nr 4 i GrProg, hÃ¸sten 2021.
 *
 *  Det er mulig Ã¥:
 *     - legge inn nye produsenter
 *     - legge inn nytt produkt hos en produsent
 *     - skrive ALLE produsenter
 *     - skrive ALLE produkter hos/for EN produsent
 *     - skrive ALLE produkter
 *     - slette ALLE produsenter og deres produkter
 *
 *  @file    Oblig4.tpl
 *  @author  Martin Kristensen Eide,  Peter Behncke Nes  &  FrodeH
 */


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LesData.h"



#define MAXPRODUSENTER  50      ///< Max antall produsenter
#define MAXPRODUKTER    20      ///< Max antall produkter hos hver produsent
const int STRLEN  =    100;     ///< Max tekstlengde
const int MAXPRIS = 100000;     ///< Max pris


/**
 *  Produkt (med navn, beskrivelse, pris).
 */
struct Produkt {
    char  *navn, *beskrivelse;
    int   pris;
};


/**
 *  Produsent (med navn, by, antallprodukter, og array med produktene).
 */
struct Produsent {
    char   *navn, *by;
    int    antallProdukter;
    struct Produkt* produktene[MAXPRODUKTER];
};


struct Produkt* finnProdukt(const struct Produsent* produsent, const char* navn);
struct Produsent* finnProdusent(const char* navn);
void   fjernAllkokerteData();
void   leggTilEttProdukt();
void   nyProdusent();
void   produktLesData(struct Produkt* produkt, const char* navn);
void   produktSkrivData(const struct Produkt* produkt);
void   produktSlettData(struct Produkt* produkt);
void   produsentLesData(struct Produsent* produsent, const char* navn);
void   produsentNyttProdukt(struct Produsent* produsent, const char* navn);
void   produsentSkrivData(const struct Produsent* produsent);
void   produsentSlettData(struct Produsent* produsent);
void   skrivAbsoluttAlt();
void   skrivMeny();
void   hentFraFil();


struct Produsent* gProdusentene[MAXPRODUSENTER];    ///< Alle produsentene.
int gAntallProdusenter = 0;         ///< Antall produsenter registrert hittil.



/**
 *  Hovedprogrammet.
 */
int main() {
    char kommando;
    
    hentFraFil();
    
    skrivMeny();
    kommando = lesChar("\nKommando");

    while (kommando != 'Q')  {
        switch (kommando) {
            case 'N': nyProdusent();         break;
            case 'L': leggTilEttProdukt();   break;
            case 'A': skrivAbsoluttAlt();    break;
            default:  skrivMeny();           break;
        }
        kommando = lesChar("\nKommando");
    };

    fjernAllkokerteData();

return 0;
}


/**
 *  Returnerer (om mulig) peker til et navngitt produkt hos en produsent.
 *
 *  @param    produsent  - Produsenten for produktet
 *  @param    navn       - Navnet til produktet
 *  @return   Peker til produktet eller NULL
 */
struct Produkt* finnProdukt(const struct Produsent* produsent, const char* navn) {
    
    int temp = 0;
    
    if(produsent->antallProdukter > 0){
    
        for (temp = 0; temp < produsent->antallProdukter; temp++){
            if (strcmp(produsent->produktene[temp]->navn, navn) == 0)
                return (produsent->produktene[temp]);
        }
        return NULL;
    }else return NULL;
}


/**
 *  Returnerer (om mulig) peker til en navngitt produsent.
 *
 *  @param    navn   - Navnet til produsenten
 *  @return   Peker til produsenten eller NULL
 */
struct Produsent* finnProdusent(const char* navn) {
    
    int temp = 0;
    
    if(gAntallProdusenter > 0){
        
        for (temp = 0; temp < gAntallProdusenter; temp++){
        
            if (strcmp(gProdusentene[temp]->navn, navn) == 0)
                return (gProdusentene[temp]);
        }
                return NULL;
    
    }else return NULL;
}


/**
 *  Fjerner ALLE allokerte data fra minnet/memory.
 *
 *  @see   produsentSlettData(...)
 */
void fjernAllkokerteData() {
    
    int i = 0;
    int f = 0;
    
    for(i = 0; i < gAntallProdusenter; i++){
        
        for(f = 0; f < gProdusentene[i]->antallProdukter; f++){
        
            produktSlettData(gProdusentene[i]->produktene[f]);
        }
        
        produsentSlettData(gProdusentene[i]);
        
    }
    
    printf("All data har blitt fjernet\n\n");
    
}


/**
 *  Legger til (om mulig) ETT nytt produkt hos en produsent.
 *
 *  @see   finnProdusent(...)
 *  @see   finnProdukt(...)
 *  @see   produsentNyttProdukt(...)
 */
void leggTilEttProdukt()  {
    
    char* Navn;
    char* pro;
    
    Navn = lagOgLesText("Produsent du vil legge produkt til:\n");
    
    if (finnProdusent(Navn) != NULL){
        
        pro = lagOgLesText("Hva slags produkt vil du legge til?:\n");
        
        if (finnProdukt(finnProdusent(Navn), pro) == NULL){
            
            produsentNyttProdukt(finnProdusent(Navn), pro);
            
            
        }else printf("Du har allerede dette produktet");
        
        
        
    }else printf("Denne produsenten finnes ikke, eller så mangler du produsenter.\n");
    
    
    
}


/**
 *  Legger inn (om mulig) en ny produsent.
 *
 *  @see   finnProdusent(...)
 *  @see   produsentLesData(...)
 */
void nyProdusent()  {
    
    if(gAntallProdusenter != MAXPRODUSENTER){
    
        char* temp;
    
        printf("Skriv inn produsent navn:\n");
    
        gProdusentene[gAntallProdusenter] = (struct Produsent*) malloc(sizeof(struct Produsent));
    
        temp = lagOgLesText("Navn");
    
        if(!finnProdusent(temp)){
            printf("En produsent ved navnet %s er blitt dannet\n", temp);
        
            produsentLesData(gProdusentene[gAntallProdusenter], temp);
        
            gAntallProdusenter++;
        
        }else printf("Denne finnes allerede\n");
        
    }else printf("Du har nådd maksimalt antall produsenter.\n");
    
    
        
    

}


/**
 *  Leser inn ALLE dataene om ETT produkt.
 *
 *  @param   produkt  - Produktet som fÃ¥r sine data lest inn
 *  @param   navn     - Hittil innlest produktnavn
 */
void produktLesData(struct Produkt* produkt, const char* navn){
    
    
    //produkt = (struct Produkt*) malloc(sizeof(struct Produkt));
    
    produkt->navn = navn;
    
    
    produkt->beskrivelse = lagOgLesText("Beskrivelse av produkt");
    
    produkt->pris = lesInt("Pris på produktet", STRLEN, MAXPRIS);
    
}


/**
 *  Skriver ALLE et produkts data pÃ¥ skjermen.
 *
 *  @param   produkt  - Produktet hvis alle data skrives ut.
 */
void produktSkrivData(const struct Produkt* produkt) {
    
   
    printf("Produkt: -%s-\n\n", produkt->navn);
    printf("Produkt beskrivelse: %s\n\n", produkt->beskrivelse);
    printf("Produkt pris: %ikr\n\n", produkt->pris);
}


/**
 *  Sletter ALLE et produkts allokerte data fra minnet.
 *
 *  @param   produkt  - Produktet som fÃ¥r slettet sine allokerte data
 */
void produktSlettData(struct Produkt* produkt) {
    
    free(produkt->navn);
    free(produkt->beskrivelse);
}


/**
 *  Leser inn en produsents data, inkl initielle produkter.
 *
 *  @param   produsent  - Produsenten som det leses inn data til
 *  @param   navn       - Navnet til produsenten
 *  @see     finnProdukt(...)
 *  @see     produsentNyttProdukt(...)
 */
void produsentLesData(struct Produsent* produsent, const char* navn){
    
    char* produktNavn;
    char svar = 'j';
    
    produsent->antallProdukter = 0;
    
    produsent->navn = navn;
    
    printf("Hvilken by kommer produsenten fra?:");
    
    produsent->by = lagOgLesText("By:");
    
    
    
    
    
    do{
        
        produktNavn = lagOgLesText("Produktnavn:");
        
        if (finnProdukt(produsent, produktNavn) == NULL){
        
            
            produsentNyttProdukt(produsent, produktNavn);
        
            
        
            printf("Vil du legge til et til produkt? (j/n)");
            scanf(" %c", &svar);
            getchar();
            
    }else printf("Du har Et likt produkt allerede.");
    
    }while (svar != 'n');
    
}


/**
 *  Legger enn ETT nytt produkt hos en produsent.
 *
 *  @param   produsent  -  Produsenten Ã¥ legge inn nytt produkt hos
 *  @param   navn       -  Nytt produkts navn
 *  @see     produktLesData(...)
 */
void produsentNyttProdukt(struct Produsent* produsent, const char* navn) {
    
    produsent->produktene[produsent->antallProdukter] = (struct Produkt*) malloc(sizeof(struct Produkt));
    
    produktLesData(produsent->produktene[produsent->antallProdukter], navn);
    
    produsent->antallProdukter++;
}


/**
 *  Skriver ALLE en produsents data ut pÃ¥ skjermen.
 *
 *  @param    produsent  - Produsenten hvis alle data skrives ut.
 *  @see      produktSkrivData(...)
 */
void produsentSkrivData(const struct Produsent* produsent) {
    
    printf("Produsent -%s-:\n", produsent->navn);
    printf("By: -%s-\n", produsent->by);
    printf("Antall produkter: %i\n\n", produsent->antallProdukter);
}


/**
 *  Sletter ALLE en produsents allokerte data fra minnet.
 *
 *  @param   produsent  - Produsenten som fÃ¥r slettet sine allokerte data
 *  @see     produktSlettData(...)
 */
void produsentSlettData(struct Produsent* produsent) {
    
    int i = 0;
    
    free(produsent->navn);
    free(produsent->by);
    
    
    for(i = 0; i < produsent->antallProdukter; i++){
        free(produsent->produktene[i]);
         }
    
    free(produsent);
    
}


/**
 *  Skriver alt om alle produsenter.
 *
 *  @see   produsentSkrivData(...)
 */
void skrivAbsoluttAlt()  {
    
    int i = 0;
    int f = 0;
    
    if(gAntallProdusenter > 0){
        for (i = 0; i < gAntallProdusenter; i++){
            produsentSkrivData(gProdusentene[i]);
            
            for(f = 0; f < gProdusentene[i]->antallProdukter; f++){
                produktSkrivData(gProdusentene[i]->produktene[f]);
            }
        }
        
    }else printf("Du har ingen produsenter enda.\n");
        
    
}


/**
 *  Skriver alle kommandoene tilgjengelig i programmet.
 */
void skrivMeny() {
  printf("\nProgrammets kommandoer:");
  printf("\n\tN - Ny  produsent (og produkter)");
  printf("\n\tL - Legg til ett produkt hos en produsent");
  printf("\n\tA - skriv Alle produsenter og alle deres produkter");
  printf("\n\tQ - Quit/avslutt\n");
}


void   hentFraFil(){
    
    char temp[STRLEN];
    char temp2[STRLEN];
    char temp3[STRLEN];
    char temp4[STRLEN];
    
    FILE* innfil;
    
    int i = 0;
    
    int f = 0;
    
    innfil = fopen("DATA.txt", "r");
    
    if(innfil){
        printf("Fant fil....");
        
        fscanf(innfil," %i\n", &gAntallProdusenter);
        
            for(i = 0; i < gAntallProdusenter; i++){
                
                
                gProdusentene[i] = (struct Produsent*) malloc(sizeof(struct Produsent));
                
                fscanf(innfil," %s", temp);
                gProdusentene[i]->by = (char*) malloc(strlen(temp) * sizeof(char));
                strcpy(gProdusentene[i]->by, temp);
                
                printf(" %s", gProdusentene[i]->by);
                
                fscanf(innfil," %i", &gProdusentene[i]->antallProdukter);    //PRODUSENT SIN ANTALLPRODUKTER
                getc(innfil);
                
                printf("%i\n", gProdusentene[i]->antallProdukter);
                
                gProdusentene[i]->produktene[gProdusentene[i]->antallProdukter] = (struct Produkt*) malloc(sizeof(struct Produkt));
                
                    for(f = 0; f < gProdusentene[i]->antallProdukter; f++){  //KJØRER ANTALL PRODUKTER TIL PRODUSENT
                        
                        
                        fgets(temp2, STRLEN, innfil); temp2[strlen(temp2)-1] = '\0';
                        printf("%s", temp2);
                        gProdusentene[i]->produktene[f]->beskrivelse = (char*) malloc(strlen(temp2)+1 * sizeof(char));
                        strcpy(gProdusentene[i]->produktene[f]->beskrivelse, temp2);
                        
                        
                        fgets(temp3, STRLEN, innfil);
                        gProdusentene[i]->produktene[f]->navn = (char*) malloc(strlen(temp3) * sizeof(char));
                        strcpy(gProdusentene[i]->produktene[f]->navn, temp3);
                        
                        fscanf(innfil," %i", &gProdusentene[i]->produktene[f]->pris);   //LEGGER INN PRIS FOR PRODUKT NUMMER "f"
                        printf(" %i", gProdusentene[0]->produktene[0]->pris);
                        
                    }
                
                fgets(temp4, STRLEN, innfil);
                gProdusentene[i]->navn = (char*) malloc(strlen(temp4) * sizeof(char));
                strcpy(gProdusentene[i]->navn, temp4);                    //SKRIVER INN NAVN PÅ PRODUSENT
            }
        
        
    }
    fclose(innfil);
    printf("Lukket fil...");
    gAntallProdusenter++;
}
