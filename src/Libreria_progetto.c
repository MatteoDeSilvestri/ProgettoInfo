#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "lib_progetto.h"

/** Funzione per caricare un'immagine BMP da file
Parametri:
	nome del file in lettura, puntatore alla struttura in cui memorizzare i dati dell'immagine
Valore di ritorno:
	0 se il caricamento è avvenuto con successo, un numero diverso da 0 altrimenti
*/
int loadBMP(char *filename, BMP_Image *image)
{

	FILE *fp;

	fp = fopen(filename, "rb");
	if(fp == NULL)
	{
		printf("Error: impossibile aprire il file in lettura\n");
		return 1;
	}

	fread(image->magic, sizeof(image->magic), 1, fp );

	if(image->magic[0]!='B' || image->magic[1]!='M')
	{
		printf("Error: tipo di immagine non corretto\n");
		return 2;
	}

	fread(&image->header, sizeof(image->header), 1, fp);
	fread(&image->info, sizeof(image->info), 1, fp);

	if(image->info.bits!=8)
	{
		printf("Error: numero di bits/pixel diverso da 8\n");
		return 3;
	}

	if(image->info.width!=DATA_DIM || image->info.height!=DATA_DIM)
	{
		printf("--- Attenzione, dimensioni non corrette ---");
	}

	fread(&image->color_table, sizeof(image->color_table), 1, fp);
	fread(image->data, sizeof(image->data), 1, fp);

	fclose(fp);
	return 0;

}

/** Funzione per salvare una struttura BMP_Image su file
Parametri:
	puntatore alla struttura da cui leggere i dati dell'immagine, nome del file su cui scrivere
Valore di ritorno:
	0 se il salvataggio è avvenuto con successo, 1 altrimenti
*/
int saveBMP(BMP_Image image, char * filename){

	FILE *fp2;
	fp2 = fopen(filename, "wb");

	if(fp2==NULL)
	{
		printf("Impossibile aprire il file in scrittura\n");
		return 1;
	}

	fwrite(&image.magic, sizeof(image.magic), 1, fp2);
	fwrite(&image.header, sizeof(image.header), 1, fp2);
	fwrite(&image.info, sizeof(image.info), 1, fp2);
	fwrite(&image.color_table, sizeof(image.color_table), 1, fp2);
	fwrite(image.data, sizeof(image.data), 1, fp2);

	fclose(fp2);
	return 0;

}

//dato l'indirizzo del puntatore al primo elemento legge tutti i caratteri che trova fino all'invio
//e restituisce il numero di elementi salvati nella stringa
int leggi_stringa (struct stringa** testa) {

    int i=0,k=1;
    char tmp;
    struct stringa* previous_element,*new_element;
    *testa=malloc(sizeof (struct stringa));
    (*testa)->previous_char=NULL;
    fscanf (stdin,"%c",&((*testa)->carattere));
    previous_element=*testa;
    i++;

    do {

        fscanf (stdin,"%c",&tmp);

        if (tmp!=10) {

            new_element=malloc (sizeof (struct stringa));
            new_element->previous_char=previous_element;
            new_element->carattere=tmp;
            previous_element=new_element;
            i++;

        }

        else k=0;

    } while (k);

    if (i>1) (*testa)->previous_char=new_element;

    else (*testa)->previous_char=(*testa);

    return i;
}


//dati il primo elemento e l'indirizzo (previous_char) in esso contenuto staampa a video l'intera stringa (compresi gli spazi)
void stampa_stringa (struct stringa** testa, struct stringa** coda) {

    if (*coda!=*testa) stampa_stringa (testa,&((*coda)->previous_char));

    printf ("%c",(*coda)->carattere);

}


//dato il puntatore alla posizione da valutare e l'indirizzo del puntatore al primo elemento,
//restituisce 1 se l'elemento nella posizione è l'ultimo della stringa, 0 altrimenti
int fine_stringa (struct stringa* pos,struct stringa** testa) {

    int fine=0;

    if ((*testa)->previous_char==pos) fine =1;

    return fine;

}

//dato il puntatore al primo elemento, la lunghezza dela stringa ed un array di char,
//trscrive la stringa dentro l'array
void trasforma_stringa (struct stringa* testa,char* nuova_stringa,int lunghezza_stringa) {

    if (lunghezza_stringa-1)

        trasforma_stringa (testa->previous_char,nuova_stringa,lunghezza_stringa-1);

    *(nuova_stringa+lunghezza_stringa-1)=testa->previous_char->carattere;
    *(nuova_stringa+lunghezza_stringa)='\0';

}



//Ricevuto un puntatore ad una struttura immagine calcola numero di sfumature di grigio
//e occorrenza di ogni singola sfumatura (riempie i rispettivi campi della struttura)
void livelli_di_grigio (t_immagine* image) {

    int i,j;

    if (&(image->img)!=NULL) {

        for (i=0;i<DATA_DIM;i++)

            for (j=0;j<DATA_DIM;j++){

                if (!(image->caratteristiche_img.distrib_livelli_grigio[image->img.data[i][j].grey])) image->caratteristiche_img.n_livelli_grigio++;

                image->caratteristiche_img.distrib_livelli_grigio[image->img.data[i][j].grey]++;

            }

    }

}

//Ricevuto un puntatore ad una struttura immagine riduce il numero di livelli di grigio ad un massimo di 10
void riduci_livelli (t_immagine* image) {

    int i,j,m,M;

    if (&(image->img)!=NULL) {

        m=min(&(image->img));
        M=max(&(image->img));

        for (i=0;i<DATA_DIM;i++)

            for (j=0;j<DATA_DIM;j++)

                if (image->img.data[i][j].grey) image->img.data[i][j].grey=approx(image->img.data[i][j].grey,m,M);

    }

}

//Ricevuta un'immagine bitmap restituisce il valore di grigio minimo presente
int min (BMP_Image* img) {

    int min,i,j;
    min=img->data[0][0].grey;

    for (i=0;i<DATA_DIM;i++)

        for (j=0;j<DATA_DIM;j++)

            if (img->data[i][j].grey<min) min=img->data[i][j].grey;

    return min;
}

