main: function_plotter.o
	gcc function_plotter.c lib/tinyexpr.c \
	-Wall -Wextra -g -o programa \
	-lm `pkg-config --cflags --libs sdl2 SDL2_ttf`


clean: 
	rm function_plotter.o 
	rm programa
