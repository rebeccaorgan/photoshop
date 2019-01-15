CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CC = g++
CC_FLAGS := -c -g -Wall -Wformat-nonliteral -ggdb -funsigned-char -fno-exceptions -std=c++1y -MMD
LD_FLAGS := -g -pthread -pg -std=c++1y -rdynamic

.PHONY: clean

# create required directories then compile all .cpp files into .o files
all: compile_dirs $(OBJ_FILES) main

# generates all required directories if they don't yet exist
compile_dirs:
	mkdir -p obj

# compile all .cpp files into their respective obj files in obj
obj/%.o: %.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

main: $(OBJ_FILES)
	$(CC) -o $@ $(LD_FLAGS) $^

# cleanup
clean:
	rm -rf obj main gmon.out

# not sure how this works, but this inspects the dependency graph and rebuilds
# everything that depended on the change
-include $(OBJ_FILES:.o=.d)

