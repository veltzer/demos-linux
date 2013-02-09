CXXFLAGS:=-I../../include -O2 -Wall -Werror

main_phase2.$(SUFFIX_BIN): main_phase2.o
	$(Q)g++ -o $@ $<
main_phase2.o: main.cc main.gcda
	$(Q)g++ $(CXXFLAGS) -fprofile-use -c -o main.o main.cc
	$(Q)mv main.o main_phase2.o
main.gcda main_run.stamp: main_phase1.$(SUFFIX_BIN)
	$(Q)./main_phase1.$(SUFFIX_BIN) 1> /dev/null 2> /dev/null
	$(Q)touch main_run.stamp
main_phase1.$(SUFFIX_BIN): main.o
	$(Q)g++ -fprofile-generate -o $@ $<
main.o: main.cc
	$(Q)g++ $(CXXFLAGS) -fprofile-generate -c -o main.o main.cc
.PHONY: clean
clean:
	$(Q)-rm -f main_phase2.o main.gcda main.o main.$(SUFFIX_BIN) main_phase1.$(SUFFIX_BIN) main_run.stamp
