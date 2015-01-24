#En este repositorio se comparten proyectos basados en microcontroladores PIC#

Los proyectos estan numerados desde lo más básico y va aumentado su complejidad, siendo cada uno un ejemplo claro de como usar las diferentes características del microcontrolador y su conexión a los componentes electrónicos.

En cada carpeta se encuentran los archivos necesarios para realizar el proyecto:

* Código fuente en lenguaje C **(.c)**
* Circuito electrónico **(.png)**
* Archivo de simulación en Proteus **(.pdsprj)**
* Archivo con el programa compilado **(.hex)**
* Documentación adicional en caso de ser necesario

##Listado de proyectos##

* PIC16F1827

01 **ParpadeoLed:** Empieza desde lo mas básico, enciende y apaga un led.

02 **LedSw:** Enciende y apaga un led con solo presionar un switch.

03 **SecuenciaLed:** Crea una secuencia de encendio y apagado de 8 leds con un ciclo for.

04 **ContadorPulsos7Segmentos:** Muestra en un display de 7 segmentos un conteo de los números del 0 al 9, con la presión de un switch.

05 **MultiplexorBCD7Segmentos:** Muestra un número de 4 digitos en cuatro displays de 7 segmentos con la técnica de multiplexación.

06 **ConversorAD10Bits:** Conversor de una señal análoga a digital a 10 bits.

07 **TempLM35Disp7Seg:** Ejemplo del uso del conversor análogo digital con un sensor LM35, mostrándose el valor sensado en 4 displays de 7 segmentos con la técnica de la multiplexación.

08 **TecladoMatricial4x4:** Ejemplo del uso de un teclado matricial 4x4.

09 **LCDestatico8bits:** Muestra un mensaje estatico en un Diplay de cristal liquido (LCD) de 2 lineas y 16 caracteres, con conexión a 8 bits.

10 **ContadorLCD8bits:** Ejemplo de un contador en un LCD con conexión a 8 bits.

11 **LCDestatico4bits:** Muestra un mensaje estatico en un Diplay de cristal liquido (LCD) de 2 lineas y 16 caracteres, con conexión a 4 bits.

12 **VoltTempLCD4bits:** Ejemplo del convesor de dos canales análogo digital y mostrando su resultado en un LCD con conexión a 4 bits.

13 **InterrupcionExterna:** Interrupción externa por un pin.

14 **InterrupcionCambioPuerto:** Interrupción externa por cambio de estado en un pin de un puerto.

15 **PWM:** Modulación por ancho de pulso PWM.

16 **ServoPWM:** Controla la posición de un servo, ubicandolo a 0, 90 y 180 grados.

17 **ServoControlAD:** Control de la posición de un servo a partir de la conversión análoga digital.

18 **2ServosControlAD:** Control de la posición de dos servos a partir de dos conversiones análoga digital.

19 **ComSerialRS232(TxBasico:** Comunicación serial RS232, transimisión básica.

20 **ComSerialRS232TxRx:** Comunicación serial RS232, transmisión y recepción de datos.

21 **2MicrosComSerialRS232:** Comunicación serial RS232 entre dos microcontroladores.