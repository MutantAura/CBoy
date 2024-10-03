CFLAGS=-std=c2x -Wall -Werror -Wextra -O2

all:
	@if [ ! -d "build" ]; then \
		echo "Build directory does not exist. Creating..." ; \
		mkdir build ; \
	fi

	gcc src/main.c src/cpu/*.c src/gfx/*.c -o build/cboy -lm $(CFLAGS) `pkg-config --cflags --libs sdl2`

release:
	make clean
	tar -czvf cboy.tar.gz build/

clean:
	rm -rf build/
	make all