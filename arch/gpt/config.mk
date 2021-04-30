#
# SPDX-License-Identifier:	GPL-2.0+
#

ifeq ($(CROSS_COMPILE),)
	CROSS_COMPILE := gpt-
endif

64bit-emul		:= elf64gptx
64bit-bfd		:= gpt-ld.bfd
PLATFORM_CPPFLAGS	+= -D__GPTEL__

PLATFORM_LDFLAGS	+= -m$(64bit-emul)
OBJCOPYFLAGS		+= -O $(64bit-bfd)


PLATFORM_CPPFLAGS += -D__GPT__ -D_GPT_SZLONG=64 

CPPASM  = $(GPCC) -E -x assembler-with-cpp 

#PLATFORM_CPPFLAGS+=-DDEBUG

PLATFORM_CPPFLAGS               += -I$(GPTXROOT)/tools/gptx-linux-gnu/include
PLATFORM_CPPFLAGS		+= -mdisable-floating-varargs -DMARCH_CHIP1=1 -DMARCH_CHIP2=2 -DUNIMPLEMENTED_MARCH_VERSION=MARCH_CHIP2
PLATFORM_LDFLAGS		+= -static -n -nostdlib  
PLATFORM_RELFLAGS		+= -ffunction-sections -fdata-sections
ifndef CONFIG_SPL_SKIP_RELOCATE
LDFLAGS_FINAL			+= --pie   
endif
LDFLAGS_FINAL			+= --gc-sections  
OBJCOPYFLAGS			+= -j .text -j .rodata -j .data -j .got -j .u_boot_list -j .rela.dyn
