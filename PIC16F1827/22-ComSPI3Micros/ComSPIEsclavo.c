///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que permite comunicación SPI entre dos o mas //
// microcontroladores.                                                       //
// Programa para microcontrolador Esclavo1 y 2 (Slave)                       //
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
#define Dato PORTA

void main(){

    // Configuración de puertos
    TRISA = 240;                // Puerto A como salida, para conectar un display
                                // 7 segmentos
    ANSELA = 0;                 // Puerto A digital
    TRISB = 18;                 // Puerto B como salida
                                // SDI (RB1) entrada de datos SPI
                                // SDO (RB2) salida de datos SPI
                                // SCK (RB4) reloj SPI (entrada)
    ANSELB = 0;                 // Puerto B digital

    // Configuración SPI
    SSP1STAT = 128;             // Muestra del dato al final de cada flanco
    SSP1CON1 = 36;              // Habilitación de pines para comunicación SPI
                                // Modo SPI esclavo con control por reloj y habilitación por pin ss
    
    while(1){                   // Programa que se ejecura continuamente
       while(!SSP1STATbits.BF){     // Estoy pendiente de la recepción de un dato
                continue;           // Mientras la bandera este 0 espero
        }                           // Cuando sea 1 continuo
        Dato = SSP1BUF;             // El dato recibido lo paso inmediatamente a Dato
                                    // Es decir, lo escribo en el puerto A
        __delay_ms(200);            // Doy un retardo
    }
}