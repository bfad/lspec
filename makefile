
PROJECT := lspec
SRC_EXT := inc

all: command/$(PROJECT).bin lib
	mv $< $(PROJECT)

install: $(PROJECT).bin.install lib.install

clean::
	@rm -f $(PROJECT) command/$(PROJECT).bin

include makefile-engine
