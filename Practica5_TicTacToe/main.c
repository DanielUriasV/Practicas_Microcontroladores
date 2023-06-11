/* Reaction time game */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "my_gpio.h"


#define LED_LINE0  23 
#define LED_LINE1  22 
#define LED_LINE2  21 
#define LED_LINE3  19 
#define LED_LINE4  18 
#define BTN_GPIO    5

#define NUM_LED_PER_COLOR 9

// Enumerations
typedef enum LedColor_tag
{
  eRedLed = 0,
  eGreenLed = 1,
  eNumOfColors = 2
}eLedColor_t;

typedef enum ButtonState_tag
{
    eBtnUndefined = 0,
    eBtnShortKeyPress,
    eBtnDoubleKeyPress,
    eBtnLongKeyPress
} eButtonState_t;

typedef enum GameState_tag
{
    eGameRestart = 0,
    eOngoingGame, 
    eStalemate, 
    eRedPlayerWin, 
    eGreenPlayerWin
} eGameState_t;

typedef struct BoardState_tag
{
    bool gameBoard[eNumOfColors][NUM_LED_PER_COLOR];
    uint8_t cursor;
    eLedColor_t currentColor;
}sBoardState_t;







//----------------------------------------------Mi parte---------------------------------------------------//

// Global variable
uint32_t _millis = 0;
uint32_t _millis_btn1 = 0;
uint32_t _millis_btn2 = 0;
uint32_t _millis_btn3 = 0;
uint32_t _millis_playSequence = 0;
uint32_t _millis_displayBoard1 = 0;
uint32_t _millis_displayBoard2 = 0;
uint32_t cont = 0;
uint32_t i = 0;
uint32_t flag1 = 0;
uint32_t flag2 = 0;

void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

static void initIO(void)
{
    my_gpio_init(BTN_GPIO, eInputPullUp);
    my_gpio_init(LED_LINE0, eInput);
    my_gpio_init(LED_LINE1, eInput);
    my_gpio_init(LED_LINE2, eInput);
    my_gpio_init(LED_LINE3, eInput);
    my_gpio_init(LED_LINE4, eInput);
}

void resetIO(void)
{
    my_gpio_write(LED_LINE0, eLow);
    my_gpio_write(LED_LINE1, eLow);
    my_gpio_write(LED_LINE2, eLow);
    my_gpio_write(LED_LINE3, eLow);
    my_gpio_write(LED_LINE4, eLow);
    my_gpio_init(LED_LINE0, eInput);
    my_gpio_init(LED_LINE1, eInput);
    my_gpio_init(LED_LINE2, eInput);
    my_gpio_init(LED_LINE3, eInput);
    my_gpio_init(LED_LINE4, eInput);
}





//----------------------------Encender--------------------------------//

void encenderLeds(gpioNum_t line1, gpioNum_t line2, eLedColor_t color)
{
    resetIO();
    my_gpio_init(line1, eOutput);
    my_gpio_init(line2, eOutput);
    if(color == eRedLed)
    {
        my_gpio_write(line1, eLow);
        my_gpio_write(line2, eHigh);
    }
    if(color == eGreenLed)
    {
        my_gpio_write(line1, eHigh);
        my_gpio_write(line2, eLow);
    }
    delayMs(2);
}



//-------------------------------------Posicion-------------------------------//

void encenderPosicion(uint32_t position, eLedColor_t color)
{
    if(position == 0) encenderLeds(LED_LINE0, LED_LINE1, color);
    if(position == 1) encenderLeds(LED_LINE0, LED_LINE2, color);
    if(position == 2) encenderLeds(LED_LINE0, LED_LINE3, color);
    if(position == 3) encenderLeds(LED_LINE1, LED_LINE2, color);
    if(position == 4) encenderLeds(LED_LINE1, LED_LINE3, color);
    if(position == 5) encenderLeds(LED_LINE3, LED_LINE4, color);
    if(position == 6) encenderLeds(LED_LINE2, LED_LINE3, color);
    if(position == 7) encenderLeds(LED_LINE1, LED_LINE4, color);
    if(position == 8) encenderLeds(LED_LINE0, LED_LINE4, color);
}



//------------------------------------------------Tablero--------------------------------------------//


void displayBoard(sBoardState_t *boardState)
{
    _millis_displayBoard2++;
    if(boardState->gameBoard[eRedLed][_millis_displayBoard1] == true)   encenderPosicion(_millis_displayBoard1, eRedLed);
    if(boardState->gameBoard[eGreenLed][_millis_displayBoard1] == true) encenderPosicion(_millis_displayBoard1, eGreenLed);

    _millis_displayBoard1++;

    if(_millis_displayBoard1 >= 9)  _millis_displayBoard1 = 0;

    if(_millis_displayBoard2 <= 250) encenderPosicion(boardState->cursor, boardState->currentColor);

    if(_millis_displayBoard2 > 250 &&_millis_displayBoard2 <= 300) resetIO();

    if(_millis_displayBoard2 > 300) _millis_displayBoard2 = 0;
}








