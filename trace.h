/*---------------------------------------------------------------------------------------------------
 *  Copyright (c) 2023 Tomasz Jastrzębski. All rights reserved.
 *-------------------------------------------------------------------------------------------------*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#if defined(TRACE_LEVEL) && TRACE_LEVEL > 0
// see: https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#define trace(level, format_msg, ...) _trace((level), (format_msg)__VA_OPT__(, ) __VA_ARGS__)
#else
#define trace(level, format_msg, ...)
#endif

#define GET_REG_FLAGX(reg, field) ((((reg)&field) == (field)) ? "X " : "  ")
#define GET_REG_FIELD(reg, field) (((reg)&field) >> field##_Pos)
#define SET_REG_FIELD(reg, field, value) reg = ((reg) & ~field) | (((value) << field##_Pos) & field)
#define GET_REG_FIELD_BIT(reg, field, bit) (((((reg)&field) >> field##_Pos) & (0x1 << bit)) >> bit)

// #define GET_REG_FIELD_2BITS(reg, fld) (GET_REG_FIELD_BIT(reg, fld, 1) * 10 + GET_REG_FIELD_BIT(reg, fld, 0))
// #define GET_REG_FIELD_3BITS(reg, fld) (GET_REG_FIELD_BIT(reg, fld, 2) * 100 + GET_REG_FIELD_BIT(reg, fld, 1) * 10 + GET_REG_FIELD_BIT(reg, fld, 0))
// #define GET_REG_FIELD_4BITS(reg, fld) (GET_REG_FIELD_BIT(reg, fld, 3) * 1000 + GET_REG_FIELD_BIT(reg, fld, 2) * 100 + GET_REG_FIELD_BIT(reg, fld, 1) * 10 + GET_REG_FIELD_BIT(reg, fld, 0))

void trace_HCD(uint8_t level);
void trace_UCPD(uint8_t level);
void trace_UCPD_ISR(uint8_t level);

#ifdef __cplusplus
}
#endif
