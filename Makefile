PROGRAM_NAME:= mini3D 
CC:= gcc
CPPFLAGS:= -Iinclude
LDFLAGS:= -Llib
CFLAGS+= -std=c99 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter #-pedantic
LDLIBS:= -lglfw3 -lcglm -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
BUILD?= debug

ifeq ($(OS),Windows_NT)
	$(error Windows builds not supported. Please use WSL or Linux.)
endif

ifeq ($(BUILD),release)
	CFLAGS += -O2 -DNDEBUG
else ifeq ($(BUILD),debug)
	CFLAGS += -g -O0 -DDEBUG -DENABLE_GL_DEBUG_OUTPUT
else
	$(error Invalid BUILD value. Use 'debug' or 'release')
endif

all: generate_bin mini3d

mini3d:
	${CC} ${CPPFLAGS} ${LDFLAGS} ${CFLAGS} -o bin/${PROGRAM_NAME} src/mini3d.c src/gl_helpers.c src/glad.c ${LDLIBS}

generate_bin:
	@mkdir -p bin

clean:
	@rm -rf bin