/* Copyright (c) Huawei Technologies Co., Ltd. 2014-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "dpu_fb.h"
#include "dpu_mmbuf_manager.h"

#define VALUE_TYPE 4

struct plat_mdc_chan_num {
	int platform;
	unsigned int chan_num;
	int val_type;
};

struct plat_mdc_chan_num soc_chan_num[] = {
	/* platform & channel_num & value_type */
	{ FB_ACCEL_DSSV320, 1, 0 },
	{ FB_ACCEL_DSSV330, 1, 0 },
	{ FB_ACCEL_DPUV410, 2, 1 },
	{ FB_ACCEL_DSSV501, 2, 1 },
	{ FB_ACCEL_DSSV510, 2, 1 },
	{ FB_ACCEL_DSSV600, 2, 1 },
	{ FB_ACCEL_DSSV350, 1, 2 },
	{ FB_ACCEL_DSSV345, 1, 3 },
	{ FB_ACCEL_DSSV346, 1, 2 },
	{ FB_ACCEL_DSSV360, 1, 2 },
};

mdc_chn_info_t mdc_chn_data[VALUE_TYPE][MAX_MDC_CHANNEL] = {
	{
		/* value_type is 0 */
		{
			FREE,
			0,
			CAP_BASE | CAP_DIM | CAP_SCL | CAP_YUV_PACKAGE | CAP_YUV_SEMI_PLANAR |
				CAP_YUV_PLANAR | CAP_YUV_DEINTERLACE,
			DSS_RCHN_V1,
			DSS_WCHN_W0,
			DSS_OVL2,
			DSS_WB_COMPOSE_COPYBIT,
		},
		{0}, {0},
	},

	{
		/* value_type is 1 */
		{
			FREE,
			0,
			CAP_BASE | CAP_DIM | CAP_YUV_PACKAGE | CAP_YUV_SEMI_PLANAR |
				CAP_YUV_PLANAR | CAP_YUV_DEINTERLACE,
			DSS_RCHN_V2,
			DSS_WCHN_W1,
			DSS_OVL3,
			DSS_WB_COMPOSE_COPYBIT,
		},

		{
			FREE,
			0,
			CAP_BASE | CAP_DIM | CAP_SCL | CAP_YUV_PACKAGE | CAP_YUV_SEMI_PLANAR |
				CAP_YUV_PLANAR | CAP_YUV_DEINTERLACE | CAP_HFBCD | CAP_AFBCD,
			DSS_RCHN_V1,
			DSS_WCHN_W1,
			DSS_OVL3,
			DSS_WB_COMPOSE_COPYBIT,
		},
		{0},
	},

	{
		/* value_type is 2 */
		{
			FREE,
			0,
			CAP_BASE | CAP_DIM | CAP_SCL | CAP_YUV_PACKAGE | CAP_YUV_SEMI_PLANAR |
				CAP_YUV_PLANAR | CAP_YUV_DEINTERLACE | CAP_HFBCD | CAP_AFBCD,
			DSS_RCHN_V1,
			DSS_WCHN_W1,
			DSS_OVL2,
			DSS_WB_COMPOSE_COPYBIT,
		},
		{0}, {0},
	},

	{
		/* value_type is 3 */
		{
			FREE,
			0,
			CAP_BASE | CAP_DIM | CAP_SCL | CAP_YUV_PACKAGE | CAP_YUV_SEMI_PLANAR |
				CAP_YUV_PLANAR | CAP_YUV_DEINTERLACE | CAP_AFBCD,
			DSS_RCHN_V1,
			DSS_WCHN_W1,
			DSS_OVL2,
			DSS_WB_COMPOSE_COPYBIT,
		},
		{0}, {0},
	}
};


static int mdc_refresh_handle_thread(void *data)
{
	struct dpu_fb_data_type *dpufd = (struct dpu_fb_data_type *)data;
	int ret;

	while (!kthread_should_stop()) {
		ret = wait_event_interruptible(dpufd->mdc_ops.refresh_handle_wait, dpufd->need_refresh); /*lint !e578*/
		if (!ret && dpufd->need_refresh) {
			dpu_fb_frame_refresh(dpufd, "mdc");
			dpufd->need_refresh = false;
		}
	}

	return 0;
}

