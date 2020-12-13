/*
 *  ======== ti_drivers_config.c ========
 *  Configured TI-Drivers module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSP_EXP432P401R
 *  by the SysConfig tool.
 */

#include <stddef.h>

#ifndef DeviceFamily_MSP432P401x
#define DeviceFamily_MSP432P401x
#endif

#include <ti/devices/DeviceFamily.h>

#include "ti_drivers_config.h"

/*
 *  =============================== ADC ===============================
 */

#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCMSP432.h>

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/adc14.h>
#include <ti/devices/msp432p4xx/driverlib/ref_a.h>

#define CONFIG_ADC_COUNT 1

/*
 *  ======== adcMSP432Objects ========
 */
ADCMSP432_Object adcMSP432Objects[CONFIG_ADC_COUNT];

/*
 *  ======== adcMSP432HWAttrs ========
 */
const ADCMSP432_HWAttrsV1 adcMSP432HWAttrs[CONFIG_ADC_COUNT] = {
    /* CONFIG_ADC_POT */
    {
        .adcPin = ADCMSP432_P4_4_A9,
        .refVoltage = ADCMSP432_REF_VOLTAGE_INT_2_5V,
        .resolution = ADC_14BIT,
        .refExtValue = 2500000
    },
};

/*
 *  ======== ADC_config ========
 */
const ADC_Config ADC_config[CONFIG_ADC_COUNT] = {
    /* CONFIG_ADC_POT */
    {
        .fxnTablePtr = &ADCMSP432_fxnTable,
        .object = &adcMSP432Objects[CONFIG_ADC_POT],
        .hwAttrs = &adcMSP432HWAttrs[CONFIG_ADC_POT]
    },
};

const uint_least8_t ADC_count = CONFIG_ADC_COUNT;


/*
 *  =============================== GPIO ===============================
 */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP432.h>

/*
 *  ======== gpioPinConfigs ========
 *  Array of Pin configurations
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* CONFIG_GPIO_LED_0 : LaunchPad LED 1 Red */
    GPIOMSP432_P1_0 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* CONFIG_GPIO_RPM */
    GPIOMSP432_P3_5 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING,
    /* CONFIG_GPIO_POSITION */
    GPIOMSP432_P5_0 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING,
    /* CONFIG_GPIO_0 */
    GPIOMSP432_P1_5 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
};

/*
 *  ======== gpioCallbackFunctions ========
 *  Array of callback function pointers
 *
 *  NOTE: Unused callback entries can be omitted from the callbacks array to
 *  reduce memory usage by enabling callback table optimization
 *  (GPIO.optimizeCallbackTableSize = true)
 */
extern void rotIncrement(uint_least8_t index);

GPIO_CallbackFxn gpioCallbackFunctions[] = {
    /* CONFIG_GPIO_LED_0 : LaunchPad LED 1 Red */
    NULL,
    /* CONFIG_GPIO_RPM */
    rotIncrement,
    /* CONFIG_GPIO_POSITION */
    NULL,
    /* CONFIG_GPIO_0 */
    NULL,
};

/*
 *  ======== GPIOMSP432_config ========
 */
const GPIOMSP432_Config GPIOMSP432_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = 4,
    .numberOfCallbacks = 4,
    .intPriority = (~0)
};


/*
 *  =============================== PWM ===============================
 */

#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerMSP432.h>

/* include MSP432 driverlib definitions */
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/timer_a.h>

#define CONFIG_PWM_COUNT 2

/*
 *  ======== pwmMSP432Objects ========
 */
PWMTimerMSP432_Object pwmTimerMSP432Objects[CONFIG_PWM_COUNT];

/*
 *  ======== pwmMSP432HWAttrs ========
 */
const PWMTimerMSP432_HWAttrsV2 pwmTimerMSP432HWAttrs[CONFIG_PWM_COUNT] = {
    /* CONFIG_PWM_0 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P2_6_TA0CCR2A, /* P2_6 */
    },
    /* CONFIG_PWM_1 */
    {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
        .pwmPin = PWMTimerMSP432_P2_7_TA0CCR1A, /* P2_7 */
    },
};

/*
 *  ======== PWM_config ========
 */
const PWM_Config PWM_config[CONFIG_PWM_COUNT] = {
    /* CONFIG_PWM_0 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_0],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_0]
    },
    /* CONFIG_PWM_1 */
    {
        .fxnTablePtr = &PWMTimerMSP432_fxnTable,
        .object = &pwmTimerMSP432Objects[CONFIG_PWM_1],
        .hwAttrs = &pwmTimerMSP432HWAttrs[CONFIG_PWM_1]
    },
};

const uint_least8_t PWM_count = CONFIG_PWM_COUNT;


/*
 *  =============================== Power ===============================
 */

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>
extern void PowerMSP432_initPolicy(void);
extern void PowerMSP432_sleepPolicy(void);

