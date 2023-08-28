CXX = g++
CC = $(CXX)

CFLAGS = -Wall -Werror
CFLAGS+= -Wextra
CFLAGS+= -pedantic
CFLAGS+= -g3

CXXFLAGS = $(CFLAGS)
CXXFLAGS += -std=c++17

INCLUDES_DIR = ../../inc
SOURCES_DIR = ../../src

TARGET = main 

LDLIBS = -lsqlite3
LDLIBS += -lPocoFoundation -lPocoUtil
LDLIBS += -lcpprest -lssl -lcrypto

# flags for preprocessor
CPPFLAGS = -I$(INCLUDES_DIR) 

OBJECTS = main.o \
		$(SOURCES_DIR)/gpt_requests.o\
		$(SOURCES_DIR)/sql_mng.o\
		$(SOURCES_DIR)/song.o\
		$(SOURCES_DIR)/parser.o\
		$(SOURCES_DIR)/executer.o\
		$(SOURCES_DIR)/songs_controller.o\
		$(SOURCES_DIR)/downloader.o



$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LDLIBS)

all: $(OBJECTS) 

check : $(TARGET)
	@./$(TARGET) -v

recheck: clean check


val: $(TARGET)
	valgrind  --leak-check=full ./main


clean:
	@$(RM) main $(OBJECTS)

.PHONY: clean all


# dependencies
main.o: 
$(SOURCES_DIR)/gpt_requests.o: $(INCLUDES_DIR)/gpt_requests.hpp
$(SOURCES_DIR)/sql_mng.o: $(INCLUDES_DIR)/sql_mng.hpp
$(SOURCES_DIR)/song.o: $(INCLUDES_DIR)/song.hpp
$(SOURCES_DIR)/parser.o: $(INCLUDES_DIR)/parser.hpp
$(SOURCES_DIR)/executer.o: $(INCLUDES_DIR)/executer.hpp
$(SOURCES_DIR)/songs_controller.o: $(INCLUDES_DIR)/songs_controller.hpp
$(SOURCES_DIR)/downloader.o: $(INCLUDES_DIR)/downloader.hpp






