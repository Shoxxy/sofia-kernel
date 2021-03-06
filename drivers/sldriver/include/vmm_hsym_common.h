/*
 * Copyright (c) 2015, Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef _VMM_HSYM_COMMON_H_
#define _VMM_HSYM_COMMON_H_
#include <linux/types.h>

/*-----------------------------------------------------*/
/* NOTE                                                */
/*                                                     */
/* This head should be the same as bp_common.h         */
/* in hypersim - these are shared data structures      */
/*-----------------------------------------------------*/

#define MAX_SECTIONS 16
#define MAX_ENTRYS   16
#define MAX_REGIONS  16

#define MAJOR_VERSION 1
#define MINOR_VERSION 0
enum sl_error_t {
	SL_SUCCESS = 0,
	SL_EVERSION_MISMATCH = -1,
	SL_EUNKNOWN = -2,
};

enum sl_patch_type_t {
	PATCH_TYPE_SECS_PTR = 0,
	PATCH_TYPE_CORE_ID,
	PATCH_TYPE_CORE_STATE_PTR,
	PATCH_TYPE_SECS_SCV,
	PATCH_TYPE_REGION_ADDR,
	PATCH_TYPE_IDT_VECTOR,
	PATCH_TYPE_ENTER_PAGE,
	PATCH_TYPE_EXIT_PAGE,
	PATCH_TYPE_SECS_SCV_UN
};

typedef struct {
	uint32_t major_version;
	uint32_t minor_version;
	uint32_t vendor_id[3];
	uint32_t cpuid_leaf_max;
	uint32_t hip_max_view;
} __packed sl_info_t;

typedef struct {
	uint32_t section_type;	/*tbd should be enum*/
	uint64_t start_gva;
	uint64_t end_gva;
	uint32_t entry_point[MAX_ENTRYS];
} __packed hsec_section_t;

typedef struct {
	uint32_t num_sections;
	hsec_section_t sections[MAX_SECTIONS];
	 uint32_t response; /*out*/
	 uint32_t view_handle; /*out*/
} __packed hsec_register_t;

typedef struct {
	/* from the base of the region.*/
	uint32_t offset;
	/* value to be patched - 0xFFFFFFFF is special value*/
	uint64_t val;
	/*used by hypervisor to decide what to patch (think of scv in secs)*/
	/*classification of the 'val'- e.g. cpuindex, secs_ptr etc.*/
	uint8_t type;
} __packed hsec_patch_info_t;

typedef struct {
	uint64_t start_gva;
	uint32_t n_pages;
	/* This is a pointer but keeping it as 64 bit as hypersim is compiled in 64 bit mode.*/
	uint64_t patch_info;
	uint32_t n_patches;
	uint32_t n_bytes;
} __packed hsec_region_t;

typedef struct {
	uint32_t n_regions;
	hsec_region_t region[MAX_REGIONS];
} __packed hsec_map_t;

/* tbd: The structure will be expanded*/
typedef struct hsec_vIDT_param {
	uint16_t vIDT_limit;
	uint64_t vIDT_base;
	uint32_t cpu;
	hsec_map_t map;
	/*# of trusted stack pages*/
	uint16_t r0stack_num_pages;
	/*note - tos of stack*/
	uint64_t r0stack_gva_tos;
	/*trampoline page to copy reg state from untrusted stack to trusted stack*/
	uint64_t r0_enter_page;
	/*trampoline page to copy reg state from trusted stack to untrusted stack*/
	uint64_t r0_exit_page;
	/*VMM usable field, saves the trusted tss host page*/
	uint64_t tss_remap_hpa;
	uint64_t enter_eresume_code;
	uint64_t exit_code;
	uint64_t async_exit_code;
} __packed hsec_vIDT_param_t;

typedef struct {
	uint64_t secs_gva;
} __packed hsec_sl_param_t;

#endif
