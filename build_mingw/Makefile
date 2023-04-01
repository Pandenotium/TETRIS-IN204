APPNAME := Tetris
SOURCE_DIR   := src
LIB_DIR	 := $(wxWidgets)\lib\gcc_dll
INCLUDE_DIR  := -I$(wxWidgets)\include -I$(LIB_DIR)\mswu
OBJECT_DIR   := obj

CC := g++
CCFLAGS := -g -Wall

SRCFILES     := $(wildcard $(SOURCE_DIR)/*.cpp)
INCLUDES     := $(INCLUDE_DIR)
OBJECTS      := $(SRCFILES:$(SOURCE_DIR)/%.cpp=$(OBJECT_DIR)/%.o)
LIBS		 := $(wildcard $(LIB_DIR)/*.a)

all: $(APPNAME)

$(APPNAME): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	-if exist $(OBJECT_DIR)\*.o del $(OBJECT_DIR)\*.o
	-if exist $(APPNAME).exe del $(APPNAME).exe