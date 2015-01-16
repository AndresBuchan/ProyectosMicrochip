///////////////////////////////////////////////////////////////////////////////
// Programa para el pic 16F1827 que muestra el valor de dos variables        //
// analogas en un LCD 2x16 (Pantalla de cristal liquido), con conexión a     //
// 4 bits. El display se controla por el puerto B
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
#define Enable RB0              // Nombramos el pin RB0 como Enable
#define RS RB1                  // Nombramos el pin RB1 como RS
#define Datos PORTB             // Nombramos el puerto B como Datos
#define Volt 0                  // Definimos Volt como 0 para seleccionar el canal AN
#define Temp 1                  // Definimos Temp como 1 para seleccionar el canal AN

// En este programa usaremos las funciones
// Aquí las declararemos
void inicio_lcd(void);
void config_lcd(void);
void irapos_lcd(unsigned char posicion);
void escribir_lcd(unsigned char caracter);
void cadena_lcd(const char * cadena);
void caracter_lcd(char caracter);
void inicio_AD(void);
void convertir_AD(char canal);
void bcd(unsigned int entero);

// Declaramos las variables globales, que seran accedidas por cualquier función
    unsigned char instruccion;
    unsigned char auxiliar;
    unsigned int conversion;
    unsigned int voltaje;
    unsigned int temperatura;
    unsigned int aux;
    char centenas = 0;
    char decenas = 0;
    char unidades = 0;

void main(){
    TRISA = 255;                // Puerto A como entrada
    ANSELA = 3;                 // RA0 y RA1 como analogas
    TRISB = 0;                  // Puerto B salida
    ANSELB = 0;                 // Puerto B Digital
    PORTB = 0;                  // Limpio el puerto B

    // Configuración LCD
    // Antes de enviar datos al LCD debemos configurarle ciertos parametros
    inicio_lcd();                       // Llamamos la función para inicializar el LCD
    inicio_AD();                        // Llamamos la función para inicializar el conversor AD

    // Establecemos los mensajes que estaran estaticos en el LCD

    irapos_lcd(0);                  // Nos ubicamos en la primera linea posición 0
    cadena_lcd("Volt:  . V");       // Envio el mensaje a la primera linea
                                    // En los espacios antes y despues del punto se ubicaran los valores
    irapos_lcd(64);                 // Nos ubicamos en la primera linea posición 0
    cadena_lcd("Temp:     C");      // Envio el mensaje a la segunda linea
                                    // En los espacion antes de °C aparacera la temperatura

    while(1){                                   // Inicio del código a ejecutar constantemente
        convertir_AD(Volt);                     // Hacemos la conversión en el canal 0
        voltaje = (conversion * 49) / 1000;     // Calculo para que se muestre el voltaje con un decimal
        bcd(voltaje);                           // Llamamos la funcion bcd para que separe voltaje en digitos
        irapos_lcd(6);                          // Vamos a la posición 6 de la primera linea
        caracter_lcd(decenas + 48);             // Escribimos el valor del voltaje (se le suma 48 por estar en ascii)
        irapos_lcd(8);                          // Vamos a la posición 8 de la primera linea
        caracter_lcd(unidades + 48);            // Escribimos el valor decimal del voltaje
        convertir_AD(Temp);                     // Hacemos la conversión en el canal 1
        temperatura = conversion / 2;           // Calculo para la temperatura
        bcd(temperatura);                       // Llamamos la funcion bcd para que separe temperatura en digitos
        irapos_lcd(70);                         // Vamos a la posición 70 de la segunda linea
        caracter_lcd(centenas + 48);            // Escribimos centenas
        irapos_lcd(71);                         // Vamos a la posición 71 de la segunda linea
        caracter_lcd(decenas + 48);             // Escribimos decenas
        irapos_lcd(72);                         // Vamos a la posición 72 de la segunda linea
        caracter_lcd(unidades + 48);            // Escribimos unidades
    }
}

// Funciones

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

// Inicialización conversor AD
void inicio_AD(void){
    ADCON0 = 1;             // Habilito la conversión AD
    ADCON1 = 192;           // Alineo el resultado de la conversión a la derecha,
                            // Fosc/8
                            // Selecciono como voltaje de referencia la alimentación 5VDC del micro
    ADRESL = 0;             // Limpio ADRESL
    ADRESH = 0;             // Limpio ADRESH
}

// Convertir señal
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

// Separar digitos en una variable
void bcd(unsigned int entero){
        aux = entero;           // Guardo en auxiliar el valor en cont que por la función viene con el nombre de "entero"
        centenas = 0;           // Pongo en cero todos los digitos
        decenas = 0;
        unidades = 0;

        while(aux > 99){        // Mientras el valor sea superior a 99
            aux -= 100;         // Le resto de 100
            centenas++;         // y por cada resta aumento una unidad al digito centenas
        }
        while(aux > 9){         // Mientras el valor sea superior a 9
            aux -= 10;          // Le resto de 10
            decenas++;          // y por cada resta aumento una unidad al digito decenas
        }

        unidades = aux;         // Por ultimo el valor restante lo asigno a unidades
}