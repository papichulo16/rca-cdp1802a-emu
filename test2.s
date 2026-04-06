; -------------------------------------------------------------------
; RCA 1802 Cross-Assembler Test File
; Function: Blinks the Q line and increments a counter in memory.
; -------------------------------------------------------------------

COUNT_L:     EQU     050H    ; Define low byte of memory address for counter
COUNT_H:     EQU     000H    ; Define high byte of memory address for counter
INIT_VAL:    EQU     00H     ; Initial value for counter

            ORG     0000H   ; Set program start origin

START:
            ; --- Initialize Counter Memory Pointer ---
            LDI     COUNT_L ; Load D with lower byte of COUNT address
            PLO     R3      ; Put D in R3.0 (Low byte of Register 3)
            LDI     COUNT_H ; Load D with upper byte of COUNT address
            PHI     R3      ; Put D in R3.1 (High byte of Register 3)
            
            ; --- Initialize Counter Value ---
            LDI     INIT_VAL; Load immediate value (00H) into D
            STR     R3      ; Store D into memory at address pointed to by R3

LOOP:
            ; --- Turn Q LED ON ---
            SEQ             ; Set Q line high
            LDI     0FFH    ; Load delay value into D
            PLO     R1      ; Set up loop counter in R1.0

WAIT1:      DEC     R1      ; Decrement R1
            GLO     R1      ; Get low byte of R1 back into D to check zero flag
            BNZ     WAIT1   ; Branch Not Zero - loop until R1.0 is 0

            ; --- Turn Q LED OFF ---
            REQ             ; Reset Q line low
            LDI     0FFH    ; Load delay value into D
            PLO     R1      ; Set up loop counter in R1.0

WAIT2:      DEC     R1      ; Decrement R1
            GLO     R1      ; Get low byte of R1 back into D
            BNZ     WAIT2   ; Branch Not Zero - loop until R1.0 is 0

            ; --- Increment the Counter ---
            LDN     R3      ; Load D with the byte at address in R3
            ADI     01H     ; Add immediate 01H to D
            STR     R3      ; Store D back to address in R3

            ; --- Repeat ---
            BR      LOOP    ; Branch unconditionally back to LOOP
