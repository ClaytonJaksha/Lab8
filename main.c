//-----------------------------------------------------------------
// Name:	Jaksha
// File:	main.c
// Date:	Fall 2014
// Purp:	ROBOT MAZEEEEEEE
//-----------------------------------------------------------------
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
