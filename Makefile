brainfuck: brainfuck.c
	gcc brainfuck.c -o brainfuck

run: brainfuck
	./brainfuck helloworld.bf
