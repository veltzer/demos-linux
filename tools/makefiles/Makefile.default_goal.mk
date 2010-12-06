# this examples shows how to set the default goal in GNU make

.DEFAULT_GOAL=all

echo:
	echo $(.DEFAULT_GOAL)

dummy:
	@echo dummy

all:
	@echo all 
