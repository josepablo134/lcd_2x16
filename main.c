/*
 *
 *	FACULTAD DE MATEMATICAS
 *	UNIVERSIDAD AUTONOMA DE YUCATAN
 *
 *	CLASE DE SISTEMAS EMBEBIDOS
 *
 *	IMPLEMENTACION DE LA LIBRERIA PARA MODULO LCD 2x16
 *      CONTADOR AUTOMATICO
 *
 * 		AUTOR : Josepablo Cruz Baas
 * 		FECHA :	30 - Septiembre - 2016
 *
 *	DESCRIPCION :
 *		EN LA SEGUNDA LINEA DEL MODULO SE DESPLEGA UN MENSAJE CUALQUIERA,
 *      MIENTRAS QUE EN LA PRIMERA SE IMPRIME UN CONTADOR QUE VANZA CADA
 *      MEDIO SEGUNDO.
 *
 * */
#include <stdint.h>					//LIBRERIA PARA EL USO DE DATOS TIPO intN_t (N bits)
#include <stdbool.h>				//LIBRERIA PARA EL USO DE DATOS TIPO BOOL
#include <stdlib.h>					//LIBRERIA ESTANDAR DE C -> MALLOC, NULL
#include "inc/hw_memmap.h"			//LIBRERIA NECESARIA PARA EL USO DE LAS MACROS (HARDWARE MEMORY MAP)
#include "driverlib/gpio.h"			//LIBRERIA PARA EL USO DE LAS GPIO
#include "driverlib/sysctl.h"		//LIBRERIA PARA EL CONTROL DEL SISTEMA (CONFIGURACION DEL RELOJ)
#include "lcd_2x16.h"				//LIBRERIA PARA EL USO DEL MODULO LCD

#define DATAPORT_BASE				GPIO_PORTE_BASE
#define CONTROLPORT_BASE			GPIO_PORTK_BASE
	#define CONTROLMASK				0x07
	#define RS						0x01
	#define RW						0x02
	#define ENABLE					0x04
	#define DATAMASK				0x0F



int main(){
	unsigned char counter = 0;
	setClockReady();
	//	Activamos los perifericos
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	// Activamos los puertos base como salida
	 GPIOPinTypeGPIOOutput(DATAPORT_BASE , DATAMASK);
	 GPIOPinTypeGPIOOutput(CONTROLPORT_BASE , RS|RW|ENABLE);
	// Limpiamos las salidas
	 GPIOPinWrite(DATAPORT_BASE,DATAMASK,0x00);
	 GPIOPinWrite(CONTROLPORT_BASE, CONTROLMASK ,0x00);
	// Activamos los puertos del lcd
	lcd_2x16 display = setLCD4(CONTROLPORT_BASE,DATAPORT_BASE,RS,RW,ENABLE,DATAMASK);
	lcd_init(display);				//Inicializamos
	lcd_cmd(display,0x00,0x0E);		//BLINK OFF
	lcd_goto(display,0x42);         //GOTO posicion 42
	lcd_puts(display,"Hello world!");//Escribimos caracteres
	while(1){
		lcd_goto(display,0x07);     //Vamos a la posicion 7
		lcd_putch(display,(counter/10)+'0');    //Escribimos decenas
		lcd_putch(display,(counter%10)+'0');    //Escribimos unidades
		counter++;                              //Aumentamos el contador
        if(counter==100){
			counter = 0;
		}
		delay(500);                 //Esperamos medio segundo
	}
}
