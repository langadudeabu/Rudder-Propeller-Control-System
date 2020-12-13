/*
 * Threads.c
 *
 *  Created on: Nov 14, 2019
 *      Author: Sean
 */

/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

/*freeRtos include */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

/* POSIX Header Files */
#include <semaphore.h>

/* Driver Header Files */
#include <gpio.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
/* Local Header Files */
#include "Utils.h"
#include "Threads.h"
#include "Commands.h"
#include <semaphore.h>
char rpmChar;
int rpmInt;
char posChar;

bool printAngle = true;
int ready_1 = 0;
int ready_2 = 0;

void *ReadPosition(void *arg0)
{
    int ms = 500;
    const TickType_t xDelay = ms / portTICK_PERIOD_MS;
    while(1)
    {
        updateAngle();// updates adcValue, Angle, and posInt
        posChar = (char)(posInt);
//        else posChar = (int8_t)0;

        ready1 = 1; // tell sendMeasurements it's value is ready
        ready_1 = 1;
        vTaskDelay(xDelay);
    }
}

void *RPM_reader(void *arg0)
{
    int ms = 2000;
    const TickType_t xDelay = ms / portTICK_PERIOD_MS;
    double multiple = 1000.0/ms;
    double revPer = 0;
    int revPerDelay = 0;

    while(1)
    {
        vTaskDelay(xDelay);

        revPer = (double)count/4.0;
        revPerDelay = (int)(ceil(revPer));
        if(revPer - floor(revPer) < 0.5)
            revPerDelay = (int)(floor(revPer));

        rpmValue = revPerDelay*(60*multiple);
        rpmInt = convert2rpmNum(rpmValue);
        rpmChar = (char)(rpmInt);

        ready2 = 1; // for Propeller Testbench
        ready_2 = 1; // tell sendMeasurements it's value is ready
        count = 0;
    }
}

void *sendMeasurements(void *arg0)
{
    int ms = 35;
    const TickType_t xDelay = ms / portTICK_PERIOD_MS;
    while(1)
    {
        vTaskDelay(xDelay);
        if(ready_1 && ready_2)
        {
            ready_1 = 0;
            ready_2 = 0;
            char message[100];
            sprintf(message, "%c%c", posChar + 100, rpmChar + 100);
//        sprintf(message, "ADC = %d\tANGLE = %f\tPOS = %c\r\n", adcValue, Angle, posInt);
//        sprintf(message, "RPMs = %d\r\n", rpmValue );
//        sprintf(message, "RPM_NUM = %d\tRPM_INT = %d\tRPM_CHAR = %d\tRPM_VAL = %d\r\n\n", rpm, rpmInt, (int)rpmChar, rpmValue);
//        sprintf(message, "RPM_NUM = %d\tRPM_VAL = %d\r\n\n", rpm, rpmValue);
//        sprintf(message, "Angle = %f\r\n", Angle);
            //sprintf(message, "adcVal = %d\r\n", adcValue);

            if(printAngle)
                putString(message);
        }
    }
}

void *bluetooth2(void *arg0)
{
    char message1[200];

    while(1)
    {
        switch(inChar)
        {
        case 'z':
            setZero = 1;
            setUsed();
            break;
        case 'v':
            setZero = 0;
            stop();
            setUsed();
            break;
        case 'e':
            rpm = 0;
            setRPM(rpm);
            setZero = 1;
            setUsed();
            break;
        case 'p':
            avgTest();
            setUsed();
            break;
        case 'c':
            calibrate();
            setUsed();
            break;
        case 'P':
            sprintf(message1, "\r\nninVoltage = %f\tmaxVoltage = %f\r\n\n", min, max);
            putString(message1);
            setUsed();
            break;
        case 't':
            printAngle = !printAngle;
            setUsed();
            break;
        case '*':
            printAngle = false;
            setUsed();
            break;
        case '&':
            printAngle = true;
            setUsed();
            break;
        case 'k':
            sprintf(message1, "%d %d %d %d %d %d %d %d %d %d \r\n", buff[0], buff[1], buff[2],
                    buff[3], buff[4], buff[5], buff[6], buff[7], buff[8], buff[9]);
            putString(message1);
            if(ind == 0)
                target = buff[9];
            else
                target = buff[ind-1];
            atTarget = 0;
            setUsed();
            break;
        case 'o':
            sprintf(message1, "\r\nTarget = %d atTarget = %d\tAngle = %f\tadcValue = %d\terror = %fposInt = %d\t%d\r\n",
                    target, atTarget, Angle, adcValue, adcValue-(min+max)/2.0, posInt, posInt == target);
            putString(message1);
            setUsed();
            break;
        case 'q':
            propellerTestbench();
            setUsed();
            break;
        case 'w':
            rudderTestbench();
            setUsed();
            break;


        }
    }
}
