/*
 * Utils.c
 *
 *  Created on: Nov 14, 2019
 *      Author: Sean
 */
/*freeRtos include */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include <semaphore.h>

/* Driver Header Files */
#include <gpio.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/ADC.h>
//#include <ti/drivers/Timer.h>
#include "C:\\ti\\simplelink_msp432p4_sdk_3_30_00_13\\source\\ti\\drivers\\Timer.h"
#include "ti_drivers_config.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <stddef.h>
//#include "uartecho.c"

//#include "Configs.h"
//#include "PID.h"
#include "Commands.h"
#include "Threads.h"
#include "Utils.h"

void setupPWM(void)
{
    PWM_init();

    pwmL = NULL;
    pwmR = NULL;

    PWM_Params params;
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US; // microseconds (us)
    params.dutyValue = 0; // duty in microseconds (us)
    params.periodUnits = PWM_PERIOD_US; // microseconds (us)
    params.periodValue = MOTORPERIOD; // period in microseconds

    do {
        pwmL = PWM_open(CONFIG_PWM_0, &params); // (P2.6) (A0.6) (PWM2) Propeller
    } while(pwmL == NULL);
    PWM_start(pwmL);

    do {
        pwmR = PWM_open(CONFIG_PWM_1, &params); // (P2.7) (A0.7) (PWM3) Rudder
    } while(pwmR == NULL);
    PWM_start(pwmR);
}

void setupUART(void)
{
//    char        input;
//    const char  echoPrompt[] = "Starting UART...\r\n";

    UART_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

//    UART_write(uart, echoPrompt, sizeof(echoPrompt));
}

void updateMinMax(void)
{
    char message[100];
    int ind = 0;
    char temp = '9';
    while(temp != '\n' && isdigit(temp))
    {
        getChar(&temp);
        message[ind++] = temp;
    }
    min = atoi(message);
    char mess[100];
    ind = 0;
    temp = '9';
    while(temp != '\n' && isdigit(temp))
    {
        getChar(&temp);
        mess[ind++] = temp;
    }
    max = atoi(mess);
}

char incomingByte = 0;
char message2[150];
void checkMotorUART(void)
{
    getChar(&incomingByte);
    switch(incomingByte)
    {
    case 'u':
        increaseRPM();
        break;
    case 'd':
        decreaseRPM();
        break;
    case 'l':
        start();
        break;
    case 'r':
        reverse();
        break;
    case 'x':
        stop();
        break;
    case 'n':
        target -= 5;
        atTarget = 0;
        buff[ind++] = target;
        break;
    case '0':
        target = 0;
        atTarget = 0;
        buff[ind++] = target;
        break;
    case 'm':
        target += 5;
        atTarget = 0;
        buff[ind++] = target;
        break;
    case 's':
        Timer_stop(Position_timer);
        break;
    case 'S':
        Timer_start(Position_timer);
        break;
    case 'j':
        updateMinMax();
        break;

//    case 'k':
////        sprintf(message2, "%d %d %d %d %d %d %d %d %d %d \n", buff[0], buff[0], buff[0],
////                buff[0], buff[0], buff[0], buff[0], buff[0], buff[0], buff[0]);
////        sprintf(message2, "buff =\n");
////        putString(message2);
//        putString("buff = \n");
//        break;
    default://this part isnt working...
        if(incomingByte != 0)
        {
            if(incomingByte <= 122)
            {
                charUsed = false;
                inChar = incomingByte;
            }
            else
            {
                target = (int)(incomingByte - 160);
                buff[ind++] = target;
                if(ind == 10)
                    ind = 0;
                GPIO_toggleOutputOnPin(1, 1 << 0);

    //            if(target == 8)
    //            {
    //                char g = 'o';
    ////                if(g == 'o');
    //            }

                atTarget = 0;
            }
        }
    }
}

void TimerInit(void)
{
    Timer_init();   // Ti's initilization of a timer

    /* Setup timer to run the UART-for-Motor-Controls function*/
    timer_params.period = 1000; // 1ms
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = (Timer_CallBackFxn)checkMotorUART;
    UART_timer = Timer_open(CONFIG_TIMER_0, &timer_params);

//    Timer_init();
//    Timer_open(CONFIG_TIMER_0, &timer_params);
    timer_params.period = 50000; // 50ms
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = (Timer_CallBackFxn)set2Target;
    Position_timer = Timer_open(1, &timer_params);
}

void TimerStart(void)
{
    Timer_start(UART_timer);
    Timer_start(Position_timer);
}

void TimerStop(void)
{
    Timer_stop(UART_timer);
    Timer_stop(Position_timer);
}