const PowerMSP432_ConfigV1 PowerMSP432_config = {
    .policyInitFxn         = PowerMSP432_initPolicy,
    .policyFxn             = PowerMSP432_sleepPolicy,
    .initialPerfLevel      = 2,
    .enablePolicy          = true,
    .enablePerf            = true,
    .enableParking         = false,
    .resumeShutdownHookFxn = NULL,
    .customPerfLevels      = NULL,
    .numCustom             = 0,
    .useExtendedPerf       = false,
    .configurePinHFXT      = false,
    .HFXTFREQ              = 0,
    .bypassHFXT            = false,
    .configurePinLFXT      = false,
    .bypassLFXT            = false,
    .LFXTDRIVE             = 0,
    .enableInterruptsCS    = false,
    .priorityInterruptsCS  = (~0),
    .isrCS                 = NULL
};


/*
 *  =============================== Timer ===============================
 */

#include <ti/drivers/Timer.h>
#include <ti/drivers/timer/TimerMSP432.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/timer_a.h>
#include <ti/devices/msp432p4xx/driverlib/timer32.h>

#define CONFIG_TIMER_COUNT 2

/*
 *  ======== timerMSP432Objects ========
 */
TimerMSP432_Object timerMSP432Objects[CONFIG_TIMER_COUNT];

/*
 *  ======== timerMSP432HWAttrs ========
 */
const TimerMSP432_HWAttrs TimerMSP432HWAttrs[CONFIG_TIMER_COUNT] = {
    /* CONFIG_TIMER_0 */
    {
        .timerBaseAddress  = TIMER32_0_BASE,
        .intNum            = INT_T32_INT1,
        .intPriority       = 0x20,
    },
    /* CONFIG_TIMER_1 */
    {
        .timerBaseAddress  = TIMER32_1_BASE,
        .intNum            = INT_T32_INT2,
        .intPriority       = (~0),
    },
};

/*
 *  ======== Timer_config ========
 */
const Timer_Config Timer_config[CONFIG_TIMER_COUNT] = {
    /* CONFIG_TIMER_0 */
    {
        .fxnTablePtr = &TimerMSP432_Timer32_fxnTable,
        .object      = &timerMSP432Objects[CONFIG_TIMER_0],
        .hwAttrs     = &TimerMSP432HWAttrs[CONFIG_TIMER_0]
    },
    /* CONFIG_TIMER_1 */
    {
        .fxnTablePtr = &TimerMSP432_Timer32_fxnTable,
        .object      = &timerMSP432Objects[CONFIG_TIMER_1],
        .hwAttrs     = &TimerMSP432HWAttrs[CONFIG_TIMER_1]
    },
};

const uint_least8_t Timer_count = CONFIG_TIMER_COUNT;


/*
 *  =============================== UART ===============================
 */

#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/uart.h>

#define CONFIG_UART_COUNT 1

UARTMSP432_Object uartMSP432Objects[CONFIG_UART_COUNT];

static const UARTMSP432_BaudrateConfig uartMSP432Baudrates[] = {
    /* {baudrate, input clock, prescalar, UCBRFx, UCBRSx, oversampling} */
    { 115200, 3000000, 1, 10, 0, 1 },
    { 115200, 6000000, 3, 4, 2, 1 },
    { 115200, 12000000, 6, 8, 32, 1 },
    { 115200, 24000000, 13, 0, 37, 1 },
};

static unsigned char uartMSP432RingBuffer0[32];


static const UARTMSP432_HWAttrsV1 uartMSP432HWAttrs[CONFIG_UART_COUNT] = {
  {
    .baseAddr           = EUSCI_A0_BASE,
    .intNum             = INT_EUSCIA0,
    .intPriority        = 0x20,
    .clockSource        = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
    .bitOrder           = EUSCI_A_UART_LSB_FIRST,
    .numBaudrateEntries = sizeof(uartMSP432Baudrates) /
                          sizeof(UARTMSP432_BaudrateConfig),
    .baudrateLUT        = uartMSP432Baudrates,
    .ringBufPtr         = uartMSP432RingBuffer0,
    .ringBufSize        = sizeof(uartMSP432RingBuffer0),
    .rxPin              = UARTMSP432_P1_2_UCA0RXD,
    .txPin              = UARTMSP432_P1_3_UCA0TXD,
    .errorFxn           = NULL
  },
};

const UART_Config UART_config[CONFIG_UART_COUNT] = {
    {   /* CONFIG_UART_0 */
        .fxnTablePtr = &UARTMSP432_fxnTable,
        .object      = &uartMSP432Objects[0],
        .hwAttrs     = &uartMSP432HWAttrs[0]
    },
};

const uint_least8_t UART_count = CONFIG_UART_COUNT;


#include <ti/drivers/Board.h>

/*
 *  ======== Board_initHook ========
 *  Perform any board-specific initialization needed at startup.  This
 *  function is declared weak to allow applications to override it if needed.
 */
void __attribute__((weak)) Board_initHook(void)
{
}

/*
 *  ======== Board_init ========
 *  Perform any initialization needed before using any board APIs
 */
void Board_init(void)
{
    /* ==== /ti/drivers/Power initialization ==== */
    Power_init();

    Board_initHook();
}
