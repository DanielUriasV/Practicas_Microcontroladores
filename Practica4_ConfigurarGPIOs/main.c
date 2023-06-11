/* Reaction time game */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "my_gpio.c"



//Entradas y salidas
#define LED1_GPIO    GPIO_NUM_21
#define LED2_GPIO    GPIO_NUM_19
#define LED3_GPIO    GPIO_NUM_5
#define LED4_GPIO    GPIO_NUM_16
#define BTN1_GPIO    GPIO_NUM_4
#define BTN2_GPIO    GPIO_NUM_23
#define BTN3_GPIO    GPIO_NUM_22
#define BTN4_GPIO    GPIO_NUM_17


// Enumerations
typedef enum ButtonState_tag
{
    eBtnUndefined = 0,
    eBtnShortPressed,
    eBtnLongPressed
} eButtonState_t;

typedef enum PlayerInputState_tag
{
    eWaiting = 0,
    eCorrect,
    eIncorrect
} ePlayerInputState_t;

typedef enum ButtonId_tag
{
    eButtonId0 = 0,
    eButtonId1,
    eButtonId2,
    eButtonId3,
    eMaxButtonId
} eButtonId_t;

typedef enum GameState_tag
{
    eGameRestart = 0,
    eWaitForStart,
    ePlayPattern,
    eWaitForPlayer,
    eYouWin,
    eYouLose
} eGameState_t;


#define MAX_LEVEL  4
#define NUM_OF_LEDS 4

// Global variable
uint32_t _millis;
uint8_t randomPattern[MAX_LEVEL];
uint8_t level;

uint8_t repeticiones=0;
uint8_t EstadoBoton1=0;
uint8_t EstadoBoton2=0;
uint8_t EstadoBoton3=0;
uint8_t EstadoBoton4=0;
uint8_t botonActual=0;
uint32_t Pulsacion;
uint8_t i=0;
uint8_t j=0;
uint8_t k=0;
uint8_t flag=1;


static void initIO(void)
{
    //Reset
    gpio_reset_pin(BTN1_GPIO);
    gpio_reset_pin(BTN2_GPIO);
    gpio_reset_pin(BTN3_GPIO);
    gpio_reset_pin(BTN4_GPIO);

    /* Set LED GPIO as a push/pull output */
    my_gpio_init(21, eOutput);
    my_gpio_init(19, eOutput);
    my_gpio_init(5,  eOutput);
    my_gpio_init(16, eOutput);


    /* Set BTN GPIO as a input pull up */
    my_gpio_init(17, eInputPullUp);
    my_gpio_init(22, eInputPullUp);
    my_gpio_init(23, eInputPullUp);
    my_gpio_init(4, eInputPullUp);

}


//Tiempo
void delayMs(uint16_t ms)
{
  vTaskDelay( ms / portTICK_PERIOD_MS );
}

//Funcion para controlar los Leds
void updateLeds(uint8_t numberLed, uint8_t reverse)
{
    //Normal
    if(numberLed == 0 && reverse == 0){
        gpio_write(LED1_GPIO, eHigh);
        gpio_write(LED2_GPIO, eHigh);        
        gpio_write(LED3_GPIO, eHigh);        
        gpio_write(LED4_GPIO, eHigh);
    }
    if(numberLed == 1 && reverse == 0){
        gpio_write(LED1_GPIO, eLow);
        gpio_write(LED2_GPIO, eHigh);        
        gpio_write(LED3_GPIO, eHigh);        
        gpio_write(LED4_GPIO, eHigh);
    }
    if(numberLed == 2 && reverse == 0){
        gpio_write(LED1_GPIO, eHigh);
        gpio_write(LED2_GPIO, eLow);        
        gpio_write(LED3_GPIO, eHigh);        
        gpio_write(LED4_GPIO, eHigh);
    }
    if(numberLed == 3 && reverse == 0){
        gpio_write(LED1_GPIO, eHigh);
        gpio_write(LED2_GPIO, eHigh);        
        gpio_write(LED3_GPIO, eLow);        
        gpio_write(LED4_GPIO, eHigh);
    }
    if(numberLed == 4 && reverse == 0){
        gpio_write(LED1_GPIO, eHigh);
        gpio_write(LED2_GPIO, eHigh);        
        gpio_write(LED3_GPIO, eHigh);        
        gpio_write(LED4_GPIO, eLow);
    }

    //Reverse
    if(numberLed == 0 && reverse == 1){
        gpio_write(LED1_GPIO, eLow);
        gpio_write(LED2_GPIO, eLow);        
        gpio_write(LED3_GPIO, eLow);        
        gpio_write(LED4_GPIO, eLow);
    }
}

