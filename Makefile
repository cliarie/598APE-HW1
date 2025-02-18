FUNC := g++
copt := -c 
OBJ_DIR := ./bin/
FLAGS := -O3 -g -Werror -std=c++17 -I$(shell brew --prefix tbb)/include
LDFLAGS := -O3 -g -Werror -std=c++17 -L$(shell brew --prefix tbb)/lib -ltbb -lm

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR),$(notdir $(CPP_FILES:.cpp=.obj)))

TEXTURE_CPP_FILES := $(wildcard src/Textures/*.cpp)
TEXTURE_OBJ_FILES := $(addprefix $(OBJ_DIR)Textures/,$(notdir $(TEXTURE_CPP_FILES:.cpp=.obj)))

all:
	cd ./src && make
	$(FUNC) ./main.cpp -o ./main.exe ./src/*.obj ./src/Textures/*.obj $(FLAGS) $(LDFLAGS)

clean:
	cd ./src && make clean
	rm -f ./*.exe
	rm -f ./*.obj
