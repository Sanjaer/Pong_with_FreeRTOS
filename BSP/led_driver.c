#include "stm32f4xx_conf.h"
#include "led_driver.h"

void Init_Leds(void){

	//1- Estructura de Configuración
	GPIO_InitTypeDef gpio;

	//2 - Habilitación del reloj del periférico
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//3 - Relleno de la estructura de configuración
	gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//La configuración afecta a los pines 2 y 3
	gpio.GPIO_Mode = GPIO_Mode_OUT;				//Pines como salidas
	gpio.GPIO_Speed = GPIO_Speed_50MHz;			//Velocidad del puerto a 100MHz
	gpio.GPIO_OType = GPIO_OType_PP;			//La salida es en modo PushPull
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;			//Sin resistencas pull-up ni pull-down

	//4 - Escritura de la configuración en el periférico
	GPIO_Init(GPIOD,&gpio);	//Se especifica el periférico y un puntero la estructura de configuración


}

void LED_On (uint8_t led){
	switch (led){
	case 0:									//LEDS 1..4
		GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

		break;
	case 1:									//LED 1
		GPIO_SetBits(GPIOD,GPIO_Pin_12);
		break;
	case 2:									//LED 2
		GPIO_SetBits(GPIOD,GPIO_Pin_13);
		break;
	case 3:									//LED 3
		GPIO_SetBits(GPIOD,GPIO_Pin_14);
		break;
	case 4:									//LED 4
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		break;
	}
}


void LED_Off (uint8_t led){
	switch (led){
	case 0:
		GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

		break;
	case 1:
		GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		break;
	case 2:
		GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		break;
	case 3:
		GPIO_ResetBits(GPIOD,GPIO_Pin_14);
		break;
	case 4:
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		break;
	}
}

void LED_Toggle(uint8_t led){

	switch (led){
	case 0:
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		break;
	case 1:
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
		break;
	case 2:
		GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
		break;
	case 3:
		GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
		break;
	case 4:
		GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
		break;
	}

}


