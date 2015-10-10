
// Dimensione immagine in pixel, quadrata
#define DATA_DIM 256

// Dimensione stringa di caratteri
#define CHAR_DIM 30

// Soglia area chiara
#define SOGLIA_H 170

// Soglia area scura
#define SOGLIA_L 85

// Soglia sfondo
#define SOGLIA_SFONDO 0

// Dimensione percorso file/cartella
#define PATH_DIM 200

// Dimensione array di immagini
#define IMAGE_DIM 30

// Dimensione codice fiscale
#define CODICE_DIM 16



//Struttura per l'header
typedef struct {
   unsigned int size;
   unsigned short int reserved1;
   unsigned short int reserved2;
   unsigned int offset;
} Header;

//Struttura per l'header info
typedef struct {
   unsigned int size;
   int width,height;
   unsigned short int planes;
   unsigned short int bits;
   unsigned int compression;
   unsigned int imagesize;
   int xresolution,yresolution;
   unsigned int ncolours;
   unsigned int importantcolours;
} Info;

//Struttura per rappresentare un pixel. Nel nostro caso solo 8 bits, un solo campo
typedef struct {
   unsigned char grey;
} Pixel;

//Struttura immagine
typedef struct{
	unsigned char magic[2];                     //Tipo di immagine
	Header header;                              //Header
	Info info;                                  //Info header
	unsigned char color_table[DATA_DIM*4];      //Tabella colori
	Pixel data[DATA_DIM][DATA_DIM];             //Matrice di pixel
}BMP_Image;

//Struttura per data
typedef struct {
    unsigned short int gg;
    unsigned short int mm;
    unsigned short int aa;
} t_data;

//Struttura per residenza
typedef struct {
    char via [CHAR_DIM];
    unsigned short int numerocivico;
    char citta[CHAR_DIM];
    int CAP;
    char provincia[2];
}t_residenza;

//Sruttura per anagrafica
typedef struct {
    char nome[CHAR_DIM];
    char cognome[CHAR_DIM];
    t_data data_nascita;
    char luogo_nascita[CHAR_DIM];
    t_residenza residenza;
} t_anagrafica;

//Struttura per paziente
typedef struct {
    t_anagrafica anagrafica_paziente;
    char sesso;
    char codice_paziente[CHAR_DIM];
} t_paziente;

//Struttura per codice immagine
typedef struct {
    char n_layer[CHAR_DIM];
    char codice_zona[CHAR_DIM];
} t_codice;

//Struttura per coordinate
typedef struct {
    int i;
    int j;
} t_coordinata;

//Struttura per analisi aree immagine
typedef struct {
    int area_pixel; //Area inclusa nel contorno in pixel
    int area_mm_2;
    int soglia_high;
    int soglia_low;
    int soglia_sfondo;
    int area_scura_pixel;
    int area_chiara_pixel;
    int area_scura_mm2;
    int area_chiara_mm2;
} t_area;

//Struttura caratteristiche immagine
typedef struct {
    int n_livelli_grigio;
    int distrib_livelli_grigio[DATA_DIM];
    t_coordinata pixel_contorno[DATA_DIM*DATA_DIM];
    t_area area_racchiusa;
} t_caratteristiche;

//Struttura immagine per database
typedef struct {
    BMP_Image img;
    t_caratteristiche caratteristiche_img;
    t_paziente paziente;
    t_codice codice_img;
} t_immagine;

//Struttura stringa
struct stringa {
    char carattere;
    struct stringa* previous_char;
};


//Caricamento immagine
int loadBMP(char * filename, BMP_Image *img);

//Salvataggio immagine
int saveBMP(BMP_Image img, char * filename);

//Leggere una stringa di dimensione incognita includendo gli spazi
int leggi_stringa (struct stringa** testa);

//Stampare a video una struct stringa
void stampa_stringa (struct stringa** testa, struct stringa** coda);

//Trovare la fine di una struct stringa
int fine_stringa (struct stringa* pos,struct stringa** testa);

//Trascrivere una struct stringa in una stringa di char
void trasforma_stringa (struct stringa* testa,char* nuova_stringa,int lunghezza_stringa);

//Calcolare numero e distribuzione dei livelli di grigio
void livelli_di_grigio (t_immagine* image);

//Ridurre i livelli di grigio ad un massimo di 10
void riduci_livelli (t_immagine* image);

//Calcolare il minimo livello di grigio di un'immagine
int min (BMP_Image* img);

//Calcolare il massimo livello di grigio di un'immagine
int max (BMP_Image* img);

//Discretizzare (su 10 livelli) un valore
unsigned char approx (unsigned char valore,int min,int max);

//Identificare i pixel di contorno di un'immagine
void rileva_contorno (t_immagine* foto1);

//Verificare quali pixel fanno parte del contorno (in una matrice 2x2)
int controlla_matrice_piccola (t_immagine* foto, int x, int y);

//Riempire il vettore di coordinate contorno
void scrittura_pixel (t_immagine* foto,int x,int y,int* pos);

//Calcolare l'are in pixel contenuta nel contorno
void calcolo_area_pixel (t_immagine* foto);

//Convertire area da pixel a mm^2
float conversione_area_mm2 (int area);

//Calcolare area scura e area chiara in pixel contenuta nel contorno
void calcolo_area_chiara_scura_pixel (t_immagine* foto);

//Confrontare le aree di  due immagini
void confronta_aree (t_immagine* img_1,t_immagine* img_2);

//Tenere memoria dei dati relativi ad un'immagine
void salva_dati_immagine(char percorso_immagini[],char percorso_cartella[]);

//Scrivere i dati in fondo all'immagine
void scrivi_dati_immagine (char filename [DATA_DIM]);

//Copare le immagini nella cartella di lavoro con il nome corretto, eliminandole dala cartella di deposito
void sposta_file_immagine (char filename [DATA_DIM],BMP_Image img,char percorso_cartella[]);

//Stampare a video, salvando una copia su file.doc, la storia di un paziente
void storicizzazione(char codice_fiscale[],char percorso_cartella[]);

//Verificare l'esistenza di un paziente dal codice fiscale
int esistenza_codicefiscale(char stringa[],char x[]);

//Salvare su file.doc l'anagrafica di uno o più pazienti
void Anagrafica(char percorso[]);

//Visualizzare il menu di scelta
int mostra_menu(void);

//Analizzre la risposta ad una domanda(si/no)
int analizza_risposta(char rix[]);

//Contare i pazienti salvati nella cartella di lavoro
void conta_pazienti (char percorso[]);

//Caricare un'immagine su diplay
void visualizza_display_1(t_immagine* img,char percorso_cartella[]);

//Resettare il display
void reset_display_1(char percorso_cartella[]);

//Caricare un'immagine su diplay 2
void visualizza_display_2(t_immagine* img,char percorso_cartella[]);

//Resettare il display 2
void reset_display_2(char percorso_cartella[]);

//Eseguire l'opzione selezionata dal menù
void esegui_scelta (int selezione,char percorso_cartella[],t_immagine* img_1,t_immagine* img_2);

//Verificare la presenza di immagini da caricare nella cartella di deposito
void controlla_nuove_immagini (char percorso_immagini[],char percorso_cartella[]);

//Risalire al proprietario di un'immagine
void trova_proprietario (char nome_immagine[],char percorso_cartella[]);
