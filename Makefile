rust-browser-game-but-sdl: main.o game.o
	cc -Wall -Wextra `pkg-config --cflags sdl2` -o rust-browser-game-but-sdl main.o game.o `pkg-config --libs sdl2`

main.o: main.c game.h
	cc -Wall -Wextra -std=c11 -c main.c

game.o: game.rs
	rustc -C opt-level=s -C panic=abort game.rs --emit=obj
