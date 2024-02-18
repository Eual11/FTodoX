OBJ = ./src/test.cpp

INCLUDE_PATH = -I{CPLUS_INCLUDE_PATH}
LIB_PATH = -L{C_LIBRARY_PATH}

 # COMPILER_FLAGS = -w -Wl,-subsystem,windows
COMPILER_FLAGS =  -g -Wall -Wextra -Wpedantic -std=c++20

OBJ_NAME = e
# LINKER_FLAGS =  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
LINKER_FLAGS = -lmingw32 #-lftxui-dom.obj -lftxui-screen -lftxui-component
# LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
# LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc
CC =g++


all: $(OBJ)
	$(CC) $(OBJ) $(INCLUDE_PATH) $(LIB_PATH) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ./build/$(OBJ_NAME)


