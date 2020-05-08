/*
 * SPI_SLAVE2_SERVO.c
 *
 * Created: 5/8/2020 7:00:44 AM
 * Author : User™
 */ 


#define  F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>


#define  SS		4
#define MOSI	5
#define MISO	6
#define servo_pin 7

#define first_position 250
#define chanel 0


void Servo_angle(char angle );
void ADC_Init();
unsigned int ADC_Read(unsigned channel);
void servo_init();

unsigned char SPI_Transfer(unsigned char sendByte){
	
	SPDR =sendByte;
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}
void spi_slave_init()
{
	DDRB|=(1<<MISO);
	DDRB&=~(1<<SS); //input pin
	SPCR|=(1<<SPE);

}



int main(void)
{
	void spi_slave_init();
	servo_init();
	DDRD|=(1<<servo_pin);
	while (1)
	{
		unsigned char recv=SPI_Transfer(0x55);
		_delay_ms(500);
		if(recv==0x05)
		{
			Servo_angle(0);
			_delay_ms(3000);		}
			Servo_angle(40);
			_delay_ms(3000);
			Servo_angle(100);
			_delay_ms(3000);
			Servo_angle(150);
			_delay_ms(3000);	
							
		
	}
}





void servo_init()
{
	TCCR1A=(1<<COM1A1)|(1<<COM1A0);																		//To set OC1A & OC1B
	TCCR1A|=(1<<COM1B1);																			    //TOo be a non inverting mode in FPWM
	TCCR1A|=(1<<WGM11);																					// FROM TCCR1A TO HAVE  THE TOP  ON ICRA1
	TCCR1B=(1<<WGM13)|(1<<WGM12);																		// FROM TCCR1B TO HAVE  THE TOP ON ICRA1
	TCCR1B|=(1<<CS10)|(1<<CS11);																		// CHOISE THE PRESCALER
	ICR1=4999;																							//CHOICE THE TOP TO HAVE THE NEEDED FREQUANCY ==50HZ
	DDRD|=(1<<PD4)|(1<<PD5);																			//THE DIRECTION OF OC1B &OC1A

}

void Servo_angle(char angle )
{
	if (angle==0)
	{
		OCR1A= first_position ;
	}
	else
	{
		OCR1A=(angle*(250/180))+first_position;
		
	}
}
