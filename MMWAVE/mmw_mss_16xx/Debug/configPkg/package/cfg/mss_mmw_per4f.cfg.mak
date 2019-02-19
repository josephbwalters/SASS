# invoke SourceDir generated makefile for mss_mmw.per4f
mss_mmw.per4f: .libraries,mss_mmw.per4f
.libraries,mss_mmw.per4f: package/cfg/mss_mmw_per4f.xdl
	$(MAKE) -f C:\Users\jwalters\Documents\ti_workspace\mmw_mss_16xx/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\jwalters\Documents\ti_workspace\mmw_mss_16xx/src/makefile.libs clean

