/*
 * Commands.c
 *
 *  Created on: Nov 14, 2019
 *      Author: Sean
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/*freeRtos include */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

// ti drivers
//#include <ti/drivers/ADC.h>
#include <ti/drivers/PWM.h>
//#include <ti/drivers/Timer.h>
#include "C:\\ti\\simplelink_msp432p4_sdk_3_30_00_13\\source\\ti\\drivers\\Timer.h"

#include "Utils.h"
//#include "Configs.h"
//#include "PID.h"
#include "Threads.h"

#include "Commands.h"


//void TableInit(void)
//{
//    int i, j;
//    //Initialize Table
//    for(i = 0; i < 26; i++)
//        for(j = 0; j < 26; j++)
//            Table[i][j] = 0;
//    Table['r'-'a']['r'-'a'] = &ToggleR;     //rr
//    Table['g'-'a']['g'-'a'] = &ToggleG;     //gg
//    Table['b'-'a']['b'-'a'] = &ToggleB;     //bb
//    Table['l'-'a']['s'-'a'] = &LowSpeed;    //ls
//    Table['m'-'a']['s'-'a'] = &MediumSpeed; //ms
//    Table['h'-'a']['s'-'a'] = &HighSpeed;   //hs
//    Table['g'-'a']['o'-'a'] = &start;   //go
//    Table['v'-'a']['v'-'a'] = &reverse;   //vv
//    Table['s'-'a']['t'-'a'] = &Start;       // st
//    Table['s'-'a']['p'-'a'] = &Stop;        // sp
//    Table['x'-'a']['x'-'a'] = &stop;        // xx
//    Table['t'-'a']['u'-'a'] = &Spin;        //tu
//    Table['z'-'a']['z'-'a'] = &Turn180;     // zz
//    Table['p'-'a']['r'-'a'] = &Park;        // pr
//    Table['c'-'a']['h'-'a'] = &ChangePID;   //ch
//    Table['c'-'a']['t'-'a'] = &ChangeThresh;   //ct
//    Table['c'-'a']['c'-'a'] = &ChangeCount;   //cc
//    Table['p'-'a']['v'-'a'] = &printValues;   //pv
//    Table['p'-'a']['s'-'a'] = &printSensors;   //ps
//    Table['c'-'a']['p'-'a'] = &changeDutyCycle; //cp
//    Table['s'-'a']['d'-'a'] = &setDuty; //sd
//
//    Table['u'-'a']['p'-'a'] = &increaseRPM; // up
//    Table['d'-'a']['o'-'a'] = &decreaseRPM; // do
//    Table['r'-'a']['i'-'a'] = &incrementAngle; // ri
//    Table['l'-'a']['e'-'a'] = &decrementAngle; // le
//
//
//}

int isEqual(char *a, char *b)
{
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0' && a[i] == b[i] && i < 11) { // ends if string is null-terminated or exceeds 11 characters (10 character command maximum)
        i++;
    }
    if(a[i] != b[i]) { // if the strings are not the same return false
        return 0;
    }
    return 1; // if the strings are the same return true
}

void ToggleR(void)//"rr"
{
    putString("\nYour Command Has Been Received\n\r");
    putString("Turning on Red LED...\n\r");
    setRGB(1,0,0);
}

void ToggleG(void)//"gg"
{
    putString("\nYour Command Has Been Received\n\r");
    putString("Turning on Green LED...\n\r");
    setRGB(0,1,0);
}

void ToggleB(void)//"bb"
{
//    putString("\nYour Command Has Been Received\n\r");
//    putString("Turning on Blue LED...\n\r");
    setRGB(0,0,1);
}

void LowSpeed(void) // ls forward
{
    pinON(5,1); // MODE == 1
    pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
    pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

    putString("\nChanging Speed to LOW...\n\r");

    uint32_t duty = (2*MOTORPERIOD)/10; // 20% duty cycle
    //uint32_t duty = 2;
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void MediumSpeed(void) // ms forward
{
    pinON(5,1); // MODE == 1
    pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
    pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

    putString("\nChanging Speed to MEDIUM...\n\r");

    uint32_t duty = (6*MOTORPERIOD)/10; //60% duty cycle
    //uint32_t duty = 6;
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void HighSpeed(void) // hs forward
{
    pinON(5,1); // MODE == 1
    pinON(3,7); //PHASE_A = 1 (Left Wheel - Forward)
    pinON(3,5); //PHASE_B = 1 (Right Wheel - Forward)

    putString("\nChanging Speed to HIGH...\n\r");

    uint32_t duty = (95*MOTORPERIOD)/100; // 85% duty cycle
    //uint32_t duty = 8;
    ChangeDuty(2, duty); // 2 = Change the duty for both wheels
}

void start(void) //go
{
    readCounts = 1;
    running = 1;
    pinON(1,7); // DIR2 --> ON for forward
    ChangeDuty(1, ACTSPEED);
    direction = 1; // forward
//    pos_feedback(1); // enable GPIO interrupts on position feedback pin
}

void reverse(void) // vv
{
    readCounts = 1;
    pinOFF(1,7); // DIR2 -> OFF for reverse
    ChangeDuty(1, ACTSPEED);
    direction = 0; // reverse
//    if(running == 0)
//        pos_feedback(1); // enable GPIO interrupts on position feedback pin
    running = 1;
}

void stop(void) //xx
{
    readCounts = 0;
    running = 0;
    uint32_t duty = 0;
    setDutyPercent(1, duty); // 2 = Change the duty for both wheels
//    pos_feedback(0); // enable GPIO interrupts on position feedback pin
}

void changeDutyCycle(void) //cp
{
    char inString[COMMANDLENGTH + 1];

    putString("\nEnter \"<Duty>\": ");
    getline(inString);

    int Duty = atoi(inString);
    ChangeDuty(2, Duty);
}

void setDuty(void) //sd
{
    char inString[COMMANDLENGTH + 1];

    putString("\nEnter \"<PercentDuty(int)>\": ");
    getline(inString);

    int percent = atoi(inString);
    setDutyPercent(both, percent);
}

void increaseRPM(void) // up
{
//    putString("up\n\r");
    rpm += 1;
    setRPM(rpm);
}

void decreaseRPM(void) // do
{
//    putString("do\n\r");
    rpm -= 1;
    setRPM(rpm);
}

void incrementAngle(void) // le
{
    angle += 1;

    char message[50];
    sprintf(message, "Angle = %d\n",angle);
    putString(message);
    setAngle(angle);
}

void decrementAngle(void) //ri
{
//    putString("ri\n\r");
    angle -= 1;

    char message[50];
    sprintf(message, "Angle = %d\n",angle);
    putString(message);

    setAngle(angle);
}
