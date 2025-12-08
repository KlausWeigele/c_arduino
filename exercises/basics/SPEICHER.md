# Speicherverwaltung in C (mit Arduino/AVR-Fokus)

## Übersicht: Speicherbereiche

In C gibt es verschiedene Speicherbereiche, die unterschiedliche Zwecke erfüllen:

```
+------------------+  Hohe Adressen
|      Stack       |  ↓ wächst nach unten
|        ↓         |
|                  |
|        ↑         |
|       Heap       |  ↑ wächst nach oben
+------------------+
|   .bss (uninit)  |  Uninitialisierte globale/static Variablen
+------------------+
|   .data (init)   |  Initialisierte globale/static Variablen
+------------------+
|      .text       |  Programmcode (bei AVR: im Flash)
+------------------+  Niedrige Adressen
```

### Arduino Uno (ATmega328P) Speicher

| Speichertyp | Größe   | Verwendung                          |
|-------------|---------|-------------------------------------|
| Flash       | 32 KB   | Programmcode (.text)                |
| SRAM        | 2 KB    | Stack, Heap, .data, .bss            |
| EEPROM      | 1 KB    | Persistente Daten                   |

---

## 1. Stack (Stapelspeicher)

**Zweck:** Lokale Variablen und Funktionsaufrufe

```c
void funktion() {
    int c = 42;  // liegt auf dem Stack
}  // c wird automatisch freigegeben
```

**Eigenschaften:**
- Automatische Speicherverwaltung (LIFO - Last In, First Out)
- Schneller Zugriff
- Begrenzte Größe (bei Arduino Uno: Teil der 2KB SRAM)
- Variable existiert nur während der Funktion läuft

---

## 2. Heap (Haldenspeicher)

**Zweck:** Dynamische Speicherallokation zur Laufzeit

```c
#include <stdlib.h>

int *ptr = malloc(sizeof(int));  // Speicher auf Heap reservieren
*ptr = 42;
free(ptr);  // Speicher manuell freigeben!
```

**Eigenschaften:**
- Manuelle Verwaltung (malloc/free)
- Flexibel, aber langsamer als Stack
- Gefahr von Memory Leaks wenn free() vergessen wird
- **Auf Arduino vermeiden!** (nur 2KB SRAM, Fragmentierung)

---

## 3. Static/Global (.data und .bss)

**Zweck:** Variablen, die das gesamte Programm über existieren

```c
int global = 100;        // .data - initialisiert
static int counter = 0;  // .data - initialisiert, nur in dieser Datei sichtbar
int uninit;              // .bss  - uninitialisiert (wird zu 0)
```

**Eigenschaften:**
- Existieren während gesamter Programmlaufzeit
- .data: Initialisierte Werte (belegen Flash UND RAM)
- .bss: Uninitialisiert, werden zu 0 gesetzt (belegen nur RAM)

---

## 4. Flash/Programmcode (.text)

**Zweck:** Der kompilierte Programmcode

Bei Arduino/AVR ist das besonders:
- Code liegt im Flash (32KB beim Uno)
- Konstanten können mit `PROGMEM` im Flash bleiben:

```c
#include <avr/pgmspace.h>

const char text[] PROGMEM = "Dieser Text liegt im Flash, nicht im RAM";
```

---

## Beispiel: `int c` Speicherreservierung

### Auf dem Arduino Uno (AVR ATmega328P):

- **int** = 2 Bytes (16 Bit)
- Wertebereich: -32.768 bis 32.767

### Lokale Variable (Stack)

```c
void setup() {
    int c = 42;  // 2 Bytes auf dem Stack
}
```

**Im Speicher:**
```
Stack (SRAM):
Adresse 0x08FF: [niederes Byte: 0x2A]  (42 = 0x002A)
Adresse 0x08FE: [höheres Byte: 0x00]
```

### Globale Variable (.data)

```c
int c = 42;  // 2 Bytes in .data Sektion

void setup() {
    c = c + 1;
}
```

**Im Speicher:**
```
.data (SRAM, feste Adresse):
Adresse 0x0100: [0x2A]  (niederes Byte)
Adresse 0x0101: [0x00]  (höheres Byte)
```

---

## AVR Assembler: Was der Compiler daraus macht

