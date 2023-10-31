exe:	main.cpp
	g++ *.cpp src/*.cpp src/*.c classes/*.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -Iclasses

asan:
	g++ *.cpp -g -fsanitize=address src/*.cpp src/*.c classes/*.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -Iclasses

gdb:
	g++ *.cpp -g src/*.cpp src/*.c classes/*.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -Iclasses

run:
	./a.out

r:
	./a.out

clean:
	rm a.out

c:
	rm a.out
