## UID: 123456789

## Pipe Up

Simulate pipe operator in C program.

## Building

For n executable, build n-1 pipes for communication between processes. Every time we create a pipe, we fork the process and redirect the child process's stdout to whe write end, execute the executable, then redirect the parent process's stdin to the read end, then begin the next round.

## Running

Run `./pipe ls wc wc` we get the result `       1       3      25`. That's because we first use `ls` to list all the files `Makefile        README.md       __pycache__     pipe            pipe.c          pipe.dSYM       test_lab1.py`. Then treat the output as the input to `wc`, get `       7       7      66`. The treat it as the input to the second `wc`, get the final result ``       1       3      25``.

## Cleaning up

Use the `make clean` command. In `Makefile`, we have 

```
clean:
	rm -f ${OBJS} pipe
```

So when we run `make clean`  it will remove `pipe.o`.
