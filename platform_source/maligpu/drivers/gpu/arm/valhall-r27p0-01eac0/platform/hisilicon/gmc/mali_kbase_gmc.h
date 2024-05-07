/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2019. All rights reserved.
 * Description: This file describe GPU memory compress feature.
 * Create: 2014-2-24
 *
 * This file is part of MALI Midgard Reclaimer for NATIVE memory
 * allocations also known as "Midgard GMC" (graphical memory compression).
 *
 * "Midgard GMC" is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Midgard GMC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with "Midgard GMC".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MALI_GMC_H
#define MALI_GMC_H

#include <linux/gmc.h>
#include <mali_kbase.h>
#include <kernel.h>

#if defined(CONFIG_MALI_NORR) || defined(CONFIG_MALI_SIGURD) ||         \
		defined(CONFIG_MALI_NORR_PHX) || defined(CONFIG_MALI_GONDUL) || defined(CONFIG_MALI_TRYM) ||         \
		defined(CONFIG_MALI_BORR)
#define PAGE_FAULT_MAP_BLOCK 128
#else
#define PAGE_FAULT_MAP_BLOCK 1
#endif
#define PAGE_FAULT_MAP_BLOCK_MASK (PAGE_FAULT_MAP_BLOCK - 1)
#define GMC_PF_OUT_OF_BOUNDS 1
#define GMC_HANDLE_ALL_KCTXS 0

/*
 * In the test of the grass running scene of game com.tencent.tmgp.pubgmhd,
 * the threshold of lost frames at the start of WeChat is 128. When this value is 128,
 * the probability of the occurrence of lost frames is low and the phenomenon is not obvious.
 * When the value is set to 64, the memory compression time increases by 20%, about 25ms,
 * Application startup animation most flow
 */
#define GMC_BATCH_OPERATION_SIZE 64

struct kbase_gmc_arg {
	gmc_op op;
	long compress_size;
};

struct kbase_gmc_tsk {
	struct kbase_context *kctx;
	struct task_struct *task;
	struct mm_struct *mm;
	int trylock_status;
};

/* generic gmc interface for compression / decompression */
int kbase_gmc_compress(pid_t pid, struct gmc_device *gmc_dev,
	long compress_size);
int kbase_gmc_decompress(pid_t pid, struct gmc_device *gmc_dev);
int kbase_gmc_cancel(long cancel, struct gmc_device *gmc_dev);
int kbase_gmc_meminfo_open(struct inode *in, struct file *file);
struct kbase_context *kbase_reg_flags_to_kctx(struct kbase_va_region *reg);
void kbase_gmc_device_init(struct kbase_device *kbdev);
void kbase_gmc_device_term(struct kbase_device *kbdev);

#define KBASE_ENTRY_COMPRESSED (1u << 7)

#define for_each_rb_node(root, node) \
	for (node = rb_first(root); node; node = rb_next(node))

static inline bool is_region_free(struct kbase_va_region *reg)
{
	return (KBASE_REG_FREE & reg->flags);
}

static inline struct gmc_storage_handle *kbase_get_gmc_handle(
	struct tagged_addr *p)
{
	return (struct gmc_storage_handle *)(uintptr_t)(p->tagged_addr);
}

#ifdef CONFIG_GPU_GMC_GENERIC
static inline bool kbase_is_entry_compressed(struct tagged_addr ta)
{
	return !!(ta.gmc_flag & KBASE_ENTRY_COMPRESSED);
}

static inline bool kbase_is_entry_decompressed(struct tagged_addr p)
{
	return !kbase_is_entry_compressed(p);
}

static inline void kbase_clear_entry_compressed(struct tagged_addr *p)
{
	p->gmc_flag &= (~KBASE_ENTRY_COMPRESSED);
}

static inline void kbase_set_gmc_handle(struct gmc_storage_handle *handle,
	struct tagged_addr *p)
{
	p->tagged_addr = (phys_addr_t)(uintptr_t)handle;
	p->gmc_flag |= KBASE_ENTRY_COMPRESSED;
}

/*
 * bring back compressed pages and optionally map them back to gpu,
 * the region is given as input
 */
int kbase_get_compressed_region(struct kbase_va_region *reg, u64 vpfn,
	size_t nr);

/* drop gmc data associated with allocation */
void kbase_gmc_invalidate_alloc(struct kbase_context *kctx,
	struct tagged_addr *start, size_t pages_num);

/* bring back compressed pages (the alloc is given as input) */
int kbase_get_compressed_alloc(struct kbase_mem_phy_alloc *alloc,
	u64 start_idx, size_t nr);

/* worker for doing parallel gmc operations */
void kbase_gmc_walk_region_work(struct work_struct *work);
#else

static inline bool kbase_is_entry_compressed(struct tagged_addr ta)
{
	return false;
}

static inline bool kbase_is_entry_decompressed(struct tagged_addr p)
{
	return !kbase_is_entry_compressed(p);
}

static inline void kbase_clear_entry_compressed(struct tagged_addr *p)
{
}

static inline void kbase_set_gmc_handle(struct gmc_storage_handle *handle,
	struct tagged_addr *p)
{
}

static inline int kbase_get_compressed_region(struct kbase_va_region *reg,
	u64 vpfn, size_t nr)
{
	return 0;
};

static inline void kbase_gmc_invalidate_alloc(struct kbase_context *kctx,
	struct tagged_addr *start, size_t pages_num)
{
}

static inline int kbase_get_compressed_alloc(struct kbase_mem_phy_alloc *alloc,
	u64 start_idx, size_t nr)
{
	return 0;
}

static inline void kbase_gmc_walk_region_work(struct work_struct *work)
{
}
#endif
#endif