void updateAngle(void)
{
    adcValue = getPotValue();
    convertToAngle(adcValue); //updates Angle
    posInt = roundAngle(Angle);
}

double Thresh = 0.20;
int RunningCondition = 0;
const int fastSpeed = 5000;
const int slowSpeed = 2500;
int workingSpeed = 0;
int speedThresh = 10;
double Error = 0;
int prevTarget = 0;
int printCounter = 0;
char message3[100];
void set2Target(void)
{
    printCounter++;
    if(printCounter == 50)
    {
        printCounter = 0;
//        if(charUsed == true)
//            inChar = 'o';
//        sprintf(message3, "%d", target);
//        putString(message3);
    }

    if(atTarget && target != posInt) // make sure the target and angle are aligned once at Target
        atTarget = 0;
    if(buff[ind-1] != target) // make sure target updated properly
    {
        target = buff[ind-1];
        atTarget = 0;
    }
    if(!atTarget)
    {
        updateAngle(); // updates Angle and posInt

        Error = Angle - target; // actual - desired

        if(abs(Error) > speedThresh)
            workingSpeed = fastSpeed;
        else
            workingSpeed = slowSpeed;

        if( Error <= Thresh && Error >= -Thresh ) // basically there...
        {
            stop();

            int i = 0;
            int count = 0;
            for(i = 0; i < goal; i++) // check 5 times in a row before stopping the thread
            {
                updateAngle(); // updates Angle and posInt
                if( Error <= Thresh && Error >= -Thresh )
                    count++;
            }
            if( count == goal )
            {
                atTarget = 1;
            }
        }
        else if(Error < -Thresh)
        {
            pinON(1,7); // DIR2 --> ON for forward
            ChangeDuty(1, workingSpeed);
        }
        else if(Error > Thresh)
        {
            pinOFF(1,7); // DIR2 -> OFF for reverse
            ChangeDuty(1, workingSpeed);
        }
    }
}

void rudderTestbench(void)
{
    int ms = 25;
    const TickType_t xDelay = ms / portTICK_PERIOD_MS;
    char message[100];
    message[0] = 0;
    double averages[15] = {0.0};

//    putString("\r\n\n\t\t************ Starting Rudder Testbench ************\r\n\n");
    Thresh = 0.20; // temporarily change the thresh... more precise
    goal = 5;
    putString("Averages\n");
    int i = -35;
    int angleInd = 0;
    for(i = -35; i <= 35; i += 5)
    {
//        sprintf(message, "Moving Rudder to %d degrees...", i);
//        putString(message);

        target = i;
        buff[ind++] = target;
        atTarget = 0;
        while(!atTarget)
        {
        }
        Timer_stop(Position_timer);

        int buffInd = 0;
        while(buffInd < 50)
        {
            updateAngle();
            rudderBuff[angleInd][buffInd++] = Angle;
            vTaskDelay(xDelay);
        }

        Timer_start(Position_timer);
        int j = 0;
        double sum = 0.0;
        for(j = 0; j < 50; j++)
            sum += rudderBuff[angleInd][j];
        averages[angleInd] = (sum / 50.0);

//        sprintf(message, "Average = %f\r\n", averages[angleInd]);
        sprintf(message, "%f\n", averages[angleInd]);
        putString(message);
        angleInd++;
    }

    Thresh = 0.20;
    goal = 2;
//    putString("\r\n\n\t\t************ End of Rudder Testbench ************\r\n\n");

    int j = 0;
    angleInd = 0;
    for(i = -35; i <= 35; i += 5)
    {
        sprintf(message, "%d degrees\n", i);
        putString(message);
        for(j = 0; j < 50; j++)
        {
            sprintf(message, "%f"
                    "\n", rudderBuff[angleInd][j]);
            putString(message);
        }
        angleInd++;
    }
}

