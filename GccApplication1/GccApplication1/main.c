/*
 * opg1_ny_E20.c
 *
 * Created: 01-09-2020 14:34:13
 * Author : Marcus Korre
 * Github : https://github.com/marcus1129/Lesson2.git
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver

#define F_CPU 16000000UL
#include <util/delay.h>

void init(){
	DDRK = 0x00; //A8-15
	DDRG = 0x00; //D4
	DDRE = 0x00; //E4
	PORTE = 0xFF;
	PORTK = 0xFF;
	PORTG = 0xFF;
}

void storeValue(char* values, uint8_t buffer, int* index){
	values[*index] = buffer;
	*index += 1;
}

volatile int gate = 0;

int main(void)
{  
	_i2c_address = 0X78;
	
	init();
	I2C_Init();
	InitializeDisplay();
	print_fonts();
	clear_display(); 
	
	EICRB = 0b00000010;
	EIMSK = 0b00010000;
	sei();
	
	char storedValues[3];
	int index = 0;
	
	while (1){
		/* ------ NO INTERRUPT SOLUTION ------ */
		/*if((PING & 0b00100000) == 0b00000000){
			while((PING & 0b00100000) == 0b00000000){}
			clear_display();
			storeValue(storedValues, PINK, &index);
			for(int i = 0; i < index; i++){
				sendCharXY(storedValues[i], 0, i);
			}
		}
		if(index == 3){
			index = 0;
		}*/
		
		/* ------ INTERRUPT SOLUTION ------ */
		if(gate == 1){
			_delay_ms(100);
			clear_display();
			storeValue(storedValues, PINK, &index);
			for(int i = 0; i < index; i++){
				sendCharXY(storedValues[i], 0, i);
			}
			if(index == 3){
				index = 0;
			}
			gate = 0;
		}
	}
}
ISR(INT4_vect){
	gate = 1;
}