//Ricevuta un'immagine bitmap restituisce il valore di grigio massimo presente
int max (BMP_Image* img) {

    int max=0,i,j;

    for (i=0;i<DATA_DIM;i++)

        for (j=0;j<DATA_DIM;j++)

            if (img->data[i][j].grey>max) max=img->data[i][j].grey;

    return max;
}


//Ricevuti i valori minimo e massimo di un'immagine ed il valore da vole approssimare, restituisce l'approssimazione
//del valore su 10 livelli
unsigned char approx (unsigned char valore,int min,int max) {

    int r=(max-min)/9;

    while (min<valore) {

        min=min+r;

    }

    if (min-valore<=r/2) return min;

    else return min-r;

}

//Ricevuta una struttura immagine identifica le coordinate dei pixel di contorno
void rileva_contorno (t_immagine* foto1) {

    int i,j,z=0;

    for (i=0;i<DATA_DIM;i++)

        for (j=0;j<DATA_DIM;j++)

            if (controlla_matrice_piccola(foto1,i,j))

                scrittura_pixel (foto1,i,j,&z);

}

//ricevuti un puntatore ad una struttura immagine e le coordinate di un punto stabilisce se quel punto è o meno parte del contorno
int controlla_matrice_piccola (t_immagine* foto, int x, int y) {

    int a=0;

    if (foto->img.data[x][y].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo||foto->img.data[x+1][y].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo||foto->img.data[x][y+1].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo||foto->img.data[x+1][y+1].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo)
        a=1;

    if (foto->img.data[x][y].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo&&foto->img.data[x+1][y].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo&&foto->img.data[x][y+1].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo&&foto->img.data[x+1][y+1].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo)
        a=0;

    return a;
}

//Riempie un vettore con le coordinate dei pixel di contorno
void scrittura_pixel (t_immagine* foto,int x,int y,int* pos) {

    if (foto->img.data[x][y].grey<=foto->caratteristiche_img.area_racchiusa.soglia_sfondo) {

        foto->caratteristiche_img.pixel_contorno[*pos].i=x;
        foto->caratteristiche_img.pixel_contorno[*pos].j=y;
        (*pos)++;

    }

    if (foto->img.data[x+1][y].grey<=foto->caratteristiche_img.area_racchiusa.soglia_sfondo) {

        foto->caratteristiche_img.pixel_contorno[*pos].i=x+1;
        foto->caratteristiche_img.pixel_contorno[*pos].j=y;
        (*pos)++;

    }

    if (foto->img.data[x][y+1].grey<=foto->caratteristiche_img.area_racchiusa.soglia_sfondo) {

        foto->caratteristiche_img.pixel_contorno[*pos].i=x;
        foto->caratteristiche_img.pixel_contorno[*pos].j=y+1;
        (*pos)++;

    }

    if (foto->img.data[x+1][y+1].grey<=foto->caratteristiche_img.area_racchiusa.soglia_sfondo) {

        foto->caratteristiche_img.pixel_contorno[*pos].i=x+1;
        foto->caratteristiche_img.pixel_contorno[*pos].j=y+1;
        (*pos)++;

    }

}

//Calcola l'area contenuta nel contorno in pixel
void calcolo_area_pixel (t_immagine* foto) {

    int i,j;
    foto->caratteristiche_img.area_racchiusa.area_pixel=0;

    for (i=0;i<DATA_DIM;i++)

        for (j=0;j<DATA_DIM;j++)

            if (foto->img.data[i][j].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo)

                foto->caratteristiche_img.area_racchiusa.area_pixel++;

}



//Converte i pixel in mm^2
float conversione_area_mm2 (int area) {

    return area*2.5*2.5;

}

//Calcola l'area molto scura e l'area molto chiara in pixel (soglie definite nella struct t_immagine)
void calcolo_area_chiara_scura_pixel (t_immagine* foto) {

    int i,j;
    foto->caratteristiche_img.area_racchiusa.area_chiara_pixel=0;
    foto->caratteristiche_img.area_racchiusa.area_scura_pixel=0;

    for (i=0;i<DATA_DIM;i++)

        for (j=0;j<DATA_DIM;j++)

            if (foto->caratteristiche_img.area_racchiusa.soglia_sfondo<foto->caratteristiche_img.area_racchiusa.soglia_low) {

                if (foto->img.data[i][j].grey>foto->caratteristiche_img.area_racchiusa.soglia_high)

                    foto->caratteristiche_img.area_racchiusa.area_chiara_pixel++;

                else if (foto->img.data[i][j].grey<foto->caratteristiche_img.area_racchiusa.soglia_low&&foto->img.data[i][j].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo)

                    foto->caratteristiche_img.area_racchiusa.area_scura_pixel++;

            }

            else if (foto->caratteristiche_img.area_racchiusa.soglia_sfondo>foto->caratteristiche_img.area_racchiusa.soglia_high) {

                if (foto->img.data[i][j].grey>foto->caratteristiche_img.area_racchiusa.soglia_high&&foto->img.data[i][j].grey<foto->caratteristiche_img.area_racchiusa.soglia_sfondo)

                    foto->caratteristiche_img.area_racchiusa.area_chiara_pixel++;

                else if (foto->img.data[i][j].grey<foto->caratteristiche_img.area_racchiusa.soglia_low)

                    foto->caratteristiche_img.area_racchiusa.area_scura_pixel++;

            }
            else {

                if (foto->img.data[i][j].grey>foto->caratteristiche_img.area_racchiusa.soglia_sfondo)

                    foto->caratteristiche_img.area_racchiusa.area_chiara_pixel++;

            }

}

//Confronta area scura e chiara di due immagini a scelta, purché appartenenti allo stesso paziente e ocnsecutive, oppure relative
//alla stessa zona anatomica ma appartenenti a pazienti diversi
void confronta_aree (t_immagine* img_1,t_immagine* img_2) {

    if (!((strcmp(img_1->paziente.codice_paziente,img_2->paziente.codice_paziente)==0&&strcmp(img_1->codice_img.codice_zona,img_2->codice_img.codice_zona)==0&&(img_1->codice_img.n_layer[2]-img_2->codice_img.n_layer[2]==1||img_1->codice_img.n_layer[2]-img_2->codice_img.n_layer[2]==-1))||(strcmp(img_1->paziente.codice_paziente,img_2->paziente.codice_paziente)&&strcmp(img_1->codice_img.codice_zona,img_2->codice_img.codice_zona)==0&&strcmp(img_1->codice_img.n_layer,img_2->codice_img.n_layer)==0)))

        printf("\n\nerrore: le immagini devono appartenere allo stesso paziente ed essere consecutive oppure se appartenenti a pazienti diversi devono riferirsi alla stessa zona\n\n");

    else {

        printf("\n\nDifferenza area scura: %d px\n\nDifferenza area chiara: %d px",img_1->caratteristiche_img.area_racchiusa.area_scura_pixel-img_2->caratteristiche_img.area_racchiusa.area_scura_pixel,img_1->caratteristiche_img.area_racchiusa.area_chiara_pixel-img_2->caratteristiche_img.area_racchiusa.area_chiara_pixel);

    }

}


