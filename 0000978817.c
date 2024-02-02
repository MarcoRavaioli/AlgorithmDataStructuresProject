/*  Marco Ravaioli  0000978817  marco.ravaioli8@studio.unibo.it
 *  Classe A
 *
 *  Nota: studente dell'anno accademico 2020/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

void TrovaCamminoMinimo(int** alt, int** piove, int nRig, int nCol);
void CercaAsciutto(int** alt, int** piove, int nRig, int nCol);
void PreparaGriglia(FILE* file, int nRig, int nCol, int** alt, int** piove);


int main(int argc, char* argv[]) 
{
    /* Dichiarazione delle variabili*/
    int i;
    int nRig;
    int nCol;
    
    FILE* file;
    
    /* Dichiarazione delle matrici*/
    int** alt;
    int** piove;
            

    /* Controlla i parametri passati da riga di comando*/
    if (argc < 2) 
    {
        printf("Errore: specificare il nome del file come argomento\n");
        return 1;
    }

    /* Apri il file di input*/
    file = fopen(argv[1], "r");
    if (file == NULL) 
    {
        printf("Errore: impossibile aprire il file di input\n");
        return 1;
    }

    /* Leggi l'input dal file*/
    if (fscanf(file, "%d %d", &nRig, &nCol) != 2) 
    {
        printf("Formato del file non valido.\n");
        fclose(file);
        return 1;
    }

    /* Allocazione dinamica delle matrici*/
    alt = malloc(nRig * sizeof(int*));
    piove = malloc(nRig * sizeof(int*));

    for (i = 0; i < nRig; i++) 
    {
        alt[i] = malloc(nCol * sizeof(int));        
        piove[i] = malloc(nCol * sizeof(int));
    }

    PreparaGriglia(file, nRig, nCol, alt, piove);

    /* Chiusura del file*/
    fclose(file);

    TrovaCamminoMinimo(alt, piove, nRig, nCol);

    for (i = 0; i < nRig; i++)
    {
        free(alt[i]);
        free(piove[i]);
    }

    free(alt);
    free(piove);
    

    return 0;
}

void PreparaGriglia(FILE* file, int nRig, int nCol, int** alt, int** piove)
{
    int i;
    int j;

    /* Scorri il file per copiare la matrice delle altezze e settare quella della pioggia tutta a 1*/
    for (i = 0; i < nRig; i++) 
    {
        for (j = 0; j < nCol; j++) 
        {
            fscanf(file, "%1d", &alt[i][j]);
            piove[i][j] = 1;
        }
    }

    CercaAsciutto(alt, piove, nRig, nCol);

}



void CercaAsciutto(int** alt, int** piove, int nRig, int nCol)
{
    int i;
    int j;
    int h;

    /* Scorri la matrice delle altezze */
    for (i = 0; i < nRig; i++) 
    {
        for (j = 0; j < nCol; j++) 
        {
            /* Quando trovi un edifiicio alto h, le h caselle a destra ponile asciutte */
            if (alt[i][j] != 0)
            {
                for (h = 1; h <= alt[i][j]; h++)
                {
                    if (j + h < nCol && alt[i][j + h] == 0)
                        piove[i][j + h] = 0;
                }
            }
        }
    }

}


