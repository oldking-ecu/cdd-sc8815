/**
 * @file sc8815.h
 * @author wwyyy (1046685883@qq.com)
 * @brief
 * @version 1.0
 * @date 2026-03-24
 *
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 *
 */
#ifndef SC8815_H__
#define SC8815_H__
#include "type.h"
#include "code_file_std.h"
#include "mcu_hal.h"


/**
 * @brief 实例配置参数
 *
 */
typedef struct {
	uint8 I2cInsId;
} MODULE_INS_CFG_TYPE(UNAME);

/**
 * @brief 实例状体变量
 *
 */
typedef struct {
	uint8 InitOk;
	uint16 UpdateRc;
	uint16 VbusFb;    // 采样电压 mv
	uint16 VbatFb;    // 采样电压 mv
	uint16 IbusFb;    // 采样流 ma
	uint16 IbatFb;    // 采样流 ma
	uint16 Adin;      // 采样电压 mv
	uint8 ChipFlag;
	uint8 VbusSetReq;
	uint16 VbusSetValue;    // mv
	uint8 IbusSetReq;
	uint16 IbusSetValue;    // ma
} MODULE_INS_INF_TYPE(UNAME);

/**
 * @brief 通用配置参数
 *
 */
typedef struct {
	const MODULE_INS_CFG_TYPE(UNAME) * InsCfgPtr;
	MODULE_INS_INF_TYPE(UNAME) * InsInfPtr;
	uint8 InsNum;
	void (*I2cStartFuncPtr)(uint8 ins);
	uint8 (*I2cWrDatChkAckFuncPtr)(uint8 ins, uint8 wdata);
	uint8 (*I2cRdDatSendAckFuncPtr)(uint8 ins, uint8 ack);
	void (*I2cStopFuncPtr)(uint8 ins);
} MODULE_CFG_TYPE(UNAME);

/**
 * @brief 全部状态合集
 *
 */
typedef struct {
#ifdef MINIMUM_RAM__
	const MODULE_CFG_TYPE(UNAME) * CfgPtr;
#else
	const MODULE_CFG_TYPE(UNAME) Config;
#endif
} MODULE_INF_TYPE(UNAME);

/**
 * @brief 变量声明
 *
 */
extern MODULE_INF_TYPE(UNAME) MODULE_INF(UNAME);

/**
 * @brief 对外提供接口
 *
 */
void MODULE_INIT_FUN(UNAME)(const MODULE_CFG_TYPE(UNAME) * cfgPtr);
MODULE_INS_INF_TYPE(UNAME) * MODULE_GET_INSINF_PTR_FUN(UNAME)(uint8 ins);
const MODULE_INS_CFG_TYPE(UNAME) * MODULE_GET_INSCFG_PTR_FUN(UNAME)(uint8 ins);
void MODULE_MAIN_FUN(UNAME)(void);
void Sc8115_SetVbus_Ibus(uint16 busVolt, uint16 busCurrent);

#endif    // SC8815_H__