void propellerTestbench(void)
{
    int ms = 1000;
    const TickType_t xDelay = ms / portTICK_PERIOD_MS;

    //putString("\r\n\n\t\t************ Starting Propeller Testbench ************\r\n\n");

    char message[100];
    message[0] = 0;
    int i = 1;
    for(i = 1; i < 11; i++)
    {
        increaseRPM();
        vTaskDelay(xDelay);

        if(i == 1)
        {
            vTaskDelay(3000/portTICK_PERIOD_MS);
            //putString("Testing for for RPM = 0... ");
//            putString("[ 0, 0, 0, 0, 0, 0 ]\r\n\n");
            //putString("[ 0, 0, 0, 0, 0, 0 ]\r\n\n");
        }
        sprintf(message, "Testing for RPM = %d... ", i);
        //putString(message);

        int buffInd = 0;
        while(buffInd < 5)
        {
            if(ready2)
            {
                propellerBuff[i][buffInd++] = rpmValue;
                ready2 = 0;
            }
        }
        sprintf(message, "[ %d, %d, %d, %d, %d ]\r\n\n",
                propellerBuff[i][0], propellerBuff[i][1],
                propellerBuff[i][2], propellerBuff[i][3],
                propellerBuff[i][4]);
        //putString(message);
    }
    rpm = 0;
    setRPM(rpm);
    //putString("\t\t************ End of Propeller Testbench ************\r\n\n");

    int j = 0;
    for(i = 0; i < 11; i++)
    {
        sprintf(message, "%d rpms\n", i);
        putString(message);

        for(j = 0; j < 5; j++)
        {
            sprintf(message, "%d\n", propellerBuff[i][j]);
            putString(message);
        }
    }
}


void positionCounter(void) //HWI
{
    if(readCounts == 1)
    {
      if(direction == 0) // reverse
        posCount -= 1;
      if(direction == 1) //forward
        posCount += 1;
    }
}

void pos_feedback(int mode)
{
    if(mode == 0)
    {
        GPIO_disableInt(CONFIG_GPIO_POSITION); /* Enable interrupts */
    }
    if(mode == 1)
    {
        GPIO_enableInt(CONFIG_GPIO_POSITION); /* Enable interrupts */
    }
}

void setupMotorControl(void)
{
    setupMotorGPIO();
    goal = 2;
    int y = 0, z = 0;
    for(y = 0; y < 11; y++)
        for(z = 0; z < 5; z++)
            propellerBuff[y][z] = 0;
    for(y = 0; y < 15; y++)
        for(z = 0; z < 50; z++)
            rudderBuff[y][z] = 0;
    rpmValue = 0;
    ready1 = 0;
    ready2 = 0;
    posInt = 0;
    Angle = 0.0;
    adcValue = 0;
    ind = 1;
    int x = 0;
    for(x = 0; x < 10; x++)
        buff[x] = 0;
    charUsed = true;
    target = 0;
    atTarget = true;
    inChar = 0;
    max = 15088.0; //12760.0;
    min = 9563.0; //7499.0;
    maxValue = 4750;
    runningCondition = 0;
    setZero = 0;
    readCounts = 0;
    running = 0;
    posCount = 0;
    direction = 0; // reverse...
    rpm = 0;
    angle = 0;
    position = 0.0;
    velocity = 4.57333; // (cm / sec)
    trackLength = (double)(trackLen)*cmPerInch;
    angleIncrement = trackLength/70.0;
    count = 0;
}

void setupMotorGPIO(void)
{
    GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN4);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5); // IR Sensor
    GPIO_setConfig(CONFIG_GPIO_0, GPIO_CFG_OUT_LOW); /* Configure the IR Sensor p */

    GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN5); // IR Sensor
    GPIO_setConfig(CONFIG_GPIO_RPM, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING); /* Configure the IR Sensor pin */
    GPIO_enableInt(CONFIG_GPIO_RPM); /* Enable interrupts */

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6); // DIR1
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7); // DIR2
    pinON(1,6); // DIR1 is ON for forward (DC motor) pwmL
    pinON(1,7); //DIR2 is ON for forward (Linear Actuator) pwmR
}