static int dpu_mdc_check_specified_request(mdc_ch_info_t *chn_info, mdc_chn_info_t *mdc_chn)
{
	/* specified chn request */
	if ((chn_info->rch_idx >= DSS_RCHN_D2) && (chn_info->rch_idx != mdc_chn->rch_idx)) {
		/* Not suitable */
		DPU_FB_DEBUG("need_chn %d not my specified chn %d\n",
			mdc_chn->rch_idx, chn_info->rch_idx);
		return -1;
	}

	/* specified cap request */
	if ((chn_info->rch_need_cap != 0) &&
		((chn_info->rch_need_cap & mdc_chn->cap_available) != chn_info->rch_need_cap)) {
		/* Not suitable */
		DPU_FB_DEBUG("need_cap[0x%x] is not available, mdc_chn->cap[0x%x]\n",
			chn_info->rch_need_cap, mdc_chn->cap_available);
		return -1;
	}

	return 0;
}

static void dpu_mdc_update_chn_info(mdc_ch_info_t *chn_info, mdc_chn_info_t *mdc_chn)
{
	chn_info->rch_idx = mdc_chn->rch_idx;
	chn_info->wch_idx = mdc_chn->wch_idx;
	chn_info->ovl_idx = mdc_chn->ovl_idx;
	chn_info->wb_composer_type = mdc_chn->wb_composer_type;
}

#if defined(CONFIG_DPU_FB_V320) || defined(CONFIG_DPU_FB_V330)
static int hwc_need_take_off_mdc_chn(struct dpu_fb_data_type *dpufd, mdc_ch_info_t *chn_info,
	mdc_func_ops_t *mdc_ops, mdc_chn_info_t *mdc_chn)
{
	if (chn_info->is_drm == 0) {
		/* hwc request mdc channel not for drm, find other mdc channel */
		DPU_FB_DEBUG("HWC request with no drm!\n");
		return -1;
	}

	dpufd->need_refresh = true;
	/* hwc request channel for drm, wait and take off this channel */
	DPU_FB_DEBUG("hwc request for drm,wait!\n");
	wake_up_interruptible_all(&(mdc_ops->refresh_handle_wait));

	dpu_mdc_update_chn_info(chn_info, mdc_chn);
	mdc_chn->status = HWC_USED;
	mdc_chn->drm_used = chn_info->is_drm;
	DPU_FB_DEBUG("channel %d hold_flag = %d, status = %d, is_drm = %d\n",
		mdc_chn->rch_idx, chn_info->hold_flag, mdc_chn->status, chn_info->is_drm);
	DPU_FB_DEBUG("Request mdc channel %d success\n", mdc_chn->rch_idx);

	return 0;
}

static int mdc_need_take_off_hwc_chn(struct dpu_fb_data_type *dpufd,
	mdc_func_ops_t *mdc_ops, mdc_chn_info_t *mdc_chn)
{
	if (mdc_chn->drm_used == 1) {
		/* find other channel for mdc request because channel is used by drm */
		DPU_FB_DEBUG("hwc drm is using!\n");
		return -1;
	}

	dpufd->need_refresh = true;
	/* mdc request channel, wait and take off this channel */
	DPU_FB_DEBUG("hwc drm is not using!\n");
	wake_up_interruptible_all(&(mdc_ops->refresh_handle_wait));

	return 0;
}
#endif

static int mdc_check_mmbuf_size_available(struct dpu_fb_data_type *dpufd, mdc_ch_info_t *chn_info,
	uint32_t reserved_size)
{
	/* chn available */
	if (((chn_info->rch_need_cap & CAP_HFBCD) == CAP_HFBCD) ||
		((chn_info->rch_need_cap & CAP_AFBCD) == CAP_AFBCD)) {
		if ((chn_info->mmbuf_size == 0) ||
			(chn_info->mmbuf_size > reserved_size) ||
			(chn_info->mmbuf_size & (MMBUF_ADDR_ALIGN - 1))) {
			DPU_FB_ERR("fb%d, mmbuf size is invalid, size = %d!\n",
				dpufd->index, chn_info->mmbuf_size);
			return -EINVAL;
		}
	}

	return 0;
}

