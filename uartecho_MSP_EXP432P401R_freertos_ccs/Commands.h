/*
 * Commands.h
 *
 *  Created on: Nov 14, 2019
 *      Author: Sean
 */
#include <stdint.h>

#include "Threads.h"

#ifndef COMMANDS_H_
#define COMMANDS_H_

//void (*Table[26][26])(void);

//void TableInit(void);

void ToggleR(void); //rr
void ToggleG(void);//gg
void ToggleB(void);//bb

void LowSpeed(void); //ls
void MediumSpeed(void); //ms
void HighSpeed(void); //hs

void start(void); //go
void reverse(void); // vv
void stop(void); //xx

void Adjust(void);
void turn90(void);
void Park(void);

void Spin(void); // tu
void Turn180(void); //zz

int isEqual(char *, char *);

void changeDutyCycle(void); //cp
void setDuty(void);

void increaseRPM(void); // up
void decreaseRPM(void); // do
void incrementAngle(void); // ri
void decrementAngle(void); // le

#endif /* COMMANDS_H_ */
