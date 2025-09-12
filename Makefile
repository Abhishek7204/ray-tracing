CXX = g++
CXXFLAGS = -std=c++17 -Wall
DIR = /home/abhi/ray-tracing
IMGVIEWER = gwenview

SRC = $(wildcard $(DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
IMG = $(DIR)/image.ppm

all: $(IMG)

$(IMG): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(DIR)/.gen_tmp.out
	$(DIR)/.gen_tmp.out > $(IMG)
	rm -f $(DIR)/.gen_tmp.out

view: $(IMG)
	nohup $(IMGVIEWER) $(IMG) >/dev/null 2>&1 &

clean:
	rm -f $(DIR)/*.o $(IMG) $(DIR)/.gen_tmp.out
