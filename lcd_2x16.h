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
 *	DESCRIPCION DE USO	:
 *		1. SE CONFIGURA EL RELOJ PARA TRABAJAR A 16 MHZ,
 *		2. SE ACTIVA EL PERIFERICO DE LOS PUERTOS A UTILIZAR
 *		3. SE DECLARAN LOS PINES DE CONTROL (LOS CUALES SE CONECTARAN A RS,RW,ENABLE)
 *			COMO SALIDA
 *		4. SE DECLARAN LOS PINES DE DATOS (LOS CUALES SE CONECTARAN A DB7,DB6,DB5,DB4)
 *			COMO SALIDA
 *		5. SE LIMPIAN TODAS LAS SALIDAS (SE ENVIA UN 0 A LOS PINES DE CONTROL Y DATOS)
 *		6. CONSTRUIMOS LA ESTRUCTURA CON LOS PUERTOS Y LOS PINES
 *		7. INICIALIZAMOS LA PANTALLA CON EL INICIALIZADOR
 *		8. ENVIAR DATOS CON EL COMANDO WRITE, PUTCH, PUTS, GOTO, CMD SEGUN SEA NECESARIO.
 *
 * */

#ifndef LCD_2X16_H_
#define LCD_2X16_H_

#include <stdint.h>                     //LIBRERIA PARA EL USO DE DATOS TIPO intN_t (N bits)
#include <stdbool.h>                    //LIBRERIA PARA EL USO DE DATOS TIPO BOOL
#include "driverlib/sysctl.h"           //LIBRERIA PARA EL CONTROL DEL SISTEMA (CONFIGURACION DEL RELOJ)
#include "inc/hw_memmap.h"              //LIBRERIA NECESARIA PARA EL USO DE LAS MACROS (HARDWARE MEMORY MAP)
#include "driverlib/gpio.h"             //LIBRERIA PARA EL USO DE LAS GPIO
//- - - - - - - - - - - - - - - - - - - -DEFINICION DE LA FRECUENCIA EN Hrz DEL RELOJ
#define HZ 					16000000//->16Mhz

/*
 *  ESTRUCTURA DE DATOS PARA EL LCD A 4 BITS
 *      -struct LCD -> lcd_2x16
 */
typedef struct LCD{
	volatile uint32_t CONTROLPORT_BASE;
	volatile uint32_t DATAPORT_BASE;
	volatile uint8_t RS;
	volatile uint8_t RW;
	volatile uint8_t Enable;
	volatile uint8_t DATAMASK;
}lcd_2x16;
/*
 * FUNCIONES DE CONFIGURACION DEL SISTEMA
 *		- CONFIGURACION DEL RELOJ A 16MHZ	void 		setClockReady
 *		- DELAY A MILISEGUNDOS				void 		delay
 *		- DELAY A MICROSEGUNDOS				void 		delay_us
 * */
void setClockReady();
void delay(uint32_t);
void delay_us(uint32_t);

//-	- - - - - - - -	- - - - - - - -	- - - - - - - -	- - - - - - - -	- - - - - - - -	- - -
/*
 * FUNCIONES DE CONTROL DEL MODULO LCD
 * 		- CONSTRUCTOR						struct LCD	setLCD4
 * 		- INICIALIZADOR						void 		lcd_init
 * 		- LANZADOR DE COMANDOS				void		lcd_cmd
 * 		- ESCRITOR DE DATOS					void 		lcd_write
 * 		- ESCRITOR DE CARACTERES			void		lcd_putch
 * 		- ESCRITOR DE CADENAS				void		lcd_puts
 * 		- DIRECCIONADOR	DE PUNTERO			void		lcd_goto
 *
 * */
lcd_2x16 setLCD4(uint32_t,uint32_t, uint8_t,uint8_t,uint8_t,uint8_t);
void lcd_init(lcd_2x16);
void lcd_cmd(lcd_2x16,uint8_t,uint8_t);
void lcd_write(lcd_2x16,uint8_t);
void lcd_putch(lcd_2x16,uint8_t);
void lcd_puts(lcd_2x16,unsigned char*);
void lcd_goto(lcd_2x16,unsigned char);
/*
 *
 * 	FUNCIONES DE CONTROL INTERNO DEL MODULO
 * 		- STROBE							void		STROBE
 */
void STROBE(lcd_2x16);

#endif /* LCD_2X16_H_ */
