-include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk
obj-$(CONFIG_BALONG_MODEM_RESET)        += reset_balong.o

ifeq ($(strip $(CFG_CONFIG_BALONG_MODEM_RESET_CTRL)),YES)
obj-y 		  += reset_balong_ctrl.o
endif

EXTRA_CFLAGS += -I$(srctree)/drivers/hisi/tzdriver \
                -I$(srctree)/drivers/tzdriver

subdir-ccflags-y := -Werror
