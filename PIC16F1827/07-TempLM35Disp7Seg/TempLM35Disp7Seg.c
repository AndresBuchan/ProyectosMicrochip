///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que lee la temperatura desde un sensor LM35  //
// y la muestra con la tecnica de multiplexación en 4 displays de 7 segmentos//
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

    TRISA = 240;                // Configuro RA0, RA1, RA2 y RA3 como salidas para controlar
                                // los transistores que habilitaran cada display
                                // RA4 se configura como entrada para conectar el sensor LM35
    TRISB = 0;                  // Puerto B como salida
    ANSELA = 16;                // Todo el puerto A como Digital, a excepción de RA4 que se
                                // configura como Analoga AN4
    ANSELB = 0;                 // Puerto B Digital
    PORTA = 0;                  // Inicializo Puerto A en 0
    PORTB = 0;                  // Inicializo Puerto B en 0

    // Configuración del conversor Analogo/Digital

    ADCON0 = 17;                // Habilito la conversión Analogo/Digital y
                                // selecciono el canal AN4
    ADCON1 = 192;               // Alineo el resultado de la conversión a la derecha,
                                // Fosc/8
                                // Selecciono como voltaje de referencia la alimentación 5VDC del micro
    ADRESL = 0;                 // Limpio ADRESL
    ADRESH = 0;                 // Limpio ADRESH

    int conversion = 0;         // Aquí se almacenara el resulatado de la conversión AD
    char temperatura = 0;        // Aquí se almacenara el valor de la temperatura
    char miles;                 // Almacenara los miles
    char centenas;              // Almacenara las centenas
    char decenas;               // Almacenara las decenas
    char unidades;              // Almacenara las unidades
    int aux = 0;                // Variable para almacenar datos temporalmente

    while(1){                   // Inicio del código a ejecutar constantemente

        // En este punto se toma el valor de la temperatura

        ADCON0bits.CHS = 4;             // Selecciono el canal AN4
        ADCON0bits.ADON = 1;            // Habilito la conversión AD
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

        // En el siguiente codigo toma el valor entregado por el sensor LM35 en
        // entrada AN4 pin 3 del 16F1827, teniendo en cuenta que por cada grado el
        // sensor entrega 10mV (mili voltios).
        // Una vez se hagan los calculos se mostrara el valor en 4 display
        // de siete segmentos conectados a un BCD 7448 y multiplaxos por transistores
        // controlados desde el puerto A.

        temperatura = conversion/2; // Divido el resultado de la conversión entre 2
                                    // y lo almaceno el la variable temperatura

        // Se pasa el valor en temperatura para que sea mostrado en los displays
        // digito a digito

        aux = temperatura;      // Guardo en auxiliar el valor en temperatura
        centenas = 0;           // Pongo en cero todos los digitos
        decenas = 0;
        unidades = 0;
        miles = 0;

        while(aux > 999){       // Mientras el valor sea superior a 999
            aux -= 1000;        // Le resto de a 1000
            miles++;            // y por cada resta aumento una unidad el digito miles
        }
        while(aux > 99){        // Mientras el valor sea superior a 99
            aux -= 100;         // Le resto de a 100
            centenas++;         // y por cada resta aumento una unidad al digito centenas
        }
        while(aux > 9){         // Mientras el valor sea superior a 9
            aux -= 10;          // Le resto de a 10
            decenas++;          // y por cada resta aumento una unidad al digito decenas
        }

        unidades = aux;         // Por ultimo el valor restante lo asigno a unidades

        PORTA = 1;              // Activo el transistor para las unidades
        PORTB = unidades;       // y paso las unidades al display
        __delay_ms(10);         // Doy un retardo, por lo general en una aplicación real debe ser de unos 3 a 5 milisegundos
        PORTA = 2;              // Activo el transistor para las decenas
        PORTB = decenas;        // y paso las decenas al display
        __delay_ms(10);         // Retardo
        PORTA = 4;              // Activo el transistor para las centenas
        PORTB = centenas;       // y paso las centenas al display
        __delay_ms(10);         // Retardo
        PORTA = 8;              // Activo el transistor para los miles
        PORTB = miles;          // y paso los miles al display
        __delay_ms(10);
    }
}
