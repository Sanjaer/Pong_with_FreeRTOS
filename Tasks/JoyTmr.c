#include <CoOS.h>			              /*!< CoOS header file	         */
#include "BSP.h"



//Prototipo de la función del timer
void JoyTimer(void);

uint8_t lastKey;


//Declaración de las banderas
OS_FlagID keyFlag;

//Funcion de espera a que una tecla se pulse
uint8_t waitForKey(uint8_t key, uint32_t timeout){

	//Esperar la bandera
	CoWaitForSingleFlag(keyFlag, timeout);
	return key;
}

//Funcion que activa la bandera correspondiente a una tecla
void SetFlagKey(uint8_t key){

	CoSetFlag(keyFlag);

}

//Inicializacion de las banderas
void  CreateJoyFlags(void){
	uint8_t i;

	//Creacion de la bandera
	keyFlag=CoCreateFlag(1,0);

}

//Creación del timer del joy
void CreateJoyTimer(void){

	//ID del timer
	OS_TCID joyId;
	uint32_t delay = 1;
	uint32_t period = 100;

	//Inicialización del joystick
	Init_Joy();
	lastKey = -1;

	//Creación e inciación del timer
	joyId=CoCreateTmr(TMR_TYPE_PERIODIC,delay,period,JoyTimer);
	CoStartTmr(joyId);
}


//Funcion del timer software
void JoyTimer(void){

	uint8_t key;

	key = Read_Joy();

	if (key != lastKey && lastKey == 0)	SetFlagKey(key);

	lastKey = key;
}



