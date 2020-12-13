/*
 * Utils.h
 *
 *  Created on: Nov 14, 2019
 *      Author: Sean
 */

#include <stdint.h>

#include <ti/drivers/UART.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/ADC.h>
//#include "Configs.h"
//#include "Commands.h"

#include <semaphore.h>

#ifndef UTILS_H_
#define UTILS_H_

#define MOTORPERIOD 10000
#define MAXSPEED 10000
#define MAXPROPELLER 1500
#define ACTSPEED 3000

/* Beginning of Motor Controls */
#define MAXRPM 20
#define MAX_ANGLE 35
#define MIN_ANGLE -35
// DIR1 (1.6) is for DC Motor
// DIR2 (1.7) is for Linear Actuator
#define propeller 0
#define rudder 1
#define both 2

#define trackLen 11.35 //inches
#define cmPerInch 2.54 // (cm / inch)

int rpm;
int angle;
double velocity;
double trackLength;
double position;
double angleIncrement;
uint32_t count; //rpmCounter
/* End of Motor Controls */

uint32_t dutyL; // LEFT WHEEL
uint32_t dutyR; // RIGHT WHEEL

int posInt;
float Angle;
uint16_t adcValue;
int buff[10];
int ind;
double max, min;
int8_t target;
volatile bool atTarget;
int maxValue;
int runningCondition;
int setZero;
int running;
int readCounts;
long int posCount;
int direction;
#define ANGLE_DIST 0.41184286 //cm per angle
char inChar;
bool charUsed;
int goal;

int propellerBuff[11][5];
double rudderBuff[15][50];
int ready1;
int ready2;
int rpmValue;

uint16_t Right, Front, Back;

UART_Handle uart;
PWM_Handle pwmL;
PWM_Handle pwmR;
Timer_Params timer_params;
Timer_Handle UART_timer;
Timer_Handle Position_timer;
ADC_Handle adcPOT; // Potentiometer

void checkMotorUART(void);
void updateMinMax(void);
void setupPWM(void);
void setupUART(void);
void setupMotorControl(void);
void setupMotorGPIO(void);
void setupADC(void);

void setRGB(int, int, int);

// Beginning of Motor Controls
void calibrate(void);
void setDutyPercent(int,int);
int convert2rpmNum(int);
void setRPM(int);
void setAngle(int);
void setPosition(double);
void moveActuator(int,double);
void *positionIncrement(void);
// End of Motor Controls

void ChangeDuty(int, uint32_t);

void putChar(char);
void putString(char *);
void getChar(char *);
uint32_t length(char *);
void getline(char *);

void TimerInit(void);
void TimerStart(void);
void TimerStop(void);

void set2Target(void);

void pos_feedback(int);
void convertToAngle(uint16_t);
int roundAngle(float);
uint16_t getPotValue(void);
void avgTest(void);
void set0(void);
void setUsed(void);
void updateAngle(void);
void propellerTestbench(void);
void rudderTestbench(void);

void peripheralInit(void);
void globalInit(void);

void pinON(int, int);
void pinOFF(int, int);
void pinTOGGLE(int, int);

void printTime(void);

//void CommandHandler(char *);

#endif /* UTILS_H_ */
