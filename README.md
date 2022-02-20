# Opgave 2

### The following document describes and documents the creative process involved in creating a program that can take an 8-bit input from a dip switch by pressing a button, then displaying the result to and LCS screen, as described by the assignment "Opgave 2".

### For documentation of the final program see "Documentation.md"

The program needs the following functionality

- Take a 1-bit input from a button

- Take an 8-bit input from a dip-switch when the button is pressed
- Write the 8-bit input as an ascii character to an LCD screen
- Clear the screen every fourth button press, so only 3 inputs can be displayed at a time



The program is really simple and so there aren't many ways to do it, but one major variable is how we react to the button being pressed, we can either read the pin like a normal input or we can use an external interrupt, for the sake of the assignment, we will implement both.

The advantage of implementing an external interrupt, is that the program handles the button press exactly when it happens, regardless of what code it is executing, this doesn't really matter in our program, but is essential for something like the breaks in a car.

The advantage of implementing out button as a normal pin and polling for an input, is simplicity, external interrupts are better in almost all cases, but there are a limited amount of pins that can handle an interrupt and so external interrupt should only be used where they are necessary.



We can describe these approaches with a flow chart for each, the approach with no interrupt looks like so:

![1645377253534](C:\Users\marcu\AppData\Roaming\Typora\typora-user-images\1645377253534.png)

For the approach with an external interrupt it would look like this.

![1645378690799](C:\Users\marcu\AppData\Roaming\Typora\typora-user-images\1645378690799.png)

Here the interrupt and main is running simultaneously.





The full program, process and all other files used for/by the program can be found in the following repo:

https://github.com/marcus1129/Lesson2.git