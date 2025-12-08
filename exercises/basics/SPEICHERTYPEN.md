# Speichertypen - Allgemeine Übersicht

## Grundlegende Speicherkategorien

### Flüchtiger Speicher (Volatile)
Verliert Daten bei Stromverlust.

### Nicht-flüchtiger Speicher (Non-Volatile)
Behält Daten auch ohne Strom.

---

## Speichertypen im Detail

### RAM (Random Access Memory)

**Eigenschaften:**
- Flüchtig (Daten gehen bei Stromverlust verloren)
- Schneller Lese- und Schreibzugriff
- Wahlfreier Zugriff (jede Adresse direkt erreichbar)

**Typen:**

| Typ   | Name                      | Eigenschaften                              |
|-------|---------------------------|--------------------------------------------|
| SRAM  | Static RAM                | Schnell, teuer, braucht keine Auffrischung |
| DRAM  | Dynamic RAM               | Günstiger, braucht regelmäßige Auffrischung|
| SDRAM | Synchronous DRAM          | Getaktet, schneller als DRAM               |
| DDR   | Double Data Rate SDRAM    | Doppelte Datenrate pro Takt                |

**Verwendung in Mikrocontrollern:**
- SRAM für Arbeitsspeicher (Stack, Heap, Variablen)
- Typisch: Wenige KB bis einige MB

---

### ROM (Read-Only Memory)

**Eigenschaften:**
- Nicht-flüchtig
- Nur lesbar (nach Herstellung)
- Sehr günstig in Massenproduktion

**Typen:**

| Typ    | Name                        | Eigenschaften                           |
|--------|-----------------------------|-----------------------------------------|
| Mask ROM | Masken-ROM               | Bei Herstellung programmiert, nicht änderbar |
| PROM   | Programmable ROM            | Einmal programmierbar                   |
| EPROM  | Erasable PROM               | Mit UV-Licht löschbar                   |
| EEPROM | Electrically Erasable PROM  | Elektrisch löschbar, byteweise          |

**Verwendung:**
- Firmware, die sich nie ändert
- Bootloader (teilweise)

---

### Flash-Speicher

**Eigenschaften:**
- Nicht-flüchtig
- Elektrisch lösch- und beschreibbar
- Block-/Sektorweise löschbar (nicht byteweise wie EEPROM)
- Begrenzte Schreibzyklen (typisch 10.000-100.000)
- Günstiger als EEPROM bei größeren Kapazitäten

**Typen:**

| Typ  | Eigenschaften                                    |
|------|--------------------------------------------------|
| NOR  | Schneller Lesezugriff, langsamer Schreibzugriff, Code direkt ausführbar |
| NAND | Höhere Dichte, günstiger, schnelleres Schreiben  |

**Verwendung in Mikrocontrollern:**
- Programmspeicher (Code)
- Bei Arduino: Sketches werden hier gespeichert

---

### EEPROM (Electrically Erasable Programmable ROM)

**Eigenschaften:**
- Nicht-flüchtig
- Byteweise les- und schreibbar
- Langsamerer Schreibzugriff als Flash
- Begrenzte Schreibzyklen (typisch 100.000-1.000.000)
- Ideal für kleine Datenmengen, die selten geändert werden

**Verwendung:**
- Konfigurationsdaten
- Kalibrierungswerte
- Zählerstände, die Stromausfall überleben sollen

---

## Speicher in Software-Begriffen

### Stack (Stapelspeicher)

```
+-------------+
|   Funktion  |  ← Aktueller Stack Frame
|   Parameter |
|   Lokale    |
|   Variablen |
+-------------+
|  Vorherige  |
|   Funktion  |
+-------------+
      ...
```

**Eigenschaften:**
- Teil des RAM
- LIFO-Prinzip (Last In, First Out)
- Automatische Verwaltung durch Compiler
- Wächst bei Funktionsaufrufen, schrumpft bei Rückkehr
- Speichert: Lokale Variablen, Rücksprungadressen, Parameter

**Gefahren:**
- Stack Overflow bei zu tiefer Rekursion
- Bei Mikrocontrollern besonders kritisch (wenig RAM)

