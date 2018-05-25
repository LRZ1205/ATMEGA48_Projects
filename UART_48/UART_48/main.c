/*
Autor: Luis Romero Zepeda 
Correo: luis.romero.ipn@gmail.com

Descrpción: Programa que manda desde el microcontrolador un mensaje a la PC
*/
#define F_CPU 20000000UL
#include <util/delay.h>
#include <avr/io.h>

void uart_Init();
unsigned char uart_Rx();
void uart_TxChar(char caracter);
void uart_SendingString(char *str);

void uart_Init(){
	UCSR0B|= (1<<RXEN0)|(1<<TXEN0); //Activa el Tranmisor y receptor del USART
	UCSR0C|= (1<<UCSZ01)|(1<<UCSZ00); //Modo asincrono, Paridad Par y un trama de datos de 8 bits
	
	UBRR0L = 129; //Valor calculado para Modo Normal Asincrono con 9600 Baudios y de acuerdo a la operacion de  20MHz /(16 * 9600)-1
	UBRR0H = 0;
}

unsigned char uart_Rx(){
	while((UCSR0A&(1<<RXC0)) ==0); // Mientras no haya un dato recibido no saldra de este bucle
	return (UDR0);
	
}

void uart_TxChar(char caracter){
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0 = caracter;
	
		
}

void uart_SendingString(char *str){
	unsigned char i=0;
	while(str[i]!= 0){
		uart_TxChar(str[i]);
		i++;
	}
}
	

int main(void)
{
	char c = 'C';
	uart_Init();
	
    while (1) 
    {
		//uart_SendingString("hola\n");
		//c = uart_Rx();
		uart_TxChar(c);
		
		_delay_ms(10);
    }
}