//Salvare l'immagine nella cartella di backup e nel registro pazienti, rispettivamente con nome originale e nome d'utilizzo
void salva_dati_immagine(char percorso_immagini[],char percorso_cartella[]) {

    BMP_Image image;
    char tmp[DATA_DIM];
    DIR* cartella;
    struct dirent* nome;
    cartella=opendir(percorso_immagini);

    if (cartella!=NULL) {

        seekdir(cartella,2);
        nome=readdir(cartella);

        while (nome!=NULL) {

            strcpy(tmp,percorso_immagini);
            strcat(tmp,"\\");
            strcat(tmp,nome->d_name);
            loadBMP(tmp,&image);
            remove(tmp);
            tmp[strlen(tmp)-strlen(nome->d_name)]='\0';
            strcat(tmp,"..\\Backup_immagini");
            mkdir(tmp);
            strcat(tmp,"\\");
            strcat(tmp,nome->d_name);
            //tmp[strlen(tmp)-(strlen(nome->d_name)-CODICE_DIM)]='\0';
            printf("\n%s\n",tmp);
            saveBMP(image,tmp);
            scrivi_dati_immagine(tmp);
            sposta_file_immagine(tmp,image,percorso_cartella);
            nome=readdir(cartella);

        }

    }

    else printf ("\n\n   *** Percorso errato ***\n\n");

}

//Aggiunge in fondo ad un'immagine BMP i dati relativi all'immagine stessa
void scrivi_dati_immagine (char filename []) {

    int gg,mm,aa,n_liv;
    char zona[CHAR_DIM],codicefiscale[CODICE_DIM+1];
    FILE* f;
    f=fopen(filename,"a");

    if (f!=NULL) {

        printf("Inserire il codice fiscale del paziente\n\n");
        scanf("%s",codicefiscale);
        fprintf(f,"*-%s-*",codicefiscale);
        printf("Inserire la data in cui e' stata effettuata la tac (gg/mm/aaaa): ");
        scanf("%d/%d/%d",&gg,&mm,&aa);
        fprintf(f,"*-%d/%d/%d-*",gg,mm,aa);
        printf("Inserire la zona a cui e' stata effettuata la tac: ");
        scanf("%s",zona);
        fprintf(f,"*-%s-*",zona);
        printf("Inserire il livello dell'immagine: ");
        scanf("%d",&n_liv);
        fprintf(f,"*-%d-*",n_liv);
        fclose(f);

    }

}

//Sposta l'immagine nella cartella di lavoro, aggiungendo il nome di  utilizzo
void sposta_file_immagine (char filename [],BMP_Image img,char percorso_cartella[]) {

    FILE* f1;
    FILE* f2;
    char stringa[50];
    char codicefiscale[CODICE_DIM+1];
    char nome[30];
    char percorso[PATH_DIM];
    f1=fopen(filename,"r");

    if (f1!=NULL) {

        fseek(f1,sizeof(BMP_Image),SEEK_SET);
        fscanf(f1,"%s",stringa);
        strncpy(codicefiscale,stringa,16);
        codicefiscale[16]='\0';
        fclose(f1);
        strcpy(percorso,percorso_cartella);
        strcat(percorso,"\\");
        strcat(percorso,codicefiscale);

        if(access(percorso,F_OK))

            mkdir(percorso);

        strcat(percorso,"\\");
        strcat(percorso,"TAC");

        if(access(percorso,F_OK))

            mkdir(percorso);

        printf("Inserire il nome di salvataggio della TAC\n Utilizzare il formato:gg-mm-aaaa.zona.livello\n");
        scanf("%s",nome);
        strcpy(percorso,percorso_cartella);
        strcat(percorso,"\\");
        strcat(percorso,codicefiscale);
        strcat(percorso,"\\TAC\\");
        strcat(percorso,nome);
        strcat(percorso,".bmp");

        f2=fopen(percorso, "w");

        if(f2!=NULL) {

            saveBMP(img,percorso);
            fclose(f2);

        }

        else printf("\n\n\t\terror f2");
        fclose(f1);

    }

    else printf("\n\n\t\terror f1");

}

//Mostra a scehrmo e stampa su file.doc la storicizzazione di un paziente
void storicizzazione(char codice_fiscale[],char percorso_lavoro[]) {

    DIR* cartella;
    struct dirent* nome;
    FILE* f1;
    char percorso[60];
    char nomefile[60];
    char stringa[11];
    char zona[7];
    char stringa1[20]="NULL";
    char livello[4];
    int i=0;
    int cont=0;


    strcpy(percorso,percorso_lavoro);
    strcat(percorso,"\\");
    strcat(percorso,codice_fiscale);
    strcat(percorso,"\\");
    strcat(percorso,"TAC");
    cartella=opendir(percorso);
    strcpy(nomefile,percorso_lavoro);
    strcat(nomefile,"\\");
    strcat(nomefile,codice_fiscale);
    strcat(nomefile,"\\");
    strcat(nomefile,codice_fiscale);
    strcat(nomefile,"-storicizzazione.doc");
    f1=fopen(nomefile,"w+");

    if (f1!=NULL) {

        printf("Si e' deciso di effettuare una storicizzazione dei dati del paziente con codice fiscale: %s\n",codice_fiscale);
        fprintf(f1,"\t\t\t\t\tStoria clinica del paziente\n\n\n");

        if (cartella!=NULL) {

            seekdir(cartella,2);
            nome=readdir(cartella);//12-09-2014.femore1123.bmp

            while (nome!=NULL) {

                strncpy(stringa,nome->d_name,10);
                stringa[10]='\0';
                printf("data %s\n",stringa);
                fprintf(f1,"Il giorno %s il paziente si e' sottoposto a TAC",stringa);
                strcpy(zona,nome->d_name+11);

                for(i=0;i<strlen(zona);i++)

                {

                    if(zona[i]!='.') cont++;

                    else break;

                }

                strncpy(stringa1,zona,cont);
                stringa1[cont]='\0';
                printf("zona %s\n",stringa1);
                fprintf(f1,", zona: %s",stringa1);
                strncpy(livello,nome->d_name+12+cont,3);

                if(livello[2]=='.')

                    livello[2]='\0';

                printf("livello %s\n",livello);
                printf("*******************************\n");
                livello[strlen(livello)+1]='\0';
                fprintf(f1,", livello: %s\n\n\n",livello);
                cont=0;
                nome=readdir(cartella);

            }

        }

        fclose(f1);

    }

}

