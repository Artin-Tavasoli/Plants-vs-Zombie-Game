.DEFAULT_GOAL := all

GPP = g++
GPPFLAGS = -std=c++2a -I./header
LFLAGS = -L/files -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR = src
OBJ_DIR = obj
EXES = ./pvz.out
MEDIA_PATH = ./images/

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

all: $(EXES)

$(EXES): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(GPP) $(GPPFLAGS) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(SRC_DIR)/*.hpp)
	@mkdir -p $(OBJ_DIR)
	$(GPP) $(GPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -f $(EXES)
