/**
 * @file sc8815_user.c
 * @author wwyyy (1046685883@qq.com)
 * @brief
 * @version 1.0
 * @date 2026-03-24
 *
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 *
 */
#include "sc8815_user.h"
#include "i2c_gpio_user.h"
#include "mcu_hal_user.h"
#include "public.h"
#include "task.h"
#include "flash_nvm_user.h"

static const MODULE_INS_CFG_TYPE(UNAME) MODULE_INS_CFG(UNAME)[] = {
	{I2C_GPIO_INS_SC8815},
};

static MODULE_INS_INF_TYPE(UNAME) MODULE_INS_INF(UNAME)[ARRAY_SIZE(MODULE_INS_CFG(UNAME))];


void MODULE_USERINIT_FUN(UNAME)(void)
{
	MODULE_CFG_TYPE(UNAME)
	MODULE_CFG(UNAME);
	MODULE_CFG(UNAME).InsCfgPtr = MODULE_INS_CFG(UNAME);
	MODULE_CFG(UNAME).InsInfPtr = MODULE_INS_INF(UNAME);
	MODULE_CFG(UNAME).InsNum = ARRAY_SIZE(MODULE_INS_CFG(UNAME));
	MODULE_CFG(UNAME).I2cStartFuncPtr = I2cGpio_Start;
	MODULE_CFG(UNAME).I2cWrDatChkAckFuncPtr = I2cGpio_WrDatChkAck;
	MODULE_CFG(UNAME).I2cRdDatSendAckFuncPtr = I2cGpio_RdDatSendAck;
	MODULE_CFG(UNAME).I2cStopFuncPtr = I2cGpio_Stop;
	MODULE_INIT_FUN(UNAME)
	(&MODULE_CFG(UNAME));
	// Task_Creat(TASK_NOR, MODULE_MAIN_FUN(UNAME), 5);
	// Sc8115_SetVbus_Ibus(Param_Info.BusVolt, Param_Info.BusCurrent);
}
