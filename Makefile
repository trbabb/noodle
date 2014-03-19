CC = g++
LD = g++
AR = ar

# compile
INCLUDES = /usr/local/boost src /Users/tbabb/code/gltoy/src
CFLAGS   = -O3 -Wall -c -fmessage-length=0 -Wno-unused -Wno-unused-local-typedefs
IFLAGS   = $(addprefix -I, $(INCLUDES))

# link
LIBDIRS  = /usr/local/boost/stage/lib \
           /System/Library/Frameworks/OpenGL.framework/Libraries \
	   /Users/tbabb/code/gltoy/lib
LIBS     = geomc GL png z SDL_mixer GLU boost_system gltoy
LDFLAGS  = $(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBDIRS)) \
           -framework GLUT -framework OpenGL \
           `/opt/local/bin/sdl-config --libs --cflags` \

# sources
MODULES  = noodle
SRC      = $(wildcard src/*.cpp) \
           $(foreach m, $(MODULES), $(wildcard src/$(m)/*.cpp))
OBJ      = $(patsubst src/%.cpp, build/%.o, $(SRC))


all: colorspace noodle circle

clean:
	rm -rf ./build/*
	rm -rf ./bin/*

## binaries

colorspace: $(OBJ)
	$(CC) $(LDFLAGS) build/colorspace.o -o bin/colorspace

circle: $(OBJ)
	$(CC) $(LDFLAGS) build/circle.o -o bin/circle

noodle: $(OBJ)
	$(CC) $(LDFLAGS) build/noodle.o -o bin/noodle

build/%.o : src/%.cpp
	mkdir -p $(patsubst src/%, build/%, $(dir $<))
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $<

