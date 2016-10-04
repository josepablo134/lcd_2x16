/*
 *
 *	FACULTAD DE MATEMATICAS
 *	UNIVERSIDAD AUTONOMA DE YUCATAN
 *
 *	CLASE DE SISTEMAS EMBEBIDOS
 *
 *	LIBRERIA PARA EL USO Y CONTROL DEL MODULO LCD 2x16 BASADO
 *		EN EL MICROCONTROLADOR HD44780 DE HITACHI
 *
 * 		AUTOR : Josepablo Cruz Baas
 * 		FECHA :	23 - Septiembre - 2016
 *
 *	DESCRIPCION :
 *		EL SIGUIENTE CODIGO PRESENTA UN CONJUNTO DE FUNCIONES PARA
 *		FACILITAR EL USO DE UN MODULO LCD A TRAVES DE DOS PUERTOS
 *		CUALESQUIERA, DESTINANDO UN PUERTO AL CONTROL Y OTRO A LOS DATOS.
 *
 *  FUNCIONES DE CONFIGURACION DEL SISTEMA
 *		- CONFIGURACION DEL RELOJ A 16MHZ	void 		setClockReady
 *		- DELAY EN MILISEGUNDOS				void 		delay
 *		- DELAY EN MICROSEGUNDOS				void 		delay_us
 *  FUNCIONES DE CONTROL DEL MODULO LCD
 * 		- CONSTRUCTOR						struct LCD	setLCD4
 * 		- INICIALIZADOR						void 		lcd_init
 * 		- LANZADOR DE COMANDOS				void		lcd_cmd
 * 		- ESCRITOR DE DATOS					void 		lcd_write
 * 		- ESCRITOR DE CARACTERES			void		lcd_putch
 * 		- ESCRITOR DE CADENAS				void		lcd_puts
 * 		- DIRECCIONADOR	DE PUNTERO			void		lcd_goto
 * 	FUNCIONES DE CONTROL INTERNO DEL MODULO
 * 		- STROBE							void		STROBE
 *
 * */

#include <stdint.h>					//LIBRERIA PARA EL USO DE DATOS TIPO intN_t (N bits)
#include <stdbool.h>				//LIBRERIA PARA EL USO DE DATOS TIPO BOOL
#include "driverlib/sysctl.h"		//LIBRERIA PARA EL CONTROL DEL SISTEMA (CONFIGURACION DEL RELOJ)
#include "inc/hw_memmap.h"			//LIBRERIA NECESARIA PARA EL USO DE LAS MACROS (HARDWARE MEMORY MAP)
#include "driverlib/gpio.h"			//LIBRERIA PARA EL USO DE LAS GPIO
#include "lcd_2x16.h"				//HEADER DE LA LIBRERIA

/*
 *CONFIGURACION DEL RELOJ A 16 MHZ:
 *  void
 *
 *
 *DESCRIPCION:
 * CONFIGURAMOS EL RELOJ INTERNO PARA TRABAJAR A 16MHz
 * ESTA FUNCION VA ANTES DE INICIALIZAR EL MODULO DISPLAY Y UTILIZAR
 *  LAS FUNCIONES DELAY.
 * */
void setClockReady(){
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
}
/*
 *DELAY EN MILISEGUNDOS
 *	uint32_t time_ms			    -> NUMERO DE 32 BITS QUE INDICA EL TIEMPO DE ESPERA EN MILISEGUNDOS
 *
 *
 *DESCRIPCION:
 * CALCULA EL TIEMPO POR MEDIO DE LOS MHZ DEL SISTEMA Y
 *  ENVIA EL RESULTADO A UNA MACRO DE DELAY
 * */
void delay(uint32_t time_ms){
	SysCtlDelay((HZ/3000)*time_ms);
}
/*
 *DELAY EN MICROSEGUNDOS:
 *	uint32_t time_us			    -> NUMERO DE 32 BITS QUE INDICA EL TIEMPO DE ESPERA EN MICROSEGUNDOS
 *
 *
 *DESCRIPCION:
 * CALCULA EL TIEMPO POR MEDIO DE LOS MHZ DEL SISTEMA Y
 * ENVIA EL RESULTADO A UNA MACRO DE DELAY
 * */
void delay_us(uint32_t time_us){
	SysCtlDelay((HZ/3000000)*time_us);
}

