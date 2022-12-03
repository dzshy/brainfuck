# Brainfuck

A brainfuck interpreter

## Build

```
make
```

## Hello World

```
./brainfuck ./helloworld.bf
```

## Lisp!

```
wget https://raw.githubusercontent.com/shinh/bflisp/master/bflisp.bf
echo "(car (quote (a b c)))" | time ./brainfuck bflisp.bf
```

WARNING: It's very slow, may takes ~10 minutes to run a `(car (quote (a b c)))`.