/* Utilizza l'algoritmo di BFS per il pathfinding pesato sul numero di celle con pioggia*/
void TrovaCamminoMinimo(int** alt, int** piove, int nRig, int nCol) 
{
    /* Dichiarazioni di matrici, vettori e variabili*/
    int** dist;
    int** pioggia;
    int* codaR;
    int* codaC;
    char* perc;

    int i;
    int j;

    int r;
    int c;

    int nr;
    int nc;

    int visto;
    int trovato;
    
    /* Direz.:   S   E   O   N */
    int dr[] = { 0,  1, -1,  0 };
    int dc[] = { 1,  0,  0, -1 };

    /* Alloccazione di memoria delle matrici */
    dist = malloc(nRig * sizeof(int*));
    pioggia = malloc(nRig * sizeof(int*));

    for (i = 0; i < nRig; i++)
    {
        dist[i] = malloc(nCol * sizeof(int));
        pioggia[i] = malloc(nCol * sizeof(int));
    }

    /* Allocazione di memoria dei vettori, coda è formato dalle coppie di valori riga e colonna */
    codaR = malloc((nRig * nCol) * sizeof(int));
    codaC = malloc((nRig * nCol) * sizeof(int));
    perc = malloc(nRig * nCol * sizeof(char));

    printf("dimendioni codaR: %d * sizeof: %lu\n", nRig * nCol, sizeof(int));
    printf("dimendioni codaC: %d * sizeof: %lu\n", nRig * nCol, sizeof(int));


    /* Inizializzazione delle matrici su si lavora*/
    for (i = 0; i < nRig; i++) 
    {       
        for (j = 0; j < nCol; j++) 
        {
            dist[i][j] = INT_MAX;
            pioggia[i][j] = 0;
        }
    }    
    
    memset(perc, '\0', sizeof(nRig * nCol + 1));


    /* Poni la prima casella con dist = 1 e se piove anche la casella della matrice pioggia a 1*/
    dist[0][0] = 1;
    if (piove[0][0] == 1)
        pioggia[0][0] = 1;

    /* Inizializzazione delle variabili che scorrono la coda*/
    visto = 0;
    trovato = 0;

    /* Assegnamento della prima cella alla coda*/
    codaR[trovato] = 0;
    codaC[trovato] = 0;
    trovato++;

    /* Ciclo che continua fino all'esaurimento della coda*/
    while (visto < trovato) 
    {
        /* Scorri la coda di una cella*/
        r = codaR[visto];
        c = codaC[visto];
        visto++;

        /* Terminazione nel caso di raggiungimento dell'ultima casella*/
        if (r == nRig - 1 && c == nCol - 1)
            break;

        /* Scorrimento delle 4 direzioni possibili*/
        for (i = 0; i < 4; i++) 
        {
            nr = r + dr[i];
            nc = c + dc[i];
            /* Se la cella in questione e' all'interno della martice 
               e non e' stata ancora visitata o e' gia' stata visitata ma ci arrivo con un percorso di lunghezza pari con minor pioggia*/
            if (nr >= 0 && nr < nRig && nc >= 0 && nc < nCol && alt[nr][nc] == 0 && (dist[r][c] + 1 < dist[nr][nc] || (dist[r][c] + 1 == dist[nr][nc] && pioggia[r][c] + piove[nr][nc] < pioggia[nr][nc]))) 
            {
                /* Se e' una cella che devo ancora visitare, la aggiungo alla coda*/
                if (dist[r][c] + 1 != dist[nr][nc])
                {
                    codaR[trovato] = nr;
                    printf("codaR[%d] = %d \t", trovato, nr );
                    codaC[trovato] = nc;
                    printf("codaC[%d] = %d \n", trovato, nc );
                    trovato++;                    
                }                
                
                /* Aggiorno la distnaza dalla prima cella e la somma di celle con pioggia minimo nel tragitto*/
                dist[nr][nc] = dist[r][c] + 1;
                pioggia[nr][nc] = pioggia[r][c] + piove[nr][nc];

            }
        }

    }

    /* Se non si è raggiunta l'ultima cella si stampa -1 -1*/
    if (dist[nRig - 1][nCol - 1] == INT_MAX) 
        printf("-1 -1\n\n");
    /* Altrimenti*/
    else 
    {
        int len;
        int tmp;
        char dir;
        int tempR;
        int tempC;

        /* Imponi le lettere del percorso nella direzione opposta*/
        char direzioni[] = { 'O', 'N', 'S', 'E' };

        /* Partendo dall'ultima casella*/
        r = nRig - 1;
        c = nCol - 1;
        len = dist[r][c] - 1;

        /* Stampa la distanza e la pioggia totale*/
        printf("%d %d\n", dist[r][c], pioggia[r][c]);

        /* Scorri per la distanza tra la prima e l'ultima casella*/
        while (len > 0) 
        {
            tmp = INT_MAX;

            /* Per ogni direzione possibile*/
            for (i = 0; i < 4; i++) 
            {
                nr = r + dr[i];
                nc = c + dc[i];

                /* Controlla se la cella e' dentro la matrice, 
                   se e' quella precedente ed a parita' di distanza si scelga quella con pioggia minore*/
                if (nr >= 0 && nr < nRig && nc >= 0 && nc < nCol && dist[nr][nc] == len && tmp > pioggia[nr][nc]) 
                {
                    dir = direzioni[i];
                    tempR = nr;
                    tempC = nc;
                    tmp = pioggia[nr][nc];
                }
            }

            /* Scrivi la direzione preferendo quella con numero di caselle con pioggia piu' basso*/
            perc[len - 1] = dir;
            r = tempR;
            c = tempC;
            len--;
        }

        /* Scorri nella giusta direzione il vettore delle direzioni e stampalo a schermo*/
        for(i = 0; i < dist[nRig - 1][nCol - 1] - 1; i++)
        {
            printf("%c", perc[i]);
        }
        printf("\n");
    }

    /* Libero la memoria allocata*/
    for (i = 0; i < nRig; i++)
    {
        free(dist[i]);
        free(pioggia[i]);
    }

    free(dist);
    free(pioggia);
    free(codaR);
    free(codaC);
    free(perc);

}

