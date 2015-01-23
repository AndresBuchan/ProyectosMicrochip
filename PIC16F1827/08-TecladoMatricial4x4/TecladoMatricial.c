///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que muestra el numero presionado en un       //
// teclado matricial 4x4 en un display de siete segmentos                    //
///////////////////////////////////////////////////////////////////////////////

// 'C' source line config statements

#include <xc.h>

// Bits de configuración

// CONFIG1
#pragma config FOSC = XT        // Oscilador con cristal de cuarzo de 4MHz conectado en los pines 15 y 16
#pragma config WDTE = OFF       // Perro guardian (WDT deshabilidado)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT deshabilidado)
#pragma config MCLRE = ON       // Master clear habilitado, pin reset
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
    TRISA = 240;                // Puerto A como salida, aqui va conectado el display de siete segmentos
    ANSELA = 0;                 // Puerto A Digital
    TRISB = 15;                 // Configuro RB0, RB1, RB2 y RB3 como entradas y
                                // RB4, RB5, RB6 y RB7 como salidas
                                // En este puerto ira conectado el teclado matricial 4x4
    ANSELB = 0;                 // Puerto B Digital
    PORTA = 0;                  // Inicializo Puerto A en 0, se mostrara el 0 en el display
    PORTB = 0;                  // Limpio el puerto A

    while(1){                   // Inicio del código a ejecutar constantemente

        // Verificación de tecla
        // Sabemos que en estado normal las entradas del puerto B van a estar leyendo un 1 logico
        // asi que por cada salida del puerto B vamos a ir alternando un 0 por cada pin
        // una vez en una de las entradas leamos un 0 vamos a saber cual tecla ha sido presionada
        // pues tabularemos la salida y entrada en cuestión.

        PORTB = 239;                                            // enviamos un cero a RB4
        if(PORTBbits.RB0 == 0 & PORTBbits.RB4 == 0){            // Preguntamos por el estado de cada entrada
                                                                // con respecto a la salida que estamos enviando
            PORTA = 1;                                          // Y enviamos el resultado al puerto A
        }else if(PORTBbits.RB1 == 0 & PORTBbits.RB4 == 0){
            PORTA = 4;
        }else if(PORTBbits.RB2 == 0 & PORTBbits.RB4 == 0){
            PORTA = 7;
        }else if(PORTBbits.RB3 == 0 & PORTBbits.RB4 == 0){
            PORTA = 10;
        }

        PORTB = 223;                                            // enviamos un cero a RB5
        if(PORTBbits.RB0 == 0 & PORTBbits.RB5 == 0){
            PORTA = 2;
        }else if(PORTBbits.RB1 == 0 & PORTBbits.RB5 == 0){
            PORTA = 5;
        }else if(PORTBbits.RB2 == 0 & PORTBbits.RB5 == 0){
            PORTA = 8;
        }else if(PORTBbits.RB3 == 0 & PORTBbits.RB5 == 0){
            PORTA = 0;
        }

        PORTB = 191;                                            // enviamos un cero a RB6
        if(PORTBbits.RB0 == 0 & PORTBbits.RB6 == 0){
            PORTA = 3;
        }else if(PORTBbits.RB1 == 0 & PORTBbits.RB6 == 0){
            PORTA = 6;
        }else if(PORTBbits.RB2 == 0 & PORTBbits.RB6 == 0){
            PORTA = 9;
        }else if(PORTBbits.RB3 == 0 & PORTBbits.RB6 == 0){
            PORTA = 11;
        }

        PORTB = 127;                                            // enviamos un cero a RB7
        if(PORTBbits.RB0 == 0 & PORTBbits.RB7 == 0){
            PORTA = 12;
        }else if(PORTBbits.RB1 == 0 & PORTBbits.RB7 == 0){
            PORTA = 13;
        }else if(PORTBbits.RB2 == 0 & PORTBbits.RB7 == 0){
            PORTA = 14;
        }else if(PORTBbits.RB3 == 0 & PORTBbits.RB7 == 0){
            PORTA = 15;
        } 
    }
}

