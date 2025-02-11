FUNC := g++
#copt := -c 
OBJ_DIR := ./bin/
FLAGS := -O3 -lm -g -Werror -std=c++17

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR),$(notdir $(CPP_FILES:.cpp=.o)))

TEXTURE_CPP_FILES := $(wildcard src/Textures/*.cpp)
TEXTURE_OBJ_FILES := $(addprefix $(OBJ_DIR)Textures/,$(notdir $(TEXTURE_CPP_FILES:.cpp=.o)))

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)Textures

$(OBJ_DIR)%.o: src/%.cpp | $(OBJ_DIR)
	$(FUNC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)Textures/%.o: src/Textures/%.cpp | $(OBJ_DIR)
	$(FUNC) $(FLAGS) -c $< -o $@

all: $(OBJ_FILES) $(TEXTURE_OBJ_FILES)	
	$(FUNC) ./main.cpp -o ./main.exe $(OBJ_FILES) $(TEXTURE_OBJ_FILES) $(FLAGS)

clean:
	rm -f main.exe $(OBJ_DIR)*.o $(OBJ_DIR)Textures/*.o
