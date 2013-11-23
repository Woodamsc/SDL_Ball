#you can use this as a template
CC=g++
headers=main.h
fIn=main.cpp
Depends=$(fIn) $(headers)
fOut=ball
cVer=-std=c++0x
Libs=-lSDL2 -lSDL2_image

ball.o : $(fIn)
	$(CC) -Wall $(fIn) -o $(fOut) $(Libs) -I /usr/local/include/SDL2 $(cVer)
