/*---------------------------------------------------------------------------------------------------
 *  Copyright (c) 2023 Tomasz Jastrzębski. All rights reserved.
 *-------------------------------------------------------------------------------------------------*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define GET_REG_FLAGX(reg, field) ((((reg)&field) == (field)) ? "X " : "  ")
#define GET_REG_FIELD(reg, field) (((reg)&field) >> field##_Pos)
#define SET_REG_FIELD(reg, field, value) reg = ((reg) & ~field) | (((value) << field##_Pos) & field)
#define GET_REG_FIELD_BIT(reg, field, bit) (((((reg)&field) >> field##_Pos) & (0x1 << bit)) >> bit)

void trace_HCD(uint8_t level);
void trace_UCPD(uint8_t level);
void trace_UCPD_ISR(uint8_t level);

#ifdef __cplusplus
}
#endif