bool playSequence(eGameState_t gameState)
{
    switch(gameState){

        //eGameRestart:
        case 0:
        break;

        //eWaitForStart:
        case 1:
            if(_millis<500){
                if(i != 0) {
                    updateLeds(1, 0);
                    _millis++;
                    delayMs(1);
                }
                if(i != 1) {
                    updateLeds(2, 0);
                    _millis++;
                    delayMs(1);
                }
                if(i != 2) {
                    updateLeds(3, 0);
                    _millis++;
                    delayMs(1);
                }
                if(i != 3) {
                    updateLeds(4, 0);
                    _millis++;
                    delayMs(1);
                }
            }
    
            if(_millis>=500){
                _millis = 0;
                i++;
                if(i > 4) i=0;
            }
        break;


        //ePlaySequence: 
        case 2:
            if(_millis <= 500){
                updateLeds(0, 0);
                flag=1;
                if(_millis >= 500) flag=0;
                return true;
            }

            else if(_millis>2000){
                _millis=0;
                flag=0;
                return true;
            }

            else if(_millis>1500 && flag==0){
                k++;
                if(k>level){
                    k=0;
                    updateLeds(0, 0);
                    return false;
                }
                flag=1;
            }

            if(flag==0){

                if(randomPattern[k]==0){
                    updateLeds(1, 0);
                    return true;
                } 
                if(randomPattern[k]==1){
                    updateLeds(2, 0);
                    return true;
                }
                if(randomPattern[k]==2){
                    updateLeds(3, 0);
                    return true;
                }
                if(randomPattern[k]==3){
                    updateLeds(4, 0);
                    return true;
                }
            }

            else if(flag==1){
                    updateLeds(0, 0);
                    return true;
            }
            return false;
        break;

        //eWaitForPlayer:
        case 3:
        break;

        //eYouWin:
        case 4:
            if(repeticiones < 3){

                if(_millis<250) updateLeds(1, 0);
                else if(_millis>=250 && _millis<=500)   updateLeds(2, 0);
                else if(_millis>500 && _millis<=750)    updateLeds(3, 0);
                else if(_millis>750 && _millis<=1000)   updateLeds(4, 0);
                else if(_millis>1000 && _millis<=1250)  updateLeds(3, 0);
                else if(_millis>1250 && _millis<=1500)  updateLeds(2, 0);

                else if(_millis>1500){
                    _millis=0;
                    repeticiones++;
                }

                delayMs(1);
                _millis++;
                return true;
            }
            
            else if(repeticiones >= 3){
                
                if(_millis<250) updateLeds(0, 0);
                else if(_millis>=250 && _millis<=500) updateLeds(0, 1);

                else if(_millis>500){
                    _millis=0;
                    repeticiones++;
                    if(repeticiones==6){
                        repeticiones=0;
                        return false;
                    }
                }

                delayMs(1);
                _millis++;
                return true;
            }
        break;

        //eYouLoose:
        case 5:
            if(_millis<1000) updateLeds(0, 0);
            else if(_millis>=1000 && _millis<=2000) updateLeds(0, 1);

            else if(_millis>2000){
                _millis=0;
                repeticiones++;
            }

            if(repeticiones>3){
                repeticiones=0;
                return false;
            }
            return true;
        break;
    }
    return false;
}


