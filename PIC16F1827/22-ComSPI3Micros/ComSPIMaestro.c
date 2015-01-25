///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que permite comunicación SPI entre dos o mas //
// microcontroladores.                                                       //
// Programa para microcontrolador Maestro (Master)                           //
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
#define Esclavo1 RB6            // Nombramos RB6 como Esclavo1, para activar la comunicación con este.
#define Esclavo2 RB7            // Nombramos RB7 como Esclavo2, para activar la comunicación con este.

void main(){

    // Configuración de puertos
    TRISA = 255;                // Puerto A como entrada
    ANSELA = 0;                 // Puerto A digital
    TRISB = 2;                  // Puerto B como salida
                                // SDI (RB1) entrada de datos SPI
                                // SDO (RB2) salida de datos SPI
                                // SCK (RB4) reloj SPI (salida)
    ANSELB = 0;                 // Puerto B digital

    // Configuración SPI
    SSP1STAT = 128;             // Muestra del dato al final de cada flanco
    SSP1CON1 = 33;              // Habilitación de pines para comunicación SPI
                                // Modo SPI maestro con Fosc/4 (para este caso Fosc/16 = 250KHz)

    // Declaración de variables
    char dato = 0;              // Declaro la variable dato en 0
    char dato2 = 9;             // Declaro la variable dato2 en 9

    // Inicialización de esclavos
    Esclavo1 = 1;
    Esclavo2 = 1;
    
    while(1){                   // Programa que se ejecura continuamente
        Esclavo1 = 0;                   // Habilito la transmisión para el esclavo 1
        Esclavo2 = 1;                   // Deshabilito la transmisión para el esclavo 2
        SSP1BUF = dato << 1;            // Le paso la variable dato al buffer de transmisión
        __delay_ms(200);                // Doy un retardo
        Esclavo1 = 1;                   // Deshabilito la transmisión para el esclavo 1
        Esclavo2 = 1;                   // Deshabilito la transmisión para el esclavo 2
        while(!SSP1STATbits.BF){        // Mientras la bandera de transmisión este en 0
            continue;                   // Espero en este clico hasta que sea 1 e indique su finalización
        }
        __delay_ms(200);                // Doy un retardo
        Esclavo2 = 0;                   // Habilito la transmisión para el esclavo 2
        Esclavo1 = 1;                   // Deshabilito la transmisión para el esclavo 1
        SSP1BUF = dato2 << 1;           // Le paso la variable dato2 al buffer de transmisión
        __delay_ms(200);                // Doy un retardo
        Esclavo2 = 1;                   // Deshabilito la transmisión para el esclavo 2
        Esclavo1 = 1;                   // Deshabilito la transmisión para el esclavo 1
        while(!SSP1STATbits.BF){        // Mientras la bandera de transmisión este en 0
            continue;                   // Espero en este clico hasta que sea 1 e indique su finalización
        }
        __delay_ms(200);                // Doy un retardo
        dato++;                         // Incremento en una unidad dato
        if(dato > 9){                   // Compruebo que la variable dato no haya superado su limite superior
            dato = 0;                   // Si es así la hago 0
        }
        if(dato2 <= 0){                 // Compruebo que la variable dato2 no haya superado su limite inferior
            dato2 = 10;                 // Su es así la hago 10
        }
        dato2--;                        // Decremento en una unidad dato2
    }
}