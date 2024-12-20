SRC_DIR:=src

SOURCES=$(wildcard $(SRC_DIR)/api_impl/*.cpp)

all:
	g++ -shared -o libgauss_blur.dylib $(SRC_DIR)/filters/gauss_blur.cpp $(SOURCES) $(SRC_DIR)/control_panel.cpp $(SRC_DIR)/menu_items.cpp -I$(SRC_DIR)
