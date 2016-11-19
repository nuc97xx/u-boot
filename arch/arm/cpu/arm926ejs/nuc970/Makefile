#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y	= cpu.o reset.o

ifndef CONFIG_SKIP_LOWLEVEL_INIT
obj-y   += lowlevel_init.o
endif

ifndef CONFIG_SPL_BUILD
#obj-y	+= relocate
endif
