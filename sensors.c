//-----------------------------------------------------------------
// Name:	Jaksha
// File:	sensors.c
// Date:	Fall 2014
// Purp:	function library for using robot IR sensors
//-----------------------------------------------------------------
#include <msp430.h>
#include "sensors.h"

unsigned int poll_left(void){
	  unsigned int left_val=0;
	  ADC10CTL0 &=~(ENC|ADC10SC);
	  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
	  ADC10CTL1 = INCH_4;
	  ADC10AE0 &= 0x0000;
	  ADC10AE0 |= BIT4;                         // PA.1 ADC option select
	  ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;// Set P1.0 to output direction
	  ADC10CTL0 |= ENC|ADC10SC;             // Sampling and conversion start
	  __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	  left_val=ADC10MEM;
	  return left_val;
}

unsigned int poll_right(void){
	unsigned int right_val=0;
	ADC10CTL0 &=~(ENC|ADC10SC);
    ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
	ADC10CTL1 = INCH_3;                       // input A4
	ADC10AE0 &= 0x0000;
	ADC10AE0 |= BIT3;                         // PA.1 ADC option select
	ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;
	ADC10CTL0 |= ENC|ADC10SC;             // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	right_val=ADC10MEM;
	if (right_val>RIGHT_THRESH){
		P1OUT|=BIT0;
	} else{
		P1OUT &=~BIT0;
	}
	return right_val;
}

unsigned int poll_cent(void){
	  unsigned int cent_val=0;
	  ADC10CTL0 &=~(ENC|ADC10SC);
	  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
	  ADC10CTL1 = INCH_5;                       // input A4
	  ADC10AE0 &= 0x0000;
	  ADC10AE0 |= BIT5;                         // PA.1 ADC option select
	  ADC10CTL1 |= ADC10SSEL1|ADC10SSEL0;                // Select SMCLK
	  ADC10CTL0 |= ENC|ADC10SC;             // Sampling and conversion start
	  __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	  cent_val=ADC10MEM;
	  if (cent_val>CENT_THRESH_COME){
	  		P1OUT|=BIT6;
	  	} else{
	  		P1OUT &=~BIT6;
	  	}
	  return cent_val;
}
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