//FUNCIONES DE CONTROL DEL MODULO LCD
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*CONSTRUCTOR:
 *  uint8_t CONTROLPORT_BASE        -> MASCARA DE BITS DEL PUERTO BASE DE CONTROL
 *  uint8_t DATAPORT_BASE           -> MASCARA DE BITS DEL PUERTO BASE DE DATOS
 * 	uint8_t rs                      -> MASCARA DE BITS DEL PIN DE CONTROL RS
 * 	uint8_t rw                      -> MASCARA DE BITS DEL PIN DE CONTROL RW
 *  uint8_t enable                  -> MASCARA DE BITS DEL PIN DE CONTROL ENABLE
 *  uint8_t data                    -> MASCARA DE BITS DE LOS PINES DE DATOS
 *
 *DESCRIPCION:
 *	CONSTRUYE Y RESERVA MEMORIA PARA LA ESTRUCTURA DE DATOS Y CADA DATO EN ELLA.
 *	INICIALIZA LOS VALORES DE LA ESTRUCTURA CON LOS VALORES QUE SE ENVIAN COMO PARAMETRO.
 * */
lcd_2x16 setLCD4(uint32_t CONTROLPORT_BASE
				,uint32_t DATAPORT_BASE
				,uint8_t rs
				,uint8_t rw
				,uint8_t enable
				,uint8_t data){
	lcd_2x16 temp;
    //Asignacion de los valores
	temp.CONTROLPORT_BASE	= CONTROLPORT_BASE;
	temp.DATAPORT_BASE	  	= DATAPORT_BASE;
	temp.RS				  	= rs;
	temp.RW				  	= rw;
	temp.Enable			  	= enable;
	temp.DATAMASK		    = data;
	return temp;
}
/*INICIALIZADOR:
 * struct lcd_2x16 lcd			-> ESTRUCTURA DE DATOS YA CONFIGURADA
 *
 *
 *DESCRIPCION:
 *	DECLARA UNA FUNCION PRIVADA Y ENVIA LOS COMANDOS CORRESPONDIENTES PARA INICIALIZAR LA LCD
 * */
void lcd_init(lcd_2x16 lcd){
	GPIOPinWrite(lcd.CONTROLPORT_BASE , lcd.RS		, 0x00);
	GPIOPinWrite(lcd.CONTROLPORT_BASE , lcd.RW		, 0x00);
	GPIOPinWrite(lcd.CONTROLPORT_BASE , lcd.Enable	, 0x00);

	delay(15);

	GPIOPinWrite(lcd.DATAPORT_BASE , lcd.DATAMASK , 0x03);
	STROBE(lcd);
	delay(5);
	STROBE(lcd);
	delay_us(200);
	STROBE(lcd);
	delay_us(200);
	GPIOPinWrite(lcd.DATAPORT_BASE , lcd.DATAMASK , 2);		//4 bit mode
	STROBE(lcd);

	lcd_write(lcd,0x28);	//Set interface length
	lcd_write(lcd,0x0F);	//Display on, cursor on, cursor blink on
	lcd_write(lcd,0x01);	//Clear screen
	lcd_write(lcd,0x06);	//Set Entry Mode
}
/*
 *LANZADOR DE COMANDOS:
 * 		struct lcd_2x16	lcd		->  ESTRUCTURA DE DATOS DE LA LCD YA CONFIGURADO
 * 		uint8_t			config	->	CONFIGURACION DEL PUERTO DE CONTROL
 * 		uint8_t			data	->	8 BITS DE DATOS A ENVIAR
 *
 *DESCRIPCION:
 *  CONFIGURA EL PUERTO DE CONTROL Y LANZA LOS 8 BITS DE DATOS MULTIPLEXADOS
 *  AL PUERTO DE DATOS DE LA LCD. AMBOS PUERTOS CONFIGURADOS EN LA ESTRUCTURA.
 *
 *
 * */
