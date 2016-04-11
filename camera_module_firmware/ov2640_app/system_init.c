/*
 * system_init.c
 *
 *      Author: Steven Long
 */


#include "driverlib.h"
#include "system_init.h"


/*********************************************
 * UART Variables
 ********************************************/
uint8_t receivedData = 0x00;
uint8_t transmitData = 0x00;
uint8_t check = 0;




/**********************************************
 * Function: clock_init()
 * Inputs: none
 * Outputs: none
 * Description:
 * 		Outputs clocks to gpio pins
 * 		Sets gpio pins for XT1
 * 		Sets XT1 to Ref for FLL
 * 		Sets MCLK to 24 Mhz
 * 		Change VCORE
 * 		Sets MCLK to SMCLK and ACLK
 *********************************************/


void clock_init(void){

	uint8_t returnValue = 0;

	//Comment out if not using VCO
	//Start

	GPIO_setAsOutputPin(
		GPIO_PORT_P3,
		GPIO_PIN5);

	GPIO_setOutputHighOnPin(
		GPIO_PORT_P3,
		GPIO_PIN5);


	//END


	//Comment out clock peripheral output setup
	//for VCO usage
	//Start
/*
    //Output MCLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P7,
        GPIO_PIN7);

    //Output ACLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0);

    //Output SMCLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN2);

    //END
*/

    //Port select XT1
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN4 + GPIO_PIN5
        );

    //Set VCore = 3 for 24MHz clock
    PMM_setVCore(PMM_CORE_LEVEL_3);

    //Initialize XT1. Returns STATUS_SUCCESS if initializes successfully
    returnValue = UCS_turnOnLFXT1WithTimeout(
        UCS_XT1_DRIVE_0,
        UCS_XCAP_3,
        UCS_XT1_TIMEOUT
        );

    UCS_setExternalClockSource(
        UCS_XT1_CRYSTAL_FREQUENCY,
        UCS_XT2_CRYSTAL_FREQUENCY
        );

    //Set DCO FLL reference = XT1
    UCS_initClockSignal(
        UCS_FLLREF,
        UCS_XT1CLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );

    //Set Ratio and Desired MCLK Frequency  and initialize DCO
    UCS_initFLLSettle(
    	MCLK_FREQ_KHZ,
        UCS_MCLK_FLLREF_RATIO
        );


    //Select DCO as ACLK source
    UCS_initClockSignal(
        UCS_ACLK,
        UCS_DCOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );

    //Select DCO as SMCLK source
    UCS_initClockSignal(
        UCS_SMCLK,
        UCS_DCOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );



    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);

    // Enable global interrupt
    __bis_SR_register(GIE);

}




/**********************************************
 * Function: gpio_init()
 * Inputs: none
 * Outputs: none
 * Description:
 * 		Sets GPIO Outputs:
 * 			-
 * 		Sets GPIO Inputs:
 * 			-
 *********************************************/

void gpio_init(void){

	//OUTPUTS

	//P7.4 -> LED D3, Initialized low
	GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
	GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);

	//P5.6 -> LED D4, Initialized low
	GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);

	//4.6 -> LED D5, Initialized low
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);
	GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);


	//1.5 camera 3 set to Zedboard
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);

	//1.6 camera 1 set to Zedboard
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);

	//P2.6 reset for camera 1
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);

	//P2.6 reset for camera 1
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);


	//Inputs

	//P7.5 -> Button SW3
	GPIO_setAsInputPin(GPIO_PORT_P7, GPIO_PIN5);

	//P1.3 Camera 3 init request line from zedboard
	GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN3);

	//P1.4 Camera 1 init request line from zedboard
	GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN4);
}


void uart_init(void){



    //P3.4,5 = USCI_A0 TXD/RXD
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P3,
        GPIO_PIN3 + GPIO_PIN4
        );

    //Baudrate = 115200, clock freq = 24MHz
    //UCBRx = 208, UCBRFx = 0, UCBRSx = 2, UCOS16 = 0
    USCI_A_UART_initParam param = {0};
    param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 109;
    param.firstModReg = 0;
    param.secondModReg = 2;
    param.parity = USCI_A_UART_NO_PARITY;
    param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    param.uartMode = USCI_A_UART_MODE;
    param.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    if(STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &param))
    {
        return;
    }

    //Enable UART module for operation
    USCI_A_UART_enable(USCI_A0_BASE);

    //Enable Receive Interrupt
    USCI_A_UART_clearInterrupt(USCI_A0_BASE,
                               USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE,
                                USCI_A_UART_RECEIVE_INTERRUPT);

    __enable_interrupt();
}

//******************************************************************************
//
//This is the USCI_A0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV,4))
    {
    //Vector 2 - RXIFG
    case 2:
        receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
        if(!(receivedData == transmitData))                       // Check value
        {
            while(1)
            {
                ;
            }
        }
        check = 1;
        break;
    default: break;
    }
}

