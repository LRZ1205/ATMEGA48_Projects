/*
Autor: Luis Romero Zepeda
Correo: luis.romero.ipn@gmail.com

Descripción:
Programa que utiliza el Timer 1 para generar señales cuadradas en el rango audible.
A traves de las entradas digitales de los PD5,PD6 y PD7 el usuario podra seleccionar un rango en especifico y con la ayuda de un potenciometro
ajustar a la frecuencia que mas le convenga, quedando de la siguiente forma.

PD5 - (20Hz - 200Hz)
PD6 - (200Hz- 2KHz)
PD7 - (2kHz - 20kHz)


*/
#define F_CPU 20000000UL //Se define que la frecuencia de trabajo es de 20 MHz,previamente configurado la utilizacion de un cristal en los fuses

#include <util/delay.h>
#include <avr/io.h>
#include <math.h>
#include <stdint.h>

//Declaracion de prototipos de funciones
void adc_init();
uint16_t adc_Read();

//Definicion de funciones
void adc_init(){
	ADMUX|=(1<<REFS0)|(1<<MUX0); //Se prende REFSO donde se especifica que se utiliza la tension del uC (5.0 V),MUX0 indica que se utiliza el canal 1 del uC
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //Se activa el ADC (es decir se habilita el modulo pero no inicia aun la conversion)y se utiliza el prescalador 128.
	DIDR0|=(1<<ADC1D); //Se desactiva la parte digital del canal 1.
	
}

uint16_t adc_Read(){
	
	ADCSRA|=(1<<ADSC); //Inicia la conversion
	uint16_t adc_total = 0;
	
	while(!(ADCSRA&(1<<ADIF))); //Espera a que la conversion termine
	
	adc_total = ADC;
	// Definimos los limites del valor del ADC para un rango de 100 - 1000
	if(adc_total < 100){
		adc_total= 100;
	}
	else if(adc_total > 1000){
		adc_total = 1000;
	}
	
	ADCSRA|=(1<<ADIF);
	

	return (adc_total);
}


int main(void)
{
	DDRB = 0xFF;	//Puerto B como salida
	DDRC = 0x00;	//Puerto C como entrada
	DDRD = 0X00;	//Puerto D como entrada
	PORTD =0xFF;
	PORTB= 0x00;	//Puerto D inicia con nivel de voltaje cero
	unsigned char z;
	int  adc_resul= 0;
	float frecuencia = 0;
	adc_init();		//Se activa el modulo ADC pero aun no hace la comversion
	TCCR1A|=(1<<6);
	TCCR1B|=(1<<WGM12);
	
	while(1)
	{
		adc_resul = adc_Read(); // Guarda la variable de retorno de la funcion
		_delay_ms(200);
		z=PIND;
		z= z & 0b11100000;
		switch(z){
			case(32):
			{
				OCR1A= 0;
				TCCR1B&=~(1<<CS11); //Apaga el bit CS11 si es que esta en alto
				TCCR1B|=(1<<CS10); //Modo CTC del Timer1 sin prescalador
				frecuencia = ((adc_resul/100.0)*2.0)*2.0;
				OCR1A = floor(20000/frecuencia);
				break;
			}
			case(64):
			{
				OCR1A=0;
				TCCR1B&=~(1<<CS10);//Apaga el bit CS10 si es que esta en alto
				TCCR1B|=(1<<CS11); //Modo CTC con prescalador 8
				frecuencia = ((adc_resul/100.0)*2.0)*2.0;//regla de tres donde el rango del ADC(100-1000) corresponde al rango de frecuencias (200Hz-2Khz)
				OCR1A = floor(25000/frecuencia);
				break;
			}
			case(128):
			{
				OCR1A=0;
				TCCR1B|=(1<<CS11)|(1<<CS10); //Modo CTC con prescalador 64
				frecuencia = ((adc_resul/100.0)*20.0)*2.0;//regla de tres donde el rango del ADC(100-1000) corresponde al rango de frecuencias (2Khz-20Khz)
				OCR1A = floor(312500/frecuencia);//Calcula la salida del Timer a partir de la frecuencia deseada (1/32us)
				break;
			}
		}
	}
}

