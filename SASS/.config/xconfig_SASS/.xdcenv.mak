#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /Users/trent/ti/simplelink_msp432p4_sdk_2_40_00_10/source;/Users/trent/ti/simplelink_msp432p4_sdk_2_40_00_10/kernel/tirtos/packages;/Users/trent/workspace_v8/SASS/.config
override XDCROOT = /Users/trent/ti/xdctools_3_51_01_18_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /Users/trent/ti/simplelink_msp432p4_sdk_2_40_00_10/source;/Users/trent/ti/simplelink_msp432p4_sdk_2_40_00_10/kernel/tirtos/packages;/Users/trent/workspace_v8/SASS/.config;/Users/trent/ti/xdctools_3_51_01_18_core/packages;..
HOSTOS = MacOS
endif
