all:
	gcc -I src/include -L src/lib -o main main.c render.c control.c get_ai_move.c generate_moves.c perft.c eval.c -lmingw32 -lSDL2main -lSDL2

main.o: main.c global.h render.h control.h get_ai_move.h generate_moves.h perft.h
	gcc -I src/include -L src/lib -c main.c -lmingw32 -lSDL2main -lSDL2

render.o: render.c global.h render.h
	gcc -I src/include -L src/lib -c render.c -lmingw32 -lSDL2main -lSDL2

control.o: control.c global.h control.h generate_moves.h render.h
	gcc -I src/include -L src/lib -c control.c -lmingw32 -lSDL2main -lSDL2

get_ai_move.o: get_aimove.c global.h get_ai_move.h eval.h control.h
	gcc -I src/include -L src/lib -c get_ai_move.c -lmingw32 -lSDL2main -lSDL2

generate_moves.o: generate_moves.c global.h control.h generate_moves.h
	gcc -I src/include -L src/lib -c generate_moves.c -lmingw32 -lSDL2main -lSDL2

perft.o: perft.c global.h generate_moves.h control.h perft.h
	gcc -I src/include -L src/lib -c perft.c -lmingw32 -lSDL2main -lSDL2

eval.o: eval.c global.h generate_moves.h control.h eval.h
	gcc -I src/include -L src/lib -c eval.c -lmingw32 -lSDL2main -lSDL2