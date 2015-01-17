///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que genera una se�al PWM por el pin CCP3     //
// (pata 2). Dicha se�al aumenta y disminuye en el tiempo.                  //
///////////////////////////////////////////////////////////////////////////////

// 'C' source line config statements

#include <xc.h>

// Bits de configuraci�n

// CONFIG1
#pragma config FOSC = XT        // Oscilador con cristal de cuarzo de 4MHz conectado en los pines 15 y 16
#pragma config WDTE = OFF       // Perro guardian (WDT deshabilidado)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT deshabilidado)
#pragma config MCLRE = ON       // Master clear habilitado (pin reset)
#pragma config CP = OFF         // Protecci�n contra lectura de c�digo
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

    // Configuraci�n del Puerto

    TRISA = 247;                  // Puerto A como salida
    ANSELA = 23;                 // Puerto A Digital

    // Configuraci�n del Timer

    T2CON = 1;                  // Preescala de 4, apago el Timer2
    PR2 = 127;                  // Periodo para la se�al PWM (Importante para su calculo)
    TMR2 = 0;                   // Limpio el contador Timer 2
    PIR1bits.TMR2IF = 0;        // Pongo en cero la bandera de interrupci�n del Timer 2
    T2CONbits.TMR2ON = 1;       // Inicializo el Timer 2
    
    // Configuraci�n PWM

    CCP3CON = 60;               // Activo el modo PWM y pongo en 11 en los bits menos significativos
                                // para el ancho del pulso
    CCPR3L = 127;               // Pongo 1111111 en los bits mas significativos para el ancho de pulso
    CCPTMRS0bits.C3TSEL = 0;    // Asocio la salida PWM CCP3 al Timer 2

    while(1){                   							// Programa que se ejecura continuamente
        for(int i = 0; i < 512; i++){       				// En este ciclo incremento la variable i desde 0 hasta el 511,
                                            				// es decir, incremento el ancho del pulso gradualmente hasta
                                            				// el tama�o tama�o total del periodo de la se�al
            CCPR3L = ((i & 0x03FC)>>2);     				// Escribo los bits mas significativos para el ancho de pulso
            CCP3CON = (CCP3CON & 0xCF) | ((i & 0x0003)<<4); // Escribo los bits menos significativos para el ancho de pulso
            __delay_ms(1);                  				// Doy un retraso de 1 ms para que se pueda apreciar un poco el cambio
        }
        for(int i = 511; i > 0; i--){       				// En este otro ciclo, hago lo contrario, decremento el pulso desde lo
                                            				// maximo hasta lo minimo
            CCPR3L = ((i & 0x03FC)>>2);     				// Escribo los bits mas significativos para el ancho de pulso
            CCP3CON = (CCP3CON & 0xCF) | ((i & 0x0003)<<4); // Escribo los bits menos significativos para el ancho de pulso
            __delay_ms(1);                  				// Doy un retraso de 1 ms para que se pueda apreciar un poco el cambio
        }
    }
}

/*
 Los calculos necesarios para hallar el periodo, el ancho del pulso, el ciclo de dureza (duty cicle) y
 la frecuencia son los siguientes:

 * Periodo del PWM = (PR2 + 1) * 4 * 1/Fosc * preecala del Timer2

 Fosc es la frecuencia del cristal, para este caso 4MHz, entonces el calculo queda:

 Periodo del PWM = (127 + 1) * 4 * 1/4000000 * 4 = 512us (microsegundos)

 * Ancho del pulso = El valor en los registros CCPRL3L:CCP3CON(4:5)(valor a bits) * 1/Fosc * preescala del Timer2

 El resultado suponiendo que vamos a la mitad, es decir, 255, recordemos que el maximo para este caso es 511 queda:

 Ancho del pulso = 255 * 1/4000000 * 4 = 255us (microsegundos)

 * Ciclo de dureza = (El valor en los registros CCPRL3L:CCP3CON(4:5)(valor a bits) / (4 * (PR2 + 1))) * 100

 Suponiendo nuevamente que va a la mitad

 Ciclo de dureza = (255 / (4 * (127 + 1))) * 100 = 49,8%

 * Frecuencia = 1/periodo del PWM

 Frecuencia  = 1/512us = 0,00195MHz = 1,95KHz

 */