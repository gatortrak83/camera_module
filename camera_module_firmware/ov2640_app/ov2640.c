/*
 * ov2640.h
 *
 *  Created on: 3, 12, 2015
 *      Author: Steven Long
 */

#include "driverlib.h"
#include "ov2640.h"

void OV2640_ReadID(OV2640_IDTypeDef* OV2640ID, uint8_t usci_num) {
	SCCB_Write(usci_num, 0xFF, 0x01, 2);
	OV2640ID->Manufacturer_ID1 = SCCB_Read(usci_num, MIDH_O);
	OV2640ID->Manufacturer_ID2 = SCCB_Read(usci_num, MIDL_O);
	OV2640ID->Version = SCCB_Read(usci_num, REG_VER_O);
	OV2640ID->PID_ID = SCCB_Read(usci_num, REG_PID_O);
}

int32_t OV2640_CheckID(OV2640_IDTypeDef* OV2640ID) {
	if (((OV2640ID->Manufacturer_ID2 << 8) | (OV2640ID->Manufacturer_ID1))
			!= OV2640_MANUFACTURER_ID_O) {
		return -1;
	}
	if (OV2640ID->PID_ID != OV2640_PID_O) {
		return -1;
	}
	if (OV2640ID->Version != OV2640_VERSION_O) {
		return -1;
	}
	return 0;
}

int32_t OV2640_Init(uint8_t usci_num) {

	OV2640_IDTypeDef OV2640ID;


	OV2640_ReadID(&OV2640ID, usci_num);

/*	if (OV2640_CheckID(&OV2640ID) != 0) {
		//Camera OV2640 not found
		while (1) {
			//Write toggle red led
		}
		return -1;
	}
*/
	OV2640_SetCameraRegisters(usci_num);

	return 0;
}

void OV2640_SetCameraRegisters(uint8_t usci_num) {
	uint32_t i = 0;
	const uint8_t (*regs)[2];

	/* Reset all the registers */
	i = 0;
	regs = reset_seq;
	while (regs[i][0]) {
		SCCB_Write(usci_num, regs[i][0], regs[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	/* Write initial registers */
	i = 0;
	regs = ov2640_init_regs;
	while (regs[i][0]) {
		SCCB_Write(usci_num, regs[i][0], regs[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	/* Change image size */
	i = 0;
	regs = ov2640_size_change_preamble_regs;
	while (regs[i][0]) {
		SCCB_Write(usci_num, regs[i][0], regs[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	i = 0;
	regs = ov2640_qvga_regs;
	while (regs[i][0]) {
		SCCB_Write(usci_num, regs[i][0], regs[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	/* change image format */
	i = 0;
	regs = ov2640_format_change_preamble_regs;
	while (regs[i][0]) {
		SCCB_Write(usci_num, regs[i][0], regs[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	i = 0;
	regs = ov2640_yuyv_regs;
	while (regs[i][0]) {
		SCCB_Write(usci_num, regs[i][0], regs[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
		;
	}
}
