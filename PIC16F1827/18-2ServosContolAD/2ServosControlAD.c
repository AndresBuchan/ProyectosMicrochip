///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que genera una señal PWM por el pin CCP3     //
// (pata 2), para controlar la posición de un servo motor por medio de una   //
// señal Analoga.                                                            //
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
void pulso_pwm1(int pulso1);
void pulso_pwm2(int pulso2);
void convertir_AD(char canal);

// Declaracion de variables globales
    int conversion = 0;         // Aquí se almacenara el resulatado de la conversión AD

void main(){

    // Configuración del Puerto
    TRISA = 224;                // Puerto A como salida
    ANSELA = 0;                 // Puerto A Digital
    TRISB = 255;                // Puerto B entrada
    ANSELB = 6;                 // RB1 y RB2 como entradas analogas

    // Configuración del conversor Analogo/Digital
    ADCON0 = 45;                // Habilito la conversión Analogo/Digital y
                                // selecciono el canal AN11
    ADCON1 = 192;               // Alineo el resultado de la conversión a la derecha,
                                // Fosc/8
                                // Selecciono como voltaje de referencia la alimentación 5VDC del micro
    ADRESL = 0;                 // Limpio ADRESL
    ADRESH = 0;                 // Limpio ADRESH

    // Configuración del Timer2
    T2CON = 3;                  // Preescala de 64, apago el Timer 2
    PR2 = 78;                   // Periodo para la señal PWM (Importante para su calculo)
    TMR2 = 0;                   // Limpio el contador Timer 2
    PIR1bits.TMR2IF = 0;        // Pongo en cero la bandera de interrupción del Timer 2
    T2CONbits.TMR2ON = 1;       // Inicializo el Timer 2

    // Configuración del Timer4
    T4CON = 3;                  // Preescala de 64, apago el Timer 4
    PR4 = 78;                   // Periodo para la señal PWM (Importante para su calculo)
    TMR4 = 0;                   // Limpio el contador Timer 4
    PIR3bits.TMR4IF = 0;        // Pongo en cero la bandera de interrupción del Timer 4
    T4CONbits.TMR4ON = 1;       // Inicializo el Timer 4

    // Configuración PWM1
    CCP3CON = 28;               // Activo el modo PWM y pongo en 1 en los bits menos significativos
                                // para el ancho del pulso
    CCPR3L = 39;                // Pongo 0100111 en los bits mas significativos para el ancho de pulso
    CCPTMRS0bits.C3TSEL = 1;    // Asocio la salida PWM CCP3 al Timer 4

    // Configuracion PWM2
    CCP4CON = 28;               // Activo el modo PWM y pongo en 1 en los bits menos significativos
                                // para el ancho del pulso
    CCPR4L = 39;                // Pongo 0100111 en los bits mas significativos para el ancho de pulso
    CCPTMRS0bits.C4TSEL = 0;    // Asocio la salida PWM CCP3 al Timer 2

    // Declaración de variables
    unsigned int pulso1 = 0;
    unsigned int pulso2 = 0;

    while(1){                  // Programa que se ejecura continuamente
        convertir_AD(11);                       // Llamo la función que convierte el dato de analogo a digital
                                                // para que combierta la señal en AN11 (RB1)
        pulso1 = ((conversion * 31)/1000)+8;    // Hago la operación para que se almacene en la variable
                                                // pulso el valor correspondiente a la posición del servo
                                                // minimo 8, maximo 39
        pulso_pwm1(pulso1);                     // Llamo la función que genera el ancho de pulso y le mando
                                                // el valor almacenado en pulso

        convertir_AD(10);                       // Llamo la función que convierte el dato de analogo a digital
                                                // para que combierta la señal en AN10 (RB2)
        pulso2 = ((conversion * 31)/1000)+8;    // Hago la operación para que se almacene en la variable
                                                // pulso el valor correspondiente a la posición del servo
                                                // minimo 8, maximo 39
        pulso_pwm2(pulso2);                     // Llamo la función que genera el ancho de pulso y le mando
                                                // el valor almacenado en pulso
    }
}

// Funciones

// Pasar el valor del ancho de pulso
void pulso_pwm1(int pulso1){                       // Esta función pasa el valor para el ancho del pulso a los registros encargado
    CCPR3L = ((pulso1 & 0x03FC)>>2);                         // Escribo los bits mas significativos para el ancho de pulso
    CCP3CON = (CCP3CON & 0xCF) | ((pulso1 & 0x0003)<<4);     // Escribo los bits menos significativos para el ancho de pulso
}

void pulso_pwm2(int pulso2){                       // Esta función pasa el valor para el ancho del pulso a los registros encargado
    CCPR4L = ((pulso2 & 0x03FC)>>2);                         // Escribo los bits mas significativos para el ancho de pulso
    CCP4CON = (CCP4CON & 0xCF) | ((pulso2 & 0x0003)<<4);     // Escribo los bits menos significativos para el ancho de pulso
}

// Conversion Analoga/Digital
void convertir_AD(char canal){
    ADCON0bits.CHS = canal;                    // Selecciono el canal
    ADCON0bits.ADON = 1;                       // Habilito la conversión AD
    __delay_ms(5);                             // Doy un retardo para la adquisición del dato
    ADCON0bits.GO_nDONE = 1;                   // Inicio el proceso de conversión AD
                                               // este cambiara a 0 automaticamente cuando
                                               // la conversion termine

    while(ADCON0bits.GO_nDONE){                // Espera a que la conversion termine
        continue;
    }

    conversion = ((ADRESH << 8) + ADRESL);     // Paso el valor del resulado en ADRESH y ADRESL
                                               // a la variable conversion
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
