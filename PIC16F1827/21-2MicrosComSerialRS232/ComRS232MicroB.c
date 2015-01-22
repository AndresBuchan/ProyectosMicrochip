///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que permite comunicación entre dos           //
// microcontroladores a través de comunicación serial RS232 a 9600 baudios.  //
// Programa B                                                                //
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
#define Datos PORTB             // Nombramos el puerto B como Datos
#define Enable RB0              // Nombramos RB0 como Enable
#define RS RB3                  // Nombramos RB3 como RS

// Declaración de funciones
unsigned char leer_serial(void);
char obtener_caracter(void);
void escribir_serial(unsigned char dato);
void enviar_caracter(char dato);
void inicio_lcd(void);
void config_lcd(void);
void irapos_lcd(unsigned char posicion);
void escribir_lcd(unsigned char caracter);
void cadena_lcd(const char * cadena);
void caracter_lcd(char caracter);

// Declaración de variables globales
unsigned char instruccion;
unsigned char auxiliar;

void main(){

    // Declaración de variables
    char cont = 32;

    // Configuración de puertos
    
    TRISB = 2;                  // Puerto B como salida, excepto RB1
                                // que es Rx y debe ser configurado como entrada
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
    inicio_lcd();
    irapos_lcd(0);                    // Nos ubicamos en la primera linea posición 0
    cadena_lcd("   Com RS232    ");   // Envio el mensaje a la primera linea
    irapos_lcd(64);                   // Nos ubicamos en la primera linea posición 0
    cadena_lcd("  Conteo en ");       // Envio el mensaje a la segunda linea
    while(1){                         // Programa que se ejecura continuamente
        cont = obtener_caracter();    // Almacenamos en cont el resultado de leer el puerto serial
        irapos_lcd(76);               // Nos ubicamos en la posición 76 de la segunda linea
        caracter_lcd(cont + 48);      // Escribimos el valor recibido en el LCD, sumandole 48 puesto que
                                      // se trata de un caracter ASCII
    }
}

// Funciones

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

// Inicialización del LCD
void inicio_lcd(void){
    instruccion = 2;		// Dato para enviar el cursor a la posición inicial
    config_lcd();               // Llamamos a la función de configuración
    instruccion = 40;		// Dato para seleccionar bus de datos a 4 bits, display a dos lineas y tamaño de caracter 5x7
    config_lcd();               // Llamamos a la función de configuración
    instruccion = 12;   	// Dato para encender display, apagar el cursor y cursor sin parpadeo
    config_lcd();               // Llamamos a la función de configuración
    instruccion = 6;            // Dato para el desplazamiento del cursor hacia la derecha y sin desplazamiento automatico de la pantalla
    config_lcd();               // Llamamos a la función de configuración
    instruccion = 1;		// Dato para limpiar el contenido del LCD
    config_lcd();               // Llamamos a la función de configuración
}

// Configuración del LCD
void config_lcd(void){
    RS = 0;                             // RS = 0 para configuración del LCD
    Enable = 1;				// Habilito comunicación con el LCD
    Datos = Datos & 15;			// Hago una AND entre lo que hay en el puerto B y 00001111 y lo escribo en el mismo puerto B
 					// De ese modo pongo en 0 todo lo que haya en los 4 bits mas significativos
 					// y conservo lo que hay en los bits menos significativos
    auxiliar = instruccion & 240;	// En la variable auxiliar almaceno el resultado de hacer una AND entre
 					// la variable instruccion y 11110000, con eso solo guardo los bits mas significativos de dato en auxiliar
    Datos = auxiliar | Datos;		// Escribo en el puerto B el resultado entre lo que que quedó almacenado en auxiliar OR lo que hay en el puerto B
 					// de ese modo se conserva lo que hay en los bits menos significativos del puerto B
    __delay_ms(2);			// Retardo de 2ms mientras el LCD procesa el dato
    Enable = 0;				// Desconecto la comunicación con el LCD
    __delay_ms(2);			// Doy otro retardo
    Enable = 1;				// Rehabilito la comunicación con el LCD
    Datos = Datos & 15;			// Vuelvo y realizo una AND entre el contenido del Puerto B y 00001111 y lo envio al puerto B
    instruccion = instruccion << 4;	// A la variable instruccion le realizo un corrimiento de 4 bits hacia la izquierda
        				// teniendo ahora los bits menos significativos del lado de los mas significativos
    auxiliar = instruccion;		// Almaceno el valor de dato en auxiliar
    auxiliar = auxiliar & 240;		// Hago una AND entre el valor en auxiliar y 11110000 y almaceno el resultado en la misma auxiliar
        				// con eso nos aseguramos que los 4 bits mas significativos se conserven y los menos significativos pasen a 0
    Datos = auxiliar | Datos;		// Escribo en el puerto B el resultado de auxiliar OR el contenido del puerto B
        				// Asi escribimos los 4 bits mas significativos de auxiliar en el puerto B y conservamos lo que ya hay en los
        				// 4 bits menos significativos del mismo puerto
    __delay_ms(2);			// Retardo de 2ms mientras el LCD procesa el dato
    Enable = 0;				// Desconecto la comunicación con el LCD
    __delay_ms(2);			// Doy otro retardo
}

