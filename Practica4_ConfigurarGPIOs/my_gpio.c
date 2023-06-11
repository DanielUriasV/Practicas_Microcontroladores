#include "my_gpio.h"
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "inttypes.h"
#include "gpio_periph.c"


#define _DEF_REG_32b( addr ) ( *(volatile uint32_t *)( addr ) )

//Registers GPIO
#define GPIO_OUT_REG        _DEF_REG_32b    ( 0x3FF44004 )  // Output value 0-31        W/O
#define GPIO_OUT1_REG       _DEF_REG_32b    ( 0x3FF44010 )  // Output value 32-39       W/O
#define GPIO_ENABLE_REG     _DEF_REG_32b    ( 0x3FF44020 )  // Enable Output 0-31       W/O
#define GPIO_ENABLE1_REG    _DEF_REG_32b    ( 0x3FF4402C )  // Enable Output 32-39      W/O
#define GPIO_IN_REG         _DEF_REG_32b    ( 0x3FF4403C )  // Input Value 0-31         RO
#define GPIO_IN1_REG        _DEF_REG_32b    ( 0x3FF44040 )  // Input Value 32-39        RO 
//#define GPIO_PINn_REG     _DEF_REG_32b    ( 0x3FF44000 + ( 0x88 + (0x4*gpioNum) ) )  // PINn Register Configuration        W/0



gpioInitError_t my_gpio_init(gpioNum_t gpioNum, gpioMode_t gpioMode)
{

    //Estos pines no se pueden utilizar
    if (gpioNum == 24 || gpioNum == 28 || gpioNum == 29 || gpioNum == 30 || gpioNum == 31) return eGpioInitError;

    //Definir direccion de memoria
    #define IO_MUX_x_REG        _DEF_REG_32b    (GPIO_PIN_MUX_REG[gpioNum]) // Mux 0 - 39    W/O

    // Limpiar registro
    IO_MUX_x_REG &= 0;    

    //Acciona  tomar dependiendo el tipo de modo
    //Input
    if (gpioMode == eInput){
        IO_MUX_x_REG |= (1 << 8);
        
        if(gpioNum > 31){
            uint32_t mascara = 0xFFFFFFFF;
            mascara ^= (1 << (gpioNum-32)); 
            GPIO_ENABLE1_REG &= mascara;
        }
        else{
            uint32_t mascara = 0xFFFFFFFF;
            mascara ^= (1 << gpioNum); 
            GPIO_ENABLE_REG &= mascara;
        }
    } 

    //Input Pull-UP
    else if (gpioMode == eInputPullUp){
        IO_MUX_x_REG |= (3 << 8);

        if(gpioNum > 31){
            uint32_t mascara = 0xFFFFFFFF;
            mascara ^= ( 1 << (gpioNum-32) ); 
            GPIO_ENABLE1_REG &= mascara;
        }
        else{
            uint32_t mascara = 0xFFFFFFFF;
            mascara ^= (1 << gpioNum); 
            GPIO_ENABLE_REG &= mascara;
        }
    }

    //Output
    else if (gpioMode == eOutput){
        if(gpioNum > 31){
            GPIO_ENABLE1_REG |= (1 << (gpioNum-32)); 
        }
        else{
            GPIO_ENABLE_REG |= (1 << gpioNum); 
        }

    }
    else return eGpioInitError;
    printf("%08X , %08X \n", GPIO_PIN_MUX_REG[gpioNum], IO_MUX_x_REG);

    return eGpioInitSuccess;
}



void gpio_write(gpioNum_t gpioNum, gpioLevel_t gpioLevel)
{
    
    if (gpioNum == 24 || gpioNum == 28 || gpioNum == 29 || gpioNum == 30 || gpioNum == 31) return;

    if(gpioLevel == 0){
        uint32_t mascara = 0xFFFFFFFF;
        mascara ^= (1 << gpioNum); 
        GPIO_OUT_REG &= mascara;
    }
    else if(gpioLevel == 1){
        if(gpioNum > 31){
            GPIO_OUT1_REG |= (1 << (gpioNum-32)); 
        }
        else{
            GPIO_OUT_REG |= (1 << gpioNum); 
        };
        
    }

    return;
}



gpioLevel_t gpio_read(gpioNum_t gpioNum)
{
    
    if (gpioNum == 24 || gpioNum == 28 || gpioNum == 29 || gpioNum == 30 || gpioNum == 31) return eMaxLevel;
    
    uint32_t valor = 0;

    if(gpioNum > 31){
        valor = (GPIO_IN1_REG >> (gpioNum-32));
        valor &= 1; 
    }
    else{
        valor = (GPIO_IN_REG >> gpioNum);
        valor &= 1;
        //printf("%08X \n", valor);
    }

    if(valor==1){
        return eHigh;
    }
    else if(valor==0){
        return eLow; 
    }
    else {
        return eMaxLevel; // invalid by default
    }
}