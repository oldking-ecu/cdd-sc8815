/**
 * @file sc8815.c
 * @author wwyyy (1046685883@qq.com)
 * @brief
 * @version 1.0
 * @date 2026-03-24
 *
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 *
 */
#include "sc8815.h"
#include "string.h"
#include "public.h"

MODULE_INF_TYPE(UNAME)
MODULE_INF(UNAME);

#ifdef MINIMUM_RAM__
	#define GCFG(Member) (MODULE_INF(UNAME).CfgPtr->Member)
#else
	#define GCFG(Member) (MODULE_INF(UNAME).Config.Member)
#endif
#define INSCFG(Member) ((GCFG(InsCfgPtr) + ins)->Member)
#define INSINF(Member) ((GCFG(InsInfPtr) + ins)->Member)

#define VBUS_RU 68
#define VBUS_RD 1200
#define RBUS    5
#define RBAT    5

#define SC8815_ADDR 0xE8

#define SCREG_VBAT_SET       0x00
#define SCREG_VBUSREF_I_SET  0x01
#define SCREG_VBUSREF_I_SET2 0x02
#define SCREG_VBUSREF_E_SET  0x03
#define SCREG_VBUSREF_E_SET2 0x04
#define SCREG_IBUS_LIM_SET   0x05
#define SCREG_IBAT_LIM_SET   0x06
#define SCREG_VINREG_SET     0x07
#define SCREG_RATIO          0x08
#define SCREG_CTRL0_SET      0x09
#define SCREG_CTRL1_SET      0x0A
#define SCREG_CTRL2_SET      0x0B
#define SCREG_CTRL3_SET      0x0C
#define SCREG_VBUS_FB_VALUE  0x0D
#define SCREG_VBUS_FB_VALUE2 0x0E
#define SCREG_VBAT_FB_VALUE  0x0F
#define SCREG_VBAT_FB_VALUE2 0x10
#define SCREG_IBUS_VALUE     0x11
#define SCREG_IBUS_VALUE2    0x12
#define SCREG_IBAT_VALUE     0x13
#define SCREG_IBAT_VALUE2    0x14
#define SCREG_ADIN_VALUE     0x15
#define SCREG_ADIN_VALUE2    0x16
#define SCREG_STATUS         0x17
#define SCREG_MASK           0x19

/**
 * @brief
 *
 * @param ins:
 * @param addr:
 * @param wdata:
 * @param size:
 * @return uint8:
 */
static uint8 Sc8115_Write(uint8 ins, uint8 addr, uint8 *wdata, uint8 size)
{
	uint8 i;
	GCFG(I2cStartFuncPtr)
	(INSCFG(I2cInsId));
	if (!GCFG(I2cWrDatChkAckFuncPtr)(INSCFG(I2cInsId), SC8815_ADDR)) {
		GCFG(I2cStopFuncPtr)
		(INSCFG(I2cInsId));
		return 1;
	}
	if (!GCFG(I2cWrDatChkAckFuncPtr)(INSCFG(I2cInsId), addr)) {
		GCFG(I2cStopFuncPtr)
		(INSCFG(I2cInsId));
		return 1;
	}
	for (i = 0; i < size; i++) {
		if (!GCFG(I2cWrDatChkAckFuncPtr)(INSCFG(I2cInsId), wdata[i])) {
			GCFG(I2cStopFuncPtr)
			(INSCFG(I2cInsId));
			return 1;
		}
	}
	GCFG(I2cStopFuncPtr)
	(INSCFG(I2cInsId));
	return 0;
}

/**
 * @brief
 *
 * @param ins:
 * @param addr:
 * @param rdata:
 * @param size:
 * @return uint8:
 */
