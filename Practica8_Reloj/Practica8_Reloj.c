//Recursos
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>
#include "esp_task_wdt.h"
#include <inttypes.h>
#include "myTimer.h"
#include "myUart.h"

// UART 0
#define PC_UART_PORT    (0)
#define PC_UART_RX_PIN  (3)
#define PC_UART_TX_PIN  (1)

#define UARTS_BAUD_RATE         (115200)
#define TASK_STACK_SIZE         (1048)
#define READ_BUF_SIZE           (1024)

#define CLOCK_X_POS     5
#define CLOCK_Y_POS     5

#define _DEF_REG_32b( addr ) ( *(volatile uint32_t *)( addr ) )
//Registers timers 0
#define TIMING0_T0CONFIG_REG    _DEF_REG_32b    ( 0x3FF5F000 )  // Config
#define TIMG0_T0LO_REG          _DEF_REG_32b    ( 0x3FF5F004 )  // Timer_lo
#define TIMG0_T0HI_REG          _DEF_REG_32b    ( 0x3FF5F008 )  // Timer_lo
#define TIMG0_T0ALARMLO_REG     _DEF_REG_32b    ( 0x3FF5F010 )  // Alarm_lo
#define TIMG0_T0ALARMHI_REG     _DEF_REG_32b    ( 0x3FF5F014 )  // Alarm_hi
#define TIMG0_T0LOADLO_REG      _DEF_REG_32b    ( 0x3FF5F018 )  // Load_lo
#define TIMG0_T0LOADHI_REG      _DEF_REG_32b    ( 0x3FF5F01C )  // Load_lo
#define TIMG0_T0UPDATE_REG      _DEF_REG_32b    ( 0x3FF5F00C )  // Update
#define TIMG0_RAW_REG           _DEF_REG_32b    ( 0x3FF5F09C )  // READ FLAG
#define TIMG0_RAW_CLR_REG       _DEF_REG_32b    ( 0x3FF5F0A4 )  // CLEAR FLAG


//---------------------------------------------------Todo lo de la practica 6--------------------------------------------------------//

void uartInit(uart_port_t uart_num, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop, uint8_t txPin, uint8_t rxPin)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = size-5,
        .parity    = parity,
        .stop_bits = stop,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_driver_install(uart_num, READ_BUF_SIZE, READ_BUF_SIZE, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, txPin, rxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

}

void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}


void uartClrScr(uart_port_t uart_num)
{
    // Uso "const" para sugerir que el contenido del arreglo lo coloque en Flash y no en RAM
    const char caClearScr[] = "\e[2J";
    uart_write_bytes(uart_num, caClearScr, sizeof(caClearScr));
}


void uartGoto11(uart_port_t uart_num)
{
    // Limpie un poco el arreglo de caracteres, los siguientes tres son equivalentes:
     // "\e[1;1H" == "\x1B[1;1H" == {27,'[','1',';','1','H'}
    const char caGoto11[] = "\e[1;1H";
    uart_write_bytes(uart_num, caGoto11, sizeof(caGoto11));
}


bool uartKbhit(uart_port_t uart_num)
{
    uint8_t length;
    uart_get_buffered_data_len(uart_num, (size_t*)&length);
    return (length > 0);
}


void uartPutchar(uart_port_t uart_num, char c)
{
    uart_write_bytes(uart_num, &c, sizeof(c));
}


char uartGetchar(uart_port_t uart_num)
{
    char c;
    // Wait for a received byte
    while(!uartKbhit(uart_num))
    {
        delayMs(10);
    }
    // read byte, no wait
    uart_read_bytes(uart_num, &c, sizeof(c), 0);

    return c;
}


void uartGets(uart_port_t uart_num, char *str)
{
    char c;
    int i = 0;

    while((c = uartGetchar(uart_num)) != 13){
        if(c >= 32 && c <= 126){
            *str = c;
            str++;
            i++;
            uartPutchar(uart_num, c);
        }

        if(c == 8){

            if(i-- > 0)
            str--;
            uartPuts(uart_num, "\e[1D");
            uartPuts(uart_num, "\e[0K");
        }
    }

    *str = '\0';
}


void uartPuts(uart_port_t uart_num, char *str){
    
    while(*str) uartPutchar(uart_num, *str++);

}



void myItoa(uint16_t number, char* str, uint8_t base){

    int n = 0, i = 0, length = 0;
    char temp;

    while(number != 0)
    {
        n = n * 10;
        n = number % base;
        str[i] = n >= 10 && n <= 15 ? n + 55 : n + 48;
        number /= base;
        i++;
    }
    str[i] = '\0';
    length = i;
    for(i = 0; i < length/2; i++)
    {
        temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i -1] = temp;
    }

    //uartPuts(UART_NUM, str);

}