---

### Heap (Haldenspeicher)

```
+-------------+
|  Allokiert  |  malloc()
+-------------+
|    Frei     |
+-------------+
|  Allokiert  |  malloc()
+-------------+
|    Frei     |  <- Fragmentierung!
+-------------+
```

**Eigenschaften:**
- Teil des RAM
- Dynamische Speicherallokation zur Laufzeit
- Manuelle Verwaltung (malloc/free in C, new/delete in C++)
- Wächst bei Bedarf (entgegengesetzt zum Stack)

**Gefahren:**
- Memory Leaks (vergessenes free())
- Fragmentierung
- Auf Mikrocontrollern möglichst vermeiden!

---

### Register

**Eigenschaften:**
- Schnellster Speicher (direkt in der CPU)
- Sehr begrenzte Anzahl (8-32 typisch)
- Für aktuelle Berechnungen

**AVR (Arduino):**
- 32 Allzweck-Register (R0-R31)
- Spezialregister (Status, Stack Pointer, etc.)

---

## Speicher-Hierarchie

```
Geschwindigkeit    Kapazität    Kosten/Byte
     ↑                ↓              ↑
+----------+
| Register |     Bytes           Sehr hoch
+----------+
|  Cache   |     KB              Hoch
+----------+
|   RAM    |     MB-GB           Mittel
+----------+
|  Flash   |     GB-TB           Niedrig
+----------+
|   HDD    |     TB              Sehr niedrig
+----------+
     ↓                ↑              ↓
```

*Hinweis: Mikrocontroller haben meist keinen Cache.*

---

## Speichertypen-Vergleich

| Eigenschaft      | SRAM      | DRAM      | Flash     | EEPROM    |
|------------------|-----------|-----------|-----------|-----------|
| Flüchtig         | Ja        | Ja        | Nein      | Nein      |
| Lesegeschw.      | Sehr hoch | Hoch      | Hoch      | Mittel    |
| Schreibgeschw.   | Sehr hoch | Hoch      | Mittel    | Niedrig   |
| Schreibzyklen    | Unbegrenzt| Unbegrenzt| 10k-100k  | 100k-1M   |
| Kosten/Bit       | Hoch      | Mittel    | Niedrig   | Mittel    |
| Typische Größe   | KB-MB     | GB        | MB-TB     | KB        |
| Löscheinheit     | Byte      | Byte      | Block     | Byte      |

---

# Arduino Speicher-Übersicht

## Speicherarchitektur bei Arduino

```
┌─────────────────────────────────────────┐
│              Mikrocontroller            │
├─────────────────────────────────────────┤
│  Flash (Programmspeicher)               │
│  - Dein Sketch/Code                     │
│  - Bootloader                           │
│  - Konstanten mit PROGMEM               │
├─────────────────────────────────────────┤
│  SRAM (Arbeitsspeicher)                 │
│  - .data (initialisierte Variablen)     │
│  - .bss (uninitialisierte Variablen)    │
│  - Heap (dynamisch, wächst ↑)           │
│  - Stack (wächst ↓)                     │
├─────────────────────────────────────────┤
│  EEPROM (Persistenter Speicher)         │
│  - Konfiguration                        │
│  - Zählerstände                         │
│  - Kalibrierungsdaten                   │
└─────────────────────────────────────────┘
```

---

## Arduino-Boards im Vergleich

### 8-Bit AVR Boards