//Controlla l'esistenza di un paziente, restituisce 1 e il paziente esiste, 0 altrimenti
int esistenza_codicefiscale(char stringa[],char x[]){

        FILE* f;
        char stringa1[200];

        strcpy(stringa1,x);
        strcat(stringa1,"\\");
        strcat(stringa1,stringa);
        strcat(stringa1,"\\");
        strcat(stringa1,stringa);
        strcat(stringa1,"-anagrafica.doc");
        f=fopen(stringa1,"r");

        if(f!=NULL) return 1;

        else return 0;

}



//Stampa su file.doc i dati relativi all'anagrafica di uno o più pazienti, dentro la cartella di lavoro
void Anagrafica(char percorso[]) {

        FILE* f1;
        int i=1;
        char rix[3];
        char stringa[20];
        int giorno_numero;
        int mese;
        int anno;
        char codicefiscale[16];
        char stringa1[200];

        if(access(percorso,F_OK)!=0) //funzione che crea la cartella solo se non è gia esistente

            mkdir(percorso);

        do {

            printf("Inserire codice fiscale\n");
            scanf("%s",codicefiscale);

            if(esistenza_codicefiscale(codicefiscale,percorso)) {

                printf("Il paziente e' gia' schedato\n");
                break;

            }

            else {

                printf("Nuovo paziente: INSERIRE ANAGRAFICA\n\n");

                strcpy(stringa1,percorso);
                strcat(stringa1,"\\");
                strcat(stringa1,codicefiscale);
                mkdir(stringa1);
                strcpy(stringa1,percorso);
                strcat(stringa1,"\\");
                strcat(stringa1,codicefiscale);
                strcat(stringa1,"\\");
                strcat(stringa1,codicefiscale);
                strcat(stringa1,"-anagrafica.doc");
                f1=fopen(stringa1,"wb+");
                fprintf(f1, "\t\t\tRegistro Pazienti- Reparto Radiologia\n\n\n");
                fprintf(f1, " Paziente %d\n\n",i);
                printf("Inserire le stringhe di testo con _ al posto dello spazio\n\n\n");
                fprintf(f1,"Codice fiscale: %s\n\n",codicefiscale);
                printf("Inserire nome paziente %d\n",i);
                scanf("%s",stringa);
                fprintf(f1,"Nome: %s\n\n",stringa);
                printf("Inserire cognome paziente: %d\n",i);
                scanf("%s",stringa);
                fprintf(f1,"Cognome: %s\n\n", stringa);
                printf("Inserire sesso paziente: %d\n",i);
                scanf(" %s",stringa);
                fprintf(f1,"Sesso: %c\n\n",stringa[0]);
                printf("Inserire la data di nascita del paziente %d\n",i);
                scanf("%d/%d/%d",&giorno_numero,&mese,&anno);
                fprintf(f1,"Data di nascita: %d/%d/%d\n\n",giorno_numero,mese,anno);
                printf("Inserire luogo di nascita paziente %d\n",i);
                scanf("%s",stringa);
                fprintf(f1,"Luogo di nascita: %s\n\n",stringa);
                printf("Inserire dati sulla residenza paziente %d\n\n",i);
                scanf("Via: %s",stringa);
                fprintf(f1,"\t\t\t\t\tDati sulla residenza\n\n");
                printf("Via: ");
                scanf("%s",stringa);
                fprintf(f1,"Via: %s\n\n",stringa);
                printf("Numero civico: ");
                scanf("%d",&giorno_numero);
                fprintf(f1,"Numero civico: %d\n\n",giorno_numero);
                printf("Città: ");
                scanf("%s",stringa);
                fprintf(f1,"Citta': %s\n\n",stringa);
                printf("CAP: ");
                scanf("%d",&giorno_numero);
                fprintf(f1,"CAP: %d\n\n",giorno_numero);
                printf("Prov: ");
                scanf("%s",stringa);
                fprintf(f1,"Prov: %s\n\n",stringa);
                fprintf(f1,"*****************************************************\n\n");
                fclose(f1);
                printf("Si vogliono inserire altri pazienti?");
                scanf("%s",rix);
                i++;

            }

        } while (analizza_risposta(rix));

}

//Stampare a video il menù di selezione
int mostra_menu(void) {

    int rix;

    printf("1 - Creare nuovi pazienti");
    printf("\n\n2 - Visualizzare la storicizzazione di un paziente");
    printf("\n\n3 - Calcolare il numero di livelli di grigio di un'immagine");
    printf("\n\n4 - Calcolare la distribuzione dei livelli di grigio di un'immagine");
    printf("\n\n5 - Ridurre i livelli di grigio di un'immagine");
    printf("\n\n6 - Identificare il contorno della parte informativa dell'immagine");
    printf("\n\n7 - Calcolare l'area racchiusa nel contorno");
    printf("\n\n8 - Calcolare area scura e chiara");
    printf("\n\n9 - Calcolare la variazione d'area tra due immagini");
    printf("\n\n10 - Risalire al proprietario di un'immagine");
    printf("\n\n\nDigitare il numero dell'opzione d'interesse:  ");
    scanf("%d",&rix);

    return rix;

}

//Valutare se una risposta è da intendere come si o come no (il "si" è più probabile)
int analizza_risposta(char rix[]) {

    int risp=1;

    if (rix[0]=='N'||rix[0]=='n'||rix[0]=='0'||rix[1]=='O'||rix[1]=='o'||rix[1]=='0') risp=0;

    return risp;

}