uint16_t myAtoi(char *str){

    uint16_t num = 0, i;
    char c;

    if (str[0] == '0' || strcmp(str, "65536"))
        num = 0;
    
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] > 47 && str[i] < 58)
        {
            num*=10;
            c = str[i];
            c-=48;
            num+=c; 
        } else{
            break;
        }
    }

    return num;

}


void uartSetColor(uart_port_t uart_num, uint8_t color)
{
    char changeC[10];
    sprintf(changeC, "\e[0;%dm", color);
    uartPuts(uart_num, changeC);
}


void uartGotoxy(uart_port_t uart_num, uint8_t x, uint8_t y)
{
    char caGotoxy[11];
    sprintf(caGotoxy, "\e[%d;%dH", x, y);
    uartPuts(uart_num, caGotoxy);
}


//--------------------------------------------------Practica 8------------------------------------------------------//


void my_timer0Init(){

    // Reiniciar el watchdog
    esp_task_wdt_init(999, false);
    
    // Limpiar registros
    TIMG0_T0LOADLO_REG &= 0;        // Solo revisan el contenido de LO y HI REG cuando sucede un UPDATE
    TIMG0_T0LOADHI_REG &= 0;
    TIMG0_T0LO_REG &= 0;
    TIMG0_T0HI_REG &= 0;
    TIMG0_RAW_CLR_REG |= 1UL;
    TIMG0_T0ALARMLO_REG &= 0;
    TIMG0_T0ALARMHI_REG &= 0;

    // Configurar prescalador
    TIMING0_T0CONFIG_REG |= (8000 << 13); // 10,000mhz cada segundo
    
    // Poner alarma
    TIMG0_T0ALARMLO_REG |= 1000*10;     // 1 segundo
    TIMG0_T0ALARMHI_REG &= 0; 
    
    // Activar timer
    TIMING0_T0CONFIG_REG |= (1UL << 10);
    TIMING0_T0CONFIG_REG |= (1UL << 31);


}


uint8_t my_timer0SecFlag ( void ){

    // Reiniciar el watchdog
    esp_task_wdt_init(999, false);

    //Revisa bandera
    if ( (TIMG0_RAW_REG &= 1) == 1 ){
        my_timer0Init();
        return 1;
    }
    return 0;
    
}



struct
{
    uint32_t seconds:8;
    uint32_t minutes:8;
    uint32_t hours:8;
}clockTime;


void clockInit(uint8_t hh, uint8_t mm, uint8_t ss)
{
    clockTime.hours = hh;
    clockTime.minutes = mm;
    clockTime.seconds = ss;
}


void clockUpdate(void)
{
    // TODO Fixme: Add logic to count hours, minutes and seconds
    clockTime.seconds++;

    if(clockTime.seconds >= 60){
        clockTime.seconds = 0;
        clockTime.minutes++;

        if(clockTime.minutes >= 60){
            clockTime.minutes = 0;
            clockTime.hours++;

            if(clockTime.hours >= 24){
                clockTime.hours = 0;
            }
        }
    }

}



void clockDisplay(void)
{
    uartClrScr(PC_UART_PORT);
    uartGotoxy(0,CLOCK_X_POS,CLOCK_Y_POS);

    char cad[16];
    sprintf(cad,"%02i : %02i : %02i", clockTime.hours, clockTime.minutes, clockTime.seconds); 
    uartPuts(0, cad);
}


    /*
    uartGotoxy(0,CLOCK_X_POS,CLOCK_Y_POS);
    myItoa(clockTime.hours,cad,10);
    uartPuts(0,cad);

    uartGotoxy(0,CLOCK_X_POS,CLOCK_Y_POS+2);
    uartPuts(0, ':');


    uartGotoxy(0,CLOCK_X_POS,CLOCK_Y_POS+3);
    myItoa(clockTime.minutes,cad,10);
    uartPuts(0,cad);

    uartGotoxy(0,CLOCK_X_POS,CLOCK_Y_POS+4);
    uartPuts(0,dospuntos);

    uartGotoxy(0,CLOCK_X_POS,CLOCK_Y_POS+5);
    myItoa(clockTime.seconds,cad,10);
    uartPuts(0,cad);
}
*/












int app_main()
{
    
    uartInit(PC_UART_PORT, 115200, 8, 1, 2, PC_UART_TX_PIN,PC_UART_RX_PIN);
    delayMs(500);
    uartClrScr(PC_UART_PORT);

    timer0Init();
    clockInit(70,70,70);

    while(1){ 

        if( timer0SecFlag() ) 
        { 
            clockUpdate();
            clockDisplay();
        }

        // Prevent watchdog fault
        delayMs(10);
    } 
    return 0; 
}
