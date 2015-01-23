///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que crea una secuencia de 8 Leds conectados  //
// al puerto B (pata 6 a la 13), la velocidad del cristal de cuarzo esta     //
// configurada a 4MHz                                                        //
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
    TRISB = 0;                  // Puerto B como salida
    ANSELB = 0;                 // Puerto B como Digital
    PORTB = 0;                  // Inicializa el Puerto B en 0

    while(1){                   // Inicio del código a ejecutar constantemente
        for(int i = 1; i <= 128; i*=2){

                                // Este ciclo toma el valor inicial en la variable i, es decir, 1
                                // y lo multiplica en cada iteración por 2, y le permite llegar
                                // a un valor máximo de 128, teniendo una secuencia asi:
                                // 1, 2, 4, 8, 16, 32, 64, 128 que representada en bits seria esta:
                                // 1   = 00000001
                                // 2   = 00000010
                                // 4   = 00000100
                                // 8   = 00001000
                                // 16  = 00010000
                                // 32  = 00100000
                                // 64  = 01000000
                                // 128 = 10000000

            PORTB = i;          // aquí se escribe el valor que hay en la variable
                                // al puerto B, para que así encienda el led correspondiente
            __delay_ms(200);    // se da un retardo de 200 milisegundos (0.2 segundos)
                                // para que se pueda apreciar el cambio
        }
    }
}