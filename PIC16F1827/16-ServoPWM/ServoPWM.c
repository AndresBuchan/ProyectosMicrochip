///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que genera una señal PWM por el pin CCP3     //
// (pata 2), para controlar la posición de un servo motor.                   //
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

// Declaración de funciones
void pulso_pwm(int pulso);

void main(){

    // Configuración del Puerto

    TRISA = 247;                // Puerto A como salida
    ANSELA = 23;                // Puerto A Digital

    // Configuración del Timer

    T2CON = 3;                  // Preescala de 64, apago el Timer2
    PR2 = 78;                   // Periodo para la señal PWM (Importante para su calculo)
    TMR2 = 0;                   // Limpio el contador Timer 2
    PIR1bits.TMR2IF = 0;        // Pongo en cero la bandera de interrupción del Timer 2
    T2CONbits.TMR2ON = 1;       // Inicializo el Timer 2

    // Configuración PWM

    CCP3CON = 28;               // Activo el modo PWM y pongo en 1 en los bits menos significativos
                                // para el ancho del pulso
    CCPR3L = 39;                // Pongo 0100111 en los bits mas significativos para el ancho de pulso
    CCPTMRS0bits.C3TSEL = 0;    // Asocio la salida PWM CCP3 al Timer 2

    while(1){                  // Programa que se ejecura continuamente
        pulso_pwm(39);         // 180 grados
        __delay_ms(1000);      // Espero 1 segundo en esa posición
        pulso_pwm(23);         // 90 grados
        __delay_ms(1000);      // Espero 1 segundo en esa posición
        pulso_pwm(8);          // 0 grados
        __delay_ms(1000);      // Espero 1 segundo en esa posición
    }
}

// Funciones

// Pasar el valor del ancho de pulso
void pulso_pwm(int pulso){                       // Esta función pasa el valor para el ancho del pulso a los registros encargado
    CCPR3L = ((pulso & 0x03FC)>>2);                         // Escribo los bits mas significativos para el ancho de pulso
    CCP3CON = (CCP3CON & 0xCF) | ((pulso & 0x0003)<<4);     // Escribo los bits menos significativos para el ancho de pulso
}

/*
 Los calculos necesarios para hallar el periodo, el ancho del pulso, el ciclo de dureza (duty cicle) y
 la frecuencia son los siguientes:

 * Periodo del PWM = (PR2 + 1) * 4 * 1/Fosc * preecala del Timer2

 Fosc es la frecuencia del cristal, para este caso 4MHz, entonces el calculo queda:

 Periodo del PWM = (78 + 1) * 4 * 1/4000000 * 64 = 5ms (milisegundos)

 * Ancho del pulso = El valor en los registros CCPRL3L:CCP3CON(4:5)(valor a bits) * 1/Fosc * preescala del Timer2

 El resultado suponiendo que vamos a ubicar el servo a 180

 Ancho del pulso = 39 * 1/4000000 * 64 = 624us (microsegundos)

 * Ciclo de dureza = (El valor en los registros CCPRL3L:CCP3CON(4:5)(valor a bits) / (4 * (PR2 + 1))) * 100

 Suponiendo nuevamente que vamos a ubicar el servo a 180

 Ciclo de dureza = (39 / (4 * (78 + 1))) * 100 = 12,3%

 * Frecuencia = 1/periodo del PWM

 Frecuencia  = 1/5ms = 0,2KHz = 200Hz

 */