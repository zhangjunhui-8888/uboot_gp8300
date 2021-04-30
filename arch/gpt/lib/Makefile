 # arch/gpt/cpu/start.S
 #  #
 # GPT boot startup code.
 #  #
 # Copyright (C) 2016, General Processor Techologies Inc.
 #  # xiesc <xiesc@hxgpt.com>
 #
 # # This program is free software; you can redistribute it and/or modify
 #  # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation; either version 2 of the License, or
 #  # (at your option) any later version.
 #
 # # This program is distributed in the hope that it will be useful,
 #  # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #  # GNU General Public License for more details.
 #
 # # You should have received a copy of the GNU General Public License
 #  # along with this program; if not, see the file COPYING, or write
 # to the Free Software Foundation, Inc.,
 #  # 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

obj-y	+= cache.o cache_init.o relocate.o sections.o 

obj-$(CONFIG_CMD_BOOTM) += bootm.o 

#lib-$(CONFIG_USE_PRIVATE_LIBGCC) += ashldi3.o ashrdi3.o lshrdi3.o libgcc.o
#lib-$(CONFIG_USE_PRIVATE_LIBGCC) +=  libgcc2.o
lib-$(CONFIG_USE_PRIVATE_LIBGCC) +=  libgcc.o

obj-$(CONFIG_USE_ARCH_STRCPY) += strcpy.o
obj-$(CONFIG_USE_ARCH_STRLEN) += strlen.o
obj-$(CONFIG_USE_ARCH_STRCMP) += strcmp.o
obj-$(CONFIG_USE_ARCH_MEMSET) += memset.o
obj-$(CONFIG_USE_ARCH_MEMCPY) += memcpy.o
obj-$(CONFIG_USE_ARCH_MEMCMP) += memcmp.o

ifdef	CONFIG_SPL_BUILD
obj-y += spl.o
endif
