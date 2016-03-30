/*
 * SCCB.c
 *
 *  Created on: Jul 24, 2015
 *      Author: Steven Long
 */
#include "SCCB.h"


#define SLAVE_ADDRESS 0x30

uint8_t transmit_data[10];
uint8_t received_data;
uint8_t send_len;
uint8_t receive_counter;
uint8_t transmit_counter;


void sccb_init_master(uint8_t usci_num){

	uint16_t i2c_base;


	if(usci_num == 0){
		i2c_base = USCI_B0_BASE;


		//Assign I2C pins to USCI_B0
		GPIO_setAsPeripheralModuleFunctionInputPin(
				GPIO_PORT_P3,
				GPIO_PIN0 + GPIO_PIN1
        	);
	}

	else if(usci_num == 1){
		i2c_base = USCI_B1_BASE;

		//Assign I2C pins to USCI_B1
		GPIO_setAsPeripheralModuleFunctionInputPin(
			GPIO_PORT_P4,
			GPIO_PIN1 + GPIO_PIN2
			);
	}


	USCI_B_I2C_initMasterParam param = {0};
	param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK;
	param.i2cClk = UCS_getSMCLK();
	param.dataRate = USCI_B_I2C_SET_DATA_RATE_100KBPS;
	USCI_B_I2C_initMaster(i2c_base, &param);

	//Set slave address
	USCI_B_I2C_setSlaveAddress(i2c_base, SLAVE_ADDRESS);


}


void SCCB_Write(uint8_t usci_num, unsigned char addr, unsigned char val, uint8_t len){

	uint16_t i2c_base;
	send_len = len;
	transmit_counter = 1;

	transmit_data[0] = addr;
	transmit_data[1] = val;


	if(usci_num == 0){
		i2c_base = USCI_B0_BASE;
	}

	else if(usci_num == 1){
		i2c_base = USCI_B1_BASE;
	}


	USCI_B_I2C_setMode(i2c_base, USCI_B_I2C_TRANSMIT_MODE);
	USCI_B_I2C_enable(i2c_base);

	USCI_B_I2C_clearInterrupt(i2c_base, USCI_B_I2C_TRANSMIT_INTERRUPT);
	USCI_B_I2C_enableInterrupt(i2c_base, USCI_B_I2C_TRANSMIT_INTERRUPT);

	//Delay between transaction
	__delay_cycles(sccb_delay);

	//Initiate start and send first character
	USCI_B_I2C_masterSendMultiByteStart(i2c_base, transmit_data[0]);

    //Enter LPM0 with interrupts enabled
    __bis_SR_register(GIE);
    __no_operation();


    //Delay until transmission completes
    while(USCI_B_I2C_isBusBusy(i2c_base))
    {
        ;
    }
}


unsigned char sccb_read_single(uint8_t usci_num){

	uint16_t i2c_base;


	if(usci_num == 0){
		i2c_base = USCI_B0_BASE;
	}

	else if(usci_num == 1){
		i2c_base = USCI_B1_BASE;
	}




   //Set receive mode
	USCI_B_I2C_setMode(i2c_base, USCI_B_I2C_RECEIVE_MODE);

    //Enable I2C Module to start operations
    USCI_B_I2C_enable(i2c_base);


    //Enable master Receive interrupt
    USCI_B_I2C_clearInterrupt(i2c_base, USCI_B_I2C_RECEIVE_INTERRUPT);
    USCI_B_I2C_enableInterrupt(i2c_base,USCI_B_I2C_RECEIVE_INTERRUPT);

	__delay_cycles(sccb_delay);

    //Initiate command to receive a single character from Slave
    USCI_B_I2C_masterReceiveSingleStart(i2c_base);

	__bis_SR_register(GIE);
	__no_operation();

	while(receive_counter);

	return received_data;
}



uint8_t SCCB_Read(uint8_t usci_num, unsigned char addr){

	uint8_t ret = 0;

	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, addr, 0xFF, 1);
	__delay_cycles(sccb_delay);
	ret = sccb_read_single(usci_num);
	__delay_cycles(sccb_delay);

	return ret;

}



//******************************************************************************
//
//This is the USCI_B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCI_B0_ISR(void)
{
	switch(__even_in_range(UCB0IV,12))
	{
	case USCI_I2C_UCTXIFG:
	{
		//Check TX byte counter
		if(transmit_counter < send_len)
		{
			//Initiate send of character from Master to Slave
			USCI_B_I2C_masterSendMultiByteNext(USCI_B0_BASE,
											   transmit_data[transmit_counter]
											   );

			//Increment TX byte counter
			transmit_counter++;
		}
		else
		{
			//Initiate stop only
			USCI_B_I2C_masterSendMultiByteStop(USCI_B0_BASE);

			//Clear master interrupt status
			USCI_B_I2C_clearInterrupt(USCI_B0_BASE,
									  USCI_B_I2C_TRANSMIT_INTERRUPT);

			//Exit LPM0 on interrupt return

		}
		break;
	}

    //Vector 10: Data received - RXIFG
    case USCI_I2C_UCRXIFG:
    {
        //Grab data from data register
        received_data = USCI_B_I2C_masterReceiveSingle(
            USCI_B0_BASE);

        //Exit low power mode 0 and disable GIE on interrupt exit
        __bic_SR_register_on_exit(GIE);
        __delay_cycles(sccb_delay);

        break;
    }

    default:    break;
    }
}

//******************************************************************************
//
//This is the USCI_B1 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B1_VECTOR)))
#endif
void USCI_B1_ISR(void)
{
	switch(__even_in_range(UCB1IV,12))
	{
	case USCI_I2C_UCTXIFG:
	{
		//Check TX byte counter
		if(transmit_counter < send_len)
		{
			//Initiate send of character from Master to Slave
			USCI_B_I2C_masterSendMultiByteNext(USCI_B1_BASE,
											   transmit_data[transmit_counter]
											   );

			//Increment TX byte counter
			transmit_counter++;
		}
		else
		{
			//Initiate stop only
			USCI_B_I2C_masterSendMultiByteStop(USCI_B1_BASE);

			//Clear master interrupt status
			USCI_B_I2C_clearInterrupt(USCI_B1_BASE,
									  USCI_B_I2C_TRANSMIT_INTERRUPT);

			//Exit LPM0 on interrupt return
			__bic_SR_register_on_exit(GIE);
		}
		break;
	}

    //Vector 10: Data received - RXIFG
    case USCI_I2C_UCRXIFG:
    {
        //Grab data from data register
        received_data = USCI_B_I2C_masterReceiveSingle(
            USCI_B1_BASE);

        //Exit low power mode 0 and disable GIE on interrupt exit
        __bic_SR_register_on_exit(GIE);
        __delay_cycles(1000);

        break;
    }

    default:    break;
    }
}



