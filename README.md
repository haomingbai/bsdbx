# bsdbx
This is a simple sandbox focusing only on limiting some system calls of a program.
## Usage

```bash
bsdbx $COMMAND --mode=$MODE $ARGS
bsdbx $COMMAND -m $MODE $ARGS
bsdbx $COMMAND --mode $MODE $ARGS
bsdbx $COMMAND $ARGS # Default mode is runner
bsdbx $COMMAND --memory-limit=$MEMORY_LIMIT $ARGS
bsdbx $COMMAND --memory-limit $MEMORY_LIMIT $ARGS
bsdbx $COMMAND --time-limit=$TIME_LIMIT $ARGS
bsdbx $COMMAND --time-limit $TIME_LIMIT $ARGS
bsdbx $COMMAND -t $TIME_LIMIT $ARGS
```

There are two modes for this command, namely "runner" and "compiler". While runner mode is stricter than the compiler mode.

### Example

```bash
./bsdbx /bin/c++ --mode=compiler a.cpp -o a # Run with args
./bsdbx /bin/a --mode=runner # Without extra args
./bsdbx /bin/a # runner mode by default
./bsdbx /bin/a --memory-limit=10000 # With memory limit
./bsdbx /bin/a --time-limit=1000 # With time limit
```

## See also
[boxjan/sandbox](https://github.com/boxjan/sandbox)