ifeq ($(MAKECMDGOALS),clean)
all:
	echo yes
clean:
	echo yes
else
all:
	echo no
clean:
	echo no
endif

ifeq ($(filter opt1 opt2,$(MAKECMDGOALS)),)
$(info doing the thing)
endif

opt1:
	@echo $(filter opt1 opt2,$(MAKECMDGOALS))
opt2:
	@echo $(filter opt1 opt2,$(MAKECMDGOALS))
foobar:
	@echo $(filter opt1 opt2,$(MAKECMDGOALS))
