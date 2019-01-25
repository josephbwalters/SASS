#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /Users/trent/ti/tirtos_msp43x_2_20_00_06/packages;/Users/trent/ti/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages;/Users/trent/ti/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages;/Users/trent/ti/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages;/Users/trent/Development/SASS/SASS/.config
override XDCROOT = /Users/trent/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /Users/trent/ti/tirtos_msp43x_2_20_00_06/packages;/Users/trent/ti/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages;/Users/trent/ti/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages;/Users/trent/ti/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages;/Users/trent/Development/SASS/SASS/.config;/Users/trent/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = MacOS
endif
