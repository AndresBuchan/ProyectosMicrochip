///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que muestra en numero de 4 cifras en 4       //
// displays de siete segmentos digito a digito, la velociadad de cristal de  //
// cuarzo esta configurada a 4MHz                                            //
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
    TRISA = 240;                // Configuro RA0, RA1, RA2 y RA3 como salidas para controlar
                                // los transistores que habilitaran cada display
    TRISB = 0;                  // Puerto B como salida
    ANSELA = 0;                 // Puerto A Digital
    ANSELB = 0;                 // Puerto B Digital
    PORTA = 0;                  // Inicializo Puerto A en 0
    PORTB = 0;                  // Inicializo Puerto B en 0
    int cont = 0;               // Variable que incrementara de 0 a 9999
    char miles;                 // Almacenara los miles
    char centenas;              // Almacenara las centenas
    char decenas;               // Almacenara las decenas
    char unidades;              // Almacenara las unidades
    int aux = 0;                // Variable para almacenar datos temporalmente

    while(1){                   // Inicio del código a ejecutar constantemente
        if(cont > 9999){        // Verifico que el contador no haya superado la cantidad maxima
                                // representable en los cuatro displays
            cont = 0;           // Si es así la reinicio a 0
        }

        aux = cont;             // Guardo en auxiliar el valor en cont
        centenas = 0;           // Pongo en cero todos los digitos
        decenas = 0;
        unidades = 0;
        miles = 0;

        while(aux > 999){       // Mientras el valor sea superior a 999
            aux -= 1000;        // Le resto de a 1000
            miles++;            // y por cada resta aumento una unidad el digito miles
        }
        while(aux > 99){        // Mientras el valor sea superior a 99
            aux -= 100;         // Le resto de 100
            centenas++;         // y por cada resta aumento una unidad al digito centenas
        }
        while(aux > 9){         // Mientras el valor sea superior a 9
            aux -= 10;          // Le resto de 10
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

        cont++;                 // Incremento en una unidad el contador
    }
}
