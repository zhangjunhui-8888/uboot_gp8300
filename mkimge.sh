make -j8
./tools/mkimage  -A gpt -O u-boot -C none  -a 0xc000000 -e 0xc000000 -d u-boot.bin  u-boot.img
dd if=spl/u-boot-spl.bin of=spl-out.bin bs=128K conv=sync && cat spl-out.bin u-boot.img > out.bin && dd if=out.bin of=uboot_gp8300_dvb.bin bs=1M conv=sync

if [ -f "monitor.bin.ihex" ];then
	cat monitor.bin.ihex >> uboot_gp8300_dvb.bin
fi
rm spl-out.bin out.bin
