/*
 * system_init.h
 *
 *  Created on: Mar 27, 2016
 *      Author: longs
 */

#ifndef OV2640_APP_SYSTEM_INIT_H_
#define OV2640_APP_SYSTEM_INIT_H_

#include "driverlib.h"


/*****************************************************************************
 *Desired Timeout for XT1 initialization
*****************************************************************************/
#define UCS_XT1_TIMEOUT 50000

/*****************************************************************************
 *Target frequency for MCLK in kHz
*****************************************************************************/
#define MCLK_FREQ_KHZ   12000


/*****************************************************************************
 *MCLK/FLLRef Ratio
*****************************************************************************/
#define UCS_MCLK_FLLREF_RATIO   366

/*****************************************************************************
 *External Crystal Frequenies
*****************************************************************************/
#define	UCS_XT1_CRYSTAL_FREQUENCY 32768
#define UCS_XT2_CRYSTAL_FREQUENCY 0





void clock_init(void);
void gpio_init(void);
void uart_init(void);




#endif /* OV2640_APP_SYSTEM_INIT_H_ */