//Stabuilire il numero di pazienti centenuti nella cartella di lavoro (percorso[])
void conta_pazienti (char percorso[]) {

    DIR* cartella;
    struct dirent* nome;
    int i=0,p;
    cartella=opendir(percorso);

    if (cartella!=NULL){

        seekdir(cartella,2);
        nome=readdir(cartella);

        while (nome!=NULL) {

            if ((nome->d_name[strlen(nome->d_name)-4])!='.') i++;

            nome=readdir(cartella);

        }

        printf("\n\nCartella analizzata: un totale di %d pazienti presenti",i);

    }

    else printf ("\n\n\t\terrore: percorso non valido");

    printf("\n\n\nPremere un qualunque tasto per continuare..  ");
    scanf("%d",&p);

}

//Caricare su Display.bmp un'immagine
void visualizza_display_1(t_immagine* img,char percorso_cartella[]) {

    FILE* f;
    char tmp[DATA_DIM];

    strcpy(tmp,percorso_cartella);
    strcat(tmp,"\\Display.bmp");
    f=fopen(tmp,"wb");

    if (f!=NULL) {

        fwrite(&img->img,sizeof(BMP_Image),1,f);
        fclose(f);

    }

    else printf("\n\nerrore: percorso non valido");

}

//Resettare il Display
void reset_display_1(char percorso_cartella[]) {

    int i,j;
    char tmp[DATA_DIM];
    BMP_Image img;

    strcpy(tmp,percorso_cartella);
    strcat(tmp,"\\Display.bmp");
    loadBMP(tmp,&img);

    for (i=0;i<DATA_DIM;i++)

        for (j=0;j<DATA_DIM;j++)

            img.data[i][j].grey=255;

    saveBMP(img,tmp);

}

//Caricare su Display 2.bmp un'immagine
void visualizza_display_2(t_immagine* img,char percorso_cartella[]) {

    FILE* f;
    char tmp[DATA_DIM];

    strcpy(tmp,percorso_cartella);
    strcat(tmp,"\\Display 2.bmp");
    f=fopen(tmp,"wb");

    if (f!=NULL) {

        fwrite(&img->img,sizeof(BMP_Image),1,f);
        fclose(f);

    }

    else printf("\n\nerrore: percorso non valido");

}

//Resettare il Display 2
void reset_display_2(char percorso_cartella[]) {

    int i,j;
    char tmp[DATA_DIM];
    BMP_Image img;

    strcpy(tmp,percorso_cartella);
    strcat(tmp,"\\Display 2.bmp");
    loadBMP(tmp,&img);

    for (i=0;i<DATA_DIM;i++)

        for (j=0;j<DATA_DIM;j++)

            img.data[i][j].grey=255;

    saveBMP(img,tmp);

}

//Verificare la presenza di immagini dentro la cartella di deposito(percorso_immagini[])
//  da caricare in backup e nella cartella di lavoro (percorso_cartella[])
void controlla_nuove_immagini (char percorso_immagini[],char percorso_cartella[]) {

    DIR* cartella;
    struct dirent* nome;
    char percorso[90],rix[3];

    strcpy(percorso,percorso_immagini);
    cartella=opendir(percorso);

    if (cartella!=NULL) {

        seekdir(cartella,2);
        nome=readdir(cartella);

        if (nome!=NULL) {

            printf("\n\nCi sono immagini non caricate nella cartella delle nuove immagini..\n\nSi desidera caricarle?  ");
            scanf("%s",rix);

            if (analizza_risposta(rix)) salva_dati_immagine(percorso_immagini,percorso_cartella);

        }

        else printf("\n\nerrore: percorso non valido\n\n");

    }

    else printf("\n\nerrore: percorso non valido\n\n");

}