static int mdc_chn_pre_request(struct dpu_fb_data_type *dpufd, mdc_ch_info_t *chn_info,
	mdc_func_ops_t *mdc_ops, uint32_t mmbuf_mdc_reserved_size)
{
	unsigned int i;
	mdc_chn_info_t *mdc_chn = NULL;

	for (i = 0; i < mdc_ops->chan_num; i++) {
		mdc_chn = &(mdc_ops->mdc_channel[i]);
		if (dpu_mdc_check_specified_request(chn_info, mdc_chn))
			continue;

		if (mdc_chn->status != FREE) {
			DPU_FB_DEBUG("mdc_chn status is not FREE!, mdc_chn status = %d\n", mdc_chn->status);
			continue;
		}

		if (mdc_check_mmbuf_size_available(dpufd, chn_info, mmbuf_mdc_reserved_size))
			return -EINVAL;

		dpu_mdc_update_chn_info(chn_info, mdc_chn);

		mdc_chn->status = (chn_info->hold_flag == HWC_REQUEST) ? HWC_USED : MDC_USED;

		mdc_chn->drm_used = chn_info->is_drm;
		DPU_FB_DEBUG("chn_info is_drm = %d, hold_flag = %d, status = %d\n", chn_info->is_drm,
			chn_info->hold_flag, mdc_chn->status);

		DPU_FB_DEBUG("request mdc channel %d seccess\n", mdc_chn->rch_idx);
		return 0;
	}

	return -EBUSY;
}

static int mdc_chn_request(struct dpu_fb_data_type *dpufd, mdc_ch_info_t *chn_info,
	mdc_func_ops_t *mdc_ops, uint32_t mmbuf_mdc_reserved_size)
{
	unsigned int i;
	mdc_chn_info_t *mdc_chn = NULL;

	for (i = 0; i < mdc_ops->chan_num; i++) {
		mdc_chn = &(mdc_ops->mdc_channel[i]);
		if (dpu_mdc_check_specified_request(chn_info, mdc_chn))
			continue;

		if ((mdc_chn->status == MDC_USED) && (chn_info->hold_flag == HWC_REQUEST)) {
#if defined(CONFIG_DPU_FB_V320) || defined(CONFIG_DPU_FB_V330)
			if (hwc_need_take_off_mdc_chn(dpufd, chn_info, mdc_ops, mdc_chn))
				continue;
			else
				return 0;
#else
			continue;
#endif
		}

		if ((mdc_chn->status == HWC_USED) && (chn_info->hold_flag == MDC_REQUEST)) {
#if defined(CONFIG_DPU_FB_V320) || defined(CONFIG_DPU_FB_V330)
			if (mdc_need_take_off_hwc_chn(dpufd, mdc_ops, mdc_chn))
				continue;
#endif
			dpufd->need_refresh = true;
			wake_up_interruptible_all(&(mdc_ops->refresh_handle_wait));
		}

		if (mdc_check_mmbuf_size_available(dpufd, chn_info, mmbuf_mdc_reserved_size))
			return -EINVAL;

		dpu_mdc_update_chn_info(chn_info, mdc_chn);
		mdc_chn->status = MDC_USED;
		mdc_chn->drm_used = chn_info->is_drm;
		DPU_FB_DEBUG("hold_status = %d, status = %d, drm_used = %d\n",
			chn_info->hold_flag, mdc_chn->status, chn_info->is_drm);

		DPU_FB_DEBUG("Request mdc channel %d success\n", mdc_chn->rch_idx);
		return 0;
	}

	return -EBUSY;
}

static int mdc_chn_request_handle(struct dpu_fb_data_type *dpufd, mdc_ch_info_t *chn_info)
{
	int ret;
	mdc_func_ops_t *mdc_ops = NULL;
	uint32_t mmbuf_mdc_reserved_size;

	dpu_check_and_return((!dpufd || !chn_info), -EINVAL, ERR, "dpufd or chn_info is NULL\n");

	mmbuf_mdc_reserved_size = dss_mmbuf_reserved_info[SERVICE_MDC].size;

	mdc_ops = &(dpufd->mdc_ops);
	if (mdc_ops->chan_num > MAX_MDC_CHANNEL) {
		DPU_FB_ERR("chan_num=%d is invalid\n", mdc_ops->chan_num);
		return -EINVAL;
	}

	ret = mdc_chn_pre_request(dpufd, chn_info, mdc_ops, mmbuf_mdc_reserved_size);
	if (ret != -EBUSY)
		return ret;

	ret = mdc_chn_request(dpufd, chn_info, mdc_ops, mmbuf_mdc_reserved_size);
	if (ret != -EBUSY)
		return ret;

	DPU_FB_DEBUG("request channel failed, have no available channel\n");

	return -1;
}