static uint8 Sc8115_Read(uint8 ins, uint8 addr, uint8 *rdata, uint8 size)
{
	uint8 i;
	GCFG(I2cStartFuncPtr)
	(INSCFG(I2cInsId));
	if (!GCFG(I2cWrDatChkAckFuncPtr)(INSCFG(I2cInsId), SC8815_ADDR)) {
		GCFG(I2cStopFuncPtr)
		(INSCFG(I2cInsId));
		return 1;
	}
	if (!GCFG(I2cWrDatChkAckFuncPtr)(INSCFG(I2cInsId), addr)) {
		GCFG(I2cStopFuncPtr)
		(INSCFG(I2cInsId));
		return 1;
	}
	GCFG(I2cStartFuncPtr)
	(INSCFG(I2cInsId));
	if (!GCFG(I2cWrDatChkAckFuncPtr)(INSCFG(I2cInsId), SC8815_ADDR | 0x01)) {
		GCFG(I2cStopFuncPtr)
		(INSCFG(I2cInsId));
		return 1;
	}
	for (i = 0; i < size - 1; i++) {
		rdata[i] = GCFG(I2cRdDatSendAckFuncPtr)(INSCFG(I2cInsId), TRUE);
	}
	rdata[i] = GCFG(I2cRdDatSendAckFuncPtr)(INSCFG(I2cInsId), FALS);
	GCFG(I2cStopFuncPtr)
	(INSCFG(I2cInsId));
	return 0;
}

/**
 * @brief
 *
 */
void MODULE_INIT_FUN(UNAME)(const MODULE_CFG_TYPE(UNAME) * cfgPtr)
{
	uint8 ins;
	uint8 databuf[10];

	memset(&MODULE_INF(UNAME), 0, sizeof(MODULE_INF_TYPE(UNAME)));
	if (NULL == cfgPtr)
		return;
#ifdef MINIMUM_RAM__
	MODULE_INF(UNAME).CfgPtr = cfgPtr;
#else
	memcpy((void *)&MODULE_INF(UNAME).Config, cfgPtr, sizeof(MODULE_CFG_TYPE(UNAME)));
#endif
	memset((void *)GCFG(InsInfPtr), 0, sizeof(MODULE_INS_INF_TYPE(UNAME)) * GCFG(InsNum));
	for (ins = 0; ins < GCFG(InsNum); ins++) {
		INSINF(InitOk) = FALS;
		databuf[0] = 0x21;    // VBAT_SEL=1 VCELL_SET=1(fix1.2) VBAT=1.2*(1+220K/13K)=21.5V
		if (Sc8115_Write(ins, SCREG_VBAT_SET, databuf, 1)) {
			continue;
		}
		databuf[0] = 0x34;    // IBAT_RATIO=1 IBUS_RATIO=1 VBAT_MON_RATIO=0 VBUS_RATIO=0
		if (Sc8115_Write(ins, SCREG_RATIO, databuf, 1)) {
			continue;
		}
		databuf[0] = 0x84;    // IBAT_RATIO=1 FREQ_SET=1
		if (Sc8115_Write(ins, SCREG_CTRL0_SET, databuf, 1)) {
			continue;
		}
		databuf[0] = 0x11;    // FB_SEL=1
		if (Sc8115_Write(ins, SCREG_CTRL1_SET, databuf, 1)) {
			continue;
		}
		databuf[0] = 0x01;    // EN_DITHER=0
		if (Sc8115_Write(ins, SCREG_CTRL2_SET, databuf, 1)) {
			continue;
		}
		databuf[0] = 0x24;    // AD_START=1 DIS_ShortFoldBack=1
		if (Sc8115_Write(ins, SCREG_CTRL3_SET, databuf, 1)) {
			continue;
		}
		databuf[0] = 0xEE;    // AC_OK_Mask=1 INDET_Mask=1 VBUS_SHORT_Mask=1 OTP_Mask=1 EOC_Mask=1
		if (Sc8115_Write(ins, SCREG_MASK, databuf, 1)) {
			continue;
		}
		// set VBUS
		databuf[0] = 0xEE;    // AC_OK_Mask=1 INDET_Mask=1 VBUS_SHORT_Mask=1 OTP_Mask=1 EOC_Mask=1
		if (Sc8115_Write(ins, SCREG_MASK, databuf, 1)) {
			continue;
		}
		INSINF(InitOk) = TRUE;
	}
}

