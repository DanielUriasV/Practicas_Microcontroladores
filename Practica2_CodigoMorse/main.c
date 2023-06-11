#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define LED_GPIO GPIO_NUM_2

#define DOT     10
#define DASH    (DOT * 3)   
#define PAUSE   (DOT)
#define BREAK   (DOT * 5)
#define END     0

/*A*/ uint8_t mA[] = {DOT, DASH, END};
/*B*/ uint8_t mB[] = {DASH, DOT, DOT, DOT, END};
/*C*/ uint8_t mC[] = {DASH, DOT, DASH, DOT, END};
/*D*/ uint8_t mD[] = {DASH, DOT, DOT, END};
/*E*/ uint8_t mE[] = {DOT, END};
/*F*/ uint8_t mF[] = {DOT, DOT, DASH, DOT, END};
/*G*/ uint8_t mG[] = {DASH, DASH, DOT, END};
/*H*/ uint8_t mH[] = {DOT, DOT, DOT, DOT, END};
/*I*/ uint8_t mI[] = {DOT, DOT};
/*J*/ uint8_t mJ[] = {DOT, DASH, DASH, DASH, END};
/*K*/ uint8_t mK[] = {DASH, DOT, DASH, END};
/*L*/ uint8_t mL[] = {DOT, DASH, DOT, DOT, END};
/*M*/ uint8_t mM[] = {DASH, DASH, END};
/*N*/ uint8_t mN[] = {DASH, DOT, END};
/*Ñ*/ uint8_t mENIE[] = {DASH, DASH, DOT, DASH, DASH, END};
/*O*/ uint8_t mO[] = {DASH, DASH, DASH, END};
/*p*/ uint8_t mP[] = {DOT, DASH, DASH, DOT, END};
/*Q*/ uint8_t mQ[] = {DASH, DASH, DOT, DASH, END};
/*R*/ uint8_t mR[] = {DOT, DASH, DOT, END};
/*S*/ uint8_t mS[] = {DOT, DOT, DOT, END};
/*T*/ uint8_t mT[] = {DASH, END};
/*U*/ uint8_t mU[] = {DOT, DOT, DASH, END};
/*V*/ uint8_t mV[] = {DOT, DOT, DOT, DASH, END};
/*W*/ uint8_t mW[] = {DOT, DASH, DASH, END};
/*X*/ uint8_t mX[] = {DASH, DOT, DOT, DASH, END};
/*Y*/ uint8_t mY[] = {DASH, DOT, DASH, DASH, END};
/*Z*/ uint8_t mZ[] = {DASH, DASH, DOT, DOT, END};

uint8_t *nombre[] = {mJ, mU, mA, mN, mD, mA, mN, mI, mE, mL, mU, mR, mI, mA, mS, mV, mE, mG, mA};
static uint8_t s_led_state = 0;

static void update_led(void)
{
    s_led_state = !s_led_state;
    gpio_set_level(LED_GPIO, s_led_state); 
}


static void configure_led(void)
{
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);
}


void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}


void sendLetter(uint8_t letra[]){

    uint8_t i = 0;

    while(letra[i]){

        //Valores reales, no es perceptible.
        /*
        update_led();
        delayMs(letra[i]);

        update_led();
        delayMs(10);
        */    

        //Para poder visualizarlo
        update_led();

        if(letra[i] == 10){
             delayMs(200);
        }
        if(letra[i] == 30){
            delayMs(600);
        }

        update_led();
        delayMs(600);

        i++;

    }

}

void sendMorse(uint8_t *data[])
{
    uint8_t j = 0;

    while(data[j]){

        sendLetter(data[j]);
        delayMs(1000);
        j++;
    }

    j = 0;

}


void app_main(void)
{

    configure_led();
    printf("Juan Daniel Urias Vega");


    while(1)
    {
 
        sendMorse(nombre);

    }
}
