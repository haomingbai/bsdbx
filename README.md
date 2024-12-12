# bsdbx
This is a simple sandbox focusing only on limiting some system calls of a program.
## Usage
bsdbx --path=$COMMAND_PATH --mode=$MODE $ARGS
### Example
```bash
./bsdbx --path=/bin/c++ --mode=compiler a.cpp -o a # Run with args
./bsdbx --path=/bin/a --mode=runner # Without extra args
./bsdbx --path=/bin/a # runner mode by default
```
## See also
[tanakh/cmdline](https://github.com/tanakh/cmdline)\
[boxjan/sandbox](https://github.com/boxjan/sandbox)