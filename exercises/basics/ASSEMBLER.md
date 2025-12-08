# Assembler-Programmierung - Übersicht

## Was ist Assembler?

Assembler ist eine Low-Level-Programmiersprache, die eine 1:1-Beziehung zu Maschinencode hat. Jede Assembler-Anweisung entspricht direkt einer CPU-Instruktion.

```
Hochsprache (C)     →    Assembler      →    Maschinencode
   x = x + 1;            ADD R24, 1          0x5F 0x08
```

---

## Grundbegriffe

| Begriff       | Bedeutung                                              |
|---------------|--------------------------------------------------------|
| **Mnemonic**  | Lesbare Abkürzung für Befehl (ADD, MOV, JMP)          |
| **Opcode**    | Numerischer Befehlscode in Maschinensprache           |
| **Operand**   | Daten oder Adressen, auf die der Befehl wirkt         |
| **Register**  | Schnelle Speicherplätze in der CPU                    |
| **Immediate** | Konstanter Wert direkt im Befehl                      |
| **Label**     | Symbolischer Name für eine Speicheradresse            |
| **Direktive** | Anweisung an den Assembler (nicht an CPU)             |

---

## CPU-Architekturen

### CISC (Complex Instruction Set Computing)

**Beispiele:** x86, x86-64 (Intel, AMD)

**Eigenschaften:**
- Viele komplexe Befehle
- Befehle unterschiedlicher Länge
- Weniger Befehle für gleiche Aufgabe
- Komplexere Hardware

```asm
; x86: String kopieren mit einem Befehl
REP MOVSB
```

### RISC (Reduced Instruction Set Computing)

**Beispiele:** ARM, AVR, MIPS, RISC-V

**Eigenschaften:**
- Wenige, einfache Befehle
- Befehle gleicher Länge (meist)
- Mehr Befehle für gleiche Aufgabe
- Einfachere, schnellere Hardware
- Load/Store-Architektur

```asm
; AVR: Muss Werte erst in Register laden
LDS R24, variable
ADD R24, R25
STS variable, R24
```

### Harvard vs. Von-Neumann

| Architektur    | Eigenschaften                           | Beispiele        |
|----------------|-----------------------------------------|------------------|
| Von-Neumann    | Gemeinsamer Speicher für Code und Daten | x86, ARM         |
| Harvard        | Getrennter Speicher für Code und Daten  | AVR, PIC, DSPs   |

**AVR (Arduino)** nutzt Harvard-Architektur:
- Flash für Programmcode
- SRAM für Daten
- Separate Busse = gleichzeitiger Zugriff möglich

---

## Allgemeine Befehlskategorien

### 1. Datentransfer

| Typ              | Beschreibung                    | Beispiel (x86)    |
|------------------|---------------------------------|-------------------|
| Register-Register| Wert zwischen Registern kopieren| `MOV EAX, EBX`    |
| Immediate-Register| Konstante in Register laden    | `MOV EAX, 42`     |
| Memory-Register  | Wert aus Speicher laden         | `MOV EAX, [addr]` |
| Register-Memory  | Wert in Speicher schreiben      | `MOV [addr], EAX` |
| Stack-Operationen| Push/Pop auf Stack              | `PUSH EAX`        |

### 2. Arithmetik

| Operation        | Beschreibung                    | Beispiel          |
|------------------|---------------------------------|-------------------|
| Addition         | Werte addieren                  | `ADD EAX, EBX`    |
| Subtraktion      | Werte subtrahieren              | `SUB EAX, 10`     |
| Multiplikation   | Werte multiplizieren            | `MUL EBX`         |
| Division         | Werte dividieren                | `DIV EBX`         |
| Inkrement        | Um 1 erhöhen                    | `INC EAX`         |
| Dekrement        | Um 1 verringern                 | `DEC EAX`         |

### 3. Logische Operationen