| Board            | Mikrocontroller | Flash   | SRAM    | EEPROM | Takt     |
|------------------|-----------------|---------|---------|--------|----------|
| **Uno R3**       | ATmega328P      | 32 KB   | 2 KB    | 1 KB   | 16 MHz   |
| **Uno R4 Minima**| RA4M1           | 256 KB  | 32 KB   | 8 KB   | 48 MHz   |
| **Nano**         | ATmega328       | 32 KB   | 2 KB    | 1 KB   | 16 MHz   |
| **Nano Every**   | ATmega4809      | 48 KB   | 6 KB    | 256 B  | 20 MHz   |
| **Mega 2560**    | ATmega2560      | 256 KB  | 8 KB    | 4 KB   | 16 MHz   |
| **Leonardo**     | ATmega32U4      | 32 KB   | 2.5 KB  | 1 KB   | 16 MHz   |
| **Micro**        | ATmega32U4      | 32 KB   | 2.5 KB  | 1 KB   | 16 MHz   |
| **Pro Mini 5V**  | ATmega328       | 32 KB   | 2 KB    | 1 KB   | 16 MHz   |
| **Pro Mini 3.3V**| ATmega328       | 32 KB   | 2 KB    | 1 KB   | 8 MHz    |

### 32-Bit ARM Boards

| Board            | Mikrocontroller | Flash   | SRAM    | EEPROM    | Takt     |
|------------------|-----------------|---------|---------|-----------|----------|
| **Due**          | ATSAM3X8E       | 512 KB  | 96 KB   | -         | 84 MHz   |
| **Zero**         | ATSAMD21G18     | 256 KB  | 32 KB   | -         | 48 MHz   |
| **MKR1000**      | ATSAMD21G18     | 256 KB  | 32 KB   | -         | 48 MHz   |
| **MKR WiFi 1010**| ATSAMD21G18     | 256 KB  | 32 KB   | -         | 48 MHz   |
| **Nano 33 IoT**  | ATSAMD21G18     | 256 KB  | 32 KB   | -         | 48 MHz   |
| **Nano 33 BLE**  | nRF52840        | 1 MB    | 256 KB  | -         | 64 MHz   |
| **Portenta H7**  | STM32H747       | 2 MB    | 1 MB    | -         | 480 MHz  |
| **Giga R1 WiFi** | STM32H747       | 2 MB    | 1 MB    | -         | 480 MHz  |

### ESP-basierte Boards (Arduino-kompatibel)

| Board            | Mikrocontroller | Flash   | SRAM    | PSRAM   | Takt     |
|------------------|-----------------|---------|---------|---------|----------|
| **ESP8266**      | ESP8266         | 1-4 MB  | 80 KB   | -       | 80 MHz   |
| **ESP32**        | ESP32           | 4-16 MB | 520 KB  | 4-8 MB  | 240 MHz  |
| **ESP32-S2**     | ESP32-S2        | 4 MB    | 320 KB  | 2 MB    | 240 MHz  |
| **ESP32-S3**     | ESP32-S3        | 8-16 MB | 512 KB  | 2-8 MB  | 240 MHz  |
| **ESP32-C3**     | ESP32-C3        | 4 MB    | 400 KB  | -       | 160 MHz  |

### Teensy Boards (Arduino-kompatibel)

| Board            | Mikrocontroller | Flash   | SRAM    | EEPROM  | Takt     |
|------------------|-----------------|---------|---------|---------|----------|
| **Teensy 4.0**   | IMXRT1062       | 2 MB    | 1 MB    | 1 KB*   | 600 MHz  |
| **Teensy 4.1**   | IMXRT1062       | 8 MB    | 1 MB    | 4 KB*   | 600 MHz  |
| **Teensy 3.2**   | MK20DX256       | 256 KB  | 64 KB   | 2 KB    | 72 MHz   |

*Emuliert in Flash

---

## Detailansicht: Beliebte Boards

### Arduino Uno R3 (ATmega328P)

```
Flash: 32 KB
├── Bootloader: ~0.5 KB
└── Verfügbar für Sketch: ~31.5 KB

SRAM: 2 KB (2048 Bytes)
├── .data: Initialisierte globale Variablen
├── .bss: Uninitialisierte globale Variablen
├── Heap: Dynamischer Speicher (wächst ↑)
└── Stack: Lokale Variablen (wächst ↓)

EEPROM: 1 KB (1024 Bytes)
└── 100.000 Schreibzyklen pro Zelle
```

**Typische Speichernutzung:**
- Kleiner Sketch: 2-5 KB Flash, 200-500 Bytes RAM
- Mittlerer Sketch: 10-20 KB Flash, 500-1000 Bytes RAM
- Großer Sketch: 25-30 KB Flash, 1500-1900 Bytes RAM

