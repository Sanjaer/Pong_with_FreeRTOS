#include "stm32f4xx_conf.h"
#include "joy_driver.h"

void Init_Joy(void){

	//1- Estructura de Configuraci�n
	GPIO_InitTypeDef gpio;

	//2 - Habilitaci�n del reloj del perif�rico
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//3 - Relleno de la estructura de configuraci�n
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;	//La configuraci�n afecta a los pines 2, 3, 4 y 5
	gpio.GPIO_Mode = GPIO_Mode_IN;				//Pines como entradas
	gpio.GPIO_PuPd = GPIO_PuPd_UP;				//Con resistencia de pull-up

	//4 - Escritura de la configuraci�n en el perif�rico
	GPIO_Init(GPIOA,&gpio);		//Se especifica el perif�rico y un puntero  la estructura de configuraci�n
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
