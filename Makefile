DIR_Config   = ./lib/Config
DIR_OLED     = ./lib/OLED
DIR_FONTS    = ./lib/Fonts
DIR_GUI      = ./lib/GUI
DIR_Function = ./lib/Function
DIR_BIN      = ./bin
DIR_TEST     = ./test
DIR_WEB 	 = ./Web
#C++ files
SRC_CPP = $(wildcard ${DIR_OLED}/*.cpp ${DIR_Config}/*.cpp ${DIR_GUI}/*.cpp ${DIR_Function}/*.cpp ${DIR_FONTS}/*.cpp ${DIR_TEST}/*.cpp ${DIR_WEB}/*.cpp)
OBJ_CPP = $(patsubst %.cpp,${DIR_BIN}/%.o,$(notdir ${SRC_CPP}))



TARGET = main

#USELIB = USE_BCM2835_LIB
#USELIB = USE_WIRINGPI_LIB
USELIB = USE_DEV_LIB
DEBUG = -D $(USELIB)
ifeq ($(USELIB), USE_BCM2835_LIB)
    LIB = -lbcm2835 -lm 
else ifeq ($(USELIB), USE_WIRINGPI_LIB)
    LIB = -lwiringPi -lm 
else ifeq ($(USELIB), USE_DEV_LIB)
    LIB =  -lwiringPi -lm 
endif


CXX = g++ -std=c++11
MSG = -g -O0 -Wall -pthread  -lboost_system


CXXFLAGS += $(MSG) $(DEBUG)

${TARGET}:${OBJ_CPP}
	$(CXX) $(CXXFLAGS) $(OBJ_CPP) -o $@ $(LIB)
	
${DIR_BIN}/%.o:$(DIR_TEST)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_OLED) -I $(DIR_Function)

${DIR_BIN}/%.o:$(DIR_OLED)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(DIR_Config)

${DIR_BIN}/%.o:$(DIR_FONTS)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${DIR_BIN}/%.o:$(DIR_GUI)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(DIR_Config) -I $(DIR_OLED) -I $(DIR_TEST)

${DIR_BIN}/%.o:$(DIR_Config)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIB)
	
${DIR_BIN}/%.o:$(DIR_Function)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

${DIR_BIN}/%.o:$(DIR_WEB)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(DIR_Function)


clean :
	rm $(DIR_BIN)/*.*
	rm $(TARGET)
