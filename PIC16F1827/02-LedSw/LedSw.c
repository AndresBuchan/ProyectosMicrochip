///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que enciende y apaga un led al pulsar un     //
// switch, la velociadad de cristal de cuarzo esta configurada a 4MHz        //
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
#define switch RA0              // Nombro RA0 como switch
#define led RB0                 // Nombro RB0 como led

void main(){
    TRISA = 255;
    ANSELA = 0;
    TRISB = 0;
    ANSELB = 0;
    PORTB = 0;

    while(1){                   // Inicio del código a ejecutar constantemente
        if(!switch){            // Se pregunta si el switch esta pulsado
            while(!switch){     // Si esta pulsado entra a este ciclo antirrebote
                continue;       // es decir, evita que durante la presión del boton
                                // se genere ruido que el micro pueda interpretar
                                // como varios pulsos
            }                   // este ciclo durara hasta que se suelte el boton
            led = !led;         // cambio el estado en led, si esta encendido lo apago
                                // y si esta apagado lo enciendo

        }
    }
}