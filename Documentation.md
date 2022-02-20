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

Firstly to use a delay, we must define the clock speed of the Arduino, and include the delay library:

```c
#define F_CPU 16000000UL
#include <util/delay.h>
```

For the approach with no interrupt we need pin d4, which is in the G register, since we only need to use one of its pins, we can simplify the code and set all its pins to the same values, we do the same with our input from the dip-switch and the interrupt pin:

```c
void init(){
	DDRK = 0x00; //A8-15
	DDRG = 0x00; //D4
	DDRE = 0x00; //E4
	PORTE = 0xFF;
	PORTK = 0xFF;
	PORTG = 0xFF;
}
```

Now we need a gate, that will keep track of when the interrupt is triggered, a volatile int is declared globally so its value will remain stable as it is used by the interrupt service routine, which will simply set it to 1:

```c
volatile int gate = 0;

int main(void){
	/*...*/
}

ISR(INT4_vect){
	gate = 1;
}
```

INT4_vect is the interrupt associated with the D2 pin.

We use a function to store the values that we read from the dip-switches and automatically increment the index counter, the index counter keeps track of how many elements are stored, and so we make sure to increment its value in the same function that stores the input so we the value is always correct:

```c
void storeValue(char* values, uint8_t buffer, int* index){
	values[*index] = buffer;
	*index += 1;
}
```

For the main code, we start by initializing the display, the required pins and the interrupt and declaring the variables we need to store data:

```c
int main(void){
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
}
```

The code snippet:

```c
	EICRB = 0b00000010;
	EIMSK = 0b00010000;
	sei();
```

Declares that the interrupt pin is in the second(B) External Interrupt Control Register, and is describes by bit 0 and 1, with this value, it will react to falling edges, because our button is pulled high, so when it is pressed, we will see a falling edge.

The External Interrupt MaSK declares that we are interested in the value of bit 5, which is associated with the pin D2.

We then call sei() to enable global interrupts



For the no interrupt approach, we can simply read ping and use a bit mask to find out if the button is pressed, then we wait for it to be released at which point we store the value, display all values, check the index and wait for the next button press:

```c
int main(void)
{  
	while (1){
		/* ------ NO INTERRUPT SOLUTION ------ */
		if((PING & 0b00100000) == 0b00000000){
			while((PING & 0b00100000) == 0b00000000){}
             clear_display();
			storeValue(storedValues, PINK, &index);
			for(int i = 0; i < index; i++){
				sendCharXY(storedValues[i], 0, i);
			}
		}
		if(index == 3){
			index = 0;
		}
	}
}
```

For the interrupt approach, we read gate instead and wait for it to be equal to 1, then we wait for 100ms, to solve the problems of input bounce, which wasn't necessary in the other solution since the last it was slow enough the input bounce stopped before we executed our important code, then we store the input and display like in the last solution and lastly reset the value of gate:

```c
int main(void)
{  
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
```