| Operation | Beschreibung           | Beispiel         |
|-----------|------------------------|------------------|
| AND       | Bitweises UND          | `AND EAX, 0xFF`  |
| OR        | Bitweises ODER         | `OR EAX, 0x01`   |
| XOR       | Exklusives ODER        | `XOR EAX, EAX`   |
| NOT       | Bitweise Negation      | `NOT EAX`        |
| Shift L   | Bits nach links        | `SHL EAX, 2`     |
| Shift R   | Bits nach rechts       | `SHR EAX, 1`     |
| Rotate    | Bits rotieren          | `ROL EAX, 1`     |

### 4. Vergleich und Sprünge

| Typ               | Beschreibung                      | Beispiel          |
|-------------------|-----------------------------------|-------------------|
| Vergleich         | Werte vergleichen (setzt Flags)   | `CMP EAX, 10`     |
| Unbedingter Sprung| Immer springen                    | `JMP label`       |
| Bedingter Sprung  | Springen wenn Bedingung erfüllt   | `JE label` (=)    |
| Funktionsaufruf   | Unterprogramm aufrufen            | `CALL funktion`   |
| Rückkehr          | Aus Unterprogramm zurück          | `RET`             |

### 5. Bedingte Sprünge (typisch)

| Befehl | Bedeutung                    | Flag-Bedingung |
|--------|------------------------------|----------------|
| JE/JZ  | Jump if Equal/Zero           | ZF = 1         |
| JNE/JNZ| Jump if Not Equal/Not Zero   | ZF = 0         |
| JG/JNLE| Jump if Greater (signed)     | ZF=0, SF=OF    |
| JL/JNGE| Jump if Less (signed)        | SF ≠ OF        |
| JA/JNBE| Jump if Above (unsigned)     | CF=0, ZF=0     |
| JB/JNAE| Jump if Below (unsigned)     | CF = 1         |
| JC     | Jump if Carry                | CF = 1         |
| JO     | Jump if Overflow             | OF = 1         |

---

## Status-/Flag-Register

Die CPU setzt Flags basierend auf dem Ergebnis von Operationen:

| Flag | Name      | Bedeutung                                    |
|------|-----------|----------------------------------------------|
| Z    | Zero      | Ergebnis ist Null                            |
| C    | Carry     | Übertrag bei unsigned Arithmetik             |
| N/S  | Negative  | Ergebnis ist negativ (höchstes Bit = 1)      |
| V/O  | Overflow  | Überlauf bei signed Arithmetik               |
| H    | Half Carry| Übertrag von Bit 3 zu Bit 4 (BCD-Arithmetik) |
| I    | Interrupt | Interrupts aktiviert/deaktiviert             |

---

## Assembler-Direktiven

Direktiven sind Anweisungen an den Assembler, keine CPU-Befehle:

```asm
; Daten definieren
.byte 0x42          ; 1 Byte
.word 0x1234        ; 2 Bytes
.long 0x12345678    ; 4 Bytes
.ascii "Hallo"      ; String ohne Null-Terminator
.asciz "Hallo"      ; String mit Null-Terminator

; Sektionen
.section .text      ; Code-Sektion
.section .data      ; Initialisierte Daten
.section .bss       ; Uninitialisierte Daten

; Symbole
.global main        ; Symbol global sichtbar machen
.equ CONST, 42      ; Konstante definieren

; Speicher reservieren
.space 100          ; 100 Bytes reservieren

; Ausrichtung
.align 4            ; Auf 4-Byte-Grenze ausrichten
```

---

# AVR-Assembler (Arduino)

## AVR-Architektur Übersicht

```
┌────────────────────────────────────────────────────────┐
│                    ATmega328P                          │
├────────────────────────────────────────────────────────┤
│  ┌──────────────┐    ┌──────────────┐                  │
│  │   32 Register │    │  ALU         │                  │
│  │   R0 - R31    │◄──►│  (Rechenwerk)│                  │
│  └──────────────┘    └──────────────┘                  │
│         │                   │                          │
│         ▼                   ▼                          │
│  ┌──────────────────────────────────┐                  │
│  │         Datenbus (8 Bit)          │                  │
│  └──────────────────────────────────┘                  │
│         │            │           │                     │
│         ▼            ▼           ▼                     │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │  SRAM    │  │  I/O     │  │  EEPROM  │             │
│  │  2 KB    │  │  Register│  │  1 KB    │             │
│  └──────────┘  └──────────┘  └──────────┘             │
│                                                        │
│  ┌──────────────────────────────────┐                  │
│  │    Flash (Programmbus, 16 Bit)   │                  │
│  │           32 KB                   │                  │
│  └──────────────────────────────────┘                  │
└────────────────────────────────────────────────────────┘
```

