///Librerias utilizadas
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "my_gpio.h"

//Puertos utilizados de el ESP32
//Este es el puerto para el boton
#define GPIO_IOMUX_5	(uint32_t *) (0x3FF4906C);

//Estos son los puertos para las lineas de la matriz
#define GPIO_IOMUX_23	(uint32_t *) (0x3FF4908C);
#define GPIO_IOMUX_22	(uint32_t *) (0x3FF49080);
#define GPIO_IOMUX_21	(uint32_t *) (0x3FF4907C);
#define GPIO_IOMUX_19	(uint32_t *) (0x3FF49074);
#define GPIO_IOMUX_18	(uint32_t *) (0x3FF49070);

//Leer el valor de salida
//Con este miro los estados de los pines en sus salida
volatile uint32_t * GPIO_OUT_REG = (uint32_t *) 0x3FF44004; // pines 0 - 31
volatile uint32_t * GPIO_OUT_REG1 = (uint32_t *) 0x3FF44010; //Pines 32-39

//Leer el valor de entrada
//Con este miro los estados de los pines en sus entradas
volatile uint32_t * GPIO_IN_REG = (uint32_t *) 0x3FF4403C; // pines 0 - 31
volatile uint32_t * GPIO_IN1_REG = (uint32_t *) 0x3FF44040; //Pines 32-39

//Habilitar la salida en los pines
volatile uint32_t * GPIO_ENABLE_REG = (uint32_t *) 0x3FF44020; // pines 0 - 31
volatile uint32_t * GPIO_ENABLE_REG1 = (uint32_t *) 0x3FF4402C; //Pines 32-39
volatile uint32_t * GPIO_OUT_W1TS_REG = (uint32_t *) 0x3FF44008; //Escriben 1 en las salidas que quiera
volatile uint32_t * GPIO_OUT_W1TC_REG = (uint32_t *) 0x3FF4400C; //Escribe 0 en las salidas que quiera o las limpio

//Habilita las salidas
volatile uint32_t *GPIO_OUT_ENABLE_REG_W1TS = (uint32_t *)(0x3FF44024);
volatile uint32_t *GPIO_OUT_ENABLE_REG_W1TC = (uint32_t *)(0x3FF44028);
volatile uint32_t *GPIO_IOMUX_GPIO = (uint32_t *)(0x3FF49074);

//Habilitan o deshabilitan la GPIO Matrix, la GPIO Matrix me habilita lo que es el acceso a UART, PWM, I2C, etc.
volatile uint32_t * GPIO_FUNC0_IN_SEL_CFG_REG = (uint32_t *) 0x3FF44130; // pines 0 - 31
volatile uint32_t * GPIO_FUNC0_OUT_SEL_CFG_REG  = (uint32_t *) 0x3FF44530; //Pines 32-39

//Inicializar un GPIO
gpioInitError_t my_gpio_init(gpioNum_t gpioNum, gpioMode_t gpioMode)
{
    if((gpioNum >= 0 && gpioNum <= 31) && (gpioMode >= 0 && gpioMode <= 2))
    {
    	if(gpioNum == 5)
    	{
    	    GPIO_IOMUX_GPIO = GPIO_IOMUX_5;
    	}
    	if(gpioNum == 23)
    	{
    	    GPIO_IOMUX_GPIO = GPIO_IOMUX_23;
    	}
    	if(gpioNum == 22)
    	{
    	    GPIO_IOMUX_GPIO = GPIO_IOMUX_22;
    	}
    	if(gpioNum == 21)
    	{
    	    GPIO_IOMUX_GPIO = GPIO_IOMUX_21;
    	}
    	if(gpioNum == 19)
    	{
    	    GPIO_IOMUX_GPIO = GPIO_IOMUX_19;
    	}
    	if(gpioNum == 18)
    	{
    	    GPIO_IOMUX_GPIO = GPIO_IOMUX_18;
    	}
		if(gpioMode == eOutput)
		{
			*GPIO_OUT_ENABLE_REG_W1TS = (1 << gpioNum);
			*GPIO_IOMUX_GPIO = (1 << 9);
		}
		if(gpioMode == eInput)
		{
			*GPIO_IOMUX_GPIO = (1 << 9);
			*GPIO_OUT_ENABLE_REG_W1TC = (1 << gpioNum);
		}
		if(gpioMode == eInputPullUp)
		{
			*GPIO_IOMUX_GPIO = (3 << 8);
		}
		return eGpioInitSuccess;
    }
    return eGpioInitError;
}

//Escribe a un pin un valor booleano
void my_gpio_write(gpioNum_t gpioNum, gpioLevel_t gpioLevel)
{
    if (gpioLevel == eLow)
    {
        *GPIO_OUT_W1TS_REG = (1 << gpioNum);
    }
    if (gpioLevel == eHigh)
    {
        *GPIO_OUT_W1TC_REG = (1 << gpioNum);
    }
}

//Leer lo que tiene un GPIO
gpioLevel_t my_gpio_read(gpioNum_t gpioNum)
{
    if(gpioNum >= 0 && gpioNum <= 31)
    {
        if ((*GPIO_IN_REG & (1 << gpioNum)))
        {
            return eLow;
        }
    }
    if(gpioNum >= 32 && gpioNum <= 39)
    {
		if ((*GPIO_IN1_REG & (1 << (gpioNum - 32))))
		{
			return eLow;
		}
	}
    return eHigh;
}