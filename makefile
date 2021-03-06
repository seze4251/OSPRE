#
#	 Makefile
#	 OSPRE
#
#	 Created by Hilton Lipschitz on 2015-09-01.
#	 Copyright (c) 2015 Maritime Capital LP. All rights reserved.
#
#        Modified By Seth Zegelstein
#        Date Modified: 11/26/2016

# HIL: No spaces or comments after otherwise it captures them!
# Determine the platform

UNAME_S := $(shell uname -s)

# CC
ifeq ($(UNAME_S),Darwin)
  CC := clang++ -arch x86_64
else
  CC := g++
endif


# Folders
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin

# Targets (6 executables)
#WATCHDOG
EXECUTABLE_WatchDog := WatchDog
TARGET_WatchDog := $(TARGETDIR)/$(EXECUTABLE_WatchDog)

#SCCOMMS
EXECUTABLE_ScComms := ScComms
TARGET_ScComms := $(TARGETDIR)/$(EXECUTABLE_ScComms)

#CAMERA CONTROLLER
EXECUTABLE_CameraController := CameraController
TARGET_CameraController := $(TARGETDIR)/$(EXECUTABLE_CameraController)

#IMAGE PROCCESSOR
EXECUTABLE_ImageProcessor := ImageProcessor
TARGET_IMAGEPROC := $(TARGETDIR)/$(EXECUTABLE_ImageProcessor)

#GNC
EXECUTABLE_GNC := GNC
TARGET_GNC := $(TARGETDIR)/$(EXECUTABLE_GNC)

#SPACECRAFT
EXECUTABLE_SPACECRAFT := Spacecraft
TARGET_SPACECRAFT := $(TARGETDIR)/$(EXECUTABLE_SPACECRAFT)

# Main OBJS
MAINDIR := Main_Files
MAINOBJ_DIR := build_Main

# Code Lists
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Folder Lists
# Note: Intentionally excludes the root of the include folder so the lists are clean
INCDIRS := $(shell find include/**/* -name '*.h' -exec dirname {} \; | sort | uniq)
INCLIST := $(patsubst include/%,-I include/%,$(INCDIRS))
BUILDLIST := $(patsubst include/%,$(BUILDDIR)/%,$(INCDIRS))


# Shared Compiler Flags
CFLAGS := -c #-Wall -Wextra
INC := -I include $(INCLIST) -I /usr/local/include


# Platform Specific Compiler Flags
ifeq ($(UNAME_S),Linux)
    CFLAGS += -std=gnu++11 -O3 #-g -fPIC

    # PostgreSQL Special
    PG_VER := 9.3
    INC += -I /usr/pgsql-$(PG_VER)/include
    LIB += -L /usr/pgsql-$(PG_VER)/lib
else
  CFLAGS += -std=c++11 -stdlib=libc++ -O3 #-g
endif

#WATCHDOG
$(TARGET_WatchDog): $(OBJECTS) $(MAINOBJ_DIR)/mainWatchDog.o Build_Apps/WatchDog.o
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "	 Linking $(TARGET_WatchDog)\n"; $(CC) $^ -o $(TARGET_WatchDog)


#SCCOMMS
$(TARGET_ScComms): $(OBJECTS) $(MAINOBJ_DIR)/mainScComms.o Build_Apps/ScComms.o
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "	 Linking $(TARGET_ScComms)\n"; $(CC) $^ -o $(TARGET_ScComms)

#CAMERA CONTROLLER
$(TARGET_CameraController): $(OBJECTS) $(MAINOBJ_DIR)/mainCameraController.o Build_Apps/CameraController.o
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "	 Linking $(TARGET_CameraController)\n"; $(CC) $^ -o $(TARGET_CameraController) `pkg-config --cflags --libs opencv` -I /usr/local/include/ -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_ml

#IMAGE PROCCESSOR
$(TARGET_IMAGEPROC): $(OBJECTS) $(MAINOBJ_DIR)/mainImageProcessor.o Build_Apps/ImageProcessor.o
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "	 Linking $(TARGET_IMAGEPROC)\n"; $(CC) $^ -o $(TARGET_IMAGEPROC) -I./Image_Processing/analyzeImagePi/ ./lib/analyzeImage.lib  `pkg-config --cflags --libs opencv` -I /usr/local/include/ -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_ml

#GNC
$(TARGET_GNC): $(OBJECTS) $(MAINOBJ_DIR)/mainGNC.o Build_Apps/GNC.o
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "	 Linking $(TARGET_GNC)\n"; $(CC) $^ -o $(TARGET_GNC) -I ./GNC/Kalman_Filter_Iteration/ ./lib/Kalman_Filter_Iteration.lib

#SPACECRAFT
$(TARGET_SPACECRAFT): $(OBJECTS) $(MAINOBJ_DIR)/mainSpacecraft.o Build_Apps/Spacecraft.o
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "	 Linking $(TARGET_SPACECRAFT)\n"; $(CC) $^ -o $(TARGET_SPACECRAFT)


$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDLIST)
	@echo "Compiling $<..."; $(CC) $(CFLAGS) $(INC) -c -o $@ $<


# Need to have all dependencies mapped out for building 
$(MAINOBJ_DIR)/%.o: $(MAINDIR)/%.$(SRCEXT)
	@mkdir -p $(MAINOBJ_DIR)
	@echo "Compiling $<..."; $(CC) $(CFLAGS) $(INC) -I Apps_inc -c -o $@ $<


Build_Apps/%.o: Apps_src/%.$(SRCEXT)
	@mkdir -p Build_Apps
	@echo "Compiling $<..."; $(CC) $(CFLAGS) $(INC) -I Apps_inc -I ./Image_Processing/analyzeImagePi/ -I ./GNC/Kalman_Filter_Iteration/ -c -o $@ $<

clean:
	@echo "Cleaning .o Files…"; $(RM) -r $(BUILDDIR) $(TARGET); rm build_Main/*;

#install:
#	@echo "Installing $(EXECUTABLE)..."; cp $(TARGET) $(INSTALLBINDIR)
	
#distclean:
#	@echo "Removing $(EXECUTABLE)"; rm $(INSTALLBINDIR)/$(EXECUTABLE)

.PHONY: clean