/***********************************************************
 * Design II Stereo Vision Project
 * ---------------------
 * Description: Initializes OV2640 Cameras
 * Team: Search and Destroy
 * Created by: Steven Long
 * Contribution by:
 ***********************************************************/

#include "ov2640.h"
#include "driverlib.h"
#include "SCCB.h"
#include "system_init.h"


/*************************************
 *Variable for Oscillator fault flags
 *************************************/
uint16_t status;



void main(void) {

/*************************************
 * Watch Dog Timer
 *************************************/
    WDT_A_hold(WDT_A_BASE);

/*************************************
 * GPIO
 *************************************/
    gpio_init();

/*************************************
 * Clocks
 *************************************/
    clock_init();

/*************************************
 * UART (not used)
 *************************************/
//	uart_init();

/*************************************
 * I2C/SCCB
 *************************************/
    //Initialize I2C/SCCB Master Mode
    sccb_init_master(0);
    sccb_init_master(1);

/*************************************
 * OV2640
 *************************************/
//comment out for 1 cam
//start
/*
    //Wait for signal then init camera 1
    while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == 0){
        __delay_cycles(1000000);
    	GPIO_toggleOutputOnPin(GPIO_PORT_P5, GPIO_PIN6);
    }

    //Toggle camera 1 reset
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
    __delay_cycles(1000000);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
    __delay_cycles(1000000);

    //Initialize camera 1
    OV2640_Init(0);

    //Signal camera 1 init complete to Zedboard
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
    __delay_cycles(24000000);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);

    //Turn off camera 1 led
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);
*/
//end

    //Wait for signal then init camera 3
    while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) == 0){
        __delay_cycles(1000000);
    	GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN6);
    }

    //Toggle camera 3 reset
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
    __delay_cycles(1000000);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
    __delay_cycles(1000000);

    //Initialize camera 3
    OV2640_Init(1);

    //Signal camera 3 init complete to zedboard
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
    __delay_cycles(24000000);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);

    //Turn off camera 3 led
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);

    //Turn on done led
	GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);

    while(1){

		__delay_cycles(6000);

    }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(UNMI_VECTOR)))
#endif
void NMI_ISR(void)
{
    do
    {
        // If it still can't clear the oscillator fault flags after the timeout,
        // trap and wait here.
        status = UCS_clearAllOscFlagsWithTimeout(1000);
    }
    while(status != 0);
}

