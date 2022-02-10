# Lesson2

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
		clear_screen();
		SendCharXY(storedValues[0], 0, 0);
		SendCharXY(storedValues[1], 0, 1);
		SendCharXY(storedValues[2], 0, 2);
	}
}
```

