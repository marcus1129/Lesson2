# Opgave 2

### The following document describes and documents the creative process involved in creating a program that can take an 8-bit input from a dip switch by pressing a button, then displaying the result to and LCS screen, as described by the assignment "Opgave 2".

### For documentation of the final program see "Documentation.md"

The program needs the following functionality

- Take a 1-bit input from a button

- Take an 8-bit input from a dip-switch when the button is pressed
- Write the 8-bit input as an ascii character to an LCD screen
- Clear the screen every fourth button press, so only 3 inputs can be displayed at a time









Firstly all inputs(signals that need to be read) should be defined as active low by using the internal pullup resistor:

```c
DDRx = 0x00;
PORTx = 0xFF;
```

This sets all pins in port x, to be outputs that are pulled high.

This means we can use the switches to to grounds the pins when the switch is pressed, this way the voltage on the pin will be 0V when the button is pressed and 5V when the button isn't pressed.

When we have set this up, we might need to have a slight delay when reading the button, to eliminate any fluctuation in the signal when switching, to ensure we only react to an actual button press.

```c
while(1){
	if(PORTx & 0b00010000){
		delay(20ms);
		while(PORTx & 0b00010000){
			storeValue();
		}
	}
	if(getLength(storedValues) == 3){
        for(int i = 0; i < 3; i++){
			SendCharXY(storedValues[i], 0, i);
        }
	}
}
```

After testing it was discovered that no fluctuation in signals occur during button press, and signals only fall once per button press, since the delay from the nested logic is great enough as is, therefore no further preventative methods have been implemented.

Also clear_screen() isn't necessary as we write to the same position every time, which overwrites the previous writing.

For implementing an interrupt we will have to rework most of our structure, therefor we start over.

Firstly we must set up an interrupt, we need to use External Interrupt Control Register B, since that is where interrupt 4, that we will be using, is located, the control register use its 2 lowest order bits for interrupt 4:

Since we are interested in bit 4(Which is the fifth bit), we will use a mask of 0b0001000

```c
#include <avr/interrupt.h>

EICRB = 0b00000010;
EIMSK = 0b00010000;
sei();

int main(void){
	while(1){
 		if(gate == 1){
 			//do stuff
 		}
	}
}

ISR(INT4_vect){
	gate = 1;
}
```

The code we want to execute is very similar to before, but we will need to add a delay, since we can no longer rely on nesting to provide enough delay to deal with debounce. so we define the clock speed and add the following library.

```c
#define F_CPU 16000000UL
#include <util/delay.h>
```

Then we can add a delay in to our code:

```c
if(gate == 1){
	_delay_ms(100);
}
```

This is all we need for the setup and we can add the code we wrote previously

```c
if(gate == 1){
	_delay_ms(100);
	storeValue(storedValues, PINK, &index);
	gate = 0;
}
if(index == 3){
    for(int i = 0; i < 3; i++){
    	sendCharXY(storedValues[i], 0, i);
    }
    index = 0;
}
```

Note that we are not using the while statement as we would not be able to break out of it.

Finally we can clean the code up, simply by changing the from gathering 3 inputs then displaying, to displaying as it gets a new input.

```c
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
```

Which concludes the program.

The full program, process and all other files used for/by the program can be found in the following repo:

https://github.com/marcus1129/Lesson2.git