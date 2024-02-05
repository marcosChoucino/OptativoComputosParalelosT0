#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "misFuncionesStrings.c"

int main(int argc, char *argv[])
{

    int filas;
    int columnas;
    char *nombreFichero;
    // comprobacion de argumentos y instanciar los parametros:
    if (argc != 4)
    {
        printf("Los parametros de entradas son: numero de filas, numero de columnas, y nombre del fichero.\nTener en cuenta que hay que contar la fila y la columna que no tienen datos como tal.\n\n\n");
        return 0;
    }
    else
    {
        filas = atoi(argv[1]);
        columnas = atoi(argv[2]);
        nombreFichero = argv[3];
        printf("numero de filas: %i\nnumero de columnas: %i\nnombre del fichero %s\n", filas, columnas, nombreFichero);
    }
    float miMatriz[filas - 1][columnas - 1]; // matriz donde guardaremos los valores para operar con ellos

    // abrir archivo
    int fd = open(nombreFichero, O_RDONLY);
    if (fd == -1)
    {
        printf("Archivo no existe");
        return 1;
    }
    int leer;
    int posbuffer;
    char contenidoEnChar[20];
    float mifloat;
    // primero trataremos la primera fila, la cual es especial por que no hay que operar con ella:
    char leido;
    posbuffer = 0;
    contenidoEnChar[0] = 'a';
    while (leido != '\n')
    {
        leer = read(fd, &leido, 1);
        posbuffer++;
    }

    // resto de las filas:
    int columnaActual = 0;
    for (int i = 0; i < filas - 1; i++) // i <= filas
    {
        if (saltaEspacio(fd) == -1) // saltamos el primer elemento de cada fila, que es el numero de procesadores.
        {
            printf("error saltar espacio\n");
            return -1;
        }
        posbuffer = 0;

        leer = read(fd, &contenidoEnChar[posbuffer], 1); // leemos y almacenamos
        // printf("En la posicion %i del bufer hay:%c:\n", posbuffer, contenidoEnChar[posbuffer]);
        posbuffer = 0;

        while (contenidoEnChar[posbuffer] != '\n' && leer > 0) // hasta que no se salte de linea...
        {
            if (contenidoEnChar[posbuffer] == ' ' || contenidoEnChar[posbuffer] == '\t') // si hay espacio tratas el numero que has estado leyendo
            {
                mifloat = atof(contenidoEnChar); // combierto del buffer a un float con el que se pueda operar
                // printf("tras combertir en float queda:%f:", mifloat);
                miMatriz[i][columnaActual] = mifloat; // guardo el float
                                                      // printf("posicion %i,%i, numero: %f\n", i, columnaActual, miMatriz[i][columnaActual]);
                for (int h = 0; h < 20; h++)          // reseteo el contenido del buffer
                {
                    contenidoEnChar[h] = '\0';
                }
                posbuffer = 0;
                columnaActual++;
                leer = read(fd, &contenidoEnChar[posbuffer], 1); // leo el siguiente caracter
                // printf("en la posicion %i del bufer hay:%c:\n", posbuffer, contenidoEnChar[posbuffer]);
            }
            else
            {
                posbuffer++;

                leer = read(fd, &contenidoEnChar[posbuffer], 1); // leo el caracter y lo guardo en el buffer
                // printf("en la posicion %i del bufer hay:%c:%i:\n", posbuffer, contenidoEnChar[posbuffer],contenidoEnChar[posbuffer]);
            }
        }
        // trato el ultimo numero leido antes del \n
        mifloat = atof(contenidoEnChar);
        // printf("tras combertir en float queda:%f:", mifloat);
        miMatriz[i][columnaActual] = mifloat;
        // printf("posicion %i,%i, numero: %f\n", i, columnaActual, miMatriz[i][columnaActual]);
        for (int h = 0; h < 20; h++)
        {
            contenidoEnChar[h] = '\0';
        }

        // imprimo la linea

        // printf("la linea numer %i es:\n", i);
        /*
                for (int r = 0; r < columnas - 1; r++)
                {
                    printf(" %f ", miMatriz[i][r]);
                }
                printf("\n");
                */
        columnaActual = 0;
    }
    // muestro la matriz entera
    printf("la matriz leida es la siguiente:\n");
    for (int b = 0; b < filas - 1; b++)
    {
        for (int v = 0; v < columnas - 1; v++)
        {
            // printf("(%i,%i): %f. ", b, v, miMatriz[b][v]);
            printf(" %f. ", miMatriz[b][v]);
        }
        printf("\n");
    }
    printf("\n");

    // Calculo los valores ya que tengo los datos cargados
    float media = 0;
    float mediaPrimero = 0.0;
    float speedUp = 0;
    float efficiency = 0;
    int procesors[filas - 1];
    float SpeUp[filas - 1];
    float effi[filas - 1];
    printf("Nºprocesadores # media # speedUp # eficiencia\n");
    // trato el primero de una forma diferente, ya que me tengo que guardar su media para despues compararla con el resto
    for (int x = 0; x < columnas - 1; x++) // calculo la media
    {
        mediaPrimero = mediaPrimero + miMatriz[0][x];
    }
    // printf("suma:%f",media);
    // calculo los resultados:
    mediaPrimero = mediaPrimero / (float)(columnas - 1);
    speedUp = 1;
    efficiency = speedUp / (1 << 0);
    // imprimo los resultados por pantalla
    printf(" %i %f %f  %f ", 1, mediaPrimero, speedUp, efficiency);
    media = 0.0;
    printf("\n");
    // guardo los resultados en vectores para posteriormente graficarlos
    procesors[0] = 1;
    SpeUp[0] = 1;
    effi[0] = 1;

    for (int z = 1; z < filas - 1; z++)
    {

        for (int x = 0; x < columnas - 1; x++) // calculo la media
        {
            media = media + miMatriz[z][x];
        }
        // calculo los resultados:
        media = media / (float)(columnas - 1);
        speedUp = mediaPrimero / media;
        efficiency = speedUp / (1 << z);
        // imprimo los resultados por pantalla

        printf(" %i %f %f  %f ", 1 << z, media, speedUp, efficiency);
        media = 0.0;
        printf("\n");
        // guardo los resultados en vectores para posteriormente graficarlos
        procesors[z] = 1 << z;
        SpeUp[z] = speedUp;
        effi[z] = efficiency;

    }

    /// crear graficos con gnuplot
    // primero la eficiencia respecto a los procesadores.
    FILE *gnuplotPipe = popen("gnuplot -persist", "w"); // Abro una tubería a Gnuplot
    fprintf(gnuplotPipe, "set xlabel 'Procesadores'\n");
    fprintf(gnuplotPipe, "set ylabel 'Eficiencia'\n");
    if (gnuplotPipe == NULL)
    {
        fprintf(stderr, "Error al abrir Gnuplot.\n");
        return 1;
    }
    printf("\nse va a graficar\n");
    fprintf(gnuplotPipe, "plot '-' with lines\n");

    for (int i = 0; i < filas - 1; i++) // mando los datos a gnuplot
    {
        //printf("%i %lf\n", procesors[i], effi[i]);

        fprintf(gnuplotPipe, "%i %lf\n", procesors[i], effi[i]);
    }
    fprintf(gnuplotPipe, "e");

    pclose(gnuplotPipe);

    // ahora speed up respecto a los procesadores

    FILE *gnuplotPipe2 = popen("gnuplot -persist", "w"); // Abro una tubería a Gnuplot
    fprintf(gnuplotPipe2, "set xlabel 'Procesadores'\n");
    fprintf(gnuplotPipe2, "set ylabel 'SpeedUp'\n");
    if (gnuplotPipe2 == NULL)
    {
        fprintf(stderr, "Error al abrir Gnuplot.\n");
        return 1;
    }
    printf("\nse va a graficar\n");
    fprintf(gnuplotPipe2, "plot '-' with lines\n");

    for (int i = 0; i < filas - 1; i++) // mando los datos a gnuplot
    {
        //printf("%i %lf\n", procesors[i], SpeUp[i]);

        fprintf(gnuplotPipe2, "%i %lf\n", procesors[i], SpeUp[i]);
    }
    fprintf(gnuplotPipe2, "e");

    pclose(gnuplotPipe2);
    close(fd);
    return 0;
}
