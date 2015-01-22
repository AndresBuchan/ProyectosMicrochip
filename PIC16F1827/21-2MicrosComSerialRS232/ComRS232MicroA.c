///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que permite comunicación entre dos           //
// microcontroladores a través de comunicación serial RS232 a 9600 baudios.  //
// Programa A                                                                //
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
#define switch RB0              // Nombramos RB0 como switch

// Declaración de funciones
unsigned char leer_serial(void);
char obtener_caracter(void);
void escribir_serial(unsigned char dato);
void enviar_caracter(char dato);

void main(){

    // Declaración de variables
    char cont;

    // Configuración de puertos
    TRISA = 224;                // Bits menos significativos del puerto A como salida
    ANSELA = 0;                 // Puerto A Digital
    PORTA = 15;                 // Pongo en 1 todos los bits menos significativos del puerto A
                                // Para que el display de 7 segmentos inicie apagado
    TRISB = 3;                  // Puerto B como salida, excepto RB1
                                // que es Rx y debe ser configurado como entrada
                                // y RB0 que sera la entrada del pulsador
    ANSELB = 0;                 // Puerto B Digital

    // Configuración de comunicación

    BAUDCON = 72;               // Generador de Baudios a 16 bits, bandera de recepción en desocupado
                                // Operación de recepción normal
    RCSTA = 144;                // Recepcion continua habilitada, Puerto serial habilitado
    TXSTA = 38;                 // Bit de transmisión vacia, habilitamos la trasnmisión
                                // y ponemos en 1 el bit BRGH.
    SPBRGL = 103;               // El valor 103 viene de la siguiente formula:
    SPBRGH = 0;

    /*

     Usar el Generador de baudios a 16 bits nos permite tener un margen de error menor

     Baudios deseados = (Frecuencia Oscilador / ( 4 * (SPBRGH:SPBRGL + 1))

     Donde SPBRGH:SPBRGL es el dato de 16 bits formado por ambas variables

     El 4 de configurar la comunicación en modo asincrono, el generador de baudios a 16 bits
     y el BRGH de TXSTA en 1 (La tabla se encuentra en la pagina 298 del datasheet - Table 26-3).

    Entonces :

    Baudios deseados = (4000000 / ( 4 * (103 + 1))
    Baudios deseados = 9615 Baudios similar a 9600 Baudios

    */

    while(1){                               // Programa que se ejecura continuamente
        if(cont > 9){                       // Hacemos una comprobacion para que cont no sea superio a 9
            cont = 0;                       // de ser mayor que nueve lo pondremos en 0
        }
        if(!switch){                        // Si el switch se pulsa
            while(!switch){                 // Hacemos un ciclo anti rebote
                continue;
            }
            PORTA = cont;                   // Escribimos el valor de cont en el puerto A
                                            // para que se muestre en el display de siete segmentos
            escribir_serial(cont);          // Enviamos el valor de cont por el puerto serial
            cont++;                         // y aumentamos en una unidad el contador
        }
    }
}

unsigned char leer_serial(void){    // Función que lee el dato en Rx
    while(!PIR1bits.RCIF){          // Mientras la bandera de recepción este en 0
        continue;                   // nada haga
    }
    if(RCSTAbits.OERR){             // Si se produce un error
        RCSTAbits.CREN = 0;         // Deshabilitamos la recepción
        RCSTAbits.CREN = 1;         // y la re-habilitamos nuevamente
    }
    return RCREG;                   // Retornamos el valor que queda en RCREG
}

char obtener_caracter(void){        // Función para obtener un unico caracter
    return leer_serial();           // llamamos la función leer serial
}

void escribir_serial(unsigned char dato){     // Función para escribir en Tx
    while(!PIR1bits.TXIF){          // Mientras la bandera de transmisión este en 0
        continue;                   // nada haga
    }
    TXREG = dato;                   // Escribe el dato en el registro TXREG para ser
                                    // trasmitido
}

void enviar_caracter(char dato){    // Función para enviar un caracter
    escribir_serial(dato);          // llamamos la función escribir serial
}