int mdc_chn_release_handle(struct dpu_fb_data_type *dpufd,
	mdc_ch_info_t *chn_info)
{
	unsigned int i;
	mdc_chn_info_t *mdc_chn = NULL;
	mdc_func_ops_t *mdc_ops = NULL;

	if (!dpufd || !chn_info) {
		DPU_FB_ERR("dpufd or chn_info is NULL\n");
		return -EINVAL;
	}

	mdc_ops = &(dpufd->mdc_ops);
	if (mdc_ops->chan_num > MAX_MDC_CHANNEL) {
		DPU_FB_ERR("chan_num=%d is invalid\n", mdc_ops->chan_num);
		return -EINVAL;
	}

	for (i = 0; i < mdc_ops->chan_num; i++) {
		mdc_chn = &(mdc_ops->mdc_channel[i]);

		if (chn_info->rch_idx != mdc_chn->rch_idx)
			continue;

		if ((chn_info->rch_need_cap & CAP_HFBCD) == CAP_HFBCD) {
			if ((chn_info->mmbuf_addr < MMBUF_BASE) || (chn_info->mmbuf_size <= 0)) {
				DPU_FB_ERR("mdc %d release failed, hfbc addr=0x%x, size=%d is invalid!\n",
					chn_info->rch_idx, chn_info->mmbuf_addr, chn_info->mmbuf_size);
				return -EINVAL;
			}
		}

		if ((mdc_chn->status == HWC_USED) && (chn_info->hold_flag == HWC_REQUEST))
			mdc_chn->status = FREE;

		if ((mdc_chn->status == MDC_USED) && (chn_info->hold_flag == MDC_REQUEST))
			mdc_chn->status = FREE;

		DPU_FB_DEBUG("Release mdc channel %d success\n", mdc_chn->rch_idx);
	}

	return 0;
}

int dpu_mdc_chn_request(struct fb_info *info, void __user *argp)
{
	int ret;
	struct dpu_fb_data_type *dpufd = NULL;
	mdc_func_ops_t *mdc_ops = NULL;
	mdc_ch_info_t chn_info;

#if defined(CONFIG_DPU_FB_V320)
	struct dpu_fb_data_type *primary_dpufd = NULL;
#endif

	dpu_check_and_return(!info, -EINVAL, ERR, "info NULL pointer!\n");

	dpufd = (struct dpu_fb_data_type *)info->par;
	dpu_check_and_return(!dpufd, -EINVAL, ERR, "dpufd NULL pointer!\n");

	if (dpufd->index != AUXILIARY_PANEL_IDX) {
		DPU_FB_INFO("fb%d don't support\n", dpufd->index);
		return -EINVAL;
	}

	dpu_check_and_return(!argp, -EINVAL, ERR, "argp NULL pointer!\n");

#if defined(CONFIG_DPU_FB_V320)
	primary_dpufd = dpufd_list[PRIMARY_PANEL_IDX];
	if (primary_dpufd && (primary_dpufd->secure_ctrl.secure_event == DSS_SEC_ENABLE)) {
		DPU_FB_INFO("fb%d in TUI mode\n", dpufd->index);
		return -EINVAL;
	}
#endif
	mdc_ops = &(dpufd->mdc_ops);
	ret = copy_from_user(&chn_info, argp, sizeof(mdc_ch_info_t));
	dpu_check_and_return((ret != 0), -EINVAL, ERR, "fb%d, copy for user failed! ret=%d\n", dpufd->index, ret);

	if (down_trylock(&mdc_ops->mdc_req_sem)) {
		if (chn_info.hold_flag == HWC_REQUEST) {
			DPU_FB_INFO("mdc request in handle!\n");
			return -EINVAL;
		}

		down(&mdc_ops->mdc_req_sem);
	}

	if (mdc_ops->chn_request_handle != NULL) {
		if (mdc_ops->chn_request_handle(dpufd, &chn_info)) {
			DPU_FB_INFO("fb%d, request chn failed!\n", dpufd->index);
			up(&mdc_ops->mdc_req_sem);
			return -EINVAL;
		}
	}

	ret = copy_to_user(argp, &chn_info, sizeof(mdc_ch_info_t));
	if (ret) {
		DPU_FB_ERR("fb%d, copy to user failed! ret=%d\n", dpufd->index, ret);
		if (mdc_ops->chn_release_handle != NULL)
			mdc_ops->chn_release_handle(dpufd, &chn_info);
	}
	up(&mdc_ops->mdc_req_sem);

	return ret;
}

