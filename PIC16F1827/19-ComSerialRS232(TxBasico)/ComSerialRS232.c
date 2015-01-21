///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que envia los numeros del 0 al 9 a traves de //
// comunicación serial RS232 a 9600 baudios							         //
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

    // Configuración de puertos
    TRISB = 0;                  // Puerto B como salida
    ANSELB = 0;                 // Puerto B Digital
    
    // Configuración de comunicación
    TXSTA = 36;                 // Habilitamos la trasnmisión y ponemos en 1 el bit
                                // BRGH que configura una constante igual a 16 para
                                // para el calculo de la velocidad de comunicación
                                // si se pone en 0 la constante sera de 64.
    SPBRG = 25;                 // El valor 25 viene de la siguiente formula:

    /*
    
    SPBRG = (Frecuencia Oscilador / ( Constante BRGH * velociadad baudios)) - 1

    Entonces :

    SPBRG = (4000000 / ( 16 * 9600 )) - 1
    SPBRG = 25

    */     

    RCSTA = 128;                // Habilitamos la comunicación serial
    

    while(1){                   // Programa que se ejecura continuamente

        for(unsigned char i = 0; i < 10; i++){  // Enviaremos los numeros del 0 al 9
           TXREG = i + 48;                      // Escribimos el dato a enviar en TXREG
                                                // Como la terminal los lee en ASCII, le sumamos 48
            while(!TRMT){                       // Mientras se este transmitiendo
                continue;                       // estaremos en este ciclo
            }
            __delay_ms(200);                     // Damos un retardo
        }   
    }
}