---

## AVR-Register

### Allzweck-Register (R0-R31)

```
R0  ─────────────────────────────────────  Temporär, MUL-Ergebnis (low)
R1  ─────────────────────────────────────  MUL-Ergebnis (high), immer 0
R2-R15  ─────────────────────────────────  Allgemein verwendbar
R16-R23  ────────────────────────────────  Immediate-Befehle möglich
R24-R25  ────────────────────────────────  Funktions-Rückgabewert
R26-R27  ────────────────────────────────  X-Register (Pointer)
R28-R29  ────────────────────────────────  Y-Register (Frame Pointer)
R30-R31  ────────────────────────────────  Z-Register (Pointer, PROGMEM)
```

### Pointer-Register

| Register | Name | Verwendung                          |
|----------|------|-------------------------------------|
| X (R26:R27) | XL:XH | Indirekte Adressierung           |
| Y (R28:R29) | YL:YH | Frame Pointer, Stack-Variablen   |
| Z (R30:R31) | ZL:ZH | Programmzugriff (LPM), Pointer   |

### Spezialregister

| Register | Adresse | Funktion                            |
|----------|---------|-------------------------------------|
| SREG     | 0x3F    | Status Register (Flags)             |
| SPH:SPL  | 0x3E:0x3D | Stack Pointer                     |
| PC       | -       | Program Counter (nicht direkt zugänglich) |

### SREG (Status Register)

```
Bit:    7    6    5    4    3    2    1    0
      ┌────┬────┬────┬────┬────┬────┬────┬────┐
SREG: │ I  │ T  │ H  │ S  │ V  │ N  │ Z  │ C  │
      └────┴────┴────┴────┴────┴────┴────┴────┘
        │    │    │    │    │    │    │    │
        │    │    │    │    │    │    │    └── Carry Flag
        │    │    │    │    │    │    └─────── Zero Flag
        │    │    │    │    │    └──────────── Negative Flag
        │    │    │    │    └───────────────── Overflow Flag
        │    │    │    └────────────────────── Sign Flag (N ⊕ V)
        │    │    └─────────────────────────── Half Carry (BCD)
        │    └──────────────────────────────── Bit Copy Storage
        └───────────────────────────────────── Interrupt Enable
```

---

## AVR-Befehlssatz

### Datentransfer

| Befehl      | Operanden    | Beschreibung                    | Zyklen |
|-------------|--------------|----------------------------------|--------|
| `MOV`       | Rd, Rr       | Register kopieren                | 1      |
| `MOVW`      | Rd, Rr       | Register-Paar kopieren           | 1      |
| `LDI`       | Rd, K        | Immediate laden (R16-R31)        | 1      |
| `LDS`       | Rd, k        | Direkt aus SRAM laden            | 2      |
| `STS`       | k, Rr        | Direkt in SRAM speichern         | 2      |
| `LD`        | Rd, X/Y/Z    | Indirekt laden                   | 2      |
| `ST`        | X/Y/Z, Rr    | Indirekt speichern               | 2      |
| `LDD`       | Rd, Y+q/Z+q  | Indirekt mit Offset              | 2      |
| `STD`       | Y+q/Z+q, Rr  | Indirekt mit Offset speichern    | 2      |
| `LD`        | Rd, X+/Y+/Z+ | Laden mit Post-Increment         | 2      |
| `LD`        | Rd, -X/-Y/-Z | Laden mit Pre-Decrement          | 2      |
| `LPM`       | Rd, Z        | Aus Programmspeicher laden       | 3      |
| `PUSH`      | Rr           | Register auf Stack               | 2      |
| `POP`       | Rd           | Register vom Stack               | 2      |
| `IN`        | Rd, A        | I/O-Register lesen               | 1      |
| `OUT`       | A, Rr        | I/O-Register schreiben           | 1      |

