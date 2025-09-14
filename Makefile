CXX = g++
CXXFLAGS = -std=c++17 -Wall
IMGVIEWER = gwenview

DIR := $(CURDIR)

SRC = $(wildcard $(DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
IMG = $(DIR)/image.ppm
BIN = $(DIR)/gen.out

all: $(IMG)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN)

$(IMG): $(BIN)
	$(BIN) > $(IMG)

view: $(IMG)
	nohup $(IMGVIEWER) $(IMG) >/dev/null 2>&1 &

clean:
	rm -f $(OBJ) $(IMG) $(BIN)
