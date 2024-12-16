# bsdbx
This is a simple sandbox focusing only on limiting some system calls of a program.
## Usage
```bash
bsdbx $COMMAND --mode=$MODE $ARGS
bsdbx $COMMAND -m $MODE $ARGS
bsdbx $COMMAND --mode $MODE $ARGS
bsdbx $COMMAND $ARGS # Default mode is runner
```
There are two modes for this command, namely "runner" and "compiler". While runner mode is stricter than the compiler mode.
### Example
```bash
./bsdbx /bin/c++ --mode=compiler a.cpp -o a # Run with args
./bsdbx /bin/a --mode=runner # Without extra args
./bsdbx /bin/a # runner mode by default
```
## See also
[tanakh/cmdline](https://github.com/tanakh/cmdline)\
[boxjan/sandbox](https://github.com/boxjan/sandbox)