### Arithmetik

| Befehl      | Operanden    | Beschreibung                    | Zyklen |
|-------------|--------------|----------------------------------|--------|
| `ADD`       | Rd, Rr       | Addition                         | 1      |
| `ADC`       | Rd, Rr       | Addition mit Carry               | 1      |
| `ADIW`      | Rd, K        | Word addieren (R24-R31)          | 2      |
| `SUB`       | Rd, Rr       | Subtraktion                      | 1      |
| `SBC`       | Rd, Rr       | Subtraktion mit Carry            | 1      |
| `SBIW`      | Rd, K        | Word subtrahieren                | 2      |
| `SUBI`      | Rd, K        | Immediate subtrahieren           | 1      |
| `SBCI`      | Rd, K        | Immediate mit Carry subtr.       | 1      |
| `INC`       | Rd           | Inkrement (+1)                   | 1      |
| `DEC`       | Rd           | Dekrement (-1)                   | 1      |
| `MUL`       | Rd, Rr       | Unsigned Multiplikation          | 2      |
| `MULS`      | Rd, Rr       | Signed Multiplikation            | 2      |
| `NEG`       | Rd           | Zweierkomplement (Negation)      | 1      |

### Logik

| Befehl      | Operanden    | Beschreibung                    | Zyklen |
|-------------|--------------|----------------------------------|--------|
| `AND`       | Rd, Rr       | Bitweises UND                    | 1      |
| `ANDI`      | Rd, K        | UND mit Immediate                | 1      |
| `OR`        | Rd, Rr       | Bitweises ODER                   | 1      |
| `ORI`       | Rd, K        | ODER mit Immediate               | 1      |
| `EOR`       | Rd, Rr       | Exklusives ODER (XOR)            | 1      |
| `COM`       | Rd           | Einerkomplement (NOT)            | 1      |
| `LSL`       | Rd           | Logical Shift Left               | 1      |
| `LSR`       | Rd           | Logical Shift Right              | 1      |
| `ASR`       | Rd           | Arithmetic Shift Right           | 1      |
| `ROL`       | Rd           | Rotate Left durch Carry          | 1      |
| `ROR`       | Rd           | Rotate Right durch Carry         | 1      |
| `SWAP`      | Rd           | Nibbles tauschen                 | 1      |

### Vergleich und Test

| Befehl      | Operanden    | Beschreibung                    | Zyklen |
|-------------|--------------|----------------------------------|--------|
| `CP`        | Rd, Rr       | Compare (Rd - Rr)                | 1      |
| `CPC`       | Rd, Rr       | Compare mit Carry                | 1      |
| `CPI`       | Rd, K        | Compare mit Immediate            | 1      |
| `TST`       | Rd           | Test (AND mit sich selbst)       | 1      |

### Sprünge

| Befehl      | Operanden    | Beschreibung                    | Zyklen |
|-------------|--------------|----------------------------------|--------|
| `RJMP`      | k            | Relativer Sprung                 | 2      |
| `JMP`       | k            | Absoluter Sprung                 | 3      |
| `RCALL`     | k            | Relativer Funktionsaufruf        | 3      |
| `CALL`      | k            | Absoluter Funktionsaufruf        | 4      |
| `RET`       | -            | Return aus Funktion              | 4      |
| `RETI`      | -            | Return aus Interrupt             | 4      |
| `IJMP`      | -            | Indirekter Sprung (Z-Register)   | 2      |
| `ICALL`     | -            | Indirekter Call (Z-Register)     | 3      |

### Bedingte Sprünge

