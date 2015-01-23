///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que convierte la señal Analoga presente en   //
// el canal AN0 (pata 17) y según su valor muestra un numero del 0 al 5 en   //
// un display de siete segmentos, la conversión es realizada a 10 bits       //
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

    // Configuración de entradas y salidas

    TRISA = 255;                // Puerto A como entradas
    ANSELA = 0x1F;              // Puerto A como Analogo
    TRISB = 0;                  // Puerto B como salida
    ANSELB = 0;                 // Puerto B como Digital

    // Configuración del conversor Analogo/Digital

    ADCON0 = 1;                 // Habilito la conversión Analogo/Digital y
                                // selecciono el canal AN0
    ADCON1 = 192;               // Alineo el resultado de la conversión a la derecha,
                                // Fosc/8
                                // Selecciono como voltaje de referencia la alimentación 5VDC del micro
    ADRESL = 0;                 // Limpio ADRESL
    ADRESH = 0;                 // Limpio ADRESH

    int conversion = 0;         // Aquí se almacenara el resulatado de la conversión AD

    while(1){                   // Programa que se ejecura continuamente

        ADCON0bits.CHS = 0;             // Selecciono el canal AN0
        ADCON0bits.ADON = 1;            // Habilito la conversión AD
        __delay_ms(5);                  // Doy un retardo para la adquisición del dato
        ADCON0bits.GO_nDONE = 1;        // Inicio el proceso de conversión AD
                                        // este cambiara a 0 automaticamente cuando
                                        // la conversion termine

        while(ADCON0bits.GO_nDONE){     // Espera a que la conversion termine
            continue;
        }
        
        conversion = ((ADRESH << 8) + ADRESL); // Paso el valor del resulado en ADRESH y ADRESL
                                               // a la variable conversion
        
        // El valor entregado en la variable "conversion" es un entero
        // entre 0 y 1023, puesto que la conversion es a 10 bits,
        // Como el voltaje de referencia es la alimentación, es decir,
        // 5V, para saber a cuantos voltios equivale el valor
        // en "conversion" se realiza la siguiente operación:

        // 5 (voltaje referencia)/ 1023 (valor maximo a 10 bits) = 0,0049

        // De este modo al leer el valor en la variable "conversion" podremos saber
        // que voltaje estamos recibiendo por la entrada analoga.

        // En el siguiente codigo se verifica que valor se encuentra en la
        // entrada AN0 pin 17 del 16F1827 y escribe un dato, en este caso
        // un numero del 0 al 5 en el puerto B para mostrar en un display
        // de siete segmentos conectado a un BCD 7448.

        if(conversion < 171){
            PORTB = 0;
        }else if(conversion < 342){
            PORTB = 1;
        }else if(conversion < 513){
            PORTB = 2;
        }else if(conversion < 684){
            PORTB = 3;
        }else if(conversion < 855){
            PORTB = 4;
        }else if(conversion < 1024){
            PORTB = 5;
        }

    }
}