---

### Arduino Mega 2560

```
Flash: 256 KB
├── Bootloader: ~8 KB
└── Verfügbar für Sketch: ~248 KB

SRAM: 8 KB (8192 Bytes)

EEPROM: 4 KB (4096 Bytes)
```

**Ideal für:**
- Viele I/O-Pins (54 digital, 16 analog)
- Große Projekte mit vielen Libraries
- Projekte mit viel String-Verarbeitung

---

### Arduino Nano 33 BLE

```
Flash: 1 MB (1048576 Bytes)
├── SoftDevice (BLE Stack): ~152 KB
├── Bootloader: ~24 KB
└── Verfügbar für Sketch: ~856 KB

SRAM: 256 KB (262144 Bytes)

Kein dediziertes EEPROM
└── Flash-Emulation möglich
```

**Ideal für:**
- Bluetooth-Projekte
- Machine Learning (TensorFlow Lite)
- Komplexe Sensorfusion

---

### ESP32

```
Flash: 4-16 MB (extern)
├── Bootloader: ~16 KB
├── Partition Table: 3 KB
├── NVS (Non-Volatile Storage): ~20 KB
├── OTA (Over-The-Air Updates): Optional
└── App: Rest verfügbar

SRAM: 520 KB
├── DRAM: ~320 KB (Daten)
└── IRAM: ~200 KB (Instruktionen/Cache)

PSRAM: 0-8 MB (optional, extern)

Kein dediziertes EEPROM
└── NVS oder Preferences Library nutzen
```

**Ideal für:**
- WiFi und Bluetooth Projekte
- Dual-Core Anwendungen
- Speicherintensive Projekte

---

## Speicherverbrauch prüfen

### Arduino IDE

Nach dem Kompilieren zeigt die IDE:
```
Sketch uses 3462 bytes (10%) of program storage space.
Global variables use 214 bytes (10%) of dynamic memory.
```

### Im Code (zur Laufzeit)

```c
// Freien RAM anzeigen (AVR)
int freeRam() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup() {
    Serial.begin(9600);
    Serial.print("Freier RAM: ");
    Serial.print(freeRam());
    Serial.println(" Bytes");
}
```

### ESP32 Speicherinfo

```c
void setup() {
    Serial.begin(115200);
    Serial.printf("Total heap: %d\n", ESP.getHeapSize());
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
}
```

---

## Speicher-Tipps für Arduino

### Flash sparen

```c
// Strings im Flash halten
Serial.println(F("Dieser String liegt im Flash!"));

// Große Konstanten mit PROGMEM
const byte daten[] PROGMEM = {0x00, 0x01, 0x02, ...};
```

### RAM sparen

```c
// Kleinere Datentypen verwenden
uint8_t kleineZahl = 42;     // 1 Byte statt 2 (int)
int16_t mittel = 1000;       // 2 Bytes
int32_t gross = 100000;      // 4 Bytes

// Strings vermeiden
// Schlecht:
String text = "Hallo";       // Dynamischer Heap-Speicher

// Besser:
char text[] = "Hallo";       // Feste Größe auf Stack
const char* text = "Hallo";  // Pointer auf Flash-String
```

### EEPROM nutzen

```c
#include <EEPROM.h>

// Schreiben (langsam, begrenzte Zyklen!)
EEPROM.write(0, 42);

// Lesen
byte wert = EEPROM.read(0);

// Update (schreibt nur wenn sich Wert ändert)
EEPROM.update(0, 42);  // Schont EEPROM!
```

---

## Zusammenfassung

| Speichertyp | Verwendung                    | Hinweis                       |
|-------------|-------------------------------|-------------------------------|
| Flash       | Programmcode, Konstanten      | Mit PROGMEM Konstanten lagern |
| SRAM        | Variablen, Stack, Heap        | Sparsam nutzen!               |
| EEPROM      | Persistente Konfiguration     | update() statt write()        |
| Register    | Aktuelle Berechnungen         | Vom Compiler verwaltet        |