int dpu_mdc_chn_release(struct fb_info *info, const void __user *argp)
{
	int ret;
	struct dpu_fb_data_type *dpufd = NULL;
	mdc_func_ops_t *mdc_ops = NULL;
	mdc_ch_info_t chn_info;

	if (!info) {
		DPU_FB_ERR("info NULL pointer!\n");
		return -EINVAL;
	}

	dpufd = (struct dpu_fb_data_type *)info->par;
	if (!dpufd) {
		DPU_FB_ERR("dpufd NULL pointer!\n");
		return -EINVAL;
	}

	if (dpufd->index != AUXILIARY_PANEL_IDX) {
		DPU_FB_INFO("fb%d don't support", dpufd->index);
		return -EINVAL;
	}

	if (!argp) {
		DPU_FB_ERR("argp NULL pointer!\n");
		return -EINVAL;
	}
	mdc_ops = &(dpufd->mdc_ops);
	down(&mdc_ops->mdc_rel_sem);

	ret = copy_from_user(&chn_info, argp, sizeof(mdc_ch_info_t));
	if (ret) {
		DPU_FB_ERR("fb%d, copy for user failed! ret=%d\n", dpufd->index, ret);
		up(&mdc_ops->mdc_rel_sem);
		return -EINVAL;
	}

	if (mdc_ops->chn_release_handle != NULL)
		ret = mdc_ops->chn_release_handle(dpufd, &chn_info);

	up(&mdc_ops->mdc_rel_sem);
	return ret;
}

static int dpu_mdc_chn_resource_init(mdc_func_ops_t *mdc_ops, unsigned int platform)
{
	int ret = 0;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(soc_chan_num); i++) {
		if (platform == soc_chan_num[i].platform) {
			mdc_ops->chan_num = soc_chan_num[i].chan_num;
			memcpy(mdc_ops->mdc_channel, &mdc_chn_data[soc_chan_num[i].val_type][0],
				soc_chan_num[i].chan_num * sizeof(mdc_chn_info_t));
			break;
		}
	}

	if (i >= ARRAY_SIZE(soc_chan_num)) {
		DPU_FB_ERR("Not support mdc copybit func!\n");
		ret = -1;
	}

	return ret;
}

int dpu_mdc_resource_init(struct dpu_fb_data_type *dpufd, unsigned int platform)
{
	int ret;
	mdc_func_ops_t *mdc_ops = NULL;

	dpu_check_and_return(!dpufd, -EINVAL, ERR, "dpufd is NULL pointer!\n");

	mdc_ops = &(dpufd->mdc_ops);
	mdc_ops->chn_request_handle = mdc_chn_request_handle;
	mdc_ops->chn_release_handle = mdc_chn_release_handle;
	sema_init(&mdc_ops->mdc_req_sem, 1);
	sema_init(&mdc_ops->mdc_rel_sem, 1);

	init_waitqueue_head(&(mdc_ops->refresh_handle_wait));
	mdc_ops->refresh_handle_thread = kthread_run(mdc_refresh_handle_thread, dpufd, "refresh_handle");
	if (IS_ERR(mdc_ops->refresh_handle_thread))
		mdc_ops->refresh_handle_thread = NULL;

	ret = dpu_mdc_chn_resource_init(mdc_ops, platform);

	DPU_FB_INFO("Init complete!\n");

	return ret;
}
