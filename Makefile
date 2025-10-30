PROGRAM_NAME:= DunGen
CC:= gcc
CPPFLAGS:= -Iinclude
LDFLAGS:= -Llib
CFLAGS+= -std=c99 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter #-pedantic
LDLIBS:= -lglfw3 -lcglm -lGL -lfreetype -lz -lX11 -lpthread -lXrandr -lXi -ldl -lm
BUILD?= debug

ifeq ($(OS),Windows_NT)
	$(error Windows builds not supported. Please use WSL or Linux.)
endif

ifeq ($(BUILD),release)
	CFLAGS += -O2 -DNDEBUG
else ifeq ($(BUILD),debug)
	CFLAGS += -g -O0 -DDEBUG -DENABLE_GL_DEBUG
else
	$(error Invalid BUILD value. Use 'debug' or 'release')
endif

all: dungen

dungen: generate_bin 
	${CC} ${CPPFLAGS} ${LDFLAGS} ${CFLAGS} -o bin/${PROGRAM_NAME} \
	src/platform/main.c \
	src/platform/glad.c \
	src/platform/platform_debug.c \
	src/platform/platform_log.c \
	${LDLIBS}

generate_bin:
	#@mkdir -p bin
	@mkdir -p bin/shaders
	@mkdir -p bin/textures
	@mkdir -p bin/fonts
	@cp res/shaders/* bin/shaders/
	@cp res/textures/* bin/textures/
	@cp res/fonts/* bin/fonts/

clean:
	@rm -rf bin