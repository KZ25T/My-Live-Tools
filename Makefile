INC_DIR = ./inc
SRC_DIR = ./src

SRC = $(wildcard $(SRC_DIR)/*.cpp)     # /*/
OBJ = $(patsubst %.cpp, $(SRC_DIR)/%.o, $(notdir $(SRC)))

TARGET = mlt

PLATFORM = debian
CXX = g++
CXXFLAGS = --static -lblkid -std=c++20 -Wall -Wextra -Os -DPLATFORM=\"$(PLATFORM)\"

$(TARGET):$(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) -o $@
	strip $(TARGET)

$(SRC_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	find $(SRC_DIR) -name *.o -exec rm {} \;
	rm $(TARGET)
