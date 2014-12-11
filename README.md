Lab 8
====
#### Clayton Jaksha | ECE 382 | Dr. York | M2A
## Objective and Purpose
### Objective

The objective of this lab is to use the robot's IR sensors to navigate it through a simple maze.

### Purpose

To incorporate all course concepts into a single lab and enjoy some practical computer engineering!


## Preliminary Design
My robot's maze-solving algorith is a very simple state machine. It essentially moves forward until it sees a wall on the center sensor. Then, it tank turns left until the wall it no longer visible to the front sensor. Then, it slow turns to the right (back into the wall) until it detects the wall again. To give the robot some hysteresis, we set two thresholds. One for leaving the wall, one for coming into the wall.
![alt text](http://i.imgur.com/3YQ8lxV.jpg "Robert's brainzzzzz")
This is how the hardware will be implemented. In short, analog signals come from the sensor, are turned into digital signal by the MSP430, processed by the MSP430, turned into PWM signals by the MSP430, then the signals drive the motors through the motor driver chip.
![alt text](http://i.imgur.com/yU7kfZz.jpg "Robert's brainzzzzz")
## Code Walkthrough
### Basic Functionality
###### Taken from `main.c`
```
#include <msp430g2553.h>
#include "start5.h"
#include "functions.h"
#include "sensors.h"

unsigned int left_reading=0;
unsigned int right_reading=0;
unsigned int cent_reading=0;

unsigned char state=0;
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
void main(void) {
	state=0;
	initMSP430();
	P1DIR |= BIT0|BIT6;
	P1OUT &=~(BIT0|BIT6);
	stopRobot();
	moveRobotForward();
	__delay_cycles(18000000);
	while(1){
		_enable_interrupt();
		left_reading=poll_left();
		right_reading=poll_right();
		cent_reading=poll_cent();
		_disable_interrupt();
		if (state==0){
			if (DETECT_CENT&&CLEAR_LEFT){
				stopRobot();
				__delay_cycles(2000);
				moveRobotLeft();
				state=1;
			} else{
				moveRobotForward();
				state=0;
			}
		} else if (state==1){
			if (CLEAR_CENT){
				moveRobotRightSlow();
				state=2;
			} else{
				moveRobotLeft();
				state=1;
			}
		} else if (state==2){
			if (DETECT_CENT){
				stopRobot();
				moveRobotLeft();
				state=1;
			} else{
				moveRobotRightSlow();
				state=2;
			}
		}
	}
} // end main

// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P1DIR |= BIT0|BIT6;                     // set LEDs to output

	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	P1OUT &= ~(BIT0 | BIT6);			// An turn the LED off

	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts


	//set up left wheel PWM
	P2DIR |= (BIT1|BIT3|BIT4|BIT5);
	P2OUT &= ~(BIT3|BIT4);
	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period
    TA1CCR1 = 0x0080;
    TA1CCTL1 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
    TA1CCR2 = 0x0080;
    TA1CCTL2 = OUTMOD_3;
}
```

#### Hardware Implementation

![alt text](http://i.imgur.com/gvoO1bs.jpg "Robert's brainzzzzz")


## Debugging

Debugging was primarily done by looking at the Nokia 1202 display and the stored register and expression values within CCS-6.0's debugging feature. I also used an oscilloscope to peer into the PWM signals coming from the microcontroller and from the motor driver chip. Also, for A functionality, ensured my old code from lab 5 still worked before continuing onward with the lab..

## Testing Methodology/Results

#### Testing Methodology
To test the funcionality of my program, I attached the hardware IAW the diagram in my deisgn section, loaded the program, ran the program, and pressed buttons on my remote (or let the robot run). If it did what I wanted, then it passed the test.
#### Results
The code works!
##### Basic Functionality
Robert the robot followed the script to the letter and did exactly what it was supposed to do.
##### A Functionality
By pressing buttons, Robert would respond exactly how I expected.

## Observations and Conclusion
#### Observations

* Reusing previous .c and .h files can make coding faster and modular.
* Hardware can be tricky, read datasheets!

#### Conclusion

Robert the robot can move in a controlled, exact manner by himself or with commands from a user. Be afraid, be very afraid.

## Documentation

None



![alt text](https://scontent-a-lax.xx.fbcdn.net/hphotos-xap1/v/t1.0-9/74267_549500225064244_1378381565_n.jpg?oh=38f2ad443d39282026c478279ec6d1fa&oe=5543E6BF "...and beat Air Force too")
