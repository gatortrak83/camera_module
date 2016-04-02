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
	const uint8_t (*regs1)[2];
	const uint8_t (*regs2)[2];
	const uint8_t (*regs3)[2];
	const uint8_t (*regs4)[2];
	const uint8_t (*regs5)[2];

	/* Reset all the registers */
	i = 0;
	regs1 = reset_seq;
	while (regs1[i][0] != 0xFF && regs1[i][1] != 0xFF) {
		SCCB_Write(usci_num, regs1[i][0], regs1[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	/* Write initial registers */
	i = 0;
	regs2 = ov2640_init_regs;
	while (regs2[i][0] != 0xFF && regs2[i][1] != 0xFF) {
		SCCB_Write(usci_num, regs2[i][0], regs2[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	/* Change image size */
	i = 0;
	regs3 = ov2640_size_change_preamble_regs;
	while (regs3[i][0] != 0xFF && regs3[i][1] != 0xFF) {
		SCCB_Write(usci_num, regs3[i][0], regs3[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	i = 0;
	regs4 = ov2640_qvga_regs;
	while (regs4[i][0] != 0xFF && regs4[i][1] != 0xFF) {
		SCCB_Write(usci_num, regs4[i][0], regs4[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}

	/* change image format */
	i = 0;
	regs5 = ov2640_format_change_preamble_regs;
	while (regs5[i][0] != 0xFF && regs5[i][1] != 0xFF) {
		SCCB_Write(usci_num, regs5[i][0], regs5[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;
	}


	/*
	i = 0;
	regs = ov2640_rgb565_le_regs;
	while (regs[i][0]) {
		SCCB_Write(usci_num, regs[i][0], regs[i][1], 2);
		__delay_cycles(sccb_delay);
		i++;

	}
*/

	SCCB_Write(usci_num, IMAGE_MODE, IMAGE_MODE_LBYTE_FIRST | IMAGE_MODE_RGB565, 2);
	SCCB_Write(usci_num, 0xd7, 0x03, 2);
	SCCB_Write(usci_num,  RESET,  0x00 , 2);
	SCCB_Write(usci_num, R_BYPASS, R_BYPASS_USE_DSP, 2);
	SCCB_Write(usci_num, 0xFF, 0xFF, 2);

}
