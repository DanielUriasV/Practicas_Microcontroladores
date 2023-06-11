#include <driver/timer.h>
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdio.h>
#include "myTimer.h"
#include <inttypes.h>
#include "myUart.h"
#include "main.c"

//-----------------------------------------------------Variables---------------------------------------------------//


typedef enum
{
    silenceStart = 0, // activar silencio freq
    silenceWait,    // espera milis para acabar silencio
    noteStart,         // activar nota freq
    noteWait         // espera milis para acabar nota
} stateSong;

stateSong state = silenceStart;



//-------------------------------------------timer0Init--------------------------------------------/

void timer0Init(void)
{

    timer_config_t config = {
        .divider = 2,
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &config);

    // Reiniciar el watchdog
    esp_task_wdt_init(999, false);
    
    // Limpiar registros
    TIMG0_T0CONFIG_REG = 0;
    TIMG0_T0LO_REG &= 0;
    TIMG0_T0HI_REG &= 0;
    TIMG0_RAW_CLR_REG |= 1;
    TIMGn_Tx_INT_ENA_REG = 1;
    TIMG0_T0ALARMLO_REG &= 0;
    TIMG0_T0ALARMHI_REG &= 0;

    // Configurar prescalador
    TIMG0_T0CONFIG_REG |= (80 << 13); // 10,000mhz cada segundo
    
    // Poner alarma
    TIMG0_T0ALARMLO_REG |= 1000;  //1*10 milis con 8000 , 1 micros con 80
    TIMG0_T0ALARMHI_REG &= 0; 
    
    // Activar timer
    TIMG0_T0CONFIG_REG |= (1UL << 10);  
    TIMG0_T0CONFIG_REG |= (1UL << 11);  
    TIMG0_T0CONFIG_REG |= (1UL << 29);
    TIMG0_T0CONFIG_REG |= (1UL << 30);
    TIMG0_T0CONFIG_REG |= (1UL << 31);
    
    // Register an ISR handler 
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer0Isr, NULL, 0, NULL);
}



//------------------------------------------timer1FrequGen----------------------------------------//

void IRAM_ATTR timer1FreqGen(uint16_t freq)
{

// Use only Low level register to configure Timer 1

    int alarma = 0;
    alarma = 10000000/(freq*2);

    timer_config_t config = {
        .divider = 10000,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_START,
        .alarm_en = TIMER_ALARM_EN,
        .intr_type = TIMER_INTR_LEVEL,
        .auto_reload = 1,
    };

    timer_init(TIMER_GROUP_0, TIMER_1, &config);

    // Limpiar registros
    TIMG0_T1CONFIG_REG = 0;
    TIMG0_T1LO_REG &= 0;
    TIMG0_T1HI_REG &= 0;
    TIMG0_RAW_CLR_REG |= 2;
    TIMG0_T1ALARMLO_REG &= 0;
    TIMG0_T1ALARMHI_REG &= 0;


    // Configurar prescalador
    TIMG0_T1CONFIG_REG |= (80UL << 13); // 10,000mhz cada segundo

    //Colocar alarma
    TIMG0_T1ALARMLO_REG = alarma;
    TIMG0_T1ALARMHI_REG = 0;

    // Activar timer
    TIMG0_T1CONFIG_REG |= (1UL << 10);  
    TIMG0_T1CONFIG_REG |= (1UL << 11);  
    TIMG0_T1CONFIG_REG |= (1UL << 29);
    TIMG0_T1CONFIG_REG |= (1UL << 30);
    TIMG0_T1CONFIG_REG |= (1UL << 31);

}



//----------------------------------------timer1Play-----------------------------------------//

void timer1Play(struct note song[], uint16_t len)
{

    // Save song pointer and restart playback state machine
    flagPlay = 1;
    idx_song = cancionActual;
    idx_note = 0;

    if(state == silenceStart)
    {
        timer1FreqGen((song + idx_note)->freq);
        timer_isr_register(TIMER_GROUP_0, TIMER_1, timer1Isr, NULL, 0, NULL);
    }

}

//---------------------------------------time0Isr----------------------------------------------//


void IRAM_ATTR timer0Isr(void *ptr)
{


    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);

    _millis++;

    //Maquina de estados 

    uint32_t getfreq = 0;

    if (flagPlay == 1) // si play == 1 reproducir notas
    {
        if (state == silenceStart) // or
        {
            _millis = 0;      // tAct = _millis
            state = silenceWait; // sig estado
            timer1FreqGen(2); // frecuencia no audible
            timer_isr_register(TIMER_GROUP_0, TIMER_1, timer1Isr, NULL, 0, NULL);
        }


        else if (state == silenceWait)
        {
            if (_millis >= 10)
            {
                state = noteStart; // cambiamos para nota si ya pasaron los 10ms
            }
        }


        else if (state == noteStart)
        {
            _millis = 0;
            
            if (idx_note < arrayLengthSongs[idx_song])
            {
                getfreq = ((arraySongs[idx_song][0] + idx_note)->freq) ;  //Obtenemos la frecuencia.
                
                if (getfreq == 0) getfreq = 20;     //Esto impide que la frecuencia sea 0.

                state = noteWait; // estado de espera de duracion nota

                timer1FreqGen(getfreq);
                timer_isr_register(TIMER_GROUP_0, TIMER_1, timer1Isr, NULL, 0, NULL);
            }
            
            else
            {
                // Terminar y limpiar variables.
                flagPlay = 0;
                _millis = 0;
                idx_note = 0;
                state = silenceStart;
           
            }
        }
        else if (state == noteWait)
        {
            if (_millis >= ((arraySongs[idx_song][0] + idx_note)->delay))
            {
                idx_note++;
                _millis = 0;
                state = silenceStart;
            }
        }
    }
    
}



//------------------------------------------timer1Isr------------------------------------------//

void IRAM_ATTR timer1Isr(void *ptr)
{

    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_1);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_1);


    // Toggle GPIO with myGpio functions
    if (nivel == 0)
    {
        gpio_set_level(Bocina, 1);
        nivel = 1;
    }
    else
    {
        gpio_set_level(Bocina, 0);
        nivel = 0;
    }

    // Register an ISR handler 
    timer_isr_register(TIMER_GROUP_0, TIMER_1, timer1Isr, NULL, 0, NULL);

}


/*
uint8_t timer1SecFlag ( void )
{
    _millis++;
    return 0;
}
*/