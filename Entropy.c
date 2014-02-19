#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#endif
#include <math.h>
#include <stdlib.h>

double  calcularEntropia(char *ruta);
char   *crearArchivo();
void    print_menu();

int
main(int argc, char *argv[])
{
    char    opcion = ' ';
    int     ch, nro;
    char    ruta[80];
    char   *nombre;
    double  entropia1, entropia2;

    while (opcion != '4') {
        print_menu();
        opcion = getchar();
        while ((ch = getchar()) != EOF && ch != '\n');

        switch (opcion) {
        case '1':
            printf("Indique el nombre o ruta del archivo: ");
            gets(ruta);
            calcularEntropia(ruta);
            break;
        case '2':
            nombre = crearArchivo();
            calcularEntropia(nombre);
            break;
        case '3':
            printf("ARCHIVO 1\n");
            nombre = crearArchivo();
            entropia1 = calcularEntropia(nombre);
            if (entropia1 == -1) {
                break;
            }
            printf("-----------------------------\n");
            printf("ARCHIVO 2\n");
            nombre = crearArchivo();
            entropia2 = calcularEntropia(nombre);
            if (entropia2 == -1) {
                break;
            }
            printf("-----------------------------\n");
            printf("RESULTADO");
            printf("\n-----------------------------\n");
            if (entropia1 > entropia2) {
                nro = 2;
            } else if (entropia1 < entropia2) {
                nro = 1;
            } else
                nro = 0;
            printf("Se podra transmitir el mensaje de manera mas eficiente \n(utilizando en promedio menos bits por simbolo) con: \n");
            if (nro == 0) {
                printf("Cualquiera de los dos archivos\n");
            } else
                printf("El ARCHIVO %d\n", nro);
            getchar();
            break;
        case '4':
            printf("Saliendo...\n");
            break;
        default:
            printf("Opcion incorrecta.\n");
            break;
        }
    }
    return 0;
}

double
calcularEntropia(char *ruta)
{
    FILE   *fp;
    long    array[256];
    double  entropy = 0;
    long    size = 0;
    int     oc;
    int     contenido;

    if ((fp = fopen(ruta, "rb")) == NULL) {
        printf("Error en archivo\n");
        return -1;
    }
    memset(array, 0, sizeof(array));

    printf("Quiere imprimir el contenido del archivo?[y/n]\n");
    contenido = toupper(getchar());
    if (contenido == 'Y')
        printf("El contenido de archivo es: ");
    while ((oc = fgetc(fp)) != EOF) {
        array[(unsigned char)oc]++;
        size++;
        if (contenido == 'Y')
            printf("%c", oc);
    }
    printf("\n");

    fclose(fp);

    while ((contenido = getchar()) != '\n' && contenido != EOF);

    printf("Quiere imprimir las ocurrencias de los carateres?[y/n]\n");
    contenido = toupper(getchar());

    if (contenido == 'Y')
        printf("Tabla de ocurrencias y su probabilidad\n");
    int     i;
    for (i = 0; i < 256; i++) {
        if (array[i] == 0)
            continue;
        double  p = 1.0 * array[i] / size;
        if (contenido == 'Y')
            printf("Caracter[%3d]\t%c\tOcurrencia:%10ld\t\tProbabilidad: %2.5f%%\n", i, i, array[i], (p * 100));
        entropy += p * log2(1 / p);
    }

    while ((contenido = getchar()) != '\n' && contenido != EOF);

    printf("La ENTROPIA del archivo es: %f [bits/byte]\n", entropy);
    printf("El tamanio del archivo es: %ld [bytes]\n", size);
    printf("El tamanio ideal del archivo comprimido seria: %lf [bytes]\n", entropy * size / 8);
    printf("Se podria reducir el tamanio del archivo a un %2.2f %% del tamanio original\n", (entropy / 8) * 100);
    return entropy;
}

char   *
crearArchivo()
{
    FILE   *fp;
    char    cadena[256];
    static char nombre[80];
    printf("Indique el nombre del archivo a guardar: ");
    gets(nombre);

    if ((fp = fopen(nombre, "wb")) == NULL) {
        printf("Error al crear archivo\n");
        return "";
    }
    printf("Ingrese el contenido del archivo: ");
    gets(cadena);
    fputs(cadena, fp);
    fclose(fp);
    return nombre;
}

void
print_menu()
{
    printf("\n\n*******************************************\n");
    printf("*********** CALCULO DE ENTROPIA ***********");
    printf("\n*******************************************\n\n");

    printf("1-Abrir un archivo y calcular su entropia\n");
    printf("2-Crear un archivo y calcular su entropia\n");
    printf("3-Comparar la entropia de dos archivos\n");
    printf("4-Salir\n");
    printf("\n--------->Elija una opcion: ");
}
