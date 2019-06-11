/*
 * BSD LICENSE
 *
 * Copyright(c) 2014-2018 Intel Corporation. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * @brief Internal defines and data structure definition
 */

#ifndef __CPU_REGISTERS_H__
#define __CPU_REGISTERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocation & Monitoring association MSR register
 * - bits [63..32] QE COS
 * - bits [31..10] Reserved
 * - bits [9..0] RMID
 */
#define PQOS_MSR_ASSOC             0xC8F
#define PQOS_MSR_ASSOC_QECOS_SHIFT 32
#define PQOS_MSR_ASSOC_QECOS_MASK  0xffffffff00000000ULL
#define PQOS_MSR_ASSOC_RMID_MASK   ((1ULL << 10) - 1ULL)

/*
 * This is a global structure to store all the vendor defines
 */
extern struct pqos_vendor_config v_def;

/**
 * Allocation class of service (COS) MSR registers
 */
#define PQOS_MSR_L3CA_MASK_START 0xC90
#define PQOS_MSR_L3CA_MASK_END   0xD0F
#define PQOS_MSR_L3CA_MASK_NUMOF				\
	(PQOS_MSR_L3CA_MASK_END - PQOS_MSR_L3CA_MASK_START + 1)

#define PQOS_MSR_L2CA_MASK_START 0xD10
#define PQOS_MSR_MBA_MASK_START  0xD50

#define PQOS_MSR_L3_QOS_CFG          0xC81   /**< L3 CAT config register */
#define PQOS_MSR_L3_QOS_CFG_CDP_EN   1ULL    /**< L3 CDP enable bit */

#define PQOS_MSR_L2_QOS_CFG          0xC82   /**< L2 CAT config register */
#define PQOS_MSR_L2_QOS_CFG_CDP_EN   1ULL    /**< L2 CDP enable bit */

/**
 * MBA linear max value
 */
#define PQOS_MBA_LINEAR_MAX 100

/**
 * Available types of allocation resource ID's.
 * (matches CPUID enumeration)
 */
#define PQOS_RES_ID_L3_ALLOCATION    1       /**< L3 cache allocation */
#define PQOS_RES_ID_L2_ALLOCATION    2       /**< L2 cache allocation */
#define PQOS_RES_ID_MB_ALLOCATION    3       /**< Memory BW allocation */

#define PQOS_CPUID_CAT_CDP_BIT       2       /**< CDP supported bit */

/**
 * Monitoring data read MSR register
 */
#define PQOS_MSR_MON_QMC             0xC8E
#define PQOS_MSR_MON_QMC_DATA_MASK   ((1ULL << 62) - 1ULL)
#define PQOS_MSR_MON_QMC_ERROR       (1ULL << 63)
#define PQOS_MSR_MON_QMC_UNAVAILABLE (1ULL << 62)

/**
 * Monitoring event selection MSR register
 * - bits [63..42] Reserved
 * - bits [41..32] RMID
 * - bits [31..8] Reserved
 * - bits [7..0] Event ID
 */
#define PQOS_MSR_MON_EVTSEL            0xC8D
#define PQOS_MSR_MON_EVTSEL_RMID_SHIFT 32
#define PQOS_MSR_MON_EVTSEL_RMID_MASK  ((1ULL << 10) - 1ULL)
#define PQOS_MSR_MON_EVTSEL_EVTID_MASK ((1ULL << 8) - 1ULL)

/**
 * PQoS vendor value and function pointers
 * @param cpuid_cache_leaf	: Cache mask leaf
 * @param default_mba		: default memory bandwidth
 * @param mba_msr_reg		: MBA mask base register
 * @param hw_mba_get		: Get MBA mask
 * @param hw_mba_set		: Set MBA mask with MSR method
 * @param os_mba_get		: Get MBA mask with OS method
 * @param os_mba_set		: Set MBA mask with OS method
 */
struct pqos_vendor_config {
	int cpuid_cache_leaf;
	unsigned mba_max;
	uint32_t mba_msr_reg;
	int (*hw_mba_get)(const unsigned socket, const unsigned max_num_cos,
			  unsigned *num_cos, struct pqos_mba *mba_tab);
	int (*hw_mba_set)(const unsigned socket, const unsigned num_cos,
			  const struct pqos_mba *requested,
			  struct pqos_mba *actual);
	int (*os_mba_get)(const unsigned socket, const unsigned max_num_cos,
			  unsigned *num_cos, struct pqos_mba *mba_tab);
	int (*os_mba_set)(const unsigned socket, const unsigned num_cos,
			  const struct pqos_mba *requested,
			  struct pqos_mba *actual);
};

/**
 * @brief initializes intel/amd vendor functions
 *
 * @param vendor configuration structures
 * @return Operation status
 * @retval Success returns 0
 */
int init_functions(struct pqos_vendor_config *ptr);

/**
 * MSR's to read instructions retired, unhalted cycles,
 * LLC references and LLC misses.
 * These MSR's are needed to calculate IPC (instructions per clock) and
 * LLC miss ratio.
 */
#define IA32_MSR_INST_RETIRED_ANY     0x309
#define IA32_MSR_CPU_UNHALTED_THREAD  0x30A
#define IA32_MSR_FIXED_CTR_CTRL       0x38D
#define IA32_MSR_PERF_GLOBAL_CTRL     0x38F
#define IA32_MSR_PMC0                 0x0C1
#define IA32_MSR_PERFEVTSEL0          0x186

#define IA32_EVENT_LLC_MISS_MASK      0x2EULL
#define IA32_EVENT_LLC_MISS_UMASK     0x41ULL

#ifdef __cplusplus
}
#endif

#endif /* __CPU_REGISTERS_H__ */