//Eseguire l'azione scelta dal menù
void esegui_scelta (int selezione,char percorso_cartella[],t_immagine* img_1,t_immagine* img_2) {

    char tmp[DATA_DIM],paziente[CODICE_DIM+1],nome_immagine[PATH_DIM],risposta[3];
    FILE* f1;
    int i,j,cont=0;

    printf("\nSelezione: %d",selezione);

    if (selezione==1) {

        printf(" - Creare nuovi pazienti\n\n");
        Anagrafica(percorso_cartella);

    }

    else if (selezione==2) {

        printf(" - Visualizzare la storicizzazione di un paziente");
        printf("\n\nDigitare codice fiscale del paziente: ");
        scanf("%s",paziente);
        storicizzazione(paziente,percorso_cartella);

    }

    else if (selezione==3) {

        printf(" - Calcolare il numero di livelli di grigio di un'immagine");
        printf("\n\nDigitare codice fiscale del paziente a cui appartiene l'immagine: ");
        scanf("%s",paziente);
        printf("\n\nDigitare nome dell'immagine (data(gg-mm-aaaa).zona.livello): ");
        scanf("%s",nome_immagine);
        strcpy(tmp,percorso_cartella);
        strcat(tmp,"\\");
        strcat(tmp,paziente);
        strcat(tmp,"\\TAC\\");
        strcat(tmp,nome_immagine);

        if (tmp[strlen(tmp)-3]!='.') strcat(tmp,".bmp");

        f1=fopen(tmp,"rb");

        if (f1!=NULL) {

            fread(&img_1->img,sizeof(BMP_Image),1,f1);
            fclose(f1);
            visualizza_display_1(img_1,percorso_cartella);
            img_1->caratteristiche_img.n_livelli_grigio=0;
            for(i=0;i<DATA_DIM;i++)

                img_1->caratteristiche_img.distrib_livelli_grigio[i]=0;

            livelli_di_grigio(img_1);
            printf("\n\nUn totale di %d differenti livelli di grigio",img_1->caratteristiche_img.n_livelli_grigio);

        }

        else printf("\n\nerrore: percorso non valido");

    }

    else if (selezione==4) {

        printf(" - Calcolare la distribuzione dei livelli di grigio di un'immagine");
        printf("\n\nDigitare codice fiscale del paziente a cui appartiene l'immagine: ");
        scanf("%s",paziente);
        printf("\n\nDigitare nome dell'immagine (data(gg-mm-aaaa).zona.livello): ");
        scanf("%s",nome_immagine);
        strcpy(tmp,percorso_cartella);
        strcat(tmp,"\\");
        strcat(tmp,paziente);
        strcat(tmp,"\\TAC\\");
        strcat(tmp,nome_immagine);

        if (tmp[strlen(tmp)-3]!='.') strcat(tmp,".bmp");

        f1=fopen(tmp,"rb");
        fread(&(img_1->img),sizeof(BMP_Image),1,f1);
        fclose(f1);
        visualizza_display_1(img_1,percorso_cartella);
        for(i=0;i<DATA_DIM;i++)

                img_1->caratteristiche_img.distrib_livelli_grigio[i]=0;

        livelli_di_grigio(img_1);
        printf("\n\nDistribuzione dei livelli di grigio:\n");

        for (i=0;i<DATA_DIM;i++)

            printf("\n%d:\t%d occorrenze;",i,img_1->caratteristiche_img.distrib_livelli_grigio[i]);

    }

    else if (selezione==5) {

        printf(" - Ridurre i livelli di grigio di un'immagine");
        printf("\n\nDigitare codice fiscale del paziente a cui appartiene l'immagine: ");
        scanf("%s",paziente);
        printf("\n\nDigitare nome dell'immagine (data(gg-mm-aaaa).zona.livello): ");
        scanf("%s",nome_immagine);
        strcpy(tmp,percorso_cartella);
        strcat(tmp,"\\");
        strcat(tmp,paziente);
        strcat(tmp,"\\TAC\\");
        strcat(tmp,nome_immagine);

        if (tmp[strlen(tmp)-3]!='.') strcat(tmp,".bmp");

        f1=fopen(tmp,"rb");

        if (f1!=NULL) {

            rewind(f1);
            fread(&(img_1->img),sizeof(BMP_Image),1,f1);
            fclose(f1);
            visualizza_display_1(img_1,percorso_cartella);
            riduci_livelli(img_1);
            visualizza_display_2(img_1,percorso_cartella);
            printf("Mantenere una copia della versione modificata? ");
            scanf("%s",risposta);

            if (analizza_risposta(risposta)) {

                tmp[strlen(tmp)-3]='\0';
                strcat(tmp,"_ridotta.bmp");
                f1=fopen(tmp,"wb");
                fwrite(img_1,sizeof(BMP_Image),1,f1);
                fclose(f1);

            }

        }

        else printf("\n\nerrore:percorso o nome file non validi\n\n");

    }

    else if (selezione==6) {

        printf(" - Identificare il contorno della parte informativa dell'immagine");
        printf("Attualmente lo sfondo viene consderato con valore: %d\n\nSi desidera modificarla? ",img_1->caratteristiche_img.area_racchiusa.soglia_sfondo);
        scanf("%s",risposta);

        if (analizza_risposta(risposta)) {

            printf("\n\nDigitare nuovo valore sfondo: ");
            scanf("%d",&img_1->caratteristiche_img.area_racchiusa.soglia_sfondo);
            img_2->caratteristiche_img.area_racchiusa.soglia_sfondo=img_1->caratteristiche_img.area_racchiusa.soglia_sfondo;

        }

        printf("\n\nDigitare codice fiscale del paziente a cui appartiene l'immagine: ");
        scanf("%s",paziente);
        printf("\n\nDigitare nome dell'immagine (data(gg-mm-aaaa).zona.livello): ");
        scanf("%s",nome_immagine);
        strcpy(tmp,percorso_cartella);
        strcat(tmp,"\\");
        strcat(tmp,paziente);
        strcat(tmp,"\\TAC\\");
        strcat(tmp,nome_immagine);

        if (tmp[strlen(tmp)-3]!='.') strcat(tmp,".bmp");

        f1=fopen(tmp,"rb");

        if (f1!=NULL) {

            fread(&(img_1->img),sizeof(BMP_Image),1,f1);
            fclose(f1);
            visualizza_display_1(img_1,percorso_cartella);

            for(i=0;i<DATA_DIM;i++) {

                img_1->caratteristiche_img.pixel_contorno[i].i=0;
                img_1->caratteristiche_img.pixel_contorno[i].j=0;

            }

            rileva_contorno(img_1);
            i=0;

            for(i=0;i<DATA_DIM;i++)

                for(j=0;j<DATA_DIM;j++)

                    img_1->img.data[i][j].grey=255;

            while (img_1->caratteristiche_img.pixel_contorno[i].i&&img_1->caratteristiche_img.pixel_contorno[i].j) {

                if (img_1->caratteristiche_img.pixel_contorno[i].i>0&&img_1->caratteristiche_img.pixel_contorno[i].j>0)

                    img_1->img.data[img_1->caratteristiche_img.pixel_contorno[i].i][img_1->caratteristiche_img.pixel_contorno[i].j].grey=0;

                i++;

            }

            visualizza_display_2(img_1,percorso_cartella);
            printf("Mantenere una copia della versione modificata? ");
            scanf("%s",risposta);

            if (analizza_risposta(risposta)) {

                tmp[strlen(tmp)-3]='\0';                            //bisogna creare l'immagine contorno!!
                strcat(tmp,"_contorno.bmp");
                f1=fopen(tmp,"wb");
                fwrite(img_1,sizeof(BMP_Image),1,f1);
                fclose(f1);

            }

        }

        else printf("\n\nerrore:percorso o nome file non validi\n\n");

    }

    else if (selezione==7) {

        printf(" - Calcolare l'area racchiusa nel contorno");
        printf("Attualmente lo sfondo viene consderato con valore: %d\n\nSi desidera modificarla? ",img_1->caratteristiche_img.area_racchiusa.soglia_sfondo);
        scanf("%s",risposta);

        if (analizza_risposta(risposta)) {

            printf("\n\nDigitare nuovo valore sfondo: ");
            scanf("%d",&img_1->caratteristiche_img.area_racchiusa.soglia_sfondo);
            img_2->caratteristiche_img.area_racchiusa.soglia_sfondo=img_1->caratteristiche_img.area_racchiusa.soglia_sfondo;

        }

        printf("\n\nDigitare codice fiscale del paziente a cui appartiene l'immagine: ");
        scanf("%s",paziente);
        printf("\n\nDigitare nome dell'immagine (data(gg-mm-aaaa).zona.livello): ");
        scanf("%s",nome_immagine);
        strcpy(tmp,percorso_cartella);
        strcat(tmp,"\\");
        strcat(tmp,paziente);
        strcat(tmp,"\\TAC\\");
        strcat(tmp,nome_immagine);

        if (tmp[strlen(tmp)-3]!='.') strcat(tmp,".bmp");

        f1=fopen(tmp,"rb");

        if (f1!=NULL) {

            fread(&(img_1->img),sizeof(BMP_Image),1,f1);
            fclose(f1);
            visualizza_display_1(img_1,percorso_cartella);
            calcolo_area_pixel(img_1);
            printf("\n\nArea racchiusa nel contorno: %d px\t\tovvero: %f mm^2",img_1->caratteristiche_img.area_racchiusa.area_pixel,conversione_area_mm2(img_1->caratteristiche_img.area_racchiusa.area_pixel));

        }

        else printf("\n\nerrore:percorso o nome file non validi\n\n");

    }

    else if (selezione==8) {

        printf(" - Calcolare area scura e chiara");
        printf("Attualmente la soglia chiara ha valore: %d\nLa soglia scura ha valore: %d\n\nSi desidera modificarle? ",img_1->caratteristiche_img.area_racchiusa.soglia_high,img_1->caratteristiche_img.area_racchiusa.soglia_low);
        scanf("%s",risposta);

        if (analizza_risposta(risposta)) {

            printf("\n\nDigitare nuovo valore soglia chiara: ");
            scanf("%d",&img_1->caratteristiche_img.area_racchiusa.soglia_high);
            img_2->caratteristiche_img.area_racchiusa.soglia_high=img_1->caratteristiche_img.area_racchiusa.soglia_high;
            printf("\n\nDigitare nuovo valore soglia scura: ");
            scanf("%d",&img_1->caratteristiche_img.area_racchiusa.soglia_low);
            img_2->caratteristiche_img.area_racchiusa.soglia_low=img_1->caratteristiche_img.area_racchiusa.soglia_low;

        }

        printf("\n\nDigitare codice fiscale del paziente a cui appartiene l'immagine: ");
        scanf("%s",paziente);
        printf("\n\nDigitare nome dell'immagine (data(gg-mm-aaaa).zona.livello): ");
        scanf("%s",nome_immagine);
        strcpy(tmp,percorso_cartella);
        strcat(tmp,"\\");
        strcat(tmp,paziente);
        strcat(tmp,"\\TAC\\");
        strcat(tmp,nome_immagine);

        if (tmp[strlen(tmp)-3]!='.') strcat(tmp,".bmp");

        f1=fopen(tmp,"rb");

        if (f1!=NULL) {

            fread(&(img_1->img),sizeof(BMP_Image),1,f1);
            fclose(f1);
            visualizza_display_1(img_1,percorso_cartella);
            calcolo_area_chiara_scura_pixel(img_1);
            printf("\n\nArea chiara racchiusa nel contorno: %d px\t\tovvero: %f mm^2",img_1->caratteristiche_img.area_racchiusa.area_chiara_pixel,conversione_area_mm2(img_1->caratteristiche_img.area_racchiusa.area_chiara_pixel));
            printf("\n\nArea scura racchiusa nel contorno: %d px\t\tovvero: %f mm^2",img_1->caratteristiche_img.area_racchiusa.area_pixel,conversione_area_mm2(img_1->caratteristiche_img.area_racchiusa.area_scura_pixel));

        }

        else printf("\n\nerrore:percorso o nome file non validi\n\n");

    }

    else if (selezione==9) {

        printf(" - Calcolare la variazione d'area tra due immagini");
        printf("Attualmente la soglia chiara ha valore: %d\nLa soglia scura ha valore: %d\n\nSi desidera modificarle? ",img_1->caratteristiche_img.area_racchiusa.soglia_high,img_1->caratteristiche_img.area_racchiusa.soglia_low);
        scanf("%s",risposta);

        if (analizza_risposta(risposta)) {

            printf("\n\nDigitare nuovo valore soglia chiara: ");
            scanf("%d",&img_1->caratteristiche_img.area_racchiusa.soglia_high);
            img_2->caratteristiche_img.area_racchiusa.soglia_high=img_1->caratteristiche_img.area_racchiusa.soglia_high;
            printf("\n\nDigitare nuovo valore soglia scura: ");
            scanf("%d",&img_1->caratteristiche_img.area_racchiusa.soglia_low);
            img_2->caratteristiche_img.area_racchiusa.soglia_low=img_1->caratteristiche_img.area_racchiusa.soglia_low;

        }

        printf("\n\nDigitare codice fiscale del paziente a cui appartiene la prima immagine: ");
        scanf("%s",paziente);
        printf("\n\nDigitare nome della prima immagine (data(gg-mm-aaaa).zona.livello): ");
        scanf("%s",nome_immagine);
        strcpy(tmp,percorso_cartella);
        strcat(tmp,"\\");
        strcat(tmp,paziente);
        strcat(tmp,"\\TAC\\");
        strcat(tmp,nome_immagine);

        if (tmp[strlen(tmp)-3]!='.') strcat(tmp,".bmp");

        f1=fopen(tmp,"rb");

        if (f1!=NULL) {

            fread(&(img_1->img),sizeof(BMP_Image),1,f1);
            fclose(f1);

        }

        strcpy(img_1->paziente.codice_paziente,paziente);
        img_1->paziente.codice_paziente[strlen(img_1->paziente.codice_paziente)]='\0';
        strcpy(img_1->codice_img.codice_zona,nome_immagine+11);
        cont=0;

        for(i=0;i<strlen(img_1->codice_img.codice_zona);i++) {

            if(img_1->codice_img.codice_zona[i]!='.') cont++;

            else break;

        }

        img_1->codice_img.codice_zona[cont]='\0';
        strncpy(img_1->codice_img.n_layer,nome_immagine+12+cont,3);
        img_1->codice_img.n_layer[strlen(img_1->codice_img.n_layer)]='\0';
        printf("\n\nDigitare codice fiscale del paziente a cui appartiene la seconda immagine: ");
        scanf("%s",paziente);
        printf("\n\nDigitare nome della seconda immagine (data(gg-mm-aaaa).zona.livello): ");
        scanf("%s",nome_immagine);
        strcpy(tmp,percorso_cartella);
        strcat(tmp,"\\");
        strcat(tmp,paziente);
        strcat(tmp,"\\TAC\\");
        strcat(tmp,nome_immagine);

        if (tmp[strlen(tmp)-3]!='.') strcat(tmp,".bmp");

        f1=fopen(tmp,"rb");

        if (f1!=NULL) {

            fread(&(img_2->img),sizeof(BMP_Image),1,f1);
            fclose(f1);

        }

        strcpy(img_2->paziente.codice_paziente,paziente);
        strcpy(img_2->codice_img.codice_zona,nome_immagine+11);
        cont=0;

        for(i=0;i<strlen(img_2->codice_img.codice_zona);i++) {

            if(img_2->codice_img.codice_zona[i]!='.') cont++;

            else break;

        }

        img_2->codice_img.codice_zona[cont]='\0';
        strncpy(img_2->codice_img.n_layer,nome_immagine+12+cont,3);
        calcolo_area_chiara_scura_pixel(img_1);
        calcolo_area_chiara_scura_pixel(img_2);
        confronta_aree(img_1,img_2);
        printf("\n\n%d   %d\n",img_1->caratteristiche_img.area_racchiusa.soglia_high,img_1->caratteristiche_img.area_racchiusa.soglia_low);
        printf("\n\n%d %d\n\n%d %d",img_1->caratteristiche_img.area_racchiusa.area_chiara_pixel,img_1->caratteristiche_img.area_racchiusa.area_scura_pixel,img_2->caratteristiche_img.area_racchiusa.area_chiara_pixel,img_2->caratteristiche_img.area_racchiusa.area_scura_pixel);

    }
    else if (selezione==10) {
        printf("\n\nDigitare il nome originale dell'immagine:  ");
        scanf("%s",tmp);
        strcat(tmp,".bmp");
        trova_proprietario(tmp,percorso_cartella);
    }

    else printf("\n\nerrore: selezione non valida");

}


