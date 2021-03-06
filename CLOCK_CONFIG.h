/*
 *
 *	FACULTAD DE MATEMATICAS
 *	UNIVERSIDAD AUTONOMA DE YUCATAN
 *
 *	CLASE DE SISTEMAS EMBEBIDOS
 *
 *	MACROS PARA EL CONTROL DEL RELOJ DEL SISTEMA
 *  Y RETARDOS
 *
 *
 * 		AUTOR : Josepablo Cruz Baas
 * 		FECHA :	5 - NOVIEMBRE - 2016
 *
 *	DESCRIPCION :
 *		DEFINICION DE MACROS QUE CONFIGURAN EL RELOJ DEL SISTEMA Y 
 *    ACTIVAN LAS FUNCIONES DELAY Y DELAY_US
 *
 *  DESCRIPCION DE USO :
 *    DEFINIR LAS CONSTANTES HZ Y CONFIG ANTES DE INCLUIR EN EL HEADER
 *    HZ ES LA FRECUENCIA DEL RELOJ EN Hz Y CONFIG LA CONFIGURACION
 *    CORRESPONDIENTE SEGUN LA FUNCION SysCtlClockFreqSet ESPECIFICADA
 *    EN EL LIBRO TivaWareTM Peripheral Driver Library PAG 483
 *    POR EJEMPLO:
 *    HZ      40000000
 *    CONFIG  SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320
 *    ESTO CONFIGURA LAS MACROS Delay, Delay_us y setClockReady A
 *    LA FRECUENCIA DESCRITA CON SU RESPECTIVA CONFIGURACION. SI NO SE DEFINEN
 *    SE CONFIGURA POR DEFECTO USANDO EL OSCILADOR INTERNO DE 16MHZ CON PLL DE 320MHZ
 *    PARA TRABAJAR A 16MHZ
 *
 * */
 
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"


#ifndef CLOCK_CONFIG_H_
	#define CLOCK_CONFIG_H_
	//SI NO SE HA DEFINIDO HZ SE CONFIGURA POR DEFECTO
	#ifndef	HZ
		#define HZ		16000000
	#endif
	//SI NO SE HA DEFINIDO UNA CONFIGURACION SE CONFIGURA POR DEFECTO
	#ifndef	CONFIG
		#define	CONFIG	SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320
	#endif
	//MANDAR CONFIGURACION DE RELOJ TM4C129
	#define setClockReady() SysCtlClockFreqSet(CONFIG,HZ);
	//DELAY EN MILISEGUNDOS
	#define delay(TIME)		SysCtlDelay((HZ/3000)*TIME);
	//DELAY EN MICROSEGUNDOS
	#define delay_us(TIME)	SysCtlDelay((HZ/3000000)*TIME);
#endif /* CLOCK_CONFIG_H_ */
