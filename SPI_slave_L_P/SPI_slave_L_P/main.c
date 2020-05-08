/*
 * SPI_slave_L_P.c
 *
 * Created: 5/8/2020 3:30:43 AM
 * Author : User™
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#define  SS		4
#define MOSI	5
#define MISO	6
#define LED0	0
#define LED1    1
#define LED2	2
#define DC_1_M1   0
#define DC_1_M2   1
#define DC_2_M1   6
#define DC_2_M2   7
#define DC_M1_EN  5
#define DC_M2_EN  4

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
void motors_init()
{

	//Direction OC1A is output
	DDRD|=(1<<DC_M1_EN);
	//Direction OC1B is output
	DDRD|=(1<<DC_M2_EN);
	//PWM_fAST_8BIT
	TCCR1A|=(1<<WGM12)|(1<<WGM20);
	//Inverted_Mode
	TCCR1A|=(1<<COM1A1)|(1<<COM1A0);
	//Pescalear = f_cpu=16 000 000 NO PRESCALER
	TCCR1B|=(1<<CS10);
}
void motor_1__speed(unsigned char speed)
{
	//clok wise direction
	PORTC=(1<<DC_1_M1);
	PORTC&=~(1<<DC_1_M2);
	// enable
	OCR1AL=(speed*255)/100;
	_delay_ms(100) ;
}
void motor_2__speed(unsigned char speed)
{
	//clok wise direction
	PORTC=(1<<DC_2_M1);
	PORTC&=~(1<<DC_2_M2);
	// enable
	OCR1AL=(speed*255)/100;
	_delay_ms(100) ;
}

int main(void)
{
	spi_slave_init();
	motors_init();
	unsigned char move =200;
	while (1)
	
	{
		unsigned char recv=SPI_Transfer(0x55);
		_delay_ms(1000);
		if(recv==0x01){
			PORTC^=(1<<LED0);
			_delay_ms(1000);
		}
		if(recv==0x02){
			PORTC^=(1<<LED1);
			_delay_ms(1000);
		}
		if(recv==0x03){
			PORTC^=(1<<LED2);
			_delay_ms(1000);
		}
		if(recv==0x04){
					motor_1__speed(move)	;		
		}
		if(recv==0x05){
			motor_2__speed(move)	;
		}
	}
}