### Lokale Variable auf Stack

**C-Code:**
```c
void beispiel() {
    int c = 42;
    c = c + 1;
}
```

**AVR Assembler (vereinfacht):**
```asm
beispiel:
    ; Stack-Frame einrichten
    push r28              ; Y-Register sichern (Frame Pointer)
    push r29
    in r28, SPL           ; Stack Pointer laden
    in r29, SPH
    sbiw r28, 2           ; 2 Bytes für 'int c' reservieren
    out SPH, r29
    out SPL, r28

    ; c = 42 (0x002A)
    ldi r24, 0x2A         ; niederes Byte (42)
    ldi r25, 0x00         ; höheres Byte (0)
    std Y+1, r24          ; auf Stack speichern
    std Y+2, r25

    ; c = c + 1
    ldd r24, Y+1          ; c laden
    ldd r25, Y+2
    adiw r24, 1           ; +1 addieren
    std Y+1, r24          ; zurückspeichern
    std Y+2, r25

    ; Stack-Frame aufräumen
    adiw r28, 2
    out SPH, r29
    out SPL, r28
    pop r29
    pop r28
    ret
```

### Globale Variable

**C-Code:**
```c
int c = 42;

void erhöhen() {
    c = c + 1;
}
```

**AVR Assembler (vereinfacht):**
```asm
.section .data
c:  .word 42              ; 2 Bytes, initialisiert mit 42

.section .text
erhöhen:
    lds r24, c            ; Lade niederes Byte von fester Adresse
    lds r25, c+1          ; Lade höheres Byte
    adiw r24, 1           ; +1 addieren
    sts c, r24            ; Zurückspeichern
    sts c+1, r25
    ret
```

### Vergleich: Lokale vs. Globale Variable

| Aspekt              | Lokal (Stack)          | Global (.data)         |
|---------------------|------------------------|------------------------|
| Assembler-Befehle   | `std`, `ldd` (relativ) | `sts`, `lds` (absolut) |
| Adresse             | Relativ zu Y-Register  | Feste RAM-Adresse      |
| Overhead            | Stack-Frame Setup      | Keiner                 |
| Speicherverbrauch   | Nur während Funktion   | Permanent              |
| Zugriff             | Schnell (Register)     | Etwas langsamer        |

---

## AVR-Register Übersicht

Der ATmega328P hat 32 Allzweck-Register (R0-R31):

| Register  | Verwendung                                    |
|-----------|-----------------------------------------------|
| R0        | Temporär (Multiplikation)                     |
| R1        | Immer 0 (Konvention)                          |
| R2-R17    | Callee-saved (müssen gesichert werden)        |
| R18-R25   | Caller-saved (frei verwendbar)                |
| R24-R25   | Rückgabewert von Funktionen                   |
| R26-R27   | X-Register (Pointer)                          |
| R28-R29   | Y-Register (Frame Pointer)                    |
| R30-R31   | Z-Register (Pointer, Programmzugriff)         |

---

## Speicher-Keywords in C

| Keyword    | Bedeutung                                      |
|------------|------------------------------------------------|
| `auto`     | Standard für lokale Variablen (Stack)          |
| `static`   | Statische Lebensdauer, lokale Sichtbarkeit     |
| `extern`   | Variable ist in anderer Datei definiert        |
| `register` | Hinweis: Variable in CPU-Register halten       |
| `volatile` | Wert kann sich unerwartet ändern (I/O, ISR)    |
| `const`    | Konstante, unveränderlich                      |

### Arduino-spezifisch:

```c
#include <avr/pgmspace.h>

const int PROGMEM wert = 42;     // Im Flash speichern
volatile int counter = 0;        // Für Interrupt-Variablen
```

---

## Zusammenfassung für Arduino

1. **Spare RAM:** Nutze `PROGMEM` für konstante Daten
2. **Vermeide Heap:** `malloc`/`free` führt zu Fragmentierung
3. **Nutze `static`:** Für Variablen, die Funktionsaufrufe überleben sollen
4. **Nutze `volatile`:** Für Variablen in Interrupt-Service-Routinen
5. **Beachte Größen:** `int` = 2 Bytes, `long` = 4 Bytes, `float` = 4 Bytes
