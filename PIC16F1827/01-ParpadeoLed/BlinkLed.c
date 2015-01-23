///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que enciende y apaga un Led conectado al pin //
// RA0 (pata 17).                                                            //
///////////////////////////////////////////////////////////////////////////////

// 'C' source line config statements

#include <xc.h>

// Bits de configuración

// CONFIG1
#pragma config FOSC = XT        // Oscilador con cristal de cuarzo de 4MHz conectado en los pines 15 y 16
#pragma config WDTE = OFF       // Perro guardian (WDT deshabilidado)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT deshabilidado)
#pragma config MCLRE = ON       // Master clear habilitado (pin reset)
#pragma config CP = OFF         // Protección contra lectura de código
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#define _XTAL_FREQ 4000000      // Oscilador a 4MHz

void main(){
    TRISA = 0;                  // Puerto A como salida
    ANSELA = 0;                 // Puerto A Digital
    PORTA = 0;                  // Inicializa el Puerto A en 0

    while(1){                   // Programa que se ejecura continuamente
        PORTAbits.RA0 = 1;      // Bit 0 del puerto A en uno, enciende el Led
        __delay_ms(500);        // Retardo de 500 milisegundos (1/2 segundo)
        PORTAbits.RA0 = 0;      // Bit 0 del puerto A en cero, apaga el Led
        __delay_ms(500);        // Retardo de 500 milisegundos (1/2 segundo)
    }
}
