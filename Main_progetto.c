#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "lib_progetto.h"

int main () {

    t_immagine img_1,img_2;
    char rix[3],percorso[PATH_DIM],cartella_nuove_immagini[PATH_DIM];
    int sel,n;
    struct stringa* testa;

    img_1.caratteristiche_img.area_racchiusa.soglia_sfondo=SOGLIA_SFONDO;
    img_2.caratteristiche_img.area_racchiusa.soglia_sfondo=SOGLIA_SFONDO;
    img_1.caratteristiche_img.area_racchiusa.soglia_high=SOGLIA_H;
    img_2.caratteristiche_img.area_racchiusa.soglia_high=SOGLIA_H;
    img_1.caratteristiche_img.area_racchiusa.soglia_low=SOGLIA_L;
    img_2.caratteristiche_img.area_racchiusa.soglia_low=SOGLIA_L;

    printf("\t\t\t\tProgramma Avviato\n\n\n\n");
    strcpy(percorso,"C:\\Users\\Matteo De Silvestri\\Desktop\\Registro_pazienti");
    printf("Cartella di lavoro attuale:\n\n%s\n\nSi desidera cambiare cartella di lavoro?  ",percorso);
    scanf("%s",rix);
    getchar();

    if (analizza_risposta(rix)) {

        printf("\nDigitare percorso cartella di lavoro:  ");
        n=leggi_stringa(&testa);
        trasforma_stringa(testa,percorso,n);
        printf("\n\n\t\tCartella di lavoro modificata");

    }

    if(access(percorso,F_OK)!=0)

        mkdir(percorso);

    conta_pazienti(percorso);
    reset_display_1(percorso);
    reset_display_2(percorso);
    system("cls");
    printf("\t\t\t\tProgramma Avviato\n\n\n\n");
    strcpy(cartella_nuove_immagini,"C:\\Users\\Matteo De Silvestri\\Desktop\\Deposito_immagini");
    printf("\n\nL'attuale percorso per il caricamento di nuove immagini e':\n\n%s\n\nSi desidera cambiare cartella?  ",cartella_nuove_immagini);
    getchar();
    scanf("%s",rix);
    getchar();

    if (analizza_risposta(rix)) {

        printf("\nDigitare percorso cartella caricamento immagini:  ");
        n=leggi_stringa(&testa);
        trasforma_stringa(testa,cartella_nuove_immagini,n);
        printf("\n\n\t\tCartella modificata");

    }

    if(access(cartella_nuove_immagini,F_OK)!=0)

        mkdir(cartella_nuove_immagini);

    controlla_nuove_immagini(cartella_nuove_immagini,percorso);

    do {

        system("cls");
        printf("\t\t\t\tProgramma Avviato\n\n\n\n");
        sel=mostra_menu();
        system("cls");
        printf("\t\t\t\tProgramma Avviato\n\n\n\n");
        esegui_scelta(sel,percorso,&img_1,&img_2);
        printf("\n\nSi desidera continuare?  ");
        scanf("%s",rix);
        reset_display_1(percorso);
        reset_display_2(percorso);

    } while(analizza_risposta(rix));

    return 0;

}
