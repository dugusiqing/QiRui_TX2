

##############################
# spcaview Makefile
##############################

INSTALLROOT=$(PWD)

HEADER_PATH = -I./../include

CC=gcc
CPP=g++

objects= Capture GetPersistentIP Sample_Cap_Callbackfun SetPersistentIP SetTemp.IP SoftTrigger Bayer2rgb

LIB_PATH = -L./../lib/

LIBS = -lMVGev

TARGET : $(objects)
	
$(objects):% : %.cpp
	$(CPP)  $< -o $@ $(HEADER_PATH) $(LIB_PATH) $(LIBS)


.PHONY : clean	
clean:
	@echo "Cleaning up directory."
	rm -rf $(objects)




