# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_HMFS_FS) += hmfs.o

hmfs-y		:= dir.o file.o inode.o namei.o hash.o super.o inline.o
hmfs-y		+= checkpoint.o gc.o data.o node.o segment.o recovery.o
hmfs-y		+= shrinker.o extent_cache.o sysfs.o
hmfs-y		+= f2fs_dump_info.o
hmfs-y		+= fscrypt_metadata.o
hmfs-$(CONFIG_HMFS_STAT_FS) += debug.o
hmfs-$(CONFIG_HMFS_FS_XATTR) += xattr.o
hmfs-$(CONFIG_HMFS_FS_POSIX_ACL) += acl.o
hmfs-$(CONFIG_HMFS_IO_TRACE) += trace.o
hmfs-$(CONFIG_HWDPS) += hwdps_context.o
ifdef CONFIG_HMFS_KIRIN_ENCRYPTION
hmfs-y += sdp_context.o
else
ifdef CONFIG_HMFS_CHIP_QCOM
hmfs-$(CONFIG_SDP_ENCRYPTION) += sdp_context.o
endif
endif
hmfs-$(CONFIG_HMFS_FS_COMPRESSION) += compress.o
hmfs-$(CONFIG_FS_VERITY) += verity.o