void lcd_cmd(lcd_2x16 lcd,uint8_t config,uint8_t data){
	delay_us(40);
		GPIOPinWrite( lcd.CONTROLPORT_BASE , lcd.RS|lcd.RW|lcd.Enable , config & 0xF);
		lcd_write(lcd,data);
}
/*
 *ESCRITOR DE DATOS:
 * 		struct lcd_2x16	lcd		->  ESTRUCTURA DE DATOS DE LA LCD YA CONFIGURADO
 * 		uint8_t			data	->	8 BITS DE DATOS A ENVIAR
 *
 *DESCRIPCION:
 *  ENVIA LOS DATOS MULTIPLEXADOS SIN IMPORTAR LA CONFIGURACION DEL PUERTO
 *  DE CONTROL, SE RECOMIENDA ASEGURAR LA CONFIGURACION DEL PUERTO
 *  ANTES DE ENVIAR DATOS.
 * */
void lcd_write(lcd_2x16 lcd,uint8_t data){
	delay_us(50);
	GPIOPinWrite(lcd.DATAPORT_BASE , lcd.DATAMASK , (data>>4)&0xF);
	STROBE(lcd);
	GPIOPinWrite(lcd.DATAPORT_BASE , lcd.DATAMASK , data & 0xF);
	STROBE(lcd);
	delay_us(675);
}
/*
 *ESCRITOR DE CARACTER:
 * 		struct lcd_2x16	lcd		->  ESTRUCTURA DE DATOS DE LA LCD YA CONFIGURADO
 * 		uint8_t			data	->	8 BITS DE DATOS A ENVIAR
 *
 *DESCRIPCION:
 *  CONFIGURA EL PUERTO DE CONTROL PARA LA ESCRITURA DE LOS DATOS,
 *  ENVIA LOS DATOS MULTIPLEXADOS AL PUERTO DE DATOS DEL MODULO.
 * */
void lcd_putch(lcd_2x16 lcd,uint8_t data){
	GPIOPinWrite(lcd.CONTROLPORT_BASE ,  lcd.RS , lcd.RS );
	lcd_write(lcd,data);
}
/*
 *ESCRITOR DE CARACTER:
 * 		struct lcd_2x16	lcd		->  ESTRUCTURA DE DATOS DE LA LCD YA CONFIGURADO
 * 		unsigned char   *c      ->	APUNTADOR AL PRIMER ELEMENTO DE UN VECTOR DE CARACTERES
 *
 *DESCRIPCION:
 *  CONFIGURA EL PUERTO DE CONTROL PARA LA ESCRITURA DE LOS DATOS,
 *  ENVIA LOS DATOS MULTIPLEXADOS AL PUERTO DE DATOS DEL MODULO
 *  CARACTER A CARACTER HASTA QUE EL APUNTADOR NO CONTENGA MAS DATOS
 *  QUE ENVIAR.
 * */
void lcd_puts(lcd_2x16 lcd,unsigned char *c){
	GPIOPinWrite(lcd.CONTROLPORT_BASE , lcd.RS , lcd.RS );
		while(*c){
			lcd_write(lcd,*c++);
		}
}
/*
 *ESCRITOR DE CARACTER:
 * 		struct lcd_2x16	lcd		->  ESTRUCTURA DE DATOS DE LA LCD YA CONFIGURADO
 * 		unsigned char   pos     ->	DIRECCION NUMERICA DE LA MEMORIA A LA QUE SE DESEA ACCEDER
 *
 *DESCRIPCION:
 *  CONFIGURA EL PUERTO DE CONTROL PARA LA CONFIGURACION A TRAVES DE COMANDOS.
 *  ENVIA LOS DATOS MULTIPLEXADOS AL PUERTO DE DATOS DEL MODULO.
 * */
void lcd_goto(lcd_2x16 lcd,unsigned char pos){
	GPIOPinWrite(lcd.CONTROLPORT_BASE , lcd.RS , 0x00);
	lcd_write(lcd,0x80+pos);
}
//FUNCIONES DE CONTROL INTERNO DEL MODULO
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 *ESTROBE:
 * 		struct lcd_2x16	lcd		->  ESTRUCTURA DE DATOS DE LA LCD YA CONFIGURADO
 *
 *
 *DESCRIPCION:
 *  ENVIA UNA SEÑAL ENABLE AL PUERTO DE CONFIGURACION DE LA ESTRUCTURA Y LO DESACTIVA
 *  INMEDIATAMENTE
 * */
void STROBE(lcd_2x16 lcd){
    GPIOPinWrite(lcd.CONTROLPORT_BASE , lcd.Enable , lcd.Enable );
    GPIOPinWrite(lcd.CONTROLPORT_BASE , lcd.Enable , 0x00);
}