eButtonState_t checkButtons(eButtonId_t *buttonNumber)
{
    //Cuando los botones se pulsan
    if(gpio_read(BTN1_GPIO) == 0){
        Pulsacion++;
        *buttonNumber = eButtonId0;
        updateLeds(1, 0);
        printf("Boton 1");
    }
    if(gpio_read(BTN2_GPIO) == 0){
        Pulsacion++;
        *buttonNumber = eButtonId1;
        updateLeds(2, 0);
        printf("Boton 2");
    }
    if(gpio_read(BTN3_GPIO) == 0){
        Pulsacion++;
        *buttonNumber = eButtonId2;
        updateLeds(3, 0);
        printf("Boton 3");
    }
    if(gpio_read(BTN4_GPIO) == 0){
        Pulsacion++;
        *buttonNumber = eButtonId3;
        updateLeds(4, 0);
        printf("Boton 4");
    }

    //Cuando los botones se dejan de pulsar
    if (gpio_read(BTN1_GPIO) == 1 && gpio_read(BTN2_GPIO) == 1 && gpio_read(BTN3_GPIO) == 1 && gpio_read(BTN4_GPIO) == 1 )
    {
        updateLeds(0, 0);
        if(Pulsacion > 35 && Pulsacion <= 1000){
            Pulsacion = 0;
            return eBtnShortPressed;
        }
        if(Pulsacion > 1000){
            Pulsacion = 0;
            return eBtnLongPressed;
        }
        else Pulsacion=0;
    }
    return eBtnUndefined;
}


ePlayerInputState_t checkPlayerInput(eButtonState_t buttonState, eButtonId_t buttonId)
{
    if(buttonState == eBtnShortPressed){
        if(randomPattern[j] == buttonId){
            printf("J: %i Secuencia: %i, Boton: %i, Correcto \n", j, randomPattern[j], buttonId);
            j++;
            if(j > level){
                j=0;
                printf("Nivel aumentado: %i \n", level+1);
                return eCorrect;
            }
            return eWaiting;
        }

        if (randomPattern[j] != buttonId){
            printf("J: %i Secuencia: %i, Boton: %i, Incorrecto \n", j, randomPattern[j], buttonId);

            //Limpiar todo
            level=0;
            j=0;
            k=0;
            i=0;
            flag=1;
            _millis=0;

            return eIncorrect;
        }
    }

    if(buttonState == eBtnLongPressed) j=0;
    return eWaiting;
}


int app_main(void)
{
    eGameState_t  currentGameState = eGameRestart;
    ePlayerInputState_t playerInputState;
    eButtonId_t buttonId;
    eButtonState_t buttonState;
    initIO();

    while(1)
    {   
        buttonState = checkButtons(&buttonId);

        if (buttonState == eBtnLongPressed){
            currentGameState = eGameRestart;

            //Limpiar todo
            level=0;
            j=0;
            k=0;
            i=0;
            flag=1;
            _millis=0;
            
        }

        switch(currentGameState)
        {
            case eGameRestart:

            printf("Se creara el patron aleatorio \n");

                for (uint8_t idx = 0; idx < MAX_LEVEL; idx++ )
                {
                    randomPattern[idx] = esp_random() & (NUM_OF_LEDS - 1);
                    printf("Posicion[%i] = %i \n", idx, randomPattern[idx]);
                }
                printf("Se creo el patron aleatorio \n");
                level = 0;
                currentGameState++;
                break;

            case eWaitForStart:
                playSequence(eWaitForStart);
                if (buttonState == eBtnShortPressed)    
                    currentGameState++;
                break;

            case ePlayPattern:

                if (!playSequence(ePlayPattern)){
                    printf("Secuencia terminada \n");
                    currentGameState++;
                }
                break;

            case eWaitForPlayer:

                playerInputState = checkPlayerInput(buttonState, buttonId);
                if (playerInputState == eCorrect)
                {
                    level++;
                    if (level < MAX_LEVEL)
                    {
                        currentGameState = ePlayPattern;
                    }
                    else
                    {
                        currentGameState = eYouWin;
                        printf("Ganaste \n");
                    }
                }
                else if (playerInputState == eIncorrect)
                {
                    currentGameState = eYouLose;
                    printf("Perdiste \n");
                }
                break;
            case eYouLose:
            case eYouWin:
                if (!playSequence(currentGameState))
                    currentGameState = eGameRestart;
                break;
        }
        delayMs(1);
        _millis++;
    }
}