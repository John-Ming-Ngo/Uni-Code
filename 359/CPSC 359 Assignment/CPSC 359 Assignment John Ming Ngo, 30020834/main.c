/**
Author: John Ming Ngo, 30020834
This file and handler.c was written by the author, everything else is by Leonard Manzala.
*/

//Defining names:
#define LED1 17
#define LED2 27
#define LED3 22
#define SWITCH_A 23
#define SWITCH_B 24

//Defining how much time between each light.
#define TIME1 0x03FFFFF
#define TIME2 0x03FFFFF/2

// Include Files
#include "uart.h"
#include "sysreg.h"
#include "gpio.h"
#include "irq.h"

// Function prototypes
void init_output_pin(int pin);
void set_pin(int pin);
void clear_pin(int pin);
void init_input_pin_rising_edge(int pin);
void init_input_pin_falling_edge(int pin);
void blink123();
void blink321();

/**
This main function sets three LEDs to turn on/off in a pattern, subject to two button inputs
that can switch it to two different patterns. See the Assignment 3 paper for more details.
Parameters: Nothing
Returns: Nothing
*/
// Initialize LIGHTSTATE; this variable allows for the program to check for state changes caused by GPIO interrupts.
unsigned int LIGHTSTATE;

void main() {
	// Set up the UART serial port. May be used for debugging purposes.
  uart_init();

	// Initialize GPIO Output Pins
	init_output_pin(LED1);
	init_output_pin(LED2);
	init_output_pin(LED3);

	// Initialize GPIO Input Pins
	init_input_pin_falling_edge(SWITCH_A);
	init_input_pin_rising_edge(SWITCH_B);

	 //Initialize GPIO Interrupts
	enableIRQ();

	//Main loop:
  LIGHTSTATE = 0;
	while (1) {
		if (LIGHTSTATE == 0) {//LIGHTSTATE 1; order is LED1, 2, 3, for about half-second each.
      blink123();
		}
		else {//LIGHTSTATE 2; order is LED3, 2, 1, for about quarter-second each.
      blink321();
		}
	}
}

// Functions:

/**
This function controls the GPIO output for the lights for state 1;
and also checks after each change for the state changing, breaking out if the state changes.
Parameters: None
Returns: None
*/
void blink123(){
  // Initialize Counter variable:
	unsigned int r = 0;

  //Set LED1 to be on for the designated amount of time.
  if (LIGHTSTATE != 0){
    return;
  }
  clear_pin(LED1);
  clear_pin(LED2);
  clear_pin(LED3);
  set_pin(LED1);
  r = TIME1;
  while (r--) {
    asm volatile("nop");
  }

  //Set LED2 to be on for the designated amount of time.
  if (LIGHTSTATE != 0){
    return;
  }
  clear_pin(LED1);
  clear_pin(LED2);
  clear_pin(LED3);
  set_pin(LED2);
  r = TIME1;
  while (r--) {
    asm volatile("nop");
  }

  //Set LED3 to be on for the designated amount of time.
  if (LIGHTSTATE != 0){
    return;
  }
  clear_pin(LED1);
  clear_pin(LED2);
  clear_pin(LED3);
  set_pin(LED3);
  r = TIME1;
  while (r--) {
    asm volatile("nop");
  }
  return;
}

/**
This function controls the GPIO output for the lights for state 2;
and also checks after each change for the state changing, breaking out if the state changes.
Parameters: None
Returns: None
*/
void blink321(){
  // Initialize Counter variable:
	unsigned int r = 0;

  //Set LED3 to be on for designated amount of time.
  if (LIGHTSTATE != 1){
    return;
  }
  clear_pin(LED1);
  clear_pin(LED2);
  clear_pin(LED3);
  set_pin(LED3);
  r = TIME2;
  while (r--) {
    asm volatile("nop");
  }

  //Set LED2 to be on for designated amount of time.
  if (LIGHTSTATE != 1){
    return;
  }
  clear_pin(LED1);
  clear_pin(LED2);
  clear_pin(LED3);
  set_pin(LED2);
  r = TIME2;
  while (r--) {
    asm volatile("nop");
  }

  //Set LED1 to be on for designated amount of time.
  if (LIGHTSTATE != 1){
    return;
  }
  clear_pin(LED1);
  clear_pin(LED2);
  clear_pin(LED3);
  set_pin(LED1);
  r = TIME2;
  while (r--) {
    asm volatile("nop");
  }
  return;
}

