CXX=g++
CXX_FLAGS=-O3 -DNDEBUG

TARGET = out

SRCS = main.cc

IMAGE_DIR = images

SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

$(TARGET): $(SRCS)
	@$(CXX) $(CXX_FLAGS) -o $(TARGET) $(SRCS) $(SFML_LIBS)

clean:
	@rm -f $(TARGET) $(IMAGE_DIR)/$(TARGET).png $(IMAGE_DIR)/$(TARGET).ppm

run: $(TARGET)
	@./$(TARGET) > $(TARGET).ppm
	@mv out.ppm $(IMAGE_DIR)/
	@mv out.png $(IMAGE_DIR)/

all: clean $(TARGET) run

install:
	sudo apt-get install $(CXX) libsfml-dev

uninstall:
	sudo apt-get remove libsfml-dev