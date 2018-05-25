#define MOSI 3
#define SCK 5
#define SS 2
#define MISO 4
#define SS_Enable PORTB&= ~(1<<SS)
#define SS_Disable PORTB|= (1<<SS)

//Declaracion de los prototipos
void SPI_init();
void SPI_write(char dato);

//Definicion de las funciones prototipo
void SPI_init(){
	DDRB|= (1<< MOSI)|(1<<SCK)|(1<<SS);
	DDRB&= ~(1<<MISO);
	PORTB|= (1<<SS);
	SPCR|=(1<<SPE) |(1<<MSTR)|(1<<SPR1);	//El modulo SPI esta activado, el primer dato transmitido es el LSB
											//Seleccion del microcontrolador en modo master y la frecuencia del reloj es 20E6 / 32
	SPSR|=(1<<SPI2X);
}

void SPI_write(char dato){
	char resp;
	SPDR =dato; //Copia el dato que se insertara en main a SPDR
	while(!(SPSR&(1<<SPIF))); //Espera a que se termine de transmitir
	resp = SPDR;
}
