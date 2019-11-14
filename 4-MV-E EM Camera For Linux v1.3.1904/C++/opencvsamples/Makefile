##############################
# spcaview Makefile
##############################

INSTALLROOT=$(PWD)

HEADER_PATH = -I./../include

CC=gcc
CPP=g++

objects= Capture Sample_Cap_Callbackfun  SoftTrigger Bayer2rgb

LIB_PATH = -L./../lib

LIBS = -lMVGev

OPENCVLIB = `pkg-config --cflags --libs opencv`

TARGET : $(objects)
	
$(objects):% : %.cpp
	$(CPP)  $< -o $@ $(HEADER_PATH) $(LIB_PATH) $(LIBS) $(OPENCVLIB)


.PHONY : clean	
clean:
	@echo "Cleaning up directory."
	rm -rf $(objects)




