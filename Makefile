brainfuck: brainfuck.c
	gcc brainfuck.c rbtree.c -o brainfuck

run: brainfuck
	./brainfuck helloworld.bf

clean:
	-rm brainfuck
