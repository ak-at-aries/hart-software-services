/*******************************************************************************
 * Copyright 2019-2021 Microchip Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * MPFS HSS Embedded Software
 *
 */

/*!
 * \file DDR related utilities
 * \brief DDR utilities
 */

#include "config.h"
#include "hss_types.h"
#include "hss_state_machine.h"
#include "hss_debug.h"

#include "ssmb_ipi.h"

extern const uint64_t __ddr_start;
extern const uint64_t __ddr_end;

/////////////////

//
// We only want to specify the DDR location and size in one place - the linker script
//
// ddr_end is too high for the RV64 toolchain, sadly.  We get an error message concerning
// "relocation truncated to fit: R_RISCV_PCREL_HI20 against symbol `__ddr_end' defined in
//  .text.init section" as it is above 4GiB away from eNVM.
//
// See https://github.com/riscv/riscv-gnu-toolchain/issues/103 for background.
//
// So we can't easily do ...
//
//     extern uint64_t __ddr_end;
//     const ptrdiff_t ddr_size = (size_t)((char *)&__ddr_end - (char *)&__ddr_start);
//
// However, we can workaround by using the GNU assembler to store the DDR size into a 64-bit memory
// location and use this size in our C code
//
asm(".align 3\n"
    "__ddr_size: .quad (__ddr_end-__ddr_start)\n");
    //".globl   __ddr_size\n");
extern const size_t __ddr_size;

size_t HSS_DDR_GetSize(void)
{
    return __ddr_size;
}

void *HSS_DDR_GetStart(void)
{
    return (void *)&__ddr_start;
}

void HSS_DDR_Train(void)
{
    //mHSS_DEBUG_PRINTF("running DDR training on hart %u..." CRLF, current_hartid());
}

