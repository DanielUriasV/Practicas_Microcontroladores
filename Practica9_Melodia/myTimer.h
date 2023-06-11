#ifndef _MY_TIMER0_H_
#define _MY_TIMER0_H_

#include <driver/timer.h>

#define c_note      261
#define d_note      294
#define e_note      329
#define f_note      349
#define g_note      391
#define gS_note     415
#define a_note      440
#define aS_note     466
#define b_note      494
#define aa_note     493
#define cH_note     523
#define cSH_note    554
#define dH_note     587
#define dSH_note    622
#define eH_note     659
#define fH_note     698
#define fSH_note    740
#define gH_note     784
#define gSH_note    830
#define aH_note     880
#define hH_note     987

#define bL_note     247
#define fS_note     370
#define cS_note     277
#define aL_note     220

#define gL_note     196
#define dS_note     311
#define gSL_note    208
#define cL_note     131
#define eL_note     165
#define aSL_note    233

#define TEMPO	    500
#define TEMPO_500   500
#define TEMPO_250   250
#define TEMPO_600   600

// More notes can be found at:
// http://www.intmath.com/trigonometric-graphs/music.php




#define SILENCE 10

#define _DEF_REG_32b( addr ) ( *(volatile uint32_t *)( addr ) )
//Registers timer 0
#define TIMG0_T0CONFIG_REG      _DEF_REG_32b    ( 0x3FF5F000 )  // Config
#define TIMG0_T0LO_REG          _DEF_REG_32b    ( 0x3FF5F004 )  // Timer_lo
#define TIMG0_T0HI_REG          _DEF_REG_32b    ( 0x3FF5F008 )  // Timer_lo
#define TIMG0_T0ALARMLO_REG     _DEF_REG_32b    ( 0x3FF5F010 )  // Alarm_lo
#define TIMG0_T0ALARMHI_REG     _DEF_REG_32b    ( 0x3FF5F014 )  // Alarm_hi
#define TIMG0_T0LOADLO_REG      _DEF_REG_32b    ( 0x3FF5F018 )  // Load_lo
#define TIMG0_T0LOADHI_REG      _DEF_REG_32b    ( 0x3FF5F01C )  // Load_lo
#define TIMG0_T0UPDATE_REG      _DEF_REG_32b    ( 0x3FF5F00C )  // Update

//Registers timer 1
#define TIMG0_T1CONFIG_REG      _DEF_REG_32b    ( 0x3FF5F024 )  // Config
#define TIMG0_T1LO_REG          _DEF_REG_32b    ( 0x3FF5F028 )  // Timer_lo
#define TIMG0_T1HI_REG          _DEF_REG_32b    ( 0x3FF5F02C )  // Timer_lo
#define TIMG0_T1ALARMLO_REG     _DEF_REG_32b    ( 0x3FF5F034 )  // Alarm_lo
#define TIMG0_T1ALARMHI_REG     _DEF_REG_32b    ( 0x3FF5F038 )  // Alarm_hi
#define TIMG0_T1UPDATE_REG      _DEF_REG_32b    ( 0x3FF5F030 )  // Update

//Flags
#define TIMG0_RAW_REG           _DEF_REG_32b    ( 0x3FF5F09C )  // READ FLAG
#define TIMG0_RAW_CLR_REG       _DEF_REG_32b    ( 0x3FF5F0A4 )  // CLEAR FLAG


#define TIMGn_Tx_INT_ENA_REG    _DEF_REG_32b    ( 0x3FF5F098 )  // Clear Flags


//Estructuras y variables
struct note
{
    uint16_t freq;
    uint16_t delay;
};

static volatile uint32_t _millis = 0; 
uint8_t idx_song = 0;
uint8_t idx_note = 0;
uint8_t nivel = 0;
uint8_t flagPlay = 0;
uint8_t silence = 0;
uint32_t index_note = 0;
uint8_t iniciado = 0;



//Pruebas
static volatile uint32_t delay = 0; 





void timer0Init( void );
void timer1FreqGen(uint16_t freq);
void timer1Play(struct note song[], uint16_t len);
void IRAM_ATTR timer0Isr(void *ptr);
void IRAM_ATTR timer1Isr(void *ptr);
//uint8_t timer1SecFlag ( void );


#endif /* _MY_TIMER0_H_ */