// Ir a una posición en el LCD
void irapos_lcd(unsigned char posicion){        // Esta función recibe un parametro con la posición donde se
                                                // ubicara el cursor
    RS = 0;                                     // RS = 0 para configuración del LCD

    escribir_lcd(128 + posicion);               // Llamamosla funcion de escribir en el LCD al tiempo que
                                                // leenviamos el comando de ubicación en la memoria DDRAM del LCD
                                                // sumandole el dato de donde ira posicionado
}

// Escribir en el LCD
void escribir_lcd(unsigned char caracter){      // Esta función recibe un parametro con el caracter a escribir en el LCD
    Enable = 1;					// Habilito la comunicación con el LCD
    Datos = Datos & 15;				// Hago una AND entre lo que hay en el puerto B y 00001111 y lo escribo en el mismo puerto B
						// De ese modo pongo en 0 todo lo que haya en los 4 bits mas significativos
						// y conservo lo que hay en los bits menos significativos
    auxiliar = caracter & 240;			// En la variable auxiliar almaceno el resultado de un AND entre caracter (el argumento recibido) y 11110000
                                        	// con el fin de tener los bits mas significativos de caracter en la variable auxiliar
    Datos = auxiliar | Datos;			// Hago un OR entre el resultado almacenado en auxiliar y el valor actual en el puerto B
 						// con esto se escriben los datos en los 4 bits mas significativos y los 4 menos significativos
 						// quedan intactos
    __delay_ms(2);				// Retardo de 2ms mientras el LCD procesa el dato
    Enable = 0;					// Deshabilito la comunicación con el LCD
    __delay_ms(2);				// Doy otro retardo
    Enable = 1;					// Re-habilito la comunicación con el LCD
    Datos = Datos & 15;                         // Hago una AND entre el valor actual en el puerto B y 00001111 y con eso borro los 4 bits mas significativos
        					// y dejo intactos los menos significativos
    caracter = caracter << 4;			// En la variable caracter hago un corrimiento de 4 posiciones a la izquierda
        					// de este modo los 4 bits menos significativos pasan a ser los 4 bits mas significativos
    auxiliar = caracter;			// Almaceno el dato luego del corrimiento en la variable auxiliar
    auxiliar = auxiliar & 240;			// Hago una AND entre auxiliar y 11110000 con el proposito de asegurarme que los 4 bits menos significativos sean 0
    Datos = auxiliar | Datos;			// Ahora hago un OR entre el contenido en el puerto B y aux para que se escriban los bits mas significativos
        					// de aux en los bits mas significativos del puerto B y los menos significativos de este puerto conserven su estado
    __delay_ms(2);				// Retardo de 2ms mientras el LCD procesa el dato
    Enable = 0;                                 // Cierro la comunicación con el LCD
    __delay_ms(2);				// Doy otro retardo
}

// Escribir cadena de caracteres
void cadena_lcd(const char * cadena){           // Esta función escribe una cadena de caracteres en el LCD
    RS = 1;                                     // Con RS = 1 le indico al LCD que voy a enviar un dato para que lo muestre
    while(*cadena){                             // Mientras hayan caracteres en la variable cadena
        escribir_lcd(*cadena++);                // Envio el siguiente caracter
    }
}

// Escribir un caracter
void caracter_lcd(char caracter){               // Esta función escribe un unico caracter
    RS = 1;                                     // Con RS = 1 le indico al LCD que voy a enviar un dato para que lo muestre
    escribir_lcd(caracter);                     // Envio el caracter al LCD
}