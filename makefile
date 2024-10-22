CFLAGS=-std=c2x -Wall -Werror -Wextra -O2
SOURCES=$(shell find src -type f -iname '*.c')
TARGET=-o build/cboy
PKGS=-lm `pkg-config --cflags --libs sdl2`

all:
	@if [ ! -d "build" ]; then \
		echo "Build directory does not exist. Creating..." ; \
		mkdir build ; \
	fi

	gcc $(SOURCES) $(TARGET) $(CFLAGS) $(PKGS)

release:
	make clean
	tar -czvf cboy.tar.gz build/

clean:
	rm -rf build/
	make all