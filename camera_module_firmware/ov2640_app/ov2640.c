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

	SCCB_Write(usci_num, BANK_SEL, BANK_SEL_SENS, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, COM7, COM7_SRST, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BANK_SEL, BANK_SEL_DSP , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x2c,   0xff , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x2e,   0xdf , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BANK_SEL, BANK_SEL_SENS , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x3c,   0x32 , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, CLKRC, CLKRC_DIV_SET(1) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, COM2, COM2_OCAP_Nx_SET(3) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, REG04, REG04_DEF | REG04_HREF_EN , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, COM8,  COM8_DEF | COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, COM9, COM9_AGC_GAIN_8x | 0x08, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x2c,   0x0c , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x33,   0x78 , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x3a,   0x33, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x3b,   0xfb, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x3e,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x43,   0x11, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x16,   0x10, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x39,   0x02, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x35,   0x88, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x22,   0x0a, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x37,   0x40, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x23,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ARCOM2, 0xa0, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x06,   0x02, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x06,   0x88, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x07,   0xc0, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x0d,   0xb7, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x0e,   0x01, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x4c,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x4a,   0x81, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x21,   0x99, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, AEW,    0x40, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, AEB,    0x38, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, VV,     VV_HIGH_TH_SET(0x08) | VV_LOW_TH_SET(0x02), 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x5c,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x63,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, FLL,    0x22, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, COM3,   0x38 | COM3_BAND_AUTO, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, REG5D,  0x55, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, REG5E,  0x7d, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, REG5F,  0x7d, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, REG60,  0x55, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, HISTO_LOW,   0x70, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, HISTO_HIGH,  0x80, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x7c,   0x05, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x20,   0x80, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x28,   0x30, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x6c,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x6d,   0x80, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x6e,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x70,   0x02, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x71,   0x94, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x73,   0xc1, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x3d,   0x34, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, COM7, COM7_RES_UXGA | COM7_ZOOM_EN, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x5a,   0x57, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BD50,   0xbb, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BD60,   0x9c, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BANK_SEL, BANK_SEL_DSP, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xe5,   0x7f, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, MC_BIST, MC_BIST_RESET | MC_BIST_BOOT_ROM_SEL, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x41,   0x24, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, RESET, RESET_JPEG | RESET_DVP, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x76,   0xff, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x33,   0xa0, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x42,   0x20, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x43,   0x18, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x4c,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, CTRL3, CTRL3_BPC_EN | CTRL3_WPC_EN | 0x10, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x88,   0x3f, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xd7,   0x03, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xd9,   0x10, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, R_DVP_SP , R_DVP_SP_AUTO_MODE | 0x2, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xc8,   0x08, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xc9,   0x80, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPADDR, 0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPDATA, 0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPADDR, 0x03, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPDATA, 0x48, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPDATA, 0x48, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPADDR, 0x08, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPDATA, 0x20, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPDATA, 0x10, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BPDATA, 0x0e, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x90,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x0e, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x1a, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x31, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x5a, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x69, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x75, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x7e, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x88, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x8f, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x96, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0xa3, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0xaf, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0xc4, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0xd7, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0xe8, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x91,   0x20, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x92,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x06, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0xe3, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x03, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x03, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x02, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x93,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x96,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x08, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x19, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x02, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x0c, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x24, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x30, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x28, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x26, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x02, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x98, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x80, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x97,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa4,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa8,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xc5,   0x11, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xc6,   0x51, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xbf,   0x80, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xc7,   0x10, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb6,   0x66, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb8,   0xA5, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb7,   0x64, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb9,   0x7C, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb3,   0xaf, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb4,   0x97, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb5,   0xFF, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb0,   0xC5, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb1,   0x94, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xb2,   0x0f, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xc4,   0x5c, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa6,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x20, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0xd8, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x1b, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x31, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x18, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x20, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0xd8, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x19, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x31, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x18, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x20, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0xd8, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x19, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x31, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xa7,   0x18, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0x7f,   0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xe5,   0x1f, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xe1,   0x77, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xdd,   0x7f, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, CTRL0,  CTRL0_YUV422 | CTRL0_YUV_EN | CTRL0_RGB_EN , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BANK_SEL, BANK_SEL_DSP , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, RESET, RESET_DVP , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, HSIZE8, HSIZE8_SET(W_UXGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, VSIZE8, VSIZE8_SET(H_UXGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, CTRL2, CTRL2_DCW_EN | CTRL2_SDE_EN | CTRL2_UV_AVG_EN | CTRL2_CMX_EN | CTRL2_UV_ADJ_EN , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, HSIZE, HSIZE_SET(W_UXGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, VSIZE, VSIZE_SET(H_UXGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, XOFFL, XOFFL_SET(0) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, YOFFL, YOFFL_SET(0) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, VHYX, VHYX_HSIZE_SET(W_UXGA) | VHYX_VSIZE_SET(H_UXGA) | VHYX_XOFF_SET(0) | VHYX_YOFF_SET(0), 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, TEST, TEST_HSIZE_SET(W_UXGA) , 2);
	__delay_cycles(sccb_delay);

	//320x240
/*
	SCCB_Write(usci_num, CTRLI, CTRLI_LP_DP | CTRLI_V_DIV_SET(2) |  CTRLI_H_DIV_SET(2), 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMOW, ZMOW_OUTW_SET(W_QVGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMOH, ZMOH_OUTH_SET(H_QVGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMHH, ZMHH_OUTW_SET(W_QVGA) | ZMHH_OUTH_SET(H_QVGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, R_DVP_SP, 4 , 2);
	__delay_cycles(sccb_delay);
*/
	//VGA
/*	SCCB_Write(usci_num, CTRLI, CTRLI_LP_DP | CTRLI_V_DIV_SET(0) |  CTRLI_H_DIV_SET(0), 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMOW, ZMOW_OUTW_SET(W_VGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMOH, ZMOH_OUTH_SET(H_VGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMHH, ZMHH_OUTW_SET(W_VGA) | ZMHH_OUTH_SET(H_VGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, R_DVP_SP, 2 , 2);
	__delay_cycles(sccb_delay);
*/


	//1600x1200
	SCCB_Write(usci_num, CTRLI, CTRLI_LP_DP | CTRLI_V_DIV_SET(0) |  CTRLI_H_DIV_SET(0), 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMOW, ZMOW_OUTW_SET(W_UXGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMOH, ZMOH_OUTH_SET(H_UXGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, ZMHH, ZMHH_OUTW_SET(W_UXGA) | ZMHH_OUTH_SET(H_UXGA) , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, R_DVP_SP, 0 , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, CTRLI, 0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, R_DVP_SP, 0x00 | R_DVP_SP_AUTO_MODE, 2);
	__delay_cycles(sccb_delay);



	SCCB_Write(usci_num, RESET, 0x00, 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, BANK_SEL, BANK_SEL_DSP , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, R_BYPASS, R_BYPASS_USE_DSP , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, IMAGE_MODE, IMAGE_MODE_LBYTE_FIRST | IMAGE_MODE_RGB565 , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, 0xd7, 0x03 , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, RESET,  0x00 , 2);
	__delay_cycles(sccb_delay);
	SCCB_Write(usci_num, R_BYPASS, R_BYPASS_USE_DSP, 2);
	__delay_cycles(sccb_delay);

/*********************************************
 * Light Modes
 ********************************************/

	//Auto
	SCCB_Write(usci_num, 0xff, 0x00, 2);
	SCCB_Write(usci_num, 0xc7, 0x00, 2);


/*
	//Office
	SCCB_Write(usci_num, 0xff, 0x00, 2);
	SCCB_Write(usci_num, 0xc7, 0x40, 2);
	SCCB_Write(usci_num, 0xcc, 0x52, 2);
	SCCB_Write(usci_num, 0xcd, 0x41, 2);
	SCCB_Write(usci_num, 0xce, 0x66, 2);


	//Home
	SCCB_Write(usci_num, 0xff, 0x00, 2);
	SCCB_Write(usci_num, 0xc7, 0x40, 2);
	SCCB_Write(usci_num, 0xcc, 0x42, 2);
	SCCB_Write(usci_num, 0xcd, 0x3f, 2);
	SCCB_Write(usci_num, 0xce, 0x71, 2);

*/


/*********************************************
 * Color Modes
 ********************************************/

/*
	//Black and White
	SCCB_Write(usci_num, 0xff, 0x00, 2);
	SCCB_Write(usci_num, 0x7c, 0x00, 2);
	SCCB_Write(usci_num, 0x7d, 0x18, 2);
	SCCB_Write(usci_num, 0x7c, 0x05, 2);
	SCCB_Write(usci_num, 0x7d, 0x80, 2);
	SCCB_Write(usci_num, 0x7d, 0x80, 2);

	//Antique
	SCCB_Write(usci_num, 0xff, 0x00, 2);
	SCCB_Write(usci_num, 0x7c, 0x00, 2);
	SCCB_Write(usci_num, 0x7d, 0x18, 2);
	SCCB_Write(usci_num, 0x7c, 0x05, 2);
	SCCB_Write(usci_num, 0x7d, 0x40, 2);
	SCCB_Write(usci_num, 0x7d, 0xa6, 2);
*/

}
