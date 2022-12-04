brainfuck: brainfuck.c htable.c crc32.c
	gcc -O3 $^ -o $@

run: brainfuck
	./brainfuck helloworld.bf

clean:
	-rm brainfuck
