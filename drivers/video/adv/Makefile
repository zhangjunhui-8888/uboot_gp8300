#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

adv_inc = -I$(srctree)/drivers/video/adv \
        -I$(srctree)/drivers/video/adv/RX \
        -I$(srctree)/drivers/video/adv/RX/HAL \
        -I$(srctree)/drivers/video/adv/RX/HAL/5G \
        -I$(srctree)/drivers/video/adv/RX/HAL/5G/ \
        -I$(srctree)/drivers/video/adv/RX/HAL/5G/ADV7612 \
        -I$(srctree)/drivers/video/adv/RX/HAL/5G/ADV7612/HAL \
        -I$(srctree)/drivers/video/adv/RX/HAL/5G/ADV7612/MACROS \
        -I$(srctree)/drivers/video/adv/RX/LIB \
        -I$(srctree)/drivers/video/adv/TX \
        -I$(srctree)/drivers/video/adv/TX/HAL \
        -I$(srctree)/drivers/video/adv/TX/HAL/COMMON \
        -I$(srctree)/drivers/video/adv/TX/HAL/WIRED \
        -I$(srctree)/drivers/video/adv/TX/HAL/WIRED/ADV7511 \
        -I$(srctree)/drivers/video/adv/TX/HAL/WIRED/ADV7511/MACROS \
        -I$(srctree)/drivers/video/adv/TX/LIB 

KBUILD_CFLAGS += $(adv_inc)

obj-$(CONFIG_HDMI_ADV) += wrapper.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_afe.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_cdec.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_cec.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_cp.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_dpp_csc.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_gen.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_hdmi.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_isr.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_multi.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_sdp.o
obj-$(CONFIG_HDMI_ADV) += RX/LIB/rx_vdp.o

obj-$(CONFIG_HDMI_ADV) += RX/HAL/rx_afe_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/rx_cp_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/rx_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/rx_hdmi_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/rx_sdp_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/rx_sdp_vdp_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/rx_vdp_hal.o

obj-$(CONFIG_HDMI_ADV) += RX/HAL/5G/rx_5g_cp_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/5G/rx_5g_gen_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/5G/rx_5g_hdmi_hal.o
obj-$(CONFIG_HDMI_ADV) += RX/HAL/5G/rx_5g_no_supp_hal.o

obj-$(CONFIG_HDMI_ADV) += RX/HAL/5G/ADV7612/HAL/7612_ini_hal.o


obj-$(CONFIG_HDMI_ADV) += TX/LIB/tx_cec.o
obj-$(CONFIG_HDMI_ADV) += TX/LIB/tx_isr.o
obj-$(CONFIG_HDMI_ADV) += TX/LIB/tx_lib.o
obj-$(CONFIG_HDMI_ADV) += TX/LIB/tx_multi.o

obj-$(CONFIG_HDMI_ADV) += TX/HAL/COMMON/tx_hal.o

obj-$(CONFIG_HDMI_ADV) += TX/HAL/WIRED/wrd_hal.o

obj-$(CONFIG_HDMI_ADV) += TX/HAL/WIRED/ADV7511/7511_hal.o





# add more
