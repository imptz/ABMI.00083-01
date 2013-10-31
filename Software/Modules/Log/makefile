cc = g++-4.7
cflags = -Wall -std=gnu++11 -g
ldflags = -g -pthread
source_dirs := . ../Base/Sources Sources ../Dispatcher/Sources
include_dirs := /usr/include/mysql
lib_dirs := /usr/lib/mysql

libs := mysqlclient

search_wildcards := $(addsuffix /*.cpp,$(source_dirs)) 

log: $(notdir $(patsubst %.cpp,%.o,$(wildcard $(search_wildcards))))
	$(cc) $(ldflags) $^ -I$(include_dirs) -L$(lib_dirs) -l$(libs)  -o $@ 

VPATH := $(source_dirs)

%.o: %.cpp
	$(cc) $(cflags) -c -MD $(addprefix -I,$(source_dirs)) -I$(include_dirs) -L$(lib_dirs) -l$(libs) $<

include $(wildcard *.d) 

clean:
	rm -f *o *d