//--------------------------------------------Revisar ganador--------------------------------------------//

bool playerWon(sBoardState_t *boardState)
{
    //Lineas Horizontales
    if(boardState->gameBoard[boardState->currentColor][0] == true 
    && boardState->gameBoard[boardState->currentColor][1] == true  
    && boardState->gameBoard[boardState->currentColor][2] == true)
        return 1;

    if(boardState->gameBoard[boardState->currentColor][3] == true 
    && boardState->gameBoard[boardState->currentColor][4] == true  
    && boardState->gameBoard[boardState->currentColor][5] == true)
        return 1;

    if(boardState->gameBoard[boardState->currentColor][6] == true 
    && boardState->gameBoard[boardState->currentColor][7] == true  
    && boardState->gameBoard[boardState->currentColor][8] == true)
        return 1;

    //Lineas Vericales
    if(boardState->gameBoard[boardState->currentColor][0] == true 
    && boardState->gameBoard[boardState->currentColor][3] == true  
    && boardState->gameBoard[boardState->currentColor][6] == true)
        return 1;

    if(boardState->gameBoard[boardState->currentColor][1] == true 
    && boardState->gameBoard[boardState->currentColor][4] == true  
    && boardState->gameBoard[boardState->currentColor][7] == true)
        return 1;

    if(boardState->gameBoard[boardState->currentColor][2] == true 
    && boardState->gameBoard[boardState->currentColor][5] == true  
    && boardState->gameBoard[boardState->currentColor][8] == true)
        return 1;


    //Lineas Diagonales
    if(boardState->gameBoard[boardState->currentColor][0] == true 
    && boardState->gameBoard[boardState->currentColor][4] == true  
    && boardState->gameBoard[boardState->currentColor][8] == true)
        return 1;

    if(boardState->gameBoard[boardState->currentColor][2] == true 
    && boardState->gameBoard[boardState->currentColor][4] == true 
    && boardState->gameBoard[boardState->currentColor][6] == true)
        return 1;

    return 0;
}


//----------------------------------------------Revisar empate-------------------------------------------//

bool playerTie(sBoardState_t *boardState)
{
    i = 0;
    cont= 0;

    for(i = 0; i < 9; i++)
    {
        if((boardState->gameBoard[eRedLed][i] == true) || (boardState->gameBoard[eGreenLed][i] == true)) cont++;
    }

    if(cont == 9) return 1;

    return 0;
}







//-------------------------------------------Revisar tablero-------------------------------------------//

eGameState_t checkBoard(sBoardState_t *boardState, eButtonState_t buttonState)
{
    switch (buttonState)
    {

        //Presionado corto
        case eBtnShortKeyPress:
            do{

                if(boardState->cursor == 8) boardState->cursor = 0;

                else boardState->cursor = (boardState->cursor + 1);

            }while((boardState->gameBoard[eRedLed][boardState->cursor] != false) || (boardState->gameBoard[eGreenLed][boardState->cursor] != false));
            break;

        //Presionado doble
        case eBtnDoubleKeyPress:
            do{
                if(boardState->cursor == 0) boardState->cursor = 8;
                
                else boardState->cursor = (boardState->cursor - 1);
                
            }while((boardState->gameBoard[eRedLed][boardState->cursor] != false) || (boardState->gameBoard[eGreenLed][boardState->cursor] != false));
            break;

        //Presionado largo
        case eBtnLongKeyPress:
            if(boardState->gameBoard[eRedLed][boardState->cursor] == false && boardState->gameBoard[eGreenLed][boardState->cursor] == false)
            {
                boardState->gameBoard[boardState->currentColor][boardState->cursor] = true;
                if(playerWon(boardState) == 1)
                {
                    if(boardState->currentColor == eRedLed) return eRedPlayerWin;
                    if(boardState->currentColor == eGreenLed) return eGreenPlayerWin;

                }
                if(playerTie(boardState) == 1) return eStalemate;

                boardState->cursor = 0;
                while((boardState->gameBoard[eRedLed][boardState->cursor] != false) || (boardState->gameBoard[eGreenLed][boardState->cursor] != false)){
                    
                    if(boardState->cursor == 8) boardState->cursor = 0;   
                    else boardState->cursor = (boardState->cursor + 1);

                }
                boardState->currentColor = (boardState->currentColor == eRedLed)? eGreenLed : eRedLed;
            }
            else return eOngoingGame;
            break;

        //Default
        default:
            return eOngoingGame;
            break;
    }
    return eOngoingGame;
}


//-------------------------------------------Colores------------------------------------------------//


//Secuencua empate
void tieSequence(eLedColor_t color)
{
    encenderPosicion(0, color);
    encenderPosicion(2, color);
    encenderPosicion(4, color);
    encenderPosicion(6, color);
    encenderPosicion(8, color);
}


