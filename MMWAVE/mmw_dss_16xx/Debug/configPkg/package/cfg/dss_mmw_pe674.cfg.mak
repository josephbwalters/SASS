# invoke SourceDir generated makefile for dss_mmw.pe674
dss_mmw.pe674: .libraries,dss_mmw.pe674
.libraries,dss_mmw.pe674: package/cfg/dss_mmw_pe674.xdl
	$(MAKE) -f C:\Users\jwalters\Documents\ti_workspace\mmw_dss_16xx/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\jwalters\Documents\ti_workspace\mmw_dss_16xx/src/makefile.libs clean

