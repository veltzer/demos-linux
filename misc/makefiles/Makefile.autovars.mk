test: demo_folder/source.file 
	$(info $$@ is $@)
	$(info $$^ is $^)

.PHONY: all
all: test
	$(info doing all)