/**
This function initalizes a pin to be an output pin.
Parameters: pin - Integer input selecting the pin we want to deal with.
Returns: Nothing
*/
void init_output_pin(int pin) {
	unsigned int pinRange; //Used to determine the register and the pin in register to activate.
	register unsigned int r = 0; //Used to store said register.
	pinRange = pin/10; //Used to identify which GPFSEL register to select.
	switch (pinRange) {
		case 0:
			r = *GPFSEL0;
			break;
		case 1:
			r = *GPFSEL1;
			break;
		case 2:
			r = *GPFSEL2;
			break;
		case 3:
			r = *GPFSEL3;
			break;
		case 4:
			r = *GPFSEL4;
			break;
		case 5:
			r = *GPFSEL5;
			break;
	}
    pinRange = pin % 10; //Used to identify the pin to select.
    r &= ~(0x7 << (pinRange*3));
    r |= (0x1 << (pinRange*3));

	pinRange = pin/10; //Used to identify which GPFSEL register to select.
	switch (pinRange) {
		case 0:
			*GPFSEL0 = r;
			break;
		case 1:
			*GPFSEL1 = r;
			break;
		case 2:
			*GPFSEL2 = r;
			break;
		case 3:
			*GPFSEL3 = r;
			break;
		case 4:
			*GPFSEL4  = r;
			break;
		case 5:
			*GPFSEL5 = r;
			break;
	}

	// Disable pullup/pulldown
    *GPPUD = 0x0;
    r = 150;
    while (r--) {
      asm volatile("nop");
    }
    *GPPUDCLK0 = (0x1 << pin);
    r = 150;
    while (r--) {
      asm volatile("nop");
    }
    *GPPUDCLK0 = 0;
}

/**
This function sets an output pin. Note that the pin should be initialized before this.
Parameters: pin - Integer input selecting the pin we want to deal with.
Returns: Nothing
*/
void set_pin(int pin){
	register unsigned int r;
	r = (0x1 << pin);
	*GPSET0 = r;
}

/**
This function clears an output pin. Note that the pin should be initialized before this.
Parameters: pin - Integer input selecting the pin we want to deal with.
Returns: Nothing
*/
void clear_pin(int pin){
	register unsigned int r;
	r = (0x1 << pin);
	*GPCLR0 = r;
}

/**
This function initalizes a pin to be an input (rising edge) pin.
Parameters: pin - Integer input selecting the pin we want to deal with.
Returns: Nothing
*/
void init_input_pin_rising_edge(int pin) {
	unsigned int pinRange; //Used to determine the register and the pin in register to activate.
	register unsigned int r = 0; //Used to store said register.
	pinRange = pin/10; //Used to identify which GPFSEL register to select.
	switch (pinRange) {
		case 0:
			r = *GPFSEL0;
			break;
		case 1:
			r = *GPFSEL1;
			break;
		case 2:
			r = *GPFSEL2;
			break;
		case 3:
			r = *GPFSEL3;
			break;
		case 4:
			r = *GPFSEL4;
			break;
		case 5:
			r = *GPFSEL5;
			break;
	}
	pinRange = pin % 10; //Used to identify the pin to select.
    r &= ~(0x7 << (pinRange*3));

    pinRange = pin/10; //Used to identify which GPFSEL register to select.
	switch (pinRange) {
		case 0:
			*GPFSEL0 = r;
			break;
		case 1:
			*GPFSEL1 = r;
			break;
		case 2:
			*GPFSEL2 = r;
			break;
		case 3:
			*GPFSEL3 = r;
			break;
		case 4:
			*GPFSEL4  = r;
			break;
		case 5:
			*GPFSEL5 = r;
			break;
	}

    //Disable internal pullup/pulldown.
    *GPPUD = 0x0;
    r = 150;
    while (r--) {
        asm volatile("nop");
    }
    *GPPUDCLK0 = (0x1 << pin);
    r = 150;
    while (r--) {
        asm volatile("nop");
    }
    *GPPUDCLK0 = 0;

	//Turn on rising edge triggering.
    *GPREN0 = (0x1 << pin);
    *IRQ_ENABLE_IRQS_2 = (0x1 << 20);
}
/**
This function initalizes a pin to be an input (rising edge) pin.
Parameters: pin - Integer input selecting the pin we want to deal with.
Returns: Nothing
*/
void init_input_pin_falling_edge(int pin) {
	unsigned int pinRange; //Used to determine the register and the pin in register to activate.
	register unsigned int r = 0; //Used to store said register.
	pinRange = pin/10; //Used to identify which GPFSEL register to select.
	switch (pinRange) {
		case 0:
			r = *GPFSEL0;
			break;
		case 1:
			r = *GPFSEL1;
			break;
		case 2:
			r = *GPFSEL2;
			break;
		case 3:
			r = *GPFSEL3;
			break;
		case 4:
			r = *GPFSEL4;
			break;
		case 5:
			r = *GPFSEL5;
			break;
	}
	pinRange = pin % 10; //Used to identify the pin to select.
    r &= ~(0x7 << (pinRange*3));

    pinRange = pin/10; //Used to identify which GPFSEL register to select.
	switch (pinRange) {
		case 0:
			*GPFSEL0 = r;
			break;
		case 1:
			*GPFSEL1 = r;
			break;
		case 2:
			*GPFSEL2 = r;
			break;
		case 3:
			*GPFSEL3 = r;
			break;
		case 4:
			*GPFSEL4  = r;
			break;
		case 5:
			*GPFSEL5 = r;
			break;
	}
	//Disable internal pullup/pulldown
    *GPPUD = 0x0;
    r = 150;
    while (r--) {
        asm volatile("nop");
    }
    *GPPUDCLK0 = (0x1 << pin);
    r = 150;
    while (r--) {
        asm volatile("nop");
    }
    *GPPUDCLK0 = 0;

	//Turn on falling edge triggering.
    *GPFEN0 = (0x1 << pin);
    *IRQ_ENABLE_IRQS_2 = (0x1 << 20);
}
