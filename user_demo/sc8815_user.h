/**
 * @file sc8815_user.h
 * @author wwyyy (1046685883@qq.com)
 * @brief
 * @version 1.0
 * @date 2026-03-24
 *
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 *
 */
#ifndef SC8815_USER_H__
#define SC8815_USER_H__
#include "sc8815.h"

typedef enum {
	MODULE_ENUM_NAME(UNAME, ONE) = 0,
	MODULE_ENUM_NAME(UNAME, TOTAL_NUM)
} MODULE_ENUM_TYPE(UNAME);

void MODULE_USERINIT_FUN(UNAME)(void);


#endif    // SC8815_USER_H__
