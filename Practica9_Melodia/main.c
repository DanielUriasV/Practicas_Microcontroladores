/* Reaction time game */
#include <stdio.h>
#include <string.h>
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "myUart.h"
#include "myTimer.h"
#include "songs.h"


//Entradas y salidas
#define Bocina    17

static void initIO(void)
{
    gpio_reset_pin(Bocina);
    gpio_set_direction(Bocina, GPIO_MODE_OUTPUT);
}


//Variables
int cancionActual = 0;
char cad[100];




//Main
int app_main()
{

    uartInit(PC_UART_PORT, 115200, 8, 0, 1, PC_UART_TX_PIN, PC_UART_RX_PIN);
    initIO();
    delayMs(1000);
    uartClrScr(PC_UART_PORT);

    timer0Init();

    while(1)
    {
        esp_task_wdt_init(999, false);
        if (uartKbhit(0))
        {

            switch (uartGetchar(0))
            {
                //Play song
                case 'p':
                case 'P':
                        if(cancionActual == 0){
                                sprintf(cad,"Se esta ejecutando Marcha Imperial."); 
                                uartPuts(0, cad);
                                timer1Play(arraySongs[cancionActual][0], arrayLengthSongs[cancionActual]);
                                //timer1Play(ImperialMarch, sizeof(ImperialMarch)/sizeof(struct note));
                        }

                        if(cancionActual == 1){
                                sprintf(cad,"Se esta ejecutando Angeles Azules."); 
                                uartPuts(0, cad);
                                timer1Play(arraySongs[cancionActual][0], arrayLengthSongs[cancionActual]);
                                //timer1Play(AngelesAzules, sizeof(AngelesAzules)/sizeof(struct note));
                        }

                        break;
                
                //Next song
                case 'n':
                case 'N':
                        cancionActual++;
                        if(cancionActual == 2) cancionActual = 0;
                        break;


                //Before song
                case 'b':
                case 'B':
                        cancionActual--;
                        if(cancionActual == -1) cancionActual = 1;
                        break;


                default:
                        break;
            }
        }
        // Prevent watchdog fault
        delayMs(10);
    }
    return 0;
}
