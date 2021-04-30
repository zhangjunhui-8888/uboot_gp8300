deps_config := \
	test/env/Kconfig \
	test/dm/Kconfig \
	test/Kconfig \
	lib/efi/Kconfig \
	lib/rsa/Kconfig \
	lib/dhry/Kconfig \
	lib/Kconfig \
	fs/cramfs/Kconfig \
	fs/ubifs/Kconfig \
	fs/jffs2/Kconfig \
	fs/fat/Kconfig \
	fs/reiserfs/Kconfig \
	fs/ext4/Kconfig \
	fs/Kconfig \
	drivers/watchdog/Kconfig \
	drivers/video/adv/Kconfig \
	drivers/video/bridge/Kconfig \
	drivers/video/Kconfig \
	drivers/usb/emul/Kconfig \
	drivers/usb/musb-new/Kconfig \
	drivers/usb/host/Kconfig \
	drivers/usb/Kconfig \
	drivers/tpm/Kconfig \
	drivers/timer/Kconfig \
	drivers/thermal/Kconfig \
	drivers/spi/Kconfig \
	drivers/sound/Kconfig \
	drivers/serial/Kconfig \
	drivers/rtc/Kconfig \
	drivers/remoteproc/Kconfig \
	drivers/ram/Kconfig \
	drivers/power/regulator/Kconfig \
	drivers/power/pmic/Kconfig \
	drivers/power/Kconfig \
	drivers/pinctrl/uniphier/Kconfig \
	drivers/pinctrl/Kconfig \
	drivers/pcmcia/Kconfig \
	drivers/pci/Kconfig \
	drivers/net/Kconfig \
	drivers/mtd/spi/Kconfig \
	drivers/mtd/nand/Kconfig \
	drivers/mtd/norflash/Kconfig \
	drivers/mtd/Kconfig \
	drivers/mmc/Kconfig \
	drivers/misc/Kconfig \
	drivers/led/Kconfig \
	drivers/input/Kconfig \
	drivers/i2c/muxes/Kconfig \
	drivers/i2c/Kconfig \
	drivers/hwmon/Kconfig \
	drivers/gpio/Kconfig \
	drivers/dma/Kconfig \
	drivers/dfu/Kconfig \
	drivers/demo/Kconfig \
	drivers/crypto/fsl/Kconfig \
	drivers/crypto/Kconfig \
	drivers/cpu/Kconfig \
	drivers/clk/Kconfig \
	drivers/block/Kconfig \
	drivers/adc/Kconfig \
	drivers/core/Kconfig \
	drivers/Kconfig \
	net/Kconfig \
	dts/Kconfig \
	common/Kconfig \
	board/gpt/gp8300_hera/Kconfig \
	board/gpt/gp8300_tcx/Kconfig \
	board/gpt/gp8300_chaokong/Kconfig \
	board/gpt/gp8300_dvb/Kconfig \
	board/gpt/gp8300_evk/Kconfig \
	board/gpt/gp8300_fpga/Kconfig \
	board/gpt/chasexilinx/Kconfig \
	board/gpt/chip2sim/Kconfig \
	board/gpt/chip2evk/Kconfig \
	board/gpt/chip2fpga/Kconfig \
	board/gpt/chip1evk/Kconfig \
	board/gpt/chip1fpga/Kconfig \
	arch/gpt/cpu/chip2/Kconfig \
	arch/gpt/cpu/chip1/Kconfig \
	arch/gpt/mach-chip2/Kconfig \
	arch/gpt/Kconfig \
	arch/x86/lib/efi/Kconfig \
	arch/x86/cpu/queensbay/Kconfig \
	arch/x86/cpu/quark/Kconfig \
	arch/x86/cpu/qemu/Kconfig \
	arch/x86/cpu/ivybridge/Kconfig \
	arch/x86/cpu/coreboot/Kconfig \
	arch/x86/cpu/baytrail/Kconfig \
	board/intel/minnowmax/Kconfig \
	board/intel/galileo/Kconfig \
	board/intel/crownbay/Kconfig \
	board/intel/bayleybay/Kconfig \
	board/intel/Kconfig \
	board/google/chromebox_panther/Kconfig \
	board/google/chromebook_link/Kconfig \
	board/google/Kconfig \
	board/emulation/qemu-x86/Kconfig \
	board/emulation/Kconfig \
	board/efi/efi-x86/Kconfig \
	board/efi/Kconfig \
	board/coreboot/coreboot/Kconfig \
	board/coreboot/Kconfig \
	arch/x86/Kconfig \
	board/gaisler/grsim_leon2/Kconfig \
	board/gaisler/grsim/Kconfig \
	board/gaisler/gr_xc3s_1500/Kconfig \
	board/gaisler/gr_ep2s60/Kconfig \
	board/gaisler/gr_cpci_ax2000/Kconfig \
	arch/sparc/Kconfig \
	board/shmin/Kconfig \
	board/renesas/sh7785lcr/Kconfig \
	board/renesas/sh7763rdp/Kconfig \
	board/renesas/sh7757lcr/Kconfig \
	board/renesas/sh7753evb/Kconfig \
	board/renesas/sh7752evb/Kconfig \
	board/renesas/rsk7269/Kconfig \
	board/renesas/rsk7264/Kconfig \
	board/renesas/rsk7203/Kconfig \
	board/renesas/r7780mp/Kconfig \
	board/renesas/r2dplus/Kconfig \
	board/renesas/r0p7734/Kconfig \
	board/renesas/ecovec/Kconfig \
	board/renesas/ap325rxa/Kconfig \
	board/renesas/MigoR/Kconfig \
	board/ms7750se/Kconfig \
	board/ms7722se/Kconfig \
	board/ms7720se/Kconfig \
	board/mpr2/Kconfig \
	board/espt/Kconfig \
	board/alphaproject/ap_sh4a_4a/Kconfig \
	arch/sh/Kconfig \
	arch/sandbox/Kconfig \
	board/xilinx/ppc440-generic/Kconfig \
	board/xilinx/ppc405-generic/Kconfig \
	board/xilinx/ml507/Kconfig \
	board/xes/xpedite1000/Kconfig \
	board/t3corp/Kconfig \
	board/mpl/pip405/Kconfig \
	board/mpl/mip405/Kconfig \
	board/mosaixtech/icon/Kconfig \
	board/liebherr/lwmon5/Kconfig \
	board/gdsys/intip/Kconfig \
	board/gdsys/gdppc440etx/Kconfig \
	board/gdsys/dlvision/Kconfig \
	board/gdsys/405ex/Kconfig \
	board/gdsys/405ep/Kconfig \
	board/esd/vom405/Kconfig \
	board/esd/pmc440/Kconfig \
	board/esd/pmc405de/Kconfig \
	board/esd/plu405/Kconfig \
	board/esd/cpci405/Kconfig \
	board/esd/cpci2dp/Kconfig \
	board/avnet/v5fx30teval/Kconfig \
	board/avnet/fx12mm/Kconfig \
	board/amcc/yucca/Kconfig \
	board/amcc/yosemite/Kconfig \
	board/amcc/walnut/Kconfig \
	board/amcc/sequoia/Kconfig \
	board/amcc/redwood/Kconfig \
	board/amcc/makalu/Kconfig \
	board/amcc/luan/Kconfig \
	board/amcc/kilauea/Kconfig \
	board/amcc/katmai/Kconfig \
	board/amcc/canyonlands/Kconfig \
	board/amcc/bubinga/Kconfig \
	board/amcc/bamboo/Kconfig \
	board/amcc/acadia/Kconfig \
	arch/powerpc/cpu/ppc4xx/Kconfig \
	board/tqc/tqm8xx/Kconfig \
	arch/powerpc/cpu/mpc8xx/Kconfig \
	board/xes/xpedite517x/Kconfig \
	board/sbc8641d/Kconfig \
	board/freescale/mpc8641hpcn/Kconfig \
	board/freescale/mpc8610hpcd/Kconfig \
	arch/powerpc/cpu/mpc86xx/Kconfig \
	board/Arcturus/ucp1020/Kconfig \
	board/xes/xpedite550x/Kconfig \
	board/xes/xpedite537x/Kconfig \
	board/xes/xpedite520x/Kconfig \
	board/varisys/cyrus/Kconfig \
	board/socrates/Kconfig \
	board/sbc8548/Kconfig \
	board/keymile/kmp204x/Kconfig \
	board/gdsys/p1022/Kconfig \
	board/freescale/t4rdb/Kconfig \
	board/freescale/t4qds/Kconfig \
	board/freescale/t208xrdb/Kconfig \
	board/freescale/t208xqds/Kconfig \
	board/freescale/t104xrdb/Kconfig \
	board/freescale/t1040qds/Kconfig \
	board/freescale/t102xrdb/Kconfig \
	board/freescale/t102xqds/Kconfig \
	board/freescale/qemu-ppce500/Kconfig \
	board/freescale/p2041rdb/Kconfig \
	board/freescale/p1_twr/Kconfig \
	board/freescale/p1_p2_rdb_pc/Kconfig \
	board/freescale/p1023rdb/Kconfig \
	board/freescale/p1022ds/Kconfig \
	board/freescale/p1010rdb/Kconfig \
	board/freescale/mpc8572ds/Kconfig \
	board/freescale/mpc8569mds/Kconfig \
	board/freescale/mpc8568mds/Kconfig \
	board/freescale/mpc8560ads/Kconfig \
	board/freescale/mpc8555cds/Kconfig \
	board/freescale/mpc8548cds/Kconfig \
	board/freescale/mpc8544ds/Kconfig \
	board/freescale/mpc8541cds/Kconfig \
	board/freescale/mpc8540ads/Kconfig \
	board/freescale/mpc8536ds/Kconfig \
	board/freescale/corenet_ds/Kconfig \
	board/freescale/c29xpcie/Kconfig \
	board/freescale/bsc9132qds/Kconfig \
	board/freescale/bsc9131rdb/Kconfig \
	board/freescale/b4860qds/Kconfig \
	arch/powerpc/cpu/mpc85xx/Kconfig \
	board/gdsys/mpc8308/Kconfig \
	board/ve8313/Kconfig \
	board/tqc/tqm834x/Kconfig \
	board/sbc8349/Kconfig \
	board/mpc8308_p1m/Kconfig \
	board/keymile/km83xx/Kconfig \
	board/ids/ids8313/Kconfig \
	board/freescale/mpc837xerdb/Kconfig \
	board/freescale/mpc837xemds/Kconfig \
	board/freescale/mpc8349itx/Kconfig \
	board/freescale/mpc8349emds/Kconfig \
	board/freescale/mpc832xemds/Kconfig \
	board/freescale/mpc8323erdb/Kconfig \
	board/freescale/mpc8315erdb/Kconfig \
	board/freescale/mpc8313erdb/Kconfig \
	board/freescale/mpc8308rdb/Kconfig \
	board/esd/vme8349/Kconfig \
	arch/powerpc/cpu/mpc83xx/Kconfig \
	board/keymile/km82xx/Kconfig \
	arch/powerpc/cpu/mpc8260/Kconfig \
	board/v38b/Kconfig \
	board/tqc/tqm5200/Kconfig \
	board/phytec/pcm030/Kconfig \
	board/munices/Kconfig \
	board/motionpro/Kconfig \
	board/jupiter/Kconfig \
	board/ipek01/Kconfig \
	board/intercontrol/digsy_mtc/Kconfig \
	board/inka4x0/Kconfig \
	board/ifm/o2dnt2/Kconfig \
	board/cm5200/Kconfig \
	board/canmb/Kconfig \
	board/a4m072/Kconfig \
	board/a3m071/Kconfig \
	arch/powerpc/cpu/mpc5xxx/Kconfig \
	board/mpl/pati/Kconfig \
	arch/powerpc/cpu/mpc5xx/Kconfig \
	board/pdm360ng/Kconfig \
	board/ifm/ac14xx/Kconfig \
	board/freescale/mpc5121ads/Kconfig \
	board/esd/mecp5123/Kconfig \
	board/davedenx/aria/Kconfig \
	arch/powerpc/cpu/mpc512x/Kconfig \
	arch/powerpc/Kconfig \
	board/openrisc/openrisc-generic/Kconfig \
	arch/openrisc/Kconfig \
	arch/nios2/Kconfig \
	board/AndesTech/adp-ag101p/Kconfig \
	arch/nds32/Kconfig \
	board/qemu-mips/Kconfig \
	board/pb1x00/Kconfig \
	board/micronas/vct/Kconfig \
	board/imgtec/malta/Kconfig \
	board/dbau1x00/Kconfig \
	arch/mips/Kconfig \
	board/xilinx/microblaze-generic/Kconfig \
	arch/microblaze/Kconfig \
	board/sysam/amcore/Kconfig \
	board/freescale/m548xevb/Kconfig \
	board/freescale/m547xevb/Kconfig \
	board/freescale/m54455evb/Kconfig \
	board/freescale/m54451evb/Kconfig \
	board/freescale/m54418twr/Kconfig \
	board/freescale/m5373evb/Kconfig \
	board/freescale/m5329evb/Kconfig \
	board/freescale/m53017evb/Kconfig \
	board/freescale/m5282evb/Kconfig \
	board/freescale/m5275evb/Kconfig \
	board/freescale/m5272c3/Kconfig \
	board/freescale/m5253evbe/Kconfig \
	board/freescale/m5253demo/Kconfig \
	board/freescale/m5249evb/Kconfig \
	board/freescale/m5235evb/Kconfig \
	board/freescale/m52277evb/Kconfig \
	board/freescale/m5208evbe/Kconfig \
	board/cobra5272/Kconfig \
	board/astro/mcf5373l/Kconfig \
	board/BuS/eb_cpu5282/Kconfig \
	arch/m68k/Kconfig \
	board/tcm-bf537/Kconfig \
	board/tcm-bf518/Kconfig \
	board/pr1/Kconfig \
	board/ip04/Kconfig \
	board/ibf-dsp561/Kconfig \
	board/dnp5370/Kconfig \
	board/cm-bf561/Kconfig \
	board/cm-bf548/Kconfig \
	board/cm-bf537u/Kconfig \
	board/cm-bf537e/Kconfig \
	board/cm-bf533/Kconfig \
	board/cm-bf527/Kconfig \
	board/br4/Kconfig \
	board/blackvme/Kconfig \
	board/blackstamp/Kconfig \
	board/bf609-ezkit/Kconfig \
	board/bf561-ezkit/Kconfig \
	board/bf561-acvilon/Kconfig \
	board/bf548-ezkit/Kconfig \
	board/bf538f-ezkit/Kconfig \
	board/bf537-stamp/Kconfig \
	board/bf537-srv1/Kconfig \
	board/bf537-pnav/Kconfig \
	board/bf537-minotaur/Kconfig \
	board/bf533-stamp/Kconfig \
	board/bf533-ezkit/Kconfig \
	board/bf527-sdp/Kconfig \
	board/bf527-ezkit/Kconfig \
	board/bf527-ad7160-eval/Kconfig \
	board/bf526-ezbrd/Kconfig \
	board/bf525-ucr2/Kconfig \
	board/bf518f-ezbrd/Kconfig \
	board/bf506f-ezkit/Kconfig \
	board/bct-brettl2/Kconfig \
	arch/blackfin/Kconfig \
	board/in-circuit/grasshopper/Kconfig \
	board/atmel/atstk1000/Kconfig \
	board/atmel/atngw100mkii/Kconfig \
	board/atmel/atngw100/Kconfig \
	arch/avr32/Kconfig \
	arch/arm/Kconfig.debug \
	board/work-microwave/work_92105/Kconfig \
	board/woodburn/Kconfig \
	board/vscom/baltos/Kconfig \
	board/technologic/ts4800/Kconfig \
	board/toradex/colibri_vf/Kconfig \
	board/toradex/colibri_pxa270/Kconfig \
	board/timll/devkit3250/Kconfig \
	board/ti/ti816x/Kconfig \
	board/ti/ti814x/Kconfig \
	board/birdland/bav335x/Kconfig \
	board/ti/am43xx/Kconfig \
	board/ti/am335x/Kconfig \
	board/tcl/sl50/Kconfig \
	board/syteco/zmx25/Kconfig \
	board/sunxi/Kconfig \
	board/st/stv0991/Kconfig \
	board/st/stm32f429-discovery/Kconfig \
	board/spear/x600/Kconfig \
	board/spear/spear600/Kconfig \
	board/spear/spear320/Kconfig \
	board/spear/spear310/Kconfig \
	board/spear/spear300/Kconfig \
	board/silica/pengwyn/Kconfig \
	board/siemens/rut/Kconfig \
	board/siemens/pxm2/Kconfig \
	board/siemens/draco/Kconfig \
	board/schulercontrol/sc_sps_1/Kconfig \
	board/sandisk/sansa_fuze_plus/Kconfig \
	board/samsung/smdk2410/Kconfig \
	board/ppcag/bg0900/Kconfig \
	board/phytec/pcm052/Kconfig \
	board/phytec/pcm051/Kconfig \
	board/olimex/mx23_olinuxino/Kconfig \
	board/mpl/vcma9/Kconfig \
	board/maxbcm/Kconfig \
	board/isee/igep0033/Kconfig \
	board/imx31_phycore/Kconfig \
	board/hisilicon/hikey/Kconfig \
	board/h2200/Kconfig \
	board/gumstix/pepper/Kconfig \
	board/freescale/vf610twr/Kconfig \
	board/freescale/mx53smd/Kconfig \
	board/freescale/mx53loco/Kconfig \
	board/freescale/mx53evk/Kconfig \
	board/freescale/mx53ard/Kconfig \
	board/freescale/mx51evk/Kconfig \
	board/freescale/mx35pdk/Kconfig \
	board/freescale/mx31pdk/Kconfig \
	board/freescale/mx31ads/Kconfig \
	board/freescale/mx28evk/Kconfig \
	board/freescale/mx25pdk/Kconfig \
	board/freescale/mx23evk/Kconfig \
	board/freescale/ls1043ardb/Kconfig \
	board/freescale/ls1021atwr/Kconfig \
	board/freescale/ls1043aqds/Kconfig \
	board/freescale/ls1021aqds/Kconfig \
	board/freescale/ls2080ardb/Kconfig \
	board/freescale/ls2080aqds/Kconfig \
	board/freescale/ls2080a/Kconfig \
	board/denx/m53evk/Kconfig \
	board/denx/m28evk/Kconfig \
	board/creative/xfi3/Kconfig \
	board/compulab/cm_t43/Kconfig \
	board/compulab/cm_t335/Kconfig \
	board/cirrus/edb93xx/Kconfig \
	board/broadcom/bcmnsp/Kconfig \
	board/broadcom/bcmcygnus/Kconfig \
	board/broadcom/bcm28155_ap/Kconfig \
	board/bluegiga/apx4devkit/Kconfig \
	board/armltd/vexpress64/Kconfig \
	board/armltd/vexpress/Kconfig \
	board/armadeus/apf27/Kconfig \
	board/Marvell/gplugd/Kconfig \
	board/Marvell/db-mv784mp-gp/Kconfig \
	board/Marvell/db-88f6820-gp/Kconfig \
	board/Marvell/aspenite/Kconfig \
	board/CarMediaLab/flea3/Kconfig \
	board/BuR/tseries/Kconfig \
	board/BuR/kwb/Kconfig \
	arch/arm/imx-common/Kconfig \
	arch/arm/cpu/armv8/Kconfig \
	arch/arm/cpu/armv8/zynqmp/Kconfig \
	arch/arm/cpu/armv7/Kconfig \
	arch/arm/mach-zynq/Kconfig \
	arch/arm/mach-uniphier/Kconfig \
	board/nvidia/p2571/Kconfig \
	board/nvidia/p2371-2180/Kconfig \
	board/nvidia/p2371-0000/Kconfig \
	board/nvidia/e2220-1170/Kconfig \
	arch/arm/mach-tegra/tegra210/Kconfig \
	board/nvidia/venice2/Kconfig \
	board/nvidia/nyan-big/Kconfig \
	board/nvidia/jetson-tk1/Kconfig \
	arch/arm/mach-tegra/tegra124/Kconfig \
	board/nvidia/dalmore/Kconfig \
	arch/arm/mach-tegra/tegra114/Kconfig \
	board/avionic-design/tec-ng/Kconfig \
	board/toradex/colibri_t30/Kconfig \
	board/nvidia/cardhu/Kconfig \
	board/nvidia/beaver/Kconfig \
	board/toradex/apalis_t30/Kconfig \
	arch/arm/mach-tegra/tegra30/Kconfig \
	board/toradex/colibri_t20/Kconfig \
	board/nvidia/whistler/Kconfig \
	board/nvidia/ventana/Kconfig \
	board/compulab/trimslice/Kconfig \
	board/avionic-design/tec/Kconfig \
	board/nvidia/seaboard/Kconfig \
	board/avionic-design/plutux/Kconfig \
	board/compal/paz00/Kconfig \
	board/avionic-design/medcom-wide/Kconfig \
	board/nvidia/harmony/Kconfig \
	arch/arm/mach-tegra/tegra20/Kconfig \
	arch/arm/mach-tegra/Kconfig \
	arch/arm/mach-socfpga/Kconfig \
	board/samsung/smdkc100/Kconfig \
	board/samsung/goni/Kconfig \
	arch/arm/mach-s5pc1xx/Kconfig \
	board/kylin/kylin_rk3036/Kconfig \
	board/evb_rk3036/evb_rk3036/Kconfig \
	arch/arm/mach-rockchip/rk3036/Kconfig \
	board/firefly/firefly-rk3288/Kconfig \
	board/google/chromebook_jerry/Kconfig \
	arch/arm/mach-rockchip/rk3288/Kconfig \
	arch/arm/mach-rockchip/Kconfig \
	board/renesas/stout/Kconfig \
	board/renesas/porter/Kconfig \
	board/renesas/silk/Kconfig \
	board/renesas/alt/Kconfig \
	board/kmc/kzm9g/Kconfig \
	board/renesas/lager/Kconfig \
	board/renesas/koelsch/Kconfig \
	board/renesas/gose/Kconfig \
	board/atmark-techno/armadillo-800eva/Kconfig \
	arch/arm/cpu/armv7/rmobile/Kconfig \
	board/LaCie/edminiv2/Kconfig \
	arch/arm/mach-orion5x/Kconfig \
	board/ti/am57xx/Kconfig \
	board/ti/dra7xx/Kconfig \
	board/ti/omap5_uevm/Kconfig \
	board/compulab/cm_t54/Kconfig \
	arch/arm/cpu/armv7/omap5/Kconfig \
	board/ti/sdp4430/Kconfig \
	board/ti/panda/Kconfig \
	board/gumstix/duovero/Kconfig \
	arch/arm/cpu/armv7/omap4/Kconfig \
	board/lge/sniper/Kconfig \
	board/quipos/cairo/Kconfig \
	board/technexion/twister/Kconfig \
	board/technexion/tao3530/Kconfig \
	board/nokia/rx51/Kconfig \
	board/logicpd/omap3som/Kconfig \
	board/htkw/mcx/Kconfig \
	board/corscience/tricorder/Kconfig \
	board/8dtech/eco5pk/Kconfig \
	board/pandora/Kconfig \
	board/ti/am3517crane/Kconfig \
	board/logicpd/zoom1/Kconfig \
	board/overo/Kconfig \
	board/isee/igep00x0/Kconfig \
	board/ti/evm/Kconfig \
	board/timll/devkit8000/Kconfig \
	board/compulab/cm_t3517/Kconfig \
	board/compulab/cm_t35/Kconfig \
	board/ti/beagle/Kconfig \
	board/teejet/mt_ventoux/Kconfig \
	board/logicpd/am3517evm/Kconfig \
	arch/arm/cpu/armv7/omap3/Kconfig \
	board/inversepath/usbarmory/Kconfig \
	arch/arm/cpu/armv7/mx5/Kconfig \
	board/warp/Kconfig \
	board/wandboard/Kconfig \
	board/udoo/Kconfig \
	board/tqc/tqma6/Kconfig \
	board/tbs/tbs2910/Kconfig \
	board/solidrun/mx6cuboxi/Kconfig \
	board/seco/Kconfig \
	board/kosagi/novena/Kconfig \
	board/gateworks/gw_ventana/Kconfig \
	board/freescale/mx6ul_14x14_evk/Kconfig \
	board/freescale/mx6sxsabresd/Kconfig \
	board/freescale/mx6slevk/Kconfig \
	board/freescale/mx6sabresd/Kconfig \
	board/freescale/mx6qsabreauto/Kconfig \
	board/freescale/mx6qarm2/Kconfig \
	board/embest/mx6boards/Kconfig \
	board/congatec/cgtqmx6eval/Kconfig \
	board/compulab/cm_fx6/Kconfig \
	board/boundary/nitrogen6x/Kconfig \
	board/barco/titanium/Kconfig \
	board/barco/platinum/Kconfig \
	board/bachmann/ot1200/Kconfig \
	board/aristainetos/Kconfig \
	arch/arm/cpu/armv7/mx6/Kconfig \
	board/freescale/mx7dsabresd/Kconfig \
	arch/arm/cpu/armv7/mx7/Kconfig \
	arch/arm/mach-mvebu/Kconfig \
	board/zyxel/nsa310s/Kconfig \
	board/Seagate/nas220/Kconfig \
	board/Seagate/goflexhome/Kconfig \
	board/Seagate/dockstar/Kconfig \
	board/raidsonic/ib62x0/Kconfig \
	board/LaCie/netspace_v2/Kconfig \
	board/LaCie/net2big_v2/Kconfig \
	board/keymile/km_arm/Kconfig \
	board/iomega/iconnect/Kconfig \
	board/d-link/dns325/Kconfig \
	board/cloudengines/pogo_e02/Kconfig \
	board/buffalo/lsxl/Kconfig \
	board/Marvell/sheevaplug/Kconfig \
	board/Marvell/guruplug/Kconfig \
	board/Marvell/dreamplug/Kconfig \
	board/Marvell/openrd/Kconfig \
	arch/arm/mach-kirkwood/Kconfig \
	board/ti/ks2_evm/Kconfig \
	arch/arm/mach-keystone/Kconfig \
	arch/arm/mach-integrator/Kconfig \
	arch/arm/mach-highbank/Kconfig \
	board/samsung/smdk5420/Kconfig \
	board/samsung/smdk5250/Kconfig \
	board/samsung/arndale/Kconfig \
	board/samsung/odroid/Kconfig \
	board/samsung/trats2/Kconfig \
	board/samsung/origen/Kconfig \
	board/samsung/universal_c210/Kconfig \
	board/samsung/trats/Kconfig \
	board/samsung/smdkv310/Kconfig \
	arch/arm/mach-exynos/Kconfig \
	board/omicron/calimain/Kconfig \
	board/davinci/ea20/Kconfig \
	board/davinci/da8xxevm/Kconfig \
	board/Barix/ipam390/Kconfig \
	arch/arm/mach-davinci/Kconfig \
	arch/arm/mach-bcm283x/Kconfig \
	board/siemens/smartweb/Kconfig \
	board/siemens/taurus/Kconfig \
	board/siemens/corvus/Kconfig \
	board/ronetix/pm9g45/Kconfig \
	board/ronetix/pm9263/Kconfig \
	board/ronetix/pm9261/Kconfig \
	board/mini-box/picosam9g45/Kconfig \
	board/esd/meesc/Kconfig \
	board/egnite/ethernut5/Kconfig \
	board/calao/usb_a9263/Kconfig \
	board/bluewater/snapper9260/Kconfig \
	board/atmel/sama5d4ek/Kconfig \
	board/atmel/sama5d4_xplained/Kconfig \
	board/atmel/sama5d3xek/Kconfig \
	board/atmel/sama5d3_xplained/Kconfig \
	board/atmel/sama5d2_xplained/Kconfig \
	board/atmel/at91sam9x5ek/Kconfig \
	board/atmel/at91sam9rlek/Kconfig \
	board/atmel/at91sam9n12ek/Kconfig \
	board/atmel/at91sam9m10g45ek/Kconfig \
	board/atmel/at91sam9263ek/Kconfig \
	board/atmel/at91sam9261ek/Kconfig \
	board/atmel/at91sam9260ek/Kconfig \
	board/atmel/at91rm9200ek/Kconfig \
	arch/arm/mach-at91/Kconfig \
	arch/arm/Kconfig \
	board/synopsys/axs101/Kconfig \
	board/synopsys/Kconfig \
	board/abilis/tb100/Kconfig \
	arch/arc/Kconfig \
	arch/Kconfig \
	Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(UBOOTVERSION)" "2016.01"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