/**
 * @brief Construct a new module ins inf type object
 *
 */
MODULE_INS_INF_TYPE(UNAME) * MODULE_GET_INSINF_PTR_FUN(UNAME)(uint8 ins)
{
	if (NULL == GCFG(InsInfPtr) || ins > GCFG(InsNum))
		return NULL;
	return GCFG(InsInfPtr) + ins;
}

/**
 * @brief
 *
 */
const MODULE_INS_CFG_TYPE(UNAME) * MODULE_GET_INSCFG_PTR_FUN(UNAME)(uint8 ins)
{
	if (NULL == GCFG(InsCfgPtr) || ins > GCFG(InsNum))
		return NULL;
	return GCFG(InsCfgPtr) + ins;
}

/**
 * @brief
 *
 */
void MODULE_MAIN_FUN(UNAME)(void)
{
	uint8 ins;
	uint8 databuf[12];
	uint16 tmp;

	for (ins = 0; ins < GCFG(InsNum); ins++) {
		if (!INSINF(InitOk)) {
			continue;
		}
		if (INSINF(VbusSetReq)) {    // set VBUS
			INSINF(VbusSetReq) = FALS;
			tmp = (uint32)INSINF(VbusSetValue) * VBUS_RU / (VBUS_RD + VBUS_RU) / 2;
			if (tmp > 1024)
				tmp = 1024;
			if (tmp > 0)
				tmp -= 1;
			databuf[0] = (tmp >> 2) & 0xFF;
			databuf[1] = (tmp << 6) & 0xFF;
			if (Sc8115_Write(ins, SCREG_VBUSREF_E_SET, databuf, 2)) {
				continue;
			}
		}
		if (INSINF(IbusSetReq)) {    // set IBUS
			INSINF(IbusSetReq) = FALS;
			tmp = (uint32)INSINF(IbusSetValue) * RBUS * 256 / 10 / 6000;
			if (tmp > 256)
				tmp = 256;
			if (tmp > 0)
				tmp -= 1;
			databuf[0] = tmp & 0xFF;
			if (Sc8115_Write(ins, SCREG_IBUS_LIM_SET, databuf, 1)) {
				continue;
			}
		}
		if (Sc8115_Read(ins, SCREG_VBUS_FB_VALUE, databuf, 11)) {
			continue;
		}
		tmp = (uint16)databuf[0] << 2 | ((databuf[1] >> 6) & 0x3) + 1;
		INSINF(VbusFb) = (uint32)tmp * 125 * 2 / 10;
		tmp = (uint16)databuf[2] << 2 | ((databuf[3] >> 6) & 0x3) + 1;
		INSINF(VbatFb) = (uint32)tmp * 125 * 2 / 10;
		tmp = (uint16)databuf[4] << 2 | ((databuf[5] >> 6) & 0x3) + 1;
		INSINF(IbusFb) = (uint32)tmp * 2 * 6000 * 10 / RBUS / 1200;
		tmp = (uint16)databuf[6] << 2 | ((databuf[7] >> 6) & 0x3) + 1;
		INSINF(IbatFb) = (uint32)tmp * 2 * 12000 * 10 / RBAT / 1200;
		tmp = (uint16)databuf[8] << 2 | ((databuf[9] >> 6) & 0x3) + 1;
		INSINF(Adin) = (uint32)tmp * 2;
		INSINF(ChipFlag) = databuf[10] & 0x6E;
		UpdateRollCount16(&INSINF(UpdateRc));
		;
	}
}

/**
 * @brief
 *
 * @param busVolt:
 * @param busCurrent:
 */
void Sc8115_SetVbus_Ibus(uint16 busVolt, uint16 busCurrent)
{
	uint8 ins = 0;
	INSINF(VbusSetValue) = busVolt;
	INSINF(VbusSetReq) = TRUE;
	INSINF(IbusSetValue) = busCurrent;
	INSINF(IbusSetReq) = TRUE;
}
