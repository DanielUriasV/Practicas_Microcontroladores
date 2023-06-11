#include <driver/timer.h>
#include "esp_task_wdt.h"
#include "myTimer.h"
 



static volatile uint8_t SecFlag; 





#define ALARM_VAL_US    10000 // TODO Fixme for 1 sec
#define TIMER_DIVIDER   8000 // TODO Fixme for 1 sec




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


#define TIMGn_Tx_INT_ENA_REG    _DEF_REG_32b    ( 0x3FF5F098 )  // CLEAR FLAG INT


/* Timer interrupt service routine */
static void IRAM_ATTR timer0Isr(void *ptr)
{
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);
    SecFlag = 1;
}






/* Timer group0 TIMER_0 initialization */
void timer0Init(void)
{
    esp_err_t ret;
    timer_config_t config = {
        .divider = TIMER_DIVIDER,
    };
    ret = timer_init(TIMER_GROUP_0, TIMER_0, &config);

    // Reiniciar el watchdog
    esp_task_wdt_init(999, false);
    
    // Limpiar registros
    TIMG0_T0LOADLO_REG &= 0;        // Solo revisan el contenido de LO y HI REG cuando sucede un UPDATE
    TIMG0_T0LOADHI_REG &= 0;
    TIMG0_T0LO_REG &= 0;
    TIMG0_T0HI_REG &= 0;
    TIMG0_RAW_CLR_REG = 1;
    TIMG0_T0ALARMLO_REG &= 0;
    TIMG0_T0ALARMHI_REG &= 0;

    // Configurar prescalador
    TIMING0_T0CONFIG_REG |= (8000 << 13); // 10,000mhz cada segundo
    
    // Poner alarma
    TIMG0_T0ALARMLO_REG |= 1*10; 
    TIMG0_T0ALARMHI_REG &= 0; 
    
    // Activar timer
    TIMING0_T0CONFIG_REG |= (1UL << 10);  
    TIMING0_T0CONFIG_REG |= (1UL << 29);
    TIMING0_T0CONFIG_REG |= (1UL << 30);
    TIMING0_T0CONFIG_REG |= (1UL << 31);

    /* Register an ISR handler */
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer0Isr, NULL, 0, NULL);
}





uint8_t timer0SecFlag ( void )
{
   if( SecFlag )
   {
        SecFlag=0;
        return 1;
    }
    else
    { 
        return 0;
   }
}