#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
// salta caractares hasta encontrar un espacio, salta ese espacio tambien

int saltaEspacio(int fd)
{
    char leido;
    int error;

    error = read(fd, &leido, 1);
    
    while (error > 0 && leido != ' '&& leido != '\t')
    {
       // printf("se ha saltado:%c:\n", leido);
        error = read(fd, &leido, 1);
    }

    if (error == -1)
    {
        perror("Error en la lectura");
        return -1;
    }

    return 0;
}