//Secuencia ganadora
void wonSequence(eLedColor_t color)
{
    tieSequence(color);
    encenderPosicion(1, color);
    encenderPosicion(3, color);
    encenderPosicion(5, color);
    encenderPosicion(7, color);
}


//-------------------------------------------Secuencias------------------------------------//

bool playSequence(eGameState_t gameState)
{
    _millis_playSequence++;
    if(gameState == eStalemate)
    {
        if(_millis_playSequence <= 500) tieSequence(eRedLed);

        if(_millis_playSequence > 500 && _millis_playSequence <= 750) resetIO();

        if(_millis_playSequence > 750 && _millis_playSequence <= 1250) tieSequence(eGreenLed);

        if(_millis_playSequence > 1250 && _millis_playSequence <= 1500) resetIO();

        if(_millis_playSequence > 1500)
        {
            _millis_playSequence = 0;
            return false;
        }
    }

    if(gameState == eRedPlayerWin)
    {
        if(_millis_playSequence <= 500) wonSequence(eRedLed);

        if(_millis_playSequence > 500 && _millis_playSequence <= 750) resetIO();

        if(_millis_playSequence > 750 && _millis_playSequence <= 1250) wonSequence(eRedLed);

        if(_millis_playSequence > 1250 && _millis_playSequence <= 1500) resetIO();  

        if(_millis_playSequence > 1500)
        {
            _millis_playSequence = 0;
            return false;
        }
    }

    if(gameState == eGreenPlayerWin)
    {
        if(_millis_playSequence <= 500) wonSequence(eGreenLed);

        if(_millis_playSequence > 500 && _millis_playSequence <= 750) resetIO();

        if(_millis_playSequence > 750 && _millis_playSequence <= 1250) wonSequence(eGreenLed);

        if(_millis_playSequence > 1250 && _millis_playSequence <= 1500) resetIO();   

        if(_millis_playSequence > 1500)
        {
            _millis_playSequence = 0;
            return false;
        }
    }

    return true;
}




eButtonState_t checkButton(void)
{
    if(my_gpio_read(BTN_GPIO))
    {
        _millis_btn1++;
        if (flag1 == 1)
        {
            flag2 = 1;
            _millis_btn3++;
        }
    }
    if(!my_gpio_read(BTN_GPIO))
    {
        if(_millis_btn1 <= 50)
        {
            _millis_btn1 = 0;
            return eBtnUndefined;
        }
        if(_millis_btn1 >= 1000 && flag1 == 0)
        {
            _millis_btn1 = 0;
            return eBtnLongKeyPress;
        }
        if(_millis_btn3 >= 1000)
        {
            _millis_btn1 = 0;
            _millis_btn2 = 0;
            _millis_btn3 = 0;
            flag1 = 0;
            flag2 = 0;
            return eBtnLongKeyPress;
        }
        flag1 = 1;
        _millis_btn2++;
        if(flag2 == 0 && _millis_btn2 > 500)
        {
            _millis_btn1 = 0;
            _millis_btn2 = 0;
            _millis_btn3 = 0;
            flag1 = 0;
            return eBtnShortKeyPress;
        }
        if(flag2 == 1 && _millis_btn2 <= 500)
        {
            _millis_btn1 = 0;
            _millis_btn2 = 0;
            _millis_btn3 = 0;
            flag1 = 0;
            flag2 = 0;
            return eBtnDoubleKeyPress;
        }
    }
    return eBtnUndefined;
}




int app_main(void)
{
    eGameState_t  currentGameState = eGameRestart;
    eButtonState_t buttonState;
    sBoardState_t boardState;
    initIO();

    while(1)
    {   
        buttonState = checkButton();
        
        switch(currentGameState)
        {
            case eGameRestart:
                for (uint8_t idx = 0; idx < NUM_LED_PER_COLOR; idx++ )
                {
                    boardState.gameBoard[eRedLed][idx] = false;
                    boardState.gameBoard[eGreenLed][idx] = false;
                    boardState.cursor = 0;
                    boardState.currentColor = eRedLed;
                }

                //Limpiar todas las variables
                _millis = 0;
                _millis_btn1 = 0;
                _millis_btn2 = 0;
                _millis_btn3 = 0;
                _millis_playSequence = 0;
                _millis_displayBoard1 = 0;
                _millis_displayBoard2 = 0;
                cont = 0;
                flag1 = 0;
                flag2 = 0;
                i = 0;
                //---------------------------

                currentGameState = eOngoingGame;
                break;
            case eOngoingGame:
                if (buttonState != eBtnUndefined)
                    currentGameState = checkBoard(&boardState, buttonState);
                displayBoard(&boardState);
                break;
            case eStalemate:
            case eRedPlayerWin:
            case eGreenPlayerWin:
                if (!playSequence(currentGameState))
                    currentGameState = eGameRestart;
            break;
        }
        delayMs(1);
        _millis++;
    }
}