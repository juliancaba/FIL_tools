PRJ = hls_prj

.PHONY: all clean export


all: export

export:
	vivado_hls script.tcl


clean:
	$(RM) -rf *~ *.log $(PRJ)
