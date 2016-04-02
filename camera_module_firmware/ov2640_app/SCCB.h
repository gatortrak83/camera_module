/*
 * SCCB.h
 *
 *  Created on: Jul 24, 2015
 *      Author: Steven Long
 */

#ifndef SCCB_SCCB_H_
#define SCCB_SCCB_H_

#include "driverlib.h"

#define sccb_delay 10000

void sccb_init_master(uint8_t usci_num);
void SCCB_Write(uint8_t usci_num, unsigned char addr, unsigned char val,
		uint8_t len);
unsigned char sccb_read_single(uint8_t usci_num);
uint8_t SCCB_Read(uint8_t usci_num, unsigned char addr);

#endif /* SCCB_SCCB_H_ */