| Befehl      | Bedingung              | Flag-Test        |
|-------------|------------------------|------------------|
| `BREQ`      | Branch if Equal        | Z = 1            |
| `BRNE`      | Branch if Not Equal    | Z = 0            |
| `BRCS/BRLO` | Branch if Carry Set    | C = 1            |
| `BRCC/BRSH` | Branch if Carry Clear  | C = 0            |
| `BRMI`      | Branch if Minus        | N = 1            |
| `BRPL`      | Branch if Plus         | N = 0            |
| `BRVS`      | Branch if Overflow Set | V = 1            |
| `BRVC`      | Branch if Overflow Clr | V = 0            |
| `BRGE`      | Branch if >= (signed)  | N ⊕ V = 0        |
| `BRLT`      | Branch if < (signed)   | N ⊕ V = 1        |

### Bit-Operationen

| Befehl      | Operanden    | Beschreibung                    | Zyklen |
|-------------|--------------|----------------------------------|--------|
| `SBI`       | A, b         | Set Bit in I/O-Register          | 2      |
| `CBI`       | A, b         | Clear Bit in I/O-Register        | 2      |
| `SBIS`      | A, b         | Skip if Bit in I/O Set           | 1-3    |
| `SBIC`      | A, b         | Skip if Bit in I/O Clear         | 1-3    |
| `SBRS`      | Rr, b        | Skip if Bit in Register Set      | 1-3    |
| `SBRC`      | Rr, b        | Skip if Bit in Register Clear    | 1-3    |
| `BST`       | Rr, b        | Bit Store to T-Flag              | 1      |
| `BLD`       | Rd, b        | Bit Load from T-Flag             | 1      |
| `SEC`       | -            | Set Carry Flag                   | 1      |
| `CLC`       | -            | Clear Carry Flag                 | 1      |
| `SEI`       | -            | Set Interrupt Flag               | 1      |
| `CLI`       | -            | Clear Interrupt Flag             | 1      |

### Sonstige

| Befehl      | Beschreibung                              | Zyklen |
|-------------|-------------------------------------------|--------|
| `NOP`       | No Operation                              | 1      |
| `SLEEP`     | Sleep Mode aktivieren                     | 1      |
| `WDR`       | Watchdog Reset                            | 1      |
| `BREAK`     | Break (Debugger)                          | 1      |

---

## Adressierungsarten

### 1. Register-Direkt
```asm
ADD R16, R17        ; R16 = R16 + R17
```

### 2. Immediate (Konstante)
```asm
LDI R16, 42         ; R16 = 42
SUBI R16, 10        ; R16 = R16 - 10
```

### 3. Direkte Adressierung
```asm
LDS R16, 0x0100     ; R16 = SRAM[0x0100]
STS 0x0100, R16     ; SRAM[0x0100] = R16
```

### 4. Indirekte Adressierung
```asm
LD R16, X           ; R16 = SRAM[X]
ST X, R16           ; SRAM[X] = R16
```

### 5. Indirekt mit Post-Increment
```asm
LD R16, X+          ; R16 = SRAM[X]; X++
ST X+, R16          ; SRAM[X] = R16; X++
```

### 6. Indirekt mit Pre-Decrement
```asm
LD R16, -X          ; X--; R16 = SRAM[X]
ST -X, R16          ; X--; SRAM[X] = R16
```

### 7. Indirekt mit Displacement
```asm
LDD R16, Y+10       ; R16 = SRAM[Y + 10]
STD Y+10, R16       ; SRAM[Y + 10] = R16
```

---

## AVR I/O-Register (wichtige Auswahl)

### Port-Register (Arduino Uno)

| Port | DDRx   | PORTx  | PINx   | Arduino Pins    |
|------|--------|--------|--------|-----------------|
| B    | DDRB   | PORTB  | PINB   | D8-D13          |
| C    | DDRC   | PORTC  | PINC   | A0-A5           |
| D    | DDRD   | PORTD  | PIND   | D0-D7           |

**Verwendung:**
- `DDRx`: Data Direction Register (0=Input, 1=Output)
- `PORTx`: Ausgabe oder Pull-up aktivieren
- `PINx`: Eingabe lesen

### I/O-Adressen

