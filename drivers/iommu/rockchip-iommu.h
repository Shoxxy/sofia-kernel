/*
 * Data structure definition for Rockchip IOMMU driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_PLAT_IOMMU_H
#define __ASM_PLAT_IOMMU_H

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/genalloc.h>
#include <linux/iommu.h>

#include <linux/rockchip_iovmm.h>

struct rockchip_iovmm {
	struct iommu_domain *domain; /* iommu domain for this iovmm */
	struct gen_pool *vmm_pool;
	struct list_head regions_list;	/* list of rockchip_vm_region */
	spinlock_t lock; /* lock for updating regions_list */
};

struct iommu_drvdata {
	struct list_head node; /* entry of rockchip_iommu_domain.clients */
	struct device *iommu;	/*  IOMMU's device descriptor */
	int num_res_mem;
	int num_res_irq;
	const char *dbgname;
	void __iomem **res_bases;
	int activations;
	spinlock_t data_lock; /*lock this structure*/
	struct iommu_domain *domain; /* domain given to iommu_attach_device() */
	unsigned long pgtable;
	struct rockchip_iovmm vmm;
};

#ifdef CONFIG_ROCKCHIP_IOVMM

#define IOVA_START 0x10000000
#define IOVM_SIZE (SZ_1G - SZ_4K) /* last 4K is for error values */

struct rockchip_vm_region {
	struct list_head node;
	dma_addr_t start;
	size_t size;
};

static inline struct rockchip_iovmm *rockchip_get_iovmm(struct device *dev)
{
	struct iommu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);

	BUG_ON(!dev->archdata.iommu || !data);

	return &data->vmm;
}

int rockchip_init_iovmm(struct device *iommu, struct rockchip_iovmm *vmm);
#else
static inline int rockchip_init_iovmm(struct device *iommu,
				   struct rockchip_iovmm *vmm)
{
	return -ENOSYS;
}
#endif

#ifdef CONFIG_ROCKCHIP_IOMMU

/**
 * rockchip_iommu_tlb_invalidate() - flush all TLB entry in iommu
 * @owner: The device whose IOMMU.
 *
 * This function flush all TLB entry in iommu
 */
int rockchip_iommu_tlb_invalidate(struct device *owner);
int rockchip_iommu_tlb_invalidate_global(struct device *owner);

#else /* CONFIG_ROCKCHIP_IOMMU */
static inline int rockchip_iommu_tlb_invalidate(struct device *owner)
{
	return -1;
}
static int rockchip_iommu_tlb_invalidate_global(struct device *owner)
{
	return -1;
}

#endif

#endif	/*__ASM_PLAT_IOMMU_H*/
