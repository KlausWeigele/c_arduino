# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 🎯 Project Context

Dieses Projekt dient dem Auffrischen von C-Kenntnissen und dem Erlernen der Arduino-Programmierung. Es ist ein Lern- und Experimentier-Repository.

## 🛠️ Development Environment

### Prerequisites
- **Arduino IDE 2.x** oder **arduino-cli** für Arduino-Entwicklung
- **GCC/Clang** für reine C-Programme (auf macOS bereits vorinstalliert)
- Optional: **PlatformIO** als Alternative zur Arduino IDE

### Arduino CLI Commands
```bash
# Board-Liste anzeigen
arduino-cli board list

# Sketch kompilieren (z.B. für Arduino Uno)
arduino-cli compile --fqbn arduino:avr:uno <sketch-folder>

# Sketch hochladen
arduino-cli upload -p /dev/cu.usbmodem* --fqbn arduino:avr:uno <sketch-folder>

# Kompilieren und Hochladen in einem Schritt
arduino-cli compile --fqbn arduino:avr:uno <sketch-folder> && arduino-cli upload -p /dev/cu.usbmodem* --fqbn arduino:avr:uno <sketch-folder>

# Serial Monitor öffnen
arduino-cli monitor -p /dev/cu.usbmodem* -c baudrate=9600
```

### Pure C Compilation (für Übungen ohne Arduino)
```bash
# Kompilieren mit GCC
gcc -Wall -Wextra -std=c11 -o programm programm.c

# Mit Debug-Informationen
gcc -Wall -Wextra -std=c11 -g -o programm programm.c

# Ausführen
./programm
```

## 📁 Project Structure

```
c_arduino/
├── CLAUDE.md           # Diese Datei
├── exercises/          # Reine C-Übungen zum Auffrischen
│   ├── basics/         # Variablen, Datentypen, Operatoren
│   ├── control/        # if, switch, loops
│   ├── functions/      # Funktionen, Parameter, Rückgabewerte
│   ├── pointers/       # Pointer und Speicherverwaltung
│   └── structs/        # Strukturen und Arrays
└── arduino/            # Arduino-Projekte
    ├── blink/          # Klassisches Blink-Beispiel
    └── sensors/        # Sensor-Projekte
```

## 📋 Arduino Sketch Struktur

Jeder Arduino-Sketch benötigt:
- `setup()` - wird einmal beim Start ausgeführt
- `loop()` - läuft kontinuierlich in Endlosschleife

```c
void setup() {
  // Initialisierung hier
}

void loop() {
  // Hauptprogramm hier
}
```

## 🔧 Häufig verwendete Arduino-Funktionen

- `pinMode(pin, INPUT/OUTPUT)` - Pin-Modus setzen
- `digitalWrite(pin, HIGH/LOW)` - Digitalen Ausgang setzen
- `digitalRead(pin)` - Digitalen Eingang lesen
- `analogRead(pin)` - Analogen Wert lesen (0-1023)
- `analogWrite(pin, value)` - PWM-Ausgabe (0-255)
- `delay(ms)` - Wartezeit in Millisekunden
- `Serial.begin(baudrate)` - Serielle Kommunikation starten
- `Serial.println(text)` - Text über Serial ausgeben

## 📝 Document History

- **2025-12-08**: Initiale Erstellung - Lernprojekt für C und Arduino
