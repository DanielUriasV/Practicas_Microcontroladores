#ifndef MY_UART_H
#define MY_UART_H

#include "driver/uart.h"



// UART 0
#define PC_UART_PORT    (0)
#define PC_UART_RX_PIN  (3)
#define PC_UART_TX_PIN  (1)

// UART 1
#define UART1_PORT      (1)
#define UART1_RX_PIN    (18)
#define UART1_TX_PIN    (13)

// UART 2
#define UART2_PORT      (2)
#define UART2_RX_PIN    (16)
#define UART2_TX_PIN    (17)

// Datos de inicializacion
#define UARTS_BAUD_RATE         (115200)
#define TASK_STACK_SIZE         (1048)
#define READ_BUF_SIZE           (1024)


//---------------------------------------------FUNCION AGREGADA EN PRACTICA 6--------------------------------//

#define _DEF_REG_32b( addr ) ( *(volatile uint32_t *)( addr ) )
//Registros GPIO
#define UART_CONFIG0_REG          _DEF_REG_32b    ( 0x3FF40020 )  // Config UART 0
#define UART_CONFIG1_REG          _DEF_REG_32b    ( 0x3FF50020 )  // Config UART 1
#define UART_CONFIG2_REG          _DEF_REG_32b    ( 0x3FF6E020 )  // Config UART 2
#define UART0_CLKDIV_REG          _DEF_REG_32b    ( 0x3FF40014 )  // Divisor UART 0
#define UART1_CLKDIV_REG          _DEF_REG_32b    ( 0x3FF50014)   // Divisor UART 1
#define UART2_CLKDIV_REG          _DEF_REG_32b    ( 0x3FF6E014 )  // Divisor UART 2









// UART Data
#define UART0_FIFO_REG      0x60000000
#define UART1_FIFO_REG      0x60010000
#define UART2_FIFO_REG      0x6002E000
#define UART_FIFO_REG(n)    (volatile uint32_t *)((n == 0)? UART0_FIFO_REG : (n == 1)? UART1_FIFO_REG : UART2_FIFO_REG)

// UART Status
#define UART0_STATUS_REG    0x3FF4001C
#define UART1_STATUS_REG    0x3FF5001C
#define UART2_STATUS_REG    0x3FF6E01C
#define UART_STATUS_REG(n)  (volatile uint32_t *)((n == 0)? UART0_STATUS_REG : (n == 1)? UART1_STATUS_REG : UART2_STATUS_REG)

#define FIFO_NUM_BITS       8
#define FIFO_SIZE           (128)
#define UART_RXFIFO_CNT_POS 0
#define UART_TXFIFO_CNT_POS 16
#define UART_FIFO_CNT_MSK   ((1 << FIFO_NUM_BITS) - 1)

#define UART_TXFIFO_CNT(st) (st >> UART_TXFIFO_CNT_POS & UART_FIFO_CNT_MSK)
#define UART_RXFIFO_CNT(st) (st >> UART_RXFIFO_CNT_POS & UART_FIFO_CNT_MSK)



void uartInit(uart_port_t uart_num, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop, uint8_t txPin, uint8_t rxPin);

// Send
void uartPuts(uart_port_t uart_num, char *str);
void uartPutchar(uart_port_t uart_num, char data);

// Receive
bool uartKbhit(uart_port_t uart_num);
char uartGetchar(uart_port_t uart_num );
void uartGets(uart_port_t uart_num, char *str);

// Escape sequences
void uartClrScr( uart_port_t uart_num );
void uartSetColor(uart_port_t uart_num, uint8_t color);
void uartGotoxy(uart_port_t uart_num, uint8_t x, uint8_t y);

#define YELLOW  33 // Fixme 
#define GREEN   32 // Fixme 
#define BLUE    34 // Fixme 

// Utils
void myItoa(uint16_t number, char* str, uint8_t base) ;
uint16_t myAtoi(char *str);
void delayMs(uint16_t ms);

#endif