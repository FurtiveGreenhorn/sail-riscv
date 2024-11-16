OBJ = .o
LIB = .a

# Sail command to use.
SAIL := sail

SOURCE_DIR = ..
SAIL_DIR := $(shell sail --dir)
SAIL_LIB_DIR := $(SAIL_DIR)/lib

DELETE = rm -f
C_INCLUDES = -I$(SOURCE_DIR)
COMPILE_CXX = g++ -g -I$(SAIL_LIB_DIR) -c $(C_INCLUDES) -O2 -o $@
MAKELIB = ar crs $@

OBJS_PRIMITIVES = riscv_cpp_test$(OBJ) riscv_cache$(OBJ)
OBJS_OTHERS = 

OBJS_ALL = $(OBJS_PRIMITIVES) $(OBJS_OTHERS)

cache$(LIB): $(OBJS_ALL)
	rm -f $@
	$(MAKELIB) $^

$(OBJS_PRIMITIVES): %$(OBJ): $(SOURCE_DIR)/%.cpp
	$(COMPILE_CXX) $(SOURCE_DIR)/$*.cpp

# $(SAIL_OBJS_DIR)/cpp_test.o: c_emulator/Cache/riscv_cpp_test.cpp
# 	g++ -g -I$(SAIL_LIB_DIR) -c $< -o $@
	
.PHONY: clean
clean:
	$(DELETE) $(OBJS_PRIMITIVES) cache$(LIB)