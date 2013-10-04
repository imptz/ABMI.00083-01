cc = g++-4.7
cflags = -Wall -std=gnu++11 -g
ldflags = -g -pthread
source_dirs := . ../Sources ../../Dispatcher/Sources

search_wildcards := $(addsuffix /*.cpp,$(source_dirs)) 

dispatcherTest: $(notdir $(patsubst %.cpp,%.o,$(wildcard $(search_wildcards))))
	$(cc) $(ldflags) $^ -o $@ 

VPATH := $(source_dirs)

%.o: %.cpp
	$(cc) $(cflags) -c -MD $(addprefix -I,$(source_dirs)) $<

include $(wildcard *.d) 