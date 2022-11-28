brainfuck:
	gcc brainfuck.c -o brainfuck

run: brainfuck
	./brainfuck test.brainfuck
