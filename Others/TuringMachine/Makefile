CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Iinclude
DEPFLAGS := -MMD -MP

TARGET    := tm
SRC_DIR   := src
BUILD_DIR := build

SRCS := main.cpp $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))
DEPS := $(OBJS:.o=.d)

vpath %.cpp . $(SRC_DIR)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)
