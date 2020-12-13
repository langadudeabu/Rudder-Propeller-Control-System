/*
 * HeartBeat.c
 *
 * the heart beat will function will be called every half a second.
 *
 *  Created on: Nov 14, 2019
 *      Author: Sean
 */

/*freeRtos include */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"

/* Local Header Files */
#include "HeartBeat.h"

#include <gpio.h>
#include <ti/drivers/GPIO.h>
//#include "Utils.h"

void *heartbeat(void *arg0)
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // RED LED pin (P1.0)

    const TickType_t xDelay = 2*500 / portTICK_PERIOD_MS;
    while (1) {
        GPIO_toggleOutputOnPin(1, 1 << 0);
        vTaskDelay( xDelay );
    }
}