| Register | I/O-Adresse | SRAM-Adresse |
|----------|-------------|--------------|
| PINB     | 0x03        | 0x23         |
| DDRB     | 0x04        | 0x24         |
| PORTB    | 0x05        | 0x25         |
| PIND     | 0x09        | 0x29         |
| DDRD     | 0x0A        | 0x2A         |
| PORTD    | 0x0B        | 0x2B         |
| SREG     | 0x3F        | 0x5F         |

---

## Praktische Beispiele

### LED Blinken (Pin 13 = PB5)

```asm
.include "m328pdef.inc"

.org 0x0000
    RJMP main

main:
    ; PB5 als Ausgang setzen (DDRB Bit 5 = 1)
    SBI DDRB, 5

loop:
    ; LED an (PORTB Bit 5 = 1)
    SBI PORTB, 5
    RCALL delay

    ; LED aus (PORTB Bit 5 = 0)
    CBI PORTB, 5
    RCALL delay

    RJMP loop

; Verzögerung ca. 500ms bei 16MHz
delay:
    LDI R18, 41
outer:
    LDI R19, 150
middle:
    LDI R20, 128
inner:
    DEC R20
    BRNE inner
    DEC R19
    BRNE middle
    DEC R18
    BRNE outer
    RET
```

### Button lesen (Pin 2 = PD2)

```asm
.include "m328pdef.inc"

main:
    ; PD2 als Eingang mit Pull-up
    CBI DDRD, 2         ; Eingang
    SBI PORTD, 2        ; Pull-up aktivieren

    ; PB5 als Ausgang (LED)
    SBI DDRB, 5

loop:
    ; Button gedrückt? (LOW = gedrückt bei Pull-up)
    SBIC PIND, 2        ; Skip if Bit Clear
    RJMP led_off

led_on:
    SBI PORTB, 5
    RJMP loop

led_off:
    CBI PORTB, 5
    RJMP loop
```

### Schleife (for i = 0 to 10)

```asm
    LDI R16, 0          ; i = 0

loop:
    ; ... hier kommt der Schleifenkörper ...

    INC R16             ; i++
    CPI R16, 10         ; Compare mit 10
    BRNE loop           ; Weiter wenn i != 10
```

### If-Else

```asm
; if (R16 > 100) { R17 = 1 } else { R17 = 0 }

    CPI R16, 101        ; Compare R16 mit 101
    BRSH greater        ; Branch if Same or Higher (unsigned)

    ; else: R16 <= 100
    LDI R17, 0
    RJMP endif

greater:
    ; R16 > 100
    LDI R17, 1

endif:
    ; weiter...
```

### 16-Bit Addition

```asm
; R25:R24 = R25:R24 + R23:R22

    ADD R24, R22        ; Low-Bytes addieren
    ADC R25, R23        ; High-Bytes mit Carry addieren
```

### Array durchlaufen

```asm
.section .data
array:  .byte 1, 2, 3, 4, 5
len:    .byte 5

.section .text
    ; Z-Pointer auf Array setzen
    LDI ZL, lo8(array)
    LDI ZH, hi8(array)

    LDI R18, 5          ; Länge
    LDI R17, 0          ; Summe

sum_loop:
    LD R16, Z+          ; Element laden, Z++
    ADD R17, R16        ; Zur Summe addieren
    DEC R18
    BRNE sum_loop

    ; R17 enthält jetzt die Summe
```

---

## Inline-Assembler in Arduino

### Einfache Syntax

```c
void setup() {
    // LED Pin 13 als Ausgang
    asm("sbi 0x04, 5");  // DDRB Bit 5 setzen
}

void loop() {
    asm("sbi 0x05, 5");  // PORTB Bit 5 = 1 (LED an)
    delay(500);
    asm("cbi 0x05, 5");  // PORTB Bit 5 = 0 (LED aus)
    delay(500);
}
```

### Erweiterte Syntax mit Variablen

