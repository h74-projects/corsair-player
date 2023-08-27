CXX = g++
CC = $(CXX)

CFLAGS = -Wall -Werror -Wextra -pedantic -g3
CXXFLAGS = $(CFLAGS) -std=c++17

INC_DIR = ../../inc
SRC_DIR = ../../src

CPPFLAGS = -I$(INC_DIR)
#LDLIBS = 

TARGET = downloader


OBJECTS = $(SRC_DIR)/downloader.o main.o

all: $(TARGET)

check: $(TARGET)
	@./$(TARGET) -v

recheck: clean check

valgrind: $(TARGET)
	valgrind --log-file=memleak.log --leak-check=full --track-origins=yes -s ./$<

clean:
	@$(RM) $(TARGET) $(OBJECTS)

.PHONY: clean all

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(CPPFLAGS) $(LDLIBS)

# Dependencies
main.o: $(INC_DIR)/downloader.hpp
$(SRC_DIR)/downloader.o: $(INC_DIR)/downloader.hpp

run-selenium:
	python3 seleniumScript.py
