CC ?= g++-14
BIN_DIR = ./bin
CFLAGS = -I/opt/homebrew/include -Ilib
LDFLAGS = -L/opt/homebrew/lib/ -lglfw -lm -Wall -Wextra  -g -lc++ -std=c++26 # -O2

SRC = src/main.cpp src/engine/*.cpp src/anguis/*.cpp

run:
	mkdir -p bin
	gcc -c lib/glad/glad.c -o ${BIN_DIR}/glad.o ${CFLAGS} # Compile GLAD (C) separately
	${CC} ${BIN_DIR}/glad.o ${SRC} -o ${BIN_DIR}/main ${CFLAGS} ${LDFLAGS}
	${BIN_DIR}/main

clean:
	rm -rf bin