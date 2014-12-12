//-----------------------------------------------------------------
// Name:	Jaksha
// File:	sensors.h
// Date:	Fall 2014
// Purp:	header file for sensor library
//-----------------------------------------------------------------

#ifndef SENSORS_H_
#define SENSORS_H_

#define CENT_THRESH_LEAVE 0x220
#define CENT_THRESH_COME 0x230
#define RIGHT_THRESH 0x280
#define LEFT_THRESH 0x260
#define DETECT_RIGHT (right_reading>RIGHT_THRESH)
#define DETECT_LEFT (left_reading>LEFT_THRESH)
#define DETECT_CENT (cent_reading>CENT_THRESH_COME)
#define CLEAR_RIGHT (right_reading<RIGHT_THRESH)
#define CLEAR_LEFT (left_reading<LEFT_THRESH)
#define CLEAR_CENT (cent_reading<CENT_THRESH_LEAVE)

__interrupt void ADC10_ISR(void);
unsigned int poll_left(void);
unsigned int poll_right(void);
unsigned int poll_cent(void);

#endif /* SENSORS_H_ */
