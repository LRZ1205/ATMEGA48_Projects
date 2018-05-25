#define F_CPU 20000000UL //frecuencia de trabajo del microcontrolador definido por el cristal puesto
#include <avr/io.h>
#include <util/delay.h>
#include "SPI_master_48_config.h"


int main(void)
{
   SPI_init();
   SS_Enable;
    while (1) 
    {
		SPI_write('L');
		_delay_ms(50);
    }
}

