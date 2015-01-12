///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que muestra un mensaje estatico en un        //
// LCD 2x16 (Pantalla de cristal liquido), con conexión a 8 bits             //
///////////////////////////////////////////////////////////////////////////////

// 'C' source line config statements

#include <xc.h>

// Bits de configuración

// CONFIG1
#pragma config FOSC = XT        // Oscilador con cristal de cuarzo de 4MHz conectado en los pines 15 y 16
#pragma config WDTE = OFF       // Perro guardian (WDT deshabilidado)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT deshabilidado)
#pragma config MCLRE = ON       // Master clear habilitado, pin reset
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
#define Enable RA0              // Nombramos el pin RA0 como Enable
#define RS RA1                  // Nombramos el pin RA1 como RS
#define Datos PORTB             // Nombramos todo el puerto B como Datos

// En este programa usaremos las funciones
// Aquí las declararemos
void inicio_lcd(void);
void config_lcd(void);
void irapos_lcd(unsigned char posicion);
void escribir_lcd(unsigned char caracter);
void cadena_lcd(const char * cadena);

void main(){
    TRISA = 252;                // Puerto A como salida, aqui conectan Enable y RS del LCD
    ANSELA = 0;                 // Puerto A Digital
    TRISB = 0;                  // Puerto B salida, acá van desde D0 hasta D7 del LCD
    ANSELB = 0;                 // Puerto B Digital
    PORTA = 0;                  // Inicializo Puerto A en 0, se mostrara el 0 en el display
    PORTB = 0;                  // Limpio el puerto B, tambien funcionaria escribir
                                // Datos = 0;

    // Configuración LCD
    // Antes de enviar datos al LCD debemos configurarle ciertos parametros
    inicio_lcd();                // Llamamos la función para inicializar el LCD

    while(1){                   // Inicio del código a ejecutar constantemente

        irapos_lcd(0);          // Nos ubicamos en la primera linea posición 0
        cadena_lcd(" LCD PIC16F1827 "); // Envio el mensaje a la primera linea
        irapos_lcd(64);         // Nos ubicamos en la segunda linea posición 0
        cadena_lcd("@alejandromunozc"); // Envio el mensaje a la segunda linea

        while(1){
            continue;           // hacemos un loop infinito, pues ya enviamos el mensaje
        }
    }
}

// Funciones

// Inicialización del LCD
void inicio_lcd(void){
    // Llevamos el cursor a la posición inicial
    Datos = 2;                  // Enviamos el codigo correspondiente al puerto
    config_lcd();               // Llamamos función de configuración que habilita y envia la configuración
                                // al LCD

    // Seleccionamos bus a 8 bits, trabajo a dos lineas y tamaño de caracter 5x7
    Datos = 56;                 // Enviamos el codigo correspondiente al puerto
    config_lcd();               // Llamamos función de configuración que habilita y envia la configuración
                                // al LCD

    // Encendido del display, encendido del cursor con parpadeo
    Datos = 15;                 // Enviamos el codigo correspondiente al puerto
    config_lcd();               // Llamamos función de configuración que habilita y envia la configuración
                                // al LCD

    // Dirección del movimiento del cursor hacia la derecha
    Datos = 6;                  // Enviamos el codigo correspondiente al puerto
    config_lcd();               // Llamamos función de configuración que habilita y envia la configuración
                                // al LCD

    // Borrado del display
    Datos = 1;                  // Enviamos el codigo correspondiente al puerto
    config_lcd();               // Llamamos función de configuración que habilita y envia la configuración
                                // al LCD
}

// Configuración del LCD
void config_lcd(void){
    RS = 0;                     // Con RS en 0 le indico al LCD que le voy a enviar un dato de configuración
    Enable = 1;                 // Con Enable en 1 habilito la comunicación con el LCD
    __delay_ms(2);              // Puesto que la duración máxima de cualquier operación en el LCD
                                // tarda 1,64ms Le damos un retardo de 2ms.
    Enable = 0;                 // Una vez pasan los 2ms cerramos la comunicación escribiendo un o en Enable
}

// Ir a una posición en el LCD
void irapos_lcd(unsigned char posicion){        // Esta función recibe un parametro con la posición donde se
                                                // ubicara el cursor
    RS = 0;                                     // Con RS en 0 le indico al LCD que le voy a enviar un dato de configuración

    escribir_lcd(128 + posicion);               // Llamamosla funcion de escribir en el LCD al tiempo que
                                                // leenviamos el comando de ubicación en la memoria DDRAM del LCD
                                                // sumandole el dato de donde ira posicionado
}

// Escribir en el LCD
void escribir_lcd(unsigned char caracter){      // Esta función recibe un parametro con el caracter a escribir en el LCD
    Datos = (caracter);                           // Envio el caracte en el puerto
    Enable = 1;                                 // Activo la comunicación con el LCD
    __delay_ms(2);                              // Retardo para realizar la tarea en el LCD
    Enable = 0;                                 // Finalizo la conexión al LCD
}

// Escribir cadena de caracteres
void cadena_lcd(const char * cadena){             // Esta función escribe una cadena de caracteres en el LCD
    RS = 1;                                     // Con RS = 1 le indico al LCD que voy a enviar un dato para que lo muestre
    while(*cadena){                             // Mientras hayan caracteres en la variable cadena
        escribir_lcd(*cadena++);                // Envio el siguiente caracter
    }
}