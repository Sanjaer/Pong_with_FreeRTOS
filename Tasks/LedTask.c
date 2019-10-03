#include <CoOS.h>			              /*!< CoOS header file	         */
#include "BSP.h"
#include "JoyTmr.h"

//Prototipos de las tareas
void LedTask0(void * parg);



//Pila de la tarea
OS_STK     led_stk[4][64];

void CreateLedTask(void){

	Init_Leds();	//Inicializaci�n de los leds


	CoCreateTask (LedTask0, 0 , 2 ,&led_stk[0][63], 64);


}

void LedTask0(void * parg){
	//Inicializaci�n de la tarea

	//Cuerpo de la tarea
	for (;;) {
		waitForKey(1,0);
		LED_Toggle(0);
		waitForKey(1,0);
		LED_Toggle(0);
	}

}

