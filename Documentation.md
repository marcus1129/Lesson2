What are ports?

Ports are the mega chips way of interfacing with other components, a port consist of 8 bits which can each either output a high or low voltage, or it can read either a high or low voltage, to do this each port requires 3 registers, a Data Direction Register(DDR), a PIN register(Inputs) and a PORT register(Output register), firstly we must declare in DDR if the port is an input or output, so if we want K0 to be an output, we would code it like so:

```
DDRK = 0b00000001;
PORTK = 0b00000001;
```

Now we have declared the direction of K0 to be output, and in PORTK we declared that it is high.

Since all other bits of K is defined as input, we can use those by changing PORTK:

```
PORTK = 0b00000101;
```

This means that K1 and K2 are inputs, but K2 is pulled high through a pullup resistor.

We can read the value of all ports and pins:

```
char temp = PINK;
```

This means temp would be equal to 0b00000101, since K0 is a high output, K2 is pulled high and not connected to anything, so it too must be high, and all other bits are not pulled high.



We have the following program

```c
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
```

