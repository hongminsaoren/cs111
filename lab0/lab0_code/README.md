# A Kernel Seedling
TODO:  In this lab, we implement a kernel module to count the current process number.

## Building
```shell
TODO: make: compile the c file
			sudo insmod proc_count.ko: insert the compiled kernel module
```

## Running
```shell
TODO: cat /proc/count: access the count file to trigger the related show function proc_count, compute and output the process number
```
TODO: 136

## Cleaning Up
```shell
TODO: sudo rmmod proc_count: remove the kernel module
			make clean: clean the compiled c file and other stuff
```

## Testing
```python
python -m unittest
```
TODO: Ran 3 tests in 4.503s

OK.

Report which kernel release version you tested your module on
(hint: use `uname`, check for options with `man uname`).
It should match release numbers as seen on https://www.kernel.org/.

```shell
uname -r -s -v
```
TODO: Linux 5.14.8-arch1-1