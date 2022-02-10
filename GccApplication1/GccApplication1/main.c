/*
 * opg1_ny_E20.c
 *
 * Created: 01-09-2020 14:34:13
 * Author : osch
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver

#define F_CPU 16000000UL
#include <util/delay.h>

void init(){
	DDRK = 0x00; //A8-15
	DDRG = 0x00; //D4
	PORTK = 0xFF;
	PORTG = 0xFF;
}

void storeValue(char* values, uint8_t buffer, int* index){
	values[*index] = buffer;
	*index += 1;
}

int main(void)
{  
	init();
	_i2c_address = 0X78;
  
	I2C_Init();
	InitializeDisplay();
	print_fonts();

	char storedValues[3];
    int index = 0;
	clear_display(); 
	
	while (1){
		if((PING & 0b00100000) == 0b00000000){
			while((PING & 0b00100000) == 0b00000000){}
			storeValue(storedValues, PINK, &index);
		}
		if(index == 3){
			for(int i = 0; i < 3; i++){
				sendCharXY(storedValues[i], 0, i);
			}
			index = 0;
		}
	}
}

