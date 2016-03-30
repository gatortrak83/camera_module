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
 * UART
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
    //Initialize ov2640
    __delay_cycles(10000);
    OV2640_Init(0);
    __delay_cycles(10000);
    OV2640_Init(1);


    while(1){

    	GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN4);
		__delay_cycles(10000000);

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