```c
void setup() {
    Serial.begin(9600);
}

void loop() {
    uint8_t a = 10;
    uint8_t b = 20;
    uint8_t result;

    // result = a + b in Assembler
    asm volatile (
        "add %0, %1"        // Befehl
        : "=r" (result)     // Output: result in beliebigem Register
        : "r" (a), "0" (b)  // Input: a in Register, b im selben wie result
    );

    Serial.println(result);  // 30
    delay(1000);
}
```

### Syntax-Erklärung

```c
asm volatile (
    "Assembler-Code"
    : Output-Operanden      // Optional
    : Input-Operanden       // Optional
    : Clobber-Liste         // Optional
);
```

**Constraint-Zeichen:**
| Zeichen | Bedeutung                              |
|---------|----------------------------------------|
| `r`     | Beliebiges Register (R0-R31)           |
| `d`     | R16-R31 (für LDI etc.)                 |
| `w`     | R24-R31 (für ADIW/SBIW)                |
| `a`     | Pointer-Register (X, Y, Z)             |
| `I`     | 6-Bit positive Konstante (0-63)        |
| `M`     | 8-Bit Konstante (0-255)                |
| `=`     | Write-only (Output)                    |
| `+`     | Read-Write                             |

### Praktisches Beispiel: Schnelles Port-Toggle

```c
// Normaler Arduino-Code (langsam)
void toggleSlow() {
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
}

// Mit Inline-Assembler (schnell)
void toggleFast() {
    asm volatile (
        "sbi 0x05, 5 \n\t"  // PORTB |= (1 << 5)
        "cbi 0x05, 5"       // PORTB &= ~(1 << 5)
    );
}

// Noch schneller: Pin-Toggle Register
void toggleFastest() {
    asm volatile ("sbi 0x03, 5");  // PINB = (1 << 5) togglet PB5
}
```

---

## C zu Assembler: Kompilierung anschauen

### Mit avr-gcc

```bash
# Assembler-Output generieren
avr-gcc -S -mmcu=atmega328p -O1 programm.c -o programm.s

# Mit C-Code als Kommentare
avr-gcc -S -mmcu=atmega328p -O1 -fverbose-asm programm.c -o programm.s

# Disassembly einer kompilierten .elf
avr-objdump -d programm.elf > programm.lst
```

### Beispiel-Output

**C-Code:**
```c
int addiere(int a, int b) {
    return a + b;
}
```

**AVR-Assembler:**
```asm
addiere:
    ; Parameter: a in R25:R24, b in R23:R22
    ; Rückgabe: R25:R24
    ADD R24, R22        ; Low-Bytes addieren
    ADC R25, R23        ; High-Bytes mit Carry
    RET
```

---

## Debugging-Tipps

### Serial-Output aus Assembler

```asm
; USART initialisieren (9600 Baud bei 16MHz)
    LDI R16, 103            ; UBRR = 16000000/16/9600 - 1
    STS UBRR0L, R16
    LDI R16, (1<<TXEN0)
    STS UCSR0B, R16

; Zeichen senden
send_char:
    LDS R17, UCSR0A
    SBRS R17, UDRE0         ; Warten bis Buffer frei
    RJMP send_char
    STS UDR0, R16           ; Zeichen senden
    RET
```

### Simulator nutzen

- **Microchip Studio** (ehemals Atmel Studio): Vollständiger AVR-Simulator
- **SimulIDE**: Open-Source Simulator mit GUI
- **QEMU**: AVR-Emulation möglich

---

## Zusammenfassung: AVR vs. x86

| Aspekt           | AVR                      | x86                        |
|------------------|--------------------------|----------------------------|
| Architektur      | 8-Bit RISC, Harvard      | CISC, Von-Neumann          |
| Register         | 32 × 8-Bit               | 8-16 Allzweck + viele mehr |
| Befehlslänge     | 16/32 Bit (fest)         | 1-15 Bytes (variabel)      |
| Speicherzugriff  | Load/Store               | Operand kann Speicher sein |
| Endianness       | Little Endian            | Little Endian              |
| Komplexität      | Einfach, überschaubar    | Sehr komplex               |
