/*
 * UART_SPI_Master_L_P.c
 *
 * Created: 5/8/2020 4:13:36 AM
 * Author : User™
 */ 
#define F_CPU	16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define MOSI	5
#define MISO	6
#define SCK		7
#define CS		0
#define CS2     1 

#define SS_ENABLE()		PORTC&=~(1<<CS)
#define SS_DISABLE()	PORTC|=(1<<CS)

#define SS2_ENABLE()		PORTC&=~(1<<CS2)
#define SS2_DISABLE()	PORTC|=(1<<CS2)


#define BAUD   9600
#define BAUD_EQ		((F_CPU/(BAUD*16UL))-1)

void SPI_Init();
char uartReceive();
void uartInit();
void uartSend(char data);
unsigned char SPI_Transfer(unsigned char sendByte);

int main(void)
{
	
	uartInit();
	while (1)
	{
		uart_sendString("ABDO_LAST_Pro\r\n");
		_delay_ms(1000);
		
			 SS_ENABLE();
			 unsigned char recvByte=SPI_Transfer(0x01);
			 SS_DISABLE();																			//Momken 23mel enable w desable mra w7da 
			 			 _delay_ms(1000);
			SS_ENABLE();
			 recvByte=SPI_Transfer(0x02);
			SS_DISABLE();
			_delay_ms(1000);
			SS_ENABLE();
			 recvByte=SPI_Transfer(0x03);
			SS_DISABLE();
			_delay_ms(1000);
			SS_ENABLE();
			 recvByte=SPI_Transfer(0x04);
			SS_DISABLE();
			_delay_ms(1000);
			SS_ENABLE();
			 recvByte=SPI_Transfer(0x05);
			SS_DISABLE();
			_delay_ms(1000);
			SS2_ENABLE();
			recvByte=SPI_Transfer(0x05);
			SS2_DISABLE();
			_delay_ms(1000);
		
	}
}

void uartInit(){
	//Select Baud Rate
	UBRRL=BAUD_EQ;
	//Enable RX and TX
	UCSRB =(1<<RXEN)|(1<<TXEN);
	//select frame size and number of stop bits enble uart
	UCSRC=(1<<URSEL)|(1<<UCSZ0) | (1<<UCSZ1);
}

void uartSend(char data){
	
	while( !(UCSRA & (1<<UDRE)));										//When UDRE==1 an send data to  UDR 
	UDR=data;
}

char uartReceive(){
	while( !(UCSRA & (1<<RXC)));
	return UDR;
}
void uart_sendString(char *str){
	char i=0;
	while(str[i]!='\0'){
		uartSend(str[i]);
		i++;
	}
}
void SPI_Init(){
	//data direction for SPI bus Pins
	DDRB|=(1<<MOSI)|(1<<SCK);
	DDRC|=(1<<CS)|(1<<CS2);
	//MODE0-CPOL=0 CPHA=0 , SPE=1 , MSTR=1 DORD=MSB(0) , SPEED=FOSC/16=1Mhz
	SPCR|=(1<<MSTR)|(1<<SPE)|(1<<SPR0);
}
unsigned char SPI_Transfer(unsigned char sendByte){
	
	SPDR=sendByte;
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}
