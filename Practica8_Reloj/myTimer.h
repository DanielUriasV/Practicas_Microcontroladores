#ifndef _MY_TIMER0_H_
#define _MY_TIMER0_H_

/* Función para inicializar el Timer0 y generar */
/* la temporización de 1 Sec. */
void timer0Init ( void );
/* Función para verificar bandera del segundo */
uint8_t timer0SecFlag ( void );

#endif /* _MY_TIMER0_H_ */