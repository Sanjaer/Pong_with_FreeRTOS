#include "stm32f4xx_conf.h"
#include "joy_driver.h"

void Init_Joy(void){

	//1- Estructura de Configuración
	GPIO_InitTypeDef gpio;

	//2 - Habilitación del reloj del periférico
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//3 - Relleno de la estructura de configuración
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;	//La configuración afecta a los pines 2, 3, 4 y 5
	gpio.GPIO_Mode = GPIO_Mode_IN;				//Pines como entradas
	gpio.GPIO_PuPd = GPIO_PuPd_UP;				//Con resistencia de pull-up

	//4 - Escritura de la configuración en el periférico
	GPIO_Init(GPIOA,&gpio);		//Se especifica el periférico y un puntero  la estructura de configuración
}

uint8_t Read_Joy (void){

	uint8_t joy=0;

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0){
		joy=1;	//Joy 1 pulsado
	}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==0){
		joy=1;	//Joy 2 pulsado
	}

	return joy;

}