//Dato il nome originale di un'immagine risale al paziente a cui l'immagine appartiene
//(se l'anagrafica è già stata compilata)
void trova_proprietario (char nome_immagine[],char percorso_cartella[]) {

    char percorso[PATH_DIM];
    char percorsoregistr[PATH_DIM];
    char percorsoanagrafica[PATH_DIM];
    char nomefile[50];
    DIR* cartella;
    DIR* cartella2;
    struct dirent* nome2;
    struct dirent* nome;
    FILE* f1;
    FILE* f2;
    char stringa[50];
    char codicefiscale[17];
    char codicefiscale1[17];
    char zona[15];
    char livello[17];
    char data[10];
    int i;
    int cont=0;
    char nomepaz[10];
    char cognome[10];

    strcpy(percorso,percorso_cartella);
    strcat(percorso,"\\..\\Backup_immagini");
    cartella=opendir(percorso);

    if (cartella!=NULL){

        seekdir(cartella,2);
        nome=readdir(cartella);

        while (strcmp(nome->d_name,nome_immagine))

            nome=readdir(cartella);

        if (nome==NULL) printf("\n\nerrore: immagine non trovata");

        else {

            strcat(percorso,"\\");
            strcat(percorso,nome->d_name);
            f1=fopen(percorso,"r");
            fseek(f1,sizeof(BMP_Image),SEEK_SET);
            fscanf(f1,"%s",stringa);
            strncpy(codicefiscale,stringa,16);
            codicefiscale[16]='\0';//codice fiscale dal fondo file backup
            fseek(f1,sizeof(BMP_Image)+20,SEEK_SET);
            fscanf(f1,"%s",stringa);
            strncpy(data,stringa,10);
            data[10]='\0';
            fseek(f1,sizeof(BMP_Image)+24+strlen(data),SEEK_SET);
            fscanf(f1,"%s",stringa);

            for(i=0;i<strlen(stringa);i++) {

                if(stringa[i]!='-') cont++;

                else break;
            }

            strncpy(zona,stringa,cont);
            zona[cont]='\0';
            fseek(f1,sizeof(BMP_Image)+24+strlen(data)+cont+4,SEEK_SET);
            fscanf(f1,"%s",stringa);
            strncpy(livello,stringa,3);
            livello[3]='\0';
            strcpy(nomefile,data);
            nomefile[2]='-';
            nomefile[5]='-';
            nomefile[10]='.';
            nomefile[11]='\0';
            strcat(nomefile,zona);
            nomefile[11+cont]='.';
            nomefile[12+cont]='\0';
            strcat(nomefile,livello);
            nomefile[12+cont+3]='\0';
            strcat(nomefile,".bmp");
            nomefile[12+cont+7]='\0';
            strncpy(codicefiscale1,codicefiscale,16);
            codicefiscale1[16]='\0';
            strcpy(percorsoregistr,percorso_cartella);
            strcat(percorsoregistr,"\\");
            strcat(percorsoregistr,codicefiscale1);
            strcat(percorsoregistr,"\\");
            strcat(percorsoregistr,"TAC");
            cartella2=opendir(percorsoregistr);

            if (cartella!=NULL){

                seekdir(cartella2,2);
                nome2=readdir(cartella2);

                while (nome2!=NULL){

                    if(strcmp(nome2->d_name,nomefile)==0){

                        printf("\n\nTrovata corrispondenza\n\n");
                        strcpy(percorsoanagrafica,percorso_cartella);
                        strcat(percorsoanagrafica,"\\");
                        strcat(percorsoanagrafica,codicefiscale1);
                        strcat(percorsoanagrafica,"\\");
                        strcat(percorsoanagrafica,codicefiscale1);
                        strcat(percorsoanagrafica,"-anagrafica.doc");
                        f2=fopen(percorsoanagrafica,"r");
                        fseek(f2,96,SEEK_SET);
                        fscanf(f2,"%s",nomepaz);
                        printf("L'immagine %s appartiene a:\n\n",nome->d_name);
                        printf("Nome: %s\n",nomepaz);
                        fseek(f2,96+strlen(nomepaz)+10,SEEK_SET);
                        fscanf(f2,"%s\n",cognome);
                        printf("Cognome: %s\n\n",cognome);
                        printf("***********************************\n");
                        break;

                    }

                    nome2=readdir(cartella2);

                }

            }

        }

    }

}