void calibrate(void)
{
    char message[100];
    message[0] = 0;
    int calibrateSpeed = 3000;
    Timer_stop(Position_timer);
    int ms = 30000;
    const TickType_t xDelay = ms / portTICK_PERIOD_MS;
    putString("\r\nEntering Calibration Mode...\r\n");

    //go to full retraction
    putString("Retracting...");
    pinON(1,7); // DIR2 --> ON for forward
    ChangeDuty(1, calibrateSpeed);
    vTaskDelay(xDelay);
    stop();
    putString(" Maximum Voltage is now measured!\r\n");
    max = getPotValue();
    updateAngle();
    double maxAngle = Angle;

    // go to full extension
    putString("Extending... ");
    pinOFF(1,7); // DIR2 -> OFF for reverse
    ChangeDuty(1, calibrateSpeed);
    vTaskDelay(xDelay);
    stop();

//     putString("retrcting a small amount...");
//    //come back a small amount to hit the knob
//    pinON(1,7); // DIR2 --> ON for forward
//    ChangeDuty(1, 3000);
//    vTaskDelay(1000 / portTICK_PERIOD_MS);\
//    putString("done retracting....");
//    stop();
    putString(" Minimum Voltage is now measured!\r\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    min = getPotValue();
    updateAngle();
    double minAngle = Angle;
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    putString("Done Calibrating.\r\n\n");

    sprintf(message,"min = %f\tmax = %f\tzero = %f\thalf = %f\r\n", min, max, (min+max)/2.0, max - (min+max)/2.0);
    putString(message);
    sprintf(message,"minAngle = %f\tmaxAngle = %f\r\n", minAngle, maxAngle);
    putString(message);

    Timer_start(Position_timer);
}

void setupADC(void)
{
    ADC_init();
    ADC_Params params;

    adcPOT = NULL;

    ADC_Params_init(&params); // FRONT
    params.isProtected = false;
    do{
        adcPOT = ADC_open(CONFIG_ADC_POT, &params); // Potentiometer -> P4.4
    } while(adcPOT == NULL);
}

void convertToAngle(uint16_t adcVal)
{
    double zero = (min + max)/2.0;
    double half = max - zero;
//    double error = adcVal - zero;
    double error = adcVal - zero;
    Angle = 35.0*(error)/(half);
}
//-35 degrees = 12736
// 0 degrees = 10042
// 35 degrees = 7460
void avgTest()
{
    uint16_t potVal;
    uint16_t values[100];
    uint32_t max = 0;
    uint32_t min = 0xFFFFFFFF;
    uint32_t sum = 0;
    uint32_t mean = 0;
    int i;
    for(i = 0; i < 100; i++)
    {
        ADC_convert(adcPOT, &potVal);
        values[i] = potVal;
        sum += potVal;
        if(potVal > max)
            max = potVal;
        if(potVal < min)
            min = potVal;
    }
    mean = (uint16_t) ((float)sum / 100.0);

    char message1[50];
    char message2[50];
    sprintf(message1, "min: %u\r\nmax: %u\r\n", min, max);
    sprintf(message2, "mean: %u\r\n", mean);
    putString(message1);
    putString(message2);

}

uint16_t getPotValue()
{
    int i = 0;
    uint32_t sum = 0;
    uint16_t adcVal;
    for(i = 0; i < 20; i++)
    {
        ADC_convert(adcPOT, &adcVal);
        sum += adcVal;
    }
    uint16_t mean = (uint16_t)((double)sum/20.0);
    return mean;
}

int roundAngle(float anglee)
{
    int roundedAngle = 0;

    double bottom = 0;
    double half = 0;
    if(anglee < 0)
    {
        bottom = ceil((double)anglee);
        half = bottom - 0.5;
        if(anglee <= half) // more negative than the half
            roundedAngle = (int)floor(anglee);
        else
            roundedAngle = (int)ceil(anglee);
    }
    if(anglee > 0)
    {
        bottom = floor((double)anglee);
        half = bottom + 0.5;

        if(anglee >= half) // more positive than the half
            roundedAngle = (int)ceil(anglee);
        else
            roundedAngle = (int)floor(anglee);
    }
    return roundedAngle;
}

void set0()
{
//    int ms = 50;
//    const TickType_t xDelay = ms / portTICK_PERIOD_MS;
//
//    int running = 0;
//    double thresh = 0.20;
//    volatile double rudderAngle = 35.0;
//    while(rudderAngle > (0 + thresh) || rudderAngle < (0 - thresh))
//    {
//        rudderAngle = convertToAngle(getPotValue());
//        if(rudderAngle < 0)
//            if(running != 1)
//            {
//                stop();
//                //vTaskDelay(xDelay);
//                start();
//                running = 1;
//            }
//        if(rudderAngle > 0)
//            if(running != -1)
//            {
//               stop();
//               //vTaskDelay(xDelay);
//               reverse();
//               running = -1;
//            }
//    }
//    stop();
//    char c = 0;
}

void setUsed(void)
{
    charUsed = true;
    inChar = 0;
}

void setRGB(int redVal, int greenVal, int blueVal)
{
    bool isValid = true;

    // Make sure the values are either: 0 or 1
    if(redVal < 0 || redVal > 1)
        isValid = false;
    if(greenVal < 0 || greenVal > 1)
        isValid = false;
    if(blueVal < 0 || blueVal > 1)
        isValid = false;

    if(!isValid)
    {
        putString("\nIncorrect Values! Enter 0 or 1 for each input.\n\r");
        return;
    }

    (redVal == 1) ? pinON(2,0) : pinOFF(2,0);
    (greenVal == 1) ? pinON(2,1) : pinOFF(2,1);
    (blueVal == 1) ? pinON(2,2) : pinOFF(2,2);
}

void setDutyPercent(int m, int percent)
{
    int offset = 300;
    int Duty = offset;// if percent == 5
    if(percent < 5)
        Duty = 0;
    else if(percent > 5)
        Duty = offset + (int)((((double)(percent-5))/100.0)*(MAXPROPELLER-offset));
    ChangeDuty(m, Duty);
}


int convert2rpmNum(int rpms)
{
    double nums[11] = {0, 386.25, 463.25, 536.25, 610.5, 670.0,
                       730.0, 803.75, 850.0, 893.0, 1005.0};

    int i = 0;
    double minDiff = 0xFFFFFFFF;
    int min = 0;

    for(i = 0; i < 11; i++)
        if(abs(rpms - nums[i]) < minDiff)
        {
            min = i;
            minDiff = abs(rpms - nums[i]);
        }

//    if(rpms >= (1000-30) || rpms <= (1000+30))
//    {
//        return 1;
//    }
//    int rpmNumber = rpms/((double)(maxValue-1000)/19.0);

    return min;
}

void setRPM(int RPM)
{
    int percent = (int)(((double)RPM/(double)MAXRPM)*100);
    setDutyPercent(propeller, percent);
}

void setAngle(int Angle)
{
    double newPosition = angle*angleIncrement;
    setPosition(newPosition);
}

void setPosition(double pos)
{
    int forward = 1;
    int reverse = -1;
    double distance = 0.0;

    if(pos > position)
    {
        distance = pos - position;
        moveActuator(forward, distance);
    }
    if(pos < position)
    {
        distance = position - pos;
        moveActuator(reverse, distance);
    }
    position = pos;
}

// Probably need to use the FreeRTOS Timer for the delay...
void moveActuator(int dir, double distance)
{
    if(dir < 0)
    {
        pinOFF(1,7); // DIR2 = 0 for reverse
        direction = 0;
    }
    else
    {
        pinON(1,7); // DIR2 = 1 for forward
        direction = 1;
    }

    const TickType_t xDelay = (int)((distance/velocity)*1000);
    setDutyPercent(rudder, ACTSPEED);
    vTaskDelay(xDelay);
    setDutyPercent(rudder,0);
}

uint8_t lastValue = 1;
void *positionIncrement(void)
{
    if(readCounts == 1)
    {
        uint8_t sensor = GPIO_read(CONFIG_GPIO_POSITION);
        if(lastValue == 1 && sensor == 0)
        {
            if(direction == 0) // reverse
                posCount -= 1;
            if(direction == 1) //forward
                posCount += 1;
        }
        lastValue = sensor;
    }
}

void rotIncrement(void) // HWI
{
    count += 1;
}

void ChangeDuty(int m, uint32_t duty)
{
    if(duty > MOTORPERIOD)
        duty = MOTORPERIOD;

    switch(m)
    {
    case 0: //LEFT WHEEL (m = 0)
        PWM_setDuty(pwmL, duty);
        dutyL = duty;
        break;
    case 1: //RIGHT WHEEL (m = 1)
        PWM_setDuty(pwmR, duty);
        dutyR = duty;
        break;
    case 2: //BOTH WHEELS (m = 2)
        PWM_setDuty(pwmL, duty);
        PWM_setDuty(pwmR, duty);
        dutyL = dutyR = duty;
        break;
    }

//    char message[100];
//    sprintf(message, "dutyL = %d\tdutyR = %d\n\r", dutyL, dutyR);
//    putString(message);
}




void putChar(char a) {
    UART_write(uart, &a, 1);
}

void putString(char *message)
{
    UART_write(uart, message, length(message));
}

void getChar(char *inChar)
{
    UART_read(uart, inChar, 1);
}

uint32_t length(char *m)
{
    uint32_t len = 0;
    while(m[len] != '\0')
    {
        len++;
    }
    return len;
}

void pinON(int port, int pin)
{
    GPIO_setOutputHighOnPin(port, 1 << pin);
}

void pinOFF(int port, int pin)
{
    GPIO_setOutputLowOnPin(port, 1 << pin);
}

void pinTOGGLE(int port, int pin){
    GPIO_toggleOutputOnPin(port, 1 << pin);
}

void getline(char inString[COMMANDLENGTH + 1])
{
    int numLetters = 0;

    char temp = 0;
    do
    {
        getChar(&temp); //read 1 char
        putChar(temp); // ECHO to terminal

        if(temp == '\b'  && numLetters > 0) // if BACKSPACE
        {
            putString(" \b"); //delete char from terminal
            numLetters--;
            continue;
        }

        inString[numLetters++] = temp;

    }while((numLetters < COMMANDLENGTH) && (temp != '\r'));
    inString[numLetters] = '\0';
}









