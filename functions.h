//-----------------------------------------------------------------
// Name:	Jaksha
// File:	functions.h
// Date:	Fall 2014
// Purp:	header file for motor functions
//-----------------------------------------------------------------

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#define SUPER_FULL_SPEED_LEFT 0x0055
#define FULL_SPEED_LEFT 0x0080
#define FULL_SPEED_RIGHT 0x0070
#define HALF_SPEED_LEFT 0x0110
#define HALF_SPEED_RIGHT 0x099
void moveRobotForward(void);
void moveRobotBackwards(void);
void moveRobotLeft(void);
void moveRobotRight(void);
void moveRobotLeftSlow(void);
void moveRobotRightSlow(void);
void stopRobot(void);


#endif
