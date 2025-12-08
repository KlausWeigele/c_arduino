/*
 * 01_hello.c - Dein erstes C-Programm
 *
 * Kompilieren: gcc -Wall -std=c11 -o 01_hello 01_hello.c
 * Ausführen:   ./01_hello
 */

#include <stdio.h>

int main(void) {
    // Einfache Textausgabe
    printf("Hallo, Welt!\n");

    // Variablen und Datentypen
    int ganzzahl = 42;
    float kommazahl = 3.14f;
    char buchstabe = 'A';

    // Formatierte Ausgabe
    printf("Integer: %d\n", ganzzahl);
    printf("Float: %.2f\n", kommazahl);
    printf("Char: %c (ASCII-Wert: %d)\n", buchstabe, buchstabe);

    // Rückgabewert 0 = Programm erfolgreich beendet
    return 0;
}
