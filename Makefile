CXX = g++
CXXFLAGS = -std=c++17 -Wall
DIR = ~/ray-tracing/
IMGVIEWER = gwenview

$(DIR)/all: $(DIR)/image.ppm

$(DIR)/gen.out: $(DIR)/gen.cpp
	$(CXX) $(CXXFLAGS) $(DIR)/gen.cpp -o $(DIR)/gen.out

$(DIR)/image.ppm: $(DIR)/gen.out
	$(DIR)/gen.out > $(DIR)/image.ppm

view: $(DIR)/image.ppm
	nohup $(IMGVIEWER) $(DIR)/image.ppm >/dev/null 2>&1 &
clean:
	rm -f $(DIR)/gen.out $(DIR)/image.ppm
