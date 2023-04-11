DIR_Config   = ./lib/Config
DIR_OLED     = ./lib/OLED
DIR_FONTS    = ./lib/Fonts
DIR_GUI      = ./lib/GUI
DIR_Function = ./lib/Function
DIR_BIN      = ./bin
DIR_MAIN     = ./main
#C++ files
OBJ_CPP = $(wildcard ${DIR_OLED}/*.cpp ${DIR_Config}/*.cpp ${DIR_GUI}/*.cpp ${DIR_Function}/*.cpp ${DIR_FONTS}/*.cpp ${DIR_MAIN}/*.cpp)
OBJ_CPP = $(patsubst %.cpp,${DIR_BIN}/%.o,$(notdir ${OBJ_CPP}))


#C files
OBJ_C = $(wildcard ${DIR_OLED}/*.c ${DIR_Config}/*.c ${DIR_GUI}/*.c ${DIR_Function}/*.c ${DIR_FONTS}/*.c ${DIR_MAIN}/*.c)
OBJ_C = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

TARGET = main

#USELIB = USE_BCM2835_LIB
USELIB = USE_WIRINGPI_LIB
#USELIB = USE_DEV_LIB
DEBUG = -D $(USELIB)
ifeq ($(USELIB), USE_BCM2835_LIB)
    LIB = -lbcm2835 -lm 
else ifeq ($(USELIB), USE_WIRINGPI_LIB)
    LIB = -lwiringPi -lm 
else ifeq ($(USELIB), USE_DEV_LIB)
    LIB = -lm 
endif


CXX = g++
CC = gcc
LD = g++
MSG = -g -O0 -Wall


CXXFLAGS += $(MSG) $(DEBUG)
CFLAGS += $(MSG) $(DEBUG)

${TARGET}: $(OBJ_C) $(OBJ_CPP)
	$(LD) $(LDFLAGS) $(OBJ_C) $(OBJ_CPP) -o $@ $(LIB)
${DIR_BIN}/%.o:$(DIR_MAIN)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_OLED) -I $(DIR_Function)

${DIR_BIN}/%.o:$(DIR_OLED)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(DIR_Config)

${DIR_BIN}/%.o:$(DIR_FONTS)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${DIR_BIN}/%.o:$(DIR_GUI)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(DIR_Config) -I $(DIR_OLED) -I $(DIR_MAIN)

${DIR_BIN}/%.o:$(DIR_Config)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIB)
${DIR_BIN}/%.o:$(DIR_Function)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(DIR_MAIN)


	
${DIR_BIN}/%.o:$(DIR_MAIN)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_OLED)

${DIR_BIN}/%.o:$(DIR_OLED)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DIR_Config)

${DIR_BIN}/%.o:$(DIR_FONTS)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_BIN}/%.o:$(DIR_GUI)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DIR_Config) -I $(DIR_OLED) -I $(DIR_Function)

${DIR_BIN}/%.o:$(DIR_Config)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIB)
${DIR_BIN}/%.o:$(DIR_Function)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(DIR_MAIN)

clean :
	rm $(DIR_BIN)/.
	rm $(TARGET)
