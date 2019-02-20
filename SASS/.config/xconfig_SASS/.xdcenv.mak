#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /Users/jwalters/ti/simplelink_msp432p4_sdk_2_40_00_10/source;/Users/jwalters/ti/simplelink_msp432p4_sdk_2_40_00_10/kernel/tirtos/packages;/Users/jwalters/Documents/school/school2019/SASS/SASS/.config
override XDCROOT = /Users/jwalters/ti/xdctools_3_51_01_18_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /Users/jwalters/ti/simplelink_msp432p4_sdk_2_40_00_10/source;/Users/jwalters/ti/simplelink_msp432p4_sdk_2_40_00_10/kernel/tirtos/packages;/Users/jwalters/Documents/school/school2019/SASS/SASS/.config;/Users/jwalters/ti/xdctools_3_51_01_18_core/packages;..
HOSTOS = MacOS
endif
