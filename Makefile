# ====================================================================
# PROJECT CONFIGURATION
# ====================================================================

# Compiler and Tools
CC = gcc
EMAR = emar rcs

# Directories
SRC_DIR    = src
INC_DIR    = include
BIN_DIR    = bin
OBJ_DIR    = obj
TEST_DIR   = tests

# Object Directories (Separated for different build types)
OBJ_DIR_NATIVE = obj/native
OBJ_DIR_SHARED = obj/shared
OBJ_DIR_EM     = obj/em

#Flags
CINC = -I$(INC_DIR)/
LDLIBS = -lm

C_FLAGS = -std=c99 -Wall -Wextra $(CINC)
NATIVE_C_FLAGS = $(C_FLAGS) -ggdb
SHARED_C_FLAGS = $(C_FLAGS) -fPIC -pedantic

# ====================================================================
# DAEDALUS LIBRARY OBJECTS (Core C Files)
# ====================================================================

DAEDALUS_SRCS = dArrays.c\
								dDUFIO.c\
								dDUFLexer.c\
								dDUFParser.c\
								dDUFQuery.c\
								dDUFValue.c\
								dFunctions.c\
								dKinematicBody.c\
								dLinkedList.c\
								dLogs.c\
								dMatrixMath.c\
								dQuadTree.c\
								dQuaternion.c\
								dStaticArrays.c\
								dStaticTables.c\
								dStrings-dArrays.c\
								dStrings.c\
								dTables.c\
								dTransform.c\
								dVectorMath.c\

NATIVE_LIB_OBJS = $(patsubst %.c, $(OBJ_DIR_NATIVE)/%.o, $(DAEDALUS_SRCS))
SHARED_LIB_OBJS = $(patsubst %.c, $(OBJ_DIR_SHARED)/%.o, $(DAEDALUS_SRCS))
TEST_OBJS = $(patsubst %.c, $(OBJ_DIR_NATIVE)/%.o, $(DAEDALUS_SRCS))
EMCC_LIB_OBJS = $(patsubst %.c, $(OBJ_DIR_EM)/%.o, $(DAEDALUS_SRCS))

MAIN_OBJ = $(OBJ_DIR_NATIVE)/main.o
TEST_DARRAY_OBJ = $(OBJ_DIR_NATIVE)/test_dArrays.o
TEST_DUF_OBJ = $(OBJ_DIR_NATIVE)/test_duf.o
TEST_EDGE_OBJ = $(OBJ_DIR_NATIVE)/test_edge_cases.o

NATIVE_EXE_OBJS = $(NATIVE_LIB_OBJS) $(MAIN_OBJ)

DARRAY_EXE_OBJS = $(TEST_OBJS) $(TEST_DARRAY_OBJ)
DUF_EXE_OBJS = $(TEST_OBJS) $(TEST_DUF_OBJ)
EDGE_EXE_OBJS = $(TEST_OBJS) $(TEST_EDGE_OBJ)

# ====================================================================
# PHONY TARGETS
# ====================================================================

.PHONY: all shared install uninstall clean bear bearclean verify test_duf
.PHONY: test_dArrays test_edge_cases test_duf_all

all: $(BIN_DIR)/native
shared: $(BIN_DIR)/libDaedalus.so

verify:
	./verify_architecture.sh

# Test target for DUF parser
test_duf: $(BIN_DIR)/test_duf $(BIN_DIR)/test_edge_cases
	$(BIN_DIR)/test_duf
	$(BIN_DIR)/test_edge_cases

test_dArrays: $(BIN_DIR)/test_dArrays

# Test target for DUF edge cases
test_edge_cases: $(BIN_DIR)/test_edge_cases
	$(BIN_DIR)/test_edge_cases

# Run all DUF tests
test_duf_all: test_duf test_edge_cases

# ====================================================================
# DIRECTORY & UTILITY RULES
# ====================================================================

# Ensure the directories exist before attempting to write files to them
$(BIN_DIR) $(OBJ_DIR_NATIVE) $(OBJ_DIR_SHARED) $(OBJ_DIR_EM):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR_SHARED)
	@clear

install:
	sudo cp $(BIN_DIR)/libDaedalus.so /usr/lib/
	sudo cp $(INC_DIR)/Daedalus.h /usr/include/

uninstall:
	sudo rm /usr/lib/libDaedalus.so
	sudo rm /usr/include/Daedalus.h

bear:
	bear -- make

bearclean:
	rm compile_commands.json

# ====================================================================
# COMPILATION RULES
# ====================================================================

$(OBJ_DIR_SHARED)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_SHARED)
	$(CC) -c $< -o $@ $(SHARED_C_FLAGS)

$(OBJ_DIR_NATIVE)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_NATIVE)
	$(CC) -c $< -o $@ $(NATIVE_C_FLAGS)

$(OBJ_DIR_NATIVE)/main.o: $(SRC_DIR)/main.c | $(OBJ_DIR_NATIVE)
	$(CC) -c $< -o $@ $(NATIVE_C_FLAGS)

$(OBJ_DIR_NATIVE)/test_dArrays.o: $(TEST_DIR)/test_dArrays.c | $(OBJ_DIR_NATIVE)
	$(CC) -c $< -o $@ $(NATIVE_C_FLAGS)

$(OBJ_DIR_NATIVE)/test_duf.o: $(TEST_DIR)/test_duf.c | $(OBJ_DIR_NATIVE)
	$(CC) -c $< -o $@ $(NATIVE_C_FLAGS)

$(OBJ_DIR_NATIVE)/test_edge_cases.o: $(TEST_DIR)/test_edge_cases.c | $(OBJ_DIR_NATIVE)
	$(CC) -c $< -o $@ $(NATIVE_C_FLAGS)

# ====================================================================
# LINKING RULES
# ====================================================================

# Target: Native Executable
$(BIN_DIR)/native: $(NATIVE_EXE_OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(NATIVE_C_FLAGS) $(LDLIBS)

$(BIN_DIR)/libDaedalus.so: $(SHARED_LIB_OBJS) | $(BIN_DIR)
	$(CC) -shared $^ -o $@

$(BIN_DIR)/libArchimedes.a: $(EMCC_LIB_OBJS) | $(BIN_DIR)
	$(EMAR) $@ $^

$(BIN_DIR)/test_duf: $(DUF_EXE_OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(NATIVE_C_FLAGS) $(LDLIBS)

$(BIN_DIR)/test_dArrays: $(DARRAY_EXE_OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(NATIVE_C_FLAGS) $(LDLIBS)

$(BIN_DIR)/test_edge_cases: $(EDGE_EXE_OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(NATIVE_C_FLAGS) $(LDLIBS)

